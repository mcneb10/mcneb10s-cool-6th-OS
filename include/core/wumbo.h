#ifndef OS6_WUMBO_H
#define OS6_WUMBO_H

#define MAX_WUMBO_SIZE 69420

/*
 * The opposite of mini
 */

#include <stdarg.h>
#include <stdint.h>
#include "../libc/stdio.h"

void __wumbo(const char* file, const char* function, uint32_t line, char* fmt, ...);

#define wumbo(fmt, ...) __wumbo(__FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define wumbo_exit_func(fmt, ...) __wumbo(__FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__); return
#define wumbo_exit_die(fmt, ...) __wumbo(__FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__); hang()


#endif //OS6_WUMBO_H
