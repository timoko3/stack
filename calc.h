#ifndef CALC_H
#define CALC_H

#include <stdio.h>

#include "general/file.h"
#include "stack.h"
#include "translator/translator.h"

const int N_CALC_COMMANDS     = 5; //
const int N_REGISTER_COMMANDS = 2; //

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
    int* byteCode;
    size_t pc; // pointer to command
};

typedef bool (*registerCommand)(processor*);

bool pushreg(processor* spu);
bool popreg(processor* spu);

struct registerCommandsDescription{
    spu_commands_codes code;
    registerCommand function;
};

void spu(stack* stk);
bool completeCommand(processor* spu);

#endif /* CALC_H */