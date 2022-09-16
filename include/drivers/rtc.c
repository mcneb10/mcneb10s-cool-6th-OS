#include "rtc.h"

// TODO: finish rtc

void rtcinit() {
    // Disable interrupts
    cli();
    // Select status register and disable NMI
    outb(0x70, 0x8A);
    // write to RTC
    outb(0x71, 0x20);
    // Enable interrupts
    sti();
}