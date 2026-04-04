#include "task.h"
#include "irq.h"
#include "system.h"
#include <string.h>

static uint8_t const Sentinel[] = {0x12u, 0x34u, 0x56u, 0x78u};

void task_init(struct task_t * task)
{
    uint8_t * p_stack = (uint8_t *)&(task->STACK[task->SIZE]) - sizeof(Sentinel);

    while(p_stack >= (uint8_t *)task->STACK)
    {
        memcpy(p_stack, Sentinel, sizeof(Sentinel));
        p_stack -= sizeof(Sentinel);
    }
    stack_init(task->STACK, task->FUNCTION);

    task->event = NULL;
    system_add_task(task);
}

bool task_wait(struct task_t * task, event_t * event, time_t timeout)
{
    bool state = irq_disable();

    task->event = event;
    task->timeout = time_now() + timeout;

    system_yield();

    irq_enable(state);

    return ((task->event != NULL) && *(task->event));
}

bool task_ready(struct task_t * task)
{
    bool ready;

    if(time_now() >= task->timeout)
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

void task_save(struct task_t * task)
{
    stack_save(task->STACK);

    uint8_t * p_stack = (uint8_t *)&(task->STACK[task->SIZE]) - sizeof(Sentinel);

    if(0 != memcmp(p_stack, Sentinel, sizeof(Sentinel)))
    {
        while(true)
        {
            /* stack overflow detected */
        }
    }
}

void task_load(struct task_t * task)
{
    stack_load(task->STACK);
}
