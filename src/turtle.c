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
#define LOG_ERR "Error writing to log"

int main(int argc, char** argv)
{
    int returnCode, rows, cols;
    char* filename;
    char** fileContents;

    returnCode = 0;
    rows = 0;
    cols = 0;

    filename = NULL;
    fileContents = NULL;

    if (checkArgs(argc, argv))
    {
        if (processArgs(argv[1], &filename, &returnCode))
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

int processArgs(char* arg, char** filename, int* returnCode)
{
    if (stringCompare(arg, "-h") ||
        stringCompare(arg, "--help"))
    {
        printUsage();
        *returnCode = RETURN_OK;
    }
    else if (stringCompare(arg, "--version"))
    {
        printVersion();
        *returnCode = RETURN_OK;
    }
    else
    {
        initStringWithContents(filename, arg);
        *returnCode = TRUE;
    }

    return *returnCode;
}

void processCommands(char** commandArr, int numCommands, int* returnCode)
{
    FILE* logFile;

    PlotFunc funcCommand;
    void* plotData;
    int i, errLine;

    char tempStr[8];
    char* logLine;

    double x1, y1, x2, y2;

    double angle;
#ifndef SIMPLE
    int fg, bg;
#endif
    char pat;

    double tempDouble;
#ifndef SIMPLE
    int tempInt;
#endif
    char tempChar;

    logFile = NULL;
    funcCommand = NULL;
    plotData = NULL;
    i = 0;
    errLine= 0;

    memset(tempStr, '\0', 8);
    logLine = NULL;

    x1 = 0.0;
    y1 = 0.0;

    x2 = 0.0;
    y2 = 0.0;

    angle = 0.0;
#ifndef SIMPLE
    fg = 7;
    bg = 0;
#endif
    pat = '+';

    if (prepareCommands(commandArr, numCommands, &errLine))
    {
        logFile = fopen("graphics.log", "a");
        printToFile(logFile, "%s\n", "---", LOG_ERR);
        clearScreen();

        do
        {
            tempDouble = 0.0;
#ifndef SIMPLE
            tempInt = 0;
#endif
            tempChar = '\0';

            funcCommand = NULL;
            plotData = NULL;

            freePtr((void**)&logLine);

            if (sscanf(commandArr[i], "%s", tempStr) == 1)
            {
                if (stringCompare(tempStr, "ROTATE"))
                {
                    sscanf(commandArr[i], "%s %lf", tempStr, &tempDouble);
                    angle += tempDouble;

                    if (DOUBLE_CHECK(0.0, angle))
                    {
                        angle += 360.0;
                    }
                    else if (DOUBLE_CHECK(angle, 360.0))
                    {
                        angle -= 360.0;
                    }
                }
                else if (stringCompare(tempStr, "MOVE"))
                {
                    sscanf(commandArr[i], "%s %lf", tempStr, &tempDouble);
                    calcNewPosition(&x1, &y1, &x2, &y2, angle, tempDouble);

                    initString(&logLine, 47);
                    sprintf(logLine, "MOVE (%8.3f, %8.3f)-(%8.3f, %8.3f)",
                                            x1, y1, x2, y2);

                    funcCommand = &doNothing;
                    plotData = NULL;
                }
                else if (stringCompare(tempStr, "DRAW"))
                {
                    sscanf(commandArr[i], "%s %lf", tempStr, &tempDouble);
                    calcNewPosition(&x1, &y1, &x2, &y2, angle, tempDouble);

                    initString(&logLine, 47);
                    sprintf(logLine, "DRAW (%8.3f, %8.3f)-(%8.3f, %8.3f)",
                                            x1, y1, x2, y2);

                    funcCommand = &putChar;
                    plotData = &pat;
                }
#ifndef SIMPSIMPLE
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

                if (funcCommand)
                {
                    printToFile(logFile, "%s\n", logLine, LOG_ERR);
                    line(
                        (int)x1, (int)y1, (int)x2 - 1, (int)y2,
                        funcCommand, plotData
                    );
                    freePtr((void**)&logLine);
                }
            }
        } while (++i < numCommands);

        fprintf(stdout, "%s", "\033[0m");
        penDown();
        fclose(logFile);
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
}

void doNothing(void* a) {}
void putChar(void* voidPtr) { fputc(*(char*)voidPtr, stdout); }

int prepareCommands(
    char** commandArr,
    int numCommands,
    int* errLine)
{
    int commandValid, tempStrLen, i;

    double real;
    int integer;
    char pat;

    char tempStr[8];
    char* origStr;

    commandValid = FALSE;
    origStr = NULL;
    tempStrLen = 0;
    i = 0;

    do
    {
        commandValid = FALSE;
        real = 0.0;
        integer = 0;
        pat = '\0';

        initStringWithContents(&origStr, commandArr[i]);
        trim(&(commandArr[i]));

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
             * be a alpha character, to uppercase
             **/
            sscanf(commandArr[i], "%s", tempStr);
            tempStrLen = strlen(tempStr);
            upperRange(commandArr[i], tempStrLen);
            strncpy(tempStr, commandArr[i], tempStrLen);

            if (stringCompare(tempStr, "ROTATE") &&
                sscanf(commandArr[i], "%s %lf", tempStr, &real) == 2 &&
                doubleBoundaryCheck(real, -360.0, 360.0))
            {
                commandValid = TRUE;
            }
            else if ((stringCompare(tempStr, "MOVE") ||
                      stringCompare(tempStr, "DRAW")) &&
                      sscanf(commandArr[i], "%s %lf", tempStr, &real) == 2 &&
                      doubleBoundaryCheck(real, 0.0, -1.0))
            {
                commandValid = TRUE;
            }
            else if (stringCompare(tempStr, "FG") &&
                     sscanf(commandArr[i], "%s %d", tempStr, &integer) == 2 &&
                     integerBoundaryCheck(integer, 0, 15))
            {
                commandValid = TRUE;
            }
            else if (stringCompare(tempStr, "BG") &&
                     sscanf(commandArr[i], "%s %d", tempStr, &integer) == 2 &&
                     integerBoundaryCheck(integer, 0, 7))
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
        else if (stringCompare(commandArr[i], ""))
        {
            commandValid = TRUE;
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
        "Usage: turtle [FILE]",
        "Draw a graphic from commands in FILE",
        "Example: turtle ./picture.txt",
        "",
        "Valid commands:",
        "",
        "    +---------+-------+-----------------------+",
        "    | Command | Type  | Range                 |",
        "    +---------+-------+-----------------------+",
        "    | ROTATE  | float | -360 to 360 inclusive |",
        "    | MOVE    | float | Positive              |",
        "    | DRAW    | float | Positive              |",
        "    | FG      | int   | 0 to 15 inclusive     |",
        "    | BG      | int   | 0 to 7 inclusive      |",
        "    | PATTERN | char  | Any character         |",
        "    +---------+-------+-----------------------+",
        "",
        "Exit values:",
        "",
        "    0 - No errors",
        "    1 - Invalid arguments",
        "    2 - Invalid file",
        "    3 - Invalid command in file",
        ""
    };

    printStringArray("%s\n", usageMsg, 24);
}

void printVersion(void)
{
    char* versionMsg[] = {
        "turtle: A terminal drawing program",
        "Written by Julian Heng (19473701)",
        "",
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

    printStringArray("%s\n", versionMsg, 3);
    fprintf(stdout, "%s %s@%s\n", versionMsg[3], USER, HOST);
    fprintf(stdout, "%s %s\n", versionMsg[4], COMPILE_DATE);
    fprintf(stdout, "%s %s\n", versionMsg[5], MOD_DATE);
}
