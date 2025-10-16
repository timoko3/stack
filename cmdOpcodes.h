#ifndef CMD_OPCODES_H
#define CMD_OPCODES_H

#include <stddef.h> 

enum cmdOpcodes{
    PUSH = 1,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    SQRT,
    OUT,
    HLT,
    JMP,
    JB,
    JBE,
    JA,
    JAE,
    JE,
    JNE,
    CALL,
    RET,
    PUSHREG = 32 + PUSH,
    POPREG
};

struct buffer_t{
    int* ptr;
    size_t size;
};

const size_t N_REGISTERS   = 10;
const size_t PREAMBLE_SIZE = 2;
const int SIGNATURE        = 999;
const int VERSION          = 1;

#endif /* CMD_OPCODES_H*/