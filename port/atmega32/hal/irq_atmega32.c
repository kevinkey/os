#include "irq.h"
#include <avr/interrupt.h>

void irq_init(void)
{

}

void irq_disable(void)
{
    cli();
}

void irq_enable(void)
{
    sei();
}