include src/config.mk

BOOTDIR=	src/boot
KERNELDIR=	src/kernel
STATICDIR=	static

BOOT=		$(BOOTDIR)/boot.bin
KERNEL=		$(KERNELDIR)/kernel.bin
FLPIMG=		httpos.img

.PHONY: all run clean

all: $(FLPIMG)

bochs: $(FLPIMG)
	bochs -f bochs_config -q

run: $(FLPIMG)
	$(QEMU) $(FLPIMG)

# Floppy image
$(FLPIMG): $(BOOT) $(KERNEL) $(call DIRFILES $(STATICDIR))
	$(eval RESERVEDSIZE=$(shell expr $$(stat -c %s $(BOOT)) / 512 + 1))

	dd if=/dev/zero of=$(FLPIMG) bs=512 count=2880
	mkfs.fat -F 12 -R $(RESERVEDSIZE) $(FLPIMG)
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
	find . -name "*.bin" -type f -delete
	find . -name "*.o" -type f -delete
	rm -rf $(FLPIMG)