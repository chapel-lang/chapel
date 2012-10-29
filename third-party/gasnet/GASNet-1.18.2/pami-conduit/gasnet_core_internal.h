/*   $Source: /var/local/cvs/gasnet/pami-conduit/gasnet_core_internal.h,v $
 *     $Date: 2012/04/23 06:40:42 $
 * $Revision: 1.10 $
 * Description: GASNet PAMI conduit header for internal definitions in Core API
 * Copyright 2012, Lawrence Berkeley National Laboratory
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>
#include <gasnet_handler.h>

#include <pami.h>

#if GASNETI_ARCH_IBMPE /* XXX: work-around hidden symbol on PERCS - fixed in later rev */
  #undef PAMI_ENDPOINT_NULL
  #define PAMI_ENDPOINT_NULL ((pami_endpoint_t)(~0))
#endif

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
#else
  #define GASNETC_PAMI_LOCK(context)   do { } while (0)
  #define GASNETC_PAMI_UNLOCK(context) do { } while (0)
#endif

/* ------------------------------------------------------------------------------------ */
/* Endpoints */

/* TODO: how must this change for multiple contexts? */
GASNETI_INLINE(gasnetc_endpoint)
pami_endpoint_t gasnetc_endpoint(gasnet_node_t node) {
  pami_endpoint_t result = gasnetc_endpoint_tbl[node];
  gasneti_assert(node != gasneti_mynode);
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
/* Misc. */

/* Get the first "always works" algorithm for a given collective operation */
extern void gasnetc_dflt_coll_alg(pami_xfer_type_t op, pami_algorithm_t *alg_p);

#endif
