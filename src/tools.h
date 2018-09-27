#ifndef TOOLS_H
#define TOOLS_H
void initString(char** str, int len);
void initStringWithContents(char** str, char* contents);
void initStringArray(char***, int rows, int cols);
void freePtr(void** ptr);
void freeArray(void*** ptr, int len);
int stringCompare(char* str1, char* str2);
void upper(char* str);
void removeTrailingNewline(char* str, int len);
void printStringArray(char* format, char** strArr, int len);
void printStringArrayUntilEOF(char* format, char** strArr);
#endif
