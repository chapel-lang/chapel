#ifndef _chplio_H_
#define _chplio_H_

#include <stdio.h>
#include "array.h" /* only needed if _write_array2_float64 is here? */
#include "chpltypes.h"
#include "domain.h"

#define _default_format_write_boolean     "%s"
#define _default_format_read_boolean      "%s"

#define _default_format_read_integer64    "%lld" // long long int
#define _default_format_write_integer64   "%lld" // long long int

#define _default_format_read_float32      "%g"   // float
#define _default_format_write_float32     "%g"   // double
#define _default_format_read_float64      "%lg"  // double
extern char* _default_format_write_float64; //     "%g"   // double
#define _default_format_read_float128     "%llg" // long double
#define _default_format_write_float128    "%Lg"  // long double 

#define _default_format_read_complex128   "%lg  +  %lg%c"
#define _default_format_write_complex128  "%g + %gi"

#define _default_string_length              256
#define _default_format_read_string       "%255s"   
#define _default_format_write_string      "%s"

#define _default_format_read_enum         "%255s"
#define _default_format_write_enum        "%s"

/*
#define _default_format_write_domain1 "%d..%d by %d"
void _write_domain1(FILE* outfile, char* format, _domain1 val);
*/
#define _write_domain(F, dom) \
  fprintf(F, "%d..%d by %d", dom.dim_info[0].lo, dom.dim_info[0].hi, dom.dim_info[0].str)


void _write_linefeed(FILE* outfile);
int _string_to_boolean(char* valueString, _boolean* val);
void _read_boolean(FILE* outfile, char* format, _boolean* val);
void _write_boolean(FILE* outfile, char* format, _boolean val);

void _read_integer64(FILE* infile, char* format, _integer64* val);
void _write_integer64(FILE* outfile, char* format, _integer64 val);
void _read_float64(FILE* infile, char* format, _float64* val);
void _write_float64(FILE* outfile, char* format, _float64 val);

void _read_complex128(FILE* infile, char* format, _complex128* val);
void _write_complex128(FILE* outfile, char* format, _complex128 val);

void _read_string(FILE* infile, char* format, _string* val);
void _write_string(FILE* outfile, char* format, _string val);

#endif

