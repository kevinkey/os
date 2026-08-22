#include "irq.h"
#include <pthread.h>

static pthread_mutex_t lock;

void irq_init(void)
{
    pthread_mutex_init(&lock, NULL);
}

void irq_disable(void)
{
    pthread_mutex_lock(&lock);
}

void irq_enable(void)
{
    pthread_mutex_unlock(&lock);
}
