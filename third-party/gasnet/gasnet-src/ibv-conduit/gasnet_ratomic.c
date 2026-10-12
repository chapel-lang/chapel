/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/gasnet_ratomic.c $
 * Description: GASNet Remote Atomics Implementation using IBV NIC offload
 * Copyright 2025, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#define GASNETI_NEED_GASNET_RATOMIC_H 1
#define GASNETI_NEED_GASNET_COLL_H 1
#include <gasnet_internal.h>

#if GASNETC_BUILD_IBVRATOMIC // Else entire file is empty

//
// GASNet-EX Remote Atomics via offload to InfiniBand HCA
//
// Conforming HCAs support FADD and FCAS on U64 directly.
// All the add/sub/inc/dec variants can be built for I64/U64 from FADD.
// GET is implemented via (build-time choice) FADD(0) or FCAS(0,0),
// either of which is a no-op.
//
// GET/CAS/FCAS for DBL are implemented via type-punning to U64.
//
// TODO-EX: In a multi-rail scenario, we are currently constrained to use a
// single QP for all AD Ops (for correctness).  Ideally, we'd use all QPs on
// one HCA.  However, in the multi-rail case we cannot (yet?) express "bind to
// any QP on a fixed HCA".
//
// TODO-EX: The constraint above could be relaxed, allowing use of all QPs on
// all HCAs, if they all report IBV_ATOMIC_GLOB.  The code for this has been
// written but is disabled due to lack of any suitable test system.
//
// TODO-EX: In a multi-rail scenario, if the first HCA reports IBV_ATOMIC_NONE,
// then we select AM-based atomics.  Ideally, we'd search for a more capable
// HCA before giving up.
//
// TODO-EX: There is an *untested* assumption that result byteorder for the
// HCAs in use in a given processes is single-valued.  However, this can only
// come into play for the *disabled* IBV_ATOMIC_GLOB-uses-multiple-HCAs case.
//
// TODO-EX: Add logic to avoid bounce-copy for in-segment *result_p?
// PRO: eliminates buffer alloc/free and 8-byte copy in critical paths.
// CON: arithmetic and branches for in-segment check and lkey lookup
// TBD: is this even a case that matters?
//
// TODO-EX: Currently the completion callbacks lack any distinction between
// fetching and non-fetching ops.  Consequently, non-fetching ops currently
// include an *unnecessary* REL fence (needed by fetching ops to ensure the
// write to *result_p is ordered before eop/iop completion.)
//
// TODO-EX: Current generation Mellanox HCAs include masked FADD and CAS
// as extensions to the standard atomics.  These should allow a straight-
// forward implementation of the existing OPs on the 32-bit types.
// Additionally, the following additional operations become possible:
//     SET via masked CAS:
//         `compare_mask = 0`, `swap_mask = ~1`, `newval = operand1`
//         This will unconditionally write the entire word.
//    SWAP via masked FCAS:
//         `compare_mask = 0`, `swap_mask = ~1`, `newval = operand1`
//         This will unconditionally swap the entire word.
//   (F)OR via masked (F)CAS:
//         `compare_mask = 0`, `swap_mask = operand1`, `newval = ~0`
//         This will unconditionally write the appropriate 1 bits.
//  (F)AND via masked (F)CAS:
//         `compare_mask = 0`, `swap_mask = ~operand1`, `newval = 0`
//         This will unconditionally write the appropriate 0 bits.
//  (F)XOR via masked (F)ADD:
//         `add_mask = ~0` to divide the word into single-bit fields
//         This will perform addition without carries (which is XOR).
// With appropriate adjustment to the masks, these apply to 32-bit as well.
// HOWEVER, it appears that NVIDIA/Mellanox no longer supports the experimental
// API version needed to access the masked atomics.

// Notes on implementation of GEX_FLAG_AD_{REL,ACQ}
//
// Because the IBV APIs are thread-safe, we believe that both injection and
// CQ polling contain sufficient memory fences.
//
// RELEASE:
// We believe that injection of IBV-level communication includes at least
// one release fence.
//
// ACQUIRE:
// We believe removing a CQ entry includes at least one acquire fence.

#include <gasnet_core_internal.h>
#include <gasnet_ratomic_internal.h>
#include <gasnet_event_internal.h>

GASNETI_IDENT(gasnetc_IdentString_IbvAtomicOffload, "$GASNetIbvAtomicOffload: 1 $");

//
// buffers for fetching ops
//

int gasnetc_ratomicbuf_limit;
gasnetc_lifo_head_t gasnetc_ratomicbuf_freelist = GASNETI_LIFO_INITIALIZER;

GASNETI_INLINE(gasnetc_get_ratomic_bbuf)
void *gasnetc_get_ratomic_bbuf(int block GASNETI_THREAD_FARG)
{
  GASNETC_STAT_EVENT(GET_RATOMICBUF);

  void *bbuf = gasnetc_lifo_pop(&gasnetc_ratomicbuf_freelist);
  if_pt (bbuf) {
    //done
  } else if (block) {
    GASNETC_TRACE_WAIT_BEGIN();
    GASNETI_SPIN_DOUNTIL(bbuf, {
        gasnetc_poll_snd();
        bbuf = gasnetc_lifo_pop(&gasnetc_ratomicbuf_freelist);
      });
    GASNETC_TRACE_WAIT_END(GET_RATOMICBUF_STALL);
  } else {
    gasnetc_poll_snd();
    bbuf = gasnetc_lifo_pop(&gasnetc_ratomicbuf_freelist);
  }
  gasneti_assert((bbuf != NULL) || !block);

  return bbuf;
}

