/* -*-Mode: c++;-*-
*/

#ifndef _num_h_
#define _num_h_

#include "geysa.h"
#include "extern.h"
#include "chpltypes.h"
#include "prim_data.h"

class Immediate : public gc { public:
  unsigned int const_kind : 4;
  unsigned int num_index : 3;
  union {
    bool v_bool;
    uint8 v_uint8;
    int8 v_int8;
    uint16 v_uint16;
    int16 v_int16;
    uint32 v_uint32;
    int32 v_int32;
    uint64 v_uint64;
    int64 v_int64;
    float32 v_float32;
    float64 v_float64;
    complex32 v_complex32;
    complex64 v_complex64;
    char *v_string;
  };
  Immediate& operator=(const Immediate&);
  Immediate();
};

class ImmHashFns { public:
  static unsigned int hash(Immediate *);
  static int equal(Immediate *, Immediate *);
};

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

#define CPP_IS_LAME {1,8,16,32,64}
EXTERN int int_type_precision[5] EXTERN_INIT(CPP_IS_LAME);
#undef CPP_IS_LAME

#define CPP_IS_LAME {16,32,48,64,80,96,112,128}
EXTERN int float_type_precision[8] EXTERN_INIT(CPP_IS_LAME);
#undef CPP_IS_LAME

#define CPP_IS_LAME {{0,0,0,0,0,0,0,0}, {"bool", "uint8","uint16","uint32","uint64",0,0,0}, {0,"int8","int16","int32","int64",0,0,0}, {0,"float32",0,"float64",0,0,0,0}}
EXTERN char *num_kind_string[4][8] EXTERN_INIT(CPP_IS_LAME);
#undef CPP_IS_LAME

inline Immediate& Immediate::operator=(const Immediate& imm) {
  memcpy(this, &imm, sizeof(imm));
  return *this;
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
void coerce_immediate(Immediate *from, Immediate *to);
void fold_result(Immediate *imm1, Immediate *imm2, Immediate *imm);
int fold_constant(int op, Immediate *im1, Immediate *im2, Immediate *imm);
void convert_string_to_immediate(char *str, Immediate *imm);

#endif

