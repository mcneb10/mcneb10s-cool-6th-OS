#include "parallelport.h"

void parallel_busy_wait(uint16_t port) {
    // Get busy bit and wait until parallel port is ready
    while(!(inb(port+1) & PARALLEL_STATUS_REG_BUSY)) {
        // LPT is busy
        msleep(10);
    }
}

void parallel_outb(uint8_t data, uint16_t port) {
    parallel_busy_wait(port);
    // Output data
    outb(port, data);
    // Get parallel port control register
    uint8_t controlReg = inb(port+2);
    // Activate the strobe line
    outb(port+2, controlReg | 1);
    msleep(10);
    // Deactivate it
    outb(port+2, controlReg);
    parallel_busy_wait(port);
}

void parallel_print(uint16_t port, char* str) {
    for(uint32_t i=0;i<strlen(str);i++) parallel_outb(str[i], port);
}

void parallel_printf(uint16_t port, char* restrict fmt, ...) {
    char buf[MAX_PRINTF_OUTPUT_SIZE];
    memset(buf, 0, MAX_PRINTF_OUTPUT_SIZE);
    va_list args;
    va_start(args, fmt);
    int written = vsprintf(buf, fmt, args);
    parallel_print(port, buf);
    va_end(args);
    return written;
}

void parallel_handle_interrupt(uint16_t port) {

}