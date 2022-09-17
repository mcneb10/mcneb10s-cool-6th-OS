; pit handler written in assembly to avoid weird compiler bugs

global irq0
extern pitTicks

irq0:
    cli                 ; no interruptions
    pushad              ; save all registers
    inc dword [pitTicks]; increment pit ticks
    cmp dword [pitTicks], 0 ; hack to make 64 bit ints work on 32 bit
    jne eoi
    inc dword [pitTicks+4]
    eoi:
    mov al, 0x20
    out 0x20, al        ; send master EOI
    popad               ; restore all registers
    sti                 ; allow interrupts again
    iret                ; return from interrupt