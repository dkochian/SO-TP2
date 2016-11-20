#include <syscall.h>
#include <string.h>

static unsigned int countDigits(int num);

void putchar(char c) {
	write(STDOUT, &c, 1);
}

void putcharColor(char c, char color) {
	char bColor;
	getColor(&bColor);
	setColor(color);
	putchar(c);
	setColor(bColor);
}

char getchar(char write) {
	char c;

	read(STDIN, &c, write);

	return c;
}

void print(char* str) {
	write(STDOUT, str, strlen(str));
}

void printColor(char* str, char color) {
	char bColor;
	getColor(&bColor);
	setColor(color);
	print(str);
	setColor(bColor);
}

void printn(char* str) {
	print(str);
	putchar('\n');
}

void printnColor(char* str, char color) {
	char bColor;
	getColor(&bColor);
	setColor(color);
	printn(str);
	setColor(bColor);
}

void printNewLine() {
	putchar('\n');
}
void printNum(int num) {
	unsigned int digits = countDigits(num);
	char str[digits + 1];
	int k = 0;

	if(num < 0) {
		num *= -1;
		str[0] = '-';
		digits += 1;
		k = 1;
	}

	for(int i = digits-1; i >= k; i--) {
		str[i] = (num % 10) + '0';
		num /= 10;
	}
	str[digits] = '\0';
	print(str);
}

void toast(char* str, char color) {
	char bColor;
	getColor(&bColor);
	setColor(color);
	write(TOAST, str, strlen(str));
	setColor(bColor);
}

static unsigned int countDigits(int num) {
	int digits = 1;

	if (num < 0)
		num *= -1;
	while ((num /= 10) > 0)
		digits++;

	return digits;
}
