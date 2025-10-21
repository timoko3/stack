#include <stddef.h>

typedef int ram_t;

const size_t RAM_CAPACITY = 100;

bool ramCtor(ram_t* ram);
bool ramDtor(ram_t* ram);