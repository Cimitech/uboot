/*
 * (C) Copyright 2012 INOV - INESC Inovacao
 * Jose Goncalves <jose.goncalves@inov.pt>
 *
 * Based on U-Boot 1.3.4 from Samsung.
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
#include <asm/io.h>
#include <asm/arch/s3c2416.h>

#define MPLL 0
#define EPLL 1

/* ------------------------------------------------------------------------- */
/*
 * CONFIG_SYS_CLK_FREQ should be defined as the input frequency of the PLL.
 *
 * get_FCLK(), get_HCLK(), get_PCLK() and get_UCLK() return the clock of
 * the specified bus in HZ.
 */
/* ------------------------------------------------------------------------- */

static ulong get_PLLCLK(int pllreg)
{
	struct s3c2416_sysctl *const sysctl = s3c2416_get_base_sysctl();
	u32 pllcon;
	u16 mdiv, pdiv, sdiv;

	if (pllreg == MPLL) {
		pllcon = readl(&sysctl->mpllcon);
		mdiv = MPLLCON_MDIV_GET(pllcon);
		pdiv = MPLLCON_PDIV_GET(pllcon);
	} else if (pllreg == EPLL) {
		pllcon = readl(&sysctl->epllcon);
		mdiv = EPLLCON_MDIV_GET(pllcon);
		pdiv = EPLLCON_PDIV_GET(pllcon);
	} else {
		hang();
	}

	sdiv = PLLCON_SDIV_GET(pllcon);

	return mdiv * (CONFIG_SYS_CLK_FREQ / (pdiv << sdiv));
}

/* return FCLK frequency */
ulong get_FCLK(void)
{
	return get_PLLCLK(MPLL);
}

/* return HCLK frequency */
ulong get_HCLK(void)
{
	struct s3c2416_sysctl *const sysctl = s3c2416_get_base_sysctl();
	u32 clkdiv0;
	u16 hclk_div, pre_div;

	clkdiv0 = readl(&sysctl->clkdiv0);
	hclk_div = (CLKDIV0_HCLKDIV_GET(clkdiv0)) + 1;
	pre_div = (CLKDIV0_PREDIV_GET(clkdiv0)) + 1;

	return get_FCLK() / (hclk_div * pre_div);
}

/* return PCLK frequency */
ulong get_PCLK(void)
{
	struct s3c2416_sysctl *const sysctl = s3c2416_get_base_sysctl();
	u32 clkdiv0;

	clkdiv0 = readl(&sysctl->clkdiv0);

	return (clkdiv0 & CLKDIV0_PCLKDIV) ? get_HCLK() / 2 : get_HCLK();
}

/* return UCLK frequency */
ulong get_UCLK(void)
{
	return get_PLLCLK(EPLL);
}

/* return ARMCORE frequency */
ulong get_ARMCLK(void)
{
	struct s3c2416_sysctl *const sysctl = s3c2416_get_base_sysctl();
	u32 clkdiv0;
	u16 arm_div;

	clkdiv0 = readl(&sysctl->clkdiv0);
	if (clkdiv0 & CLKDIV0_DVS)
		return get_FCLK();

	arm_div = CLKDIV0_ARMDIV_GET(clkdiv0) + 1;

	return get_FCLK() / arm_div;
}
