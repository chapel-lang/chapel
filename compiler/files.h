#ifndef _files_H_
#define _files_H_

char* createtmpdir(void);
void deletetmpdir(void);

FILE* openoutfile(char* infilename);
void closefile(FILE* thefile);

void openMakefile(char* srcfilename, char* compilerDir, char* binName);
void closeMakefile(void);

#endif

