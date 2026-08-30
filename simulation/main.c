#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "support/time_sim.h"
#include "support/uart_sim.h"

#include "system.h"
#include "shell.h"

bool Shutdown = false;

void test_put(char const str[])
{
    printf("%s", str);
}

size_t test_get(char str[], size_t length)
{
    return strlen(fgets(str, length, stdin));
}

struct shell_t Shell =
{
    .CONFIG = &(struct shell_config_t){
        .put = test_put,
        .get = test_get
    }
};

int main(void)
{
    system_init();

    time_sim_init();
    uart_sim_init();

    shell_init(&Shell);
    shell_process(&Shell);

    system_start();

    Shutdown = true;
    time_sim_deinit();
    uart_sim_deinit();

    exit(EXIT_SUCCESS);
}
