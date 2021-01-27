/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_reduce.c $
 * Description: Reference implemetation of GASNet-EX Reductions
 * Copyright (c) 2018 The Regents of the University of California.
 * Terms of use are as specified in license.txt
 */

#include <coll/gasnet_coll_internal.h>
#include <coll/gasnet_scratch.h>
#include <coll/gasnet_trees.h>
#include <coll/gasnet_autotune_internal.h>

/*---------------------------------------------------------------------------------*/

// TODO-EX: factor the following, which is common to Reduce and Atomics
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

/*---------------------------------------------------------------------------------*/

// Macros for built-in opcodes:
#define GASNETE_REDUCE_OP_ADD(a,b)  (a + b)
#define GASNETE_REDUCE_OP_MULT(a,b) (a * b)
#define GASNETE_REDUCE_OP_AND(a,b)  (a & b)
#define GASNETE_REDUCE_OP_OR(a,b)   (a | b)
#define GASNETE_REDUCE_OP_XOR(a,b)  (a ^ b)
#define GASNETE_REDUCE_OP_MIN(a,b)  MIN(a, b)
#define GASNETE_REDUCE_OP_MAX(a,b)  MAX(a, b)

// GASNETE_REDUCE_OP_APPLY(dtcode,FN)
//
// This macro expands to
//    FN(dtcode,opname)
// repeated for all reduce op valid for dtcode.
// opname is the portion following 'GEX_OP_'.
#define GASNETE_REDUCE_OP_APPLY(dtcode,FN) \
       _GASNETE_REDUCE_OP_APPLY1(dtcode,dtcode##_isint,FN)
// This extra pass expands the "isint" token prior to additional concatenation
#define _GASNETE_REDUCE_OP_APPLY1(dtcode,isint,FN) \
        _GASNETE_REDUCE_OP_APPLY2(dtcode,isint,FN)
#define _GASNETE_REDUCE_OP_APPLY2(dtcode,isint,FN) \
  FN(dtcode,ADD) FN(dtcode,MULT) FN(dtcode,MIN) FN(dtcode,MAX) \
  GASNETE_REDUCE_OP_APPLY_INT##isint(dtcode,FN)
#define GASNETE_REDUCE_OP_APPLY_INT0(dtcode,FN) /*empty*/
#define GASNETE_REDUCE_OP_APPLY_INT1(dtcode,FN) \
  FN(dtcode,AND) FN(dtcode,OR) FN(dtcode,XOR)

/*---------------------------------------------------------------------------------*/
// "Shrink ray" - reduces its targets
//
// TODO-EX: replace this switch-intensive implementation.
// The cringe-worthy name is intended to encourage a short lifetime.

#define GASNETE_SHRINKRAY_CASE(dtcode,opname) \
    case GEX_OP_##opname:                                 \
      for (size_t i = 0; i < count; ++i) {                \
         y[i] = GASNETE_REDUCE_OP_##opname(x[i], y[i]);   \
      }                                                   \
      break;
#define GASNETE_SHRINKRAY_DEFN(dtcode) \
void gasnete_shrinkray##dtcode (                            \
            const void * op1,                               \
            void *       op2_and_out,                       \
            size_t       count,                             \
            const void * cdata)                             \
{                                                           \
  const gex_OP_t opcode = (gex_OP_t)(uintptr_t)cdata;       \
  const dtcode##_type * GASNETI_RESTRICT x = op1;           \
  dtcode##_type * GASNETI_RESTRICT y = op2_and_out;         \
  switch (opcode) {                                         \
    GASNETE_REDUCE_OP_APPLY(dtcode, GASNETE_SHRINKRAY_CASE) \
    default: gasneti_unreachable();                         \
  }                                                         \
}
GASNETE_DT_APPLY(GASNETE_SHRINKRAY_DEFN)
#undef GASNETE_SHRINKRAY_CASE
#undef GASNETE_SHRINKRAY_DEFN

/*---------------------------------------------------------------------------------*/

// TODO-EX: can perform fewer (log(child_cnt)) calls w/ longer counts
GASNETI_INLINE(local_reduce_helper)
void *local_reduce_helper(
                        const gasnete_tm_reduce_args_t *args,
                        size_t dt_cnt, size_t stride,
                        gex_Rank_t child_cnt,
                        const void *src, void *buffer)
{
  gex_Coll_ReduceFn_t const op_fnptr = args->op_fnptr;
  void * const op_cdata = args->op_cdata;
  const void *prev = src;
  void *curr = buffer;
  for (gex_Rank_t r = 0; r < child_cnt; ++r) {
    (*op_fnptr)(prev, curr, dt_cnt, op_cdata);
    prev = curr;
    curr = (void*)(stride + (uintptr_t)curr);
  }
  gasneti_assert(!child_cnt || prev == gasnete_coll_scale_ptr(buffer, child_cnt-1, stride));
  return (/*non const*/ void*)prev;
}

/*---------------------------------------------------------------------------------*/

