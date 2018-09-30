#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

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
    int strLen;

    strLen = strlen(contents) + 1;
    initString(str, strLen);
    strncpy(*str, contents, strLen);
}

void initStringArray(char*** arr, int rows, int cols)
{
    int i;

    *arr = (char**)malloc(rows * sizeof(char*));
    for (i = 0; i < rows; i++)
    {
        initString((*arr) + i, cols);
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
            freePtr(&(*ptr)[i]);
        }
        freePtr((void**)ptr);
    }
}

/**
 *  Utilities
 **/

int stringCompare(char* str1, char* str2)
{
    int returnCode;

    returnCode = FALSE;

    if (! strcmp(str1, str2))
    {
        returnCode = TRUE;
    }

    return returnCode;
}

void upperRange(char* str, int range)
{
    int i;

    i = 0;

    while (str[i] != '\0' && i < range)
    {
        UPPER(str[i]);
        i++;
    }
}

int integerBoundaryCheck(int i, int lower, int upper)
{
    int isValid;

    isValid = FALSE;

    if (INT_BOUND(i, lower, upper))
    {
        isValid = TRUE;
    }

    return isValid;
}

int doubleCompare(double num1, double num2)
{
    int isEqual;

    isEqual = FALSE;

    if (fabs(num1 - num2) < TOLERANCE)
    {
        isEqual = TRUE;
    }

    return isEqual;
}

int doubleBoundaryCheck(double i, double lower, double upper)
{
    int isValid;

    isValid = FALSE;

    if (upper < lower)
    {
        if (DOUBLE_CHECK(i, lower))
        {
            isValid = TRUE;
        }
    }
    else if (upper < lower)
    {
        if (DOUBLE_CHECK(upper, i))
        {
            isValid = TRUE;
        }
    }
    else
    {
        if (DOUBLE_BOUND(i, lower, upper))
        {
            isValid = TRUE;
        }
    }

    return isValid;
}

void removeTrailingNewline(char* str, int len)
{
    int removed, i;

    removed = FALSE;
    i = len;

    while (! removed && --i >= 0)
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';
            removed = TRUE;
        }
    }
}

int countWhiteSpace(char* str)
{
    int count, i;

    count = 0;
    i = 0;

    while (str[i] != '\0')
    {
        if (isspace(str[i]))
        {
            count++;
        }
        i++;
    }

    return count;
}

void trim(char** str)
{
    int start, end, len, newLen, i;
    char* tempStr;

    start = 0;
    end = 0;
    len = strlen(*str) + 1;
    newLen = 0;
    i = 0;
    tempStr = NULL;

    while (isspace((*str)[i]) && ++i < len)
    {
        start++;
    }

    if (start == (len - 1))
    {
        freePtr((void**)str);
        initStringWithContents(str, "");
    }
    else
    {
        i = len - 2;

        while (isspace((*str)[i]) && --i >= 0)
        {
            end++;
        }

        newLen = len - (start + end);
        initString(&tempStr, newLen);
        strncpy(tempStr, &((*str)[start]), newLen - 1);

        freePtr((void**)str);
        initStringWithContents(str, tempStr);
        freePtr((void**)&tempStr);
    }
}

void printStringArray(char* format, char** strArr, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        fprintf(stdout, format, strArr[i]);
    }
}
