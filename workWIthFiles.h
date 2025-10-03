#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

const char* const ALERT_GET_INFO_FAILURE            = "Ошибка при попытке получить информацию о файле\n";
const char* const GET_FILE_SIZE_ERROR_DESCRIPTION   = "getFileSize() failed";
const char* const ALERT_FILE_OPEN_FAILURE           = "Ошибка при открытии файла %s\n";
const char* const OPEN_INPUT_FILE_ERROR_DESCRIPTION = "openInputFile() failed";

size_t getFileSize(const char* fileName);
FILE* openInputFile(const char* fileName);

#endif /* FILES_H */