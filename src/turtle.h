#ifndef TURTLE_H
#define TURTLE_H

/* Function prototypes */
int checkArgs(int argc, char** argv);
int processArgs(char* arg, char** filename);
int validateList(LinkedList* list);
void printLineError(char* line, int lineNum);
void processList(LinkedList* list);
void calcNewPosition(
    double* x1,
    double* y1,
    double* x2,
    double* y2,
    double angle,
    double length);
void doNothing(void* voidPtr);
void putChar(void* voidPtr);
void printUsage(void);
void printVersion(void);

#endif /* End of turtle.h */
