#include "uart.h"
#include "os.h"
#include <avr/io.h>
#include <avr/interrupt.h>

struct uart_t Uart = {.rx = {.LENGTH = 32}, .tx = {.LENGTH = 32}};

void uart_config(struct uart_t * uart, uint32_t baud, enum uart_parity_t parity, enum uart_stop_t stop)
{
    uint16_t ubrr = (F_CPU / 16) / (baud - 1);

    UBRRH = (uint8_t)(ubrr >> 8);
    UBRRL = (uint8_t)ubrr;

    uint8_t ucsrc = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

    switch (parity)
    {
        case UART_PARITY_EVEN: ucsrc |= (1 << UPM1); break;
        case UART_PARITY_ODD: ucsrc |= (1 << UPM1) | (1 << UPM0); break;
        case UART_PARITY_NONE: default: /* do nothing */ break;
    }

    if (stop == UART_STOP_2) { ucsrc |= (1 << USBS); }

    UCSRC = ucsrc;
}

void uart_enable(struct uart_t * uart, bool transmit, bool receive)
{
    uint8_t ucsrb = 0;

    if (transmit) { ucsrb |= (1 << TXEN); }
    if (receive) { ucsrb |= (1 << RXEN) | (1 << RXCIE); }

    UCSRB = ucsrb;
}

// bool uart_read(struct uart_t * uart, uint8_t buffer[], size_t length, uint32_t timeout)
// {
//     bool read;

//     os_enter_critical();
//     read = ring_read(&uart->rx, buffer, length);
//     os_exit_critical();

//     return read;
// }

void uart_write(struct uart_t * uart, uint8_t const buffer[], size_t length)
{
    size_t index = 0;

    do
    {
        os_enter_critical();
        os_event_clear(&uart->tx_complete);
        if (ring_write(&uart->tx, &buffer[index], 1))
        {
            index++;
        }
        UCSRB |= (1 << UDRIE);
        os_exit_critical();
    }
    while (index < length);
}

ISR(USART_UDRE_vect)
{
    uint8_t byte;

    if (ring_read(&Uart.tx, &byte, 1)) { UDR = byte; }
    else
    {
        os_event_set(&Uart.tx_complete);
        UCSRB &= ~(1 << UDRIE);
    }
}

ISR(USART_RXC_vect)
{
    uint8_t byte = UDR;
    PORTB ^= (1 << PB0);

    if (ring_write(&Uart.rx, &byte, 1))
    {
        os_event_set(&Uart.rx_data);
    }
    else
    {
        os_event_set(&Uart.rx_overflow);
    }
}
