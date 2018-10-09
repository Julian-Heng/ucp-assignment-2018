#include <stdio.h>
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
#define RETURN_ERROR_WRITE 3
#define RETURN_INVALID_CMD 4

#define MIN_ARGS 1
#define MAX_CMD_LENGTH 8
#define LOG_LENGTH 43

#define LOG_FILENAME "graphics.log"
#define SPLIT "---"
#define LOG_FORMAT "%s (%7.3f, %7.3f)-(%7.3f, %7.3f)"
#define LOG_ERR "Error writing to log"

int main(int argc, char** argv)
{
    LinkedList* commandList;

    char* filename;
    int returnCode;

    commandList = NULL;
    filename = NULL;
    returnCode = RETURN_OK;

    if (checkArgs(argc, argv))
    {
        if (processArgs(argv[1], &filename))
        {
            commandList = readFileToList(filename);

            if (commandList)
            {
                if (validateList(commandList))
                {
                    processList(commandList);
                }
                else
                {
                    returnCode = RETURN_INVALID_CMD;
                }

                clearListMalloc(&commandList);
            }
            else
            {
                returnCode = RETURN_INVALID_FILE;
            }

            freePtr((void**)&filename);
        }
    }
    else
    {
        printUsage();
        returnCode = RETURN_INVALID_ARGS;
    }

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

int validateList(LinkedList* list)
{
    LinkedListNode* node;
    int commandValid, lineNum;

    double real;
    int integer;
    char pat;

    char tempStr[MAX_CMD_LENGTH];
    char* command;

    node = list -> head;
    commandValid = FALSE;
    lineNum = 0;

    real = 0.0;
    integer = 0;
    pat = '\0';

    memset(tempStr, '\0', MAX_CMD_LENGTH);

    while (node)
    {
        lineNum++;
        commandValid = FALSE;
        memset(tempStr, '\0', MAX_CMD_LENGTH);
        initStringWithContents(&command, node -> value);
        node = node -> next;

        real = 0.0;
        integer = 0;
        pat = 0;

        trim(&command);

        if (countWords(command) == 2)
        {
            sscanf(command, "%s", tempStr);
            upper(tempStr);

            if (stringCompare(tempStr, "ROTATE") &&
                sscanf(command, "%s %lf", tempStr, &real) == 2 &&
                DOUBLE_BOUND(real, -360.0, 360.0))
            {
                commandValid = TRUE;
            }
            else if ((stringCompare(tempStr, "MOVE") ||
                      stringCompare(tempStr, "DRAW")) &&
                      sscanf(command, "%s %lf", tempStr, &real) == 2 &&
                      DOUBLE_CHECK(real, 0.0))
            {
                commandValid = TRUE;
            }
            else if (stringCompare(tempStr, "FG") &&
                     sscanf(command, "%s %d", tempStr, &integer) == 2 &&
                     INT_BOUND(integer, 0, 15))
            {
                commandValid = TRUE;
            }
            else if (stringCompare(tempStr, "BG") &&
                     sscanf(command, "%s %d", tempStr, &integer) == 2 &&
                     INT_BOUND(integer, 0, 7))
            {
                commandValid = TRUE;
            }
            else if (stringCompare(tempStr, "PATTERN") &&
                     sscanf(command, "%s %c", tempStr, &pat) == 2 &&
                     pat != ' ')
            {
                commandValid = TRUE;
            }
            else
            {
                printLineError(command, lineNum);
            }
        }
        else if (stringCompare(command, ""))
        {
            commandValid = TRUE;
        }
        else
        {
            printLineError(command, lineNum);
        }

        freePtr((void**)&command);
    }

    return commandValid;
}

void printLineError(char* line, int lineNum)
{
    fprintf(stderr, "%s %d: ", "Invalid command on line", lineNum);
    fprintf(stderr, "%s\n", line);
}

void processList(LinkedList* list)
{
    LinkedListNode* node;

    FILE* logFile;
    int writeOK;
    char* logLine;

    PlotFunc plotter;
    void* data;

    char tempStr[MAX_CMD_LENGTH];
    char* command;

    double x1, y1, x2, y2, x3, y3;
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

    node = NULL;

    logFile = fopen(LOG_FILENAME, "a");
    writeOK = TRUE;
    logLine = NULL;

    plotter = NULL;
    data = NULL;

    memset(tempStr, '\0', MAX_CMD_LENGTH);
    command = NULL;

    x1 = 0.0;
    y1 = 0.0;

    x2 = 0.0;
    y2 = 0.0;

    x3 = 0.0;
    y3 = 0.0;

    angle = 0.0;
#ifndef SIMPLE
    fg = 7;
    bg = 0;
#endif
    pat = '+';

    tempDouble = 0.0;
#ifndef SIMPLE
    tempInt = 0;
#endif
    tempChar = '\0';

    if (! logFile)
    {
        printFileError("Error opening", LOG_FILENAME);
    }
    else
    {
        writeOK = appendToFile(logFile, "%s\n", SPLIT, LOG_ERR);
        clearScreen();
        node = list -> head;

        if (! writeOK)
        {
            printFileError(LOG_ERR, "");
        }

        while (node && writeOK)
        {
            memset(tempStr, '\0', MAX_CMD_LENGTH);

            command = node -> value;
            node = node -> next;

            tempDouble = 0.0;
#ifndef SIMPLE
            tempInt = 0;
#endif
            tempChar = '\0';

            plotter = NULL;
            data = NULL;

            sscanf(command, "%s", tempStr);
            upper(tempStr);

            if (stringCompare(tempStr, "ROTATE"))
            {
                sscanf(command, "%s %lf", tempStr, &tempDouble);
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
                sscanf(command, "%s %lf", tempStr, &tempDouble);
                calcNewPosition(&x1, &y1, &x2, &y2, angle, tempDouble - 1);

                plotter = doNothing;
                data = NULL;
            }
            else if (stringCompare(tempStr, "DRAW"))
            {
                sscanf(command, "%s %lf", tempStr, &tempDouble);
                calcNewPosition(&x1, &y1, &x2, &y2, angle, tempDouble - 1);

                plotter = putChar;
                data = &pat;
            }
#ifndef SIMPLE
            else if (stringCompare(tempStr, "FG"))
            {
                sscanf(command, "%s %d", tempStr, &tempInt);
                fg = tempInt;
                setFgColour(fg);
            }
            else if (stringCompare(tempStr, "BG"))
            {
                sscanf(command, "%s %d", tempStr, &tempInt);
                bg = tempInt;
                setBgColour(bg);
            }
#endif
            else if (stringCompare(tempStr, "PATTERN"))
            {
                sscanf(command, "%s %c", tempStr, &tempChar);
                pat = tempChar;
            }

            if (plotter)
            {
                line(
                    (int)x1, (int)y1, (int)x2, (int)y2,
                    plotter, data
                );

                x3 = x1;
                y3 = y1;

                calcNewPosition(&x1, &y1, &x2, &y2, angle, 1.0);

                initString(&logLine, LOG_LENGTH);
                upper(tempStr);
                sprintf(logLine, LOG_FORMAT, tempStr, x3, y3, x2, y2);
#ifdef DEBUG
                fprintf(stderr, "%s\n", logLine);
#endif

                writeOK = appendToFile(logFile, "%s\n", logLine, LOG_ERR);

                if (! writeOK)
                {
                    printFileError(LOG_ERR, "");
                }

                freePtr((void**)&logLine);
            }
        }

        fprintf(stdout, "%s", "\033[0m");
        penDown();
        fclose(logFile);
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

    xDelta = 0.0;
    yDelta = 0.0;

    if (fmod(angle, 90.0) == 0.0)
    {
        if (doubleCompare(angle, 0.0) || doubleCompare(angle, 360.0))
        {
            xDelta = length;
        }
        else if (doubleCompare(angle, 90.0))
        {
            yDelta = length;
        }
        else if (doubleCompare(angle, 180.0))
        {
            xDelta = -length;
        }
        else if (doubleCompare(angle, 270.0))
        {
            yDelta = -length;
        }
        else
        {
            xDelta = length * cos(DEG_TO_RAD(angle));
            yDelta = length * sin(DEG_TO_RAD(angle));
        }
    }
    else
    {
        xDelta = length * cos(DEG_TO_RAD(angle));
        yDelta = length * sin(DEG_TO_RAD(angle));
    }

    *x1 = *x2;
    *x2 += xDelta;

    *y1 = *y2;
    *y2 += yDelta;
}

void doNothing(void* a) {}
void putChar(void* voidPtr) { fputc(*(char*)voidPtr, stdout); }

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
        "    3 - Error writing to log file",
        "    4 - Invalid command in file",
        ""
    };

    printStringArray("%s\n", usageMsg, GET_STACK_ARRAY_SIZE(usageMsg));
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
