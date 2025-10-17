#ifndef PROCESSOR_CMD_H
#define PROCESSOR_CMD_H

#include "cmdOpcodes.h"

typedef int cmdParam_t;

enum commandType{
    // NO_PARAM,
    UNARY,
    BINARY,
    NUMBER
};
typedef bool (*noParamCommandPtr)(cmdParam_t* result);
typedef bool (*unaryCommandPtr)  (cmdParam_t param, cmdParam_t* result);
typedef bool (*binaryCommandPtr) (cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);

union handlers{
    unaryCommandPtr   unaryHandler;
    binaryCommandPtr  binaryHandler;
    noParamCommandPtr noParamHandler;
};

struct command{
    handlers         handler;
    cmdOpcodes       code; 
    commandType      type;
};

bool add(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool sub(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool mul(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool div(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool sqrt(cmdParam_t param,  cmdParam_t* result);

const command commandsHandler[]{ 
    {.handler = {.binaryHandler = add}, ADD, BINARY},
    {.handler = {.binaryHandler = sub}, SUB, BINARY},
    {.handler = {.binaryHandler = mul}, MUL, BINARY},
    {.handler = {.binaryHandler = div}, DIV, BINARY},
    {.handler = {.unaryHandler = sqrt}, SQRT, UNARY},
    
    // {OUT,     out},
    // {HLT,     hlt},
    // {RET,     returnFunc},
    // {CALL,    callFunc},

    // {PUSH,    push},

    // {JMP,     jmpCond},
    // {JB,      jmpCond}, // jmpJB
    // {JBE,     jmpCond},
    // {JA,      jmpCond},
    // {JAE,     jmpCond},
    // {JE,      jmpCond},
    // {JNE,     jmpCond},

    // {PUSHREG, pushreg},
    // {POPREG,  popreg},



}; 

#endif /* PROCESSOR_CMD_H */