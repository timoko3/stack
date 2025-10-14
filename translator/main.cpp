#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "general/file.h"
#include "general/strFunc.h"
#include "translator.h"

const char* const FLAG_INPUT_FILE  = "-i";
const char* const FLAG_OUTPUT_FILE = "-o";

int main(){
    const char* textCommandsFileName   = "test.txt"; 
    const char* outputByteCodeFileName = "test.asm";

    setSpuCommandsHash();

    DataFromInputFIle spuCommandsNames;
    parseStringsFile(&spuCommandsNames, (const char*) textCommandsFileName); 
    
    size_t curByteBufferSize = 0;
    int* byteCodeBuffer = createByteCodeBuffer(&spuCommandsNames, &curByteBufferSize);
    
    #if DEBUG_TRANSLATOR > 0
    printByteCodeBuffer(byteCodeBuffer, curByteBufferSize);
    #endif /* DEBUG_TRANSLATOR */

    // write 
    createByteCodeFile(byteCodeBuffer, curByteBufferSize, (const char*) outputByteCodeFileName);

    free(spuCommandsNames.buffer); // 
    free(spuCommandsNames.strings); // 
    free(byteCodeBuffer);

}



