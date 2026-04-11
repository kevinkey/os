#include "test.h"
#include "term.h"
#include <stdio.h>
#include <string.h>

void test_put(char const str[])
{
    printf("%s", str);
}

size_t test_get(char str[], size_t length)
{
    return strlen(fgets(str, length, stdin));
}

struct term_t Term = {.PUT = test_put, .GET = test_get};

int main(void)
{
    term_init(&Term);
    term_process(&Term);

    exit(EXIT_SUCCESS);
}
