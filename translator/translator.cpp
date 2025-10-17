#include "translator.h"
#include "general/hash.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

command_t commands[] = {
    {PUSH,    "PUSH",    0},
    {POP,     "POP",     0},
    {PUSHREG, "PUSHREG", 0},
    {POPREG,  "POPREG",  0},
    {ADD,     "ADD",     0},
    {SUB,     "SUB",     0},
    {MUL,     "MUL",     0},
    {DIV,     "DIV",     0},
    {SQRT,    "SQRT",    0},
    {OUT,     "OUT",     0},
    {HLT,     "HLT",     0},
    {JMP,     "JMP",     0},
    {JB,      "JB",      0},
    {JBE,     "JBE",     0},
    {JA,      "JA",      0},
    {JAE,     "JAE",     0},
    {JE,      "JE",      0},
    {JNE,     "JNE",     0},
    {CALL,    "CALL",    0},
    {RET,     "RET",     0}
};

label_t nameLabels[N_LABELS] = {};

#define ON_DEBUG(expression) if(DEBUG_TRANSLATOR){expression;};

////// redactor this file after prev mentoring

static void poisonLabels(label_t* labels);
static void printNameTable();

static void setSpuCommandsHash();
static bool fillOpcodeBuffer(translator_t* translator);
static bool addCommandParameter(int commandCode, translator_t* translator, size_t curString);
static int  encodeCommand(char* curCommand);
static bool getLabel(translator_t* translator, size_t curString);
static int  getLabelPar(char* curLabel);

translator_t translatorCtor(){
    translator_t translator;

    translator.cmds = commands;
    setSpuCommandsHash();

    translator.ptrLabels = nameLabels;

    translator.opcode = (buffer_t*) calloc(1, sizeof(buffer_t)); 
    assert(translator.opcode);

    return translator;
}

bool loadTextCommands(translator_t* translator, const char* fileName){
    assert(translator);
    assert(fileName);

    if((parseStringsFile(&translator->spuCommandsText, (const char*) fileName)) == EXIT_FAILURE) return false;

    return true;
}

bool assemble(translator_t* translator){
    assert(translator);
    
    translator->opcode->ptr = (int*) calloc(sizeof(int), translator->spuCommandsText.nStrings * 2 + PREAMBLE_SIZE);
    assert(translator->opcode);
    
    poisonLabels(translator->ptrLabels);

    fillOpcodeBuffer(translator);
    translator->opcode->size = 0;
    printf("*************\n");
    fillOpcodeBuffer(translator);
    
    ON_DEBUG(printf("curByteBufferSize: %lu\n", translator->opcode->size))

    return true;
}   

bool translatorDtor(translator_t* translator){
    assert(translator);
    
    free(translator->spuCommandsText.buffer); // 
    free(translator->spuCommandsText.strings); // 
    free(translator->opcode);

    return true;
}

static bool fillOpcodeBuffer(translator_t* translator){
    assert(translator);

    for(size_t curString = 0; curString < translator->spuCommandsText.nStrings; curString++){
        char curCommand[COMMAND_NAME_MAX_SIZE] = {0};

        ///Обработка строки если она метка
        if(getLabel(translator, curString)) continue;

        sscanf(translator->spuCommandsText.strings[curString].stringPtr, "%s", curCommand);
        ON_DEBUG(printf("curCommand: %s\n", curCommand))
        int commandCode = encodeCommand(curCommand); 
        if(commandCode == ASSEMBLE_FAILURE) continue;
        translator->opcode->ptr[translator->opcode->size] = commandCode;
        
        ON_DEBUG(printf("byteCodeBuffer now: %d\n", translator->opcode->ptr[translator->opcode->size]))
        
        (translator->opcode->size)++;

        addCommandParameter(commandCode, translator, curString);

        ON_DEBUG(printf("\n"))
        
    }

    return true;
}

