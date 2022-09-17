#ifndef OS6_IDT_H
#define OS6_IDT_H

#include <stdint.h>
#include "io.h"
#include "tty.h"
#include "../core/utils.h"

#pragma pack(push, 1)
typedef struct {
	uint16_t offsetLowWord;
	uint16_t segSelector;
	uint8_t reserved;
	enum GATE_TYPE {
		taskGate = 5,
		interrupt16 = 6,
		trap16 = 7,
		interrupt32 = 0xE,
		trap32 = 0xF
	} gateType:4;
	uint8_t zero:1;
	enum RINGS {
		RING0 = 0,
		RING1 = 1,
		RING2 = 2,
		RING3 = 3
	} privilegeLevel:2;
	uint8_t present:1;
	uint16_t offsetHighWord;
} IDT_ENTRY;
typedef struct {
	uint16_t size;
	uint32_t offset;
} IDT_DESCRIPTOR;
typedef struct {
	uint32_t ip;
	uint32_t cs;
	uint32_t flags;
	uint32_t sp;
	uint32_t ss;
} INTERRUPT_FRAME;
#pragma pack(pop)

#define IDT_ENTRY_COUNT 256

void IDT_INIT();
void set_idt_entry(uint8_t n, uint32_t isrPTR);
void IDT_LOAD();

#endif