#include "calc.h"

calcCommandsDescription calcCommands[]{ // N_CALC_COMMANDS
    {ADD, add},
    {SUB, sub},
    {MUL, mul},
    {DIV, div},
    {OUT, out}
}; 

registerCommandsDescription registerCommands[N_REGISTER_COMMANDS]{
    {PUSHREG, pushreg},
    {POPREG,  popreg}
};

void spu(stack* stk){
    assert(stk);

    processor spu1 = {0}; /// ?
    int regs[10];
    size_t sizeByteCode = getFileSize(BITE_CODE_FILE_NAME);
    int* byteCode = (int*) calloc(1, sizeByteCode);
    assert(byteCode);

    FILE* byteCodeFile = openInputFile(BITE_CODE_FILE_NAME);
    assert(byteCodeFile);

    fread(byteCode, 1, sizeByteCode, byteCodeFile);
    fclose(byteCodeFile);

    // readTextFromFile

    spu1.byteCode = byteCode;
    spu1.stk = *stk;
    spu1.pc = 0;

    while(spu1.pc < (sizeByteCode / sizeof(int))){
        completeCommand(&spu1);
        spu1.pc++;
    }
    printf("END IS NEAR\n");
    free(byteCode);
}

bool completeCommand(processor* spu){
    assert(spu);

    bool result = false;

    if(spu->byteCode[spu->pc] == PUSH){
        spu->pc++;
        stack_t pushParameter = spu->byteCode[spu->pc];
        spu->pc++;
        stackPush(&(spu->stk), pushParameter); 

        (spu->pc)++;
        return true;
    }

    for(size_t curCommandInd = 0; curCommandInd < N_CALC_COMMANDS; curCommandInd++){
        if(calcCommands[curCommandInd].code == spu->byteCode[spu->pc]){
            calcCommands[curCommandInd].function(&spu->stk);

            result = true;
            break;
        }
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
    stack_t curReg = spu->regs[spu->byteCode[spu->pc + 1]];
    stackPush(&spu->stk, curReg);

    return true; 
}

bool popreg(processor* spu){
    stack_t curReg = spu->regs[spu->byteCode[spu->pc + 1]];
    stackPop(&spu->stk, &curReg);

    return true; 
}