#ifndef _INC_SHELL_
	#define _INC_SHELL_

	#define MAX_COMMANDS	15
	#define MAX_ARG_BUFFER	MAX_BUFFER - 2
	#define MAX_COLORS		4
	
	typedef struct {
		short created;
		char* name;
		char* description;
		char* help;
		void (*func)(char* arguments);
	} commandExec;

	typedef struct {
		char* name;
		char* args;
	} commandData;

	int main(int argc, char ** argv);
	commandExec* getAllCommands();
	void shellPrint(char* str, short mustPrint, short newline);
	
#endif
