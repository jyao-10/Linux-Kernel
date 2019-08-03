#include "syscall.h"

int32_t process_stack[NUM_PROGRAMS];
int32_t stack_index, i;

extern file_ops_table_t fot_rtc;
extern file_ops_table_t fot_file_sys;
dentry_t open_dentry;

/*void pm_init(void)
Inputs: None
Outputs: None
initializes the porcess stack
*/
void pm_init(void){
  for (i = 0; i < NUM_PROGRAMS; i++){
    process_stack[i] = i;
  }
  stack_index = 0;
  return;
}

/*int32_t pushpm(int32_t push)
Inputs: process number to be pushed
Outputs: 0 if successful -1 if stack is full
pushes onto the porcess stack
*/
int32_t pushpm(int32_t push){
  if (stack_index <= 0 && stack_index > NUM_PROGRAMS){
    return ERR;
  }
  stack_index--;
  process_stack[stack_index] = push;
  return SUCCESS;
}

/*int32_t poppm(void)
Inputs: None
Outputs: 0 if successful -1 if stack is empty
pops from the porcess stack
*/
int32_t poppm(void){
  if (stack_index < NUM_PROGRAMS && stack_index >= 0){
    stack_index++;
    return process_stack[stack_index-1];
  }
  return ERR;
}

/*int32_t sys_halt(uint8_t status)
Inputs: uint8_t status
Outputs: return value to the parent program’s execute system call
implimented halt system call expanding the 8-bit argument from BL into the 32-bit return value
*/
int32_t sys_halt (uint8_t status){
  if (pcb_ptr->program_counter == 0){
    printf("0 process error\n");
    return ERR;
  }
  int32_t pc = pushpm(pcb_ptr->program_counter);
  // printf("%d,%d,%d\n",__LINE__, __func__, __FILE__);
  if(ERR==pc){
    return ERR;
  }
  // printf("%d,%d,%d\n",__LINE__, __func__, __FILE__);
  pcb_ptr = (pcb_t*)(pcb_ptr->parent);
  // printf("%d,%d,%d\n",__LINE__, __func__, __FILE__);
  pc = pcb_ptr->program_counter;
  // printf("%d,%d,%d\n",__LINE__, __func__, __FILE__);
  // set_dentry_lg(USER_PAGE, 8*MBYTE + 4*MBYTE*pc);
  printf("halt paging\n");
  set_program_page(pc);
  // printf("%d,%d,%d\n",__LINE__, __func__, __FILE__);
  set_tss_esp(KERNEL_STACK, pc);
  // printf("%d,%d,%d\n",__LINE__, __func__, __FILE__);
  asm volatile("\n\
    movl %0, %%esp \n\
    movl %1, %%ebp \n\
    movl %2, %%eax \n\
    leave \n\
    ret"
  : /* no outputs */
  : "r"(pcb_ptr->esp), "r"(pcb_ptr->ebp), "r"(status & 0xFF)
  :"esp", "ebp", "eax");
  // printf("%d,%d,%d\n",__LINE__, __func__, __FILE__);
  return (status & 0xFF);
}


