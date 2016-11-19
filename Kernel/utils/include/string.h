#ifndef _INC_STRING_
	#define _INC_STRING_
	
	#include "../include/common.h"

	int k_strlen(const char* str);
	int k_strcmp(char *str1, char *str2);
	int k_strcpy(char* dest, const char* src);
	int k_strcat(char* dest, const char* src);
	void k_itoa(int n, char s[]);
	void k_reverse(char s[]);

#endif