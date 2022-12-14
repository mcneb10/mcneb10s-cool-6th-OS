#include "tty.h"


SCREENENTRY* screen;

VGACOLOR fgcolor;
VGACOLOR bgcolor;
uint16_t screenptr;
const uint8_t VGA_WIDTH = 80;
const uint8_t VGA_HEIGHT = 25;
uint16_t mousePos = 0;
uint8_t mouseEnabled = 0;

void tty_clrscreen() {
    for(uint32_t i=0;i<VGA_WIDTH*VGA_HEIGHT;i++) {
        screen[i].c = ' ';
        screen[i].fgcolor = fgcolor;
        screen[i].bgcolor = bgcolor;
    }
}


void tty_init_screen(VGACOLOR fgcolorIn, VGACOLOR bgcolorIn) {
    screen = (SCREENENTRY*)0xb8000;
    fgcolor = fgcolorIn;
    bgcolor = bgcolorIn;
    screenptr = 0;
    mousePos = 0;
    tty_clrscreen();
}
// setMouse(nonzero value) enables mouse
// setMouse(0) enables mouse
void setMouse(uint8_t r) {
    mouseEnabled = r;
}

void tty_printat(char* s, uint16_t coord) {
    uint16_t sb = screenptr;
    screenptr = 0xb8000+coord;
    tty_print(s);
    screenptr = sb;
}

void tty_putcat(char c, uint16_t coord) {
    uint16_t sb = screenptr;
    screenptr = 0xb8000+coord;
    tty_putc(c);
    screenptr = sb;
}

void tty_print(char* string) {
    for(uint32_t i=0;i<strlen(string);i++) tty_putc(string[i]);
}

// upper half x coord lower half y coord
uint32_t tty_indtocoord(uint16_t ind) {
    uint16_t x = ind % VGA_WIDTH;
    uint16_t y = ind / VGA_WIDTH;
    return (x<<16)|y;
}

uint16_t tty_coordtoind(uint16_t x, uint16_t y) {
    return (VGA_WIDTH*y)+x;
}

void tty_cursor_setpos(uint16_t pos) {

    // cursor hi byte
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    // cursor low byte
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

void tty_cursor_setpos_xy(uint16_t x, uint16_t y) {
    tty_cursor_setpos(tty_coordtoind(x,y));
    mousePos = tty_coordtoind(x, y);
}

uint32_t tty_cursor_getpos_xy() {
    return tty_indtocoord(mousePos);
}

void tty_putc(char c) {
    switch(c) {
        case '\n':
            screenptr += VGA_WIDTH;
        case '\r':
            screenptr -= screenptr % VGA_WIDTH;
            break;
        case '\a':
            playsound(523,100);
            break;
        case '\b':
            screen[--screenptr].c = ' ';
            break;
        default:
            screen[screenptr].c = c;
            screen[screenptr].fgcolor = fgcolor;
            screen[screenptr].bgcolor = bgcolor;
            screenptr++;
    }
    if(!mouseEnabled) tty_cursor_setpos(screenptr);
}