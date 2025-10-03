#ifndef CALC_H
#define CALC_H

#include <stdio.h>

#include "workWIthFiles.h"
#include "stack.h"
#include "translator/translator.h"

const int N_CALC_COMMANDS    = 5;
const int N_REGISTER_COMMANDS = 2;

typedef bool (*stackCommand)(stack*);


bool add(stack* stk);
bool sub(stack* stk);
bool mul(stack* stk);
bool div(stack* stk);
bool out(stack* stk);

struct calcCommandsDescription{
    spu_commands_codes code;
    stackCommand function;
};

struct processor{
    stack stk;
    int regs[10] = {0};
    int* biteCode;
    size_t pc;
};

typedef bool (*registerCommand)(processor*);

struct registerCommandsDescription{
    spu_commands_codes code;
    registerCommand function;
};

bool pushreg(processor* spu);
bool popreg(processor* spu);

void calculator(stack* stk);
bool completeCommand(processor* spu);

#endif /* CALC_H */