#ifndef FILE_PARSE_H
#define FILE_PARSE_H

// 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>

#include "strFunc.h"
#include "translator/translator.h"

const char* const ALERT_FILE_OPEN_FAILURE               = "Ошибка при открытии файла %s\n";
const char* const ALERT_GET_INFO_FAILURE                = "Ошибка при попытке получить информацию о файле\n";
const char* const ALERT_STR_FROM_FILE_TO_STRUCT_FAILURE = "Ошибка при передаче информации о файле в структуру\n";
const char* const ALERT_BITE_CODE_FILE_OPEN_FAILURE     = "Ошибка при открыти файла для записи байт-кода\n";

// FUNC
const char* const GET_FILE_SIZE_ERROR_DESCRIPTION       = "getFileSize() failed";
const char* const OPEN_INPUT_FILE_ERROR_DESCRIPTION     = "openInputFile() failed";
const char* const FAILURE_STRINGS_FROM_FILE_TO_STRUCT   = "stringsFromFileToStructure() failed";
const char* const OPEN_BITE_CODE_FILE_ERROR_DESCRIPTION = "openByteCodeFile() failed";

struct DataFromInputFIle {
    size_t fileSize;
    FILE* inputFile;
    int nStrings;
    char* buffer;
    size_t bufferSize;
    string* strings;
};

int stringsFromFileToStructure(DataFromInputFIle* DataFromInputFIle);

size_t getFileSize(const char* fileName);
FILE* openInputFile(const char* fileName);
FILE* openByteCodeFile();

#endif /*FILE_PARSE_H*/
