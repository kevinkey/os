#include "list.h"

void list_init(struct list_t * list)
{
    list->head = NULL;
    list->tail = NULL;
}

void list_add(struct list_t * list, struct list_item_t * item, enum list_add_t add)
{
    switch(add)
    {
        case LIST_ADD_HEAD:
            item->prev = NULL;
            item->next = list->head;

            if(item->next != NULL)
            {
                item->next->prev = item;
            }
            else
            {
                list->tail = item;
            }

            list->head = item;
            break;
        case LIST_ADD_TAIL:
        default:
            item->prev = list->tail;
            item->next = NULL;

            if(item->prev != NULL)
            {
                item->prev->next = item;
            }
            else
            {
                list->head = item;
            }

            list->tail = item;
            break;
    }
}

void list_del(struct list_t * list, struct list_item_t * item)
{
    if(list->head == item)
    {
        list->head = item->next;
    }
    else if(item->prev != NULL)
    {
        item->prev->next = item->next;
    }

    if(list->tail == item)
    {
        list->tail = item->prev;
    }
    else if(item->next != NULL)
    {
        item->next->prev = item->prev;
    }

    item->next = NULL;
    item->prev = NULL;
}
