#include "strFunc.h"

int strCmpSpuCom4(const char* a, const char* b){
    printf("численное представление b: %d\n", *(int*)b);
    return *(int*)a - *(int*)b;
}