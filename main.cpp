#include "processor/processor.h"
#include "translator/translator.h"
#include "general/file.h"
#include "general/poison.h"
#include "general/stack/stack.h"

const char* const FLAG_INPUT_FILE  = "-i";
const char* const FLAG_OUTPUT_FILE = "-o";

const char* textCommandsFileName   = "quadratic.txt"; 
const char* outputByteCodeFileName = "factorial.asm";

// #define BUFFER_FROM_FILE

int main(void){
    data buf = {};
    if((parseStringsFile(&buf, textCommandsFileName)) == EXIT_FAILURE) return false;

    translator_t translator;
    translatorCtor(&translator);

    loadTextCommands(&translator, {buf.strings, buf.nStrings});

    assemble(&translator);

    buffer_t* opcode = &translator.opcode;

    translatorDtor(&translator);
    free(buf.buffer); 
    free(buf.strings); 

    processor spu1; 
    processorCtor(&spu1);

    loadOpcode(&spu1, *opcode);

    runProcessor(&spu1);
    
    processorDtor(&spu1);

    poisonMemory(spu1.opcode.ptr, spu1.opcode.size);
    free(spu1.opcode.ptr);
    spu1.opcode.ptr = NULL;
    poisonMemory(&spu1.opcode.size, sizeof(spu1.opcode.size));
}