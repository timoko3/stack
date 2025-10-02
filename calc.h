#ifndef CALC_H
#define CALC_H

#include <stdio.h>

#include "workWIthFiles.h"
#include "stack.h"

const int nCommands = 5;

typedef bool (*command)(stack*);

bool add(stack* stk);
bool sub(stack* stk);
bool mul(stack* stk);
bool div(stack* stk);
bool out(stack* stk);

struct commandDescription{
    const char* name;
    command function;
};

void calculator(stack* stk);
static bool completeCommand(char* curCommand, stack* stk, string* comandBuffer);

#endif /* CALC_H */