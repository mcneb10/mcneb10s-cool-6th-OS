global cpuHelper

cpuHelper:
    ; Save EFLAGS twice
    ; The first will be the backup
    ; The second will be the one that is modified
    pushfd
    pushfd
    ; Toggle the CPUID bit in the eflags
    xor dword [esp], 0x00200000
    ; Pop it back into the EFLAGS register
    popfd
    ; Bring it back
    pushfd
    ; Save to return register
    pop eax
    ; Was it modified
    xor dword eax, [esp]
    ; Restore original EFLAGS
    popfd
    ; Let the c code do the rest of the work
    ret

