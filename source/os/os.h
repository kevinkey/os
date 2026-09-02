#ifndef OS_H
#define OS_H

#include "types.h"
#include "os_task.h"

void os_init(void);
void os_tick(uint32_t amount);
void os_add_task(struct os_task_t * task);
void os_yield(void);
void os_enter_critical(void);
void os_exit_critical(void);

#endif
