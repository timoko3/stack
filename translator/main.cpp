#include <stdio.h>

#include "general/file.h"
#include "general/strFunc.h"
// #include "translator.h"

struct byteCodeBuffer{
    size_t size;
    char* pointer;
};

spu_command spu_commands[N_SPU_COMMANDS] = {
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

/// Нельзя убирать
#define DEBUG 0

#define ON_DEBUG(expression) if(DEBUG){expression;};

#if DEBUG
static void printByteCodeBuffer(int* buffer, size_t curByteBufferSize);
#endif /* DEBUG */

static int* createByteCodeBuffer(DataFromInputFIle* calcCommands, size_t* curByteBufferSize);
static bool addStackFunctionParameters(int commandCode, char* stringPtr, int* byteCodeBuffer, size_t* curByteBufferSize);
static bool addRegisterFunctionParameters(int commandCode, char* stringPtr, int* byteCodeBuffer, size_t* curByteBufferSize);
static int assemble(char* curCommand);
static void createByteCodeFile(int* byteCodeBuffer, int curByteBufferSize);

int main(void){
    fprintf(stderr, "MEOW BEGIN\n");
    DataFromInputFIle calcCommands;
    stringsFromFileToStructure(&calcCommands);
    
    size_t curByteBufferSize = 0;
    int* byteCodeBuffer = createByteCodeBuffer(&calcCommands, &curByteBufferSize);
    
    #if DEBUG > 0
    printByteCodeBuffer(byteCodeBuffer, curByteBufferSize);
    #endif /* DEBUG */

    createByteCodeFile(byteCodeBuffer, curByteBufferSize);

    free(calcCommands.buffer); // 
    free(calcCommands.strings); // 
    free(byteCodeBuffer);

    fprintf(stderr, "MEOW END\n");
}

static int* createByteCodeBuffer(DataFromInputFIle* calcCommands, size_t* curByteBufferSize){
    assert(calcCommands);
    assert(curByteBufferSize);
    
    /// выделение памяти можно доработать
    int* byteCodeBuffer = (int*) calloc(sizeof(int), calcCommands->nStrings * 2);
    fprintf(stderr, "byteCodeBuffer size = %d\n", calcCommands->nStrings * 2);
    assert(byteCodeBuffer);

    for(int curString = 0; curString < calcCommands->nStrings; curString++){
        char curCommand[20] = {0}; // 20
        sscanf(calcCommands->strings[curString].stringPtr, "%s", curCommand);
        ON_DEBUG(printf("curCommand: %s\n", curCommand))
        
        // assemble (com + arg)
        // decode / code 
        // assemble command 
        int commandCode = assemble(curCommand); 
        if(commandCode == ASSEMBLE_FAILURE) continue;
        byteCodeBuffer[*curByteBufferSize] = commandCode;
        
        ON_DEBUG(printf("byteCodeBuffer now: %d\n", byteCodeBuffer[*curByteBufferSize]))
        
        fprintf(stderr, "curByteBufferSize = (%p)\n", curByteBufferSize);
        fprintf(stderr, "curByteBufferSize = (%p) %d\n", curByteBufferSize, *curByteBufferSize);
        
        (*curByteBufferSize)++;

        // command
        addStackFunctionParameters(commandCode, calcCommands->strings[curString].stringPtr, byteCodeBuffer, curByteBufferSize);
        addRegisterFunctionParameters(commandCode, calcCommands->strings[curString].stringPtr, byteCodeBuffer, curByteBufferSize);

        ON_DEBUG(printf("\n"))
        
    }
    ON_DEBUG(printf("curByteBufferSize: %ld\n", *curByteBufferSize))

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

static int assemble(char* curCommand){
    assert(curCommand);

    for(size_t curCommandInd = 0; curCommandInd < N_SPU_COMMANDS; curCommandInd++){
        ON_DEBUG(printf("Результат сравнения строк при помощи strCmpSpuCom(): %d\n", strCmpSpuCom(curCommand, spu_commands[curCommandInd].name)))
        if(!strCmpSpuCom(curCommand, spu_commands[curCommandInd].name)){
            ON_DEBUG(printf("Code to return: %d\n", spu_commands[curCommandInd].code))
            return spu_commands[curCommandInd].code;
        }
    }
    
    return ASSEMBLE_FAILURE;
}

static void createByteCodeFile(int* byteCodeBuffer, int curByteBufferSize){
    assert(byteCodeBuffer);

    FILE* byteCodeFile = openByteCodeFile();
    assert(byteCodeFile);

    size_t written = fwrite(byteCodeBuffer, sizeof(int), curByteBufferSize, byteCodeFile);
    if(written != curByteBufferSize){
        ///log
        perror("fwrite error");
    }

    fclose(byteCodeFile);
}

#if DEBUG > 0
static void printByteCodeBuffer(int* buffer, size_t curByteBufferSize){
    assert(buffer);

    for(size_t curBufferElemInd = 0; curBufferElemInd < curByteBufferSize; curBufferElemInd++){
        printf("%ld) элемент буфера: %d\n", curBufferElemInd + 1, buffer[curBufferElemInd]);
    }
}

#endif /* DEBUG */