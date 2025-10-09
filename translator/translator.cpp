#include "translator.h"

#include <assert.h>
#include <stdlib.h>

////// redactor this file after prev mentoring

#define ON_DEBUG(expression) if(DEBUG_TRANSLATOR){expression;};

spu_command spu_commands[] = {
    {"PUSH",    0, PUSH},
    {"POP",     0, POP},
    {"PUSHREG", 0, PUSHREG},
    {"POPREG",  0, POPREG},
    {"ADD",     0, ADD},
    {"SUB",     0, SUB},
    {"MUL",     0, MUL},
    {"DIV",     0, DIV},
    {"OUT",     0, OUT},
    {"JMP",     0, JMP}
};

static bool addStackCommandParameters(int commandCode, char* stringPtr, int* byteCodeBuffer, size_t* curByteBufferSize);
static bool addRegisterCommandParameters(int commandCode, char* stringPtr, int* byteCodeBuffer, size_t* curByteBufferSize);
static int  encodeCommand(char* curCommand);

/// MENTOR Про перенос в strFunc хочу еще раз обсудить()
int* createByteCodeBuffer(DataFromInputFIle* spuCommandsNames, size_t* curByteBufferSize){
    assert(spuCommandsNames);
    assert(curByteBufferSize);
    
    int* byteCodeBuffer = (int*) calloc(sizeof(int), spuCommandsNames->nStrings * 2);
    assert(byteCodeBuffer);

    for(size_t curString = 0; curString < spuCommandsNames->nStrings; curString++){
        char curCommand[COMMAND_NAME_MAX_SIZE] = {0};
        sscanf(spuCommandsNames->strings[curString].stringPtr, "%s", curCommand);
        ON_DEBUG(printf("curCommand: %s\n", curCommand))
        
        int commandCode = encodeCommand(curCommand); 
        if(commandCode == ASSEMBLE_FAILURE) continue;
        byteCodeBuffer[*curByteBufferSize] = commandCode;
        
        ON_DEBUG(printf("byteCodeBuffer now: %d\n", byteCodeBuffer[*curByteBufferSize]))
        
        (*curByteBufferSize)++;

        // command 
        addStackCommandParameters(commandCode, spuCommandsNames->strings[curString].stringPtr, byteCodeBuffer, curByteBufferSize);
        addRegisterCommandParameters(commandCode, spuCommandsNames->strings[curString].stringPtr, byteCodeBuffer, curByteBufferSize);

        ON_DEBUG(printf("\n"))
        
    }
    ON_DEBUG(printf("curByteBufferSize: %lu\n", *curByteBufferSize))

    return byteCodeBuffer;
}   

// const
static bool addStackCommandParameters(int commandCode, char* stringPtr, int* byteCodeBuffer, size_t* curByteBufferSize){
    assert(stringPtr);
    assert(byteCodeBuffer);
    assert(curByteBufferSize);

    if((commandCode == PUSH) || 
       (commandCode == POP)){

        int pushParameter = 0;
        sscanf(stringPtr, "%*s %d", &pushParameter);

        byteCodeBuffer[*curByteBufferSize] = pushParameter;
        ON_DEBUG(printf("byteCodeBuffer now: %d\n", byteCodeBuffer[*curByteBufferSize]);)
        (*curByteBufferSize)++;
    }

    return true;
}

static bool addRegisterCommandParameters(int commandCode, char* stringPtr, int* byteCodeBuffer, size_t* curByteBufferSize){
    assert(stringPtr);
    assert(byteCodeBuffer);
    assert(curByteBufferSize);

    if((commandCode == PUSHREG) || 
       (commandCode == POPREG)){

        char reg[REGISTER_NAME_MAX_SIZE];
        sscanf(stringPtr, "%*s %s", reg);

        byteCodeBuffer[*curByteBufferSize] = reg[0] - A_ASCII_CODE; /////
        ON_DEBUG(printf("byteCodeBuffer now: %d\n", byteCodeBuffer[*curByteBufferSize]);)
        (*curByteBufferSize)++;
    }

    return true;
}

static int encodeCommand(char* curCommand){
    assert(curCommand);
    unsigned long curCommandHash = hashStr(curCommand);
    for(size_t curCommandInd = 0; curCommandInd < sizeof(spu_commands) / sizeof(spu_command); curCommandInd++){
        ON_DEBUG(printf("Результат сравнения строк при помощи cmpHashSpuCom(): %d\n", cmpHashSpuCom(curCommandHash, spu_commands[curCommandInd].hash)))
        
        if(!cmpHashSpuCom(curCommandHash, spu_commands[curCommandInd].hash)){
            printf("CurCommandIndex: %lu\n" , curCommandInd);
            ON_DEBUG(printf("Code to return: %d\n", spu_commands[curCommandInd].code))
            return spu_commands[curCommandInd].code;
        }
    }
    
    return ASSEMBLE_FAILURE;
}

void setSpuCommandsHash(){
    for(size_t curCommand = 0; curCommand < sizeof(spu_commands) / sizeof(spu_command); curCommand++){
        spu_commands[curCommand].hash = hashStr(spu_commands[curCommand].name);
    }
}

#if DEBUG_TRANSLATOR
void printByteCodeBuffer(int* buffer, size_t curByteBufferSize){
    assert(buffer);

    for(size_t curBufferElemInd = 0; curBufferElemInd < curByteBufferSize; curBufferElemInd++){
        printf("%ld) элемент буфера: %d\n", curBufferElemInd + 1, buffer[curBufferElemInd]);
    }
}

#endif /* DEBUG */