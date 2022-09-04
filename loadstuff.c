//#include "types.h"
#include "idt.h"
#include "utils.h"
#include "irq.h"

extern VGACOLOR fgcolor;
extern VGACOLOR bgcolor;

__attribute__((__noreturn__)) void stage2(uint8_t drive) {
    initscreen(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    print("Hello, World!\n");
    print("Drive number: 0x");
	printhexu8(/*drive*/0xFE);
	// Set up error interrupts
	IDT_INIT();
	// Set up IRQs

	// Load the IDT
	IDT_LOAD();
	print("\nInterrupts Initialized\n");

    // Hang
	hang();
    __asm__(
		".asciz \"Hello, World!\""
	);
}