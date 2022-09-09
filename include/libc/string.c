#include "string.h"


uint32_t strlen(const char* string) {
    uint32_t len = 0;
    while(string[len++] != '\0');
    return len-1;
}