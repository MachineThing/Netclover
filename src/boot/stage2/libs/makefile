include ../../../config.mk

SRCFILES=	$(wildcard *.c)
OBJFILES=	$(SRCFILES:.c=.o)

all: $(OBJFILES) x86.o

x86.o: x86.asm
	$(ASM) -f elf32 -o x86.o x86.asm

%.o: %.c
	$(CC32) $(CFLAGS32) -Os -fdata-sections -ffunction-sections -Wl,--gc-sections -c $< -o $@