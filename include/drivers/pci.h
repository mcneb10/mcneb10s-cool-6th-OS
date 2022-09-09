#ifndef OS6_PCI_H
#define OS6_PCI_H

#include "../core/utils.h"

#define PCI_CONFIG_ADDRESS_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC
typedef union {
struct {
    uint8_t offset:8;
    uint8_t func:3;
    uint8_t deviceNumber:5;
    uint8_t busNumber:8;
    uint8_t zero:2;
    uint8_t reserved:5;
    uint8_t enable:1; // Needs to be 1
} fields;
uint32_t bits;
} PCI_CONFIG_ADDRESS;

#endif
