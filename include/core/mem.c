#include "mem.h"

uint8_t* bigFreeSpace = (uint8_t*)0x10000;

FREESPACE* freeSpace;
uint16_t freeSpacePtr = 0;

uint8_t initialized = 0;

void* malloc(uint32_t size) {
    void* initialPtr = bigFreeSpace;
    if(initialized) {
        for (uint16_t x = 0; x < FREESPACE_ENTRY_MAX; x++) {
            if (freeSpace[x].freeSpaceSize >= size) {
                initialPtr = freeSpace[x].freeSpacePtr;
                freeSpace[x].freeSpacePtr += size + sizeof(MEM_AREA_DESCRIPTOR);
                freeSpace[x].freeSpaceSize -= size + sizeof(MEM_AREA_DESCRIPTOR);
            }
        }
    }
    ((MEM_AREA_DESCRIPTOR*)initialPtr)->size = size;
    ((MEM_AREA_DESCRIPTOR*)initialPtr)->owner = 0x0; // reserved fo future use

    bigFreeSpace+=sizeof(MEM_AREA_DESCRIPTOR);
    for(uint32_t i=0;i<size;i++) {
        *(((uint8_t*)initialPtr)+i) = 0;
    }
    void* ptr = (void*)bigFreeSpace;
    bigFreeSpace+=size;
    // Align by 4
    bigFreeSpace+=4-(uint32_t)bigFreeSpace%4;
    return ptr;
}


void* calloc(uint32_t count, uint32_t size) {
    return malloc(count*size);
}

void mem_init() {
    freeSpace = calloc(FREESPACE_ENTRY_MAX, sizeof(FREESPACE));
    initialized = 1;
}

void free(void* mem) {
    // Size is at PTR-4
    MEM_AREA_DESCRIPTOR* memAreaDescriptor = ((MEM_AREA_DESCRIPTOR*)mem-sizeof(MEM_AREA_DESCRIPTOR));
    for(uint32_t i=0;i<memAreaDescriptor->size;i++) {
        ((uint8_t*)mem)[i] = 0;
    }
    if(freeSpacePtr < FREESPACE_ENTRY_MAX) {
        freeSpace[freeSpacePtr].freeSpacePtr = mem;
        freeSpace[freeSpacePtr].freeSpaceSize = memAreaDescriptor->size;
        freeSpacePtr++;
    }
}

// "Borrowed" from OSDev

int memcmp(const void* aptr, const void* bptr, size_t size) {
    const unsigned char* a = (const unsigned char*) aptr;
    const unsigned char* b = (const unsigned char*) bptr;
    for (size_t i = 0; i < size; i++) {
        if (a[i] < b[i])
            return -1;
        else if (b[i] < a[i])
            return 1;
    }
    return 0;
}

void* memset(void* bufptr, int value, size_t size) {
    unsigned char* buf = (unsigned char*) bufptr;
    for (size_t i = 0; i < size; i++)
        buf[i] = (unsigned char) value;
    return bufptr;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size) {
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;
    for (size_t i = 0; i < size; i++)
        dst[i] = src[i];
    return dstptr;
}

void* memmove(void* dstptr, const void* srcptr, size_t size) {
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;
    if (dst < src) {
        for (size_t i = 0; i < size; i++)
            dst[i] = src[i];
    } else {
        for (size_t i = size; i != 0; i--)
            dst[i-1] = src[i-1];
    }
    return dstptr;
}

char* strcpy(char* dest, const char* src) {
    memcpy(dest, src, strlen(src));
    return dest;
}