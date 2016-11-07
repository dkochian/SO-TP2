all: toolchain bootloader kernel userland video image
	@echo -e "\n\e[1;32mProject build successfully\033[0m"

toolchain:
	@cd Toolchain; make all --no-print-directory

bootloader:
	@cd Bootloader; make all --no-print-directory

kernel:
	@cd Kernel; make all --no-print-directory

userland:
	@cd Userland; make all --no-print-directory

image: kernel bootloader userland
	@cd Image; make all --no-print-directory

clean:	
	@cd Toolchain; make clean --no-print-directory
	@cd Bootloader; make clean --no-print-directory
	@cd Image; make clean --no-print-directory
	@cd Kernel; make clean --no-print-directory
	@cd Userland; make clean --no-print-directory

run: clean all
	qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -vga std

.PHONY: toolchain bootloader image collections kernel userland video all clean run