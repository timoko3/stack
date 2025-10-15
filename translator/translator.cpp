#include "translator.h"
#include "general/hash.h"

#include <assert.h>
#include <stdlib.h>

////// redactor this file after prev mentoring

static void poisonLabels(nameTranslator* labels);
void printNameTable();

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
    {"SQRT",    0, SQRT},
    {"OUT",     0, OUT},
    {"HLT",     0, HLT},
    {"JMP",     0, JMP},
    {"JB",      0, JB},
    {"JBE",     0, JBE},
    {"JA",      0, JA},
    {"JAE",     0, JAE},
    {"JE",      0, JE},
    {"JNE",     0, JNE},
    {"CALL",    0, CALL},
    {"RET",     0, RET}
};

nameTranslator nameTable[N_LABELS] = {};

static bool addCommandParameter(int commandCode, char* stringPtr, int* byteCodeBuffer, size_t* curByteBufferSize);
static int  encodeCommand(char* curCommand);
static int getLabelPar(char* curLabel);

/// MENTOR Про перенос в strFunc хочу еще раз обсудить()
int* createByteCodeBuffer(DataFromInputFIle* spuCommandsNames, size_t* curByteBufferSize){
    assert(spuCommandsNames);
    assert(curByteBufferSize);
    
    int* byteCodeBuffer = (int*) calloc(sizeof(int), spuCommandsNames->nStrings * 2 + PREAMBLE_SIZE);
    assert(byteCodeBuffer);
    
    poisonLabels(nameTable);

    fillByteCodeBuffer(spuCommandsNames, curByteBufferSize, byteCodeBuffer);
    *curByteBufferSize = 0;
    printf("*************\n");
    fillByteCodeBuffer(spuCommandsNames, curByteBufferSize, byteCodeBuffer);
    
    ON_DEBUG(printf("curByteBufferSize: %lu\n", *curByteBufferSize))

    return byteCodeBuffer;
}   

bool fillByteCodeBuffer(DataFromInputFIle* spuCommandsNames, size_t* curByteBufferSize, int* byteCodeBuffer){
    size_t curLabelInd = 0;
    for(size_t curString = 0; curString < spuCommandsNames->nStrings; curString++){
        char curCommand[COMMAND_NAME_MAX_SIZE] = {0};

        ///Обработка строки если она метка
        char curLabelName[20];
        if(sscanf(spuCommandsNames->strings[curString].stringPtr, ":%s", curLabelName)){
            printf("Замена\n");
            printf("curByteBufferSize: %d\n", (int)*curByteBufferSize);
            printNameTable();
            nameTable[curLabelInd].code = (int) *curByteBufferSize - 1;
            nameTable[curLabelInd].name = curLabelName;
            nameTable[curLabelInd].hash = hash(curLabelName, myStrLen(curLabelName, '\0'));
            curLabelInd++;
            printf("\n");
            printNameTable();
            continue;
        }

        sscanf(spuCommandsNames->strings[curString].stringPtr, "%s", curCommand);
        ON_DEBUG(printf("curCommand: %s\n", curCommand))
        
        int commandCode = encodeCommand(curCommand); 
        if(commandCode == ASSEMBLE_FAILURE) continue;
        byteCodeBuffer[*curByteBufferSize] = commandCode;
        
        ON_DEBUG(printf("byteCodeBuffer now: %d\n", byteCodeBuffer[*curByteBufferSize]))
        
        (*curByteBufferSize)++;

        addCommandParameter(commandCode, 
                            spuCommandsNames->strings[curString].stringPtr, 
                            byteCodeBuffer, 
                            curByteBufferSize);

        ON_DEBUG(printf("\n"))
        
    }

    return true;
}

