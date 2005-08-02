#ifndef _chplio_H_
#define _chplio_H_

#include <stdio.h>
#include "chpltypes.h"
#include "domain.h"

#define _default_format_write_boolean     "%s"
#define _default_format_read_boolean      "%s"

#define _default_format_read_integer64    "%lld" /* long long int */
#define _default_format_write_integer64   "%lld" /* long long int */

#define _default_format_read_float64      "%lg"  /* double */
#define _default_format_write_float64     "%lg"  /* double */

#define _default_format_read_complex128   "%lg  +  %lg%c"
#define _default_format_write_complex128  "%g + %gi"

#define _default_format_read_string       "%255s"   
#define _default_format_write_string      "%s"

#define _default_string_length              256


typedef FILE* CFILEPTR;
extern CFILEPTR _NULLCFILEPTR;

int _string_to_boolean(char* valueString, _boolean* val);

void _chpl_write_boolean(_boolean val);
void _chpl_write_integer(_integer64 val);
void _chpl_write_float(_float64 val);
void _chpl_write_string(_string val);
void _chpl_write_nil(void* val);

void _chpl_write_linefeed(void);

void _chpl_read_boolean(_boolean* val);
void _chpl_read_integer(_integer64* val);
void _chpl_read_float(_float64* val);
void _chpl_read_string(_string* val);


void _chpl_read_complex(_complex128** val);

#endif

