#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "types.h"
#include "filesys.h"
#include "paging.h" // for KBYTE define
#include "x86_desc.h"
#include "pcb.h"

#define EXCEPTION 256
#define HEADER_SIZE 52 // in Bytes
#define USER_STACK 0x083FFFFC
#define NUM_PROGRAMS 3
#define	KERNEL_STACK 0x800000
#define PROG_VIRTUAL_ADDR 0x8000000
#define MAGIC_ELF 0x464c457f

int32_t sys_execute (const uint8_t* command);
int32_t sys_halt (uint8_t status);
int32_t sys_execute (const uint8_t* command);
int32_t sys_read (int32_t fd, void* buf, int32_t nbytes);
int32_t sys_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t sys_open (const uint8_t* filename);
int32_t sys_close (int32_t fd);
int32_t sys_getargs (uint8_t* buf, int32_t nbytes);
int32_t sys_vidmap (uint8_t** screen_start);
int32_t sys_set_handler (int32_t signum, void* handler_address);
int32_t sys_sigreturn (void);


void pm_init(void);
int32_t pushpm(int32_t push);
int32_t poppm(void);
uint32_t set_tss_esp(uint32_t addr, int32_t process_num);
#endif /* _SYSCALL_H */
