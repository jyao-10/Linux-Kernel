#ifndef _LINKER_H
#define _LINKER_H

#include "types.h"
#include "x86_desc.h"

#ifndef ASM

#define KERNEL_CS 0x0010
#define KERNEL_DS 0x0018
extern int32_t system_calls(void);

#endif /* ASM */

#endif /* _LINKER_H */
