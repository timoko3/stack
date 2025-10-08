#include "processor.h"
#include "general/litter.h"

static processorErr getInfoFromByteCodeFile(processor* spu);

processorCommandsDescription calcCommands[]{ // N_CALC_COMMANDS
    {ADD, add},
    {SUB, sub},
    {MUL, mul},
    {DIV, div},
    {OUT, out}
}; 

registerCommandsDescription registerCommands[]{
    {PUSHREG, pushreg},
    {POPREG,  popreg}
};

processorErr processorCtor(processor* spu){
    assert(spu);

    stack stackSpu = {0};
    stackCtor(&stackSpu, 10);
    spu->stk = stackSpu;
    getInfoFromByteCodeFile(spu);

    spu->pc = 0;

    return SPU_PROCESS_OK;
}

static processorErr getInfoFromByteCodeFile(processor* spu){
    assert(spu);

    size_t sizeByteCode = getFileSize(BYTE_CODE_FILE_NAME);
    spu->byteCode = (int*) calloc(1, sizeByteCode);
    assert(spu->byteCode);

    spu->sizeByteCode = sizeByteCode;

    fileDescription byteCodeFileDes = 
    {
        BYTE_CODE_FILE_NAME,
        "rb"
    };

    FILE* byteCodeFile = myOpenFile(&byteCodeFileDes);
    assert(byteCodeFile);

    fread(spu->byteCode, 1, sizeByteCode, byteCodeFile);

    fclose(byteCodeFile);   

    return SPU_PROCESS_OK;
}

processorErr processorDtor(processor* spu){
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

bool completeCommand(processor* spu){
    assert(spu);

    bool result = false;
    
    if(spu->byteCode[spu->pc] == PUSH){
        spu->pc++;
        stack_t pushParameter = spu->byteCode[spu->pc];

        stackPush(&(spu->stk), pushParameter); 

        return true;
    }
    // else if(spu->byteCode[spu->pc] == POP){


    // }

    for(size_t curRegCommandInd = 0; curRegCommandInd < N_REGISTER_COMMANDS; curRegCommandInd++){
        if(registerCommands[curRegCommandInd].code == spu->byteCode[spu->pc]){
            registerCommands[curRegCommandInd].function(spu);

            result = true;
            break;
        }
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