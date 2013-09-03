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

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * SoC Configuration
 */
#define CONFIG_ARM926EJS	/* ARM926EJS CPU Core */
#define CONFIG_S3C24XX		/* SAMSUNG S3C24XX Family */
#define CONFIG_S3C2416		/* SAMSUNG S3C2416 SoC */
#define CONFIG_SYS_CLK_FREQ	12000000
#define CONFIG_SYS_HZ		1000

/*
 * Memory Information
 */
#define CONFIG_SYS_IRAM_BASE	0x00000000	/* Steppingstone base address */
#define CONFIG_SYS_IRAM_SIZE	(8 << 10)	/* 8KB of Steppingstone */
#define CONFIG_SYS_IRAM_END	(CONFIG_SYS_IRAM_BASE + CONFIG_SYS_IRAM_SIZE)

#define CONFIG_SYS_SRAM_BASE	0x00002000	/* SRAM base address */
#define CONFIG_SYS_SRAM_SIZE	(56 << 10)	/* 56KB of SRAM */
#define CONFIG_SYS_SRAM_END	(CONFIG_SYS_SRAM_BASE + CONFIG_SYS_SRAM_SIZE)

#define CONFIG_SYS_SDRAM_BASE	0x30000000	/* DDR2 SDRAM base address */
#define CONFIG_SYS_SDRAM_SIZE	(64 << 20)	/* 64MB of DDR2 SDRAM */
#define CONFIG_SYS_SDRAM_END	(CONFIG_SYS_SDRAM_BASE + CONFIG_SYS_SDRAM_SIZE)



/*
 * USB
 *
 */

#define CONFIG_SYS_DCACHE_OFF //damn, nowhere written that this is needed...
#define CONFIG_CMD_USB
#define CONFIG_DOS_PARTITION
#define CONFIG_SUPPORT_VFAT
#define CONFIG_LEGACY_USB_INIT_SEQ 1

#define CONFIG_CMD_KEYBOARD

#define CONFIG_USB_OHCI_NEW
#define CONFIG_SYS_USB_OHCI_CPU_INIT
#define CONFIG_SYS_USB_OHCI_MAX_ROOT_PORTS 2
#define CONFIG_USB_OHCI_S3C24XX
#define CONFIG_USB_STORAGE
#define CONFIG_CMD_FAT

#define CONFIG_SYS_USB_OHCI_REGS_BASE  0x49000000
#define CONFIG_SYS_USB_OHCI_SLOT_NAME "s3c2400"


/*
 * Linux Interface
 */
#define MACH_TYPE_MINI2416		3850
#define CONFIG_MACH_TYPE		MACH_TYPE_MINI2416
#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_BOOTDELAY		3
#define CONFIG_BOOTARGS			"console=ttySAC1,115200n8"
#define CONFIG_BOOTCOMMAND		""	/* TBD */

/*
 * SPL
 */
#define CONFIG_SPL
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_BOARD_INIT

#define CONFIG_SPL_NAND_SUPPORT
#define CONFIG_SPL_NAND_DRIVERS
#define CONFIG_SPL_NAND_SIMPLE

#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_TEXT_BASE		0x00000000 /* CONFIG_SYS_IRAM_BASE */
#define CONFIG_SPL_MAX_SIZE		CONFIG_SYS_IRAM_SIZE
#define CONFIG_SPL_BSS_START_ADDR	CONFIG_SYS_SRAM_BASE
#define CONFIG_SPL_BSS_MAX_SIZE		(CONFIG_SYS_SRAM_SIZE - (8 << 10))
#define CONFIG_SPL_STACK		CONFIG_SYS_SRAM_END /* 8KB for stack */

/*
 * Monitor Interface
 */
#define CONFIG_SYS_PROMPT		"SOM2416II# "
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					 sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		16
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_CMDLINE_EDITING

/*
 * Command Definition
//#define CONFIG_CMD_DATE
 */
#define CONFIG_SYS_NO_FLASH	/* No NOR Flash */
#include <config_cmd_default.h>
#define CONFIG_CMD_CACHE
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MISC
#define CONFIG_CMD_NAND
#define CONFIG_CMD_PING

