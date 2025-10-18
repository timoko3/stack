#include "processor.h"

bool push(processor* spu);
bool out(processor* spu);
bool pushreg(processor* spu);
bool popreg(processor* spu);
bool hlt(processor* spu);
bool jmp(processor* spu);
bool jmpCond(processor* spu);

bool callFunc(processor* spu);
bool returnFunc(processor* spu);

