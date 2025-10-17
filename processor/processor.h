#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "general/stack/stack.h"
#include "cmdOpcodes.h"
#include "general/strFunc.h" ///  

struct processor{
    stack stk;
    stack funcRetAddr;
    int regs[N_REGISTERS] = {};
    size_t pc; // pointer to command
    buffer_t opcode;
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