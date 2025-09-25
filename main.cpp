typedef int stack_t;

#include "stack.h"

stack_t main(void){
    stack stk1 = {};
    size_t capacity = 100000;
    
    stackCtor(&stk1, capacity);

    stackPush(&stk1, 10);
    stackPush(&stk1, 37);

    // stackDump(&stk1);

    stack_t stackElem = 0;
    stackPop(&stk1, &stackElem);
    stackPop(&stk1, &stackElem);

    // stackDump(&stk1);
    
    free(stk1.data);
}