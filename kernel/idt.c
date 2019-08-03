#include "multiboot.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "keyboard.h"
#include "rtc.h"
#include "debug.h"
#include "idt.h"
#include "linker.h"

#define  KEYBOARD_ADDR  33
#define  RTC_ADDR       40
#define  SYS_CALL       128
#define  USER_DEFINED   32


/* void set_idt ()
Inputs: None
Outputs: None
sets the idt structure and calls assembly wrapper
*/
void set_idt(){



	int i;
	for (i = 0; i < NUM_VEC; i++)
	{
		idt[i].seg_selector = KERNEL_CS;
		idt[i].size = 1;
		idt[i].reserved4 = 0;
		idt[i].reserved2 = 1;
		idt[i].reserved1 = 1;
		idt[i].reserved0 = 0;
		if(i<USER_DEFINED){
			idt[i].dpl = 0;
			idt[i].present = 1;
			idt[i].reserved3 = 0;

		}
		else{
			idt[i].dpl = 3;
			if((i == KEYBOARD_ADDR) |(i==RTC_ADDR) |(i==SYS_CALL)){
				idt[i].present = 1;
			}
			else{
				idt[i].present = 0;
			}
			idt[i].reserved3 = 1;
			if ((i==KEYBOARD_ADDR)||(i==RTC_ADDR)){
				idt[i].reserved3 = 0;
			}

		}
	}
	SET_IDT_ENTRY(idt[0],handler_divide_error);
	SET_IDT_ENTRY(idt[1],handler_reserved);
	SET_IDT_ENTRY(idt[2],handler_nmi);
	SET_IDT_ENTRY(idt[3],handler_breakpoint);
	SET_IDT_ENTRY(idt[4],handler_overflow);
	SET_IDT_ENTRY(idt[5],handler_bound);
	SET_IDT_ENTRY(idt[6],handler_inval_oc);
	SET_IDT_ENTRY(idt[7],handler_device_na);
	SET_IDT_ENTRY(idt[8],handler_double_fault);
	SET_IDT_ENTRY(idt[9],handler_segment_overrun);
	SET_IDT_ENTRY(idt[10],handler_inval_tss);
	SET_IDT_ENTRY(idt[11],handler_segment_not_present);
	SET_IDT_ENTRY(idt[12],handler_stack_segment_fault);
	SET_IDT_ENTRY(idt[13],handler_general_protection);
	SET_IDT_ENTRY(idt[14],handler_page_fault);
	SET_IDT_ENTRY(idt[15],handler_intel_reserved);
	SET_IDT_ENTRY(idt[16],handler_floating_point_error);
	SET_IDT_ENTRY(idt[17],handler_alignment_check);
	SET_IDT_ENTRY(idt[18],handler_machine_check);
	SET_IDT_ENTRY(idt[19],handler_floating_point_execption);
	for (i = 20; i < 32; i++){
		SET_IDT_ENTRY(idt[i],handler_intel_reserved);
	}
	SET_IDT_ENTRY(idt[SYS_CALL], system_calls);
	SET_IDT_ENTRY(idt[RTC_IDT_PORT], rtc_handler);
	SET_IDT_ENTRY(idt[KBD_IDT_PORT], kbd_handler);

}

/* void handler_divide_error ()
Inputs: None
Outputs: None
Divide Error Exception - Interrupt 0. When the divisor operand is 0 or the
result cannot be represented in the number of bits for the destination
*/

void handler_divide_error (){
	printf("\nDivide Error\n");
	cli();
	while(1){}

}

/*void handler_reserved ()
Inputs: None
Outputs: None
This function indicates that one more more of several debug exception conditions has
been detected.
*/

void handler_reserved (){
	printf("\nReserved\n");
	cli();
	while(1){}
}

/*
void handler_nmi ()
Inputs: None
outputs: None
NMI interrupt. NMI is generated externally by asserting the processor's NMI pin or through
an NMI request set by I/O APIC to the local APIC.
*/

void handler_nmi (){
	printf("\nNMI Interrupt\n");
	cli();
	while(1){}
}
/*
void handler_breakpoint ()
Inputs: None
Outputs: None
Breakpoint Exception. Indicates that a breakpoint instruction was executed, which
causes a breakpoint trap to be generated.
*/

void handler_breakpoint (){
	printf("\nBreakpoint\n");
	cli();
	while(1){}
}
/*void handler_overflow ()
Inputs: None
Outputs: None
When an overflow trap occurred when an INTO instruction was executed.
*/

void handler_overflow (){
	printf("\nOverflow\n");
	cli();
	while(1){}
}

/*void handler_bound ()
Inputs: None
Outputs: None
 If the array index is not within the bounds of the
array, a BOUND-range-exceeded fault is generated
*/
void handler_bound (){
	printf("\nBound Range Exceeded\n");
	cli();
	while(1){}
}

