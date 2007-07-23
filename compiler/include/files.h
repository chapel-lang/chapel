#ifndef _files_H_
#define _files_H_

#include <stdio.h>

extern char executableFilename[FILENAME_MAX];
extern char saveCDir[FILENAME_MAX];
extern char ccflags[256];
extern char ldflags[256];
extern bool ccwarnings;

struct fileinfo {
  FILE* fptr;
  char* filename;
  char* pathname;
};

void codegen_makefile(fileinfo* mainfile);

void deleteTmpDir(void);

void genCFilenames(char* modulename, char** outfilename);

void openCFile(fileinfo* fi, char* name, char* ext = NULL);
void closeCFile(fileinfo* fi);

fileinfo* openTmpFile(char* tmpfilename);

void openfile(fileinfo* thefile, char* mode);
void closefile(fileinfo* thefile);

FILE* openInputFile(char* filename);
void closeInputFile(FILE* infile);
void testInputFiles(int numFilenames, char* filename[]);
char* nthFilename(int i);
void addLibInfo(char* filename);

char* createGDBFile(int argc, char* argv[]);

void makeBinary(void);

#endif
