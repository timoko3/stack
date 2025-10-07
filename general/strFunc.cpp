#include "strFunc.h"

#define PLUS true
#define MINUS false

int strCmpSpuCom(const char* a, const char* b){
    return *(int*)a - *(int*)b;
}

static void myToLower(char* sym);
static void skipPunct(int* curSym, const char* str, bool sign);

int myStrCmpFromBegin(const char* leftStr, const char* rightStr){
    assert(leftStr);
    assert(rightStr);
    
    int result = 0;
    int leftCurSymInd = 0, rightCurSymInd = 0;
    while(true){
        
        skipPunct(&leftCurSymInd, leftStr, PLUS);
        skipPunct(&rightCurSymInd, rightStr, PLUS);

        char leftCurSym = leftStr[leftCurSymInd];
        char rightCurSym = rightStr[rightCurSymInd];

        myToLower(&leftCurSym);
        myToLower(&rightCurSym);

        if(leftCurSym == END_COMPARISON_STR && rightCurSym == END_COMPARISON_STR){
             result = 0;
             break;
        }
        else if(leftCurSym != rightCurSym){
            result = leftCurSym - rightCurSym;
            break;
        }
        
        leftCurSymInd++;
        rightCurSymInd++;
    }

    return result;
}

size_t myStrLen(const char* start, char endStr){
    assert(start);

    size_t curSymInd = 0;
    while(start[curSymInd] != endStr) curSymInd++;

    return curSymInd;
}

static void myToLower(char* sym){
    assert(sym);

    if((*sym >= UPPER_SYM_MIN) && (*sym <= UPPER_SYM_MAX)){
        *sym += UPPER_TO_LOWER_SHIFT;
    }
    
}

static void skipPunct(int* curSymInd, const char* str, bool sign){
    assert(curSymInd);
    assert(str);

    while(true){
        if(ispunct(str[*curSymInd])){
            if(sign){
                (*curSymInd)++;
            }
            else{
                (*curSymInd)--;
            }
        }
        else{
            break;
        }
    }

}