// GEX Reduce-to-one via Eager messages on a binomial tree
static int gasnete_coll_pf_tm_reduce_BinomialEager(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gex_TM_t const tm = op->e_tm;
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_tm_reduce_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, tm_reduce);
  gasnete_coll_p2p_t *p2p = data->p2p;
  gex_Flags_t flags = 0;
  void *payload;
  int result = 0;

  // TODO-EX: pre-compute quantities such as these and (dt_sz*dt_cnt) once
  //          at injection, rather than repeatedly upon every poll.
  gex_Rank_t rel_rank = gasnete_tm_binom_rel_root(tm, args->root);
  gex_Rank_t child_cnt = gasnete_tm_binom_children(tm, rel_rank);

  gasneti_assert(p2p != NULL);
  gasneti_assert(p2p->state != NULL);
  gasneti_assert(p2p->data != NULL);
  
  switch (data->state) {
    case 0: {   // Wait for arrival of data from children, if any
      volatile uint32_t *state = p2p->state;
      for (gex_Rank_t r = 0; r < child_cnt; ++r) {
        if (! state[r]) return 0; // At least one child has not contributed their value
      } 
      gasneti_sync_reads();
      data->state = 1; GASNETI_FALLTHROUGH
    }
      
    case 1: {
      const size_t nbytes = args->dt_sz * args->dt_cnt; // TODO-EX: compute *once*

      // Compute reduction (if any)
      if (child_cnt) {
        payload = local_reduce_helper(args, args->dt_cnt, nbytes, child_cnt, args->src, p2p->data);
      } else {
        payload = (/*non-const*/ void*) args->src;
      }

      // Data movement, either local or first try to parent
      if (! rel_rank) { // I am root
        GASNETI_MEMCPY(args->dst, payload, nbytes);
        goto done;
      }
      flags = GEX_FLAG_IMMEDIATE;
      data->private_data = payload;
      data->state = 2; GASNETI_FALLTHROUGH
    }

    case 2: {   // Data movement to parent (IMM on first try only)
      const size_t nbytes = args->dt_sz * args->dt_cnt;
      gex_Rank_t parent = gasnete_tm_binom_parent(tm, rel_rank);
      gex_Rank_t offset = gasnete_tm_binom_age(tm, rel_rank);
      payload = data->private_data;
      // TODO-EX: use lc_opt for async injection
      if (gasnete_tm_p2p_eager_put(op, parent, payload, nbytes,
                                   GEX_EVENT_NOW, flags, offset, 1
                                   GASNETI_THREAD_PASS)) {
        break; // back pressure
      }
    }

    done:
      // Done
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      break;

    default: gasneti_unreachable();
  } // end switch
  
  return result;
}

