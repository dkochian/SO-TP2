#include "include/visualProdCons.h"

#define BUFFER_SIZE 10
const int bufferSize = BUFFER_SIZE;

int r = 0;
int w = 0;
char auxbuffer[BUFFER_SIZE];

void insertItem(char c);
char removeItem();

void insertItem(char c) {
	clearConsumer();
	auxbuffer[w] = c;
	w = (w + 1) % bufferSize;
	visualProduce(c);
}

char removeItem() {
	char result = auxbuffer[r];
	r = (r + 1) % bufferSize;
	visualConsume(result);
	return result;
}