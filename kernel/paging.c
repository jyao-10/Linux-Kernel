#include "paging.h"

uint32_t page_dir[KBYTE] __attribute__((aligned (4*KBYTE)));
uint32_t page_table[KBYTE] __attribute__((aligned(4*KBYTE)));

void paging_init(void){
  int32_t i;
  for (i = 0; i < KBYTE; i++){
    set_page_dentry(&(page_dir[i]), 0, 0, 0);
    set_page_dentry(&(page_table[i]), 0, 0, 0);
  }
  // set first page directory entry to table
  set_page_dentry(&(page_dir[0]), (uint32_t)&(page_table[0]), SM_MASK, \
    FLAG_P | FLAG_RW | FLAG_G);

  // set second page directory entry for kernel
  set_page_dentry(&(page_dir[1]), 4*MBYTE, LG_MASK, \
    FLAG_P | FLAG_RW | FLAG_LG | FLAG_G);

  set_page_dentry(&(page_table[VIDEO_PAGE]), VIDEO_ADDR, SM_MASK, \
    FLAG_P | FLAG_RW | FLAG_G);

	asm volatile("                     \n\
		movl %0, %%cr3                 \n\
		movl %%cr4, %%eax              \n\
		movl $0x00000010, %%eax        \n\
		movl %%eax, %%cr4              \n\
		movl %%cr0, %%eax              \n\
		movl $0x80000011, %%eax        \n\
		movl %%eax, %%cr0              \n\
		"
		: /* no outputs */
		:"r"(page_dir)
		: "memory"
	);
  return;
}

void set_page_dentry(uint32_t* page, uint32_t addr, uint32_t mask, uint32_t flags){
  *page = (addr & mask) | flags;
  return;
}

uint32_t set_program_page(uint32_t prog_num){
  uint32_t old_pn = (page_dir[PROGRAM_PAGE] >> 22) - 2;

  set_page_dentry(&(page_dir[PROGRAM_PAGE]), \
    4*MBYTE*prog_num + 8*MBYTE, \
    LG_MASK, \
    FLAG_P | FLAG_RW | FLAG_US | FLAG_LG);

  printf("\n  New Pn: %d;  Old Pn: %d;  addr: %x\n", prog_num, old_pn, page_dir[PROGRAM_PAGE]);
  asm volatile("\n\
    movl %0, %%cr3"
    : /* no outputs */
    :"r" (page_dir)
    :"memory"
  );
  return old_pn;
}
