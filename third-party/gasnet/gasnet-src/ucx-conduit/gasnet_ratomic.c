/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ucx-conduit/gasnet_ratomic.c $
 * Description: GASNet Remote Atomics API Header (forward decls)
 * Copyright 2019, Mellanox Technologies, Inc. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#define GASNETI_NEED_GASNET_RATOMIC_H 1
#include <gasnet_internal.h>
#include <gasnet_ratomic.h>
#include <gasnet_core_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_ratomic_internal.h>
#include "gasnet_ucx_req.h"

#define GASNETE_DT_INT_APPLY(FN) \
        FN(_gex_dt_I32) FN(_gex_dt_U32) \
        FN(_gex_dt_I64) FN(_gex_dt_U64)

// + fetching INC or DEC: must generate +/- 1 operand for FADD
#define GASNETE_UCX_RATOMIC_PREP_FINC(op_idx)                     \
        gasneti_assert((op_idx == gasneti_op_idx_FINC) ||         \
                       (op_idx == gasneti_op_idx_FDEC));          \
        uint64_t inc = (op_idx == gasneti_op_idx_FINC) ? 1 : -1

#define GASNETE_UCX_RATOMIC_PREP_INC(op_idx)                      \
        gasneti_assert((op_idx == gasneti_op_idx_INC) ||          \
                       (op_idx == gasneti_op_idx_DEC));           \
        uint64_t inc = (op_idx == gasneti_op_idx_INC) ? 1 : -1

// + fetching single-operand: need to negate operand for FSUB
#define GASNETE_UCX_RATOMIC_PREP_FOP(op_idx) /* integer */        \
        uint64_t op1 = (op_idx == gasneti_op_idx_FSUB) ? -operand1 : operand1

#define GASNETE_UCX_RATOMIC_PREP_OP(op_idx) /* integer */        \
        uint64_t op1 = (op_idx == gasneti_op_idx_SUB) ? -operand1 : operand1

GASNETI_INLINE(amo_fetch_op_map_gex_dt_I32)
ucp_atomic_fetch_op_t amo_fetch_op_map_gex_dt_I32(gasneti_op_idx_t op_idx) {
  switch (op_idx) {
    case gasneti_op_idx_FADD:   return UCP_ATOMIC_FETCH_OP_FADD; /* Atomic Fetch and add    */
    case gasneti_op_idx_GET:    return UCP_ATOMIC_FETCH_OP_FADD; /* Atomic Fetch and add 0  */
    case gasneti_op_idx_SET:    return UCP_ATOMIC_FETCH_OP_SWAP; /* Atomic swap, ignore fetch */
    case gasneti_op_idx_FAND:   return UCP_ATOMIC_FETCH_OP_FAND; /* Atomic Fetch and and    */
    case gasneti_op_idx_FSUB:   return UCP_ATOMIC_FETCH_OP_FADD; /* Atomic Fetch and add  x */
    case gasneti_op_idx_SWAP:   return UCP_ATOMIC_FETCH_OP_SWAP; /* Atomic swap */
    case gasneti_op_idx_FCAS:   return UCP_ATOMIC_FETCH_OP_CSWAP;/* Atomic conditional swap */
    case gasneti_op_idx_FOR:    return UCP_ATOMIC_FETCH_OP_FOR;  /* Atomic Fetch and or     */
    case gasneti_op_idx_FXOR:   return UCP_ATOMIC_FETCH_OP_FXOR; /* Atomic Fetch and xor    */
    case gasneti_op_idx_FINC:   return UCP_ATOMIC_FETCH_OP_FADD; /* Atomic Fetch and add 1  */
    case gasneti_op_idx_FDEC:   return UCP_ATOMIC_FETCH_OP_FADD; /* Atomic Fetch and add -1 */
    case gasneti_op_idx_CAS:    return UCP_ATOMIC_FETCH_OP_CSWAP;/* Atomic conditional swap, ignore fetch */
    default: gasneti_fatalerror("unknown AMO operation");
  }
  return UCP_ATOMIC_FETCH_OP_LAST;
}

