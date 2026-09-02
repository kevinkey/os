#include "shell_mem.h"
#include "mem.h"
#include "numstr.h"
#include <string.h>

static bool mem_cmd(struct shell_t * shell)
{
    char str[32] = "";

    strcat(str, "Total: ");
    numstr_dec(str, MEM_SIZE, 0);
    strcat(str, "\n");
    shell_put(shell, str);

    str[0] = '\0';
    strcat(str, "Used: ");
    numstr_dec(str, mem_used(), 0);
    strcat(str, "\n");
    shell_put(shell, str);

    return true;
}

struct shell_cmd_t Shell_Mem =
{
    .NAME = "mem",
    .DESC = "Prints memory total and used",
    .FUNCTION = mem_cmd,
};
