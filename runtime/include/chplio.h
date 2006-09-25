#ifndef _chplio_H_
#define _chplio_H_

#include <stdio.h>
#include "chpltypes.h"

#define _default_format_write_bool        "%s"
#define _default_format_read_bool         "%s"

#define _default_format_read_int64        "%lld" /* long long int */

#define _default_format_write_int8        "%d"
#define _default_format_write_int16       "%d"
#define _default_format_write_int32       "%d"
#define _default_format_write_int64       "%lld" /* long long int */

#define _default_format_read_uint64       "%llu"

#define _default_format_write_uint8       "%u"
#define _default_format_write_uint16      "%u"
#define _default_format_write_uint32      "%u"
#define _default_format_write_uint64      "%llu"

#define _default_format_read_float64      "%lg"  /* double */

#define _default_format_write_float32     "%lg"
#define _default_format_write_float64     "%lg"  /* double */

#define _default_format_read_complex64    "%g + %g%c"
#define _default_format_read_complex128   "%lg + %lg%c"
#define _default_format_read_complex256   "%llg + %llg%c"

extern char* _default_format_write_complex64;
extern char* _default_format_write_complex128;
extern char* _default_format_write_complex256;

#define _default_format_read_string       "%255s"   
#define _default_format_write_string      "%s"

#define _default_string_length             256

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
