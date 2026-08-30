#include "os_time.h"
#include <stdio.h>

static uint32_t Time;
static struct os_time_t Clock;

uint32_t os_time_now(void)
{
    return Time;
}

struct os_time_t const * os_time_clock(void)
{
    return &Clock;
}

uint32_t os_time_elapsed(uint32_t start)
{
    uint32_t const now = os_time_now();

    return (now - start);
}

void os_time_increment(uint8_t amount)
{
    Time += (uint32_t)amount;

    Clock.ms += (uint16_t)amount;
    if (Clock.ms >= 1000)
    {
        Clock.ms -= 1000;
        if (++Clock.sec >= 60)
        {
            Clock.sec = 0;
            if (++Clock.min >= 60)
            {
                Clock.min = 0;
                if (++Clock.hr >= 24)
                {
                    Clock.hr = 0;
                    Clock.day++;
                }
            }
        }
    }
}
