#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <limits.h>

#include "consoleColors.h"

#define DEBUG

const int POISON_NUMBER = 0;
const int AMOUNT_ERROR_TYPES = 6;

#ifdef DEBUG
const stack_t CANARY_PROTECTION_SIZE = 2;
#endif /* DEBUG */

/// для использования в функциях этого файла
#define verify if(verifyStack(stk, __FUNCTION__, __FILE__, __LINE__)!=PROCESS_OK)return stk->error.type;
#define dump(stk) stackDump(stk, function, file, line);

enum stackErr{
    BAD_CAPACITY,
    NULL_POINTER,
    BAD_SIZE,
    BAD_MEMORY_ALLOCATION,
    SIZE_EXCEEDS_CAPACITY,
    PROCESS_OK
};

struct errorDescription{
    stackErr type;
    const char* text;
};

struct stack{
    stack_t* data;
    size_t size;
    size_t capacity;
    errorDescription error;
};

struct errorDescription errors[AMOUNT_ERROR_TYPES]{
    {BAD_CAPACITY, "Вместимость data недопустима\n"},
    {NULL_POINTER, "Указатели не должны быть нулевыми\n"},
    {BAD_SIZE, "Размер недопустим\n"},
    {BAD_MEMORY_ALLOCATION, "Некорректное выделение памяти\n"},
    {SIZE_EXCEEDS_CAPACITY, "Превышения размером вместимоcти недопустимо\n"},
    {PROCESS_OK, "Все хорошо\n"}
};


stackErr stackCtor(stack* stk, size_t capacity);
stackErr stackPush(stack* stk, stack_t value);
stackErr stackPop(stack* stk, stack_t* stackElem);

#ifdef DEBUG
void stackDump(stack* stk, const char* function, const char* file, const int line);
stackErr verifyStack(stack* stk, const char* function, const char* file, const int line);
static void assignErrorStruct(stack* stk, stackErr type);
#endif /* DEBUG */

stackErr stackCtor(stack* stk, size_t capacity){
    assert(stk);
    assert(capacity > 0);
    assert(capacity < (size_t) 1e9);

    #ifdef DEBUG
        stk->capacity = capacity + CANARY_PROTECTION_SIZE;
    #else 
        stk->capacity = capacity;
    #endif /* DEBUG */

    stk->size = 0;
    stk->data = (stack_t*) calloc(stk->capacity, sizeof(stack_t));

    #ifdef DEBUG
    verify
    #endif /* DEBUG */

    return PROCESS_OK;
}

stackErr stackPush(stack* stk, stack_t value){

    #ifdef DEBUG
    verify
    #endif /* DEBUG */

    #ifdef DEBUG
    if(stk->size == stk->capacity - CANARY_PROTECTION_SIZE){
    #else   
    if(stk->size == stk->capacity){
    #endif
        #ifdef DEBUG
        verify
        #endif /* DEBUG */

        stk->capacity = (stk->capacity) * 2;
        stack_t* temp = (stack_t*) realloc(stk->data, (stk->capacity) * sizeof(stack_t));
        assert(temp);

        stk->data = temp;
    }

    #ifdef DEBUG
    verify
    #endif /* DEBUG */

    #ifdef DEBUG
    stk->data[stk->size + (CANARY_PROTECTION_SIZE / 2)] = value;
    #else
    stk->data[stk->size] = value;
    #endif

    (stk->size)++;

    #ifdef DEBUG
    verify
    #endif /* DEBUG */

    return PROCESS_OK;
}

stackErr stackPop(stack* stk, stack_t* stackElem){
    assert(stackElem);

    #ifdef DEBUG
    verify
    #endif /* DEBUG */

    #ifdef DEBUG
    *stackElem = stk->data[stk->size];
    stk->data[stk->size] = POISON_NUMBER;
    #else
    *stackElem = stk->data[stk->size - 1];
    stk->data[stk->size - 1] = POISON_NUMBER;
    #endif

    (stk->size)--;

    #ifdef DEBUG
    verify
    #endif /* DEBUG */
    
    return PROCESS_OK;
}

#ifdef DEBUG
void stackDump(stack* stk, const char* function, const char* file, const int line){
    if(stk == NULL){
        printf("Передача нулевого указателя недопустима\n");                                                                                                                                            printf ("MEOW \a\a\a\a\a\a");
        return;
    }
    
    if(stk->error.type == PROCESS_OK){
        printf(SET_STYLE_FONT_GREEN"OK\n"RESET);
    }
    else{
        printf(SET_STYLE_FONT_RED"ERROR: %s\n"RESET, stk->error.text);
    }

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
        assignErrorStruct(stk, NULL_POINTER);
        printf("stk — нулевой указатель\n");  
    } 
    else{
        if(stk->data == NULL){  
            assignErrorStruct(stk, NULL_POINTER);
            printf("data — нулевой указатель\n");
        }

        else if((stk == 0) || (stk->capacity > (size_t) 1e+9)){
            assignErrorStruct(stk, BAD_CAPACITY);
        }

        else if(stk->size > stk->capacity){
            assignErrorStruct(stk, SIZE_EXCEEDS_CAPACITY);
        }

        else if(malloc_usable_size(stk->data) != (sizeof(stack_t) * stk->capacity)){
            assignErrorStruct(stk, BAD_MEMORY_ALLOCATION);
        }
        else{
            assignErrorStruct(stk, PROCESS_OK);
        }
              
    }

    dump(stk);
    return stk->error.type;
}


static void assignErrorStruct(stack* stk, stackErr type){
    assert(stk);

    for(size_t curErrInd = 0; curErrInd < AMOUNT_ERROR_TYPES; curErrInd++){
        if(errors[curErrInd].type == type){
            stk->error = errors[curErrInd];
        }
    }
}

#endif /* DEBUG */

#endif /* STACK_H */