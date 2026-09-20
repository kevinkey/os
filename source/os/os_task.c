#include "os_task.h"
#include "os_time.h"
#include "os.h"
#include "stack.h"
#include "irq.h"
#include <string.h>

struct list_t os_tasks;
struct os_task_t * os_task_current;

static bool Task_Init = false;
static uint8_t const Sentinel[] = {0x12, 0x34, 0x56, 0x78};

static void idle_task(void)
{
    while (true);
}

static uint8_t Stack[STACK_CONTEXT_SIZE];
static struct os_task_t Idle =
{
    .CONFIG = &(struct task_config_t) {
        .name = "IDLE",
        .func = idle_task,
        .stack = Stack,
        .size = STACK_CONTEXT_SIZE,
        .priority = TASK_PRIORITY_LOW
    }
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

    for (size_t i = 0; i < task->CONFIG->size; i++)
    {
        uint8_t sentinel = Sentinel[i & (sizeof(Sentinel) - 1)];
        task->CONFIG->stack[i] = sentinel;
    }
    task->stack = stack_init(task->CONFIG->stack, task->CONFIG->size, task->CONFIG->func);

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
    if (task != NULL) { task->stack = stack; }
}

uint8_t * os_task_load(struct os_task_t * task)
{
    return task->stack;
}

size_t os_task_usage(struct os_task_t * task)
{
    size_t i;

    for (i = 0; i < task->CONFIG->size; i++)
    {
        uint8_t sentinel = Sentinel[i & (sizeof(Sentinel) - 1)];
        if (task->CONFIG->stack[i] != sentinel)
        {
            break;
        }
    }

    return task->CONFIG->size - i;
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
        else if((uint_t)task->CONFIG->priority > (uint_t)next->CONFIG->priority)
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
