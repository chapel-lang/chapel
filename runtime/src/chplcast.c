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
