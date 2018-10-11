#ifndef TOOLS_H
#define TOOLS_H

/* Constants */
#define TOLERANCE 0.0000001
#define PI 3.14159265358979323846

/* Macros */

/* Checks if one double is more than or equal to another double */
#define DOUBLE_CHECK(a, b) \
    (((a) > (b)) || (doubleCompare((a), (b))))

/* Check if an integer is within the bounds of two integers, inclusive */
#define INT_BOUND(a, low, up) \
    (((a) >= (low)) && ((up) >= (a)))

/* Check if a double is within the bounds of two doubles, inclusive */
#define DOUBLE_BOUND(a, low, up) \
    ((DOUBLE_CHECK((a), (low))) && (DOUBLE_CHECK((up), (a))))

/* Convert degrees to radians */
#define DEG_TO_RAD(i) ((i) * (PI / 180.0))

/* Get the size of a stack array */
#define GET_STACK_ARRAY_SIZE(arr) \
    (sizeof((arr)) / sizeof((arr)[0]))

/* Function prototypes */
void initString(char** str, int len);
void initStringWithContents(char** str, char* contents);
void freePtr(void** ptr);
int stringCompare(char* str1, char* str2);
void upper(char* str);
int doubleCompare(double num1, double num2);
double doubleAbs(double num);
double doubleMod(double num, double divide);
void removeTrailingNewline(char* str, int len);
int countWords(char* str);
void trim(char** str);
void printStringArray(char* format, char** strArr, int len);

#endif /* End of tools.h */
