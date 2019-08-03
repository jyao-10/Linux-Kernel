#define ASM      1
#include "linker.h"

.text

.globl system_calls
.globl sys_halt
.globl sys_read
.globl sys_write
.globl sys_open
.globl sys_close
.globl sys_execute
.globl sys_set_handler
.globl sys_sigreturn

//still need to #define for entries in jump table
system_call_jmp_table:
	.long   skip0, sys_halt, sys_execute, sys_read, sys_write, sys_open, sys_close, sys_getargs, sys_vidmap//, sys_set_handler, sys_sigreturn
	/*.long    skip0, halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn*/
system_calls:
  pushl %edi
	pushl %esi
	pushl %ebp
	pushl %ebx
	pushl %edx
	pushl %ecx
  pushw %fs
  pushw %gs
  pushw %ss
  pushw %ds
  pushw %es

  cmpl    $-1, %eax
  je      CALL_ERROR
  cmpl    $1, %eax
  jl      CALL_ERROR
  cmpl    $10, %eax
  jg      CALL_ERROR

	pushl %edx
	pushl %ecx
	pushl %ebx
	call *system_call_jmp_table(, %eax, 4)
	addl $12, %esp

  jmp     pop_regs

CALL_ERROR:
  movl    $-1, %eax

pop_regs:

	popw %es
	popw %ds
	popw %ss
	popw %gs
	popw %fs
	popl %ecx
	popl %edx
	popl %ebx
	popl %ebp
	popl %esi
	popl %edi
  iret

skip0:
	jmp CALL_ERROR
	ret
