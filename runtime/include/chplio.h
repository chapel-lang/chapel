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

#define chpl_cstdin() (stdin)
#define chpl_cstdout() (stdout)
#define chpl_cstderr() (stderr)
#define chpl_cerrno() (strerror(errno))
#define chpl_cnullfile() (0)
#define chpl_fopen(filename, modestring) fopen(filename, modestring)
#define chpl_fclose(file) fclose(file)
#define chpl_fflush(file) fflush(file)
#define chpl_fprintf(file, s) fprintf(file, "%s", s)

typedef FILE* _cfile;

chpl_string _fscan_string(FILE* fp, int32_t lineno, chpl_string filename);
int32_t _fscan_int32(FILE* fp, int32_t lineno, chpl_string filename);
uint32_t _fscan_uint32(FILE* fp, int32_t lineno, chpl_string filename);
_real64 _fscan_real64(FILE* fp, int32_t lineno, chpl_string filename);

chpl_bool _fscan_literal(FILE* fp, chpl_string val, chpl_bool ignoreWhiteSpace,
                          int32_t lineno, chpl_string filename);

void _readToEndOfLine(FILE* fp);

char* chpl_refToString(void* ref);
char* chpl_wideRefToString(void* wideref);

void binfwrite (const void *ptr, size_t size, size_t nmemb, FILE *stream, long *res, int *err);
void binfread (void *ptr, size_t size, size_t nmemb, FILE *stream, long *res, int *err);

#endif
