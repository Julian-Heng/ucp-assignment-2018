/**
 * Filename: fileIO.c
 * Author:   Julian Heng (19473701)
 * Purpose:  Provide functions relating to file input and output.
 * Last Modified: 2018-10-05T18:28:19+08:00
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"
#include "fileIO.h"

#define FALSE 0
#define TRUE !FALSE

#define EXTRA_PAD 4

/**
 * Name:     readFileToArray
 * Purpose:
 *     Read a file into an array. Useful for getting the contents
 *     to a string array for use in a program, instead of using
 *     the FILE pointer to process the file.
 *
 * Parameters:
 *   - filename : A string for the filename
 *   - arr      : A pointer to a string array
 *   - lines    : A pointer to an int for the number of lines
 *   - length   : A pointer to an int for the maximum length of a line
 *
 * Returns: int / boolean
 * Assertions:
 *     Assumptions:
 *         None
 *     Results:
 *         arr will contain the file contents
 **/

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

    /* Check if file is NULL */
    if (! file)
    {
        /* Print error message */
        printFileError("Error opening", filename);
        statusCode = FALSE;
    }
    else
    {
        statusCode = TRUE;
        /* Get file statistics and check if it's length is not 0 */
        if (getFileStats(filename, file, lines, length) &&
            *lines != 0)
        {
            /* Allocate memory in heap to store file contents */
            initStringArray(arr, *lines, *length);

            do
            {
                /* Allocate memory for a temporary string */
                initString(&tempStr, (*length) + 1);

                /**
                 * Get line in file, with the maximum line length
                 * Plus one is for including newline
                 **/
                if (fgets(tempStr, (*length) + 1, file))
                {
                    /* Copy from temporary string to array */
                    removeTrailingNewline(tempStr, (*length) + 1);
                    strncpy((*arr)[count], tempStr, (*length));
                }
                else
                {
                    isEOF = TRUE;
                }

                /* Check for file error */
                if (ferror(file))
                {
                    /* Print error and clear array */
                    printFileError("Error reading", filename);
                    freeArray((void***)arr, *lines);
                    statusCode = FALSE;
                    isEOF = TRUE;
                }

                /* Garbage collection */
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

/**
 * Name:     appendToFile
 * Purpose:
 *     Append a line to a file.
 *
 * Parameters:
 *   - file   : A FILE pointer to a file stream
 *   - format : A string for the format print
 *   - line   : A string for the line to append
 *   - errMsg : A string for the error message if it occurs
 *
 * Returns: int / boolean
 * Assertions:
 *     Assumptions:
 *         file is already set to a file stream
 *         line is a valid string
 *         All error handling is to be done outside this function
 *     Results:
 *         line will be appended to file
 **/

int appendToFile(FILE* file, char* format, char* line, char* errMsg)
{
    int statusCode;

    /* Check if file is set to a file stream */
    if (! file)
    {
        printFileError(errMsg, "");
        statusCode = FALSE;
    }
    else
    {
        /* Print to file */
        statusCode = TRUE;
        fprintf(file, format, line);
    }

    return statusCode;
}

/**
 * Name:     getFileStats
 * Purpose:
 *     Get a file's length and maximum line length. Useful for getting
 *     information needed to create an array to fit the file or checking
 *     if the file is valid.
 *
 * Parameters:
 *   - filename : A string for the filename
 *   - file     : A FILE pointer to a file stream
 *   - lines    : A pointer to an int for the number of lines
 *   - length   : A pointer to an int for the maximum line length
 *
 * Returns:
 * Assertions:
 *     Assumptions:
 *         filename is a valid string
 *         file is already set to a file stream
 *     Results:
 *         lines and length are set to no. of lines and maximum line length
 **/

int getFileStats(char* filename, FILE* file, int* lines, int* length)
{
    int statusCode, currentLength, ch;

    statusCode = TRUE;
    currentLength = 0;

    /* Check if file is a valid file stream */
    if (file)
    {
        do
        {
            /* If character from file is a newline, increment count */
            if ((ch = fgetc(file)) == '\n')
            {
                (*lines)++;
                /* Check current line is longer than the current maximum */
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

            /* Error checking */
            if (ferror(file))
            {
                printFileError("Error reading", filename);
                statusCode = FALSE;
            }

        } while (ch != EOF && statusCode);

        /* Increment line length for current line and rewind file to start */
        (*length)++;
        fseek(file, 0, SEEK_SET);
    }
    else
    {
        statusCode = FALSE;
    }

    return statusCode;
}

/**
 * Name:     printFileError
 * Purpose:
 *     Print an error message if a file error occurs. Simply a wrapper
 *     function to format and print a file error.
 *
 * Parameters:
 *   - msg      : The error message to print
 *   - filename : The filename related to the file error
 *
 * Returns:
 * Assertions:
 *     Assumptions:
 *         filename can be invalid, meaning that a filename is not provided
 *         msg is a valid string
 *     Results:
 *         An error message will be printed
 **/

void printFileError(char* msg, char* filename)
{
    char* errMsg;
    int errMsgLen;

    /* If filename is not provided, just print msg */
    if (stringCompare(filename, ""))
    {
        perror(msg);
    }
    else
    {
        /* Allocate memory for string and format it */
        errMsg = NULL;
        errMsgLen = strlen(msg) + strlen(filename) + EXTRA_PAD;

        initString(&errMsg, errMsgLen);
        sprintf(errMsg, "%s '%s'", msg, filename);

        /* Print string and free string */
        perror(errMsg);
        freePtr((void**)&errMsg);
    }
}
