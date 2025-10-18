#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "general/file.h"
#include "general/strFunc.h"
#include "cmd.h"

#define DEBUG_TRANSLATOR 0

struct translator_t{ 
    command_t* cmds; 
    strings_t  input_buffer; 
    buffer_t   opcode;
    label_t*   labels;
    state_t    curState;
};

bool translatorCtor(translator_t* translator);
bool loadTextCommands(translator_t* translator, strings_t textCommands);
bool assemble(translator_t* translator);
bool translatorDtor(translator_t* translator);

#if DEBUG_TRANSLATOR
void printByteCodeBuffer(int* buffer, size_t curByteBufferSize);
#endif /* DEBUG */

#endif /* TRANSLATOR_H */
