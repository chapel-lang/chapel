#include "geysa.h"
#include "num.h"

static int 
sprint_float_val(char* str, double val) {
  int numchars = sprintf(str, "%g", val);
  if (strchr(str, '.') == NULL && strchr(str, 'e') == NULL) {
    strcat(str, ".0");
    return numchars + 2;
  } else {
    return numchars;
  }
}

static int 
sprint_complex_val(char* str, double real, double imm) {
  int numchars = 0;
  numchars += sprintf(str+numchars, "(");
  numchars += sprint_float_val(str+numchars, real);
  numchars += sprintf(str+numchars, ",");
  numchars += sprint_float_val(str+numchars, imm);
  numchars += sprintf(str+numchars, ")");
  return numchars;
}

int 
sprint_imm(char *str, Immediate &imm) {
  int res = -1;
  switch (imm.const_kind) {
    case IF1_NUM_KIND_NONE:
      break;
    case IF1_NUM_KIND_UINT: {
      switch (imm.num_index) {
        case IF1_INT_TYPE_1: 
          res = sprintf(str, "%u", imm.v_bool); break;
        case IF1_INT_TYPE_8: 
          res = sprintf(str, "%u", imm.v_uint8); break;
        case IF1_INT_TYPE_16:
          res = sprintf(str, "%u", imm.v_uint16); break;
        case IF1_INT_TYPE_32:
          res = sprintf(str, "%u", imm.v_uint32); break;
        case IF1_INT_TYPE_64:
          res = sprintf(str, "%llu", imm.v_uint64); break;
        default: assert(!"case");
      }
      break;
    }
    case IF1_NUM_KIND_INT: {
      switch (imm.num_index) {
        case IF1_INT_TYPE_8: 
          res = sprintf(str, "%d", imm.v_int8); break;
        case IF1_INT_TYPE_16:
          res = sprintf(str, "%d", imm.v_int16); break;
        case IF1_INT_TYPE_32:
          res = sprintf(str, "%d", imm.v_int32); break;
        case IF1_INT_TYPE_64:
          res = sprintf(str, "%lld", imm.v_int64); break;
        default: assert(!"case");
      }
      break;
    }
    case IF1_NUM_KIND_FLOAT:
      switch (imm.num_index) {
        case IF1_FLOAT_TYPE_32:
          res = sprint_float_val(str, imm.v_float32); break;
        case IF1_FLOAT_TYPE_64:
          res = sprint_float_val(str, imm.v_float64); break;
        default: assert(!"case");
      }
      break;
    case IF1_NUM_KIND_COMPLEX:
      switch (imm.num_index) {
        case IF1_FLOAT_TYPE_32: 
          res = sprint_complex_val(str, imm.v_complex32.r, imm.v_complex32.i); break;
        case IF1_FLOAT_TYPE_64:
          res = sprint_complex_val(str, imm.v_complex64.r, imm.v_complex64.i); break;
        default: assert(!"case");
      }
      break;
    case IF1_CONST_KIND_STRING:
      res = sprintf(str, "%s", imm.v_string); break;
      break;
  }
  return res;
}

