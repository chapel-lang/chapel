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
extern CFILEPTR _STDINCFILEPTR;
extern CFILEPTR _STDOUTCFILEPTR;
extern CFILEPTR _STDERRCFILEPTR;

void initChplio(void);

void _chpl_fwrite_float_help(FILE* fp, _float64 val);
void _chpl_fread_string_help(FILE* fp, _string* val);
int  _readLitChar(FILE* fp, _string val, int ignoreWhiteSpace);

#endif

