#ifndef _RTC_H
#define _RTC_H

//#ifndef ASM

#include "types.h"

#define 	RTC_REG_A		0x0A
#define		RTC_REG_B		0x0B
#define 	RTC_REG_C 		0x0C

#define 	RTC_PORT		0x70
#define 	CMOS_PORT 		0x71
#define 	INTERRUPT_REQUEST_8		8

#define 	RTC_IDT_PORT	0x28

#define freq_2		2
#define freq_4		4
#define freq_8		8
#define freq_16		16
#define freq_32		32
#define freq_64		64
#define freq_128	128
#define freq_256	256
#define freq_512	512
#define freq_1024	1024

#define rate_15		15
#define rate_14		14
#define rate_13		13
#define rate_12		12
#define rate_11		11
#define rate_10		10
#define rate_9		9
#define rate_8		8
#define rate_7		7
#define rate_6		6





volatile int32_t rtc_interrupt_flag;
void rtc_initialization();
void rtc_handler();

int32_t rtc_open(const uint8_t* filename);
int32_t rtc_read(int32_t fd, void *buf, int32_t nbytes);
int32_t rtc_write(int32_t fd, int32_t* buf, int32_t nbytes);
int32_t rtc_rate_helper(int32_t frequency);
int32_t rtc_close(int32_t fd);



#endif
//#endif