//
// completion callbacks
//

extern void gasnetc_cb_eop_rmw(gasnetc_atomic_val_t *p) {
  gasnete_eop_t *eop = gasneti_container_of(p, gasnete_eop_t, initiated_cnt);
  gasnete_eop_check(eop);
  // sync_writes orders our caller's (optional) write of the fetched value prior to eop completion
  gasneti_sync_writes();
  // Non-atomic decrement is chaper than atomic increment of eop->completed_cnt.
  // Correct because (unlike Put and Get) there cannot be concurrency between
  // advancing this counter at AMO injection and the corresponding callback.
  (*p) -= 1;
  GASNETE_EOP_MARKDONE(eop);
}

extern void gasnetc_cb_iop_rmw(gasnetc_atomic_val_t *p) {
  gasnete_iop_t *iop = gasneti_container_of(p, gasnete_iop_t, initiated_rmw_cnt);
  gasnete_iop_check(iop);
  // REL fence orders our caller's (optional) write of the fetched value prior to iop completion
  GASNETE_IOP_CNT_FINISH(iop, rmw, 1, GASNETI_ATOMIC_REL);
}

enum {
  use_any_qp = -1, // gasnetc_bind_cep() can use any QP
  use_one_qp = 0,  // gasnetc_bind_cep() can use only qpi=0
};
#if (GASNETC_IB_MAX_HCAS == 1)
  #define gasnetc_ratomic_qpi use_any_qp
#else
  static int gasnetc_ratomic_qpi = use_one_qp;
#endif

//
// low-level OP injection
// Subject to specialization on 'opcode' and 'fetching' when inlined
//
GASNETI_INLINE(gasnete_ratomic_inner)
int gasnete_ratomic_inner(
                gasneti_TM_t i_tm, int fetching, void *result_p,
                gex_Rank_t tgt_rank, void *tgt_addr, gex_Flags_t flags,
                enum ibv_wr_opcode opcode, uint64_t operand1, uint64_t operand2,
                gasnetc_atomic_val_t *initiated_cnt, gasnetc_cb_t completion_cb
                GASNETI_THREAD_FARG)
{
  void *bbuf = NULL;
  if (fetching) {
    bbuf = gasnetc_get_ratomic_bbuf(!(flags & GEX_FLAG_IMMEDIATE) GASNETI_THREAD_PASS);
    if (!bbuf) return 1;
  }

  gasnetc_EP_t ep = (gasnetc_EP_t) i_tm->_ep;
  gex_Rank_t jobrank = gasnete_ratomic_jobrank(i_tm, tgt_rank, flags);
  gasnetc_epid_t epid = gasnetc_epid(jobrank, gasnetc_ratomic_qpi);
  const int rem_auxseg = gasneti_in_auxsegment(jobrank, tgt_addr, sizeof(uint64_t));

  gasnetc_sreq_t *sreq = gasnetc_get_sreq(GASNETC_OP_INVALID GASNETI_THREAD_PASS);
  gasnetc_cep_t *cep = gasnetc_bind_cep(ep, epid, sreq);

  GASNETC_DECL_SR_DESC(sr_desc, 1);

  sr_desc->wr.atomic.remote_addr = (uintptr_t)tgt_addr;
  sr_desc->wr.atomic.rkey = rem_auxseg ? cep->hca->aux_rkeys[jobrank]
                                       : GASNETC_SEG_RKEY(cep);
  sr_desc->num_sge = 1;
  sr_desc->sg_list[0].length = sizeof(uint64_t);

  if (!fetching) {
    gasneti_assert(! result_p);
    sreq->opcode = GASNETC_OP_ATOMIC;
    sr_desc->sg_list[0].addr = (uintptr_t)GASNETC_RATOMIC_SINK(cep);
  } else {
    gasneti_assert(result_p);
    gasneti_assert(bbuf);
    sreq->opcode = GASNETC_OP_ATOMIC_BOUNCE;
    sreq->amo_result = result_p;
    sreq->amo_bbuf = bbuf;
    sr_desc->sg_list[0].addr = (uintptr_t)sreq->amo_bbuf;
  }
  sr_desc->sg_list[0].lkey = cep->hca->aux_reg.handle->lkey;

  sr_desc->opcode = opcode;
  sr_desc->wr.atomic.compare_add = operand1;
  if (opcode == IBV_WR_ATOMIC_CMP_AND_SWP) {
    sr_desc->wr.atomic.swap = operand2;
  }

  sreq->comp.cb = completion_cb;
  sreq->comp.data = initiated_cnt;

  (*initiated_cnt) += 1;
  gasnetc_snd_post_common(sreq, sr_desc, 0, 0 GASNETI_THREAD_PASS);

  return 0;
}

