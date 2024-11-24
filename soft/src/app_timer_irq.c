/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : app_timer_irq.c
 * Author               : Anthony Convers
 * Date                 : 27.10.2022
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: Using timer IRQ on DE1-SoC board
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 0.0    27.10.2022  ACS           Initial version.
 *
*****************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "timer_function.h"
#include "exceptions.h"
#include "pio_function.h"
#include "axi_lw.h"

int __auto_semihosting;

uint32_t counter = 0;

void timer_irq_callback(void)
{
	if (counter == 0) {
		stop_timer();

		// clear led 8 as we stop the timer and led 9 as the timer is done
		Leds_clear(1 << 9 | 1 << 8);
		return;
	}

	counter--;
	Leds_toggle(1 << 9);
	Seg7_write_time(counter);
}

void read_keys(bool *keys_state)
{
	for (int i = 0; i < NUM_KEYS; i++) {
		keys_state[i] = Key_read(i);
	}
}

void update_old_keys(bool *keys_state, bool *keys_state_old)
{
	for (int i = 0; i < NUM_KEYS; i++) {
		keys_state_old[i] = keys_state[i];
	}
}

int main(void)
{
	printf("Laboratoire: Timer IRQ \n");
	printf("Constant ID = 0x%lx\n", (unsigned long)CONST_ID);

	// Initialize the irqs
	disable_A9_interrupts();
	set_A9_IRQ_stack();
	config_GIC();

	// As asked, we want to refresh the screen every 100ms
	int clock_frequency = 25e6;
	double period = 1.0 / clock_frequency;
	double refresh_period = 0.1;
	uint32_t load_value = (uint32_t)(refresh_period / period);
	setup_timer(load_value, timer_irq_callback);

	// Enable the timer irq
	enable_A9_interrupts();

	Keys_init();
	Leds_init();
	Switchs_init();
	Segs7_init();

	bool keys_state[4] = { false, false, false, false };
	bool keys_state_old[4] = { false, false, false, false };

	while (1) {
		const uint32_t switchs_value = Switchs_read();
		read_keys(keys_state);

		if (keys_state[KEY_0] && !keys_state_old[KEY_0]) {
			start_timer();
			Leds_set(1 << 8);
		}

		if (keys_state[KEY_1] && !keys_state_old[KEY_1]) {
			stop_timer();
			Leds_clear(1 << 8);
		}

		if (keys_state[KEY_2] && !keys_state_old[KEY_2]) {
			stop_timer();
			Leds_write(switchs_value & 0xFF);
			counter = ((switchs_value & 0xFF) * 10);
			Seg7_write_time(counter);
		}

		update_old_keys(keys_state, keys_state_old);
	}
}
