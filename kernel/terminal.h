#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"


#define BUFF_LEN 128
#define NUM_COLS 80
#define NUM_ROWS 25


//the terminal buffer structure
typedef struct terminal_buff{
	uint8_t buff [BUFF_LEN+1];
	uint8_t * start;
	uint8_t * end;
	uint8_t * begin;
	uint8_t * curr;
	volatile int num;
}terminal_buff_t;



//initializes terminal
void terminal_init();
//prints typed character to terminal
int32_t terminal_type(char input);
//delete character form terminal prints result
void terminal_del();
//clears terminal reprints unexecuted commands
void terminal_clear();
//places one command from the terminal buffer to the provided buffer
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);
//writes the buffer to the terminal
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);
//updates position of buffer
void update_cursor();
#endif /*_TERMINAL_H */
