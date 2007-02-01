#ifndef _error_H_
#define _error_H_

#include "chpltypes.h"

void printError(char* message, _int32 lineno, _string filename);
void printInternalError(char* message);

#endif
