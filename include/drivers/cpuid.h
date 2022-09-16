#ifndef OS6_CPUID_H
#define OS6_CPUID_H

#include <stdint.h>

/*
    This function checks to see if the CPU supports CPUID
    by changing the CPUID enable flag and seeing if the CPU saves the result
*/
uint8_t cpuid_supported();

uint8_t cpuid_supportsLeaf(uint32_t leaf);

uint8_t cpuid_vendorString(char* out);

uint8_t cpuid_brandString(char* out);

#endif //OS6_CPUID_H
