include Makefile.inc

all: lib shellModule

lib:
	cd libc; make

shellModule:
	cd shell; make

clean:
	cd libc; make clean
	cd shell; make clean

.PHONY:
	libc shellModule all clean