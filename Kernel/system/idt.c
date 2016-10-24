#include "include/idt.h"

IDTEntry *
	IDTEntries = (IDTEntry *) 0x0;

void insertEntry(int dest, void* addr) {
	IDTEntries[dest].offsetLo = (int64_t) addr & 0xFFFF;
	IDTEntries[dest].selector = 0x08;
	IDTEntries[dest].z1 = 0;
	IDTEntries[dest].attributes = DEFAULT_ATTRIBUTES;
	IDTEntries[dest].offsetMi = ((int64_t) addr >> 16) & 0xFFFF;
	IDTEntries[dest].offsetHi = ((int64_t) addr >> 32) & 0xFFFFFFFF;
	IDTEntries[dest].z2 = 0;
}

void buildIDT() {
	_cli();

	insertEntry(0x20, &_timerTickHandler);
	insertEntry(0x21, &_keyboardHandler);
	insertEntry(0x80, &_sysCallHandler);

	_write_port(0x21, 0xFC);

	_sti();
}