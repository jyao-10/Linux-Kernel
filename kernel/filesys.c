#include "filesys.h"

uint32_t* filesys_start;
uint32_t* filesys_end;

uint32_t* dentry_start;
uint32_t* inode_start;
uint32_t* dblock_start;

uint8_t ifname[IFNAME_SIZE]; // file name index
uint32_t iinode[IINODE_SIZE]; // index node...er...index

uint32_t num_dentry;
uint32_t num_inode;
uint32_t num_dblock;
uint32_t ininode[16]={
	0, 2, 6, 16, 17, 26, 28, 33, 37, 46, 50, 51, 53, 54, 60, 63
};
static uint32_t index;
static uint32_t nindex;
/* filesys_init:
 *     initializes file system name and index arrays; parses boot block;
 *         stores number of directory entries, index nodes, and data blocks;
 * Arguments:
 *   uint32_t* addr_start  :  starting address of file system module
 *   uint32_t* addr_end    :  ending address of file system module
 *
 */
void filesys_init(uint32_t* addr_start,  uint32_t* addr_end){
	uint32_t i, j;
	dentry_t* d;

	filesys_start = addr_start;
	filesys_end = addr_end;

	num_dentry = *(addr_start);
	num_inode = *(addr_start + 1);
	num_dblock = *(addr_start + 2);

	dentry_start = addr_start + DENTRY_SIZE/WORD;
	inode_start = addr_start + BLOCK/WORD;
	dblock_start = inode_start + num_inode*(BLOCK/WORD);

	for (i = 0; i < num_dentry; i++){
		d = (dentry_t*) (dentry_start + i*DENTRY_SIZE/WORD);
		for (j = 0; j < NAME_SIZE; j++){
			ifname[i*NAME_SIZE + j] = d->fname[j];
		}
		iinode[i] = d->inode;
	}
	filesys_start = filesys_start;
	return;
}

/* read_dentry_by_name:
 *     searches for directory entries by name
 * Arguments:
 *   const uint8_t* fname  :  pointer to array of chars
 *   dentry_t* dentry      :  pointer to directory entry
 * Returns:
 *   ERR     -1  :  No directories found matching name
 *   SUCCESS  0  :  directory entry found and copied to argument 2 pointer
 */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry){
	uint32_t i = 0, j = 0, flag = 0;
	dentry_t* d;
	while (i < num_dentry && 0==flag){
		while(*(fname + j) == ifname[i*NAME_SIZE+j]){
			if (*(fname + j) == '\0'){
				flag = PASS;
			}
			j++;
		}
		if (j==NAME_SIZE){
			flag = PASS;
		}
		i++;
	}
	if (flag == PASS){
		i--;
		d = (dentry_t*)(dentry_start + i*DENTRY_SIZE/WORD);
		for (j = 0; j < NAME_SIZE; j ++){
			dentry->fname[j] = d->fname[j];
		}
		dentry->ftype = d->ftype;
		dentry->inode = d->inode;
		return SUCCESS;
	} else {
		return ERR;
	}
}

/* read_dentry_by_index:
 *     searches for directory entries by index #
 * Arguments:
 *   uint32_t index    :  index number to search for
 *   dentry_t* dentry  :  pointer to directory entry
 * Returns:
 *   ERR     -1  :  no directories found matching index #
 *   SUCCESS  0  :  directory entry found and copied to argument 2 pointer
 */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
	uint32_t i = 0, j;
	dentry_t* d;
	while((index != iinode[i]) && (i < num_dentry)){
		i++;
	}
	if (index == iinode[i]){
		d = (dentry_t*)(dentry_start + i*DENTRY_SIZE/WORD);
		for (j = 0; j < NAME_SIZE; j ++){
			dentry->fname[j] = d->fname[j];
		}
		dentry->ftype = d->ftype;
		dentry->inode = d->inode;
		return SUCCESS;
	} else {
		return ERR;
	}
}

/* read_data:
 *     reads data from given inode.  Follows table of data blocks given in
 *         inode struct and reads the i+offset element
 * Arguments:
 *   uint32_t inode   :  index number to read off of
 *   uint32_t offset  :  number of bytes offset at the beginning of reading
 *   uint8_t* buf     :  pointer to buffer
 *   uint32_t length  :  number of bytes to be read (from 0 - 128)
 * Returns:
 *   NONNEGATIVE  :  number of bytes successfully read and stored in buffer
 *   ERR          :  invalid inode value
 */
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
	uint32_t i = 0;
	inode_t* n;
	dblock_t* d;

	if (inode >= num_inode){
		return ERR;
	}
	n = (inode_t*)(inode_start + inode*BLOCK/WORD);
	d = (dblock_t*)((n->dblock[offset >> DIV_KBYTE])*BLOCK/WORD + dblock_start);
	while ((i < length) && ((i + offset) < (n->length))){
		if (((i + offset) & OFFSET_MASK) == NULL){
			d = (dblock_t*)((n->dblock[(i + offset) >> DIV_KBYTE])*BLOCK/WORD + dblock_start);
		}
		*(buf + i) = d->data[(i + offset) & OFFSET_MASK];
		i++;
	}
	return i;
}



