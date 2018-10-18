/**
 * Filename: tools.c
 * Author:   Julian Heng (19473701)
 * Purpose:  Provide utility functions to other files
 *           that includes tools.h. Such tools include
 *           mallocing functions, freeing functions,
 *           string manipulation and number comparisons
 *
 * Last Modified: 2018-10-18T11:32:24+08:00
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"

#define FALSE 0
#define TRUE !FALSE

/* Constants */
#define TOLERANCE 0.0000001
#define PI 3.14159265358979323846

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

    /* If malloc did not fail and contents is a valid pointer */
    if (*str && contents)
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
 *         None
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
 *         None
 *     Results:
 *         Returns TRUE (1) or FALSE (0)
 **/

int stringCompare(char* str1, char* str2)
{
    int returnCode;

    /* If string matches, return true */
    returnCode = strcmp(str1, str2) == 0 ? TRUE : FALSE;

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
            if (intBound(str[i], 'a', 'z'))
            {
                str[i] -= 32;
            }
            i++;
        }
    }
}

/**
 * Name:     intBound
 * Purpose:
 *     Check if an integer is within the bounds of two other integers
 *
 * Parameters:
 *   - i   : An integer to check
 *   - low : An integer for the lower boundaries
 *   - up  : An integer for the upper boundaries
 *
 * Returns: int / boolean
 * Asertions:
 *     Assumptions:
 *         None
 *     Result:
 *         Returns TRUE (1) or FALSE (0)
 **/

int intBound(int i, int low, int up)
{
    int inRange;
    inRange = ((i >= low) && (up >= i)) ? TRUE : FALSE;
    return inRange;
}

/**
 * Name:     doubleBound
 * Purpose:
 *     Check if a double is within the bounds of two other doubles
 *
 * Parameters:
 *   - i   : A double to check
 *   - low : A double for the lower boundaries
 *   - up  : A double for the upper boundaries
 *
 * Returns: int / boolean
 * Asertions:
 *     Assumptions:
 *         None
 *     Result:
 *         Returns TRUE (1) or FALSE (0)
 **/

int doubleBound(double i, double low, double up)
{
    int inRange;
    inRange = (doubleCheck(i, low) && doubleCheck(up, i)) ? TRUE : FALSE;
    return inRange;
}

/**
 * Name:     doubleCompare
 * Purpose:
 *     Compares two doubles. Needed due to how doubles are not precise
 *     due to floating points. Inherited from Java.
 *
 * Parameters:
 *   - num1 : First double
 *   - num2 : Second double to compare with
 *
 * Returns:  int / boolean
 * Assertions:
 *     Assumptions:
 *         None
 *     Results:
 *         Returns TRUE (1) or FALSE (0)
 **/

int doubleCompare(double num1, double num2)
{
    int isEqual;

    /* Check the double's tolerance value after getting the difference */
    isEqual = (doubleAbs(num1 - num2) < TOLERANCE) ? TRUE : FALSE;

    return isEqual;
}

/**
 * Name:     doubleCheck
 * Purpose:
 *     Check if one double is larger or equal to another double.
 *     Needed for checking the range of a double. Inherited from
 *     Java.
 *
 * Parameters:
 *   - num1 : First double
 *   - num2 : Second double to compare with
 *
 * Returns:  int / boolean
 * Assertions:
 *     Assumptions:
 *         None
 *     Results:
 *         Returns TRUE (1) or FALSE (0)
 **/

int doubleCheck(double num1, double num2)
{
    int isLargerThan;

    /* Check if num1 is larger or equal to num2 */
    isLargerThan = (num1 > num2 || doubleCompare(num1, num2)) ? TRUE : FALSE;

    return isLargerThan;
}

/**
 * Name:     doubleAbs
 * Purpose:
 *     Get the absolute value of a double.
 *
 * Parameters:
 *   - num : A double to find the absolute value
 *
 * Returns:  double
 * Assertions:
 *     Assumptions:
 *         None
 *     Results:
 *         Returns absolute value of num
 **/

double doubleAbs(double num)
{
    /* If num is negative, get negative */
    if (num < 0.0)
    {
        num = -num;
    }

    return num;
}

/**
 * Name:     doubleMod
 * Purpose:
 *     Compares two doubles. Needed due to how doubles are not precise
 *     due to floating points. Inherited from Java.
 *
 * Parameters:
 *   - num    : The double to be mod with
 *   - divide : The double to be divided with
 *
 * Returns:  double
 * Assertions:
 *     Assumptions:
 *         None
 *     Results:
 *         Returns the modulus of num by divide
 **/

double doubleMod(double num, double divide)
{
    /* While num is more than divide */
    if (! doubleCompare(divide, 0.0))
    {
        while (doubleCheck(num -= divide, divide));
    }

    return num;
}

/**
 * Name:     doubleRound
 * Purpose:
 *     Round a double to the nearest integer
 *
 * Parameters:
 *   - num : A double to be rounded
 *
 * Returns:  int
 * Assertions:
 *     Assumptions:
 *         None
 *     Results:
 *         Returns TRUE (1) or FALSE (0)
 **/

int doubleRound(double num)
{
    int rounded;
    rounded = (num > 0.0) ? (int)(num + 0.5) : (int)(num - 0.5);
    return rounded;
}

/**
 * Name:     degToRad
 * Purpose:
 *     Converts degrees to radians. Needed for calling trigonmetric
 *     functions.
 *
 * Parameters:
 *   - deg : The angle in degrees
 *
 * Returns:  double
 * Assertions:
 *     Assumptions:
 *         None
 *     Results:
 *         Returns the angle in radians
 **/

double degToRad(double deg)
{
    double rad;
    rad = deg * (PI / 180);
    return rad;
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
            if (str[i] == ' ')
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
                if (str[i] == ' ')
                {
                    count++;
                    while (str[i] == ' ' && i++ < len);
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
        while ((*str)[i] == ' ' && ++i < len && ++start);

        /* If the string is all white space */
        if (start == (len - 1))
        {
            freePtr((void**)str);
            initStringWithContents(str, "");
        }
        else
        {
            /**
             * Set i to the end of the string
             * Minus 2 because of array index and null terminator
             **/
            i = len - 2;

            /* Count number of trailing whitespaces */
            while ((*str)[i] == ' ' && --i >= 0 && ++end);

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
