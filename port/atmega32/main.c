#define F_CPU 8000000UL // Define clock speed (change if using a different crystal)
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "shell.h"
#include "numstr.h"
#include "os_time.h"

// Initialize UART
void uart_init(unsigned int ubrr) {
    // Set baud rate registers
    UBRRH = (unsigned char)(ubrr >> 8);
    UBRRL = (unsigned char)ubrr;
    // Enable transmitter and receiver
    UCSRB = (1 << TXEN) | (1 << RXEN);
    // Set frame format: 8 data bits, 1 stop bit
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

void timer1_init(void) {
    // 1. Set CTC mode (Clear Timer on Compare Match)
    // WGM12 is located in TCCR1B
    TCCR1B |= (1 << WGM12);

    // 2. Set the compare value for 1ms interval (8MHz / 8 prescaler) - 1
    OCR1A = 999;

    // 3. Enable Timer1 Compare Match A Interrupt
    TIMSK |= (1 << OCIE1A);

    // 4. Set prescaler to 8 and start the timer
    // CS11 is located in TCCR1B
    TCCR1B |= (1 << CS11);
}

ISR(TIMER1_COMPA_vect)
{
    os_time_increment(1);
}

// Transmit a single character
void uart_transmit(char data) {
    // Wait for empty transmit buffer
    while (!(UCSRA & (1 << UDRE)));
    // Put data into buffer, sends the data
    UDR = data;
}

char uart_receive(void) {
    // Wait for data to be received (RXC flag becomes 1)
    while (!(UCSRA & (1 << RXC)));

    // Get and return received data from buffer
    return UDR;
}

void uart_put(char const str[])
{
    while (*str)
    {
        if (*str == '\n') { uart_transmit('\r'); }
        uart_transmit(*str++);
    }
}

size_t uart_get(char str[], size_t length)
{
    size_t i = 0;

    do
    {
        bool eos = false;

        str[i] = uart_receive();
        uart_transmit(str[i]);

        switch(str[i])
        {
            case '\n': uart_transmit('\r'); eos = true; break;
            case '\r': uart_transmit('\n'); eos = true; break;
            case '\b': i--; break;
            default: i++; break;
        }

        if (eos) { break; }
    }
    while (i < length);

    return i;
}

struct shell_t Shell =
{
    .CONFIG = &(struct shell_config_t){
        .put = uart_put,
        .get = uart_get
    }
};

int main(void) {

    shell_init(&Shell);

    // Set Pin 0 of Port B as an output
    DDRB |= (1 << PB0);

    // Initialize UART peripheral
    uart_init(MYUBRR);

    // Initialize the timer
    timer1_init();

    // Enable global interrupts
    sei();

    uint32_t last_time = 0;

    while (1) {

        shell_process(&Shell);

        if (os_time_elapsed(last_time) >= 1000)
        {
            last_time = os_time_now();

            // Toggle the LED on
            PORTB ^= (1 << PB0);
        }
    }
}
