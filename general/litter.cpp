#include "litter.h"

void poisonMemory(void* ptr, size_t sizeToPollute){
    assert(ptr);
    char* toPollute = (char*) ptr;

    for(size_t i = 0; i < sizeToPollute; i++){
        *(toPollute + i) = (char) (rand() % 255);
    }
}