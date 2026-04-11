#ifndef LIST_H
#define LIST_H

#include "types.h"

struct list_item_t
{
    struct list_item_t * prev;
    struct list_item_t * next;
};

struct list_t
{
    struct list_item_t * head;
    struct list_item_t * tail;
};

enum list_add_t
{
    LIST_ADD_HEAD,
    LIST_ADD_TAIL,
};

void list_init(struct list_t * list);
void list_add(struct list_t * list, struct list_item_t * item, enum list_add_t add);
void list_del(struct list_t * list, struct list_item_t * item);

#define LIST_FOR_EACH(l, t, i)                                                  \
    for (t i = (t)((l)->head); (i) != NULL; (i) = (t)(((struct list_item_t *)i)->next))

#endif
