//#include "types.h"
#include "include/core/idt.h"
#include "include/core/utils.h"
#include "include/core/irq.h"
#include "include/core/consts.h"

extern VGACOLOR fgcolor;
extern VGACOLOR bgcolor;

void stage2(uint8_t drive) {
    initscreen(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    print("Hello, World!\n");
    print("Welcome to ");
	print(OSNAME);
	print(" version ");
	print(OSVER);
	print("\nCompiled on ");
	print(__DATE__);
	print(" at ");
	print(__TIME__);
	print("\n");
	print(atoi(0xAA55, 1));
	print("\n");
	// Set up error interrupts
	IDT_INIT();
	// Set up IRQs
	IRQ_INIT();
	// Load the IDT
	IDT_LOAD();
	sti();
	print("Interrupts Initialized\n");

    // Hang
	hang();
    __asm__(
		".asciz \"Hello, World!\""
	);
}