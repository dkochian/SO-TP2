#include <common.h>
#include <moduleLoader.h>

#include "utils/include/lib.h"
#include "system/include/idt.h"
#include "system/include/mmu.h"
#include "drivers/include/video.h"
#include "system/include/syscalls.h"
#include "drivers/include/keyboard.h"
#include "system/ipc/include/pipe.h"
#include "system/scheduler/include/process.h"
#include "system/scheduler/include/scheduler.h"

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

void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void *getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void *initializeKernelBinary()
{
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main() {
	bool pmStatus, scStatus, kbStatus, piStatus;

	mmuBuild();
	if(videoBuild() == false)
		return 1;
	
	pmStatus = buildProcessManager();
	scStatus = buildScheduler();
	kbStatus = keyboardInit();
	piStatus = pipeInit();
	buildIDT();
	_accelPIT();

	print("Memory Manager	[ ", -1);
	print("OK", GREEN);
	print(" ]\n", -1);

	print("Video driver			[ ", -1);
	print("OK", GREEN);
	print(" ]\n", -1);

	print("Proces Manager	[ ", -1);
	if(scStatus == true)
		print("OK", GREEN);
	else
		print("ERROR", COLOR_ERROR);
	print(" ]\n", -1);

	print("Scheduler						[ ", -1);
	if(scStatus == true)
		print("OK", GREEN);
	else
		print("ERROR", COLOR_ERROR);
	print(" ]\n", -1);

	print("Pipes											[", -1);
	if(piStatus == true)
		print("OK", GREEN);
	else
		print("ERROR", COLOR_ERROR);
	print(" ]\n", -1);

	print("IDT												[ ", -1);
	print("OK", GREEN);
	print(" ]\n", -1);

	print("Keyboard							[ ", -1);
	if(kbStatus == true)
		print("OK", GREEN);
	else
		print("ERROR", COLOR_ERROR);
	print(" ]\n", -1);

	print("PIT turbo						[ ", -1);
	print("OK", GREEN);
	print(" ]\n", -1);

	if(!(pmStatus & scStatus & kbStatus & piStatus))
		return 1;

	//videoStartLocking(); //TODO: Uncomment this line, it's commented just for the pipe's testing.

	clear();
	((EntryPoint)sampleCodeModuleAddress)();

	return 0;
}