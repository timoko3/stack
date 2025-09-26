typedef int stack_t;

#include "stack.h"

#define stackCtor(stk, capacity) if(stackCtor(stk, capacity) != PROCESS_OK) return stk1.error
#define stackPush(stk, value)    if(stackPush(stk, value) != PROCESS_OK) return stk1.error
#define stackPop(stk, stkElem)   if(stackPop(stk, stkElem) != PROCESS_OK) return stk1.error

int main(void){
    stack stk1 = {};
    size_t capacity = 5;
    
    stackCtor(&stk1, capacity);

    stackPush(&stk1, 10);
    stackPush(&stk1, 37);

    // stackDump(&stk1);

    stack_t stackElem = 0;
    stackPop(&stk1, &stackElem);
    
    // stackDump(&stk1);
    
    free(stk1.data);

    return 0;
}