#ifndef MEM_H
#define MEM_H

#include "types.h"

#ifndef MEM_SIZE
#define MEM_SIZE 256
#endif

uint8_t * mem_alloc(size_t size);
size_t mem_used(void);

#endif
