#ifndef LIB_H
	#define LIB_H

	#include <stdint.h>

	void * memset(void * destination, int32_t character, uint64_t length);
	void * memcpy(void * destination, const void * source, uint64_t length);

	int strcpy(char* dest, const char* src);

	char *cpuVendor(char *result);

	void _accelPIT(void);

#endif