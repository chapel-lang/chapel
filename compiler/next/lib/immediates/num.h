/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef CHPL_IMMEDIATES_NUM_H
#define CHPL_IMMEDIATES_NUM_H

#include "chpl/queries/Context.h"
#include "chpl/queries/UniqueString.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>

#ifndef IFA_EXTERN
#define IFA_EXTERN extern
#endif
#ifndef IFA_EXTERN_INIT
#define IFA_EXTERN_INIT(x)
#endif

extern unsigned int open_hash_multipliers[256];

struct complex64 {
  float r;
  float i;
};
struct complex128 {
  double r;
  double i;
};

using ImmString = chpl::detail::PODUniqueString;

//
// NOTE: NUM_KIND_LAST is used to mark the last entry in this enum. The
// 'IF1_const_kind' enum below uses it to set values.
//
enum IF1_num_kind : uint8_t {
  NUM_KIND_NONE, NUM_KIND_BOOL, NUM_KIND_UINT, NUM_KIND_INT, NUM_KIND_REAL,
  NUM_KIND_IMAG, NUM_KIND_COMPLEX, NUM_KIND_COMMID, NUM_KIND_LAST
};

enum IF1_const_kind : uint8_t {
  CONST_KIND_STRING = NUM_KIND_LAST + 1, CONST_KIND_SYMBOL
};

enum IF1_string_kind : uint8_t {
  STRING_KIND_STRING,
  STRING_KIND_BYTES,
  STRING_KIND_C_STRING
};

// This is a sentinel value whose actual value doesn't really matter
// as long as it can be distinguished from other legal bool sizes (so
// that `bool` remains a distinct type from `bool(n)` for the legal
// values of 'n'
//
#define BOOL_SYS_WIDTH 0

enum IF1_bool_type : uint8_t {
  BOOL_SIZE_SYS, BOOL_SIZE_8, BOOL_SIZE_16, BOOL_SIZE_32,
  BOOL_SIZE_64, BOOL_SIZE_NUM
};

// when updating these, be sure to also update int_type_precision!
enum IF1_int_type : uint8_t {
  INT_SIZE_8, INT_SIZE_16, INT_SIZE_32, INT_SIZE_64, INT_SIZE_NUM
};

// when updating these, be sure to also update float_type_precision!
enum IF1_float_type : uint8_t {
  FLOAT_SIZE_32, FLOAT_SIZE_64, FLOAT_SIZE_NUM
};

// these should correspond to double the IF1_float_types.
// i.e. float_type_precision[i] here should refer to the real size of i
enum IF1_complex_type : uint8_t {
  COMPLEX_SIZE_64, COMPLEX_SIZE_128, COMPLEX_SIZE_NUM
};

//
// The following #defines are intended to indicate what bit widths
// from the enums above are used as the default for Chapel types that
// are specified without an explicit width.  For example a Chapel
// 'int' type corresponds to an 'int(64)' since INT_SIZE_DEFAULT below
// is INT_SIZE_64.
//
#define BOOL_SIZE_DEFAULT BOOL_SIZE_SYS
#define INT_SIZE_DEFAULT INT_SIZE_64
#define FLOAT_SIZE_DEFAULT FLOAT_SIZE_64
#define COMPLEX_SIZE_DEFAULT COMPLEX_SIZE_128

namespace chpl {
namespace types {

class Immediate { public:
  uint8_t const_kind;
  IF1_string_kind string_kind;
  uint8_t num_index;
  union {
    // Unions are initialized based off the first element, so we need to have
    // the largest thing first to make sure it is all zero initialized

    // complex values - only used for the type's default value
    complex128 v_complex128;
    complex64  v_complex64;

    // floating-point values
    double     v_float64;
    float      v_float32;

    // signed integer values
    // int128     v_int128;
    int64_t    v_int64;
    int32_t    v_int32;
    int16_t    v_int16;
    int8_t     v_int8;

    // unsigned integer values
    // uint128    v_uint128;
    uint64_t   v_uint64;
    uint32_t   v_uint32;
    uint16_t   v_uint16;
    uint8_t    v_uint8;

