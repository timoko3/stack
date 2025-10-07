#include "strFunc.h"

#include <ctype.h>
#include <assert.h>

static void myToLower(char* sym);

int strCmpSpuCom(const char* curCommandFromFile, const char* curReferenceCommand){
    return *(int*)curCommandFromFile - *(int*)curReferenceCommand;
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

int countStrings(char* buf, int fileSize, char endStr){
    assert(buf);

    int nStrings = 1;

    int curSym = 0;
    while(curSym < fileSize){
        if(buf[curSym] == endStr){
            nStrings++;
        }
        curSym++;
    }

    return nStrings;
}