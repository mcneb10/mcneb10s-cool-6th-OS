#include "stdlib.h"

char* itoa(uint64_t n, char* buf, uint16_t radix) {
    if(radix<2) return NULL;
    uint64_t num = n;
    uint8_t i=0;
    if(num == 0) {
        buf[0] = '0';
        i++;
    } else {
        while(num != 0) {
            uint8_t dig = num % radix;
            char c = '?';
            if(dig > 9) {
                c = 'A' + (dig-10);
            } else {
                c = '0' + dig;
            }
            buf[i] = c;
            num/=radix;
            i++;
        }
    }
    buf[i] = '\0';
    uint8_t t;
    uint32_t x,y;
    for(x=0,y=strlen(buf)-1;x<y;x++,y--) {
        t = buf[x];
        buf[x] = buf[y];
        buf[y] = t;
    }
    return buf;
}