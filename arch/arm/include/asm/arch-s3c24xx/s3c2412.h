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
 * NAME	    : s3c2412.h
 *
 * Based on S3C2412 User's manual Rev 1.06a
 ************************************************/

#ifndef __S3C2412_H__
#define __S3C2412_H__

/* Base addresses for S3C2412 specific modules */
#define S3C2412_INTERRUPT_BASE		0x4A000000
#define S3C2412_SYSCTL_BASE		0x4C000000
#define S3C2412_LCD_BASE		0x4D000000
#define S3C2412_USB_DEVICE_BASE		0x52000000
#define S3C2412_ADC_BASE		0x58000000
#define S3C2412_SPI_BASE		0x59000000
#define S3C2412_SDIO_BASE		0x5A000000

#define S3C24XX_UART_CHANNELS	3
#define S3C24XX_DMA_CHANNELS	4
#define S3C24XX_SMC_BANKS	8

#ifdef CONFIG_S3C2412
#define S3C24XX_CPU_NAME	"S3C2412"
#else
#define S3C24XX_CPU_NAME	"S3C2413"
#endif

enum s3c24xx_uarts {
	S3C24XX_UART0,
	S3C24XX_UART1,
	S3C24XX_UART2
};

enum s3c24xx_dmas {
	S3C24XX_DMA0,
	S3C24XX_DMA1,
	S3C24XX_DMA2,
	S3C24XX_DMA3
};

enum s3c24xx_smcs {
	S3C24XX_SMC0,
	S3C24XX_SMC1,
	S3C24XX_SMC2,
	S3C24XX_SMC3,
	S3C24XX_SMC4,
	S3C24XX_SMC5,
	S3C24XX_SMC6,
	S3C24XX_SMC7
};

/* Interrupt Controller */
struct s3c2412_interrupt {
	u32 srcpnd;
	u32 intmod;
	u32 intmsk;
	u32 priority;
	u32 intpnd;
	u32 intoffset;
	u32 subsrcpnd;
	u32 intsubmsk;
};

/* System Controller */
struct s3c2412_sysctl {
	u32 locktime;
	u32 mpllcon;
	u32 upllcon;
	u32 clkcon;
	u32 _res1;
	u32 clkdivn;
	u32 oscset;
	u32 clksrc;
	u32 _res2;
	u32 pwrcfg;
	u32 _res3[2];
	u32 swrstcon;
	u32 rstcon;
	u32 _res4[13];
	u32 inform[4];
};

#define PLLCON_MDIV_GET(x)	(((x)>>12) & 0xFF)
#define PLLCON_PDIV_GET(x)	(((x)>>4) & 0x3F)
#define PLLCON_SDIV_GET(x)	(((x)>>0) & 0x3)

#define CLKDIVN_DVS		(1<<4)
#define CLKDIVN_ARMDIV_GET(x)	(((x)>>3) & 0x1)
#define CLKDIVN_ARMDIV		(1<<3)
#define CLKDIVN_PCLKDIV		(1<<2)
#define CLKDIVN_HCLKDIV_GET(x)	(((x)>>0) & 0x3)

static inline struct s3c2412_interrupt *s3c2412_get_base_interrupt(void)
{
	return (struct s3c2412_interrupt *)S3C2412_INTERRUPT_BASE;
}

static inline struct s3c2412_sysctl *s3c2412_get_base_sysctl(void)
{
	return (struct s3c2412_sysctl *)S3C2412_SYSCTL_BASE;
}

/* Include common stuff */
#include <asm/arch/s3c24xx.h>

#endif /*__S3C2412_H__*/
