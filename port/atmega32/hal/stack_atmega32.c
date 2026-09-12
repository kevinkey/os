#include "stack.h"
#include <avr/io.h>

uint8_t * stack_init(uint8_t stack[], size_t size, void (*function)(void))
{
    uint8_t * top = &stack[size - 1];

    *top = (uint8_t)function;
    top--;
    *top = (uint8_t)((uint16_t)function >> 8);
    top--;

    *top = 0;
    top--;

    *top = 0x80;
    top--;

    for (uint8_t i = 1; i < 32; i++) { *top = 0; top--; }

    return top;
}
