#include "os.h"
#include "os_time.h"
#include "irq.h"
#include "list.h"

struct os_task_t * Active_Task;
static struct list_t Task_List;
static uint8_t Critical;

static void idle_task(void)
{
    while (true);
}

uint8_t Stack[STACK_CONTEXT_SIZE];
struct os_task_t Idle =
{
    .CONFIG = &(struct task_config_t) {
        .name = "IDLE\r\n",
        .func = idle_task,
        .stack = Stack,
        .size = STACK_CONTEXT_SIZE,
        .priority = TASK_PRIORITY_LOW
    }
};

static void schedule_task(void)
{
    Active_Task = &Idle;

    LIST_FOR_EACH(&Task_List, struct os_task_t *, task)
    {
        if (!os_task_ready(task))
        {
            /* Task must be ready to be scheduled */
        }
        else if((uint_t)task->CONFIG->priority > (uint_t)Active_Task->CONFIG->priority)
        {
            Active_Task = task;
        }
    }
}

void os_init(void)
{
    irq_disable();
    Critical = 0u;

    list_init(&Task_List);
    os_task_init(&Idle);
    Active_Task = NULL;
}

void os_start(void)
{
    //schedule_task();
    //os_task_load(Active_Task);

    irq_enable();
    while (true);
}

uint8_t * os_tick(uint32_t amount, uint8_t * stack)
{
    if (Active_Task != NULL) { os_task_save(Active_Task, stack); }
    os_time_increment(amount);
    schedule_task();
    return os_task_load(Active_Task);
}

void os_add_task(struct os_task_t * task)
{
    list_add(&Task_List, (struct list_item_t *)task, LIST_ADD_HEAD);
}

void os_yield(void)
{
    //(void)os_tick(0, (uint8_t *)SP);
}

void os_enter_critical(void)
{
    irq_disable();
    Critical++;
}

void os_exit_critical(void)
{
    if (Critical <= 1) { Critical = 0; irq_enable(); }
    else { Critical--; }
}
