#include "pcb.h"
#include "terminal.h"
#include "rtc.h"
#include "filesys.h"

file_ops_table_t fot_stdin;
file_ops_table_t fot_stdout;
file_ops_table_t fot_rtc;
file_ops_table_t fot_file_sys;
pcb_t* pcb_ptr;

/* void ppcb_init ()
Inputs: the value of the esp register  
Outputs: None
sets the pcb structure
*/
void pcb_init (uint32_t esp){
	uint32_t offset = esp & 0xFFFFE000;
	pcb_ptr = (pcb_t*)offset;
	pcb_ptr->idx = 2;
	fot_stdin.read = &terminal_read;
	fot_stdout.write = &terminal_write;
	fot_rtc.read = &rtc_read;
	fot_rtc.write = &rtc_write;
	fot_rtc.open = &rtc_open;
	fot_rtc.close = &rtc_close;
	fot_file_sys.read = &fs_read;
	fot_file_sys.write = &fs_write;
	fot_file_sys.open = &fs_open;
	fot_file_sys.close = &fs_close;
	pcb_ptr->fd[0].fot = &fot_stdin;
	pcb_ptr->fd[1].fot = &fot_stdout;
	pcb_ptr->true_parent = (pcb_t*)0x7fc000;
	int32_t i;
  	for(i = 2; i < 8; i++){
    	pcb_ptr->fd[i].flags = 0;
    	pcb_ptr->fd[i].file_pos = 0;
  	}
	return;
}
