#define F_CPU 8000000UL // Define clock speed (change if using a different crystal)
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "numstr.h"

// Volatile variable to track milliseconds elapsed
volatile uint32_t millis_count = 0;

// Initialize UART
void uart_init(unsigned int ubrr) {
    // Set baud rate registers
    UBRRH = (unsigned char)(ubrr >> 8);
    UBRRL = (unsigned char)ubrr;
    // Enable transmitter
    UCSRB = (1 << TXEN);
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

ISR(TIMER1_COMPA_vect) {
    millis_count++;
}

// Transmit a single character
void uart_transmit(char data) {
    // Wait for empty transmit buffer
    while (!(UCSRA & (1 << UDRE)));
    // Put data into buffer, sends the data
    UDR = data;
}

// Transmit a string
void uart_print(const char* str) {
    while (*str) {
        uart_transmit(*str++);
    }
}

void uart_print_num(uint32_t num)
{
    char str[11];

    numstr_dec(num, str, NUM(str));

    uart_print(str);
}

int main(void) {
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

        if ((millis_count - last_time) >= 1000)
        {
            last_time = millis_count;

            // Send string to FTDI chip
            uart_print("[");
            uart_print_num(millis_count);
            uart_print("] Hello World!\r\n");

            // Toggle the LED on
            PORTB ^= (1 << PB0);
        }
    }
}
