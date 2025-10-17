#include "processor.h"
#include "cmd.h"
#include "general/poison.h"
#include "general/file.h"

#include <math.h>

static bool unaryCommand(processor* spu, unaryCommandPtr handler);
static bool binaryCommand(processor* spu, binaryCommandPtr handler);

static void processorDump(processor* spu);
static void simplePrintStack(stack* stk);
static void printByteCode(int* byteCode, size_t byteCodeSize, size_t pc);
static void printRegs(int* regs);

// static bool sqrt(processor* spu);
// static bool out(processor* spu);
// static bool hlt(processor* spu);
// static bool jmp(processor* spu);
// static bool jmpCond(processor* spu);
// static bool push(processor* spu);
// static bool pushreg(processor* spu);
// static bool popreg(processor* spu);
// static bool callFunc(processor* spu);
// static bool returnFunc(processor* spu);

// bool JBcmp(int arg1, int arg2) { return arg1 <  arg2; }
// bool JBcmp(int arg1, int arg2) { return arg1 <= arg2; }
// bool JBcmp(int arg1, int arg2) { return arg1 >  arg2; }
// bool JBcmp(int arg1, int arg2) { return arg1 >= arg2; }
// bool JBcmp(int arg1, int arg2) { return arg1 == arg2; }
// bool JBcmp(int arg1, int arg2) { return arg1 != arg2; }
// jmpJB(...){ jmpCond (JBcond); }

bool getOpcodeBuffer(processor* spu, const char* fileName){
    assert(spu);
    assert(fileName);

    spu->opcode.size = getFileSize(fileName) / sizeof(int);
    spu->opcode.ptr = (int*) calloc(spu->opcode.size, sizeof(int));
    assert(spu->opcode.ptr);

    fileDescription byteCodeFileDes = {
        fileName,
        "rb"
    };
    getIntNumsToBuffer(byteCodeFileDes, spu->opcode.size * sizeof(int), &spu->opcode.ptr);

    return true;
}

processorStatus processorCtor(processor* spu){
    assert(spu);

    stackCtor(&spu->stk, 10);
    stackCtor(&spu->funcRetAddr, 10);
    spu->pc = 0;

    return SPU_PROCESS_OK;
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

    for(size_t curCommandInd = 0; curCommandInd < sizeof(commandsHandler) / sizeof(command); curCommandInd++){
        if(commandsHandler[curCommandInd].code == spu->opcode.ptr[spu->pc]){
            command curCmd = commandsHandler[curCommandInd];
            switch(curCmd.type){
                case BINARY:   binaryCommand(spu, curCmd.handler.binaryHandler); break;
                case UNARY:    unaryCommand(spu,  curCmd.handler.unaryHandler); break;
                default: break;
            }
            
            break;
        } 
    } 
    (spu->pc)++;

    return true;
}

processorStatus processorDtor(processor* spu){
    assert(spu);

    stackDtor(&spu->stk);
    stackDtor(&spu->funcRetAddr);

    poisonMemory(spu->opcode.ptr, spu->opcode.size);
    free(spu->opcode.ptr);
    spu->opcode.ptr = NULL;

    //
    spu->opcode.size = (size_t) rand();

    poisonMemory(&spu->opcode, sizeof(buffer_t));
    poisonMemory(spu->regs, sizeof(spu->regs));
    //
    spu->pc = (size_t) rand();

    return SPU_PROCESS_OK;
}

static bool unaryCommand(processor* spu, unaryCommandPtr handler){
    assert(spu);
    
    stackData_t param = 0;
    
    stackPop(&spu->stk, &param);

    stackData_t result = 0;
    bool check = handler(param, &result);

    stackPush(&spu->stk, result);

    return check;
}   

static bool binaryCommand(processor* spu, binaryCommandPtr handler){
    assert(spu);
    
    stackData_t param1 = 0;
    stackData_t param2 = 0;
    
    stackPop(&spu->stk, &param2);
    stackPop(&spu->stk, &param1);

    stackData_t result = 0;
    bool check = handler(param1, param2, &result);

    stackPush(&spu->stk, result);

    return check;
}   

// static bool out(processor* spu){
//     assert(spu);

//     printf("Все элементы стека:\n");

//     stackData_t curElem = 0;
//     while(stackPop(&spu->stk, &curElem) != EMPTY_STACK){
//         if(spu->stk.error.type != PROCESS_OK){
//             break;
//         }

//         printf("%d ", curElem);
//     }
//     printf("\n");

//     return true;
// }

// static bool hlt(processor* spu){
//     assert(spu);

//     return false;
// }

// static bool jmp(processor* spu){
//     assert(spu);

//     spu->pc = (size_t) spu->opcode.ptr[spu->pc + 1];

//     return true;
// }

// static bool jmpCond(processor* spu){
//     assert(spu);

//     stackData_t superiorStackElem = 0;
//     stackData_t preSuperiorStackElem = 0;

//     stackPop(&spu->stk, &superiorStackElem);
//     stackPop(&spu->stk, &preSuperiorStackElem);    

//     int jumpSign = spu->opcode.ptr[spu->pc];

//     bool doJump = false;
//     switch(jumpSign){
//         // case JB : doJump = JBcond(arg1, arg2);  break;
//         case JB : if(superiorStackElem <  preSuperiorStackElem) doJump = true;  break;  
//         case JBE: if(superiorStackElem <= preSuperiorStackElem) doJump = true;  break;  
//         case JA : if(superiorStackElem >  preSuperiorStackElem) doJump = true;  break;
//         case JAE: if(superiorStackElem >= preSuperiorStackElem) doJump = true;  break; 
//         case JE : if(superiorStackElem == preSuperiorStackElem) doJump = true;  break; 
//         case JNE: if(superiorStackElem != preSuperiorStackElem) doJump = true;  break; 
//         default : doJump = false;
//     }
    
//     if(doJump){
//         spu->pc = (size_t) spu->opcode.ptr[spu->pc + 1];
//     }
//     else{
//         (spu->pc)++;
//     }

//     return true;
// }

// static bool callFunc(processor* spu){
//     assert(spu);

//     stackPush(&(spu->funcRetAddr), (stackData_t) spu->pc + 1);
//     spu->pc = (size_t) spu->opcode.ptr[spu->pc + 1];
    
//     return true;
// }

// static bool returnFunc(processor* spu){
//     assert(spu);

//     popreg(spu);

//     stackData_t retAddr = 0;
//     stackPop(&(spu->funcRetAddr), &retAddr);
//     spu->pc = (size_t) retAddr;

//     return true;
// }

// static bool push(processor* spu){
//     assert(spu);

//     stackData_t pushParameter = spu->opcode.ptr[spu->pc + 1];
//     stackPush(&(spu->stk), pushParameter); 
//     spu->pc++;

//     processorDump(spu);

//     return true;
// }

// static bool pushreg(processor* spu){
//     assert(spu);

//     int curReg = spu->regs[spu->opcode.ptr[spu->pc + 1]];
//     printf("curReg : %d\n", curReg);
//     stackPush(&spu->stk, curReg);

//     (spu->pc)++;

//     return true; 
// }

// static bool popreg(processor* spu){
//     assert(spu);

//     stackPop(&spu->stk, &(spu->regs[spu->opcode.ptr[spu->pc + 1]]));

//     (spu->pc)++;

//     return true; 
// }

static void processorDump(processor* spu){
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
        printf("%d ", stk->data[curStackElem]);
    }
}

static void printByteCode(int* byteCode, size_t byteCodeSize, size_t pc){
    assert(byteCode);

    for(size_t curByte = 0; curByte < byteCodeSize * sizeof(int); curByte++){
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