#ifndef OS_TASK_H
#define OS_TASK_H

#include "types.h"
#include "os_event.h"
#include "list.h"
#include "stack.h"
#include "time.h"

struct os_task_t
{
    struct list_item_t item;
    os_event_t * event;
    char * NAME;
    void (*FUNCTION)(void);
    size_t STACK_SIZE;
    uint32_t timeout;
    //uint32_t count;
    uint8_t * stack;
    uint8_t * stack_pointer;
    uint8_t PRIORITY;
};

extern struct list_t os_tasks;
extern struct os_task_t * os_task_current;

void os_task_init(struct os_task_t * task);
bool os_task_wait(struct os_task_t * task, os_event_t * event, uint32_t timeout);
bool os_task_ready(struct os_task_t * task);
void os_task_save(struct os_task_t * task, uint8_t * stack);
uint8_t * os_task_load(struct os_task_t * task);
size_t os_task_usage(struct os_task_t * task);
void os_task_next(void);

#endif
