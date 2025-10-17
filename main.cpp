#include "processor/processor.h"
#include "translator/translator.h"
#include "general/file.h"

const char* const FLAG_INPUT_FILE  = "-i";
const char* const FLAG_OUTPUT_FILE = "-o";

const char* textCommandsFileName   = "factorial.txt"; 
const char* outputByteCodeFileName = "factorial.asm";

int main(void){
    translator_t translator = translatorCtor();

    loadTextCommands(&translator, textCommandsFileName);

    assemble(&translator);

    writeOpcode(translator.opcode->ptr, translator.opcode->size, (const char*) outputByteCodeFileName);

    processor spu1; 
    getOpcodeBuffer(&spu1, outputByteCodeFileName); ///
    processorCtor(&spu1);

    runProcessor(&spu1);

    processorDtor(&spu1);

    free(translator.spuCommandsText.buffer); // 
    free(translator.spuCommandsText.strings); // 
    free(translator.opcode->ptr);
    free(translator.opcode);
}