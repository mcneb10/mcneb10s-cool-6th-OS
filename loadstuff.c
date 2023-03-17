#include "include/drivers/idt.h"
#include "include/drivers/irq.h"
#include "include/drivers/sleep.h"
#include "include/drivers/tty.h"
#include "include/drivers/cpuid.h"
#include "include/drivers/ps2.h"
#include "include/drivers/parallelport.h"
#include "include/drivers/mem.h"
#include "include/core/utils.h"
#include "include/core/consts.h"
#include "include/libc/stdio.h"

extern uint64_t pitTicks;

void cpuidDump() {
    char namestring[48];
    char vendorstring[15];
    printf
    (
            "CPU String: %s\nCPU Vendor string: %s\n",
            cpuid_brandString(namestring) ? namestring : "None",
            cpuid_vendorString(vendorstring) ? vendorstring : "Duuude your CPU is super old or janky as hell!\nIt doesn't even have a vendor string!"
    );
    //TODO: implement more CPUID stuff
}

void stage2(uint8_t drive) {
    // Set up screen
    tty_init_screen(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    // Set up memory stuff
    mem_init();
    // Print stuff
    printf("Hello, World!\nBoot drive: 0x%hx\nWelcome to %s version %s\nCompiled on %s at %s\n", drive, OSNAME, OSVER, __DATE__, __TIME__);
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
    uint16_t ps2InitResult = ps2_init();
    switch (ps2InitResult >> 8) {
        case 0:
            tty_print("PS2 successfully initialized!\n");
            break;
        case 1:
            printf("PS2 initialization failed!\nPS2 device not responding to echo commands\nExpected echo value: 0x%x\nActual echo value: 0x%x\n", PS2_ECHO_CMD, (uint8_t)ps2InitResult);
            break;
        case 2:
            printf("PS2 initialization failed!\nPS2 device failed it's self test\nExpected self test result: 0x%x\nActual self test result: 0x%x\n", PS2_SELFTEST_PASS, (uint8_t)ps2InitResult);
            break;
    }
    printf("CPUID Supported? %s\n", cpuid_supported() ? "yes" : "no");
    if(cpuid_supported()) cpuidDump();
	tty_print("Interrupts Initialized\n");
    parallel_printf(LPT1, "Hello, parallel universe!\r\n0xAB: 0x%x\r\n", 0xAB);
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