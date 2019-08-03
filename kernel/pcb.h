#ifndef _PCB_H
#define _PCB_H

#include "types.h"
#include "paging.h" //for KBYTE 



typedef struct file_ops_table
{
	int32_t (*read) (int32_t fd, void* buf, int32_t nbytes);
	int32_t (*write) (int32_t fd, const void* buf, int32_t nbytes);
	int32_t (*open) (const uint8_t* filename);
	int32_t (*close) (int32_t fd);
} file_ops_table_t;

typedef struct file_desc
{
	file_ops_table_t * fot;
	uint32_t inode;
	uint32_t file_pos;
	uint32_t flags;
} file_desc_t;

typedef struct pcb
{
	file_desc_t fd[8];
	int32_t idx;
	struct pcb_t* parent;
	struct pcb_t* true_parent;
	int32_t ebx;
    int32_t ecx;
    int32_t edx;
    int32_t edi;
    int32_t esi;
    int32_t ebp;
	int32_t esp;
    int8_t fs;
    int8_t gs;
    int8_t cs;
    int8_t ss;
    int8_t ds;
    int8_t es;
    int32_t program_counter;
	int8_t pcb_buf[KBYTE];
}pcb_t;
extern pcb_t* pcb_ptr;

void pcb_init (uint32_t esp);

#endif /* PCB_H */
