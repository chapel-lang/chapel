/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/ratomic/gasnet_refratomic.c $
 * Description: Reference implemetation of GASNet Remote Atomics, using Active Messages
 * Copyright 2017, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#define GASNETI_NEED_GASNET_COLL_H GASNET_DEBUG // Yes, this works.
#define GASNETI_NEED_GASNET_RATOMIC_H 1

#include <gasnet_internal.h>
#include <gasnet_ratomic_internal.h>
#include <gasnet_refratomic.h>

#ifndef _GEX_AD_T

#ifndef gasneti_import_ad
gasneti_AD_t gasneti_import_ad(gex_AD_t _ad) {
  const gasneti_AD_t _real_ad = GASNETI_IMPORT_POINTER(gasneti_AD_t,_ad);
  GASNETI_IMPORT_MAGIC(_real_ad, AD);
  return _real_ad;
}
#endif

#ifndef gasneti_export_ad
gex_AD_t gasneti_export_ad(gasneti_AD_t _real_ad) {
  GASNETI_CHECK_MAGIC(_real_ad, GASNETI_AD_MAGIC);
  return GASNETI_EXPORT_POINTER(gex_AD_t, _real_ad);
}
#endif

extern gasneti_AD_t gasneti_alloc_ad(
                       gasneti_TM_t tm,
                       gex_DT_t dt,
                       gex_OP_t ops,
                       gex_Flags_t flags,
                       size_t alloc_size)
{
  gasneti_AD_t ad = gasneti_malloc(alloc_size ? alloc_size : sizeof(*ad));
  gasneti_assert(!alloc_size || alloc_size >= sizeof(*ad));
  GASNETI_INIT_MAGIC(ad, GASNETI_AD_MAGIC);
  ad->_cdata = NULL;
  ad->_tm = tm;
  ad->_rank = tm->_rank; // Used often enough to justify caching
  ad->_tm0   = tm->_ep->_client->_tm0;
  ad->_rank0 = tm->_ep->_client->_tm0->_rank;
  ad->_flags = flags;
  ad->_dt = dt;
  ad->_ops = ops;
#if GASNET_DEBUG
  ad->_tools_safe = -1;
  ad->_fn_tbl = NULL;
#endif
#ifdef GASNETI_AD_ALLOC_EXTRA
  GASNETI_AD_ALLOC_EXTRA(ad);
#endif
  return ad;
}

void gasneti_free_ad(gasneti_AD_t ad)
{
#ifdef GASNETI_AD_FREE_EXTRA
  GASNETI_AD_FREE_EXTRA(ad);
#endif
  GASNETI_INIT_MAGIC(ad, GASNETI_AD_BAD_MAGIC);
  gasneti_free(ad);
}

void gasneti_AD_Create(
        gex_AD_t                   *ad_p,
        gex_TM_t                   tm,
        gex_DT_t                   dt,
        gex_OP_t                   ops,
        gex_Flags_t                flags)
{
  gasneti_TM_t real_tm = gasneti_import_tm(tm);

  // Argument validation is done here, rather than gasneti_alloc_ad(), to
  // allow conduit-specific extensions (such as additional types or ops).
  // However, this leaves a significant amount of code to be cloned into
  // the conduit.

#if GASNET_DEBUG
  // Verify that call is collective and single-valued
  {
    struct {
        gex_DT_t       dt;
        gex_OP_t       ops;
        gex_Flags_t    flags;
    } args;
    if (!real_tm->_rank) {
      args.dt    = dt;
      args.ops   = ops;
      args.flags = flags;
    }
    gex_Event_Wait(gex_Coll_BroadcastNB(tm, 0, &args, &args, sizeof(args), 0));
    gasneti_assert(args.dt    == dt);
    gasneti_assert(args.ops   == ops);
    gasneti_assert(args.flags == flags);
  }

  // Does the 'dt' arument name a single valid data type?
  gasneti_assert(gasneti_dt_valid_atomic(dt));

  // Does ops specify a non-empty set with ALL members valid for remote atomics on the data type?
  gasneti_assert(ops); // Not empty
  gasneti_assert(gasneti_op_atomic_mask(ops));
  gasneti_assert(gasneti_op_fp_mask(ops)  || !gasneti_dt_fp(dt));
  gasneti_assert(gasneti_op_int_mask(ops) || !gasneti_dt_int(dt));

  // Does the 'flags' argument include at most one AD_FAVOR_* bit?
  gasneti_assert(GASNETI_POWEROFTWO(flags & (GEX_FLAG_AD_FAVOR_MY_RANK |
                                             GEX_FLAG_AD_FAVOR_MY_NBRHD |
                                             GEX_FLAG_AD_FAVOR_REMOTE)));

  // Verify we agree on the size of the FP type, if any
  gasneti_assert((dt != GEX_DT_FLT) || sizeof(float) == 4);
  gasneti_assert((dt != GEX_DT_DBL) || sizeof(double) == 8);
#endif

  gasneti_AD_t real_ad = gasneti_alloc_ad(real_tm, dt, ops, flags, 0);

  // Algorithm selection:
#ifdef GASNETI_AD_CREATE_HOOK
  GASNETI_AD_CREATE_HOOK(real_ad, real_tm, dt, ops, flags);
  gasneti_assert(real_ad->_tools_safe >= 0);
  gasneti_assert(real_ad->_fn_tbl != NULL);
#endif

  *ad_p = gasneti_export_ad(real_ad);
  return;
}

void gasneti_AD_Destroy(gex_AD_t ad)
{
  gasneti_AD_t real_ad = gasneti_import_ad(ad);

#if GASNET_DEBUG
  // TODO: might try to verify that call is collective?
  // TODO: can/should we verify that there are no incompete ops?
#endif

  gasneti_free_ad(real_ad);
  return;
}


