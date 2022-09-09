CC := gcc
.PHONY: do run build clean rebuild

CFLAGS:=-fno-pie -save-temps -nostdlib -ffreestanding -g -m32 -DDebug

LDFLAGS:=-lgcc -m elf_i386 -T link.ld

CORE=\
include/core/mem.o\
include/core/utils.o

DRIVERS=\
include/drivers/io.o\
include/drivers/pci.o\
include/drivers/pcspeak.o\
include/drivers/rtc.o\
include/drivers/sleep.o\
include/drivers/tty.o\
include/drivers/cpuid.o\
include/drivers/parallelport.o

CLIB_STUFF_OBJS=\
include/crti.o \
include/crtbegin.o \
include/crtend.o \
include/crtn.o

OBJS=\
boot.o\
loadstuff.o\
$(CORE)\
$(DRIVERS)\
$(CLIB_STUFF_OBJS)

INTERRUPT_OBJS=\
include/drivers/idt.o\
include/drivers/irq.o

LIBC_OBJS=\
include/libc/stdio.o\
include/libc/stdlib.o\
include/libc/string.o\
$(CLIB_STUFF_OBJS)

LIBD_OBJS=\
$(DRIVERS)\
$(INTERRUPT_OBJS)

LINK_OBJS=\
boot.o\
loadstuff.o\
libcore.a\
libc.a\
libd.a

include makefile.conf

.SUFFIXES:.o .c .S

redo: rebuild run

do: build run

run:
	cmd.exe /c qemu.bat
	gdb --command=gdbcommands

build: boot.bin

rebuild: clean build

boot.o:
	nasm -felf32 -o boot.o bootloader.asm

include/crtbegin.o include/crtend.o:
	cp "$(CRTPATH)/crtbegin.o" include
	cp "$(CRTPATH)/crtend.o" include

$(INTERRUPT_OBJS):
	$(CC) -c -mgeneral-regs-only -mno-red-zone $(CFLAGS) $(EXTRACOPTS) $(basename $@).c -o $@

.c.o:
	$(CC) -c -Wall -Wextra $(CFLAGS) $(EXTRACOPTS) $< -o $@

.S.o:
	$(AS) -c -nostdlib -march=i386 --32 $< -o $@

libcore.a: $(CORE)
	$(AR) rcs $@ $(CORE)

libc.a: $(LIBC_OBJS)
	$(AR) rcs $@ $(LIBC_OBJS)

libd.a: $(LIBD_OBJS)
	$(AR) rcs $@ $(LIBD_OBJS)


boot.bin: $(LINK_OBJS)
	# Link bootloader and C code, convert to flat binary and dump symbols
	$(LD) $(LINK_OBJS) $(EXTRALDOPTS) -L$(CRTPATH) $(LDFLAGS) -o boot.bin
	$(LD) $(LINK_OBJS) $(EXTRALDOPTS) -L$(CRTPATH) $(LDFLAGS) --oformat elf32-i386 -o symbols.elf
	@echo -n "Size of boot.bin before padding to fit floppy disk (in bytes): "
	@wc -c boot.bin | grep -o '^\S*'
	# Pad output to size of 1.44MB floppy
	truncate -s 1474560 boot.bin

clean:
	# Delete all compiled files
	find . \( -name "*.o" -o -name "*.elf" -o -name "*.bin" -o -name "*.s" -o -name "*.i" -o -name "*.a" \) -exec rm {} \;
