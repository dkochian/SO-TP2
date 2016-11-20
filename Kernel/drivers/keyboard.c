#include "include/video.h"
#include "include/keyboard.h"
#include "../system/ipc/include/mutex.h"
#include "../system/ipc/include/condVar.h"
#include "../system/scheduler/include/scheduler.h"

static void addKeyBuffer(int key);

static unsigned char kb_map[3][85] = {
	{
		DO_NOTHING, ESCAPE, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
		DO_NOTHING, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
		DO_NOTHING, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', SHIFTL,
		'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', SHIFTR, DO_NOTHING,
		DO_NOTHING, ' ', CAPS, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING,
		DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, '7', '8', '9',
		'-', '4', '5', '6', '+', '1', '2', '3', '0', DO_NOTHING, DO_NOTHING
	},//No Shift
	{
		DO_NOTHING, ESCAPE, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
		DO_NOTHING, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
		DO_NOTHING, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', SHIFTL,
		'\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', SHIFTR, DO_NOTHING,
		DO_NOTHING, ' ', CAPS, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING,
		DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, '7', '8', '9',
		'-', '4', '5', '6', '+', '1', '2', '3', '0', DO_NOTHING, DO_NOTHING
	},//Shift
	{
		DO_NOTHING, ESCAPE, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
		DO_NOTHING, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',
		DO_NOTHING, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '|', SHIFTL,
		'\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', SHIFTR, DO_NOTHING,
		DO_NOTHING, ' ', CAPS, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING,
		DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, DO_NOTHING, '7', '8', '9',
		'-', '4', '5', '6', '+', '1', '2', '3', '0', DO_NOTHING, DO_NOTHING
	}//Caps On
};

static char lineBuffer[KB_SIZE];
static int lineIndex;
static kbStatus	kb;
/*static mutex kb_mutex;
static cond_t cv;
static bool bufferEmpty = true;*/

bool keyboardInit() {
	/*kb_mutex = lockBuild();

	if(kb_mutex == NULL)
		return false;

	cv = cvInitialize();

	if(cv == NULL){
		lockDestroy(kb_mutex);
		print("siiiiiii",-1);
		return false;
	}*/

	for(int i = 0; i < KB_SIZE; ++i) {
		kb.buffer[i] = EMPTY;
		lineBuffer[i] = EMPTY;
	}

	kb.writeIndex = 0;
	kb.readIndex = 0;
	kb.capsOn = false;
	kb.shiftOn = false;
	lineIndex = 0;

	return true;
}

void keyboardHandler(unsigned char key) {
	addKeyBuffer(key);
	//signal(&sem)
}
/*void readFull(char * buffer, char aux){


    lock(kb_mutex);
    while (getForeground()->id != getCurrentPid() || bufferEmpty == true ){
        cvWait(cv, kb_mutex);
    }
    unlock(kb_mutex);

	process *p = getForeground();
	*buffer = getKey(aux, p);

}*/
char getKey(char write, process *p) {
	char c;

	//wait(sem)

	c = kb.buffer[kb.readIndex];
	
	kb.buffer[kb.readIndex++] = EMPTY; //Remove the readed char from the buffer

	if(kb.readIndex == KB_SIZE)
		kb.readIndex = 0;

	if(c != EMPTY && write == true)
		putChar(c, -1); //Print the char to the console

	return c;
}

static void addKeyBuffer(int key) {
	unsigned char
		keyboard = 0;

	if(key == CAPS)
		kb.capsOn = !kb.capsOn;
	else if(key == SHIFTR || key == SHIFTL)
		kb.shiftOn = true;
	else if(key == SHIFTR_RL || key == SHIFTL_RL)
		kb.shiftOn = false;
	else if(!((key >> 7) & 0x1)) {
		if(kb.capsOn)
			keyboard = 2;
		else if (kb.shiftOn)
			keyboard = 1;

		char
			value = kb_map[keyboard][key];

		if(value == '\b') {
			if(lineIndex > 0) {
				if(lineBuffer[lineIndex - 1] != EMPTY) {
					lineBuffer[--lineIndex] = EMPTY;
					kb.buffer[kb.writeIndex++] = '\b';
				}
			}
		}else if(value == '\n') {
			lineIndex = 0;
			for(int i=0; i<KB_SIZE; i++)
				lineBuffer[i] = EMPTY;

			kb.buffer[kb.writeIndex++] = value;
		}else if(value != DO_NOTHING && value != ESCAPE && lineIndex < KB_SIZE) {
			lineBuffer[lineIndex++] = value;
			kb.buffer[kb.writeIndex++] = value;
		}

		if(kb.writeIndex == KB_SIZE)
			kb.writeIndex = 0;

		/*bufferEmpty = false;
		cvBroadcast(cv);*/
	}
}