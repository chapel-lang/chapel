#ifndef _files_H_
#define _files_H_

extern char executableFilename[FILENAME_MAX];
extern char saveCDir[FILENAME_MAX];

void createtmpdir(void);
void deletetmpdir(void);

FILE* openoutfile(char* infilename);
void closefile(FILE* thefile);

void openMakefile(char* srcfilename, char* compilerDir);
void closeMakefile(void);

void makeAndCopyBinary(void);

#endif

