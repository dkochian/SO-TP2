include Makefile.inc

KERNEL=kernel.bin
SYSTEM=
SOURCES= $(wildcard utils/*.c) $(wildcard system/ipc/*.c) $(wildcard system/scheduler/*.c) $(wildcard *.c) $(wildcard system/*.c) $(wildcard drivers/*.c) $(wildcard tests/*.c)
SOURCES_ASM=$(wildcard asm/*.asm) $(wildcard system/asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)
	@echo -e "Kernel		[ \033[0;32mOK \033[0m]"

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	@$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

%.o: %.c
	@$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	@$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	@$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	@rm -rf asm/*.o *.o *.bin system/*.o system/scheduler/*.o system/ipc/*.o system/asm/*.o drivers/*.o utils/*.o

.PHONY: all clean