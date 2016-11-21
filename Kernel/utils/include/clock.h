#ifndef __CLOCK_H__
	#define __CLOCK_H__

	#include "../include/common.h"

	unsigned int seconds();
	unsigned int minutes();
	unsigned int hour();
	unsigned int month();
	unsigned int year();
	unsigned int day();
	unsigned int dayweak();

	void getTime(timeStruct* time);
	void getDate(dateStruct* date);
	void sleep(int secs);
	int rand();

#endif