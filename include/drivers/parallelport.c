#include "parallelport.h"

void parallel_busy_wait(uint16_t port) {
    parallel_port_sanity_check(port);
    // Get busy bit (active low) and wait until parallel port is ready
    while(!(inb(port+1) & PARALLEL_STATUS_REG_BUSY)) {
        // LPT is busy
        msleep(10);
    }
}

void parallel_outb(uint8_t data, uint16_t port) {
    parallel_port_sanity_check(port);
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
    parallel_port_sanity_check(port);
    for(uint32_t i=0;i<strlen(str);i++) parallel_outb(str[i], port);
}

void parallel_printf(uint16_t port, char* restrict fmt, ...) {
    parallel_port_sanity_check(port);
    char buf[MAX_PRINTF_OUTPUT_SIZE];
    memset(buf, 0, MAX_PRINTF_OUTPUT_SIZE);
    va_list args;
    va_start(args, fmt);
    int written = vsprintf(buf, fmt, args);
    parallel_print(port, buf);
    va_end(args);
    return;// written;
}

static uint8_t shift(uint8_t* array, uint32_t size) {
    uint8_t res = array[0];
    for(uint32_t i=0;i<size-1;i++) {
        array[i] = array[i+1];
    }
    array[size-1] = 0;
    return res;
}

uint8_t lpt1buf[PARALLEL_BUF_SIZE];
uint16_t lpt1bufptr = 0;
uint8_t lpt2buf[PARALLEL_BUF_SIZE];
uint16_t lpt2bufptr = 0;

void parallel_handle_interrupt(uint16_t port) {
    parallel_port_sanity_check(port);
    uint8_t data = inb(port);
    switch(port) {
        case LPT1:
            if(lpt1bufptr == PARALLEL_BUF_SIZE) {
                shift(lpt1buf, PARALLEL_BUF_SIZE);
                lpt1buf[PARALLEL_BUF_SIZE-1] = data;
            } else {
                lpt1buf[lpt1bufptr++] = data;
            }
            break;
        case LPT2:
            if(lpt2bufptr == PARALLEL_BUF_SIZE) {
                shift(lpt2buf, PARALLEL_BUF_SIZE);
                lpt2buf[PARALLEL_BUF_SIZE-1] = data;
            } else {
                lpt2buf[lpt2bufptr++] = data;
            }
            break;
    }
}