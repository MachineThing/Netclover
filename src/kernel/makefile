include ../config.mk

MAIN=		main.o
CFILES=		$(shell find . -type f -name '*.c' -not -name 'main.c')
ASMFILES=	$(shell find . -type f -name '*.asm')
OBJFILES=	$(CFILES:.c=.o) $(ASMFILES:.asm=asm.o)

LIBSRC=		$(wildcard ./$(LIBDIR)/*.c) $(wildcard ./$(LIBDIR)/*.asm)

all: kernel.bin

kernel.bin: $(MAIN) $(OBJFILES)
	$(LD) $(LDFLAGS) -Tlinker.ld -o kernel.bin $(MAIN) $(LIBC) $(OBJFILES) -lc

$(MAIN): main.c $(LIBC)
	$(CC) $(CFLAGS) -c main.c -o $(MAIN)

%asm.o: %.asm
	$(ASM) -f elf64 -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@