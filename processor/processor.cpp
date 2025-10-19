#include "processor.h"
#include "cmd.h"
#include "general/poison.h"
#include "general/file.h" 

#include <math.h>

static void processorDump(processor* spu);
static void simplePrintStack(stack* stk);
static void printByteCode(int* byteCode, size_t byteCodeSize, size_t pc);
static void printRegs(int* regs);

processorStatus processorCtor(processor* spu){
    assert(spu);
    
    stackCtor(&spu->stk, 10);
    stackCtor(&spu->funcRetAddr, 10);
    spu->pc = 0;
    spu->isWork = true;
    
    return SPU_PROCESS_OK;
}

processorStatus processorDtor(processor* spu){
    assert(spu);

    stackDtor(&spu->stk);
    stackDtor(&spu->funcRetAddr);

    poisonMemory(spu->regs, sizeof(spu->regs));
    
    poisonMemory(&spu->pc, sizeof(spu->pc));

    spu->isWork = false;

    return SPU_PROCESS_OK;
}

bool loadOpcode(processor* spu, buffer_t opcode){
    assert(spu);

    spu->opcode = opcode;

    return true;
}

bool runProcessor(processor* spu){
    assert(spu);

    bool result = false;
    while(spu->pc < (spu->opcode.size)){
        if(executeCommand(spu) == false) return false;
    }

    return result;
}

bool executeCommand(processor* spu){
    assert(spu);
    
    processorDump(spu);

    if(spu->isWork == false){
        return false;
    }

    for(size_t curCommandInd = 0; curCommandInd < sizeof(commandsHandler) / sizeof(command); curCommandInd++){
        if(commandsHandler[curCommandInd].code == spu->opcode.ptr[spu->pc]){
            command curCmd = commandsHandler[curCommandInd];

            assert(curCmd.handler);
            size_t old_ps = spu->pc;
            ///
            curCmd.handler(spu);
            // return_value == if command don't change
            if (spu->pc == old_ps) {
                (spu->pc)++;
                spu->pc += (size_t) curCmd.nArgs;
            }
            
            break;
        } 
    } 

    return true;
}

bool spuPush(processor* spu, stackData_t data){
    assert(spu);

    stackPush(&(spu->stk), data); 

    return true;
}

bool spuPop(processor* spu, stackData_t* data){
    assert(spu);

    if(stackPop(&(spu->stk), data) == EMPTY_STACK){
        return false;
    }

    return true;
}

bool spuJump(processor* spu, stackData_t pos){
    assert(spu);

    spu->pc = (size_t) pos; 

    return true;
}

bool spuGetArg(processor* spu, stackData_t* index){ 
    assert(spu);

    *index = (size_t) spu->opcode.ptr[spu->pc + 1];

    return true;
}  

bool spuPushReg(processor* spu, stackData_t* regNumber){
    assert(spu);
    assert(regNumber);

    int curRegValue = spu->regs[*regNumber];
    stackPush(&spu->stk, curRegValue);

    return true;
}

bool spuPopReg(processor* spu, stackData_t* regNumber){
    assert(spu);
    assert(regNumber);

    stackPop(&spu->stk, &spu->regs[*regNumber]);

    return true;
}

bool spuHlt(processor* spu){
    assert(spu);

    spu->isWork = false;

    return true;
}

bool spuCall(processor* spu){
    assert(spu);

    stackPush(&(spu->funcRetAddr), (stackData_t) spu->pc + 2);

    stackData_t pos = 0;
    spuGetArg(spu, &pos);

    spuJump(spu, pos);
    
    return true;
}

bool spuRet(processor* spu){
    assert(spu);

    stackData_t regNum = 0;
    spuGetArg(spu, &regNum);
    spuPopReg(spu, &regNum);

    stackData_t retAddr = 0;
    stackPop(&(spu->funcRetAddr), &retAddr);
    spu->pc = (size_t) retAddr;

    return true;
}

static void processorDump(processor* spu){
    assert(spu);

    printf("\nSPU dump:");

    printf("\n\tpc:     ");
    printf(SET_STYLE_BOLD_FONT_PURPLE "%lu" RESET, spu->pc);

    printf("\n\tstack: ");
    simplePrintStack(&spu->stk);
    printf("\n");

    printf("\n\tretAddrStack: ");
    simplePrintStack(&spu->funcRetAddr);
    printf("\n");

    printf("\tCode:");
    printByteCode(spu->opcode.ptr, spu->opcode.size, spu->pc);
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

        if(stk->data[curStackElem] == POISON_NUMBER){
            printf(SET_STYLE_ITALICS_FONT_TURQUOISE "POISON " RESET);
        }
        else {
            printf("%d ", stk->data[curStackElem]);
        }
    }
}

static void printByteCode(int* byteCode, size_t byteCodeSize, size_t pc){
    assert(byteCode);

    printf("\n\t\t " SET_STYLE_BOLD_FONT_RED "%08x %08x %08x %08x" RESET, (unsigned int) 0,
                                                                          (unsigned int) 1,
                                                                          (unsigned int) 2,
                                                                          (unsigned int) 3);
    for(size_t curByte = 0; curByte < byteCodeSize; curByte++){
        if((curByte % 1) == 0) printf(" ");
        if(((curByte % 4) == 0)) printf("\n\t" SET_STYLE_BOLD_FONT_RED "%08x " RESET, (unsigned int) curByte);
        if((curByte == pc)){
            printf(SET_STYLE_BOLD_FONT_PURPLE "%08x" RESET, (unsigned int) *(byteCode + curByte));
        }
        else{
            printf(SET_STYLE_BOLD_FONT_YELLOW "%08x" RESET, (unsigned int) *(byteCode + curByte));
        }
    }
}
ЭТО ИЗ-ЗА СВЕТЛОЙ ТЕМЫ
static void printRegs(int* regs){
    assert(regs);
    
    printf("\n\t\t");
    for(size_t curReg = 0; curReg < N_REGISTERS; curReg++){
        printf("%d ", regs[curReg]);
    }
}