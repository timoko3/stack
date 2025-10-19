#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "general/stack/stack.h"
#include "cmdOpcodes.h"
#include "general/strFunc.h" 

struct processor{
    stack stk;
    stack funcRetAddr;
    int regs[N_REGISTERS] = {};
    size_t pc; 
    buffer_t opcode;
};

enum processorStatus{
    SPU_PROCESS_OK,
    SPU_PROCESS_ERROR
};

bool spuPush(processor* spu, stackData_t data);
bool spuPop(processor* spu, stackData_t* data);
bool spuJump(processor* spu, stackData_t data);
bool spuGetArg(processor* spu, stackData_t* data);

processorStatus processorCtor(processor* spu);
processorStatus processorDtor(processor* spu);
bool loadOpcode(processor* spu, buffer_t opcode);
bool runProcessor(processor* spu);
bool executeCommand(processor* spu);

#endif /* PROCESSOR_H */