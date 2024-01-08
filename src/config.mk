QEMU:=		qemu-system-x86_64

TARGET:=	i686-elf
CC:=		$(TARGET)-gcc
CFLAGS:=	-ffreestanding -nostdlib -lgcc
ASM:=		nasm
#ASMFLAGS=	-f elf

LD:=		$(TARGET)-gcc
LDFLAGS:=	-nostdlib

CPWD:=		$(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
PWD:=		$(abspath $(CPWD)/..)

GITIGNORE:=	$(shell cat $(PWD)/.gitignore | grep -v '^#' | grep -v '^$$')
DIRFILES:=	$(shell find $1 -type f $(foreach pattern,$(GITIGNORE),-not -path '$(pattern)'))