#include "mem.h"

static size_t Index;

static uint8_t Memory[MEM_SIZE];

uint8_t * mem_alloc(size_t size)
{
    uint8_t * mem = NULL;
    size_t total = Index + size;

    if (total <= MEM_SIZE)
    {
        mem = &Memory[Index];
        Index = total;
    }

    return mem;
}

size_t mem_used(void)
{
    return Index;
}
