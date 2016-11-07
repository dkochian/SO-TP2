//BASED ON Wyrm/Process stackframe
#include "stackFrame.h"

StackFrame * newStackFrame() {
    StackFrame * stackFrame = k_malloc(sizeof(StackFrame));
    return stackFrame;
}

void removeStackFrame(StackFrame * stackFrame) {
    k_free(stackFrame);
}