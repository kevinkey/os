#ifndef RING_H
#define RING_H

#include "types.h"

struct ring_t
{
    uint8_t * buffer;
    size_t length;
    size_t head;
    size_t tail;
    bool full;
};

void ring_init(struct ring_t * ring, uint8_t buffer[], size_t length);
size_t ring_count(struct ring_t * ring);
size_t ring_space(struct ring_t * ring);
bool ring_write(struct ring_t * ring, uint8_t const data[], size_t length);
bool ring_read(struct ring_t * ring, uint8_t data[], size_t length);


#endif
