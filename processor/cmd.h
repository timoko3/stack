#ifndef PROCESSOR_CMD_H
#define PROCESSOR_CMD_H

#include "cmdOpcodes.h"
#include "cmd_spu.h"
#include "cmd_math.h"

typedef int cmdParam_t;

enum commandType{
    CALC,
    JUMP,
    PROCESSOR
};

enum commandparam_t{
    NO_CMD_PARAM,
    UNARY,
    BINARY
};

typedef bool (*unaryCalcCommandPtr)  (cmdParam_t param, cmdParam_t* result);
typedef bool (*binaryCalcCommandPtr) (cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
typedef bool (*spuNoParamPtr)        (processor* spu);

union handlers{
    spuNoParamPtr        spuHandler; // control
    unaryCalcCommandPtr  calcUnaryHandler;
    binaryCalcCommandPtr calcBinaryHandler;
};

struct command{
    cmdOpcodes     code; 
    handlers       handler;
    commandType    type;
    commandparam_t param;  
};

const command commandsHandler[]{ 
    {PUSH,    {.spuHandler = push},        PROCESSOR, NO_CMD_PARAM},
    {PUSHREG, {.spuHandler = pushreg},     PROCESSOR, NO_CMD_PARAM},
    {POPREG,  {.spuHandler = popreg},      PROCESSOR, NO_CMD_PARAM},
    {OUT,     {.spuHandler = out},         PROCESSOR, NO_CMD_PARAM},
    {HLT,     {.spuHandler = hlt},         PROCESSOR, NO_CMD_PARAM},
    {JMP,     {.spuHandler = jmp},         PROCESSOR, NO_CMD_PARAM},
    {CALL,    {.spuHandler = callFunc},    PROCESSOR, NO_CMD_PARAM},
    {RET,     {.spuHandler = returnFunc},  PROCESSOR, NO_CMD_PARAM},

    {JBE,     {.calcBinaryHandler = lt},   JUMP, NO_CMD_PARAM},
    {JA,      {.calcBinaryHandler = le},   JUMP, NO_CMD_PARAM},
    {JAE,     {.calcBinaryHandler = gt},   JUMP, NO_CMD_PARAM},
    {JE,      {.calcBinaryHandler = ge},   JUMP, NO_CMD_PARAM},
    {JNE,     {.calcBinaryHandler = eq},   JUMP, NO_CMD_PARAM},
    {JB,      {.calcBinaryHandler = ne},   JUMP, NO_CMD_PARAM}, 

    {ADD,     {.calcBinaryHandler = add},  CALC, BINARY},
    {SUB,     {.calcBinaryHandler = sub},  CALC, BINARY},
    {MUL,     {.calcBinaryHandler = mul},  CALC, BINARY},
    {DIV,     {.calcBinaryHandler = div},  CALC, BINARY},
    {SQRT,    {.calcUnaryHandler  = sqrt}, CALC, UNARY}
}; 

#endif /* PROCESSOR_CMD_H */