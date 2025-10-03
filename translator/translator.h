#ifndef TRANSLATOR_H
#define TRANSLATOR_H

// #include <stdio.h>

const char* const  BITE_CODE_FILE_NAME = "biteCode.asm";

const int ASSEMBLE_FAILURE = -1;

enum spu_commands_codes{
    PUSH = 1,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    OUT,
    PUSHREG = 33,
    POPREG
};

struct spu_command{
    const char* name;
    spu_commands_codes code;
};

const size_t N_SPU_COMMANDS = 9;

#endif /* TRANSLATOR_H */
