#include "irq.h"
#include "utils.h"

#pragma region IRQS

uint64_t pitTicks = 0;

inline void masterEOI() {
    outb(MASTER_PIC_COMMAND, 0x20);
}

inline void slaveEOI() {
    outb(MASTER_PIC_COMMAND, 0x20);
    outb(SLAVE_PIC_COMMAND, 0x20);
}

// PIT
__attribute__((interrupt)) void irq0() {
    pitTicks++;
    masterEOI();
}
// Keyboard
__attribute__((interrupt)) void irq1() {
    print("Keyboard\n");
    masterEOI();
}
// COM2
__attribute__((interrupt)) void irq3() {

    masterEOI();
}
// COM1
__attribute__((interrupt)) void irq4() {

    masterEOI();
}
// LPT2
__attribute__((interrupt)) void irq5() {

    masterEOI();
}
// Floppy Disk
__attribute__((interrupt)) void irq6() {

    masterEOI();
}
uint64_t spuriousCount = 0;
// LPT1 or spurious interrupt
__attribute__((interrupt)) void irq7() {

    spuriousCount++;
    masterEOI();
}
// RTC
__attribute__((interrupt)) void irq8() {

    slaveEOI();
}
// Peripheral 1
__attribute__((interrupt)) void irq9() {

    slaveEOI();
}
// Peripheral 2
__attribute__((interrupt)) void irq10() {

    slaveEOI();
}
// Peripheral 3
__attribute__((interrupt)) void irq11() {

    slaveEOI();
}
// PS2 mouse
__attribute__((interrupt)) void irq12() {

    slaveEOI();
}
// FPU/Coprocessor?
__attribute__((interrupt)) void irq13() {

    slaveEOI();
}
// ATA HDD 1
__attribute__((interrupt)) void irq14() {

    slaveEOI();
}
// ATA HDD 2
__attribute__((interrupt)) void irq15() {

    slaveEOI();
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
    // PIC offsets (remaps IRQs so they don't clash with interrupts reserved for errors)
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

    set_idt_gate(32, (uint32)irq0);
    set_idt_gate(33, (uint32)irq1);
    // Used internally by the PICs
    //set_idt_gate(34, (uint32)irq2);
    set_idt_gate(35, (uint32)irq3);
    set_idt_gate(36, (uint32)irq4);
    set_idt_gate(37, (uint32)irq5);
    set_idt_gate(38, (uint32)irq6);
    set_idt_gate(39, (uint32)irq7);
    set_idt_gate(40, (uint32)irq8);
    set_idt_gate(41, (uint32)irq9);
    set_idt_gate(42, (uint32)irq10);
    set_idt_gate(43, (uint32)irq11);
    set_idt_gate(44, (uint32)irq12);
    set_idt_gate(45, (uint32)irq13);
    set_idt_gate(46, (uint32)irq14);
    set_idt_gate(47, (uint32)irq15);
}
