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

char* _default_format_write_complex64 = "%g + %gi";
char* _default_format_write_complex128 = "%lg + %lgi";
char* _default_format_write_complex256 = "%Lg + %Lgi";

/* _glom_strings() expects every argument after the first to be 
   of type char*. */
static int maxNumStrings = 0;
static char** stringlist = NULL;


int isGlomStringsMem(void* ptr) {
  return (ptr == stringlist);
}


char* _glom_strings(int numstrings, ...) {
  int i;

  if (numstrings > maxNumStrings) {
    maxNumStrings = numstrings;
    stringlist = (char**)_chpl_realloc(stringlist, numstrings, sizeof(char*),
                                       "stringlist buffer in _glom_strings");
  }

  va_list ap;
  va_start(ap, numstrings);
  for (i=0; i<numstrings; i++) {
    stringlist[i] = va_arg(ap, char*);
  }
  va_end(ap);

  unsigned int totlen = 0;
  for (i=0; i<numstrings; i++) {
    totlen += strlen(stringlist[i]);
  }

  char* newstring = (char*)_chpl_malloc((totlen + 1), sizeof(char), 
                                        "_glom_strings result");
  newstring[0] = '\0';
  for (i=0; i<numstrings; i++) {
    strcat(newstring, stringlist[i]);
  }

  if (strlen(newstring) > totlen) {
    char* message = "_glom_strings() buffer overflow";
    printInternalError(message);
  }

  return newstring;
}


/* _string _copy_string(_string* lhs, _string rhs) { */
/*   /\*** REALLOC vs MALLOC */
/*        We have memory leaks when dealing with strings. */
/*        *lhs = (char*)_chpl_realloc(*lhs, (strlen(rhs)+1), sizeof(char), "string copy"); *\/ */
/*   *lhs = (char*)_chpl_malloc(strlen(rhs)+1, sizeof(char), "string copy"); */

/*   strcpy(*lhs, rhs); */
/*   return *lhs; */
/* } */


char* _chpl_tostring_bool(_bool x, char* format) {
  if (x) {
    return _glom_strings(1, "true");
  } else {
    return _glom_strings(1, "false");
  }
}


char* _chpl_tostring_int32(_int32 x, char* format) {
  char buffer[256];
  sprintf(buffer, format, x);
  return _glom_strings(1, buffer);
}

char* _chpl_tostring_int64(_int64 x, char* format) {
  char buffer[256];
  sprintf(buffer, format, x);
  return _glom_strings(1, buffer);
}


char* _chpl_tostring_uint32(_uint32 x, char* format) {
  char buffer[256];
  sprintf(buffer, format, x);
  return _glom_strings(1, buffer);
}


char* _chpl_tostring_uint64(_uint64 x, char* format) {
  char buffer[256];
  sprintf(buffer, format, x);
  return _glom_strings(1, buffer);
}


static void ensurePointZero(char* buffer) {
  if (!strchr(buffer, '.') && !strchr(buffer, 'e')) {
    if (strchr(buffer, 'i')) {
      buffer[strlen(buffer)-1] = '\0';
      strcat(buffer, ".0i");
    } else {
      strcat(buffer, ".0");
    }
  }
}


char* _chpl_tostring_real32(_real32 x, char* format) {
  char buffer[256];
  sprintf(buffer, format, x);
  if (isnan(x)) {
    return NANSTRING;
  } else if (isinf(x)) {
    if (x < 0) {
      return NEGINFSTRING;
    } else {
      return POSINFSTRING;
    }
  } else {
    ensurePointZero(buffer);
  }
  return _glom_strings(1, buffer);
}


char* _chpl_tostring_real64(_real64 x, char* format) {
  char buffer[256];
  sprintf(buffer, format, x);
  if (isnan(x)) {
    return NANSTRING;
  } else if (isinf(x)) {
    if (x < 0) {
      return NEGINFSTRING;
    } else {
      return POSINFSTRING;
    }
  } else {
    ensurePointZero(buffer);
  }
  return _glom_strings(1, buffer);
}


char* _chpl_tostring_real128(_real128 x, char* format) {
  char buffer[256];
  sprintf(buffer, format, x);
  if (isnan(x)) {
    return NANSTRING;
  } else if (isinf(x)) {
    if (x < 0) {
      return NEGINFSTRING;
    } else {
      return POSINFSTRING;
    }
  } else {
    ensurePointZero(buffer);
  }
  return _glom_strings(1, buffer);
}


char* _chpl_tostring_complex64( _complex64 x, char* format) {
  if (isnan(x.re) || isnan(x.im)) {
    return NANSTRING;
  }
  if (format == _default_format_write_complex64) {
    char* re = _chpl_tostring_real32(x.re, "%g");
    _real32 imval = x.im;
    char* op = " + ";
    if (imval < 0) {
      imval = -x.im;
      op = " - ";
    }
    char* im = _chpl_tostring_real32(imval, "%g");
    // Not sure how to test for negative zero to handle this earlier
    if (strcmp(im, "-0.0") == 0) {
      op = " - ";
      im = "0.0";
    }
    return _glom_strings(4, re, op, im, "i");
  } else {
    char buffer[256];
    sprintf(buffer, format, x.re, x.im);
    return _glom_strings(1, buffer);
  }
}


char* _chpl_tostring_complex128( _complex128 x, char* format) {
  if (isnan(x.re) || isnan(x.im)) {
    return NANSTRING;
  }
  if (format == _default_format_write_complex128) {
    char* re = _chpl_tostring_real64(x.re, "%g");
    _real64 imval = x.im;
    char* op = " + ";
    if (imval < 0) {
      imval = -x.im;
      op = " - ";
    }
    char* im = _chpl_tostring_real64(imval, "%g");
    // Not sure how to test for negative zero to handle this earlier
    if (strcmp(im, "-0.0") == 0) {
      op = " - ";
      im = "0.0";
    }
    return _glom_strings(4, re, op, im, "i");
  } else {
    char buffer[256];
    sprintf(buffer, format, x.re, x.im);
    return _glom_strings(1, buffer);
  }
}


char* _chpl_tostring_complex256( _complex256 x, char* format) {
  if (isnan(x.re) || isnan(x.im)) {
    return NANSTRING;
  }
  if (format == _default_format_write_complex256) {
    char* re = _chpl_tostring_real128(x.re, "%Lg");
    _real128 imval = x.im;
    char* op = " + ";
    if (imval < 0) {
      imval = -x.im;
      op = " - ";
    }
    char* im = _chpl_tostring_real128(imval, "%Lg");
    // Not sure how to test for negative zero to handle this earlier
    if (strcmp(im, "-0.0") == 0) {
      op = " - ";
      im = "0.0";
    }
    return _glom_strings(4, re, op, im, "i");
  } else {
    char buffer[256];
    sprintf(buffer, format, x.re, x.im);
    return _glom_strings(1, buffer);
  }
}


_string
string_concat(_string x, _string y) {
  return _glom_strings(2, x, y);
}


_string
string_strided_select(_string x, int low, int high, int stride) {
  _string result =
    _chpl_malloc((high - low + 2), sizeof(char), "_chpl_string_strided_select temp");
  _string src = x + low - 1;
  _string dst = result;
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


_int64 
real2int( _real64 f) {
  return *(_uint64*)(&f);
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
