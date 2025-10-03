#ifndef STR_FUNC_H
#define STR_FUNC_H

#include <ctype.h>
#include <assert.h>
#include <stdio.h>

struct string {
    char* stringPtr;
    size_t len;
};

const int SIZE_OF_END_STR = 2;
const char END_STR = '\n';

int strCmpSpuCom(const char* a, const char* b);

#endif /* STR_FUNC_H */