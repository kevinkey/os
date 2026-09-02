#ifndef UART_H
#define UART_H

#include "types.h"
#include "os_event.h"
#include "ring.h"
#include "list.h"
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

struct uart_t
{
    struct list_item_t item;
    struct ring_t rx;
    struct ring_t tx;
    os_event_t tx_complete;
    os_event_t tx_space_avail;
    os_event_t rx_data;
    os_event_t rx_overflow;
    uint8_t ID;
};

struct uart_t * uart_ref(uint8_t id);
void uart_init(struct uart_t * uart);
void uart_config(struct uart_t * uart, uint32_t baud, enum uart_parity_t parity, enum uart_stop_t stop);
void uart_enable(struct uart_t * uart, bool transmit, bool receive);
void uart_write(struct uart_t * uart, uint8_t const buffer[], size_t length);
bool uart_read(struct uart_t * uart, uint8_t buffer[], size_t length, uint32_t timeout);
size_t uart_bytes_avail(struct uart_t * uart);
bool uart_overflow(struct uart_t * uart);

#endif
