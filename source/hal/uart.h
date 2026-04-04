#ifndef UART_H
#define UART_H

#include "types.h"
#include "event.h"
#include "ring.h"
#include "time.h"

enum uart_parity_t
{
    UART_PARITY_NONE,
    UART_PARITY_ODD,
    UART_PARITY_EVEN,
};

enum uart_stop_t
{
    UART_STOP_1,
    UART_STOP_2,
};

struct uart_config_t
{
    void (*transmit)(uint8_t, uint8_t const[], size_t length);
    struct ring_config_t receive;
    uint32_t baudrate;
    enum uart_parity_t parity;
    enum uart_stop_t stop;
    uint8_t id;
};

struct uart_t
{
    struct uart_config_t const * CONFIG;
    struct ring_t receive;
    event_t tx_complete;
    event_t rx_data;
};

void uart_init(struct uart_t * uart);
void uart_write(struct uart_t * uart, uint8_t const buffer[], size_t length);
bool uart_read(struct uart_t * uart, uint8_t buffer[], size_t length, time_t timeout);

#endif
