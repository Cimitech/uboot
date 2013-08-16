/*
 * (C) Copyright 2012 INOV - INESC Inovacao
 * Jose Goncalves <jose.goncalves@inov.pt>
 *
 * Based on drivers/mtd/nand/s3c64xx.c and U-Boot 1.3.4 from Samsung.
 * Supports only SLC NAND Flash chips.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <nand.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <asm/arch/s3c24xx_cpu.h>

#define TACLS_VAL	7	/* CLE & ALE duration setting (0~7) */
#define TWRPH0_VAL	7	/* TWRPH0 duration setting (0~7) */
#define TWRPH1_VAL	7	/* TWRPH1 duration setting (0~7) */

#define MAX_CHIPS	2
#ifdef CONFIG_SPL_BUILD

/* in the early stage of NAND flash booting, printf() is not available */
#define printf(fmt, args...)
void write_hex (unsigned char i)
{
    char cc; 

    cc = i >> 4;
    cc &= 0xf;
    if (cc > 9)
        serial_putc (cc + 55);
    else
        serial_putc (cc + 48);
    cc = i & 0xf;
    if (cc > 9)
        serial_putc (cc + 55);
    else
        serial_putc (cc + 48);
}

void nand_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
	int i;
	struct nand_chip *this = mtd->priv;
   // serial_puts("nand_read_buf\n");
	for (i = 0; i < len; i++)
        
		buf[i] = readb(this->IO_ADDR_R);
        //write_hex((unsigned char) buf[i]);
}
#endif

/* Struct to store NAND chip private data */
struct s3c_chip_info {
	const int cs;
};

static void s3c_nand_select_chip(struct mtd_info *mtd, int chip)
{
	struct s3c24xx_nand *const nand = s3c24xx_get_base_nand();
	u32 nfcont;

	nfcont = readl(&nand->nfcont);

	switch (chip) {
	case -1:
		nfcont |= NFCONT_NCE1 | NFCONT_NCE0;
		break;
	case 0:
		nfcont &= ~NFCONT_NCE0;
		break;
	case 1:
		nfcont &= ~NFCONT_NCE1;
		break;
	default:
		printf("S3C24XX NAND: Invalid chip select (%d).\n", chip);
		return;
	}

	writel(nfcont, &nand->nfcont);
}

/*
 * Hardware specific access to control-lines function
 */
static void s3c_nand_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct s3c24xx_nand *const nand = s3c24xx_get_base_nand();
	struct nand_chip *this;
	struct s3c_chip_info *info;

	if (ctrl & NAND_CTRL_CHANGE) {
		if (ctrl & NAND_NCE) {
			this = mtd->priv;
			info = this->priv;
			s3c_nand_select_chip(mtd, info->cs);
		} else {
			s3c_nand_select_chip(mtd, -1);
		}
	}

	if (cmd == NAND_CMD_NONE)
		return;

	if (ctrl & NAND_CLE)
		writeb(cmd, &nand->nfcmmd);
	else
		writeb(cmd, &nand->nfaddr);
}

/*
 * Function for checking device ready pin
 */
static int s3c_nand_device_ready(struct mtd_info *mtdinfo)
{
	struct s3c24xx_nand *const nand = s3c24xx_get_base_nand();

	return readl(&nand->nfstat) & NFSTAT_RNB;
}

#ifdef CONFIG_S3C24XX_NAND_HWECC
/*
 * This function is called before encoding ECC codes to ready ECC engine.
 */
static void s3c_nand_enable_hwecc(struct mtd_info *mtd, int mode)
{
	struct s3c24xx_nand *const nand = s3c24xx_get_base_nand();

	/* Set 1-bit ECC */
	clrsetbits_le32(&nand->nfconf, NFCONF_ECCTYPE_MASK,
			NFCONF_ECCTYPE_1BIT);

	/* Initialize & unlock ECC */
	clrsetbits_le32(&nand->nfcont, NFCONT_MECCLOCK, NFCONT_INITMECC);
}

/*
 * This function is called immediately after encoding ECC codes.
 * This function returns encoded ECC codes.
 */
static int s3c_nand_calculate_ecc(struct mtd_info *mtd, const uint8_t *dat,
				  uint8_t *ecc_code)
{
	struct s3c24xx_nand *const nand = s3c24xx_get_base_nand();
	u32 nfmecc0;

	/* Lock ECC */
	setbits_le32(&nand->nfcont, NFCONT_MECCLOCK);

	/* Return 1-bit ECC */
	nfmecc0 = readl(&nand->nfmecc[0]);

	ecc_code[0] = nfmecc0 & 0xff;
	ecc_code[1] = (nfmecc0 >> 8) & 0xff;
	ecc_code[2] = (nfmecc0 >> 16) & 0xff;
	ecc_code[3] = (nfmecc0 >> 24) & 0xff;

	return 0;
}

