#include "processor.h"
#include "cmd_math.h"

typedef int cmdParam_t;

typedef bool (*handler_t)(processor* spu);

typedef bool (*unaryHandler)(cmdParam_t arg,
                              cmdParam_t* result);
typedef bool (*binaryHandler)(cmdParam_t arg1, cmdParam_t arg2, 
                              cmdParam_t* result);

bool add(processor* spu);
bool sub(processor* spu);
bool mul(processor* spu);
bool div(processor* spu);
bool sqrt(processor* spu);

bool jmp(processor* spu);
bool jb(processor* spu);
bool jbe(processor* spu);
bool ja(processor* spu);
bool jae(processor* spu);
bool je(processor* spu);
bool jne(processor* spu);

bool push(processor* spu);
bool pushreg(processor* spu);
bool popreg(processor* spu);
bool in(processor* spu);
bool out(processor* spu);
bool hlt(processor* spu);
bool callFunc(processor* spu);
bool retFunc(processor* spu);




