#ifndef OS6_PS2_H
#define OS6_PS2_H

#include <stdint.h>
#include "io.h"

#define PS2_ACK 0xFA
#define PS2_RESEND 0xFE
#define PS2_SELFTEST_PASS 0xAA
#define PS2_SELF_TEST_FAILED_0 0xFC
#define PS2_SELF_TEST_FAILED_1 0xFD

#define PS2_DATA_PORT 0x60
#define PS2_COMMAND_AND_STATUS 0x64

#define PS2_LED_CMD 0xED
#define PS2_ECHO_CMD 0xEE
#define PS2_SCANCODE_CMD 0xF0
#define PS2_ID_CMD 0xF2
#define PS2_TYPEMATIC_CMD 0xF3
#define PS2_SCANNING_ENABLE_CMD 0xF4
#define PS2_SCANNING_DISABLE_CMD 0xF5
#define PS2_SELFTEST_CMD 0xFF

// For autorepeat
#define PS2_TYPEMATIC_DELAY 0xF3

typedef union {
    uint8_t bits;
    struct {
        uint8_t repeatRate:5;
        uint8_t delay:2;
        uint8_t zero:1;
    } fields;
} PS2_TYPEMATIC_BYTE;

typedef enum {
    MOUSE = 0,
    MOUSE_WITH_SCROLL_WHEEL = 3,
    MOUSE_5_BUTTON = 4,
    KEYBOARD_WITH_TRANSLATION0 = 0x41AB,
    KEYBOARD_WITH_TRANSLATION1 = 0xC1AB,
    KEYBOARD = 0x83AB
} PS2_DEVICE_TYPE;

#pragma region scancodes

// TODO: finish this scancode crap
typedef enum {
    // Pressed
    F3_PRESSED = 0x4,
    F4_PRESSED = 0xC,
    LEFT_CTRL_PRESSED = 0x14,
    A_PRESSED = 0x1C,
    E_PRESSED = 0x24,
    T_PRESSED = 0x2C,
    G_PRESSED = 0x34,
    U_PRESSED = 0x3C,
    O_PRESSED = 0x44,
    SEMICOLON_PRESSED = 0x4C,
    LEFT_SQUARE_BRACKET_PRESSED = 0x54,
    CAPSLOCK_PRESSED = 0x58,
    KEYPAD_7_PRESSED = 0x6C,
    KEYPAD_0_PRESSED = 0x70,
    KEYPAD_6_PRESSED = 0x74,
    F11_PRESSED = 0x78,
    KEYPAD_ASTERISK_PRESSED = 0x7C,
    PRINT_SCREEN_PRESSED = 0xE012E07C,
    WWW_SEARCH_RELEASED = 0xE0F010,
    RIGHT_CTRL_RELEASED = 0xE0F014,
    WWW_FAVORITES_RELEASED = 0xE0F018,
    WWW_REFRESH_RELEASED = 0xE0F030
    // Released

} PS2_SCANCODES;

#pragma endregion

uint16_t ps2_init();

PS2_DEVICE_TYPE ps2_get_device_type();

#endif //OS6_PS2_H