int 
fprint_imm(FILE *fp, Immediate &imm) {
  int res = -1;
  switch (imm.const_kind) {
    case IF1_NUM_KIND_NONE:
      break;
    case IF1_NUM_KIND_UINT: {
      switch (imm.num_index) {
        case IF1_INT_TYPE_1: 
          res = fprintf(fp, "%u", imm.v_bool); break;
        case IF1_INT_TYPE_8: 
          res = fprintf(fp, "%u", imm.v_uint8); break;
        case IF1_INT_TYPE_16:
          res = fprintf(fp, "%u", imm.v_uint16); break;
        case IF1_INT_TYPE_32:
          res = fprintf(fp, "%u", imm.v_uint32); break;
        case IF1_INT_TYPE_64:
          res = fprintf(fp, "%llu", imm.v_uint64); break;
        default: assert(!"case");
      }
      break;
    }
    case IF1_NUM_KIND_INT: {
      switch (imm.num_index) {
        case IF1_INT_TYPE_8: 
          res = fprintf(fp, "%d", imm.v_int8); break;
        case IF1_INT_TYPE_16:
          res = fprintf(fp, "%d", imm.v_int16); break;
        case IF1_INT_TYPE_32:
          res = fprintf(fp, "%d", imm.v_int32); break;
        case IF1_INT_TYPE_64:
          res = fprintf(fp, "%lld", imm.v_int64); break;
        default: assert(!"case");
      }
      break;
    }
    case IF1_NUM_KIND_FLOAT:
      switch (imm.num_index) {
        case IF1_FLOAT_TYPE_32: {
          char str[80];
          res = sprint_float_val(str, imm.v_float32); 
          fputs(str, fp);
          break;
        }
        case IF1_FLOAT_TYPE_64: {
          char str[80];
          res = sprint_float_val(str, imm.v_float64); 
          fputs(str, fp);
          break;
        }
        default: assert(!"case");
      }
      break;
    case IF1_NUM_KIND_COMPLEX:
      switch (imm.num_index) {
        case IF1_FLOAT_TYPE_32: {
          char str[80];
          res = sprint_complex_val(str, imm.v_complex32.r, imm.v_complex32.i); 
          fputs(str, fp);
          break;
        }
        case IF1_FLOAT_TYPE_64: {
          char str[80];
          res = sprint_complex_val(str, imm.v_complex64.r, imm.v_complex64.i); 
          fputs(str, fp);
          break;
        }
        default: assert(!"case");
      }
      break;
    case IF1_CONST_KIND_STRING:
      res = fprintf(fp, "%s", imm.v_string); break;
      break;
  }
  return res;
}

void
coerce_immediate(Immediate *from, Immediate *to) {
#include "cast_code.cpp"
}

#define DO_FOLD(_op) \
      switch (imm->const_kind) { \
        case IF1_NUM_KIND_NONE: \
          break; \
        case IF1_NUM_KIND_UINT: { \
          switch (imm->num_index) { \
            case IF1_INT_TYPE_1:  \
              imm->v_bool = im1->v_bool _op im2->v_bool; break; \
            case IF1_INT_TYPE_8:  \
              imm->v_uint8 = im1->v_uint8 _op im2->v_uint8; break; \
            case IF1_INT_TYPE_16: \
              imm->v_uint16 = im1->v_uint16 _op im2->v_uint16; break; \
            case IF1_INT_TYPE_32: \
              imm->v_uint32 = im1->v_uint32 _op im2->v_uint32; break; \
            case IF1_INT_TYPE_64: \
              imm->v_uint64 = im1->v_uint64 _op im2->v_uint64; break; \
            default: assert(!"case"); \
          } \
          break; \
        } \
        case IF1_NUM_KIND_INT: { \
          switch (imm->num_index) { \
            case IF1_INT_TYPE_8:  \
              imm->v_int8 = im1->v_int8 _op im2->v_int8; break; \
            case IF1_INT_TYPE_16: \
              imm->v_int16 = im1->v_int16 _op im2->v_int16; break; \
            case IF1_INT_TYPE_32: \
              imm->v_int32 = im1->v_int32 _op im2->v_int32; break; \
            case IF1_INT_TYPE_64: \
              imm->v_int64 = im1->v_int64 _op im2->v_int64; break; \
            default: assert(!"case"); \
          } \
          break; \
        } \
        case IF1_NUM_KIND_FLOAT: \
          switch (imm->num_index) { \
            case IF1_FLOAT_TYPE_32: \
              imm->v_float32 = im1->v_float32 _op im2->v_float32; break; \
            case IF1_FLOAT_TYPE_64: \
              imm->v_float64 = im1->v_float64 _op im2->v_float64; break; \
            default: assert(!"case"); \
          } \
          break; \
      }

