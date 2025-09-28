#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <limits.h>
#include <stdlib.h>

#include "consoleColors.h"

#define DEBUG_LEVEL 2

#if DEBUG_LEVEL == 0
#define NDEBUG
#endif

const int POISON_NUMBER      = INT_MAX / 10;
const int AMOUNT_ERROR_TYPES = 9;

#if DEBUG_LEVEL > 1
const size_t CANARY_PROTECTION_SIZE    = 2;
const stack_t CANARY_PROTECTION_NUMBER = INT_MAX / 20;
#endif /* DEBUG */

#if DEBUG_LEVEL > 0

#define verify if(verifyStack(stk, __FUNCTION__, __FILE__, __LINE__)!=PROCESS_OK)return stk->error.type;
#define dump(stk) stackDump(stk, function, file, line);

#endif /* DEBUG */

enum stackErr{
    BAD_CAPACITY,
    NULL_POINTER,
    BAD_SIZE,
    BAD_MEMORY_ALLOCATION,
    SIZE_EXCEEDS_CAPACITY,
    CANARY_TORTURE,
    POP_WITH_BAD_SIZE,
    UNPLANNED_STACK_CHANGE,
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
    {UNPLANNED_STACK_CHANGE, "Стек незапланировано изменен\n"},
    {PROCESS_OK, "Все хорошо\n"}
};

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

stackErr stackCtor(stack* stk, size_t capacity);
stackErr stackPush(stack* stk, stack_t value);
stackErr stackPop(stack* stk, stack_t* stackElem);
stackErr stackDtor(stack* stk);
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

#if DEBUG_LEVEL > 2

unsigned long genHash(stack* stk);
static stackErr hashCheck(stack* stk);

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
    
    #if DEBUG_LEVEL > 0
    verify
    #endif /* DEBUG */

    InitializeStackBuffer(stk, 0);
    
    #if DEBUG_LEVEL == 0
    stk->error = errors[AMOUNT_ERROR_TYPES - 1];
    #endif

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

        #if DEBUG_LEVEL > 0
        verify
        #endif /* DEBUG */
        
        InitializeStackBuffer(stk, stk->size + 1);

        #if DEBUG_LEVEL > 1
        setCanaryProtection(stk);
        #endif /* DEBUG */
    }

    #if DEBUG_LEVEL > 0
    verify
    #endif /* DEBUG */

    #if DEBUG_LEVEL > 1
    stk->data[stk->size + (CANARY_PROTECTION_SIZE / 2)] = value;
    #else
    stk->data[stk->size] = value;
    #endif

    #if DEBUG_LEVEL > 2
    stk->hash = genHash(stk);
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

    #if DEBUG_LEVEL > 2
        stk->hash = genHash(stk);
    #endif /* DEBUG */

    (stk->size)--;

    #if DEBUG_LEVEL > 0
    verify
    #endif /* DEBUG */
    
    return PROCESS_OK;
}

stackErr stackDtor(stack* stk){
    assert(stk);
    
    for(size_t curElemInd = 0; curElemInd < stk->size; curElemInd++){
        stk->data[curElemInd] = rand();           
    }

    free(stk->data);

    return PROCESS_OK;
}

static void InitializeStackBuffer(stack* stk, size_t startStackInd){
    assert(stk);

    for(size_t curStackElem = startStackInd; curStackElem < stk->capacity; curStackElem++){
        stk->data[curStackElem] = POISON_NUMBER;
    }

    #if DEBUG_LEVEL > 2
    stk->hash = genHash(stk);
    #endif
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
        else if(canaryCheck(stk) != PROCESS_OK && stk->canaryStatus == true);
        #endif

        #if DEBUG_LEVEL > 2
        else if(hashCheck(stk) != PROCESS_OK);
        #endif

        else{
            assignErrorStruct(stk, PROCESS_OK);
            // return PROCESS_OK;
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
        printf(SET_STYLE_FONT_GREEN"\nOK\n"RESET);
    }
    else{
        printf(SET_STYLE_FONT_RED"\nERROR: %s\n"RESET, stk->error.text);
    }

    printf("%s()\n", __FUNCTION__);
    printf("stack<int>[%p]\n", stk);

    printf("Called from function %s at file %s:" SET_STYLE_BOLD_FONT_PURPLE " line %d\n" RESET, function, file, line);

    printf("{\n");
    printf(SET_STYLE_BOLD_FONT_BLUE "\tsize = %lu\n" RESET, stk->size);
    printf(SET_STYLE_BOLD_FONT_BLUE "\tcapacity = %lu\n\n" RESET, stk->capacity);

    if(sizeof(stack_t) * stk->capacity == malloc_usable_size(stk->data)){
        printf(SET_STYLE_BOLD_FONT_GREEN "\tНеобходимый размер блока data: %lu байт(-ов)\n" RESET, sizeof(stack_t) * stk->capacity);
        printf(SET_STYLE_BOLD_FONT_GREEN "\tРазмер выделенного блока data: %lu байт(-ов)\n" RESET, malloc_usable_size(stk->data));
    }
    else{
        printf(SET_STYLE_BOLD_FONT_GREEN "\tНеобходимый размер блока data: %lu байт(-ов)\n" RESET, sizeof(stack_t) * stk->capacity);
        printf( SET_STYLE_BOLD_FONT_RED"\tРазмер выделенного блока data:"RESET 
                SET_STYLE_BLINKING_FONT_RED" %lu байт(-ов)\n" RESET, malloc_usable_size(stk->data));
    }

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

    stk->canaryStatus = true;

    #if DEBUG_LEVEL > 2
    stk->hash = genHash(stk);
    #endif
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

#if DEBUG_LEVEL > 2

unsigned long genHash(stack* stk){
    assert(stk);

    unsigned long hash = 5381;

    size_t curElemInd = 0;
    while(curElemInd < stk->capacity){
        hash = ((hash << 5) + hash) + (unsigned long) stk->data[curElemInd];
        curElemInd++;
    }

    #if DEBUG_LEVEL > 3
    printf("CurHash: %lu\n", hash);
    #endif /* DEBUG */

    return hash;
}

static stackErr hashCheck(stack* stk){
    assert(stk);
    if(stk->hash != genHash(stk)){
        assignErrorStruct(stk, UNPLANNED_STACK_CHANGE);
        return UNPLANNED_STACK_CHANGE;
    }

    return PROCESS_OK;
}



#endif /* DEBUG */

#endif /* STACK_H */