/*int32_t sys_execute(const uint8_t* command)
Inputs: uint8_t * command - space seperated \0 terminated
Outputs: returns -1 if the command cannot be executed or a value in the range 0 to 255 if the program executes a halt system call
attempts to load and execute a new program, handing off the processor to the new program until it terminates
*/
int32_t sys_execute (const uint8_t* command){
  // printf("%s", command);
  uint8_t c[KBYTE];
  //uint8_t args[KBYTE];
  int8_t args[KBYTE]; //int8_t for strcpy arguments
  uint8_t* header;
  uint32_t* entry;
  uint32_t old_pn;
  pcb_t* parent = pcb_ptr;
  int32_t i = 0, j = 0, process = poppm();
  dentry_t d;
  printf("%d:\n", process);
  //printf("1\n");
  if (ERR == process){
    return ERR;
  }
  while(i < KBYTE && '\0' != command[i] && ' ' != command[i]){
    c[i] = command[i];
    i++;
  }
  //printf("2\n");
  if (command[i] == ' '){
	  i++;
  }
  //printf("3\n");
  while(i < KBYTE && j < KBYTE && '\0' != command[i]){
    args[j] = command[i];
    i++;
    j++;
  }
  //printf("4\n");
  if (ERR == read_dentry_by_name(c, &d)){
    return ERR;
  }

  //printf("5\n");

  // old_page=set_dentry_lg(USER_PAGE, 8*MBYTE + FOUR_MBYTE*process);
  printf("execute paging\n");
  old_pn = set_program_page(process);

  if (ERR == load_prog((int8_t*)c)){
    // set_dentry_lg(USER_PAGE, old_page);
    printf("Error: %d,%d,%d\n",__LINE__, __func__, __FILE__);
    set_program_page(old_pn);
    return ERR;
  }
  //printf("6\n");
  header = (uint8_t*) PROG_PTR;
  if (*((uint32_t*)header) != MAGIC_ELF){
    // set_dentry_lg(USER_PAGE, old_page);
    printf("Error: %d,%d,%d\n",__LINE__, __func__, __FILE__);
    set_program_page(old_pn);
    return ERR;
  }
  //printf("7\n");
  entry = (uint32_t*) (header + 24);
  // set tss

  // set pcb
  pcb_init(set_tss_esp(KERNEL_STACK, process));

  printf("\n%x\n", pcb_ptr);
	/*testing getargs
	int8_t buf_test[20];
	buf_test[0] = 'a';
	buf_test[1] = 'b';
	buf_test[2] = 'c';
	buf_test[3] = 'd';
	for (j = 0; j < 4; j++){
		printf("S[%d] = %c\n", j, buf_test[j]);
	}

	strcpy(pcb_ptr->pcb_buf, buf_test);
	//strcpy((int8_t *) buf_test, (int8_t *)pcb_ptr->pcb_buf);

	printf("%c\n", pcb_ptr->pcb_buf[0]);
	printf("%c\n", pcb_ptr->pcb_buf[1]);
	printf("%c\n", pcb_ptr->pcb_buf[2]);

	int8_t after_arg[20];
	strcpy((int8_t *) after_arg, (int8_t *)pcb_ptr->pcb_buf);

	printf("%c\n", after_arg[0]);
	printf("%c\n", after_arg[1]);

	*/

  //copy the args to pcb
  strcpy(pcb_ptr->pcb_buf, args); //or pcb_ptr->pcb_buf = args in pbc_init?


  pcb_ptr->program_counter = process;
  pcb_ptr->parent = (pcb_t*)parent;
  // pcb_ptr->esp = USER_STACK;
  // pcb_ptr->ebp = USER_STACK;
  asm volatile("\n\
    movl %%esp, %0 \n\
    movl %%ebp, %1"
    :"=r"(pcb_ptr->esp), "=r"(pcb_ptr->ebp));
  asm volatile("\n\
    movw    %0, %%ds               \n\
    pushl   %0                     \n\
    pushl   %3                     \n\
    pushf                          \n\
    pushl   %1                     \n\
    pushl   %2                     \n\
    iret"
    :
    :"r" (USER_DS), "r" (USER_CS), "r" (*entry), "r" (USER_STACK)
    : "memory"
  );
  return 256;
}

/*int32_t sys_read (int32_t fd, void* buf, int32_t nbytes)
Inputs: fd determines the entry in the file discriptore table, buf empty buffer, nbytes size of buffer
Outputs: returns 0 for normal files and the size of the command for terminal
system call reads data from the keyboard, a file, device (RTC), or directory
*/
int32_t sys_read (int32_t fd, void* buf, int32_t nbytes){
    int32_t retval;
    retval = (pcb_ptr->fd[fd].fot->read)(fd, buf, nbytes);
    if(retval == -1){
      return retval;
    }
    pcb_ptr->fd[fd].file_pos += retval;
    return retval;
}

