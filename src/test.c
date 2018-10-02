#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"
#include "linkedList.h"
#include "fileIO.h"
#include "test.h"

int main(void)
{
    int status;

    status = 1;

    header("MAKE SURE THIS IS RUN WITH MAKE");
    testTools(&status);
    testFileIO(&status);
    testLinkedList(&status);

    if (status)
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
    /* 11 */ "Testing upperRange() with lowercase",
    /* 12 */ "Testing upperRange() with uppercase",
    /* 13 */ "Testing upperRange() with mix case",
    /* 14 */ "Testing integerBoundaryCheck() [in bounds]",
    /* 15 */ "Testing integerBoundaryCheck() [out of lower bounds]",
    /* 16 */ "Testing integerBoundaryCheck() [out of upper bounds]",
    /* 17 */ "Testing integerBoundaryCheck() [inclusive lower bounds]",
    /* 18 */ "Testing integerBoundaryCheck() [inclusive upper bounds]",
    /* 19 */ "Testing doubleCompare() with 1.0 and 1.0",
    /* 20 */ "Testing doubleCompare() with 1.0 and 1.1",
    /* 21 */ "Testing doubleCompare() with 1.1 and 1.0",
    /* 22 */ "Testing doubleCompare() with 1.1000010 and 1.1000010",
    /* 23 */ "Testing doubleCompare() with 1.1000010 and 1.1000011",
    /* 24 */ "Testing doubleBoundaryCheck() [in bounds]",
    /* 25 */ "Testing doubleBoundaryCheck() [out of lower bounds]",
    /* 26 */ "Testing doubleBoundaryCheck() [out of upper bounds]",
    /* 27 */ "Testing doubleBoundaryCheck() [inclusive lower bounds]",
    /* 28 */ "Testing doubleBoundaryCheck() [inclusive upper bounds]",
    /* 29 */ "Testing doubleBoundaryCheck() [just out of lower bounds]",
    /* 30 */ "Testing doubleBoundaryCheck() [just out of upper bounds]",
    /* 31 */ "Testing removeTrailingNewline() with a newline string",
    /* 32 */ "Testing removeTrailingNewline() without a newline string",
    /* 33 */ "Testing countWords() with empty string",
    /* 34 */ "Testing countWords() with 2 words",
    /* 35 */ "Testing countWords() with whitespace",
    /* 36 */ "Testing trim() with leading whitespace",
    /* 37 */ "Testing trim() with trailing whitespace",
    /* 38 */ "Testing trim() with all whitespace",
    /* 39 */ "Testing trim() with a normal string",
    /* 40 */ "Testing trim() with a large gap"
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
        *status = printResult(
            (strArr[0]) &&
            (strArr[1]) &&
            (strArr[2])
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
        upperRange(str, strlen(str));
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
        upperRange(str, strlen(str));
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
        upperRange(str, strlen(str));
        *status = printResult(
            (strcmp(str, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") == 0)
        );
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[14]);
        *status = printResult(integerBoundaryCheck(1, 0, 2));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[15]);
        *status = printResult(! integerBoundaryCheck(-1, 0, 1));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[16]);
        *status = printResult(! integerBoundaryCheck(10, 0, 8));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[17]);
        *status = printResult(integerBoundaryCheck(0, 0, 1));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[18]);
        *status = printResult(integerBoundaryCheck(1, 0, 1));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[19]);
        *status = printResult(doubleCompare(1.0, 1.0));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[20]);
        *status = printResult(! doubleCompare(1.0, 1.1));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[21]);
        *status = printResult(! doubleCompare(1.1, 1.0));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[22]);
        *status = printResult(doubleCompare(1.1000010, 1.1000010));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[23]);
        *status = printResult(! doubleCompare(1.1000010, 1.1000011));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[24]);
        *status = printResult(doubleBoundaryCheck(10.01, 1.01, 12.01));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[25]);
        *status = printResult(! doubleBoundaryCheck(0.01, 1.01, 12.01));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[26]);
        *status = printResult(! doubleBoundaryCheck(13.01, 1.01, 12.01));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[27]);
        *status = printResult(doubleBoundaryCheck(1.01, 1.01, 12.01));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[28]);
        *status = printResult(doubleBoundaryCheck(12.01, 1.01, 12.01));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[29]);
        *status = printResult(! doubleBoundaryCheck(1.00, 1.01, 12.01));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[30]);
        *status = printResult(! doubleBoundaryCheck(12.02, 1.01, 12.01));
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[31]);
        initStringWithContents(&str, "This has a newline\n");
        removeTrailingNewline(str, strlen(str));
        *status = printResult(strcmp(str, "This has a newline") == 0);
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[32]);
        initStringWithContents(&str, "This does not have a newline");
        removeTrailingNewline(str, strlen(str));
        *status = printResult(
            strcmp(str, "This does not have a newline") == 0
        );
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[33]);
        *status = printResult(countWords("") == 0);
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[34]);
        *status = printResult(
            countWords("two words") == 2
        );
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[35]);
        *status = printResult(
            countWords("      ") == 0
        );
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[36]);
        initStringWithContents(&str, "    leading whitespace");
        trim(&str);
        *status = printResult(strcmp(str, "leading whitespace") == 0);
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[37]);
        initStringWithContents(&str, "trailing whitespace    ");
        trim(&str);
        *status = printResult(strcmp(str, "trailing whitespace") == 0);
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[38]);
        initStringWithContents(&str, "        ");
        trim(&str);
        *status = printResult(strcmp(str, "") == 0);
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[39]);
        initStringWithContents(&str, "a normal string");
        trim(&str);
        *status = printResult(strcmp(str, "a normal string") == 0);
        free(str);
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[40]);
        initStringWithContents(&str, "a        b");
        trim(&str);
        *status = printResult(strcmp(str, "a        b") == 0);
        free(str);
        str = NULL;
    }

    header("Finish testing tools.c");
}

