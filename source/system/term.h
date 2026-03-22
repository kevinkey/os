#ifndef TERM_H
#define TERM_H

#include "types.h"
#include "list.h"

#ifndef TERM_LINE_SIZE
#define TERM_LINE_SIZE 80
#endif

struct term_opt_t
{
    char * NAME;
    char * DESC;
};

struct term_t
{
    struct list_t cmd;
    void (*PUT)(char const str[]);
    size_t (*GET)(char str[], size_t length);
    char in[TERM_LINE_SIZE];
    char out[TERM_LINE_SIZE];
    bool shutdown;
};

struct term_cmd_t
{
    struct list_item_t next;
    void (*FUNCTION)(struct term_t * term);
    char * NAME;
    char * DESC;
    struct term_opt_t * OPTION;
    size_t COUNT;
};

void term_init(struct term_t * term);
void term_register(struct term_t * term, struct term_cmd_t * cmd);
void term_process(struct term_t * term);

#endif
