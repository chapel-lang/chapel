/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/ratomic/gasnet_ratomic.h $
 * Description: GASNet Remote Atomics API Header
 * Copyright 2017, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_RATOMIC_H
#define _GASNET_RATOMIC_H
#define _IN_GASNET_RATOMIC_H

#include <gasnetex.h>

GASNETI_BEGIN_EXTERNC
GASNETI_BEGIN_NOWARN

// gex_AD_t is an opaque scalar handle
struct gasneti_ad_t;
typedef struct gasneti_ad_t *gex_AD_t;

// GEX_AD_INVALID is guaranteed to be zero
#define GEX_AD_INVALID ((gex_AD_t)(uintptr_t)0)

// Forward decl of (internal) dispatch table type:
union gasnete_ratomic_fn_tbl_u;
typedef union gasnete_ratomic_fn_tbl_u *gasnete_ratomic_fn_tbl_t;

#ifndef _GEX_AD_T
  // TODO-EX: Current design has a pointer to the dispatch table in each AD.
  // However, we should investigate the alterative design in which the table
  // appears directly within the AD.
  #define GASNETI_AD_COMMON \
    GASNETI_OBJECT_HEADER              \
    gasneti_TM_t       _tm,   _tm0;    \
    gex_Rank_t         _rank, _rank0;  \
    gex_DT_t           _dt;            \
    gex_OP_t           _ops;           \
    int                _tools_safe;    \
    gasnete_ratomic_fn_tbl_t _fn_tbl;
  typedef struct { GASNETI_AD_COMMON } *gasneti_AD_t;
  #if GASNET_DEBUG
    extern gasneti_AD_t gasneti_import_ad(gex_AD_t _ad);
    extern gex_AD_t gasneti_export_ad(gasneti_AD_t _real_ad);
  #else
    #define gasneti_import_ad(x) ((gasneti_AD_t)(x))
    #define gasneti_export_ad(x) ((gex_AD_t)(x))
  #endif
  #define gex_AD_SetCData(ad,val)  ((void)(gasneti_import_ad(ad)->_cdata = (val)))
  #define gex_AD_QueryCData(ad)    ((void*)gasneti_import_ad(ad)->_cdata)
  #define gex_AD_QueryTM(ad)       gasneti_export_tm(gasneti_import_ad(ad)->_tm)
  #define gex_AD_QueryOps(ad)      ((gex_OP_t)gasneti_import_ad(ad)->_ops)
  #define gex_AD_QueryDT(ad)       ((gex_DT_t)gasneti_import_ad(ad)->_dt)
  #define gex_AD_QueryFlags(ad)    ((gex_Flags_t)gasneti_import_ad(ad)->_flags)
#endif

// Collective creation of an atomic domain - default implementation
#ifndef gex_AD_Create
  extern void gasneti_AD_Create(
        gex_AD_t                   *_ad_p,            // Output
        gex_TM_t                   _tm,               // The team
        gex_DT_t                   _dt,               // The data type
        gex_OP_t                   _ops,              // OR of operations to be supported
        gex_Flags_t                _flags             // flags
        );
  #define gex_AD_Create gasneti_AD_Create
#endif

// Collective destruction of an atomic domain - default implementation
#ifndef gex_AD_Destroy
  extern void gasneti_AD_Destroy(gex_AD_t _ad);
  #define gex_AD_Destroy gasneti_AD_Destroy
#endif


/*---------------------------------------------------------------------------------*/
//
// Common logic for RAtomic implementatons
//

#ifndef _GEX_AD_T
  #if GASNET_DEBUG
    extern void gasnete_ratomic_validate(
        gex_AD_t            _ad,        void             *_result_p,
        gex_Rank_t          _tgt_rank,  void             *_tgt_addr,
        gex_OP_t            _opcode,    gex_DT_t         _datatype,
        gex_Flags_t         _flags);
  #else
    #define gasnete_ratomic_validate(ad,result_p,rank,addr,op,dt,flags) ((void)0)
  #endif
#endif

// Which types may be accessed in cross-mapped segments using Tools.
// TODO-EX: We don't yet have preprocessor defines for precisely the property we
//   need here, but the following logic *is* accurate for the current tools code.
//   We should name the propery and assert it in gasnet_atomic_bits.h, not here.
#ifndef GASNETE_RATOMIC_PSHMSAFE_32
  #if GASNETI_ATOMIC32_NOT_SIGNALSAFE
    #define GASNETE_RATOMIC_PSHMSAFE_32 0
  #else
    #define GASNETE_RATOMIC_PSHMSAFE_32 1
  #endif
#endif
#ifndef GASNETE_RATOMIC_PSHMSAFE_64
  #if GASNETI_ATOMIC64_NOT_SIGNALSAFE
    #define GASNETE_RATOMIC_PSHMSAFE_64 0
  #else
    #define GASNETE_RATOMIC_PSHMSAFE_64 1
  #endif
#endif
#ifndef GASNETE_RATOMIC_PSHMSAFE_gex_dt_I32
  #define GASNETE_RATOMIC_PSHMSAFE_gex_dt_I32 GASNETE_RATOMIC_PSHMSAFE_32
#endif
#ifndef GASNETE_RATOMIC_PSHMSAFE_gex_dt_U32
  #define GASNETE_RATOMIC_PSHMSAFE_gex_dt_U32 GASNETE_RATOMIC_PSHMSAFE_32
#endif
#ifndef GASNETE_RATOMIC_PSHMSAFE_gex_dt_I64
  #define GASNETE_RATOMIC_PSHMSAFE_gex_dt_I64 GASNETE_RATOMIC_PSHMSAFE_64
#endif
#ifndef GASNETE_RATOMIC_PSHMSAFE_gex_dt_U64
  #define GASNETE_RATOMIC_PSHMSAFE_gex_dt_U64 GASNETE_RATOMIC_PSHMSAFE_64
#endif
#ifndef GASNETE_RATOMIC_PSHMSAFE_gex_dt_FLT
  #define GASNETE_RATOMIC_PSHMSAFE_gex_dt_FLT GASNETE_RATOMIC_PSHMSAFE_32
#endif
#ifndef GASNETE_RATOMIC_PSHMSAFE_gex_dt_DBL
  #define GASNETE_RATOMIC_PSHMSAFE_gex_dt_DBL GASNETE_RATOMIC_PSHMSAFE_64
#endif

