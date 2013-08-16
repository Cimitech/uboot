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

MEMORY { .iram : ORIGIN = CONFIG_SPL_TEXT_BASE, \
                 LENGTH = CONFIG_SPL_MAX_SIZE }
MEMORY { .sram : ORIGIN = CONFIG_SPL_BSS_START_ADDR, \
                 LENGTH = CONFIG_SPL_BSS_MAX_SIZE }

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	.text :
	{
		CPUDIR/start.o (.text)
		*(.text*)
	} > .iram

	. = ALIGN(4);
	.rodata :
	{
		*(SORT_BY_ALIGNMENT(.rodata*))
	} > .iram

	. = ALIGN(4);
	.data :
	{
		*(SORT_BY_ALIGNMENT(.data*))
	} > .iram

	. = ALIGN(4);
	_end = .;

	.bss :
	{
		. = ALIGN(4);
		__bss_start = .;
		*(.bss*)
		. = ALIGN(4);
		__bss_end = .;
	} >.sram
}
