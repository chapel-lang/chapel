#ifndef _RUN_PASSES_H_
#define _RUN_PASSES_H_

class PhaseTracker;

#include "vec.h"

void runPasses(PhaseTracker& tracker, bool isChpldoc);
void initLogFlags(Vec<char>&);

extern int currentPassNo;

#endif
