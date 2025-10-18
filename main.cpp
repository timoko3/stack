#include "processor/processor.h"
#include "translator/translator.h"
#include "general/file.h"

#include "processor/cmd.h"

const char* const FLAG_INPUT_FILE  = "-i";
const char* const FLAG_OUTPUT_FILE = "-o";

const char* textCommandsFileName   = "test.txt"; 
const char* outputByteCodeFileName = "factorial.asm";

// #define BUFFER_FROM_FILE

int main(void){
    translator_t translator = translatorCtor();

    DataFromInputFIle buf = {};
    if((parseStringsFile(&buf, textCommandsFileName)) == EXIT_FAILURE) return false;
    
    loadTextCommands(&translator, {buf.strings, buf.nStrings});

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

    // stackPush(&spu1.stk, 5);
    // stackPush(&spu1.stk, 7);
    // stackPush(&spu1.stk, 3);

    runProcessor(&spu1);
    
    // stackDump(&spu1.stk, __FUNCTION__, __FILE__, __LINE__);

    free(buf.buffer); // 
    free(buf.strings); // 
    
    processorDtor(&spu1);
}