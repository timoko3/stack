#include "strFunc.h"

#include <ctype.h>
#include <assert.h>

// static void myToLower(char* sym);

unsigned long hashStr(const char* str){
    assert(str);
    unsigned long hash = 0;

    int curSym = *str;
    while (curSym != END_COMPARISON_STR) {
        curSym = *(str++);
        hash = ((hash << 5) + hash) ^ (unsigned long) curSym;  // hash * 33 ^ c
    }

    return hash;
}

///////
int cmpHashSpuCom(unsigned long curCommandFromFile, unsigned long curReferenceCommand){
    return (int)(curCommandFromFile - curReferenceCommand);
}

size_t myStrLen(const char* start, char endStr){
    assert(start);

    size_t curSymInd = 0;
    while(start[curSymInd] != endStr) curSymInd++;

    return curSymInd;
}

// static void myToLower(char* sym){
//     assert(sym);

//     if((*sym >= UPPER_SYM_MIN) && (*sym <= UPPER_SYM_MAX)){
//         *sym += UPPER_TO_LOWER_SHIFT;
//     }
    
// }

size_t countStrings(char* buf, size_t fileSize, char endStr){
    assert(buf);

    size_t nStrings = 1;

    size_t curSym = 0;
    while(curSym < fileSize){
        if(buf[curSym] == endStr){
            nStrings++;
        }
        curSym++;
    }

    return nStrings;
}