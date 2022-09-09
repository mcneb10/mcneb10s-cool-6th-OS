#ifndef OS6_SLEEP_H
#define OS6_SLEEP_H

#include "../core/utils.h"
#include "irq.h"
#include "../libc/stdio.h"

#pragma pack(push,1)
typedef union pcr {
    struct {
        enum {
            binary16Bit = 0,
            BCD4Digit = 1
        } inputMode: 1;
        enum {
            latchCountValue = 0,
            lowByte = 1,
            highByte = 2,
            lowAndHighByte = 3
        } accessMode: 2;
        enum {
            // See https://wiki.osdev.org/Programmable_Interval_Timer#I.2FO_Ports
            mode0 = 0,
            mode1 = 1,
            mode2 = 2,
            mode3 = 3,
            mode4 = 4,
            mode5 = 5,
            mode2Again = 6,
            mode3Again = 7
        } operatingMode: 3;
        enum {
            channel0 = 0,
            channel1 = 1,
            channel2 = 2,
            readBack = 3
        } channel: 2;
    } fields;
    uint8_t bits;
} PIT_COMMAND_REG;
#pragma pack(pop)

#define channel0Data 0x40
#define channel1Data 0x41
#define channel2Data 0x42
#define commandRegister 0x42

#define PIT_FREQ_CONST 1193181

void sleep_init();


void msleep(uint64_t time);

void sleep(uint64_t time);

#endif