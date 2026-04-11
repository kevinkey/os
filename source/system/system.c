#include "system.h"
#include "irq.h"
#include "list.h"

struct list_t Task_List;
struct task_t * Active_Task;

static void idle_task(void)
{
    while (true)
    {
        /* do nothing */
    }
}

uint_t Stack[STACK_CONTEXT_SIZE];
struct task_t Idle =
{
    .CONFIG = &(struct task_config_t) {
        .func = idle_task,
        .stack = Stack,
        .size = STACK_CONTEXT_SIZE,
        .priority = TASK_PRIORITY_LOW
    }
};

static void schedule_task(void)
{
    Active_Task = &Idle;

    LIST_FOR_EACH(&Task_List, struct task_t *, task)
    {
        if(task_ready(task))
        {
            if((uint_t)task->CONFIG->priority > (uint_t)Active_Task->CONFIG->priority)
            {
                Active_Task = task;
            }
        }
    }
}

void system_init(void)
{
    list_init(&Task_List);
    task_init(&Idle);
}

void system_start(void)
{
    schedule_task();
    task_load(Active_Task);

    irq_enable(true);
}

void system_tick(time_t amount)
{
    bool state = irq_disable();

    task_save(Active_Task);
    time_increment(amount);
    schedule_task();
    task_load(Active_Task);

    irq_enable(state);
}

void system_add_task(struct task_t * task)
{
    bool state = irq_disable();

    list_add(&Task_List, (struct list_item_t *)task, LIST_ADD_HEAD);

    irq_enable(state);
}

void system_yield(void)
{
    system_tick(0u);
}
