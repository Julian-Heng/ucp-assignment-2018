/**
 * Filename: tools.c
 * Author:   Julian Heng (19473701)
 * Purpose:  Provide utility functions to other files
 *           that includes tools.h. Such tools include
 *           mallocing functions, freeing functions,
 *           string manipulation and number comparisons
 *
 * Last Modified: 2018-10-05T18:41:10+08:00
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "tools.h"

#define FALSE 0
#define TRUE !FALSE

/**
 * Name:     initString
 * Purpose:
 *     Allocate memory in the heap for a string and then
 *     fill the string with null terminators. Required as to
 *     prevent manually setting null terminator at the end
 *     of a string and gives a clean state to a string
 *
 * Parameters:
 *   - str : A pointer to a string
 *   - len : An integer for the string length
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         len is 1 or larger
 *     Results:
 *         str will be allocated memory for storing a string
 **/

void initString(char** str, int len)
{
    /* Allocate memory in the heap and set all elements to null character */
    *str = (char*)malloc(len * sizeof(char));

    /* Check if malloc failed */
    if (*str)
    {
        memset(*str, '\0', len);
    }
}

/**
 * Name:     initStringWithContents
 * Purpose:
 *     Allocate memory in the heap for a string and then
 *     copy the contents of the imported string to the
 *     new string. Useful for initialising a string with
 *     a set string.
 *
 * Parameters:
 *   - str      : A pointer to a string
 *   - contents : A string to be allocated
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         contents contains a string
 *     Results:
 *         str will be allocated memory for storing contents
 **/

void initStringWithContents(char** str, char* contents)
{
    int len;

    /* Get length of string plus one for null terminator, then init string*/
    len = strlen(contents) + 1;
    initString(str, len);

    /* If malloc failed */
    if (*str)
    {
        strncpy(*str, contents, len);
    }
}

/**
 * Name:     freePtr
 * Purpose:
 *     Wrapper function for free(). Does two tasks; free the pointer
 *     and set it to null. Simply a wrapper, nothing else.
 *
 * Parameters:
 *   - ptr : A void pointer to be freed
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         none
 *     Results:
 *         ptr will be freed
 **/

void freePtr(void** ptr)
{
    /* Check if pointer is not null */
    if (*ptr)
    {
        /* Free and set to NULL */
        free(*ptr);
        *ptr = NULL;
    }
}

/**
 * Name:     stringCompare
 * Purpose:
 *     Wrapper function for strcmp so that it returns sensible
 *     boolean value. This is simply syntatic sugar.
 *
 * Parameters:
 *   - str1 : A string
 *   - str2 : A string
 *
 * Returns:  int / boolean
 * Assertions:
 *     Assumptions:
 *         none
 *     Results:
 *         Returns TRUE (1) or FALSE (0)
 **/

int stringCompare(char* str1, char* str2)
{
    int returnCode;

    returnCode = FALSE;

    /* If string matches, return TRUE */
    if (! strcmp(str1, str2))
    {
        returnCode = TRUE;
    }

    return returnCode;
}

/**
 * Name:     upper
 * Purpose:
 *     Convert all characters in string to uppercase
 *
 * Parameters:
 *   - str   : A string to be changed to uppercase
 *   - range : An integer for the range
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         str contains a string
 *         range is less than string length and larger than one
 *     Results:
 *         Characters up to range will be uppercase
 **/

void upper(char* str)
{
    int i;

    i = 0;

    /* If string is valid */
    if (str && ! stringCompare(str, ""))
    {
        /* While within range, convert to uppercase */
        while (str[i] != '\0')
        {
            str[i] = toupper(str[i]);
            i++;
        }
    }
}

/**
 * Name:     doubleComare
 * Purpose:
 *     Compares two doubles. Needed due to how doubles are not precise
 *     due to floating points. Inherited from Java.
 *
 * Parameters:
 *   - num1 : First double
 *   - num2 : Second double to compare two
 *
 * Returns:  int / boolean
 * Assertions:
 *     Assumptions:
 *         none
 *     Results:
 *         Returns TRUE (1) or FALSE (0)
 **/

int doubleCompare(double num1, double num2)
{
    int isEqual;

    isEqual = FALSE;

    /* Check the double's tolerance value after getting the difference */
    if (fabs(num1 - num2) < TOLERANCE)
    {
        isEqual = TRUE;
    }

    return isEqual;
}

