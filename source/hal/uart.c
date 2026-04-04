#include "uart.h"

void uart_init(struct uart_t * uart)
{
    uart->receive.CONFIG = &uart->CONFIG->receive;
    ring_init(&uart->receive);
}

void uart_write(struct uart_t * uart, uint8_t const buffer[], size_t length)
{
    event_clear(&uart->tx_complete);
    uart->CONFIG->transmit(uart->CONFIG->id, buffer, length);
    event_wait(&uart->tx_complete, -1);
}

bool uart_read(struct uart_t * uart, uint8_t buffer[], size_t length, time_t timeout)
{
    time_t start_time = time_now();
    time_t elapsed = 0u;
    bool read = false;

    do
    {
        read = ring_read(&uart->receive, buffer, length);
        if (read) { break; }

        elapsed = time_now() - start_time;
        if (elapsed >= timeout) { break; }
    }
    while (event_wait(&uart->rx_data, timeout - elapsed));

    return read;
}
