bootdir=	src/boot
kerneldir=	src/kernel

boot=		$(bootdir)/boot.bin
kernel=		$(kerneldir)/kernel.bin
flpimg=		httpos.img
.PHONY: all clean

all: $(flpimg)

run: $(flpimg)
	qemu-system-x86_64 $(flpimg)

# Floppy image
$(flpimg): $(boot) $(kernel)
	dd if=/dev/zero of=$(flpimg) bs=512 count=2880
	mkfs.fat -F 12 $(flpimg)
	dd if=$(boot) of=$(flpimg) conv=notrunc
	truncate -s 1440k $(flpimg)
	mcopy -i $(flpimg) $(kernel) "::kernel.bin"

# Kernel
$(kernel): $(kerneldir)
	make -C $(kerneldir)

# Bootloader
$(boot): $(bootdir)
	make -C $(bootdir)

# Clean
clean:
	rm -r $(boot) $(flpimg)