#ifndef CALC_H
#define CALC_H

#include <stdio.h>

#include "workWIthFiles.h"
#include "stack.h"
#include "translator/translator.h"

const int nCommands = 5;

typedef bool (*command)(stack*);

bool add(stack* stk);
bool sub(stack* stk);
bool mul(stack* stk);
bool div(stack* stk);
bool out(stack* stk);

struct commandDescription{
    spu_commands_codes code;
    command function;
};

void calculator(stack* stk);
bool completeCommand(int* biteCode, size_t* curBiteCodeArrInd, stack* stk);

#endif /* CALC_H */