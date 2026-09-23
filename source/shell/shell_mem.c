#include "shell_mem.h"
#include "mem.h"
#include "str.h"

static bool mem_cmd(struct shell_t * shell)
{
    char str[20] = "";

    str_copy(str, "Total: ");
    str_dec(str, MEM_SIZE, 0, ' ');
    str_append(str, "\n");
    shell_put(shell, str);

    str_copy(str, "Used: ");
    str_dec(str, mem_used(), 0, ' ');
    str_append(str, "\n");
    shell_put(shell, str);

    return true;
}

struct shell_cmd_t Shell_Mem =
{
    .NAME = "mem",
    .DESC = "Prints memory total and used",
    .FUNCTION = mem_cmd,
};
