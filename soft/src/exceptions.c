
/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : exceptions.c
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

#include "exceptions.h"
#include "timer_function.h"

// Define the IRQ exception handler
void __attribute__((interrupt)) __cs3_isr_irq(void)
{
	// Read CPU Interface registers to determine which peripheral has caused an interrupt
	int interrupt_ID = *((volatile int *)GIC_ICCIAR);

	// Handle the interrupt if it comes from the timer
	if (interrupt_ID == TIMER_IRQ_ID) {
		timer_interrupt_handler();
	}

	// Clear interrupt from the CPU Interface
	*((volatile int *)GIC_ICCEOIR) = interrupt_ID;

	return;
}

// Define the remaining exception handlers
void __attribute__((interrupt)) __cs3_reset(void)
{
	while (1)
		;
}

void __attribute__((interrupt)) __cs3_isr_undef(void)
{
	while (1)
		;
}

void __attribute__((interrupt)) __cs3_isr_swi(void)
{
	while (1)
		;
}

void __attribute__((interrupt)) __cs3_isr_pabort(void)
{
	while (1)
		;
}

void __attribute__((interrupt)) __cs3_isr_dabort(void)
{
	while (1)
		;
}

void __attribute__((interrupt)) __cs3_isr_fiq(void)
{
	while (1)
		;
}

/* Initialize the banked stack pointer register for IRQ mode */
void set_A9_IRQ_stack(void)
{
	uint32_t stack, mode;
	stack = A9_ONCHIP_END -
		7; // Top of A9 on-chip memory, aligned to 8 bytes
	mode = INT_DISABLE | IRQ_MODE;

	// Change processor to IRQ mode with interrupts disabled
	asm("msr cpsr, %[ps]" : : [ps] "r"(mode));

	// Set banked stack pointer
	asm("mov sp, %[ps]" : : [ps] "r"(stack));

	// Go back to SVC mode before executing subroutine return!
	mode = INT_DISABLE | SVC_MODE;
	asm("msr cpsr, %[ps]" : : [ps] "r"(mode));
}

/* Turn on interrupts in the ARM processor */
void enable_A9_interrupts(void)
{
	uint32_t status = SVC_MODE | INT_ENABLE;
	asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}

/* Turn off interrupts in the ARM processor */
void disable_A9_interrupts(void)
{
	int status = 0b11010011;
	asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}

/* Configure Set Enable Registers (ICDISERn) and Interrupt Processor Target Registers (ICDIPTRn) */
void config_interrupt(int N, int CPU_target)
{
	int reg_offset, index, value, address;

	// Configure the Interrupt Set-Enable Registers (ICDISERn)
	reg_offset = (N >> 3) & GIC_ALIGN_MASK;
	index = N & GIC_32_BIT_MASK;
	value = 0x1 << index;
	address = GIC_DIST_SET_ENABLE_ADDR + reg_offset;
	*((volatile int *)address) |= value;

	// Configure the Interrupt Processor Targets Register (ICDIPTRn)
	reg_offset = (N & GIC_ALIGN_MASK);
	index = N & GIC_BYTE_MASK;
	address = GIC_DIST_PROC_TARGET_ADDR + reg_offset + index;
	*((volatile char *)address) = (char)CPU_target;
}

/* Configure the Generic Interrupt Controller (GIC) */
void config_GIC(void)
{
	// Configure the FPGA KEYs interrupt (201)
	config_interrupt(TIMER_IRQ_ID, GIC_CPU_TARGET_CPU0);

	// Set Interrupt Priority Mask Register (ICCPMR) to enable all priorities
	*((volatile int *)GIC_CPU_PRIORITY_MASK_REG) = GIC_PRIORITY_ENABLE_ALL;

	// Set CPU Interface Control Register (ICCICR) to enable signaling of interrupts
	*((volatile int *)GIC_CPU_CONTROL_REG) = GIC_CONTROL_ENABLE;

	// Enable the Distributor Control Register (ICDDCR) to send pending interrupts to CPUs
	*((volatile int *)GIC_DIST_CONTROL_REG) = GIC_CONTROL_ENABLE;
}
