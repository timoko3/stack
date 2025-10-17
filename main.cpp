#include "processor/processor.h"
#include "translator/translator.h"
#include "general/file.h"

const char* const FLAG_INPUT_FILE  = "-i";
const char* const FLAG_OUTPUT_FILE = "-o";

const char* textCommandsFileName   = "factorial.txt"; 
const char* outputByteCodeFileName = "factorial.asm";

#define BUFFER_FROM_FILE

int main(void){
    translator_t translator = translatorCtor();

    loadTextCommands(&translator, textCommandsFileName);

    assemble(&translator);
    
    #ifdef BUFFER_FROM_FILE
    writeOpcode(translator.opcode, (const char*) outputByteCodeFileName);
    #endif /* BUFFER_FROM_FILE */

    processor spu1; 
    processorCtor(&spu1);
    #ifndef BUFFER_FROM_FILE
    spu1.opcode.ptr = translator.opcode->ptr;
    spu1.opcode.size = translator.opcode->size;
    #endif /* BUFFER_FROM_FILE */
    #ifdef BUFFER_FROM_FILE
    free(translator.opcode->ptr);
    #endif /* BUFFER_FROM_FILE */

    translatorDtor(&translator);

    #ifdef BUFFER_FROM_FILE
    getOpcodeBuffer(&spu1, outputByteCodeFileName); ///  
    #endif /* BUFFER_FROM_FILE */  

    runProcessor(&spu1);
    
    processorDtor(&spu1);
}