GASNETI_INLINE(gasnete_ratomic_nb)
gex_Event_t gasnete_ratomic_nb(
                gasneti_TM_t i_tm, int fetching, void *result_p,
                gex_Rank_t tgt_rank, void *tgt_addr, gex_Flags_t flags,
                enum ibv_wr_opcode opcode, uint64_t operand1, uint64_t operand2
                GASNETI_THREAD_FARG)
{
  gasnete_eop_t *eop = gasnete_eop_new(GASNETI_MYTHREAD);
  int imm = gasnete_ratomic_inner(
                     i_tm, fetching, result_p, tgt_rank, tgt_addr,
                     flags, opcode, operand1, operand2,
                     &eop->initiated_cnt, gasnetc_cb_eop_rmw
                     GASNETI_THREAD_PASS);
  if (imm) {
    SET_EVENT_DONE(eop, 0);
    gasnete_eop_free(eop GASNETI_THREAD_PASS);
    return GEX_EVENT_NO_OP;
  }
  return (gex_Event_t)eop;
}

GASNETI_INLINE(gasnete_ratomic_nbi)
int gasnete_ratomic_nbi(
                gasneti_TM_t i_tm, int fetching, void *result_p,
                gex_Rank_t tgt_rank, void *tgt_addr, gex_Flags_t flags,
                enum ibv_wr_opcode opcode, uint64_t operand1, uint64_t operand2
                GASNETI_THREAD_FARG)
{
  gasnete_iop_t * const iop = GASNETI_MYTHREAD->current_iop;
  return gasnete_ratomic_inner(
                     i_tm, fetching, result_p, tgt_rank, tgt_addr,
                     flags, opcode, operand1, operand2,
                     &iop->initiated_rmw_cnt, gasnetc_cb_iop_rmw
                     GASNETI_THREAD_PASS);
}

