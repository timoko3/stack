#ifndef TRANSLATOR_CMD_TYPE_H
#define TRANSLATOR_CMD_TYPE_H

#include "cmdOpcodes.h"

enum param_t{
    NO_PARAM,
    LABEL_PARAM,
    REG_PARAM,
    NUMBER_PARAM
};

struct command_t{
    cmdOpcodes    code;
    const char*   name;
    param_t       param;
    unsigned long hash;
};

#endif /* TRANSLATOR_CMD_TYPE_H */