/*---------------------------------------------------------------------------------*/
#if GASNET_DEBUG
void gasnete_ratomic_validate(
        gex_AD_t            ad,        void             *result_p,
        gex_Rank_t          tgt_rank,  void             *tgt_addr,
        gex_OP_t            opcode,    gex_DT_t         datatype,
        gex_Flags_t         flags)
{
    gasneti_assert(ad != GEX_AD_INVALID);
    gasneti_AD_t real_ad = gasneti_import_ad(ad);

    // Rank must be valid
    if (flags & GEX_FLAG_RANK_IS_JOBRANK) {
      if (gasneti_i_tm_jobrank_to_rank(real_ad->_tm, tgt_rank) == GEX_RANK_INVALID) {
        gasneti_fatalerror("gex_AD_Op*() called with invalid target jobrank");
      }
    } else {
      if (tgt_rank >= real_ad->_tm->_size) {
        gasneti_fatalerror("gex_AD_Op*() called with invalid target rank");
      }
    }

    // Datatype must match AD
    if (datatype != real_ad->_dt) {
      char *str1 = gasneti_malloc(gasneti_format_dt(NULL, datatype));
      char *str2 = gasneti_malloc(gasneti_format_dt(NULL, real_ad->_dt));
      gasneti_format_dt(str1, datatype);
      gasneti_format_dt(str2, real_ad->_dt);
      gasneti_fatalerror("gex_AD_Op*() called with data type 0x%x (%s), not matching the AD (%s)",
                         (unsigned int)datatype, str1, str2);
    }

    // Opcode must be exactly 1 bit and valid for AD
    if (! gasneti_op_valid_atomic(opcode)) {
      char *str1 = gasneti_malloc(gasneti_format_op(NULL, opcode));
      gasneti_format_op(str1, opcode);
      gasneti_fatalerror("gex_AD_Op*() called with an unknown/invalid opcode 0x%x (%s)",
                         (unsigned int)opcode, str1);
    }
    if (! (opcode & real_ad->_ops)) {
      char *str1 = gasneti_malloc(gasneti_format_op(NULL, opcode));
      char *str2 = gasneti_malloc(gasneti_format_op(NULL, real_ad->_ops));
      gasneti_format_op(str1, opcode);
      gasneti_format_op(str2, real_ad->_ops);
      gasneti_fatalerror("gex_AD_Op*() called with an opcode 0x%x (%s) not valid for the AD (%s)",
                         (unsigned int)opcode, str1, str2);
    }

    // Fetching ops must have non-NULL result_p
    if (gasneti_op_fetch(opcode) && !result_p) {
      char *str1 = gasneti_malloc(gasneti_format_op(NULL, opcode));
      gasneti_format_op(str1, opcode);
      gasneti_fatalerror("gex_AD_Op*() called with fetching opcode %s, but result_p==NULL", str1);
    }

    // Flags may provide at most one affinity assertion
    if (!GASNETI_POWEROFTWO(flags & (GEX_FLAG_AD_MY_RANK | GEX_FLAG_AD_MY_NBRHD))) {
      gasneti_fatalerror("gex_AD_Op*() called with more than one GEX_FLAG_AD_MY_* flag");
    }

    // Address must be in bound segment
    // TODO: remove this restriction?
    gasneti_boundscheck(gasnete_ratomic_e_tm(real_ad, flags), tgt_rank, tgt_addr, gasneti_dt_size(datatype));
}
#endif

/*---------------------------------------------------------------------------------*/

//
// Non-inlined instances of the dispatch functions
//
#define GASNETE_RATOMIC_EXTERNS(dtcode) \
        _GASNETE_RATOMIC_EXTERNS(gasnete_ratomic##dtcode, dtcode##_type)
#define _GASNETE_RATOMIC_EXTERNS(prefix, type)                     \
    gex_Event_t prefix##_NB_external(                             \
        gex_AD_t            ad,        type           *result_p,  \
        gex_Rank_t          tgt_rank,  void           *tgt_addr,  \
        gex_OP_t            opcode,    type           operand1,   \
        type                operand2,  gex_Flags_t    flags       \
        GASNETI_THREAD_FARG)                                      \
    {                                                             \
      return prefix##_NB(ad, result_p, tgt_rank, tgt_addr,        \
                         opcode, operand1, operand2, flags        \
                         GASNETI_THREAD_PASS);                    \
    } \
    int prefix##_NBI_external(                                    \
        gex_AD_t            ad,        type           *result_p,  \
        gex_Rank_t          tgt_rank,  void           *tgt_addr,  \
        gex_OP_t            opcode,    type           operand1,   \
        type                operand2,  gex_Flags_t    flags       \
        GASNETI_THREAD_FARG)                                      \
    {                                                             \
      return prefix##_NBI(ad, result_p, tgt_rank, tgt_addr,       \
                         opcode, operand1, operand2, flags        \
                         GASNETI_THREAD_PASS);                    \
    }
GASNETE_DT_APPLY(GASNETE_RATOMIC_EXTERNS)

/*---------------------------------------------------------------------------------*/
//
// AM-based Implementation
// Built unless GASNETE_BUILD_AMRATOMIC is defined to 0
// If GASNETE_BUILD_AMRATOMIC_STUBS is 1, then we build stubs that fatalerror
//

// Notes on implementation of GEX_FLAG_AD_{REL,ACQ}
//
// The reference implementation of Remote Atomics has two portions.
//
// One is used for target ranks meeting "MY_RANK" and "MY_NBRHD"
// conditions (with possible datatype constraints).  This implementation
// performs all atomic operations synchronously using GASNet-Tools, which
// provides the necessary support for REL and ACQ fences.
//
// The second is the AM-based code below.  The remainder of this note is an
// effort to explain how GEX_FLAG_AD_{REL,ACQ} are implemented.  The short
// version is that both fences are *unconditionally* present in the
// implementation and so the flags are ignored with no loss of correctness.
//
// RELEASE:
// We believe that all current AM implementations include at least one release
// fence on the path to AM injection.
//
// ACQUIRE:
// We believe that all current AM implementations include at least one acquire
// fence on the path to reception of the AM Reply, which is sufficient in a
// single-threaded build.  In the case of a multi-threaded build, the
// implementation of all GASNet-EX synchronization calls (both on gex_Event_t
// and NBI) include an acquire fence if the set of operations synchronized
// could potentially include a RMW or GET operation.

