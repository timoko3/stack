#include "translator.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/// Нельзя убирать
#define DEBUG 0

#if DEBUG
static void printByteCodeBuffer(int* buffer, size_t curByteBufferSize);
#endif /* DEBUG */

#define ON_DEBUG(expression) if(DEBUG){expression;};

const size_t N_SPU_COMMANDS = 9;
spu_command spu_commands[] = {
    {"PUSH", PUSH},
    {"POP", POP},
    {"PUSHREG", PUSHREG},
    {"POPREG", POPREG},
    {"ADD", ADD},
    {"SUB", SUB},
    {"MUL", MUL},
    {"DIV", DIV},
    {"OUT", OUT}
};

static bool addStackFunctionParameters(int commandCode, char* stringPtr, int* byteCodeBuffer, size_t* curByteBufferSize);
static bool addRegisterFunctionParameters(int commandCode, char* stringPtr, int* byteCodeBuffer, size_t* curByteBufferSize);
static int  assembleCommand(char* curCommand);

int* createByteCodeBuffer(DataFromInputFIle* calcCommands, size_t* curByteBufferSize){
    assert(calcCommands);
    assert(curByteBufferSize);
    
    int* byteCodeBuffer = (int*) calloc(sizeof(int), calcCommands->nStrings * 2);
    fprintf(stderr, "byteCodeBuffer size = %lu\n", calcCommands->nStrings * 2);
    assert(byteCodeBuffer);

    for(size_t curString = 0; curString < calcCommands->nStrings; curString++){
        char curCommand[20] = {0}; // 20
        sscanf(calcCommands->strings[curString].stringPtr, "%s", curCommand);
        ON_DEBUG(printf("curCommand: %s\n", curCommand))
        
        // assembleCommand (com + arg)
        // decode / code 
        // assembleCommand command  MENTOR
        int commandCode = assembleCommand(curCommand); 
        if(commandCode == ASSEMBLE_FAILURE) continue;
        byteCodeBuffer[*curByteBufferSize] = commandCode;
        
        ON_DEBUG(printf("byteCodeBuffer now: %d\n", byteCodeBuffer[*curByteBufferSize]))
        
        (*curByteBufferSize)++;

        // command 
        addStackFunctionParameters(commandCode, calcCommands->strings[curString].stringPtr, byteCodeBuffer, curByteBufferSize);
        addRegisterFunctionParameters(commandCode, calcCommands->strings[curString].stringPtr, byteCodeBuffer, curByteBufferSize);

        ON_DEBUG(printf("\n"))
        
    }
    ON_DEBUG(printf("curByteBufferSize: %lu\n", *curByteBufferSize))

    return byteCodeBuffer;
}   

// const
static bool addStackFunctionParameters(int commandCode, char* stringPtr, int* byteCodeBuffer, size_t* curByteBufferSize){
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

static bool addRegisterFunctionParameters(int commandCode, char* stringPtr, int* byteCodeBuffer, size_t* curByteBufferSize){
    assert(stringPtr);
    assert(byteCodeBuffer);
    assert(curByteBufferSize);

    if((commandCode == PUSHREG) || 
       (commandCode == POPREG)){

        char reg[5]; // 5
        sscanf(stringPtr, "%*s %s", reg);

        /// преобразование имя стека в номер стека
        byteCodeBuffer[*curByteBufferSize] = reg[0] - 'A';
        ON_DEBUG(printf("byteCodeBuffer now: %d\n", byteCodeBuffer[*curByteBufferSize]);)
        (*curByteBufferSize)++;
    }

    return true;
}

static int assembleCommand(char* curCommand){
    assert(curCommand);
    unsigned long curCommandHash = hashStr(curCommand);
    for(size_t curCommandInd = 0; curCommandInd < N_SPU_COMMANDS; curCommandInd++){
        ON_DEBUG(printf("Результат сравнения строк при помощи cmpHashSpuCom(): %d\n", cmpHashSpuCom(curCommandHash, spu_commands[curCommandInd].hash)))

        

        if(!cmpHashSpuCom(curCommandHash, spu_commands[curCommandInd].hash)){
            ON_DEBUG(printf("Code to return: %d\n", spu_commands[curCommandInd].code))
            return spu_commands[curCommandInd].code;
        }
    }
    
    return ASSEMBLE_FAILURE;
}

void setSpuCommandsHash(){
    for(size_t curCommand = 0; curCommand < N_SPU_COMMANDS; curCommand++){
        spu_commands[curCommand].hash = hashStr(spu_commands[curCommand].name);
    }
}

#if DEBUG > 0
static void printByteCodeBuffer(int* buffer, size_t curByteBufferSize){
    assert(buffer);

    for(size_t curBufferElemInd = 0; curBufferElemInd < curByteBufferSize; curBufferElemInd++){
        printf("%ld) элемент буфера: %d\n", curBufferElemInd + 1, buffer[curBufferElemInd]);
    }
}

#endif /* DEBUG */