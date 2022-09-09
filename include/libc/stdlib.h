#ifndef OS6_STDLIB_H
#define OS6_STDLIB_H

#include <stdint.h>
#include <stddef.h>
#include "string.h"

char* itoa(uint64_t n, char* buf, uint16_t radix);

#endif //OS6_STDLIB_H
