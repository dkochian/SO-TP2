include ../Makefile.inc

LIB=../libc.a
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: $(LIB)
	@echo -e "	libC		[ \033[0;32mOK \033[0m]"

$(LIB): $(OBJECTS) $(OBJECTS_ASM)
	@$(AR) $(ARFLAGS) -c $(LIB) $(OBJECTS_ASM) $(OBJECTS)

$(OBJECTS): $(SOURCES)

$(OBJECTS_ASM): $(SOURCES_ASM)

%.o: %.c
	@$(GCC) $(GCCFLAGS) -c $< -o $@

%.o: %.asm
	@$(ASM) $(ASMFLAGS) $< -o $@

clean:
	@rm -rf *.o ../*.a

.PHONY:
	all clean