//
// Macro for applying a 1-argument macro (FN) to each datatype
//
// Since the GEX_DT_* tokens are macros, they cannot safely be used as arguments.
// Instead a family of _gex_dt_* tokens are used, which can be mapped to
// several related tokens via concatenation to generate one of the macros
// which immediately follow.
#define GASNETE_DT_APPLY(FN) \
        FN(_gex_dt_I32) FN(_gex_dt_U32) \
        FN(_gex_dt_I64) FN(_gex_dt_U64) \
        FN(_gex_dt_FLT) FN(_gex_dt_DBL)
//
#define _gex_dt_I32_isint 1
#define _gex_dt_U32_isint 1
#define _gex_dt_I64_isint 1
#define _gex_dt_U64_isint 1
#define _gex_dt_FLT_isint 0
#define _gex_dt_DBL_isint 0
//
#define _gex_dt_I32_bits  32
#define _gex_dt_U32_bits  32
#define _gex_dt_I64_bits  64
#define _gex_dt_U64_bits  64
#define _gex_dt_FLT_bits  32
#define _gex_dt_DBL_bits  64
//
#define _gex_dt_I32_type  int32_t
#define _gex_dt_U32_type  uint32_t
#define _gex_dt_I64_type  int64_t
#define _gex_dt_U64_type  uint64_t
#define _gex_dt_FLT_type  float
#define _gex_dt_DBL_type  double
//
#define _gex_dt_I32_dtype GEX_DT_I32
#define _gex_dt_U32_dtype GEX_DT_U32
#define _gex_dt_I64_dtype GEX_DT_I64
#define _gex_dt_U64_dtype GEX_DT_U64
#define _gex_dt_FLT_dtype GEX_DT_FLT
#define _gex_dt_DBL_dtype GEX_DT_DBL
//
#define _gex_dt_I32_fmt "%i"
#define _gex_dt_U32_fmt "%u"
#define _gex_dt_I64_fmt "%" PRIi64
#define _gex_dt_U64_fmt "%" PRIu64
#define _gex_dt_FLT_fmt "%g"
#define _gex_dt_DBL_fmt "%g"
//
#define _gex_dt_I32_fmt_cast (int)
#define _gex_dt_U32_fmt_cast (unsigned int)
#define _gex_dt_I64_fmt_cast
#define _gex_dt_U64_fmt_cast
#define _gex_dt_FLT_fmt_cast
#define _gex_dt_DBL_fmt_cast


// Unions for type-punning
#define GASNETI_RATOMIC_UNION(bits) union gasnete_ratomic_##bits##_u
GASNETI_RATOMIC_UNION(32) {
    uint32_t _gex_ui;
    float    _gex_fp;
};
GASNETI_RATOMIC_UNION(64) {
    uint64_t _gex_ui;
    double   _gex_fp;
};