/*
 * Miscellaneous Settings
 */
#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO
#define CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + (1 << 20))
#define CONFIG_SYS_MONITOR_BASE		(CONFIG_SYS_SDRAM_END - (1 << 20))
#define CONFIG_SYS_MONITOR_LEN		(256 << 10) /*262144*/
#define CONFIG_SYS_TEXT_BASE		0x33F00000 /* CONFIG_SYS_MONITOR_BASE */
#define CONFIG_SYS_MALLOC_LEN		(384 << 10)
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_SRAM_END - \
					 GENERATED_GBL_DATA_SIZE)

/*
 * NAND Flash
 */
#ifdef CONFIG_CMD_NAND

#define CONFIG_NAND_S3C24XX
#define CONFIG_S3C24XX_NAND_HWECC
#define CONFIG_SYS_NAND_BASE		0x4E000010
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CFG_NAND_FLASH_BBT

/* SPL NAND Driver */
#define CONFIG_SPL_NAND_ECC

#define CONFIG_SYS_NAND_PAGE_SIZE	(2 << 10)
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128 << 10)
#define CONFIG_SYS_NAND_PAGE_COUNT	(CONFIG_SYS_NAND_BLOCK_SIZE / \
					 CONFIG_SYS_NAND_PAGE_SIZE)
#define CONFIG_SYS_NAND_OOBSIZE		64
#define CONFIG_SYS_NAND_ECCSIZE		512
#define CONFIG_SYS_NAND_ECCBYTES	4
#define CONFIG_SYS_NAND_ECCPOS		{40, 41, 42, 43, 44, 45, 46, 47, \
					 48, 49, 50, 51, 52, 53, 54, 55}
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	0
#define CONFIG_SYS_NAND_HW_ECC_OOBFIRST
#define CONFIG_SYS_NAND_5_ADDR_CYCLE

#define CONFIG_SYS_NAND_U_BOOT_DST	    CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_NAND_U_BOOT_DST
#define CONFIG_SYS_NAND_U_BOOT_OFFS	    CONFIG_SPL_MAX_SIZE
/*#define CONFIG_SYS_NAND_U_BOOT_OFFS	    1179648*/
#define CONFIG_SYS_NAND_U_BOOT_SIZE	    (CONFIG_SYS_MONITOR_LEN - \
					                     CONFIG_SPL_MAX_SIZE)
#define CONFIG_SYS_UBOOT_START         CONFIG_SYS_NAND_U_BOOT_DST

#endif
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128 << 10)

/*
 * Serial Driver
 */
#define CONFIG_S3C24X0_SERIAL
#define CONFIG_SERIAL2
#define CONFIG_BAUDRATE			115200

/*
 * Ethernet
 */
#ifdef CONFIG_CMD_NET
#define CONFIG_SMC911X
#define CONFIG_SMC911X_BASE		0x08000000
#define CONFIG_SMC911X_16_BIT
#define CONFIG_ETHADDR			FE:11:22:33:44:55
#define CONFIG_OVERWRITE_ETHADDR_ONCE
#define CONFIG_IPADDR			192.168.1.111
#define CONFIG_NETMASK			255.255.255.0
#define CONFIG_SERVERIP			192.168.1.175
#define CONFIG_GATEWAYIP		192.168.1.1
#endif

/*
 * RTC
#ifdef CONFIG_CMD_DATE
#define CONFIG_RTC_S3C24X0
#endif
 */

/*
 * Environment
#ifdef CONFIG_CMD_NAND
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_SIZE			CONFIG_SYS_NAND_BLOCK_SIZE
#define CONFIG_ENV_OFFSET		CONFIG_SYS_MONITOR_LEN
#define CONFIG_ENV_SIZE_REDUND		CONFIG_ENV_SIZE
#define CONFIG_ENV_OFFSET_REDUND	(CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE)
#else
 */
#define CONFIG_ENV_IS_NOWHERE
/*#endif*/

#define CONFIG_ENV_SIZE			CONFIG_SYS_NAND_BLOCK_SIZE

/*
 * File System
 */
#ifdef CONFIG_CMD_NAND
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#endif

#endif /* __CONFIG_H */
