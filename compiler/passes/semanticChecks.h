#ifndef _SEMANTIC_CHECKS_H_
#define _SEMANTIC_CHECKS_H_

#include "pass.h"

void semanticCheckI(void); // after parsing
void semanticCheckII(void); // after scope resolution
void semanticCheckIII(void); // after function resolution

#endif