static bool addCommandParameter(int commandCode, translator_t* translator, size_t curString){
    assert(translator);

    if((commandCode == PUSHREG) || 
       (commandCode == POPREG)){
        char reg[REGISTER_NAME_MAX_SIZE];
        sscanf(translator->spuCommandsText.strings[curString].stringPtr, "%*s %s", reg);
        if (reg[0] == 'R'){
            translator->opcode->ptr[translator->opcode->size] = N_REGISTERS - 1;
        }
        else{
            translator->opcode->ptr[translator->opcode->size] = reg[0] - A_ASCII_CODE;
        }

        ON_DEBUG(printf("byteCodeBuffer now: %d\n", translator->opcode->ptr[translator->opcode->size]);)
    }
    else if((commandCode == RET)){
        translator->opcode->ptr[translator->opcode->size] = N_REGISTERS - 1;
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
        if(sscanf(translator->spuCommandsText.strings[curString].stringPtr, "%*s :%s", labelParName)){
            printf("labelParName: %s\n", labelParName);
            labelPar = getLabelPar(labelParName);
            if(labelPar == LABEL_POISON){
                (translator->opcode->size)++;
                return true;
            }

            printf("labelPar: %d\n", labelPar);

            translator->opcode->ptr[translator->opcode->size] = labelPar;
            (translator->opcode->size)++;
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
        sscanf(translator->spuCommandsText.strings[curString].stringPtr, "%*s %d", &pushParameter);

        translator->opcode->ptr[translator->opcode->size] = pushParameter;
        ON_DEBUG(printf("byteCodeBuffer now: %d\n", translator->opcode->ptr[translator->opcode->size]);)
        
    }

    (translator->opcode->size)++;

    return true;
}

static int encodeCommand(char* curCommand){
    assert(curCommand);

    unsigned long curCommandHash = hash(curCommand, myStrLen(curCommand, '\0'));
    for(size_t curCommandInd = 0; curCommandInd < sizeof(commands) / sizeof(command_t); curCommandInd++){
        ON_DEBUG(printf("Результат сравнения строк при помощи cmpHashSpuCom(): %d\n", curCommandHash == commands[curCommandInd].hash))
        
        if(curCommandHash == commands[curCommandInd].hash){
            ON_DEBUG(printf("Code to return: %d\n", commands[curCommandInd].code))
            return commands[curCommandInd].code;
        }
    }
    
    return ASSEMBLE_FAILURE;
}

static bool getLabel(translator_t* translator, size_t curString){
    assert(translator);

    static size_t curLabelInd = 0;

    char curLabelName[20];
    if(sscanf(translator->spuCommandsText.strings[curString].stringPtr, ":%s", curLabelName)){
        printf("Замена\n");
        printf("curByteBufferSize: %d\n", (int)translator->opcode->size);
        printNameTable();
        nameLabels[curLabelInd].code = (int) translator->opcode->size - 1;
        nameLabels[curLabelInd].name = curLabelName;
        nameLabels[curLabelInd].hash = hash(curLabelName, myStrLen(curLabelName, '\0'));
        curLabelInd++;
        printf("\n");
        printNameTable();
        return true;
    }

    return false;
}

static int getLabelPar(char* curLabel){
    assert(curLabel);

    unsigned long curLabelHash = hash(curLabel, myStrLen(curLabel, '\0'));
    for(size_t curLabelInd = 0; curLabelInd < N_LABELS; curLabelInd++){
        if(curLabelHash == nameLabels[curLabelInd].hash){
            return nameLabels[curLabelInd].code;
        }
    }

    return LABEL_POISON;
}

static void setSpuCommandsHash(){
    for(size_t curCommand = 0; curCommand < sizeof(commands) / sizeof(command_t); curCommand++){
        commands[curCommand].hash = hash(commands[curCommand].name, myStrLen(commands[curCommand].name, '\0'));
    }
}

static void poisonLabels(label_t* labels){
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

static void printNameTable(){
    for(size_t curNameInd = 0; curNameInd < N_LABELS; curNameInd++){
        printf("%lu метка — код: %d, hash: %lu\n", curNameInd, nameLabels[curNameInd].code, nameLabels[curNameInd].hash);
    }
}