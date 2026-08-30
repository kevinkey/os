#include "numstr.h"
#include <string.h>

size_t numstr_dec(int32_t num, char str[], size_t len)
{
    char dec[] = "0123456789";
    char temp[11];

    temp[10] = '\0';
    uint8_t index = 10;

    do
    {
        temp[--index] = dec[num % 10];
        num /= 10;
        if (num == 0) { break; }
    }
    while (index > 0);

    size_t length = NUM(temp) - index;
    memcpy(str, &temp[index], length);

    return length - 1;
}