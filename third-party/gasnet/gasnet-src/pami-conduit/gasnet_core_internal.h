/*   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/gasnet_core_internal.h $
 * Description: GASNet PAMI conduit header for internal definitions in Core API
 * Copyright 2012, Lawrence Berkeley National Laboratory
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>
#include <gasnet_handler.h>

#include <pami.h>

#define GASNETC_PAMI_CHECK(rc,msg) \
  if_pf ((rc) != PAMI_SUCCESS) \
    { gasneti_fatalerror("Unexpected error %d on node %i/%i %s",\
                         (rc), gasneti_mynode, gasneti_nodes, (msg)); }

#define GASNETC_PAMI_CHECK_ADVANCE(rc,msg) \
  if_pf ((rc) != PAMI_SUCCESS && (rc) != PAMI_EAGAIN) \
    { gasneti_fatalerror("Unexpected error %d on node %i/%i %s",\
                         (rc), gasneti_mynode, gasneti_nodes, (msg)); }

/* ------------------------------------------------------------------------------------ */
/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 0

/* ------------------------------------------------------------------------------------ */
#define GASNETC_HANDLER_BASE  1 /* reserve 1-63 for the core API */
#define _hidx_gasnetc_auxseg_reqh             (GASNETC_HANDLER_BASE+0)
/* add new core API handlers here and to the bottom of gasnet_core.c */

/* ------------------------------------------------------------------------------------ */
/* handler table (recommended impl) */
#define GASNETC_MAX_NUMHANDLERS   256
extern gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS];

/* ------------------------------------------------------------------------------------ */
/* AM category (recommended impl if supporting PSHM) */
typedef enum {
  gasnetc_Short=0,
  gasnetc_Medium=1,
  gasnetc_Long=2
} gasnetc_category_t;

/* ------------------------------------------------------------------------------------ */

enum {
  GASNETC_DISP_NOOP = 0, /* dispatch id 0 may be reserved? */
  GASNETC_DISP_SHORT,
  GASNETC_DISP_MED,
  GASNETC_DISP_LONG,
  GASNETC_DISP_DISSEM_BARR,
#if GASNET_PSHM
  GASNETC_DISP_SNODE_BCAST,
#endif
  GASNETC_NUM_DISP
};


#if GASNET_DEBUG
#define GASNETC_MSG_DEBUG_HDR \
  uint8_t               rep_sent : 1;
#else
#define GASNETC_MSG_DEBUG_HDR \
  /* empty */
#endif

#define GASNETC_MSG_COMMON_HDR                        \
  gasnet_node_t         srcnode; /* must be first */  \
  gasnet_handler_t      handler;                      \
  uint8_t               numargs : 5;                  \
  uint8_t               is_req  : 1;                  \
  GASNETC_MSG_DEBUG_HDR

typedef struct {
  GASNETC_MSG_COMMON_HDR
} gasnetc_genmsg_t;  /* Generic portions of header */

typedef struct {
  GASNETC_MSG_COMMON_HDR
  gasnet_handlerarg_t   args[GASNETC_MAX_ARGS];
} gasnetc_shortmsg_t;

typedef struct {
  GASNETC_MSG_COMMON_HDR
  uint16_t              nbytes;
  gasnet_handlerarg_t   args[GASNETC_MAX_ARGS];
} gasnetc_medmsg_t;

typedef struct {
  GASNETC_MSG_COMMON_HDR
  uintptr_t             addr;
  uint32_t              nbytes; /* type limits our MaxLong */
  gasnet_handlerarg_t   args[GASNETC_MAX_ARGS];
} gasnetc_longmsg_t;

#define GASNETC_ARGSEND_AUX(s,nargs) \
        (offsetof(s,args)+(nargs*sizeof(gasnet_handlerarg_t)))
