#include "sleep.h"

extern uint64_t pitTicks;

void sleep_init() {
    PIT_COMMAND_REG pitCommandReg;
    pitCommandReg.fields.inputMode = binary16Bit;
    pitCommandReg.fields.accessMode = lowAndHighByte;
    pitCommandReg.fields.operatingMode = mode3;
    pitCommandReg.fields.channel = channel0;
    outb(commandRegister, pitCommandReg.bits);
    // Rate = 1 uS
    uint16_t rate = 1; // PIT_FREQ_CONST / 1000000
    outb(0x42, (uint8_t)rate);
    outb(0x42, (uint8_t)(rate >> 8));
}

void usleep(uint64_t time) {
    uint64_t waitTime = getPitTicks()+time;
    char buf[100];
    tty_print("Time: ");
    itoa(time, &buf, 10);
    tty_print(buf);
    tty_print("\nWait time:");
    itoa(waitTime, &buf, 10);
    tty_print(buf);
    tty_print("\n");
    while(getPitTicks()<waitTime);
}

// millisecond sleep
void msleep(uint64_t time) {
    usleep(time*1000);
}

// second sleep
void sleep(uint64_t time) {
    msleep(time*1000);
}