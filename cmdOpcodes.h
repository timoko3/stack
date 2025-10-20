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
    IN,
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

struct command{
    cmdOpcodes            code; 
    handler_t             handler;   
    int                   nArgs; 
    param_t       param;
    const char*   name;
    unsigned long hash;
};

struct commandGeneral commands[]{
    {ADD,      add,       0, NO_PARAM},
    {SUB,      sub,       0, NO_PARAM},
    {MUL,      mul,       0, NO_PARAM},
    {DIV,      div,       0, NO_PARAM},
    {SQRT,     sqrt,      0, NO_PARAM},
            
        
    {JMP,      jmp,        1, LABEL_PARAM},
    {JB,       jb,         1, LABEL_PARAM},
    {JBE,      jbe,        1, LABEL_PARAM},
    {JA,       ja ,        1, LABEL_PARAM},
    {JAE,      jae,        1, LABEL_PARAM},
    {JE,       je ,        1, LABEL_PARAM},
    {JNE,      jne,        1, LABEL_PARAM},
    {    
    {PUSH,     push,       1, NUMBER_PARAM},
    {PUSHREG,  pushreg,    1, REG_PARAM},
    {POPREG,   popreg,     1, REG_PARAM},
    {IN,       in,         0, NO_PARAM},
    {OUT,      out,        0, NO_PARAM},
    {HLT,      hlt,        0, NO_PARAM},
    {CALL,     callFunc,   1, LABEL_PARAM},
    {RET,      retFunc,    1, NO_PARAM}
};



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

const size_t N_REGISTERS   = 10;
const size_t PREAMBLE_SIZE = 2;
const int SIGNATURE        = 999;
const int VERSION          = 1;

#endif /* CMD_OPCODES_H */