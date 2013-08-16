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

/******************************************************
 * NAME	    : s3c24xx.h
 *
 * Common stuff for SAMSUNG S3C24XX SoC (ARM926EJ core)
 ******************************************************/

#ifndef __S3C24XX_H__
#define __S3C24XX_H__

/* Base addresses for S3C24XX common modules */
#define S3C24XX_DRAMCTL_BASE		0x48000000
#define S3C24XX_USB_HOST_BASE		0x49000000
#define S3C24XX_DMA_BASE		0x4B000000
#define S3C24XX_NAND_BASE		0x4E000000
#define S3C24XX_SMC_BASE		0x4F000000
#define S3C24XX_UART_BASE		0x50000000
#define S3C24XX_TIMER_BASE		0x51000000
#define S3C24XX_WATCHDOG_BASE		0x53000000
#define S3C24XX_I2C_BASE		0x54000000
#define S3C24XX_I2S_BASE		0x55000000
#define S3C24XX_GPIO_BASE		0x56000000
#define S3C24XX_RTC_BASE		0x57000000
#define S3C24XX_ADC_BASE		0x58000000

/* Mobile DRAM Controller */
struct s3c24xx_dramctl {
	u32 bankcfg;
	u32 bankcon1;
	u32 bankcon2;
	u32 bankcon3;
	u32 refresh;
	u32 timeout;
};

#define BANKCFG_VAL_DDR2		0x00049253

#define BANKCON1_VAL_DDR2		0x44000040
#define BANKCON1_INIT_NORMAL		0x0
#define BANKCON1_INIT_PALL		0x1
#define BANKCON1_INIT_MRS		0x2
#define BANKCON1_INIT_EMRS		0x3

#define BANKCON2_VAL_DDR2		0x005D0035

#define BANKCON3_VAL_EMRS1		0x44000000
#define BANKCON3_EMRS1_CAS3		(3<<4)
#define BANKCON3_EMRS1_OCD7		(7<<23)
#define BANKCON3_VAL_EMRS2		0x80000000
#define BANKCON3_VAL_EMRS3		0xC0000000
#define BANKCON3_VAL_MRS		0x44000030
#define BANKCON3_MRS_DLL_RESET		(1<<8)

/* USB Host Controller */
struct s3c24xx_usb_host {
	u32 HcRevision;
	u32 HcControl;
	u32 HcCommonStatus;
	u32 HcInterruptStatus;
	u32 HcInterruptEnable;
	u32 HcInterruptDisable;
	u32 HcHCCA;
	u32 HcPeriodCuttendED;
	u32 HcControlHeadED;
	u32 HcControlCurrentED;
	u32 HcBulkHeadED;
	u32 HcBuldCurrentED;
	u32 HcDoneHead;
	u32 HcRmInterval;
	u32 HcFmRemaining;
	u32 HcFmNumber;
	u32 HcPeriodicStart;
	u32 HcLSThreshold;
	u32 HcRhDescriptorA;
	u32 HcRhDescriptorB;
	u32 HcRhStatus;
	u32 HcRhPortStatus1;
	u32 HcRhPortStatus2;
};

/* DMA Controller */
struct s3c24xx_dma {
	u32 disrc;
	u32 disrcc;
	u32 didst;
	u32 didstc;
	u32 dcon;
	u32 dstat;
	u32 dcsrc;
	u32 dcdst;
	u32 dmasktrig;
	u32 dmareqsel0;
};

/* NAND Flash Controller */
struct s3c24xx_nand {
	u32 nfconf;
	u32 nfcont;
	u32 nfcmmd;
	u32 nfaddr;
	u32 nfdata;
	u32 nfmeccd[2];
	u32 nfseccd;
	u32 nfsblk;
	u32 nfeblk;
	u32 nfstat;
	u32 nfeccerr[2];
	u32 nfmecc[2];
	u32 nfsecc;
	u32 nfmlcbitpt;
#if !(defined(CONFIG_S3C2412) || defined(CONFIG_S3C2413))
	u32 nf8eccerr[3];
	u32 nfm8ecc[4];
	u32 nfmlc8bitpt[2];
#endif
};

#if defined(CONFIG_S3C2412) || defined(CONFIG_S3C2413)
#define NFCONF_ECCTYPE_MASK	(1<<24)
#define NFCONF_ECCTYPE_1BIT	(0<<24)
#define NFCONF_ECCTYPE_4BIT	(1<<24)
#else
#define NFCONF_ECCTYPE_MASK	(3<<23)
#define NFCONF_ECCTYPE_1BIT	(0<<23)
#define NFCONF_ECCTYPE_8BIT	(1<<23)
#define NFCONF_ECCTYPE_4BIT	(2<<23)
#endif
#define NFCONF_TACLS_MASK	(7<<12)
#define NFCONF_TWRPH0_MASK	(7<<8)
#define NFCONF_TWRPH1_MASK	(7<<4)
#define NFCONF_TACLS(x)		((x)<<12)
#define NFCONF_TWRPH0(x)	((x)<<8)
#define NFCONF_TWRPH1(x)	((x)<<4)

