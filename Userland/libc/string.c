#include <string.h>

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

int strcat(char* dest, const char* src) {
	int
		indexD = 0,
		indexS = 0;

	while(dest[indexD] != '\0') {
		indexD++;
	}

	while(src[indexS] != '\0') {
		dest[indexD] = src[indexS];
		indexS++;
		indexD++;
	}

	dest[indexD++] = '\0';
	return indexD;
}