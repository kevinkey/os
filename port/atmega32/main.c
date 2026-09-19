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

void UART_TxChar(char data) {
    // Wait until the transmit buffer (UDR) is empty and ready for new data
    while (!(UCSRA & (1 << UDRE))) {
        // Do nothing, just loop (this is the blocking part)
    }

    // Put data into the buffer, which sends the byte
    UDR = data;
}

// 3. Blocking Transmit for an entire string
void UART_TxString(const char *str) {
    while (*str) {
        UART_TxChar(*str);
        str++;
    }
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
        .CONFIG = &(struct task_config_t){
            .name = "SHELL\r\n",
            .func = shell_func,
            .priority = TASK_PRIORITY_NORMAL,
            .size = 256,
            .stack = (uint8_t[256]){0}
        }
    },
    .CONFIG = &(struct shell_config_t){
        .put = uart_put,
        .get = uart_get
    }
};

static void shell_func(void)
{
    while (true) { shell_process(&Shell); }
}

static void Blink(void);

struct os_task_t Blinky = {
    .CONFIG = &(struct task_config_t){
        .name = "BLINK\r\n",
        .func = Blink,
        .priority = TASK_PRIORITY_NORMAL,
        .size = 128,
        .stack = (uint8_t[128]){0}
    }
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

    // 1. Read the current status register
    uint8_t reset_reason = MCUCSR;

    // 2. Clear the flags right away so the next reset records accurately
    MCUCSR = 0x00;

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

    // 3. Process the results
    if (reset_reason & (1 << PORF)) {
        UART_TxString("POR\r\n");
    }
    if (reset_reason & (1 << EXTRF)) {
        UART_TxString("EXT\r\n");
    }
    if (reset_reason & (1 << BORF)) {
        UART_TxString("BOR\r\n");
    }
    if (reset_reason & (1 << WDRF)) {
        UART_TxString("WDR\r\n");
    }
    if (reset_reason & (1 << JTRF)) {
        UART_TxString("JTR\r\n");
    }

    UART_TxString("Starting...\r\n");

    os_start();
}
