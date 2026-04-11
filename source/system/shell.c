#include "shell.h"
#include "time.h"
#include <stdio.h>
#include <string.h>

static void help_cmd(struct shell_t * shell)
{
    LIST_FOR_EACH(&shell->cmd, i)
    {
        struct shell_cmd_t * cmd = (struct shell_cmd_t *)i;

        snprintf(shell->out, SHELL_LINE_SIZE, "%-20s %s\n", cmd->NAME, cmd->DESC);
        shell->CONFIG->put(shell->out);
    }
}

static struct shell_cmd_t Help =
{
    .NAME = "help",
    .DESC = "Prints a list of all registered commands",
    .OPTION = NULL,
    .COUNT = 0u,
    .FUNCTION = help_cmd,
};

static void time_cmd(struct shell_t * shell)
{
    (void)time_string(shell->out, SHELL_LINE_SIZE);
    strcat(shell->out, "\n");

    shell->CONFIG->put(shell->out);
}

static struct shell_cmd_t Time =
{
    .NAME = "time",
    .DESC = "Prints the current system time",
    .OPTION = NULL,
    .COUNT = 0u,
    .FUNCTION = time_cmd,
};

static void exit_cmd(struct shell_t * shell)
{
    shell->shutdown = true;
}

static struct shell_cmd_t Exit =
{
    .NAME = "exit",
    .DESC = "Ends execution of the shellinal",
    .OPTION = NULL,
    .COUNT = 0u,
    .FUNCTION = exit_cmd,
};

static struct shell_cmd_t * find_cmd(struct shell_t * shell, char const * name)
{
    struct shell_cmd_t * cmd = NULL;

    LIST_FOR_EACH(&shell->cmd, i)
    {
        struct shell_cmd_t * c = (struct shell_cmd_t *)i;

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
    shell_register(shell, &Exit);
}

void shell_register(struct shell_t * shell, struct shell_cmd_t * cmd)
{
    list_add(&shell->cmd, (struct list_item_t *)cmd, LIST_ADD_TAIL);
}

void shell_process(struct shell_t * shell)
{
    shell->CONFIG->put("Hello world!\n");

    help_cmd(shell);

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
                cmd->FUNCTION(shell);
            }
        }
    }
}
