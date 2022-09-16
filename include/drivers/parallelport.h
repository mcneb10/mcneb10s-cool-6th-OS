#ifndef OS6_PARALLELPORT_H
#define OS6_PARALLELPORT_H

#include <stdint.h>
#include <stdarg.h>
#include "io.h"
#include "sleep.h"
#include "../libc/stdio.h"

#define LPT1 0x378
#define PARALLEL1_DATA_PORT LPT1
#define PARALLEL1_STATUS_REG_PORT PARALLEL1_DATA_PORT+1
#define PARALLEL1_CONTROL_REG_PORT PARALLEL_STATUS_REG_PORT+2

#define LPT2 0x279

#define PARALLEL_STATUS_REG_BUSY 1<<7
#define PARALLEL_STATUS_REG_ACK 1<<6
#define PARALLEL_STATUS_REG_PAPER_OUT 1<<5
#define PARALLEL_STATUS_REG_SELECT_IN 1<<4
#define PARALLEL_STATUS_REG_ERROR 1<<3
#define PARALLEL_STATUS_REG_IRQ 1<<2

void parallel_busy_wait(uint16_t port);

void parallel_outb(uint8_t data, uint16_t port);

void parallel_print(uint16_t port, char* str);

void parallel_printf(uint16_t port, char* restrict fmt, ...);

void parallel_handle_interrupt(uint16_t port);

#endif
