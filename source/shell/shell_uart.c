#include "shell_uart.h"
#include "uart.h"

static bool write(struct shell_t * shell, struct uart_t * uart)
{
    uint8_t bytes[16];
    size_t count;

    for (count = 0u; count < NUM(bytes); count++)
    {
        int_t num;
        if (!shell_getnum(shell, &num))
        {
            break;
        }
        else if (!RANGE(num, 0, 255))
        {
            shell_put(shell, "Invalid value to write.\n");
            return false;
        }
        else
        {
            bytes[count] = (uint8_t)num;
        }
    }

    if (count != 0u)
    {
        uart_write(uart, bytes, count);
    }

    return true;
}

static bool read(struct shell_t * shell, struct uart_t * uart)
{
    int_t num;
    if (!shell_getnum(shell, &num))
    {
        num = uart_bytes(uart);
    }

    uint8_t bytes[16];

    while (num > 0)
    {
        if (!uart_read(uart, bytes, MIN(16, num), 0u))
        {
            shell_put(shell, "Insufficient bytes available.\n");
            return false;
        }
        shell_putbytes(shell, bytes, MIN(16, num));
        num -= 16;
    }

    return true;
}

static bool status(struct shell_t * shell, struct uart_t * uart)
{
    shell_put(shell, "Available: ");
    shell_putnum(shell, (int_t)uart_bytes(uart));
    shell_put(shell, "Overflow: ");
    shell_putnum(shell, uart_overflow(uart) ? 1 : 0);

    return true;
}

static bool uart_cmd(struct shell_t * shell)
{
    bool valid = false;
    int_t id;
    struct uart_t * uart;

    if (!shell_getnum(shell, &id))
    {
        shell_put(shell, "Invalid value for ID.\n");
    }
    else if (!RANGE(id, 0, 255))
    {
        shell_put(shell, "ID value should be [0, 255].\n");
    }
    else if ((uart = uart_ref((uint8_t)id)) == NULL)
    {
        shell_put(shell, "ID value not found.\n");
    }
    else
    {
        valid = true;

        switch (shell_find(shell, (char const * []){"write", "read", "status"}, 3u))
        {
            default:
                valid = false;
                shell_put(shell, "Unrecognized command.\n");
                break;
            case 0u:
                valid = write(shell, uart);
                break;
            case 1u:
                valid = read(shell, uart);
                break;
            case 2u:
                valid = status(shell, uart);
                break;
        }
    }

    return valid;
};

struct shell_cmd_t Shell_Uart =
{
    .NAME = "uart",
    .DESC = "Write/read data using a uart port",
    .HELP = "Example:\n"
    "    $ uart <id> write <bytes>\n"
    "    $ uart <id> read <number>\n"
    "    $ uart <id> status\n",
    .FUNCTION = uart_cmd,
};
