#include "workWIthFiles.h"

// #define DEBUG

static int countStrings(char* buf, int fileSize, char endStr);

static size_t getFileSize();
static FILE* openInputFile();
static char* getTextToBuffer(FILE* inputFile, int fileSize, int* nStrings);
static string* divideBufferToStruct(char* buffer, int nStrings);

#ifdef DEBUG
static void printBuffer(char* buffer);
#endif /* DEBUG */

int stringsFromFileToStructure(DataFromInputFIle* DataFromInputFIle){
    assert(DataFromInputFIle);

    if((DataFromInputFIle->fileSize = getFileSize()) == EXIT_FAILURE) return EXIT_FAILURE;

    DataFromInputFIle->inputFile = openInputFile();
    if(!DataFromInputFIle->inputFile) return EXIT_FAILURE;
    assert(DataFromInputFIle->inputFile);

    
    DataFromInputFIle->buffer = getTextToBuffer(DataFromInputFIle->inputFile, DataFromInputFIle->fileSize, &DataFromInputFIle->nStrings);
    assert(DataFromInputFIle->buffer);
    
    fclose(DataFromInputFIle->inputFile);
    
    DataFromInputFIle->bufferSize = DataFromInputFIle->fileSize + SIZE_OF_END_STR;
    
    DataFromInputFIle->strings = divideBufferToStruct(DataFromInputFIle->buffer, DataFromInputFIle->nStrings);
    assert(DataFromInputFIle->strings);

    return 0;
}

static size_t getFileSize(){
    struct stat file_info = {};

    if(stat(INPUT_FILE_NAME, &file_info) != 0){
        fprintf(stderr, ALERT_GET_INFO_FAILURE);
        perror(GET_FILE_SIZE_ERROR_DESCRIPTION);
        return EXIT_FAILURE;
    }

    return file_info.st_size;
}

static FILE* openInputFile(){
    FILE* inputFile = NULL;

    if(!(inputFile = fopen(INPUT_FILE_NAME, "rb"))){
        fprintf(stderr, ALERT_FILE_OPEN_FAILURE, INPUT_FILE_NAME);
        perror(OPEN_INPUT_FILE_ERROR_DESCRIPTION);
        return NULL;
    }

    return inputFile;
}

static char* getTextToBuffer(FILE* inputFIle, int fileSize, int* nStrings){
    assert(inputFIle);
    assert(nStrings);

    char* buffer = (char*) calloc(fileSize + SIZE_OF_END_STR, sizeof(char)); 
    assert(buffer);
    buffer[fileSize] = '\n';

    fread(buffer, sizeof(char), fileSize, inputFIle);
    
    *nStrings = countStrings(buffer, fileSize, END_STR);

    return buffer;
}

static string* divideBufferToStruct(char* buffer, int nStrings){
    assert(buffer);
    
    string* strings = (string*) calloc(nStrings, sizeof(string));
    assert(strings);

    strings[0].stringPtr = buffer;

    #ifdef DEBUG
    printBuffer(buffer);
    #endif /* DEBUG */

    int curStr = 1;
    int curSym = 0;
    for(curSym = 0; buffer[curSym] != '\0'; curSym++){
        if((buffer[curSym] == END_STR) && (buffer[curSym + 1] != '\0')){
            strings[curStr].stringPtr = buffer + curSym + 1;
            strings[curStr - 1].len = strings[curStr].stringPtr - 
                                      strings[curStr - 1].stringPtr;
            curStr++;
        }
    }

    strings[curStr - 1].len = (buffer + curSym) - strings[curStr - 1].stringPtr;

    return strings;
}

static int countStrings(char* buf, int fileSize, char endStr){
    assert(buf);

    int nStrings = 1;

    int curSym = 0;
    while(curSym < fileSize){
        if(buf[curSym] == endStr){
            nStrings++;
        }
        curSym++;
    }

    return nStrings;
}

#ifdef DEBUG
static void printBuffer(char* buffer){
    assert(buffer);

    for(size_t curSym = 0; curSym < 18; curSym++){
        printf("Символ %ld: %d\n", curSym, buffer[curSym]);
    }   
}
#endif /* DEBUG */