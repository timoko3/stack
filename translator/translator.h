#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "general/file.h"
#include "general/strFunc.h"

#include <stdio.h>

const char* const TEXT_COMMANDS_FILE_NAME = "translator/calcCommands.txt";   
const char* const BYTE_CODE_FILE_NAME     = "byteCode.asm";

const int ASSEMBLE_FAILURE = -1;

enum spu_commands_codes{
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
    spu_commands_codes code;
    unsigned long hash;
};

struct byteCodeBuffer{
    size_t size;
    char* pointer;
};

void setSpuCommandsHash();
int* createByteCodeBuffer(DataFromInputFIle* calcCommands, size_t* curByteBufferSize);

#endif /* TRANSLATOR_H */
