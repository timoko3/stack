#ifndef TRANSLATOR_CMD_H
#define TRANSLATOR_CMD_H

#include "cmdOpcodes.h"

enum paramType{
    NO_PARAM,
    LABEL_PARAM,
    REG_PARAM,
    NUMBER_PARAM
};

struct command_t{
    cmdOpcodes    code;
    const char*   name;
    unsigned long hash;
    paramType     param;
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
    {RET,     "RET",     0, NO_PARAM    }
};

#endif /* TRANSLATOR_CMD_H */