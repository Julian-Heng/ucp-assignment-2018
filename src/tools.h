#ifndef TOOLS_H
#define TOOLS_H
void initString(char** str, int len);
void initStringWithContents(char** str, char* contents);
void initStringArray(char***, int rows, int cols);
void freePtr(void** ptr);
void freeArray(void*** ptr, int len);
int stringCompare(char* str1, char* str2);
void upper(char* str);
void lower(char* str);
int integerBoundaryCheck(int i, int lower, int upper);
int doubleCompare(double num1, double num2);
int doubleBoundaryCheck(double i, double lower, double upper);
void removeTrailingNewline(char* str, int len);
void printStringArray(char* format, char** strArr, int len);
void printStringArrayUntilEOF(char* format, char** strArr);
#endif
