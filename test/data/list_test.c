#include "test.h"
#include "list.h"

struct test_t
{
    struct list_item_t item;
    uint8_t data;
};

static void list_tail_test(void)
{
    struct list_t list;
    struct test_t pool[16];

    list_init(&list);

    for(uint8_t i = 0; i < 16; i++)
    {
        pool[i].data = i;
        list_add(&list, (struct list_item_t *)&(pool[i]), LIST_ADD_TAIL);
    }

    uint8_t count = 0;
    LIST_FOR_EACH(&list, struct test_t *, test)
    {
        TEST_EQUAL(test->data, count);
        count++;
    }

    for(uint8_t i = 0; i < 16; i += 2)
    {
        list_del(&list, (struct list_item_t *)&(pool[i]));
    }

    count = 1;
    LIST_FOR_EACH(&list, struct test_t *, test)
    {
        TEST_EQUAL(test->data, count);
        count += 2;
    }
}

static void list_head_test(void)
{
    struct list_t list;
    struct test_t pool[16];

    list_init(&list);

    for(uint8_t i = 0; i < 16; i++)
    {
        pool[i].data = i;
        list_add(&list, (struct list_item_t *)&(pool[i]), LIST_ADD_HEAD);
    }

    uint8_t count = 15;
    LIST_FOR_EACH(&list, struct test_t *, test)
    {
        TEST_EQUAL(test->data, count);
        count--;
    }

    for(uint8_t i = 0; i < 16; i += 2)
    {
        list_del(&list, (struct list_item_t *)&(pool[i]));
    }

    count = 15;
    LIST_FOR_EACH(&list, struct test_t *, test)
    {
        TEST_EQUAL(test->data, count);
        count -= 2;
    }
}

int main(void)
{
    list_tail_test();
    list_head_test();

    exit(EXIT_SUCCESS);
}
