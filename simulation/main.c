#include "test.h"
#include "shell.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

bool Shutdown = false;

void test_put(char const str[])
{
    printf("%s", str);
}

size_t test_get(char str[], size_t length)
{
    return strlen(fgets(str, length, stdin));
}

struct shell_t Shell = {
    .CONFIG = &(struct shell_config_t){
        .put = test_put,
        .get = test_get
    }
};

void * time_sec(void * arg)
{
    while (!Shutdown)
    {
        time_increment(1000);
        (void)sleep(1);
    }

    return NULL;
}

void uart_tx(uint8_t id, uint8_t const buffer[], size_t length)
{
    printf("\e[1;32mUART%d: ", id);
    for (size_t i = 0; i < length; i++)
    {
        printf("%02X ", buffer[i]);
    }
    printf("\e[0m\n");
}

struct uart_t Uart0 = {
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
        uint8_t byte = (uint8_t)rand();
        uart_rx(&Uart0, &byte, 1);
        (void)sleep(1);
    }

    return NULL;
}

int main(void)
{
    pthread_t time_thread;
    if (pthread_create(&time_thread, NULL, time_sec, NULL))
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    pthread_t uart0_thread;
    uart_init(&Uart0);
    if (pthread_create(&uart0_thread, NULL, uart0_rx, NULL))
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    shell_init(&Shell);
    shell_process(&Shell);

    Shutdown = true;
    pthread_join(time_thread, NULL);
    pthread_join(uart0_thread, NULL);

    exit(EXIT_SUCCESS);
}
