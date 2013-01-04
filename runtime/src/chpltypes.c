#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <chplfp.h>
#include "chpl-mem.h"
#include "chplrt.h"
#include "chpltimers.h"
#include "chpltypes.h"
#include "chpl-comm.h"
#include "chpl-comm-compiler-macros.h"
#include "error.h"

#define NANSTRING "nan"
#define NEGINFSTRING "-inf"
#define POSINFSTRING "inf"

const c_locale_t _rootLocaleID = {0};

const char* _default_format_write_complex64 = "%g + %gi";
const char* _default_format_write_complex128 = "%g + %gi";

char* chpl_glom_strings(int numstrings, ...) {
  va_list ap;
  int i, len;
  char* str;

  va_start(ap, numstrings);
  len = 0;
  for (i=0; i<numstrings; i++)
    len += strlen(va_arg(ap, char*));
  va_end(ap);

  str = (char*)chpl_mem_allocMany(len+1, sizeof(char),
                                  CHPL_RT_MD_GLOM_STRINGS_DATA, 0, 0);

  va_start(ap, numstrings);
  str[0] = '\0';
  for (i=0; i<numstrings; i++)
    strcat(str, va_arg(ap, char*));
  va_end(ap);

  return str;
}


chpl_string chpl_format(chpl_string format, ...) {
  va_list ap;
  char z[128];

  va_start(ap, format);
  if (!vsnprintf(z, 127, format, ap))
    chpl_error("overflow encountered in format", 0, 0);
  return string_copy(z, 0, 0);
}


#ifndef LAUNCHER
struct __chpl____wide_chpl_string {
  c_locale_t locale;
  chpl_string addr;
  int64_t size;
};

chpl_string
chpl_wide_string_copy(struct __chpl____wide_chpl_string* x, int32_t lineno, chpl_string filename) {
  if (x->locale.node == chpl_localeID)
    return string_copy(x->addr, lineno, filename);
  else {
    chpl_string s;
    CHPL_COMM_WIDE_GET_STRING(s, *x,
                              -CHPL_TYPE_chpl_string /* this is unfortunate */,
                              lineno, filename);
    return s;
  }
}
#endif


chpl_string
string_copy(chpl_string x, int32_t lineno, chpl_string filename)
{
  char *z;

  // hilde sez: if the input string is null, just return null.
  if (x == NULL)
    return NULL;

  z = (char*)chpl_mem_allocMany(strlen(x)+1, sizeof(char),
                                      CHPL_RT_MD_STRING_COPY_DATA,
                                      lineno, filename);
  return strcpy(z, x);
}


chpl_string
string_concat(chpl_string x, chpl_string y, int32_t lineno, chpl_string filename) {
  char *z = (char*)chpl_mem_allocMany(strlen(x)+strlen(y)+1, sizeof(char),
                                      CHPL_RT_MD_STRING_CONCAT_DATA,
                                      lineno, filename);
  z[0] = '\0';
  strcat(z, x);
  strcat(z, y);
  return z;
}


chpl_string
string_strided_select(chpl_string x, int low, int high, int stride, int32_t lineno, chpl_string filename) {
  int64_t length = string_length(x);
  char* result = NULL;
  char* dst = NULL;
  chpl_string src = stride > 0 ? x + low - 1 : x + high - 1;
  int size = high - low >= 0 ? high - low : 0;
  if (low < 1 || low > length || high > length) {
    chpl_error("string index out of bounds", lineno, filename);
  }
  result = chpl_mem_allocMany(size + 2, sizeof(char),
                              CHPL_RT_MD_STRING_STRIDED_SELECT_DATA,
                              lineno, filename);
  dst = result;
  if (stride > 0) {
    while (src - x <= high - 1) {
      *dst++ = *src;
      src += stride;
    }
  } else {
    while (src - x >= low - 1) {
      *dst++ = *src;
      src += stride;
    }
  }
  *dst = '\0';
  return chpl_glom_strings(1, result);
}

chpl_string
string_select(chpl_string x, int low, int high, int32_t lineno, chpl_string filename) {
  return string_strided_select(x, low, high, 1, lineno, filename);
}

chpl_string
string_index(chpl_string x, int i, int32_t lineno, chpl_string filename) {
  char buffer[2];
  if (i-1 < 0 || i-1 >= string_length(x))
    chpl_error("string index out of bounds", lineno, filename);
  sprintf(buffer, "%c", x[i-1]);
  return chpl_glom_strings(1, buffer);
}


chpl_bool
string_contains(chpl_string x, chpl_string y) {
  if (strstr(x, y))
    return true;
  else
    return false;
}


int32_t chpl_string_compare(chpl_string x, chpl_string y) {
  return (int32_t)strcmp(x, y);
}


int64_t
string_length(chpl_string x) {
  return strlen(x);
}


int64_t real2int( _real64 f) {
  // need to use a union here rather than a pointer cast to avoid gcc
  // warnings when compiling -O3
  union {     
    _real64 r;
    uint64_t u;
  } converter;

  converter.r = f;
  return converter.u;
}


int64_t 
object2int( _chpl_object o) {
  return (intptr_t) o;
}


_timervalue* chpl_now_timer_help(_timervalue* time) {
  gettimeofday(time, NULL);
  return time;
}
_timervalue chpl_default_timer;

int32_t chpl_now_year(void) {
  struct tm * now;
  _timervalue t;
  gettimeofday(&t, NULL);
  now = localtime(&t.tv_sec);
  return now->tm_year;
}

int32_t chpl_now_month(void) {
  struct tm * now;
  _timervalue t;
  gettimeofday(&t, NULL);
  now = localtime(&t.tv_sec);
  return now->tm_mon;
}

int32_t chpl_now_day(void) {
  struct tm * now;
  _timervalue t;
  gettimeofday(&t, NULL);
  now = localtime(&t.tv_sec);
  return now->tm_mday;
}

int32_t chpl_now_dow(void) {
  struct tm * now;
  _timervalue t;
  gettimeofday(&t, NULL);
  now = localtime(&t.tv_sec);
  return now->tm_wday;
}