GASNETE_TM_DECLARE_REDUCE_ALG(BinomialEager)
{
#if GASNET_DEBUG // make sure this is a valid choice of algorithm
  gex_Rank_t rel_rank = gasnete_tm_binom_rel_root(tm, root);
  gex_Rank_t child_cnt = gasnete_tm_binom_children(tm, rel_rank);
  gasnet_team_handle_t team = gasneti_import_tm(tm)->_coll_team;
  gasneti_assert(team->p2p_eager_buffersz >= dt_sz * dt_cnt * child_cnt);
  gasneti_assert(gex_AM_LUBRequestMedium() >= dt_sz * dt_cnt );
#endif

  const int options = GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
  return gasnete_tm_generic_reduce_nb(tm, root, dst, src, dt, dt_sz, dt_cnt,
                                      op, op_fnptr, op_cdata, coll_flags,
                                      &gasnete_coll_pf_tm_reduce_BinomialEager,
                                      options, NULL, sequence, 0, NULL, NULL
                                      GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/

// GEX Reduce-to-one via Eager messages on a binomial tree - SEGMENTED
// Multiple rounds, each processing as many elements as possible
static int gasnete_coll_pf_tm_reduce_BinomialEagerSeg(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gex_TM_t const tm = op->e_tm;
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_tm_reduce_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, tm_reduce);
  gasnete_coll_p2p_t *p2p = data->p2p;
  gex_Flags_t flags = 0;
  int result = 0;

  struct pdata {
    void *      payload;
    size_t      chunk_cnt; // elems
    size_t      chunk_len; // bytes
    size_t      curr_cnt;  // elems
    size_t      curr_len;  // bytes
    size_t      offset;    // bytes
    size_t      remain;    // elems
    gex_Rank_t  width;
    gex_Rank_t  rel_rank;
    gex_Rank_t  child_cnt;
    gex_Rank_t  parent;
    gex_Rank_t  age;
    int         phase;
    int         last;
  } *pdata;

  if (data->state) {
    pdata = data->private_data;
  } else {
    // Allocate and initialize pdata
    gasneti_assert(!data->private_data);
    pdata = gasneti_calloc(1, sizeof(struct pdata));
    data->private_data = pdata;

    pdata->rel_rank  = gasnete_tm_binom_rel_root(tm, args->root);
    pdata->child_cnt = gasnete_tm_binom_children(tm, pdata->rel_rank);
    pdata->parent    = gasnete_tm_binom_parent(tm, pdata->rel_rank);
    pdata->age       = gasnete_tm_binom_age(tm, pdata->rel_rank);

    pdata->width      = 1 + gasnete_coll_log2_rank(gasneti_import_tm(tm)->_size - 1);

    pdata->chunk_cnt = MIN(op->team->p2p_eager_buffersz / pdata->width,
                           gex_AM_LUBRequestMedium()) / args->dt_sz;
    pdata->chunk_len = pdata->chunk_cnt * args->dt_sz;
    pdata->curr_cnt  = pdata->chunk_cnt;
    pdata->curr_len  = pdata->chunk_len;
    pdata->remain    = args->dt_cnt;

    pdata->phase     = 1;
    
    if (!pdata->rel_rank) {
      // check if we  should be using non-segmented version
      gasneti_assert_uint(pdata->chunk_cnt ,<, args->dt_cnt);
    }
    gasneti_assert(pdata->chunk_cnt != 0);
    gasneti_assert(pdata->offset == 0);
    gasneti_assert(pdata->last == 0);

    p2p->state[pdata->width] = 1; // Initial CTS
    data->state = 1;
  }

  gasneti_assert(p2p != NULL);
  gasneti_assert(p2p->state != NULL);
  gasneti_assert(p2p->data != NULL);
  
  switch (data->state) {
    // case 0: pdata allocation/initialization, above

    case 1: {   // Wait for arrival of data from children, if any
      const int ready = pdata->phase;
      volatile uint32_t *state = p2p->state;
      for (gex_Rank_t r = 0; r < pdata->child_cnt; ++r) {
        if (state[r] != ready) return 0; // At least one child has not contributed their value
      } 
      gasneti_sync_reads();
      pdata->last = (pdata->remain <= pdata->chunk_cnt);
      if (pdata->last) {
        pdata->curr_cnt = pdata->remain;
        pdata->curr_len = pdata->remain * args->dt_sz;
      }
      data->state = 2; GASNETI_FALLTHROUGH
    }
      
    case 2: {
      // Compute reduction (if any)
      const void *src = (const void *)(pdata->offset + (uintptr_t)args->src);
      if (pdata->child_cnt) {
        pdata->payload = local_reduce_helper(args, pdata->curr_cnt, pdata->curr_len,
                                             pdata->child_cnt, src, p2p->data);
      } else {
        pdata->payload = (/*non-const*/ void*) src;
      }
      data->state = 3; GASNETI_FALLTHROUGH
    }

    case 3:
      if (! pdata->rel_rank) { // I am root
        void *dst = (void *)(pdata->offset + (uintptr_t)args->dst);
        GASNETI_MEMCPY(dst, pdata->payload, pdata->curr_len);
      } else {
        // Stall for parent's clear-to-send
        if (p2p->state[pdata->width] != pdata->phase) break;
      }
      flags |= GEX_FLAG_IMMEDIATE; // We fall through to the first attempt at comms
      data->state = 4; GASNETI_FALLTHROUGH

    case 4: {
      // Send partial result to parent (if any)
      if (pdata->rel_rank) { // NOT root
        if (gasnete_tm_p2p_eager_put(op, pdata->parent, pdata->payload, pdata->curr_len,
                                     GEX_EVENT_NOW, flags, pdata->age, pdata->phase
                                     GASNETI_THREAD_PASS)) {
          break; // back pressure
        }
      }

      int comms_done = 1;
      // Send CTS message to children (if any) unless at end
      if (pdata->child_cnt && !pdata->last) {
        volatile uint32_t *state = p2p->state;
        const gex_Rank_t self = gex_TM_QueryRank(tm);
        const gex_Rank_t size = gex_TM_QuerySize(tm);
        int next_phase = pdata->phase ^ 1;
        for (int idx = pdata->child_cnt - 1; idx >= 0; --idx) { // Reverse order for deepest subtree first
          gex_Rank_t distance = 1 << idx;
          gex_Rank_t peer = (distance >= size - self) ? self - (size - distance) : self + distance;
          if (gasnete_tm_p2p_change_state(op, peer, flags, pdata->width,
                                          next_phase GASNETI_THREAD_PASS)) {
            state[idx] = 2;  // mark for retry
            comms_done = 0;
          }
        }
      }

      if (comms_done) goto comms_done; // Skip non-IMM comms

      // Yield.  Control will resume at non-IMMEDIATE comms
      gasneti_assert(! result);
      data->state = 5;
      break;
    }

    case 5:
      // Second try (NON immediate) to send CTS to any children
      gasneti_assert(! (flags & GEX_FLAG_IMMEDIATE));
      if (pdata->child_cnt && !pdata->last) {
        volatile uint32_t *state = p2p->state;
        const gex_Rank_t self = gex_TM_QueryRank(tm);
        const gex_Rank_t size = gex_TM_QuerySize(tm);
        int next_phase = pdata->phase ^ 1;
        for (int idx = pdata->child_cnt - 1; idx >= 0; --idx) { // Reverse order for deepest subtree first
          gex_Rank_t distance = 1 << idx;
          gex_Rank_t peer = (distance >= size - self) ? self - (size - distance) : self + distance;
          if (state[idx] == 2) {
            gasnete_tm_p2p_change_state(op, peer, flags, pdata->width,
                                        next_phase GASNETI_THREAD_PASS);
          }
        }
      }

      comms_done:
      // Comms are done, reduction might be too
      if (pdata->last) {
        result = GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE;
        goto done;
      }

      // Advance phase, offset and remain for next iter
      pdata->phase ^= 1;
      pdata->offset += pdata->curr_len;
      pdata->remain -= pdata->curr_cnt;
      gasneti_assert(pdata->remain);

      // Yield.  Control will resume at next iteration.
      gasneti_assert(! result);
      data->state = 1;
      break;

    done:
      // Done
      gasneti_free(pdata);
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      break;

    default: gasneti_unreachable_error(("Unknown state: %i",(int)data->state));
  } // end switch
  
  return result;
}

GASNETE_TM_DECLARE_REDUCE_ALG(BinomialEagerSeg)
{
#if GASNET_DEBUG // make sure this is a valid choice of algorithm
  gex_Rank_t rel_rank = gasnete_tm_binom_rel_root(tm, root);
  gex_Rank_t child_cnt = gasnete_tm_binom_children(tm, rel_rank);
  gasnet_team_handle_t team = gasneti_import_tm(tm)->_coll_team;
  gasneti_assert(team->p2p_eager_buffersz >= dt_sz * child_cnt);
  gasneti_assert(gex_AM_LUBRequestMedium() >= dt_sz);
#endif

  const int options = GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
  return gasnete_tm_generic_reduce_nb(tm, root, dst, src, dt, dt_sz, dt_cnt,
                                      op, op_fnptr, op_cdata, coll_flags,
                                      &gasnete_coll_pf_tm_reduce_BinomialEagerSeg,
                                      options, NULL, sequence, 0, NULL, NULL
                                      GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/

// GEX Reduce-to-one via Long AMs into scratch space on a tree
static int gasnete_coll_pf_tm_reduce_TreePut(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gex_TM_t const tm = op->e_tm;
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_tm_reduce_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, tm_reduce);
  gasnete_coll_p2p_t *p2p = data->p2p;
  gex_Flags_t flags = 0; // TODO-EX: GEX_FLAG_SELF_SEG_SOME (scratch resides in client or aux seg)
  void *payload;
  int result = 0;

  gasneti_assert(p2p != NULL);
  gasneti_assert(p2p->state != NULL);
  gasneti_assert(op->scratch_req);

  gasnete_coll_team_t team = op->team;
  gasnete_coll_local_tree_geom_t *geom = data->tree_geom;
  const gex_Rank_t child_cnt = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom);
  const gex_Rank_t myrank = gex_TM_QueryRank(tm);
  
  switch (data->state) {
    case 0:     // Wait for scratch allocation
      if (!gasnete_coll_scratch_alloc_nb(op GASNETI_THREAD_PASS)) {
        break;
      }
      data->state = 1; GASNETI_FALLTHROUGH

    case 1: {   // Wait for arrival of data from children, if any
      volatile uint32_t *state = p2p->state;
      for (gex_Rank_t r = 0; r < child_cnt; ++r) {
        if (! state[r]) return 0; // At least one child has not contributed their value
      } 
      gasneti_sync_reads();
      data->state = 2; GASNETI_FALLTHROUGH
    }
      
    case 2: {
      const size_t nbytes = args->dt_sz * args->dt_cnt; // TODO-EX: compute *once*

      // Compute reduction (if any)
      if (child_cnt) {
        void *myscratch = gasnete_coll_scratch_myaddr(op, 0);
        payload = local_reduce_helper(args, args->dt_cnt, nbytes, child_cnt, args->src, myscratch);
      } else {
        payload = (/*non-const*/ void*) args->src;
      }

      // Data movement, either local or first try to parent
      if (myrank == args->root) {
        GASNETI_MEMCPY(args->dst, payload, nbytes);
        goto done;
      }
      flags |= GEX_FLAG_IMMEDIATE;
      data->private_data = payload;
      data->state = 3; GASNETI_FALLTHROUGH
    }

    case 3: {   // Data movement to parent (IMM on first try only)
      const size_t nbytes = args->dt_sz * args->dt_cnt; // TODO: compute *once*
      const gex_Rank_t parent = GASNETE_COLL_TREE_GEOM_PARENT(geom);
      const gex_Rank_t offset = GASNETE_COLL_TREE_GEOM_SIBLING_ID(geom);
      void* parent_scratch = gasnete_coll_scratch_addr(op, parent, 0, 0);
      void* destaddr = gasnete_coll_scale_ptr(parent_scratch, offset, nbytes);
      payload = data->private_data;
      // TODO-EX: use lc_opt for async injection
      if (gasnete_tm_p2p_signalling_put(op, parent,
                                        destaddr, payload, nbytes,
                                        GEX_EVENT_NOW, flags, offset, 1
                                        GASNETI_THREAD_PASS)) {
        break; // back pressure
      }
    }

    done:
      // Done
      gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      break;

    default: gasneti_unreachable();
  } // end switch

  return result;
}

GASNETE_TM_DECLARE_REDUCE_ALG(TreePut)
{
  const size_t nbytes = dt_sz * dt_cnt; // TODO-EX: compute this only *once*
  gasnet_team_handle_t team = gasneti_import_tm(tm)->_coll_team;

  gasneti_assert(coll_params);
  gasnete_coll_local_tree_geom_t *geom = (gasnete_coll_local_tree_geom_t *)coll_params;

  // make sure this is a valid choice of algorithm
  gasneti_assert(team->scratch_size >= nbytes * geom->max_radix);
  gasneti_assert(gex_AM_LUBRequestLong() >= nbytes);

  // Scratch space
  gasnete_coll_scratch_req_t *scratch_req = gasnete_coll_scratch_alloc_req(team);
  // fill out the tree information
  scratch_req->tree_type = geom->tree_type;
  scratch_req->tree_dir = GASNETE_COLL_UP_TREE;
  scratch_req->root = root;
  scratch_req->op_type = GASNETE_COLL_TREE_OP;
  // fill out the peer information
  //  in: recv 'nbytes' from each child
  //  out: self and siblings each send 'nbytes' to parent
  scratch_req->incoming_size = nbytes * GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom);
  scratch_req->num_in_peers = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom);
  scratch_req->in_peers = GASNETE_COLL_TREE_GEOM_CHILDREN(geom);      
  if (team->myrank == root) {
    scratch_req->num_out_peers = 0;
    scratch_req->out_peers = NULL;
    scratch_req->out_sizes = NULL;
  } else {
    scratch_req->num_out_peers = 1;
    gasnete_coll_scratch_alloc_out_sizes(scratch_req, 1);
    scratch_req->out_sizes[0] = nbytes * geom->num_siblings;
    scratch_req->out_peers = &(GASNETE_COLL_TREE_GEOM_PARENT(geom));
  }

  const int options = GASNETE_COLL_GENERIC_OPT_P2P | GASNETE_COLL_USE_SCRATCH;
  return gasnete_tm_generic_reduce_nb(tm, root, dst, src, dt, dt_sz, dt_cnt,
                                      op, op_fnptr, op_cdata, coll_flags,
                                      &gasnete_coll_pf_tm_reduce_TreePut,
                                      options, geom, sequence, 0, NULL, scratch_req
                                      GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/

// GEX Reduce-to-one via Long AMs into scratch space on a tree - SEGMENTED
// Multiple rounds, each processing as many elements as possible
//
// Sketch of algorithm:
// Each iteration reduces chunk_cnt elements, with data transmitted up a tree
// to buffers in the scratch space using Longs to signal arrivals.  To help
// avoid stalling the pipeline, the send of data up the tree is done with
// lc_opt = &ev.  However, to avoid making a copy, this asynchrony ties up a
// buffer that would otherwise be the target of incoming data from the last
// child (where the local reduce "lands").  Therefore, we have allocated
// scratch space for one more buffer than we have children, and use it to
// "double buffer" the contribution of the last child (alternating between use
// of the last two buffers).  This wrinkle is hidden somewhat in the
// flow-control, which sends a clear-to-send message from the parent to child
// which names the slot the next Long should target.
//
// state[] used for flow-control:
//   state[0] holds clear-to-send from our parent, if any
//            GEX_RANK_INVALID marks the not-ready state
//            otherwise holds the offset (in units of chunk_len) to Put to
//   state[i] for (i > 0) is arrival indication from child i-1
//            value (phase^1) indicates ready state
//            value 2 used to mark children which have not been sent CTS
//
// All comms are attempted once with GEX_FLAG_IMMEDIATE.
static int gasnete_coll_pf_tm_reduce_TreePutSeg(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_tm_reduce_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, tm_reduce);
  gasnete_coll_p2p_t *p2p = data->p2p;
  gex_Flags_t flags = 0; // TODO-EX: GEX_FLAG_SELF_SEG_SOME (scratch resides in client or aux seg)
  int result = 0;

  gasneti_assert(p2p != NULL);
  gasneti_assert(p2p->state != NULL);
  gasneti_assert(op->scratch_req);

  gasnete_coll_local_tree_geom_t *geom = data->tree_geom;
  const gex_Rank_t child_cnt = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom);
  const gex_Rank_t parent = GASNETE_COLL_TREE_GEOM_PARENT(geom);

  struct pdata {
    const void *put_src;
    void       *put_dst;
    void       *myscratch;
    void       *upscratch;
    gex_Event_t ev;
    size_t      chunk_cnt; // elems
    size_t      chunk_len; // bytes
    size_t      curr_cnt;  // elems
    size_t      curr_len;  // bytes
    size_t      offset;    // bytes
    size_t      remain;    // elems
    int         last;
    int         phase;
  } *pdata;

  if (data->state) {
    pdata = data->private_data;
  } else {
    // Allocate and initialize pdata
    if (sizeof(struct pdata) <= op->team->p2p_eager_buffersz) {
      // Store in p2p space, unused by this algorithm
      // TODO-EX: need a more general way to avoid small dynamic allocations
      pdata = (struct pdata *) p2p->data;
    } else {
      pdata = gasneti_calloc(1, sizeof(struct pdata));
    }
    gasneti_assert(!data->private_data);
    data->private_data = pdata;

    pdata->curr_cnt = pdata->chunk_cnt = op->param_list[0];
    pdata->curr_len = pdata->chunk_len = pdata->curr_cnt * args->dt_sz;
    pdata->remain = args->dt_cnt;
    gasneti_assert(pdata->ev == GEX_EVENT_INVALID);
    gasneti_assert(pdata->offset == 0);
    gasneti_assert(pdata->phase == 0);

    p2p->state[0] = GASNETE_COLL_TREE_GEOM_SIBLING_ID(geom); // initial CTS
    data->state = 1;
  }

  switch (data->state) {
    // case 0: pdata allocation/initialization, above

    case 1: {   // Wait for scratch allocation
      if (!gasnete_coll_scratch_alloc_nb(op GASNETI_THREAD_PASS)) {
        break;
      }
      pdata->myscratch = gasnete_coll_scratch_myaddr(op, 0);
      if (parent != GEX_RANK_INVALID) {
        pdata->upscratch = gasnete_coll_scratch_addr(op, parent, 0, 0);
      }
      data->state = 2; GASNETI_FALLTHROUGH
    }

    case 2: {   // Wait for arrival of data from children, if any
      volatile uint32_t *state = p2p->state;
      const int ready = pdata->phase ^ 1;
      for (gex_Rank_t r = 1; r <= child_cnt; ++r) { // NOTE: 1-based
        if (state[r] != ready) return 0; // At least one child has not contributed their value
      }
      gasneti_sync_reads();
      pdata->last = (pdata->remain <= pdata->chunk_cnt);
      if (pdata->last) {
        pdata->curr_cnt = pdata->remain;
        pdata->curr_len = pdata->remain * args->dt_sz;
      }
      data->state = 3; GASNETI_FALLTHROUGH
    }

    case 3: {
      // Compute reduction (if any)
      const void *src = (const void *)(pdata->offset + (uintptr_t)args->src);
      if (child_cnt) {
        const size_t elem_cnt = pdata->curr_cnt;
        const size_t stride = pdata->chunk_len;
        const int phase = pdata->phase;
        void * data = pdata->myscratch;
        // In even phases: contributions from all children are contiguous
        // In odd phases: contributions from last child is non-contiguous (dbl buffered)
        // Step 3a: local reduction over (child_cnt - phase) children
        void *tmp = local_reduce_helper(args, elem_cnt, stride, child_cnt - phase, src, data);
        if (phase) {
          // Step 3b: reduce the non-contiguous contribution from the final child
          // 'tmp' now points to intermediate result, which excludes last child
          gasneti_assert(tmp == ((child_cnt < 2) ? src
                                                 : gasnete_coll_scale_ptr(data, child_cnt-2, stride)));
          data = gasnete_coll_scale_ptr(data, child_cnt, stride);
          tmp = local_reduce_helper(args, elem_cnt, stride, 1, tmp, data);
        }
        pdata->put_src = tmp;
      } else {
        pdata->put_src = src;
      }
      data->state = 4; GASNETI_FALLTHROUGH
    }

    case 4:
      if (parent == GEX_RANK_INVALID) {
        void *dst = (void *)(pdata->offset + (uintptr_t)args->dst);
        GASNETI_MEMCPY(dst, pdata->put_src, pdata->curr_len);
      } else {
        // Wait for parent's clear-to-send and sync of previous parent send
        gex_Rank_t dest_slot = p2p->state[0];
        gasneti_assert(pdata->ev != GEX_EVENT_NO_OP);
        if ((dest_slot == GEX_RANK_INVALID) || pdata->ev) break;
        // Compute addr of send to parent from dest_slot
        pdata->put_dst = gasnete_coll_scale_ptr(pdata->upscratch, dest_slot, pdata->chunk_len);
        // Reset state[0] prior to sending, to avoid race
        p2p->state[0] = GEX_RANK_INVALID;
      }
      data->state = 5; GASNETI_FALLTHROUGH

    case 5: {   // First attempt at comms using IMMEDIATE
      flags |= GEX_FLAG_IMMEDIATE; // We fall through to the first attempt at comms
      const int next_phase = pdata->phase ^ 1;
      int comms_done = 1;

      // Send partial result to parent (if any)
      if (parent != GEX_RANK_INVALID) {
        gex_Rank_t index = 1 + GASNETE_COLL_TREE_GEOM_SIBLING_ID(geom);
        if (gasnete_tm_p2p_signalling_put(op, parent,
                                          pdata->put_dst, pdata->put_src, pdata->curr_len,
                                          &pdata->ev, flags, index, next_phase
                                          GASNETI_THREAD_PASS)) {
          pdata->ev = GEX_EVENT_NO_OP; // mark for retry
          comms_done = 0;
        } else if (pdata->ev != GEX_EVENT_INVALID) {
          gasneti_assert(pdata->ev != GEX_EVENT_NO_OP);
          gasnete_coll_save_event(&pdata->ev); // add to polling set
        }
      }

      // Send CTS message to children (if any) unless at end
      if (child_cnt && !pdata->last) {
        gex_Rank_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(geom);
        volatile uint32_t *state_plus1 = p2p->state + 1;
        gex_Rank_t r;
        for (r = 0; r < (child_cnt - 1); ++r) {
          if (gasnete_tm_p2p_change_state(op, children[r], flags,
                                          0, r GASNETI_THREAD_PASS)) {
            state_plus1[r] = 2;  // mark for retry
            comms_done = 0;
          }
        }
        // Need to alternate location of last child's contribution with phase
        gasneti_assert(r == child_cnt - 1);
        if (gasnete_tm_p2p_change_state(op, children[r], flags,
                                        0, (r + next_phase) GASNETI_THREAD_PASS)) {
          state_plus1[r] = 2;  // mark for retry
          comms_done = 0;
        }
      }

      if (comms_done) goto comms_done; // Skip non-IMM comms

      // Yield.  Control will resume at non-IMMEDIATE comms
      gasneti_assert(! result);
      data->state = 6;
      break;
    }

    case 6: {   // Second attempt at comms *not* using IMMEDIATE
      gasneti_assert(! (flags & GEX_FLAG_IMMEDIATE));
      const int next_phase = pdata->phase ^ 1;

      // Send partial result to parent (if any)
      if (pdata->ev == GEX_EVENT_NO_OP) {
        gasneti_assert(parent != GEX_RANK_INVALID);
        gex_Rank_t index = 1 + GASNETE_COLL_TREE_GEOM_SIBLING_ID(geom);
        gasnete_tm_p2p_signalling_put(op, parent,
                                      pdata->put_dst, pdata->put_src, pdata->curr_len,
                                      &pdata->ev, flags, index, next_phase
                                      GASNETI_THREAD_PASS);
        if (pdata->ev != GEX_EVENT_INVALID) {
          gasnete_coll_save_event(&pdata->ev); // add to polling set
        }
      }

      // Send CTS message to children (if any) unless at end
      if (child_cnt && !pdata->last) {
        gex_Rank_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(geom);
        volatile uint32_t *state_plus1 = p2p->state + 1;
        gex_Rank_t r;
        for (r = 0; r < (child_cnt - 1); ++r) {
          if (state_plus1[r] == 2) {
            gasnete_tm_p2p_change_state(op, children[r], flags,
                                        0, r GASNETI_THREAD_PASS);
          }
        }
        // Need to alternate location of last child's contribution with phase
        gasneti_assert(r == child_cnt - 1);
        if (state_plus1[r] == 2) {
          gasnete_tm_p2p_change_state(op, children[r], flags,
                                      0, (r + next_phase) GASNETI_THREAD_PASS);
        }
      }
    }

    comms_done:
      // Comms are done, reduction might be too
      if (pdata->last) {
        if (pdata->ev) {
          // OP and scratch must remain active until async Put to parent is complete
          gasneti_assert(pdata->ev != GEX_EVENT_NO_OP);
          result = GASNETE_COLL_OP_COMPLETE;
          data->state = 7;
          break;
        } else {
          // Completely done
          result = GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE;
          goto done;
        }
      }

      // Advance phase, offset and remain for next iter
      pdata->phase ^= 1;
      pdata->offset += pdata->curr_len;
      pdata->remain -= pdata->curr_cnt;
      gasneti_assert(pdata->remain);

      // Yield.  Control will resume at next iteration.
      gasneti_assert(! result);
      data->state = 2;
      break;

    case 7:    // Stall for async Put to parent to complete
      if (pdata->ev) break;
      result = GASNETE_COLL_OP_INACTIVE;

    done:
      gasneti_assert(result);
      if (pdata != (struct pdata *)p2p->data) {
        gasneti_free(pdata);
      }
      gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      break;

    default: gasneti_unreachable();
  } // end switch

  return result;
}

