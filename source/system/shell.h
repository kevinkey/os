#ifndef SHELL_H
#define SHELL_H

#include "types.h"
#include "list.h"

#ifndef SHELL_LINE_SIZE
#define SHELL_LINE_SIZE 80
#endif

struct shell_opt_t
{
    char * NAME;
    char * DESC;
};

struct shell_t
{
    struct list_t cmd;
    void (*PUT)(char const str[]);
    size_t (*GET)(char str[], size_t length);
    char in[SHELL_LINE_SIZE];
    char out[SHELL_LINE_SIZE];
    bool shutdown;
};

struct shell_cmd_t
{
    struct list_item_t next;
    void (*FUNCTION)(struct shell_t * shell);
    char * NAME;
    char * DESC;
    struct shell_opt_t * OPTION;
    size_t COUNT;
};

void shell_init(struct shell_t * shell);
void shell_register(struct shell_t * shell, struct shell_cmd_t * cmd);
void shell_process(struct shell_t * shell);

#endif
