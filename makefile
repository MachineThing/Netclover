boot=	src/boot/boot.bin
.PHONY: all clean

all: $(boot)
	
$(boot):
	make -C src/boot


clean:
	rm -r $(boot)