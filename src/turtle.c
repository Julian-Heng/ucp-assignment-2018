/**
 * Filename: turtle.c
 * Author:   Julian Heng (19473701)
 * Purpose:  The main program. Draws a graphic from a list of
 *           commands in file
 *
 * Last Modified: 2018-10-16T02:38:09+08:00
 **/

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "tools.h"
#include "linkedList.h"
#include "fileIO.h"
#include "effects.h"
#include "turtle.h"

/* Setting fallback values to compiler defined string */
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

/* Define various return codes possible */
#define RETURN_OK 0
#define RETURN_INVALID_ARGS 1
#define RETURN_INVALID_FILE 2
#define RETURN_ERROR_WRITE 3
#define RETURN_INVALID_CMD 4

/* Prevent magic numbers */
#define MIN_ARGS 1
#define MAX_CMD_LENGTH 8

/* Actual log length is 43, but gcc complains about format overflow */
#define LOG_LENGTH 46

/* Log defines */
#define LOG_FILENAME "graphics.log"
#define SPLIT "---"
#define LOG_FORMAT "%4s (%7.3f, %7.3f)-(%7.3f, %7.3f)"
#define LOG_ERR "Error writing to log"

/**
 * Name:     turtle.c
 * Purpose:
 *     The main function start the program
 *
 * Arguments:
 *   - filename
 *
 * Exit codes:
 *   - 0 = success
 *   - 1 = invalid arguments
 *   - 2 = invalid file
 *   - 3 = invalid file write
 *   - 4 = invalid command in file
 **/

int main(int argc, char** argv)
{
    LinkedList* commandList;

    char* filename;
    int returnCode;

    commandList = NULL;
    filename = NULL;
    returnCode = RETURN_OK;

    /* Check if arguments are valid */
    if (checkArgs(argc, argv))
    {
        /* Set filename variable */
        if (processArgs(argv[1], &filename))
        {
            /* Read file to list */
            commandList = readFileToList(filename);

            /* If commandList has file commands */
            if (commandList)
            {
                /* Validate and execute list */
                if (validateList(commandList))
                {
                    processList(commandList);
                }
                else
                {
                    returnCode = RETURN_INVALID_CMD;
                }

                /* Garbage collection */
                clearListMalloc(&commandList);
            }
            else
            {
                returnCode = RETURN_INVALID_FILE;
            }
        }
    }
    else
    {
        /* Print program usage and exit */
        printUsage();
        returnCode = RETURN_INVALID_ARGS;
    }

    return returnCode;
}


/**
 * Name:     checkArgs
 * Purpose:
 *     Check if command line arguments are valid. If not, then exit
 *     the program
 *
 * Parameters:
 *   - argc : The number of command line arguments
 *   - argv : The command lines arguments
 *
 * Returns:  int / boolean
 * Assertions:
 *     Assumptions:
 *         None
 *     Result:
 *         Returns either TRUE (1) or FALSE (0)
 **/

int checkArgs(int argc, char** argv)
{
    int isValid;

    /**
     * If number of arguments is less than the expected
     * number of arguments.
     *
     * Expecting at least 2 arguments, program name and filename
     **/
    isValid = (argc > MIN_ARGS) ? TRUE : FALSE;

    return isValid;
}

/**
 * Name:     processArgs
 * Purpose:
 *     Determine what arguments are passed into program
 *
 * Parameters:
 *   - arg      : The single argument for the filename
 *   - filename : A string pointer for the filename of the input file
 *
 * Returns:  int / boolean
 * Assertions:
 *     Assumptions:
 *         None
 *     Result:
 *         filename variable will be set, or command line switches
 *         toggle specific actions
 **/

int processArgs(char* arg, char** filename)
{
    int continueProgram;

    continueProgram = TRUE;

    /* If argument is a command line switch */
    if (strstr(arg, "--"))
    {
        /* If help is needed */
        if (stringCompare(arg, "-h") ||
            stringCompare(arg, "--help"))
        {
            printUsage();
            continueProgram = FALSE;
        }
        else if (stringCompare(arg, "--version"))
        {
            /* If version info is needed */
            printVersion();
            continueProgram = FALSE;
        }
        else
        {
            /* If argument is invalid */
            printUsage();
            fprintf(stderr, "%s: ", "Invalid argument");
            fprintf(stderr, "%s\n\n", arg);
            continueProgram = FALSE;
        }
    }
    else
    {
        /* Set filename variable */
        *filename = arg;
    }

    return continueProgram;
}

