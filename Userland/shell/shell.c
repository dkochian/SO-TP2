#include <shell.h>
#include <stdio.h>
#include <string.h>
#include <syscall.h>

#include "include/common.h"
#include "extra/include/commands.h"
#include "extra/include/philosophers.h"
#include "extra/include/producerConsumer.h"
#include "tests/include/freeprocess_test.h"
#include "tests/include/mutex_test.h"
#include "tests/include/scheduler_test.h"
#include "tests/include/semaphore_test.h"
#include "tests/include/video_test.h"
#include "tests/include/waitpid_test.h"
#include "tests/include/cond_var_test.h"
#include "tests/include/pipe_test.h"


typedef struct {
	int argc;
	char* name;
	char **argv;
	bool foreground;
} commandData;

static int insertToBuffer(char c);
static void resetBuffer();
static bool parseBuffer(commandData *cmd);
static void clearBuffer(commandData *cmd);
static bool executeCommand(commandData cmd);
static void printHeader();

char
	buffer[MAX_BUFFER];
int
	bIndex;
commandExec
	commandTable[MAX_COMMANDS] = {
		{true, "echo", "Prints the input.", "Usage: echo <text>", &echoCommand},
		{true, "help", "Prints information about a command.", "Usage: help <command>", &helpCommand},
		{true, "clear", "Clears the screen", "Usage: clear", &clearCommand},
		{true, "commands", "Prints all commands.", "Usage: commands", &commandsCommand},
		{true, "time", "Prints the time.", "Usage: time", &timeCommand},
		{true, "date", "Prints todays date.", "Usage: date", &dateCommand},
		{true, "sleep", "Sleeps the system.", "Usage: sleep <seconds>", &sleepCommand},
		{true, "fractal", "Draws a fractal. To exit the demostration press Esc key", "Usage: fractal", &fractalCommand},
		{true, "credits", "Prints the OS credits.", "Usage: credits", &creditsCommand},
		{true, "color", "Sets the color of the shell text.", 	"Usage: color <code>\n\n------------- Color codes ------------\n0 Black          1 Blue\n2 Green          3 Cyan\n4 Red            5 Magenta\n6 Brown          7 Light Gray\n8 Dark Grey      9 Light Blue\n10 Light Green   11 Light Cyan\n12 Light Red     13 Light Magenta\n14 Yellow        15 White\n--------------------------------------", &colorCommand},
		{true, "disclaimer", "Must read", "Usage: disclaimer", &disclaimerCommand},
		{true, "background", "Sets the color of the shell background.", 	"Usage: background <code>\n\n------------- Color codes ------------\n0 Black          1 Blue\n2 Green          3 Cyan\n4 Red            5 Magenta\n6 Brown          7 Light Gray\n8 Dark Grey      9 Light Blue\n10 Light Green   11 Light Cyan\n12 Light Red     13 Light Magenta\n14 Yellow        15 White\n--------------------------------------", &colorBgCommand},
		{true, "pid", "Prints the process pid.", "Usage: pid", &pidCommand},
		{true, "kill", "Kills the selected id process.", "Usage: kill <pid>", &killCommand},
		{true, "ps", "Prints all the processes information.", "Usage: ps", &psCommand},
		{true, "philosophers", "Starts Dining Philosophers problem.", "Usage: philosophers", &philosophers},
		{true, "prodcon", "Starts ProducerConsumer program.", "Usage: producerconsumer", &producerConsumer},
		{true, "ipc", "Shows all the IPC implemented.", "Usage: ipc", &ipcCommand},
		{true, "mtest", "Deploys mutext test.", "Usage: mtest", &mutexTestCommand},
		{true, "fptest", "Deploys free process test.", "Usage: fptest", &freeProcessTestCommand},
		{true, "sctest", "Deploys scheduler test.", "Usage: sctest", &schedulerTestCommand},
		{true, "semtest", "Deploys semaphore test.", "Usage: semtest", &semaphoreTestCommand},
		{true, "vidtest", "Deploys video test.", "Usage: vidtest", &videoTestCommand},
		{true, "wptest", "Deploys waitpid test.", "Usage: wptest", &waitpidTestCommand},
		{true, "cvtest", "Deploys condition variable test.", "Usage: cvtest", &varaibleConditionTestCommand},
		{true, "pitest", "Deploys pipe test.", "Usage: pitest", &pipeTestCommand}
	};

