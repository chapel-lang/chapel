#ifndef _files_H_
#define _files_H_

#include <cstdio>

extern char executableFilename[FILENAME_MAX+1];
extern char saveCDir[FILENAME_MAX+1];
extern char ccflags[256];
extern char ldflags[256];
extern bool ccwarnings;

struct fileinfo {
  FILE* fptr;
  const char* filename;
  const char* pathname;
};

void codegen_makefile(fileinfo* mainfile, fileinfo *gpusrcfile = NULL);

void ensureDirExists(const char* /* dirname */, const char* /* explanation */);
void deleteTmpDir(void);

void openCFile(fileinfo* fi, const char* name, const char* ext = NULL);
void appendCFile(fileinfo* fi, const char* name, const char* ext = NULL);
void closeCFile(fileinfo* fi, bool beautifyIt=true);

fileinfo* openTmpFile(const char* tmpfilename, const char* mode = "w");

void openfile(fileinfo* thefile, const char* mode);
void closefile(fileinfo* thefile);

FILE* openInputFile(const char* filename);
void closeInputFile(FILE* infile);
bool isChplSource(const char* filename);
void testInputFiles(int numFilenames, char* filename[]);
const char* nthFilename(int i);
void addLibInfo(const char* filename);
void addIncInfo(const char* incDir);

void genIncludeCommandLineHeaders(FILE* outfile);

const char* createGDBFile(int argc, char* argv[]);

void makeBinary(void);

const char* runUtilScript(const char* script);

void setupModulePaths(void);
void addFlagModulePath(const char* newpath);
void addDashMsToUserPath(void);
void addModulePathFromFilename(const char* filename);

const char* modNameToFilename(const char* modName, bool isInternal, 
                              bool* isStandard);
const char* stdModNameToFilename(const char* modName);

void printModuleSearchPath(void);

char* dirHasFile(const char *dir, const char *file);
char* findProgramPath(const char* argv0);
bool isSameFile(const char* pathA, const char* pathB);

#endif