/*
 * This function determines whether read data is good or not.
 * On SLC, must write ECC codes to controller before reading status bit.
 * If status bit is good, return 0.
 * If a correctable error occured, correct it and return 1.
 * If an uncorrectable error occured, return -1.
 */
static int s3c_nand_correct_data(struct mtd_info *mtd, uint8_t *dat,
				 uint8_t *read_ecc, uint8_t *calc_ecc)
{
	struct s3c24xx_nand *const nand = s3c24xx_get_base_nand();
	int ret;
	u32 nfeccerr0, nfmeccdata0, nfmeccdata1, err_byte_addr;
	uint8_t err_type, repaired;

	/* SLC: Write ECC to compare */
	nfmeccdata0 = (((u32) read_ecc[1]) << 16) | read_ecc[0];
	nfmeccdata1 = (((u32) read_ecc[3]) << 16) | read_ecc[2];
	writel(nfmeccdata0, &nand->nfmeccd[0]);
	writel(nfmeccdata1, &nand->nfmeccd[1]);

	/* Read ECC status */
	nfeccerr0 = readl(&nand->nfeccerr[0]);
	err_type = nfeccerr0 & 0x3;

	switch (err_type) {
	case 0:		/* No error */
		ret = 0;
		break;

	case 1:
		/*
		 * 1 bit error (Correctable)
		 * (nfeccerr0 >> 7) & 0x7ff     :error byte number
		 * (nfeccerr0 >> 4) & 0x7       :error bit number
		 */
		err_byte_addr = (nfeccerr0 >> 7) & 0x7ff;
		repaired = dat[err_byte_addr] ^ (1 << ((nfeccerr0 >> 4) & 0x7));

		printf("S3C24XX NAND: 1 bit error detected at byte %u. "
		       "Correcting from 0x%02x to 0x%02x\n",
		       err_byte_addr, dat[err_byte_addr], repaired);

		dat[err_byte_addr] = repaired;

		ret = 1;
		break;

	case 2:		/* Multiple error */
	case 3:		/* ECC area error */
		puts("S3C24XX NAND: ECC uncorrectable error detected.\n");
		ret = -1;
		break;
	}

	return ret;
}
#endif /* CONFIG_S3C24XX_NAND_HWECC */

/*
 * Board-specific NAND initialization.
 */
int board_nand_init(struct nand_chip *nand)

{
	static struct s3c_chip_info chip_info[MAX_CHIPS] = { {0}, {1} };
	static int chip_n;
	struct s3c24xx_nand *const nand_reg = s3c24xx_get_base_nand();

	if (chip_n == 0) {
		/* Extend NAND timings to the maximum */
		clrsetbits_le32(&nand_reg->nfconf,
				NFCONF_TACLS_MASK | NFCONF_TWRPH0_MASK |
				NFCONF_TWRPH1_MASK,
				NFCONF_TACLS(TACLS_VAL) |
				NFCONF_TWRPH0(TWRPH0_VAL) |
				NFCONF_TWRPH1(TWRPH1_VAL));

		/* Disable chip selects and soft lock, enable controller */
		clrsetbits_le32(&nand_reg->nfcont, NFCONT_WP,
				NFCONT_NCE1 | NFCONT_NCE0 | NFCONT_ENABLE);
	} else if (chip_n >= MAX_CHIPS) {
		return -ENODEV;
	}

	nand->IO_ADDR_R = &nand_reg->nfdata;
	nand->IO_ADDR_W = &nand_reg->nfdata;
	nand->cmd_ctrl = s3c_nand_hwcontrol;
	nand->dev_ready = s3c_nand_device_ready;
	nand->select_chip = s3c_nand_select_chip;
	nand->options = 0;
#ifdef CONFIG_SPL_BUILD
	nand->read_buf = nand_read_buf;
#endif

#ifdef CONFIG_S3C24XX_NAND_HWECC
	nand->ecc.hwctl = s3c_nand_enable_hwecc;
	nand->ecc.calculate = s3c_nand_calculate_ecc;
	nand->ecc.correct = s3c_nand_correct_data;
	nand->ecc.mode = NAND_ECC_HW_OOB_FIRST;
	nand->ecc.size = CONFIG_SYS_NAND_ECCSIZE;
	nand->ecc.bytes = CONFIG_SYS_NAND_ECCBYTES;
    /// FIXME NOT SURE WHAT IS THE CORRECT VALUE HERE
	//nand->ecc.strength = 1;
#else
	nand->ecc.mode = NAND_ECC_SOFT;
#endif /* ! CONFIG_S3C24XX_NAND_HWECC */

	nand->priv = &chip_info[chip_n++];

	return 0;
}
