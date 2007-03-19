#ifndef _chplio_H_
#define _chplio_H_

#include <stdio.h>
#include "chpltypes.h"

#define _default_format_read_int64        "%lld"

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

_string string_fscanf(FILE* fp, _int32 lineno, _string filename);
_bool readLit(FILE* fp, _string val, _bool ignoreWhiteSpace);

#endif
