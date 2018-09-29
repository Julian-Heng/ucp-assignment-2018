#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tools.h"
#include "fileIO.h"
#include "effects.h"
#include "turtle.h"

#define FALSE 0
#define TRUE !FALSE

#define RETURN_OK 0
#define RETURN_INVALID_ARGS 1
#define RETURN_INVALID_FILE 2
#define RETURN_INVALID_CMD 3

#define MIN_ARGS 1

#define PI 3.14159265358979323846

#define DEG_TO_RAD(i) ((i) * (PI / 180.0))

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
        if (processArgs(argc, argv, &filename, &returnCode))
        {
            if (readFileToArray(filename, &fileContents, &rows, &cols))
            {
                processCommands(fileContents, rows, &returnCode);
            }
            else
            {
                returnCode = RETURN_INVALID_FILE;
            }
        }
    }
    else
    {
        printUsage();
        returnCode = RETURN_INVALID_ARGS;
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
        *returnCode = RETURN_OK;
    }
    else if (stringCompare(argv[1], "--version"))
    {
        printVersion();
        *returnCode = RETURN_OK;
    }
    else
    {
        initStringWithContents(filename, argv[1]);
        *returnCode = TRUE;
    }

    return *returnCode;
}

void processCommands(char** commandArr, int numCommands, int* returnCode)
{
    PlotFunc funcCommand;
    void* plotData;
    int i;
    int errLine;
    int callFunc;

    char tempStr[256];

    double x1, y1, x2, y2;

    double angle;
    int fg, bg;
    char pat;

    double tempDouble;
    int tempInt;
    char tempChar;

    funcCommand = NULL;
    plotData = NULL;
    errLine= 0;
    callFunc = FALSE;

    memset(tempStr, '\0', 256);

    x1 = 0.0;
    y1 = 0.0;

    x2 = 0.0;
    y2 = 0.0;

    angle = 0.0;
    fg = 7;
    bg = 0;
    pat = '+';

    if (prepareCommands(commandArr, numCommands, &errLine))
    {
        clearScreen();

        for (i = 0; i < numCommands; i++)
        {
            tempDouble = 0.0;
            tempInt = 0;
            tempChar = '\0';

            callFunc = FALSE;
            funcCommand = &doNothing;
            plotData = NULL;

            sscanf(commandArr[i], "%s", tempStr);

            if (stringCompare(tempStr, "ROTATE"))
            {
                sscanf(commandArr[i], "%s %lf", tempStr, &tempDouble);
                angle += tempDouble;
            }
            else if (stringCompare(tempStr, "MOVE"))
            {
                sscanf(commandArr[i], "%s %lf", tempStr, &tempDouble);

                calcNewPosition(&x1, &y1, &x2, &y2, angle, tempDouble);

                callFunc = TRUE;
                funcCommand = &doNothing;
                plotData = NULL;
            }
            else if (stringCompare(tempStr, "DRAW"))
            {
                sscanf(commandArr[i], "%s %lf", tempStr, &tempDouble);

                calcNewPosition(&x1, &y1, &x2, &y2, angle, tempDouble);

                callFunc = TRUE;
                funcCommand = &putChar;
                plotData = &pat;
            }
#ifndef SIMPLE
            else if (stringCompare(tempStr, "FG"))
            {
                sscanf(commandArr[i], "%s %d", tempStr, &tempInt);
                fg = tempInt;
                setFgColour(fg);
            }
            else if (stringCompare(tempStr, "BG"))
            {
                sscanf(commandArr[i], "%s %d", tempStr, &tempInt);
                bg = tempInt;
                setBgColour(bg);
            }
#endif
            else if (stringCompare(tempStr, "PATTERN"))
            {
                sscanf(commandArr[i], "%s %c", tempStr, &tempChar);
                pat = tempChar;
            }

            if (callFunc)
            {
                line(
                    (int)x1, (int)y1, (int)x2 - 1, (int)y2,
                    funcCommand, plotData
                );
            }
        }

        penDown();
        *returnCode = RETURN_OK;
    }
    else
    {
        fprintf(stderr, "%s %d: ", "Invalid command on line", errLine);
        fprintf(stderr, "%s\n", commandArr[errLine - 1]);
        *returnCode = RETURN_INVALID_CMD;
    }
}

void calcNewPosition(
    double* x1,
    double* y1,
    double* x2,
    double* y2,
    double angle,
    double length)
{
    double xDelta, yDelta;

    if (doubleCompare(angle, 0.0) || doubleCompare(angle, 360.0))
    {
        xDelta = length;
        yDelta = 0.0;
    }
    else if (doubleCompare(angle, 90.0))
    {
        xDelta = 0.0;
        yDelta = length;
    }
    else if (doubleCompare(angle, 180.0))
    {
        xDelta = -length;
        yDelta = 0.0;
    }
    else if (doubleCompare(angle, 270.0))
    {
        xDelta = 0.0;
        yDelta= -length;
    }
    else
    {
        xDelta = (length * cos(DEG_TO_RAD(angle)));
        yDelta = (length * sin(DEG_TO_RAD(angle)));
    }

    *x1 = *x2;
    *x2 += xDelta;

    *y1 = *y2;
    *y2 += yDelta;

    /*
    fprintf(stderr, "Length = %f\n", length);
    fprintf(stderr, "Cos(%f) = %f\n", angle, xDelta);
    fprintf(stderr, "Sin(%f) = %f\n", angle, yDelta);
    fprintf(stderr, "Delta x: %0.2f\nDelta y: %0.2f\n", xDelta, yDelta);
    fprintf(stderr, "(%d, %d) -> (%d, %d)\n\n",
        (int)*x1, (int)*y1, (int)*x2, (int)*y2);
        */
}

void doNothing(void* a) {}

void putChar(void* voidPtr)
{
    fputc(*(char*)voidPtr, stdout);
}

int prepareCommands(
    char** commandArr,
    int numCommands,
    int* errLine)
{
    int commandValid, tempStrLen, i;

    double angle;
    double length;
    int color;
    char pat;

    char tempStr[256];
    char* origStr;

    commandValid = FALSE;
    origStr = NULL;
    tempStrLen = 0;
    i = 0;

    do
    {
        commandValid = FALSE;
        angle = 0.0;
        length = 0.0;
        color = 0;
        pat = '\0';

        initStringWithContents(&origStr, commandArr[i]);

        if (countWhiteSpace(commandArr[i]) == 1)
        {
            /**
             * --[ACTIONS]-
             * 1. Get the command string and length
             * 2. Uppercase only the command string portion of the commands
             *   array
             * 3. Copy back from commands array to tempstr for comparison
             *
             * --[RATIONALE]--
             * Required for converting the command to uppercase in order
             * to accept mixed case commands
             *
             * Range is so that we do not change the pattern, which can
             * be a alpha character, to upper case
             **/
            sscanf(commandArr[i], "%s", tempStr);
            tempStrLen = strlen(tempStr);
            upperRange(commandArr[i], tempStrLen);
            strncpy(tempStr, commandArr[i], tempStrLen);

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
