#ifndef PROCESSOR_CMD_H
#define PROCESSOR_CMD_H

#include "cmdOpcodes.h"
#include "cmd_spu.h"
#include "cmd_math.h"

typedef int cmdParam_t;

enum commandType{
    CALC,
    PROCESSOR
};

enum commandParamType{
    NO_CMD_PARAM,
    UNARY,
    BINARY
};

typedef bool (*unaryCommandPtr)  (cmdParam_t param, cmdParam_t* result);
typedef bool (*binaryCommandPtr) (cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
typedef bool (*spuNoParamPtr)    (processor* spu);

union handlers{
    spuNoParamPtr    spuHandler;
    unaryCommandPtr  calcUnaryHandler;
    binaryCommandPtr calcBinaryHandler;
};

struct command{
    cmdOpcodes       code; 
    handlers         handler;
    commandType      type;
    commandParamType param;  
};

const command commandsHandler[]{ 
    {PUSH,    {.spuHandler = push},       PROCESSOR, NO_CMD_PARAM},
    {PUSHREG, {.spuHandler = pushreg},    PROCESSOR, NO_CMD_PARAM},
    {POPREG,  {.spuHandler = popreg},     PROCESSOR, NO_CMD_PARAM},
    {OUT,     {.spuHandler = out},        PROCESSOR, NO_CMD_PARAM},
    {HLT,     {.spuHandler = hlt},        PROCESSOR, NO_CMD_PARAM},
    {JMP,     {.spuHandler = jmp},        PROCESSOR, NO_CMD_PARAM},

    {JB,      {.spuHandler =jmpCond},     PROCESSOR, NO_CMD_PARAM}, 
    {JBE,     {.spuHandler =jmpCond},     PROCESSOR, NO_CMD_PARAM},
    {JA,      {.spuHandler =jmpCond},     PROCESSOR, NO_CMD_PARAM},
    {JAE,     {.spuHandler =jmpCond},     PROCESSOR, NO_CMD_PARAM},
    {JE,      {.spuHandler =jmpCond},     PROCESSOR, NO_CMD_PARAM},
    {JNE,     {.spuHandler =jmpCond},     PROCESSOR, NO_CMD_PARAM},

    {CALL,    {.spuHandler = callFunc},   PROCESSOR, NO_CMD_PARAM},
    {RET,     {.spuHandler = returnFunc}, PROCESSOR, NO_CMD_PARAM},

    {ADD,  {.calcBinaryHandler = add},  CALC, BINARY},
    {SUB,  {.calcBinaryHandler = sub},  CALC, BINARY},
    {MUL,  {.calcBinaryHandler = mul},  CALC, BINARY},
    {DIV,  {.calcBinaryHandler = div},  CALC, BINARY},
    {SQRT, {.calcUnaryHandler  = sqrt}, CALC, UNARY}

}; 

#endif /* PROCESSOR_CMD_H */