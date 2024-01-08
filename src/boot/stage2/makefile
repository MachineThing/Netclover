include ../../config.mk

INIT=		init.bin
MAIN=		main.bin

LIBDIR=		libs
LIBSRC=		$(wildcard ./$(LIBDIR)/*.c) $(wildcard ./$(LIBDIR)/*.asm)
LIBOBJ=		$(addsuffix .o, $(basename $(LIBSRC)))

all: stage2.bin

stage2.bin: $(INIT) $(MAIN)
	$(LD) $(LDFLAGS) -T linker.ld -e _start $(INIT) $(MAIN) $(LIBOBJ) -o stage2.bin
	./calcsectors.sh > size.inc

$(MAIN): main.c $(LIBOBJ)
	$(CC) $(CFLAGS) -c main.c -o $(MAIN)

$(INIT): init.asm
	$(ASM) -f elf32 -o $(INIT) init.asm

$(LIBOBJ): $(LIBSRC)
	make -C $(LIBDIR)