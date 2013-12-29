CC = /home/eirik/workspace/os/tools/gcc/bin/i586-elf-gcc
AS =  /home/eirik/workspace/os/tools/gcc/bin/i586-elf-as

# Do everything by default if it isn't done already
all: boot kernel link

kernel: objfolder
	$(CC) -c src/kernel.c -o obj/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

boot: objfolder
	$(AS) src/boot.s -o obj/boot.o

link: kernel boot binfolder
	$(CC) -T src/linker.ld -o bin/myos.bin -ffreestanding -O2 -nostdlib obj/boot.o obj/kernel.o -lgcc

iso: link workfolder bin
	cp bin/myos.bin work/boot/myos.bin
	cp grub.cfg work/boot/grub/grub.cfg
	grub-mkrescue -o bin/myos.iso work
	
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
