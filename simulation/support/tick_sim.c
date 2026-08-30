#include "tick_sim.h"

#include "system.h"
#include "irq.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern bool Shutdown;

void * tick_sec(void * arg)
{
    while (!Shutdown)
    {
        irq_disable();
        system_tick(1000);
        irq_enable();

        (void)sleep(1);
    }

    return NULL;
}

pthread_t Time_Thread;

void tick_sim_init(void)
{
    if (pthread_create(&Time_Thread, NULL, tick_sec, NULL))
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
}

void tick_sim_deinit(void)
{
    pthread_join(Time_Thread, NULL);
}
