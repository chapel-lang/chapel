/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#include <cstring>
#include <cstdio>
#include "chplmath.h"
#include "num.h"
#include "prim_data.h"
#include "stringutil.h"

static int
snprint_float_val(char* buf, size_t max, double val, bool hex) {
  if (chpl_isfinite(val)) {
    int nc = 0;
    if (chpl_signbit(val)) nc = snprintf(buf, max, "-%g" , -val);
    else                   nc = snprintf(buf, max, "%g" , val);

    if (strchr(buf, '.') == NULL &&
        strchr(buf, 'e') == NULL &&
        strchr(buf, 'p') == NULL) {
      strncat(buf, ".0", max-nc);
      return nc + 2;
    } else {
      return nc;
    }
  } else if (chpl_isinf(val)) {
    if (chpl_signbit(val)) strncpy(buf, "-INFINITY", max);
    else                   strncpy(buf, "INFINITY", max);
    return strlen(buf);
  } else {
    if (chpl_signbit(val)) strncpy(buf, "-NAN", max);
    else                   strncpy(buf, "NAN", max);
    return strlen(buf);
  }
}

static int
snprint_imag_val(char* str, size_t max, double val, bool hex) {
  int numchars = snprint_float_val(str, max, val, hex);
  strncat(str, "i", max-numchars);
  return numchars + 1;
}


static int
snprint_complex_val(char* str, size_t max, double real, double imm) {
  int numchars = 0;
  numchars += snprint_float_val(str+numchars, max-numchars, real, false);
  if (chpl_signbit(imm)) {
    numchars += snprintf(str+numchars, max-numchars, " - ");
    numchars += snprint_float_val(str+numchars, max-numchars, -imm, false);
  } else {
    numchars += snprintf(str+numchars, max-numchars, " + ");
    numchars += snprint_float_val(str+numchars, max-numchars, imm, false);
  }
  numchars += snprintf(str+numchars, max-numchars, "i");
  return numchars;
}

int
snprint_imm(char *str, size_t max, const Immediate &imm) {
  int res = -1;
  switch (imm.const_kind) {
    case NUM_KIND_NONE:
      break;
    case NUM_KIND_BOOL: {
      // Since I'm using uints to store the bools, should cast to the
      // same to be consistent.
      res = snprintf(str, max, "%u", (unsigned)imm.bool_value()); break;
    }
    case NUM_KIND_UINT: {
      switch (imm.num_index) {
        case INT_SIZE_8:
          res = snprintf(str, max, "%u", (unsigned)imm.v_uint8); break;
        case INT_SIZE_16:
          res = snprintf(str, max, "%u", (unsigned)imm.v_uint16); break;
        case INT_SIZE_32:
          res = snprintf(str, max, "%u", (unsigned)imm.v_uint32); break;
        case INT_SIZE_64:
          res = snprintf(str, max, "%" PRIu64, imm.v_uint64); break;
        default: INT_FATAL("Unhandled case in switch statement");
      }
      break;
    }
    case NUM_KIND_INT: {
      switch (imm.num_index) {
        case INT_SIZE_8:
          res = snprintf(str, max, "%d", imm.v_int8); break;
        case INT_SIZE_16:
          res = snprintf(str, max, "%d", imm.v_int16); break;
        case INT_SIZE_32:
          res = snprintf(str, max, "%" PRId32, imm.v_int32); break;
        case INT_SIZE_64:
          res = snprintf(str, max, "%" PRId64, imm.v_int64); break;
        default: INT_FATAL("Unhandled case in switch statement");
      }
      break;
    }
    case NUM_KIND_REAL: case NUM_KIND_IMAG:
      switch (imm.num_index) {
        case FLOAT_SIZE_32:
          res = snprint_float_val(str, max, imm.v_float32, true); break;
        case FLOAT_SIZE_64:
          res = snprint_float_val(str, max, imm.v_float64, true); break;
        default: INT_FATAL("Unhandled case in switch statement");
      }
      break;
    case NUM_KIND_COMPLEX:
      switch (imm.num_index) {
        case COMPLEX_SIZE_64:
          res = snprint_complex_val(str, max,
                                    imm.v_complex64.r, imm.v_complex64.i);
          break;
        case COMPLEX_SIZE_128:
          res = snprint_complex_val(str, max,
                                    imm.v_complex128.r, imm.v_complex128.i);
          break;
        default: INT_FATAL("Unhandled case in switch statement");
      }
      break;
    case CONST_KIND_STRING: {
      char const * fmt = NULL;
      if (imm.string_kind == STRING_KIND_C_STRING)
        fmt = "c\"%s\"";
      else if (imm.string_kind == STRING_KIND_BYTES)
        fmt = "b\"%s\"";
      else
        fmt = "\"%s\"";
      res = snprintf(str, max, fmt, imm.v_string);
      break;
    }
  }
  return res;
}

