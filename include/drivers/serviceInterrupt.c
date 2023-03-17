#include "serviceInterrupt.h"

/*
 * EAX: System call number
 * EBX: Argument #0
 * ECX: Argument #1
 * EDX: Argument #2
 */

extern void serviceInterruptHandler();

void serviceInterrupt(uint32_t systemCall, uint32_t arg0, uint32_t arg1, uint32_t arg2) {
    switch(systemCall) {
        case 0: // SYS_READ
            break;
        case 1: // SYS_WRITE
            break;
        case 2: // SYS_OPEN

            break;
        case 3: // SYS_CLOSE
            break;
    }
}

void SERVICE_INTERRUPT_INIT() {
    set_idt_entry(0x80, (uint32_t)serviceInterruptHandler);
}