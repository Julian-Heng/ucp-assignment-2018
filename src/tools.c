#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"

/**
 *  Mallocing functions
 **/

void initString(char** str, int len)
{
    *str = (char*)malloc(len * sizeof(char));
    memset(*str, '\0', len);
}

void initStringWithContents(char** str, char* contents)
{
    int strLen = strlen(contents) + 1;
    initString(str, strLen);
    strncpy(*str, contents, strLen);
}

void initStringArray(char*** arr, int rows, int cols)
{
    int i;
    *arr = (char**)malloc(rows * sizeof(char*));
    for (i = 0; i < rows; i++)
    {
        initString(((*arr) + i), cols);
    }
}

/**
 *  Free functions
 **/

void freePtr(void** ptr)
{
    free(*ptr);
    *ptr = NULL;
}

void freeArray(void*** ptr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        freePtr((*ptr) + i);
    }
    freePtr((void**)ptr);
}

/**
 *  Utilities
 **/

int stringCompare(char* str1, char* str2)
{
    int returnCode = 0;
    if (strcmp(str1, str2) == 0)
    {
        returnCode = 1;
    }
    return returnCode;
}

void upper(char* str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if ((str[i] < 'A') || (str[i] > 'Z'))
        {
            str[i] = str[i] - 32;
        }
        i++;
    }
}

void printStringArrayUntilEOF(char** strArr)
{
    int i = 0;
    while (! stringCompare(strArr[i], "EOF"))
    {
        fprintf(stdout, "%s", strArr[i++]);
    }
}
