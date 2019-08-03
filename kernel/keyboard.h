// copy format of i8259 for init, etc
// then run init in kernel.c line 152
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"
#include "filesys.h"

#define KBD_IDT_PORT 0x21
#define KBD_DATA_PORT 0x60
#define KBD_IRQ 1
#define SHIFT_R_PRESSED 0x36
#define SHIFT_R_RELEASED 0xB6
#define SHIFT_L_PRESSED 0x2A
#define SHIFT_L_RELEASED 0xAA
#define CAPS_LOCK 0x3A
#define CTRL_R_PRESSED 0x1D
#define CTRL_R_RELEASED 0x9D
#define CTRL_L_PRESSED 0x1D
#define CTRL_L_RELEASED 0x9D
#define Q_PRESSED 0x10
#define P_PRESSED 0x19
#define A_PRESSED 0x1E
#define L_PRESSED 0x26
#define Z_PRESSED 0x2C
#define M_PRESSED 0x32
#define ONE_PRESSED 0x02
#define TWO_PRESSED 0x03
#define THREE_PRESSED 0x04
#define FOUR_PRESSED 0x05
#define FIVE_PRESSED 0x06
#define SIX_PRESSED 0x07
#define SEVEN_PRESSED 0x08

void keyboard_init(void);
void kbd_handler();
#endif /* _KEYBOARD_H */
