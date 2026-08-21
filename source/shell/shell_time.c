#include "shell_time.h"
#include "time.h"
#include <string.h>

static bool time_cmd(struct shell_t * shell)
{
    (void)time_string(shell->out, SHELL_LINE_SIZE);
    strcat(shell->out, "\n");

    shell_put(shell, shell->out);
}

struct shell_cmd_t Shell_Time =
{
    .NAME = "time",
    .DESC = "Prints the current system time",
    .FUNCTION = time_cmd,
};
