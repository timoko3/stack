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

command_t commands[] = {
    {PUSH,    "PUSH",    0},
    {POP,     "POP",     0},
    {PUSHREG, "PUSHREG", 0},
    {POPREG,  "POPREG",  0},
    {ADD,     "ADD",     0},
    {SUB,     "SUB",     0},
    {MUL,     "MUL",     0},
    {DIV,     "DIV",     0},
    {SQRT,    "SQRT",    0},
    {OUT,     "OUT",     0},
    {HLT,     "HLT",     0},
    {JMP,     "JMP",     0},
    {JB,      "JB",      0},
    {JBE,     "JBE",     0},
    {JA,      "JA",      0},
    {JAE,     "JAE",     0},
    {JE,      "JE",      0},
    {JNE,     "JNE",     0},
    {CALL,    "CALL",    0},
    {RET,     "RET",     0}
};

label_t nameLabels[N_LABELS] = {};

void setSpuCommandsHash();
translator_t translatorCtor();
int* createByteCodeBuffer(DataFromInputFIle* spuTextCommands, size_t* curByteBufferSize);
bool fillByteCodeBuffer(DataFromInputFIle* spuTextCommands, size_t* curByteBufferSize, int* byteCodeBuffer);

#if DEBUG_TRANSLATOR
void printByteCodeBuffer(int* buffer, size_t curByteBufferSize);
#endif /* DEBUG */

#endif /* TRANSLATOR_H */
