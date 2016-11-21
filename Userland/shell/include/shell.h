#ifndef _INC_SHELL_
	#define _INC_SHELL_

	#include "../../../Common/common.h"

	#define MAX_COMMANDS	21
	#define MAX_ARG_BUFFER	MAX_BUFFER - 2
	#define MAX_COLORS		4
	
	typedef struct {
		bool created;
		char* name;
		char* description;
		char* help;		
		int (*func)(int argc, char **argv);
	} commandExec;

	int main(int argc, char **argv);
	commandExec* getAllCommands();
	
#endif
