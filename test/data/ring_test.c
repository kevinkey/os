#include "test.h"
#include "ring.h"

static void ring_test(void)
{
    uint8_t buffer[32];
    struct ring_t ring;

    ring_init(&ring, buffer, sizeof(buffer));

    TEST_EQUAL(ring_count(&ring), 0u);
    TEST_EQUAL(ring_space(&ring), 32u);

    uint8_t write_count = 0u;
    uint8_t read_count = 0u;

    for (uint8_t i = 0u; i < 11u; i++)
    {
        uint8_t write[9];

        for (uint8_t j = 0u; j < sizeof(write); j++)
        {
            write[j] = write_count;
            write_count++;
        }

        TEST_TRUE(ring_write(&ring, write, sizeof(write)));

        uint8_t read[7];

        TEST_TRUE(ring_read(&ring, read, sizeof(read)));

        for (uint8_t j = 0u; j < sizeof(read); j++)
        {
            TEST_EQUAL(read[j], read_count);
            read_count++;
        }

        TEST_EQUAL(ring_count(&ring), (i + 1) * 2u);
        TEST_EQUAL(ring_space(&ring), 32u - ((i + 1)  * 2u));
    }

    TEST_TRUE(ring_write(&ring, (uint8_t[]){1u, 2u, 3u, 4u, 5u}, 5u));
    TEST_TRUE(ring_write(&ring, (uint8_t[]){1u, 2u, 3u, 4u, 5u}, 5u));

    TEST_EQUAL(ring_count(&ring), 32u);
    TEST_EQUAL(ring_space(&ring), 0u);
}

int main(void)
{
    ring_test();

    exit(EXIT_SUCCESS);
}
