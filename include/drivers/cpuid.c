#include "cpuid.h"
#include <cpuid.h>

uint8_t cpuid_supported() {
    return __get_cpuid_max(0, 0) != 0;
}

uint8_t cpuid_supportsLeaf(uint32_t leaf) {
    uint32_t a, _;
    __get_cpuid(0, &a, &_, &_, &_);
    return a >= leaf;
}

uint8_t cpuid_vendorString(char* out) {
    uint32_t a, b, c, d;
    __get_cpuid(0, &a, &b, &c, &d);
    out[0] = (char)b;
    out[1] = (char)(b>>8);
    out[2] = (char)(b>>16);
    out[3] = (char)(b>>24);

    out[4] = (char)d;
    out[5] = (char)(d>>8);
    out[6] = (char)(d>>16);
    out[7] = (char)(d>>24);

    out[8] = (char)c;
    out[9] = (char)(c>>8);
    out[10] = (char)(c>>16);
    out[11] = (char)(c>>24);
    return b | c | d;
}

uint8_t cpuid_brandString(char* out) {
    if(cpuid_supportsLeaf(0x80000004)) {
        uint32_t string[12];
        __get_cpuid(0x80000002, string, string+1, string+2, string+3);
        __get_cpuid(0x80000003, string+4, string+5, string+6, string+7);
        __get_cpuid(0x80000004, string+8, string+9, string+10, string+11);
        uint8_t x;
        for(uint8_t i=0;i<12;i++) {
            x = i*4;
            out[x] = (char)string[i];
            out[x+1] = (char)(string[i] >> 8);
            out[x+2] = (char)(string[i] >> 16);
            out[x+3] = (char)(string[i] >> 24);
        }
        return 1;
    }
    return 0;
}