typedef int stack_t;

#include "stack.h"

#define stackCtor(stk, capacity) if(stackCtor(stk, capacity) != PROCESS_OK) return stk1.error.type
#define stackPush(stk, value)    if(stackPush(stk, value) != PROCESS_OK) return stk1.error.type
#define stackPop(stk, stkElem)   if(stackPop(stk, stkElem) != PROCESS_OK) return stk1.error.type
#define function __FUNCTION__
#define line __LINE__
#define file __FILE__

int main(void){
    stack stk1 = {0};
    size_t capacity = 10;
    
    stackCtor(&stk1, capacity);

    stackPush(&stk1, 10);
    stackPush(&stk1, 37);
    stackPush(&stk1, 37);
    stackPush(&stk1, 37);
    

    // dump(&stk1);

    stack_t stackElem = 0;
    stackPop(&stk1, &stackElem);
    stackPop(&stk1, &stackElem);

    printf("Элемент стека — %d\n", stackElem);
    // stackDump(&stk1);
    
    free(stk1.data);

    return 0;
}