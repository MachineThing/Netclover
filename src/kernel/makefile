include ../config.mk

MAIN=		main.o

LIBDIR=		libs
LIBSRC=		$(wildcard ./$(LIBDIR)/*.c) $(wildcard ./$(LIBDIR)/*.asm)
LIBOBJ=		$(addsuffix .o, $(basename $(LIBSRC)))

all: kernel.bin

kernel.bin: $(MAIN) gdt.o gdtasm.o
	$(LD) $(LDFLAGS) -Tlinker.ld -o kernel.bin $(MAIN) $(LIBOBJ) gdt.o gdtasm.o

gdt.o: gdt.c
	$(CC) $(CFLAGS) -c gdt.c -o gdt.o

gdtasm.o: gdt.asm
	$(ASM) -f elf32 -o gdtasm.o gdt.asm

$(MAIN): main.c $(LIBOBJ)
	$(CC) $(CFLAGS) -c main.c -o $(MAIN)

$(LIBOBJ): $(LIBSRC)
	make -C $(LIBDIR)