#define DO_FOLDI(_op) \
      switch (imm->const_kind) { \
        case IF1_NUM_KIND_NONE: \
          break; \
        case IF1_NUM_KIND_UINT: { \
          switch (imm->num_index) { \
            case IF1_INT_TYPE_1:  \
              imm->v_bool = im1->v_bool _op im2->v_bool; break; \
            case IF1_INT_TYPE_8:  \
              imm->v_uint8 = im1->v_uint8 _op im2->v_uint8; break; \
            case IF1_INT_TYPE_16: \
              imm->v_uint16 = im1->v_uint16 _op im2->v_uint16; break; \
            case IF1_INT_TYPE_32: \
              imm->v_uint32 = im1->v_uint32 _op im2->v_uint32; break; \
            case IF1_INT_TYPE_64: \
              imm->v_uint64 = im1->v_uint64 _op im2->v_uint64; break; \
            default: assert(!"case"); \
          } \
          break; \
        } \
        case IF1_NUM_KIND_INT: { \
          switch (imm->num_index) { \
            case IF1_INT_TYPE_8:  \
              imm->v_int8 = im1->v_int8 _op im2->v_int8; break; \
            case IF1_INT_TYPE_16: \
              imm->v_int16 = im1->v_int16 _op im2->v_int16; break; \
            case IF1_INT_TYPE_32: \
              imm->v_int32 = im1->v_int32 _op im2->v_int32; break; \
            case IF1_INT_TYPE_64: \
              imm->v_int64 = im1->v_int64 _op im2->v_int64; break; \
            default: assert(!"case"); \
          } \
          break; \
        } \
        case IF1_NUM_KIND_FLOAT: \
          switch (imm->num_index) { \
            default: assert(!"case"); \
          } \
          break; \
      }

#define DO_FOLD1(_op) \
      switch (imm->const_kind) { \
        case IF1_NUM_KIND_NONE: \
          break; \
        case IF1_NUM_KIND_UINT: { \
          switch (imm->num_index) { \
            case IF1_INT_TYPE_1:  \
              imm->v_bool = _op im1->v_bool; break; \
            case IF1_INT_TYPE_8:  \
              imm->v_uint8 = _op im1->v_uint8; break; \
            case IF1_INT_TYPE_16: \
              imm->v_uint16 = _op im1->v_uint16; break; \
            case IF1_INT_TYPE_32: \
              imm->v_uint32 = _op im1->v_uint32; break; \
            case IF1_INT_TYPE_64: \
              imm->v_uint64 = _op im1->v_uint64; break; \
            default: assert(!"case"); \
          } \
          break; \
        } \
        case IF1_NUM_KIND_INT: { \
          switch (imm->num_index) { \
            case IF1_INT_TYPE_8:  \
              imm->v_int8 = _op im1->v_int8; break; \
            case IF1_INT_TYPE_16: \
              imm->v_int16 = _op im1->v_int16; break; \
            case IF1_INT_TYPE_32: \
              imm->v_int32 = _op im1->v_int32; break; \
            case IF1_INT_TYPE_64: \
              imm->v_int64 = _op im1->v_int64; break; \
            default: assert(!"case"); \
          } \
          break; \
        } \
        case IF1_NUM_KIND_FLOAT: \
          switch (imm->num_index) { \
            case IF1_FLOAT_TYPE_32: \
              imm->v_float32 = _op im1->v_float32; break; \
            case IF1_FLOAT_TYPE_64: \
              imm->v_float64 =  _op im1->v_float64; break; \
            default: assert(!"case"); \
          } \
          break; \
      }

