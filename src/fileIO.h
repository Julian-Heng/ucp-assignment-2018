#ifndef FILEIO_H
#define FILEIO_H
int readFileToArray(char* filename, char*** arr, int* lines, int* length);
int getFileStats(char* filename, FILE* file, int* lines, int* length);
void printFileError(char* msg, char* filename);
#endif