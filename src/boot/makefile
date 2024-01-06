include ../config.mk

all: boot.bin

boot.bin: boot.asm
	$(ASM) -f bin -o boot.bin boot.asm