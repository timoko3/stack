#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "general/file.h"
#include "general/strFunc.h"
#include "cmdOpcodes.h"

#define DEBUG_TRANSLATOR 0

const size_t COMMAND_NAME_MAX_SIZE  = 10;
const size_t REGISTER_NAME_MAX_SIZE = 5;
const int A_ASCII_CODE              = (int) 'A';
const int ASSEMBLE_FAILURE          = -1;

const size_t N_LABELS  = 10;
const int LABEL_POISON = 46878756;

struct label_t{
    const char* name;
    unsigned long hash;
    int code;
};

struct command_t{
    cmdOpcodes code;
    const char* name;
    unsigned long hash;
};

struct translator_t{
    DataFromInputFIle spuCommandsText;
    command_t* cmds;
    label_t*   ptrLabels;
    buffer_t*    opcode;
};

void setSpuCommandsHash();
translator_t translatorCtor();
bool loadTextCommands(translator_t* translator, const char* fileName);
bool assemble(translator_t* translator);
bool fillByteCodeBuffer(DataFromInputFIle* spuTextCommands, size_t* curByteBufferSize, int* byteCodeBuffer);

#if DEBUG_TRANSLATOR
void printByteCodeBuffer(int* buffer, size_t curByteBufferSize);
#endif /* DEBUG */

#endif /* TRANSLATOR_H */
