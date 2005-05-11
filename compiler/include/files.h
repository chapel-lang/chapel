#ifndef _files_H_
#define _files_H_

#include <stdio.h>

extern char executableFilename[FILENAME_MAX];
extern char saveCDir[FILENAME_MAX];

extern FILE* codefile;
extern FILE* intheadfile;
extern FILE* extheadfile;

struct fileinfo {
  FILE* fptr;
  char* filename;
  char* pathname;
};

char* sysdirToChplRoot(char* systemDir);

void deleteTmpDir(void);

void open_common(fileinfo* commonfile);
void genCFilenames(char* modulename, char** outfilename, 
                   char** extheadfilename, char** intheadfilename);
void openCFiles(char* modulename, fileinfo* outfile,
                fileinfo* extheadfile, fileinfo* intheadfile);
void closeCFiles(fileinfo* outfile, 
                 fileinfo* extheadfile, fileinfo* intheadfile);

fileinfo* openTmpFile(char* tmpfilename);

void openMakefile(char* srcfilename, char* systemDir);
void closeMakefile(void);

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

