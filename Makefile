BASEDIR = $(shell echo $(CURDIR)|sed 's/ /\\ /g')
BINDIR=bin
OBJDIR=obj
WORKDIR=work
CROSSDIR = /usr/local/cross/ia32
CC = $(CROSSDIR)/bin/i686-elf-gcc
AS = $(CROSSDIR)/bin/i686-elf-as

SOURCES=display.o gdt.o io.o kernel.o string.o util.o boot.o gdt_flush.o idt.o idt_flush.o isr.o
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES) )

CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
#-nostdlib -nostdinc -fno-builtin -fno-stack-protector

help:
	@echo 'Usage:       - make command'
	@echo 'help         - shows this help message'
	@echo 'clean        - deletes bin, obj and work directory'
	@echo 'compile      - compiles all files to object files'
	@echo 'iso          - creates an iso image of the kernel'
	@echo 'link         - links all object files together to a binary'
	@echo 'run          - runs the kernel, using qemu'

clean:
	if [ -e $(OBJDIR) ]; then rm -r $(OBJDIR); fi
	if [ -e $(BINDIR) ]; then rm -r $(BINDIR); fi
	if [ -e $(WORKDIR) ]; then rm -r $(WORKDIR); fi

compile: $(OBJDIR) $(OBJECTS)

iso: compile link
	cp $(BINDIR)/hammeros.bin $(WORKDIR)/boot/hammeros.bin
	cp grub.cfg $(WORKDIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(BINDIR)/hammeros.iso $(WORKDIR)
	
link: $(BINDIR)
	# ld $(LDFLAGS) -o kernel $(SOURCES)
	$(CC) -T src/linker.ld -o $(BINDIR)/hammeros.bin -ffreestanding -O2 -nostdlib $(OBJECTS) -lgcc

run: compile link
	qemu-system-i386 -kernel $(BINDIR)/hammeros.bin

$(OBJDIR)/%.o: src/%.s
	$(AS) $< -o $@
	
$(OBJDIR)/%.o: src/%.c
	$(CC) -c -o $@ $(CFLAGS) $<

$(OBJDIR):
	mkdir -p $(OBJDIR)
	
$(BINDIR):
	mkdir -p $(BINDIR)
	
$(WORKDIR):
	mkdir -p $(WORKDIR)/boot/grub
