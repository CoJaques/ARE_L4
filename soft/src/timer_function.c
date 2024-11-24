
#include "timer_function.h"
#include <string.h>

// Global variable to store the callback function
timer_callback_t timer_callback_function = NULL;

/*
 * Setup the timer with a callback function
 */
void setup_timer(uint32_t load_value, timer_callback_t callback)
{
	// Reset the timer
	TIMER_REG(TIMER_CONTROL_OFFSET) = 0;

	// Set the user mode on the timer
	TIMER_REG(TIMER_CONTROL_OFFSET) |= (1 << 1);

	// Unmask irqs into the timer
	TIMER_REG(TIMER_CONTROL_OFFSET) &= ~(1 << 2);

	// Set the load value in the timer
	TIMER_REG(TIMER_LOAD_OFFSET) = load_value;

	// Store the callback function
	timer_callback_function = callback;
}

/*
 * Start the timer
 */
void start_timer(void)
{
	TIMER_REG(TIMER_CONTROL_OFFSET) |= 1;
}

/*
 * Stop the timer
 */
void stop_timer(void)
{
	// Disable timer
	TIMER_REG(TIMER_CONTROL_OFFSET) &= ~(1);
}

/*
 * Timer interrupt handler (should be called by the IRQ handler)
 */
void timer_interrupt_handler(void)
{
	// Clear the timer interrupt
	TIMER_REG(TIMER_INTCLR_OFFSET);

	// Execute the callback function if it's set
	if (timer_callback_function) {
		timer_callback_function();
	}
}
