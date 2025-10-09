#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "general/file.h"
#include "general/strFunc.h"

#define DEBUG_TRANSLATOR 0

const size_t COMMAND_NAME_MAX_SIZE  = 10;
const size_t REGISTER_NAME_MAX_SIZE = 5;
const int A_ASCII_CODE              = (int) 'A';

const int ASSEMBLE_FAILURE = -1;

enum spuCommandsCodes{
    PUSH = 1,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    OUT,
    JMP,
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
int* createByteCodeBuffer(DataFromInputFIle* spuCommandsNames, size_t* curByteBufferSize);

#if DEBUG_TRANSLATOR
void printByteCodeBuffer(int* buffer, size_t curByteBufferSize);
#endif /* DEBUG */

#endif /* TRANSLATOR_H */
