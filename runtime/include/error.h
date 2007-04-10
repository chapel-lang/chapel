#ifndef _error_H_
#define _error_H_

#include "chpltypes.h"

void _printError(char* message, _int32 lineno, _string filename);
void _printInternalError(char* message);

#endif
