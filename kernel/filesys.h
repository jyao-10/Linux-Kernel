#ifndef _FILESYS_H
#define _FILESYS_H

#include "types.h"
#include "paging.h" // included to use KBYTE definition and set user-level
                    //   program page
#include "terminal.h"
#include "lib.h"
#include "pcb.h"

#define WORD 4
#define BLOCK (4 * KBYTE)
#define RESERVE_52 52
#define RESERVE_24 24
#define NAME_SIZE 32
#define DENTRY_SIZE 64
#define IINODE_SIZE 63
#define IFNAME_SIZE 2016

#define DIV_KBYTE 12

#define OFFSET_MASK 0x00000FFF
#define PASS 1

#define BUFFER_SIZE 128

#define PROG_PTR 0x08048000

#define USER_PAGE 32

/*
typedef struct file_des
{
  uint32_t* op_jump[4];
  uint32_t* inode;
  uint32_t file_pos;
  uint32_t flags;
} file_des_t;
*/

extern uint32_t num_dentry;
extern uint32_t num_inode;
extern uint32_t num_dblock;

typedef struct inode
{
	uint32_t length;
	uint32_t dblock[1023];
} inode_t;

typedef struct dblock
{
	uint8_t data[BLOCK];
} dblock_t;

typedef struct dentry
{
	uint8_t fname[NAME_SIZE];
	uint32_t ftype;
	uint32_t inode;
	uint8_t reserved[RESERVE_24];
} dentry_t;
// uint32_t ininode[16]={
// 	0, 2, 6, 16, 17, 26, 28, 33, 37, 46, 50, 51, 53, 54, 60, 63
// };
/* sets the location of the filesystem in memory
 *    IMPORTANT: run before initiating paging
 */
void filesys_init (uint32_t* addr_start, uint32_t* addr_end);
/* reads filesys_img directory entry by name */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);

/* reads filesys_img directory entry by index */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);

/* reads data up to 'length' bytes starting at position 'offset' in file with
 * given 'inode'
 *   returns the number of bytes read and placed in buffer
 */
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

int32_t load_prog (const int8_t* fname);
void test_init(void);
void test_ls(void);
void test_index(void);
void test_name(void);
void test_load(void);


int32_t fs_read (int32_t fd, void* buf, int32_t nbytes);
int32_t fs_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t fs_open (const uint8_t* filename);
int32_t fs_close (int32_t fd);
#endif /* _FILESYS_H */
