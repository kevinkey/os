#include "os.h"
#include "os_time.h"
#include "irq.h"
#include "list.h"

static uint8_t Critical;

void os_init(void)
{
    irq_disable();
    Critical = 0u;
}

void os_start(void)
{
    irq_enable();
    while (true);
}

uint8_t * os_tick(uint32_t amount, uint8_t * stack)
{
    os_task_save(os_task_current, stack);
    os_time_increment(amount);

    static uint32_t last_time = 0;

    if (os_time_elapsed(last_time) >= 1000)
    {
        last_time = os_time_now();
    }

    os_task_next();
    return os_task_load(os_task_current);
}

#include <avr/io.h>

void os_yield(void)
{
    STACK_SAVE();
    uint8_t * stack = (uint8_t * )SP;
    stack = os_tick(0, stack);
    SP = (uint16_t)stack;
    STACK_LOAD();

    __asm__ __volatile__ ("reti");
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
