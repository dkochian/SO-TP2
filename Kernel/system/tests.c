//
// Created by mariano on 11/13/16.
//

#include "include/tests.h"

int mmuStackTest() {

	int ret = true;

	void * first;
	void * second;

	first = k_malloc(1);
	second = k_malloc(1);

	strcpy(first, "FIRST");
	strcpy(second, "SECOND");

	k_free(first);
	k_free(second);

	second = k_malloc(1);
	first = k_malloc(1);

	if (strcmp(first, "FIRST") != 0) {
		ret = false;
	}

	if (strcmp(second, "SECOND") != 0) {
		ret = false;
	}

	k_free(second);
	k_free(first);

	return ret;
}



//AUX FUNCTIONS
int strlen(const char* str) {
	int
			i = 0;

	while(str[i] != '\0')
		i++;

	return i;
}

int strcmp(char *str1, char *str2)
{
	while((*str1 == *str2) && (*str1 != '\0')) {
		str1++;
		str2++;
	}

	if(*str1 > *str2)
		return 1;

	if(*str1 < *str2)
		return -1;

	return 0;
}

int strcpy(char* dest, const char* src) {
	int
			index = 0;

	while(src[index] != '\0') {
		dest[index] = src[index];
		index++;
	}

	dest[index++] = '\0';
	return index;
}