    // boolean value
    uint64_t   v_bool;

    // string value
    ImmString v_string;
  };

  int64_t  int_value( void)     const;
  int64_t  commid_value( void)  const;
  uint64_t uint_value( void)    const;
  uint64_t bool_value( void)    const;
  const char* string_value( void)const;
  double real_value( void)const;
  // calls int_value, uint_value, or bool_value as appropriate.
  int64_t  to_int( void)        const;
  uint64_t to_uint( void)       const;
  std::string to_string(void)   const;

  Immediate& operator=(const Immediate&);
  Immediate& operator=(bool b) {
    const_kind = NUM_KIND_BOOL;
    num_index = BOOL_SIZE_SYS;
    v_bool = b;
    return *this;
  }
  /*Immediate& operator=(ImmString s) {
    const_kind = CONST_KIND_STRING;
    string_kind = STRING_KIND_C_STRING;
    v_string = s;
    return *this;
  }*/

  explicit
  Immediate(bool b) :
    const_kind(NUM_KIND_BOOL),
    string_kind(STRING_KIND_STRING),
    num_index(BOOL_SIZE_SYS)
  {
    v_bool = b;
  }

  Immediate(Context* context,
            const char* str, size_t len,
            IF1_string_kind kind) :
    const_kind(CONST_KIND_STRING),
    string_kind(kind),
    num_index(0)
  {
    v_string = ImmString::build(context, str, len);
  }

  Immediate();
  Immediate(const Immediate &im);
};

} // end namespace chpl
} // end namespace types

using Immediate = chpl::types::Immediate;

inline uint64_t
Immediate::bool_value( void) const {
  assert(const_kind == NUM_KIND_BOOL);
  return v_bool;
}

inline int64_t
Immediate::int_value( void) const {
  int64_t val = 0;
  assert(const_kind == NUM_KIND_INT);
  switch (num_index) {
  case INT_SIZE_8 : val = v_int8;  break;
  case INT_SIZE_16: val = v_int16; break;
  case INT_SIZE_32: val = v_int32; break;
  case INT_SIZE_64: val = v_int64; break;
  default:
    assert(false && "unknown int size");
  }
  return val;
}

inline const char*
Immediate::string_value( void) const {
  assert(const_kind == CONST_KIND_STRING);
  assert(string_kind == STRING_KIND_STRING ||
         string_kind == STRING_KIND_BYTES ||
         string_kind == STRING_KIND_C_STRING);

  return v_string.c_str();
}

inline double
Immediate::real_value( void) const {
  double val = 0.0;
  assert(const_kind == NUM_KIND_REAL || const_kind == NUM_KIND_IMAG);
  switch (num_index) {
  case FLOAT_SIZE_32: val = v_float32; break;
  case FLOAT_SIZE_64: val = v_float64; break;
  default:
    assert(false && "unknown real size");
  }
  return val;
}


inline int64_t
Immediate::commid_value( void) const {
  assert(const_kind == NUM_KIND_COMMID && num_index == INT_SIZE_64);
  return v_int64;
}


inline uint64_t
Immediate::uint_value( void) const {
  uint64_t val = 0;
  assert(const_kind == NUM_KIND_UINT);
  switch (num_index) {
  case INT_SIZE_8 : val = v_uint8;  break;
  case INT_SIZE_16: val = v_uint16; break;
  case INT_SIZE_32: val = v_uint32; break;
  case INT_SIZE_64: val = v_uint64; break;
  default:
    assert(false && "unknown uint size");
  }
  return val;
}

inline int64_t
Immediate::to_int( void) const {
  int64_t val = 0;
  switch (const_kind) {
    case NUM_KIND_INT : val = int_value();  break;
    case NUM_KIND_UINT: val = uint_value(); break;
    case NUM_KIND_BOOL: val = bool_value(); break;
  default:
    assert(false && "kind not handled in to_int");
  }
  return val;
}


inline uint64_t
Immediate::to_uint( void) const {
  uint64_t val = 0;
  switch (const_kind) {
    case NUM_KIND_INT : val = int_value();  break;
    case NUM_KIND_UINT: val = uint_value(); break;
    case NUM_KIND_BOOL: val = bool_value(); break;
  default:
    assert(false && "kind not handled in to_uint");
  }
  return val;
}

