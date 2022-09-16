#ifndef OS6_TTY_H
#define OS6_TTY_H

#include <stdint.h>

#include "io.h"
#include "pcspeak.h"
#include "../libc/string.h"

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

void tty_init_screen(VGACOLOR fgcolorIn, VGACOLOR bgcolorIn);

void tty_putc(char c);

void tty_print(char* string);

void tty_clrscreen();

uint32_t tty_indtocoord(uint16_t ind);

uint16_t tty_coordtoind(uint16_t x, uint16_t y);

void tty_cursor_setpos_xy(uint16_t x, uint16_t y);

uint32_t tty_cursor_getpos_xy();

#endif //OS6_TTY_H
