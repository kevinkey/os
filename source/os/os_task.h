#ifndef OS_TASK_H
#define OS_TASK_H

#include "types.h"
#include "os_event.h"
#include "list.h"
#include "stack.h"
#include "time.h"

enum os_task_priority_t
{
    TASK_PRIORITY_LOW,
    TASK_PRIORITY_NORMAL,
    TASK_PRIORITY_HIGH,
    TASK_PRIORITY_CRITICAL
};

struct task_config_t
{
    void (*func)(void);
    uint_t * stack;
    uint_t size;
    enum os_task_priority_t priority;
};

struct os_task_t
{
    struct list_item_t item;
    struct task_config_t const * CONFIG;
    os_event_t * event;
    uint32_t timeout;
};

void os_task_init(struct os_task_t * task);
bool os_task_wait(struct os_task_t * task, os_event_t * event, uint32_t timeout);
bool os_task_ready(struct os_task_t * task);
void os_task_save(struct os_task_t * task);
void os_task_load(struct os_task_t * task);

#endif
