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
#include <spl.h>
#include <version.h>
#include <nand.h>
#include <asm/io.h>
#include <asm/arch/s3c24xx_cpu.h>

/* FCLK = 800 MHz, HCLK = 133 MHz, PCLK = 66 MHz */
#define M_MDIV	400
#define M_PDIV	3
#define M_SDIV	1
#define ARMDIV	1
#define PREDIV	2
#define HCLKDIV	1
#define M_LTIME	0x0E10

/* EPLLclk = 96MHz */
#define E_MDIV	32
#define E_PDIV	1
#define E_SDIV	2
#define E_LTIME	0x1780

DECLARE_GLOBAL_DATA_PTR;

static inline void asm_delay(ulong loops)
{
	asm volatile ("1:\n" "subs %0, %1, #1\n"
		      "bne 1b" : "=r" (loops) : "0"(loops));
}

static inline void watchdog_disable(void)
{
	struct s3c24xx_watchdog *const watchdog = s3c24xx_get_base_watchdog();

	writel(WTCON_DISABLE_VAL, &watchdog->wtcon);
}

static void pinmux_init(void)
{
	struct s3c24xx_gpio *const gpio = s3c24xx_get_base_gpio();

	/* Init LED pin and turn LED off (LED on GPC5) */
	clrsetbits_le32(&gpio->gpccon, GPCCON_MASK(5), GPCCON_OUTPUT(5));
	setbits_le32(&gpio->gpcdat, GPCDAT(5));

	/* Init UART pins */
	clrsetbits_le32(&gpio->gphcon, GPHCON_MASK(0) | GPHCON_MASK(1) |
			GPHCON_MASK(2) | GPHCON_MASK(3) | GPHCON_MASK(4) |
			GPHCON_MASK(5) | GPHCON_MASK(6) | GPHCON_MASK(7),
			GPHCON_TXD(0) | GPHCON_RXD(0) | GPHCON_TXD(1) |
			GPHCON_RXD(1) | GPHCON_TXD(2) | GPHCON_RXD(2) |
			GPHCON_TXD(3) | GPHCON_RXD(3));

	/* Init NAND interface */
	setbits_le32(&gpio->gpacon, GPACON_CLE | GPACON_ALE | GPACON_NFWE |
		     GPACON_NFRE | GPACON_NRSTOUT | GPACON_NFCE);
}

static void pll_init(void)
{
	struct s3c2416_sysctl *const sysctl = s3c2416_get_base_sysctl();

	/* Configure clocks division ratio */
	clrsetbits_le32(&sysctl->clkdiv0,
			CLKDIV0_ARMDIV_MASK | CLKDIV0_PREDIV_MASK |
			CLKDIV0_HCLKDIV_MASK,
			CLKDIV0_ARMDIV(ARMDIV) | CLKDIV0_PREDIV(PREDIV) |
			CLKDIV0_HALFHCLK | CLKDIV0_PCLKDIV |
			CLKDIV0_HCLKDIV(HCLKDIV));

	/* Set MPLL lock time */
	writel(M_LTIME, &sysctl->lockcon0);

	/* Configure MPLL */
	writel(MPLLCON_MDIV(M_MDIV) | MPLLCON_PDIV(M_PDIV) |
	       PLLCON_SDIV(M_SDIV), &sysctl->mpllcon);

	/* Set EPLL lock time */
	writel(E_LTIME, &sysctl->lockcon1);

	/* Configure EPLL */
	writel(EPLLCON_MDIV(E_MDIV) | EPLLCON_PDIV(E_PDIV) |
	       PLLCON_SDIV(E_SDIV), &sysctl->epllcon);

	/* MSYSCLK = MPLL and ESYSCLK = EPLL */
	setbits_le32(&sysctl->clksrc,
		     CLKSRC_MSYSCLK_MPLL | CLKSRC_ESYSCLK_EPLL);
}

