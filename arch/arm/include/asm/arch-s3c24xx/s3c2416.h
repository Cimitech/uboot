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

/************************************************
 * NAME	    : s3c2416.h
 *
 * Based on S3C2416 User's manual Rev 1.40
 ************************************************/

#ifndef __S3C2416_H__
#define __S3C2416_H__

/* Base addresses for S3C2416 specific modules */
#define S3C2416_USB_DEVICE_BASE		0x49800000
#define S3C2416_INTERRUPT_BASE		0x4A000000
#define S3C2416_HSMMC_BASE		0x4A800000
#define S3C2416_SYSCTL_BASE		0x4C000000
#define S3C2416_LCD_BASE		0x4C800000
#define S3C2416_HSSPI_BASE		0x52000000
#define S3C2416_ADC_BASE		0x58000000

#define S3C24XX_UART_CHANNELS	4
#define S3C24XX_DMA_CHANNELS	6
#define S3C24XX_SMC_BANKS	6

#ifdef CONFIG_S3C2416
#define S3C24XX_CPU_NAME	"S3C2416"
#else
#define S3C24XX_CPU_NAME	"S3C2450"
#endif

enum s3c24xx_uarts {
	S3C24XX_UART0,
	S3C24XX_UART1,
	S3C24XX_UART2,
	S3C24XX_UART3
};

enum s3c24xx_dmas {
	S3C24XX_DMA0,
	S3C24XX_DMA1,
	S3C24XX_DMA2,
	S3C24XX_DMA3,
	S3C24XX_DMA4,
	S3C24XX_DMA5
};

enum s3c24xx_smcs {
	S3C24XX_SMC0,
	S3C24XX_SMC1,
	S3C24XX_SMC2,
	S3C24XX_SMC3,
	S3C24XX_SMC4,
	S3C24XX_SMC5
};

/* Interrupt Controller */
struct s3c2416_interrupt {
	u32 srcpnd1;
	u32 intmod1;
	u32 intmsk1;
	u32 _res1;
	u32 intpnd1;
	u32 intoffset1;
	u32 subsrcpnd;
	u32 intsubmsk;
	u32 _res2[4];
	u32 priority_mode1;
	u32 priority_update1;
	u32 _res3[2];
	u32 srcpnd2;
	u32 intmod2;
	u32 intmsk2;
	u32 _res4;
	u32 intpnd2;
	u32 intoffset2;
	u32 _res5[6];
	u32 priority_mode2;
	u32 priority_update2;
};

/* System Controller */
struct s3c2416_sysctl {
	u32 lockcon0;
	u32 lockcon1;
	u32 oscset;
	u32 _res1;
	u32 mpllcon;
	u32 _res2;
	u32 epllcon;
	u32 epllcon_k;
	u32 clksrc;
	u32 clkdiv0;
	u32 clkdiv1;
	u32 clkdiv2;
	u32 hclkcon;
	u32 pclkcon;
	u32 sclkcon;
	u32 _res3;
	u32 pwrmode;
	u32 swrst;
	u32 _res4[2];
	u32 busprio;
	u32 _res5[3];
	u32 pwrcfg;
	u32 rstcon;
	u32 rststat;
	u32 wkupstat;
	u32 inform[4];
	u32 uphyctrl;
	u32 uphypwr;
	u32 urstcon;
	u32 uclkcon;
};

#define MPLLCON_MDIV_MASK	(0x3FF<<14)
#define MPLLCON_MDIV(x)		((x)<<14)
#define MPLLCON_MDIV_GET(x)	(((x)>>14) & 0x3FF)
#define MPLLCON_PDIV_MASK	(0x3F<<5)
#define MPLLCON_PDIV(x)		((x)<<5)
#define MPLLCON_PDIV_GET(x)	(((x)>>5) & 0x3F)

#define EPLLCON_MDIV_MASK	(0xFF<<16)
#define EPLLCON_MDIV(x)		((x)<<16)
#define EPLLCON_MDIV_GET(x)	(((x)>>16) & 0xFF)
#define EPLLCON_PDIV_MASK	(0x3F<<8)
#define EPLLCON_PDIV(x)		((x)<<8)
#define EPLLCON_PDIV_GET(x)	(((x)>>8) & 0x3F)

#define PLLCON_SDIV_MASK	(0x7<<0)
#define PLLCON_SDIV(x)		((x)<<0)
#define PLLCON_SDIV_GET(x)	(((x)>>0) & 0x7)

#define CLKSRC_MSYSCLK_MPLL	(1<<4)
#define CLKSRC_ESYSCLK_EPLL	(1<<6)

#define CLKDIV0_HCLKDIV_MASK	(0x3<<0)
#define CLKDIV0_HCLKDIV(x)	((x)<<0)
#define CLKDIV0_HCLKDIV_GET(x)	(((x)>>0) & 0x3)
#define CLKDIV0_PCLKDIV		(1<<2)
#define CLKDIV0_HALFHCLK	(1<<3)
#define CLKDIV0_PREDIV_MASK	(3<<4)
#define CLKDIV0_PREDIV(x)	((x)<<4)
#define CLKDIV0_PREDIV_GET(x)	(((x)>>4) & 0x3)
#define CLKDIV0_ARMDIV_MASK	(7<<9)
#define CLKDIV0_ARMDIV(x)	((x)<<9)
#define CLKDIV0_ARMDIV_GET(x)	(((x)>>9) & 0x7)
#define CLKDIV0_DVS		(1<<13)

static inline struct s3c2416_interrupt *s3c2416_get_base_interrupt(void)
{
	return (struct s3c2416_interrupt *)S3C2416_INTERRUPT_BASE;
}

static inline struct s3c2416_sysctl *s3c2416_get_base_sysctl(void)
{
	return (struct s3c2416_sysctl *)S3C2416_SYSCTL_BASE;
}

/* Include common stuff */
#include <asm/arch/s3c24xx.h>

#endif /*__S3C2416_H__*/