/**
 * Name:     removeTrailingNewline
 * Purpose:
 *     Remove trailing newline in string. Useful for striping a string
 *     from any trailing newline, cleaning the string up.
 *
 * Parameters:
 *   - str : A string
 *   - len : The string length
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         str is a valid string
 *     Results:
 *         Trailing newline will be removed from str
 **/

void removeTrailingNewline(char* str, int len)
{
    int removed, i;

    removed = FALSE;
    i = len;

    /* Check if string is valid */
    if (str && ! stringCompare(str, ""))
    {
        /* While newline is not yet removed and not at the start of string */
        while (! removed && --i >= 0)
        {
            if (str[i] == '\n')
            {
                str[i] = '\0';
                removed = TRUE;
            }
        }
    }
}

/**
 * Name:     countWords
 * Purpose:
 *     Count the number of words in string. Useful for either scanning
 *     the contents of a string to variables, or checking if string
 *     contains the right amount of inputs we're expecting.
 *
 * Parameters:
 *   - str : A string
 *
 * Returns:  int
 * Assertions:
 *     Assumptions:
 *         str is a valid string that has no leading
 *         or trailing whitespaces
 *     Results:
 *         Returns number of words of string
 **/

int countWords(char* str)
{
    int len, count, spaceCount, i;

    len = 0;
    count = 0;
    spaceCount = 0;
    i = 0;

    /* If string is valid */
    if (str && ! stringCompare(str, ""))
    {
        len = strlen(str) + 1;

        /* Count number of whitespaces */
        while (str[i] != '\0' && i < len)
        {
            if (isspace(str[i]))
            {
                spaceCount++;
            }
            i++;
        }

        /* Check if string is all whitespace */
        if (spaceCount != (len - 1))
        {
            i = 0;

            /* From the start of the string, count words */
            while (str[i] != '\0' && i < len)
            {
                /* If whitespace, find the index for the next word */
                if (isspace(str[i]))
                {
                    count++;
                    while (isspace(str[i]) && i++ < len);
                }
                i++;
            }
            count++;
        }
        else
        {
            count = 0;
        }
    }

    return count;
}

/**
 * Name:     trim
 * Purpose:
 *     Remove leading and trailing whitespaces from string. Useful for
 *     cleaning up a string.
 *
 * Parameters:
 *   - str : A pointer to a string
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         str is a valid string in stored in heap
 *     Results:
 *         Leading and trailing whitespace is removed from string
 **/

void trim(char** str)
{
    int start, end, len, newLen, i;
    char* tempStr;

    start = 0;
    end = 0;
    len = 0;
    newLen = 0;
    i = 0;
    tempStr = NULL;

    /* Check if str is a valid string */
    if (*str && ! stringCompare(*str, ""))
    {
        len = strlen(*str) + 1;

        /* Count the number of leading whitespace */
        while (isspace((*str)[i]) && ++i < len)
        {
            start++;
        }

        /* If the string is all white space */
        if (start == (len - 1))
        {
            freePtr((void**)str);
            initStringWithContents(str, "");
        }
        else
        {
            /* Minus 2 because of array index and null terminator */
            i = len - 2;

            /* Count number of trailing whitespaces */
            while (isspace((*str)[i]) && --i >= 0)
            {
                end++;
            }

            /* Init a new string with new size */
            newLen = len - (start + end);
            initString(&tempStr, newLen);

            /* Copy the contents of the string, starting at start */
            strncpy(tempStr, (*str) + start, newLen - 1);

            /**
             * Reallocate the contents of the strings back to the
             * original str variable
             **/
            freePtr((void**)str);
            initStringWithContents(str, tempStr);

            /* Garbage collection */
            freePtr((void**)&tempStr);
        }
    }
}

/**
 * Name:     printStringArray
 * Purpose:
 *     Print each element in a string array. Useful for printing out
 *     the contents of a string array.
 *
 * Parameters:
 *   - format : The format string to print
 *   - strArr : An array of strings
 *   - len    : An int for the number or elements in strArr
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         strArr is a valid string array
 *     Results:
 *         Contents of strArr is printed
 **/

void printStringArray(char* format, char** strArr, int len)
{
    int i;

    /* Check if strArr is a valid pointer */
    if (strArr)
    {
        /* Print each element in strArr to stdout */
        for (i = 0; i < len; i++)
        {
            fprintf(stdout, format, strArr[i]);
        }
    }
}
