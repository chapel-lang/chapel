#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "chplfp.h"
#include "chplmem.h"
#include "chplrt.h"
#include "chpltypes.h"
#include "error.h"

#define NANSTRING "nan"
#define NEGINFSTRING "-inf"
#define POSINFSTRING "inf"

const char* _default_format_write_complex64 = "%g + %gi";
const char* _default_format_write_complex128 = "%lg + %lgi";
const char* _default_format_write_complex256 = "%Lg + %Lgi";

char* _glom_strings(int numstrings, ...) {
  va_list ap;
  int i, len;
  char* str;

  va_start(ap, numstrings);
  len = 0;
  for (i=0; i<numstrings; i++)
    len += strlen(va_arg(ap, char*));
  va_end(ap);

  str = (char*)_chpl_malloc(len+1, sizeof(char), "_glom_strings result", 0, 0);

  va_start(ap, numstrings);
  str[0] = '\0';
  for (i=0; i<numstrings; i++)
    strcat(str, va_arg(ap, char*));
  va_end(ap);

  return str;
}


_string
string_concat(_string x, _string y) {
  return _glom_strings(2, x, y);
}


_string
string_strided_select(_string x, int low, int high, int stride) {
  char* result =
    _chpl_malloc((high - low + 2), sizeof(char), "_chpl_string_strided_select temp", 0, 0);
  _string src = x + low - 1;
  char* dst = result;
  while (src - x <= high - 1) {
    *dst++ = *src;
    src += stride;
  }
  *dst = '\0';
  return _glom_strings(1, result);
}

_string
string_select(_string x, int low, int high) {
  return string_strided_select(x, low, high, 1);
}

_string
string_index(_string x, int i) {
  char buffer[2];
  sprintf(buffer, "%c", x[i-1]);
  return _glom_strings(1, buffer);
}


_bool
string_contains(_string x, _string y) {
  if (strstr(x, y))
    return true;
  else
    return false;
}


_bool
string_equal(_string x, _string y) {
  if (!strcmp(x, y)) {
    return true;
  } else {
    return false;
  }
}


_int64
string_length(_string x) {
  return strlen(x);
}


_int64 real2int( _real64 f) {
  // need to use a union here rather than a pointer cast to avoid gcc
  // warnings when compiling -O3
  union {     
    _real64 r;
    _uint64 u;
  } converter;

  converter.r = f;
  return converter.u;
}


_int64 
object2int( _chpl_object o) {
  return (intptr_t) o;
}


_complex64 
_chpl_complex64( _real32 r, _real32 i) {
  _complex64 ret_c = {r, i};
  return ret_c;
}

_complex128 
_chpl_complex128( _real64 r, _real64 i) {
  _complex128 ret_c = {r, i};
  return ret_c;
}

_complex256 
_chpl_complex256( _real128 r, _real128 i) {
  _complex256 ret_c = {r, i};
  return ret_c;
}

_timervalue* _now_timer_help(_timervalue* time) {
  struct timezone tz;
  gettimeofday(time, &tz);
  return time;
}
_timervalue _default_timer;

_int32 _now_year(void) {
  struct tm * now;
  struct timezone tz;
  _timervalue t;
  gettimeofday(&t, &tz);
  now = localtime(&t.tv_sec);
  return now->tm_year;
}

_int32 _now_month(void) {
  struct tm * now;
  struct timezone tz;
  _timervalue t;
  gettimeofday(&t, &tz);
  now = localtime(&t.tv_sec);
  return now->tm_mon;
}

_int32 _now_day(void) {
  struct tm * now;
  struct timezone tz;
  _timervalue t;
  gettimeofday(&t, &tz);
  now = localtime(&t.tv_sec);
  return now->tm_mday;
}

_real64 _now_time(void) {
  struct tm * now;
  struct timezone tz;
  _timervalue t;
  gettimeofday(&t, &tz);
  now = localtime(&t.tv_sec);
  return (_real64)(now->tm_hour)*3600.0e+6 +
    (_real64)(now->tm_min)*60.0e+6 +
    (_real64)(now->tm_sec)*1.0e+6 +
    (_real64)(t.tv_usec);
}
