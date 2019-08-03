#ifndef _IDT_H
#define _IDT_H

#include "types.h"

#define HALT_N 0x1
#define EXE_N 0x2
#define READ_N 0x3
#define WRITE_N 0x4
#define OPEN_N 0x5
#define CLOSE_N 0x6
#define GETARGS_N 0x7
#define VIDMAP_N 0x8
#define SET_HANDLER_N 0x9
#define SIGRETURN_N 0xA

//set up the interupt data table
extern void set_idt();
//Divide Error Exception
extern void handler_divide_error ();
//Debug Exception
extern void handler_reserved ();
//NMI Interrupt
extern void handler_nmi ();
//Breakpoint Exception
extern void handler_breakpoint  ();
//Overflow Exception
extern void handler_overflow ();
//BOUND Range Exceeded Exception
extern void handler_bound ();
//Invalid Opcode Exception
extern void handler_inval_oc ();
//Device Not Available Exception
extern void handler_device_na ();
//Double Fault Exception
extern void handler_double_fault ();
//Coprocessor Segment Overrun
extern void handler_segment_overrun ();
//Invalid TSS Exception
extern void handler_inval_tss ();
//Segment Not Present
extern void handler_segment_not_present ();
//Stack Fault Exception
extern void handler_stack_segment_fault ();
//General Protection Exception
extern void handler_general_protection ();
//Page-Fault Exception
extern void handler_page_fault ();
//intel reserved
extern void handler_intel_reserved ();
//FPU Floating-Point Error
extern void handler_floating_point_error ();
//Alignment Check Exception
extern void handler_alignment_check ();
//Machine-Check Exception
extern void handler_machine_check ();
//SIMD Floating-Point Exception
extern void handler_floating_point_execption ();


#endif /* IDT_H */
