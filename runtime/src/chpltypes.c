#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "chplmem.h"
#include "chplrt.h"
#include "chpltypes.h"
#include "error.h"

#define NANSTRING "nan"
#define NEGINFSTRING "-inf"
#define POSINFSTRING "inf"

char* _default_format_write_complex32 = "%g + %gi";
char* _default_format_write_complex64 = "%lg + %lgi";

/* _glom_strings() expects every argument after the first to be 
   of type char*. */

char* _glom_strings(int numstrings, ...) {
  static int maxNumStrings = 0;
  static char** stringlist = NULL;
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
    strcat(buffer, ".0");
  }
}


char* _chpl_tostring_float32(_float32 x, char* format) {
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


char* _chpl_tostring_float64(_float64 x, char* format) {
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


char* _chpl_tostring_complex32( _complex32 x, char* format) {
  if (isnan(x.re) || isnan(x.im)) {
    return NANSTRING;
  }
  if (format == _default_format_write_complex32) {
    char* re = _chpl_tostring_float32(x.re, "%g");
    _float32 imval = x.im;
    char* op = " + ";
    if (imval < 0) {
      imval = -x.im;
      op = " - ";
    }
    char* im = _chpl_tostring_float32(imval, "%g");
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


char* _chpl_tostring_complex64( _complex64 x, char* format) {
  if (isnan(x.re) || isnan(x.im)) {
    return NANSTRING;
  }
  if (format == _default_format_write_complex64) {
    char* re = _chpl_tostring_float64(x.re, "%g");
    _float64 imval = x.im;
    char* op = " + ";
    if (imval < 0) {
      imval = -x.im;
      op = " - ";
    }
    char* im = _chpl_tostring_float64(imval, "%g");
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
float2int( _float64 f) {
  return *(_uint64*)(&f);
}


_int64 
object2int( _chpl_object o) {
  return (intptr_t) o;
}


_complex32 
_chpl_complex32( _float32 r, _float32 i) {
  _complex32 ret_c = {r, i};
  return ret_c;
}

_complex64 
_chpl_complex64( _float64 r, _float64 i) {
  _complex64 ret_c = {r, i};
  return ret_c;
}

_complex128 
_chpl_complex128( _float128 r, _float128 i) {
  _complex128 ret_c = {r, i};
  return ret_c;
}
