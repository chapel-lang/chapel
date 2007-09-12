#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "chplcast.h"
#include "chplrt.h"
#include "chpltypes.h"
#include "chplfp.h"
#include "error.h"

static int illegalFirstUnsChar(char c) {
  return ((c <= '0' || c >= '9') && (c != '+'));
}

#define _define_string_to_int_precise(base, width, uns)                 \
  _##base##width _string_to_##base##width##_precise(const char* str,    \
                                                    int* invalid,       \
                                                    char* invalidCh) {  \
    char* endPtr;                                                       \
    _##base##width val = (_##base##width)strtol(str, &endPtr, 10);      \
    *invalid = (*str == '\0' || *endPtr != '\0');                       \
    *invalidCh = *endPtr;                                               \
    /* for negatives, strtol works, but we wouldn't want chapel to */   \
    if (*invalid == 0 && uns && illegalFirstUnsChar(*str)) {            \
      *invalid = 1;                                                     \
      *invalidCh = *str;                                                \
    }                                                                   \
    return val;                                                         \
  }

#define _define_string_to_bigint_precise(base, width, uns, format)      \
  _##base##width _string_to_##base##width##_precise(const char* str,    \
                                                    int* invalid,       \
                                                    char* invalidCh) {  \
    _##base##width val;                                                 \
    int numbytes;                                                       \
    int numitems = sscanf(str, format"%n", &val, &numbytes);            \
    /* this numitems check is vague since implementations vary about    \
       whether or not to count %n as a match. */                        \
    if (numitems == 1 || numitems == 2) {                               \
      if (numbytes == strlen(str)) {                                    \
        /* for negatives, sscanf works, but we wouldn't want chapel to */ \
        if (uns && illegalFirstUnsChar(*str)) {                         \
          *invalid = 1;                                                 \
          *invalidCh = *str;                                            \
        } else {                                                        \
          *invalid = 0;                                                 \
          *invalidCh = '\0';                                            \
        }                                                               \
      } else {                                                          \
        *invalid = 1;                                                   \
        *invalidCh = *(str+numbytes);                                   \
      }                                                                 \
    } else {                                                            \
      *invalid = 1;                                                     \
      *invalidCh = *str;                                                \
    }                                                                   \
    return val;                                                         \
  }

#define _define_string_to_int(base, width)                              \
  _##base##width _string_to_##base##width(const char* str, int lineno,  \
                                          const char* filename) {       \
    int invalid;                                                        \
    char invalidStr[2] = "\0\0";                                        \
    _##base##width val = _string_to_##base##width##_precise(str,        \
                                                            &invalid,   \
                                                            invalidStr); \
    if (invalid) {                                                      \
      const char* message;                                              \
      if (invalidStr[0]) {                                              \
        message = _glom_strings(3, "Illegal character when converting from string to " #base "(" #width "): '", invalidStr, "'"); \
      } else {                                                          \
        message = "Empty string when converting from string to " #base "(" #width ")"; \
      }                                                                 \
      _printError(message, lineno, filename);                           \
    }                                                                   \
    return val;                                                         \
  }

_define_string_to_int_precise(int, 8, 0)
_define_string_to_int_precise(int, 16, 0)
_define_string_to_int_precise(int, 32, 0)
_define_string_to_bigint_precise(int, 64, 0, "%lld")
_define_string_to_int_precise(uint, 8, 1)
_define_string_to_int_precise(uint, 16, 1)
_define_string_to_int_precise(uint, 32, 1)
_define_string_to_bigint_precise(uint, 64, 1, "%llu")

_define_string_to_int(int, 8)
_define_string_to_int(int, 16)
_define_string_to_int(int, 32)
_define_string_to_int(int, 64)
_define_string_to_int(uint, 8)
_define_string_to_int(uint, 16)
_define_string_to_int(uint, 32)
_define_string_to_int(uint, 64)


/*
 *  string to complex
 */
_string _string_get_imag_part(_string s) {
  const char* ps = s;
  while (*ps == ' ') // eat space
    ps++;
  if (*ps == '-' || *ps == '+') // eat sign
    ps++;
  if (!isdigit((int)*ps)) // test for digit
    return "";
  while (isdigit((int)*ps)) // eat integral part
    ps++;
  if (*ps == '.') { // test for decimal
    ps++;
    if (!isdigit((int)*ps)) // test for digit
      return "";
    while (isdigit((int)*ps)) // eat fractional part
      ps++;
  }
  if (*ps == 'e' || *ps == 'E') { // test for exponent
    ps++;
    if (*ps == '+' || *ps == '-') // test for exponent sign
      ps++;
    if (!isdigit((int)*ps)) // test for digit
      return "";
    while (isdigit((int)*ps)) // eat exponent part
      ps++;
  }
  while (*ps == ' ') // eat space
    ps++;
  if (*ps == '+' || *ps == '-') // check for imaginary part
    return ++ps;
  return "";
}

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
