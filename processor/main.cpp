#include "processor.h"
#include "general/dump.h"

int main(void){
    const char* byteCodeInputFile = "byteCode.asm";

    processor spu1; 
    spu1.sizeByteCode = getFileSize(byteCodeInputFile);
    spu1.byteCode = (int*) calloc(1, spu1.sizeByteCode);
    assert(spu1.byteCode);

    fileDescription byteCodeFileDes = 
    {
        byteCodeInputFile,
        "rb"
    };
    getIntNumsToBuffer(byteCodeFileDes, spu1.sizeByteCode, &spu1.byteCode);
    
    processorCtor(&spu1);

    runProcessor(&spu1);

    processorDtor(&spu1);
}