//
// Macros for building local atomic RMW operations using (strong) atomics
// TODO: generalize to weak atomic too?
//
// GASNETE_RATOMIC_CAS(output, tgt, dtcode, op3, operator, fences)
//   Use appropriate fixed-width CAS to apply an operator to the given data type
//
// Ex1:
//   GASNETE_RATOMIC_CAS(x, tgt, _gex_dt_I32, op1, GASNETE_RATOMIC_CAS_OP_MULT, 0)
//   Atomically performs { x = *tgt; *tgt = *tgt * op1; } on values of type int32_t
//
// Ex2:
//   GASNETE_RATOMIC_CAS(x, tgt, _gex_dt_DBL, op1, GASNETE_RATOMIC_CAS_OP_MAX, 0)
//   Atomically performs { x = *tgt; *tgt = MAX(*tgt, op1); } on values of type double
//
// Care is taken to evaluate 'output', 'tgt' and 'op1' exactly once.
// All operations are "fetching" (writing to an output variable).
// INC (DEC) are available indirectly using ADD (SUB) with an 'op1' of 1.
//
// The 'fences' argument is 0 or an OR of GASNETI_ATOMIC_* flags.
//
// Operators:
#define GASNETE_RATOMIC_CAS_OP_ADD(a,b)  (a + b)
#define GASNETE_RATOMIC_CAS_OP_SUB(a,b)  (a - b)
#define GASNETE_RATOMIC_CAS_OP_MULT(a,b) (a * b)
#define GASNETE_RATOMIC_CAS_OP_AND(a,b)  (a & b)
#define GASNETE_RATOMIC_CAS_OP_OR(a,b)   (a | b)
#define GASNETE_RATOMIC_CAS_OP_XOR(a,b)  (a ^ b)
#define GASNETE_RATOMIC_CAS_OP_MIN(a,b)  MIN(a, b)
#define GASNETE_RATOMIC_CAS_OP_MAX(a,b)  MAX(a, b)
//
#define GASNETE_RATOMIC_CAS(output, tgt, dtcode, op1, operator, fences) \
        _GASNETE_RATOMIC_CAS1(output, tgt, dtcode##_type, dtcode##_bits, op1, dtcode##_isint, operator, fences)
// This extra pass expands the "bits" and "isint" tokens prior to additional concatenation
#define _GASNETE_RATOMIC_CAS1(output, tgt, type, bits, op1, isint, operator, fences) \
        _GASNETE_RATOMIC_CAS2(output, tgt, type, bits, op1, isint, operator, fences)
#define _GASNETE_RATOMIC_CAS2(output, tgt, type, bits, op1, isint, operator, fences) \
        _GASNETE_RATOMIC_CAS3##isint(output, tgt, type, bits, op1, operator, fences)
#define _GASNETE_RATOMIC_CAS31(output, tgt, type, bits, op1, operator, fences) do { /* integer DTs */ \
        type const _op1 = (op1);                                                     \
        gasneti_atomic##bits##_t *_tgt = (gasneti_atomic##bits##_t*)(tgt);           \
        type _newval, _oldval;                                                       \
        do {                                                                         \
            _oldval = gasneti_atomic##bits##_read(_tgt, 0);                          \
            _newval = operator(_oldval, _op1);                                       \
        } while (! gasneti_atomic##bits##_compare_and_swap(_tgt,_oldval,_newval,fences)); \
        (output) = _oldval;                                                          \
    } while (0);
#define _GASNETE_RATOMIC_CAS30(output, tgt, type, bits, op1, operator, fences) do { /* FP DTs */ \
        type const _op1 = (op1);                                                     \
        gasneti_atomic##bits##_t *_tgt = (gasneti_atomic##bits##_t*)(tgt);           \
        GASNETI_RATOMIC_UNION(bits) _newval, _oldval;                                \
        do {                                                                         \
            _oldval._gex_ui = gasneti_atomic##bits##_read(_tgt, 0);                  \
            _newval._gex_fp = operator(_oldval._gex_fp, _op1);                       \
        } while (! gasneti_atomic##bits##_compare_and_swap(_tgt,_oldval._gex_ui,_newval._gex_ui,fences)); \
        (output) = _oldval._gex_fp;                                                  \
    } while (0);

//
// Define inline functions to perform local atomic operations
// Always fetching (w/ only SET having an unspecified value)
//
// TODO: it may be possible to further factor to reduce duplication between
// integer and floating-point ("SW1" and "SW0, respectively).
// Attempts so far have either been fragile, unmaintainable or made excessive
// use of type-punning and/or temporaries (which could harm optimization).
// Issues obstructing further factorization include:
//  + Desire to use gasneti_atomicNN_{add,sub}() with integer types (not CAS)
//  + Desire to apply union-based type-punning *only* with floating-point types
//
#define GASNETE_RATOMIC_FN_DEFN(dtcode) \
        _GASNETE_RATOMIC_FN_DEFN1(gasnete_ratomicfn##dtcode, dtcode, \
                                  dtcode##_type, dtcode##_bits, dtcode##_isint)
// This extra pass expands the "bits" and "isint" tokens prior to additional concatenation
#define _GASNETE_RATOMIC_FN_DEFN1(fname, dtcode, type, bits, isint) \
        _GASNETE_RATOMIC_FN_DEFN2(fname, dtcode, type, bits, isint)
#define _GASNETE_RATOMIC_FN_DEFN2(fname, dtcode, type, bits, isint) \
  GASNETI_INLINE(fname)                                                         \
  type fname(type *_tgt_addr, type _operand1, type _operand2, gex_OP_t _opcode, int _fences) \
  { type _result;                                                               \
    gasneti_atomic##bits##_t *_ratgt = (gasneti_atomic##bits##_t *)(_tgt_addr); \
    switch(_opcode) {                                                           \
      _GASNETE_RATOMIC_FN_SW##isint(dtcode, type, bits);                        \
      _GASNETE_RATOMIC_FN_CAS_CASE(dtcode, MULT);                               \
      _GASNETE_RATOMIC_FN_CAS_CASE(dtcode, MIN);                                \
      _GASNETE_RATOMIC_FN_CAS_CASE(dtcode, MAX);                                \
      default: gasneti_unreachable();                                           \
    }                                                                           \
    GASNETI_BUG3697();                                                          \
    return _result;                                                             \
  }
// Big SWitch cases for integer types (isint == 1)
#define _GASNETE_RATOMIC_FN_SW1(dtcode,type,bits) \
      case GEX_OP_SET:                                                           \
        gasneti_atomic##bits##_set(_ratgt, _operand1, _fences);                  \
        _result = 0; /* Sigh. Just to silence warnings */                        \
        break;                                                                   \
      case GEX_OP_GET:                                                           \
        _result = gasneti_atomic##bits##_read(_ratgt, _fences);                  \
        break;                                                                   \
      case GEX_OP_SWAP:                                                          \
        _result = gasneti_atomic##bits##_swap(_ratgt, _operand1, _fences);       \
        break;                                                                   \
      case GEX_OP_CAS: case GEX_OP_FCAS: {                                       \
        type _ratmp = _operand1;                                                 \
        do {                                                                     \
          if (gasneti_atomic##bits##_compare_and_swap(_ratgt, _ratmp, _operand2, _fences)) { \
            break;                                                               \
          }                                                                      \
          _ratmp = gasneti_atomic##bits##_read(_ratgt, 0);                       \
        } while (_ratmp == _operand1);                                           \
        _result = _ratmp;                                                        \
        break;                                                                   \
      }                                                                          \
      case GEX_OP_INC: case GEX_OP_FINC:                                         \
        _operand1 = 1; GASNETI_FALLTHROUGH /* to ADD */                          \
      case GEX_OP_ADD: case GEX_OP_FADD:                                         \
        _result = gasneti_atomic##bits##_add(_ratgt, _operand1, _fences) - _operand1;  \
        break;                                                                   \
      case GEX_OP_DEC: case GEX_OP_FDEC:                                         \
        _operand1 = 1; GASNETI_FALLTHROUGH /* to SUB */                          \
      case GEX_OP_SUB: case GEX_OP_FSUB:                                         \
        _result = gasneti_atomic##bits##_subtract(_ratgt, _operand1, _fences) + _operand1; \
        break;                                                                   \
      _GASNETE_RATOMIC_FN_CAS_CASE(dtcode, AND);                                 \
      _GASNETE_RATOMIC_FN_CAS_CASE(dtcode, OR);                                  \
      _GASNETE_RATOMIC_FN_CAS_CASE(dtcode, XOR);
// Big SWitch cases for floating-point types (isint == 0)
#define _GASNETE_RATOMIC_FN_SW0(dtcode,type,bits) \
      case GEX_OP_SET: {                                                         \
        GASNETI_RATOMIC_UNION(bits) _ratmp; _ratmp._gex_fp = _operand1;          \
        gasneti_atomic##bits##_set(_ratgt, _ratmp._gex_ui, _fences);             \
        _result = 0; /* Sigh. Just to silence warnings */                        \
        break;                                                                   \
      }                                                                          \
      case GEX_OP_GET: {                                                         \
        GASNETI_RATOMIC_UNION(bits) _ratmp;                                      \
        _ratmp._gex_ui = gasneti_atomic##bits##_read(_ratgt, _fences);           \
        _result = _ratmp._gex_fp;                                                \
        break;                                                                   \
      }                                                                          \
      case GEX_OP_SWAP: {                                                        \
        GASNETI_RATOMIC_UNION(bits) _ratmp; _ratmp._gex_fp = _operand1;          \
        _ratmp._gex_ui = gasneti_atomic##bits##_swap(_ratgt, _ratmp._gex_ui, _fences); \
        _result = _ratmp._gex_fp;                                                \
        break;                                                                   \
      }                                                                          \
      case GEX_OP_CAS: case GEX_OP_FCAS: {                                       \
        GASNETI_RATOMIC_UNION(bits) _raold; _raold._gex_fp = _operand1;          \
        GASNETI_RATOMIC_UNION(bits) _ranew; _ranew._gex_fp = _operand2;          \
        do {                                                                     \
          if (gasneti_atomic##bits##_compare_and_swap(_ratgt, _raold._gex_ui, _ranew._gex_ui, _fences)) { \
            break;                                                               \
          }                                                                      \
          _raold._gex_ui = gasneti_atomic##bits##_read(_ratgt, 0);               \
        } while (_raold._gex_fp == _operand1);                                   \
        _result = _raold._gex_fp;                                                \
        break;                                                                   \
      }                                                                          \
      case GEX_OP_INC: case GEX_OP_FINC:                                         \
        _operand1 = 1.; GASNETI_FALLTHROUGH /* to ADD */                         \
      _GASNETE_RATOMIC_FN_CAS_CASE(dtcode, ADD);                                 \
      case GEX_OP_DEC: case GEX_OP_FDEC:                                         \
        _operand1 = 1.; GASNETI_FALLTHROUGH /* to SUB */                         \
      _GASNETE_RATOMIC_FN_CAS_CASE(dtcode, SUB);
// Case for a CAS-based operation (both fetching and non-fetching)
#define _GASNETE_RATOMIC_FN_CAS_CASE(dtcode, opname) \
      case GEX_OP_##opname: case GEX_OP_F##opname: \
        GASNETE_RATOMIC_CAS(_result,_ratgt,dtcode,_operand1,GASNETE_RATOMIC_CAS_OP_##opname,_fences); \
        break;
// Work-around for bug 3697
#if PLATFORM_COMPILER_SUN
  #define GASNETI_BUG3697() gasneti_compiler_fence()
#else
  #define GASNETI_BUG3697() ((void)0)
#endif
//
GASNETE_DT_APPLY(GASNETE_RATOMIC_FN_DEFN)

//
// Typedef the function types used in dispatch (10 function types per DT).
//
// gasnete_ratomic[dtcode]_{NB,NBI}_{N,F}{0,1,2}_fn_t
//         N  Non-fetching operation
//         F  Fetching operation
//   {0,1,2}  Operand count
//
// Also defines a family of
//   GASNETE_RATOMIC_ARGS_{N,F}{0,1,2}(type)
// and
//   GASNETE_RATOMIC_PASS_{N,F}{0,1,2}
// macros for use in macros which declare or define such functions.
//
#define GASNETE_RATOMIC_FNTYPES(dtcode) \
  typedef gex_Event_t (gasnete_ratomic##dtcode##_NB_N0_fn_t )(GASNETE_RATOMIC_ARGS_N0(dtcode##_type)); \
  typedef gex_Event_t (gasnete_ratomic##dtcode##_NB_N1_fn_t )(GASNETE_RATOMIC_ARGS_N1(dtcode##_type)); \
  typedef gex_Event_t (gasnete_ratomic##dtcode##_NB_N2_fn_t )(GASNETE_RATOMIC_ARGS_N2(dtcode##_type)); \
  typedef gex_Event_t (gasnete_ratomic##dtcode##_NB_F0_fn_t )(GASNETE_RATOMIC_ARGS_F0(dtcode##_type)); \
  typedef gex_Event_t (gasnete_ratomic##dtcode##_NB_F1_fn_t )(GASNETE_RATOMIC_ARGS_F1(dtcode##_type)); \
  typedef gex_Event_t (gasnete_ratomic##dtcode##_NB_F2_fn_t )(GASNETE_RATOMIC_ARGS_F2(dtcode##_type)); \
  typedef int         (gasnete_ratomic##dtcode##_NBI_N0_fn_t)(GASNETE_RATOMIC_ARGS_N0(dtcode##_type)); \
  typedef int         (gasnete_ratomic##dtcode##_NBI_N1_fn_t)(GASNETE_RATOMIC_ARGS_N1(dtcode##_type)); \
  typedef int         (gasnete_ratomic##dtcode##_NBI_N2_fn_t)(GASNETE_RATOMIC_ARGS_N2(dtcode##_type)); \
  typedef int         (gasnete_ratomic##dtcode##_NBI_F0_fn_t)(GASNETE_RATOMIC_ARGS_F0(dtcode##_type)); \
  typedef int         (gasnete_ratomic##dtcode##_NBI_F1_fn_t)(GASNETE_RATOMIC_ARGS_F1(dtcode##_type)); \
  typedef int         (gasnete_ratomic##dtcode##_NBI_F2_fn_t)(GASNETE_RATOMIC_ARGS_F2(dtcode##_type)); \
//
#define GASNETE_RATOMIC_ARGS_N0(type) \
        gasneti_AD_t _real_ad, gex_Rank_t _tgt_rank, void* _tgt_addr, \
        gex_Flags_t _flags GASNETI_THREAD_FARG
#define GASNETE_RATOMIC_ARGS_N1(type) \
        gasneti_AD_t _real_ad, gex_Rank_t _tgt_rank, void* _tgt_addr, \
        type _operand1, gex_Flags_t _flags GASNETI_THREAD_FARG
#define GASNETE_RATOMIC_ARGS_N2(type) \
        gasneti_AD_t _real_ad, gex_Rank_t _tgt_rank, void* _tgt_addr, \
        type _operand1, type _operand2, gex_Flags_t _flags GASNETI_THREAD_FARG
#define GASNETE_RATOMIC_ARGS_F0(type) \
        gasneti_AD_t _real_ad, type* _result_p, gex_Rank_t _tgt_rank, void* _tgt_addr, \
        gex_Flags_t _flags GASNETI_THREAD_FARG
#define GASNETE_RATOMIC_ARGS_F1(type) \
        gasneti_AD_t _real_ad, type* _result_p, gex_Rank_t _tgt_rank, void* _tgt_addr, \
        type _operand1, gex_Flags_t _flags GASNETI_THREAD_FARG
#define GASNETE_RATOMIC_ARGS_F2(type) \
        gasneti_AD_t _real_ad, type* _result_p, gex_Rank_t _tgt_rank, void* _tgt_addr, \
        type _operand1, type _operand2, gex_Flags_t _flags GASNETI_THREAD_FARG
//
#define GASNETE_RATOMIC_PASS_N0 \
        _real_ad, _tgt_rank, _tgt_addr, _flags GASNETI_THREAD_PASS
#define GASNETE_RATOMIC_PASS_N1 \
        _real_ad, _tgt_rank, _tgt_addr, _operand1, _flags GASNETI_THREAD_PASS
#define GASNETE_RATOMIC_PASS_N2 \
        _real_ad, _tgt_rank, _tgt_addr, _operand1, _operand2, _flags GASNETI_THREAD_PASS
#define GASNETE_RATOMIC_PASS_F0 \
        _real_ad, _result_p, _tgt_rank, _tgt_addr, _flags GASNETI_THREAD_PASS
#define GASNETE_RATOMIC_PASS_F1 \
        _real_ad, _result_p, _tgt_rank, _tgt_addr, _operand1, _flags GASNETI_THREAD_PASS
#define GASNETE_RATOMIC_PASS_F2 \
        _real_ad, _result_p, _tgt_rank, _tgt_addr, _operand1, _operand2, _flags GASNETI_THREAD_PASS
//
GASNETE_DT_APPLY(GASNETE_RATOMIC_FNTYPES)

//
// Typedef structures full of function pointers used in dispatch (1 struct type per DT)
//
#define GASNETE_RATOMIC_FN_TBL(dtcode) \
  typedef struct { \
    _GASNETE_RATOMIC_FN_TBL(gasnete_ratomic##dtcode##_NB)  _gex_nb;  \
    _GASNETE_RATOMIC_FN_TBL(gasnete_ratomic##dtcode##_NBI) _gex_nbi; \
  } gasnete_ratomic##dtcode##_fn_tbl_t;
//
#define _GASNETE_RATOMIC_FN_TBL(prefix) \
  struct {                                                         \
    prefix##_N1_fn_t *_gex_op_AND,  *_gex_op_OR,   *_gex_op_XOR;   \
    prefix##_N1_fn_t *_gex_op_ADD,  *_gex_op_SUB,  *_gex_op_MULT;  \
    prefix##_N1_fn_t *_gex_op_MIN,  *_gex_op_MAX;                  \
    prefix##_N0_fn_t *_gex_op_INC,  *_gex_op_DEC;                  \
    prefix##_F1_fn_t *_gex_op_FAND, *_gex_op_FOR,  *_gex_op_FXOR;  \
    prefix##_F1_fn_t *_gex_op_FADD, *_gex_op_FSUB, *_gex_op_FMULT; \
    prefix##_F1_fn_t *_gex_op_FMIN, *_gex_op_FMAX;                 \
    prefix##_F0_fn_t *_gex_op_FINC, *_gex_op_FDEC;                 \
    prefix##_N1_fn_t *_gex_op_SET;                                 \
    prefix##_F0_fn_t *_gex_op_GET;                                 \
    prefix##_F1_fn_t *_gex_op_SWAP;                                \
    prefix##_F2_fn_t *_gex_op_FCAS;                                \
    prefix##_N2_fn_t *_gex_op_CAS;                                 \
  }
//
GASNETE_DT_APPLY(GASNETE_RATOMIC_FN_TBL)

//
// Single union of all gasnete_ratomic[dtcode]_fn_tbl_t types
//
#define GASNETE_RATOMIC_FN_UNION(dtcode) gasnete_ratomic##dtcode##_fn_tbl_t dtcode;
union gasnete_ratomic_fn_tbl_u { GASNETE_DT_APPLY(GASNETE_RATOMIC_FN_UNION) };

//
// Tracing / statistics support
//

#if GASNETI_STATS_OR_TRACE
  #define _GASNETE_RATOMIC_EVENT(prefix,dtype) do { \
    switch(dtype) {                                               \
      case GEX_DT_I32: _GASNETI_STAT_EVENT(R,prefix##I32); break; \
      case GEX_DT_U32: _GASNETI_STAT_EVENT(R,prefix##U32); break; \
      case GEX_DT_I64: _GASNETI_STAT_EVENT(R,prefix##I64); break; \
      case GEX_DT_U64: _GASNETI_STAT_EVENT(R,prefix##U64); break; \
      case GEX_DT_FLT: _GASNETI_STAT_EVENT(R,prefix##FLT); break; \
      case GEX_DT_DBL: _GASNETI_STAT_EVENT(R,prefix##DBL); break; \
      default: gasneti_unreachable();                             \
    }                                                             \
  } while (0)
#endif

#if GASNET_TRACE
  #define _GASNETE_TRACE_RATOMIC(prefix,ad,dtype,result_p,tgt_rank,tgt_addr,opcode,flags,fmt,cast,op1,op2) do { \
    const char *_trat_suffix = "";                                      \
    switch(dtype) {                                                     \
      case GEX_DT_I32: _trat_suffix="I32"; break;                       \
      case GEX_DT_U32: _trat_suffix="U32"; break;                       \
      case GEX_DT_I64: _trat_suffix="I64"; break;                       \
      case GEX_DT_U64: _trat_suffix="U64"; break;                       \
      case GEX_DT_FLT: _trat_suffix="FLT"; break;                       \
      case GEX_DT_DBL: _trat_suffix="DBL"; break;                       \
      default: gasneti_unreachable();                                   \
    }                                                                   \
    if (GASNETI_TRACE_ENABLED(R)) {                                     \
      _GASNETE_RATOMIC_EVENT(prefix,dtype);                             \
      char *_trat_opstr = (char *)gasneti_extern_malloc(gasneti_format_op(NULL,opcode));\
      gasneti_format_op(_trat_opstr,opcode);                            \
      char _trat_resultstr[32] = "";                                    \
      if (gasneti_op_fetch(opcode)) {                                   \
        snprintf(_trat_resultstr, sizeof(_trat_resultstr),              \
                 " " GASNETI_LADDRFMT " <-",GASNETI_LADDRSTR(result_p));\
      }                                                                 \
      GASNETI_TRACE_PRINTF(R,(#prefix "%s: %s%s " GASNETI_RADDRFMT " flags=0x%x",\
                           _trat_suffix,_trat_opstr,_trat_resultstr,    \
                           GASNETI_RADDRSTR(gex_AD_QueryTM(ad),tgt_rank,tgt_addr),flags)); \
      gasneti_extern_free(_trat_opstr);                                 \
    }                                                                   \
    if (gasneti_op_1arg(opcode)) {                                      \
      GASNETI_TRACE_PRINTF(D,(#prefix "%s: operand = " fmt,             \
                              _trat_suffix, cast op1));                 \
    } else if (opcode == GEX_OP_FCAS || opcode == GEX_OP_CAS) {         \
      GASNETI_TRACE_PRINTF(D,(#prefix "%s: oldval = " fmt               \
                              ", newval = " fmt,                        \
                              _trat_suffix, cast op1, cast op2));       \
    } else gasneti_assert(gasneti_op_0arg(opcode) || !gasneti_op_valid_atomic(opcode)); \
  } while (0)
  #define GASNETE_TRACE_RATOMIC_gex_nb(ad,dtype,result_p,tgt_rank,tgt_addr,opcode,flags,fmt,cast,op1,op2) \
          _GASNETE_TRACE_RATOMIC(RATOMIC_NB_,ad,dtype,result_p,tgt_rank,tgt_addr,opcode,flags,fmt,cast,op1,op2)
  #define GASNETE_TRACE_RATOMIC_gex_nbi(ad,dtype,result_p,tgt_rank,tgt_addr,opcode,flags,fmt,cast,op1,op2) \
          _GASNETE_TRACE_RATOMIC(RATOMIC_NBI_,ad,dtype,result_p,tgt_rank,tgt_addr,opcode,flags,fmt,cast,op1,op2)
#elif GASNET_STATS
  #define GASNETE_TRACE_RATOMIC_gex_nb(ad,dtype,result_p,tgt_rank,tgt_addr,opcode,flags,fmt,cast,op1,op2) \
          _GASNETE_RATOMIC_EVENT(RATOMIC_NB_,dtype)
  #define GASNETE_TRACE_RATOMIC_gex_nbi(ad,dtype,result_p,tgt_rank,tgt_addr,opcode,flags,fmt,cast,op1,op2) \
          _GASNETE_RATOMIC_EVENT(RATOMIC_NBI_,dtype)
#else
  #define GASNETE_TRACE_RATOMIC_gex_nb(ad,dtype,result_p,tgt_rank,tgt_addr,opcode,flags,fmt,cast,op1,op2) \
          ((void)0)
  #define GASNETE_TRACE_RATOMIC_gex_nbi(ad,dtype,result_p,tgt_rank,tgt_addr,opcode,flags,fmt,cast,op1,op2) \
          ((void)0)
#endif

//
// Helpers for supporting GEX_FLAG_RANK_IS_JOBRANK
//
GASNETI_INLINE(gasnete_ratomic_i_tm)
gasneti_TM_t gasnete_ratomic_i_tm(gasneti_AD_t _ad, gex_Flags_t _flags) {
  return (_flags & GEX_FLAG_RANK_IS_JOBRANK) ? _ad->_tm0 : _ad->_tm;
}
GASNETI_INLINE(gasnete_ratomic_e_tm)
gex_TM_t gasnete_ratomic_e_tm(gasneti_AD_t _ad, gex_Flags_t _flags) {
  return gasneti_export_tm(gasnete_ratomic_i_tm(_ad, _flags));
}
GASNETI_INLINE(gasnete_ratomic_self)
gex_Rank_t gasnete_ratomic_self(gasneti_AD_t _ad, gex_Flags_t _flags) {
  return (_flags & GEX_FLAG_RANK_IS_JOBRANK) ? _ad->_rank0 : _ad->_rank;
}

//
// Define of a full family of "dispatch" functions that together
// constitute the default implementation of remote atomics.
//
// GASNETE_RATOMIC_DISP(dtcode):
//   Expands to definitions of two inline functions:
//       inline gex_Event_t gasnete_ratomic##dtcode##_NB() { ... }
//       inline int         gasnete_ratomic##dtcode##_NBI() { ... }
//     and extern declarations two functions of identical functionality:
//       extern gex_Event_t gasnete_ratomic##dtcode##_NB_external();
//       extern int         gasnete_ratomic##dtcode##_NBI_external();
//     Each has the signature of a gex_AD_* op-initiation call for 'type'.
#define GASNETE_RATOMIC_DISP(dtcode) \
        _GASNETE_RATOMIC_DISP1(gasnete_ratomic##dtcode, dtcode##_isint, dtcode##_type, dtcode)
// This extra pass expands the "isint" token prior to additional concatenation
#define _GASNETE_RATOMIC_DISP1(prefix, isint, type, dtcode) \
        _GASNETE_RATOMIC_DISP2(prefix##_NB, _gex_nb, isint, type, dtcode, gex_Event_t, GEX_EVENT_INVALID) \
        _GASNETE_RATOMIC_DISP2(prefix##_NBI, _gex_nbi, isint, type, dtcode, int, 0)
#define _GASNETE_RATOMIC_DISP2(fname,nbnbi,isint,type,dtcode,rettype,retdone) \
    extern rettype fname##_external(_GASNETE_RATOMIC_DISP_ARGS(type))    \
                                      _GASNETE_RATOMIC_DISP_WARN##nbnbi; \
    GASNETI_INLINE(fname) _GASNETE_RATOMIC_DISP_WARN##nbnbi              \
    rettype fname(_GASNETE_RATOMIC_DISP_ARGS(type))                      \
    {                                                                    \
        GASNETE_TRACE_RATOMIC##nbnbi(_ad, dtcode##_dtype, _result_p,     \
                                     _tgt_rank,_tgt_addr,_opcode,_flags, \
                                     dtcode##_fmt,dtcode##_fmt_cast,     \
                                     _operand1,_operand2);               \
        gasnete_ratomic_validate(_ad,_result_p,_tgt_rank,_tgt_addr,      \
                                 _opcode, dtcode##_dtype, _flags);       \
        gasneti_AD_t _real_ad = gasneti_import_ad(_ad);                  \
        if (GASNETE_RATOMIC_ALWAYS_TOOLS_SAFE##dtcode ||                 \
            _real_ad->_tools_safe) {                                     \
          _GASNETE_RATOMIC_DISP_TOOLS_SAFE(dtcode,type,retdone);         \
        }                                                                \
        switch(_opcode) {                                                \
            _GASNETE_RATOMIC_DISP_CASE1(dtcode, nbnbi, SET,   N1)        \
            _GASNETE_RATOMIC_DISP_CASE1(dtcode, nbnbi, GET,   F0)        \
            _GASNETE_RATOMIC_DISP_CASE1(dtcode, nbnbi, SWAP,  F1)        \
            _GASNETE_RATOMIC_DISP_CASE1(dtcode, nbnbi, FCAS,  F2)        \
            _GASNETE_RATOMIC_DISP_CASE1(dtcode, nbnbi, CAS,   N2)        \
            _GASNETE_RATOMIC_DISP_INT##isint(dtcode, nbnbi, AND, 1)      \
            _GASNETE_RATOMIC_DISP_INT##isint(dtcode, nbnbi, OR,  1)      \
            _GASNETE_RATOMIC_DISP_INT##isint(dtcode, nbnbi, XOR, 1)      \
            _GASNETE_RATOMIC_DISP_CASE2(dtcode, nbnbi, ADD,   1)         \
            _GASNETE_RATOMIC_DISP_CASE2(dtcode, nbnbi, SUB,   1)         \
            _GASNETE_RATOMIC_DISP_CASE2(dtcode, nbnbi, MULT,  1)         \
            _GASNETE_RATOMIC_DISP_CASE2(dtcode, nbnbi, MIN,   1)         \
            _GASNETE_RATOMIC_DISP_CASE2(dtcode, nbnbi, MAX,   1)         \
            _GASNETE_RATOMIC_DISP_CASE2(dtcode, nbnbi, INC,   0)         \
            _GASNETE_RATOMIC_DISP_CASE2(dtcode, nbnbi, DEC,   0)         \
            default: gasneti_unreachable();                              \
        }                                                                \
        gasneti_unreachable();                                           \
        return retdone;                                                  \
    }
#define _GASNETE_RATOMIC_DISP_WARN_gex_nb  GASNETI_WARN_UNUSED_RESULT
#define _GASNETE_RATOMIC_DISP_WARN_gex_nbi /*empty*/
// Note that _GASNETE_RATOMIC_DISP_TOOLS_SAFE has unusually "flow" in that it
// EITHER completes the operation synchronously using tools and *returns*
// OR it continues through to the next statement.
#define _GASNETE_RATOMIC_DISP_TOOLS_SAFE(dtcode,type,retdone) do { \
        _GASNETE_RATOMIC_DISP_TOOLS_CHECK(dtcode)                            \
        const int _fences = ((_flags & GEX_FLAG_AD_ACQ) ? GASNETI_ATOMIC_ACQ : 0) \
                          | ((_flags & GEX_FLAG_AD_REL) ? GASNETI_ATOMIC_REL : 0);\
        type _result = gasnete_ratomicfn##dtcode((type *)_tgt_addr,          \
                                                 _operand1, _operand2,       \
                                                 _opcode, _fences);          \
        if (gasneti_op_fetch(_opcode)) {                                     \
            *_result_p = _result;                                            \
        }                                                                    \
        return retdone;                                                      \
    } while (0)
#if GASNET_PSHM
  #define _GASNETE_RATOMIC_DISP_TOOLS_CHECK(dtcode) \
    if (_flags & GEX_FLAG_AD_MY_RANK) {                                  \
        gasneti_assert(_tgt_rank == gasnete_ratomic_self(_real_ad,_flags));\
        /* Will use tools */                                             \
    } else if (GASNETE_RATOMIC_PSHMSAFE##dtcode) {                       \
        if (_flags & GEX_FLAG_AD_MY_NBRHD) {                             \
            gex_TM_t _tm = gasnete_ratomic_e_tm(_real_ad,_flags);        \
            gasneti_assert(GASNETI_NBRHD_LOCAL(_tm,_tgt_rank));          \
            _tgt_addr = GASNETI_NBRHD_LOCAL_ADDR(_tm,_tgt_rank,_tgt_addr);\
            /* Will use tools */                                         \
        } else {                                                         \
            gex_TM_t _tm = gasnete_ratomic_e_tm(_real_ad,_flags);        \
            void *_tmp_addr = GASNETI_NBRHD_LOCAL_ADDR_OR_NULL(_tm,_tgt_rank,_tgt_addr);\
            if (!_tmp_addr) break; /* Leave enclosing do/while w/o using tools */ \
            _tgt_addr = (dtcode##_type *)_tmp_addr;                      \
            /* Will use tools */                                         \
        }                                                                \
    } else {                                                             \
       break; /* Leave enclosing do/while w/o using tools */             \
    }
#else
  #define _GASNETE_RATOMIC_DISP_TOOLS_CHECK(dtcode) \
    if ((_flags & (GEX_FLAG_AD_MY_RANK|GEX_FLAG_AD_MY_NBRHD)) ||         \
        (_tgt_rank == gasnete_ratomic_self(_real_ad,_flags))) {          \
        gasneti_assert(_tgt_rank == gasnete_ratomic_self(_real_ad,_flags));\
        /* Will use tools */                                             \
    } else {                                                             \
       break; /* Leave enclosing do/while w/o using tools */             \
    }
#endif
#define _GASNETE_RATOMIC_DISP_ARGS(type) \
         gex_AD_t            _ad,        type             *_result_p,    \
         gex_Rank_t          _tgt_rank,  void             *_tgt_addr,    \
         gex_OP_t            _opcode,    type             _operand1,     \
         type                _operand2,  gex_Flags_t      _flags         \
         GASNETI_THREAD_FARG
#define _GASNETE_RATOMIC_DISP_INT0(dtcode,nbnbi,opstem,nargs)  /*empty*/
#define _GASNETE_RATOMIC_DISP_INT1 _GASNETE_RATOMIC_DISP_CASE2
#define _GASNETE_RATOMIC_DISP_CASE2(dtcode,nbnbi,opstem,nargs) \
        _GASNETE_RATOMIC_DISP_CASE1(dtcode,nbnbi,opstem,N##nargs) \
        _GASNETE_RATOMIC_DISP_CASE1(dtcode,nbnbi,F##opstem,F##nargs)
#define _GASNETE_RATOMIC_DISP_CASE1(dtcode,nbnbi,opname,args) \
            case GEX_OP_##opname: \
                return (_GASNETE_RATOMIC_DISP_TBL(dtcode).nbnbi._gex_op_##opname)(GASNETE_RATOMIC_PASS_##args);
#if GASNETE_RATOMIC_AMONLY
  // AM is the only implementation - ignore _fn_tbl and resolve at link time instead
  #define _GASNETE_RATOMIC_DISP_TBL(dtcode) gasnete_amratomic##dtcode##_fn_tbl
  #define GASNETE_AMRATOMIC_TBL_DECL(dtcode) \
    extern gasnete_ratomic##dtcode##_fn_tbl_t gasnete_amratomic##dtcode##_fn_tbl;
  GASNETE_DT_APPLY(GASNETE_AMRATOMIC_TBL_DECL)
  #undef GASNETE_AMRATOMIC_TBL_DECL
#else
  #define _GASNETE_RATOMIC_DISP_TBL(dtcode) _real_ad->_fn_tbl->dtcode
#endif
//
GASNETE_DT_APPLY(GASNETE_RATOMIC_DISP)

/*---------------------------------------------------------------------------------*/
//
// Default implementation
// Maps client-facing gex_Op{NB,NBI}_*() calls to the default dispatch
// functions, unless the conduit has previously defined overrides.
//

// GASNETE_RATOMIC_NO_EXTERNAL is a compile-time "kill switch" for the use of
// gasneti_constant_p() to call an external function for dispatch of gex_AD_Op*
// when 'op' is non-constant.
// If defined non-zero then the inline functions will always be used.

#if GASNETE_RATOMIC_NO_EXTERNAL
  #define GASNETE_RATOMIC_FN(suffix,ad,result_p,rank,addr,opcode,op1,op2,flags) \
     gasnete_ratomic_##suffix(ad,result_p,rank,addr,opcode,op1,op2,flags GASNETI_THREAD_GET)
#else
  #define GASNETE_RATOMIC_FN(suffix,ad,result_p,rank,addr,opcode,op1,op2,flags) \
    (gasneti_constant_p(opcode) \
     ? gasnete_ratomic_##suffix(ad,result_p,rank,addr,opcode,op1,op2,flags GASNETI_THREAD_GET) \
     : gasnete_ratomic_##suffix##_external(ad,result_p,rank,addr,opcode,op1,op2,flags GASNETI_THREAD_GET))
#endif
//
#if !defined(gex_AD_OpNB_I32) || !defined(gex_AD_OpNBI_I32)
  #if defined(gex_AD_OpNB_I32) || defined(gex_AD_OpNBI_I32)
    #error "Conduit must override gex_AD_OpNB_I32 and gex_AD_OpNBI_I32_NBI together"
  #endif
  #define gex_AD_OpNB_I32(ad,result_p,rank,addr,opcode,op1,op2,flags) \
          GASNETE_RATOMIC_FN(gex_dt_I32_NB,ad,result_p,rank,addr,opcode,op1,op2,flags)
  #define gex_AD_OpNBI_I32(ad,result_p,rank,addr,opcode,op1,op2,flags) \
          GASNETE_RATOMIC_FN(gex_dt_I32_NBI,ad,result_p,rank,addr,opcode,op1,op2,flags)
#endif
#if !defined(gex_AD_OpNB_U32) || !defined(gex_AD_OpNBI_U32)
  #if defined(gex_AD_OpNB_U32) || defined(gex_AD_OpNBI_U32)
    #error "Conduit must override gex_AD_OpNB_U32 and gex_AD_OpNBI_U32_NBI together"
  #endif
  #define gex_AD_OpNB_U32(ad,result_p,rank,addr,opcode,op1,op2,flags) \
          GASNETE_RATOMIC_FN(gex_dt_U32_NB,ad,result_p,rank,addr,opcode,op1,op2,flags)
  #define gex_AD_OpNBI_U32(ad,result_p,rank,addr,opcode,op1,op2,flags) \
          GASNETE_RATOMIC_FN(gex_dt_U32_NBI,ad,result_p,rank,addr,opcode,op1,op2,flags)
#endif
#if !defined(gex_AD_OpNB_I64) || !defined(gex_AD_OpNBI_I64)
  #if defined(gex_AD_OpNB_I64) || defined(gex_AD_OpNBI_I64)
    #error "Conduit must override gex_AD_OpNB_I64_NB and gex_AD_OpNBI_I64_NBI together"
  #endif
  #define gex_AD_OpNB_I64(ad,result_p,rank,addr,opcode,op1,op2,flags) \
          GASNETE_RATOMIC_FN(gex_dt_I64_NB,ad,result_p,rank,addr,opcode,op1,op2,flags)
  #define gex_AD_OpNBI_I64(ad,result_p,rank,addr,opcode,op1,op2,flags) \
          GASNETE_RATOMIC_FN(gex_dt_I64_NBI,ad,result_p,rank,addr,opcode,op1,op2,flags)
#endif
#if !defined(gex_AD_OpNB_U64) || !defined(gex_AD_OpNBI_U64)
  #if defined(gex_AD_OpNB_U64) || defined(gex_AD_OpNBI_U64)
    #error "Conduit must override gex_AD_OpNB_U64_NB and gex_AD_OpNBI_U64_NBI together"
  #endif
  #define gex_AD_OpNB_U64(ad,result_p,rank,addr,opcode,op1,op2,flags) \
          GASNETE_RATOMIC_FN(gex_dt_U64_NB,ad,result_p,rank,addr,opcode,op1,op2,flags)
  #define gex_AD_OpNBI_U64(ad,result_p,rank,addr,opcode,op1,op2,flags) \
          GASNETE_RATOMIC_FN(gex_dt_U64_NBI,ad,result_p,rank,addr,opcode,op1,op2,flags)
#endif
#if !defined(gex_AD_OpNB_FLT) || !defined(gex_AD_OpNBI_FLT)
  #if defined(gex_AD_OpNB_FLT) || defined(gex_AD_OpNBI_FLT)
    #error "Conduit must override gex_AD_OpNB_FLT_NB and gex_AD_OpNBI_FLT_NBI together"
  #endif
  #define gex_AD_OpNB_FLT(ad,result_p,rank,addr,opcode,op1,op2,flags) \
          GASNETE_RATOMIC_FN(gex_dt_FLT_NB,ad,result_p,rank,addr,opcode,op1,op2,flags)
  #define gex_AD_OpNBI_FLT(ad,result_p,rank,addr,opcode,op1,op2,flags) \
          GASNETE_RATOMIC_FN(gex_dt_FLT_NBI,ad,result_p,rank,addr,opcode,op1,op2,flags)
#endif
#if !defined(gex_AD_OpNB_DBL) || !defined(gex_AD_OpNBI_DBL)
  #if defined(gex_AD_OpNB_DBL) || defined(gex_AD_OpNBI_DBL)
    #error "Conduit must override gex_AD_OpNB_DBL_NB and gex_AD_OpNBI_DBL_NBI together"
  #endif
  #define gex_AD_OpNB_DBL(ad,result_p,rank,addr,opcode,op1,op2,flags) \
          GASNETE_RATOMIC_FN(gex_dt_DBL_NB,ad,result_p,rank,addr,opcode,op1,op2,flags)
  #define gex_AD_OpNBI_DBL(ad,result_p,rank,addr,opcode,op1,op2,flags) \
          GASNETE_RATOMIC_FN(gex_dt_DBL_NBI,ad,result_p,rank,addr,opcode,op1,op2,flags)
#endif

/*---------------------------------------------------------------------------------*/

GASNETI_END_NOWARN
GASNETI_END_EXTERNC

#undef _IN_GASNET_RATOMIC_H
#endif
