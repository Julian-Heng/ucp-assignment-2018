#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"
#include "fileIO.h"

#define FALSE 0
#define TRUE !FALSE

#define EXTRA_PAD 4

int readFileToArray(char* filename, char*** arr, int* lines, int* length)
{
    FILE* file;
    char* tempStr;
    int isEOF, statusCode, count;

    file = NULL;
    tempStr = NULL;

    isEOF = FALSE;
    statusCode = TRUE;
    count = 0;

    file = fopen(filename, "r");

    if (! file)
    {
        printFileError("Error opening", filename);
        statusCode = FALSE;
    }
    else
    {
        statusCode = TRUE;
        if (getFileStats(filename, file, lines, length) &&
            *lines != 0)
        {
            initStringArray(arr, *lines, *length);
            do
            {
                initString(&tempStr, (*length) + 1);
                if (fgets(tempStr, (*length) + 1, file))
                {
                    removeTrailingNewline(tempStr, (*length) + 1);
                    strncpy((*arr)[count], tempStr, (*length));
                }
                else
                {
                    isEOF = TRUE;
                }

                if (ferror(file))
                {
                    printFileError("Error reading", filename);
                    freeArray((void***)arr, *lines);
                    statusCode = FALSE;
                    isEOF = TRUE;
                }
                freePtr((void**)&tempStr);
            } while (++count < *lines && ! isEOF);
        }
        else
        {
            statusCode = FALSE;
        }

        fclose(file);
    }

    return statusCode;
}

int printToFile(FILE* file, char* format, char* line, char* errMsg)
{
    int statusCode;

    if (! file)
    {
        printFileError(errMsg, "");
        statusCode = FALSE;
    }
    else
    {
        statusCode = TRUE;
        fprintf(file, format, line);
    }

    return statusCode;
}

int getFileStats(char* filename, FILE* file, int* lines, int* length)
{
    int statusCode, currentLength, ch;

    statusCode = TRUE;
    currentLength = 0;

    do
    {
        if ((ch = fgetc(file)) == '\n')
        {
            (*lines)++;
            if (currentLength > *length)
            {
                *length = currentLength;
            }
            currentLength = 0;
        }
        else
        {
            currentLength++;
        }

        if (ferror(file))
        {
            printFileError("Error reading", filename);
            statusCode = FALSE;
        }

    } while (ch != EOF && statusCode);

    (*length)++;
    fseek(file, 0, SEEK_SET);

    return statusCode;
}

void printFileError(char* msg, char* filename)
{
    char* errMsg;
    int errMsgLen;

    if (stringCompare(filename, ""))
    {
        perror(msg);
    }
    else
    {
        errMsg = NULL;
        errMsgLen = strlen(msg) + strlen(filename) + EXTRA_PAD;

        initString(&errMsg, errMsgLen);
        sprintf(errMsg, "%s '%s'", msg, filename);
        perror(errMsg);

        freePtr((void**)&errMsg);
    }
}
