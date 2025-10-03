#include "calc.h"

#define stackCtor(stk, capacity) if(stackCtor(stk, capacity) != PROCESS_OK) return stk1.error.type
#define stackPush(stk, value)    if(stackPush(stk, value)    != PROCESS_OK) return stk1.error.type
#define stackPop(stk, stkElem)   if(stackPop(stk, stkElem)   != PROCESS_OK) return stk1.error.type

int main(void){
    stack stk1 = {0};
    stackCtor(&stk1, 10);

    calculator(&stk1);
    
    stackDtor(&stk1);

    return 0;
}