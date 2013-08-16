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
#include <linux/compiler.h>
#include <asm/io.h>
#include <asm/arch/s3c24xx_cpu.h>

void enable_caches(void)
{
#ifndef CONFIG_SYS_ICACHE_OFF
	icache_enable();
#endif
#ifndef CONFIG_SYS_DCACHE_OFF
	dcache_enable();
#endif
}

/*
 * Reset the cpu by setting up the watchdog timer and let him time out.
 */
void __noreturn reset_cpu(ulong addr)
{
	struct s3c24xx_watchdog *const watchdog = s3c24xx_get_base_watchdog();

	/* Disable watchdog */
	writel(WTCON_DISABLE_VAL, &watchdog->wtcon);

	/* Initialize watchdog timer count register with the minimum value */
	writel(0x0001, &watchdog->wtcnt);

	/* Enable watchdog timer; assert reset at timer timeout */
	writel(WTCON_RSTEN | WTCON_ENABLE, &watchdog->wtcon);

	while (1)
		/* loop forever and wait for reset to happen */;
}
