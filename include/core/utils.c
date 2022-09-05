#include "utils.h"
#include "mem.h"
#include <stdint.h>

SCREENENTRY* screen;

VGACOLOR fgcolor;
VGACOLOR bgcolor;
uint16_t screenptr;
const uint8_t VGA_WIDTH = 80;
const uint8_t VGA_HEIGHT = 25;

void clrscreen() {
	for(uint32_t i=0;i<VGA_WIDTH*VGA_HEIGHT;i++) {
		screen[i].c = ' ';
		screen[i].fgcolor = fgcolor;
		screen[i].bgcolor = bgcolor;
	}
}

void initscreen(VGACOLOR fgcolorIn, VGACOLOR bgcolorIn) {
	screen = (SCREENENTRY*)0xb8000;
	fgcolor = fgcolorIn;
	bgcolor = bgcolorIn;
	screenptr = 0;
	clrscreen();
}

void putc(char c) {
	SCREENENTRY test;
    test.c = c;
    test.bgcolor = bgcolor;
    test.fgcolor = fgcolor;
	uint8_t elementsize = sizeof(SCREENENTRY);
	switch(c) {
		case '\n':
			screenptr += VGA_WIDTH;
		case '\r':
			screenptr -= screenptr % VGA_WIDTH;
			break;
			
		default:
			screen[screenptr++] = test;
	}
	// cursor hi byte
	outb(0x3D4, 14);
	outb(0x3D5, screenptr >> 8);
	// cursor low byte
	outb(0x3D4, 15);
	outb(0x3D5, screenptr & 0xFF);
}

void printat(char* s, uint16_t coord) {
	uint16_t sb = screenptr;
	print(s);
	screenptr = sb;
}

void putcat(char c, uint16_t coord) {
	uint16_t sb = screenptr;
	putc(c);
	screenptr = sb;
}

uint32_t strlen(char* string) {
	uint32_t len = 0;
	while(string[len++] != '\0');
	return len-1;
}

void print(char* string) {
	for(uint32_t i=0;i<strlen(string);i++) putc(string[i]);
}

// upper half x coord lower half y coord
uint32_t indtocoord(uint16_t ind) {
	uint16_t x = ind % VGA_WIDTH;
	uint16_t y = ind / VGA_WIDTH;
	return (x<<16)|y;
}

uint16_t coordtoind(uint16_t x, uint16_t y) {
	return (VGA_WIDTH*y)+x;
}

#define BUFSIZE 100

char* atoi(uint32_t n, uint16_t radix) {
	uint32_t num = n;
	char buf[BUFSIZE];
	char* resbuf = malloc(BUFSIZE);
	uint8_t i=0;
	if(num == 0) {
		buf[0] = '0';
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
			if(num != 0) i++;
		}
	}
	while(1) {
		if(
			(buf[i] >= '0' && buf[i] <= '9')||
			(buf[i] >= 'A' && buf[i] <= 'F')
		) { 
			resbuf[BUFSIZE - i] = buf[i];
		}
		if(i==0) break;
		i--;
	}
	return resbuf;
}

void outb(uint16_t port, uint8_t b) {
	__asm__("out %%al, %%dx" : : "a" (b), "d" (port));
}

uint8_t inb(uint16_t port) {
	uint8_t r;
	__asm__("in %%dx, %%al" : "=a" (r) : "d" (port));
	return r;
}

void hang() {
	sti();
	print("The operating system will now hang.\n");
	__asm__("hangloop: jmp hangloop");
}
/*
uint8_t supportsCPUID() {
    uint32_t eflagsout;
    __asm__ __volatile__(
//Change to intel syntax
".intel_syntax noprefix\n"
//Save EFLAGS twice
//The first will be the backup
//The second will be the one that is modified
"pushfd\n"
"pushfd\n"
//Toggle the CPUID bit in the eflags
"xor dword [esp], 0x00200000\n"
//Pop it back into the EFLAGS register
"popfd\n"
//Bring it back
"pushfd\n"
//Save to register
"pop %0\n"
//Was it modified
"xor %0, [esp]\n"
//Restore original EFLAGS
"popfd\n"
//Change back to GAS syntax
".att_syntax"
: "=r" (eflagsout) : : "eax");
return (eflagsout >> 21) & 1;
}*/