//
// Mid-level (inline) functions marshalling arguments to atomic op injection
//
#define GASNETE_IBVRATOMIC_MID(dtcode) \
        _GASNETE_IBVRATOMIC_MID1(gasnete_ibvratomic##dtcode, dtcode##_type)
#define _GASNETE_IBVRATOMIC_MID1(prefix, type) \
  GASNETI_INLINE(prefix##_NB_Nadd) \
  gex_Event_t prefix##_NB_Nadd(type addend, GASNETE_RATOMIC_ARGS_N0(type))  \
  {                                                                         \
    gasneti_TM_t _i_tm = _real_ad->_tm;                                     \
    return gasnete_ratomic_nb (_i_tm, 0, NULL, _tgt_rank, _tgt_addr,        \
                               _flags, IBV_WR_ATOMIC_FETCH_AND_ADD,         \
                               addend, 0 GASNETI_THREAD_PASS);              \
  } \
  GASNETI_INLINE(prefix##_NBI_Nadd) \
  int prefix##_NBI_Nadd(type addend, GASNETE_RATOMIC_ARGS_N0(type))         \
  {                                                                         \
    gasneti_TM_t _i_tm = _real_ad->_tm;                                     \
    return gasnete_ratomic_nbi(_i_tm, 0, NULL, _tgt_rank, _tgt_addr,        \
                               _flags, IBV_WR_ATOMIC_FETCH_AND_ADD,         \
                               addend, 0 GASNETI_THREAD_PASS);              \
  } \
  \
  GASNETI_INLINE(prefix##_NB_Fadd) \
  gex_Event_t prefix##_NB_Fadd(type addend, GASNETE_RATOMIC_ARGS_F0(type))  \
  {                                                                         \
    gasneti_TM_t _i_tm = _real_ad->_tm;                                     \
    return gasnete_ratomic_nb (_i_tm, 1, _result_p, _tgt_rank, _tgt_addr,   \
                               _flags, IBV_WR_ATOMIC_FETCH_AND_ADD,         \
                               addend, 0 GASNETI_THREAD_PASS);              \
  } \
  GASNETI_INLINE(prefix##_NBI_Fadd) \
  int prefix##_NBI_Fadd(type addend, GASNETE_RATOMIC_ARGS_F0(type))         \
  {                                                                         \
    gasneti_TM_t _i_tm = _real_ad->_tm;                                     \
    return gasnete_ratomic_nbi(_i_tm, 1, _result_p, _tgt_rank, _tgt_addr,   \
                               _flags, IBV_WR_ATOMIC_FETCH_AND_ADD,         \
                               addend, 0 GASNETI_THREAD_PASS);              \
  } \
  \
  GASNETI_INLINE(prefix##_NB_Ncas) \
  gex_Event_t prefix##_NB_Ncas(GASNETE_RATOMIC_ARGS_N2(type))               \
  {                                                                         \
    gasneti_TM_t _i_tm = _real_ad->_tm;                                     \
    return gasnete_ratomic_nb (_i_tm, 0, NULL, _tgt_rank, _tgt_addr,        \
                               _flags, IBV_WR_ATOMIC_CMP_AND_SWP,           \
                               _operand1, _operand2 GASNETI_THREAD_PASS);   \
  } \
  GASNETI_INLINE(prefix##_NBI_Ncas) \
  int prefix##_NBI_Ncas(GASNETE_RATOMIC_ARGS_N2(type))                      \
  {                                                                         \
    gasneti_TM_t _i_tm = _real_ad->_tm;                                     \
    return gasnete_ratomic_nbi(_i_tm, 0, NULL, _tgt_rank, _tgt_addr,        \
                               _flags, IBV_WR_ATOMIC_CMP_AND_SWP,           \
                               _operand1, _operand2 GASNETI_THREAD_PASS);   \
  } \
  \
  GASNETI_INLINE(prefix##_NB_Fcas) \
  gex_Event_t prefix##_NB_Fcas(GASNETE_RATOMIC_ARGS_F2(type))               \
  {                                                                         \
    gasneti_TM_t _i_tm = _real_ad->_tm;                                     \
    return gasnete_ratomic_nb (_i_tm, 1, _result_p, _tgt_rank, _tgt_addr,   \
                               _flags, IBV_WR_ATOMIC_CMP_AND_SWP,           \
                               _operand1, _operand2 GASNETI_THREAD_PASS);   \
  } \
  GASNETI_INLINE(prefix##_NBI_Fcas) \
  int prefix##_NBI_Fcas(GASNETE_RATOMIC_ARGS_F2(type))                      \
  {                                                                         \
    gasneti_TM_t _i_tm = _real_ad->_tm;                                     \
    return gasnete_ratomic_nbi(_i_tm, 1, _result_p, _tgt_rank, _tgt_addr,   \
                               _flags, IBV_WR_ATOMIC_CMP_AND_SWP,           \
                               _operand1, _operand2 GASNETI_THREAD_PASS);   \
  }
//
GASNETE_IBVRATOMIC_MID(_gex_dt_U64)
GASNETE_IBVRATOMIC_MID(_gex_dt_I64)

// GET operations (type independent)
// These are use nop CAS or FADD as an atomic Get
//
// These do not just call gasnete_ratomic_{nb,nbi}, since at least NBI
// needs to use different counter and callback.

#ifdef GASNETC_RATOMIC_GET_OP
  // Preserve existing value
#else
  // Uncomment exactly one:
  //#define GASNETC_RATOMIC_GET_OP CMP_AND_SWP
  #define GASNETC_RATOMIC_GET_OP FETCH_AND_ADD
#endif
#define GASNETC_RATOMIC_GET_OPCODE _CONCAT(IBV_WR_ATOMIC_,GASNETC_RATOMIC_GET_OP)

static
gex_Event_t gasnete_ratomic_get_nb(
        gasneti_TM_t i_tm, void *result_p,
        gex_Rank_t tgt_rank, void *tgt_addr,
        size_t nbytes,
        gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasnete_eop_t *eop = gasnete_eop_new(GASNETI_MYTHREAD);
  int imm = gasnete_ratomic_inner(
                     i_tm, 1, result_p, tgt_rank, tgt_addr,
                     flags, GASNETC_RATOMIC_GET_OPCODE, 0, 0,
                     &eop->initiated_cnt, gasnetc_cb_eop_get
                     GASNETI_THREAD_PASS);
  if (imm) {
    SET_EVENT_DONE(eop, 0);
    gasnete_eop_free(eop GASNETI_THREAD_PASS);
    return GEX_EVENT_NO_OP;
  }
  return (gex_Event_t)eop;
}

static
int gasnete_ratomic_get_nbi(
        gasneti_TM_t i_tm, void *result_p,
        gex_Rank_t tgt_rank, void *tgt_addr,
        size_t nbytes,
        gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasnete_iop_t * const iop = GASNETI_MYTHREAD->current_iop;
  return gasnete_ratomic_inner(
                     i_tm, 1, result_p, tgt_rank, tgt_addr,
                     flags, GASNETC_RATOMIC_GET_OPCODE, 0, 0,
                     &iop->initiated_get_cnt,
                     iop->next ? gasnetc_cb_nar_get : gasnetc_cb_iop_get
                     GASNETI_THREAD_PASS);
}

//
// Ratomic OP functions (called by top-level dispatch functions) which
// call the GET or mid-level inline functions defined above.
//

// Accessors (GET)
#define _GASNETE_IBVRATOMIC_DEF_GET(dtcode) \
  static gex_Event_t gasnete_ibvratomic##dtcode##_NB_GET(GASNETE_RATOMIC_ARGS_F0(dtcode##_type)) { \
    return gasnete_ratomic_get_nb (_real_ad->_tm, _result_p, _tgt_rank, _tgt_addr,                 \
                                   sizeof(dtcode##_type), _flags GASNETI_THREAD_PASS);             \
  } \
  static int gasnete_ibvratomic##dtcode##_NBI_GET(GASNETE_RATOMIC_ARGS_F0(dtcode##_type)) {        \
    return gasnete_ratomic_get_nbi(_real_ad->_tm, _result_p, _tgt_rank, _tgt_addr,                 \
                                   sizeof(dtcode##_type), _flags GASNETI_THREAD_PASS);             \
  }
// (F)ADD operations
#define _GASNETE_IBVRATOMIC_DEF_ADD(dtcode,opstem,nargs,addend) \
        _GASNETE_IBVRATOMIC_DEF_ADD1(dtcode,   opstem,N##nargs,N,addend) \
        _GASNETE_IBVRATOMIC_DEF_ADD1(dtcode,F##opstem,F##nargs,F,addend)
#define _GASNETE_IBVRATOMIC_DEF_ADD1(dtcode,opname,args,fetching,addend) \
  static gex_Event_t gasnete_ibvratomic##dtcode##_NB_##opname(GASNETE_RATOMIC_ARGS_##args(dtcode##_type)) { \
    return gasnete_ibvratomic##dtcode##_NB_##fetching##add(addend,GASNETE_RATOMIC_PASS_##fetching##0);      \
  } \
  static int gasnete_ibvratomic##dtcode##_NBI_##opname(GASNETE_RATOMIC_ARGS_##args(dtcode##_type)) {        \
    return gasnete_ibvratomic##dtcode##_NBI_##fetching##add(addend,GASNETE_RATOMIC_PASS_##fetching##0);     \
  }
// (F)CAS operations on integer types
#define _GASNETE_IBVRATOMIC_DEF_CAS(dtcode) \
        _GASNETE_IBVRATOMIC_DEF_CAS1(dtcode, CAS,N2,N) \
        _GASNETE_IBVRATOMIC_DEF_CAS1(dtcode,FCAS,F2,F)
#define _GASNETE_IBVRATOMIC_DEF_CAS1(dtcode,opname,args,fetching) \
  static gex_Event_t gasnete_ibvratomic##dtcode##_NB_##opname(GASNETE_RATOMIC_ARGS_##args(dtcode##_type)) { \
    return gasnete_ibvratomic##dtcode##_NB_##fetching##cas(GASNETE_RATOMIC_PASS_##args);                    \
  } \
  static int gasnete_ibvratomic##dtcode##_NBI_##opname(GASNETE_RATOMIC_ARGS_##args(dtcode##_type)) {        \
    return gasnete_ibvratomic##dtcode##_NBI_##fetching##cas(GASNETE_RATOMIC_PASS_##args);                   \
  }
// (F)CAS operations on floating-point types
#define _GASNETE_IBVRATOMIC_DEF_FP_CAS(dtcode) \
        _GASNETE_IBVRATOMIC_DEF_FP_CAS1(dtcode, dtcode##_bits)
// This extra pass expands the "bits" token prior to additional concatenation
#define _GASNETE_IBVRATOMIC_DEF_FP_CAS1(dtcode, bits) \
        _GASNETE_IBVRATOMIC_DEF_FP_CAS2(dtcode, bits)
#define _GASNETE_IBVRATOMIC_DEF_FP_CAS2(dtcode, bits) \
  static gex_Event_t gasnete_ibvratomic##dtcode##_NB_CAS(GASNETE_RATOMIC_ARGS_N2(dtcode##_type)) {  \
    GASNETI_RATOMIC_UNION(bits) _op1; _op1._gex_fp = _operand1;                                     \
    GASNETI_RATOMIC_UNION(bits) _op2; _op2._gex_fp = _operand2;                                     \
    return gasnete_ibvratomic_gex_dt_U##bits##_NB_Ncas(                                             \
                    _real_ad, _tgt_rank, _tgt_addr,                                                 \
                    _op1._gex_ui, _op2._gex_ui, _flags GASNETI_THREAD_PASS);                        \
  } \
  static int gasnete_ibvratomic##dtcode##_NBI_CAS(GASNETE_RATOMIC_ARGS_N2(dtcode##_type)) {         \
    GASNETI_RATOMIC_UNION(bits) _op1; _op1._gex_fp = _operand1;                                     \
    GASNETI_RATOMIC_UNION(bits) _op2; _op2._gex_fp = _operand2;                                     \
    return gasnete_ibvratomic_gex_dt_U##bits##_NBI_Ncas(                                            \
                    _real_ad, _tgt_rank, _tgt_addr,                                                 \
                    _op1._gex_ui, _op2._gex_ui, _flags GASNETI_THREAD_PASS);                        \
  } \
  static gex_Event_t gasnete_ibvratomic##dtcode##_NB_FCAS(GASNETE_RATOMIC_ARGS_F2(dtcode##_type)) { \
    GASNETI_RATOMIC_UNION(bits) _op1; _op1._gex_fp = _operand1;                                     \
    GASNETI_RATOMIC_UNION(bits) _op2; _op2._gex_fp = _operand2;                                     \
    return gasnete_ibvratomic_gex_dt_U##bits##_NB_Fcas(                                             \
                    _real_ad, (uint##bits##_t *)_result_p, _tgt_rank, _tgt_addr,                    \
                    _op1._gex_ui, _op2._gex_ui, _flags GASNETI_THREAD_PASS);                        \
  } \
  static int gasnete_ibvratomic##dtcode##_NBI_FCAS(GASNETE_RATOMIC_ARGS_F2(dtcode##_type)) {        \
    GASNETI_RATOMIC_UNION(bits) _op1; _op1._gex_fp = _operand1;                                     \
    GASNETI_RATOMIC_UNION(bits) _op2; _op2._gex_fp = _operand2;                                     \
    return gasnete_ibvratomic_gex_dt_U##bits##_NBI_Fcas(                                            \
                    _real_ad, (uint##bits##_t *)_result_p, _tgt_rank, _tgt_addr,                    \
                    _op1._gex_ui, _op2._gex_ui, _flags GASNETI_THREAD_PASS);                        \
  }
// Unreachable functions for non-offloadable ops
#define _GASNETE_IBVRATOMIC_BAD2(dtcode,opstem,nargs) \
        _GASNETE_IBVRATOMIC_BAD1(dtcode,opstem,N##nargs) \
        _GASNETE_IBVRATOMIC_BAD1(dtcode,F##opstem,F##nargs)
#define _GASNETE_IBVRATOMIC_BAD1(dtcode,opname,args) \
  static gex_Event_t gasnete_ibvratomic##dtcode##_NB_##opname(GASNETE_RATOMIC_ARGS_##args(dtcode##_type)) { \
    gasneti_unreachable_error(("Invalid offload of gex_AD_OpNB_" dtcode##_string "(GEX_OP_" #opname ")"));  \
    return GEX_EVENT_INVALID;                                                                               \
  } \
  static int gasnete_ibvratomic##dtcode##_NBI_##opname(GASNETE_RATOMIC_ARGS_##args(dtcode##_type)) {        \
    gasneti_unreachable_error(("Invalid offload of gex_AD_OpNBI_" dtcode##_string "(GEX_OP_" #opname ")")); \
    return 0;                                                                                               \
  }

#define GASNETE_IBVRATOMIC_INT_DEFS(dtcode) \
  _GASNETE_IBVRATOMIC_DEF_GET(dtcode)       \
  \
  _GASNETE_IBVRATOMIC_DEF_ADD(dtcode,ADD,1,_operand1)  \
  _GASNETE_IBVRATOMIC_DEF_ADD(dtcode,SUB,1,-_operand1) \
  _GASNETE_IBVRATOMIC_DEF_ADD(dtcode,INC,0,1)          \
  _GASNETE_IBVRATOMIC_DEF_ADD(dtcode,DEC,0,-1)         \
  \
  _GASNETE_IBVRATOMIC_DEF_CAS(dtcode)       \
  \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,AND,1)    \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,OR,1)     \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,XOR,1)    \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,MULT,1)   \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,MIN,1)    \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,MAX,1)    \
  _GASNETE_IBVRATOMIC_BAD1(dtcode,SET,N1)   \
  _GASNETE_IBVRATOMIC_BAD1(dtcode,SWAP,F1)
//
GASNETE_IBVRATOMIC_INT_DEFS(_gex_dt_U64)
GASNETE_IBVRATOMIC_INT_DEFS(_gex_dt_I64)

#define GASNETE_IBVRATOMIC_FP_DEFS(dtcode)  \
  _GASNETE_IBVRATOMIC_DEF_GET(dtcode)       \
  \
  _GASNETE_IBVRATOMIC_DEF_FP_CAS(dtcode)    \
  \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,ADD,1)    \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,SUB,1)    \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,INC,0)    \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,DEC,0)    \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,AND,1)    \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,OR,1)     \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,XOR,1)    \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,MULT,1)   \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,MIN,1)    \
  _GASNETE_IBVRATOMIC_BAD2(dtcode,MAX,1)    \
  _GASNETE_IBVRATOMIC_BAD1(dtcode,SET,N1)   \
  _GASNETE_IBVRATOMIC_BAD1(dtcode,SWAP,F1)
