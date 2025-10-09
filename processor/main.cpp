#include "processor.h"
#include "general/dump.h"

int main(void){
    processor spu1; 
    
    spu1.sizeByteCode = getFileSize(BYTE_CODE_FILE_NAME);
    spu1.byteCode = (int*) calloc(1, spu1.sizeByteCode);
    assert(spu1.byteCode);

    fileDescription byteCodeFileDes = 
    {
        BYTE_CODE_FILE_NAME,
        "rb"
    };
    getIntNumsToBuffer(byteCodeFileDes, spu1.sizeByteCode, &spu1.byteCode);
    
    processorCtor(&spu1);

    processorDump(&spu1);
    runProcessor(&spu1);
    processorDump(&spu1);

    processorDtor(&spu1);
}





