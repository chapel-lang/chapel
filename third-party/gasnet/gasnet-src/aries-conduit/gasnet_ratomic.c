/*   $Source: bitbucket.org:berkeleylab/gasnet.git/aries-conduit/gasnet_ratomic.c $
 * Description: GASNet Remote Atomics Implementation using Aries NIC offload
 * Copyright 2018, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#define GASNETI_NEED_GASNET_RATOMIC_H 1
#include <gasnet_internal.h>

#if GASNETC_BUILD_GNIRATOMIC
//
// GASNet-EX Remote Atomics via offload to Aries NIC (not Gemini)
//
// TODO-EX: Add logic to avoid bounce-copy for in-segment *result_p?
// The branches and lookups to set up the fetch directly to the client's
// memory are almost certainly more expensive than the 4- or 8-byte copy.
//
// Aries (but not Gemini) NICs support the I32, I64, FLT and DBL types directly.
//
// Notes on mapping from GEX to GNI opcodes:
//  + INC, DEC and SUB are mapped to ADD w/ the appropriate operand
//  + (F)CAS operations use 32- and 64-bit integer types with the result
//    that IEEE +0 and -0 will *not* compare as equal
//  + Since non-fetching CAS ops require "doubled" alignment, we need to
//    use the fetching ops and ignore the result.
//  + Atomic SET is implemented as a non-fetching integer SWAP (follows [1])
//  + Atomic GET is implemented as an integer FAND(~0) (follows [1])
// TODO: Follow up w/ Howard Prichard regarding the rationale behind the
// mappings used for SET and GET.  In particular why is GET not FOR(0)?
//

// Notes on implementation of GEX_FLAG_AD_{REL,ACQ}
//
// RELEASE:
// We believe that injection of a GNI-level operation must include at least one
// release fence.  Additionally we must obtain a "gpd" from a gasneti_lifo_t,
// which in a multi-threaded build includes a full rel/acq on both x86-64 and
// arm64.
//
// ACQUIRE:
// The CQ handling in GNI requires an acquire fence for signaling between the
// NIC/driver, which is sufficient for a single-threaded build.  In the case of
// a multi-threaded build, the conduit's processing of the CQ includes a full
// mutex lock/unlock.

#include <gasnet_aries.h>
#include <gasnet_ratomic_internal.h>

// Value intended to trigger GNI_RC_ILLEGAL_OP if used as amo_cmd
#define GASNETC_INVALID_ATOMIC ((gni_fma_cmd_type_t)0xffff)

//
// Functions translating GEX atomic opcode index to GNI equivalents.
// Expected to be reduce to constants when fully inlined.
//

GASNETI_INLINE(amo_cmd_map_gex_dt_I32)
gni_fma_cmd_type_t amo_cmd_map_gex_dt_I32(gasneti_op_idx_t op_idx) {
    switch (op_idx) {
        case gasneti_op_idx_AND:   return GNI_FMA_ATOMIC2_AND_S;
        case gasneti_op_idx_OR:    return GNI_FMA_ATOMIC2_OR_S;
        case gasneti_op_idx_XOR:   return GNI_FMA_ATOMIC2_XOR_S;
        case gasneti_op_idx_ADD:   return GNI_FMA_ATOMIC2_IADD_S;
        case gasneti_op_idx_SUB:   return GNI_FMA_ATOMIC2_IADD_S;  // SUB: via ADD w/ negated arg
        case gasneti_op_idx_MIN:   return GNI_FMA_ATOMIC2_IMIN_S;  // SIGNED only!
        case gasneti_op_idx_MAX:   return GNI_FMA_ATOMIC2_IMAX_S;  // SIGNED only!
        case gasneti_op_idx_INC:   return GNI_FMA_ATOMIC2_IADD_S;  // INC: via ADD +1
        case gasneti_op_idx_DEC:   return GNI_FMA_ATOMIC2_IADD_S;  // DEC: via ADD -1
        case gasneti_op_idx_FAND:  return GNI_FMA_ATOMIC2_FAND_S;
        case gasneti_op_idx_FOR:   return GNI_FMA_ATOMIC2_FOR_S;
        case gasneti_op_idx_FXOR:  return GNI_FMA_ATOMIC2_FXOR_S;
        case gasneti_op_idx_FADD:  return GNI_FMA_ATOMIC2_FIADD_S;
        case gasneti_op_idx_FSUB:  return GNI_FMA_ATOMIC2_FIADD_S; // FSUB: via FADD w/ negated arg
        case gasneti_op_idx_FMIN:  return GNI_FMA_ATOMIC2_FIMIN_S; // SIGNED only!
        case gasneti_op_idx_FMAX:  return GNI_FMA_ATOMIC2_FIMAX_S; // SIGNED only!
        case gasneti_op_idx_FINC:  return GNI_FMA_ATOMIC2_FIADD_S; // FINC: via FADD +1
        case gasneti_op_idx_FDEC:  return GNI_FMA_ATOMIC2_FIADD_S; // FDEC: via FADD -1
        case gasneti_op_idx_SET:   return GNI_FMA_ATOMIC2_SWAP_S;  // SET: via (non-fetching) SWAP
        case gasneti_op_idx_GET:   return GNI_FMA_ATOMIC2_FAND_S;  // GET: via FAND(~0)
        case gasneti_op_idx_SWAP:  return GNI_FMA_ATOMIC2_FSWAP_S;
        case gasneti_op_idx_FCAS:  return GNI_FMA_ATOMIC2_FCSWAP_S;
        case gasneti_op_idx_CAS:   return GNI_FMA_ATOMIC2_FCSWAP_S; // CAS via FCAS and ignore result
        default: gasneti_unreachable();
    }
    return GASNETC_INVALID_ATOMIC;
}
#define amo_cmd_map_gex_dt_U32 amo_cmd_map_gex_dt_I32

GASNETI_INLINE(amo_cmd_map_gex_dt_I64)
gni_fma_cmd_type_t amo_cmd_map_gex_dt_I64(gasneti_op_idx_t op_idx) {
    switch (op_idx) {
        case gasneti_op_idx_AND:   return GNI_FMA_ATOMIC2_AND;
        case gasneti_op_idx_OR:    return GNI_FMA_ATOMIC2_OR;
        case gasneti_op_idx_XOR:   return GNI_FMA_ATOMIC2_XOR;
        case gasneti_op_idx_ADD:   return GNI_FMA_ATOMIC2_IADD;
        case gasneti_op_idx_SUB:   return GNI_FMA_ATOMIC2_IADD;  // SUB: via ADD w/ negated arg
        case gasneti_op_idx_MIN:   return GNI_FMA_ATOMIC2_IMIN;  // SIGNED only!
        case gasneti_op_idx_MAX:   return GNI_FMA_ATOMIC2_IMAX;  // SIGNED only!
        case gasneti_op_idx_INC:   return GNI_FMA_ATOMIC2_IADD;  // INC: via ADD +1
        case gasneti_op_idx_DEC:   return GNI_FMA_ATOMIC2_IADD;  // DEC: via ADD -1
        case gasneti_op_idx_FAND:  return GNI_FMA_ATOMIC2_FAND;
        case gasneti_op_idx_FOR:   return GNI_FMA_ATOMIC2_FOR;
        case gasneti_op_idx_FXOR:  return GNI_FMA_ATOMIC2_FXOR;
        case gasneti_op_idx_FADD:  return GNI_FMA_ATOMIC2_FIADD;
        case gasneti_op_idx_FSUB:  return GNI_FMA_ATOMIC2_FIADD; // FSUB: via FADD w/ negated arg
        case gasneti_op_idx_FMIN:  return GNI_FMA_ATOMIC2_FIMIN; // SIGNED only!
        case gasneti_op_idx_FMAX:  return GNI_FMA_ATOMIC2_FIMAX; // SIGNED only!
        case gasneti_op_idx_FINC:  return GNI_FMA_ATOMIC2_FIADD; // FINC: via FADD +1
        case gasneti_op_idx_FDEC:  return GNI_FMA_ATOMIC2_FIADD; // FDEC: via FADD -1
        case gasneti_op_idx_SET:   return GNI_FMA_ATOMIC2_SWAP;  // SET: via (non-fetching) SWAP
        case gasneti_op_idx_GET:   return GNI_FMA_ATOMIC2_FAND;  // GET: via FAND(~0)
        case gasneti_op_idx_SWAP:  return GNI_FMA_ATOMIC2_FSWAP;
        case gasneti_op_idx_FCAS:  return GNI_FMA_ATOMIC2_FCSWAP;
        case gasneti_op_idx_CAS:   return GNI_FMA_ATOMIC2_FCSWAP; // CAS via FCAS and ignore result
        default: gasneti_unreachable();
    }
    return GASNETC_INVALID_ATOMIC;
}
#define amo_cmd_map_gex_dt_U64 amo_cmd_map_gex_dt_I64

GASNETI_INLINE(amo_cmd_map_gex_dt_FLT)
gni_fma_cmd_type_t amo_cmd_map_gex_dt_FLT(gasneti_op_idx_t op_idx) {
    switch (op_idx) {
        case gasneti_op_idx_ADD:   return GNI_FMA_ATOMIC2_FPADD_S;
        case gasneti_op_idx_SUB:   return GNI_FMA_ATOMIC2_FPADD_S;  // SUB: via ADD w/ negated arg
        case gasneti_op_idx_MIN:   return GNI_FMA_ATOMIC2_FPMIN_S;
        case gasneti_op_idx_MAX:   return GNI_FMA_ATOMIC2_FPMAX_S;
        case gasneti_op_idx_INC:   return GNI_FMA_ATOMIC2_FPADD_S;  // INC: via ADD +1
        case gasneti_op_idx_DEC:   return GNI_FMA_ATOMIC2_FPADD_S;  // DEC: via ADD -1
        case gasneti_op_idx_FADD:  return GNI_FMA_ATOMIC2_FFPADD_S;
        case gasneti_op_idx_FSUB:  return GNI_FMA_ATOMIC2_FFPADD_S; // FSUB: via FADD w/ negated arg
        case gasneti_op_idx_FMIN:  return GNI_FMA_ATOMIC2_FFPMIN_S;
        case gasneti_op_idx_FMAX:  return GNI_FMA_ATOMIC2_FFPMAX_S;
        case gasneti_op_idx_FINC:  return GNI_FMA_ATOMIC2_FFPADD_S; // FINC: via FADD +1
        case gasneti_op_idx_FDEC:  return GNI_FMA_ATOMIC2_FFPADD_S; // FDEC: via FADD -1
        case gasneti_op_idx_SET:   return GNI_FMA_ATOMIC2_SWAP_S;   // SET: via (non-fetching) SWAP
        case gasneti_op_idx_GET:   return GNI_FMA_ATOMIC2_FAND_S;   // GET: via FAND(~0)
        case gasneti_op_idx_SWAP:  return GNI_FMA_ATOMIC2_FSWAP_S;
        case gasneti_op_idx_FCAS:  return GNI_FMA_ATOMIC2_FCSWAP_S;
        case gasneti_op_idx_CAS:   return GNI_FMA_ATOMIC2_FCSWAP_S; // CAS via FCAS and ignore result
        default: gasneti_unreachable();
    }
    return GASNETC_INVALID_ATOMIC;
}

GASNETI_INLINE(amo_cmd_map_gex_dt_DBL)
gni_fma_cmd_type_t amo_cmd_map_gex_dt_DBL(gasneti_op_idx_t op_idx) {
    switch (op_idx) {
        case gasneti_op_idx_ADD:   return GNI_FMA_ATOMIC2_FPADD;
        case gasneti_op_idx_SUB:   return GNI_FMA_ATOMIC2_FPADD;  // SUB: via ADD w/ negated arg
        case gasneti_op_idx_MIN:   return GNI_FMA_ATOMIC2_FPMIN;
        case gasneti_op_idx_MAX:   return GNI_FMA_ATOMIC2_FPMAX;
        case gasneti_op_idx_INC:   return GNI_FMA_ATOMIC2_FPADD;  // INC: via ADD +1
        case gasneti_op_idx_DEC:   return GNI_FMA_ATOMIC2_FPADD;  // DEC: via ADD -1
        case gasneti_op_idx_FADD:  return GNI_FMA_ATOMIC2_FFPADD;
        case gasneti_op_idx_FSUB:  return GNI_FMA_ATOMIC2_FFPADD; // FSUB: via FADD w/ negated arg
        case gasneti_op_idx_FMIN:  return GNI_FMA_ATOMIC2_FFPMIN;
        case gasneti_op_idx_FMAX:  return GNI_FMA_ATOMIC2_FFPMAX;
        case gasneti_op_idx_FINC:  return GNI_FMA_ATOMIC2_FFPADD; // FINC: via FADD +1
        case gasneti_op_idx_FDEC:  return GNI_FMA_ATOMIC2_FFPADD; // FDEC: via FADD -1
        case gasneti_op_idx_SET:   return GNI_FMA_ATOMIC2_SWAP;   // SET: via (non-fetching) SWAP
        case gasneti_op_idx_GET:   return GNI_FMA_ATOMIC2_FAND;   // GET: via FAND(~0)
        case gasneti_op_idx_SWAP:  return GNI_FMA_ATOMIC2_FSWAP;
        case gasneti_op_idx_FCAS:  return GNI_FMA_ATOMIC2_FCSWAP;
        case gasneti_op_idx_CAS:   return GNI_FMA_ATOMIC2_FCSWAP; // CAS via FCAS and ignore result
        default: gasneti_unreachable();
    }
    return GASNETC_INVALID_ATOMIC;
}

// Generic post of a GNI AMO
//
// First 3 params (fetching, op_cnt, length) will be manifest constants
// which to lead to specialization of the code upon inlining.
GASNETI_WARN_UNUSED_RESULT // Returns non-zero in IMMEDIATE case only
GASNETI_INLINE(gasnete_ratomic_inner)
int gasnete_ratomic_inner(
        const int fetching, const int op_cnt, const int length,
        void *result_p, gasneti_TM_t i_tm, gex_Rank_t tgt_rank, void *tgt_addr,
        gni_fma_cmd_type_t cmd, uint64_t operand1, uint64_t operand2,
        gasneti_weakatomic_val_t *initiated_p, gasnete_op_t * const op,
        uint32_t gpd_flags, gex_Flags_t flags GASNETC_DIDX_FARG)
{
  gasneti_suspend_spinpollers();
  gasnetc_post_descriptor_t * const gpd =
                gasnete_cntr_gpd(initiated_p, op, gpd_flags, flags GASNETC_DIDX_PASS);
  if (gpd) {
    if (fetching) {
      gpd->gpd_amo_result = (uintptr_t) result_p;
      gpd->gpd_flags |= ((length == 4) ? GC_POST_COPY_AMO4 : GC_POST_COPY_AMO8);
    }
    gpd->gpd_amo_len = length;
    gpd->gpd_amo_cmd = cmd;
    switch (op_cnt) {
      case 2: gpd->gpd_amo_op2 = operand2;
              GASNETI_FALLTHROUGH
      case 1: gpd->gpd_amo_op1 = operand1;
              break;
      default: gasneti_unreachable();
    }
    gasnetc_post_amo(i_tm, tgt_rank, tgt_addr, gpd, flags);
  }
  gasneti_resume_spinpollers();
  return !gpd;
}

// NB-specific wrapper around gasnete_ratomic_inner()
GASNETI_INLINE(gasnete_ratomic_nb)
gex_Event_t gasnete_ratomic_nb(
        const int fetching, const int op_cnt, const int length,
        void *result_p, gasneti_AD_t ad, gex_Rank_t tgt_rank, void *tgt_addr,
        gni_fma_cmd_type_t cmd, uint64_t operand1, uint64_t operand2,
        gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  GASNETC_DIDX_POST(mythread->domain_idx);
  gasnete_eop_t * const eop = gasnete_eop_new(mythread);
  gasneti_TM_t i_tm = gasnete_ratomic_i_tm(ad, flags);
  int imm = gasnete_ratomic_inner(fetching, op_cnt, length,
                                  result_p, i_tm, tgt_rank, tgt_addr,
                                  cmd, operand1, operand2,
                                  GASNETE_EOP_CNTRS(eop),
                                  flags GASNETC_DIDX_PASS);
  if (imm) {
      SET_EVENT_DONE(eop, 0);
      gasnete_eop_free(eop GASNETI_THREAD_PASS);
      return GEX_EVENT_NO_OP;
  }
  return (gex_Event_t) eop;
}

// NBI-specific wrapper around gasnete_ratomic_inner()
GASNETI_INLINE(gasnete_ratomic_nbi)
int gasnete_ratomic_nbi(
        const int fetching, const int op_cnt, const int length,
        void *result_p, gasneti_AD_t ad, gex_Rank_t tgt_rank, void *tgt_addr,
        gni_fma_cmd_type_t cmd, uint64_t operand1, uint64_t operand2,
        gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  GASNETC_DIDX_POST(mythread->domain_idx);
  gasnete_iop_t * const iop = mythread->current_iop;
  gasneti_TM_t i_tm = gasnete_ratomic_i_tm(ad, flags);
  int imm = gasnete_ratomic_inner(fetching, op_cnt, length,
                                  result_p, i_tm, tgt_rank, tgt_addr,
                                  cmd, operand1, operand2,
                                  GASNETE_IOP_CNTRS(iop, rmw),
                                  flags GASNETC_DIDX_PASS);
  return imm;
}

//
// Functions (called by the next group of functions) that
// then invoke the gasnete_ratomic_{nb,nbi}() functions, above.
//
// The [FN][012] interface is described in gasnet_ratomic.h, with
// the definition of the GASNETE_RATOMIC_FNTYPES() macro.
//
#define GASNETE_GNIRATOMIC_MID(dtcode) \
        _GASNETE_GNIRATOMIC_MID1(gasnete_gniratomic##dtcode, dtcode##_isint, dtcode##_type, dtcode)
// This extra pass expands the "isint" token prior to additional concatenation
#define _GASNETE_GNIRATOMIC_MID1(prefix, isint, type, dtcode) \
        _GASNETE_GNIRATOMIC_MID2(prefix, isint, type, dtcode)
#define _GASNETE_GNIRATOMIC_MID2(prefix, isint, type, dtcode)    \
    GASNETI_INLINE(prefix##_NB_N0)                                \
    gex_Event_t prefix##_NB_N0(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
        gasneti_assert(gasneti_op_0arg(((gex_OP_t)1 << op_idx))); \
        _GASNETE_GNIRATOMIC_PREP_INC##isint(type, op_idx);        \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        return gasnete_ratomic_nb(0, 1, sizeof(type), NULL,       \
                                  ad, tgt_rank, tgt_addr,         \
                                  cmd, inc, 0,                    \
                                  flags GASNETI_THREAD_PASS);     \
    } \
    GASNETI_INLINE(prefix##_NB_N1)                                \
    gex_Event_t prefix##_NB_N1(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,                          \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
        gasneti_assert(gasneti_op_1arg(((gex_OP_t)1 << op_idx))); \
        _GASNETE_GNIRATOMIC_PREP_NOP##isint(type, op_idx);        \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        return gasnete_ratomic_nb(0, 1, sizeof(type), NULL,       \
                                  ad, tgt_rank, tgt_addr,         \
                                  cmd, op1, 0,                    \
                                  flags GASNETI_THREAD_PASS);     \
    } \
    GASNETI_INLINE(prefix##_NB_N2)                                \
    gex_Event_t prefix##_NB_N2(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,  type        operand2,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
        gasneti_assert(gasneti_op_2arg(((gex_OP_t)1 << op_idx))); \
        _GASNETE_GNIRATOMIC_PREP_CAS##isint(type);                \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        return gasnete_ratomic_nb(0, 2, sizeof(type), NULL,       \
                                  ad, tgt_rank, tgt_addr,         \
                                  cmd, op1, op2,                  \
                                  flags GASNETI_THREAD_PASS);     \
    } \
    GASNETI_INLINE(prefix##_NB_F0)                                \
    gex_Event_t prefix##_NB_F0(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
        gasneti_assert(gasneti_op_0arg(((gex_OP_t)1 << op_idx))); \
        _GASNETE_GNIRATOMIC_PREP_FINC##isint(type, op_idx);       \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        return gasnete_ratomic_nb(1, 1, sizeof(type), result_p,   \
                                  ad, tgt_rank, tgt_addr,         \
                                  cmd, inc, 0,                    \
                                  flags GASNETI_THREAD_PASS);     \
    } \
    GASNETI_INLINE(prefix##_NB_F1)                                \
    gex_Event_t prefix##_NB_F1(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,                          \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
        gasneti_assert(gasneti_op_1arg(((gex_OP_t)1 << op_idx))); \
        _GASNETE_GNIRATOMIC_PREP_FOP##isint(type, op_idx);        \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        return gasnete_ratomic_nb(1, 1, sizeof(type), result_p,   \
                                  ad, tgt_rank, tgt_addr,         \
                                  cmd, op1, 0,                    \
                                  flags GASNETI_THREAD_PASS);     \
    } \
    GASNETI_INLINE(prefix##_NB_F2)                                \
    gex_Event_t prefix##_NB_F2(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,  type        operand2,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
        gasneti_assert(gasneti_op_2arg(((gex_OP_t)1 << op_idx))); \
        _GASNETE_GNIRATOMIC_PREP_CAS##isint(type);                \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        return gasnete_ratomic_nb(1, 2, sizeof(type), result_p,   \
                                  ad, tgt_rank, tgt_addr,         \
                                  cmd, op1, op2,                  \
                                  flags GASNETI_THREAD_PASS);     \
    } \
    GASNETI_INLINE(prefix##_NB_S1)                                \
    gex_Event_t prefix##_NB_S1(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,                          \
                gex_Flags_t flags         GASNETI_THREAD_FARG)    \
    {                                                             \
        _GASNETE_GNIRATOMIC_PREP_SET##isint(type);                \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        return gasnete_ratomic_nb(0, 1, sizeof(type), NULL,       \
                                  ad, tgt_rank, tgt_addr,         \
                                  cmd, val, 0,                    \
                                  flags GASNETI_THREAD_PASS);     \
    } \
    GASNETI_INLINE(prefix##_NB_G0)                                \
    gex_Event_t prefix##_NB_G0(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                gex_Flags_t flags         GASNETI_THREAD_FARG)    \
    {                                                             \
        _GASNETE_GNIRATOMIC_PREP_GET();                           \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        return gasnete_ratomic_nb(1, 1, sizeof(type), result_p,   \
                                  ad, tgt_rank, tgt_addr,         \
                                  cmd, op1, 0,                    \
                                  flags GASNETI_THREAD_PASS);     \
    } \
    GASNETI_INLINE(prefix##_NBI_N0)                               \
    int prefix##_NBI_N0(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
        gasneti_assert(gasneti_op_0arg(((gex_OP_t)1 << op_idx))); \
        _GASNETE_GNIRATOMIC_PREP_INC##isint(type, op_idx);        \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        return gasnete_ratomic_nbi(0, 1, sizeof(type), NULL,      \
                                   ad, tgt_rank, tgt_addr,        \
                                   cmd, inc, 0,                   \
                                   flags GASNETI_THREAD_PASS);    \
    } \
    GASNETI_INLINE(prefix##_NBI_N1)                               \
    int prefix##_NBI_N1(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,                          \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
        gasneti_assert(gasneti_op_1arg(((gex_OP_t)1 << op_idx))); \
        _GASNETE_GNIRATOMIC_PREP_NOP##isint(type, op_idx);        \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        return gasnete_ratomic_nbi(0, 1, sizeof(type), NULL,      \
                                   ad, tgt_rank, tgt_addr,        \
                                   cmd, op1, 0,                   \
                                   flags GASNETI_THREAD_PASS);    \
    } \
    GASNETI_INLINE(prefix##_NBI_N2)                               \
    int prefix##_NBI_N2(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,  type        operand2,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
        gasneti_assert(gasneti_op_2arg(((gex_OP_t)1 << op_idx))); \
        _GASNETE_GNIRATOMIC_PREP_CAS##isint(type);                \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        return gasnete_ratomic_nbi(0, 2, sizeof(type), NULL,      \
                                   ad, tgt_rank, tgt_addr,        \
                                   cmd, op1, op2,                 \
                                   flags GASNETI_THREAD_PASS);    \
    } \
    GASNETI_INLINE(prefix##_NBI_F0)                               \
    int prefix##_NBI_F0(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
        gasneti_assert(gasneti_op_0arg(((gex_OP_t)1 << op_idx))); \
        _GASNETE_GNIRATOMIC_PREP_FINC##isint(type, op_idx);       \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        return gasnete_ratomic_nbi(1, 1, sizeof(type), result_p,  \
                                   ad, tgt_rank, tgt_addr,        \
                                   cmd, inc, 0,                   \
                                   flags GASNETI_THREAD_PASS);    \
    } \
    GASNETI_INLINE(prefix##_NBI_F1)                               \
    int prefix##_NBI_F1(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type           *result_p,                         \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,                          \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
        gasneti_assert(gasneti_op_1arg(((gex_OP_t)1 << op_idx))); \
        _GASNETE_GNIRATOMIC_PREP_FOP##isint(type, op_idx);        \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        return gasnete_ratomic_nbi(1, 1, sizeof(type), result_p,  \
                                   ad, tgt_rank, tgt_addr,        \
                                   cmd, op1, 0,                   \
                                   flags GASNETI_THREAD_PASS);    \
    } \
    GASNETI_INLINE(prefix##_NBI_F2)                               \
    int prefix##_NBI_F2(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,  type        operand2,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
        gasneti_assert(gasneti_op_2arg(((gex_OP_t)1 << op_idx))); \
        _GASNETE_GNIRATOMIC_PREP_CAS##isint(type);                \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        return gasnete_ratomic_nbi(1, 2, sizeof(type), result_p,  \
                                   ad, tgt_rank, tgt_addr,        \
                                   cmd, op1, op2,                 \
                                   flags GASNETI_THREAD_PASS);    \
    } \
    GASNETI_INLINE(prefix##_NBI_S1)                               \
    int prefix##_NBI_S1(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,                          \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
        _GASNETE_GNIRATOMIC_PREP_SET##isint(type);                \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD; \
        GASNETC_DIDX_POST(mythread->domain_idx);                  \
        gasnete_iop_t * const iop = mythread->current_iop;        \
        gasneti_TM_t _i_tm = gasnete_ratomic_i_tm(ad, flags);     \
        return gasnete_ratomic_inner(0, 1, sizeof(type), NULL,    \
                                     _i_tm, tgt_rank, tgt_addr,   \
                                     cmd, val, 0,                 \
                                     GASNETE_IOP_CNTRS(iop, put), \
                                     flags GASNETC_DIDX_PASS);    \
    } \
    GASNETI_INLINE(prefix##_NBI_G0)                               \
    int prefix##_NBI_G0(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
        _GASNETE_GNIRATOMIC_PREP_GET();                           \
        gni_fma_cmd_type_t cmd = amo_cmd_map##dtcode(op_idx);     \
        gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD; \
        GASNETC_DIDX_POST(mythread->domain_idx);                  \
        gasnete_iop_t * const iop = mythread->current_iop;        \
        gasneti_TM_t _i_tm = gasnete_ratomic_i_tm(ad, flags);     \
        return gasnete_ratomic_inner(1, 1, sizeof(type), result_p,\
                                     _i_tm, tgt_rank, tgt_addr,   \
                                     cmd, op1, 0,                 \
                                     GASNETE_IOP_CNTRS(iop, get), \
                                     flags GASNETC_DIDX_PASS);    \
    }
//
// Operand wrangling
// GNI uses uint64_t to pass all atomic operands
//
// + non-fetching single-operand: need to negate operand for SUB
#define _GASNETE_GNIRATOMIC_PREP_NOP1(type, op_idx) /* integer */ \
        uint64_t op1 = (op_idx == gasneti_op_idx_SUB) ? -operand1 : operand1
