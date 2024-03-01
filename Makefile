CC      = gcc
CFLAGS  = -m32 -Wall -fno-builtin -nostdinc -nostdlib -ggdb3
# CFLAGS  = -m32 -Wall -fno-builtin -nostdinc -nostdlib 
LD      = ld

OBJFILES = \
	loader.o  \
	common/printf.o  \
	common/screen.o  \
	common/pci.o  \
	common/bios.o  \
	common/vbe.o  \
	common/qdivrem.o  \
	common/udivdi3.o  \
	common/umoddi3.o  \
	common/divdi3.o  \
	common/moddi3.o  \
	common/setjmp.o  \
	common/string.o  \
	common/s_floor.o  \
	x86emu/x86emu.o  \
	x86emu/x86emu_util.o  \
	fractal.o \
	kernel.o

image:
	bash ./create_image.bash
run: 
	qemu-system-i386 -hda hdd.img
runcgdb: clean all image
	qemu-system-i386 -s -S -hda hdd.img &
all: kernel.bin
rebuild: clean all image
.s.o:
	as --32 -o $@ $<
.c.o:
	$(CC) -Ix86emu -Iinclude $(CFLAGS) -o $@ -c $<
kernel.bin: $(OBJFILES)
	$(LD) -m elf_i386 -T linker.ld -o $@.dbg $^
	cp $@.dbg $@
	strip $@
clean:
	rm -f $(OBJFILES) hdd.img kernel.bin
