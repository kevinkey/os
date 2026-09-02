#include "os_task.h"
#include "os_time.h"
#include "os.h"
#include <string.h>

static uint8_t const Sentinel[] = {0x12, 0x34, 0x56, 0x78};

void os_task_init(struct os_task_t * task)
{
    uint8_t * stack = (uint8_t *)&(task->CONFIG->stack[task->CONFIG->size]) - sizeof(Sentinel);

    while(stack >= (uint8_t *)task->CONFIG->stack)
    {
        memcpy(stack, Sentinel, sizeof(Sentinel));
        stack -= sizeof(Sentinel);
    }
    stack_init(task->CONFIG->stack, task->CONFIG->func);

    task->event = NULL;
    os_add_task(task);
}

bool os_task_wait(struct os_task_t * task, os_event_t * event, uint32_t timeout)
{
    os_enter_critical();
    task->event = event;
    task->timeout = os_time_now() + timeout;
    os_yield();
    os_exit_critical();

    return ((task->event != NULL) && *(task->event));
}

bool os_task_ready(struct os_task_t * task)
{
    bool ready;

    if(os_time_now() >= task->timeout)
    {
        ready = true;
    }
    else if((task->event != NULL) && *(task->event))
    {
        ready = true;
    }
    else
    {
        ready = false;
    }

    return ready;
}

void os_task_save(struct os_task_t * task)
{
    stack_save(task->CONFIG->stack);

    uint8_t * p_stack = (uint8_t *)&(task->CONFIG->stack[task->CONFIG->size]) - sizeof(Sentinel);

    if(0 != memcmp(p_stack, Sentinel, sizeof(Sentinel)))
    {
        while(true)
        {
            /* stack overflow detected */
        }
    }
}

void os_task_load(struct os_task_t * task)
{
    stack_load(task->CONFIG->stack);
}
