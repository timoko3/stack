#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "general/file.h"
#include "general/strFunc.h"
#include "translator.h"

#define DEBUG 0

int main(void){
    fprintf(stderr, "MEOW BEGIN\n");

    setSpuCommandsHash();

    DataFromInputFIle calcCommands;
    stringsFromFileToFileDataStructure(&calcCommands);
    
    size_t curByteBufferSize = 0;
    int* byteCodeBuffer = createByteCodeBuffer(&calcCommands, &curByteBufferSize);
    
    #if DEBUG > 0
    printByteCodeBuffer(byteCodeBuffer, curByteBufferSize);
    #endif /* DEBUG */

    createByteCodeFile(byteCodeBuffer, curByteBufferSize);

    free(calcCommands.buffer); // 
    free(calcCommands.strings); // 
    free(byteCodeBuffer);

    fprintf(stderr, "MEOW END\n");
}



