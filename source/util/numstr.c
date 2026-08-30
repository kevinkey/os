#include "numstr.h"
#include <string.h>

void numstr_dec(char str[], int32_t num, uint8_t digits)
{
    char dec[] = "0123456789";
    char temp[11];

    temp[10] = '\0';
    uint8_t index = 10;

    for (uint8_t i = 0; ((i < digits) || (digits == 0)) && (index > 0); i++)
    {
        temp[--index] = dec[num % 10];
        num /= 10;
        if ((num == 0) && (digits == 0)) { break; }
    }

    strcat(str, &temp[index]);
}

void numstr_hex(char str[], uint8_t const bytes[], size_t length)
{
    char hex[] = "0123456789abcdef";

    strcat(str, "0x");

    char hstr[] = "XX ";
    for (size_t i = 0; i < length; i++)
    {
        hstr[0] = hex[bytes[i] >> 4];
        hstr[1] = hex[bytes[i] & 0xf];

        strcat(str, hstr);
    }
}
