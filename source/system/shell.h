#ifndef SHELL_H
#define SHELL_H

#include "types.h"
#include "list.h"

#ifndef SHELL_LINE_SIZE
#define SHELL_LINE_SIZE 80
#endif

struct shell_config_t
{
    void (*put)(char const str[]);
    size_t (*get)(char str[], size_t length);
};

struct shell_t
{
    struct list_t cmd;
    struct shell_config_t const * CONFIG;
    char in[SHELL_LINE_SIZE];
    char out[SHELL_LINE_SIZE];
    bool shutdown;
};

struct shell_cmd_t
{
    struct list_item_t next;
    bool (*FUNCTION)(struct shell_t * shell);
    char * NAME;
    char * DESC;
    char * HELP;
};

void shell_init(struct shell_t * shell);
void shell_register(struct shell_t * shell, struct shell_cmd_t * cmd);
void shell_process(struct shell_t * shell);
void shell_put(struct shell_t * shell, char const string[]);
size_t shell_find(struct shell_t * shell, char const * string[], size_t count);
bool shell_integer(struct shell_t * shell, int_t * num);

#endif
