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
#include <asm/arch/s3c2412.h>

#define MPLL 0
#define UPLL 1

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
	struct s3c2412_sysctl *const sysctl = s3c2412_get_base_sysctl();
	u32 pllcon, f;
	u16 mdiv, pdiv, sdiv;

	if (pllreg == MPLL)
		pllcon = readl(&sysctl->mpllcon);
	else if (pllreg == UPLL)
		pllcon = readl(&sysctl->upllcon);
	else
		hang();

	mdiv = PLLCON_MDIV_GET(pllcon);
	pdiv = PLLCON_PDIV_GET(pllcon);
	sdiv = PLLCON_SDIV_GET(pllcon);

	f = (mdiv + 8) * (CONFIG_SYS_CLK_FREQ / ((pdiv + 2) << sdiv));
	if (pllreg == MPLL)
		f <<= 1;

	return f;
}

/* return FCLK frequency */
ulong get_FCLK(void)
{
	return get_PLLCLK(MPLL);
}

/* return HCLK frequency */
ulong get_HCLK(void)
{
	struct s3c2412_sysctl *const sysctl = s3c2412_get_base_sysctl();
	u32 clkdivn;
	u16 hclk_div, arm_div;

	clkdivn = readl(&sysctl->clkdivn);
	hclk_div = CLKDIVN_HCLKDIV_GET(clkdivn) + 1;
	arm_div = CLKDIVN_ARMDIV_GET(clkdivn) + 1;

	return get_FCLK() / (hclk_div * arm_div);
}

/* return PCLK frequency */
ulong get_PCLK(void)
{
	struct s3c2412_sysctl *const sysctl = s3c2412_get_base_sysctl();
	u32 clkdivn;

	clkdivn = readl(&sysctl->clkdivn);

	return (clkdivn & CLKDIVN_PCLKDIV) ? get_HCLK() / 2 : get_HCLK();
}

/* return UCLK frequency */
ulong get_UCLK(void)
{
	return get_PLLCLK(UPLL);
}

/* return ARMCORE frequency */
ulong get_ARMCLK(void)
{
	struct s3c2412_sysctl *const sysctl = s3c2412_get_base_sysctl();
	u32 clkdivn;

	clkdivn = readl(&sysctl->clkdivn);
	if (clkdivn & CLKDIVN_DVS)
		return get_FCLK();

	return (clkdivn & CLKDIVN_ARMDIV) ? get_FCLK() / 2 : get_FCLK();
}
