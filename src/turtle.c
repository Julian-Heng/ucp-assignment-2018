#include <stdio.h>
#include <stdlib.h>

#include "tools.h"
#include "fileIO.h"
#include "turtle.h"

int main(void)
{
    printUsage();
    printVersion();
    return 0;
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

    #ifndef USERHOST
    #define USERHOST "unknown"
    #endif

    #ifndef COMPILE_DATE
    #define COMPILE_DATE "unknown"
    #endif

    #ifndef MOD_DATE
    #define MOD_DATE "unknown"
    #endif

    printStringArray("%s", versionMsg, 3);
    fprintf(stdout, "%s %s\n", versionMsg[3], USERHOST);
    fprintf(stdout, "%s %s\n", versionMsg[4], COMPILE_DATE);
    fprintf(stdout, "%s %s\n", versionMsg[5], MOD_DATE);
}
