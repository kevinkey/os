#include "term.h"
#include "time.h"
#include <stdio.h>
#include <string.h>

static void help_cmd(struct term_t * term)
{
    LIST_FOR_EACH(&term->cmd, i)
    {
        struct term_cmd_t * cmd = (struct term_cmd_t *)i;

        snprintf(term->out, TERM_LINE_SIZE, "%-20s %s\n", cmd->NAME, cmd->DESC);
        term->PUT(term->out);
    }
}

static struct term_cmd_t Help =
{
    .NAME = "help",
    .DESC = "Prints a list of all registered commands",
    .OPTION = NULL,
    .COUNT = 0u,
    .FUNCTION = help_cmd,
};

static void time_cmd(struct term_t * term)
{
    (void)time_string(term->out, TERM_LINE_SIZE);
    strcat(term->out, "\n");

    term->PUT(term->out);
}

static struct term_cmd_t Time =
{
    .NAME = "time",
    .DESC = "Prints the current system time",
    .OPTION = NULL,
    .COUNT = 0u,
    .FUNCTION = time_cmd,
};

static void exit_cmd(struct term_t * term)
{
    term->shutdown = true;
}

static struct term_cmd_t Exit =
{
    .NAME = "exit",
    .DESC = "Ends execution of the terminal",
    .OPTION = NULL,
    .COUNT = 0u,
    .FUNCTION = exit_cmd,
};

static struct term_cmd_t * find_cmd(struct term_t * term, char const * name)
{
    struct term_cmd_t * cmd = NULL;

    LIST_FOR_EACH(&term->cmd, i)
    {
        struct term_cmd_t * c = (struct term_cmd_t *)i;

        if (strcmp(c->NAME, name) == 0)
        {
            cmd = c;
            break;
        }
    }

    return cmd;
}

void term_init(struct term_t * term)
{
    list_init(&term->cmd);
    term_register(term, &Help);
    term_register(term, &Time);
    term_register(term, &Exit);
}

void term_register(struct term_t * term, struct term_cmd_t * cmd)
{
    list_add(&term->cmd, (struct list_item_t *)cmd, LIST_ADD_TAIL);
}

void term_process(struct term_t * term)
{
    term->PUT("Hello world!\n");

    help_cmd(term);

    while (!term->shutdown)
    {
        term->PUT("$ ");
        (void)term->GET(term->in, TERM_LINE_SIZE);
        char * line = term->in;

        if (line = strtok(line, " \t\n\r\f\v"))
        {
            struct term_cmd_t * cmd = find_cmd(term, line);

            if (cmd == NULL)
            {
                term->PUT("Command not found.\n");
            }
            else
            {
                cmd->FUNCTION(term);
            }
        }
    }
}
