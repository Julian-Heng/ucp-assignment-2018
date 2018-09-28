#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"
#include "fileIO.h"
#include "effects.h"
#include "turtle.h"

#define FALSE 0
#define TRUE !FALSE

#define INVALID_ARGS 1
#define FILE_NOT_FOUND 2

#define MIN_ARGS 1

int main(int argc, char** argv)
{
    int returnCode, rows, cols;
    char* filename;
    char** fileContents;

    returnCode = 0;
    filename = NULL;
    fileContents = NULL;
    rows = 0;
    cols = 0;

    if (checkArgs(argc, argv))
    {
        if (processArgs(argc, argv, &filename, &returnCode) &&
            readFileToArray(filename, &fileContents, &rows, &cols))
        {
            processCommands(fileContents, rows);
        }
        else
        {
            if (returnCode)
            {
                returnCode = FILE_NOT_FOUND;
            }
            else
            {
                returnCode = FALSE;
            }
        }
    }
    else
    {
        printUsage();
        returnCode = INVALID_ARGS;
    }

    freeArray((void***)&fileContents, rows);
    freePtr((void**)&filename);

    return returnCode;
}

int checkArgs(int argc, char** argv)
{
    int isValid;

    isValid = FALSE;

    if (argc > MIN_ARGS)
    {
        isValid = TRUE;
    }

    return isValid;
}

int processArgs(int argc, char** argv, char** filename, int* returnCode)
{
    if (stringCompare(argv[1], "-h") ||
        stringCompare(argv[1], "--help"))
    {
        printUsage();
        *returnCode = FALSE;
    }
    else if (stringCompare(argv[1], "--version"))
    {
        printVersion();
        *returnCode = FALSE;
    }
    else
    {
        initStringWithContents(filename, argv[1]);
        *returnCode = TRUE;
    }

    return *returnCode;
}

void processCommands(char** commandArr, int numCommands)
{
    int errLine;

    errLine= 0;

    if (prepareCommands(commandArr, numCommands, &errLine))
    {
        /* Actual Drawing */
    }
    else
    {
        fprintf(stderr, "%s %d: ", "Invalid command on line", errLine);
        fprintf(stderr, "%s\n", commandArr[errLine - 1]);
    }
}

int prepareCommands(
    char** commandArr,
    int numCommands,
    int* errLine)
{
    int commandValid, i;

    double angle;
    double length;
    int color;
    char pat;

    char tempStr[256];
    char* origStr;

    commandValid = FALSE;
    origStr = NULL;
    i = 0;

    do
    {
        commandValid = FALSE;
        resetVariables(&angle, &length, &color, &pat);

        initStringWithContents(&origStr, commandArr[i]);

        if (countWhiteSpace(commandArr[i]) == 1)
        {
            upper(commandArr[i]);
            sscanf(commandArr[i], "%s", tempStr);
            if ((stringCompare(tempStr, "ROTATE")) &&
                sscanf(commandArr[i], "%s %lf", tempStr, &angle) == 2 &&
                doubleBoundaryCheck(angle, -360.0, 360.0))
            {
                commandValid = TRUE;
            }
            else if ((stringCompare(tempStr, "MOVE") ||
                     (stringCompare(tempStr, "DRAW"))) &&
                     sscanf(commandArr[i], "%s %lf", tempStr, &length) == 2 &&
                     doubleBoundaryCheck(length, 0.0, -1.0))
            {
                commandValid = TRUE;
            }
            else if (stringCompare(tempStr, "FG") &&
                     sscanf(commandArr[i], "%s %d", tempStr, &color) == 2 &&
                     integerBoundaryCheck(color, 0, 15))
            {
                commandValid = TRUE;
            }
            else if (stringCompare(tempStr, "BG") &&
                     sscanf(commandArr[i], "%s %d", tempStr, &color) == 2 &&
                     integerBoundaryCheck(color, 0, 7))
            {
                commandValid = TRUE;
            }
            else if (stringCompare(tempStr, "PATTERN") &&
                     sscanf(commandArr[i], "%s %c", tempStr, &pat) == 2 &&
                     pat != ' ')
            {
                commandValid = TRUE;
            }
            else
            {
                freePtr((void**)&commandArr[i]);
                initStringWithContents(&commandArr[i], origStr);
            }
        }
        else
        {
            freePtr((void**)&commandArr[i]);
            initStringWithContents(&commandArr[i], origStr);
        }

        freePtr((void**)&origStr);
    } while (++i < numCommands && commandValid);

    *errLine = i;
    return commandValid;
}

void resetVariables(double* angle, double* length, int* color, char* pat)
{
    *angle = 0.0;
    *length = 0.0;
    *color = 0;
    *pat = '\0';
}

void printUsage(void)
{
    char* usageMsg[] = {
        "Usage: turtle [FILE]\n",
        "Draw a graphic from commands in FILE\n",
        "Example: turtle ./picture.txt\n",
        "\n",
        "Valid commands:\n",
        "\n",
        "    +---------+-------+-----------------------+\n",
        "    | Command | Type  | Range                 |\n",
        "    +---------+-------+-----------------------+\n",
        "    | ROTATE  | float | -360 to 360 inclusive |\n",
        "    | MOVE    | float | Positive              |\n",
        "    | DRAW    | float | Positive              |\n",
        "    | FG      | int   | 0 to 15 inclusive     |\n",
        "    | BG      | int   | 0 to 7 inclusive      |\n",
        "    | PATTERN | char  | Any character         |\n",
        "    +---------+-------+-----------------------+\n",
        "\n",
        "EOF"
    };

    printStringArrayUntilEOF("%s", usageMsg);
}

void printVersion(void)
{
    char* versionMsg[] = {
        "turtle: A terminal drawing program\n",
        "Written by Julian Heng (19473701)\n",
        "\n",
        "Compile by    :",
        "Compile time  :",
        "Last Modified :"
    };

    #ifndef USER
    #define USER "unknown"
    #endif

    #ifndef HOST
    #define HOST "unknown"
    #endif

    #ifndef COMPILE_DATE
    #define COMPILE_DATE "unknown"
    #endif

    #ifndef MOD_DATE
    #define MOD_DATE "unknown"
    #endif

    printStringArray("%s", versionMsg, 3);
    fprintf(stdout, "%s %s@%s\n", versionMsg[3], USER, HOST);
    fprintf(stdout, "%s %s\n", versionMsg[4], COMPILE_DATE);
    fprintf(stdout, "%s %s\n", versionMsg[5], MOD_DATE);
}
