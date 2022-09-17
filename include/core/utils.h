#ifndef OS6_UTILS_H
#define OS6_UTILS_H

#include "../drivers/tty.h"

void hang();

#define breakpoint() __asm__("int3")

#endif