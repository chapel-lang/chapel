/* -*-Mode: c++;-*-
*/

#ifndef _num_h_
#define _num_h_

#include "defs.h"
#include "prim_data.h"

enum IF1_num_kind {
  IF1_NUM_KIND_NONE, IF1_NUM_KIND_UINT, IF1_NUM_KIND_INT, IF1_NUM_KIND_FLOAT, IF1_NUM_KIND_COMPLEX
};

enum IF1_const_kind {
  IF1_CONST_KIND_STRING = IF1_NUM_KIND_COMPLEX + 1, IF1_CONST_KIND_SYMBOL
};

enum IF1_int_type { 
  IF1_INT_TYPE_1, IF1_INT_TYPE_8, IF1_INT_TYPE_16, IF1_INT_TYPE_32, IF1_INT_TYPE_64,
  IF1_INT_TYPE_NUM
};

enum IF1_float_type { 
  IF1_FLOAT_TYPE_16, IF1_FLOAT_TYPE_32, IF1_FLOAT_TYPE_48, IF1_FLOAT_TYPE_64, 
  IF1_FLOAT_TYPE_80, IF1_FLOAT_TYPE_96, IF1_FLOAT_TYPE_112, IF1_FLOAT_TYPE_128, 
  IF1_FLOAT_TYPE_NUM
};

class Immediate : public gc { public:
  unsigned int const_kind : 4;
  unsigned int num_index : 3;
  union {
    bool      v_bool;
    int8      v_int8;
    int16     v_int16;
    int32     v_int32;
    int64     v_int64;
    // int128    v_int128;
    uint8     v_uint8;
    uint16    v_uint16;
    uint32    v_uint32;
    uint64    v_uint64;
    // uint128   v_uint128;
    float32   v_float32;
    float64   v_float64;
    float128  v_float128;
    complex32 v_complex32;
    complex64 v_complex64;
    char *v_string;
  };

  int64  int_value( void);
  uint64 uint_value( void);

  void set_int(int64 l, IF1_int_type int_type=IF1_INT_TYPE_64) {
    const_kind = IF1_NUM_KIND_INT;
    num_index = int_type;
    switch( int_type) {
    case IF1_INT_TYPE_8:  v_int8  = l; break;
    case IF1_INT_TYPE_16: v_int16 = l; break;
    case IF1_INT_TYPE_32: v_int32 = l; break;
    case IF1_INT_TYPE_64: v_int64 = l; break;
    default:
      assert(!"unsupported int IF1_int_type");
    }
  }

  void set_uint(uint64 ul, IF1_int_type uint_type=IF1_INT_TYPE_64) {
    const_kind = IF1_NUM_KIND_UINT;
    num_index = uint_type;
    switch( uint_type) {
    case IF1_INT_TYPE_8:  v_uint8  = ul; break;
    case IF1_INT_TYPE_16: v_uint16 = ul; break;
    case IF1_INT_TYPE_32: v_uint32 = ul; break;
    case IF1_INT_TYPE_64: v_uint64 = ul; break;
    default:
      assert(!"unsupported uint IF1_int_type");
    }
  }

  void set_float(float128 f, IF1_float_type fp_type=IF1_FLOAT_TYPE_64) {
    const_kind = IF1_NUM_KIND_FLOAT;
    num_index = fp_type;
    switch( fp_type) {
    case IF1_FLOAT_TYPE_32:  v_float32 = f;  break;
    case IF1_FLOAT_TYPE_64:  v_float64 = f;  break;
    case IF1_FLOAT_TYPE_128: v_float128 = f; break;
    default:
      assert(!"unsupported float IF1_float_type");
    }
  }

  void set_bool(bool b) {
    const_kind = IF1_NUM_KIND_UINT;
    num_index = IF1_INT_TYPE_1;
    v_bool = b;
  }
  Immediate& operator=(const Immediate&);
  Immediate& operator=(bool b) {
    const_kind = IF1_NUM_KIND_UINT;
    num_index = IF1_INT_TYPE_1;
    v_bool = b;
    return *this;
  }
  Immediate& operator=(char *s) {
    const_kind = IF1_CONST_KIND_STRING;
    v_string = s;
    return *this;
  }
  Immediate(bool b) {
    memset(this, 0, sizeof(*this));
    const_kind = IF1_NUM_KIND_UINT;
    num_index = IF1_INT_TYPE_1;
    v_bool = b;
  }
  Immediate(char *s) {
    memset(this, 0, sizeof(*this));
    const_kind = IF1_CONST_KIND_STRING;
    v_string = s;
  }
  Immediate();
  Immediate(const Immediate &im);
};


inline int64
Immediate::int_value( void) {
  int64 val;
  switch (num_index) {
  case IF1_INT_TYPE_8 : val = v_int8;  break;
  case IF1_INT_TYPE_16: val = v_int16; break;
  case IF1_INT_TYPE_32: val = v_int32; break;
  case IF1_INT_TYPE_64: val = v_int64; break;
  default:
    assert(!"unknown int size");
  }
  return val;
}


inline uint64
Immediate::uint_value( void) {
  uint64 val;
  switch (num_index) {
  case IF1_INT_TYPE_1 : val = v_bool;  break;
  case IF1_INT_TYPE_8 : val = v_uint8;  break;
  case IF1_INT_TYPE_16: val = v_uint16; break;
  case IF1_INT_TYPE_32: val = v_uint32; break;
  case IF1_INT_TYPE_64: val = v_uint64; break;
  default:
    assert(!"unknown uint size");
  }
  return val;
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

#define CPP_IS_LAME {{0,0,0,0,0,0,0,0}, {"bool", "uint8","uint16","uint32","uint64",0,0,0}, {"bool","int8","int16","int32","int64",0,0,0}, {0,"float32",0,"float64",0,0,0,"float128"}}
IFA_EXTERN char *num_kind_string[4][8] IFA_EXTERN_INIT(CPP_IS_LAME);
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

int fprint_imm(FILE *fp, Immediate &imm);
int sprint_imm(char *s, Immediate &imm);
int sprint_imm(char *str, char *control_string, Immediate &imm);
void coerce_immediate(Immediate *from, Immediate *to);
void fold_result(Immediate *imm1, Immediate *imm2, Immediate *imm);
void fold_constant(int op, Immediate *im1, Immediate *im2, Immediate *imm);
void convert_string_to_immediate(char *str, Immediate *imm);

#endif