static bool addCommandParameter(int commandCode, char* stringPtr, int* byteCodeBuffer, size_t* curByteBufferSize){
    assert(stringPtr);
    assert(byteCodeBuffer);
    assert(curByteBufferSize);

    if((commandCode == PUSHREG) || 
       (commandCode == POPREG)){
        char reg[REGISTER_NAME_MAX_SIZE];
        sscanf(stringPtr, "%*s %s", reg);
        if (reg[0] == 'R'){
            byteCodeBuffer[*curByteBufferSize] = N_REGISTERS - 1;
        }
        else{
            byteCodeBuffer[*curByteBufferSize] = reg[0] - A_ASCII_CODE;
        }

        ON_DEBUG(printf("byteCodeBuffer now: %d\n", byteCodeBuffer[*curByteBufferSize]);)
    }
    else if((commandCode == RET)){
        byteCodeBuffer[*curByteBufferSize] = N_REGISTERS - 1;
    }
    /// добавить структуры
    
    else if((commandCode == JMP) ||
            (commandCode == JB)  ||
            (commandCode == JBE) ||
            (commandCode == JA)  ||
            (commandCode == JAE) ||
            (commandCode == JE)  ||
            (commandCode == JNE) ||
            (commandCode == CALL)){
        int   labelPar     = 0;
        char  labelParName[20];

        // printNameTable();
        if(sscanf(stringPtr, "%*s :%s", labelParName)){
            printf("labelParName: %s\n", labelParName);
            labelPar = getLabelPar(labelParName);
            if(labelPar == LABEL_POISON){
                (*curByteBufferSize)++;
                return true;
            }

            printf("labelPar: %d\n", labelPar);

            byteCodeBuffer[*curByteBufferSize] = labelPar;
            (*curByteBufferSize)++;
            return true;
        }
        else{
            printf("PROBLEM\n");
            
            return false;
        }
        
    }

    else if((commandCode != ADD) &&
            (commandCode != SUB) &&
            (commandCode != MUL) &&
            (commandCode != DIV) &&
            (commandCode != OUT) &&
            (commandCode != HLT)){
        int pushParameter = 0;
        sscanf(stringPtr, "%*s %d", &pushParameter);

        byteCodeBuffer[*curByteBufferSize] = pushParameter;
        ON_DEBUG(printf("byteCodeBuffer now: %d\n", byteCodeBuffer[*curByteBufferSize]);)
        
    }

    (*curByteBufferSize)++;

    return true;
}

static int encodeCommand(char* curCommand){
    assert(curCommand);

    unsigned long curCommandHash = hash(curCommand, myStrLen(curCommand, '\0'));
    for(size_t curCommandInd = 0; curCommandInd < sizeof(spu_commands) / sizeof(spu_command); curCommandInd++){
        ON_DEBUG(printf("Результат сравнения строк при помощи cmpHashSpuCom(): %d\n", curCommandHash == spu_commands[curCommandInd].hash))
        
        if(curCommandHash == spu_commands[curCommandInd].hash){
            ON_DEBUG(printf("Code to return: %d\n", spu_commands[curCommandInd].code))
            return spu_commands[curCommandInd].code;
        }
    }
    
    return ASSEMBLE_FAILURE;
}

static int getLabelPar(char* curLabel){
    assert(curLabel);

    unsigned long curLabelHash = hash(curLabel, myStrLen(curLabel, '\0'));
    for(size_t curLabelInd = 0; curLabelInd < N_LABELS; curLabelInd++){
        if(curLabelHash == nameTable[curLabelInd].hash){
            return nameTable[curLabelInd].code;
        }
    }

    return LABEL_POISON;
}

void setSpuCommandsHash(){
    for(size_t curCommand = 0; curCommand < sizeof(spu_commands) / sizeof(spu_command); curCommand++){
        spu_commands[curCommand].hash = hash(spu_commands[curCommand].name, myStrLen(spu_commands[curCommand].name, '\0'));
    }
}

static void poisonLabels(nameTranslator* labels){
    assert(labels);

    for(size_t curLabel = 0; curLabel < N_LABELS; curLabel++){
        labels[curLabel].code = LABEL_POISON;
        labels[curLabel].hash = 0;
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

void printNameTable(){
    for(size_t curNameInd = 0; curNameInd < N_LABELS; curNameInd++){
        printf("%lu метка — код: %d, hash: %lu\n", curNameInd, nameTable[curNameInd].code, nameTable[curNameInd].hash);
    }
}