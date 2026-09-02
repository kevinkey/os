#ifndef OS_EVENT_H
#define OS_EVENT_H

#include "types.h"
#include "time.h"

typedef bool os_event_t;

void os_event_clear(os_event_t * event);
void os_event_set(os_event_t * event);
bool os_event_wait(os_event_t * event, uint32_t timeout);

#endif
