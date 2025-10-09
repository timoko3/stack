#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack/stack.h"
#include "translator/translator.h"

const size_t N_REGISTERS = 10;
struct processor{
    stack stk;
    int regs[N_REGISTERS] = {};
    size_t pc; // pointer to command
    int* byteCode;
    size_t sizeByteCode;
};

typedef bool (*commandPtr)(processor*);

bool add(processor* spu);
bool sub(processor* spu);
bool mul(processor* spu);
bool div(processor* spu);
bool out(processor* spu);
bool push(processor* spu);
bool pushreg(processor* spu);
bool popreg(processor* spu);

struct command{
    spuCommandsCodes code; 
    commandPtr       ptr;
};

enum processorStatus{
    SPU_PROCESS_OK
};

processorStatus processorCtor(processor* spu);
processorStatus processorDtor(processor* spu);
bool runProcessor(processor* spu);

#endif /* PROCESSOR_H */