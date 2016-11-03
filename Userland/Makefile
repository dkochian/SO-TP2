include Makefile.inc

all: lib shellModule

lib:
	@echo -e "Userland:"
	@cd libc; make --no-print-directory

shellModule:
	@cd shell; make --no-print-directory

clean:
	@cd libc; make clean --no-print-directory
	@cd shell; make clean --no-print-directory

.PHONY:
	libc shellModule all clean