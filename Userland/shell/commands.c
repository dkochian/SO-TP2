#include <shell.h>
#include <stdio.h>
#include <integer.h>
#include <string.h>
#include <syscall.h>
#include <fractals.h>
#include "../../Common/common.h"

static void * const sampleDataModuleAddress = (void*)0x500000;
static int readNumber();

void echoCommand(char* str) {
	printn(str);
	//rPid(pid());
}

void helpCommand(char* str) {
	commandExec* struc = getAllCommands();
	if(str[0] == '\0') {
		printn(struc[1].help);
		return;
	}
	for(int index = 0; index < MAX_COMMANDS; index++) {
		if(struc[index].created == true && strcmp(str, struc[index].name) == 0) {
			printn(struc[index].help);
			return;
		}
	}
	printColor("Error", COLOR_ERROR);
	printn(": The command do not exist.");
}

void pidCommand(char *str) {
	if(strlen(str)) {
		helpCommand("pid");
		return;
	}
	printColor("El pid actual es : ", LIGHT_BLUE);
	printNum(pid());
	printNewline();
}

void psCommand(char *str){
	psContext * allProcess = ps();
	char separate = allProcess->separateChar;
	
	printColor("Number of processes: ", LIGHT_GRAY);
	printNum(allProcess->numbProcess);
	printNewline();

	int max = allProcess->numbProcess;
	int i;
	int term = 0;
	for( i = 0; i < max; i++){
		int index = 0;
		char c = allProcess->processes[i][index];
		
		printColor("PID : ", LIGHT_MAGENTA);
		while( c != '\0'){
			c = allProcess->processes[i][index++];
			
			if (c == separate && term == 0){
				printColor("   NAME : ", LIGHT_MAGENTA);
				term++;
			}else if (c == separate && term == 1){
				printColor("     STATE :", LIGHT_MAGENTA);
				term++;
			}else if (term == 2)	{
				if (c == '0')
					printColor(" RUNNING", CYAN);
				else if(c == '1')
					printColor(" BLOCKED", RED);
				else
					printColor(" WAITING", LIGHT_BLUE);

				printColor("    FOREGROUND :", LIGHT_MAGENTA);
				term++;
			} else if (term == 3) {
				term = 0;
				c = allProcess->processes[i][index++];
				if (c == '1')	
					printColor(" TRUE", YELLOW);
				else
					printColor(" FALSE", BROWN);
			}else
				putchar(c);
		}
		printColor("     USING MEMORY : ", LIGHT_MAGENTA);
		print("4KB");
		printNewline();
	}
	free(allProcess->processes);
	free(allProcess);
}

void killCommand(char* str) {
	int pidNumber = strint(str);
	if(pidNumber == -1 || strlen(str) == 0) {
		helpCommand("kill");
		return;
	}
	kill((uint64_t) pidNumber);
}

void clearCommand(char* str) {
	if(strlen(str)) {
		helpCommand("clear");
		return;
	}
	clear();
}

void commandsCommand(char* str) {
	if(strlen(str)) {
		helpCommand("commands");
		return;
	}

	const
		commandExec*  struc = getAllCommands();

	printn("---------- Commands ---------");
	
	for(int index = 0; index < MAX_COMMANDS; index++){
		if(struc[index].created == true){
			putchar('\t');
			printColor(struc[index].name, COLOR_INFO);
			print(": ");
			printn(struc[index].description);
		}
	}	
	printn("---------- End Commands ---------");
}

void dateCommand(char* str) {
	if(strlen(str)) {
		helpCommand("date");
		return;
	}
	dateStruct
		date;
	printn("adentro dateCommand");
	getDate(&date);

	print("Hoy es ");
	switch(date.dayOfWeak){
		case 1:
			print("Domingo");
			break;
		case 2:
			print("Lunes");
			break;
		case 3:
			print("Martes");
			break;
		case 4:
			print("Miercoles");
			break;
		case 5:
			print("Jueves");
			break;
		case 6:
			print("Viernes");
			break;
		case 7:
			print("Sabado");
			break;
	}
	print(" ");
	printNum(date.day);
	print(" de ");
	switch(date.month){
		case 1:
			print("Enero");
			break;
		case 2:
			print("Febrero");
			break;
		case 3:
			print("Marzo");
			break;
		case 4:
			print("Abril");
			break;
		case 5:
			print("Mayo");
			break;
		case 6:
			print("Junio");
			break;
		case 7:
			print("Julio");
			break;
		case 8:
			print("Agosto");
			break;
		case 9:
			print("Septiembre");
			break;
		case 10:
			print("Octubre");
			break;
		case 11:
			print("Noviembre");
			break;
		case 12:
			print("Diciembre");
			break;
	}
	print(" de 20");
	printNum(date.year);
	printNewline();
}

