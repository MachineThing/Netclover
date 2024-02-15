include ../config.mk

SRCFILES=	$(wildcard **/*.c) $(wildcard *.c)
OBJFILES=	$(SRCFILES:.c=.o)

all: libc.o

libc.o: $(OBJFILES) netclover/io.o
	$(TARGET)-ar rcs libc.a $(OBJFILES) netclover/io.o

netclover/io.o: netclover/io.asm
	$(ASM) -f elf32 -o netclover/io.o netclover/io.asm

%.o: %.c
	$(CC) $(CFLAGS) -fdata-sections -ffunction-sections -c $< -o $@