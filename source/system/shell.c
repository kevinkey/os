#include "shell.h"
#include "time.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void help_cmd(struct shell_t * shell, int_t count, char * args[])
{
    LIST_FOR_EACH(&shell->cmd, struct shell_cmd_t *, cmd)
    {
        snprintf(shell->out, SHELL_LINE_SIZE, "%-20s %s\n", cmd->NAME, cmd->DESC);
        shell->CONFIG->put(shell->out);
    }
}

static struct shell_cmd_t Help =
{
    .NAME = "help",
    .DESC = "Prints a list of all registered commands",
    .FUNCTION = help_cmd,
};

static void time_cmd(struct shell_t * shell, int_t count, char * args[])
{
    (void)time_string(shell->out, SHELL_LINE_SIZE);
    strcat(shell->out, "\n");

    shell->CONFIG->put(shell->out);
}

static struct shell_cmd_t Time =
{
    .NAME = "time",
    .DESC = "Prints the current system time",
    .FUNCTION = time_cmd,
};

static void exit_cmd(struct shell_t * shell, int_t count, char * args[])
{
    shell->shutdown = true;
}

static struct shell_cmd_t Exit =
{
    .NAME = "exit",
    .DESC = "Ends execution of the shell",
    .FUNCTION = exit_cmd,
};

static void uart_cmd(struct shell_t * shell, int_t count, char * args[])
{
    int32_t port = strtol(args[0], NULL, 0);

    printf("port: %d\n", port);
};

static struct shell_cmd_t Uart =
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
    shell->CONFIG->put("Hello world!\n");

    help_cmd(shell, 0, NULL);

    while (!shell->shutdown)
    {
        shell->CONFIG->put("$ ");
        (void)shell->CONFIG->get(shell->in, SHELL_LINE_SIZE);
        char * line = shell->in;

        if (line = strtok(line, " \t\n\r\f\v"))
        {
            struct shell_cmd_t * cmd = find_cmd(shell, line);

            if (cmd == NULL)
            {
                shell->CONFIG->put("Command not found.\n");
            }
            else
            {
                int_t count = 0u;
                char * args[SHELL_MAX_ARGS];
                bool help = false;

                while (line = strtok(NULL, " \t\n\r\f\v"))
                {
                    help = (0 == memcmp("help", line, 4u));
                    args[count] = line;
                    count++;
                }

                if (help)
                {
                    shell->CONFIG->put(cmd->HELP);
                }
                else
                {
                    cmd->FUNCTION(shell, count, args);
                }
            }
        }
    }
}
