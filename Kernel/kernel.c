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

#include <clock.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

static int processA(int argc, char **argv);
static int processB(int argc, char **argv);

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

	keyboardInit();
	print("Keyboard							[ ", -1);
	print("OK", GREEN);
	print(" ]", -1);
	printNewline();

	if(!status)
		return 0;
	
	//clear();


	//Test multi-task
	uint64_t pA;
	uint64_t pB;
	print("Creating process A", -1);
	printNewline();
	pA = newProcess("Process A", processA, 0, NULL);
	if(pA == INVALID_PROCESS_ID) {
		print("Couldn't create process A", -1);
		printNewline();
	}
	else {
		print("Process A id: ", -1);
		printDec(pA, -1);
		printNewline();
	}

	print("Creating process B", -1);
	printNewline();
	pB = newProcess("Process B", processB, 0, NULL);
	if(pB == INVALID_PROCESS_ID) {
		print("Couldn't create process B", -1);
		printNewline();
	}
	else {
		print("Process B id: ", -1);
		printDec(pB, -1);
		printNewline();
	}
	
	//newProcess(&sampleCodeModuleAddress, "SHELL", true);
	//((EntryPoint)sampleCodeModuleAddress)();

	return 0;
}

static int processA(int argc, char **argv) {
	static int counter = 0;

	print("Process A is running", -1);
	printNewline();

	while(true) {
		print("Process A: row ", -1);
		printDec(counter++, -1);
		printNewline();

		sleep(1);
	}

	return 0;
}

static int processB(int argc, char **argv) {
	static int counter = 0;

	print("Process B is running", -1);
	printNewline();

	while(true) {
		print("Process B: row ", -1);
		printDec(counter++, -1);
		printNewline();

		sleep(1);
	}

	return 0;
}