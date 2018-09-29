#ifndef TURTLE_H
#define TURTLE_H

int checkArgs(int argc, char** argv);
int processArgs(int argc, char** argv, char** filename, int* returnCode);
void processCommands(char** commandArr, int numCommands, int* returnCode);
void calcNewPosition(
    double* x1,
    double* y1,
    double* x2,
    double* y2,
    double angle,
    double length);
void doNothing(void* voidPtr);
void putChar(void* voidPtr);
int prepareCommands(char** commandArr, int numCommands, int* errLine);
void printUsage(void);
void printVersion(void);

#endif
