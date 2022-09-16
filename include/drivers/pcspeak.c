#include "pcspeak.h"

//
void playsound(uint32_t freq, uint16_t timeMs) {
    uint32_t thing = PIT_FREQ_CONST / freq;
    // Set PIT channel 2 to lobyte/hibyte access and to 
    outb(0x43, 0xB6);
    outb(0x42, (uint8_t)thing);
    outb(0x42, (uint8_t)(thing >> 8));
    // Turn on speaker and tell it to listen accept frequency from PIT channel 2
    outb(0x61, inb(0x61) & 0b11);
    // Wait for duration of sound
    msleep(timeMs);
    // Turn off speaker
    outb(0x61, inb(0x61) & 0b11111100);
}