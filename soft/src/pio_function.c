/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : pio_function.c
 * Author               : Colin Jaques
 * Date                 : 
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: Pio function
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 1.0    13.10.2024  CJS          INITIAL VERSION
 *
*****************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "axi_lw.h"
#include "pio_function.h"

void Switchs_init(void)
{
	// set switches as input
	PIO0_DIR &= ~SWITCHS_BITS;
}

void Leds_init(void)
{
	// set leds as output
	PIO0_DIR |= LEDS_BITS;
	Leds_write(INIT_LEDS);
}

void Keys_init(void)
{
	// set keys as input
	PIO0_DIR &= ~INIT_KEYS;
}

void Segs7_init(void)
{
	// set 7-segments as output
	PIO1_DIR |= SEGMENTS_BITS;

	// clear 7-segments and write base data
	Seg7_write_time(0);
}

uint32_t Switchs_read(void)
{
	return (PIO0_DATA & SWITCHS_BITS) >> SWITCHS_OFFSET;
}

void Leds_write(uint32_t value)
{
	uint32_t value_to_write = (value << LEDS_OFFSET) & LEDS_BITS;
	uint32_t actual_value = PIO0_DATA & ~LEDS_BITS;
	PIO0_DATA = actual_value | value_to_write;
}

void Leds_set(uint32_t maskleds)
{
	PIO0_DATA |= (maskleds << LEDS_OFFSET);
}

void Leds_clear(uint32_t maskleds)
{
	PIO0_DATA &= ~(maskleds << LEDS_OFFSET) & LEDS_BITS;
}

void Leds_toggle(uint32_t maskleds)
{
	PIO0_DATA ^= (maskleds << LEDS_OFFSET) & LEDS_BITS;
}

bool Key_read(int key_number)
{
	return !(PIO0_DATA & (1 << (key_number + KEYS_OFFSET)));
}

void Seg7_write(int seg7_number, uint32_t value)
{
	const uint8_t shift = seg7_number * 7;
	const uint32_t mask = 0x7F << shift;

	// value is inverted because the 7-segment display is active low
	// so we need to invert the value to display correspond to the specification
	PIO1_DATA = (PIO1_DATA & ~mask) | (~value << shift);
}

void Seg7_write_hex(int seg7_number, uint32_t value)
{
	Seg7_write(seg7_number, ~decimal_to_7seg_conversion[value]);
}

void Seg7_write_time(uint32_t miliseconds)
{
	Seg7_write_hex(0, miliseconds % 10);
	Seg7_write_hex(1, (miliseconds / 10) % 10);
	Seg7_write_hex(2, (miliseconds / 100) % 10);
	Seg7_write_hex(3, (miliseconds / 1000) % 10);
}