/**
 * Name:     validateList
 * Purpose:
 *     Check if the command file is valid. Needed in order to draw
 *     the graphic without any errors.
 *
 * Parameters:
 *   - list : A LinkedList pointer containing the file contents
 *
 * Returns:  int / boolean
 * Assertions:
 *     Assumptions:
 *         list contains the file entirely
 *     Result:
 *         Returns either TRUE (1) or FALSE (0)
 **/

int validateList(LinkedList* list)
{
    LinkedListNode* node;
    int commandValid, lineNum;

    /* Temporary variables for reading a specific data type */
    double real;
    int integer;
    char pat;

    /* Temporary string for containing action in command */
    char tempStr[MAX_CMD_LENGTH];
    char* command;

    /* Start at the start of the file */
    node = list -> head;
    commandValid = TRUE;
    lineNum = 0;

    /* Reset variables to default */
    real = 0.0;
    integer = 0;
    pat = '\0';

    memset(tempStr, '\0', MAX_CMD_LENGTH);

    /* While there is a node and command is valid */
    while (node && commandValid)
    {
        /**
         * Increment line number count for printing error on a
         * specific line, if it occurs
         **/
        lineNum++;

        /* Reset variables for each iteration */
        commandValid = FALSE;
        memset(tempStr, '\0', MAX_CMD_LENGTH);

        /* Make a copy of the original line in file for manipulation */
        initStringWithContents(&command, node -> value);
        node = node -> next;

        real = 0.0;
        integer = 0;
        pat = 0;

        /* Remove leading and trailing whitespace for counting words */
        trim(&command);

        /* If command has exactly 2 words */
        if (countWords(command) == 2)
        {
            /* Get action from command and convert to uppercase */
            sscanf(command, "%s", tempStr);
            upper(tempStr);

            /* Check if action matches and the command parameter is valid */
            if (stringCompare(tempStr, "ROTATE") &&
                sscanf(command, "%s %lf", tempStr, &real) == 2 &&
                doubleBound(real, -360.0, 360.0))
            {
                /**
                 * Checks if command is rotate, and there's a second argument
                 * and angle is within -360 and 360 degrees
                 **/
                commandValid = TRUE;
            }
            else if ((stringCompare(tempStr, "MOVE") ||
                      stringCompare(tempStr, "DRAW")) &&
                      sscanf(command, "%s %lf", tempStr, &real) == 2 &&
                      doubleCheck(real, 0.0))
            {
                /**
                 * Checks if command is move or draw, and there's a second
                 * argument and command parameter is not negative
                 **/
                commandValid = TRUE;
            }
            else if (stringCompare(tempStr, "FG") &&
                     sscanf(command, "%s %d", tempStr, &integer) == 2 &&
                     intBound(integer, 0, 15))
            {
                /**
                 * Check if command is changing foreground, and there's a
                 * second argument and the foreground is within 0 and 15
                 * inclusive
                 **/
                commandValid = TRUE;
            }
            else if (stringCompare(tempStr, "BG") &&
                     sscanf(command, "%s %d", tempStr, &integer) == 2 &&
                     intBound(integer, 0, 7))
            {
                /**
                 * Same as foreground, except the range is between 0 and 7
                 * inclusive
                 **/
                commandValid = TRUE;
            }
            else if (stringCompare(tempStr, "PATTERN") &&
                     sscanf(command, "%s %c", tempStr, &pat) == 2 &&
                     pat != ' ')
            {
                /**
                 * Check if command is pattern, and there's a second argument
                 * and pattern is not a whitespace
                 **/
                commandValid = TRUE;
            }
            else
            {
                /* Format and print the line as an error message */
                printLineError(command, lineNum);
            }
        }
        else if (stringCompare(command, ""))
        {
            /* If command is empty, i.e is a newline */
            commandValid = TRUE;
        }
        else
        {
            /* Format and print the line as an error message */
            printLineError(command, lineNum);
        }

        /* Garbage collection */
        freePtr((void**)&command);
    }

    return commandValid;
}