GASNETI_INLINE(amo_post_op_map_gex_dt_I32)
ucp_atomic_fetch_op_t amo_post_op_map_gex_dt_I32(gasneti_op_idx_t op_idx) {
  switch (op_idx) {
    case gasneti_op_idx_ADD:    return UCP_ATOMIC_POST_OP_ADD;
    case gasneti_op_idx_SUB:    return UCP_ATOMIC_POST_OP_ADD;
    case gasneti_op_idx_AND:    return UCP_ATOMIC_POST_OP_AND;
    case gasneti_op_idx_OR:     return UCP_ATOMIC_POST_OP_OR;
    case gasneti_op_idx_XOR:    return UCP_ATOMIC_POST_OP_XOR;
    case gasneti_op_idx_INC:    return UCP_ATOMIC_POST_OP_ADD;
    case gasneti_op_idx_DEC:    return UCP_ATOMIC_POST_OP_ADD;
    default: gasneti_fatalerror("unknown AMO operation");
  }
  return UCP_ATOMIC_POST_OP_LAST;
}

#define amo_fetch_op_map_gex_dt_U32 amo_fetch_op_map_gex_dt_I32
#define amo_fetch_op_map_gex_dt_I64 amo_fetch_op_map_gex_dt_I32
#define amo_fetch_op_map_gex_dt_U64 amo_fetch_op_map_gex_dt_I32

#define amo_post_op_map_gex_dt_U32 amo_post_op_map_gex_dt_I32
#define amo_post_op_map_gex_dt_I64 amo_post_op_map_gex_dt_I32
#define amo_post_op_map_gex_dt_U64 amo_post_op_map_gex_dt_I32

GASNETI_INLINE(gasnete_ratomic_jobrank)
gex_Rank_t gasnete_ratomic_jobrank(gasneti_TM_t i_tm, gex_Rank_t tgt_rank,
                                   gex_Flags_t flags)
{
  if (flags & GEX_FLAG_RANK_IS_JOBRANK) {
    gasneti_assert(GEX_RANK_INVALID !=
        gasneti_i_tm_jobrank_to_rank(i_tm, tgt_rank));
    return tgt_rank;
  } else {
    return gasneti_i_tm_rank_to_jobrank(i_tm, tgt_rank);
  }
}

static
void gasnetc_ucx_amo_cb(void *request, ucs_status_t status)
{
  gasnetc_ucx_request_t *req = (gasnetc_ucx_request_t*) request;

  if (status != UCS_OK) {
    gasneti_fatalerror("UCX AMO operation failed: %s",
                       ucs_status_string(status));
  }
  if (req->completion.cbfunc) {
    req->completion.cbfunc(req->completion.cbdata);
  }
  req->completion.cbfunc = NULL;
  req->completion.cbdata = NULL;
  ucp_request_free(request);
  return;
}

GASNETI_INLINE(gasnete_ratomic_fetch_inner)
int gasnete_ratomic_fetch_inner(gex_Rank_t rank, const int length,
                                void *tgt_addr, ucp_atomic_fetch_op_t op,
                                int op_cnt, void *result_p,
                                uint64_t operand1, uint64_t operand2,
                                gasnetc_atomic_val_t *initiated_cnt,
                                gasnetc_cbfunc_t complete_cb)
{
  gasnetc_ucx_request_t *req;
  ucp_ep_h ep = GASNETC_UCX_GET_EP(rank);
  gasnetc_mem_info_t *mem_info;
  int result_free = 0;

  gasneti_assert((8 == length) || (4 == length));

  mem_info = gasnetc_find_mem_info(tgt_addr, length, rank);
  if (NULL == mem_info) {
    gasneti_fatalerror("UCX atomic operation failed: rkey cannot be found");
  }
  if (NULL == result_p) {
    result_p = gasneti_malloc(length);
    result_free = 1;
  }
  if (op_cnt == 2) {
    if (4 == length) {
      uint32_t *tmp = result_p;
      *tmp = operand2;
    } else if (8 == length) {
      uint64_t *tmp = result_p;
      *tmp = operand2;
    }
  }
  (*initiated_cnt)++;
  req = (gasnetc_ucx_request_t *)
      ucp_atomic_fetch_nb(ep, op, operand1, result_p, length,
                          (uint64_t)tgt_addr, mem_info->rkey,
                          gasnetc_ucx_amo_cb);
  if (NULL == req) {
    /* completed inplace */
    if (result_free) {
      gasneti_free(result_p);
    }
    if (complete_cb) {
      complete_cb(initiated_cnt);
    }
    return 1;
  }
  if_pf (UCS_PTR_IS_ERR(req)) {
    gasneti_fatalerror("UCX atomic operation failed: %s",
                       ucs_status_string(UCS_PTR_STATUS(req)));
  }
  req->completion.cbdata = initiated_cnt;
  req->completion.cbfunc = complete_cb;
  req->result_p = result_free ? result_p : NULL;

  return 0;
}

