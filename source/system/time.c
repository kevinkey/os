#include "time.h"
#include <stdio.h>

static uint32_t Time;

uint32_t time_now(void)
{
    return Time;
}

uint32_t time_elapsed(uint32_t start)
{
    uint32_t const now = time_now();

    return (now - start);
}

void time_increment(uint32_t amount)
{
    uint32_t now = time_now();
    uint32_t total = now + amount;

    Time = total;
}

size_t time_string(char time[], size_t length)
{
    uint32_t t = time_now();

    uint32_t s = t / 1000;
    uint32_t ms = t % 1000;
    uint32_t m = s / 60;
    s %= 60;
    uint32_t h = m / 60;
    m %= 60;

    return snprintf(time, length, "%02d:%02d:%02d.%03d", h, m, s, ms);
}