#if GASNETE_BUILD_AMRATOMIC

#if ! GASNETE_BUILD_AMRATOMIC_STUBS

//
// Pool of small structs describing an in-flight remote atomic
// TODO: per-thread free lists?
//
static gasneti_lifo_head_t gasnete_amratomic_op_pool = GASNETI_LIFO_INITIALIZER;
typedef struct {
    void *result_p;
    gasneti_eop_t *eop;
    gasneti_iop_t *iop;
    gex_EC_t iop_type;
} *gasnete_amratomic_op_t;

GASNETI_INLINE(gasnete_amratomic_op_alloc) GASNETI_MALLOC
gasnete_amratomic_op_t gasnete_amratomic_op_alloc(void) {
  gasnete_amratomic_op_t result = gasneti_lifo_pop(&gasnete_amratomic_op_pool);
  if_pf (NULL == result) {
    // TODO: grow pool by large contig. blocks?
    result = gasneti_malloc(sizeof(*result));
    gasneti_leak(result);
  }
  return result;
}

GASNETI_INLINE(gasnete_amratomic_op_free)
void gasnete_amratomic_op_free(gasnete_amratomic_op_t rop) {
  gasneti_lifo_push(&gasnete_amratomic_op_pool, rop);
}


//
// AM Request Handler
// TODO: For a SEQ endpoint the updates can be non-atomic!
//
GASNETI_INLINE(gasnete_amratomic_reqh_inner)
void gasnete_amratomic_reqh_inner(
                gex_Token_t token,
                void *addr, size_t nbytes,
                gex_AM_Arg_t ctrl, void *tgt, void *rop)
{
    const gasneti_op_idx_t op_idx = (gasneti_op_idx_t) (ctrl & 0x1F); // Low 5 bits
    const gex_OP_t opcode = ((gex_OP_t)1 << op_idx);
    const gex_DT_t dt = ((gex_DT_t)ctrl >> 8);

    union { int32_t u_gex_dt_I32; uint32_t u_gex_dt_U32;
            int64_t u_gex_dt_I64; uint64_t u_gex_dt_U64;
            float   u_gex_dt_FLT; double   u_gex_dt_DBL; } result;
    size_t typesz;

    gasneti_assert(nbytes ==
                   (gasneti_dt_size(dt) *
                    (gasneti_op_0arg(opcode)?0:gasneti_op_1arg(opcode)?1:2)));

    // Load the operands from the Medium payload into 'op1' and 'op2'
    // and set 'typesz'.  This is necessary, instead of direct use of
    // ops[0] and ops[1], since the code that follows will pass *both*
    // operands, even if one or both lies beyond the end of the payload.
    //
    // Note: all parameter and variable names are hardcoded.
    //
    // Compilers have no way to understand which operations use which
    // operands, and so we are faced with maybe-used-uninitialized warnings
    // unless we add unneeded initializers.  So, we have zero initializers
    // here under the assumption that, relative to the AM overheads, these
    // are not "too costly".
    // TODO: revisit this.
    #define GASNETE_AMRATOMIC_REQH_OPS(type)    \
        type op1 = 0;                           \
        type op2 = 0;                           \
        type *ops = (type*) addr;               \
        switch (nbytes / sizeof(type)) {        \
            case 2: op2 = ops[1];               \
                    GASNETI_FALLTHROUGH         \
            case 1: op1 = ops[0];               \
        }                                       \
        typesz = sizeof(type)

    // Case for a single datatype.
    // Note: all parameter and variable names are hardcoded.
    #define GASNETE_AMRATOMIC_REQH_CASE(dtcode) \
        case dtcode##_dtype: {                                                \
            GASNETE_AMRATOMIC_REQH_OPS(dtcode##_type);                        \
            result.u##dtcode = gasnete_ratomicfn##dtcode(tgt,op1,op2,opcode,0);\
            break;                                                            \
        }

    switch (dt) {
        GASNETE_DT_APPLY(GASNETE_AMRATOMIC_REQH_CASE)
        default: gasneti_unreachable();
    }

    gex_AM_ReplyMedium(token, gasneti_handleridx(gasnete_amratomic_reph),
                       &result, gasneti_op_fetch(opcode) ? typesz : 0,
                       GEX_EVENT_NOW, 0, PACK(rop));

    #undef GASNETE_AMRATOMIC_REQH_CASE
    #undef GASNETE_AMRATOMIC_REQH_OPS
}
MEDIUM_HANDLER(gasnete_amratomic_reqh,3,5,
               (token,addr,nbytes, a0, UNPACK(a1),      UNPACK(a2)    ),
               (token,addr,nbytes, a0, UNPACK2(a1, a2), UNPACK2(a3, a4)));

//
// AM Reply Handler
//
GASNETI_INLINE(gasnete_amratomic_reph_inner)
void gasnete_amratomic_reph_inner(
                gex_Token_t token,
                void *payload, size_t nbytes,
                gasnete_amratomic_op_t rop)
{
    gasneti_assert(rop);
    switch (nbytes) {
        case 4:
            *(uint32_t*)rop->result_p = *(uint32_t*)payload;
            gasneti_sync_writes();
            break;
        case 8:
            *(uint64_t*)rop->result_p = *(uint64_t*)payload;
            gasneti_sync_writes();
            break;
        default:
            gasneti_assert(!nbytes && !rop->result_p);
    }
    gasneti_assert((!rop->eop) ^ (!rop->iop)); // Exactly one is non-NULL
    if (rop->eop) {
        gasneti_eop_markdone(rop->eop);
    } else if (rop->iop_type == GEX_EC_RMW) {
        gasneti_iop_markdone_rmw(rop->iop, 1); // TODO-EX: EOP_INTERFACE - to be replaced
    } else {
        gasneti_assert((rop->iop_type == GEX_EC_PUT) || (rop->iop_type == GEX_EC_GET));
        gasneti_iop_markdone(rop->iop, 1, (rop->iop_type == GEX_EC_GET));
    }
    gasnete_amratomic_op_free(rop);
}
MEDIUM_HANDLER(gasnete_amratomic_reph,1,2,
               (token,addr,nbytes, UNPACK(a0)      ),
               (token,addr,nbytes, UNPACK2(a0, a1)));


