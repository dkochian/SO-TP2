#include <shell.h>
#include <stdio.h>
#include <integer.h>
#include <string.h>
#include <syscall.h>

#include "include/fractal.h"
#include "include/commands.h"
#include "../include/common.h"

static void *const sampleDataModuleAddress = (void *) 0x500000;
static int readNumber();

int echoCommand(int argc, char **argv) {
	if(argc == 0) {
		helpCommand(1, (char **) "echo");
		return 1;
	}

	printn(argv[0]);
	return 0;
}

int helpCommand(int argc, char **argv) {
	commandExec* struc = getAllCommands();
	if(argc == 0) {
		printn(struc[1].help);
		return 1;
	}

	for(int index = 0; index < MAX_COMMANDS; index++) {
		if(struc[index].created == true && strcmp(argv[0], struc[index].name) == 0) {
			printn(struc[index].help);
			return 0;
		}
	}

	printColor("Error", COLOR_ERROR);
	printn(": The command do not exist.");
	return 1;
}

int pidCommand(int argc, char **argv) {
	if(argc != 0) {
		helpCommand(1, (char **) "pid");
		return 1;
	}

	print("El id del proceso actual es: ");
	printNum(pid());
	printNewline();
	return 0;
}

int psCommand(int argc, char **argv) {
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

	return 0;
}

int killCommand(int argc, char **argv) {
	if(argc == 0) {
		helpCommand(1, (char **) "kill");
		return 1;
	}

	int pidNumber = strint(argv[0]);
	if(pidNumber < 0) {
		helpCommand(1, (char **) "kill");
		return 1;
	}

	kill((uint64_t) pidNumber);
	return 0;
}

int clearCommand(int argc, char **argv) {
	if(argc != 0) {
		helpCommand(1, (char **) "clear");
		return 1;
	}

	clear();
	return 0;
}

int commandsCommand(int argc, char **argv) {
	if(argc != 0) {
		helpCommand(1, (char **) "commands");
		return 1;
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
	return 0;
}

int dateCommand(int argc, char **argv) {
	if(argc != 0) {
		helpCommand(1, (char **) "date");
		return 1;
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
	return 0;
}

int timeCommand(int argc, char **argv) {
	if(argc != 0) {
		helpCommand(1, (char **) "time");
		return 1;
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
	return 0;
}

int sleepCommand(int argc, char **argv) {
	if(argc == 0) {
		helpCommand(1, (char **) "sleep");
		return 1;
	}

	int
		seconds = strint(argv[0]);

	if(seconds == -1) {
		printColor("Error", COLOR_ERROR);
		printn(": Invalid number of seconds.");
		return 1;
	}

	sleep(seconds);

	return 0;
}

int fractalCommand(int argc, char **argv) {
	if(argc != 0) {
		helpCommand(1, (char **) "fractal");
		return 1;
	}

	clearCommand(0, (char **) "");
	fractal(readNumber());
	toast("PRESS Q KEY TO RETURN TO THE CONSOLE...", WHITE);
	while(true) {
		char
			c = getchar(false);

		if(c == 'q' || c == 'Q')
			break;
	}
	clearCommand(0, (char **) "");
	return 1;
}

int creditsCommand(int argc, char **argv) {
	if(argc != 0) {
		helpCommand(1, (char **) "credits");
		return 1;
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

	return 0;
}

int ipcCommand(int argc, char **argv){
	if(argc != 0) {
		helpCommand(1, (char **) "ipc");
		return 1;
	}

	printn("------------IPC implemented------------");
	printnColor("\t. Global Mutexes ", LIGHT_BLUE);
	printnColor("\t. Global variables condition", YELLOW);
	printnColor("\t. Semaphores", LIGHT_BLUE);

	printn("-------------------------------");
	return 0;
}

int colorCommand(int argc, char **argv) {
	if(argc == 0) {
		helpCommand(1, (char **) "color");
		return 1;
	}

	int color = strint(argv[0]);

	if(color < 0 || color > 15) {
		printColor("Error", COLOR_ERROR);
		printn(": Invalid color. Type help color for more information");
		return 1;
	}
	
	setColor(color);

	return 0;
}

int colorBgCommand(int argc, char **argv) {
	if(argc == 0) {
		helpCommand(1, (char **) "background");
		return 1;
	}

	int color = strint(argv[0]);

	if(color < 0 || color > 15) {
		printColor("Error", COLOR_ERROR);
		printn(": Invalid color. Type help color for more information");
		return 1;
	}
	
	setBgColor(color);
	clearCommand(0, (char **) "");

	return 0;
}

int disclaimerCommand(int argc, char **argv) {
	if(argc != 0) {
		helpCommand(1, (char **) "disclaimer");
		return 1;
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

	return 0;
}

int mutextest(int argc, char **argv) {
	if(argc != 0) {
		helpCommand(1, (char **) "test");
		return 1;
	}

	void *m;
	bool lock;

	m = sysMutexInit();
	if(m == NULL) {
		printn("Couldn't create the mutex");
		return 1;
	}
	printn("Muted Created");

	sysMutexLock(m);
	sysMutexIsLocked(m, &lock);
	if(lock == false) {
		printn("Lock failed");
		sysMutexDestroy(m);
		return 1;
	}
	printn("Locked successfully");

	sysMutexUnlock(m);
	sysMutexIsLocked(m, &lock);
	if(lock == true) {
		printn("Unlock failed");
		sysMutexDestroy(m);
		return 1;
	}
	printn("Unlocked successfully");

	sysMutexDestroy(m);
	printn("Mutex destroyed");

	return 0;
}

static int readNumber() {
  char *stringNumber = sampleDataModuleAddress;
  int number = 0;
  int index = 0;

  while (stringNumber[index] != '\0'){
    number *= 10;
    number += stringNumber[index];
    index++;
  }

  return number;
}