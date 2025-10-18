#ifndef PROCESSOR_CMD_H
#define PROCESSOR_CMD_H

#include "cmdOpcodes.h"
#include "cmd_spu.h"

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
    // math
    // jmp
    // spu specifical
};

const command commandsHandler[]{ 
    {PUSH, {.spuHandler = push}, PROCESSOR, NO_CMD_PARAM},

    {OUT,  {.spuHandler = out}, PROCESSOR, NO_CMD_PARAM}
    // {PUSHREG, {.spuHandler = pushreg}, PROCESSOR, NO_CMD_PARAM},
    // {POPREG,  {.spuHandler = popreg}, PROCESSOR, NO_CMD_PARAM},
    // {HLT,     hlt},


    // {JMP,     jmpCond},
    // {JB,      jmpCond}, // jmpJB
    // {JBE,     jmpCond},
    // {JA,      jmpCond},
    // {JAE,     jmpCond},
    // {JE,      jmpCond},
    // {JNE,     jmpCond},

    // math

    // {RET,     returnFunc},
    // {CALL,    callFunc},

}; 

#endif /* PROCESSOR_CMD_H */