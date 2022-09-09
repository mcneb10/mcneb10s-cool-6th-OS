#include "io.h"

void outb(uint16_t port, uint8_t b) {
    __asm__("out %%al, %%dx" : : "a" (b), "d" (port));
}

uint8_t inb(uint16_t port) {
    uint8_t r;
    __asm__("in %%dx, %%al" : "=a" (r) : "d" (port));
    return r;
}

void outl(uint16_t port, uint32_t b) {
    __asm__("outl %%eax, %%dx" : : "a" (b), "d" (port));
}

uint32_t inl(uint16_t port) {
    uint32_t r;
    __asm__("in %%dx, %%eax" : "=a" (r) : "d" (port));
    return r;
}