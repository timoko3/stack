#include "cmd_math.h"
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


bool lt(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){
    if(param1 < param2){
        *result = 1;
        return true;
    }

    *result = 0;
    return false;
}

bool le(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){
    if(param1 <= param2){
        *result = 1;
        return true;
    }

    *result = 0;
    return false;
}

bool gt(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){
    if(param1 > param2){
        *result = 1;
        return true;
    }

    *result = 0;
    return false;
}

bool ge(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){
    if(param1 >= param2){
        *result = 1;
        return true;
    }

    *result = 0;
    return false;
}

bool eq(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){
    if(param1 == param2){
        *result = 1;
        return true;
    }

    *result = 0;
    return false;
}

bool ne(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result){
    if(param1 != param2){
        *result = 1;
        return true;
    }

    *result = 0;
    return false;
}