#include "processor.h"
#include "translator.h"
#include "file.h"

const char* const FLAG_INPUT_FILE  = "-i";
const char* const FLAG_OUTPUT_FILE = "-o";

const char* textCommandsFileName   = "factorial.txt"; 
const char* outputByteCodeFileName = "factorial.asm";

int main(void){
    translator_t translator = translatorCtor();

    parseStringsFile(&translator.spuCommandsText, (const char*) textCommandsFileName); 

    int* byteCodeBuffer = createByteCodeBuffer(&spuTextCommands, &curByteBufferSize);

    // write 
    createByteCodeFile(byteCodeBuffer, curByteBufferSize, (const char*) outputByteCodeFileName);

    free(spuTextCommands.buffer); // 
    free(spuTextCommands.strings); // 
    free(byteCodeBuffer);
}