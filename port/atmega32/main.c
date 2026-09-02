#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "shell.h"
#include "numstr.h"
#include "os_time.h"
#include "uart_atmega32.h"
#include <string.h>

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

void uart_put(char const str[])
{
    size_t len = strlen(str);

    uart_write(&Uart, (uint8_t const *)str, len);
    if (str[len - 1] == '\n') { uart_write(&Uart, (uint8_t[]){'\r'}, 1); }
}

size_t uart_get(char str[], size_t length)
{
    size_t i = 0;

    do
    {
        bool eos = false;

        if (uart_read(&Uart, (uint8_t *)&str[i], 1, 0))
        {
            switch(str[i])
            {
                case '\n':
                case '\r':
                    uart_write(&Uart, (uint8_t *)"\r\n", 2);
                    eos = true;
                    break;
                case '\b':
                    if (i > 0)
                    {
                        uart_write(&Uart, "\b", 1);
                        i--;
                    }
                    break;
                default:
                    uart_write(&Uart, (uint8_t *)&str[i], 1);
                    i++;
                    break;
            }

            if (eos) { break; }
        }
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

    uart_init(&Uart);
    uart_config(&Uart, 9600, UART_PARITY_NONE, UART_STOP_1);
    uart_enable(&Uart, true, true);

    // Set Pin 0 of Port B as an output
    DDRB |= (1 << PB0);

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
