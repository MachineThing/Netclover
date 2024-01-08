include ../../config.mk

all: stage2.bin

stage2.bin: main.asm
	$(ASM) -f bin -o stage2.bin main.asm
	./calcsectors.sh > size.inc