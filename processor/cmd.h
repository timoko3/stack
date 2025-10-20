#ifndef PROCESSOR_CMD_H
#define PROCESSOR_CMD_H

#include "cmdOpcodes.h"
#include "cmd_spu.h"

struct command{
    cmdOpcodes            code; 
    handler_t             handler;   
    int                   nArgs; 
};

#define __cringe(...)  __VA_ARGS__

const command commandsHandler[]{ 
    {ADD,       add,       0}, 
    {SUB,       sub,       0},
    {MUL,       mul,       0},
    {DIV,       div,       0},
    {SQRT,      sqrt,      0},
        
        
    {JMP,      jmp,        1},
    {JB,       jb,         1},
    {JBE,      jbe,        1},
    {JA,       ja ,        1},
    {JAE,      jae,        1},
    {JE,       je ,        1},
    {JNE,      jne,        1},
    
    {PUSH,     push,       1},
    {PUSHREG,  pushreg,    1},
    {POPREG,   popreg,     1}, 
    {IN,       in,         0},
    {OUT,      out,        0},
    {HLT,      hlt,        0},
    {CALL,     callFunc,   1},
    {RET,      retFunc,    1}
}; 

#endif /* PROCESSOR_CMD_H */
