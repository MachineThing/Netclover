include ../../../config.mk

SRCFILES=	$(wildcard *.c)
OBJFILES=	$(SRCFILES:.c=.o)

all: $(OBJFILES)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@