#include <shell.h>
#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <commands.h>
#include "../../Common/common.h"

static void shell();
static int insertToBuffer(char c);
static void resetBuffer();
static int parseBuffer(commandData* cmd);
static void clearBuffer(commandData* cmd);
static int bufferHasAlpha();
static int executeCommand(commandData cmd);
static void printHeader();

char
	buffer[MAX_BUFFER];
int
	bIndex;
commandExec
	commandTable[MAX_COMMANDS] = {
		{TRUE, "echo", "Prints the input.", "Usage: echo <text>", &echoCommand},
		{TRUE, "help", "Prints information about a command.", "Usage: help (command)", &helpCommand},
		{TRUE, "clear", "Clears the screen", "Usage: clear", &clearCommand},
		{TRUE, "commands", "Prints all commands.", "Usage: commands", &commandsCommand},
		{TRUE, "time", "Prints the time.", "Usage: time", &timeCommand},
		{TRUE, "date", "Prints the date.", "Usage: date", &dateCommand},
		{TRUE, "sleep", "Sleeps the system.", "Usage: sleep <seconds>", &sleepCommand},
		{TRUE, "fractal", "Draws a fractal. To exit the demostration press Esc key", "Usage: fractal", &fractalCommand},
		{TRUE, "credits", "Prints the OS credits.", "Usage: credits", &creditsCommand},
		{TRUE, "color", "Sets the color of the shell text.", 	"Usage: color <code>\n\n------------- Color codes ------------\n0 Black          1 Blue\n2 Green          3 Cyan\n4 Red            5 Magenta\n6 Brown          7 Light Gray\n8 Dark Grey      9 Light Blue\n10 Light Green   11 Light Cyan\n12 Light Red     13 Light Magenta\n14 Yellow        15 White\n--------------------------------------", &colorCommand},
		{TRUE, "disclaimer", "Must read", "Usage: disclaimer", &disclaimerCommand},
		{TRUE, "background", "Sets the color of the shell background.", 	"Usage: background <code>\n\n------------- Color codes ------------\n0 Black          1 Blue\n2 Green          3 Cyan\n4 Red            5 Magenta\n6 Brown          7 Light Gray\n8 Dark Grey      9 Light Blue\n10 Light Green   11 Light Cyan\n12 Light Red     13 Light Magenta\n14 Yellow        15 White\n--------------------------------------", &colorBgCommand},
		{TRUE, "pid", "Prints the process pid.", "Usage: pid", &pidCommand},
		{TRUE, "kill", "Kills the selected process.", "Usage: kill <pid>", &killCommand},
		{TRUE, "ps", "Prints all the processes information.", "Usage: ps", &ps}

	};

int main(int argc, char ** argv) {

	printn("main called");
	
	newProcess((void *)&shell, "SHELL", TRUE);

	shell();
	while(1) { }
	return 1;
}

static void shell() {
		char
		c;
	commandData
		cmd;

	cmd.name = malloc(MAX_BUFFER*sizeof(char));
	cmd.args = malloc(MAX_ARG_BUFFER*sizeof(char));

	bIndex = 0;

	printHeader();

	while(TRUE) {
		//printn("[SHELL] while(TRUE)");
		c = getchar(TRUE);

		if(insertToBuffer(c) == 1) {
			printn("0");
			int
				result = parseBuffer(&cmd);//if result==0 empty line, result==1 try to execute command, result == -1 command's name is too long
			printn("1");
			resetBuffer();
			print("cmd.name: ");
			printn(cmd.name);
			print("cmd.args: ");
			printn(cmd.args);
			if(result == -1) {
				printn("1.1");
				printColor("Error", COLOR_ERROR);
				printn(": The command's name is too long (AKA: It doesn't exists.)");
				printn("Info: Use \"commands\" to see all avaliable commands.");
			}
			else if(result == 1) {
				printn("1.2");
				if(!executeCommand(cmd)) {
					printColor("Error", COLOR_ERROR);
					printn(": The command doesn't exists.");
					printColor("Info", COLOR_INFO);
					printn(": Use \"commands\" to see all avaliable commands.");
				}
			}
			else
				print("");//It's it's an empty buffer, it halt unless we do something here, like print nothing.
			printHeader();
		}
	}

	free(cmd.name);
	free(cmd.args);
}

commandExec* getAllCommands() {
	return commandTable;
}

static int insertToBuffer(char c) {
	if(c == '\b') {
		buffer[bIndex] = '\0';
		if(bIndex > 0)
			bIndex--;

		return 0;
	}
	else if(c == '\n')
		buffer[bIndex++] = '\0';
	else {
		if(bIndex == MAX_BUFFER - 1)
			return -1;

		buffer[bIndex++] = c;

		return 0;
	}
	return 1;
}

static void resetBuffer() {
	for(int i = 0; i < MAX_BUFFER; i++)
		buffer[i] = '\0';

	bIndex = 0;
}

static int parseBuffer(commandData* cmd) {
	clearBuffer(cmd);
	int
		index = 0; 
	if(!bufferHasAlpha())
		return 0;

	while(buffer[index] != '\0') {
		if(buffer[index] == ' ' || buffer[index] == '\n') {
			cmd->name[index] = '\0';
			break;
		}

		cmd->name[index] = buffer[index];
		index++;
		if(index == MAX_BUFFER-1)
			return -1;
	}

	strcpy(cmd->args, &buffer[++index]);

	index = strlen(cmd->args);
	if(cmd->args[index - 1] == '\n')
		cmd->args[index - 1] = '\0';

	return 1;
}
static int bufferHasAlpha() {
	int
		index = 0;

	while(buffer[index] != '\0') {		
		if(buffer[index] != ' ' && buffer[index] != '\n')
			return 1;

		index++;
	}
	return 0;
}
	

static void clearBuffer(commandData* cmd) {
	int i = 0;
	int flag = FALSE;
	while(TRUE) {
		flag = (i < MAX_BUFFER) & (i < MAX_ARG_BUFFER);
		if(i < MAX_BUFFER)
			cmd->name[i] = '\0';
		if(i < MAX_ARG_BUFFER)
			cmd->args[i] = '\0';
		if(!flag)
			break;
		i++;
	}
}

static int executeCommand(commandData cmd) {
	for(int index = 0; index < MAX_COMMANDS; index++) {
		if(commandTable[index].created == TRUE && strcmp(cmd.name, commandTable[index].name) == 0) {
			commandTable[index].func(cmd.args);
			return 1;
		}
	}
	return 0;
}

static void printHeader() {
	printColor("root", GREEN);
	printColor("@", RED);
	printColor("Userland", GREEN);
	printColor(":", WHITE);
	printColor("$", RED);
}