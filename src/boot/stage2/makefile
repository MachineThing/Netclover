include ../../config.mk

INIT=		init.bin
MAIN=		main.bin

LIBDIR=		libs
LIBSRC=		$(wildcard ./$(LIBDIR)/*.c)
LIBOBJ=		$(LIBSRC:.c=.o)

all: stage2.bin

stage2.bin: $(INIT) $(MAIN)
	$(LD) $(LDFLAGS) -T linker.ld -e _start $(INIT) $(MAIN) -o stage2.bin
	./calcsectors.sh > size.inc

$(MAIN): main.c $(LIBOBJ)
	$(CC) $(CFLAGS) -c main.c -o $(MAIN)

$(INIT): init.asm
	$(ASM) -f elf32 -o $(INIT) init.asm

$(LIBOBJ): $(LIBSRC)
	make -C $(LIBDIR)