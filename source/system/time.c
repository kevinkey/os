#include "time.h"

uint32_t Time;

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
