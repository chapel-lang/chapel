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

_int8 _string_to_int8_precise(const char* str, int* valid) {
  char* endPtr;
  _int8 val = (_int8)strtol(str, &endPtr, 10);
  *valid = (*endPtr == '\0');
  return val;
}

_int16 _string_to_int16_precise(const char* str, int* valid) {
  char* endPtr;
  _int16 val = (_int16)strtol(str, &endPtr, 10);
  *valid = (*endPtr == '\0');
  return val;
}

_int32 _string_to_int32_precise(const char* str, int* valid) {
  char* endPtr;
  _int32 val = (_int32)strtol(str, &endPtr, 10);
  *valid = (*endPtr == '\0');
  return val;
}

_int64 _string_to_int64_precise(const char* str, int* valid) {
  _int64 val;
  int numitems = sscanf(str, "%lld", &val);
  *valid = (numitems == 1);  // BLC: a poor test; could be extra chars at end
  return val;
}

_int8 _string_to_int8(const char* str) {
  int valid;
  _int8 val;
  val = _string_to_int8_precise(str, &valid);
  if (!valid) {
    _printError("Extra characters found when converting string to int(8)", 0, 0);
  }
  return val;
}

_int16 _string_to_int16(const char* str) {
  int valid;
  _int16 val;
  val = _string_to_int16_precise(str, &valid);
  if (!valid) {
    _printError("Extra characters found when converting string to int(16)", 0, 0);
  }
  return val;
}

_int32 _string_to_int32(const char* str) {
  int valid;
  _int32 val;
  val = _string_to_int32_precise(str, &valid);
  if (!valid) {
    _printError("Extra characters found when converting string to int(32)", 0, 0);
  }
  return val;
}

_int64 _string_to_int64(const char* str) {
  int valid;
  _int64 val;
  val = _string_to_int64_precise(str, &valid);
  if (!valid) {
    _printError("Extra characters found when converting string to int(64)", 0, 0);
  }
  return val;
}

_uint8 _string_to_uint8_precise(const char* str, int* valid) {
  char* endPtr;
  _uint8 val = (_uint8)strtol(str, &endPtr, 10);
  *valid = (*endPtr == '\0');
  return val;
}

_uint16 _string_to_uint16_precise(const char* str, int* valid) {
  char* endPtr;
  _uint16 val = (_uint16)strtol(str, &endPtr, 10);
  *valid = (*endPtr == '\0');
  return val;
}

_uint32 _string_to_uint32_precise(const char* str, int* valid) {
  char* endPtr;
  _uint32 val = (_uint32)strtol(str, &endPtr, 10);
  *valid = (*endPtr == '\0');
  return val;
}

_uint64 _string_to_uint64_precise(const char* str, int* valid) {
  _uint64 val;
  int numitems = sscanf(str, "%llu", &val);
  *valid = (numitems == 1);  // BLC: a poor test; could be extra chars at end
  return val;
}


_uint8 _string_to_uint8(const char* str) {
  int valid;
  _uint8 val;
  val = _string_to_uint8_precise(str, &valid);
  if (!valid) {
    _printError("Extra characters found when converting string to uint(8)", 0, 0);
  }
  return val;
}

_uint16 _string_to_uint16(const char* str) {
  int valid;
  _uint16 val;
  val = _string_to_uint16_precise(str, &valid);
  if (!valid) {
    _printError("Extra characters found when converting string to uint(16)", 0, 0);
  }
  return val;
}

_uint32 _string_to_uint32(const char* str) {
  int valid;
  _uint32 val;
  val = _string_to_uint32_precise(str, &valid);
  if (!valid) {
    _printError("Extra characters found when converting string to uint(32)", 0, 0);
  }
  return val;
}

_uint64 _string_to_uint64(const char* str) {
  int valid;
  _uint64 val;
  val = _string_to_uint64_precise(str, &valid);
  if (!valid) {
    _printError("Extra characters found when converting string to uint(64)", 0, 0);
  }
  return val;
}


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
