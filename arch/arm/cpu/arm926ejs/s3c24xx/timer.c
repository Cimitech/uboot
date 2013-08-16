/*
 * (C) Copyright 2012 INOV - INESC Inovacao
 * Jose Goncalves <jose.goncalves@inov.pt>
 *
 * Based on arch/arm/cpu/armv7/s5p-common/timer.c and U-Boot 1.3.4 from Samsung.
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
#include <asm/arch/s3c24xx_cpu.h>

#define PRESCALER 15

DECLARE_GLOBAL_DATA_PTR;

static ulong get_current_tick(void)
{
	struct s3c24xx_timers *const timers = s3c24xx_get_base_timers();
	ulong now = readl(&timers->tcnto4);

	if (gd->arch.lastinc >= now)
		gd->arch.tbl += gd->arch.lastinc - now;
	else
		gd->arch.tbl += gd->arch.lastinc + gd->arch.tbu - now;

	gd->arch.lastinc = now;

	return gd->arch.tbl;
}

int timer_init(void)
{
	struct s3c24xx_timers *const timers = s3c24xx_get_base_timers();

	/* use PWM Timer 4 because it has no output */
	if (gd->arch.tbu == 0) {
		/* set divider value for Timer 4 to 2 */
		clrsetbits_le32(&timers->tcfg1, TCFG1_MUX4_MASK,
				TCFG1_MUX4_DIV2);
		/* set prescaler value for Timer 4 to PRESCALER */
		clrsetbits_le32(&timers->tcfg0, TCFG0_PRESCALER1_MASK,
				TCFG0_PRESCALER1(PRESCALER));
		/*
		 * Timer Freq(HZ) =
		 *      PCLK / (prescaler_value + 1) / (divider_value)
		 */
		gd->arch.timer_rate_hz = get_PCLK() / (PRESCALER + 1) / 2;
		gd->arch.tbu = gd->arch.timer_rate_hz / CONFIG_SYS_HZ;
	}
	/* load value for selected timeout */
	writel(gd->arch.tbu, &timers->tcntb4);
	/* auto reload, manual update of timer 4 */
	clrsetbits_le32(&timers->tcon, TCON_T4START,
			TCON_T4RELOAD | TCON_T4MANUALUPD);
	/* auto reload, start timer 4 */
	clrsetbits_le32(&timers->tcon, TCON_T4MANUALUPD,
			TCON_T4RELOAD | TCON_T4START);
	gd->arch.lastinc = 0;
	gd->arch.tbl = 0;

	return 0;
}

ulong get_timer(ulong base)
{
	return get_timer_masked() - base;
}

void __udelay(unsigned long usec)
{
	ulong tmo, tmp;

	if (usec >= 1000) {
		/*
		 * if "big" number, spread normalization
		 * to seconds
		 * 1. start to normalize for usec to ticks per sec
		 * 2. find number of "ticks" to wait to achieve target
		 * 3. finish normalize.
		 */
		tmo = usec / 1000;
		tmo *= gd->arch.timer_rate_hz;
		tmo /= 1000;
	} else {
		/* else small number, don't kill it prior to HZ multiply */
		tmo = usec * gd->arch.timer_rate_hz;
		tmo /= (1000 * 1000);
	}

	/* get current timestamp */
	tmp = get_current_tick();

	/* if advancing the timestamp would roll-up the timer, reset it */
	if ((tmo + tmp + 1) < tmp)
		reset_timer_masked();
	else
		tmo += tmp;

	/* loop till event */
	while (get_current_tick() < tmo)
		/* NOP */;
}

void reset_timer_masked(void)
{
	struct s3c24xx_timers *const timers = s3c24xx_get_base_timers();

	/* reset time */
	gd->arch.lastinc = readl(&timers->tcnto4);
	gd->arch.tbl = 0;
}

ulong get_timer_masked(void)
{
	return get_current_tick() / gd->arch.tbu;
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return get_timer_masked();
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}