GASNETE_TM_DECLARE_REDUCE_ALG(TreePutSeg)
{
  gasnet_team_handle_t team = gasneti_import_tm(tm)->_coll_team;

  gasneti_assert(coll_params);
  gasnete_coll_local_tree_geom_t *geom = (gasnete_coll_local_tree_geom_t *)coll_params;

  // Determine what can fit in scratch space or Long
  const size_t slot_sz = team->scratch_size / (1 + geom->max_radix);
  const size_t limit = MIN(slot_sz, gex_AM_LUBRequestLong());
  size_t chunk_cnt;
#if 1 // Branches are cheaper than integer division
  if (gasneti_dt_8byte(dt)) {
    chunk_cnt = limit >> 3;
  } else if (gasneti_dt_4byte(dt)) {
    chunk_cnt = limit >> 2;
  } else
#endif
  chunk_cnt = limit / dt_sz;

  // We pass as "param" to avoid recomputing in the pf, but that requires 32-bit type
  uint32_t pipe_seg_sz = MIN(MIN(dt_cnt, chunk_cnt), 0xFFFFFFFFu);

  // make sure this is a valid choice of algorithm
  gasneti_assert(pipe_seg_sz > 0);

  // Scratch space
  const size_t chunk_len = pipe_seg_sz * dt_sz;
  gasnete_coll_scratch_req_t *scratch_req = gasnete_coll_scratch_alloc_req(team);
  // fill out the tree information
  scratch_req->tree_type = geom->tree_type;
  scratch_req->tree_dir = GASNETE_COLL_UP_TREE;
  scratch_req->root = root;
  scratch_req->op_type = GASNETE_COLL_TREE_OP;
  // fill out the peer information
  //  in: recv 'chunk_len' from each child, PLUS a spare for send to parent (if any)
  //  out: self and siblings each send 'chunk_len' to parent
  scratch_req->incoming_size = chunk_len * (1 + GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom));
  scratch_req->num_in_peers = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom);
  scratch_req->in_peers = GASNETE_COLL_TREE_GEOM_CHILDREN(geom);      
  if (team->myrank == root) {
    scratch_req->num_out_peers = 0;
    scratch_req->out_peers = NULL;
    scratch_req->out_sizes = NULL;
  } else {
    scratch_req->num_out_peers = 1;
    gasnete_coll_scratch_alloc_out_sizes(scratch_req, 1);
    scratch_req->out_sizes[0] = chunk_len * (1 + geom->num_siblings);
    scratch_req->out_peers = &(GASNETE_COLL_TREE_GEOM_PARENT(geom));
  }

  const int options = GASNETE_COLL_GENERIC_OPT_P2P | GASNETE_COLL_USE_SCRATCH;
  return gasnete_tm_generic_reduce_nb(tm, root, dst, src, dt, dt_sz, dt_cnt,
                                      op, op_fnptr, op_cdata, coll_flags,
                                      &gasnete_coll_pf_tm_reduce_TreePutSeg,
                                      options, geom, sequence, 1, &pipe_seg_sz, scratch_req
                                      GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/

// GEX Reduce-to-all via Reduce-to-one + Broadcast
// This is correct, but not ideal.
//
// NOTE:
// This uses a SUBORDINATE operation for the ReduceToOne, which has some
// important caveats.  Because scratch space management itself has a collective
// initiation requirement, we can only safely invoke a SUBORDINATE which may use
// scratch space (as large enough reductions do) if we do so as the very first
// step without any conditions or stalls (and then only because there is an
// AMPoll at the end of ReduceToAll injection to ensure this pf runs before that
// may return).
// This also means the Broadcast step *cannot* use scratch space, since we
// cannot ensure that its initation is collectively ordered.  This, in turn,
// means we must either restrict it to use of algorithms which do not us
// scratch, or else we can implement that step explicitly here.
// CURRENTLY, this is resolved by using either of two algorithms known not to
// use scratch, with the choice based on size.  This selection happens to match
// the defaults for out-of-segment src and dst of those sizes, but relying on
// that it too fragile (e.g. turning on autotuner *will* break it).
// Unfortunately, the gasnete_coll_bcast_RVous() used for the large case is
// known to scale poorly, since it is a flat-tree implementation.  Independent
// of its use here, that should be replaced with an equivalent tree-based
// implementation.  Ideally this implementation will be discarded by then.

static int gasnete_coll_pf_tm_reduce_all_Bcast(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gex_TM_t const tm = op->e_tm;
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_tm_reduce_all_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, tm_reduce_all);
  int result = 0;

  switch (data->state) {
    case 0:     // Initiate reduce-to-one with root == 0
      data->handle =
        gasnete_tm_reduce_nb(tm, 0, args->dst, args->src, args->dt, args->dt_sz, args->dt_cnt,
                             args->opcode, args->op_fnptr, args->op_cdata,
                             GASNETI_FLAG_COLL_SUBORDINATE, op->sequence+1 GASNETI_THREAD_PASS);
      if (data->handle != GEX_EVENT_INVALID) {
        gasnete_coll_save_event(&data->handle);
      }
      data->state = 1; GASNETI_FALLTHROUGH

    case 1:     // Poll for completion of reduce-to-one
      if (data->handle != GEX_EVENT_INVALID) {
        break;
      }
      data->state = 2; GASNETI_FALLTHROUGH

    case 2: {   // Initiate dst->dst broadcast from root == 0
      const size_t eager_limit = MIN(gasnete_coll_p2p_eager_min, gex_AM_LUBRequestMedium());
      const size_t nbytes = args->dt_sz * args->dt_cnt;
      gasnete_coll_team_t team = op->team;
      int flags = GASNETE_COLL_SUBORDINATE;
      // See NOTE above about why we explicitly select the impl
      struct gasnete_coll_implementation_t_ impl;
      impl.need_to_free = 0;
      impl.num_params = 0;
      if (nbytes <= eager_limit) {
        impl.tree_type = gasnete_coll_autotune_get_tree_type(team->autotune_info,
                                                             GASNET_COLL_BROADCAST_OP,
                                                             -1, nbytes, flags);
        data->handle =
          gasnete_coll_bcast_TreeEager(team, args->dst, 0, args->dst, nbytes,
                                       flags, &impl, op->sequence+2 GASNETI_THREAD_PASS);
      } else {
        // TODO: update this if/when a tree-based RVous is implemented (if this code is still live)
        data->handle =
          gasnete_coll_bcast_RVous(team, args->dst, 0, args->dst, nbytes,
                                   flags, &impl, op->sequence+2 GASNETI_THREAD_PASS);
      }
      if (data->handle != GEX_EVENT_INVALID) {
        gasnete_coll_save_event(&data->handle);
      }
      data->state = 3; GASNETI_FALLTHROUGH
    }

    case 3:     // Poll for completion of broadcast
      if (data->handle != GEX_EVENT_INVALID) {
        break;
      }

    // Done
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE;
      break;

    default: gasneti_unreachable();
  } // end switch

  return result;
}

GASNETE_TM_DECLARE_REDUCE_ALL_ALG(Bcast)
{
  gasneti_assert(!(coll_flags & GASNETE_COLL_SUBORDINATE));
  return gasnete_tm_generic_reduce_all_nb(
                        tm, dst, src, dt, dt_sz, dt_cnt,
                        op, op_fnptr, op_cdata, coll_flags,
                        &gasnete_coll_pf_tm_reduce_all_Bcast,
                        0, NULL, 2 /* subordinates */, 0, NULL, NULL
                        GASNETI_THREAD_PASS);
}
/*---------------------------------------------------------------------------------*/
