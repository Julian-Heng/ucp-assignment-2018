#ifndef TURTLE_H
#define TURTLE_H

int checkArgs(int argc, char** argv);
int processArgs(char* arg, char** filename);
void processCommands(
    char** commandArr,
    int numCommands,
    LinkedList** commandList,
    int* returnCode);
void calcNewPosition(
    double* x1,
    double* y1,
    double* x2,
    double* y2,
    double angle,
    double length);
void doNothing(void* voidPtr);
void putChar(void* voidPtr);
LinkedList* validateCommands(
    char** commandArr,
    int numCommands,
    int* errLine);
void printUsage(void);
void printVersion(void);

#endif
