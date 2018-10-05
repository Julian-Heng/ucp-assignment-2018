#ifndef TOOLS_H
#define TOOLS_H

#define DOUBLE_CHECK(a, b) \
    (((a) > (b)) || (doubleCompare((a), (b))))

#define INT_BOUND(a, low, up) \
    (((a) >= (low)) && ((up) >= (a)))
#define DOUBLE_BOUND(a, low, up) \
    ((DOUBLE_CHECK((a), (low))) && (DOUBLE_CHECK((up), (a))))

#define TOLERANCE 0.0000001
#define PI 3.14159265358979323846
#define DEG_TO_RAD(i) ((i) * (PI / 180.0))

#define GET_STACK_2D_ARRAY_SIZE(arr) \
    (sizeof((arr)) / sizeof((arr)[0]))

void initString(char** str, int len);
void initStringWithContents(char** str, char* contents);
void initStringArray(char***, int rows, int cols);
void freePtr(void** ptr);
void freeArray(void*** ptr, int len);
int stringCompare(char* str1, char* str2);
void upperRange(char* str, int range);
int doubleCompare(double num1, double num2);
void removeTrailingNewline(char* str, int len);
int countWords(char* str);
void trim(char** str);
void printStringArray(char* format, char** strArr, int len);

#endif /* End of tools.h */
