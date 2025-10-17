#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "general/file.h"
#include "general/strFunc.h"
#include "translator/cmd.h"

#define DEBUG_TRANSLATOR 0

const size_t COMMAND_NAME_MAX_SIZE  = 10;
const size_t REGISTER_NAME_MAX_SIZE = 5;
const int A_ASCII_CODE              = (int) 'A';
const int ASSEMBLE_FAILURE          = -1;

const size_t N_LABELS  = 10;
const int LABEL_POISON = 46878756;

// enum paramType{
//     NO_PARAM,
//     LABEL_PARAM,
//     REG_PARAM,
//     NUMBER_PARAM
// };

struct label_t{
    const char* name;
    unsigned long hash;
    int code; // 
};


struct translator_t{ 
    command_t* cmds; 
    strings_t  input_buffer; 
    buffer_t*  opcode; // 
    label_t*   ptrLabels;

    // // state
    // // {
    //     // cur string
    //      size_t curCmd;
        paramType  curCmdParType;
    // // }
};

translator_t translatorCtor();
bool loadTextCommands(translator_t* translator, strings_t textCommands);
bool assemble(translator_t* translator);
bool translatorDtor(translator_t* translator);

#if DEBUG_TRANSLATOR
void printByteCodeBuffer(int* buffer, size_t curByteBufferSize);
#endif /* DEBUG */

#endif /* TRANSLATOR_H */
