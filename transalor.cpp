#include <stdio.h>

#include "workWIthFiles.h"

int main(void){
    DataFromInputFIle calcCommands;
    stringsFromFileToStructure(&calcCommands);

    for(int curString = 0; curString < calcCommands.nStrings; curString++){
        char curCommand[20] = {0};
        sscanf(calcCommands.strings[curString].stringPtr, "%s", curCommand);

    }

}