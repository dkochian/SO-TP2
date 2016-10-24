#include <shell.h>
#include <stdio.h>
#include <integer.h>
#include <string.h>
#include <syscall.h>
#include <fractals.h>
#include "../include/common.h"

static void * const sampleDataModuleAddress = (void*)0x500000;
static int readNumber();

void echoCommand(char* str) {
	printn(str);
}

void helpCommand(char* str) {
	commandExec* struc = getAllCommands();
	if(str[0] == '\0') {
		printn(struc[1].help);
		return;
	}
	for(int index = 0; index < MAX_COMMANDS; index++) {
		if(struc[index].created == TRUE && strcmp(str, struc[index].name) == 0) {
			printn(struc[index].help);
			return;
		}
	}
	printColor("Error", COLOR_ERROR);
	printn(": The command do not exist.");
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
		if(struc[index].created == TRUE){
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
	while(TRUE) {
		char
			c = getchar(FALSE);

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
	printnColor("\tOfficial Coder:", COLOR_INFO);
	printn("Jonas Kochian");
	printnColor("\tButterfly coder:", COLOR_INFO);
	printn("Juan Pablo Fernandes Rojo");
	printnColor("\tSales Manager:", COLOR_INFO);
	printn("Martin Capparelli");
	printn("Julian Sevilla");
	printnColor("\tBarebones:",COLOR_INFO);
	printn("Rodrigo Rearden (RowDaBoat)");
	printn("Augusto Nizzo Mc Intosh");
	printnColor("\tFractal:", COLOR_INFO);
	printn("Gaston Julia");
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
	char* str2;
	clearCommand(str2);
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
