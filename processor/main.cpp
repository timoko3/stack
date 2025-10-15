#include "processor.h"
#include "general/dump.h"

int main(void){
    const char* byteCodeInputFile = "factorial.asm";

    processor spu1; 
    processorCtor(&spu1, byteCodeInputFile);

    runProcessor(&spu1);

    processorDtor(&spu1);
}





