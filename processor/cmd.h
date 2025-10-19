#ifndef PROCESSOR_CMD_H
#define PROCESSOR_CMD_H

#include "cmdOpcodes.h"
// #include "cmd_spu.h"
// #include "cmd_math.h"

typedef int cmdParam_t;

typedef bool (*commandGroupHandler_t)(processor* spu, command cmd);
typedef bool (*mathHandler_t)(processor* spu);

struct command{
    cmdOpcodes            code; 
    commandGroupHandler_t groupHandler;
    mathHandler_t     
    int                   nArgs; 
};

// pop
// push
// setpc
// hlt

// pop
// pop

// push

// pop
// pop

// setPc

static bool calcCommand(processor* spu, command cmd){
    assert(spu);

    cmdParam_t arg1 = 0, arg2 = 0;

    spuPop(spu, &arg1);
    spuPop(spu, &arg2);

    cmdParam_t result = 0;
    
    

    spuPush(spu, result);

    return check;
}

// static bool spuCommand(processor* spu, command cmd){
//     assert(spu);

//     bool check = cmd.handler.spuHandler(spu);

//     return check;
// } 

// static bool jmpCond(processor* spu, command cmd){
//     assert(spu);

//     stackData_t superiorStackElem = 0;
//     stackData_t preSuperiorStackElem = 0;

//     stackPop(&spu->stk, &superiorStackElem);
//     stackPop(&spu->stk, &preSuperiorStackElem);  
    
//     cmdParam_t result = 0;
//     cmd.handler.calcBinaryHandler(superiorStackElem, preSuperiorStackElem, &result);

//     if(result){
//         jmp(spu);
//     }
//     else{
//         (spu->pc)++;
//     }

//     return true;
// }

bool add(processor* spu){
    cmdParam_t arg1 = 0, arg2 = 0;
    
    spuPop(spu, &arg1);
    spuPop(spu, &arg2);

    spuPush(spu, arg1 + arg2);

    return true;
}

bool sub(processor* spu){
    cmdParam_t arg1 = 0, arg2 = 0;
    
    spuPop(spu, &arg1);
    spuPop(spu, &arg2);

    spuPush(spu, arg1 - arg2);

    return true;
}

bool mul(processor* spu){
    cmdParam_t arg1 = 0, arg2 = 0;
    
    spuPop(spu, &arg1);
    spuPop(spu, &arg2);

    spuPush(spu, arg1 * arg2);

    return true;
}

bool div(processor* spu){
    cmdParam_t arg1 = 0, arg2 = 0;
    
    spuPop(spu, &arg1);
    spuPop(spu, &arg2);

    spuPush(spu, arg1 / arg2);

    return true;
}

bool sqrt(processor* spu){
    cmdParam_t arg1 = 0, arg2 = 0;
    
    spuPop(spu, &arg1);
    spuPop(spu, &arg2);

    spuPush(spu, arg1 * arg2);

    return true;
}

bool jb(processor* spu){
    cmdParam_t arg1 = 0, arg2 = 0;
    
    spuPop(spu, &arg1);
    spuPop(spu, &arg2);

    cmdParam_t result = (arg1 < arg2);

    if(result){
        int pos = 0;
        spuGetArg(spu, &pos);
        spuJump(spu, pos);
    }

    return true;
}

const command commandsHandler[]{ 
    {ADD,  add,   0}, 
    {SUB,  sub,   0},
    {MUL,  mul,   0},

    {JB,  jb,    1},
    // {PUSH,    {.spuHandler = push},        PROCESSOR, NO_CMD_PARAM},
    // {PUSHREG, {.spuHandler = pushreg},     PROCESSOR, NO_CMD_PARAM},
    // {POPREG,  {.spuHandler = popreg},      PROCESSOR, NO_CMD_PARAM},
    // {OUT,     {.spuHandler = out},         PROCESSOR, NO_CMD_PARAM},
    // {HLT,     {.spuHandler = hlt},         PROCESSOR, NO_CMD_PARAM},
    // {JMP,     {.spuHandler = jmp},         PROCESSOR, NO_CMD_PARAM},
    // {CALL,    {.spuHandler = callFunc},    PROCESSOR, NO_CMD_PARAM},
    // {RET,     {.spuHandler = returnFunc},  PROCESSOR, NO_CMD_PARAM},

    // {JBE,     {.calcBinaryHandler = lt},   JUMP, NO_CMD_PARAM},
    // {JA,      {.calcBinaryHandler = le},   JUMP, NO_CMD_PARAM},
    // {JAE,     {.calcBinaryHandler = gt},   JUMP, NO_CMD_PARAM},
    // {JE,      {.calcBinaryHandler = ge},   JUMP, NO_CMD_PARAM},
    // {JNE,     {.calcBinaryHandler = eq},   JUMP, NO_CMD_PARAM},
    // {JB,      {.calcBinaryHandler = ne},   JUMP, NO_CMD_PARAM}, 

    // {ADD,     {.calcBinaryHandler = add},  CALC, BINARY},
    // {SUB,     {.calcBinaryHandler = sub},  CALC, BINARY},
    // {MUL,     {.calcBinaryHandler = mul},  CALC, BINARY},
    // {DIV,     {.calcBinaryHandler = div},  CALC, BINARY},
    // {SQRT,    {.calcUnaryHandler  = sqrt}, CALC, UNARY}
}; 

#endif /* PROCESSOR_CMD_H */