// NB-specific wrapper around gasnete_ratomic_inner()
GASNETI_INLINE(gasnete_ratomic_fetch_nb)
gex_Event_t gasnete_ratomic_fetch_nb(const int length, void *result_p,
                                     gasneti_TM_t i_tm, gex_Rank_t tgt_rank,
                                     void *tgt_addr, ucp_atomic_fetch_op_t op,
                                     int op_cnt, uint64_t operand1, uint64_t operand2,
                                     gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasnete_eop_t * const eop = gasnete_eop_new(GASNETI_MYTHREAD);
  gex_Rank_t rank = gasnete_ratomic_jobrank(i_tm, tgt_rank, flags);
  ucp_ep_h ep = GASNETC_UCX_GET_EP(rank);

  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  // TODO: add support GEX_FLAG_IMMEDIATE flag
  gasnete_ratomic_fetch_inner(rank, length, tgt_addr, op, op_cnt,
                              result_p, operand1, operand2,
                              &eop->initiated_cnt, gasnetc_cb_eop_get);
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);

  return (gex_Event_t) eop;
}

int gasnete_ratomic_fetch_nbi(const int length, void *result_p,
                              gasneti_TM_t i_tm, gex_Rank_t tgt_rank,
                              void *tgt_addr, ucp_atomic_fetch_op_t op,
                              int op_cnt, uint64_t operand1, uint64_t operand2,
                              gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t * const iop = mythread->current_iop;
  gex_Rank_t rank = gasnete_ratomic_jobrank(i_tm, tgt_rank, flags);

  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  // TODO: add support GEX_FLAG_IMMEDIATE flag
  gasnete_ratomic_fetch_inner(rank, length, tgt_addr, op, op_cnt, result_p,
                              operand1, operand2, &iop->initiated_rmw_cnt,
                              iop->next ? gasnetc_cb_nar_rmw : gasnetc_cb_iop_rmw);
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);
  return 0;
}

GASNETI_INLINE(gasnete_ratomic_post_inner)
int gasnete_ratomic_post_inner(const int length,
                                gasneti_TM_t i_tm, gex_Rank_t tgt_rank,
                                void *tgt_addr, ucp_atomic_fetch_op_t op,
                                uint64_t operand, gasnetc_atomic_val_t *cnt,
                                gasnetc_cbfunc_t cbfunc, gex_Flags_t flags)
{
  gex_Rank_t rank = gasnete_ratomic_jobrank(i_tm, tgt_rank, flags);
  gasnetc_mem_info_t *mem_info;
  ucp_ep_h ep = GASNETC_UCX_GET_EP(rank);
  gasnetc_ucx_request_t *req;
  ucs_status_t status;
  int immediate = 0;

  gasneti_assert((8 == length) || (4 == length));

  mem_info = gasnetc_find_mem_info(tgt_addr, length, rank);
  if (NULL == mem_info) {
    gasneti_fatalerror("atomic post operation failed: rkey cannot be found");
  }
  status = ucp_atomic_post(ep, op, operand, length, (uint64_t)tgt_addr,
                           mem_info->rkey);
  if_pf (UCS_OK != status) {
      gasneti_fatalerror("atomic post operation failed: %s",
                         ucs_status_string(UCS_PTR_STATUS(status)));
  }

  if_pt (cnt) {
    (*cnt)++;
    req = ucp_ep_flush_nb(ep, 0, gasnetc_ucx_amo_cb);
    if (req == NULL) {
      cbfunc(cnt);
      immediate = 1;
    } else {
      req->completion.cbdata = cnt;
      req->completion.cbfunc = cbfunc;
    }
  }
  return immediate;
}

