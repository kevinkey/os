#include "irq.h"

static bool Irq;

bool irq_disable(void)
{
    bool state = Irq;

    Irq = false;

    return state;
}

void irq_enable(bool enable)
{
    if(enable)
    {
        Irq = true;
    }
}
