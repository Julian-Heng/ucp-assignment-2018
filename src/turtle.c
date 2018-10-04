#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tools.h"
#include "linkedList.h"
#include "fileIO.h"
#include "effects.h"
#include "turtle.h"

#ifndef COMPILER_INFO
#define COMPILER_INFO "unknown"
#endif

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

#define FALSE 0
#define TRUE !FALSE

#define RETURN_OK 0
#define RETURN_INVALID_ARGS 1
#define RETURN_INVALID_FILE 2
#define RETURN_INVALID_CMD 3

#define MIN_ARGS 1
#define MAX_CMD_LENGTH 8
#define LOG_LENGTH 47

#define LOG_FILENAME "graphics.log"
#define SPLIT "---"
#define MOVE_LOG "MOVE (%8.3f, %8.3f)-(%8.3f, %8.3f)"
#define DRAW_LOG "DRAW (%8.3f, %8.3f)-(%8.3f, %8.3f)"
#define LOG_ERR "Error writing to log"

int main(int argc, char** argv)
{
    LinkedList* commandList;

    int returnCode, rows, cols;
    char* filename;
    char** fileContents;

    commandList = NULL;

    returnCode = 0;
    rows = 0;
    cols = 0;

    filename = NULL;
    fileContents = NULL;

    if (checkArgs(argc, argv))
    {
        if (processArgs(argv[1], &filename))
        {
            if (readFileToArray(filename, &fileContents, &rows, &cols))
            {
                processCommands(
                    fileContents,
                    rows,
                    &commandList,
                    &returnCode
                );
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
    clearListStack(&commandList);

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

int processArgs(char* arg, char** filename)
{
    int continueProgram;

    continueProgram = TRUE;

    if (stringCompare(arg, "-h") ||
        stringCompare(arg, "--help"))
    {
        printUsage();
        continueProgram = FALSE;
    }
    else if (stringCompare(arg, "--version"))
    {
        printVersion();
        continueProgram = FALSE;
    }
    else
    {
        initStringWithContents(filename, arg);
        if (! *filename)
        {
            continueProgram = FALSE;
        }
    }

    return continueProgram;
}

void processCommands(
    char** commandArr,
    int numCommands,
    LinkedList** commandList,
    int* returnCode)
{
    FILE* logFile;

    PlotFunc funcCommand;
    void* plotData;
    int errLine;

    LinkedListNode* nodeFromList;
    char* commandFromList;

    char tempStr[MAX_CMD_LENGTH];
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
    errLine= 0;

    nodeFromList = NULL;
    commandFromList = NULL;

    memset(tempStr, '\0', MAX_CMD_LENGTH);
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

    *commandList = validateCommands(commandArr, numCommands, &errLine);

    if (*commandList)
    {
        logFile = fopen(LOG_FILENAME, "a");
        printToFile(logFile, "%s\n", SPLIT, LOG_ERR);
        clearScreen();

        nodeFromList = (*commandList) -> head;
        while (nodeFromList)
        {
            commandFromList = nodeFromList -> value;
            nodeFromList = nodeFromList -> next;

            tempDouble = 0.0;
#ifndef SIMPLE
            tempInt = 0;
#endif
            tempChar = '\0';

            funcCommand = NULL;
            plotData = NULL;

            freePtr((void**)&logLine);

            sscanf(commandFromList, "%s", tempStr);

            if (stringCompare(tempStr, "ROTATE"))
            {
                sscanf(commandFromList, "%s %lf", tempStr, &tempDouble);
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
                sscanf(commandFromList, "%s %lf", tempStr, &tempDouble);
                calcNewPosition(&x1, &y1, &x2, &y2, angle, tempDouble);

                initString(&logLine, LOG_LENGTH);
                sprintf(logLine, MOVE_LOG, x1, y1, x2, y2);

                funcCommand = &doNothing;
                plotData = NULL;
            }
            else if (stringCompare(tempStr, "DRAW"))
            {
                sscanf(commandFromList, "%s %lf", tempStr, &tempDouble);
                calcNewPosition(&x1, &y1, &x2, &y2, angle, tempDouble);

                initString(&logLine, LOG_LENGTH);
                sprintf(logLine, DRAW_LOG, x1, y1, x2, y2);

                funcCommand = &putChar;
                plotData = &pat;
            }
#ifndef SIMPLE
            else if (stringCompare(tempStr, "FG"))
            {
                sscanf(commandFromList, "%s %d", tempStr, &tempInt);
                fg = tempInt;
                setFgColour(fg);
            }
            else if (stringCompare(tempStr, "BG"))
            {
                sscanf(commandFromList, "%s %d", tempStr, &tempInt);
                bg = tempInt;
                setBgColour(bg);
            }
#endif
            else if (stringCompare(tempStr, "PATTERN"))
            {
                sscanf(commandFromList, "%s %c", tempStr, &tempChar);
                pat = tempChar;
            }

            if (funcCommand)
            {
#ifdef DEBUG
                fprintf(stderr, "%s\n", logLine);
#endif
                printToFile(logFile, "%s\n", logLine, LOG_ERR);
                if (doubleCompare(x2 - x1, 1.0) ||
                    doubleCompare(x2 - x1, 2.0))
                {
                    line(
                        (int)x1, (int)y1, (int)x2 - 1, (int)y2,
                        funcCommand, plotData
                    );
                }
                else
                {
                    line(
                        (int)x1, (int)y1, (int)x2, (int)y2,
                        funcCommand, plotData
                    );
                }

                freePtr((void**)&logLine);
            }
        }

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

LinkedList* validateCommands(
    char** commandArr,
    int numCommands,
    int* errLine)
{
    LinkedList* commandList;

    int commandValid, tempStrLen, i;

    double real;
    int integer;
    char pat;

    char tempStr[8];
    char* origStr;

    commandList = initList();
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

        if (countWords(commandArr[i]) == 2)
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
                insertLast(commandList, commandArr[i]);
                commandValid = TRUE;
            }
            else if ((stringCompare(tempStr, "MOVE") ||
                      stringCompare(tempStr, "DRAW")) &&
                      sscanf(commandArr[i], "%s %lf", tempStr, &real) == 2 &&
                      doubleBoundaryCheck(real, 0.0, -1.0))
            {
                insertLast(commandList, commandArr[i]);
                commandValid = TRUE;
            }
            else if (stringCompare(tempStr, "FG") &&
                     sscanf(commandArr[i], "%s %d", tempStr, &integer) == 2 &&
                     integerBoundaryCheck(integer, 0, 15))
            {
                insertLast(commandList, commandArr[i]);
                commandValid = TRUE;
            }
            else if (stringCompare(tempStr, "BG") &&
                     sscanf(commandArr[i], "%s %d", tempStr, &integer) == 2 &&
                     integerBoundaryCheck(integer, 0, 7))
            {
                insertLast(commandList, commandArr[i]);
                commandValid = TRUE;
            }
            else if (stringCompare(tempStr, "PATTERN") &&
                     sscanf(commandArr[i], "%s %c", tempStr, &pat) == 2 &&
                     pat != ' ')
            {
                insertLast(commandList, commandArr[i]);
                commandValid = TRUE;
            }
            else
            {
                freePtr((void**)&commandArr[i]);
                initStringWithContents(&commandArr[i], origStr);
                clearListStack(&commandList);
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
            clearListStack(&commandList);
        }

        freePtr((void**)&origStr);
    } while (++i < numCommands && commandValid);

    *errLine = i;

    return commandList;
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
        "Compiler      :",
        "Compile by    :",
        "Compile time  :",
        "Last Modified :"
    };

    char* compiler = COMPILER_INFO;
    char* user = USER;
    char* host = HOST;
    char* compileDate = COMPILE_DATE;
    char* modDate = MOD_DATE;

    if (stringCompare(compiler, ""))
    {
        compiler = "unknown";
    }

    if (stringCompare(user, ""))
    {
        user = "unknown";
    }

    if (stringCompare(host, ""))
    {
        host = "unknown";
    }

    if (stringCompare(compileDate, ""))
    {
        compileDate = "unknown";
    }

    if (stringCompare(modDate, ""))
    {
        modDate = "unknown";
    }

    printStringArray("%s\n", versionMsg, 3);
    fprintf(stdout, "%s %s\n", versionMsg[3], compiler);
    fprintf(stdout, "%s %s@%s\n", versionMsg[4], user, host);
    fprintf(stdout, "%s %s\n", versionMsg[5], compileDate);
    fprintf(stdout, "%s %s\n", versionMsg[6], modDate);
}
