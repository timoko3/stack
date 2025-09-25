#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "stackSafety.h"

const int AMOUNT_OF_ADDITIONAL_PARAM = 2;
const int POISON_NUMBER = 0;

struct stack{
    stack_t* data;
    size_t size;
    size_t capacity;
};

enum stackErr{
    BAD_CAPACITY,
    NULL_POINTER,
    PROCESS_OK
};

stackErr stackCtor(stack* stk, size_t capacity);
stackErr stackPush(stack* stk, stack_t value);
stackErr stackPop(stack* stk, int* stackElem);
void stackDump(stack* stk);

stackErr stackCtor(stack* stk, size_t capacity){
    // assert(stk);
    // assert(capacity > 0);
    

    stk->capacity = capacity;
    stk->size = 0;
    stk->data = (stack_t*) calloc(stk->capacity, sizeof(stack_t));

    return PROCESS_OK;
}

stackErr stackPush(stack* stk, stack_t value){
    assert(stk);

    stk->data[stk->size] = value;
    (stk->size)++;
    
    return PROCESS_OK;
}

stackErr stackPop(stack* stk, int* stackElem){
    assert(stk);
    assert(stackElem);

    *stackElem = stk->data[stk->size - 1];
    stk->data[stk->size - 1] = POISON_NUMBER;
    (stk->size)--;

    return PROCESS_OK;
}

void stackDump(stack* stk){
    printf("%s()\n", __FUNCTION__);
    printf("stack<int>[%p]\n", stk);
    printf("{\n");
    printf("\tsize = %lu\n", stk->size);
    printf("\tcapacity = %lu\n", stk->capacity);
    printf("\tdata[%p]\n", stk->data);
    printf("\t{\n");
    for(size_t curElemInd = 0; curElemInd < stk->capacity; curElemInd++){
        if(stk->data[curElemInd] != POISON_NUMBER){
            printf("\t\t*[%lu] = %d\n", curElemInd, stk->data[curElemInd]);
        }
        else {
            printf("\t\t[%lu] = %d (POISON NUMBER)\n", curElemInd, stk->data[curElemInd]);
        }
    }
    printf("\t}\n");
    printf("}\n");
    
}

#endif /* STACK_H */