/**
 * Name:     printLineError
 * Purpose:
 *     Format and print a line from the command list as an error message
 *
 * Parameters:
 *   - line    : The problematic line in file
 *   - lineNum : The problematic line number in file
 *
 * Returns:  None
 * Assertions:
 *     Assumptions:
 *         line is invalid from validateList
 *     Result:
 *         An error will be printed to stderr
 **/

void printLineError(char* line, int lineNum)
{
    fprintf(stderr, "%s %d: ", "Invalid command on line", lineNum);
    fprintf(stderr, "%s\n", line);
}

/**
 * Name:     processList
 * Purpose:
 *     Execute each commands in list
 *
 * Parameters:
 *   - list : A LinkedList pointer containing the file contents
 *
 * Returns:  void
 * Assertions:
 *     Assumptions:
 *         All commands in list is valid
 *     Result:
 *         A graphic form the commands in list will be drawn
 **/

void processList(LinkedList* list)
{
    LinkedListNode* node;

    /* Variables relating to log files */
    FILE* logFile;
    int writeOK;
    char logLine[LOG_LENGTH];

    /* Variables relating to plotting the line */
    PlotFunc plotter;
    void* data;

    /* Temporary string for containing action in command */
    char tempStr[MAX_CMD_LENGTH];
    char* command;

    /**
     * Variables relating to drawing the graphic
     * (x1, y1) and (x2, y2) are for calculating drawing coords
     * (x3, y3) and (x4, y4) are for passing to the line function
     * (x5, y5) is for printing to log
     **/
    int isZero;
    double x1, y1, x2, y2, x5, y5;
    int x3, y3, x4, y4;
    double angle;
    int fg, bg;
    char pat;

    double tempDouble;
#ifndef SIMPLE
    int tempInt;
#endif
    char tempChar;

    node = NULL;

    /* Open log file to append to */
    logFile = fopen(LOG_FILENAME, "a");
    writeOK = TRUE;
    memset(logLine, '\0', LOG_LENGTH);

    plotter = NULL;
    data = NULL;

    memset(tempStr, '\0', MAX_CMD_LENGTH);
    command = NULL;

    isZero = FALSE;

    /* Set default values */
    x1 = 0.0;
    y1 = 0.0;

    x2 = 0.0;
    y2 = 0.0;

    x3 = 0;
    y3 = 0;

    x4 = 0;
    y4 = 0;

    x5 = 0.0;
    y5 = 0.0;

    angle = 0.0;
#ifndef SIMPLE
    fg = 7;
    bg = 0;
#else
    /* If simple is set, set background to white and foreground to black */
    fg = 0;
    bg = 7;
#endif
    pat = '+';

    tempDouble = 0.0;
#ifndef SIMPLE
    tempInt = 0;
#endif
    tempChar = '\0';

    setFgColour(fg);
    setBgColour(bg);

    /* Check if log file pointer is not valid */
    if (! logFile)
    {
        printFileError("Error opening", LOG_FILENAME);
    }
    else
    {
        /* Append split to log file */
        writeOK = appendToFile(logFile, "%s\n", SPLIT, LOG_ERR);

        /* Clear screen for drawing */
        clearScreen();

        /* Get the first line in file */
        node = list -> head;

        /* Check if there's an error writing to log file */
        if (! writeOK)
        {
            printFileError(LOG_ERR, "");
        }

        while (node && writeOK)
        {
            /* Clear action string */
            memset(tempStr, '\0', MAX_CMD_LENGTH);

            /* Get the command and the next line in file */
            command = node -> value;
            node = node -> next;

            /* Reset variables to default */
            isZero = FALSE;

            x3 = 0;
            y3 = 0;

            x4 = 0;
            y4 = 0;

            x5 = 0.0;
            y5 = 0.0;

            tempDouble = 0.0;
#ifndef SIMPLE
            tempInt = 0;
#endif
            tempChar = '\0';

            plotter = NULL;
            data = NULL;

            /* Get action from command and convert to uppercase */
            sscanf(command, "%s", tempStr);
            upper(tempStr);

            if (stringCompare(tempStr, "ROTATE"))
            {
                /**
                 * If action is rotating, get the angle and add
                 * to the current angle
                 *
                 * We minus the angle because adding a positive angle,
                 * indicating that we want to turn clockwise makes the
                 * angle turn counterclockwise, see the unit circle.
                 *
                 * Thus, we subtract the angle.
                 **/
                sscanf(command, "%s %lf", tempStr, &tempDouble);
                angle += tempDouble;
                /*
                fprintf(stderr, "%f, %f\n", tempDouble, angle);
                */

                if (doubleCheck(0.0, angle))
                {
                    angle += 360.0;
                }
                else if (doubleCheck(angle, 360.0))
                {
                    angle -= 360.0;
                }
            }
            else if (stringCompare(tempStr, "MOVE"))
            {
                /**
                 * If action is moving, get the length and calculate
                 * new coordinates
                 **/
                sscanf(command, "%s %lf", tempStr, &tempDouble);

                /* Check if length is 0 */
                if (doubleCompare(tempDouble, 0.0))
                {
                    isZero = TRUE;
                }
                else
                {
                    /**
                     * Calculate new coordinates to length - 1
                     *
                     * length - 1 because line() will always draw
                     * the length plus one extra character
                     **/
                    calcNewPosition(
                        &x1, &y1, &x2, &y2,
                        angle, tempDouble - 1
                    );

                    /* Set function call to do nothing */
                    plotter = doNothing;
                    data = NULL;
                }
            }
            else if (stringCompare(tempStr, "DRAW"))
            {
                /**
                 * If action is drawing, get the length and calculate
                 * new coordinates
                 **/
                sscanf(command, "%s %lf", tempStr, &tempDouble);

                /* Check if length is 0 */
                if (doubleCompare(tempDouble, 0.0))
                {
                    isZero = TRUE;
                }
                else
                {
                    /* Calculate new positions */
                    calcNewPosition(
                        &x1, &y1, &x2, &y2,
                        angle, tempDouble - 1
                    );

                    /* Set funciton call to print one character */
                    plotter = putChar;
                    data = &pat;
                }
            }
#ifndef SIMPLE
            else if (stringCompare(tempStr, "FG"))
            {
                /**
                 * If action is changing foreground, get the parameter
                 * and change foreground
                 **/
                sscanf(command, "%s %d", tempStr, &tempInt);
                fg = tempInt;
                setFgColour(fg);
            }
            else if (stringCompare(tempStr, "BG"))
            {
                /**
                 * If action is changing background, get the parameter
                 * and change background
                 **/
                sscanf(command, "%s %d", tempStr, &tempInt);
                bg = tempInt;
                setBgColour(bg);
            }
#endif
            else if (stringCompare(tempStr, "PATTERN"))
            {
                /**
                 * If action is changing pattern, change pattern
                 * to parameter
                 **/
                sscanf(command, "%s %c", tempStr, &tempChar);
                pat = tempChar;
            }

            /* If plotter is not null and length is not zero */
            if (plotter && ! isZero)
            {
                /**
                 * Get drawing coordinates by rounding the
                 * value of the calculate coordinates
                 **/
                x3 = doubleRound(x1);
                y3 = doubleRound(y1);

                x4 = doubleRound(x2);
                y4 = doubleRound(y2);

                /**
                 * Set (x5, y5) to (x1, y1) for recording in log
                 * due to (x1, y1) being reset to the current
                 * coordinates, moved in one unit direction
                 **/
                x5 = x1;
                y5 = y1;

                /**
                 * Prepare the coordinates for the next move or draw
                 * command
                 **/
                calcNewPosition(&x1, &y1, &x2, &y2, angle, 1.0);

                /* Convert to uppercase for log file */
                upper(tempStr);

                /* Format and print (if set) log entry */
                sprintf(logLine, LOG_FORMAT, tempStr, x5, y5, x2, y2);
#ifdef DEBUG
                fprintf(stderr, "%s\n", logLine);
#endif

                /* Append log entry to log file */
                writeOK = appendToFile(logFile, "%s\n", logLine, LOG_ERR);

                /* Check for file errors */
                if (! writeOK)
                {
                    printFileError(LOG_ERR, "");
                }

                /* Reset log line */
                memset(logLine, '\0', LOG_LENGTH);

                /* Call the line function in effects.c */
                line(
                    x3, y3, x4, y4,
                    plotter, data
                );
            }
        }

        /* Print reset ascii to clear colors */
        fprintf(stdout, "%s", "\033[0m");

        /* Stop drawing and close log file */
        penDown();
        fclose(logFile);
    }
}