void testFileIO(int* status)
{
    char** fileContents;
    char* testMsg[] = {
    /*  0 */ "Testing readFileToArray() with known rows and columns",
    /*  1 */ "Testing readFileToArray() with vertical pyramid",
    /*  2 */ "Testing readFileToArray() with vertical pyramid [trail]",
    /*  3 */ "Testing readFileToArray() with horizontal pyramid",
    /*  4 */ "Testing readFileToArray() with horizontal pyramid [trail]",
    /*  5 */ "Testing readFileToArray() with interchanging line size",
    /*  6 */ "Testing readFileToArray() with interchanging line size [trail]",
    /*  7 */ "Testing readFileToArray() with a single newline character",
    /*  8 */ "Testing readFileToArray() with 12 whitespaces",
    /*  9 */ "Testing readFileToArray() with an empty file",
    /* 10 */ "Testing readFileToArray() with a non existant file"
    };

    int i, rows, columns;

    i = 0;
    rows = 0;
    columns = 0;

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
            "../test/empty_file",
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

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[10]);
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

void testLinkedList(int* status)
{
    LinkedList* list;
    LinkedListNode* listNode;

    void* voidPtr;
    char* str;
    int i;

    char* testMsg[] = {
    /*  0 */ "Testing initNode() without value",
    /*  1 */ "Testing initNode() with value",
    /*  2 */ "Testing initList() with no nodes",
    /*  3 */ "Testing insertFirst() without value",
    /*  4 */ "Testing insertFirst() with 5 strings",
    /*  5 */ "Testing insertFirst() with 10000 malloced strings",
    /*  6 */ "Testing insertLast() without value",
    /*  7 */ "Testing insertLast() with 5 strings",
    /*  8 */ "Testing insertFirst() with 10000 malloced strings",
    /*  9 */ "Testing removeFirst() on an empty list",
    /* 10 */ "Testing removeFirst() on a single node list",
    /* 11 */ "Testing removeFirst() on 10000 malloced string list",
    /* 12 */ "Testing removeLast() on an empty list",
    /* 13 */ "Testing removeLast() on a single node list",
    /* 14 */ "Testing removeLast() on 10000 malloced string list",
    /* 15 */ "Testing peekFirst() on an empty list",
    /* 16 */ "Testing peekFirst() on a single node list",
    /* 17 */ "Testing peekFirst() on 10000 malloced string list",
    /* 18 */ "Testing peekLast() on an empty list",
    /* 19 */ "Testing peekLast() on a single node list",
    /* 20 */ "Testing peekLast() on 10000 malloced string list",
    /* 21 */ "Testing getLength() on an empty list",
    /* 22 */ "Testing getLength() on a single node list",
    /* 23 */ "Testing getLength() on 10000 malloced string list",
    /* 24 */ "Testing clearListStack() on an empty list",
    /* 25 */ "Testing clearListStack() on a single node list",
    /* 26 */ "Testing clearListStack() on 10 stack strings",
    /* 27 */ "Testing clearListMalloc() on an empty list",
    /* 28 */ "Testing clearListMalloc() on a single node list",
    /* 29 */ "Testing clearListMalloc() on 10000 malloced string list"
    };

    voidPtr = NULL;
    str = NULL;
    list = NULL;
    listNode = NULL;
    i = 0;

    header("Testing functions in linkedList.c");

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[0]);
        listNode = initNode(NULL);
        *status = printResult(!! listNode);
        free(listNode);
        listNode = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[1]);
        listNode = initNode(testMsg[1]);
        *status = printResult(
            !! listNode &&
            strcmp(listNode -> value, testMsg[1]) == 0
        );
        free(listNode);
        listNode = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[2]);
        list = initList();
        *status = printResult(!! list);
        free(list -> head);
        list -> head = NULL;
        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[3]);
        list = initList();
        insertFirst(list, testMsg[3]);
        *status = printResult(
            !! list &&
            strcmp(list -> head -> value, testMsg[3]) == 0
        );
        free(list -> head);
        list -> head = NULL;
        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[4]);
        list = initList();

        for (i = 0; i < 5; i++)
        {
            insertFirst(list, testMsg[i]);
        }

        i = 0;

        listNode = list -> head;

        while (listNode != NULL && i < 5 && *status)
        {
            if (strcmp(listNode -> value, testMsg[4 - i]) == 0)
            {
                *status = 1;
                listNode = listNode -> next;
            }
            else
            {
                *status = 0;
            }
            i++;
        }

        *status = printResult(*status);

        while (list -> head != NULL)
        {
            listNode = list -> head;
            list -> head = list -> head -> next;
            free(listNode);
        }

        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[5]);
        list = initList();

        for (i = 0; i < 10000; i++)
        {
            initStringWithContents(&str, "abcdefghijklmnopqrstuvwxyz");
            insertFirst(list, str);
        }

        i = 0;

        listNode = list -> head;

        while (listNode != NULL && *status)
        {
            if (strcmp(listNode -> value, "abcdefghijklmnopqrstuvwxyz") == 0)
            {
                *status = 1;
                listNode = listNode -> next;
            }
            else
            {
                *status = 0;
            }
            i++;
        }

        *status = printResult(i == 10000);

        while (list -> head != NULL)
        {
            listNode = list -> head;
            list -> head = list -> head -> next;
            free(listNode -> value);
            listNode -> value = NULL;
            free(listNode);
            listNode = NULL;
        }

        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[6]);
        list = initList();
        insertLast(list, testMsg[6]);
        *status = printResult(
            !! list &&
            strcmp(list -> head -> value, testMsg[6]) == 0
        );
        free(list -> head);
        list -> head = NULL;
        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[7]);
        list = initList();

        for (i = 0; i < 5; i++)
        {
            insertLast(list, testMsg[i]);
        }

        i = 0;

        listNode = list -> head;

        while (listNode != NULL && i < 5 && *status)
        {
            if (strcmp(listNode -> value, testMsg[i]) == 0)
            {
                *status = 1;
                listNode = listNode -> next;
            }
            else
            {
                *status = 0;
            }
            i++;
        }

        *status = printResult(*status);

        while (list -> head != NULL)
        {
            listNode = list -> head;
            list -> head = list -> head -> next;
            free(listNode);
            listNode = NULL;
        }

        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[8]);
        list = initList();

        for (i = 0; i < 10000; i++)
        {
            initStringWithContents(&str, "abcdefghijklmnopqrstuvwxyz");
            insertLast(list, str);
        }

        i = 0;

        listNode = list -> head;

        while (listNode != NULL && *status)
        {
            if (strcmp(listNode -> value, "abcdefghijklmnopqrstuvwxyz") == 0)
            {
                *status = 1;
                listNode = listNode -> next;
            }
            else
            {
                *status = 0;
            }
            i++;
        }

        *status = printResult(i == 10000);

        while (list -> head != NULL)
        {
            listNode = list -> head;
            list -> head = list -> head -> next;
            free(listNode -> value);
            listNode -> value = NULL;
            free(listNode);
            listNode = NULL;
        }

        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[9]);
        list = initList();
        listNode = removeFirst(list);

        *status = printResult(! listNode);

        free(list);
        list = NULL;
        listNode = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[10]);
        list = initList();
        insertFirst(list, testMsg[10]);
        listNode = removeFirst(list);
        *status = printResult(
            !! listNode &&
            strcmp(listNode -> value, testMsg[10]) == 0 &&
            isEmpty(list)
        );

        free(listNode);
        listNode = NULL;
        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[11]);
        list = initList();

        initStringWithContents(&str, "This is the first string");
        insertFirst(list, str);

        for (i = 1; i < 10000; i++)
        {
            initStringWithContents(&str, "abcdefghijklmnopqrstuvwxyz");
            insertLast(list, str);
        }

        listNode = removeFirst(list);

        *status = printResult(
            listNode != NULL &&
            strcmp(listNode -> value, "This is the first string") == 0
        );

        while (list -> head != NULL)
        {
            listNode = list -> head;
            list -> head = list -> head -> next;
            free(listNode -> value);
            listNode -> value = NULL;
            free(listNode);
            listNode = NULL;
        }

        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[12]);
        list = initList();
        listNode = removeLast(list);

        *status = printResult(! listNode);

        free(list);
        list = NULL;
        listNode = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[13]);
        list = initList();
        insertFirst(list, testMsg[13]);
        listNode = removeLast(list);
        *status = printResult(
            !! listNode &&
            strcmp(listNode -> value, testMsg[13]) == 0 &&
            isEmpty(list)
        );

        free(listNode);
        listNode = NULL;
        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[14]);
        list = initList();

        initStringWithContents(&str, "This is the last string");
        insertLast(list, str);

        for (i = 1; i < 10000; i++)
        {
            initStringWithContents(&str, "abcdefghijklmnopqrstuvwxyz");
            insertFirst(list, str);
        }

        listNode = removeLast(list);

        *status = printResult(
            !! listNode &&
            strcmp(listNode -> value, "This is the last string") == 0
        );

        while (list -> head != NULL)
        {
            listNode = list -> head;
            list -> head = list -> head -> next;
            free(listNode -> value);
            listNode -> value = NULL;
            free(listNode);
            listNode = NULL;
        }

        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[15]);
        list = initList();
        voidPtr = peekFirst(list);

        *status = printResult(! voidPtr);

        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[16]);
        list = initList();
        insertFirst(list, testMsg[16]);
        voidPtr = peekFirst(list);
        *status = printResult(
            !! voidPtr &&
            strcmp(voidPtr, testMsg[16]) == 0
        );

        voidPtr = NULL;
        free(list -> head);
        list -> head = NULL;
        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[17]);
        list = initList();

        initStringWithContents(&str, "This is the first string");
        insertFirst(list, str);

        for (i = 1; i < 10000; i++)
        {
            initStringWithContents(&str, "abcdefghijklmnopqrstuvwxyz");
            insertLast(list, str);
        }

        voidPtr = peekFirst(list);

        *status = printResult(
            !! voidPtr &&
            strcmp(voidPtr, "This is the first string") == 0
        );

        while (list -> head != NULL)
        {
            listNode = list -> head;
            list -> head = list -> head -> next;
            free(listNode -> value);
            listNode -> value = NULL;
            free(listNode);
            listNode = NULL;
        }

        voidPtr = NULL;
        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[18]);
        list = initList();
        voidPtr = removeLast(list);

        *status = printResult(! voidPtr);

        free(list);
        list = NULL;
        voidPtr = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[19]);
        list = initList();
        insertFirst(list, testMsg[19]);
        voidPtr = peekLast(list);
        *status = printResult(
            !! voidPtr &&
            strcmp(voidPtr, testMsg[19]) == 0
        );

        free(list -> head);
        listNode = NULL;
        free(list);
        list = NULL;
        voidPtr = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[20]);
        list = initList();

        initStringWithContents(&str, "This is the last string");
        insertLast(list, str);

        for (i = 1; i < 10000; i++)
        {
            initStringWithContents(&str, "abcdefghijklmnopqrstuvwxyz");
            insertFirst(list, str);
        }

        voidPtr = peekLast(list);

        *status = printResult(
            !! voidPtr &&
            strcmp(voidPtr, "This is the last string") == 0
        );

        while (list -> head != NULL)
        {
            listNode = list -> head;
            list -> head = list -> head -> next;
            free(listNode -> value);
            listNode -> value = NULL;
            free(listNode);
            listNode = NULL;
        }

        free(list);
        list = NULL;
        voidPtr = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[21]);
        list = initList();

        *status = printResult(getListLength(list) == 0);

        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[22]);
        list = initList();
        insertFirst(list, testMsg[22]);

        *status = printResult(getListLength(list) == 1);

        free(list -> head);
        listNode = NULL;
        free(list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[23]);
        list = initList();

        initStringWithContents(&str, "This is the last string");
        insertLast(list, str);

        for (i = 1; i < 10000; i++)
        {
            initStringWithContents(&str, "abcdefghijklmnopqrstuvwxyz");
            insertFirst(list, str);
        }


        *status = printResult(getListLength(list) == 10000);

        while (list -> head != NULL)
        {
            listNode = list -> head;
            list -> head = list -> head -> next;
            free(listNode -> value);
            listNode -> value = NULL;
            free(listNode);
            listNode = NULL;
        }

        free(list);
        list = NULL;
        voidPtr = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[24]);
        list = initList();

        clearListStack(&list);

        *status = printResult(! list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[25]);
        list = initList();

        insertFirst(list, testMsg[25]);

        clearListStack(&list);

        *status = printResult(! list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[2]);
        list = initList();

        for (i = 0; i < 10; i++)
        {
            insertFirst(list, testMsg[i]);
        }

        clearListStack(&list);

        *status = printResult(! list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[27]);
        list = initList();

        clearListMalloc(&list);

        *status = printResult(! list);
        list = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[28]);
        list = initList();

        initStringWithContents(&str, "a malloc string");
        insertFirst(list, str);

        clearListMalloc(&list);

        *status = printResult(! list);

        list = NULL;
        str = NULL;
    }

    if (*status)
    {
        fprintf(stdout, "%s: ", testMsg[29]);
        list = initList();

        for (i = 0; i < 10000; i++)
        {
            initStringWithContents(&str, "10000 malloced strings");
            insertLast(list, str);
        }

        clearListMalloc(&list);

        *status = printResult(! list);

        list = NULL;
        str = NULL;
    }

    header("Finish testing linkedList.c");
}

int printResult(int testCondition)
{
    char* green;
    char* red;
    char* reset;

    green = "\033[32m";
    red = "\033[31m";
    reset = "\033[0m";

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
    int msgLen;
    char* line;

    msgLen = strlen(msg) + 1;
    line = (char*)malloc(msgLen * sizeof(char));
    memset(line, '\0', msgLen);
    memset(line, '=', msgLen - 1);

    fprintf(stdout, "%s\n%s\n%s\n", line, msg, line);
    free(line);
}
