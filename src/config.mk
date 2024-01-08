QEMU=		qemu-system-x86_64
ASM=		nasm

GITIGNORE=	$(shell cat .gitignore | grep -v '^#' | grep -v '^$$')
DIRFILES=	$(shell find $1 -type f $(foreach pattern,$(GITIGNORE),-not -path '$(pattern)'))
