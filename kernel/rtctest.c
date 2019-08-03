#include "rtc.h"
#include "i8259.h"
#include "lib.h"
#include "terminal.h"
#include "idt.h"
#include "paging.h"
#include "x86_desc.h"

/* test function for rtc_open */
void rtc_open_test()
{
	rtc_open(0);
}
/* test function for rtc_close */
void rtc_close_test()
{
	rtc_close(0);
	disable_irq(2);
}
/* test function for rtc_write, frequency is chosen here */
void rtc_write_test()
{
	int32_t *rtc_buf;
	int32_t freq = 16;

	rtc_buf = &freq;
	rtc_write(0, rtc_buf, 0); //rtc_write is called with the frequency in rtc_buf

}
/* test function for rtc_read
will print returned from read after rtc_read returns*/
void rtc_read_test()
{

		set_idt();
		paging_init();
		i8259_init();
		lidt(idt_desc_ptr);

		sti();


		rtc_open(0);
		rtc_read(0,0,0);

		terminal_write(0, "read", 4);
}
