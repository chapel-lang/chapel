#ifndef _arg_H_
#define _arg_H_

#include <stdint.h>
#include "chpltypes.h"

extern int32_t blockreport;

void parseArgs(int argc, char* argv[]);
int32_t getArgNumLocales(void);
int _runInGDB(void);

#endif
