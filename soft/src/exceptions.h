
/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : exceptions.h
 * Author               : Anthony Convers
 * Date                 : 27.10.2022
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: Defines exception vectors for the A9 processor
 *        Provides code that sets the IRQ mode stack, and that dis/enables interrupts
 *        Provides code that initializes the generic interrupt controller
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Engineer      Comments
 * 0.0    27.10.2022  ACS           Initial version.
 *
 *****************************************************************************************/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdint.h>
#include "address_map_arm.h"
#include "int_defines.h"

// IRQ Timer ID
#define TIMER_IRQ_ID		  201

// Base addresses for GIC registers
#define GIC_DIST_BASE_ADDR	  0xFFFED000
#define GIC_DIST_SET_ENABLE_ADDR  0xFFFED100
#define GIC_DIST_PROC_TARGET_ADDR 0xFFFED800
#define GIC_ICCICR		  0xFFFEC100
#define GIC_ICCIAR		  0xFFFEC10C
#define GIC_ICCEOIR		  0xFFFEC110

// Specific registers in the CPU Interface
#define GIC_CPU_PRIORITY_MASK_REG (GIC_ICCICR + 0x04)
#define GIC_CPU_CONTROL_REG	  (GIC_ICCICR + 0x00)

// Distributor Control Register
#define GIC_DIST_CONTROL_REG	  (GIC_DIST_BASE_ADDR + 0x00)

// Masks and shifts
#define GIC_32_BIT_MASK		  0x1F
#define GIC_BYTE_MASK		  0x3
#define GIC_ALIGN_MASK		  0xFFFFFFFC

// Constants
#define GIC_PRIORITY_ENABLE_ALL	  0xFFFF
#define GIC_CPU_TARGET_CPU0	  0x01
#define GIC_CONTROL_ENABLE	  0x1

// Function prototypes
void hps_timer_ISR(void);
void __attribute__((interrupt)) __cs3_isr_irq(void);
void __attribute__((interrupt)) __cs3_reset(void);
void __attribute__((interrupt)) __cs3_isr_undef(void);
void __attribute__((interrupt)) __cs3_isr_swi(void);
void __attribute__((interrupt)) __cs3_isr_pabort(void);
void __attribute__((interrupt)) __cs3_isr_dabort(void);
void __attribute__((interrupt)) __cs3_isr_fiq(void);

void set_A9_IRQ_stack(void);
void enable_A9_interrupts(void);
void disable_A9_interrupts(void);
void config_interrupt(int N, int CPU_target);
void config_GIC(void);

#endif // EXCEPTIONS_H
