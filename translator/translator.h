#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "general/file.h"
#include "general/strFunc.h"

#include <stdio.h> //////

const char* const TEXT_COMMANDS_FILE_NAME = "spuCommands.txt";  // in main 
const char* const BYTE_CODE_FILE_NAME     = "byteCode.asm";

const int ASSEMBLE_FAILURE = -1;

enum spuCommandsCodes{
    PUSH = 1,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    OUT,
    PUSHREG = 33,
    POPREG
};

struct spu_command{
    const char* name;
    unsigned long hash; ////
    spuCommandsCodes code;
};

struct byteCodeBuffer{ /////
    size_t size; 
    char* pointer;
};

void setSpuCommandsHash();
int* createByteCodeBuffer(DataFromInputFIle* calcCommands, size_t* curByteBufferSize);

#endif /* TRANSLATOR_H */
