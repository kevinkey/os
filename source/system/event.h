#ifndef EVENT_H
#define EVENT_H

#include "types.h"
#include "time.h"

typedef bool event_t;

void event_clear(event_t * event);
void event_set(event_t * event);
bool event_wait(event_t * event, time_t timeout);

#endif
