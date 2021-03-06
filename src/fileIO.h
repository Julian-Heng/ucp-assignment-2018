#ifndef FILEIO_H
#define FILEIO_H

/* Function prototypes */
LinkedList* readFileToList(char* filename);
int appendToFile(FILE* file, char* format, char* line, char* errMsg);
int getFileStats(char* filename, FILE* file, int* lines, int* length);
void printFileError(char* msg, char* filename);

#endif /* End of fileIO.h */
