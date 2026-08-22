#include "uart.h"
#include "irq.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern bool Shutdown;

void uart_tx(uint8_t id, uint8_t const buffer[], size_t length)
{
    printf("\e[1;32mUART%d: ", id);
    for (size_t i = 0; i < length; i++)
    {
        printf("%02X ", buffer[i]);
    }
    printf("\e[0m\n");
}

struct uart_t Uart0 =
{
    .CONFIG = &(struct uart_config_t){
        .baudrate = 115200,
        .parity = UART_PARITY_NONE,
        .id = 0,
        .transmit = uart_tx,
        .stop = UART_STOP_1,
        .receive = {.buffer = (uint8_t[32]){0}, .length = 32}
    },
};

void * uart0_rx(void * arg)
{
    while (!Shutdown)
    {
        irq_disable();
        uint8_t byte = (uint8_t)rand();
        uart_rx(&Uart0, &byte, 1);
        irq_enable();

        (void)sleep(1);
    }

    return NULL;
}

pthread_t Uart0_Thread;

void uart_sim_init(void)
{
    uart_init(&Uart0);
    if (pthread_create(&Uart0_Thread, NULL, uart0_rx, NULL))
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
}

void uart_sim_deinit(void)
{
    pthread_join(Uart0_Thread, NULL);
}
