/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#ifndef _RUN_PASSES_H_
#define _RUN_PASSES_H_

#include "vec.h"

extern bool printPasses;

void runPasses(void);
void initLogFlags(Vec<char>&);

#endif
