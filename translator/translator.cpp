#include "workWIthFiles.h"
#include "strFunc.h"
#include "translator.h"

struct biteCodeBuffer{
    size_t size;
    char* pointer;
};

spu_command spu_commands[N_SPU_COMMANDS] = {
    {"PUSH", PUSH},
    {"ADD", ADD},
    {"SUB", SUB},
    {"MUL", MUL},
    {"DIV", DIV},
    {"OUT", OUT}
};

int main(void){
    DataFromInputFIle calcCommands;
    stringsFromFileToStructure(&calcCommands);

    char* biteCodeBuffer = (char*) calloc(sizeof(char), 100);
    for(int curString = 0; curString < calcCommands.nStrings; curString++){
        char curCommand[20] = {0};
        sscanf(calcCommands.strings[curString].stringPtr, "%s", curCommand);

        printf("curCommand: %s\n", curCommand);
        int commandCode = assemble(curCommand);
        sprintf(biteCodeBuffer, "%d ", commandCode);
        printf("biteCodeBuffer now: %s\n", biteCodeBuffer);
        

        if(!strCmpSpuCom4(spu_commands[0].name, curCommand)){

            printf("MEOW\n");
            int pushParameter = 0;

            sscanf(calcCommands.strings[curString].stringPtr, "%*s %d", &pushParameter);

            sprintf(biteCodeBuffer, "%d ", pushParameter);
            printf("biteCodeBuffer now: %s\n", biteCodeBuffer);
        }
        printf("\n");
        
    }

    printf("BiteCodeBuffer: %s\n", biteCodeBuffer);

    FILE* biteCodeFile = openBiteCodeFile();
    fwrite(biteCodeBuffer, sizeof(char), 100, biteCodeFile);

    fclose(biteCodeFile);
    free(biteCodeBuffer);
}

int assemble(char* curCommand){
    assert(curCommand);

    for(size_t curCommandInd = 0; curCommandInd < N_SPU_COMMANDS; curCommandInd++){
        printf("Результат сравнения строк при помощи strCmpSpuCom4(): %d\n", strCmpSpuCom4(curCommand, spu_commands[curCommandInd].name));
        if(!strCmpSpuCom4(curCommand, spu_commands[curCommandInd].name)){
            printf("Code to return: %d\n", spu_commands[curCommandInd].code);
            return spu_commands[curCommandInd].code;
        }
    }
    return ASSEMBLE_FAILURE;
}