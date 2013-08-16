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

/* To be able to use s3c24x0 drivers */

#ifndef __S3C24X0_CPU_H__
#define __S3C24X0_CPU_H__

#include <asm/arch/s3c24xx_cpu.h>

#define s3c24x0_rtc s3c24xx_rtc
#define s3c24x0_get_base_rtc s3c24xx_get_base_rtc

#define S3C24X0_UART0 S3C24XX_UART0
#define S3C24X0_UART1 S3C24XX_UART1
#define S3C24X0_UART2 S3C24XX_UART2
#define S3C24X0_UART3 S3C24XX_UART3
#define s3c24x0_uart s3c24xx_uart
#define s3c24x0_get_base_uart s3c24xx_get_base_uart

#endif
