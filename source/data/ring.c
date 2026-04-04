#include "ring.h"
#include <string.h>

void ring_init(struct ring_t * ring)
{
    ring->head = 0u;
    ring->tail = 0u;
    ring->full = false;
}

size_t ring_count(struct ring_t * ring)
{
    size_t count;

    if (ring->full)
    {
        count = ring->CONFIG->length;
    }
    else if (ring->tail >= ring->head)
    {
        count = ring->tail - ring->head;
    }
    else
    {
        count = (ring->CONFIG->length - ring->head) + ring->tail;
    }

    return count;
}

size_t ring_space(struct ring_t * ring)
{
    return ring->CONFIG->length - ring_count(ring);
}

bool ring_write(struct ring_t * ring, uint8_t const data[], size_t length)
{
    size_t space = ring_space(ring);
    bool sufficient_space = (space >= length);

    if (sufficient_space)
    {
        ring->full = (space == length);

        size_t room_to_end = ring->CONFIG->length - ring->tail;

        if (room_to_end <= length)
        {
            size_t amount_wrapped = length - room_to_end;

            memcpy(&ring->CONFIG->buffer[ring->tail], data, room_to_end);
            if (amount_wrapped > 0u)
            {
                memcpy(ring->CONFIG->buffer, &data[room_to_end], amount_wrapped);
            }

            ring->tail = amount_wrapped;
        }
        else
        {
            memcpy(&ring->CONFIG->buffer[ring->tail], data, length);
            ring->tail += length;
        }
    }

    return sufficient_space;
}

bool ring_read(struct ring_t * ring, uint8_t data[], size_t length)
{
    bool sufficient_data = (ring_count(ring) >= length);

    if (sufficient_data)
    {
        if (length != 0u)
        {
            ring->full = false;
        }

        size_t data_to_end = ring->CONFIG->length - ring->head;

        if (data_to_end <= length)
        {
            size_t amount_wrapped = length - data_to_end;

            memcpy(data, &ring->CONFIG->buffer[ring->head], data_to_end);
            if (amount_wrapped > 0u)
            {
                memcpy(&data[data_to_end], ring->CONFIG->buffer, amount_wrapped);
            }

            ring->head = amount_wrapped;
        }
        else
        {
            memcpy(data, &ring->CONFIG->buffer[ring->head], length);
            ring->head += length;
        }
    }

    return sufficient_data;
}
