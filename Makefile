BASEDIR = $(shell echo $(CURDIR)|sed 's/ /\\ /g')
BINDIR=$(BASEDIR)/bin
SRCDIR=$(BASEDIR)/src
WORKDIR=$(BASEDIR)/work
CROSSDIR = /usr/local/cross/ia32
CC = $(CROSSDIR)/bin/i686-elf-gcc
AS = $(CROSSDIR)/bin/i686-elf-as

CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra  -fno-builtin -fno-stack-protector -nostdlib
#-nostdinc
INCLUDEDIRS = $(SRCDIR)

ASSEMBLIES_ABS = $(shell find $(SRCDIR) -name '*.s')
SOURCES_ABS = $(shell find $(SRCDIR) -name '*.c')

ASSEMBLIES = $(subst $(BASEDIR)/,,$(ASSEMBLIES_ABS))
SOURCES = $(subst $(BASEDIR)/,,$(SOURCES_ABS))

TARGETS = $(SOURCES:.c=.o) $(ASSEMBLIES:.s=.o)

OBJECTS = $(shell find $(SRCDIR) -name '*.o')

help:
	@echo 'Usage:       - make command'
	@echo 'compile      - compiles all files to object files'
	@echo 'iso          - creates an iso image of the kernel'
	@echo 'link         - links all object files together to a binary'
	@echo 'run          - runs the kernel, using qemu'
	@echo 'clean        - deletes bin, obj and work directory'
	@echo 'help         - shows this help message'

clean:
	@if [ ! -z "$(OBJECTS)" ]; then rm $(OBJECTS); fi
	@if [ -e $(BINDIR) ]; then rm -r $(BINDIR); fi
	@if [ -e $(WORKDIR) ]; then rm -r $(WORKDIR); fi

compile: $(TARGETS)
	
%.o: %.s
	$(AS) -o $@ $<
	
%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $< -I $(INCLUDEDIRS)

iso: $(WORKDIR) compile link
	mkdir -p $(WORKDIR)/boot/grub
	cp $(BINDIR)/hammeros.bin $(WORKDIR)/boot/hammeros.bin
	cp grub.cfg $(WORKDIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(BINDIR)/hammeros.iso $(WORKDIR)
	
link: $(BINDIR)
	$(CC) -T $(SRCDIR)/linker.ld -o $(BINDIR)/hammeros.bin -ffreestanding -O2 -nostdlib $(OBJECTS) -lgcc

# Exit alt-2 q <enter>
run: compile link
	qemu-system-i386 --curses -kernel $(BINDIR)/hammeros.bin

$(BINDIR):
	mkdir -p $(BINDIR)
	
$(WORKDIR):
	mkdir -p $(WORKDIR)/boot/grub

