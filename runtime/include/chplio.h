#ifndef _chplio_H_
#define _chplio_H_

#include <stdio.h>
#include "array.h" /* only needed if _write_array2_float64 is here? */
#include "chpltypes.h"
#include "domain.h"

void _write_linefeed(FILE* outfile);
void _write_integer64(FILE* outfile, char* format, _integer64 val);
void _write_string(FILE* outfile, char* format, _string val);
void _write_domain1(FILE* outfile, char* format, _domain1 val);

/* Need to generalize this and/or generate it automatically: */
void _write_array1_integer64(FILE* outfile, char* format, _array1_integer64 arr);
void _write_array2_float64(FILE* outfile, char* format, _array2_float64 arr);

#endif

