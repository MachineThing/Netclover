include ../config.mk

CFILES=		$(shell find . -type f -name '*.c')
ASMFILES=	$(shell find . -type f -name '*.asm')
OBJFILES32=	$(CFILES:.c=32.o) $(ASMFILES:.asm=32.o)
OBJFILES=	$(CFILES:.c=.o) $(ASMFILES:.asm=.o)

all: libc.o libc32.o

libc.o: $(OBJFILES)
	$(TARGET)-ar rcs libc.a $(OBJFILES)

libc32.o: $(OBJFILES32)
	$(TARGET32)-ar rcs libc32.a $(OBJFILES32)

%.o: %.asm
	$(ASM) -f elf64 -o $@ $<

%32.o: %.asm
	$(ASM) -f elf32 -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -fdata-sections -ffunction-sections -Wl,--gc-sections -c $< -o $@

%32.o: %.c
	$(CC32) $(CFLAGS32) -fdata-sections -ffunction-sections -Wl,--gc-sections -c $< -o $@