/**
 * Name:     calcNewPosition
 * Purpose:
 *     Calculate and update the coordinates
 *     for a given length and direction
 *
 * Parameters:
 *   - x1     : A double pointer for x1
 *   - y1     : A double pointer for y1
 *   - x2     : A double pointer for x2
 *   - y2     : A double pointer for y2
 *   - angle  : A double for the angle
 *   - length : A double for the length
 *
 * Returns:  None
 * Assertions:
 *     Assumptions:
 *         None
 *     Result:
 *         Coordinates (x1, y1) and (x2, y2) will be updated
 **/

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

    /* If angle is a right angle */
    if (doubleCompare(doubleMod(angle, 90.0), 0.0))
    {
        /* Angle is 0 or 360, only x is positive */
        if (doubleCompare(angle, 0.0) || doubleCompare(angle, 360.0))
        {
            xDelta = length;
        }
        else if (doubleCompare(angle, 90.0))
        {
            /* Angle is 90, only y is positive */
            yDelta = length;
        }
        else if (doubleCompare(angle, 180.0))
        {
            /* Angle is 180, only x is negative */
            xDelta = -length;
        }
        else if (doubleCompare(angle, 270.0))
        {
            /* Angle is 270, only y is negative */
            yDelta = -length;
        }
        else
        {
            /* Fallback calculation */
            xDelta = length * cos(degToRad(angle));
            yDelta = length * sin(degToRad(angle));
        }
    }
    else
    {
        /* Calculate changes in the coordinates */
        xDelta = length * cos(degToRad(angle));
        yDelta = length * sin(degToRad(angle));
    }

    /* Set second set to first set, and add delta to second set */
    *x1 = *x2;
    *x2 += xDelta;

    /* Subtract for y delta because of angle shenanigans */
    *y1 = *y2;
    *y2 -= yDelta;
}

