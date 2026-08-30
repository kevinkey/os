#ifndef TIME_H
#define TIME_H

#include "types.h"

uint32_t time_now(void);
uint32_t time_elapsed(uint32_t start);
void time_increment(uint32_t amount);
size_t time_string(char time[], size_t length);

#endif
