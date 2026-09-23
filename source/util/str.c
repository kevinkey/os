#include "str.h"

size_t str_len(char const str[])
{
    size_t len = 0;

    while (str[len] != '\0') { len++; }

    return len;
}

void str_copy(char str[], char const copy[])
{
    size_t len = str_len(copy) + 1;

    for (size_t i = 0; i < len; i++)
    {
        str[i] = copy[i];
    }
}

void str_append(char str[], char const append[])
{
    str_copy(&str[str_len(str)], append);
}

void str_pad(char str[], size_t len, char pad)
{
    for (size_t i = str_len(str); i < len; i++) { str[i] = pad; }
    str[len] = '\0';
}

void str_dec(char str[], int32_t num, uint8_t digits, char pad)
{
    char dec[] = "0123456789";
    char temp[11];

    temp[10] = '\0';
    uint8_t index = 10;

    for (uint8_t i = 0; ((i < digits) || (digits == 0)) && (index > 0); i++)
    {
        if ((num == 0) && (digits == 0))
        {
            break;
        }
        else if (num == 0)
        {
            temp[--index] = pad;
        }
        else
        {
            temp[--index] = dec[num % 10];
            num /= 10;
        }

    }

    str_append(str, &temp[index]);
}

char * str_split(char str[], char delimiter)
{
    static char * prev = NULL;

    char * current = (str == NULL) ? prev : str;

    if (current == NULL) { return NULL; }

    for (size_t i = 0; current[i] != '\0'; i++)
    {
        if (current[i] == delimiter)
        {
            current[i] = '\0';
            prev = &current[i + 1];
            return current;
        }
    }

    prev = NULL;
    return current;
}

char * str_find(char str[], char c)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == c) { return &str[i]; }
    }

    return NULL;
}

void str_trim(char str[])
{
    for (size_t i = 0; str[i] != '\0'; i++ )
    {
        if (str_find(" \t\n\r\f\v", str[i]) == NULL)
        {
            if (i != 0) { str_copy(str, &str[i]); }
            break;
        }
    }

    for (size_t i = str_len(str); i != 0; i--)
    {
        if (str_find(" \t\n\r\f\v", str[i - 1]) == NULL) { break; }
        else { str[i - 1] = '\0'; }
    }
}
