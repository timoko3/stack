#include "ram.h"
#include "poison.h"

#include <stdlib.h>
#include <assert.h>

bool ramCtor(ram_t* ram){
    assert(ram);

    ram = (ram_t*) calloc(RAM_CAPACITY, sizeof(ram_t));

    return true;
}

bool ramDtor(ram_t* ram){
    assert(ram);

    poisonMemory(ram, sizeof(ram));
    free(ram);

    return true;
}