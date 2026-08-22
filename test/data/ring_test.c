#include "test.h"
#include "ring.h"

static void ring_test(void)
{
    uint8_t buffer[32];
    struct ring_t ring =
    {
        .CONFIG = &(struct ring_config_t){
            .buffer = buffer,
            .length = sizeof(buffer)
        }
    };

    ring_init(&ring);

    TEST_EQUAL(ring_count(&ring), 0);
    TEST_EQUAL(ring_space(&ring), 32);

    uint8_t write_count = 0;
    uint8_t read_count = 0;

    for (uint8_t i = 0; i < 11; i++)
    {
        uint8_t write[9];

        for (uint8_t j = 0; j < sizeof(write); j++)
        {
            write[j] = write_count;
            write_count++;
        }

        TEST_TRUE(ring_write(&ring, write, sizeof(write)));

        uint8_t read[7];

        TEST_TRUE(ring_read(&ring, read, sizeof(read)));

        for (uint8_t j = 0; j < sizeof(read); j++)
        {
            TEST_EQUAL(read[j], read_count);
            read_count++;
        }

        TEST_EQUAL(ring_count(&ring), (i + 1) * 2);
        TEST_EQUAL(ring_space(&ring), 32 - ((i + 1)  * 2));
    }

    TEST_TRUE(ring_write(&ring, (uint8_t[]){1, 2, 3, 4, 5}, 5));
    TEST_TRUE(ring_write(&ring, (uint8_t[]){1, 2, 3, 4, 5}, 5));

    TEST_EQUAL(ring_count(&ring), 32);
    TEST_EQUAL(ring_space(&ring), 0);
}

int main(void)
{
    ring_test();

    exit(EXIT_SUCCESS);
}
