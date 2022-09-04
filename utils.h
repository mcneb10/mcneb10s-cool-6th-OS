#ifndef OS6_UTILS_H
#define OS6_UTILS_H

#include <stdint.h>

typedef enum {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15
} VGACOLOR;

#pragma pack(push,1)

typedef struct {
    char c;
	VGACOLOR fgcolor:4;
    VGACOLOR bgcolor:4;
} SCREENENTRY;

#pragma pack(pop)

#define cli() __asm__("cli")
#define sti() __asm__("sti")

void initscreen();

void putc(char c);

uint32_t strlen(char* string);

void print(char* string);

uint32_t indtocoord(uint16_t ind);

uint16_t coordtoind(uint16_t x, uint16_t y);

void printhexu8(uint8_t num);

void printhexu16(uint16_t num);

void printhexu32(uint32_t num);

void hang();

void outb(uint16_t port, uint8_t b);

#define io_wait() outb(0x80, 0);

/*
    This function checks to see if the CPU supports CPUID
    by changing the CPUID enable flag and seeing if the CPU saves the result
*/
uint8_t supportsCPUID();

uint8_t inb(uint16_t port);

#endif