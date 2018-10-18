#ifndef TOOLS_H
#define TOOLS_H

/* Function prototypes */
void initString(char** str, int len);
void initStringWithContents(char** str, char* contents);
void freePtr(void** ptr);
int stringCompare(char* str1, char* str2);
void upper(char* str);
int intBound(int i, int low, int up);
int doubleBound(double i, double low, double up);
int doubleCompare(double num1, double num2);
int doubleCheck(double a, double b);
double doubleAbs(double num);
double doubleMod(double num, double divide);
int doubleRound(double i);
double degToRad(double deg);
int doubleRound(double num);
void removeTrailingNewline(char* str, int len);
int countWords(char* str);
void trim(char** str);
void printStringArray(char* format, char** strArr, int len);

#endif /* End of tools.h */
