#ifndef _chplio_H_
#define _chplio_H_

#include <stdio.h>
#include "chpltypes.h"
#include "domain.h"

void _write_linefeed(FILE* outfile);
void _write_integer64(FILE* outfile, char* format, _integer64 val);
void _write_string(FILE* outfile, char* format, _string val);
void _write_domain1(FILE* outfile, char* format, _domain1 val);

#endif

