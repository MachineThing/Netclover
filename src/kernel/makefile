include ../config.mk

MAIN=		main.bin

LIBDIR=		libs
LIBSRC=		$(wildcard ./$(LIBDIR)/*.c) $(wildcard ./$(LIBDIR)/*.asm)
LIBOBJ=		$(addsuffix .o, $(basename $(LIBSRC)))

all: kernel.bin

kernel.bin: $(MAIN)
	$(LD) $(LDFLAGS) -Tlinker.ld -o kernel.bin $(MAIN) $(LIBOBJ)

$(MAIN): main.c $(LIBOBJ)
	$(CC) $(CFLAGS) -c main.c -o $(MAIN)

$(LIBOBJ): $(LIBSRC)
	make -C $(LIBDIR)