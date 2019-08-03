#include "rtc.h"
#include "i8259.h"
#include "lib.h"
#include "terminal.h"

#define     bitmask1        0x44
#define     bitmask2        0xF0
#define 	rate_bitmask	0x0F
#define 	NMI_INTERRUPT	0x02
#define 	INITIAL_RATE	15
#define 	FREQUENCY_MIN 	2
#define 	FREQUENCY_MAX 1024
//volatile int32_t rtc_interrupt_flag;

int32_t rate;

/*
void rtc_initialization()
Inputs: None
Outputs: None
This function initializes the RTC. x70 and x71 are the 2 ports used for RTC and CMOS.
0x70 is used to specify an index and to disable NMI, and 0x71 is used to read from/to that
byte of CMOS configuration space. The periodic interrupt is turned on and the RTC interrupt
rate is set to 2 hz.

*/




void rtc_initialization()
{

	//frequency = 1000 0000 0000 0000 >> (rate - 1)
	int init_rate = INITIAL_RATE; //15 for 2 hz
	rate = INITIAL_RATE;

	char prev, prev2;
	/*  turn on periodic interrupt */
	outb(RTC_REG_B, RTC_PORT);	// NMI is disabled and register B is selected
	prev = inb(CMOS_PORT);		//Value of register B is read
	outb(RTC_REG_B, RTC_PORT);	//Index is set again
	outb((prev | bitmask1), CMOS_PORT); //0x40 for BCD, 0x44 for binary

	rtc_interrupt_flag = 1;
	/* Setting interrupt rate */
	outb(RTC_REG_A, RTC_PORT); 	//Disable NMI and set index to register A
	prev2 = inb(CMOS_PORT);		//Get the value of register A
	outb(RTC_REG_A, RTC_PORT);	//Reset index for register A
	outb( ((prev2 & bitmask2) | init_rate) , CMOS_PORT); 	//Write rate to register A

	enable_irq(NMI_INTERRUPT);
	enable_irq(INTERRUPT_REQUEST_8);	//Enable IRQ8



}


/* void rtc_handler()
Inputs: None
Outputs: None
This function is the RTC handler for when an RTC interrupt occurs. Register C is read
in this function. If Regsiter C is not read after an IRQ8, then the interrupt will not
happen again. test_interrupts() is ran in this function.
*/

void rtc_handler()
{
	//cli();
	/* if register C is not read not after an IRQ8, then interrupt will not happen again */
	outb(RTC_REG_C, RTC_PORT); 	//select register C
	inb(CMOS_PORT); 	//throw away contents

	rtc_interrupt_flag = 1;

	rtc_send_eoi(INTERRUPT_REQUEST_8); 	//end of interrupt

	//test_interrupts();
	// terminal_write(0, "1", 1); //terminal write 1 for tests

	sti();
	return;
}

/* rtc_open function
Inputs: uint8_t
Outputs: return 0
This function initializes the RTC and returns 0
*/

int32_t rtc_open (const uint8_t* filename)
{

	rtc_initialization();
	return 0;


}


/*
rtc_read (int32_t fd, void *buf, int32_t nbytes)
This function should always return 0, but only after an interrupt occurs.
A flag is set and should wait until the interrupt handler clears it, and then return 0
*/

int32_t rtc_read (int32_t fd, void *buf, int32_t nbytes)
{

	/*Waits until rtc_handler sets rtc_interrupt_flag*/
	while (rtc_interrupt_flag == 0)
	{
	}

	rtc_interrupt_flag = 0; //clears rtc_interrupt_flag

	return 0;

}


/*
int32_t rtc_write (int32_t fd, const void* buf, int32_t nbytes)
Inputs: int32_t fd, const void* buf, int32_t nbytes
Outputs: returns 0, returns -1 on failure
Accepts a 4 byte integer interrupt rate in Hz, rtc_write sets the periodic rate accordingly.
The interrupt rate should be limited to 1024 hz
*/
//int32_t rtc_write (int32_t fd, const void* buf, int32_t nbytes)
int32_t rtc_write (int32_t fd, int32_t* buf, int32_t nbytes)
{
	int32_t frequency = *buf;

	/* Check the range of the frequency */
	if (frequency < FREQUENCY_MIN || frequency > FREQUENCY_MAX)
	{
		return -1;
	}

	rate = rtc_rate_helper(frequency); //rtc_rate_helper gets the rate to set interrupt rate

	//printf("rate is %d\n", rate);

	rate &= rate_bitmask;

	cli();
	/* Sets interrupt rate */
	outb(RTC_REG_A, RTC_PORT);
	char prev = inb(CMOS_PORT);
	outb(RTC_REG_A, RTC_PORT);
	outb( (prev & bitmask2) | rate, CMOS_PORT);

	sti();

	return 0;

}

/*
int32_t rtc_rate_helper(int32_t frequency)
Inputs: frequency in hz
Outputs: returns rate
This helper functions takes in a int32_t frequency value in hz and returns the rate
*/
int32_t rtc_rate_helper(int32_t frequency)
{
	/*
	int32_t i;


	for (i = 6; i <16; i++)
	{
		if (frequency == (32768 >> (i-1)) )
		{
			return i;
		}
	}
	*/

	if (frequency == freq_2)
	{
		rate = rate_15;
	}

	if (frequency == freq_4)
	{
		rate = rate_14;
	}

	if (frequency == freq_8)
	{
		rate = rate_13;
	}

	if (frequency == freq_16)
	{
		rate = 	rate_12;
	}
	if (frequency == freq_32)
	{
		rate = 	rate_11;
	}

	if (frequency == freq_64)
	{
		rate = 	rate_10;
	}

	if (frequency == freq_128)
	{
		rate = 	rate_9;
	}

	if (frequency == freq_256)
	{
		rate = rate_8;
	}

	if (frequency == freq_512)
	{
		rate = rate_7;
	}

	if (frequency == freq_1024)
	{
		rate = rate_6;
	}

	return rate;
}



/*
int32_t rtc_close (int32_t fd)
Inputs: int32_t fd
Outputs: returns 0
*/
int32_t rtc_close (int32_t fd)
{
	return 0;
}
