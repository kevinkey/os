#include "event.h"

void event_clear(event_t * event)
{
    *event = false;
}

void event_set(event_t * event)
{
    *event = true;
}

bool event_wait(event_t * event, time_t timeout)
{

}