int
fprint_imm(FILE *fp, const Immediate &imm, bool showType) {
  int res = -1;
  switch (imm.const_kind) {
    case NUM_KIND_NONE:
      if (showType) res = fputs(":NONE", fp);
      break;
    case NUM_KIND_BOOL: {
      // Since I'm using uints to store the bools, should cast to the
      // same to be consistent.
      res = fprintf(fp, "%" PRIu64, imm.bool_value());
      if (showType) res += fputs(" :bool", fp);
      break;
    }
    case NUM_KIND_UINT: {
      switch (imm.num_index) {
        case INT_SIZE_8:
          res = fprintf(fp, "%u", (unsigned)imm.v_uint8);
          if (showType) res += fputs(" :uint(8)", fp);
          break;
        case INT_SIZE_16:
          res = fprintf(fp, "%u", (unsigned)imm.v_uint16);
          if (showType) res += fputs(" :uint(16)", fp);
          break;
        case INT_SIZE_32:
          res = fprintf(fp, "%u", (unsigned)imm.v_uint32);
          if (showType) res += fputs(" :uint(32)", fp);
          break;
        case INT_SIZE_64:
          res = fprintf(fp, "%" PRIu64, imm.v_uint64);
          if (showType) res += fputs(" :uint(64)", fp);
          break;
        default: INT_FATAL("Unhandled case in switch statement");
      }
      break;
    }
    case NUM_KIND_INT: {
      switch (imm.num_index) {
        case INT_SIZE_8:
          res = fprintf(fp, "%d", imm.v_int8);
          if (showType) res += fputs(" :int(8)", fp);
          break;
        case INT_SIZE_16:
          res = fprintf(fp, "%d", imm.v_int16);
          if (showType) res += fputs(" :int(16)", fp);
          break;
        case INT_SIZE_32:
          res = fprintf(fp, "%" PRId32, imm.v_int32);
          if (showType) res += fputs(" :int(32)", fp);
          break;
        case INT_SIZE_64:
          res = fprintf(fp, "%" PRId64, imm.v_int64);
          if (showType) res += fputs(" :int(64)", fp);
          break;
        default: INT_FATAL("Unhandled case in switch statement");
      }
      break;
    }
    case NUM_KIND_REAL: case NUM_KIND_IMAG: {
      char str[80];
      const char* size = NULL;
      switch (imm.num_index) {
        case FLOAT_SIZE_32:
          res = snprint_float_val(str, sizeof(str), imm.v_float32, false);
          size = "(32)";
          break;
        case FLOAT_SIZE_64: {
          res = snprint_float_val(str, sizeof(str), imm.v_float64, false);
          size = "(64)";
          break;
        }
        default: INT_FATAL("Unhandled case in switch statement");
      }
      fputs(str, fp);
      if (showType) {
        res += fputs(imm.const_kind == NUM_KIND_IMAG?" :imag":" :real", fp);
        res += fputs(size, fp);
      }
      break;
    }
    case NUM_KIND_COMPLEX:
      switch (imm.num_index) {
        case COMPLEX_SIZE_64: {
          char str[160];
          res = snprint_complex_val(str, sizeof(str),
                                    imm.v_complex64.r, imm.v_complex64.i);
          fputs(str, fp);
          if (showType) res += fputs(" :complex(64)", fp);
          break;
        }
        case COMPLEX_SIZE_128: {
          char str[160];
          res = snprint_complex_val(str, sizeof(str),
                                    imm.v_complex128.r, imm.v_complex128.i);
          fputs(str, fp);
          if (showType) res += fputs(" :complex(128)", fp);
          break;
        }
        default: INT_FATAL("Unhandled case in switch statement");
      }
      break;
    case CONST_KIND_STRING: {
      char const * fmt = NULL;
      if (imm.string_kind == STRING_KIND_C_STRING)
        fmt = "c\"%s\"";
      else if (imm.string_kind == STRING_KIND_BYTES)
        fmt = "b\"%s\"";
      else
        fmt = "\"%s\"";
      res = fprintf(fp, fmt, imm.v_string);
      // obvious, skip: if (showType) res += fputs(" :string", fp);
      break;
    }
    default: INT_FATAL("Unhandled case in switch statement");
  }
  return res;
}

void
coerce_immediate(Immediate *from, Immediate *to) {
#include "cast_code.cpp"
}

