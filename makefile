CC := gcc
.PHONY: do run build clean
do: build run

run:
	cmd.exe /c qemu.bat
	gdb --command=gdbcommands

build:
	# Compile the bootloader
	nasm -felf32 -o boot.o bootloader.asm
	# Compile the C code
	$(CC) -c -fno-pie -save-temps -nostdlib -fno-asynchronous-unwind-tables -ffreestanding -m32 loadstuff.c -o loadstuff.o
	$(CC) -c -fno-pie -save-temps -nostdlib -fno-asynchronous-unwind-tables -ffreestanding -m32 utils.c -o utils.o
	$(CC) -c -fno-pie -save-temps -nostdlib -fno-asynchronous-unwind-tables -ffreestanding -m32 irq.c -o irq.o
	$(CC) -c -fno-pie -save-temps -nostdlib -mgeneral-regs-only -fno-asynchronous-unwind-tables -ffreestanding -m32 idt.c -o idt.o
	$(CC) -c -fno-pie -save-temps -nostdlib -mgeneral-regs-only -fno-asynchronous-unwind-tables -ffreestanding -m32 irq.c -o irq.o
	# Link bootloader and C code, convert to flat binary and dump symbols
	$(LD) boot.o loadstuff.o utils.o idt.o irq.o -m elf_i386 -T link.ld -o boot.bin
	$(LD) boot.o loadstuff.o utils.o idt.o irq.o -m elf_i386 -T link.ld --oformat elf32-i386 -o symbols.elf
	@echo -n "Size of boot.bin before padding to fit floppy disk (in bytes): "
	@wc -c boot.bin | grep -o '^\S*'
	# Pad output to size of 1.44MB floppy
	truncate -s 1474560 boot.bin

clean:
	rm -rf *.o
	rm -rf *.elf
	rm -rf *.bin
	rm -rf *.s
	rm -rf *.i
