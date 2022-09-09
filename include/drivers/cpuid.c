#include "cpuid.h"


uint8_t supportsCPUID() {
    uint32_t eflagsout;
    __asm__ __volatile__(
//Change to intel syntax
".intel_syntax noprefix\n"
//Save EFLAGS twice
//The first will be the backup
//The second will be the one that is modified
"pushfd\n"
"pushfd\n"
//Toggle the CPUID bit in the eflags
"xor dword [esp], 0x00200000\n"
//Pop it back into the EFLAGS register
"popfd\n"
//Bring it back
"pushfd\n"
//Save to register
"pop %0\n"
//Was it modified
"xor %0, [esp]\n"
//Restore original EFLAGS
"popfd\n"
//Change back to GAS syntax
".att_syntax"
: "=r" (eflagsout) : : "eax");
return (eflagsout >> 21) & 1;
}