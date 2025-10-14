#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "general/file.h"
#include "general/strFunc.h"

#define DEBUG_TRANSLATOR 0

const size_t COMMAND_NAME_MAX_SIZE  = 10;
const size_t REGISTER_NAME_MAX_SIZE = 5;
const size_t PREAMBLE_SIZE          = 2;
const int A_ASCII_CODE              = (int) 'A';
const int ASSEMBLE_FAILURE          = -1;

const int SIGNATURE    = 999;
const int VERSION      = 1;
const int LABEL_POISON = 46878756;
const size_t N_LABELS  = 10;

enum spuCommandsCodes{
    PUSH = 1,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    SQRT,
    OUT,
    HLT,
    JMP,
    JB,
    JBE,
    JA,
    JAE,
    JE,
    JNE,
    PUSHREG = 33,
    POPREG  = 34
};

struct spu_command{
    const char* name;
    unsigned long hash;
    spuCommandsCodes code;
};

void setSpuCommandsHash();
int* createByteCodeBuffer(DataFromInputFIle* spuCommandsNames, size_t* curByteBufferSize);
bool fillByteCodeBuffer(DataFromInputFIle* spuCommandsNames, size_t* curByteBufferSize, int* byteCodeBuffer, int* labels);

#if DEBUG_TRANSLATOR
void printByteCodeBuffer(int* buffer, size_t curByteBufferSize);
#endif /* DEBUG */

#endif /* TRANSLATOR_H */
