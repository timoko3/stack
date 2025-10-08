#ifndef CALC_H
#define CALC_H

#include <stdio.h>

#include "general/file.h"
#include "stack/stack.h"
#include "translator/translator.h"

const size_t N_CALC_COMMANDS     = 5; //
const size_t N_REGISTER_COMMANDS = 2; //
const size_t N_REGISTERS         = 10;

typedef bool (*stackCommand)(stack*);

bool add(stack* stk);
bool sub(stack* stk);
bool mul(stack* stk);
bool div(stack* stk);
bool out(stack* stk);

struct processorCommandsDescription{
    spu_commands_codes code;
    stackCommand function;
};

struct processor{
    stack stk;
    int regs[N_REGISTERS] = {};
    int* byteCode;
    size_t pc; // pointer to command
    size_t sizeByteCode;
};

typedef bool (*registerCommand)(processor*);

bool pushreg(processor* spu);
bool popreg(processor* spu);

struct registerCommandsDescription{
    spu_commands_codes code;
    registerCommand function;
};

struct stackCommandsDescription{
    spu_commands_codes code;
    stackCommand function;
};

enum processorErr{
    SPU_PROCESS_OK
};

processorErr processorCtor(processor* spu);
processorErr processorDtor(processor* spu);
bool completeCommand(processor* spu);

#endif /* CALC_H */