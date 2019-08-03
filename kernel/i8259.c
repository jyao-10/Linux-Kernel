/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts
 * are enabled and disabled */
uint8_t master_mask = 0xFF; /* IRQs 0-7 */
uint8_t slave_mask = 0xFF; /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void
i8259_init(void)
{
	// save masks
	master_mask = inb(PICM_DATA);
	slave_mask = inb(PICS_DATA);

	// send init command to master and slave
	outb(ICW1, PICM_COMMAND);
	outb(ICW1, PICS_COMMAND);

	// send vector offset
	outb(ICW2_MASTER, PICM_DATA);
	outb(ICW2_SLAVE, PICS_DATA);

	// send master/slave wiring
	outb(ICW3_MASTER, PICM_DATA);
	outb(ICW3_SLAVE, PICS_DATA);

	// send additional env information
	outb(ICW4, PICM_DATA);
	outb(ICW4, PICS_DATA);

	// restore masks
	outb(master_mask, PICM_DATA);
	outb(slave_mask, PICS_DATA);
	uint32_t i;
	for (i = 0; i < 16; ++i)
	{
		disable_irq(i);
	}

	enable_irq(2);
}

/* Enable (unmask) the specified IRQ */
void
enable_irq(uint32_t irq_num)
{
	uint16_t port;
	uint8_t value;
	// check to see if irq_num is in master pic
	// or slave pic
	if (irq_num < 8){
		port = PICM_DATA;
	} else {
		port = PICS_DATA;
		irq_num -= 8;
	}
	// fetch current mask and OR the irq_num
	value = inb(port) & ~(1 << irq_num);
	// send new mask
	outb(value, port);
}

/* Disable (mask) the specified IRQ */
void
disable_irq(uint32_t irq_num)
{
	uint16_t port;
	uint8_t value;
	// check to see if irq_num is in master pic
	// or slave pic
	if (irq_num < 8){
		port = PICM_DATA;
	} else {
		port = PICS_DATA;
		irq_num -= 8;
	}
	// fetch current mask and AND the NOT irq_num
	value = inb(port) | (1 << irq_num);
	// send new mask
	outb(value, port);
}

/* Send end-of-interrupt signal for the specified IRQ */
void
send_eoi(uint32_t irq_num)
{
	if (irq_num >= 8)
		outb((irq_num | EOI)-8, PICS_COMMAND);

	outb(irq_num | EOI, PICM_COMMAND);
}



void rtc_send_eoi(uint32_t irq_num)
{
	if (irq_num >= 8){
		outb((irq_num | EOI)-8, PICS_COMMAND);
		outb(ICW3_SLAVE|EOI, PICM_COMMAND);
	}
	else
	{
	outb(irq_num | EOI, PICM_COMMAND);
	}
}
