// copy format of i8259 for init, etc
// then run init in kernel.c line 152
#include "keyboard.h"
#include "i8259.h"
#include "lib.h"
#include "types.h"
#include "terminal.h"
#include "rtc.h"
#include "rtctest.h"

uint32_t caps = 0;
uint32_t caps_l = 0;
uint32_t ctrl = 0;
/* Scan code table*/
char scan_code_set2[2][128] =
{
   {0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', /* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', '`',
  0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
  0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,  0,   0,   0,   0,   0,   0,   0,   0,  0,	/* F1 key ... F10 key */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
	},
	{0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', /* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',	/* Enter key */
    0,			/* Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':','\"', '~',
  0,		/* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '\?',
  0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,  0,   0,   0,   0,   0,   0,   0,   0,  0,	/* F1 key ... F10 key */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
	}
};

/*keyboard_init(void)
*Inputs: None
*Outputs: None
*initializes the keyboard by enabling the appropriate irq on the PIC
*/
void
keyboard_init(void)
{
	enable_irq(KBD_IRQ);
}

/*kbd_handler()
*Inputs: None
*Outputs: None
*keyboard handler that gets called when the keyboar generates an interrupt.
*prints the key pressed to the screen
*/
void
kbd_handler(){
	//uint32_t flags;
	cli();//_and_save(flags);

	uint32_t scan_code = inb(KBD_DATA_PORT);
	if(scan_code == CAPS_LOCK)
		caps_l = (caps_l+1)%2;
	if((scan_code == SHIFT_R_PRESSED) || (scan_code == SHIFT_L_PRESSED))
		 caps = 1;
	if((scan_code == SHIFT_R_RELEASED) || (scan_code == SHIFT_L_RELEASED))
		 caps = 0;
	if((scan_code == CTRL_R_PRESSED) || (scan_code == CTRL_L_PRESSED))
		 ctrl = 1;
	if((scan_code == CTRL_R_RELEASED) || (scan_code == CTRL_L_RELEASED))
		 ctrl = 0;
	char c;
	if(((scan_code >= Q_PRESSED)&&(scan_code <= P_PRESSED))||((scan_code >= A_PRESSED)&&(scan_code <= L_PRESSED))||((scan_code >= Z_PRESSED)&&(scan_code <= M_PRESSED))){
		c = scan_code_set2[caps ^ caps_l][scan_code];
	}
	else{
		c = scan_code_set2[caps][scan_code];
	}
	//char s[2];
	//s[0] = c;
	//s[1] = '\0';
	if(ctrl && (scan_code < 0x80)){
		switch(scan_code){
			case L_PRESSED:
				terminal_clear();
				break;
			case ONE_PRESSED:
				rtc_open_test();
				break;
			case TWO_PRESSED:
				rtc_write_test();
				break;
			case THREE_PRESSED:
				rtc_close_test();
				break;
			case FOUR_PRESSED:
				rtc_read_test();
				break;
			case FIVE_PRESSED:
				test_ls();
				break;
			case SIX_PRESSED:
				test_index();
				break;
			case SEVEN_PRESSED:
				test_name();
				break;

		}

	}
	else if((c == '\b') && (scan_code < 0x80))
		terminal_del();
	else if((scan_code < 0x80) && (c != 0))
		terminal_type(c);
		//printf("%c",c);


	//restore_flags(flags);
	sti();
	send_eoi(KBD_IRQ);

}