//
GASNETE_IBVRATOMIC_FP_DEFS(_gex_dt_DBL)

//
// Build the dispatch tables
//
#define GASNETE_IBVRATOMIC_TBL(dtcode) \
    gasnete_ratomic##dtcode##_fn_tbl_t gasnete_ibvratomic##dtcode##_fn_tbl = \
        GASNETE_RATOMIC_FN_TBL_INIT(gasnete_ibvratomic##dtcode,dtcode);
GASNETE_IBVRATOMIC_TBL(_gex_dt_U64)
GASNETE_IBVRATOMIC_TBL(_gex_dt_I64)
GASNETE_IBVRATOMIC_TBL(_gex_dt_DBL)

//
// Masks of available capabilities
//
#define GASNETE_IBVRATOMIC_BASE_OPS ( GEX_OP_CAS | GEX_OP_FCAS | GEX_OP_GET ) // Note lack of SET
#define GASNETE_IBVRATOMIC_FADD_OPS ( GEX_OP_ADD | GEX_OP_FADD | GEX_OP_SUB | GEX_OP_FSUB | \
                                      GEX_OP_INC | GEX_OP_FINC | GEX_OP_DEC | GEX_OP_FDEC )
#define GASNETE_IBVRATOMIC_TYPES ( GEX_DT_I64 | GEX_DT_U64 | GEX_DT_DBL )

