#include "include/string.h"

int k_strlen(const char* str) {
	int
		i = 0;

	while(str[i] != '\0')
		i++;

	return i;
}

int k_strcmp(char *str1, char *str2)
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

int k_strcpy(char* dest, const char* src) {
	int
		index = 0;

	while(src[index] != '\0') {
		dest[index] = src[index];
		index++;
	}

	dest[index++] = '\0';
	return index;
}

int k_strcat(char* dest, const char* src) {
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
 /* itoa:  convert n to characters in s */
 void k_itoa(int n, char s[]){
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     k_reverse(s);
 }

  void k_reverse(char s[]) {
    int i, j;
    char c;
 	int length = k_strlen(s);
    for (i = 0, j = length -1; i < j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
    }
 }