/*void handler_inval_oc ()
Inputs: None
Outputs: None
processor did one of the following:
 execute an invalid or reserved opcode
 execute an instruction with an operand type that is invalid for its accompanying opcode
 MMX or SSE/SSE2/SSE3 instruction on an IA-32 processor that does not support the MMX technology or SSE/SSE2/SSE3 extensions
 MMX instruction or SSE/SSE2/SSE3 SIMD instruction when the EM flag in control register CR0 is set (1)
 SSE/SE2/SSE3 instruction when the OSFXSR bit in controlregister CR4 is clear (0)
 an SSE/SSE2/SSE3 instruction on an IA-32 processor that causes a SIMD floating-point exception when the OSXMMEXCPT bit in control register CR4 is clear (0)
 Executed a UD2 instruction
 Detected a LOCK prefix that precedes an instruction that may not be locked
 execute an LLDT, SLDT, LTR, STR, LSL, LAR, VERR, VERW, or ARPL instruction while in real-address or virtual-8086 mode
*/
void handler_inval_oc (){
	printf("\nIvalid Opcode(undefined Opcode)\n");
	cli();
	while(1){}
}

/*void handler_device_na ()
Inputs: None
Outputs: None
Indicates one of the following things:
 executed an x87 FPU floating-point instruction while the EM flag in control register CR0 was set (1)
 executed a WAIT/FWAIT instruction while the MP and TS flags of register CR0 were set
 executed an x87 FPU, MMX, or SSE/SSE2/SSE3 instruction  while the TS flag in control register CR0 was set and the EM flag is clear
*/
void handler_device_na (){
	printf("\nDevice Not Available\n");
	cli();
	while(1){}
}

/*void handler_double_fault ()
Inputs: None
Outputs: None
processor detected a second exception while calling an exception handler for a prior exception
*/
void handler_double_fault (){
	printf("\nDouble Fault\n");
	cli();
	while(1){}
}

/*void handler_segment_overrun ()
Inputs: None
Outputs: None
 Intel 387 math coprocessor detected a page or segment violation
*/
void handler_segment_overrun (){
	printf("\nCoprocessor Segment Overrun(reserved)\n");
	cli();
	while(1){}
}

/*void handler_inval_tss ()
Inputs: None
Outputs: None
Indicates that there was an error related to a TSS
*/
void handler_inval_tss (){
	printf("\nInvalid TSS\n");
	cli();
	while(1){}
}

/*void handler_segment_not_present ()
Inputs: None
Outputs: None
Indicates that the present flag of a segment or gate descriptor is clear
*/
void handler_segment_not_present (){
	printf("\nSegment not present\n");
	cli();
	while(1){}
}

/*void handler_stack_segment_fault ()
Inputs: None
Outputs: None
the following stack related conditions was detected: When a limit violation
is detected during an operatoin that refers to the SS register, a not-present stack segment
is detected when trying to load the SS register.

*/
void handler_stack_segment_fault (){
	printf("\nStack-Segment Fault\n");
	cli();
	while(1){}
}

/*void handler_general_protection ()
Inputs: None
Outputs: None
Gneral Protection Exception indicates that the processor detected "general
protection violations". The violations compromise all protection violations that do not cause
other exceptions to be generated.
*/
void handler_general_protection (){
	printf("\nGeneral Protection\n");
	cli();
	while(1){}
}

/*void handler_page_fault ()
Inputs: None
Outputs: None
Indicates the processor detected the page-fault exception conditions when translating a linear address to
a physical address while using page-translation mechanism.
*/
void handler_page_fault (){
	uint32_t creg;
	asm volatile("\n\
		movl %%cr2, %0;"
		:"=r" (creg));
	printf("\nPage fault \n");
	printf("\nAddr: 0x%x", creg);
	cli();
	while(1){}
}

/*void handler_intel_reserved()
Inputs: None
Outputs: None
Vector number 15 is reserved by Intel, it should not be used.
*/
void handler_intel_reserved(){
	printf("\nIntel reserved. Do not use\n");
	cli();
	while(1){}
}

/*void handler_floating_point_error ()
Inputs: None
Outputs: None
Indicates the x87 FPU detected a floating-point error. NE flag in register CR0 must
be set for interrupt 16 to be generated.
*/
void handler_floating_point_error (){
	printf("\nx87 FPU Floating-Poit Error (Math Fault)\n");
	cli();
	while(1){}
}

/*void handler_alignment_check ()
Inputs: None
Outputs: None
Alignment Check Exception indicates the processor detected an unaligned
memory operand when alignment checking was enabled.
*/
void handler_alignment_check (){
	printf("\nAlignment Check\n");
	cli();
	while(1){}
}

/*void handler_machine_check ()
Inputs: None
Outputs: None
Indicates the processor detected a bus error or an internal machine error or
that an external agent detected a bus error.
*/
void handler_machine_check (){
	printf("\nMachine Check\n");
	cli();
	while(1){}
}

/*void handler_floating_point_execption ()
Inputs: None
Outputs: None
The processor has detected a SSE/SSE2/SSE3 SIMD floating-point exception. The MXCSR register
must be set for the right flags and the particular exception is unmasked.
*/
void handler_floating_point_execption (){
	printf("\nSIMD Floating-Point Exception\n");
	cli();
	while(1){}
}
