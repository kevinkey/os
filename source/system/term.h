#ifndef TERM_H
#define TERM_H

#include "types.h"
#include "list.h"

struct term_opt_t
{
    char * NAME;
    char * DESC;
};

struct term_t
{
    struct list_t exec;
    void (*PUTS)(char const * str);
    char const * (*GETS)(void);
    bool shutdown;
};

struct term_exec_t
{
    struct list_item_t next;
    void (*FUNCTION)(struct term_t * term);
    char * NAME;
    char * DESC;
    struct term_opt_t * OPTION;
    size_t COUNT;
};

void term_init(struct term_t * term);
void term_register(struct term_t * term, struct term_exec_t * exec);
void term_process(struct term_t * term);

#endif