void test_init(void){
	index = 0;
	nindex = 0;
	return;
}

void test_ls(void){
	uint32_t i;
	dentry_t d;
	int8_t lbuf[4];
	for (i = 0; i < num_inode; i++){
		if (SUCCESS==read_dentry_by_index(i, &d)){
			// printf("file_name: %s, file_type: %d, index: %d\n", d.fname, d.ftype, d.inode);
			terminal_write(NULL, "file_name: ", 11);
			terminal_write(NULL, d.fname, NAME_SIZE);
			terminal_write(NULL, " | file_type: ", 14);
			itoa(d.ftype, lbuf, 10);
			terminal_write(NULL, lbuf, 2);
			terminal_write(NULL, " | index: ", 10);
			itoa(d.inode, lbuf, 10);
			terminal_write(NULL, lbuf, 2);
			terminal_write(NULL, "\n", 1);
		}
	}
	return;
}

void test_index(void){
	dentry_t d;
	int32_t read, len;
	inode_t* i;
	static uint8_t buf[BUFFER_SIZE + 1];
	if (SUCCESS==read_dentry_by_index(ininode[index], &d)){
		i = (inode_t*)(inode_start + ((d.inode)*(BLOCK/WORD)));
		if ((i->length) < BUFFER_SIZE){
			len = i->length;
		} else {
			len = BUFFER_SIZE;
		}
		read = read_data(d.inode, 0, buf, len);
		if (ERR != read){
			// printf("%s\nfile_name: %s\n", buf, d.fname);
			terminal_write(NULL, buf, len);
			terminal_write(NULL, "\n", 1);
			terminal_write(NULL, "file_name: ", 11);
			terminal_write(NULL, d.fname, NAME_SIZE);
			terminal_write(NULL, "\n", 1);
			if (index <15){
				index ++;
			} else {
				index = 0;
			}
		}
	} else {
		// printf("fail :(\n");
		terminal_write(NULL, "you failed\n",11);
	}
	return;
}

void test_name(void){
	dentry_t d;
	int32_t read, len, j;
	inode_t* i;
	static uint8_t nbuf[BUFFER_SIZE + 1];
	uint8_t fn[NAME_SIZE];
	for (j = 0; j < NAME_SIZE; j++){
		fn[j] = ifname[nindex*NAME_SIZE+j];
	}
	if (SUCCESS==read_dentry_by_name(fn, &d)){
		i = (inode_t*)(inode_start + ((d.inode)*(BLOCK/WORD)));
		if ((i->length) < BUFFER_SIZE){
			len = i->length;
		} else {
			len = BUFFER_SIZE;
		}
		read = read_data(d.inode, 0, nbuf, len);
		if (ERR != read){
			// printf("%s\nfile_name: %s\n", buf, d.fname);
			terminal_write(NULL, nbuf, len);
			terminal_write(NULL, "\n", 1);
			terminal_write(NULL, "file_name: ", 11);
			terminal_write(NULL, d.fname, NAME_SIZE);
			terminal_write(NULL, "\n", 1);
			if (nindex <15){
				nindex ++;
			} else {
				nindex = 0;
			}
		}
	} else {
		// printf("fail :(\n");
		terminal_write(NULL, "you failed\n",11);
	}
	return;
}

int32_t load_prog (const int8_t* fname){
	dentry_t d;
	uint32_t length;
	uint8_t* prog_ptr = (uint8_t*)PROG_PTR;
	inode_t* inode;
	if (ERR == read_dentry_by_name((uint8_t*)fname, &d)){
		return ERR;
	}
	inode = (inode_t*)(inode_start + ((d.inode)*(BLOCK/WORD)));
	length = inode->length;
	if (ERR == read_data(d.inode, 0, prog_ptr, length)){
		return ERR;
	}
	return SUCCESS;
}

void test_load(void){
	// set_dentry_lg(USER_PAGE, 8*MBYTE);
	load_prog("shell");
	return;
}

int32_t fs_read (int32_t fd, void* buf, int32_t nbytes){
	dentry_t d;
	file_desc_t* fdesc = &(pcb_ptr->fd[fd]);
	uint32_t fp = fdesc->file_pos;
	if (ERR == read_dentry_by_index(fd, &d)){
		return ERR;
	}
	fdesc->file_pos += read_data(d.inode, fp, (uint8_t*)buf, nbytes);
	return fdesc->file_pos;
}
int32_t fs_write (int32_t fd, const void* buf, int32_t nbytes){
	return ERR;
}
int32_t fs_open (const uint8_t* filename){
	dentry_t d;
	if (ERR == read_dentry_by_name(filename, &d)){
		return ERR;
	}
	return SUCCESS;
}
int32_t fs_close (int32_t fd){
	return SUCCESS;
}
