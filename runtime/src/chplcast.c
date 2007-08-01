#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "chplcast.h"
#include "chplrt.h"
#include "chpltypes.h"
#include "chplfp.h"

/*
 *  string to complex
 */
static _string _get_string_imag_part(_string s) {
  const char* ps = s;
  while (*ps == ' ') // eat space
    ps++;
  if (*ps == '-' || *ps == '+') // eat sign
    ps++;
  if (!isdigit((int)*ps)) // test for digit
    return NULL;
  while (isdigit((int)*ps)) // eat integral part
    ps++;
  if (*ps == '.') { // test for decimal
    ps++;
    if (!isdigit((int)*ps)) // test for digit
      return NULL;
    while (isdigit((int)*ps)) // eat fractional part
      ps++;
  }
  if (*ps == 'e' || *ps == 'E') { // test for exponent
    ps++;
    if (*ps == '+' || *ps == '-') // test for exponent sign
      ps++;
    if (!isdigit((int)*ps)) // test for digit
      return NULL;
    while (isdigit((int)*ps)) // eat exponent part
      ps++;
  }
  while (*ps == ' ') // eat space
    ps++;
  if (*ps == '+' || *ps == '-') // check for imaginary part
    return ++ps;
  return NULL;
}

#define string_to_complex(type, realtype)               \
  type _string_to##type(_string x) {                    \
    _string im;                                         \
    type c = {(realtype)0, (realtype)0};                \
    c.re = (realtype)(atof(x));                         \
    im = _get_string_imag_part(x);                      \
    if (im)                                             \
      c.im = (realtype)(atof(im));                      \
    return c;                                           \
  }

string_to_complex(_complex64, _real32)
string_to_complex(_complex128, _real64)
string_to_complex(_complex256, _real128)

/*
 *  int and uint to string
 */
#define integral_to_string(type, format)        \
  _string type##_to_string(type x) {            \
    char buffer[256];                           \
    sprintf(buffer, format, x);                 \
    return string_copy(buffer);                 \
  }

integral_to_string(_int8, "%d")
integral_to_string(_int16, "%d")
integral_to_string(_int32, "%d")
integral_to_string(_int64, "%lld")
integral_to_string(_uint8, "%u")
integral_to_string(_uint16, "%u")
integral_to_string(_uint32, "%u")
integral_to_string(_uint64, "%llu")

/*
 *  real and imag to string
 */
static void ensureDecimal(char* buffer) {
  /* add decimal if one does not exist */
  if (!strchr(buffer, '.') && !strchr(buffer, 'e')) {
    if (strchr(buffer, 'i')) {
      buffer[strlen(buffer)-1] = '\0';
      strcat(buffer, ".0i");
    } else {
      strcat(buffer, ".0");
    }
  }
}

#define NANSTRING "nan"
#define NEGINFSTRING "-inf"
#define POSINFSTRING "inf"

#define real_to_string(type, format)          \
  _string type##_to_string(type x) {          \
    if (isnan(x)) {                           \
      return NANSTRING;                       \
    } else if (isinf(x)) {                    \
      if (x < 0) {                            \
        return NEGINFSTRING;                  \
      } else {                                \
        return POSINFSTRING;                  \
      }                                       \
    } else {                                  \
      char buffer[256];                       \
      sprintf(buffer, format, x);             \
      ensureDecimal(buffer);                  \
      return string_copy(buffer);             \
    }                                         \
  }

real_to_string(_real32, "%lg")
real_to_string(_real64, "%lg")
real_to_string(_real128, "%Lg")
real_to_string(_imag32, "%lgi")
real_to_string(_imag64, "%lgi")
real_to_string(_imag128, "%Lgi")

/*
 *  complex to string
 */
#define complex_to_string(type, realtype)               \
  _string type##_to_string(type x) {                    \
    if (isnan(x.re) || isnan(x.im)) {                   \
      return NANSTRING;                                 \
    } else {                                            \
      const char* re = realtype##_to_string(x.re);      \
      const char* im;                                   \
      realtype imval = x.im;                            \
      const char* op = " + ";                           \
      if (imval < 0) {                                  \
        imval = -x.im;                                  \
        op = " - ";                                     \
      }                                                 \
      im = realtype##_to_string(imval);                 \
      if (strcmp(im, "-0.0") == 0) {                    \
        /* is there a better test for -0.0? */          \
        op = " - ";                                     \
        im = "0.0";                                     \
      }                                                 \
      return _glom_strings(4, re, op, im, "i");         \
    }                                                   \
  }

complex_to_string(_complex64, _real32);
complex_to_string(_complex128, _real64);
complex_to_string(_complex256, _real128);
