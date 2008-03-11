#ifndef _chplio_H_
#define _chplio_H_

#include "chpltypes.h"
#include <stdio.h>
#include <inttypes.h>

#define _default_format_read_int64        "%" SCNd64

extern char* _default_format_write_complex64;
extern char* _default_format_write_complex128;

#define _default_format_read_string       "%255s"   
#define _default_format_write_string      "%s"

#define _default_string_length             256

#define get_errno() (strerror(errno))
#define get_stdin() (stdin)
#define get_stdout() (stdout)
#define get_stderr() (stderr)
#define get_nullfile() (0)

typedef FILE* _cfile;

_string _fscan_string(FILE* fp, int32_t lineno, _string filename);
int32_t _fscan_int32(FILE* fp, int32_t lineno, _string filename);
uint32_t _fscan_uint32(FILE* fp, int32_t lineno, _string filename);
_real64 _fscan_real64(FILE* fp, int32_t lineno, _string filename);

chpl_bool _fscan_literal(FILE* fp, _string val, chpl_bool ignoreWhiteSpace,
                          int32_t lineno, _string filename);

void _readToEndOfLine(FILE* fp);

#endif
