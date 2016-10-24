#include "include/keyboard.h"
#include "include/video.h"

static void addKeyBuffer(int key);

unsigned char kb_map[3][85] = {
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

char
	lineBuffer[KB_SIZE];
int
	lineIndex;
kbStatus
	kb;

void keyboardInit() {
	for(int i = 0; i < KB_SIZE; ++i) {
		kb.buffer[i] = EMPTY;
		lineBuffer[i] = EMPTY;
	}

	kb.writeIndex = 0;
	kb.readIndex = 0;
	kb.capsOn = FALSE;
	kb.shiftOn = FALSE;
	lineIndex = 0;
}

void keyboardHandler(unsigned char key) {
	addKeyBuffer(key);
}

char getKey(char write) {
	char
		c = kb.buffer[kb.readIndex];
	
	kb.buffer[kb.readIndex++] = EMPTY; //Remove the readed char from the buffer

	if(kb.readIndex == KB_SIZE)
		kb.readIndex = 0;

	if(c != EMPTY && write == TRUE)
		putChar(c, -1); //Print the char to the console

	return c;
}

static void addKeyBuffer(int key) {
	unsigned char
		keyboard = 0;

	if(key == CAPS)
		kb.capsOn = !kb.capsOn;
	else if(key == SHIFTR || key == SHIFTL)
		kb.shiftOn = TRUE;
	else if(key == SHIFTR_RL || key == SHIFTL_RL)
		kb.shiftOn = FALSE;
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
	}
}