static void dramctl_init(void)
{
	struct s3c24xx_dramctl *const dramctl = s3c24xx_get_base_dramctl();

	/* Step 1: Init BANKCFG & BANKCON1 */
	writel(BANKCFG_VAL_DDR2, &dramctl->bankcfg);
	writel(BANKCON1_VAL_DDR2, &dramctl->bankcon1);

	/* Step 2: Init BANKCON2 */
	writel(BANKCON2_VAL_DDR2, &dramctl->bankcon2);

	/* Step 3: Issue a PALL command */
	writel(BANKCON1_VAL_DDR2 | BANKCON1_INIT_PALL, &dramctl->bankcon1);

	/* Step 4: Issue a EMRS2 command */
	writel(BANKCON3_VAL_EMRS2, &dramctl->bankcon3);
	writel(BANKCON1_VAL_DDR2 | BANKCON1_INIT_EMRS, &dramctl->bankcon1);

	/* Step 5: Issue a EMRS3 command */
	writel(BANKCON3_VAL_EMRS3, &dramctl->bankcon3);
	writel(BANKCON1_VAL_DDR2 | BANKCON1_INIT_EMRS, &dramctl->bankcon1);

	/* Step 6: Issue a EMRS1 command */
	writel(BANKCON3_VAL_EMRS1, &dramctl->bankcon3);
	writel(BANKCON1_VAL_DDR2 | BANKCON1_INIT_EMRS, &dramctl->bankcon1);

	/* Step 7: Issue a MRS command */
	writel(BANKCON3_VAL_MRS | BANKCON3_MRS_DLL_RESET, &dramctl->bankcon3);
	writel(BANKCON1_VAL_DDR2 | BANKCON1_INIT_MRS, &dramctl->bankcon1);

	/* Step 8: Issue a PALL command */
	writel(BANKCON1_VAL_DDR2 | BANKCON1_INIT_PALL, &dramctl->bankcon1);

	/* Step 9: Write 0xFF into the refresh timer */
	writel(0xFF, &dramctl->refresh);

	/* Step 10: Wait more than 120 clocks */
	asm_delay(256);

	/* Step 11: Issue a MRS command */
	writel(BANKCON3_VAL_MRS, &dramctl->bankcon3);
	writel(BANKCON1_VAL_DDR2 | BANKCON1_INIT_MRS, &dramctl->bankcon1);

	/* Step 12: Issue a EMRS1 command */
	writel(BANKCON3_VAL_EMRS1 | BANKCON3_EMRS1_OCD7 | BANKCON3_EMRS1_CAS3,
	       &dramctl->bankcon3);
	writel(BANKCON1_VAL_DDR2 | BANKCON1_INIT_EMRS, &dramctl->bankcon1);

	writel(BANKCON3_VAL_EMRS1 | BANKCON3_EMRS1_CAS3, &dramctl->bankcon3);
	writel(BANKCON1_VAL_DDR2 | BANKCON1_INIT_EMRS, &dramctl->bankcon1);

	/* Step 13: Write 0x87 into the refresh timer */
	writel(0x87, &dramctl->refresh);

	/* Step 14: Normal Mode */
	writel(BANKCON1_VAL_DDR2 | BANKCON1_INIT_NORMAL, &dramctl->bankcon1);
}

#ifndef CONFIG_SPL_LIBCOMMON_SUPPORT
#ifdef CONFIG_SPL_SERIAL_SUPPORT
void puts(const char *str)
{
	serial_puts(str);
}
#else
void puts(const char *str)
{
}
#endif
#endif

void spl_board_init(void)
{
	watchdog_disable();
	pinmux_init();
	pll_init();
	preloader_console_init();
	dramctl_init();
}

u32 spl_boot_device(void)
{
#ifdef CONFIG_SPL_NAND_SIMPLE
	puts("Loading U-Boot from NAND Flash...\n");
	return BOOT_DEVICE_NAND;
#else
	puts("Unknown boot device\n");
	hang();
#endif
}
