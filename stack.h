#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <assert.h>
#include <malloc.h>

#include "consoleColors.h"

const int POISON_NUMBER = 0;
const int AMOUNT_ERROR_TYPES = 6;

/// для использования в функциях этого файла
#define verify if(verifyStack(stk, __FUNCTION__, __FILE__, __LINE__)!=PROCESS_OK)return stk->error;
#define dump(stk) stackDump(stk, function, file, line);

enum stackErr{
    BAD_CAPACITY,
    NULL_POINTER,
    BAD_SIZE,
    BAD_MEMORY_ALLOCATION,
    SIZE_EXCEEDS_CAPACITY,
    PROCESS_OK
};

// struct errorDescription{
//     stackErr type;
//     const char* text;
// };

struct stack{
    stack_t* data;
    size_t size;
    size_t capacity;
    stackErr error = PROCESS_OK;
};

// struct errorDescription errors[AMOUNT_ERROR_TYPES]{
//     {BAD_CAPACITY, "Вместимость data недопустима\n"},
//     {NULL_POINTER, "Указатели не должны быть нулевыми\n"},
//     {BAD_SIZE, "Размер недопустим\n"},
//     {BAD_MEMORY_ALLOCATION, "Некорректное выделение памяти\n"},
//     {SIZE_EXCEEDS_CAPACITY, "Превышения размером вместимоти недопустимо\n"},
//     {PROCESS_OK, "Все хорошо\n"}
// };



stackErr stackCtor(stack* stk, size_t capacity);
stackErr stackPush(stack* stk, stack_t value);
stackErr stackPop(stack* stk, stack_t* stackElem);
void stackDump(stack* stk, const char* function, const char* file, const int line);
stackErr verifyStack(stack* stk, const char* function, const char* file, const int line);

stackErr stackCtor(stack* stk, size_t capacity){
    assert(stk);

    stk->capacity = capacity;

    stk->size = 18;
    stk->data = (stack_t*) calloc(stk->capacity, sizeof(stack_t));

    verify
    

    return PROCESS_OK;
}

stackErr stackPush(stack* stk, stack_t value){
    verify

    stk->data[stk->size] = value;
    (stk->size)++;
    
    verify

    return PROCESS_OK;
}

stackErr stackPop(stack* stk, stack_t* stackElem){
    assert(stackElem);

    verify

    *stackElem = stk->data[stk->size - 1];
    stk->data[stk->size - 1] = POISON_NUMBER;
    (stk->size)--;

    verify

    return PROCESS_OK;
}

void stackDump(stack* stk, const char* function, const char* file, const int line){
    if(stk == NULL){
        printf("Передача нулевого указателя недопустима\n");                                                                                                                                            printf ("MEOW \a\a\a\a\a\a");
        return;
    }
    
    printf(SET_STYLE_FONT_RED"ERROR TYPE: %d\n"RESET, stk->error);
    printf("%s()\n", __FUNCTION__);
    printf("stack<int>[%p]\n", stk);

    printf("Called from function %s at file %s:line %d\n", function, file, line);

    printf("{\n");
    printf("\tsize = %lu\n", stk->size);
    printf("\tcapacity = %lu\n\n", stk->capacity);
    printf("\tНеобходимый размер блока data: %lu байт(-ов)\n", sizeof(stack_t) * stk->capacity);
    printf("\tРазмер выделенного блока data: %lu байт(-ов)\n", malloc_usable_size(stk->data));
    printf("\tdata[%p]\n", stk->data);
    printf("\t{\n");
    for(size_t curElemInd = 0; curElemInd < stk->capacity; curElemInd++){
        if(stk->data[curElemInd] != POISON_NUMBER){
            printf("\t\t*[%lu] = %d\n", curElemInd, stk->data[curElemInd]);
        }
        else {
            printf("\t\t[%lu] = %d" SET_STYLE_FONT_RED" (POISON NUMBER)\n"RESET, curElemInd, stk->data[curElemInd]);
        }
    }
    printf("\t}\n");
    printf("}\n");
    
}

stackErr verifyStack(stack* stk, const char* function, const char* file, const int line){

    if(stk == NULL){
        stk->error = NULL_POINTER;
        printf("stk — нулевой указатель\n");  
    } 

    if(stk->data == NULL){  
        stk->error = NULL_POINTER;
        printf("data — нулевой указатель\n");
    }

    if((stk == 0) || (stk->capacity > (size_t) 1e+9)){
        stk->error = BAD_CAPACITY;
        dump(stk)
    }

    if(stk->size > stk->capacity){
        stk->error = SIZE_EXCEEDS_CAPACITY;
        dump(stk);
    }

    if(malloc_usable_size(stk->data) != (sizeof(stack_t) * stk->capacity)){
        stk->error = BAD_MEMORY_ALLOCATION;
        dump(stk);
    }      
    
    return stk->error;
}




#endif /* STACK_H */