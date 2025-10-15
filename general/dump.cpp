#include "dump.h"
#include "genConsoleColors/consoleColors.h"

static void simplePrintStack(stack* stk);
static void printByteCode(int* byteCode, size_t byteCodeSize, size_t pc);
static void printRegs(int* regs);

void processorDump(processor* spu){
    assert(spu);

    printf("\nSPU dump:");

    printf("\n\tpc: %lu", spu->pc);

    printf("\n\tstack: ");
    simplePrintStack(&spu->stk);
    printf("\n");

    printf("\n\tretAddrStack: ");
    simplePrintStack(&spu->funcRetAddr);
    printf("\n");

    printf("\tCode:");
    printByteCode(spu->byteCode, spu->sizeByteCode, spu->pc);
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

static void printByteCode(int* byteCode, size_t byteCodeSize, size_t pc){
    assert(byteCode);

    // for()

    for(size_t curByte = 0; curByte < byteCodeSize; curByte++){
        if((curByte % 4) == 0) printf(" ");
        if(((curByte % 16) == 0)) printf("\n\t\t");
        if((curByte == pc * 4)){
            printf(SET_STYLE_BOLD_FONT_PURPLE "%02x" RESET, *((unsigned  char*)(byteCode) + curByte));
        }
        else{
            printf(SET_STYLE_BOLD_FONT_YELLOW "%02x" RESET, *((unsigned  char*)(byteCode) + curByte));
        }
    }
}

static void printRegs(int* regs){
    assert(regs);
    
    printf("\n\t\t");
    for(size_t curReg = 0; curReg < N_REGISTERS; curReg++){
        printf("%d ", regs[curReg]);
    }
}