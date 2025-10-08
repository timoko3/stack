#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "general/file.h"
#include "general/strFunc.h"
#include "translator.h"

#define DEBUG 0

void createByteCodeFile(int* byteCodeBuffer, size_t curByteBufferSize);

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

void createByteCodeFile(int* byteCodeBuffer, size_t curByteBufferSize){
    assert(byteCodeBuffer);

    fileDescription byteCode = {
        BYTE_CODE_FILE_NAME,
        "wb"
    };

    FILE* byteCodeFile = myOpenFile(&byteCode);
    assert(byteCodeFile);

    size_t written = fwrite(byteCodeBuffer, sizeof(int), curByteBufferSize, byteCodeFile);
    if(written != curByteBufferSize){
        ///log
        perror("fwrite error");
    }

    fclose(byteCodeFile);
}

