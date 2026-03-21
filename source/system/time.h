#ifndef TIME_H
#define TIME_H

#include "types.h"

typedef uint32_t time_t;

time_t time_now(void);
time_t time_elapsed(time_t start);
void time_increment(time_t amount);

#endif