void timeCommand(char* str) {
	if(strlen(str)) {
		helpCommand("time");
		return;
	}

	timeStruct
		timeSystem;

	getTime(&timeSystem);

	print("La hora es ");
	
	if(timeSystem.hrs < 10)
		printNum(0);
	printNum(timeSystem.hrs);

	print(":");
	if(timeSystem.mins < 10)
		printNum(0);

	printNum(timeSystem.mins);
	print(":");
	
	if(timeSystem.secs < 10)
		printNum(0);
	printNum(timeSystem.secs);
	printNewline();
}

void sleepCommand(char* str) {
	if(!strlen(str)) {
		helpCommand("sleep");
		return;
	}
	int
		seconds = strint(str);

	if(seconds == -1) {
		printColor("Error", COLOR_ERROR);
		printn(": Invalid number of seconds.");
		return;		
	}

	sleep(seconds);
}

void fractalCommand(char* str) {
	clearCommand(str);
	if(strlen(str)) {
		helpCommand("fractal");

		return;
	}
	fractal(readNumber());
	toast("PRESS Q KEY TO RETURN TO THE CONSOLE...", WHITE);
	while(true) {
		char
			c = getchar(false);

		if(c == 'q' || c == 'Q')
			break;
	}
	clearCommand(str);
}

void creditsCommand(char* str) {
	if(strlen(str)) {
		helpCommand("credits");
		return;
	}

	printn("------------Credits------------");
	printnColor("\tOfficial Coders:", COLOR_INFO);

	printn("Juan Fraser\t53100");
	printn("Mariano Garrigó Montagna\t54393");
	printn("Juan Pablo Fernandes Rojo\t53164");
	printn("Jonas Kochian\t54110");

	printnColor("\tBarebones:",COLOR_INFO);
	printn("Rodrigo Rearden");
	printn("Ariel Godio");
	printnColor("\tFractal:", COLOR_INFO);
	printn("Gaston Julia");
	printn("-------------------------------");
}

void ipcCommand(char* str){
	if(strlen(str)) {
		helpCommand("ipcCommand");
		return;
	}

	printn("------------IPC implemented------------");
	printnColor("\t. Global Mutexes ", LIGHT_BLUE);
	printnColor("\t. Global variables condition", YELLOW);
	printnColor("\t. Semaphores", LIGHT_BLUE);

	printn("-------------------------------");
}

void colorCommand(char* str) {
	if(!strlen(str)) {
		helpCommand("color");
		return;
	}

	int color = strint(str);

	if(color < 0 || color > 15) {
		printColor("Error", COLOR_ERROR);
		printn(": Invalid color. Type help color for more information");
		return;
	}
	
	setColor(color);
}

void colorBgCommand(char* str) {
	if(!strlen(str)) {
		helpCommand("background");
		return;
	}

	int color = strint(str);

	if(color < 0 || color > 15) {
		printColor("Error", COLOR_ERROR);
		printn(": Invalid color. Type help color for more information");
		return;
	}
	
	setBgColor(color);
	clearCommand("");
}

void disclaimerCommand(char* str) {
	if(strlen(str)) {
		helpCommand("disclaimer");
		return;
	}

	printnColor("|-----------------------------------------------------|", 0x07);
	printnColor("|                     Disclaimer                      |", 0x07);
	printnColor("|-----------------------------------------------------|", 0x07);
	printnColor("|Any crash that may occur. Must (for sure) be caused  |", 0x07);
	printnColor("|by the change in the flow of the eddy currents in the|", 0x07);
	printnColor("|upper atmosphere, generated by the single flap of a  |", 0x07);
	printColor("|", 0x07);
	printColor ("BUTTERFLY", COLOR_ERROR);
	printnColor(".                                           |", 0x07);
	printnColor("|-----------------------------------------------------|", 0x07);
}

static int readNumber(){
  char* stringNumber = sampleDataModuleAddress;
  int number = 0;
  int index = 0;
  while (stringNumber[index] != '\0'){
    number *= 10;
    number += stringNumber[index];
    index++;
  }
  return number;
}

void mutextest(char* str) {
	if(strlen(str)) {
		helpCommand("mTest");
		return;
	}
	void * m;
	bool lock;

	m = sysMutexInit();
	if(m == NULL) {
		printn("Couldn't create the mutex");
		return;
	}
	printn("Muted Created");

	sysMutexLock(m);
	sysMutexIsLocked(m, &lock);
	if(lock == false) {
		printn("Lock failed");
		sysMutexDestroy(m);
		return;
	}
	printn("Locked successfully");

	sysMutexUnlock(m);
	sysMutexIsLocked(m, &lock);
	if(lock == true) {
		printn("Unlock failed");
		sysMutexDestroy(m);
		return;
	}
	printn("Unlocked successfully");

	sysMutexDestroy(m);
	printn("Mutex destroyed");
}