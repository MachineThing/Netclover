include ../../config.mk

INIT=		init.bin
MAIN=		main.bin

SLIBDIR=	libs
LIBSRC=		$(wildcard ./$(SLIBDIR)/*.c) $(wildcard ./$(SLIBDIR)/*.asm)
LIBOBJ=		$(addsuffix .o, $(basename $(LIBSRC)))
BOOTOBJ=	../boot.o

all: stage2.bin

stage2.bin: $(INIT) $(MAIN) $(BOOTOBJ)
	$(LD) $(LDFLAGS) -Tlinker.ld -e _start -r -o stage2.bin $(INIT) $(MAIN) $(LIBOBJ) -lc

$(MAIN): main.c $(LIBOBJ)
	$(CC) $(CFLAGS) -c main.c -o $(MAIN)

$(INIT): init.asm
	$(ASM) -f elf32 -o $(INIT) init.asm

$(LIBOBJ): $(LIBSRC)
	make -C $(SLIBDIR)