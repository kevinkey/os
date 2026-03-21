#include "term.h"
#include <string.h>

static void help_exec(struct term_t * term)
{
    LIST_FOR_EACH(&term->exec, i)
    {
        struct term_exec_t * exec = (struct term_exec_t *)i;

        term->PUTS(exec->NAME);
    }
}

struct term_exec_t Help =
{
    .NAME = "help",
    .DESC = "Prints a list of all registered commands",
    .OPTION = NULL,
    .COUNT = 0u,
    .FUNCTION = help_exec,
};

void term_init(struct term_t * term)
{
    list_init(&term->exec);
    term_register(term, &Help);
}

void term_register(struct term_t * term, struct term_exec_t * exec)
{
    list_add(&term->exec, (struct list_item_t *)exec, LIST_ADD_TAIL);
}

void term_process(struct term_t * term)
{
    term->PUTS("Hello world!");

    while (!term->shutdown)
    {
        char * line = term->GETS();

        if (line = strtok(line, " \t\n\r\f\v"))
        {
            struct term_exec_t * exec = NULL;

            LIST_FOR_EACH(&term->exec, i)
            {
                struct term_exec_t * e = (struct term_exec_t *)i;

                if (strcmp(e->NAME, line) == 0)
                {
                    exec = e;
                    break;
                }
            }

            if (exec == NULL)
            {
                term->PUTS("Command not found.");
            }
            else
            {
                term->PUTS("Command found!");
                exec->FUNCTION(term);
            }
        }
    }
}
