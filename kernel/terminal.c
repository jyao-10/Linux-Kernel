#include "terminal.h"
#include "lib.h"



terminal_buff_t tb;
extern  int screen_x;
extern  int screen_y;


/*terminal_init(void)
*Inputs: None
*Outputs: None
*initializes the terminal sets the initial values of terminal buffer
*/
void
terminal_init(){
	tb.buff[0] = '\n';
	tb.start = &tb.buff[0];
	tb.end = &tb.buff[BUFF_LEN];
	tb.begin = &tb.buff[0];
	tb.curr = &tb.buff[1];
	tb.num = 0;
}

/*terminal_type(char * input)
*Inputs: char buffer ending in \0
*Outputs: int 0 if success -1 if failure
*places typed characters on terminal buffer displays in on the terminal
*/
int
terminal_type(char input){
		if(input == '\n'){
			*tb.curr = input;
			tb.num++;
		}
		if ((tb.begin == tb.curr)){
			return -1;
		}
		putc(input);
		*(tb.curr) = input;

		if(tb.curr == tb.end){
			tb.curr = tb.start;
		}
		else{
			tb.curr += 1;
		}
	update_cursor();
 	return 0;
}

/*terminal_del()
*Inputs: None
*Outputs: None
*deletes a charcter on the terminal buffer and displays the result on the terminal
*/
void
terminal_del(){
	uint8_t * prev;
	if(tb.curr == tb.start)
		prev = tb.end;
	else
		prev = tb.curr -1;
	if(*prev != '\n'){
		tb.curr = prev;
		*tb.curr = ' ';
		if(screen_x == 0){
			screen_y = (screen_y-1)%NUM_ROWS;
			screen_x = NUM_COLS - 1;
		}
		else{
			screen_x = (screen_x-1)%NUM_COLS;
		}

		putc(' ');
		if(screen_x == 0){
			screen_y = (screen_y-1)%NUM_ROWS;
			screen_x = NUM_COLS - 1;
		}
		else{
			screen_x = (screen_x-1)%NUM_COLS;
		}
	}
	update_cursor();
}


/*terminal_read(int32_t fd, void* buf, int32_t nbytes)
*Inputs: 32 bit int, an empty buffer and the number of bytes in the buffer
*Outputs: the number of bytes filled on the buffer
*places one command from the terminal to the buffer provided
*/
int32_t
terminal_read(int32_t fd, void* buf, int32_t nbytes){
	//sti();
	while(tb.num == 0){}
	int32_t len = nbytes;
	//char * next;
	uint8_t * cbuf;
	cbuf = buf;
	if(tb.begin == tb.end){
		tb.begin = tb.start;
	}
	else{
		tb.begin  = tb.begin+1;
	}
	while (*(tb.begin) != '\n'){
		if(len == 0)
			return nbytes;
		*cbuf = *(tb.begin);
		len = len - 1;

		cbuf++;

		if(tb.begin == tb.end){
			tb.begin = tb.start;
		}
		else{
			tb.begin = tb.begin + 1;
		}
	}
	//tb.begin = next;
	if (tb.begin == tb.curr){
		if(tb.curr == tb.end){
			tb.curr = tb.start;
		}
		else{
			tb.curr = tb.curr + 1;
		}
		//tb.curr = next;
	}
	tb.num--;
	return nbytes-len;
}

/*terminal_write(int32_t fd, const void* buf, int32_t nbytes)
*Inputs: 32 bit int, an empty buffer and the number of bytes in the buffer
*Outputs: int32_t 0 if success -1 if failure
*displays the contents of the buffer onto the terminal
*/
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes){
	int i = 0;
	while(nbytes > 0){
		putc(*((char*)buf+i));
		i++;
		nbytes--;
	}
	update_cursor();
	return 0;
}

/*terminal_clear()
*Inputs: None
*Outputs: None
*clears the terminal and reprints all unexecuted commands
*/
void
terminal_clear(){
	clear();
	screen_x = 0;
	screen_y = 0;
	uint8_t *c;
	if(tb.begin == tb.end){
		c = tb.start;
	}
	else{
		c  = tb.begin+1;
	}
	do{
		putc(*c);
		if(c == tb.end){
			c = tb.start;
		}
		else{
			c++;
		}
	}while(c != tb.curr);
	update_cursor();
}

/*uptade_cursor()
*Inputs: None
*Outputs: None
*updates the position of the cursor on the terminal
*/
void
update_cursor(){
	int cursor = screen_x+(screen_y*NUM_COLS);
	outw (0x0E, 0x03D4);
	outw ((uint16_t)((cursor>>8) & 0xFF), 0x03D5);
	outw (0x0F, 0x03D4);
	outw ((uint16_t)(cursor & 0xFF), 0x03D5);
}