#define DO_FOLD(_op,add,sub) \
      switch (imm->const_kind) { \
        case NUM_KIND_NONE: \
          break; \
        case NUM_KIND_BOOL: { \
          imm->v_bool = im1.bool_value() _op im2.bool_value(); break; \
        } \
        case NUM_KIND_UINT: { \
          switch (imm->num_index) { \
            case INT_SIZE_8:  \
              imm->v_uint8 = im1.v_uint8 _op im2.v_uint8; break; \
            case INT_SIZE_16: \
              imm->v_uint16 = im1.v_uint16 _op im2.v_uint16; break; \
            case INT_SIZE_32: \
              imm->v_uint32 = im1.v_uint32 _op im2.v_uint32; break; \
            case INT_SIZE_64: \
              imm->v_uint64 = im1.v_uint64 _op im2.v_uint64; break; \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        } \
        case NUM_KIND_INT: { \
          switch (imm->num_index) { \
            case INT_SIZE_8:  \
              imm->v_int8 = im1.v_int8 _op im2.v_int8; break; \
            case INT_SIZE_16: \
              imm->v_int16 = im1.v_int16 _op im2.v_int16; break; \
            case INT_SIZE_32: \
              imm->v_int32 = im1.v_int32 _op im2.v_int32; break; \
          case INT_SIZE_64: {                                            \
              imm->v_int64 = im1.v_int64 _op im2.v_int64; \
              break; \
          } \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        } \
        case NUM_KIND_REAL: case NUM_KIND_IMAG: \
          switch (imm->num_index) { \
            case FLOAT_SIZE_32: \
              if (add && im1.v_float32 == 0.0) \
                imm->v_float32 = im2.v_float32; \
              else if (sub && im1.v_float32 == 0.0) \
                imm->v_float32 = -im2.v_float32; \
              else \
                imm->v_float32 = im1.v_float32 _op im2.v_float32; \
              break; \
            case FLOAT_SIZE_64: \
              if (add && im1.v_float64 == 0.0) \
                imm->v_float64 = im2.v_float64; \
              else if (sub && im1.v_float64 == 0.0) \
                imm->v_float64 = -im2.v_float64; \
              else \
                imm->v_float64 = im1.v_float64 _op im2.v_float64; \
              break; \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        case NUM_KIND_COMPLEX: \
          switch (imm->num_index) { \
            case COMPLEX_SIZE_64: \
              if (add && im1.v_complex64.r == 0.0) \
                imm->v_complex64.r = im2.v_complex64.r; \
              else if (sub && im1.v_complex64.r == 0.0) \
                imm->v_complex64.r = -im2.v_complex64.r; \
              else \
                imm->v_complex64.r = im1.v_complex64.r _op im2.v_complex64.r; \
              if (add && im1.v_complex64.i == 0.0) \
                imm->v_complex64.i = im2.v_complex64.i; \
              else if (sub && im1.v_complex64.i == 0.0) \
                imm->v_complex64.i = -im2.v_complex64.i; \
              else \
                imm->v_complex64.i = im1.v_complex64.i _op im2.v_complex64.i; \
              break; \
            case COMPLEX_SIZE_128: \
              if (add && im1.v_complex128.r == 0.0) \
                imm->v_complex128.r = im2.v_complex128.r; \
              else if (sub && im1.v_complex128.r == 0.0) \
                imm->v_complex128.r = -im2.v_complex128.r; \
              else \
                imm->v_complex128.r = im1.v_complex128.r _op im2.v_complex128.r; \
              if (add && im1.v_complex128.i == 0.0) \
                imm->v_complex128.i = im2.v_complex128.i; \
              else if (sub && im1.v_complex128.i == 0.0) \
                imm->v_complex128.i = -im2.v_complex128.i; \
              else \
                imm->v_complex128.i = im1.v_complex128.i _op im2.v_complex128.i; \
              break; \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
      }

#define COMPUTE_INT_POW(type, b, e)                      \
  type base = b;                                         \
  type exp = e;                                          \
  type res = 1;                                          \
  if (base == 0 && exp < 0) {                            \
    USR_FATAL("0 cannot be raised to a negative power"); \
  } else if (exp < 0) {                                  \
    if (base == 1) {                                     \
      res = 1;                                           \
    } else if (base == -1) {                             \
      res = exp % 2 == 0 ? 1 : -1;                       \
    } else {                                             \
      res = 0;                                           \
    }                                                    \
  } else {                                               \
    type i = exp;                                        \
    type z = base;                                       \
    while (i != 0) {                                     \
      if (i % 2 == 1)                                    \
        res *= z;                                        \
      z *= z;                                            \
      i /= 2;                                            \
    }                                                    \
  }

#define COMPUTE_UINT_POW(type, b, e)                 \
  type base = b;                                     \
  type exp = e;                                      \
  type res = 1;                                      \
  type i = exp;                                      \
  type z = base;                                     \
  while (i != 0) {                                   \
    if (i % 2 == 1)                                  \
      res *= z;                                      \
    z *= z;                                          \
    i /= 2;                                          \
  }

#define DO_FOLDPOW() \
      switch (imm->const_kind) { \
        case NUM_KIND_NONE: \
          break; \
        case NUM_KIND_BOOL: { \
          COMPUTE_UINT_POW(uint64_t, im1.bool_value(), im2.bool_value()); \
          imm->v_bool = res;                                     \
          break;                                                  \
        } \
        case NUM_KIND_UINT: { \
          switch (imm->num_index) { \
            case INT_SIZE_8:  \
              {                                                         \
                COMPUTE_UINT_POW(uint8_t, im1.v_uint8, im2.v_uint8);       \
                imm->v_uint8 = res;                                     \
                break;                                                  \
              }                                                         \
            case INT_SIZE_16: \
              {                                                         \
                COMPUTE_UINT_POW(uint16_t, im1.v_uint16, im2.v_uint16);       \
                imm->v_uint16 = res;                                     \
                break;                                                  \
              }                                                         \
            case INT_SIZE_32: \
              {                                                         \
                COMPUTE_UINT_POW(uint32_t, im1.v_uint32, im2.v_uint32);       \
                imm->v_uint32 = res;                                     \
                break;                                                  \
              }                                                         \
            case INT_SIZE_64: \
              {                                                         \
                COMPUTE_UINT_POW(uint64_t, im1.v_uint64, im2.v_uint64); \
                imm->v_uint64 = res;                                     \
                break;                                                  \
              }                                                         \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        } \
        case NUM_KIND_INT: { \
          switch (imm->num_index) { \
            case INT_SIZE_8: \
              {                                                         \
                COMPUTE_INT_POW(int8_t, im1.v_int8, im2.v_int8);       \
                imm->v_int8 = res;                                     \
                break;                                                  \
              }                                                         \
            case INT_SIZE_16: \
              {                                                         \
                COMPUTE_INT_POW(int16_t, im1.v_int16, im2.v_int16);       \
                imm->v_int16 = res;                                     \
                break;                                                  \
              }                                                         \
            case INT_SIZE_32: \
              {                                                         \
                COMPUTE_INT_POW(int32_t, im1.v_int32, im2.v_int32);       \
                imm->v_int32 = res;                                     \
                break;                                                  \
              }                                                         \
            case INT_SIZE_64: \
              {                                                         \
                COMPUTE_INT_POW(int64_t, im1.v_int64, im2.v_int64); \
                imm->v_int64 = res;                                     \
                break;                                                  \
              }                                                         \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        } \
        case NUM_KIND_REAL: case NUM_KIND_IMAG: case NUM_KIND_COMPLEX: \
          INT_FATAL("Cannot fold ** on floating point values"); \
          break; \
      }

#define DO_FOLDB(_op,_complex_combine) \
      switch (im1.const_kind) { \
        case NUM_KIND_NONE: \
          break; \
        case NUM_KIND_BOOL: { \
          imm->v_bool = im1.bool_value() _op im2.bool_value(); break; \
        } \
        case NUM_KIND_UINT: { \
          switch (im1.num_index) { \
            case INT_SIZE_8:  \
              imm->v_bool = im1.v_uint8 _op im2.v_uint8; break; \
            case INT_SIZE_16: \
              imm->v_bool = im1.v_uint16 _op im2.v_uint16; break; \
            case INT_SIZE_32: \
              imm->v_bool = im1.v_uint32 _op im2.v_uint32; break; \
            case INT_SIZE_64: \
              imm->v_bool = im1.v_uint64 _op im2.v_uint64; break; \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        } \
        case NUM_KIND_INT: { \
          switch (im1.num_index) { \
            case INT_SIZE_8:  \
              imm->v_bool = im1.v_int8 _op im2.v_int8; break; \
            case INT_SIZE_16: \
              imm->v_bool = im1.v_int16 _op im2.v_int16; break; \
            case INT_SIZE_32: \
              imm->v_bool = im1.v_int32 _op im2.v_int32; break; \
            case INT_SIZE_64: \
              imm->v_bool = im1.v_int64 _op im2.v_int64; break; \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        } \
        case NUM_KIND_REAL: case NUM_KIND_IMAG: \
          switch (im1.num_index) { \
            case FLOAT_SIZE_32: \
              imm->v_bool = im1.v_float32 _op im2.v_float32; break; \
            case FLOAT_SIZE_64: \
              imm->v_bool = im1.v_float64 _op im2.v_float64; break; \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        case NUM_KIND_COMPLEX: \
          switch (im1.num_index) { \
            case COMPLEX_SIZE_64: \
              imm->v_bool = (im1.v_complex64.r _op im2.v_complex64.r) \
                            _complex_combine \
                            (im1.v_complex64.i _op im2.v_complex64.i); \
              break; \
            case COMPLEX_SIZE_128: \
              imm->v_bool = (im1.v_complex128.r _op im2.v_complex128.r) \
                            _complex_combine \
                            (im1.v_complex128.i _op im2.v_complex128.i); \
              break; \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
      }

#define DO_FOLDI(_op) \
      switch (imm->const_kind) { \
        case NUM_KIND_NONE: \
          break; \
        case NUM_KIND_BOOL: { \
          imm->v_bool = im1.bool_value() _op im2.bool_value(); break; \
        } \
        case NUM_KIND_UINT: { \
          switch (imm->num_index) { \
            case INT_SIZE_8:  \
              imm->v_uint8 = im1.v_uint8 _op im2.v_uint8; break; \
            case INT_SIZE_16: \
              imm->v_uint16 = im1.v_uint16 _op im2.v_uint16; break; \
            case INT_SIZE_32: \
              imm->v_uint32 = im1.v_uint32 _op im2.v_uint32; break; \
            case INT_SIZE_64: \
              imm->v_uint64 = im1.v_uint64 _op im2.v_uint64; break; \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        } \
        case NUM_KIND_INT: { \
          switch (imm->num_index) { \
            case INT_SIZE_8:  \
              imm->v_int8 = im1.v_int8 _op im2.v_int8; break; \
            case INT_SIZE_16: \
              imm->v_int16 = im1.v_int16 _op im2.v_int16; break; \
            case INT_SIZE_32: \
              imm->v_int32 = im1.v_int32 _op im2.v_int32; break; \
            case INT_SIZE_64: \
              imm->v_int64 = im1.v_int64 _op im2.v_int64; break; \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        } \
        case NUM_KIND_REAL: case NUM_KIND_IMAG: case NUM_KIND_COMPLEX: \
          INT_FATAL("Unhandled case in switch statement"); \
      }

#define DO_FOLD1(_op) \
      switch (imm->const_kind) { \
        case NUM_KIND_NONE: \
          break; \
        case NUM_KIND_BOOL: { \
          imm->v_bool = _op im1.bool_value(); break; \
        } \
        case NUM_KIND_UINT: { \
          switch (imm->num_index) { \
            case INT_SIZE_8:  \
              imm->v_uint8 = _op im1.v_uint8; break; \
            case INT_SIZE_16: \
              imm->v_uint16 = _op im1.v_uint16; break; \
            case INT_SIZE_32: \
              imm->v_uint32 = _op im1.v_uint32; break; \
            case INT_SIZE_64: \
              imm->v_uint64 = _op im1.v_uint64; break; \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        } \
        case NUM_KIND_INT: { \
          switch (imm->num_index) { \
            case INT_SIZE_8:  \
              imm->v_int8 = _op im1.v_int8; break; \
            case INT_SIZE_16: \
              imm->v_int16 = _op im1.v_int16; break; \
            case INT_SIZE_32: \
              imm->v_int32 = _op im1.v_int32; break; \
            case INT_SIZE_64: \
              imm->v_int64 = _op im1.v_int64; break;        \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        } \
        case NUM_KIND_REAL: case NUM_KIND_IMAG: \
          switch (imm->num_index) { \
            case FLOAT_SIZE_32: \
              imm->v_float32 = _op im1.v_float32; break; \
            case FLOAT_SIZE_64: \
              imm->v_float64 =  _op im1.v_float64; break; \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        case NUM_KIND_COMPLEX: \
          switch (imm->num_index) { \
            case COMPLEX_SIZE_64: \
              imm->v_complex64.r = _op im1.v_complex64.r; \
              imm->v_complex64.i = _op im1.v_complex64.i; \
              break; \
            case COMPLEX_SIZE_128: \
              imm->v_complex128.r = _op im1.v_complex128.r; \
              imm->v_complex128.i = _op im1.v_complex128.i; \
              break; \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
      }

#define DO_FOLD1I(_op) \
      switch (imm->const_kind) { \
        case NUM_KIND_NONE: \
          break; \
        case NUM_KIND_BOOL: { \
          imm->v_bool = _op im1.bool_value(); break; \
        } \
        case NUM_KIND_UINT: { \
          switch (imm->num_index) { \
            case INT_SIZE_8:  \
              imm->v_uint8 = _op im1.v_uint8; break; \
            case INT_SIZE_16: \
              imm->v_uint16 = _op im1.v_uint16; break; \
            case INT_SIZE_32: \
              imm->v_uint32 = _op im1.v_uint32; break; \
            case INT_SIZE_64: \
              imm->v_uint64 = _op im1.v_uint64; break; \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        } \
        case NUM_KIND_INT: { \
          switch (imm->num_index) { \
            case INT_SIZE_8:  \
              imm->v_int8 = _op im1.v_int8; break; \
            case INT_SIZE_16: \
              imm->v_int16 = _op im1.v_int16; break; \
            case INT_SIZE_32: \
              imm->v_int32 = _op im1.v_int32; break; \
            case INT_SIZE_64: \
              imm->v_int64 = _op im1.v_int64; break;        \
            default: INT_FATAL("Unhandled case in switch statement"); \
          } \
          break; \
        } \
        case NUM_KIND_REAL: case NUM_KIND_IMAG: case NUM_KIND_COMPLEX: \
          INT_FATAL("Unhandled case in switch statement"); \
          break; \
      }

static int
max(int a, int b) {
  if (a >= b)
    return a;
  else
    return b;
}

static int
num_kind_int_to_float(int num_index) {
  if (int_type_precision[num_index] <= 32)
    return FLOAT_SIZE_32;
  else
    return FLOAT_SIZE_64;
}

void
fold_result(Immediate *im1, Immediate *im2, Immediate *imm) {
  if (im1->const_kind == im2->const_kind) {
    if (im1->num_index > im2->num_index) {
      imm->const_kind = im1->const_kind;
      imm->num_index = im1->num_index;
      return;
    } else {
      imm->const_kind = im2->const_kind;
      imm->num_index = im2->num_index;
      return;
    }
  }

  // if non-complex and complex -> complex
  if (im1->const_kind == NUM_KIND_COMPLEX ||
      im2->const_kind == NUM_KIND_COMPLEX) {
    if (im2->const_kind == NUM_KIND_COMPLEX) {
      // swap im1 to the complex
      Immediate *t = im1;
      im1 = im2;
      im2 = t;
    }
    if (im2->const_kind == NUM_KIND_UINT ||
        im2->const_kind == NUM_KIND_INT) {
      int index2 = num_kind_int_to_float(im2->num_index);
      imm->num_index = max(im1->num_index, index2);
    } else {  // else, im2 must be float?
      INT_ASSERT(im2->const_kind == NUM_KIND_REAL ||
                 im2->const_kind == NUM_KIND_IMAG);
      imm->num_index = max(im1->num_index, im2->num_index);
    }
    imm->const_kind = NUM_KIND_COMPLEX;
    return;
  }

  // non-imag and imag -> complex
  // note if one was complex, it would have been handled in the above case.
  if (im1->const_kind == NUM_KIND_IMAG ||
      im2->const_kind == NUM_KIND_IMAG) {
    if (im2->const_kind == NUM_KIND_IMAG) {
      // swap im1 to the imag
      Immediate *t = im1;
      im1 = im2;
      im2 = t;
    }

    if (im2->const_kind == NUM_KIND_UINT ||
        im2->const_kind == NUM_KIND_INT) {
      int index2 = num_kind_int_to_float(im2->num_index);
      imm->num_index = max(im1->num_index, index2);
    } else {  // else, im2 must be float?
      imm->num_index = max(im1->num_index, im2->num_index);
    }
    imm->const_kind = NUM_KIND_COMPLEX;
    return;
  }

  if (im1->const_kind == NUM_KIND_REAL ||
      im2->const_kind == NUM_KIND_REAL) {

    if (im2->const_kind == NUM_KIND_REAL) {
      // swap im1 to the real
      Immediate *t = im2; im2 = im1; im1 = t;
    }

    if (im2->const_kind == NUM_KIND_UINT ||
        im2->const_kind == NUM_KIND_INT) {
      int index2 = num_kind_int_to_float(im2->num_index);
      imm->num_index = max(im1->num_index, index2);
    } else {  // else, im2 must be float?
      INT_ASSERT(im2->const_kind == NUM_KIND_REAL ||
                 im2->const_kind == NUM_KIND_IMAG);
      imm->num_index = max(im1->num_index, im2->num_index);
    }

    imm->const_kind = NUM_KIND_REAL;
    return;
  }

  // TODO -- check this function after this point!
  // probably problems with int_type_precision called on bool num_index

  if (im1->const_kind != NUM_KIND_BOOL && im2->const_kind != NUM_KIND_BOOL) {
    // mixed signed and unsigned
    if (im1->num_index >= INT_SIZE_64 || im2->num_index >= INT_SIZE_64) {
      imm->const_kind = NUM_KIND_INT;
      imm->num_index = INT_SIZE_64;
      return;
    } else if (im1->num_index >= INT_SIZE_32 || im2->num_index >= INT_SIZE_32) {
      imm->const_kind = NUM_KIND_INT;
      imm->num_index = INT_SIZE_32;
      return;
    } else if (im1->num_index >= INT_SIZE_16 || im2->num_index >= INT_SIZE_16) {
      imm->const_kind = NUM_KIND_INT;
      imm->num_index = INT_SIZE_16;
      return;
    } else {
      imm->const_kind = NUM_KIND_INT;
      imm->num_index = INT_SIZE_8;
      return;
    }
  }
  // At this point, we can assume we are mixing a bool and either a uint or an
  // int.
  if (im2->const_kind == NUM_KIND_BOOL) {
    // swap im1 to the bool to simplify the code
    Immediate *t = im1;
    im1 = im2;
    im2 = t;
  }
  // The const_kind will be either a uint or an int, depending on what the
  // non-bool argument is.  Keep the larger size
  if (im1->num_index >= BOOL_SIZE_64 || im2->num_index >= INT_SIZE_64) {
    imm->const_kind = im2->const_kind;
    imm->num_index = INT_SIZE_64;
    return;
  } else if (im1->num_index >= BOOL_SIZE_32 || im2->num_index >= INT_SIZE_32) {
    imm->const_kind = im2->const_kind;
    imm->num_index = INT_SIZE_32;
    return;
  } else if (im1->num_index >= BOOL_SIZE_16 || im2->num_index >= INT_SIZE_16) {
    imm->const_kind = im2->const_kind;
    imm->num_index = INT_SIZE_16;
    return;
  } else {
    imm->const_kind = im2->const_kind;
    imm->num_index = INT_SIZE_8;
    return;
  }
}

void
fold_constant(int op, Immediate *aim1, Immediate *aim2, Immediate *imm) {
  Immediate im1(*aim1), im2, coerce;
  if (aim2)
    im2 = *aim2;
  switch (op) {
    default: INT_FATAL("fold constant op not supported"); break;
    case P_prim_mult:
    case P_prim_div:
    case P_prim_mod:
    case P_prim_add:
    case P_prim_subtract:
    case P_prim_and:
    case P_prim_or:
    case P_prim_xor:
    case P_prim_pow:
      fold_result(&im1, &im2, &coerce);
      imm->const_kind = coerce.const_kind;
      imm->num_index = coerce.num_index;
      break;
    case P_prim_lsh:
    case P_prim_rsh:
      imm->const_kind = im1.const_kind;
      imm->num_index = im1.num_index;
      break;
    case P_prim_less:
    case P_prim_lessorequal:
    case P_prim_greater:
    case P_prim_greaterorequal:
    case P_prim_equal:
    case P_prim_notequal:
      fold_result(&im1, &im2, &coerce);
      imm->const_kind = NUM_KIND_BOOL;
      imm->num_index = BOOL_SIZE_SYS;
      break;
    case P_prim_land:
    case P_prim_lor:
    case P_prim_lnot:
      coerce.const_kind = imm->const_kind = NUM_KIND_BOOL;
      coerce.num_index = imm->num_index = BOOL_SIZE_SYS;
      break;
    case P_prim_plus:
    case P_prim_minus:
    case P_prim_not:
      imm->const_kind = im1.const_kind;
      imm->num_index = im1.num_index;
      break;
  }
  if (coerce.const_kind) {
    coerce_immediate(&im1, &coerce);
    im1 = coerce;
    if (aim2) {
      coerce_immediate(&im2, &coerce);
      im2 = coerce;
    }
  }
  switch (op) {
    default: INT_FATAL("fold constant op not supported"); break;
    case P_prim_mult: DO_FOLD(*, false, false); break;
    case P_prim_div: DO_FOLD(/, false, false); break;
    case P_prim_mod: DO_FOLDI(%); break;
    case P_prim_add: DO_FOLD(+, true, false); break;
    case P_prim_subtract: DO_FOLD(-, false, true); break;
    case P_prim_lsh: DO_FOLDI(<<); break;
    case P_prim_rsh: DO_FOLDI(>>); break;
    case P_prim_less: DO_FOLDB(<, &&); break;
    case P_prim_lessorequal: DO_FOLDB(<=, &&); break;
    case P_prim_greater: DO_FOLDB(>, &&); break;
    case P_prim_greaterorequal: DO_FOLDB(>=, &&); break;
    case P_prim_equal: DO_FOLDB(==, &&); break;
    case P_prim_notequal: DO_FOLDB(!=, ||); break;
    case P_prim_and: DO_FOLDI(&); break;
    case P_prim_xor: DO_FOLDI(^); break;
    case P_prim_or: DO_FOLDI(|); break;
    case P_prim_land: DO_FOLD(&&, false, false); break;
    case P_prim_lor: DO_FOLD(||, false, false); break;
    case P_prim_plus: DO_FOLD1(+); break;
    case P_prim_minus: DO_FOLD1(-); break;
    case P_prim_not: DO_FOLD1I(~); break;
    case P_prim_lnot: DO_FOLD1(!); break;
  case P_prim_pow: {
    DO_FOLDPOW();
    break;
  }
  }
}

void
convert_string_to_immediate(const char *str, Immediate *imm) {
  switch (imm->const_kind) {
    case NUM_KIND_NONE:
      break;
    case NUM_KIND_BOOL: {
      if (!strcmp(str, "false")) {
        imm->v_bool = false;
      } else if (!strcmp(str, "true")) {
        imm->v_bool = true;
      } else if (str[0] == '\0') {
        imm->v_bool = !imm->v_bool;
      } else {
        INT_FATAL("Bad bool value");
      }
      break;
    }
    case NUM_KIND_UINT: {
      switch (imm->num_index) {
        case INT_SIZE_8:
          if (str[0] != '\'')
            imm->v_uint8 = str2uint8(str);
          else {
            if (str[1] != '\\')
              imm->v_uint8 = str[1];
            else
              imm->v_uint8 = str[2];
          }
          break;
        case INT_SIZE_16:
          imm->v_uint16 = str2uint16(str); break;
        case INT_SIZE_32:
          imm->v_uint32 = str2uint32(str); break;
        case INT_SIZE_64:
          imm->v_uint64 = str2uint64(str); break;
        default: INT_FATAL("Unhandled case in switch statement");
      }
      break;
    }
    case NUM_KIND_INT: {
      switch (imm->num_index) {
        case INT_SIZE_8:
          if (str[0] != '\'')
            imm->v_int8 = str2int8(str);
          else {
            if (str[1] != '\\')
              imm->v_int8 = str[1];
            else
              imm->v_int8 = str[2];
          }
          break;
        case INT_SIZE_16:
          imm->v_int16 = str2int16(str); break;
        case INT_SIZE_32:
          imm->v_int32 = str2int32(str); break;
        case INT_SIZE_64:
          imm->v_int64 = str2int64(str); break;
        default: INT_FATAL("Unhandled case in switch statement");
      }
      break;
    }
    case NUM_KIND_REAL: case NUM_KIND_IMAG:
      switch (imm->num_index) {
        case FLOAT_SIZE_32:
          imm->v_float32 = atof( str); break;
        case FLOAT_SIZE_64:
          imm->v_float64 = atof( str); break;
        default: INT_FATAL("Unhandled case in switch statement");
      }
      break;
  }
}


// these support coerce_immediate (param casts)
const char* istrFromUserUint(long long unsigned int i) {
  char s[64];
  if (snprintf(s, sizeof(s), "%llu", i) >= (int) sizeof(s))
    INT_FATAL("istr buffer overflow");
  return astr(s);
}

const char* istrFromUserInt(long long int i) {
  char s[64];
  if (snprintf(s, sizeof(s), "%lld", i) >= (int) sizeof(s))
    INT_FATAL("istr buffer overflow");
  return astr(s);
}

const char* istrFromUserDouble(double i) {
  char s[64];
  if (snprint_float_val(s, sizeof(s), i, false) >= (int) sizeof(s))
    INT_FATAL("istr buffer overflow");
  return astr(s);
}

const char* istrFromUserImag(double i) {
  char s[64];
  if (snprint_imag_val(s, sizeof(s), i, false) >= (int) sizeof(s))
    INT_FATAL("istr buffer overflow");
  return astr(s);
}


const char* istrFromUserComplex(double re, double im) {
  char s[140];
  if (snprint_complex_val(s, sizeof(s), re, im) >= (int) sizeof(s))
    INT_FATAL("istr buffer overflow");
  return astr(s);
}
