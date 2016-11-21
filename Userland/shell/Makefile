include ../Makefile.inc

MODULE=shellModule.bin
SOURCES=_loader.c shell.c extra/commands.c extra/fractal.c extra/philosophers.c extra/producerConsumer.c extra/buffer.c extra/visualProdCons.c $(wildcard tests/*.c)
OBJECTS=$(SOURCES:.c=.o)
LIBC=-L../ -lc

all: $(MODULE) video
	@echo -e "	Shell		[ \033[0;32mOK \033[0m]"

$(MODULE): $(OBJECTS)
	@$(LD) -T shell.ld $(OBJECTS) $(LIBC) -o ../$(MODULE)

%.o: %.c
	@$(GCC) $(GCCFLAGS) -c $< -o $@

video:
	@cat Video >> video.bin && dd if=/dev/zero bs=1 count=1 >> video.bin

clean:
	@rm -rf extra/*.o test/*.o *.o ../*.bin *.bin

.PHONY: all clean video