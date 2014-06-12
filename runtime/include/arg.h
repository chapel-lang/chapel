#ifndef _arg_H_
#define _arg_H_

#include "chpltypes.h" // for c_string

#include <stdint.h> // for int32_t

//
// defined in arg.c
//
extern int32_t blockreport;
extern int32_t taskreport;

void parseNumLocales(const char* numPtr, int32_t lineno, c_string filename);
void parseArgs(int* argc, char* argv[]);
int32_t getArgNumLocales(void);
int32_t chpl_baseUniqueLocaleID(int32_t r);
int _runInGDB(void);
int chpl_specify_locales_error(void);

//
// defined with main()
//
int handleNonstandardArg(int* argc, char* argv[], int argNum, 
                         int32_t lineno, c_string filename);
void printAdditionalHelp(void);

void printHelpTable(void);

#endif
