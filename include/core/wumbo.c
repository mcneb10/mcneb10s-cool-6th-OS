#include "wumbo.h"

void __wumbo(const char* file, const char* function, uint32_t line, char* fmt, ...) {
    va_list params;
    va_start(params, fmt);
    char wumboString[MAX_WUMBO_SIZE];
    vsprintf(wumboString, fmt, params);
    printf("Wumbo: %s\nWumbology: Occurred in File %s\nFunction: %s\nLine: %u\n", wumboString, file, function, line);
    va_end(params);
}