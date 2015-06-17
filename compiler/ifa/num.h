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

#ifndef _num_h_
#define _num_h_

#include <cstring>
#include "chpltypes.h"
#include "map.h"
#include "misc.h"

#ifndef IFA_EXTERN
#define IFA_EXTERN extern
#endif
#ifndef IFA_EXTERN_INIT
#define IFA_EXTERN_INIT(x)
#endif

enum IF1_num_kind {
  NUM_KIND_NONE, NUM_KIND_BOOL, NUM_KIND_UINT, NUM_KIND_INT, NUM_KIND_REAL,
  NUM_KIND_IMAG, NUM_KIND_COMPLEX
};

enum IF1_const_kind {
  CONST_KIND_STRING = NUM_KIND_COMPLEX + 1, CONST_KIND_SYMBOL
};

enum IF1_bool_type {
  BOOL_SIZE_1, BOOL_SIZE_SYS, BOOL_SIZE_8, BOOL_SIZE_16, BOOL_SIZE_32, 
  BOOL_SIZE_64, BOOL_SIZE_NUM
};

enum IF1_int_type { 
  INT_SIZE_8, INT_SIZE_16, INT_SIZE_32, INT_SIZE_64, INT_SIZE_NUM
};

enum IF1_float_type { 
  FLOAT_SIZE_16, FLOAT_SIZE_32, FLOAT_SIZE_48, FLOAT_SIZE_64, 
  FLOAT_SIZE_80, FLOAT_SIZE_96, FLOAT_SIZE_112, FLOAT_SIZE_128, 
  FLOAT_SIZE_NUM
};

enum IF1_complex_type { 
  COMPLEX_SIZE_32, COMPLEX_SIZE_64, COMPLEX_SIZE_96, COMPLEX_SIZE_128, 
  COMPLEX_SIZE_160, COMPLEX_SIZE_192, COMPLEX_SIZE_224, COMPLEX_SIZE_256, 
  COMPLEX_SIZE_NUM
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


class Immediate { public:
  uint32_t const_kind;
  uint32_t num_index;
  union {
    uint64_t   v_bool;
    int8_t     v_int8;
    int16_t    v_int16;
    int32_t    v_int32;
    int64_t    v_int64;
    // int128     v_int128;
    uint8_t    v_uint8;
    uint16_t   v_uint16;
    uint32_t   v_uint32;
    uint64_t   v_uint64;
    // uint128    v_uint128;
    float      v_float32;
    double     v_float64;
    complex64  v_complex64;
    complex128 v_complex128;
    const char *v_string;
  };

  int64_t  int_value( void);
  uint64_t uint_value( void);
  uint64_t bool_value( void);
  const char* string_value( void);

  Immediate& operator=(const Immediate&);
  Immediate& operator=(bool b) {
    const_kind = NUM_KIND_BOOL;
    num_index = BOOL_SIZE_SYS;
    v_bool = b;
    return *this;
  }
  Immediate& operator=(char *s) {
    const_kind = CONST_KIND_STRING;
    v_string = s;
    return *this;
  }
  Immediate(bool b) {
    memset(this, 0, sizeof(*this));
    const_kind = NUM_KIND_BOOL;
    num_index = BOOL_SIZE_SYS;
    v_bool = b;
  }
  Immediate(const char *s) {
    memset(this, 0, sizeof(*this));
    const_kind = CONST_KIND_STRING;
    v_string = s;
  }
  Immediate();
  Immediate(const Immediate &im);
};

inline uint64_t
Immediate::bool_value( void) {
  return v_bool;
}

inline int64_t
Immediate::int_value( void) {
  int64_t val = 0;
  switch (num_index) {
  case INT_SIZE_8 : val = v_int8;  break;
  case INT_SIZE_16: val = v_int16; break;
  case INT_SIZE_32: val = v_int32; break;
  case INT_SIZE_64: val = v_int64; break;
  default:
    INT_FATAL("unknown int size");
  }
  return val;
}


inline uint64_t
Immediate::uint_value( void) {
  uint64_t val = 0;
  switch (num_index) {
  case INT_SIZE_8 : val = v_uint8;  break;
  case INT_SIZE_16: val = v_uint16; break;
  case INT_SIZE_32: val = v_uint32; break;
  case INT_SIZE_64: val = v_uint64; break;
  default:
    INT_FATAL("unknown uint size");
  }
  return val;
}

inline const char*
Immediate::string_value( void) {
  if( this->const_kind != CONST_KIND_STRING )
    INT_FATAL("not a string");
  return v_string;
}



class ImmHashFns { public:
  static unsigned int hash(Immediate *);
  static int equal(Immediate *, Immediate *);
};

#define CPP_IS_LAME {1,8,16,32,64}
IFA_EXTERN int int_type_precision[5] IFA_EXTERN_INIT(CPP_IS_LAME);
#undef CPP_IS_LAME

#define CPP_IS_LAME {16,32,48,64,80,96,112,128}
IFA_EXTERN int float_type_precision[8] IFA_EXTERN_INIT(CPP_IS_LAME);
#undef CPP_IS_LAME

#define CPP_IS_LAME {{0,0,0,0,0,0,0,0}, {"bool","uint8","uint16","uint32","uint64",0,0,0}, {"bool","int8","int16","int32","int64",0,0,0}, {0,"float32",0,"float64",0,0,0,"float128"}}
IFA_EXTERN const char *num_kind_string[4][8] IFA_EXTERN_INIT(CPP_IS_LAME);
#undef CPP_IS_LAME

inline Immediate& Immediate::operator=(const Immediate& imm) {
  memcpy(this, &imm, sizeof(imm));
  return *this;
}

inline Immediate::Immediate(const Immediate& imm) {
  memcpy(this, &imm, sizeof(imm));
}

inline Immediate::Immediate() {
  memset(this, 0, sizeof(*this));
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

int fprint_imm(FILE *fp, Immediate &imm, bool showType = false);
int snprint_imm(char *s, size_t max, Immediate &imm);
int snprint_imm(char *str, size_t max, char *control_string, Immediate &imm);
void coerce_immediate(Immediate *from, Immediate *to);
void fold_result(Immediate *imm1, Immediate *imm2, Immediate *imm);
void fold_constant(int op, Immediate *im1, Immediate *im2, Immediate *imm);
void convert_string_to_immediate(const char *str, Immediate *imm);
const char* istrFromUserUint(long long unsigned int i);
const char* istrFromUserInt(long long int i);

#endif

