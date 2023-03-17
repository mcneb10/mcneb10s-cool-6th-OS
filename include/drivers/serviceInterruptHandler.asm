; Service interrupt handler
extern serviceInterrupt
global serviceInterruptHandler
serviceInterruptHandler:
    cli
    pushad
    push edx
    push ecx
    push ebx
    push eax
    call serviceInterrupt
    popad
    sti
    iret