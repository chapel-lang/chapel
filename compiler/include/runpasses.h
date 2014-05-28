#ifndef _RUN_PASSES_H_
#define _RUN_PASSES_H_

#include "vec.h"

#include <cstdio>

void runPasses();
void initLogFlags(Vec<char>&);


extern bool        printPasses;
extern FILE*       printPassesFile; 

extern int         currentPassNo;
extern const char* currentPassName;


#endif
