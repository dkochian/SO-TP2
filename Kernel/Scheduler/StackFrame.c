//BASED ON Wyrm/Process stackframe
#include "StackFrame.h"

StackFrame * newStackFrame() {
    StackFrame * stackFrame = malloc(sizeof(StackFrame));
    return stackFrame;
}

void removeStackFrame(StackFrame * stackFrame) {
    free(stackFrame);
}