#ifndef _arg_H_
#define _arg_H_

#include <stdint.h>
#include "chpltypes.h"

//
// defined in arg.c
//
extern int32_t blockreport;
extern int32_t taskreport;

void parseNumLocales(const char* numPtr, int32_t lineno, chpl_string filename);
void parseArgs(int* argc, char* argv[]);
int32_t getArgNumLocales(void);
int _runInGDB(void);

//
// defined with main()
//
int handleNonstandardArg(int* argc, char* argv[], int argNum, 
                         int32_t lineno, chpl_string filename);

#endif
