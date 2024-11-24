/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : pio_function.h
 * Author               : Anthony Convers
 * Date                 : 27.07.2022
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: Header file for pio function
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 0.0    27.07.2022  ACS           Initial version.
 * 1.0    13.10.2024  CJS          Implement lab2.
 *
*****************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "axi_lw.h"

// Constant ID
#define CONST_ID	      AXI_LW_REG(0x0)

// Base address
#define PIO_CORE0_BASE_OFFSET 0x100
#define PIO_CORE1_BASE_OFFSET 0x120
#define PIO_CORE0_BASE_ADD    (AXI_LW_HPS_FPGA_BASE_ADD + PIO_CORE0_BASE_OFFSET)
#define PIO_CORE1_BASE_ADD    (AXI_LW_HPS_FPGA_BASE_ADD + PIO_CORE1_BASE_OFFSET)

// ACCESS MACROS
// _x_ is an offset with respect to the base address
#define PIO0_REG(_x_)	      *(volatile uint32_t *)(PIO_CORE0_BASE_ADD + _x_)
#define PIO1_REG(_x_)	      *(volatile uint32_t *)(PIO_CORE1_BASE_ADD + _x_)
#define PIO0_DIR	      PIO0_REG(PIO_DIR_OFFSET)
#define PIO1_DIR	      PIO1_REG(PIO_DIR_OFFSET)
#define PIO0_DATA	      PIO0_REG(PIO_DATA_OFFSET)
#define PIO1_DATA	      PIO1_REG(PIO_DATA_OFFSET)

// PIO Registers
#define PIO_DATA_OFFSET	      0x0
#define PIO_DIR_OFFSET	      0x4

// Define PIO bits usage
#define SWITCHS_BITS	      0x000003FF
#define LEDS_BITS	      	0x000FFC00
#define SEGMENTS_BITS	      0x0FFFFFFF
#define SEGMENT_MASK	      0x7F

#define INIT_KEYS	      0x00F00000
#define INIT_LEDS	      0x000

#define SWITCHS_OFFSET	      0x0
#define LEDS_OFFSET	      10
#define KEYS_OFFSET			20

#define SEGMENT_LENGTH	      0x7
#define MAX_7SEG_VALUE	      16

#define NUM_KEYS	      4
#define KEY_0		      0
#define KEY_1		      1
#define KEY_2		      2
#define KEY_3		      3

static const uint8_t decimal_to_7seg_conversion[] = {
	0x40, // 0
	0x79, // 1
	0x24, // 2
	0x30, // 3
	0x19, // 4
	0x12, // 5
	0x02, // 6
	0x78, // 7
	0x00, // 8
	0x10, // 9
	0x08, // A
	0x03, // B
	0x46, // C
	0x21, // D
	0x06, // E
	0x0E, // F
};

//***************************//
//****** Init function ******//

// Swicths_init function : Initialize all Switchs in PIO core (SW9 to SW0)
void Switchs_init(void);

// Leds_init function : Initialize all Leds in PIO core (LED9 to LED0)
void Leds_init(void);

// Keys_init function : Initialize all Keys in PIO core (KEY3 to KEY0)
void Keys_init(void);

// Segs7_init function : Initialize all 7-segments display in PIO core (HEX3 to HEX0)
void Segs7_init(void);

//***********************************//
//****** Global usage function ******//

// Switchs_read function : Read the switchs value
// Parameter : None
// Return : Value of all Switchs (SW9 to SW0)
uint32_t Switchs_read(void);

// Leds_write function : Write a value to all Leds (LED9 to LED0)
// Parameter : "value"= data to be applied to all Leds
// Return : None
void Leds_write(uint32_t value);

// Leds_set function : Set to ON some or all Leds (LED9 to LED0)
// Parameter : "maskleds"= Leds selected to apply a set (maximum 0x3FF)
// Return : None
void Leds_set(uint32_t maskleds);

// Leds_clear function : Clear to OFF some or all Leds (LED9 to LED0)
// Parameter : "maskleds"= Leds selected to apply a clear (maximum 0x3FF)
// Return : None
void Leds_clear(uint32_t maskleds);

// Leds_toggle function : Toggle the curent value of some or all Leds (LED9 to LED0)
// Parameter : "maskleds"= Leds selected to apply a toggle (maximum 0x3FF)
// Return : None
void Leds_toggle(uint32_t maskleds);

// Key_read function : Read one Key status, pressed or not (KEY0 or KEY1 or KEY2 or KEY3)
// Parameter : "key_number"= select the key number to read, from 0 to 3
// Return : True(1) if key is pressed, and False(0) if key is not pressed
bool Key_read(int key_number);

// Seg7_write function : Write digit segment value to one 7-segments display (HEX0 or HEX1 or HEX2 or HEX3)
// Parameter : "seg7_number"= select the 7-segments number, from 0 to 3
// Parameter : "value"= digit segment value to be applied on the selected 7-segments (maximum 0x7F to switch ON all segments)
// Return : None
void Seg7_write(int seg7_number, uint32_t value);

// Seg7_write_hex function : Write an Hexadecimal value to one 7-segments display (HEX0 or HEX1 or HEX2 or HEX3)
// Parameter : "seg7_number"= select the 7-segments number, from 0 to 3
// Parameter : "value"= Hexadecimal value to be display on the selected 7-segments, form 0x0 to 0xF
// Return : None
void Seg7_write_hex(int seg7_number, uint32_t value);

// Seg7_write_time function : Write a time in miliseconds to the 7-segments display (HEX0 to HEX3)
// Parameter : "miliseconds"= time in miliseconds to be display on the 7-segments display
// Return : None
void Seg7_write_time(uint32_t miliseconds);
