//
// Created by mariano on 11/12/16.
//
#include <test1.h>
#include "drivers/include/video.h"

void test1() {
	int i = 0;
	while(1) {
		sleep(i%2);
		print(".", -1);
		i++;
	}
}