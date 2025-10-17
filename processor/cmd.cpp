#include "cmd.h"
#include <stdio.h>
#include <limits.h>
#include <math.h>

bool add(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){
    *result = param1 + param2;

    return true;
}

bool sub(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){   
    *result = param1 - param2;

    return true;
}

bool mul(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){  
    *result = param1 * param2;

    return true;
}

bool div(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){  
    if(param2 == 0){
        printf("Деление на 0 невозможно\n");
        return false;
    }
    
    *result = param1 / param2;

    return true;
}

bool sqrt(cmdParam_t param, cmdParam_t* result){
    if(param < 0){
        printf("Взятие корня из числа меньше 0 невозможно\n");
        return false;
    }
    
    *result = (cmdParam_t) sqrt(param);

    return true;
}