//
// Functions to send AM Medium with either NB or NBI completion upon reply.
//
GASNETI_INLINE(gasnete_amratomic_request_NB)
gex_Event_t gasnete_amratomic_request_NB(
        gasneti_AD_t ad, gex_Rank_t tgt_rank,
        void *result_p, void *tgt_addr,
        gasneti_op_idx_t op_idx, gex_DT_t datatype,
        void *payload, size_t length, gex_Flags_t flags
        GASNETI_THREAD_FARG)
{
    gasnete_amratomic_op_t rop = gasnete_amratomic_op_alloc();
    rop->result_p = result_p;
    rop->iop = NULL;
    rop->eop = gasneti_eop_create(GASNETI_THREAD_PASS_ALONE);
    gex_Event_t result = gasneti_eop_to_event(rop->eop);
    gex_TM_t tm = gasnete_ratomic_e_tm(ad, flags);
    gex_Flags_t am_flags = flags & GEX_FLAG_IMMEDIATE;
    int imm = gex_AM_RequestMedium(tm, tgt_rank, gasneti_handleridx(gasnete_amratomic_reqh),
                                   payload, length, GEX_EVENT_NOW, am_flags,
                                   (op_idx | (datatype << 8)), PACK(tgt_addr), PACK(rop));
    if (imm) {
        gasneti_eop_markdone(rop->eop);
        gasneti_assert_zeroret(gasnete_test(result GASNETI_THREAD_PASS));
        gasnete_amratomic_op_free(rop);
        result = GEX_EVENT_NO_OP;
    }
    return result;
}
GASNETI_INLINE(gasnete_amratomic_request_NBI)
int gasnete_amratomic_request_NBI(
        gasneti_AD_t ad, gex_Rank_t tgt_rank,
        void *result_p, void *tgt_addr,
        gasneti_op_idx_t op_idx, gex_DT_t datatype,
        void *payload, size_t length, gex_Flags_t flags
        GASNETI_THREAD_FARG)
{
    gasnete_amratomic_op_t rop = gasnete_amratomic_op_alloc();
    rop->result_p = result_p;
    rop->eop = NULL;
    rop->iop = gasneti_iop_register_rmw(1 GASNETI_THREAD_PASS); // TODO-EX: EOP_INTERFACE - to be replaced
    rop->iop_type = GEX_EC_RMW;
    gex_TM_t tm = gasnete_ratomic_e_tm(ad, flags);
    gex_Flags_t am_flags = flags & GEX_FLAG_IMMEDIATE;
    int imm = gex_AM_RequestMedium(tm, tgt_rank, gasneti_handleridx(gasnete_amratomic_reqh),
                                   payload, length, GEX_EVENT_NOW, am_flags,
                                   (op_idx | (datatype << 8)), PACK(tgt_addr), PACK(rop));
    if (imm) {
        gasneti_iop_markdone_rmw(rop->iop, 1); // TODO-EX: EOP_INTERFACE - to be replaced
        gasnete_amratomic_op_free(rop);
    }
    return imm;
}

