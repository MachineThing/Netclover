include ../../config.mk

INIT=		init.bin
MAIN=		main.bin

SLIBDIR=	libs
LIBSRC=		$(wildcard ./$(SLIBDIR)/*.c) $(wildcard ./$(SLIBDIR)/*.asm)
LIBOBJ=		$(addsuffix .o, $(basename $(LIBSRC)))
BOOTOBJ=	../boot.o

all: stage2.bin

stage2.bin: $(INIT) $(MAIN) $(BOOTOBJ)
	$(LD) $(LDFLAGS) --gc-sections -Tlinker.ld -e _start -r -o stage2.bin $(INIT) $(MAIN) $(LIBOBJ)

$(MAIN): main.c $(LIBOBJ)
	$(CC) $(CFLAGS) -Os -fdata-sections -ffunction-sections -Wl,--gc-sections -c main.c -o $(MAIN)

$(INIT): init.asm
	$(ASM) -f elf32 -o $(INIT) init.asm

$(LIBOBJ): $(LIBSRC)
	make -C $(SLIBDIR)