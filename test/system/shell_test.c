#include "test.h"
#include "shell.h"
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

struct shell_t Shell = {.PUT = test_put, .GET = test_get};

int main(void)
{
    shell_init(&Shell);
    shell_process(&Shell);

    exit(EXIT_SUCCESS);
}