#define NFCONT_ENABLE		(1<<0)
#define NFCONT_NCE0		(1<<1)
#define NFCONT_NCE1		(1<<2)
#define NFCONT_INITSECC		(1<<4)
#define NFCONT_INITMECC		(1<<5)
#define NFCONT_SECCLOCK		(1<<6)
#define NFCONT_MECCLOCK		(1<<7)
#define NFCONT_WP		(1<<16)
#define NFCONT_ECC_ENC		(1<<18)

#define NFSTAT_RNB		(1<<0)

/* SMC */
struct s3c24xx_smc {
	u32 smbidcy;
	u32 smbwstrd;
	u32 smbwstwr;
	u32 smbwstoen;
	u32 smbwstwen;
	u32 smbc;
	u32 smbs;
	u32 smbwstbrd;
};

#define SMBC_RBLE		(1<<0)
#define SMBC_WAIT_POL		(1<<1)
#define SMBC_WAIT_EN		(1<<2)
#define SMBC_MW_MASK		(0x3<<4)
#define SMBC_MW_16BIT		(1<<4)
#define SMBC_MW_8BIT		(0<<4)
#define SMBC_DRNCS		(1<<7)
#define SMBC_RSMAVD_RD		(1<<12)
#define SMBC_DRNOWE		(1<<15)
#define SMBC_RSMAVD_WR		(1<<20)

/* UART */
struct s3c24xx_uart {
	u32 ulcon;
	u32 ucon;
	u32 ufcon;
	u32 umcon;
	u32 utrstat;
	u32 uerstat;
	u32 ufstat;
	u32 umstat;
#ifdef __BIG_ENDIAN
	u8 _res1[3];
	u8 utxh;
	u8 _res2[3];
	u8 urxh;
#else				/* Little Endian */
	u8 utxh;
	u8 _res1[3];
	u8 urxh;
	u8 _res2[3];
#endif
	u32 ubrdiv;
	u32 udivslot;
};

#define ULCON_8N1		0x03

#define UCON_POLL		0x005

#define UMCON_NOAFC		0

#define UFCON_FIFOMODE		(1<<0)
#define UFCON_RESETRX		(1<<1)
#define UFCON_RESETTX		(1<<2)

#define UTRSTAT_RXDR		(1<<0)
#define UTRSTAT_TXFE		(1<<1)
#define UTRSTAT_TXE		(1<<2)

/* PWM Timer */
struct s3c24xx_timer {
	u32 tcntb;
	u32 tcmpb;
	u32 tcnto;
};

struct s3c24xx_timers {
	u32 tcfg0;
	u32 tcfg1;
	u32 tcon;
	struct s3c24xx_timer ch[4];
	u32 tcntb4;
	u32 tcnto4;
};

#define TCFG0_PRESCALER1_MASK	(0xFF<<8)
#define TCFG0_PRESCALER1(x)	((x)<<8)

#define TCFG1_MUX4_DIV2		(0<<16)
#define TCFG1_MUX4_DIV4		(1<<16)
#define TCFG1_MUX4_DIV8		(2<<16)
#define TCFG1_MUX4_DIV16	(3<<16)
#define TCFG1_MUX4_TCLK1	(4<<16)
#define TCFG1_MUX4_MASK		(0xF<<16)

#define TCON_T4START		(1<<20)
#define TCON_T4MANUALUPD	(1<<21)
#define TCON_T4RELOAD		(1<<22)

/* Watchdog Timer */
struct s3c24xx_watchdog {
	u32 wtcon;
	u32 wtdat;
	u32 wtcnt;
};

#define WTCON_DISABLE_VAL	0
#define WTCON_RSTEN		(1<<0)
#define WTCON_INTEN		(1<<2)
#define WTCON_ENABLE		(1<<5)

/* IIC-Bus Interface */
struct s3c24xx_i2c {
	u32 iiccon;
	u32 iicstat;
	u32 iicadd;
	u32 iicds;
	u32 iiclc;
};

/* IIS-Bus Interface */
struct s3c24xx_i2s {
	u32 iiscon;
	u32 iismod;
	u32 iisfic;
	u32 iispsr;
	u32 iistxd;
	u32 iisrxd;
};

