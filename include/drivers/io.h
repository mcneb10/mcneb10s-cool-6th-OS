#ifndef OS6_IO_H
#define OS6_IO_H

#include <stdint.h>

void outb(uint16_t port, uint8_t b);

uint8_t inb(uint16_t port);

void outl(uint16_t port, uint32_t b);

uint32_t inl(uint16_t port);

#define io_wait() outb(0x80, 0)

#define cli() __asm__("cli")
#define sti() __asm__("sti")

#define pause() __asm__("pause")

#endif //OS6_IO_H
