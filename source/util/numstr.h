#ifndef NUMSTR_H
#define NUMSTR_H

#include "types.h"

void numstr_dec(char str[], int32_t num, uint8_t digits);
void numstr_hex(char str[], uint8_t const bytes[], size_t length);

#endif
