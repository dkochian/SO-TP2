#include <common.h>
#include <lib.h>
#include <moduleLoader.h>

#include "system/include/idt.h"
#include "system/include/mmu.h"
#include "drivers/include/video.h"
#include "system/include/syscalls.h"
#include "drivers/include/keyboard.h"
#include "system/scheduler/include/process.h"
#include "system/scheduler/include/scheduler.h"

//Tests (AKA should be removed)
#include "tests/include/mutex_test.h"
#include "tests/include/scheduler_test.h"

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
	bool status = true;

	clear();

	k_initialize();
	print("Memory Manager	[ ", -1);
	print("OK", GREEN);
	print(" ]", -1);
	printNewline();

	print("Proces Manager	[ ", -1);
	if(buildProcessManager() == false) {
		print("ERROR", COLOR_ERROR);
		status = false;
	}
	else
		print("OK", GREEN);
	
	print(" ]", -1);
	printNewline();

	print("Scheduler						[ ", -1);
	if(buildScheduler() == false) {
		print("ERROR", COLOR_ERROR);
		status = false;
	}
	else
		print("OK", GREEN);
	
	print(" ]", -1);
	printNewline();


	buildIDT();
	print("IDT												[ ", -1);
	print("OK", GREEN);
	print(" ]", -1);
	printNewline();

	print("Keyboard							[ ", -1);
	if(keyboardInit() == false) {
		print("ERROR", COLOR_ERROR);
		status = false;
	}
	else
		print("OK", GREEN);
	print(" ]", -1);
	printNewline();

	if(!status)
		return 0;
	
	_accelPIT();	//scheduler runs and crashes everything
	print("PIT turbo			 		[ ", -1);
	print("OK", GREEN);
	print(" ]", -1);
	printNewline();
	
	clear();

	//Tests
	//startSchedulerTest();
	startLockTest();

	//((EntryPoint)sampleCodeModuleAddress)();

	return 0;
}