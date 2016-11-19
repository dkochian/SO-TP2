#include "include/clock.h"
#include "../include/common.h"

void getTime(uint64_t time) {
	timeStruct* aux = (timeStruct*) time;
	uint8_t
		mins = minutes(),
		hrs = hour(),
		secs = seconds();

	if(hrs == 1)
		hrs = 10;
	else if(hrs == 2)
		hrs = 11;
	else
		hrs -= 3;
	
	aux->secs = secs;
	aux->mins = mins;
	aux->hrs = hrs;
}

void getDate(uint64_t date) {
	dateStruct*
		aux = (dateStruct*) date;
	uint8_t
		d = day(),
		m = month(),
		y = year(),
		dw = dayweak();

	aux->day = d;
	aux->dayOfWeak = dw;
	aux->month = m;
	aux->year = y;	
}

void sleep(int secs) {
	int
		secs_to_sleep = seconds(),
		minut = minutes(),
		segundos;

	secs_to_sleep += secs;
	if(secs_to_sleep > 60)
		minut += secs_to_sleep / 60;

	segundos = secs_to_sleep % 60;

	while(minutes() < minut) {};
	while(seconds() < segundos) {};	
}


int rand() {
	unsigned int
		secs = seconds(),
		secs2 = seconds();
	int
		random = 1;

	secs = secs << 1;
	secs += secs2 << 4;

	random += secs; 

	return random;
}