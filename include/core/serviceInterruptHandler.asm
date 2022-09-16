; Service interrupt handler
extern serviceInterrupt
global serviceInterruptHandler
serviceInterruptHandler:
    pushad
    push edx
    push ecx
    push ebx
    push eax
    call serviceInterrupt
    popad
    iret