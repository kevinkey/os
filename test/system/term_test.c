#include "test.h"
#include "term.h"
#include <stdio.h>

void test_puts(char const * str)
{
    (void)puts(str);
}

char const * test_gets(void)
{
    static char line[80];
    return fgets(line, sizeof(line), stdin);
}

struct term_t Term =
{
    .PUTS = test_puts,
    .GETS = test_gets,
};

int main(void)
{
    term_init(&Term);
    term_process(&Term);
}
