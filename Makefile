BASEDIR = $(shell echo $(CURDIR)|sed 's/ /\\ /g')
CROSSDIR = /home/hammerstad/workspace/gcc-cross
CC = $(CROSSDIR)/bin/i686-elf-gcc
AS = $(CROSSDIR)/bin/i686-elf-as

# Do everything by default if it isn't done already
all: run

kernel: objfolder
	$(CC) -c src/kernel.c -o obj/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	$(CC) -c src/display.c -o obj/display.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

boot: objfolder
	$(AS) src/boot.s -o obj/boot.o

link: kernel boot binfolder
	$(CC) -T src/linker.ld -o bin/hameros.bin -ffreestanding -O2 -nostdlib obj/boot.o obj/kernel.o obj/display.o -lgcc

iso: link workfolder bin
	cp bin/hammeros.bin work/boot/hammeros.bin
	cp grub.cfg work/boot/grub/grub.cfg
	grub-mkrescue -o bin/hammeros.iso work

run: link
	qemu-system-i386 -kernel bin/hammeros.bin
	
clean:
	if [ -e obj ]; then rm -r obj; fi
	if [ -e work ]; then rm -r work; fi

purge: clean
	if [ -e bin ]; then rm -r bin; fi

#
# Create directories if they don't exist already
#
objfolder:
	mkdir -p obj
binfolder:
	mkdir -p bin
workfolder:
	mkdir -p work/boot/grub
