#include "shell.h"
#include "time.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static bool help_cmd(struct shell_t * shell)
{
    LIST_FOR_EACH(&shell->cmd, struct shell_cmd_t *, cmd)
    {
        snprintf(shell->out, SHELL_LINE_SIZE, "%-20s %s\n", cmd->NAME, cmd->DESC);
        shell_put(shell, shell->out);
    }
}

static struct shell_cmd_t Help =
{
    .NAME = "help",
    .DESC = "Prints a list of all registered commands",
    .FUNCTION = help_cmd,
};

static bool time_cmd(struct shell_t * shell)
{
    (void)time_string(shell->out, SHELL_LINE_SIZE);
    strcat(shell->out, "\n");

    shell_put(shell, shell->out);
}

static struct shell_cmd_t Time =
{
    .NAME = "time",
    .DESC = "Prints the current system time",
    .FUNCTION = time_cmd,
};

static bool exit_cmd(struct shell_t * shell)
{
    shell->shutdown = true;
}

static struct shell_cmd_t Exit =
{
    .NAME = "exit",
    .DESC = "Ends execution of the shell",
    .FUNCTION = exit_cmd,
};

static bool uart_cmd(struct shell_t * shell)
{
    bool valid = true;

    int_t port;
    if (!shell_integer(shell, &port))
    {
        valid = false;
        shell_put(shell, "Port value not found.\n");
    }
    else if (uart_ref())
    else
    {
        switch (shell_find(shell, (char const * []){"write", "read", "status"}, 3u))
        {
            default:
                valid = false;
                shell_put(shell, "Unrecognized command.\n");
                break;
            case 0u:
                shell_put(shell, "Write\n");
                break;
            case 1u:
                shell_put(shell, "Read\n");
                break;
            case 2u:
                shell_put(shell, "Status\n");
                break;
        }
    }

    return valid;
};

static struct shell_cmd_t Uart_Shell =
{
    .NAME = "uart",
    .DESC = "Write/read data using a uart port",
    .HELP = "Example:\n"
    "    $ uart <id> write <bytes>\n"
    "    $ uart <id> read <number>\n"
    "    $ uart <id> status\n",
    .FUNCTION = uart_cmd,
};

static struct shell_cmd_t * find_cmd(struct shell_t * shell, char const * name)
{
    struct shell_cmd_t * cmd = NULL;

    LIST_FOR_EACH(&shell->cmd, struct shell_cmd_t *, c)
    {
        if (strcmp(c->NAME, name) == 0)
        {
            cmd = c;
            break;
        }
    }

    return cmd;
}

void shell_init(struct shell_t * shell)
{
    list_init(&shell->cmd);
    shell_register(shell, &Help);
    shell_register(shell, &Time);
    shell_register(shell, &Uart);
    shell_register(shell, &Exit);
}

void shell_register(struct shell_t * shell, struct shell_cmd_t * cmd)
{
    list_add(&shell->cmd, (struct list_item_t *)cmd, LIST_ADD_TAIL);
}

void shell_process(struct shell_t * shell)
{
    shell_put(shell, "Hello world!\n");

    help_cmd(shell);

    while (!shell->shutdown)
    {
        shell_put(shell, "$ ");
        (void)shell->CONFIG->get(shell->in, SHELL_LINE_SIZE);
        char * line = shell->in;

        if (line = strtok(line, " \t\n\r\f\v"))
        {
            struct shell_cmd_t * cmd = find_cmd(shell, line);

            if (cmd == NULL)
            {
                shell_put(shell, "Command not found.\n");
            }
            else
            {
                if (cmd->FUNCTION(shell))
                {
                    /* Command was successful */
                }
                else if (cmd->HELP != NULL)
                {
                    shell_put(shell, cmd->HELP);
                }
            }
        }
    }
}

void shell_put(struct shell_t * shell, char const string[])
{
    shell->CONFIG->put(string);
}

size_t shell_find(struct shell_t * shell, char const * string[], size_t count)
{
    size_t index = count;
    char * word;

    if (word = strtok(NULL, " \t\n\r\f\v"))
    {
        for (size_t i = 0u; i < count; i++)
        {
            if (strcmp(word, string[i]) == 0)
            {
                index = i;
                break;
            }
        }
    }

    return index;
}

bool shell_integer(struct shell_t * shell, int_t * num)
{
    bool valid = false;
    char * word;

    if (word = strtok(NULL, " \t\n\r\f\v"))
    {
        valid = true;
        *num = 0;

        if ((word[0] == '0') && (word[1] == 'x'))
        {
            for (size_t i = 2u; word[i] != '\0'; i++)
            {
                *num <<= 4;

                if (RANGE(word[i], '0', '9'))
                {
                    *num |= word[i] - '0';
                }
                else if (RANGE(word[i], 'a', 'f'))
                {
                    *num |= 0xA + (word[i] - 'a');
                }
                else if (RANGE(word[i], 'A', 'F'))
                {
                    *num |= 0xA + (word[i] - 'A');
                }
                else
                {
                    valid = false;
                    break;
                }
            }
        }
        else
        {
            bool negative = (word[0] == '-');

            for (size_t i = negative ? 1u : 0u; word[i] != '\0'; i++)
            {
                *num *= 10;

                if (RANGE(word[i], '0', '9'))
                {
                    *num += word[i] - '0';
                }
                else
                {
                    valid = false;
                    break;
                }
            }

            if (negative)
            {
                *num *= -1;
            }
        }
    }

    return valid;
}
