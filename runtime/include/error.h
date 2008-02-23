#ifndef _error_H_
#define _error_H_

#include "chpltypes.h"
#include <stdint.h>

extern int verbosity;

void _chpl_warning(const char* message, int32_t lineno, _string filename);
void _chpl_error(const char* message, int32_t lineno, _string filename);
void _chpl_internal_error(const char* message);
void _chpl_msg(int verbose_level, const char* fmt, ...)
  __attribute__((format(printf, 2, 3)));

#endif
