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
  const char* filename;
  const char* pathname;
};

void codegen_makefile(fileinfo* mainfile);

void deleteTmpDir(void);

void openCFile(fileinfo* fi, const char* name, const char* ext = NULL);
void closeCFile(fileinfo* fi);

fileinfo* openTmpFile(const char* tmpfilename, const char* mode = "w");

void openfile(fileinfo* thefile, const char* mode);
void closefile(fileinfo* thefile);

FILE* openInputFile(const char* filename);
void closeInputFile(FILE* infile);
bool isChplSource(const char* filename);
void testInputFiles(int numFilenames, char* filename[]);
const char* nthFilename(int i);
void addLibInfo(const char* filename);

void genIncludeCommandLineHeaders(FILE* outfile);

const char* createGDBFile(int argc, char* argv[]);

void makeBinary(void);

char* runUtilScript(const char* script);

#endif
