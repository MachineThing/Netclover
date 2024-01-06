include src/config.mk

BOOTDIR=	src/boot
KERNELDIR=	src/kernel
STATICDIR=	static

BOOT=		$(BOOTDIR)/boot.bin
KERNEL=		$(KERNELDIR)/kernel.bin
FLPIMG=		httpos.img

GITIGNORE=	$(shell cat .gitignore | grep -v '^#' | grep -v '^$$')
DIRFILES=	$(shell find $1 -type f $(foreach pattern,$(GITIGNORE),-not -path '$(pattern)'))

.PHONY: all run clean

all: $(FLPIMG)

run: $(FLPIMG)
	$(QEMU) $(FLPIMG)

# Floppy image
$(FLPIMG): $(BOOT) $(KERNEL) $(call DIRFILES $(STATICDIR))
	dd if=/dev/zero of=$(FLPIMG) bs=512 count=2880
	mkfs.fat -F 12 $(FLPIMG)
	dd if=$(BOOT) of=$(FLPIMG) conv=notrunc
	truncate -s 1440k $(FLPIMG)
	mcopy -i $(FLPIMG) $(KERNEL) "::kernel.bin"

# Kernel
$(KERNEL): $(call DIRFILES $(KERNELDIR))
	make -C $(KERNELDIR)

# Bootloader
$(BOOT): $(call DIRFILES $(BOOTDIR))
	make -C $(BOOTDIR)

# Clean
clean:
	rm -rf $(BOOT) $(KERNEL) $(FLPIMG)