#include "cpuid.h"

uint8_t supportsCPUID() {
return (cpuHelper() >> 21) & 1;
}