#define DO_FOLD1I(_op) \
      switch (imm->const_kind) { \
        case IF1_NUM_KIND_NONE: \
          break; \
        case IF1_NUM_KIND_UINT: { \
          switch (imm->num_index) { \
            case IF1_INT_TYPE_1:  \
              imm->v_bool = _op im1->v_bool; break; \
            case IF1_INT_TYPE_8:  \
              imm->v_uint8 = _op im1->v_uint8; break; \
            case IF1_INT_TYPE_16: \
              imm->v_uint16 = _op im1->v_uint16; break; \
            case IF1_INT_TYPE_32: \
              imm->v_uint32 = _op im1->v_uint32; break; \
            case IF1_INT_TYPE_64: \
              imm->v_uint64 = _op im1->v_uint64; break; \
            default: assert(!"case"); \
          } \
          break; \
        } \
        case IF1_NUM_KIND_INT: { \
          switch (imm->num_index) { \
            case IF1_INT_TYPE_8:  \
              imm->v_int8 = _op im1->v_int8; break; \
            case IF1_INT_TYPE_16: \
              imm->v_int16 = _op im1->v_int16; break; \
            case IF1_INT_TYPE_32: \
              imm->v_int32 = _op im1->v_int32; break; \
            case IF1_INT_TYPE_64: \
              imm->v_int64 = _op im1->v_int64; break; \
            default: assert(!"case"); \
          } \
          break; \
        } \
        case IF1_NUM_KIND_FLOAT: \
          switch (imm->num_index) { \
            default: assert(!"case"); \
          } \
          break; \
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
  if (im2->const_kind == IF1_NUM_KIND_FLOAT) {
    Immediate *t = im2; im2 = im1; im1 = t;
  }
  if (im1->const_kind == IF1_NUM_KIND_FLOAT) {
    if (int_type_precision[im2->const_kind] <= float_type_precision[im1->const_kind]) {
      imm->const_kind = im1->const_kind;
      imm->num_index = im1->num_index;
      return;
    }
    if (int_type_precision[im2->const_kind] >= 32) {
      imm->const_kind = IF1_NUM_KIND_FLOAT;
      imm->num_index = IF1_FLOAT_TYPE_32;
      return;
    }
    imm->const_kind = IF1_NUM_KIND_FLOAT;
    imm->num_index = IF1_FLOAT_TYPE_64;
    return;
  }
  // mixed signed and unsigned
  if (im1->num_index >= IF1_INT_TYPE_64 || im2->num_index >= IF1_INT_TYPE_64) {
    imm->const_kind = IF1_NUM_KIND_INT;
    imm->num_index = IF1_INT_TYPE_64;
    return;
  } else if (im1->num_index >= IF1_INT_TYPE_32 || im2->num_index >= IF1_INT_TYPE_32) {
    imm->const_kind = IF1_NUM_KIND_INT;
    imm->num_index = IF1_INT_TYPE_32;
    return;
  } else if (im1->num_index >= IF1_INT_TYPE_16 || im2->num_index >= IF1_INT_TYPE_16) {
    imm->const_kind = IF1_NUM_KIND_INT;
    imm->num_index = IF1_INT_TYPE_16;
    return;
  } else if (im1->num_index >= IF1_INT_TYPE_8 || im2->num_index >= IF1_INT_TYPE_8) {
    imm->const_kind = IF1_NUM_KIND_INT;
    imm->num_index = IF1_INT_TYPE_8;
    return;
  }
  imm->const_kind = IF1_NUM_KIND_UINT;
  imm->num_index = IF1_INT_TYPE_1;
  return;
}

