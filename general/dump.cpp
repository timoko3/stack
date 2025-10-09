#include "dump.h"

static void simplePrintStack(stack* stk);
static void printByteCode(int* byteCode, size_t byteCodeSize);
static void printRegs(int* regs);

void processorDump(processor* spu){
    assert(spu);

    printf("\nSPU dump:");

    printf("\n\tstack: ");
    simplePrintStack(&spu->stk);
    printf("\n");

    printf("\tCode:");
    printByteCode(spu->byteCode, spu->sizeByteCode);
    printf("\n");

    printf("\tRegs:");
    printRegs(spu->regs);

    getchar();
    printf("\n\n");
}

static void simplePrintStack(stack* stk){
    assert(stk);
    
    for(size_t curStackElem = 0; curStackElem < stk->capacity; curStackElem++){
        if((curStackElem % 4) == 0) printf("\n\t\t");
        printf("%d ", stk->data[curStackElem]);
    }
}

static void printByteCode(int* byteCode, size_t byteCodeSize){
    assert(byteCode);

    // for()

    for(size_t curByte = 0; curByte < byteCodeSize; curByte++){
        if((curByte % 4) == 0) printf(" ");
        if(((curByte % 16) == 0)) printf("\n\t\t");
        printf("%02x", *((unsigned  char*)(byteCode) + curByte));
    }
}

static void printRegs(int* regs){
    assert(regs);
    
    printf("\n\t\t");
    for(size_t curReg = 0; curReg < N_REGISTERS; curReg++){
        printf("%d ", regs[curReg]);
    }
}