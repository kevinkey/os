#ifndef OS_TIME_H
#define OS_TIME_H

#include "types.h"

struct os_time_t
{
    uint16_t ms;
    uint8_t sec;
    uint8_t min;
    uint8_t hr;
    uint8_t day;
};

uint32_t os_time_now(void);
struct os_time_t const * os_time_clock(void);
uint32_t os_time_elapsed(uint32_t start);
void os_time_increment(uint8_t amount);

#endif
