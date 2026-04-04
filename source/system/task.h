#ifndef TASK_H
#define TASK_H

#include "types.h"
#include "event.h"
#include "list.h"
#include "stack.h"
#include "time.h"

enum task_priority_t
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
    enum task_priority_t priority;
};

struct task_t
{
    struct list_item_t item;
    struct task_config_t const * CONFIG;
    event_t * event;
    time_t timeout;
};

void task_init(struct task_t * task);
bool task_wait(struct task_t * task, event_t * event, time_t timeout);
bool task_ready(struct task_t * task);
void task_save(struct task_t * task);
void task_load(struct task_t * task);

#endif
