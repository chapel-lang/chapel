#ifndef _chplio_H_
#define _chplio_H_

#include "chpltypes.h"
#include <stdio.h>
#include <inttypes.h>

#define _default_string_length             256
#define _default_format_read_string       "%255s"   

typedef FILE* _cfile;

static inline _cfile chpl_cnullfile(void) { return (_cfile) 0; }

char* chpl_refToString(void* ref);
char* chpl_wideRefToString(void* wideref);

#endif
