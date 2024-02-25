CC      = gcc
# CFLAGS  = -m32 -Wall -fno-builtin -nostdinc -nostdlib -ggdb3
CFLAGS  = -m32 -Wall -fno-builtin -nostdinc -nostdlib 
LD      = ld

OBJFILES = \
	loader.o  \
	common/printf.o  \
	common/screen.o  \
	kernel.o

image_part1:
	bash ./create_image_part1.bash
run: 
	qemu-system-i386 -hda hdd.img
all: kernel.bin
rebuild: clean all
.s.o:
	as --32 -o $@ $<
.c.o:
	$(CC) -Iinclude $(CFLAGS) -o $@ -c $<
kernel.bin: $(OBJFILES)
	$(LD) -m elf_i386 -T linker.ld -o $@ $^
clean:
	rm -f $(OBJFILES) hdd.img kernel.bin
# cp $@ $@.dbg 
# strip $@