// HCA's hca_cap.atomic_cap
enum {
    atomic_cap_uninitialized,
    atomic_cap_none, // Use AMs
    atomic_cap_hca,
    atomic_cap_glob
};
static int gasnetc_atomic_cap = atomic_cap_uninitialized;

//
// Init-hook to install the dispatch tables (aka algorithm selection)
//
void gasnete_ibvratomic_init_hook(gasneti_AD_t real_ad)
{
    const char *msg = "";
    gex_Flags_t flags = real_ad->_flags;
    gasneti_TM_t real_tm = real_ad->_tm;
    gex_DT_t dt = real_ad->_dt;
    gex_OP_t ops = real_ad->_ops;

    // Check for unsupported dt or op(s)
    if (dt  & ~GASNETE_IBVRATOMIC_TYPES) {
      msg = "unsupported datatype";
      goto use_am;
    }
    gex_OP_t avail_ops = GASNETE_IBVRATOMIC_BASE_OPS;
    if (gasneti_dt_int(dt)) avail_ops |= GASNETE_IBVRATOMIC_FADD_OPS;
    if (ops & ~avail_ops) {
      msg = "unsupported operation(s) on a supported datatype";
      goto use_am;
    }

    // Check for singleton, which is a pain to support for no clear benefit
    if (real_tm->_size == 1) {
      msg = "singleton team";
      goto use_am;
    }

    // Check for supported cases that should favor AM over NIC
    if (flags & GEX_FLAG_AD_FAVOR_REMOTE) {
        // Client's flags want offload
    } else if (gasnetc_atomic_cap == atomic_cap_glob) {
        // HCA can interoperate with tools-based atomics w/i RANK and NBRHD
    } else if (flags & (GEX_FLAG_AD_FAVOR_MY_RANK | GEX_FLAG_AD_FAVOR_MY_NBRHD)) {
        // Client's flags favor AM-based atomics
        msg = "GEX_FLAG_AD_FAVOR_* flags(s)";
        goto use_am;
    }
    #if GASNET_PSHM
        // TODO-EX: this closed form does not generalize for multi-EP nor TM_Split
        else if (gasneti_mysupernode.node_count == gasneti_nodes) {
            // Single-neighborhood case favors AM-based *if* the datatype is
            // "tools safe" (and thus not actually using AM).  Otherwise, we
            // will assume that the NIC is a better option since it does not
            // rely on target attentiveness.
            msg = "enabling shared-memory optimization";
            switch (dt) {
                case GEX_DT_U64:
                    if (GASNETE_RATOMIC_PSHMSAFE_gex_dt_U64) goto use_am;
                    break;
                case GEX_DT_I64:
                    if (GASNETE_RATOMIC_PSHMSAFE_gex_dt_I64) goto use_am;
                    break;
                case GEX_DT_DBL:
                    if (GASNETE_RATOMIC_PSHMSAFE_gex_dt_DBL) goto use_am;
                    break;
                default:
                    gasneti_unreachable_error(("unknown data type %d", dt));
            }
        }
    #endif

    switch (gasnetc_atomic_cap) {
        case atomic_cap_none:
            msg = "missing HCA support";
            goto use_am;

        case atomic_cap_hca:
            // HCA-initiated atomics may use separate PCI read and write transactions, and
            // thus are not guaranteed to preserve atomic coherence wrt CPU AMO instructions
            real_ad->_tools_safe = 0;
            break;

        case atomic_cap_glob:
            real_ad->_tools_safe = 1;
            break;

        default: gasneti_unreachable_error(("invalid atomic_cap"));
    }

    switch (dt) {
        case GEX_DT_U64:
            real_ad->_fn_tbl = (gasnete_ratomic_fn_tbl_t)&gasnete_ibvratomic_gex_dt_U64_fn_tbl;
            break;
        case GEX_DT_I64:
            real_ad->_fn_tbl = (gasnete_ratomic_fn_tbl_t)&gasnete_ibvratomic_gex_dt_I64_fn_tbl;
            break;
        case GEX_DT_DBL:
            real_ad->_fn_tbl = (gasnete_ratomic_fn_tbl_t)&gasnete_ibvratomic_gex_dt_DBL_fn_tbl;
            break;
        default:
            gasneti_unreachable_error(("unknown data type %d", dt));
    }

    real_ad->_is_ref = 0;
    return;

use_am:
    GASNETI_TRACE_PRINTF(O,("gex_AD_Create(dt=%d, ops=0x%x): %s", (int)dt, (unsigned int)ops, msg));
    gasnete_amratomic_init_hook(real_ad);
    return;
}

