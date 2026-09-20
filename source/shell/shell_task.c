#include "shell_task.h"
#include "os_task.h"
#include "str.h"

static bool task_cmd(struct shell_t * shell)
{
    LIST_FOR_EACH(&os_tasks, struct os_task_t *, task)
    {
        char str[32] = "";

        str_copy(str, task->CONFIG->name);
        str_append(str, " ");

        size_t usage = os_task_usage(task);
        str_dec(str, usage, 6, ' ');
        str_append(str, " ");

        str_dec(str, task->CONFIG->size, 6, ' ');
        str_append(str, "\n");

        shell_put(shell, str);
    }

    return true;
}

struct shell_cmd_t Shell_Task =
{
    .NAME = "task",
    .DESC = "Prints the task information for each task",
    .FUNCTION = task_cmd,
};
