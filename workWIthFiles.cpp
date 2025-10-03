#include "workWIthFiles.h"

#include <sys/stat.h>

size_t getFileSize(const char* fileName){
    assert(fileName);
    
    struct stat file_info = {};

    if(stat(fileName, &file_info) != 0){
        fprintf(stderr, ALERT_GET_INFO_FAILURE);
        perror(GET_FILE_SIZE_ERROR_DESCRIPTION);
        return EXIT_FAILURE;
    }

    return file_info.st_size;
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