// Subsystem initialization
//  1) query/log HCA atomic capabilty
//  2) detect byteorder of FADD and FCAS results
//  3) set gasnetc_ratomic_qpi
int gasnetc_ratomic_init(gasnetc_EP_t ep0) {
  GASNET_BEGIN_FUNCTION(); // OK - not a critical-path

  gasneti_TM_t i_tm = ep0->_client->_tm0;
  gex_TM_t e_tm = gasneti_export_tm(i_tm);

  //
  // 1) check and log just the first HCA's reported atomics capability
  //

  gasneti_assert_int(gasnetc_atomic_cap ,==, atomic_cap_uninitialized);

  // quoted enum descriptions are from https://www.rdmamojo.com/2012/07/13/ibv_query_device/
  switch (gasnetc_hca[0].hca_cap.atomic_cap) {
    case IBV_ATOMIC_NONE:
      // "Atomic operations aren't supported at all"
      GASNETI_TRACE_PRINTF(I,("gex_AD: HCA does not support atomics"));
      gasnetc_atomic_cap = atomic_cap_none;
      break;

    case IBV_ATOMIC_HCA:
      // "Atomicity is guaranteed between QPs on this device only"
      GASNETI_TRACE_PRINTF(I,("gex_AD: HCA supports device-scope atomicity"));
      gasnetc_atomic_cap = atomic_cap_hca;
      break;

    case IBV_ATOMIC_GLOB:
      // "Atomicity is guaranteed between this device and any other component,
      // such as CPUs, IO devices and other RDMA devices"
      GASNETI_TRACE_PRINTF(I,("gex_AD: HCA supports global-scope atomicity"));
      gasnetc_atomic_cap = atomic_cap_glob;
      break;

    default:
      gasneti_unreachable_error(("gex_AD: unknown hca_cap.atomic_cap value %i",
                                (int)gasnetc_hca[0].hca_cap.atomic_cap));
  }

  // Reduction to find global min
  if (gasneti_nodes > 1) {
    int32_t global_cap;
    int32_t local_cap = (int32_t) gasnetc_atomic_cap;
    gex_Event_Wait(
      gex_Coll_ReduceToAllNB(e_tm, &global_cap, &local_cap,
                             GEX_DT_I32, sizeof(int32_t), 1,
                             GEX_OP_MIN, NULL, NULL, 0));
    if (global_cap != local_cap) {
      gasnetc_atomic_cap = global_cap;
      switch (gasnetc_atomic_cap) {
        case atomic_cap_none:
          GASNETI_TRACE_PRINTF(I,("gex_AD: at least one process lacks atomics support"));
          break;

        case atomic_cap_hca:
          GASNETI_TRACE_PRINTF(I,("gex_AD: at least one process supports only HCA-scope atomicity"));
          break;

        default:
          gasneti_unreachable_error(("gex_AD: impossible atomic cap reduction output"));
      }
    }
  }

  // Stop here if no support
  if (gasnetc_atomic_cap == atomic_cap_none) {
    GASNETI_TRACE_PRINTF(I,("gex_AD: disabled due to lack of HCA support"));
    return GASNET_OK;
  }

  //
  // 2) Check byteorder of FADD and FCAS results
  //

  // A location in aux segment we can use as needed
  uint64_t *tmp = (uint64_t *)GASNETC_RATOMIC_SINK(cep);

  // Two back-to-back loop-back FADD(1) operations:
  uint64_t result[2];
  for (int i = 0; i < 2; ++i) {
    gex_Event_Wait(
      gasnete_ratomic_nb(i_tm, 1, &result[i], gasneti_mynode, tmp,
                         0, IBV_WR_ATOMIC_FETCH_AND_ADD,
                         1, 0 GASNETI_THREAD_PASS));
  }

  // Check the two results to determine byteorder
  gasnetc_hca_t *hca = &gasnetc_hca[0];
  hca->amo_bswap = (result[1] - result[0]) != 1;
  GASNETI_TRACE_PRINTF(I,("gex_AD: amo results are %sin host byteorder",
                          hca->amo_bswap ? "not " : ""));

#if GASNET_DEBUG
  // Try again to be sure
  for (int i = 0; i < 2; ++i) {
    gex_Event_Wait(
      gasnete_ratomic_nb(i_tm, 1, &result[i], gasneti_mynode, tmp,
                         0, IBV_WR_ATOMIC_FETCH_AND_ADD,
                         1, 0 GASNETI_THREAD_PASS));
  }
  gasneti_assert_uint(result[1] - result[0] ,==, 1);
#endif

  //
  // 3) set gasnetc_ratomic_qpi
  // Controls whether AD Ops will be issued on a single QP or striped over all of them.
  //

#if (GASNETC_IB_MAX_HCAS > 1)
  switch (gasnetc_atomic_cap) {
    case atomic_cap_glob:
    #if 0 // TODO: enable if/when a (multi-rail + IBV_ATOMIC_GLOB) platform can be tested
    {
      // It is safe to use all QPs if and only if all HCAs report IBV_ATOMIC_GLOB
      gasnetc_ratomic_qpi = use_any_qp;
      gasnetc_hca_t *hca;
      GASNETC_FOR_ALL_HCA(hca) {
        if (hca->hca_cap.atomic_cap != IBV_ATOMIC_GLOB) {
          gasnetc_ratomic_qpi = use_one_qp;
          break;
        } else {
          // TODO: replace this untested homogeneity assumption with a real probe
          hca->amo_bswap = gasnetc_hca[0]->amo_bswap;
        }
      }
      break;
    }
    #else
      // fall-though
    #endif

    case atomic_cap_hca:
      // It is safe to use all QPs if and only if using a single HCA
      gasnetc_ratomic_qpi = (gasnetc_num_hcas == 1) ? use_any_qp : use_one_qp;
      break;

    default:
      gasneti_unreachable_error(("gex_AD: impossible atomic cap value"));
  }
#else
  gasneti_static_assert(gasnetc_ratomic_qpi == use_any_qp);
#endif

  GASNETI_TRACE_PRINTF(I,("gex_AD: using %s",
                          (gasnetc_ratomic_qpi == use_any_qp)?"all QPs":"one QP"));

  return GASNET_OK;
}

#endif // GASNETC_BUILD_IBVRATOMIC
