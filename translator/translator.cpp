#include "translator.h"
#include "cmd.h"
#include "general/hash.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

label_t nameLabels[N_LABELS] = {};

#define ON_DEBUG(expression) if(DEBUG_TRANSLATOR){expression;};

////// redactor this file after prev mentoring

static void poisonLabels(label_t* labels);
static void printNameTable();

static void setSpuCommandsHash();
static bool fillOpcodeBuffer(translator_t* translator);
static bool addCommandParameter(translator_t* translator, size_t curString);
static bool addRegParameter(translator_t* translator, size_t curString);
static bool addLabelParameter(translator_t* translator, size_t curString);
static bool addNumberParameter(translator_t* translator, size_t curString); 
static bool encodeCommand(char* curCommand, translator_t* translator);
static bool getLabel(translator_t* translator, size_t curString);
static int  getLabelPar(char* curLabel); // name const

translator_t translatorCtor(){
    translator_t translator;

    translator.cmds = commands;
    setSpuCommandsHash();

    // label_t nameLabels[N_LABELS] = {};
    translator.ptrLabels = nameLabels;
    poisonLabels(translator.ptrLabels);

    translator.opcode = (buffer_t*) calloc(1, sizeof(buffer_t)); 
    assert(translator.opcode);

    return translator;
}

bool loadTextCommands(translator_t* translator, strings_t textCommands){
    assert(translator);

    translator->input_buffer = textCommands;

    return true;
}

bool assemble(translator_t* translator){
    assert(translator);
    
    translator->opcode->ptr = (int*) calloc(sizeof(int), translator->input_buffer.count * 2 + PREAMBLE_SIZE);
    assert(translator->opcode);

    fillOpcodeBuffer(translator);
    translator->opcode->size = 0;
    printf("*************\n");
    fillOpcodeBuffer(translator);
    
    ON_DEBUG(printf("curByteBufferSize: %lu\n", translator->opcode->size))

    return true;
}   

bool translatorDtor(translator_t* translator){
    assert(translator);
    
    free(translator->opcode);

    return true;
}

static bool fillOpcodeBuffer(translator_t* translator){
    assert(translator);

    for(size_t curString = 0; curString < translator->input_buffer.count; curString++){
        char curCommand[COMMAND_NAME_MAX_SIZE] = {0};

        if(getLabel(translator, curString)) continue;

        sscanf(translator->input_buffer.ptr[curString].ptr, "%s", curCommand);
        ON_DEBUG(printf("curCommand: %s\n", curCommand))
        if(!encodeCommand(curCommand, translator)) break; 

        printf("%d\n", translator->opcode->ptr[curString]);
        
        ON_DEBUG(printf("byteCodeBuffer now: %d\n", translator->opcode->ptr[translator->opcode->size - 1]))
        
        addCommandParameter(translator, curString);

        ON_DEBUG(printf("\n"))
    }

    return true;
}

static bool encodeCommand(char* curCommand, translator_t* translator){
    assert(curCommand);

    unsigned long curCommandHash = hash(curCommand, myStrLen(curCommand));
    for(size_t curCommandInd = 0; curCommandInd < sizeof(commands) / sizeof(command_t); curCommandInd++){
        ON_DEBUG(printf("Результат сравнения строк при помощи cmpHashSpuCom(): %d\n", curCommandHash == commands[curCommandInd].hash))
        
        if(curCommandHash == commands[curCommandInd].hash){
            ON_DEBUG(printf("Code to return: %d\n", commands[curCommandInd].code))
            
            translator->opcode->ptr[translator->opcode->size] = commands[curCommandInd].code;
            translator->curCmdParType = commands[curCommandInd].param;
            
            (translator->opcode->size)++;

            return true;
        }
    }
    
    return false;
}

static bool addCommandParameter(translator_t* translator, size_t curString){
    assert(translator);

    switch(translator->curCmdParType){
        case REG_PARAM:    addRegParameter(translator, curString);    break;
        case LABEL_PARAM:  addLabelParameter(translator, curString);  break;
        case NUMBER_PARAM: addNumberParameter(translator, curString); break;
        case NO_PARAM:     break;
        default: break;
    }

    return true;
}
///
static bool addRegParameter(translator_t* translator, size_t curString){
    assert(translator);

    char reg[REGISTER_NAME_MAX_SIZE];
    sscanf(translator->input_buffer.ptr[curString].ptr, "%*s %s", reg);
    if (reg[0] == 'R'){
        translator->opcode->ptr[translator->opcode->size] = N_REGISTERS - 1;
    }
    else{
        translator->opcode->ptr[translator->opcode->size] = reg[0] - A_ASCII_CODE;
    }
    (translator->opcode->size)++;

    ON_DEBUG(printf("byteCodeBuffer now: %d\n", translator->opcode->ptr[translator->opcode->size]);)

    return true;
}

static bool addLabelParameter(translator_t* translator, size_t curString){
    assert(translator);

    int   labelPar     = 0;
    char  labelParName[20];

    if(sscanf(translator->input_buffer.ptr[curString].ptr, "%*s :%s", labelParName)){
        printf("labelParName: %s\n", labelParName);
        labelPar = getLabelPar(labelParName);
        printf("labelPar: %d\n", labelPar);

        if(labelPar != LABEL_POISON){
            translator->opcode->ptr[translator->opcode->size] = labelPar;
        } 

        (translator->opcode->size)++;
        return true;
    }
    else{
        printf("PROBLEM\n");
        
        return false;
    }
}

static bool addNumberParameter(translator_t* translator, size_t curString){
    assert(translator);

    int pushParameter = 0; // 
    sscanf(translator->input_buffer.ptr[curString].ptr, "%*s %d", &pushParameter);

    translator->opcode->ptr[translator->opcode->size] = pushParameter;
    ON_DEBUG(printf("byteCodeBuffer now: %d\n", translator->opcode->ptr[translator->opcode->size]);)

    (translator->opcode->size)++;

    return true;
}

static bool getLabel(translator_t* translator, size_t curString){
    assert(translator);

    static size_t curLabelInd = 0;

    char curLabelName[20]; // 20
    if(sscanf(translator->input_buffer.ptr[curString].ptr, ":%s", curLabelName)){
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