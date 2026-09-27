#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "shell.h"
#include "numstr.h"
#include "os.h"
#include "os_task.h"
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

ISR(TIMER1_COMPA_vect, ISR_NAKED)
{
    STACK_SAVE();
    uint8_t * stack = (uint8_t * )SP;

    stack = os_tick(1, stack);

    SP = (uint16_t)stack;
    STACK_LOAD();

    __asm__ __volatile__ ("reti");
}

void uart_put(char c)
{
    uart_write(&Uart, (uint8_t *)&c, 1);
}

extern struct shell_t Shell;

char uart_get(void)
{
    uint8_t byte;
    while (!uart_read(&Uart, &byte, 1, 0))
    {
        os_task_wait(&Shell.task, NULL, 1);
    }

    return byte;
}

static void shell_func(void);

struct shell_t Shell =
{
    .task = {
        .NAME = "SHELL",
        .FUNCTION = shell_func,
        .STACK_SIZE = 256,
        .PRIORITY = 10,
    },
    .PUT = uart_put,
    .GET = uart_get
};

static void shell_func(void)
{
    while (true) { shell_process(&Shell); }
}

static void Blink(void);

struct os_task_t Blinky = {
    .NAME = "BLINK",
    .FUNCTION = Blink,
    .STACK_SIZE = 128,
    .PRIORITY = 10,
};

static void Blink(void)
{
    while (true)
    {
        os_task_wait(&Blinky, NULL, 1000);
        PORTB ^= (1 << PB0);
    }
}

int main(void) {

    // Set Pin 0 of Port B as an output
    DDRB |= (1 << PB0);

    os_init();
    os_task_init(&Blinky);

    uart_init(&Uart);
    uart_config(&Uart, 9600, UART_PARITY_NONE, UART_STOP_1);
    uart_enable(&Uart, true, true);

    shell_init(&Shell);

    // Initialize the timer
    timer1_init();

    os_start();
}
