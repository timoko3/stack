#include "processor.h"
#include "general/litter.h"
#include "general/file.h"
#include "general/dump.h"

#include <math.h>

command commands[]{ 
    {ADD,     add},
    {SUB,     sub},
    {MUL,     mul},
    {DIV,     div},
    {OUT,     out},
    {PUSH,    push},
    {JMP,     jmpCond},
    {JB,      jmpCond},
    {JBE,     jmpCond},
    {JA,      jmpCond},
    {JAE,     jmpCond},
    {JE,      jmpCond},
    {JNE,     jmpCond},
    {PUSHREG, pushreg},
    {POPREG,  popreg}
}; 

processorStatus processorCtor(processor* spu, const char* fileName){
    assert(spu);

    spu->sizeByteCode = getFileSize(fileName);
    spu->byteCode = (int*) calloc(1, spu->sizeByteCode);
    assert(spu1.byteCode);

    fileDescription byteCodeFileDes = {
        fileName,
        "rb"
    };
    getIntNumsToBuffer(byteCodeFileDes, spu->sizeByteCode, &spu->byteCode);

    stackCtor(&spu->stk, 10);
    spu->pc = 0;

    return SPU_PROCESS_OK;
}

bool runProcessor(processor* spu){
    assert(spu);

    bool result = false;
    while(spu->pc < (spu->sizeByteCode / sizeof(int))){
        executeCommand(spu);
        (spu->pc)++;
    }

    return result;
}

bool executeCommand(processor* spu){
    for(size_t curCommandInd = 0; curCommandInd < sizeof(commands) / sizeof(command); curCommandInd++){
        if(commands[curCommandInd].code == spu->byteCode[spu->pc]){
            commands[curCommandInd].ptr(spu);

            processorDump(spu);
            break;
        } 
    }
    return true;
}

processorStatus processorDtor(processor* spu){
    assert(spu);

    stackDtor(&spu->stk);

    poisonMemory(spu->byteCode, spu->sizeByteCode);
    free(spu->byteCode);
    spu->byteCode = NULL;

    poisonMemory(spu->regs, sizeof(spu->regs));
    
    spu->pc = (size_t) rand();
    spu->sizeByteCode = (size_t) rand();

    return SPU_PROCESS_OK;
}

bool add(processor* spu){
    assert(spu);

    stack_t term1 = 0;
    stack_t term2 = 0;

    stackPop(&spu->stk, &term1);
    stackPop(&spu->stk, &term2);

    stackPush(&spu->stk, term1 + term2);
    
    return true;
}

bool sub(processor* spu){   
    assert(spu);

    stack_t minuend = 0;
    stack_t subtrahend = 0;

    stackPop(&spu->stk, &minuend);
    stackPop(&spu->stk, &subtrahend);

    stackPush(&spu->stk, minuend - subtrahend);

    return true;
}

bool mul(processor* spu){
    assert(spu);

    stack_t factor1 = 0;
    stack_t factor2 = 0;

    stackPop(&spu->stk, &factor1);
    stackPop(&spu->stk, &factor2);

    stackPush(&spu->stk, factor1 * factor2);

    return true;
}

bool div(processor* spu){
    assert(spu);

    stack_t dividend = 0;
    stack_t divider = 0;

    stackPop(&spu->stk, &dividend);
    stackPop(&spu->stk, &divider);

    if(divider == 0){
        printf("Деление на 0 невозможно\n");
        stackPush(&spu->stk, divider);
        stackPush(&spu->stk, dividend);
        return false;
    }
    stackPush(&spu->stk, dividend / divider);

    return true;
}

bool sqrt(processor* spu){
    assert(spu);

    stack_t radicalExpression = 0;

    stackPop(&spu->stk, &radicalExpression);

    if(radicalExpression < 0){
        printf("Взятие корня из числа меньше 0 невозможно\n");
        stackPush(&spu->stk, radicalExpression);
        return false;
    }
    stackPush(&spu->stk, (stack_t) sqrt(radicalExpression));

    return true;
}

bool out(processor* spu){
    assert(spu);

    printf("Все элементы стека:\n");

    stack_t curElem = 0;
    while(stackPop(&spu->stk, &curElem) != EMPTY_STACK){
        if(spu->stk.error.type != PROCESS_OK){
            break;
        }

        printf("%d ", curElem);
    }
    printf("\n");

    return true;
}

bool jmp(processor* spu){
    assert(spu);

    spu->pc = (size_t) spu->byteCode[spu->pc + 1];

    return true;
}

bool jmpCond(processor* spu){
    assert(spu);

    stack_t superiorStackElem = 0;
    stack_t preSuperiorStackElem = 0;

    stackPop(&spu->stk, &superiorStackElem);
    stackPop(&spu->stk, &preSuperiorStackElem);    

    int jumpSign = spu->byteCode[spu->pc];

    bool doJump = false;
    switch(jumpSign){
        case JB : if(superiorStackElem <  preSuperiorStackElem)doJump = true;  break;  
        case JBE: if(superiorStackElem <= preSuperiorStackElem)doJump = true;  break;  
        case JA : if(superiorStackElem >  preSuperiorStackElem)doJump = true;  break;
        case JAE: if(superiorStackElem >= preSuperiorStackElem)doJump = true;  break; 
        case JE : if(superiorStackElem == preSuperiorStackElem)doJump = true;  break; 
        case JNE: if(superiorStackElem != preSuperiorStackElem)doJump = true;  break; 
        default : doJump = false;
    }
    
    if(doJump){
        spu->pc = (size_t) spu->byteCode[spu->pc + 1];
    }

    return true;
}

bool push(processor* spu){
    assert(spu);

    stack_t pushParameter = spu->byteCode[spu->pc + 1];
    stackPush(&(spu->stk), pushParameter); 

    spu->pc++;

    return true;
}

bool pushreg(processor* spu){
    assert(spu);

    int curReg = spu->regs[spu->byteCode[spu->pc + 1]];
    printf("curReg : %d\n", curReg);
    stackPush(&spu->stk, curReg);

    (spu->pc)++;

    return true; 
}

bool popreg(processor* spu){
    assert(spu);

    stackPop(&spu->stk, &(spu->regs[spu->byteCode[spu->pc + 1]]));

    (spu->pc)++;

    return true; 
}