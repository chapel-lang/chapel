#ifndef _chplio_H_
#define _chplio_H_

#include <stdio.h>
#include "array.h" /* only needed if _write_array2_float64 is here? */
#include "chpltypes.h"
#include "domain.h"

#define _default_format_write_boolean     "%s"

#define _default_format_read_integer64    "%lld" // long long int
#define _default_format_write_integer64   "%lld" // long long int

#define _default_format_read_float32      "%g"   // float
#define _default_format_write_float32     "%g"   // double
#define _default_format_read_float64      "%lg"  // double
#define _default_format_write_float64     "%g"   // double
#define _default_format_read_float128     "%llg" // long double
#define _default_format_write_float128    "%Lg"  // long double 

#define _default_format_write_complex128  "%g + %gi"

/**** IF CHANGING THIS, change _read_string() in chplio.c as well ****/
#define _default_format_read_string       "%255s"   
#define _default_format_write_string      "%s"

#define _default_format_read_enum         "%255s"
#define _default_format_write_enum        "%s"

#define _default_format_write_domain1 "%d..%d by %d"


void _write_linefeed(FILE* outfile);
void _write_boolean(FILE* outfile, char* format, _boolean val);
void _read_integer64(FILE* infile, char* format, _integer64* val);
void _write_integer64(FILE* outfile, char* format, _integer64 val);
void _read_float64(FILE* infile, char* format, _float64* val);
void _write_float64(FILE* outfile, char* format, _float64 val);
void _write_complex128(FILE* outfile, char* format, _complex128 val);
void _read_string(FILE* infile, char* format, _string* val);
void _write_string(FILE* outfile, char* format, _string val);
void _write_domain1(FILE* outfile, char* format, _domain1 val);

/* Need to generalize this and/or generate it automatically: */
void _write_array1_integer64(FILE* outfile, char* format, _array1_integer64 arr);
void _write_array2_float64(FILE* outfile, char* format, _array2_float64 arr);

#endif