#define GASNETC_ARGSEND(cat,nargs) \
        GASNETC_ARGSEND_AUX(gasnetc_##cat##msg_t,(nargs))

/* ------------------------------------------------------------------------------------ */
/* Global data */

extern pami_client_t      gasnetc_pami_client;
extern pami_context_t     gasnetc_context; /* XXX: More than one */
extern pami_geometry_t    gasnetc_world_geom;
extern pami_endpoint_t    *gasnetc_endpoint_tbl;
extern size_t             gasnetc_num_contexts;
extern pami_memregion_t   gasnetc_mymemreg;
extern pami_memregion_t   *gasnetc_memreg;
extern size_t             gasnetc_send_imm_max;
extern size_t             gasnetc_recv_imm_max;

/* ------------------------------------------------------------------------------------ */
/* Thread safety */

/* TODO: debug checks for recursion, etc? */
#if GASNET_PAR
  #define GASNETC_PAMI_LOCK(context)   PAMI_Context_lock(context)
  #define GASNETC_PAMI_UNLOCK(context) PAMI_Context_unlock(context)
  #define GASNETC_PAMI_TRYLOCK(context) (PAMI_SUCCESS != PAMI_Context_trylock(context)) /* 0 on success */
#else
  #define GASNETC_PAMI_LOCK(context)   do { } while (0)
  #define GASNETC_PAMI_UNLOCK(context) do { } while (0)
  #define GASNETC_PAMI_TRYLOCK(context) (0)
#endif

/* ------------------------------------------------------------------------------------ */
/* Endpoints */

/* TODO: how must this change for multiple contexts? */
GASNETI_INLINE(gasnetc_endpoint)
pami_endpoint_t gasnetc_endpoint(gasnet_node_t node) {
  pami_endpoint_t result = gasnetc_endpoint_tbl[node];
  gasneti_assert(node < gasneti_nodes);
  if_pf (result == PAMI_ENDPOINT_NULL) {
    /* NOTE: thread-safety based on fact that type is single word */
    PAMI_Endpoint_create(gasnetc_pami_client, node, 0, &result);
    gasnetc_endpoint_tbl[node] = result;
  }
  return result;
}

/* ------------------------------------------------------------------------------------ */
/* Completion counters */

extern void gasnetc_cb_inc_uint(pami_context_t, void *, pami_result_t);
extern void gasnetc_cb_inc_atomic(pami_context_t, void *, pami_result_t);
extern void gasnetc_cb_inc_release(pami_context_t, void *, pami_result_t);
extern void gasnetc_cb_free(pami_context_t, void *, pami_result_t);

/* spin-poll a simple (non-atomic) counter */
GASNETI_INLINE(gasnetc_wait_uint)
pami_result_t gasnetc_wait_uint(pami_context_t context,
                                volatile unsigned int *counter_p,
                                unsigned int goal) {
  while (*counter_p != goal) {
    pami_result_t rc = PAMI_Context_advance(context, 1);
    if_pf (rc != PAMI_SUCCESS && rc != PAMI_EAGAIN) return rc;
  }
  return PAMI_SUCCESS;
}

/* spin-poll an atomic counter */
GASNETI_INLINE(gasnetc_wait_atomic)
pami_result_t gasnetc_wait_atomic(pami_context_t context,
                                  gasneti_weakatomic_t *counter_p,
                                  gasneti_weakatomic_val_t goal) {
  while (gasneti_weakatomic_read(counter_p, 0) != goal) {
    pami_result_t rc = PAMI_Context_advance(context, 1);
    if_pf (rc != PAMI_SUCCESS && rc != PAMI_EAGAIN) return rc;
  }
  return PAMI_SUCCESS;
}

/* ------------------------------------------------------------------------------------ */
/* Collectives (incl. bootstrap collective ops) */

/* Get the default or user-specified algorithm for a given collective operation */
extern void gasnetc_dflt_coll_alg(pami_geometry_t geom, pami_xfer_type_t op, pami_algorithm_t *alg_p);

/* Used for ALLSYNC barrier in blocking collectives: */
extern void gasnetc_fast_barrier(void);

/* The Boostrap collective operations: */
extern void gasnetc_bootstrapBarrier(void);
extern void gasnetc_bootstrapExchange(void *src, size_t len, void *dst);

/* Hooks for conduit-specific collectives */
#if !defined(GASNET_NO_PAMI_COLL)
  /* NOTE: Focus is on blocking collectives because they are simpler, and are all that UPCR uses */
  #define GASNET_PAMI_NATIVE_COLL 1
  #define gasnete_coll_init_conduit gasnete_coll_init_pami
  #define gasnete_coll_team_init_conduit gasnete_coll_team_init_pami

  #define gasnete_coll_broadcast  gasnete_coll_broadcast_pami
  #define gasnete_coll_broadcastM gasnete_coll_broadcastM_pami

  #define gasnete_coll_exchange   gasnete_coll_exchange_pami
  #define gasnete_coll_exchangeM  gasnete_coll_exchangeM_pami

  #define gasnete_coll_gather     gasnete_coll_gather_pami
  #define gasnete_coll_gatherM    gasnete_coll_gatherM_pami

  #define gasnete_coll_gather_all  gasnete_coll_gather_all_pami
  #define gasnete_coll_gather_allM gasnete_coll_gather_allM_pami

  #define gasnete_coll_scatter    gasnete_coll_scatter_pami
  #define gasnete_coll_scatterM   gasnete_coll_scatterM_pami

  #if GASNET_PAR
    #define GASNETE_COLL_TEAM_EXTRA struct {  \
        /* collective geom & algorithms: */   \
        pami_geometry_t geom;                 \
        pami_algorithm_t allga_alg;           \
        pami_algorithm_t allto_alg;           \
        pami_algorithm_t bcast_alg;           \
        pami_algorithm_t gathr_alg;           \
        pami_algorithm_t scatt_alg;           \
        pami_algorithm_t allgavi_alg;         \
        pami_algorithm_t alltovi_alg;         \
        pami_algorithm_t gathrvi_alg;         \
        pami_algorithm_t scattvi_alg;         \
        /* for multi-image intermediates: */  \
        size_t scratch_max_nbytes;            \
        size_t scratch_max_nbytes_allto;      \
        void * scratch_space;                 \
        int * counts;                         \
        int * displs;                         \
        size_t prev_nbytes;                   \
        /* for syncronization: */             \
        void * volatile tmp_addr;             \
        volatile int barrier_phase;           \
        char _pad[GASNETI_CACHE_LINE_BYTES];  \
        gasneti_atomic_t barrier_counter[2];  \
    } pami;
  #else
    #define GASNETE_COLL_TEAM_EXTRA struct {  \
        pami_geometry_t geom;                 \
        pami_algorithm_t allga_alg;           \
        pami_algorithm_t allto_alg;           \
        pami_algorithm_t bcast_alg;           \
        pami_algorithm_t gathr_alg;           \
        pami_algorithm_t scatt_alg;           \
    } pami;
  #endif
#endif

#endif
