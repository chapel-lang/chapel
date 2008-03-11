#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "chplio.h"
#include "chplrt.h"
#include "error.h"

static void _fscan_error_check(int err, int32_t lineno, _string filename) {
  if (err == EOF)
    chpl_error("read failed: eof", lineno, filename);
  else if (err < 0)
    chpl_error(string_concat("read failed: ", strerror(errno), lineno, filename),
                lineno, filename);
  else if (err == 0)
    chpl_error("read failed: value not read", lineno, filename);
}

_string _fscan_string(FILE* fp, int32_t lineno, _string filename) {
  char result[_default_string_length];
  char dsl[1024];
  int err;

  err = fscanf(fp, "%255s", result);
  _fscan_error_check(err, lineno, filename);
  if (strlen(result) == (_default_string_length - 1)) {
    sprintf(dsl, "%d", _default_string_length);
    chpl_error(string_concat("maximum string length is ", dsl, lineno, filename),
                lineno, filename);
  }
  return _glom_strings(1, result);
}

int32_t _fscan_int32(FILE* fp, int32_t lineno, _string filename) {
  int32_t result;
  int err;

  err = fscanf(fp, "%" SCNd32, &result);
  _fscan_error_check(err, lineno, filename);
  return result;
}

uint32_t _fscan_uint32(FILE* fp, int32_t lineno, _string filename) {
  uint32_t result;
  int err;

  err = fscanf(fp, "%" SCNu32, &result);
  _fscan_error_check(err, lineno, filename);
  return result;
}

_real64 _fscan_real64(FILE* fp, int32_t lineno, _string filename) {
  _real64 result;
  int err;

  err = fscanf(fp, "%lg", &result);
  _fscan_error_check(err, lineno, filename);
  return result;
}

chpl_bool _fscan_literal(FILE* fp, _string val, chpl_bool ignoreWhiteSpace,
                          int32_t lineno, _string filename) {
  char ch  = ' ';
  int err;

  if (ignoreWhiteSpace) {
    while ((ch == ' ') || (ch == '\n') || (ch == '\r') || (ch == '\t')) {
      err = fscanf(fp, "%c", &ch);
      _fscan_error_check(err, lineno, filename);
    }
  } else {
    err = fscanf(fp, "%c", &ch);
    _fscan_error_check(err, lineno, filename);
  }

  if (ch != *val) {
    err = ungetc(ch, fp);
    _fscan_error_check(err, lineno, filename);
    return false;
  }
  return true;
}

void _readToEndOfLine(FILE* fp) {
  char result[_default_string_length];
  fgets(result, _default_string_length-1 , fp);
}
