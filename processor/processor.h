#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "general/stack/stack.h"
#include "cmdOpcodes.h"

struct processor{
    stack stk;
    stack funcRetAddr;
    int regs[N_REGISTERS] = {};
    size_t pc; // pointer to command
    int* byteCode;
    size_t sizeByteCode;
};

enum processorStatus{
    SPU_PROCESS_OK
};

bool getOpcodeBuffer(processor* spu, const char* fileName);
processorStatus processorCtor(processor* spu);
processorStatus processorDtor(processor* spu);
bool runProcessor(processor* spu);
bool executeCommand(processor* spu);

#endif /* PROCESSOR_H */