GASNETI_INLINE(gasnete_ratomic_post_nb)
gex_Event_t gasnete_ratomic_post_nb(const int length,
                                    gasneti_TM_t i_tm, gex_Rank_t tgt_rank,
                                    void *tgt_addr, ucp_atomic_fetch_op_t op,
                                    uint64_t operand,
                                    gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasnete_eop_t * const eop = gasnete_eop_new(GASNETI_MYTHREAD);

  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  // TODO: add support GEX_FLAG_IMMEDIATE flag
  gasnete_ratomic_post_inner(length, i_tm, tgt_rank, tgt_addr, op,
                             operand, &eop->initiated_cnt,
                             gasnetc_cb_eop_put, flags);
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);
  return (gex_Event_t) eop;
}

GASNETI_INLINE(gasnete_ratomic_post_nbi)
int gasnete_ratomic_post_nbi(const int length,
                             gasneti_TM_t i_tm, gex_Rank_t tgt_rank,
                             void *tgt_addr, ucp_atomic_fetch_op_t op,
                             uint64_t operand,
                             gex_Flags_t flags GASNETI_THREAD_FARG)
{
  gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
  gasnete_iop_t * const iop = mythread->current_iop;

  GASNETC_LOCK_ACQUIRE(GASNETC_LOCK_REGULAR);
  // TODO: add support GEX_FLAG_IMMEDIATE flag
  gasnete_ratomic_post_inner(length, i_tm, tgt_rank, tgt_addr, op,
                             operand, &iop->initiated_rmw_cnt,
                             iop->next ? gasnetc_cb_nar_rmw : gasnetc_cb_iop_rmw,
                             flags);
  GASNETC_LOCK_RELEASE(GASNETC_LOCK_REGULAR);
  return 0;
}

