#ifndef OS_H
#define OS_H

#include "types.h"
#include "os_task.h"

extern struct os_task_t * Active_Task;

void os_init(void);
void os_start(void);
uint8_t * os_tick(uint32_t amount, uint8_t * stack);
void os_add_task(struct os_task_t * task);
void os_yield(void) __attribute__((naked));
void os_enter_critical(void);
void os_exit_critical(void);

#endif
