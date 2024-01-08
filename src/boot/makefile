include ../config.mk

STAGE2DIR=	stage2
STAGE2=		$(STAGE2DIR)/stage2.bin
BOOTBIN=	boot.bin

all: $(BOOTBIN)

$(BOOTBIN): boot.asm $(STAGE2)
	$(ASM) -f bin -o $(BOOTBIN) boot.asm
	cat $(STAGE2) >> $(BOOTBIN)

$(STAGE2): $(call DIRFILES $(STAGE2DIR))
	make -C $(STAGE2DIR)