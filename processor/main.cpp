#include "processor.h"
#include "general/dump.h"

int main(void){
    processor spu1; //MENTOR
    processorCtor(&spu1);

    processorDump(&spu1);
    while(spu1.pc < (spu1.sizeByteCode / sizeof(int))){
        completeCommand(&spu1);

        spu1.pc++;
    }
    processorDump(&spu1);

    printf("END IS NEAR\n");

    processorDtor(&spu1);
}

