//BASED ON Wyrm/ProcessSlot
#include "processSlot.h"

ProcessSlot* newProcessSlot() {
	ProcessSlot* processSlot = k_malloc(sizeof(ProcessSlot));
	return processSlot;
}

void removeProcessSlot(ProcessSlot* processSlot) {
	k_free(processSlot);
}