#include "stdio.h"

int toupper(int c) {
    if(c >= 'a' && c <= 'c') {
        return c-32;
    }
    return c;
}

int tolower(int c) {
    if(c >= 'A' && c <= 'Z') {
        return c+32;
    }
    return c;
}

char* strtolower(char* str) {
    for(uint32_t i=0;i<strlen(str);i++) {
        str[i] = tolower((uint8_t)str[i]);
    }
    return str;
}

char* strtoupper(char* str) {
    for(uint32_t i=0;i<strlen(str);i++) {
        str[i] = toupper((uint8_t)str[i]);
    }
    return str;
}

int sprintf(char* result, char* restrict fmt, ...) {
    va_list list;
    va_start(list, fmt);
    int written = vsprintf(result, fmt, list);
    va_end(list);
    return written;
}

int vsprintf(char* result, char* restrict fmt, va_list params) {
    char buf[MAX_PRINTF_OUTPUT_SIZE];
    uint8_t isFormatSpecifier = 0;
    uint8_t escapedPercent = 0;
    uint8_t numberFormat = 0;
    uint32_t len = strlen(fmt);
    uint32_t resultPtr = 0;
    for(uint32_t i=0;i<len;i++) {
        switch (fmt[i]) {
            case '%':
                if(i!=len-1) {
                    if (escapedPercent || isFormatSpecifier) {
                        isFormatSpecifier = 0;
                        escapedPercent = 0;
                    } else {
                        isFormatSpecifier = 1;
                    }
                } else {
                    result[resultPtr++] = '%';
                }
                break;
            case '\\':
                if(i==len-1) break;
                if(!(escapedPercent = fmt[i+1] == '%')) {
                    result[resultPtr++] = fmt[i];
                }
                break;
            case 'i': // TODO: implement signed integers
            case 'd':
            case 'u':
                if(numberFormat != 0 || isFormatSpecifier) {
                    switch (numberFormat) {
                        case 0: // uint32_t
                            itoa(va_arg(params, uint32_t), buf, 10);
                            strcpy(result+resultPtr, buf);
                            resultPtr += strlen(buf);
                            break;
                        case 1: // uint16_t
                            itoa((uint16_t)va_arg(params, uint32_t), buf, 10);
                            strcpy(result+resultPtr, buf);
                            resultPtr += strlen(buf);
                            break;
                        case 2: // uint64_t
                            itoa(va_arg(params, uint64_t), buf, 10);
                            strcpy(result+resultPtr, buf);
                            resultPtr += strlen(buf);
                            break;
                    }
                    numberFormat = 0;
                } else {
                        result[resultPtr++] = fmt[i];
                }
                break;
            case 'x':
                if(numberFormat != 0 || isFormatSpecifier) {
                    switch (numberFormat) {
                        case 0: // uint32_t
                            itoa(va_arg(params, uint32_t), buf, 16);
                            strcpy(result+resultPtr, buf);
                            resultPtr += strlen(buf);
                            break;
                        case 1: // uint16_t
                            itoa((uint16_t)va_arg(params, uint32_t), buf, 16);
                            strcpy(result+resultPtr, buf);
                            resultPtr += strlen(buf);
                            break;
                        case 2: // uint64_t
                            itoa(va_arg(params, uint64_t), buf, 16);
                            strcpy(result+resultPtr, buf);
                            resultPtr += strlen(buf);
                            break;
                    }
                    numberFormat = 0;
                } else {
                    result[resultPtr++] = fmt[i];
                }
                break;
            case 'X':
                if(numberFormat != 0 || isFormatSpecifier) {
                    switch (numberFormat) {
                        case 0: // uint32_t
                            itoa(va_arg(params, uint32_t), buf, 16);
                            strcpy(result+resultPtr, strtoupper(buf));
                            resultPtr += strlen(buf);
                            break;
                        case 1: // uint16_t
                            itoa((uint16_t)va_arg(params, uint32_t), buf, 16);
                            strcpy(result+resultPtr, strtoupper(buf));
                            resultPtr += strlen(buf);
                            break;
                        case 2: // uint64_t
                            itoa(va_arg(params, uint64_t), buf, 16);
                            strcpy(result+resultPtr, strtoupper(buf));
                            resultPtr += strlen(buf);
                            break;
                    }
                    numberFormat = 0;
                } else {
                    result[resultPtr++] = fmt[i];
                }
                break;
            case 'c':
                if(isFormatSpecifier) {
                    result[resultPtr++] = (char)va_arg(params, uint32_t);
                } else {
                    result[resultPtr++] = fmt[i];
                }
                break;
            case 's':
                if(isFormatSpecifier) {
                    char *str = va_arg(params, char*);
                    strcpy(result+resultPtr, str);
                    resultPtr += strlen(str);
                    isFormatSpecifier = 0;
                }  else {
                    result[resultPtr++] = fmt[i];
                }
                break;
            case 'h':
                if(isFormatSpecifier) {
                    numberFormat = 1;
                } else {
                    result[resultPtr++] = fmt[i];
                }
                break;
            case 'l':
                if(isFormatSpecifier) {
                    numberFormat = 2;
                } else {
                    result[resultPtr++] = fmt[i];
                }
                break;
            default:
                if(isFormatSpecifier) {
                    isFormatSpecifier = 0;
                    break;
                }
                result[resultPtr++] = fmt[i];
                break;
        }
        if(isFormatSpecifier && fmt[i] != '%') {
            isFormatSpecifier = 0;
        }
    }
    return resultPtr;
}

int printf(char* restrict fmt, ...) {
    char buf[MAX_PRINTF_OUTPUT_SIZE];
    memset(buf, 0, MAX_PRINTF_OUTPUT_SIZE);
    va_list args;
    va_start(args, fmt);
    int written = vsprintf(buf, fmt, args);
    tty_print(buf);
    va_end(args);
    return written;
}