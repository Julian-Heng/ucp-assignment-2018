#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"
#include "fileIO.h"
#include "test.h"

int main(void)
{
    int status = 1;

    header("MAKE SURE THIS IS RUN WITH MAKE");
    testTools(&status);
    testFileIO(&status);

    if (status == 1)
    {
        status = 0;
    }
    else
    {
        status = 1;
    }

    return status;
}

void testTools(int* status)
{
    char* str;
    char** strArr;

    char* testMsg[] = {
    /*  0 */ "Testing initString() with size 1",
    /*  1 */ "Testing initStringWithContents() using \"abcde\"",
    /*  2 */ "Testing initStringArray() with 3 rows 5 columns",
    /*  3 */ "Testing initStringArray() with 3 rows 5 columns with strings",
    /*  4 */ "Testing freePtr()",
    /*  5 */ "Testing freeArray()",
    /*  6 */ "Testing initStringArray() with a large value",
    /*  7 */ "Testing freeArray() with a large value",
    /*  8 */ "Testing stringCompare() with matching strings",
    /*  9 */ "Testing stringCompare() with non-matching strings [left]",
    /* 10 */ "Testing stringCompare() with non-matching strings [right]",
    /* 11 */ "Testing upper() with lowercase",
    /* 12 */ "Testing upper() with uppercase",
    /* 13 */ "Testing upper() with mix case",
    /* 14 */ "Testing lower() with lowercase",
    /* 15 */ "Testing lower() with uppercase",
    /* 16 */ "Testing lower() with mix case"
    };

    str = NULL;
    strArr = NULL;

    header("Testing functions in tools.c");

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[0]);
        initString(&str, 1);
        *status = printResult(!!str);
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[1]);
        initStringWithContents(&str, "abcde");
        *status = printResult(strcmp(str, "abcde") == 0);
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[2]);
        initStringArray(&strArr, 3, 5);
        *status = printResult((strArr[0]) && (strArr[1]) && (strArr[2]));
        free(strArr[0]);
        free(strArr[1]);
        free(strArr[2]);
        strArr[0] = NULL;
        strArr[1] = NULL;
        strArr[2] = NULL;
        free(strArr);
        strArr = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[3]);
        initStringArray(&strArr, 3, 5);

        strncpy(strArr[0], "aaaa", 5);
        strncpy(strArr[1], "bbbb", 5);
        strncpy(strArr[2], "cccc", 5);

        *status = printResult(
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
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[4]);
        freePtr((void**)&str);
        *status = printResult(! str);
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[5]);
        initStringArray(&strArr, 3, 5);

        strncpy(strArr[0], "aaaa", 5);
        strncpy(strArr[1], "bbbb", 5);
        strncpy(strArr[2], "cccc", 5);

        freeArray((void***)&strArr, 3);
        *status = printResult(! strArr);
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[6]);
        initStringArray(&strArr, 50000, 75000);
        *status = printResult(!! strArr);
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[7]);
        freeArray((void***)&strArr, 50000);
        *status = printResult(! strArr);
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[8]);
        *status = printResult(stringCompare("aaaa", "aaaa"));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[9]);
        *status = printResult(! stringCompare("aaaa", "bbbb"));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[10]);
        *status = printResult(! stringCompare("bbbb", "aaaa"));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[11]);
        initStringWithContents(
            &str,
            "abcdefghijklmnopqrstuvwxyz");
        upper(str);
        *status = printResult(
            (strcmp(str, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") == 0)
        );
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[12]);
        initStringWithContents(
            &str,
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        upper(str);
        *status = printResult(
            (strcmp(str, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") == 0)
        );
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[13]);
        initStringWithContents(
            &str,
            "abCdeFgHIjklMnoPqrsTuvwxyZ");
        upper(str);
        *status = printResult(
            (strcmp(str, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") == 0)
        );
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[14]);
        initStringWithContents(
            &str,
            "abcdefghijklmnopqrstuvwxyz");
        lower(str);
        *status = printResult(
            (strcmp(str, "abcdefghijklmnopqrstuvwxyz") == 0)
        );
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[15]);
        initStringWithContents(
            &str,
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        lower(str);
        *status = printResult(
            (strcmp(str, "abcdefghijklmnopqrstuvwxyz") == 0)
        );
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[16]);
        initStringWithContents(
            &str,
            "abCdeFgHIjklMnoPqrsTuvwxyZ");
        lower(str);
        *status = printResult(
            (strcmp(str, "abcdefghijklmnopqrstuvwxyz") == 0)
        );
        free(str);
        str = NULL;
    }

    header("Finish testing tools.c");
}

void testFileIO(int* status)
{
    char** fileContents;
    char* testMsg[] = {
    /* 0 */ "Testing readFileToArray() with known rows and columns",
    /* 1 */ "Testing readFileToArray() with vertical pyramid",
    /* 2 */ "Testing readFileToArray() with vertical pyramid [trail]",
    /* 3 */ "Testing readFileToArray() with horizontal pyramid",
    /* 4 */ "Testing readFileToArray() with horizontal pyramid [trail]",
    /* 5 */ "Testing readFileToArray() with interchanging line size",
    /* 6 */ "Testing readFileToArray() with interchanging line size [trail]",
    /* 7 */ "Testing readFileToArray() with a single newline character",
    /* 8 */ "Testing readFileToArray() with 12 whitespaces",
    /* 9 */ "Testing readFileToArray() with a non existant file"
    };

    int i;
    int rows = 0;
    int columns = 0;

    fileContents = NULL;

    header("Testing functions in fileIO.c");

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[0]);
        readFileToArray(
            "../test/3_5",
            &fileContents,
            &rows,
            &columns
        );

        *status = printResult(
            (!! fileContents) &&
            (rows == 3) &&
            (columns == 6) &&
            (strcmp(fileContents[0], "aaaaa") == 0) &&
            (strcmp(fileContents[1], "bbbbb") == 0) &&
            (strcmp(fileContents[2], "ccccc") == 0)
        );

        for (i = 0; i < rows; i++)
        {
            free(fileContents[i]);
            fileContents[i] = NULL;
        }
        free(fileContents);
        fileContents = NULL;

        rows = 0;
        columns = 0;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[1]);
        readFileToArray(
            "../test/3vpyramids",
            &fileContents,
            &rows,
            &columns
        );

        *status = printResult(
            (!! fileContents) &&
            (rows == 5) &&
            (columns == 4) &&
            (strcmp(fileContents[0], "x") == 0) &&
            (strcmp(fileContents[1], "xx") == 0) &&
            (strcmp(fileContents[2], "xxx") == 0) &&
            (strcmp(fileContents[3], "xx") == 0) &&
            (strcmp(fileContents[4], "x") == 0)
        );

        for (i = 0; i < rows; i++)
        {
            free(fileContents[i]);
            fileContents[i] = NULL;
        }
        free(fileContents);
        fileContents = NULL;

        rows = 0;
        columns = 0;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[2]);
        readFileToArray(
            "../test/3vpyramids_trail",
            &fileContents,
            &rows,
            &columns
        );

        *status = printResult(
            (!! fileContents) &&
            (rows == 5) &&
            (columns == 4) &&
            (strcmp(fileContents[0], "x  ") == 0) &&
            (strcmp(fileContents[1], "xx ") == 0) &&
            (strcmp(fileContents[2], "xxx") == 0) &&
            (strcmp(fileContents[3], "xx ") == 0) &&
            (strcmp(fileContents[4], "x  ") == 0)
        );

        for (i = 0; i < rows; i++)
        {
            free(fileContents[i]);
            fileContents[i] = NULL;
        }
        free(fileContents);
        fileContents = NULL;

        rows = 0;
        columns = 0;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[3]);
        readFileToArray(
            "../test/3hpyramids",
            &fileContents,
            &rows,
            &columns
        );

        *status = printResult(
            (!! fileContents) &&
            (rows == 3) &&
            (columns == 6) &&
            (strcmp(fileContents[0], "  x") == 0) &&
            (strcmp(fileContents[1], " xxx") == 0) &&
            (strcmp(fileContents[2], "xxxxx") == 0)
        );

        for (i = 0; i < rows; i++)
        {
            free(fileContents[i]);
            fileContents[i] = NULL;
        }
        free(fileContents);
        fileContents = NULL;

        rows = 0;
        columns = 0;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[4]);
        readFileToArray(
            "../test/3hpyramids_trail",
            &fileContents,
            &rows,
            &columns
        );

        *status = printResult(
            (!! fileContents) &&
            (rows == 3) &&
            (columns == 6) &&
            (strcmp(fileContents[0], "  x  ") == 0) &&
            (strcmp(fileContents[1], " xxx ") == 0) &&
            (strcmp(fileContents[2], "xxxxx") == 0)
        );

        for (i = 0; i < rows; i++)
        {
            free(fileContents[i]);
            fileContents[i] = NULL;
        }
        free(fileContents);
        fileContents = NULL;

        rows = 0;
        columns = 0;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[5]);
        readFileToArray(
            "../test/4_1_4_1",
            &fileContents,
            &rows,
            &columns
        );

        *status = printResult(
            (!! fileContents) &&
            (rows == 4) &&
            (columns == 5) &&
            (strcmp(fileContents[0], "xxxx") == 0) &&
            (strcmp(fileContents[1], "x") == 0) &&
            (strcmp(fileContents[2], "xxxx") == 0) &&
            (strcmp(fileContents[3], "x") == 0)
        );

        for (i = 0; i < rows; i++)
        {
            free(fileContents[i]);
            fileContents[i] = NULL;
        }
        free(fileContents);
        fileContents = NULL;

        rows = 0;
        columns = 0;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[6]);
        readFileToArray(
            "../test/4_1_4_1_trail",
            &fileContents,
            &rows,
            &columns
        );

        *status = printResult(
            (!! fileContents) &&
            (rows == 4) &&
            (columns == 5) &&
            (strcmp(fileContents[0], "xxxx") == 0) &&
            (strcmp(fileContents[1], "x   ") == 0) &&
            (strcmp(fileContents[2], "xxxx") == 0) &&
            (strcmp(fileContents[3], "x   ") == 0)
        );

        for (i = 0; i < rows; i++)
        {
            free(fileContents[i]);
            fileContents[i] = NULL;
        }
        free(fileContents);
        fileContents = NULL;

        rows = 0;
        columns = 0;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[7]);
        readFileToArray(
            "../test/newline",
            &fileContents,
            &rows,
            &columns
        );

        *status = printResult(
            (!! fileContents) &&
            (rows == 1) &&
            (columns == 1) &&
            (strcmp(fileContents[0], "") == 0)
        );

        for (i = 0; i < rows; i++)
        {
            free(fileContents[i]);
            fileContents[i] = NULL;
        }
        free(fileContents);
        fileContents = NULL;

        rows = 0;
        columns = 0;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[8]);
        readFileToArray(
            "../test/whitespace",
            &fileContents,
            &rows,
            &columns
        );

        *status = printResult(
            (!! fileContents) &&
            (rows == 1) &&
            (columns == 13) &&
            (strcmp(fileContents[0], "            ") == 0)
        );

        for (i = 0; i < rows; i++)
        {
            free(fileContents[i]);
            fileContents[i] = NULL;
        }
        free(fileContents);
        fileContents = NULL;

        rows = 0;
        columns = 0;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[9]);
        readFileToArray(
            "../test/a non existant file",
            &fileContents,
            &rows,
            &columns
        );

        *status = printResult(! fileContents);

        for (i = 0; i < rows; i++)
        {
            free(fileContents[i]);
            fileContents[i] = NULL;
        }
        free(fileContents);
        fileContents = NULL;

        rows = 0;
        columns = 0;
    }

    header("Finish testing fileIO.c");
}

int printResult(int testCondition)
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

    return testCondition;
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