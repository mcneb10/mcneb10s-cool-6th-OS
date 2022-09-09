#ifndef OS6_MEM_H
#define OS6_MEM_H

#include <stddef.h>
#include "utils.h"

#define FREESPACE_ENTRY_MAX 40000

void* malloc(uint32_t size);

void* calloc(uint32_t count, uint32_t size);

void free(void* mem);

void mem_init();

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
char* strcpy(char* dest, const char* src);

// this struct does not need to be packed because it is specific to the OS
typedef struct {
    void* freeSpacePtr;
    uint32_t freeSpaceSize;
} FREESPACE;

typedef struct {
    uint32_t size;
    uint32_t owner;
} MEM_AREA_DESCRIPTOR;

#endif