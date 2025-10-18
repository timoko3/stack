#include "translator.h"
#include "general/hash.h"
#include "general/poison.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define ON_DEBUG(expression) if(DEBUG_TRANSLATOR){expression;};

static void poisonLabels(label_t* labels);
static void setSpuCommandsHash();
// static void printNameTable();

static bool assemblePass(translator_t* translator);
static bool getLabel(translator_t* translator);
static bool encodeCommand(translator_t* translator);

static bool addCommandParameter(translator_t* translator);
static bool addRegParameter(translator_t* translator);
static bool addNumberParameter(translator_t* translator); 
static bool addLabelParameter(translator_t* translator);
static int  getCmdNumber(translator_t* translator);

bool translatorCtor(translator_t* translator){
    translator->cmds = commands;
    setSpuCommandsHash();

    static label_t nameLabels[N_LABELS] = {};
    translator->labels = nameLabels;
    poisonLabels(translator->labels);

    return translator;
}

bool loadTextCommands(translator_t* translator, strings_t textCommands){
    assert(translator);

    translator->input_buffer = textCommands;

    return true;
}

bool assemble(translator_t* translator){
    assert(translator);
    
    translator->opcode.ptr = (int*) calloc(sizeof(int), translator->input_buffer.count * 2 + PREAMBLE_SIZE);
    assert(translator->opcode.ptr);

    assemblePass(translator);
    translator->opcode.size = 0;
    printf("*************\n");
    assemblePass(translator);
    
    ON_DEBUG(printf("curByteBufferSize: %lu\n", translator->opcode.size))

    return true;
}   

bool translatorDtor(translator_t* translator){
    assert(translator);

    poisonMemory(translator->cmds, sizeof(translator->cmds));
    poisonMemory(&translator->input_buffer, sizeof(translator->input_buffer));
    poisonMemory(translator->labels, sizeof(translator->labels));
    poisonMemory(&translator->curState, sizeof(translator->curState));

    return true;
}

static bool assemblePass(translator_t* translator){
    assert(translator);

    for(size_t curStringInd = 0; curStringInd < translator->input_buffer.count - 1; curStringInd++){
        translator->curState.StringInd = curStringInd;

        if(getLabel(translator)) continue;

        char curCmdName[COMMAND_NAME_MAX_SIZE] = {0};
        sscanf(translator->input_buffer.ptrs[curStringInd].ptr, "%s", curCmdName);
        translator->curState.cmdName = curCmdName;
        ON_DEBUG(printf("curCmdName: %s\n", curCmdName))
        if(!encodeCommand(translator)) break; 
        
        ON_DEBUG(printf("byteCodeBuffer now: %d\n", translator->opcode.ptr[translator->opcode.size - 1]))
        
        addCommandParameter(translator);
        printf("curStringInd: %lu\n", curStringInd);
        printf("curStringInd: %lu\n", translator->input_buffer.count);
        ON_DEBUG(printf("\n"))
    }
    printf("FILLED\n");
    return true;
}

static bool getLabel(translator_t* translator){
    assert(translator);

    static size_t curLabelInd = 0;

    char curLabelName[LABEL_NAME_MAX_SIZE]; 
    if(sscanf(translator->input_buffer.ptrs[translator->curState.StringInd].ptr, ":%s", curLabelName)){
        translator->labels[curLabelInd].addr = (int) translator->opcode.size - 1;
        translator->labels[curLabelInd].name = curLabelName;
        translator->labels[curLabelInd].hash = hash(curLabelName, myStrLen(curLabelName));
        curLabelInd++;
        printf("\n");
        return true;
    }

    return false;
}

static bool encodeCommand(translator_t* translator){
    assert(translator);

    unsigned long curCommandHash = hash(translator->curState.cmdName, myStrLen(translator->curState.cmdName));
    for(size_t curCommandInd = 0; curCommandInd < sizeof(commands) / sizeof(command_t); curCommandInd++){
        ON_DEBUG(printf("Результат сравнения строк при помощи cmpHashSpuCom(): %d\n", curCommandHash == commands[curCommandInd].hash))
        
        if(curCommandHash == commands[curCommandInd].hash){
            ON_DEBUG(printf("Code to return: %d\n", commands[curCommandInd].code))
            
            printf("command name: %s, code: %d\n", commands[curCommandInd].name, commands[curCommandInd].code);
            translator->opcode.ptr[translator->opcode.size] = commands[curCommandInd].code;
            translator->curState.par = commands[curCommandInd].param;
            
            (translator->opcode.size)++;

            return true;
        }
    }
    
    return false;
}

