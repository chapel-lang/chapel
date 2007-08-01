#ifndef _error_H_
#define _error_H_

#include "chpltypes.h"

extern int verbosity;

void _printError(const char* message, _int32 lineno, _string filename);
void _printInternalError(const char* message);
void _chpl_msg(int verbose_level, const char* fmt, ...);

#endif
