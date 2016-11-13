//
// Created by mariano on 11/12/16.
//
#include <test2.h>
#include "drivers/include/video.h"

void test2() {
	int i = 0;
	while(1) {
		sleep(i%3);
		print("-", -1);
		i++;
	}
}