#ifndef SYSTEM_H
#define SYSTEM_H

#include "types.h"
#include "task.h"

void system_init(void);
void system_tick(uint32_t amount);
void system_add_task(struct task_t * task);
void system_yield(void);

#endif
