
/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 *
 * File                 : timer_function.h
 * Author               : Colin Jaques
 * Date                 : 24.11.2024
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: implement base functions for the timer 
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 1.0    24.11.2024  CJS          Implement lab4.
 * 1.1    24.11.2024  CJS          Improved timer setup with dynamic callback.
 *
 *****************************************************************************************/

#ifndef TIMER_FUNCTION_H
#define TIMER_FUNCTION_H

#include <stdint.h>

#define TIMER_BASE_OFFSET    0xFFD00000
#define TIMER_REG(_x_)	     *(volatile uint32_t *)(TIMER_BASE_OFFSET + _x_)

#define TIMER_LOAD_OFFSET    0x0
#define TIMER_VALUE_OFFSET   0x4
#define TIMER_CONTROL_OFFSET 0x8
#define TIMER_INTCLR_OFFSET  0xC

// Define a type for the timer callback function
typedef void (*timer_callback_t)(void);

// Global pointer to store the callback function
extern timer_callback_t timer_callback_function;

/*
 * Function used to setup the timer with a callback function
 * @param load_value : the value to load in the timer
 * @param callback   : pointer to the callback function to be executed on timer interrupt
 */
void setup_timer(uint32_t load_value, timer_callback_t callback);

/*
 * Function used to start the timer
 */
void start_timer(void);

/*
 * Function used to stop the timer
 */
void stop_timer(void);

#endif
