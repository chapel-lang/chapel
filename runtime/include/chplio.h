#ifndef _chplio_H_
#define _chplio_H_

#include <stdio.h>
#include "chpltypes.h"

#define _default_format_write_bool     "%s"
#define _default_format_read_bool      "%s"

#define _default_format_read_int64    "%lld" /* long long int */
#define _default_format_write_int64   "%lld" /* long long int */

#define _default_format_read_float64      "%lg"  /* double */
#define _default_format_write_float64     "%lg"  /* double */

#define _default_format_read_complex128   "%lg  +  %lg%c"
#define _default_format_write_complex128  "%g + %gi"

#define _default_format_read_string       "%255s"   
#define _default_format_write_string      "%s"

#define _default_string_length              256

#define get_errno() (strerror(errno))
#define get_eof() (EOF)
#define get_stdin() (stdin)
#define get_stdout() (stdout)
#define get_stderr() (stderr)
#define get_nullfile() (0)

typedef FILE* _cfile;

_string string_fscanf(FILE* fp);
_bool readLit(FILE* fp, _string val, _bool ignoreWhiteSpace);

#endif
