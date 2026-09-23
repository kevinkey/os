#ifndef STR_H
#define STR_H

#include "types.h"

size_t str_len(char const str[]);
void str_copy(char str[], char const copy[]);
void str_append(char str[], char const append[]);
void str_pad(char str[], size_t len, char pad);
void str_dec(char str[], int32_t num, uint8_t digits, char pad);
char * str_split(char str[], char delimiter);
void str_trim(char str[]);

#endif
