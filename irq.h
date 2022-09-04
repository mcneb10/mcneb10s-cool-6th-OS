#ifndef OS6_IRQ_H
#define OS6_IRQ_H

#include "utils.h"

#define MASTER_PIC_COMMAND 0x20
#define MASTER_PIC_DATA 0x21
#define SLAVE_PIC_COMMAND 0xA0
#define SLAVE_PIC_DATA 0xA1

// Enable ICW4 register
#define ICW1_ICW4 1
// Init PIC
#define ICW1_INIT 0x10

void IRQ_INIT();

#endif