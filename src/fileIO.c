#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"
#include "fileIO.h"

#define FALSE 0
#define TRUE !FALSE

int readFileToArray(char* filename, char*** arr, int* lines, int* length)
{
    FILE* file = NULL;

    char* tempStr;

    int isEOF = FALSE;
    int statusCode = TRUE;
    int count = 0;

    file = fopen(filename, "r");

    if (! file)
    {
        printFileError("Error opening", filename);
        statusCode = FALSE;
    }
    else
    {
        statusCode = TRUE;
        if (getFileStats(filename, file, lines, length))
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

int getFileStats(char* filename, FILE* file, int* lines, int* length)
{
    int statusCode = TRUE;
    int currentLength = 0;
    int ch;

    do
    {
        ch = fgetc(file);

        if (ferror(file))
        {
            printFileError("Error reading", filename);
            statusCode = FALSE;
        }

        if (ch == '\n')
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

    } while (ch != EOF && statusCode);

    (*length)++;
    fseek(file, 0, SEEK_SET);
    return statusCode;
}

void printFileError(char* msg, char* filename)
{
    char* errMsg = NULL;
    int errMsgLen = strlen(msg) + strlen(filename) + 4;

    initString(&errMsg, errMsgLen);
    sprintf(errMsg, "%s '%s'", msg, filename);
    perror(errMsg);

    freePtr((void**)&errMsg);
}
