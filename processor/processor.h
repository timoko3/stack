#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack/stack.h"
#include "translator/translator.h"


struct processor{
    stack stk;
    stack funcRetAddr;
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
bool sqrt(processor* spu);
bool out(processor* spu);
bool hlt(processor* spu);
bool jmp(processor* spu);
bool jmpCond(processor* spu);
bool push(processor* spu);
bool pushreg(processor* spu);
bool popreg(processor* spu);
bool callFunc(processor* spu);
bool returnFunc(processor* spu);
// args...

struct command{
    spuCommandsCodes code; 
    commandPtr       ptr;
};

enum processorStatus{
    SPU_PROCESS_OK
};

processorStatus processorCtor(processor* spu, const char* fileName);
processorStatus processorDtor(processor* spu);
bool runProcessor(processor* spu);
bool executeCommand(processor* spu);

#endif /* PROCESSOR_H */