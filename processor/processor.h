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

typedef bool (*calcCommandPtr)(stack*);

bool add(stack* stk);
bool sub(stack* stk);
bool mul(stack* stk);
bool div(stack* stk);
bool out(stack* stk);

struct calcCommand{
    spuCommandsCodes code; 
    calcCommandPtr   command;
};

typedef bool (*registerCommandPtr)(processor*);

bool pushreg(processor* spu);
bool popreg(processor* spu);

struct registerCommand{
    spuCommandsCodes code;
    registerCommandPtr command;
};

enum processorStatus{
    SPU_PROCESS_OK
};

processorStatus processorCtor(processor* spu);
processorStatus processorDtor(processor* spu);
bool runProcessor(processor* spu);

#endif /* PROCESSOR_H */