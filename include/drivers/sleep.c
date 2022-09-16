#include "sleep.h"

void sleep_init() {
    PIT_COMMAND_REG pitCommandReg;
    pitCommandReg.fields.inputMode = binary16Bit;
    pitCommandReg.fields.accessMode = lowAndHighByte;
    pitCommandReg.fields.operatingMode = mode2;
    pitCommandReg.fields.channel = channel0;
    outb(commandRegister, pitCommandReg.bits);
    uint16_t rate = PIT_FREQ_CONST / 1000;
    outb(channel0Data, (uint8_t)rate);
    outb(channel0Data, (uint8_t)(rate >> 8));
}

void msleep(uint64_t time) {
    uint64_t waitTime = getPitTicks()+time;
    while(getPitTicks()<waitTime) pause();
}

// second sleep
void sleep(uint64_t time) {
    msleep(time*1000);
}