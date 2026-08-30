#include "shell_time.h"
#include "os_time.h"
#include "numstr.h"
#include <string.h>

static bool time_cmd(struct shell_t * shell)
{
    struct os_time_t const * clock = os_time_clock();
    char str[32] = "";

    numstr_dec(str, clock->day, 2);
    strcat(str, ":");
    numstr_dec(str, clock->hr, 2);
    strcat(str, ":");
    numstr_dec(str, clock->min, 2);
    strcat(str, ":");
    numstr_dec(str, clock->sec, 2);
    strcat(str, ".");
    numstr_dec(str, clock->ms, 3);
    strcat(str, "\n");

    shell_put(shell, str);

    return true;
}

struct shell_cmd_t Shell_Time =
{
    .NAME = "time",
    .DESC = "Prints the current system time",
    .FUNCTION = time_cmd,
};
