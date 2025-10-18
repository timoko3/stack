#ifndef TRANSLATOR_CMD_H
#define TRANSLATOR_CMD_H

#include "cmdOpcodes.h"

const int    ASSEMBLE_FAILURE       = -1;

const size_t COMMAND_NAME_MAX_SIZE  = 10;
const size_t REGISTER_NAME_MAX_SIZE = 5;
const size_t LABEL_NAME_MAX_SIZE    = 20;

const size_t N_LABELS     = 10;
const int    LABEL_POISON = 46878756;

typedef int value;
struct label_t{
    const char* name;
    unsigned long hash;
    value addr;
};

enum param_t{
    NO_PARAM,
    LABEL_PARAM,
    REG_PARAM,
    NUMBER_PARAM
};

struct state_t{
    param_t par;
    size_t  StringInd;
    char*   cmdName;
    char*   labelName;
};

struct command_t{
    cmdOpcodes    code;
    const char*   name;
    unsigned long hash;
    param_t       param;
};

static command_t commands[] = {
    {PUSH,    "PUSH",    0, NUMBER_PARAM},
    {POP,     "POP",     0, NO_PARAM    },
    {PUSHREG, "PUSHREG", 0, REG_PARAM   },
    {POPREG,  "POPREG",  0, REG_PARAM   },
    {ADD,     "ADD",     0, NO_PARAM    },
    {SUB,     "SUB",     0, NO_PARAM    },
    {MUL,     "MUL",     0, NO_PARAM    },
    {DIV,     "DIV",     0, NO_PARAM    },
    {SQRT,    "SQRT",    0, NO_PARAM    },
    {OUT,     "OUT",     0, NO_PARAM    },
    {HLT,     "HLT",     0, NO_PARAM    },
    {JMP,     "JMP",     0, LABEL_PARAM },
    {JB,      "JB",      0, LABEL_PARAM },
    {JBE,     "JBE",     0, LABEL_PARAM },
    {JA,      "JA",      0, LABEL_PARAM },
    {JAE,     "JAE",     0, LABEL_PARAM },
    {JE,      "JE",      0, LABEL_PARAM },
    {JNE,     "JNE",     0, LABEL_PARAM },
    {CALL,    "CALL",    0, LABEL_PARAM },
    {RET,     "RET",     0, REG_PARAM   }
};

#endif /* TRANSLATOR_CMD_H */