//
// Inline functions (called by the ratomic functions) that invoke
// the gasnete_amratomic_request_{NB,NBI}() functions, above.
//
#define GASNETE_AMRATOMIC_MID_NB(dtcode) \
        _GASNETE_AMRATOMIC_MID_NB(gasnete_amratomic##dtcode, dtcode##_dtype, dtcode##_type)
#define _GASNETE_AMRATOMIC_MID_NB(prefix, datatype, type)         \
    GASNETI_INLINE(prefix##_NB_N0)                                \
    gex_Event_t prefix##_NB_N0(                                   \
                gasneti_op_idx_t op_idx,    gasneti_AD_t ad,      \
                gex_Rank_t       tgt_rank,  void       *tgt_addr, \
                gex_Flags_t      flags      GASNETI_THREAD_FARG)  \
    {                                                             \
        gasneti_assert(gasneti_op_0arg(((gex_OP_t)1 << op_idx))); \
        return gasnete_amratomic_request_NB(                      \
                        ad, tgt_rank,                             \
                        NULL, tgt_addr, op_idx, datatype,         \
                        NULL, 0, flags                            \
                        GASNETI_THREAD_PASS);                     \
    } \
    GASNETI_INLINE(prefix##_NB_N1)                                \
    gex_Event_t prefix##_NB_N1(                                   \
                gasneti_op_idx_t op_idx,    gasneti_AD_t ad,      \
                gex_Rank_t       tgt_rank,  void       *tgt_addr, \
                type             operand1,                        \
                gex_Flags_t      flags      GASNETI_THREAD_FARG)  \
    {                                                             \
        gasneti_assert(gasneti_op_1arg(((gex_OP_t)1 << op_idx))); \
        return gasnete_amratomic_request_NB(                      \
                        ad, tgt_rank,                             \
                        NULL, tgt_addr, op_idx, datatype,         \
                        &operand1, sizeof(type), flags            \
                        GASNETI_THREAD_PASS);                     \
    } \
    GASNETI_INLINE(prefix##_NB_N2)                                \
    gex_Event_t prefix##_NB_N2(                                   \
                gasneti_op_idx_t op_idx,    gasneti_AD_t ad,      \
                gex_Rank_t       tgt_rank,  void       *tgt_addr, \
                type             operand1,  type        operand2, \
                gex_Flags_t      flags      GASNETI_THREAD_FARG)  \
    {                                                             \
        gasneti_assert(gasneti_op_2arg(((gex_OP_t)1 << op_idx))); \
        type payload[2];                                          \
        payload[0] = operand1; payload[1] = operand2;             \
        return gasnete_amratomic_request_NB(                      \
                        ad, tgt_rank,                             \
                        NULL, tgt_addr, op_idx, datatype,         \
                        &payload, 2*sizeof(type), flags           \
                        GASNETI_THREAD_PASS);                     \
    } \
    GASNETI_INLINE(prefix##_NB_F0)                                \
    gex_Event_t prefix##_NB_F0(                                   \
                gasneti_op_idx_t op_idx,    gasneti_AD_t ad,      \
                type             *result_p,                       \
                gex_Rank_t       tgt_rank,  void       *tgt_addr, \
                gex_Flags_t      flags      GASNETI_THREAD_FARG)  \
    {                                                             \
        gasneti_assert(gasneti_op_0arg(((gex_OP_t)1 << op_idx))); \
        return gasnete_amratomic_request_NB(                      \
                        ad, tgt_rank,                             \
                        result_p, tgt_addr, op_idx, datatype,     \
                        NULL, 0, flags                            \
                        GASNETI_THREAD_PASS);                     \
    } \
    GASNETI_INLINE(prefix##_NB_F1)                                \
    gex_Event_t prefix##_NB_F1(                                   \
                gasneti_op_idx_t op_idx,    gasneti_AD_t ad,      \
                type             *result_p,                       \
                gex_Rank_t       tgt_rank,  void       *tgt_addr, \
                type             operand1,                        \
                gex_Flags_t      flags      GASNETI_THREAD_FARG)  \
    {                                                             \
        gasneti_assert(gasneti_op_1arg(((gex_OP_t)1 << op_idx))); \
        return gasnete_amratomic_request_NB(                      \
                        ad, tgt_rank,                             \
                        result_p, tgt_addr, op_idx, datatype,     \
                        &operand1, sizeof(type), flags            \
                        GASNETI_THREAD_PASS);                     \
    } \
    GASNETI_INLINE(prefix##_NB_F2)                                \
    gex_Event_t prefix##_NB_F2(                                   \
                gasneti_op_idx_t op_idx,    gasneti_AD_t ad,      \
                type             *result_p,                       \
                gex_Rank_t       tgt_rank,  void       *tgt_addr, \
                type             operand1,  type        operand2, \
                gex_Flags_t      flags      GASNETI_THREAD_FARG)  \
    {                                                             \
        gasneti_assert(gasneti_op_2arg(((gex_OP_t)1 << op_idx))); \
        type payload[2];                                          \
        payload[0] = operand1; payload[1] = operand2;             \
        return gasnete_amratomic_request_NB(                      \
                        ad, tgt_rank,                             \
                        result_p, tgt_addr, op_idx, datatype,     \
                        &payload, 2*sizeof(type), flags           \
                        GASNETI_THREAD_PASS);                     \
    }
#define GASNETE_AMRATOMIC_MID_NBI(dtcode) \
        _GASNETE_AMRATOMIC_MID_NBI(gasnete_amratomic##dtcode, dtcode##_dtype, dtcode##_type)
#define _GASNETE_AMRATOMIC_MID_NBI(prefix, datatype, type)        \
    GASNETI_INLINE(prefix##_NBI_N0)                               \
    int prefix##_NBI_N0(                                          \
                gasneti_op_idx_t op_idx,    gasneti_AD_t ad,      \
                gex_Rank_t       tgt_rank,  void       *tgt_addr, \
                gex_Flags_t      flags      GASNETI_THREAD_FARG)  \
    {                                                             \
        gasneti_assert(gasneti_op_0arg(((gex_OP_t)1 << op_idx))); \
        return gasnete_amratomic_request_NBI(                     \
                        ad, tgt_rank,                             \
                        NULL, tgt_addr, op_idx, datatype,         \
                        NULL, 0, flags                            \
                        GASNETI_THREAD_PASS);                     \
    } \
    GASNETI_INLINE(prefix##_NBI_N1)                               \
    int prefix##_NBI_N1(                                          \
                gasneti_op_idx_t op_idx,    gasneti_AD_t ad,      \
                gex_Rank_t       tgt_rank,  void       *tgt_addr, \
                type             operand1,                        \
                gex_Flags_t      flags      GASNETI_THREAD_FARG)  \
    {                                                             \
        gasneti_assert(gasneti_op_1arg(((gex_OP_t)1 << op_idx))); \
        return gasnete_amratomic_request_NBI(                     \
                        ad, tgt_rank,                             \
                        NULL, tgt_addr, op_idx, datatype,         \
                        &operand1, sizeof(type), flags            \
                        GASNETI_THREAD_PASS);                     \
    } \
    GASNETI_INLINE(prefix##_NBI_N2)                               \
    int prefix##_NBI_N2(                                          \
                gasneti_op_idx_t op_idx,    gasneti_AD_t ad,      \
                gex_Rank_t       tgt_rank,  void       *tgt_addr, \
                type             operand1,  type        operand2, \
                gex_Flags_t      flags      GASNETI_THREAD_FARG)  \
    {                                                             \
        gasneti_assert(gasneti_op_2arg(((gex_OP_t)1 << op_idx))); \
        type payload[2];                                          \
        payload[0] = operand1; payload[1] = operand2;             \
        return gasnete_amratomic_request_NBI(                     \
                        ad, tgt_rank,                             \
                        NULL, tgt_addr, op_idx, datatype,         \
                        &payload, 2*sizeof(type), flags           \
                        GASNETI_THREAD_PASS);                     \
    } \
    GASNETI_INLINE(prefix##_NBI_F0)                               \
    int prefix##_NBI_F0(                                          \
                gasneti_op_idx_t op_idx,    gasneti_AD_t ad,      \
                type             *result_p,                       \
                gex_Rank_t       tgt_rank,  void       *tgt_addr, \
                gex_Flags_t      flags      GASNETI_THREAD_FARG)  \
    {                                                             \
        gasneti_assert(gasneti_op_0arg(((gex_OP_t)1 << op_idx))); \
        return gasnete_amratomic_request_NBI(                     \
                        ad, tgt_rank,                             \
                        result_p, tgt_addr, op_idx, datatype,     \
                        NULL, 0, flags                            \
                        GASNETI_THREAD_PASS);                     \
    } \
    GASNETI_INLINE(prefix##_NBI_F1)                               \
    int prefix##_NBI_F1(                                          \
                gasneti_op_idx_t op_idx,    gasneti_AD_t ad,      \
                type             *result_p,                       \
                gex_Rank_t       tgt_rank,  void       *tgt_addr, \
                type             operand1,                        \
                gex_Flags_t      flags      GASNETI_THREAD_FARG)  \
    {                                                             \
        gasneti_assert(gasneti_op_1arg(((gex_OP_t)1 << op_idx))); \
        return gasnete_amratomic_request_NBI(                     \
                        ad, tgt_rank,                             \
                        result_p, tgt_addr, op_idx, datatype,     \
                        &operand1, sizeof(type), flags            \
                        GASNETI_THREAD_PASS);                     \
    } \
    GASNETI_INLINE(prefix##_NBI_F2)                               \
    int prefix##_NBI_F2(                                          \
                gasneti_op_idx_t op_idx,    gasneti_AD_t ad,      \
                type             *result_p,                       \
                gex_Rank_t       tgt_rank,  void       *tgt_addr, \
                type             operand1,  type        operand2, \
                gex_Flags_t      flags      GASNETI_THREAD_FARG)  \
    {                                                             \
        gasneti_assert(gasneti_op_2arg(((gex_OP_t)1 << op_idx))); \
        type payload[2];                                          \
        payload[0] = operand1; payload[1] = operand2;             \
        return gasnete_amratomic_request_NBI(                     \
                        ad, tgt_rank,                             \
                        result_p, tgt_addr, op_idx, datatype,     \
                        &payload, 2*sizeof(type), flags           \
                        GASNETI_THREAD_PASS);                     \
    } \
    GASNETI_INLINE(prefix##_NBI_S1)                               \
    int prefix##_NBI_S1(                                          \
                gasneti_AD_t         ad,                          \
                gex_Rank_t     tgt_rank,    void       *tgt_addr, \
                type           operand1,                          \
                gex_Flags_t      flags      GASNETI_THREAD_FARG)  \
    {                                                             \
        gasnete_amratomic_op_t rop = gasnete_amratomic_op_alloc();\
        rop->result_p = NULL;                                     \
        rop->eop = NULL;                                          \
        rop->iop = gasneti_iop_register(1,0 GASNETI_THREAD_PASS); \
        rop->iop_type = GEX_EC_PUT;                               \
        gex_TM_t tm = gasnete_ratomic_e_tm(ad, flags);            \
        const gasneti_op_idx_t op_idx = gasneti_op_idx_SET;       \
        gex_Flags_t am_flags = flags & GEX_FLAG_IMMEDIATE;        \
        int imm = gex_AM_RequestMedium(tm, tgt_rank, gasneti_handleridx(gasnete_amratomic_reqh), \
                                       &operand1, sizeof(type), GEX_EVENT_NOW, am_flags,         \
                                       (op_idx | (datatype << 8)), PACK(tgt_addr), PACK(rop));   \
        if (imm) {                                                \
            gasneti_iop_markdone(rop->iop,1,0);                   \
            gasnete_amratomic_op_free(rop);                       \
        }                                                         \
        return imm;                                               \
    } \
    GASNETI_INLINE(prefix##_NBI_G0)                               \
    int prefix##_NBI_G0(                                          \
                gasneti_AD_t         ad,                          \
                type          *result_p,                          \
                gex_Rank_t     tgt_rank,    void       *tgt_addr, \
                gex_Flags_t      flags      GASNETI_THREAD_FARG)  \
    {                                                             \
        gasnete_amratomic_op_t rop = gasnete_amratomic_op_alloc();\
        rop->result_p = result_p;                                 \
        rop->eop = NULL;                                          \
        rop->iop = gasneti_iop_register(1,1 GASNETI_THREAD_PASS); \
        rop->iop_type = GEX_EC_GET;                               \
        gex_TM_t tm = gasnete_ratomic_e_tm(ad, flags);            \
        const gasneti_op_idx_t op_idx = gasneti_op_idx_GET;       \
        gex_Flags_t am_flags = flags & GEX_FLAG_IMMEDIATE;        \
        int imm = gex_AM_RequestMedium(tm, tgt_rank, gasneti_handleridx(gasnete_amratomic_reqh), \
                                       NULL, 0, GEX_EVENT_NOW, am_flags,                         \
                                       (op_idx | (datatype << 8)), PACK(tgt_addr), PACK(rop));   \
        if (imm) {                                                \
            gasneti_iop_markdone(rop->iop,1,1);                   \
            gasnete_amratomic_op_free(rop);                       \
        }                                                         \
        return imm;                                               \
    }
//
GASNETE_DT_APPLY(GASNETE_AMRATOMIC_MID_NB)
GASNETE_DT_APPLY(GASNETE_AMRATOMIC_MID_NBI)

//
// Ratomic functions (called by top-level dispatch functions) that then
// call the functions defined by the GASNETE_AMRATOMIC_MID macro, above.
//
#define GASNETE_AMRATOMIC_DEFS(dtcode) \
        _GASNETE_AMRATOMIC_DEFS1(dtcode, dtcode##_isint)
// This extra pass expands the "isint" token prior to additional concatenation
#define _GASNETE_AMRATOMIC_DEFS1(dtcode, isint) \
        _GASNETE_AMRATOMIC_DEFS2(dtcode, isint)
#define _GASNETE_AMRATOMIC_DEFS2(dtcode, isint) \
    _GASNETE_AMRATOMIC_DEFN_INT##isint(dtcode,AND,1) \
    _GASNETE_AMRATOMIC_DEFN_INT##isint(dtcode,OR,1)  \
    _GASNETE_AMRATOMIC_DEFN_INT##isint(dtcode,XOR,1) \
    _GASNETE_AMRATOMIC_DEFN2(dtcode,ADD,1)           \
    _GASNETE_AMRATOMIC_DEFN2(dtcode,SUB,1)           \
    _GASNETE_AMRATOMIC_DEFN2(dtcode,MULT,1)          \
    _GASNETE_AMRATOMIC_DEFN2(dtcode,MIN,1)           \
    _GASNETE_AMRATOMIC_DEFN2(dtcode,MAX,1)           \
    _GASNETE_AMRATOMIC_DEFN2(dtcode,INC,0)           \
    _GASNETE_AMRATOMIC_DEFN2(dtcode,DEC,0)           \
    _GASNETE_AMRATOMIC_SETGET(dtcode)                \
    _GASNETE_AMRATOMIC_DEFN1(dtcode,SWAP,F1)         \
    _GASNETE_AMRATOMIC_DEFN1(dtcode,FCAS,F2)         \
    _GASNETE_AMRATOMIC_DEFN1(dtcode,CAS,N2)
//
#define _GASNETE_AMRATOMIC_DEFN_INT0(dtcode,opname,nargs) /*empty*/
#define _GASNETE_AMRATOMIC_DEFN_INT1 _GASNETE_AMRATOMIC_DEFN2
#define _GASNETE_AMRATOMIC_DEFN2(dtcode,opstem,nargs) \
        _GASNETE_AMRATOMIC_DEFN1(dtcode,opstem,N##nargs) \
        _GASNETE_AMRATOMIC_DEFN1(dtcode,F##opstem,F##nargs)
#define _GASNETE_AMRATOMIC_DEFN1(dtcode,opname,args) \
        _GASNETE_AMRATOMIC_DEFN1_NB(dtcode,opname,args) \
        _GASNETE_AMRATOMIC_DEFN1_NBI(dtcode,opname,args)
#define _GASNETE_AMRATOMIC_DEFN1_NB(dtcode,opname,args) \
  static gex_Event_t gasnete_amratomic##dtcode##_NB_##opname(GASNETE_RATOMIC_ARGS_##args(dtcode##_type)) { \
    return gasnete_amratomic##dtcode##_NB_##args(gasneti_op_idx_##opname,GASNETE_RATOMIC_PASS_##args);     \
  }
#define _GASNETE_AMRATOMIC_DEFN1_NBI(dtcode,opname,args) \
  static int gasnete_amratomic##dtcode##_NBI_##opname(GASNETE_RATOMIC_ARGS_##args(dtcode##_type)) {     \
    return gasnete_amratomic##dtcode##_NBI_##args(gasneti_op_idx_##opname,GASNETE_RATOMIC_PASS_##args); \
  }
#define _GASNETE_AMRATOMIC_SETGET(dtcode) \
        _GASNETE_AMRATOMIC_SETGET1(dtcode, GASNETE_AMRATOMIC_USE_RMA##dtcode)
// This extra pass expands the "use_rma" token prior to additional concatenation
#define _GASNETE_AMRATOMIC_SETGET1(dtcode, use_rma) \
        _GASNETE_AMRATOMIC_SETGET2(dtcode, use_rma)
#define _GASNETE_AMRATOMIC_SETGET2(dtcode, use_rma) \
        _GASNETE_AMRATOMIC_SETGET_RMA##use_rma(dtcode)
#define _GASNETE_AMRATOMIC_SETGET_RMA0(dtcode) /* Use AM for SET and GET */ \
  /* NB are same as other ops, but NBI are specialized for distinct gex_EC_t */ \
  _GASNETE_AMRATOMIC_DEFN1_NB(dtcode,SET,N1) \
  static int gasnete_amratomic##dtcode##_NBI_SET(GASNETE_RATOMIC_ARGS_N1(dtcode##_type)) {     \
    return gasnete_amratomic##dtcode##_NBI_S1(GASNETE_RATOMIC_PASS_N1); \
  } \
  _GASNETE_AMRATOMIC_DEFN1_NB(dtcode,GET,F0) \
  static int gasnete_amratomic##dtcode##_NBI_GET(GASNETE_RATOMIC_ARGS_F0(dtcode##_type)) {     \
    return gasnete_amratomic##dtcode##_NBI_G0(GASNETE_RATOMIC_PASS_F0); \
  }
#define _GASNETE_AMRATOMIC_SETGET_RMA1(dtcode) /* Use RMA for SET and GET */ \
        _GASNETE_AMRATOMIC_SETGET_RMA2(dtcode, dtcode##_type, dtcode##_bits)
// This extra pass expands the "bits" token prior to additional concatenation
#define _GASNETE_AMRATOMIC_SETGET_RMA2(dtcode, type, bits) \
        _GASNETE_AMRATOMIC_SETGET_RMA3(dtcode, type, bits)
#define _GASNETE_AMRATOMIC_SETGET_RMA3(dtcode, type, bits) \
  static gex_Event_t gasnete_amratomic##dtcode##_NB_SET (GASNETE_RATOMIC_ARGS_N1(type)) { \
    union { uint##bits##_t uint; type op1; } u; u.op1 = _operand1;                        \
    gex_TM_t tm = gasnete_ratomic_e_tm(_real_ad, _flags);                                 \
    return gex_RMA_PutNBVal(tm, _tgt_rank, _tgt_addr,                                     \
                            u.uint, sizeof(type), _flags);                                \
  } \
  static int gasnete_amratomic##dtcode##_NBI_SET (GASNETE_RATOMIC_ARGS_N1(type)) {        \
    union { uint##bits##_t uint; type op1; } u; u.op1 = _operand1;                        \
    gex_TM_t tm = gasnete_ratomic_e_tm(_real_ad, _flags);                                 \
    return gex_RMA_PutNBIVal(tm, _tgt_rank, _tgt_addr,                                    \
                             u.uint, sizeof(type), _flags);                               \
  } \
  static gex_Event_t gasnete_amratomic##dtcode##_NB_GET (GASNETE_RATOMIC_ARGS_F0(type)) { \
    gex_TM_t tm = gasnete_ratomic_e_tm(_real_ad, _flags);                                 \
    return gex_RMA_GetNB(tm, _result_p,                                                   \
                         _tgt_rank, _tgt_addr, sizeof(type), _flags);                     \
  } \
  static int gasnete_amratomic##dtcode##_NBI_GET (GASNETE_RATOMIC_ARGS_F0(type)) {        \
    gex_TM_t tm = gasnete_ratomic_e_tm(_real_ad, _flags);                                 \
    return gex_RMA_GetNBI(tm, _result_p,                                                  \
                          _tgt_rank, _tgt_addr, sizeof(type), _flags);                    \
  }
//
GASNETE_DT_APPLY(GASNETE_AMRATOMIC_DEFS)

#else // GASNETE_BUILD_AMRATOMIC_STUBS.  So define stubs that fatalerror

#define GASNETE_AMRATOMIC_STUBS(dtcode) \
        _GASNETE_AMRATOMIC_STUBS1(dtcode, dtcode##_isint)
// This extra pass expands the "isint" token prior to additional concatenation
#define _GASNETE_AMRATOMIC_STUBS1(dtcode, isint) \
        _GASNETE_AMRATOMIC_STUBS2(dtcode, isint)
#define _GASNETE_AMRATOMIC_STUBS2(dtcode, isint) \
    _GASNETE_AMRATOMIC_STUB_INT##isint(dtcode,AND,1) \
    _GASNETE_AMRATOMIC_STUB_INT##isint(dtcode,OR,1)  \
    _GASNETE_AMRATOMIC_STUB_INT##isint(dtcode,XOR,1) \
    _GASNETE_AMRATOMIC_STUB2(dtcode,ADD,1)           \
    _GASNETE_AMRATOMIC_STUB2(dtcode,SUB,1)           \
    _GASNETE_AMRATOMIC_STUB2(dtcode,MULT,1)          \
    _GASNETE_AMRATOMIC_STUB2(dtcode,MIN,1)           \
    _GASNETE_AMRATOMIC_STUB2(dtcode,MAX,1)           \
    _GASNETE_AMRATOMIC_STUB2(dtcode,INC,0)           \
    _GASNETE_AMRATOMIC_STUB2(dtcode,DEC,0)           \
    _GASNETE_AMRATOMIC_STUB1(dtcode,SET,N1)          \
    _GASNETE_AMRATOMIC_STUB1(dtcode,GET,F0)          \
    _GASNETE_AMRATOMIC_STUB1(dtcode,SWAP,F1)         \
    _GASNETE_AMRATOMIC_STUB1(dtcode,FCAS,F2)         \
    _GASNETE_AMRATOMIC_STUB1(dtcode,CAS,N2)
//
#define _GASNETE_AMRATOMIC_STUB_INT0(dtcode,opname,nargs) /*empty*/
#define _GASNETE_AMRATOMIC_STUB_INT1 _GASNETE_AMRATOMIC_STUB2
#define _GASNETE_AMRATOMIC_STUB2(dtcode,opstem,nargs) \
        _GASNETE_AMRATOMIC_STUB1(dtcode,opstem,N##nargs) \
        _GASNETE_AMRATOMIC_STUB1(dtcode,F##opstem,F##nargs)
#define _GASNETE_AMRATOMIC_STUB1(dtcode,opname,args) \
        _GASNETE_AMRATOMIC_STUB1_NB(dtcode,opname,args) \
        _GASNETE_AMRATOMIC_STUB1_NBI(dtcode,opname,args)
#define _GASNETE_AMRATOMIC_STUB1_NB(dtcode,opname,args) \
  static gex_Event_t gasnete_amratomic##dtcode##_NB_##opname(GASNETE_RATOMIC_ARGS_##args(dtcode##_type)) { \
    gasneti_fatalerror("gex_AD_OpNB_" dtcode##_suff "() is not implemented on this platform.\n"    \
                       "Please see Bug 3727 under Known Problems in GASNet's smp-conduit README."); \
    return GEX_EVENT_INVALID; \
  }
#define _GASNETE_AMRATOMIC_STUB1_NBI(dtcode,opname,args) \
  static int gasnete_amratomic##dtcode##_NBI_##opname(GASNETE_RATOMIC_ARGS_##args(dtcode##_type)) { \
    gasneti_fatalerror("gex_AD_OpNBI_" dtcode##_suff "() is not implemented on this platform.\n"    \
                       "Please see Bug 3727 under Known Problems in GASNet's smp-conduit README.");  \
    return 0; \
  }
#define _gex_dt_I32_suff "I32"
#define _gex_dt_U32_suff "U32"
#define _gex_dt_I64_suff "I64"
#define _gex_dt_U64_suff "U64"
#define _gex_dt_FLT_suff "FLT"
#define _gex_dt_DBL_suff "DBL"
//
GASNETE_DT_APPLY(GASNETE_AMRATOMIC_STUBS)

#endif

//
// Build the dispatch tables
//
#define GASNETE_AMRATOMIC_TBL(dtcode) \
    gasnete_ratomic##dtcode##_fn_tbl_t gasnete_amratomic##dtcode##_fn_tbl = \
        GASNETE_RATOMIC_FN_TBL_INIT(gasnete_amratomic##dtcode,dtcode);
GASNETE_DT_APPLY(GASNETE_AMRATOMIC_TBL)

//
// Create-hook to install the dispatch tables
//
void gasnete_amratomic_create_hook(
        gasneti_AD_t               real_ad,
        gasneti_TM_t               real_tm,
        gex_DT_t                   dt,
        gex_OP_t                   ops,
        gex_Flags_t                flags)
{
    real_ad->_tools_safe = 1;
    #define GASNETE_AMRATOMIC_TBL_CASE(dtcode) \
        case dtcode##_dtype: \
            real_ad->_fn_tbl = (gasnete_ratomic_fn_tbl_t) &gasnete_amratomic##dtcode##_fn_tbl; \
            break;
    switch (dt) {
        GASNETE_DT_APPLY(GASNETE_AMRATOMIC_TBL_CASE)
        default: gasneti_unreachable();
    }
    #undef GASNETE_AMRATOMIC_TBL_CASE

#if GASNETE_BUILD_AMRATOMIC_STUBS
    GASNETI_TRACE_PRINTF(C,("gex_AD_Create(dt=%d, ops=0x%x) -> AM_stubs", (int)dt, (unsigned int)ops));
#else
    GASNETI_TRACE_PRINTF(C,("gex_AD_Create(dt=%d, ops=0x%x) -> AM", (int)dt, (unsigned int)ops));
#endif
}

#endif // GASNETE_BUILD_AMRATOMIC
/*---------------------------------------------------------------------------------*/

#endif // _GEX_AD_T
