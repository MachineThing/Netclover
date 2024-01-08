include ../../config.mk

all: stage2.bin

stage2.bin: init.asm
	$(ASM) -f bin -o stage2.bin init.asm
	./calcsectors.sh > size.inc