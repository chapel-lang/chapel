#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chplio.h"
#include "chplrt.h"
#include "error.h"

static void _fscan_error_check(int err, _int32 lineno, _string filename) {
  if (err == EOF)
    _printError("read failed: eof", lineno, filename);
  else if (err < 0)
    _printError(string_concat("read failed: ", strerror(errno)),
                lineno, filename);
  else if (err == 0)
    _printError("read failed: value not read", lineno, filename);
}

_string _fscan_string(FILE* fp, _int32 lineno, _string filename) {
  char result[_default_string_length];
  char dsl[1024];
  int err;

  err = fscanf(fp, "%255s", result);
  _fscan_error_check(err, lineno, filename);
  if (strlen(result) == (_default_string_length - 1)) {
    sprintf(dsl, "%d", _default_string_length);
    _printError(string_concat("maximum string length is ", dsl),
                lineno, filename);
  }
  return string_copy(result);
}

_int32 _fscan_int32(FILE* fp, _int32 lineno, _string filename) {
  _int32 result;
  int err;

  err = fscanf(fp, "%d", &result);
  _fscan_error_check(err, lineno, filename);
  return result;
}

_uint32 _fscan_uint32(FILE* fp, _int32 lineno, _string filename) {
  _uint32 result;
  int err;

  err = fscanf(fp, "%u", &result);
  _fscan_error_check(err, lineno, filename);
  return result;
}

_real64 _fscan_real64(FILE* fp, _int32 lineno, _string filename) {
  _real64 result;
  int err;

  err = fscanf(fp, "%lg", &result);
  _fscan_error_check(err, lineno, filename);
  return result;
}

_bool _fscan_literal(FILE* fp, _string val, _bool ignoreWhiteSpace,
                     _int32 lineno, _string filename) {
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
