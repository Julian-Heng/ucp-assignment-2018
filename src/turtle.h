#ifndef TURTLE_H
#define TURTLE_H
int checkArgs(int argc, char** argv);
int processArgs(int argc, char** argv, char** filename, int* returnCode);
void processCommands(char** commandArr, int numCommands);
int prepareCommands(char** commandArr, int numCommands, int* errLine);
void resetVariables(double* angle, double* length, int* color, char* pat);
void printUsage(void);
void printVersion(void);
#endif