/*int32_t sys_write (int32_t fd, const void* buf, int32_t nbytes)
Inputs: fd determines the entry in the file discriptore table, buf buffer to be printed to terminal, nbytes size of buffer
Outputs: with regular files should always return -1 to indicate failure since the file system is read-only. Otherwise the call returns the number of bytes written, or -1 on failure
writes data to the terminal or to a device (RTC)
*/
int32_t sys_write (int32_t fd, const void* buf, int32_t nbytes){

    int32_t retval;
    retval = (pcb_ptr->fd[fd].fot->write)(fd, buf, nbytes);
    if(retval == -1){
      return retval;
    }
    pcb_ptr->fd[fd].file_pos += retval;
    return retval;
}

/*int32_t sys_open (const uint8_t* filename)
Inputs: filename
Outputs: If the named file does not exist or no descriptors are free, the call returns -1
The call should find the directory entry corresponding to the named file, allocate an unused file descriptor, and set up any data necessary to handle the given type of file
*/
int32_t sys_open (const uint8_t* filename){

  if (pcb_ptr->idx == 8)
    return ERR;
  if(read_dentry_by_name (filename, &open_dentry) == ERR)
    return ERR;
  if(open_dentry.ftype){
    pcb_ptr->fd[pcb_ptr->idx].fot = &fot_file_sys;

  }
  else{
    pcb_ptr->fd[pcb_ptr->idx].fot = &fot_rtc;

  }
  pcb_ptr->fd[pcb_ptr->idx].flags = 1;
  int32_t i;
  for(i = 0; i < 8; i++){
    if(pcb_ptr->fd[i].flags == 0)
      break;
  }
  pcb_ptr->idx = i;

  return 0;
}

/*int32_t sys_close (int32_t fd)
Inputs: file directory entry index
Outputs:  Trying to close an invalid descriptor should result in a return value of -1; successful closes should return 0.
system call closes the specified file descriptor and makes it available for return from later calls to open
*/
int32_t sys_close (int32_t fd){
  if(pcb_ptr->fd[fd].flags == 0)
    return ERR;
  pcb_ptr->fd[fd].flags = 0;
  int32_t i;
  for(i = 0; i < 8; i++){
    if(pcb_ptr->fd[i].flags == 0)
      break;
  }
  pcb_ptr->idx = i;
  return SUCCESS;
}


/*sys_getargs (uint8_t* buf, int32_t nbytes)
Inputs: buf empty buffer, nbytes size of buffer
Outputs: If the arguments and a terminal NULL (0-byte) do not fit in the buffer, simply return -1
l reads the program’s command line arguments into a user-level buffer
*/
int32_t sys_getargs (uint8_t* buf, int32_t nbytes){
	//printf("gets to args\n");
	if ( buf == NULL || nbytes == NULL ){
		return ERR;
	}

	//copy args to buf
	strcpy((int8_t *) buf, (int8_t *)pcb_ptr->pcb_buf);

	//printf("%s\n", buf);
	return SUCCESS;
}

/*int32_t sys_vidmap (uint8_t** screen_start)
Inputs: start of video mem buffer
Outputs: ). If the location is invalid, the call should return -1.
maps the text-mode video memory into user space at a pre-set virtual address
*/
int32_t sys_vidmap (uint8_t** screen_start){


	if ( (uint32_t)screen_start < PROG_VIRTUAL_ADDR || (uint32_t)screen_start > (PROG_VIRTUAL_ADDR + (4 * MBYTE)) ){
		return ERR;
	}

	*screen_start = (uint8_t*)(PROG_VIRTUAL_ADDR + (4 * MBYTE));


	return SUCCESS;
}

//handles signals not implimented yet
int32_t set_handler (int32_t signum, void* handler_address){
  return ERR;
}
//handles signal not implimented yey
int32_t sigreturn (void){
  return ERR;
}

/* sets esp0 of TSS */
uint32_t set_tss_esp(uint32_t addr, int32_t process_num){
	tss.esp0 = addr - (8*KBYTE * (process_num + 1)) - 4;
	return tss.esp0;
}
