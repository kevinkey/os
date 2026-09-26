#include "os_task.h"
#include "os_time.h"
#include "os.h"
#include "stack.h"
#include "irq.h"
#include "mem.h"
#include <string.h>

struct list_t os_tasks;
struct os_task_t * os_task_current;

static bool Task_Init = false;
static uint8_t const Sentinel[] = {0x12, 0x34, 0x56, 0x78};

static void idle_task(void)
{
    while (true);
}

static struct os_task_t Idle =
{
    .NAME = "IDLE",
    .STACK_SIZE = STACK_CONTEXT_SIZE,
    .FUNCTION = idle_task,
    .PRIORITY = 0,
};

void os_task_init(struct os_task_t * task)
{
    if (!Task_Init)
    {
        Task_Init = true;
        list_init(&os_tasks);
        os_task_init(&Idle);
        os_task_current = NULL;
    }

    //task->count = 0;
    task->stack = mem_alloc(task->STACK_SIZE);

    for (size_t i = 0; i < task->STACK_SIZE; i++)
    {
        uint8_t sentinel = Sentinel[i & (sizeof(Sentinel) - 1)];
        task->stack[i] = sentinel;
    }
    task->stack_pointer = stack_init(task->stack, task->STACK_SIZE, task->FUNCTION);

    task->event = NULL;
    list_add(&os_tasks, (struct list_item_t *)task, LIST_ADD_HEAD);
}

bool os_task_wait(struct os_task_t * task, os_event_t * event, uint32_t timeout)
{
    irq_disable();
    task->event = event;
    task->timeout = os_time_now() + timeout;
    os_yield();
    irq_enable();

    return ((task->event != NULL) && *(task->event));
}

bool os_task_ready(struct os_task_t * task)
{
    bool ready;

    if (os_time_now() >= task->timeout)
    {
        ready = true;
    }
    else if ((task->event != NULL) && *(task->event))
    {
        ready = true;
    }
    else
    {
        ready = false;
    }

    return ready;
}

void os_task_save(struct os_task_t * task, uint8_t * stack)
{
    if (task != NULL) { task->stack_pointer = stack; }
}

uint8_t * os_task_load(struct os_task_t * task)
{
    return task->stack_pointer;
}

size_t os_task_usage(struct os_task_t * task)
{
    size_t i;

    for (i = 0; i < task->STACK_SIZE; i++)
    {
        uint8_t sentinel = Sentinel[i & (sizeof(Sentinel) - 1)];
        if (task->stack[i] != sentinel)
        {
            break;
        }
    }

    return task->STACK_SIZE - i;
}

void os_task_next(void)
{
    struct os_task_t * next = &Idle;

    LIST_FOR_EACH(&os_tasks, struct os_task_t *, task)
    {
        if (!os_task_ready(task))
        {
            /* Task must be ready to be scheduled */
        }
        else if((uint_t)task->PRIORITY > (uint_t)next->PRIORITY)
        {
            next = task;
        }
    }

    if (next != os_task_current)
    {
        os_task_current = next;
        //next->count++;
    }
}