#define _GASNETE_GNIRATOMIC_PREP_NOP0(type, op_idx) /* floating-point */ \
        union { type fp; uint64_t u64; } u1;                           \
        u1.fp = (op_idx == gasneti_op_idx_SUB) ? -operand1 : operand1; \
        uint64_t op1 = u1.u64
// + fetching single-operand: need to negate operand for FSUB
#define _GASNETE_GNIRATOMIC_PREP_FOP1(type, op_idx) /* integer */ \
        uint64_t op1 = (op_idx == gasneti_op_idx_FSUB) ? -operand1 : operand1
#define _GASNETE_GNIRATOMIC_PREP_FOP0(type, op_idx) /* floating-point */ \
        union { type fp; uint64_t u64; } u1;                           \
        u1.fp = (op_idx == gasneti_op_idx_FSUB) ? -operand1 : operand1; \
        uint64_t op1 = u1.u64
// + CAS
#define _GASNETE_GNIRATOMIC_PREP_CAS1(type) /* integer */ \
        uint64_t op1 = operand1; uint64_t op2 = operand2
#define _GASNETE_GNIRATOMIC_PREP_CAS0(type) /* floating-point */ \
        union { type fp; uint64_t u64; } u1, u2; \
        u1.fp = operand1; uint64_t op1 = u1.u64; \
        u2.fp = operand2; uint64_t op2 = u2.u64
