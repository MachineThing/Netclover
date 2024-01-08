include ../../config.mk

INIT=		init.bin
MAIN=		main.bin

all: stage2.bin

stage2.bin: $(INIT) $(MAIN)
	$(LD) $(LDFLAGS) -T linker.ld -e _start $(INIT) $(MAIN) -o stage2.bin
	./calcsectors.sh > size.inc

$(MAIN): main.c
	$(CC) $(CFLAGS) -c main.c -o $(MAIN)

$(INIT): init.asm
	$(ASM) -f elf32 -o $(INIT) init.asm