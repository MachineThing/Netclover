include ../config.mk

STAGE2DIR=	stage2
STAGE2=		$(STAGE2DIR)/stage2.bin
BOOTBIN=	boot.bin

all: $(BOOTBIN)

$(BOOTBIN): boot.asm $(call DIRFILES $(STAGE2DIR))
	$(ASM) -f bin -o $(BOOTBIN) boot.asm
	cat $(STAGE2) >> $(BOOTBIN)

$(STAGE2):
	make -C $(STAGE2DIR)