void
fold_constant(int op, Immediate *im1, Immediate *im2, Immediate *imm) {
  switch (op) {
    case P_prim_mult:
    case P_prim_div:
    case P_prim_mod:
    case P_prim_add:
    case P_prim_subtract:
    case P_prim_lsh:
    case P_prim_rsh:
      fold_result(im1, im2, imm);
      break;
    case P_prim_less:
    case P_prim_lessorequal:
    case P_prim_greater:
    case P_prim_greaterorequal:
    case P_prim_equal:
    case P_prim_notequal:
    case P_prim_and:
    case P_prim_xor:
    case P_prim_or:
    case P_prim_land:
    case P_prim_lor:
      imm->const_kind = IF1_NUM_KIND_UINT;
      imm->num_index = IF1_INT_TYPE_1;
      break;
    case P_prim_plus:
    case P_prim_minus:
    case P_prim_bnot:
    case P_prim_not:
      imm->const_kind = im1->const_kind;
      imm->num_index = im1->num_index;
      break;
  }
  coerce_immediate(im1, imm);
  *im1 = *imm;
  if (im2) {
    coerce_immediate(im2, imm);
    *im2 = *imm;
  }
  switch (op) {
    case P_prim_mult: DO_FOLD(*); break;
    case P_prim_div: DO_FOLD(/); break;
    case P_prim_mod: DO_FOLDI(%); break;
    case P_prim_add: DO_FOLD(+); break;
    case P_prim_subtract: DO_FOLD(-); break;
    case P_prim_lsh: DO_FOLDI(<<); break;
    case P_prim_rsh: DO_FOLDI(>>); break;
    case P_prim_less: DO_FOLD(<); break;
    case P_prim_lessorequal: DO_FOLD(<=); break;
    case P_prim_greater: DO_FOLD(>); break;
    case P_prim_greaterorequal: DO_FOLD(>=); break;
    case P_prim_equal: DO_FOLD(==); break;
    case P_prim_notequal: DO_FOLD(!=); break;
    case P_prim_and: DO_FOLD(&&); break;
    case P_prim_xor: DO_FOLDI(^); break;
    case P_prim_or: DO_FOLD(||); break;
    case P_prim_land: DO_FOLDI(&); break;
    case P_prim_lor: DO_FOLDI(|); break;
    case P_prim_plus: DO_FOLD1(+); break;
    case P_prim_minus: DO_FOLD1(-); break;
    case P_prim_bnot: DO_FOLD1I(~); break;
    case P_prim_not: DO_FOLD1(!); break;
  }
}

void
convert_string_to_immediate(char *str, Immediate *imm) {
  switch (imm->const_kind) {
    case IF1_NUM_KIND_NONE:
      break;
    case IF1_NUM_KIND_UINT: {
      switch (imm->num_index) {
        case IF1_INT_TYPE_8: 
          if (str[0] != '\'')
            imm->v_uint8 = strtoul(str, 0, 0);
          else {
            if (str[1] != '\\')
              imm->v_uint8 = str[1];
            else
              imm->v_uint8 = str[2];
          }
          break;
        case IF1_INT_TYPE_16:
          imm->v_uint16 = strtoul(str, 0, 0); break;
        case IF1_INT_TYPE_32:
          imm->v_uint32 = strtoul(str, 0, 0); break;
        case IF1_INT_TYPE_64:
          imm->v_uint64 = strtoull(str, 0, 0); break;
        default: assert(!"case");
      }
      break;
    }
    case IF1_NUM_KIND_INT: {
      switch (imm->num_index) {
        case IF1_INT_TYPE_8: 
          if (str[0] != '\'')
            imm->v_int8 = strtoul(str, 0, 0);
          else {
            if (str[1] != '\\')
              imm->v_int8 = str[1];
            else
              imm->v_int8 = str[2];
          }
          break;
        case IF1_INT_TYPE_16:
          imm->v_int16 = strtol(str, 0, 0); break;
        case IF1_INT_TYPE_32:
          imm->v_int32 = strtol(str, 0, 0); break;
        case IF1_INT_TYPE_64:
          imm->v_int64 = strtoll(str, 0, 0); break;
        default: assert(!"case");
      }
      break;
    }
    case IF1_NUM_KIND_FLOAT:
      switch (imm->num_index) {
        case IF1_FLOAT_TYPE_32:
          imm->v_float32 = strtod(str, 0); break;
        case IF1_FLOAT_TYPE_64:
          imm->v_float64 = strtod(str, 0); break;
        default: assert(!"case");
      }
      break;
  }
}
