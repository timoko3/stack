#include "calc.h"

commandDescription comands[nCommands]{
    {ADD, add},
    {SUB, sub},
    {MUL, mul},
    {DIV, div},
    {OUT, out}
};

void calculator(stack* stk){
    assert(stk);

    size_t sizeBiteCode = getFileSize(BITE_CODE_FILE_NAME);
    int* biteCode = (int*) calloc(1, sizeBiteCode);

    FILE* biteCodeFile = openInputFile(BITE_CODE_FILE_NAME);

    fread(biteCode, 1, sizeBiteCode, biteCodeFile);
    
    // printf("Объем выделенной памяти: %d\n", malloc_usable_size(biteCode));
    assert(biteCode);

    size_t curBiteCodeArrInd = 0;
    while(curBiteCodeArrInd < (sizeBiteCode / sizeof(int))){
        completeCommand(biteCode, &curBiteCodeArrInd, stk);
        curBiteCodeArrInd++;
    }

    free(biteCode);
}

bool completeCommand(int* biteCode, size_t* curBiteCodeArrInd, stack* stk){
    assert(biteCode);
    assert(stk);

    bool result = false;

    // printf("biteCode[*curBiteCodeArr] = %d; PUSH = %d\n", biteCode[*curBiteCodeArrInd], PUSH);
    if(biteCode[*curBiteCodeArrInd] == PUSH){
        stack_t pushParameter = biteCode[*curBiteCodeArrInd + 1];
        stackPush(stk, pushParameter);

        (*curBiteCodeArrInd)++;
        return true;
    }

    for(size_t curCommandInd = 0; curCommandInd < nCommands; curCommandInd++){
        if(comands[curCommandInd].code == biteCode[*curBiteCodeArrInd]){
            comands[curCommandInd].function(stk);

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