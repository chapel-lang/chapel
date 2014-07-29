#ifndef _RUN_PASSES_H_
#define _RUN_PASSES_H_

class PhaseTracker;

#include "vec.h"

#include <cstdio>

void runPasses(PhaseTracker& tracker);
void initLogFlags(Vec<char>&);

extern int   currentPassNo;

#endif