/* I/O Ports */
struct s3c24xx_gpio {
#if defined(CONFIG_S3C2412) || defined(CONFIG_S3C2413)
	u32 gpacon;
	u32 gpadat;
	u32 _res1[2];
	u32 gpbcon;
	u32 gpbdat;
	u32 gpbdn;
	u32 gpbslpcon;
	u32 gpccon;
	u32 gpcdat;
	u32 gpcdn;
	u32 gpcslpcon;
	u32 gpdcon;
	u32 gpddat;
	u32 gpddn;
	u32 gpdslpcon;
	u32 gpecon;
	u32 gpedat;
	u32 gpedn;
	u32 gpeslpcon;
	u32 gpfcon;
	u32 gpfdat;
	u32 gpfdn;
	u32 _res2;
	u32 gpgcon;
	u32 gpgdat;
	u32 gpgdn;
	u32 gpgslpcon;
	u32 gphcon;
	u32 gphdat;
	u32 gphdn;
	u32 gphslpcon;
	u32 gpjcon;
	u32 gpjdat;
	u32 gpjdn;
	u32 gpjslpcon;

	u32 misccr;
	u32 dclkcon;
	u32 extint[3];
	u32 eintflt[4];
	u32 eintmask;
	u32 eintpend;
	u32 gstatus[6];
	u32 mstcon;
	u32 mslcon;
	u32 dsc[2];
#else
	u32 gpacon;
	u32 gpadat;
	u32 _res1[2];
	u32 gpbcon;
	u32 gpbdat;
	u32 gpbudp;
	u32 gpbsel;
	u32 gpccon;
	u32 gpcdat;
	u32 gpcudp;
	u32 _res2;
	u32 gpdcon;
	u32 gpddat;
	u32 gpdudp;
	u32 _res3;
	u32 gpecon;
	u32 gpedat;
	u32 gpeudp;
	u32 gpesel;
	u32 gpfcon;
	u32 gpfdat;
	u32 gpfudp;
	u32 _res4;
	u32 gpgcon;
	u32 gpgdat;
	u32 gpgudp;
	u32 _res5;
	u32 gphcon;
	u32 gphdat;
	u32 gphudp;
	u32 _res6;

	u32 misccr;
	u32 dclkcon;
	u32 extint[3];
	u32 _res7[2];
	u32 eintflt2;
	u32 eintflt3;
	u32 eintmask;
	u32 eintpend;
	u32 gstatus[2];
	u32 _res8[3];
	u32 dsc[4];

	u32 gpjcon;
	u32 gpjdat;
	u32 gpjudp;
	u32 gpjsel;
	u32 gpkcon;
	u32 gpkdat;
	u32 gpkudp;
	u32 _res9;
	u32 gplcon;
	u32 gpldat;
	u32 gpludp;
	u32 gplsel;
	u32 gpmcon;
	u32 gpmdat;
	u32 gpmudp;
	u32 _res10;

	u32 dsc3;
	u32 pddmcon;
	u32 pdsmcon;
#endif
};

#define GPACON_NRCS1		(1<<12)
#define GPACON_CLE		(1<<17)
#define GPACON_ALE		(1<<18)
#define GPACON_NFWE		(1<<19)
#define GPACON_NFRE		(1<<20)
#define GPACON_NRSTOUT		(1<<21)
#define GPACON_NFCE		(1<<22)

#define GPCCON_MASK(x)		(0x3 << ((x) * 2))
#define GPCCON_OUTPUT(x)	(0x1 << ((x) * 2))
#define GPCDAT(x)		(1 << (x))

#define GPHCON_MASK(x)		(0x3 << ((x) * 2))
#define GPHCON_TXD(x)		(0x2 << ((x) * 4))
#define GPHCON_RXD(x)		(0x2 << (((x) * 4) + 2))

