#include "file.h"
#include "strFunc.h"
#include "translator.h"

struct biteCodeBuffer{
    size_t size;
    char* pointer;
};

spu_command spu_commands[N_SPU_COMMANDS] = {
    {"PUSH", PUSH},
    {"ADD", ADD},
    {"SUB", SUB},
    {"MUL", MUL},
    {"DIV", DIV},
    {"OUT", OUT}
};

/// Нельзя убирать
#define DEBUG 0

#define ON_DEBUG(expression) if(DEBUG){expression};

#if DEBUG
static void printBiteCodeBuffer(int* buffer, size_t curBiteBufferSize);
#endif /* DEBUG */

static int* createBiteCodeBuffer(DataFromInputFIle* calcCommands, size_t* curBiteBufferSize);
static int assemble(char* curCommand);
static void createBiteCodeFile(int* biteCodeBuffer, int curBiteBufferSize);

int main(void){
    DataFromInputFIle calcCommands;
    stringsFromFileToStructure(&calcCommands);

    size_t curBiteBufferSize = 0;
    int* biteCodeBuffer = createBiteCodeBuffer(&calcCommands, &curBiteBufferSize);

    #if DEBUG > 0
    printBiteCodeBuffer(biteCodeBuffer, curBiteBufferSize);
    #endif /* DEBUG */

    createBiteCodeFile(biteCodeBuffer, curBiteBufferSize);

    free(biteCodeBuffer);
}

static int* createBiteCodeBuffer(DataFromInputFIle* calcCommands, size_t* curBiteBufferSize){
    assert(calcCommands);
    assert(curBiteBufferSize);

    /// выделение памяти можно доработать
    int* biteCodeBuffer = (int*) calloc(sizeof(int), calcCommands->nStrings * 2);
    assert(biteCodeBuffer);

    for(int curString = 0; curString < calcCommands->nStrings; curString++){
        char curCommand[20] = {0};
        sscanf(calcCommands->strings[curString].stringPtr, "%s", curCommand);
        ON_DEBUG(printf("curCommand: %s\n", curCommand);)

        int commandCode = assemble(curCommand);
        if(commandCode == ASSEMBLE_FAILURE)continue;
        
        biteCodeBuffer[*curBiteBufferSize] = commandCode;
        ON_DEBUG(printf("biteCodeBuffer now: %d\n", biteCodeBuffer[*curBiteBufferSize]);)
        (*curBiteBufferSize)++;

        if(!strCmpSpuCom4(spu_commands[0].name, curCommand)){
            int pushParameter = 0;

            sscanf(calcCommands->strings[curString].stringPtr, "%*s %d", &pushParameter);

            biteCodeBuffer[*curBiteBufferSize] = pushParameter;
            ON_DEBUG(printf("biteCodeBuffer now: %d\n", biteCodeBuffer[*curBiteBufferSize]);)
            (*curBiteBufferSize)++;
        }

        

        ON_DEBUG(printf("\n");)
        
    }
    ON_DEBUG(printf("curBiteBufferSize: %ld\n", *curBiteBufferSize);)

    return biteCodeBuffer;
}   

static int assemble(char* curCommand){
    assert(curCommand);

    for(size_t curCommandInd = 0; curCommandInd < N_SPU_COMMANDS; curCommandInd++){
        ON_DEBUG(printf("Результат сравнения строк при помощи strCmpSpuCom4(): %d\n", strCmpSpuCom4(curCommand, spu_commands[curCommandInd].name));)
        if(!strCmpSpuCom4(curCommand, spu_commands[curCommandInd].name)){
            ON_DEBUG(printf("Code to return: %d\n", spu_commands[curCommandInd].code);)
            return spu_commands[curCommandInd].code;
        }
    }
    
    return ASSEMBLE_FAILURE;
}

static void createBiteCodeFile(int* biteCodeBuffer, int curBiteBufferSize){
    assert(biteCodeBuffer);

    FILE* biteCodeFile = openBiteCodeFile();
    assert(biteCodeFile);

    size_t written = fwrite(biteCodeBuffer, sizeof(int), curBiteBufferSize, biteCodeFile);
    if(written != curBiteBufferSize){
        ///log
        perror("fwrite error");
    }

    fclose(biteCodeFile);
}

#if DEBUG > 0
static void printBiteCodeBuffer(int* buffer, size_t curBiteBufferSize){
    assert(buffer);

    for(size_t curBufferElemInd = 0; curBufferElemInd < curBiteBufferSize; curBufferElemInd++){
        printf("%ld) элемент буфера: %d\n", curBufferElemInd + 1, buffer[curBufferElemInd]);
    }
}

#endif /* DEBUG */