#ifndef OS_TIME_H
#define OS_TIME_H

#include "types.h"

uint32_t os_time_now(void);
uint32_t os_time_elapsed(uint32_t start);
void os_time_increment(uint32_t amount);
size_t os_time_string(char time[], size_t length);

#endif
