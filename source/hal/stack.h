#ifndef STACK_H
#define STACK_H

#include "types.h"

#define STACK_CONTEXT_SIZE 20

void stack_init(uint_t stack[], void (*function)(void));
void stack_save(uint_t stack[]);
void stack_load(uint_t const stack[]);

#endif