#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"

#define FALSE 0
#define TRUE !FALSE

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
    if (*ptr)
    {
        free(*ptr);
        *ptr = NULL;
    }
}

void freeArray(void*** ptr, int len)
{
    int i;

    if (*ptr)
    {
        for (i = 0; i < len; i++)
        {
            freePtr((*ptr) + i);
        }
        freePtr((void**)ptr);
    }
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

void lower(char* str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if ((str[i] < 'a') || (str[i] > 'z'))
        {
            str[i] = str[i] + 32;
        }
        i++;
    }
}

void removeTrailingNewline(char* str, int len)
{
    int removed = FALSE;
    int i = 0;

    while (! removed && i < len)
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';
            removed = TRUE;
        }
        i++;
    }
}

void printStringArray(char* format, char** strArr, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        fprintf(stdout, format, strArr[i]);
    }
}

void printStringArrayUntilEOF(char* format, char** strArr)
{
    int i = 0;
    while (! stringCompare(strArr[i], "EOF"))
    {
        fprintf(stdout, format, strArr[i++]);
    }
}
