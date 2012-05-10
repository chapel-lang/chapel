#ifndef _chplio_H_
#define _chplio_H_

#include "chpltypes.h"
#include <stdio.h>
#include <inttypes.h>

#define _default_string_length             256
#define _default_format_read_string       "%255s"   

typedef FILE* _cfile;

#define chpl_cnullfile() (0)

char* chpl_refToString(void* ref);
char* chpl_wideRefToString(void* wideref);

#endif
