#include <stdint.h>
#include "utils/include/lib.h"
#include <moduleLoader.h>
#include "drivers/include/video.h"

static void loadModule(uint8_t ** module, void * targetModuleAddress);
static uint32_t readUint32(uint8_t ** address);

void loadModules(void * payloadStart, void ** targetModuleAddress)
{
	int i;
	uint8_t * currentModule = (uint8_t*)payloadStart;
	uint32_t moduleCount = readUint32(&currentModule);

	for (i = 0; i < moduleCount; i++)
		loadModule(&currentModule, targetModuleAddress[i]);
}

static void loadModule(uint8_t ** module, void * targetModuleAddress)
{
	uint32_t moduleSize = readUint32(module);

	print("  Will copy module at 0x", -1);
	printHex((uint64_t)*module, -1);
	print(" to 0x", -1);
	printHex((uint64_t)targetModuleAddress, -1);
	print(" (", -1);
	printDec(moduleSize, -1);
	print(" bytes)", -1);

	memcpy(targetModuleAddress, *module, moduleSize);
	*module += moduleSize;

	print(" [Done]", -1);
	printNewline();
}

static uint32_t readUint32(uint8_t ** address)
{
	uint32_t result = *(uint32_t*)(*address);
	*address += sizeof(uint32_t);
	return result;
}
