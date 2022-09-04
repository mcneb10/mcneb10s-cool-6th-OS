; Reusing old bootloader from last time
bits 16
section .bootloader
global start
extern stage2
start:
; STAGE 0 Load stuff
; save drive number
mov byte [drv], dl
mov ah, 2 ; read mode
mov al, 18 ; 18 sectors
mov ch, 0 ; cylinder
mov cl, 2 ;sectors start at 1, first is bootloader
mov dh, 0 ; head 0
; drive number already in dl
; ex = 0
xor bx, bx
mov es, bx
mov bx, 0x7E00 ; load next stage into 0000:7E00
int 0x13 ;call system
; Stage 1 enter protected mode
; disable hardware interrupts
; we must do this because the timer is constantly sending interrupts to the CPU
; after we enter protected mode, the CPU doesn't know what to do with this interrupt
; and restarts itself, causing a bootloop
; we'll re-enable them once we tell the CPU how to handle the timer
cli
; load gdt
lgdt [gdtd]
; load cr0
mov eax, cr0
; set pm bit
or al, 1
; set cr0
mov cr0, eax
; do far jump
jmp CODESEG:jump
bits 32
jump:
mov ax, DATASEG
mov ds, ax
mov es, ax
mov ss, ax
mov fs, ax
mov gs, ax
mov ebp, 0x90000
mov esp, ebp
; give next stage the drive number
xor eax, eax
mov al, byte [drv]
push eax
; enter stage2
jmp stage2
;; Variables ;;
; Drive number
drv: dd 0
;; GDT;;
gdt:
	dq 0 ; null entry
	; second entry
.code:
	dw 0xffff ; limit low part
	dw 0 ; base low part
	db 0 ; base middle part
	db 10011010b ; access
	db 11001111b ; flags and limit
	db 0 ; base high part
.data:
	dw 0xffff ; limit low part
	dw 0 ; base low part
	db 0; base middle part
	db 10010010b ; access
	db 11001111b ; flags and limit
	db 0 ; base high part
gdtd:
	dw gdtd - gdt - 1
	dd gdt
	
CODESEG equ gdt.code - gdt
DATASEG equ gdt.data - gdt
	
; pad with zeros
times (510 - ($-$$)) db 0
; bootable signature
dw 0xAA55