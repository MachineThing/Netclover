include ../../config.mk

SRCFILES=	$(wildcard *.c)
OBJFILES=	$(SRCFILES:.c=.o)

all: $(OBJFILES) io.o

io.o: io.asm
	$(ASM) -f elf32 -o io.o io.asm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@