typedef int stack_t;

#include "stack.h"
#include "calc.h"

#define stackCtor(stk, capacity) if(stackCtor(stk, capacity) != PROCESS_OK) return stk1.error.type
#define stackPush(stk, value)    if(stackPush(stk, value) != PROCESS_OK) return stk1.error.type
#define stackPop(stk, stkElem)   if(stackPop(stk, stkElem) != PROCESS_OK) return stk1.error.type


int main(void){
    stack stk1 = {0};
    calculator(&stk1);
    
    // #ifdef DEBUG
    // size_t capacity = 10;
    // stackCtor(&stk1, capacity);

    // stackPush(&stk1, 10);
    // stackPush(&stk1, 37);
    // stackPush(&stk1, 37);
    // stackPush(&stk1, 37);

    // stack_t stackElem = 0;
    // stackPop(&stk1, &stackElem);
    // stackPop(&stk1, &stackElem);
    // printf("Элемент стека — %d\n", stackElem);
    // #endif

    free(stk1.data);

    return 0;
}