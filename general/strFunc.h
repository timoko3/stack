#ifndef STR_FUNC_H
#define STR_FUNC_H

#include <ctype.h>
#include <assert.h>
#include <stdio.h>

struct string {
    char* stringPtr;
    size_t len;
};

const int UPPER_SYM_MIN        = 65;
const int UPPER_SYM_MAX        = 90;
const int UPPER_TO_LOWER_SHIFT = 32;
const int SIZE_OF_END_FILE      = 2;

const char END_STR             = '\n';

const char END_COMPARISON_STR  = '\0';

int myStrCmpFromBegin(const char* leftStr, const char* rightStr);
size_t myStrLen(const char* start, char endStr);

int strCmpSpuCom(const char* a, const char* b); ////////////////////

#endif /* STR_FUNC_H */