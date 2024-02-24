# Functions
CPWD:=		$(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
PWD:=		$(abspath $(CPWD)/..)

# LibC
LIBDIR=		$(PWD)/src/libc
LIBC=		$(LIBDIR)/libc.a

QEMU:=		qemu-system-x86_64

TARGET:=	i686-elf
CC:=		$(TARGET)-gcc
CFLAGS:=	-ffreestanding -nostdlib -I$(LIBDIR)/headers -L$(LIBDIR) -lc
ASM:=		nasm
#ASMFLAGS=	-f elf

LD:=		$(TARGET)-ld
LDFLAGS:=	-nostdlib -L$(LIBDIR)

GITIGNORE:=	$(shell cat $(PWD)/.gitignore | grep -v '^#' | grep -v '^$$')
DIRFILES:=	$(shell find $1 -type f $(foreach pattern,$(GITIGNORE),-not -path '$(pattern)'))