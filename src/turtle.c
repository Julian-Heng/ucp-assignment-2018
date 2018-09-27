#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"
#include "fileIO.h"
#include "effects.h"
#include "turtle.h"

#define FALSE 0
#define TRUE !FALSE

int main(int argc, char** argv)
{
    int returnCode = 0;

    char* filename;
    char** fileContents;

    int rows, cols;

    filename = NULL;
    fileContents = NULL;
    rows = 0;
    cols = 0;

    if (checkArgs(argc, argv))
    {
        if (processArgs(argc, argv, &filename) &&
            readFileToArray(filename, &fileContents, &rows, &cols))
        {
            processCommands(fileContents, rows);
        }
        else
        {
            returnCode = 2;
        }
    }
    else
    {
        printUsage();
        returnCode = 1;
    }

    freeArray((void***)&fileContents, rows);
    freePtr((void**)&filename);

    return returnCode;
}

int checkArgs(int argc, char** argv)
{
    int isValid = FALSE;
    if (argc > 1)
    {
        isValid = TRUE;
    }
    return isValid;
}

int processArgs(int argc, char** argv, char** filename)
{
    int returnCode = 1;

    if (stringCompare(argv[1], "-h") ||
        stringCompare(argv[1], "--help"))
    {
        printUsage();
        returnCode = 0;
    }
    else if (stringCompare(argv[1], "--version"))
    {
        printVersion();
        returnCode = 0;
    }
    else
    {
        initStringWithContents(filename, argv[1]);
    }

    return returnCode;
}

void processCommands(char** commandArr, int numCommands)
{
    int errLine = 0;
    if (prepareCommands(commandArr, numCommands, &errLine))
    {
        printStringArray("%s\n", commandArr, numCommands);
    }
    else
    {
        fprintf(stderr, "%s %d\n", "Invalid command on line", errLine);
    }
}

int prepareCommands(char** commandArr, int numCommands, int* errLine)
{
    int commandValid = FALSE;
    int i;

    double angle;
    double length;
    int color;
    char pat;
    char buff[256];

    i = 0;

    do
    {
        commandValid = FALSE;
        resetVariables(&angle, &length, &color, &pat);
        upper(commandArr[i]);

        if (strstr(commandArr[i], "ROTATE") &&
            sscanf(commandArr[i], "%s %lf", buff, &angle) == 2 &&
            doubleBoundaryCheck(angle, -360.0, 360.0))
        {
            commandValid = TRUE;
        }
        else if ((strstr(commandArr[i], "MOVE") ||
                 (strstr(commandArr[i], "DRAW"))) &&
                 sscanf(commandArr[i], "%s %lf", buff, &length) == 2 &&
                 doubleBoundaryCheck(length, 0.0, -1.0))
        {
            commandValid = TRUE;
        }
        else if (strstr(commandArr[i], "FG") &&
                 sscanf(commandArr[i], "%s %d", buff, &color) == 2 &&
                 integerBoundaryCheck(color, 0, 15))
        {
            commandValid = TRUE;
        }
        else if (strstr(commandArr[i], "BG") &&
                 sscanf(commandArr[i], "%s %d", buff, &color) == 2 &&
                 integerBoundaryCheck(color, 0, 7))
        {
            commandValid = TRUE;
        }
        else if (strstr(commandArr[i], "PATTERN") &&
                 sscanf(commandArr[i], "%s %c", buff, &pat) == 2 &&
                 pat != ' ')
        {
            commandValid = TRUE;
        }
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
        "    +---------+-------+--------------------+\n",
        "    | Command | Type  | Range              |\n",
        "    +---------+-------+--------------------+\n",
        "    | ROTATE  | float | 0 to 360 inclusive |\n",
        "    | MOVE    | float | Positive           |\n",
        "    | DRAW    | float | Positive           |\n",
        "    | FG      | int   | 0 to 15 inclusive  |\n",
        "    | BG      | int   | 0 to 7 inclusive   |\n",
        "    | PATTERN | char  | Any character      |\n",
        "    +---------+-------+--------------------+\n",
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
