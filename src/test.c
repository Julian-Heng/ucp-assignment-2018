#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"
#include "fileIO.h"
#include "test.h"

int main(void)
{
    testTools();
    return 0;
}

void testTools(void)
{
    char* str;
    char** strArr;

    char* testMsg[] = {
        "Testing initString() with size 1",
        "Testing initStringWithContents() using \"abcde\"",
        "Testing initStringArray() with 3 rows 5 columns",
        "Testing initStringArray() with 3 rows 5 columns with contents",
        "Testing freePtr()",
        "Testing freeArray()",
        "Testing initStringArray() with a large value",
        "Testing freeArray() with a large value",
        "Testing stringCompare() with matching strings",
        "Testing stringCompare() with non-matching strings [left]",
        "Testing stringCompare() with non-matching strings [right]",
        "Testing upper() with lowercase",
        "Testing upper() with uppercase",
        "Testing upper() with mix case"
    };

    str = NULL;
    strArr = NULL;

    header("Testing functions in tools.c");

    fprintf(stdout, "%s: ", testMsg[0]);
    initString(&str, 1);
    printResult(!!str);
    free(str);
    str = NULL;

    fprintf(stdout, "%s: ", testMsg[1]);
    initStringWithContents(&str, "abcde");
    printResult(strcmp(str, "abcde") == 0);
    free(str);
    str = NULL;

    fprintf(stdout, "%s: ", testMsg[2]);
    initStringArray(&strArr, 3, 5);
    printResult((strArr[0]) && (strArr[1]) && (strArr[2]));
    free(strArr[0]);
    free(strArr[1]);
    free(strArr[2]);
    strArr[0] = NULL;
    strArr[1] = NULL;
    strArr[2] = NULL;
    free(strArr);
    strArr = NULL;

    fprintf(stdout, "%s: ", testMsg[3]);
    initStringArray(&strArr, 3, 5);

    strncpy(strArr[0], "aaaa", 5);
    strncpy(strArr[1], "bbbb", 5);
    strncpy(strArr[2], "cccc", 5);

    printResult(
        (strcmp(strArr[0], "aaaa") == 0) &&
        (strcmp(strArr[1], "bbbb") == 0) &&
        (strcmp(strArr[2], "cccc") == 0)
    );
    free(strArr[0]);
    free(strArr[1]);
    free(strArr[2]);
    strArr[0] = NULL;
    strArr[1] = NULL;
    strArr[2] = NULL;
    free(strArr);
    strArr = NULL;

    fprintf(stdout, "%s: ", testMsg[4]);
    freePtr((void**)&str);
    printResult(! str);

    fprintf(stdout, "%s: ", testMsg[5]);
    initStringArray(&strArr, 3, 5);

    strncpy(strArr[0], "aaaa", 5);
    strncpy(strArr[1], "bbbb", 5);
    strncpy(strArr[2], "cccc", 5);

    freeArray((void***)&strArr, 3);
    printResult(! strArr);

    fprintf(stdout, "%s: ", testMsg[6]);
    initStringArray(&strArr, 50000, 75000);
    printResult(!! strArr);

    fprintf(stdout, "%s: ", testMsg[7]);
    freeArray((void***)&strArr, 50000);
    printResult(! strArr);

    fprintf(stdout, "%s: ", testMsg[8]);
    printResult(stringCompare("aaaa", "aaaa"));

    fprintf(stdout, "%s: ", testMsg[9]);
    printResult(! stringCompare("aaaa", "bbbb"));

    fprintf(stdout, "%s: ", testMsg[10]);
    printResult(! stringCompare("bbbb", "aaaa"));

    fprintf(stdout, "%s: ", testMsg[11]);
    initStringWithContents(
        &str,
        "abcdefghijklmnopqrstuvwxyz");
    upper(str);
    printResult(
        (strcmp(str, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") == 0)
    );
    free(str);
    str = NULL;

    fprintf(stdout, "%s: ", testMsg[12]);
    initStringWithContents(
        &str,
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    upper(str);
    printResult(
        (strcmp(str, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") == 0)
    );
    free(str);
    str = NULL;

    fprintf(stdout, "%s: ", testMsg[13]);
    initStringWithContents(
        &str,
        "abCdeFgHIjklMnoPqrsTuvwxyZ");
    upper(str);
    printResult(
        (strcmp(str, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") == 0)
    );
    free(str);
    str = NULL;

    header("Finish testing tools.c");
}

void printResult(int testCondition)
{
    char* green = "\033[22;32m";
    char* red = "\033[22;31m";
    char* reset = "\033[22;0m";

    if (testCondition)
    {
        fprintf(stdout, "%s%s%s\n", green, "Passed", reset);
    }
    else
    {
        fprintf(stdout, "%s%s%s\n", red, "Failed", reset);
    }
}

void header(char* msg)
{
    int msgLen = strlen(msg) + 1;
    char* line = (char*)malloc(msgLen * sizeof(char));
    memset(line, '\0', msgLen);
    memset(line, '=', msgLen - 1);
    fprintf(stdout, "%s\n%s\n%s\n", line, msg, line);
    free(line);
}
