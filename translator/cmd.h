#ifndef TRANSLATOR_CMD_H
#define TRANSLATOR_CMD_H

#include "cmd_type.h"

command_t commands[] = {
    {PUSH,    "PUSH",     NUMBER_PARAM},
    {POP,     "POP",      NO_PARAM    },
    {PUSHREG, "PUSHREG",  REG_PARAM   },
    {POPREG,  "POPREG",   REG_PARAM   },
    {ADD,     "ADD",      NO_PARAM    },
    {SUB,     "SUB",      NO_PARAM    },
    {MUL,     "MUL",      NO_PARAM    },
    {DIV,     "DIV",      NO_PARAM    },
    {SQRT,    "SQRT",     NO_PARAM    },
    {IN,      "IN",       NO_PARAM    },
    {OUT,     "OUT",      NO_PARAM    },
    {HLT,     "HLT",      NO_PARAM    },
    {JMP,     "JMP",      LABEL_PARAM },
    {JB,      "JB",       LABEL_PARAM },
    {JBE,     "JBE",      LABEL_PARAM },
    {JA,      "JA",       LABEL_PARAM },
    {JAE,     "JAE",      LABEL_PARAM },
    {JE,      "JE",       LABEL_PARAM },
    {JNE,     "JNE",      LABEL_PARAM },
    {CALL,    "CALL",     LABEL_PARAM },
    {RET,     "RET",      REG_PARAM   }
};

#endif /* TRANSLATOR_CMD_H */