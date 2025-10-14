#ifndef STR_FUNC_H
#define STR_FUNC_H

#include <stddef.h>

struct string {
    char* stringPtr;
    size_t len;
};

const int UPPER_SYM_MIN        = 'A'; 
const int UPPER_SYM_MAX        = 'Z'; 
const int UPPER_TO_LOWER_SHIFT = 'A' - 'a'; 
const int SIZE_OF_END_FILE     = 1; 

const char END_STR             = '\n';

size_t countStrings(char* buf, size_t fileSize, char endStr);
size_t myStrLen(const char* start, char endStr);

#endif /* STR_FUNC_H */