// + SET via non-fetcing SWAP
#define _GASNETE_GNIRATOMIC_PREP_SET1(type) /* integer */ \
        uint64_t val = operand1
#define _GASNETE_GNIRATOMIC_PREP_SET0(type) /* floating-point */ \
        union { type fp; uint64_t u64; } u1;                \
        u1.fp = operand1; uint64_t val = u1.u64
// + GET via FAND(~0)
#define _GASNETE_GNIRATOMIC_PREP_GET() /* any type */ \
        uint64_t op1 = -1
// + non-fetching INC or DEC: must generate +/- 1 operand for ADD
#define _GASNETE_GNIRATOMIC_PREP_INC1(type, op_idx) /* integer */ \
        gasneti_assert((op_idx == gasneti_op_idx_INC) ||        \
                       (op_idx == gasneti_op_idx_DEC));         \
        uint64_t inc = (op_idx == gasneti_op_idx_INC) ? 1 : -1
#define _GASNETE_GNIRATOMIC_PREP_INC0(type, op_idx) /* floating-point */ \
        gasneti_assert((op_idx == gasneti_op_idx_INC) ||        \
                       (op_idx == gasneti_op_idx_DEC));         \
        union { type fp; uint64_t u64; } u1;                    \
        u1.fp = (op_idx == gasneti_op_idx_INC) ? 1. : -1.;      \
        uint64_t inc = u1.u64
