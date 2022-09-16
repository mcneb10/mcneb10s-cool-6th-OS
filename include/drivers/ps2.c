#include "ps2.h"

uint8_t ps2_send_command_nodata(uint8_t command) {
    outb(PS2_COMMAND_AND_STATUS, command);
    io_wait();
    uint8_t res = inb(PS2_DATA_PORT);
    io_wait();
    return res;
}

uint8_t ps2_send_command(uint8_t command, uint8_t data) {
    uint8_t response = 0;
    do {
        outb(PS2_COMMAND_AND_STATUS, command);
        io_wait();
        outb(PS2_DATA_PORT, data);
        io_wait();
        response = inb(PS2_DATA_PORT);
        io_wait();
    } while (response == 0xFE);
    return response;
}

uint8_t ps2_send_command_wait_for_ack(uint8_t command, uint8_t data) {
    uint8_t response = 0;
    do {
        outb(PS2_COMMAND_AND_STATUS, command);
        io_wait();
        outb(PS2_DATA_PORT, data);
        io_wait();
        response = inb(PS2_DATA_PORT);
        io_wait();
    } while (response != 0xFA);
    return response;
}

uint8_t ps2_send_command_wait_for_ack_nodata(uint8_t command) {
    uint8_t response = 0;
    do {
        outb(PS2_COMMAND_AND_STATUS, command);
        io_wait();
        response = inb(PS2_DATA_PORT);
        io_wait();
    } while (response != 0xFA);
    return response;
}

PS2_DEVICE_TYPE ps2_get_device_type() {
    cli();
    PS2_DEVICE_TYPE res;
    // Get first id byte
    uint8_t idByte1 = ps2_send_command_wait_for_ack_nodata(PS2_ID_CMD);
    res = idByte1;
    if(idByte1 == 0xAB) {
        // If first id byte is 0xAB, get the next one and OR it to the result
        uint8_t idByte2 = inb(PS2_DATA_PORT);
        io_wait();
        res |= idByte2 << 8;
    }
    sti();
    return res;
}

uint16_t ps2_init() {
    cli(); // don't let our keyboard commands be interrupted
    // Does the keyboard even exist?
    // If the echo port returns 0xEE, the device exists
    ps2_send_command_wait_for_ack_nodata(PS2_ECHO_CMD);
    uint8_t echoResult = inb(PS2_DATA_PORT);
    if(echoResult != 0xEE) {
        sti();
        return (1 << 8) | echoResult;
    }
    // Tell it to self test and restart
    ps2_send_command_wait_for_ack_nodata(PS2_SELFTEST_CMD);
    uint8_t selfTestResult = inb(PS2_DATA_PORT);
    if(selfTestResult != PS2_SELFTEST_PASS) {
        sti();
        return (2 << 8) | selfTestResult;
    }
    // Tell keyboard to go to scancode set 2
    ps2_send_command(PS2_SCANCODE_CMD, 2);
    // Initialize the second PS2 device if it exists

    sti();
    return 0;
}