#include "include/drivers/idt.h"
#include "include/drivers/irq.h"
#include "include/drivers/sleep.h"
#include "include/drivers/tty.h"
#include "include/core/utils.h"
#include "include/core/consts.h"
#include "include/core/mem.h"
#include "include/libc/stdio.h"
#include "include/libc/stdlib.h"

extern uint64_t pitTicks;

void stage2(uint8_t drive) {
    // Set up screen
    tty_init_screen(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    // Set up memory stuff
    mem_init();
    // Print stuff
    int written = printf("Hello, World!\nBoot drive: 0x%hx\nWelcome to %s version %s\nCompiled on %s at %s\n", drive, OSNAME, OSVER, __DATE__, __TIME__);
    char* str = malloc(20);
    itoa(written, str, 10);
    tty_print("Written: ");
    tty_print(str);
    tty_print("\n");
    free(str);
	// Set up error interrupts
	IDT_INIT();
	// Set up IRQs
	IRQ_INIT();
	// Load the IDT
	IDT_LOAD();
    // Set up PIT channel 0
    sleep_init();
    // Enable Interrupt
	sti();
	tty_print("Interrupts Initialized\n");
    /*
    char *time = malloc(100);
    uint64_t t = 0;
    while(1) {
        itoa(t, time, 10);
        print("\r");
        print(time);
        sleep(1);
        t++;
    }*/
    sleep(1);
    tty_print("Done Sleeping!\n");
    // Hang
	hang();
    __asm__(
		".asciz \"Hello, World!\""
	);
}