#ifndef _PAGING_H
#define _PAGING_H

#include "types.h"

// cr3 settings
#define PAGING_BIT (1 << 31) // sets paging in cr0
#define PSE_BIT (1 << 4) // sets page size extention

// page directory entry
#define SM_MASK 0xFFFFF000 // 4kb page_dir addr
#define LG_MASK 0xFFC00000 // 4mb page_dir addr

#define FLAG_P (1 << 0) // is the page usable?
#define FLAG_RW (1 << 1) // can I write to this page?
#define FLAG_US (1 << 2) // can a user access this?
#define FLAG_PWT (1 << 3) // write-through caching?
#define FLAG_PCD (1 << 4) // can I cache this?
#define FLAG_A (1 << 5) // have I read from this page?
#define FLAG_D (1 << 6) // have I written to this page?
#define FLAG_LG (1 << 7) // is this page large (4mb)?
#define FLAG_G (1 << 8) // prevent flushing?

// page table entry
#define FLAG_PAT (1 << 7) // selects attribute table index

#define KBYTE (1 << 10)
#define MBYTE (1 << 20)

#define VIDEO_ADDR 0xB8000
#define VIDEO_PAGE 0xB8

#define PROGRAM_PAGE 32 // 0x08000000 >> 22


void paging_init(void);
void set_page_dentry(uint32_t* page, uint32_t addr, uint32_t mask, uint32_t flags);
uint32_t set_program_page(uint32_t prog_num);

#endif /* _PAGING_H */