// + fetching INC or DEC: must generate +/- 1 operand for FADD
#define _GASNETE_GNIRATOMIC_PREP_FINC1(type, op_idx) /* integer */ \
        gasneti_assert((op_idx == gasneti_op_idx_FINC) ||       \
                       (op_idx == gasneti_op_idx_FDEC));        \
        uint64_t inc = (op_idx == gasneti_op_idx_FINC) ? 1 : -1
#define _GASNETE_GNIRATOMIC_PREP_FINC0(type, op_idx) /* floating-point */ \
        gasneti_assert((op_idx == gasneti_op_idx_FINC) ||       \
                       (op_idx == gasneti_op_idx_FDEC));        \
        union { type fp; uint64_t u64; } u1;                    \
        u1.fp = (op_idx == gasneti_op_idx_FINC) ? 1. : -1.;     \
        uint64_t inc = u1.u64
//
GASNETE_DT_APPLY(GASNETE_GNIRATOMIC_MID)

//
// Ratomic functions (called by top-level dispatch functions) that then
// call the functions defined by the GASNETE_GNIRATOMIC_MID macro, above.
//
#define GASNETE_GNIRATOMIC_DEFS(dtcode) \
        _GASNETE_GNIRATOMIC_DEFS1(dtcode, dtcode##_isint)
// This extra pass expands the "isint" token prior to additional concatenation
#define _GASNETE_GNIRATOMIC_DEFS1(dtcode, isint) \
        _GASNETE_GNIRATOMIC_DEFS2(dtcode, isint)
#define _GASNETE_GNIRATOMIC_DEFS2(dtcode, isint) \
    _GASNETE_GNIRATOMIC_DEFN_INT##isint(dtcode,AND,1) \
    _GASNETE_GNIRATOMIC_DEFN_INT##isint(dtcode,OR,1)  \
    _GASNETE_GNIRATOMIC_DEFN_INT##isint(dtcode,XOR,1) \
    _GASNETE_GNIRATOMIC_DEFN2(dtcode,ADD,1)           \
    _GASNETE_GNIRATOMIC_DEFN2(dtcode,SUB,1)           \
    _GASNETE_GNIRATOMIC_DEFN2(dtcode,MULT,1)          \
    _GASNETE_GNIRATOMIC_DEFN2(dtcode,MIN,1)           \
    _GASNETE_GNIRATOMIC_DEFN2(dtcode,MAX,1)           \
    _GASNETE_GNIRATOMIC_DEFN2(dtcode,INC,0)           \
    _GASNETE_GNIRATOMIC_DEFN2(dtcode,DEC,0)           \
    _GASNETE_GNIRATOMIC_DEFN1(dtcode,SET,S1)          \
    _GASNETE_GNIRATOMIC_DEFN1(dtcode,GET,G0)          \
    _GASNETE_GNIRATOMIC_DEFN1(dtcode,SWAP,F1)         \
    _GASNETE_GNIRATOMIC_DEFN1(dtcode,FCAS,F2)         \
    _GASNETE_GNIRATOMIC_DEFN1(dtcode,CAS,N2)
//
#define _GASNETE_GNIRATOMIC_DEFN_INT0(dtcode,opname,nargs) /*empty*/
#define _GASNETE_GNIRATOMIC_DEFN_INT1 _GASNETE_GNIRATOMIC_DEFN2
#define _GASNETE_GNIRATOMIC_DEFN2(dtcode,opstem,nargs) \
        _GASNETE_GNIRATOMIC_DEFN1(dtcode,opstem,N##nargs) \
        _GASNETE_GNIRATOMIC_DEFN1(dtcode,F##opstem,F##nargs)
#define _GASNETE_GNIRATOMIC_DEFN1(dtcode,opname,args) \
  static gex_Event_t gasnete_gniratomic##dtcode##_NB_##opname(GASNETE_RATOMIC_ARGS_##args(dtcode##_type)) { \
    return gasnete_gniratomic##dtcode##_NB_##args(gasneti_op_idx_##opname,GASNETE_RATOMIC_PASS_##args);     \
  } \
  static int gasnete_gniratomic##dtcode##_NBI_##opname(GASNETE_RATOMIC_ARGS_##args(dtcode##_type)) {        \
    return gasnete_gniratomic##dtcode##_NBI_##args(gasneti_op_idx_##opname,GASNETE_RATOMIC_PASS_##args);    \
  }
#define GASNETE_RATOMIC_ARGS_S1 GASNETE_RATOMIC_ARGS_N1
#define GASNETE_RATOMIC_ARGS_G0 GASNETE_RATOMIC_ARGS_F0
#define GASNETE_RATOMIC_PASS_S1 GASNETE_RATOMIC_PASS_N1
#define GASNETE_RATOMIC_PASS_G0 GASNETE_RATOMIC_PASS_F0
//
GASNETE_DT_APPLY(GASNETE_GNIRATOMIC_DEFS)

//
// Build the dispatch tables
//
#define GASNETE_GNIRATOMIC_TBL(dtcode) \
    gasnete_ratomic##dtcode##_fn_tbl_t gasnete_gniratomic##dtcode##_fn_tbl = \
        GASNETE_RATOMIC_FN_TBL_INIT(gasnete_gniratomic##dtcode,dtcode);
GASNETE_DT_APPLY(GASNETE_GNIRATOMIC_TBL)

//
// Masks of missing capabilities
//
//   NO_MULT: Aries NIC has no atomic MULT operation
// NO_MINMAX: Aries NIC has no atomic MIN or MAX operation for UNSIGNED integer types
// NO_ADDSUB: Aries NIC has known[1] bad double-precision ADD (from which we build other ops)
//
#define GASNETE_GNIRATOMIC_NO_MULT   (GEX_OP_MULT | GEX_OP_FMULT)
#define GASNETE_GNIRATOMIC_NO_MINMAX (GEX_OP_MIN  | GEX_OP_FMIN | GEX_OP_MAX  | GEX_OP_FMAX)
#define GASNETE_GNIRATOMIC_NO_ADDSUB (GEX_OP_ADD  | GEX_OP_FADD | GEX_OP_SUB  | GEX_OP_FSUB | \
                                      GEX_OP_INC  | GEX_OP_FINC | GEX_OP_DEC  | GEX_OP_FDEC)
#define GASNETE_GNIRATOMIC_BADOPS_gex_dt_I32  GASNETE_GNIRATOMIC_NO_MULT
#define GASNETE_GNIRATOMIC_BADOPS_gex_dt_U32 (GASNETE_GNIRATOMIC_NO_MULT | GASNETE_GNIRATOMIC_NO_MINMAX)
#define GASNETE_GNIRATOMIC_BADOPS_gex_dt_I64  GASNETE_GNIRATOMIC_NO_MULT
#define GASNETE_GNIRATOMIC_BADOPS_gex_dt_U64 (GASNETE_GNIRATOMIC_NO_MULT | GASNETE_GNIRATOMIC_NO_MINMAX)
#define GASNETE_GNIRATOMIC_BADOPS_gex_dt_FLT  GASNETE_GNIRATOMIC_NO_MULT
#define GASNETE_GNIRATOMIC_BADOPS_gex_dt_DBL (GASNETE_GNIRATOMIC_NO_MULT | GASNETE_GNIRATOMIC_NO_ADDSUB)

//
// Create-hook to install the dispatch tables (aka algoritm selection)
//
void gasnete_gniratomic_create_hook(
        gasneti_AD_t               real_ad,
        gasneti_TM_t               real_tm,
        gex_DT_t                   dt,
        gex_OP_t                   ops,
        gex_Flags_t                flags)
{
    // Check for cases that should favor AM over NIC
    if (! (flags & GEX_FLAG_AD_FAVOR_REMOTE)) {
        if (flags & (GEX_FLAG_AD_FAVOR_MY_RANK | GEX_FLAG_AD_FAVOR_MY_NBRHD)) {
            // Client's flags favor AM-based atomics
            goto use_am;
        } else if (real_tm->_size == 1) {
            // Singleton team case favors AM-based
            goto use_am;
        }
    #if GASNET_PSHM
        // TODO-EX: this closed form does not generalize for multi-EP nor TM_Split
        else if (gasneti_mysupernode.node_count == gasneti_nodes) {
	    // Single-neighborhood case favors AM-based *if* the datatype is
	    // "tools safe" (and thus not actually using AM).  Otherwise, we
	    // will assume that the NIC is a better option since it does not
	    // rely on target attentiveness.
            #define GASNETE_GNIRATOMIC_TOOLS_CASE(dtcode) \
                case dtcode##_dtype:                    \
                    if (GASNETE_RATOMIC_PSHMSAFE##dtcode) goto use_am; \
                    break;
            switch (dt) {
                GASNETE_DT_APPLY(GASNETE_GNIRATOMIC_TOOLS_CASE)
                default: gasneti_unreachable();
            }
        }
    #endif
    }

    #define GASNETE_GNIRATOMIC_TBL_CASE(dtcode) \
        case dtcode##_dtype:                    \
            if (ops & GASNETE_GNIRATOMIC_BADOPS##dtcode) goto use_am; \
            real_ad->_fn_tbl = (gasnete_ratomic_fn_tbl_t) &gasnete_gniratomic##dtcode##_fn_tbl; \
            break;
    switch (dt) {
        GASNETE_DT_APPLY(GASNETE_GNIRATOMIC_TBL_CASE)
        default: gasneti_unreachable();
    }
    #undef GASNETE_GNIRATOMIC_TBL_CASE

    GASNETI_TRACE_PRINTF(C,("gex_AD_Create(dt=%d, ops=0x%x) -> Aries", (int)dt, (unsigned int)ops));
    real_ad->_tools_safe = 0;
    return;

use_am:
    gasnete_amratomic_create_hook(real_ad, real_tm, dt, ops, flags);
    return;
}

#endif // GASNETC_BUILD_GNIRATOMIC

/* ------------------------------------------------------------------------------------ */
// References:
// [1] libfabric-1.5.1:prov/gni/src/gnix_atomic.c