/* RTC */
struct s3c24xx_rtc {
#ifdef __BIG_ENDIAN
	u8 _res1[67];
	u8 rtccon;
	u8 _res2[3];
	u8 ticnt0;
#if defined(CONFIG_S3C2412) || defined(CONFIG_S3C2413)
	u8 _res3[4];
#else
	u8 _res3[3];
	u8 ticnt2;
#endif
	u8 _res4[3];
	u8 ticnt1;
	u8 _res5[3];
	u8 rtcalm;
	u8 _res6[3];
	u8 almsec;
	u8 _res7[3];
	u8 almmin;
	u8 _res8[3];
	u8 almhour;
	u8 _res9[3];
	u8 almdate;
	u8 _res10[3];
	u8 almmon;
	u8 _res11[3];
	u8 almyear;
	u8 _res12[7];
	u8 bcdsec;
	u8 _res13[3];
	u8 bcdmin;
	u8 _res14[3];
	u8 bcdhour;
	u8 _res15[3];
	u8 bcddate;
	u8 _res16[3];
	u8 bcdday;
	u8 _res17[3];
	u8 bcdmon;
	u8 _res18[3];
	u8 bcdyear;
#if !(defined(CONFIG_S3C2412) || defined(CONFIG_S3C2413))
	u8 _res19[3];
	u8 tickcnt;
#endif
#else				/*  little endian */
	u8 _res0[64];
	u8 rtccon;
	u8 _res1[3];
	u8 ticnt0;
	u8 _res2[3];
#if defined(CONFIG_S3C2412) || defined(CONFIG_S3C2413)
	u8 _res3[4];
#else
	u8 ticnt2;
	u8 _res3[3];
#endif
	u8 ticnt1;
	u8 _res4[3];
	u8 rtcalm;
	u8 _res5[3];
	u8 almsec;
	u8 _res6[3];
	u8 almmin;
	u8 _res7[3];
	u8 almhour;
	u8 _res8[3];
	u8 almdate;
	u8 _res9[3];
	u8 almmon;
	u8 _res10[3];
	u8 almyear;
	u8 _res11[7];
	u8 bcdsec;
	u8 _res12[3];
	u8 bcdmin;
	u8 _res13[3];
	u8 bcdhour;
	u8 _res14[3];
	u8 bcddate;
	u8 _res15[3];
	u8 bcdday;
	u8 _res16[3];
	u8 bcdmon;
	u8 _res17[3];
	u8 bcdyear;
	u8 _res18[3];
#if !(defined(CONFIG_S3C2412) || defined(CONFIG_S3C2413))
	u8 tickcnt;
	u8 _res19[3];
#endif
#endif
};

#define RTCCON_RTCEN	(1<<0)
#define RTCCON_CNTSEL	(1<<2)
#define RTCCON_CLKRST	(1<<3)
#define RTCCON_TICSEL	(1<<4)

/* ADC & Touch Screen Interface */
struct s3c24xx_adc {
	u32 adccon;
	u32 adctsc;
	u32 adcdly;
	u32 adcdat0;
	u32 adcdat1;
#if !(defined(CONFIG_S3C2412) || defined(CONFIG_S3C2413))
	u32 adcupdn;
	u32 adcmux;
#endif
};

static inline struct s3c24xx_dramctl *s3c24xx_get_base_dramctl(void)
{
	return (struct s3c24xx_dramctl *)S3C24XX_DRAMCTL_BASE;
}

static inline struct s3c24xx_usb_host *s3c24xx_get_base_usb_host(void)
{
	return (struct s3c24xx_usb_host *)S3C24XX_USB_HOST_BASE;
}

static inline struct s3c24xx_dma *s3c24xx_get_base_dma(enum s3c24xx_dmas n)
{
#if defined(CONFIG_S3C2412) || defined(CONFIG_S3C2413)
	return (struct s3c24xx_dma *)(S3C24XX_DMA_BASE + (n * 0x40));
#else
	return (struct s3c24xx_dma *)(S3C24XX_DMA_BASE + (n * 0x100));
#endif
}

static inline struct s3c24xx_nand *s3c24xx_get_base_nand(void)
{
	return (struct s3c24xx_nand *)S3C24XX_NAND_BASE;
}

static inline struct s3c24xx_smc *s3c24xx_get_base_smc(enum s3c24xx_smcs n)
{
	return (struct s3c24xx_smc *)(S3C24XX_SMC_BASE + (n * 0x20));
}

static inline struct s3c24xx_uart *s3c24xx_get_base_uart(enum s3c24xx_uarts n)
{
	return (struct s3c24xx_uart *)(S3C24XX_UART_BASE + (n * 0x4000));
}

static inline struct s3c24xx_timers *s3c24xx_get_base_timers(void)
{
	return (struct s3c24xx_timers *)S3C24XX_TIMER_BASE;
}

static inline struct s3c24xx_watchdog *s3c24xx_get_base_watchdog(void)
{
	return (struct s3c24xx_watchdog *)S3C24XX_WATCHDOG_BASE;
}

static inline struct s3c24xx_i2c *s3c24xx_get_base_i2c(void)
{
	return (struct s3c24xx_i2c *)S3C24XX_I2C_BASE;
}

static inline struct s3c24xx_i2s *s3c24xx_get_base_i2s(void)
{
	return (struct s3c24xx_i2s *)S3C24XX_I2S_BASE;
}

static inline struct s3c24xx_gpio *s3c24xx_get_base_gpio(void)
{
	return (struct s3c24xx_gpio *)S3C24XX_GPIO_BASE;
}

static inline struct s3c24xx_rtc *s3c24xx_get_base_rtc(void)
{
	return (struct s3c24xx_rtc *)S3C24XX_RTC_BASE;
}

static inline struct s3c24xx_adc *s3c24xx_get_base_adc(void)
{
	return (struct s3c24xx_adc *)S3C24XX_ADC_BASE;
}

static inline char *s3c24xx_get_cpu_name(void)
{
	return S3C24XX_CPU_NAME;
}

extern ulong get_ARMCLK(void);

#endif /*__S3C24XX_H__*/
