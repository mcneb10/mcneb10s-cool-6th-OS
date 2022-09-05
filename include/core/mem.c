#include "mem.h"
#include "utils.h"

void free(void* mem) {
    void* ptr = (uint32_t*)mem-4;
    // Size is at PTR-4
    uint32_t size = *((uint32_t*)ptr);
    

}

uint32_t bigFreeSpace = ;

void* malloc(uint32_t size) {
	
}


void* calloc(uint32_t count, uint32_t size) {
	return malloc(count*size);
}