inline std::string Immediate::to_string(void) const {
  std::stringstream ss;
  switch(const_kind) {
  case NUM_KIND_INT: ss << int_value(); break;
  case NUM_KIND_BOOL: ss << bool_value(); break;
  case NUM_KIND_UINT: ss << uint_value(); break;
  case CONST_KIND_STRING: return v_string.toString();
  case NUM_KIND_REAL: {
    if (num_index == FLOAT_SIZE_32) {
      ss << v_float32;
    } else if (num_index == FLOAT_SIZE_64) {
      ss << v_float64;
    } else {
      assert(false && "Unexpected real size");
    }
    break;
  }
  case NUM_KIND_COMPLEX: {
    // Not expected to execute except for the type's
    // default value
    if (num_index == COMPLEX_SIZE_64) {
      ss << v_complex64.r << "+ " << v_complex64.i;
    } else if (num_index == COMPLEX_SIZE_128) {
      ss << v_complex128.r << "+ " << v_complex128.i;
    } else {
      assert(false && "Unexpected complex size");
    }
    ss << "i";
    break;
  }
  default: assert(false && "Unexpected type to convert to string"); break;
  } // Closes switch statement
  return ss.str();
}

class ImmHashFns { public:
  static unsigned int hash(Immediate *);
  static int equal(Immediate *, Immediate *);
};

#define CPP_IS_LAME {1,8,16,32,64}
IFA_EXTERN int int_type_precision[5] IFA_EXTERN_INIT(CPP_IS_LAME);
#undef CPP_IS_LAME

#define CPP_IS_LAME {32,64}
IFA_EXTERN int float_type_precision[8] IFA_EXTERN_INIT(CPP_IS_LAME);
#undef CPP_IS_LAME

#define CPP_IS_LAME {{0,0,0,0,0,0,0,0}, {"bool","uint8","uint16","uint32","uint64",0,0,0}, {"bool","int8","int16","int32","int64",0,0,0}, {0,"float32",0,"float64",0,0,0,"float128"}}
IFA_EXTERN const char *num_kind_string[4][8] IFA_EXTERN_INIT(CPP_IS_LAME);
#undef CPP_IS_LAME

inline Immediate& Immediate::operator=(const Immediate& imm) {
  memcpy((void*)this, &imm, sizeof(imm));
  return *this;
}

inline Immediate::Immediate(const Immediate& imm) {
  memcpy((void*)this, &imm, sizeof(imm));
}

inline Immediate::Immediate() {
  memset((void*)this, 0, sizeof(*this));
}

inline unsigned int
ImmHashFns::hash(Immediate *imm) {
  unsigned int h = 0;
  for (int i = 0; i < (int)(sizeof(*imm)/sizeof(unsigned int)); i++)
    h = h + open_hash_multipliers[i] * ((unsigned int*)imm)[i];
  return h;
}

inline int
ImmHashFns::equal(Immediate *imm1, Immediate *imm2) {
  return !memcmp(imm1, imm2, sizeof(*imm1));
}

int fprint_imm(FILE *fp, const Immediate &imm, bool showType = false);
int snprint_imm(char *s, size_t max, const Immediate &imm);
void coerce_immediate(chpl::Context* context, Immediate *from, Immediate *to);
void fold_result(Immediate *imm1, Immediate *imm2, Immediate *imm);
void fold_constant(chpl::Context* context, int op,
                   Immediate *im1, Immediate *im2, Immediate *imm);
void convert_string_to_immediate(const char *str, Immediate *imm);
ImmString istrFromUserBool(chpl::Context* context, bool b);
ImmString istrFromUserUint(chpl::Context* context, uint64_t i);
ImmString istrFromUserInt(chpl::Context* context, int64_t i);
ImmString istrFromUserDouble(chpl::Context* context, double i);
ImmString istrFromUserImag(chpl::Context* context, double i);
ImmString istrFromUserComplex(chpl::Context* context,
                                       double re, double im);

#endif
