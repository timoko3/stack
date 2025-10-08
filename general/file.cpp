#include "file.h"
#include "translator/translator.h"

#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

// #define DEBUG

static char* getTextToBuffer(FILE* inputFile, size_t fileSize);
static string* divideBufferToStringsStructure(char* buffer, size_t nStrings);

#ifdef DEBUG
static void printBuffer(char* buffer);
#endif /* DEBUG */

int stringsFromFileToFileDataStructure(DataFromInputFIle* DataFromInputFIle){
    assert(DataFromInputFIle);

    if((DataFromInputFIle->fileSize = getFileSize(TEXT_COMMANDS_FILE_NAME)) == EXIT_FAILURE) return EXIT_FAILURE;

    fileDescription inputFile = {
        TEXT_COMMANDS_FILE_NAME, 
        "rb"
    };

    DataFromInputFIle->inputFile = myOpenFile(&inputFile);
    if(!DataFromInputFIle->inputFile) return EXIT_FAILURE;
    assert(DataFromInputFIle->inputFile);

    DataFromInputFIle->buffer = getTextToBuffer(DataFromInputFIle->inputFile, DataFromInputFIle->fileSize);
    assert(DataFromInputFIle->buffer);

    fclose(DataFromInputFIle->inputFile);

    DataFromInputFIle->nStrings = countStrings(DataFromInputFIle->buffer, DataFromInputFIle->fileSize, END_STR);
    DataFromInputFIle->bufferSize = DataFromInputFIle->fileSize + SIZE_OF_END_FILE;
    
    DataFromInputFIle->strings = divideBufferToStringsStructure(DataFromInputFIle->buffer, DataFromInputFIle->nStrings);
    assert(DataFromInputFIle->strings);

    return 0;
}

size_t getFileSize(const char* fileName){
    assert(fileName);
    
    struct stat file_info = {};
    if(stat(fileName, &file_info) != 0){
        fprintf(stderr, ALERT_GET_INFO_FAILURE);
        perror(FUNCTION_FAILURE_ALERT);
        return EXIT_FAILURE;
    }

    return (size_t) file_info.st_size;
}

FILE* myOpenFile(fileDescription* file){
    assert(file);

    FILE* filePtr = fopen(file->name, file->openMode);
    if(!filePtr){
        fprintf(stderr, ALERT_FILE_OPEN_FAILURE, file->name);
        perror(FUNCTION_FAILURE_ALERT);
        return NULL;
    }

    return filePtr;
}

// MENTOR FILE* openByteCodeFile(){
// static FILE* myOpenFile(){

static char* getTextToBuffer(FILE* inputFIle, size_t fileSize){
    assert(inputFIle);
    
    char* buffer = (char*) calloc(fileSize + SIZE_OF_END_FILE, sizeof(char)); 
    assert(buffer);
    buffer[fileSize] = '\n';
    buffer[fileSize + 1] = '\0';

    fread(buffer, sizeof(char), fileSize, inputFIle);
    // MENTOR return buffer
    return buffer;
}

static string* divideBufferToStringsStructure(char* buffer, size_t nStrings){
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
        #ifdef DEBUG
        fprintf(stderr, "curSym = %d\n", curSym);
        fprintf(stderr, "buffer[curSym] = %d\n", buffer[curSym]);
        fprintf(stderr, "buffer[curSym + 1] = %d\n", buffer[curSym + 1]);
        #endif /* DEBUG */
        if(buffer[curSym] == END_STR){
            
            strings[curStr].stringPtr = buffer + curSym + 1;
            strings[curStr - 1].len = (size_t) strings[curStr].stringPtr - 
                                      (size_t) strings[curStr - 1].stringPtr;
            curStr++;
        }
        
    }
    
    strings[curStr - 1].len = (size_t) ((buffer + curSym) - strings[curStr - 1].stringPtr);

    return strings;
}

void createByteCodeFile(int* byteCodeBuffer, size_t curByteBufferSize){
    assert(byteCodeBuffer);

    fileDescription byteCode = {
        BYTE_CODE_FILE_NAME,
        "wb"
    };

    FILE* byteCodeFile = myOpenFile(&byteCode);
    assert(byteCodeFile);

    size_t written = fwrite(byteCodeBuffer, sizeof(int), curByteBufferSize, byteCodeFile);
    if(written != curByteBufferSize){
        ///log
        perror("fwrite error");
    }

    fclose(byteCodeFile);
}

#ifdef DEBUG
static void printBuffer(char* buffer){
    assert(buffer);

    for(size_t curSym = 0; curSym < 31; curSym++){
        printf("Символ %ld: %d\n", curSym, buffer[curSym]);
    }   
}
#endif /* DEBUG */