static bool addCommandParameter(translator_t* translator){
    assert(translator);

    printf("translator->curCmdParType — %d\n", translator->curState.par);
    switch(translator->curState.par){
        case REG_PARAM:    addRegParameter(translator);    break;
        case LABEL_PARAM:  addLabelParameter(translator);  break;
        case NUMBER_PARAM: addNumberParameter(translator); break;
        case NO_PARAM:     break;
        default: break;
    }

    return true;
}
///
static bool addRegParameter(translator_t* translator){
    assert(translator);

    if(translator->opcode.ptr[translator->opcode.size - 1] == RET){
        translator->opcode.ptr[translator->opcode.size] = N_REGISTERS - 1;

        (translator->opcode.size)++;

        return true;
    }

    char reg[REGISTER_NAME_MAX_SIZE];
    sscanf(translator->input_buffer.ptrs[translator->curState.StringInd].ptr, "%*s %sX\n", reg);
    if (reg[0] == 'R'){
        translator->opcode.ptr[translator->opcode.size] = N_REGISTERS - 1;
    }
    else{
        translator->opcode.ptr[translator->opcode.size] = reg[0] - UPPER_SYM_MIN;
    }
    (translator->opcode.size)++;

    ON_DEBUG(printf("byteCodeBuffer now: %d\n", translator->opcode.ptr[translator->opcode.size]);)

    return true;
}

static bool addNumberParameter(translator_t* translator){
    assert(translator);

    int numberParam = 0; 
    sscanf(translator->input_buffer.ptrs[translator->curState.StringInd].ptr, "%*s %d", &numberParam);

    translator->opcode.ptr[translator->opcode.size] = numberParam;
    ON_DEBUG(printf("byteCodeBuffer now: %d\n", translator->opcode.ptr[translator->opcode.size]);)

    (translator->opcode.size)++;

    return true;
}

static bool addLabelParameter(translator_t* translator){
    assert(translator);
    
    int   cmdNumber     = 0;
    char  labelParName[LABEL_NAME_MAX_SIZE];

    if(sscanf(translator->input_buffer.ptrs[translator->curState.StringInd].ptr, "%*s :%s", labelParName)){
        translator->curState.labelName = labelParName;
        cmdNumber = getCmdNumber(translator);

        if(cmdNumber != LABEL_POISON){
            translator->opcode.ptr[translator->opcode.size] = cmdNumber;
        } 

        (translator->opcode.size)++;
        return true;
    }
    else{
        
        return false;
    }
}

static int getCmdNumber(translator_t* translator){
    assert(translator);

    unsigned long curLabelHash = hash(translator->curState.labelName, myStrLen(translator->curState.labelName));
    for(size_t curLabelInd = 0; curLabelInd < N_LABELS; curLabelInd++){
        if(curLabelHash == translator->labels[curLabelInd].hash){
            return translator->labels[curLabelInd].addr;
        }
    }

    return LABEL_POISON;
}

static void setSpuCommandsHash(){
    for(size_t curCmdName = 0; curCmdName < sizeof(commands) / sizeof(command_t); curCmdName++){
        commands[curCmdName].hash = hash(commands[curCmdName].name, myStrLen(commands[curCmdName].name));
    }
}

static void poisonLabels(label_t* labels){
    assert(labels);

    for(size_t curLabel = 0; curLabel < N_LABELS; curLabel++){
        labels[curLabel].addr = LABEL_POISON;
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

// static void printNameTable(){
//     for(size_t curNameInd = 0; curNameInd < N_LABELS; curNameInd++){
//         printf("%lu метка — код: %d, hash: %lu\n", curNameInd, nameLabels[curNameInd].addr, nameLabels[curNameInd].hash);
//     }
// }