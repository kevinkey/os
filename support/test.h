#ifndef TEST_H
#define TEST_H

#include "types.h"
#include <stdio.h>
#include <stdlib.h>

#define TEST_EQUAL(value, expect) test_equal(#value, #expect, value, expect, __LINE__)

#define TEST_TRUE(value) test_true(#value, value, __LINE__)

static void test_equal(char * vstr, char * estr, int_t value, int_t expect, int_t line)
{
    if (value != expect)
    {
        fprintf(stderr,
                "\"%s\" does not equal \"%s\", expected %d got %d on line %d\n",
                vstr,
                estr,
                expect,
                value,
                line);
        exit(EXIT_FAILURE);
    }
}

static void test_true(char * vstr, bool value, int_t line)
{
    if (!value)
    {
        fprintf(stderr, "\"%s\" is not true on line %d\n", vstr, __LINE__);
        exit(EXIT_FAILURE);
    }
}

#endif
