#ifndef OS6_STDIO_H
#define OS6_STDIO_H

#include <stdint.h>
#include <stdarg.h>
#include "stdlib.h"

#include "../core/mem.h"
// TODO: don't let user space processes access kernel functions
#include "../core/utils.h"

#define MAX_PRINTF_OUTPUT_SIZE 5000

int sprintf(char* result, char* restrict fmt, ...);
int vsprintf(char* result, char* restrict fmt, va_list params);
int printf(char* restrict fmt, ...);

#endif //OS6_STDIO_H
