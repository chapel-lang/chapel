/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "chplrt.h"

#include "chplcast.h"
#include "chpltypes.h"
#include "chplfp.h"
#include "error.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

static int scanningNCounts(void) {
  static int answer = -1;
  if (answer == -1) {
    int result;
    int position;
    int numitems = sscanf("10", "%d%n", &result, &position);
    if (numitems == 1) {
      answer = 0;
    } else if (numitems == 2) {
      answer = 1;
    } else {
      chpl_internal_error("Misassumption in scanningNCounts()");
    }
  }
  return answer;
}

static int illegalFirstUnsChar(char c) {
  return ((c < '0' || c > '9') && (c != '+'));
}

/* Need to use this helper macro for PGI where doing otherwise causes
   spaces to disappear between the type name and the subsequent
   tokens */

#define _type(base, width) base##width##_t

#define _define_string_to_int_precise(base, width, uns)                 \
  _type(base,width) c_string_to_##base##width##_t_precise(c_string str, \
                                                          int* invalid,    \
                                                          char* invalidCh) { \
    char* endPtr;                                                       \
    _type(base, width) val = (_type(base, width))strtol(str, &endPtr, 10);  \
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
  _type(base, width)  c_string_to_##base##width##_t_precise(c_string str, \
                                                            int* invalid,  \
                                                            char* invalidCh) { \
    _type(base, width)  val;                                            \
    int numbytes;                                                       \
    int numitems = sscanf(str, format"%n", &val, &numbytes);            \
    if (scanningNCounts() && numitems == 2) {                           \
      numitems = 1;                                                     \
    }                                                                   \
    if (numitems == 1) {                                                \
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


_define_string_to_int_precise(int, 8, 0)
_define_string_to_int_precise(int, 16, 0)
_define_string_to_int_precise(int, 32, 0)
_define_string_to_bigint_precise(int, 64, 0, "%" SCNd64)
_define_string_to_int_precise(uint, 8, 1)
_define_string_to_int_precise(uint, 16, 1)
_define_string_to_int_precise(uint, 32, 1)
_define_string_to_bigint_precise(uint, 64, 1, "%" SCNu64)


chpl_bool c_string_to_chpl_bool(c_string str, int lineno, c_string filename) {
  if (string_compare(str, "true") == 0) {
    return true;
  } else if (string_compare(str, "false") == 0) {
    return false;
  } else {
    const char* message = 
      chpl_glom_strings(3, "Unexpected value when converting from string to bool: '",
                        str, "'");
    chpl_error(message, lineno, filename);
    return false;
  }
}


#define _real_type(base, width) _##base##width

#define _define_string_to_float_precise(base, width, format)            \
  _real_type(base, width) c_string_to_##base##width##_precise(c_string str, \
                                                              int* invalid,   \
                                                              char* invalidCh) { \
    _real_type(base, width) val;                                        \
    int numbytes;                                                       \
    int numitems = sscanf(str, format"%n", &val, &numbytes);            \
    if (scanningNCounts() && numitems == 2) {                           \
      numitems = 1;                                                     \
    }                                                                   \
    if (numitems == 1) {                                                \
      if (numbytes == strlen(str)) {                                    \
        /* for negatives, sscanf works, but we wouldn't want chapel to */ \
        *invalid = 0;                                                   \
        *invalidCh = '\0';                                              \
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

_define_string_to_float_precise(real, 32, "%f")
_define_string_to_float_precise(real, 64, "%lf")

#define _define_string_to_imag_precise(base, width, format)             \
  _real_type(base, width) c_string_to_##base##width##_precise(c_string str, \
                                                              int* invalid,   \
                                                              char* invalidCh) { \
    _real_type(base, width) val;                                        \
    int numbytes;                                                       \
    char i = '\0';                                                      \
    int numitems = sscanf(str, format"%c%n", &val, &i, &numbytes);      \
    if (scanningNCounts() && numitems == 3) {                           \
      numitems = 2;                                                     \
    }                                                                   \
    if (numitems == 1) {                                                \
      /* missing terminating i */                                       \
      *invalid = 2;                                                     \
      *invalidCh = i;                                                   \
    } else if (numitems == 2) {                                         \
      if (i != 'i') {                                                   \
        *invalid = 2;                                                   \
        *invalidCh = i;                                                 \
      } else if (numbytes == strlen(str)) {                             \
        *invalid = 0;                                                   \
        *invalidCh = '\0';                                              \
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


_define_string_to_imag_precise(imag, 32, "%f")
_define_string_to_imag_precise(imag, 64, "%lf")



#define _define_string_to_complex_precise(base, width, format, halfwidth) \
  _real_type(base, width) c_string_to_##base##width##_precise(c_string str, \
                                                              int* invalid,   \
                                                              char* invalidCh) { \
    _real_type(base, width) val = {0.0, 0.0};                           \
    /* check for pure imaginary case first */                           \
    val.im = c_string_to_imag##halfwidth##_precise(str, invalid, invalidCh); \
    if (*invalid) {                                                     \
      int numbytes = -1;                                                \
      char sign = '\0';                                                 \
      char i = '\0';                                                    \
      int numitems;                                                     \
      int posAfterReal, posBeforeSign, posBeforeImag;                   \
      val.im = 0.0; /* reset */                                         \
      numitems = sscanf(str, format"%n %n%c %n"format"%c%n",            \
                        &(val.re), &posAfterReal, &posBeforeSign,       \
                        &sign, &posBeforeImag, &(val.im), &i,           \
                        &numbytes);                                     \
      if (scanningNCounts()) {                                          \
        if (numitems == 3) {                                            \
          numitems = 1;                                                 \
        } else if (numitems == 5) {                                     \
          numitems = 2;                                                 \
        } else if (numitems == 6) {                                     \
          numitems = 3;                                                 \
        } else if (numitems == 8) {                                     \
          numitems = 4;                                                 \
        } else if (numitems != 0) {                                     \
          chpl_internal_error("Unexpected case in define_string_to_complex_precise"); \
        }                                                               \
      }                                                                 \
      if (numitems == 1) {                                              \
        *invalid = 0;                                                   \
        *invalidCh = '\0';                                              \
      } else if (numitems == 2) {                                       \
        *invalid = 1;                                                   \
        if (sign == 'i' && posAfterReal == posBeforeSign) {             \
          *invalidCh = *(str+posBeforeImag);                            \
        } else {                                                        \
          *invalidCh = sign;                                            \
        }                                                               \
      } else if (numitems == 3) {                                       \
        *invalid = 2;                                                   \
        *invalidCh = i;                                                 \
      } else if (numitems == 4) {                                       \
        if (sign != '-' && sign != '+') {                               \
          *invalid = 1;                                                 \
          *invalidCh = sign;                                            \
        } else if (i != 'i') {                                          \
          *invalid = 1;                                                 \
          *invalidCh = i;                                               \
        } else if (numbytes == strlen(str)) {                           \
          if (sign == '-') {                                            \
            val.im = -val.im;                                           \
          }                                                             \
          *invalid = 0;                                                 \
          *invalidCh = '\0';                                            \
        } else {                                                        \
          *invalid = 1;                                                 \
          *invalidCh = *(str+numbytes);                                 \
        }                                                               \
      } else {                                                          \
        *invalid = 1;                                                   \
        *invalidCh = *str;                                              \
      }                                                                 \
    }                                                                   \
    return val;                                                         \
  }

_define_string_to_complex_precise(complex, 64, "%f", 32)
_define_string_to_complex_precise(complex, 128, "%lf", 64)




#define _define_string_to_type(base, width)                             \
  _type(base, width) c_string_to_##base##width##_t(c_string str, int lineno, \
                                                   c_string filename) {   \
    int invalid;                                                        \
    char invalidStr[2] = "\0\0";                                        \
    _type(base, width) val = 0;                                         \
    if (!str) {                                                         \
      invalid = 1;                                                      \
    } else {                                                            \
      val = c_string_to_##base##width##_t_precise(str,                  \
                                                  &invalid,             \
                                                  invalidStr);          \
    }                                                                   \
                                                                        \
    if (invalid) {                                                      \
      const char* message;                                              \
      if (invalid == 2) {                                               \
        if (invalidStr[0] == '\0') {                                    \
          message = "Missing terminating 'i' character when converting from string to " #base "(" #width ")"; \
        } else {                                                        \
          message = chpl_glom_strings(3, "Missing terminating 'i' character when converting from string to " #base "(" #width "); got '", invalidStr, "' instead"); \
        }                                                               \
      } else if (invalidStr[0]) {                                       \
        message = chpl_glom_strings(3, "Unexpected character when converting from string to " #base "(" #width "): '", invalidStr, "'"); \
      } else {                                                          \
        message = "Empty string when converting from string to " #base "(" #width ")"; \
      }                                                                 \
      chpl_error(message, lineno, filename);                           \
    }                                                                   \
    return val;                                                         \
  }

_define_string_to_type(int, 8)
_define_string_to_type(int, 16)
_define_string_to_type(int, 32)
_define_string_to_type(int, 64)
_define_string_to_type(uint, 8)
_define_string_to_type(uint, 16)
_define_string_to_type(uint, 32)
_define_string_to_type(uint, 64)

#define _define_string_to_real_type(base, width)                        \
  _real_type(base, width) c_string_to_##base##width(c_string str, int lineno, \
                                                    c_string filename) {   \
    int invalid;                                                        \
    char invalidStr[2] = "\0\0";                                        \
    _real_type(base, width) val = c_string_to_##base##width##_precise(str,    \
                                                                         &invalid, \
                                                                         invalidStr); \
    if (invalid) {                                                      \
      const char* message;                                              \
      if (invalid == 2) {                                               \
        if (invalidStr[0] == '\0') {                                    \
          message = "Missing terminating 'i' character when converting from string to " #base "(" #width ")"; \
        } else {                                                        \
          message = chpl_glom_strings(3, "Missing terminating 'i' character when converting from string to " #base "(" #width "); got '", invalidStr, "' instead"); \
        }                                                               \
      } else if (invalidStr[0]) {                                       \
        message = chpl_glom_strings(3, "Unexpected character when converting from string to " #base "(" #width "): '", invalidStr, "'"); \
      } else {                                                          \
        message = "Empty string when converting from string to " #base "(" #width ")"; \
      }                                                                 \
      chpl_error(message, lineno, filename);                            \
    }                                                                   \
    return val;                                                         \
  }

_define_string_to_real_type(real, 32)
_define_string_to_real_type(real, 64)
_define_string_to_real_type(imag, 32)
_define_string_to_real_type(imag, 64)
_define_string_to_real_type(complex, 64)
_define_string_to_real_type(complex, 128)


c_string_copy
integral_to_c_string_copy(int64_t x, uint32_t size, chpl_bool isSigned)
{
  char buffer[256];
  const char* format = "";
  enum {UNSIGNED = 0<<16, SIGNED = 1<<16 };
  switch (SIGNED * isSigned + size)
  {
   default:
    chpl_error("Unexpected case in integral_to_c_string_copy", -1, "");
    break;

   case UNSIGNED + 1: format = "%" PRIu8;  break;
   case UNSIGNED + 2: format = "%" PRIu16; break;
   case UNSIGNED + 4: format = "%" PRIu32; break;
   case UNSIGNED + 8: format = "%" PRIu64; break;
   case   SIGNED + 1: format = "%" PRId8;  break;
   case   SIGNED + 2: format = "%" PRId16; break;
   case   SIGNED + 4: format = "%" PRId32; break;
   case   SIGNED + 8: format = "%" PRId64; break;
  }
  sprintf(buffer, format, x);
  return string_copy(buffer, 0, NULL);
}

/*
 *  real and imag to string
 */
static char* ensureDecimal(char* buffer) {
  char* last = buffer + strlen(buffer);
  if (!strchr(buffer, '.') && !strchr(buffer, 'e')) {
    strcat(buffer, ".0");
    last += 2;
  }
  return last;
}

#define NANSTRING "nan"
#define NEGINFSTRING "-inf"
#define POSINFSTRING "inf"

// Note: This function is thread-safe, since the stack-allocated buffer is
// private to the thread, and each returned c_string_copy is a unique object.
c_string_copy
real_to_c_string_copy(_real64 x, chpl_bool isImag)
{
  if (isnan(x)) {
    return string_copy(NANSTRING, 0, NULL);
  } else if (isinf(x)) {
    if (x < 0) {
      return string_copy(NEGINFSTRING, 0, NULL);
    } else {
      return string_copy(POSINFSTRING, 0, NULL);
    }
  } else {
    char buffer[256];
    char* last;

    sprintf(buffer, "%lg", x);
    last = ensureDecimal(buffer);
    if (isImag)
      strcat(last, "i");
    return string_copy(buffer, 0, NULL);
  }
}
