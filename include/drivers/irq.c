#include "irq.h"

#pragma region IRQS
// Rolls over every 584942 years, 152 days, 8 hours, 1 minutes, 49.5 seconds

static uint64_t pitTicks;

#define masterEOI() outb(MASTER_PIC_COMMAND, 0x20)

#define slaveEOI() (outb(SLAVE_PIC_COMMAND, 0x20),outb(MASTER_PIC_COMMAND, 0x20))

// PIT
__attribute__((interrupt)) void irq0(uint8_t* ptrToNothing) {
    cli();
    pitTicks+=1;
    printf("\r%u", pitTicks);
    masterEOI();
    sti();
}
// Keyboard
__attribute__((interrupt)) void irq1(uint8_t* ptrToNothing) {
    cli();
    tty_print("Keyboard\n");
    masterEOI();
    sti();
}
// COM2
__attribute__((interrupt)) void irq3(uint8_t* ptrToNothing) {
    cli();

    masterEOI();
    sti();
}
// COM1
__attribute__((interrupt)) void irq4(uint8_t* ptrToNothing) {
    cli();

    masterEOI();
    sti();
}
// LPT2
__attribute__((interrupt)) void irq5(uint8_t* ptrToNothing) {
    cli();

    masterEOI();
    sti();
}
// Floppy Disk
__attribute__((interrupt)) void irq6(uint8_t* ptrToNothing) {
    cli();

    masterEOI();
    sti();
}
volatile uint64_t spuriousCount = 0;
// LPT1 or spurious interrupt
__attribute__((interrupt)) void irq7(uint8_t* ptrToNothing) {
    cli();
    // Was an interrupt actually triggered
    outb(MASTER_PIC_COMMAND, OCW_IRQ_IN_SERVICE);
    if(inb(MASTER_PIC_COMMAND) & 0b10000000) {
        // Yes, do stuff with LPT1 and send EOI

        masterEOI();
    } else {
        // No, increment a counter and DON'T send EOI
        spuriousCount++;
    }
    sti();
}
// RTC
__attribute__((interrupt)) void irq8(uint8_t* ptrToNothing) {
    cli();

    slaveEOI();
    sti();
}
// Peripheral 1
__attribute__((interrupt)) void irq9(uint8_t* ptrToNothing) {
    cli();

    slaveEOI();
    sti();
}
// Peripheral 2
__attribute__((interrupt)) void irq10(uint8_t* ptrToNothing) {
    cli();

    slaveEOI();
    sti();
}
// Peripheral 3
__attribute__((interrupt)) void irq11(uint8_t* ptrToNothing) {
    cli();

    slaveEOI();
    sti();
}
// PS2 mouse
__attribute__((interrupt)) void irq12(uint8_t* ptrToNothing) {
    cli();

    slaveEOI();
    sti();
}
// FPU/Coprocessor?
__attribute__((interrupt)) void irq13(uint8_t* ptrToNothing) {
    cli();

    slaveEOI();
    sti();
}
// ATA HDD 1
__attribute__((interrupt)) void irq14(uint8_t* ptrToNothing) {
    cli();

    slaveEOI();
    sti();
}
// ATA HDD 2
__attribute__((interrupt)) void irq15(uint8_t* ptrToNothing) {
    cli();

    slaveEOI();
    sti();
}
#pragma endregion

void IRQ_INIT() {
    // PIC remapping
    uint8_t masterMask = inb(MASTER_PIC_DATA);
    io_wait();
    uint8_t slaveMask = inb(SLAVE_PIC_DATA);
    io_wait();
    // PIC flags
    outb(MASTER_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(SLAVE_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    // PIC offsets (remaps IRQs, so they don't clash with interrupts reserved for errors)
    outb(MASTER_PIC_DATA, 0x20);
    io_wait();
    outb(SLAVE_PIC_DATA, 0x28);
    io_wait();
    // Tell the Master PIC there is a slave PIC
    outb(MASTER_PIC_DATA, 4);
    io_wait();
    // Tell slave pic it's cascade identity
    outb(SLAVE_PIC_DATA, 2);
    io_wait();
    // Put PICs in 8086 mode
    outb(MASTER_PIC_DATA, 1);
    io_wait();
    outb(SLAVE_PIC_DATA, 1);
    io_wait();

    outb(MASTER_PIC_DATA, masterMask);
    outb(SLAVE_PIC_DATA, slaveMask);

    set_idt_entry(32, (uint32_t)irq0);
    set_idt_entry(33, (uint32_t)irq1);
    // Used internally by the PICs
    //set_idt_entry(34, (uint32_t)irq2);
    set_idt_entry(35, (uint32_t)irq3);
    set_idt_entry(36, (uint32_t)irq4);
    set_idt_entry(37, (uint32_t)irq5);
    set_idt_entry(38, (uint32_t)irq6);
    set_idt_entry(39, (uint32_t)irq7);
    set_idt_entry(40, (uint32_t)irq8);
    set_idt_entry(41, (uint32_t)irq9);
    set_idt_entry(42, (uint32_t)irq10);
    set_idt_entry(43, (uint32_t)irq11);
    set_idt_entry(44, (uint32_t)irq12);
    set_idt_entry(45, (uint32_t)irq13);
    set_idt_entry(46, (uint32_t)irq14);
    set_idt_entry(47, (uint32_t)irq15);

    pitTicks = 0;
}

uint64_t getPitTicks() {
    return pitTicks;
}