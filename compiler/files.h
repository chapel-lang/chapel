#ifndef _files_H_
#define _files_H_

#include <stdio.h>

extern char executableFilename[FILENAME_MAX];
extern char saveCDir[FILENAME_MAX];

struct fileinfo {
  FILE* fptr;
  char* filename;
  char* pathname;
};

void deleteTmpDir(void);

void openCFiles(char* infilename, fileinfo* outfile,
		fileinfo* extheadfile, fileinfo* intheadfile);
void closeCFiles(fileinfo* outfile, 
		 fileinfo* extheadfile, fileinfo* intheadfile);

fileinfo* openTmpFile(char* tmpfilename);

void openMakefile(char* srcfilename, char* compilerDir);
void closeMakefile(void);

void openfile(fileinfo* thefile, char* mode);
void closefile(fileinfo* thefile);

FILE* openInputFile(char* filename);
void closeInputFile(FILE* infile);

char* createGDBFile(int argc, char* argv[]);

void makeAndCopyBinary(void);

#endif

