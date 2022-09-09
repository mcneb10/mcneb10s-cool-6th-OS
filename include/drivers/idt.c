#include "idt.h"

void set_idt_entry(uint8_t n, uint32_t isrPTR) {
	idt[n].offsetLowWord = isrPTR & 0xFFFF;
	idt[n].segSelector = 8; // Select code (where the code is running from)
	idt[n].reserved = 0;
	// Flags
	idt[n].gateType = interrupt32;
	idt[n].zero = 0;
	idt[n].privilegeLevel = RING0;
	idt[n].present = 1; // Bit MUST be set to one for the IDT entry to be recognized
	idt[n].offsetHighWord = isrPTR >> 16;
}

#pragma region ISR_HANDLERS

void fatalerror() {
	tty_print("FATAL ERROR!!!!\n");
}
/*
void framedump(INTERRUPT_FRAME* frame) {
	tty_print("Some Registers at the time of the incident:\n");
	tty_print("IP (Instruction Pointer): 0x");
	printhexu16(frame->ip);
	tty_print("\nCS (Code Segment): 0x");
	printhexu16(frame->cs);
	tty_print("\nSP (Stack Pointer): 0x");
	printhexu16(frame->sp);
	tty_print("\nSS (Stack Segment): 0x");
	printhexu16(frame->ss);
	tty_print("\nFLAGS: 0b");
	for(uint8_t i=0;i<16;i++) {
		putc((frame->flags >> i) & 1 ? '1' : '0');
	}
	// TODO: finish this
	tty_print("\n");
} */

__attribute__((interrupt)) void isrGenericResrvedStub(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("A reserved interrupt was triggered.\n");
	
	hang();
}

__attribute__((interrupt)) void isr0(uint8_t* ptrToNothing) {
	cli();
	tty_print("Divide by zero error\n");
	
	hang();
}

__attribute__((interrupt)) void isr1(uint8_t* ptrToNothing) {
	cli();
	tty_print("Debug interrupt triggered\n");
	
	sti();
}

__attribute__((interrupt)) void isr2(uint8_t* ptrToNothing) {
	cli();
	tty_print("The Non-Maskable Interrupt was triggered\n");
	sti();
}

__attribute__((interrupt)) void isr3(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("Into Dectected Overflow\n");
	
	hang();
}

__attribute__((interrupt)) void isr4(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("Out of Bounds\n");
	
	hang();
}

__attribute__((interrupt)) void isr5(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("Invalid OpCode Encountered\n");
	
	hang();
}

__attribute__((interrupt)) void isr6(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("No CoProcessor\n");
	
	hang();
}

__attribute__((interrupt)) void isr7(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("While another fault was being handled, this fault occured.\n");
	
	hang();
}

__attribute__((interrupt)) void isr8(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("CoProcessor Segment Overrun\n");
	
	hang();
}

__attribute__((interrupt)) void isr9(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("Invalid TSS\n");
	
	hang();
}

__attribute__((interrupt)) void isr10(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("Segment not present\n");
	
	hang();
}

__attribute__((interrupt)) void isr11(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("Stack Fault\n");
	
	hang();
}

__attribute__((interrupt)) void isr12(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("General Protection Fault\n");
	
	hang();
}

__attribute__((interrupt)) void isr13(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("Page Fault\n");
	
	hang();
}

__attribute__((interrupt)) void isr14(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("An unknown interrupt has been triggered\n");
	
	hang();
}

__attribute__((interrupt)) void isr15(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("CoProcessor Fault\n");
	
	hang();
}

__attribute__((interrupt)) void isr16(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("Unaligned memory operand\n");
	
	hang();
}

__attribute__((interrupt)) void isr17(uint8_t* ptrToNothing) {
	cli();
	fatalerror();
	tty_print("Warning! A peice of hardware (most likely the CPU) in your system has reported that it is malfunctioning.\n");
	hang();
}


#pragma endregion

void IDT_INIT() {
	// Set IDT entries
	set_idt_entry(0, (uint32_t)isr0);
	set_idt_entry(1, (uint32_t)isr1);
	set_idt_entry(2, (uint32_t)isr2);
	set_idt_entry(3, (uint32_t)isr3);
	set_idt_entry(4, (uint32_t)isr4);
	set_idt_entry(5, (uint32_t)isr5);
	set_idt_entry(6, (uint32_t)isr6);
	set_idt_entry(7, (uint32_t)isr7);
	set_idt_entry(8, (uint32_t)isr8);
	set_idt_entry(9, (uint32_t)isr9);
	set_idt_entry(10, (uint32_t)isr10);
	set_idt_entry(11, (uint32_t)isr11);
	set_idt_entry(12, (uint32_t)isr12);
	set_idt_entry(13, (uint32_t)isr13);
	set_idt_entry(14, (uint32_t)isr14);
	set_idt_entry(15, (uint32_t)isr15);
	set_idt_entry(16, (uint32_t)isr16);
	set_idt_entry(17, (uint32_t)isr17);
	// Do reserved interrupts
	for(uint8_t i=18;i<31;i++) {
		set_idt_entry(i, (uint32_t)isrGenericResrvedStub);
	}
}

void IDT_LOAD() {
	// Set up IDT descriptor
	idtr.offset = (uint32_t)&idt;
	idtr.size = IDT_ENTRY_COUNT * sizeof(IDT_ENTRY) - 1;
	// Tell the CPU where the IDT descriptor is
	__asm__ __volatile__ ("lidtl (%0)" : : "r" (&idtr));
}