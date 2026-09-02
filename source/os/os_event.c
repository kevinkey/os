#include "os_event.h"

void os_event_clear(os_event_t * event)
{
    *event = false;
}

void os_event_set(os_event_t * event)
{
    *event = true;
}

bool os_event_wait(os_event_t * event, uint32_t timeout)
{
    return *event;
}
