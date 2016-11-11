#include <common.h>
#include <lib.h>
#include <moduleLoader.h>

#include "system/include/idt.h"
#include "system/include/mmu.h"
#include "drivers/include/video.h"
#include "system/include/syscalls.h"
#include "drivers/include/keyboard.h"
#include "system/scheduler/process.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef void* (*EntryPoint)();

char get_key();

void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}
void * initializeKernelBinary()
{
	char
		buffer[10];

	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	print("[x64BareBones]", -1);
	printNewline();

	print("CPU Vendor:", -1);
	print(cpuVendor(buffer), -1);
	printNewline();

	print("[Loading modules]", -1);
	printNewline();

	loadModules(&endOfKernelBinary, moduleAddresses);

	print("[Done]", -1);
	printNewline();
	printNewline();

	print("[Initializing kernel's binary]", -1);
	printNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	print("  text: 0x", -1);
	printHex((uint64_t)&text, -1);
	printNewline();

	print("  rodata: 0x", -1);
	printHex((uint64_t)&rodata, -1);
	printNewline();

	print("  data: 0x", -1);
	printHex((uint64_t)&data, -1);
	printNewline();

	print("  bss: 0x", -1);
	printHex((uint64_t)&bss, -1);
	printNewline();

	print("[Done]", -1);
	printNewline();
	printNewline();

	return getStackBase();
}

int main() {

	k_initialize();
	print("MMU ready.", -1);
	printNewline();

	_accelPIT();
	print("Accelerated IRQ0 to 1000Hz.", -1);
	printNewline();

	buildIDT();
	print("Interrupt Descriptor Table is ready.", -1);
	printNewline();

	keyboardInit();
	print("Keyboard ready.", -1);
	printNewline();
	
	print("Loading EntryPoint....", -1);
	printNewline();
	clear();

	//newProcess(&sampleCodeModuleAddress, "SHELL", TRUE);
	((EntryPoint)sampleCodeModuleAddress)();



	return 0;
}