include ../config.mk

MAIN=		main.o

LIBSRC=		$(wildcard ./$(LIBDIR)/*.c) $(wildcard ./$(LIBDIR)/*.asm)

all: kernel.bin

kernel.bin: $(MAIN) x86/gdt.o x86/gdtasm.o x86/idt.o x86/idtasm.o
	$(LD) $(LDFLAGS) -Tlinker.ld -o kernel.bin $(MAIN) $(LIBC) x86/gdt.o x86/gdtasm.o x86/idt.o x86/idtasm.o -lc

$(MAIN): main.c $(LIBC)
	$(CC) $(CFLAGS) -c main.c -o $(MAIN)

x86/gdt.o: x86/gdt.c
	$(CC) $(CFLAGS) -c x86/gdt.c -o x86/gdt.o

x86/gdtasm.o: x86/gdt.asm
	$(ASM) -f elf32 -o x86/gdtasm.o x86/gdt.asm

x86/idt.o: x86/idt.c
	$(CC) $(CFLAGS) -c x86/idt.c -o x86/idt.o

x86/idtasm.o: x86/idt.asm
	$(ASM) -f elf32 -o x86/idtasm.o x86/idt.asm