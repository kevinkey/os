#include "test.h"
#include "shell.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>

void test_put(char const str[])
{
    printf("%s", str);
}

size_t test_get(char str[], size_t length)
{
    return strlen(fgets(str, length, stdin));
}

struct shell_t Shell = {.CONFIG = &(struct shell_config_t){.put = test_put, .get = test_get}};

void uart_tx(uint8_t id, uint8_t const buffer[], size_t length)
{
    printf("\e[1;32mUART%d: ", id);
    for (size_t i = 0u; i < length; i++)
    {
        printf("%02X ", buffer[i]);
    }
    printf("\e[0m\n");
}

struct uart_t Uart0 = {
    .CONFIG = &(struct uart_config_t){
        .baudrate = 115200u,
        .parity = UART_PARITY_NONE,
        .id = 0u,
        .transmit = uart_tx,
    },
};

int main(void)
{
    uart_init(&Uart0);

    shell_init(&Shell);
    shell_process(&Shell);

    exit(EXIT_SUCCESS);
}
