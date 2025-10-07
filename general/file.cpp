#include "file.h"
#include <sys/stat.h>
#include <unistd.h>

// #define DEBUG

static int countStrings(char* buf, int fileSize, char endStr);

static size_t getFileSize();
static FILE* openInputFile();
static char* getTextToBuffer(FILE* inputFile, int fileSize, int* nStrings);
static string* divideBufferToStruct(char* buffer, int nStrings);

#ifdef DEBUG
static void printBuffer(char* buffer);
#endif /* DEBUG */

size_t getFileSize(const char* fileName){
    assert(fileName);
    
    struct stat file_info = {};
    if(stat(fileName, &file_info) != 0){
        fprintf(stderr, ALERT_GET_INFO_FAILURE);
        perror(GET_FILE_SIZE_ERROR_DESCRIPTION);
        return EXIT_FAILURE;
    }

    return (size_t) file_info.st_size;
}

FILE* openInputFile(const char* fileName){
    FILE* inputFile = NULL;

    if(!(inputFile = fopen(fileName, "rb"))){
        fprintf(stderr, ALERT_FILE_OPEN_FAILURE, fileName);
        perror(OPEN_INPUT_FILE_ERROR_DESCRIPTION);
        return NULL;
    }

    return inputFile;
}

int stringsFromFileToStructure(DataFromInputFIle* DataFromInputFIle){
    assert(DataFromInputFIle);

    if((DataFromInputFIle->fileSize = getFileSize(TEXT_COMMANDS)) == EXIT_FAILURE) return EXIT_FAILURE;

    DataFromInputFIle->inputFile = openInputFile(TEXT_COMMANDS);
    if(!DataFromInputFIle->inputFile) return EXIT_FAILURE;
    assert(DataFromInputFIle->inputFile);

    DataFromInputFIle->buffer = getTextToBuffer(DataFromInputFIle->inputFile, DataFromInputFIle->fileSize, &DataFromInputFIle->nStrings);
    assert(DataFromInputFIle->buffer);
    
    fclose(DataFromInputFIle->inputFile);
    
    DataFromInputFIle->bufferSize = DataFromInputFIle->fileSize + SIZE_OF_END_FILE;
    
    DataFromInputFIle->strings = divideBufferToStruct(DataFromInputFIle->buffer, DataFromInputFIle->nStrings);
    assert(DataFromInputFIle->strings);

    return 0;
}

// MENTOR FILE* openByteCodeFile(){
// static FILE* openInputFile(){

FILE* openByteCodeFile(){
    FILE* byteCodeFile = fopen(BITE_CODE_FILE_NAME, "wb");
    if(!byteCodeFile){
        fprintf(stderr, ALERT_BITE_CODE_FILE_OPEN_FAILURE);
        perror(OPEN_BITE_CODE_FILE_ERROR_DESCRIPTION);
        return NULL;
    }

    return byteCodeFile;
}

static char* getTextToBuffer(FILE* inputFIle, int fileSize, int* nStrings){
    assert(inputFIle);
    assert(nStrings);
    
    char* buffer = (char*) calloc(fileSize + SIZE_OF_END_FILE, sizeof(char)); 
    assert(buffer);
    buffer[fileSize] = '\n';
    buffer[fileSize + 1] = '\0';

    fread(buffer, sizeof(char), fileSize, inputFIle);
    // return buffer
    
    *nStrings = countStrings(buffer, fileSize, END_STR);

    return buffer;
}

static string* divideBufferToStruct(char* buffer, int nStrings){
    // struct 
    assert(buffer);
    
    string* strings = (string*) calloc(nStrings, sizeof(string));
    assert(strings);


    strings[0].stringPtr = buffer;

    #ifdef DEBUG
    printBuffer(buffer);
    #endif /* DEBUG */

    int curStr = 1;
    int curSym = 0;
    for(curSym = 0; buffer[curSym + 1] != '\0'; curSym++){
        fprintf(stderr, "curSym = %d\n", curSym);
        fprintf(stderr, "buffer[curSym] = %d\n", buffer[curSym]);
        fprintf(stderr, "buffer[curSym + 1] = %d\n", buffer[curSym + 1]);
        if(buffer[curSym] == END_STR){
            
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

    for(size_t curSym = 0; curSym < 31; curSym++){
        printf("Символ %ld: %d\n", curSym, buffer[curSym]);
    }   
}
#endif /* DEBUG */