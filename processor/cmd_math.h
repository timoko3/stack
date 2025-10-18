typedef int cmdParam_t;

bool div(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool add(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool sub(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool mul(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool sqrt(cmdParam_t param,  cmdParam_t* result);

bool lt(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool le(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool gt(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool ge(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool eq(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);
bool ne(cmdParam_t param1, cmdParam_t param2, cmdParam_t* result);

    