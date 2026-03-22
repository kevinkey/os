#include "time.h"
#include <stdio.h>

static uint32_t Time;

time_t time_now(void)
{
    return Time;
}

time_t time_elapsed(time_t start)
{
    time_t const now = time_now();

    return (now - start);
}

void time_increment(time_t amount)
{
    time_t now = time_now();
    time_t total = now + amount;

    Time = total;
}

size_t time_string(char time[], size_t length)
{
    time_t t = time_now();

    time_t s = t / 1000u;
    time_t ms = t % 1000u;
    time_t m = s / 60u;
    s %= 60u;
    time_t h = m / 60u;
    m %= 60u;

    return snprintf(time, length, "%02d:%02d:%02d.%03d", h, m, s, ms);
}