/**
 * Name:     doNothing
 * Purpose:
 *     Do nothing, used for move command
 *
 * Parameters:
 *   - a : A void pointer to the pattern
 *
 * Returns:  None
 * Assertions:
 *     Assumptions:
 *         None
 *     Result:
 *         Nothing will be done, ever in this function
 **/

void doNothing(void* a) {}

/**
 * Name:     putChar
 * Purpose:
 *     Print a single character to stdout
 *
 * Parameters:
 *   - voidPtr : A void pointer for the character to print
 *
 * Returns:  None
 * Assertions:
 *     Assumptions:
 *         None
 *     Result:
 *         A character will be printed
 **/

void putChar(void* voidPtr)
{
    fputc(*(char*)voidPtr, stdout);
}

/**
 * Name:     printUsage
 * Purpose:
 *     Print a message showing how to use this program
 *
 * Parameters:
 *   - None
 *
 * Returns:  None
 * Assertions:
 *     Assumptions:
 *         None
 *     Result:
 *         Usage message will be printed
 **/

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

    /* Print usage message */
    printStringArray(
        "%s\n", usageMsg, sizeof(usageMsg) / sizeof(usageMsg[0])
    );
}

/**
 * Name:     printVersion
 * Purpose:
 *     Print a message showing build information
 *
 * Parameters:
 *   - None
 *
 * Returns:  None
 * Assertions:
 *     Assumptions:
 *         None
 *     Result:
 *         Version message will be printed
 **/

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

    char* compiler;
    char* user;
    char* host;
    char* compileDate;
    char* modDate;

    compiler = ! stringCompare(COMPILER_INFO, "") ? COMPILER_INFO : "unknown";
    user = ! stringCompare(USER, "") ? USER : "unknown";
    host = ! stringCompare(HOST, "") ? HOST : "unknown";
    compileDate = ! stringCompare(COMPILE_DATE, "") ? COMPILE_DATE : "unknown";
    modDate = ! stringCompare(MOD_DATE, "") ? MOD_DATE : "unknown";

    /* Print version information */
    printStringArray("%s\n", versionMsg, 3);
    fprintf(stdout, "%s %s\n", versionMsg[3], compiler);
    fprintf(stdout, "%s %s@%s\n", versionMsg[4], user, host);
    fprintf(stdout, "%s %s\n", versionMsg[5], compileDate);
    fprintf(stdout, "%s %s\n", versionMsg[6], modDate);
}
