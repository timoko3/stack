#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "general/file.h"
#include "general/strFunc.h"
#include "translator.h"

const char* const FLAG_INPUT_FILE  = "-i";
const char* const FLAG_OUTPUT_FILE = "-o";

int main(/*int argc, char* argv[]*/){
    const char* textCommandsFileName   = "spuCommands.txt"; 
    const char* outputByteCodeFileName = "byteCode.asm";
    // if((argc > 2) && (argv[1] == FLAG_INPUT_FILE)){
    //     textCommandsFileName = argv[2];
    //     if((argc > 4) && (argv[3] == FLAG_OUTPUT_FILE)){
    //         outputByteCodeFileName = argv[4];
    //     }
    // }

    setSpuCommandsHash();

    DataFromInputFIle spuCommandsNames;
    stringsFromFileToFileDataStructure(&spuCommandsNames, (const char*) textCommandsFileName);
    
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



