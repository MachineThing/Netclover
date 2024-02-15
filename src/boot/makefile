include ../config.mk

STAGE2DIR=	stage2
STAGE2=		$(STAGE2DIR)/stage2.bin
BOOTOBJ=	boot.o
BOOTBIN=	boot.bin

all: $(BOOTBIN)

$(BOOTBIN): $(BOOTOBJ) $(STAGE2) $(LIBC)
	$(LD) $(LDFLAGS) -Tlinker.ld -o $(BOOTBIN) $(BOOTOBJ) $(STAGE2) -lc

$(BOOTOBJ): boot.asm
	$(ASM) -f elf32 -g -o $(BOOTOBJ) boot.asm

$(STAGE2): $(call DIRFILES $(STAGE2DIR))
	make -C $(STAGE2DIR)