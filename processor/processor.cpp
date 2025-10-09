#include "processor.h"
#include "general/litter.h"
#include "general/file.h"

calcCommand calcCommands[]{ 
    {ADD, add},
    {SUB, sub},
    {MUL, mul},
    {DIV, div},
    {OUT, out}
}; 

registerCommand registerCommands[]{
    {PUSHREG, pushreg},
    {POPREG,  popreg}
};

processorStatus processorCtor(processor* spu){
    assert(spu);

    stackCtor(&spu->stk, 10);
    spu->pc = 0;

    return SPU_PROCESS_OK;
}

processorStatus processorDtor(processor* spu){
    assert(spu);

    stackDtor(&spu->stk);

    litterMemory(spu->byteCode, spu->sizeByteCode);
    free(spu->byteCode);
    spu->byteCode = NULL;

    litterMemory(spu->regs, sizeof(spu->regs));
    
    spu->pc = (size_t) rand();
    spu->sizeByteCode = (size_t) rand();

    return SPU_PROCESS_OK;
}

bool runProcessor(processor* spu){
    assert(spu);

    bool result = false;
    while(spu->pc < (spu->sizeByteCode / sizeof(int))){
        if(spu->byteCode[spu->pc] == PUSH){
            spu->pc++;
            stack_t pushParameter = spu->byteCode[spu->pc];

            stackPush(&(spu->stk), pushParameter); 

            return true;
        }

        /// Потом подумать как избавиться от копипаста попробовать избавиться от копипаста
        for(size_t curRegCommandInd = 0; curRegCommandInd < sizeof(registerCommands) / sizeof(registerCommand); curRegCommandInd++){
            if(registerCommands[curRegCommandInd].code == spu->byteCode[spu->pc]){
                registerCommands[curRegCommandInd].command(spu);

                result = true;
                break;
            }
        }

        for(size_t curCommandInd = 0; curCommandInd < sizeof(calcCommands) / sizeof(calcCommand); curCommandInd++){
            if(calcCommands[curCommandInd].code == spu->byteCode[spu->pc]){
                calcCommands[curCommandInd].command(&spu->stk);

                result = true;
                break;
            }
        }

        (spu->pc)++;
    }

    return result;
}

bool add(stack* stk){
    stack_t term1 = 0;
    stack_t term2 = 0;

    stackPop(stk, &term1);
    stackPop(stk, &term2);

    stackPush(stk, term1 + term2);
    
    return true;
}

bool sub(stack* stk){
    stack_t minuend = 0;
    stack_t subtrahend = 0;

    stackPop(stk, &minuend);
    stackPop(stk, &subtrahend);

    stackPush(stk, minuend - subtrahend);

    return true;
}

bool mul(stack* stk){
    stack_t factor1 = 0;
    stack_t factor2 = 0;

    stackPop(stk, &factor1);
    stackPop(stk, &factor2);

    stackPush(stk, factor1 * factor2);

    return true;
}

bool div(stack* stk){
    stack_t dividend = 0;
    stack_t divider = 0;

    stackPop(stk, &dividend);
    stackPop(stk, &divider);

    if(divider == 0){
        printf("Деление на 0 невозможно\n");
        stackPush(stk, divider);
        stackPush(stk, dividend);
        return false;
    }
    stackPush(stk, dividend / divider);

    return true;
}

bool out(stack* stk){
    printf("Все элементы стека:\n");

    stack_t curElem = 0;
    while(stackPop(stk, &curElem) != EMPTY_STACK){
        if(stk->error.type != PROCESS_OK){
            break;
        }

        printf("%d ", curElem);
    }
    printf("\n");

    return true;
}

bool pushreg(processor* spu){
    assert(spu);

    stack_t curReg = spu->regs[spu->byteCode[spu->pc + 1]];
    stackPush(&spu->stk, curReg);

    return true; 
}

bool popreg(processor* spu){
    assert(spu);

    stackPop(&spu->stk, &(spu->regs[spu->byteCode[spu->pc + 1]]));

    return true; 
}