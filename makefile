.PHONY: do run build clean rebuild

CFLAGS+=-fno-pie -save-temps -nostdlib -ffreestanding -g -m32 -DDebug

LDFLAGS+=-lgcc -m elf_i386 -T link.ld

CORE=\
include/core/mem.o\
include/core/utils.o\
include/core/serviceInterrupt.o\
include/core/serviceInterruptHandler.o\
include/core/wumbo.o

DRIVERS=\
include/drivers/io.o\
include/drivers/pci.o\
include/drivers/pcspeak.o\
include/drivers/rtc.o\
include/drivers/sleep.o\
include/drivers/tty.o\
include/drivers/cpuid.o\
include/drivers/ps2.o\
include/drivers/parallelport.o\
include/drivers/pit.o

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

# The mystery of wumbo
# Even though it is part of libcore and is linked with the rest of the kernel,
# Other objects can't resolve symbols unless it is linked directly

LINK_OBJS=\
boot.o\
loadstuff.o\
libcore.a\
libc.a\
libd.a\
include/core/wumbo.o

include makefile.conf

.SUFFIXES:.o .c .S .asm

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
	$(CC) -c -mgeneral-regs-only -mno-red-zone $(CFLAGS) $(basename $@).c -o $@

.asm.o:
	nasm -f elf32 $< -o $@

.c.o:
	$(CC) -c -Wall -Wextra $(CFLAGS) $< -o $@

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
	$(LD) $(LINK_OBJS) -L$(CRTPATH) $(LDFLAGS) -o boot.bin
	$(LD) $(LINK_OBJS) -L$(CRTPATH) $(LDFLAGS) --oformat elf32-i386 -o symbols.elf
	FAT_OFFSET=$(wc -c boot.bin | grep -o '^\S*')
	@echo "Size of boot.bin before padding to fit floppy disk (in bytes): $(FAT_OFFSET)"
	#mkfs.fat -D 0 -F 12 -g 2/18 -n OS6BOOT --mbr no --offset $(FAT_OFFSET) boot.bin
	# Pad output to size of 1.44MB floppy
	truncate -s 1474560 boot.bin

clean:
	# Delete all compiled files
	find . \( -name "*.o" -o -name "*.elf" -o -name "*.bin" -o -name "*.s" -o -name "*.i" -o -name "*.a" \) -exec rm {} \;
