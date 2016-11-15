#include <string.h>
#include <integer.h>

int strint(const char* str) {
	int
		result = 0,
		index = 0,
		current,
		flag = 0;

	while(str[index] != '\0') {
		if(index == 0 && str[index] == '-') {
			flag = 1;
			continue;
		}

		current = str[index] - '0';

		if(current < 0 || current > 9)
			return -1;

		result = result*10 + current;
		index++;
	}

	return (flag ? (-1)*result : result);
}

int strintPos(const char* str) {
	int
		result = 0,
		index = 0,
		current;

	while(str[index] != '\0') {

		current = str[index] - '0';

		if(current < 0 || current > 9)
			return -1;

		result = result*10 + current;
		index++;
	}

	return result;
}
void reverse(char s[]) {
	int i, j;
	char c;
	for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}
int isNumber (unsigned char n) {
	int t = ('0' <= n && n <= '9');
	return t;
}
void itoa(int n, char* s) {
	int i, sign;
	if ((sign = n) < 0)
		n = -n;
	i = 0;
	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}
