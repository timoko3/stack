#ifndef STACK_H
#define STACK_H

///
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <limits.h>
#include <stdlib.h>

#include "genConsoleColors/consoleColors.h"

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 3
#endif /* DEBUG_LEVEL */

#if DEBUG_LEVEL == 0
#define NDEBUG
#endif

typedef int stack_t;

const int POISON_NUMBER = INT_MAX / 10;
const size_t STACK_MAX_CAPACITY = 1e9;

#if DEBUG_LEVEL > 1
const size_t CANARY_PROTECTION_SIZE    = 1;  
const stack_t CANARY_PROTECTION_NUMBER = INT_MAX / 20;
#endif /* DEBUG */

#if DEBUG_LEVEL > 0

#define verify if(verifyStack(stk, __FUNCTION__, __FILE__, __LINE__)!=PROCESS_OK)return stk->error.type;
#define dump(stk) stackDump(stk, function, file, line);

#endif /* DEBUG */

enum stackErr{
    CAPACITY_EXCEEDS_LIMIT,
    NULL_POINTER,
    CAPACITY_IS_ZERO,
    BAD_MEMORY_ALLOCATION,
    SIZE_EXCEEDS_CAPACITY,
    CANARY_TORTURE,
    EMPTY_STACK,
    UNPLANNED_STACK_CHANGE,
    PROCESS_OK
};

struct errorDescription{
    stackErr type;
    const char* text;
};

const int AMOUNT_ERROR_TYPES = 9;

// #pragma pack(push, 1)
struct stack{
    stack_t* data;
    size_t size;
    size_t capacity;
    errorDescription error;
    #if DEBUG_LEVEL > 1
    bool canaryStatus = false;
    #endif
    #if DEBUG_LEVEL > 2
    unsigned long hash;
    #endif
};
// #pragma pack(pop)

stackErr stackCtor(stack* stk, size_t capacity);
stackErr stackPush(stack* stk, stack_t value);
stackErr stackPop(stack* stk, stack_t* stackElem);
stackErr stackDtor(stack* stk);

#endif /* STACK_H */