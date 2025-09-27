#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <limits.h>

#include "consoleColors.h"

#define DEBUG_LEVEL 0 

#if DEBUG_LEVEL == 0
#define NDEBUG
#endif

const int POISON_NUMBER      = INT_MAX / 10;
const int AMOUNT_ERROR_TYPES = 8;

#if DEBUG_LEVEL > 1
const size_t CANARY_PROTECTION_SIZE    = 2;
const stack_t CANARY_PROTECTION_NUMBER = INT_MAX / 20;
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
    CANARY_TORTURE,
    POP_WITH_BAD_SIZE,
    PROCESS_OK
};

struct errorDescription{
    stackErr type;
    const char* text;
};

struct errorDescription errors[AMOUNT_ERROR_TYPES]{
    {BAD_CAPACITY, "Вместимость data недопустима\n"},
    {NULL_POINTER, "Указатели не должны быть нулевыми\n"},
    {BAD_SIZE, "Размер недопустим\n"},
    {BAD_MEMORY_ALLOCATION, "Некорректное выделение памяти\n"},
    {SIZE_EXCEEDS_CAPACITY, "Превышения размером вместимоcти недопустимо\n"},
    {CANARY_TORTURE, "Канарейка была замучена до смерти\n"},
    {POP_WITH_BAD_SIZE, "Попытка достать элемент из недопустимой области\n"},
    {PROCESS_OK, "Все хорошо\n"}
};

struct stack{
    stack_t* data;
    size_t size;
    size_t capacity;
    errorDescription error;
};


stackErr stackCtor(stack* stk, size_t capacity);
stackErr stackPush(stack* stk, stack_t value);
stackErr stackPop(stack* stk, stack_t* stackElem);
static void InitializeStackBuffer(stack* stk, size_t startStackInd);

#if DEBUG_LEVEL > 0
static void stackDump(stack* stk, const char* function, const char* file, const int line);
static stackErr verifyStack(stack* stk, const char* function, const char* file, const int line);
static void assignErrorStruct(stack* stk, stackErr type);
#endif /* DEBUG */

#if DEBUG_LEVEL > 1
static void setCanaryProtection(stack* stk);
static stackErr canaryCheck(stack* stk);
#endif /* DEBUG */

stackErr stackCtor(stack* stk, size_t capacity){
    assert(stk);
    assert(capacity > 0);
    assert(capacity < (size_t) 1e9);

    #if DEBUG_LEVEL > 1
        stk->capacity = capacity + CANARY_PROTECTION_SIZE;
    #else 
        stk->capacity = capacity;
    #endif /* DEBUG */

    stk->size = 0;
    stk->data = (stack_t*) calloc(stk->capacity, sizeof(stack_t));

    InitializeStackBuffer(stk, 0);
    
    #if DEBUG_LEVEL > 1
    setCanaryProtection(stk);
    #endif

    #if DEBUG_LEVEL > 0
    verify
    #endif /* DEBUG */

    return PROCESS_OK;
}

stackErr stackPush(stack* stk, stack_t value){

    #if DEBUG_LEVEL > 0
    verify
    #endif /* DEBUG */

    #if DEBUG_LEVEL > 1
    if(stk->size == stk->capacity - CANARY_PROTECTION_SIZE){
    #else   
    if(stk->size == stk->capacity){
    #endif
        #if DEBUG_LEVEL > 0
        verify
        #endif /* DEBUG */

        stk->capacity = (stk->capacity) * 2;
        stack_t* temp = (stack_t*) realloc(stk->data, (stk->capacity) * sizeof(stack_t));
        assert(temp);

        stk->data = temp;

        InitializeStackBuffer(stk, stk->size + 1);

        #if DEBUG_LEVEL > 1
        setCanaryProtection(stk);
        #endif /* DEBUG */
    }

    #if DEBUG_LEVEL
    verify
    #endif /* DEBUG */

    #if DEBUG_LEVEL > 1
    stk->data[stk->size + (CANARY_PROTECTION_SIZE / 2)] = value;
    #else
    stk->data[stk->size] = value;
    #endif

    (stk->size)++;

    #if DEBUG_LEVEL > 0
    verify
    #endif /* DEBUG */

    return PROCESS_OK;
}

stackErr stackPop(stack* stk, stack_t* stackElem){
    assert(stackElem);

    #if DEBUG_LEVEL > 0
    verify
    #endif /* DEBUG */

    #if DEBUG_LEVEL > 1 
    if(stk->data[stk->size] == CANARY_PROTECTION_NUMBER){
        return POP_WITH_BAD_SIZE;
    }
    *stackElem = stk->data[stk->size];
    stk->data[stk->size] = POISON_NUMBER;
    #else
    if(stk->size <= 0){
        return POP_WITH_BAD_SIZE;
    }
    *stackElem = stk->data[stk->size - 1];
    stk->data[stk->size - 1] = POISON_NUMBER;
    #endif

    (stk->size)--;

    #if DEBUG_LEVEL > 0
    verify
    #endif /* DEBUG */
    
    return PROCESS_OK;
}

static void InitializeStackBuffer(stack* stk, size_t startStackInd){
    assert(stk);

    for(size_t curStackElem = startStackInd; curStackElem < stk->capacity; curStackElem++){
        stk->data[curStackElem] = POISON_NUMBER;
    }
}

#if DEBUG_LEVEL > 0

static stackErr verifyStack(stack* stk, const char* function, const char* file, const int line){
    if(stk == NULL){
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
        #if DEBUG_LEVEL > 1
        else if(canaryCheck(stk) != PROCESS_OK);
        #endif
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

void static stackDump(stack* stk, const char* function, const char* file, const int line){
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
        if(stk->data[curElemInd] == POISON_NUMBER) {
            printf("\t\t[%lu] = %d" SET_STYLE_FONT_RED" (POISON NUMBER)\n"RESET, curElemInd, stk->data[curElemInd]);
        }
        #if DEBUG_LEVEL > 1
        else if(stk->data[curElemInd] == CANARY_PROTECTION_NUMBER){
            printf("\t\t[%lu] = %d" SET_STYLE_FONT_YELLOW" (CANARY NUMBER)\n"RESET, curElemInd, stk->data[curElemInd]);
        }
        #endif
        else{
            printf("\t\t*[%lu] = %d\n", curElemInd, stk->data[curElemInd]);
        }
    }
    printf("\t}\n");
    printf("}\n");
    
}
#endif /* DEBUG */

#if DEBUG_LEVEL > 1
static void setCanaryProtection(stack* stk){
    assert(stk);

    stk->data[0] = CANARY_PROTECTION_NUMBER;
    stk->data[stk->capacity - 1] = CANARY_PROTECTION_NUMBER;

}

static stackErr canaryCheck(stack* stk){
    assert(stk);
    if((stk->data[0] != CANARY_PROTECTION_NUMBER) || (stk->data[stk->capacity - CANARY_PROTECTION_SIZE / 2] != CANARY_PROTECTION_NUMBER)){

        assignErrorStruct(stk, CANARY_TORTURE);
        return CANARY_TORTURE;
    }

    return PROCESS_OK;
}
#endif /* DEBUG */

#endif /* STACK_H */