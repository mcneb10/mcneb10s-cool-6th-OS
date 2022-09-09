#ifndef OS6_CPUID_H
#define OS6_CPUID_H

#include <stdint.h>

/*
    This function checks to see if the CPU supports CPUID
    by changing the CPUID enable flag and seeing if the CPU saves the result
*/
uint8_t supportsCPUID();

#endif //OS6_CPUID_H
