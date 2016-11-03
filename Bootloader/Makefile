all: bmfs pure64

bmfs:
	@echo -e "Bootloader:"
	@cd BMFS; $(MAKE) all --no-print-directory

pure64:
	@cd Pure64; $(MAKE) all --no-print-directory

clean:
	@cd BMFS; $(MAKE) clean --no-print-directory
	@cd Pure64; $(MAKE) clean --no-print-directory

.PHONY: all bmfs pure64 clean