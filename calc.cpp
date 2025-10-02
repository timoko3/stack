#include "calc.h"

commandDescription comands[nCommands]{
    {"ADD", add},
    {"SUB", sub},
    {"MUL", mul},
    {"DIV", div},
    {"OUT", out}
};

void calculator(stack* stk){
    assert(stk);

    DataFromInputFIle calcCommands;
    stringsFromFileToStructure(&calcCommands);

    for(int curString = 0; curString < calcCommands.nStrings; curString++){
        char curCommand[20] = {0};
        sscanf(calcCommands.strings[curString].stringPtr, "%s", curCommand);

        #ifdef DEBUG
        printf("%d команда: %s\n",curString, curCommand);
        #endif /* DEBUG */

        completeCommand(curCommand, stk, &calcCommands.strings[curString]);
    }

    free(calcCommands.buffer);
    free(calcCommands.strings);
}

static bool completeCommand(char* curCommand, stack* stk, string* commandBuffer){
    assert(curCommand);

    bool result = false;

    #ifdef DEBUG
    for(size_t i = 0; i < myStrLen(curCommand, '\0'); i++){
        printf("%ld символ - %d\n", i, curCommand[i]);
    }
    #endif /* DEBUG */

    if(!myStrCmpFromBegin("PUSH", curCommand)){
        stack_t pushParameter = 0;
        sscanf(commandBuffer->stringPtr, "%*s %d", &pushParameter);

        stackPush(stk, pushParameter);
    }

    for(size_t curCommandInd = 0; curCommandInd < nCommands; curCommandInd++){
        if(!myStrCmpFromBegin(comands[curCommandInd].name, curCommand)){
            comands[curCommandInd].function(stk);

            #ifdef DEBUG
            printf("Сработала команда %s\n", comands[curCommandInd].name);
            #endif /* DEBUG */

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
    while(stackPop(stk, &curElem) != POP_WITH_BAD_SIZE){
        if(stk->error.type != PROCESS_OK){
            break;
        }
        printf("%d ", curElem);
    }
    printf("\n");

    return true;
}