# Functions
CPWD:=		$(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
PWD:=		$(abspath $(CPWD)/..)

# LibC
LIBDIR=		$(PWD)/src/libc
LIBC=		$(LIBDIR)/libc.a

QEMU:=		qemu-system-x86_64

TARGET:=	x86_64-elf
CC:=		$(TARGET)-gcc
CFLAGS:=	-ffreestanding -nostdlib -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -I$(LIBDIR)/headers -L$(LIBDIR) -lc

TARGET32:=	i686-elf
CC32:=		$(TARGET32)-gcc
CFLAGS32:=	-ffreestanding -nostdlib -I$(LIBDIR)/headers -L$(LIBDIR) -lc32
ASM:=		nasm
#ASMFLAGS=	-f elf

LD:=		$(TARGET)-ld
LDFLAGS:=	-nostdlib -L$(LIBDIR)

LD32:=		$(TARGET32)-ld
LDFLAGS32:=	$(LDFLAGS)

GITIGNORE:=	$(shell cat $(PWD)/.gitignore | grep -v '^#' | grep -v '^$$')
DIRFILES:=	$(shell find $1 -type f $(foreach pattern,$(GITIGNORE),-not -path '$(pattern)'))