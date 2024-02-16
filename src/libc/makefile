include ../config.mk

CFILES=		$(shell find . -type f -name '*.c')
ASMFILES=	$(shell find . -type f -name '*.asm')
OBJFILES=	$(CFILES:.c=.o) $(ASMFILES:.asm=.o)

all: libc.o

libc.o: $(OBJFILES)
	$(TARGET)-ar rcs libc.a $(OBJFILES)

%.o: %.asm
	$(ASM) -f elf32 -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -fdata-sections -ffunction-sections -Wl,--gc-sections -c $< -o $@