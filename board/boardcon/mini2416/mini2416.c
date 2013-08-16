/*
 * (C) Copyright 2012 INOV - INESC Inovacao
 * Jose Goncalves <jose.goncalves@inov.pt>
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
#include <netdev.h>
#include <asm/io.h>
#include <asm/arch/s3c24xx_cpu.h>

#define SMBIDCY1_VAL	0	/* Idle Cycle */
#define SMBWSTWR1_VAL	14	/* Write Wait State */
#define SMBWSTOEN1_VAL	2	/* Output Enable Assertion Delay */
#define SMBWSTWEN1_VAL	2	/* Write Enable Assertion Delay */
#define SMBWSTRD1_VAL	14	/* Read Wait State */

DECLARE_GLOBAL_DATA_PTR;

static void ether_if_init(void)
{
	/* Ethernet chip is on memory bank 1 */
	struct s3c24xx_smc *const smc = s3c24xx_get_base_smc(S3C24XX_SMC1);
	struct s3c24xx_gpio *const gpio = s3c24xx_get_base_gpio();

	/* Set bus timings */
	writel(SMBIDCY1_VAL, &smc->smbidcy);
	writel(SMBWSTWR1_VAL, &smc->smbwstwr);
	writel(SMBWSTOEN1_VAL, &smc->smbwstoen);
	writel(SMBWSTWEN1_VAL, &smc->smbwstwen);
	writel(SMBWSTRD1_VAL, &smc->smbwstrd);

	/* Init SMC control register */
	clrsetbits_le32(&smc->smbc,
			(SMBC_RSMAVD_WR | SMBC_RSMAVD_RD | SMBC_MW_MASK |
			 SMBC_WAIT_POL),
			(SMBC_DRNOWE | SMBC_DRNCS | SMBC_MW_16BIT |
			 SMBC_WAIT_EN | SMBC_RBLE));

	/* Enable CS pin */
	setbits_le32(&gpio->gpacon, GPACON_NRCS1);
}

int board_init(void)
{
	struct s3c24xx_gpio *const gpio = s3c24xx_get_base_gpio();

	/* Turn LED on */
	clrbits_le32(&gpio->gpcdat, GPCDAT(5));

	/* Init interface with Ethernet chip */
	ether_if_init();

	/* Arch number of MINI2416 Board */
	gd->bd->bi_arch_number = MACH_TYPE_MINI2416;

	/* Address of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

	return 0;
}

int dram_init(void)
{
	gd->ram_size = get_ram_size((void *)CONFIG_SYS_SDRAM_BASE,
				    CONFIG_SYS_SDRAM_SIZE);
	return 0;
}

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	printf("\nBoard: MINI2416\n");
	return 0;
}
#endif

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_SMC911X
	rc = smc911x_initialize(0, CONFIG_SMC911X_BASE);
#endif
	return rc;
}
#endif