int main(int argc, char ** argv) {
	char
		c;
	commandData
		cmd;

	cmd.name = (char *) malloc(MAX_BUFFER*sizeof(char));
	cmd.argv = (char **) malloc(MAX_ARG_BUFFER*sizeof(char));

	bIndex = 0;

	printHeader();

	while(true) {
		c = getchar(true);

		if(c == EMPTY)
			print("fuck");

		if(insertToBuffer(c) == 1) {
			int
				result = parseBuffer(&cmd);//if result==0 empty line, result==1 try to execute command, result == -1 command's name is too long
			resetBuffer();

			if(strlen(cmd.name) != 0) {
				if(result == false) {
					printColor("Error", COLOR_ERROR);
					printn(": The command's name is too long (AKA: It doesn't exists.)");
					printn("Info: Use \"commands\" to see all avaliable commands.");
				}
				else {
					if(executeCommand(cmd) == false) {
						printColor("Error", COLOR_ERROR);
						printn(": The command doesn't exists.");
						printColor("Info", COLOR_INFO);
						printn(": Use \"commands\" to see all avaliable commands.");
					}
				}
			}

			printHeader();
		}
	}

	free(cmd.name);
	free(cmd.argv);
}

commandExec* getAllCommands() {
	return commandTable;
}

static int insertToBuffer(char c) {
	if(c == EMPTY)
		return 0;

	if(c == '\b') {
		buffer[bIndex] = '\0';
		if(bIndex > 0)
			bIndex--;

		return 0;
	}
	if(bIndex == MAX_BUFFER - 1)
		return -1;

	buffer[bIndex++] = c;

	if(c == '\n')
		return 1;

	return 0;
}

static void resetBuffer() {
	for(int i = 0; i < MAX_BUFFER; i++)
		buffer[i] = '\0';

	bIndex = 0;
}

static bool parseBuffer(commandData *cmd) {
	clearBuffer(cmd);

	int
		index = 0;

	if(buffer[0] == '&') {
		index++;
		cmd->foreground = true;
	}

	while(buffer[index] != '\n') {
		if(buffer[index] == ' ') {
			cmd->name[index] = '\0';
			break;
		}

		if(cmd->foreground == true)
			cmd->name[index - 1] = buffer[index];
		else
			cmd->name[index] = buffer[index];

		index++;

		if(index == MAX_BUFFER-1)
			return false;
	}

	if(buffer[index - 1] != '\n') {
		index++;

		strcpy(cmd->argv[0], &buffer[index]);

		index = strlen(cmd->argv[0]);

		if(index > 0)
			cmd->argc = 1;

		if(cmd->argc != 0  && cmd->argv[0][index - 1] == '\n')
			cmd->argv[0][index - 1] = '\0';
	}

	return true;
}
	
static void clearBuffer(commandData *cmd) {
	int i = 0;
	int flag = false;

	while(true) {
		flag = (i < MAX_BUFFER) & (i < MAX_ARG_BUFFER);
		if(i < MAX_BUFFER)
			cmd->name[i] = '\0';
		if(i < MAX_ARG_BUFFER)
			cmd->argv[0][i] = '\0';
		if(!flag)
			break;
		i++;
	}
	cmd->argc = 0;
	cmd->foreground = false;
}

static bool executeCommand(commandData cmd) {
	for(int index = 0; index < MAX_COMMANDS; index++) {
		if(commandTable[index].created == true && strcmp(cmd.name, commandTable[index].name) == 0) {
			if(cmd.foreground == true) {
				char tmp[MAX_BUFFER];
				int i = 0;

				strcpy(tmp, cmd.name);
				cmd.name[0] = '&';

				while(tmp[i] != '\0') {
					cmd.name[i + 1] = tmp[i];
					i++;
				}
			}
			
			uint64_t pid = newProcess(cmd.name, commandTable[index].func, cmd.argc, cmd.argv);
			wPid(pid);
			return true;
		}
	}
	return false;
}

static void printHeader() {
	printColor("root", GREEN);
	printColor("@", RED);
	printColor("Userland", GREEN);
	printColor(":", WHITE);
	printColor("$", RED);
}