#define GASNETE_UCXRATOMIC_MID(dtcode) \
        _GASNETE_UCXRATOMIC_MID1(gasnete_ucxratomic##dtcode, dtcode##_isint, dtcode##_type, dtcode)
// This extra pass expands the "isint" token prior to additional concatenation
#define _GASNETE_UCXRATOMIC_MID1(prefix, isint, type, dtcode) \
        _GASNETE_UCXRATOMIC_MID2(prefix, isint, type, dtcode)
#define _GASNETE_UCXRATOMIC_MID2(prefix, isint, type, dtcode)    \
    GASNETI_INLINE(prefix##_NB_N0)                                \
    gex_Event_t prefix##_NB_N0(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_post_op_t op = amo_post_op_map##dtcode(op_idx);  \
      GASNETE_UCX_RATOMIC_PREP_INC(op_idx);                       \
      return gasnete_ratomic_post_nb(sizeof(type), ad->_tm,       \
                                     tgt_rank, tgt_addr, op, inc, \
                                     flags GASNETI_THREAD_PASS);  \
      return GEX_EVENT_INVALID;                                   \
    }                                                             \
    GASNETI_INLINE(prefix##_NB_N1)                                \
    gex_Event_t prefix##_NB_N1(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,                          \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_post_op_t op = amo_post_op_map##dtcode(op_idx);  \
      GASNETE_UCX_RATOMIC_PREP_OP(op_idx);                        \
      return gasnete_ratomic_post_nb(sizeof(type), ad->_tm,       \
                                     tgt_rank, tgt_addr, op, op1, \
                                     flags GASNETI_THREAD_PASS);  \
    }                                                             \
    GASNETI_INLINE(prefix##_NB_N2)                                \
    gex_Event_t prefix##_NB_N2(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,  type        operand2,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_fetch_op_t op = amo_fetch_op_map##dtcode(op_idx);\
      return                                                      \
        gasnete_ratomic_fetch_nb(sizeof(type), NULL, ad->_tm,     \
                              tgt_rank, tgt_addr, op, 2, operand1,\
                              operand2,                           \
                              flags GASNETI_THREAD_PASS);         \
    }                                                             \
    GASNETI_INLINE(prefix##_NB_F0)                                \
    gex_Event_t prefix##_NB_F0(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_fetch_op_t op = amo_fetch_op_map##dtcode(op_idx);\
      GASNETE_UCX_RATOMIC_PREP_FINC(op_idx);                      \
      return                                                      \
        gasnete_ratomic_fetch_nb(sizeof(type), result_p,ad->_tm,  \
                                tgt_rank, tgt_addr, op, 1, inc, 0,\
                                flags GASNETI_THREAD_PASS);       \
    }                                                             \
    GASNETI_INLINE(prefix##_NB_F1)                                \
    gex_Event_t prefix##_NB_F1(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,                          \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_fetch_op_t op = amo_fetch_op_map##dtcode(op_idx);\
      GASNETE_UCX_RATOMIC_PREP_FOP(op_idx);                       \
      return                                                      \
        gasnete_ratomic_fetch_nb(sizeof(type), result_p, ad->_tm, \
                                tgt_rank, tgt_addr, op, 1, op1,0, \
                                flags GASNETI_THREAD_PASS);       \
    }                                                             \
    GASNETI_INLINE(prefix##_NB_F2)                                \
    gex_Event_t prefix##_NB_F2(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,  type        operand2,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_fetch_op_t op = amo_fetch_op_map##dtcode(op_idx);\
      return                                                      \
        gasnete_ratomic_fetch_nb(sizeof(type), result_p, ad->_tm, \
                                 tgt_rank, tgt_addr, op, 2,       \
                                 operand1, operand2,              \
                                 flags GASNETI_THREAD_PASS);      \
    }                                                             \
    GASNETI_INLINE(prefix##_NB_S1)                                \
    gex_Event_t prefix##_NB_S1(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,                          \
                gex_Flags_t flags         GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_fetch_op_t op = amo_fetch_op_map##dtcode(op_idx);\
      return                                                      \
        gasnete_ratomic_fetch_nb(sizeof(type), NULL, ad->_tm,     \
                              tgt_rank, tgt_addr, op, 1, operand1,\
                              0, flags GASNETI_THREAD_PASS);      \
    }                                                             \
    GASNETI_INLINE(prefix##_NB_G0)                                \
    gex_Event_t prefix##_NB_G0(                                   \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                gex_Flags_t flags         GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_fetch_op_t op = amo_fetch_op_map##dtcode(op_idx);\
      return                                                      \
        gasnete_ratomic_fetch_nb(sizeof(type), result_p, ad->_tm, \
                                 tgt_rank, tgt_addr, op, 0, 0, 0, \
                                 flags GASNETI_THREAD_PASS);      \
    }                                                             \
    GASNETI_INLINE(prefix##_NBI_N0)                               \
    int prefix##_NBI_N0(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_post_op_t op = amo_post_op_map##dtcode(op_idx);  \
      GASNETE_UCX_RATOMIC_PREP_INC(op_idx);                       \
      return gasnete_ratomic_post_nbi(sizeof(type), ad->_tm,      \
                                      tgt_rank, tgt_addr, op, inc,\
                                      flags GASNETI_THREAD_PASS); \
    }                                                             \
    GASNETI_INLINE(prefix##_NBI_N1)                               \
    int prefix##_NBI_N1(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,                          \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_post_op_t op = amo_post_op_map##dtcode(op_idx);  \
      GASNETE_UCX_RATOMIC_PREP_OP(op_idx);                        \
      return gasnete_ratomic_post_nbi(sizeof(type), ad->_tm,      \
                                      tgt_rank, tgt_addr, op, op1,\
                                      flags GASNETI_THREAD_PASS); \
    }                                                             \
    GASNETI_INLINE(prefix##_NBI_N2)                               \
    int prefix##_NBI_N2(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,  type        operand2,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_fetch_op_t op = amo_fetch_op_map##dtcode(op_idx);\
      return                                                      \
        gasnete_ratomic_fetch_nbi(sizeof(type), NULL, ad->_tm,    \
                             tgt_rank, tgt_addr, op, 2, operand1, \
                             operand2,                            \
                             flags GASNETI_THREAD_PASS);          \
    }                                                             \
    GASNETI_INLINE(prefix##_NBI_F0)                               \
    int prefix##_NBI_F0(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_fetch_op_t op = amo_fetch_op_map##dtcode(op_idx);\
      GASNETE_UCX_RATOMIC_PREP_FINC(op_idx);                      \
      return                                                      \
        gasnete_ratomic_fetch_nbi(sizeof(type), result_p,ad->_tm, \
                              tgt_rank, tgt_addr, op, 1, inc, 0,  \
                              flags GASNETI_THREAD_PASS);         \
    }                                                             \
    GASNETI_INLINE(prefix##_NBI_F1)                               \
    int prefix##_NBI_F1(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type           *result_p,                         \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,                          \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_fetch_op_t op = amo_fetch_op_map##dtcode(op_idx);\
      GASNETE_UCX_RATOMIC_PREP_FOP(op_idx);                       \
      return                                                      \
        gasnete_ratomic_fetch_nbi(sizeof(type), result_p, ad->_tm,\
                                tgt_rank, tgt_addr, op, 1, op1,0, \
                                flags GASNETI_THREAD_PASS);       \
    }                                                             \
    GASNETI_INLINE(prefix##_NBI_F2)                               \
    int prefix##_NBI_F2(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,  type        operand2,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_fetch_op_t op = amo_fetch_op_map##dtcode(op_idx);\
      return                                                      \
        gasnete_ratomic_fetch_nbi(sizeof(type), result_p, ad->_tm,\
                              tgt_rank, tgt_addr, op, 2, operand1,\
                              operand2,flags GASNETI_THREAD_PASS);\
    }                                                             \
    GASNETI_INLINE(prefix##_NBI_S1)                               \
    int prefix##_NBI_S1(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                type           operand1,                          \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_fetch_op_t op = amo_fetch_op_map##dtcode(op_idx);\
      return                                                      \
        gasnete_ratomic_fetch_nbi(sizeof(type), NULL, ad->_tm,    \
                              tgt_rank, tgt_addr, op, 1, operand1,\
                              0, flags GASNETI_THREAD_PASS);      \
    }                                                             \
    GASNETI_INLINE(prefix##_NBI_G0)                               \
    int prefix##_NBI_G0(                                          \
                gasneti_op_idx_t op_idx,  gasneti_AD_t      ad,   \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,  void       *tgt_addr,   \
                gex_Flags_t       flags   GASNETI_THREAD_FARG)    \
    {                                                             \
      ucp_atomic_fetch_op_t op = amo_fetch_op_map##dtcode(op_idx);\
      return                                                      \
        gasnete_ratomic_fetch_nbi(sizeof(type), result_p, ad->_tm,\
                                 tgt_rank, tgt_addr, op, 0, 0, 0, \
                                 flags GASNETI_THREAD_PASS);      \
    }
GASNETE_DT_INT_APPLY(GASNETE_UCXRATOMIC_MID)


#define GASNETE_UCXRATOMIC_DEFS(dtcode) \
        _GASNETE_UCXRATOMIC_DEFS1(dtcode, dtcode##_isint)
// This extra pass expands the "isint" token prior to additional concatenation
#define _GASNETE_UCXRATOMIC_DEFS1(dtcode, isint) \
        _GASNETE_UCXRATOMIC_DEFS2(dtcode, isint)
#define _GASNETE_UCXRATOMIC_DEFS2(dtcode, isint) \
    _GASNETE_UCXRATOMIC_DEFN_INT##isint(dtcode,AND,1) \
    _GASNETE_UCXRATOMIC_DEFN_INT##isint(dtcode,OR,1)  \
    _GASNETE_UCXRATOMIC_DEFN_INT##isint(dtcode,XOR,1) \
    _GASNETE_UCXRATOMIC_DEFN2(dtcode,ADD,1)           \
    _GASNETE_UCXRATOMIC_DEFN2(dtcode,SUB,1)           \
    _GASNETE_UCXRATOMIC_DEFN2(dtcode,MULT,1)          \
    _GASNETE_UCXRATOMIC_DEFN2(dtcode,MIN,1)           \
    _GASNETE_UCXRATOMIC_DEFN2(dtcode,MAX,1)           \
    _GASNETE_UCXRATOMIC_DEFN2(dtcode,INC,0)           \
    _GASNETE_UCXRATOMIC_DEFN2(dtcode,DEC,0)           \
    _GASNETE_UCXRATOMIC_DEFN1(dtcode,SET,S1)          \
    _GASNETE_UCXRATOMIC_DEFN1(dtcode,GET,G0)          \
    _GASNETE_UCXRATOMIC_DEFN1(dtcode,SWAP,F1)         \
    _GASNETE_UCXRATOMIC_DEFN1(dtcode,FCAS,F2)         \
    _GASNETE_UCXRATOMIC_DEFN1(dtcode,CAS,N2)

#define _GASNETE_UCXRATOMIC_DEFN_INT0(dtcode,opname,nargs) /*empty*/
#define _GASNETE_UCXRATOMIC_DEFN_INT1 _GASNETE_UCXRATOMIC_DEFN2
#define _GASNETE_UCXRATOMIC_DEFN2(dtcode,opstem,nargs) \
        _GASNETE_UCXRATOMIC_DEFN1(dtcode,opstem,N##nargs) \
        _GASNETE_UCXRATOMIC_DEFN1(dtcode,F##opstem,F##nargs)
#define _GASNETE_UCXRATOMIC_DEFN1(dtcode,opname,args) \
  static gex_Event_t gasnete_ucxratomic##dtcode##_NB_##opname(GASNETE_RATOMIC_ARGS_##args(dtcode##_type)) { \
    return gasnete_ucxratomic##dtcode##_NB_##args(gasneti_op_idx_##opname,GASNETE_RATOMIC_PASS_##args);     \
  } \
  static int gasnete_ucxratomic##dtcode##_NBI_##opname(GASNETE_RATOMIC_ARGS_##args(dtcode##_type)) {        \
    return gasnete_ucxratomic##dtcode##_NBI_##args(gasneti_op_idx_##opname,GASNETE_RATOMIC_PASS_##args);    \
  }

#define GASNETE_RATOMIC_ARGS_S1 GASNETE_RATOMIC_ARGS_N1
#define GASNETE_RATOMIC_ARGS_G0 GASNETE_RATOMIC_ARGS_F0
#define GASNETE_RATOMIC_PASS_S1 GASNETE_RATOMIC_PASS_N1
#define GASNETE_RATOMIC_PASS_G0 GASNETE_RATOMIC_PASS_F0

GASNETE_DT_INT_APPLY(GASNETE_UCXRATOMIC_DEFS)

#define GASNETE_UCXRATOMIC_TBL(dtcode) \
    gasnete_ratomic##dtcode##_fn_tbl_t gasnete_ucxratomic##dtcode##_fn_tbl = \
        GASNETE_RATOMIC_FN_TBL_INIT(gasnete_ucxratomic##dtcode,dtcode);
GASNETE_DT_INT_APPLY(GASNETE_UCXRATOMIC_TBL)

// Masks of missing capabilities
#define GASNETE_UCXRATOMIC_NO_MULT   (GEX_OP_MULT | GEX_OP_FMULT)
#define GASNETE_UCXRATOMIC_NO_MINMAX (GEX_OP_MIN  | GEX_OP_FMIN | GEX_OP_MAX  | GEX_OP_FMAX)

#define GASNETE_UCXRATOMIC_BADOPS_gex_dt_I32 (GASNETE_UCXRATOMIC_NO_MULT | GASNETE_UCXRATOMIC_NO_MINMAX)
#define GASNETE_UCXRATOMIC_BADOPS_gex_dt_U32 (GASNETE_UCXRATOMIC_NO_MULT | GASNETE_UCXRATOMIC_NO_MINMAX)
#define GASNETE_UCXRATOMIC_BADOPS_gex_dt_I64 (GASNETE_UCXRATOMIC_NO_MULT | GASNETE_UCXRATOMIC_NO_MINMAX)
#define GASNETE_UCXRATOMIC_BADOPS_gex_dt_U64 (GASNETE_UCXRATOMIC_NO_MULT | GASNETE_UCXRATOMIC_NO_MINMAX)

void gasnete_ucxratomic_create_hook(
        gasneti_AD_t               real_ad,
        gasneti_TM_t               real_tm,
        gex_DT_t                   dt,
        gex_OP_t                   ops,
        gex_Flags_t                flags)
{
#define GASNETE_UCXRATOMIC_TBL_CASE(dtcode) \
    case dtcode##_dtype:                    \
        if (ops & GASNETE_UCXRATOMIC_BADOPS##dtcode) goto use_am; \
        real_ad->_fn_tbl = (gasnete_ratomic_fn_tbl_t) &gasnete_ucxratomic##dtcode##_fn_tbl; \
        real_ad->_tools_safe = 0; \
        return;
  switch(dt) {
      GASNETE_DT_INT_APPLY(GASNETE_UCXRATOMIC_TBL_CASE)
      default: ((void)0); // FLT and DBL fall-through to 'use_am' below
  }
#undef GASNETE_UCXRATOMIC_TBL_CASE

use_am:
  gasnete_amratomic_create_hook(real_ad, real_tm, dt, ops, flags);
  return;
}
