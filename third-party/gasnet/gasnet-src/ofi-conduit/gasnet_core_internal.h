/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ofi-conduit/gasnet_core_internal.h $
 * Description: GASNet libfabric (OFI) conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2015, Intel Corporation
 * Portions copyright 2018-2020, The Regents of the University of California.
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>

/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 1

/* ------------------------------------------------------------------------------------ */
#define _hidx_gasnetc_exchg_reqh              (GASNETC_HANDLER_BASE+0)
#define _hidx_gasnetc_exit_reqh               (GASNETC_HANDLER_BASE+1)
/* add new core API handlers here and to the bottom of gasnet_core.c */

/* ------------------------------------------------------------------------------------ */
/* handler table (temporary global impl) */
extern gex_AM_Entry_t *gasnetc_handler;

/* ------------------------------------------------------------------------------------ */
/* Configure gasnet_event_internal.h and gasnet_event.c */
// TODO-EX: prefix needs to move from "extended" to "core"

// Define as needed if iop counters should use something other than weakatomics:
/* #define gasnete_op_atomic_(_id) gasnetc_atomic_##_id */

// Define if conduit performs local-completion detection:
// TODO-EX:  #define GASNETE_HAVE_LC

/* Additions to default eop and iop types */
#define GASNETE_CONDUIT_EOP_FIELDS \
                gasnetc_ofi_op_ctxt_t ofi;
#define GASNETE_CONDUIT_IOP_FIELDS      \
                gasnetc_ofi_op_ctxt_t get_ofi;  \
                gasnetc_ofi_op_ctxt_t put_ofi;

/* ------------------------------------------------------------------------------------ */
// For EOP/IOP fields, above

#include <rdma/fabric.h>

typedef void (*rdma_callback_fn) (void *buf);

typedef enum GASNETC_OFI_OP_TYPE {
  OFI_TYPE_AM = 0,
  OFI_TYPE_AM_DATA,
  OFI_TYPE_EGET,
  OFI_TYPE_EPUT,
  OFI_TYPE_IGET,
  OFI_TYPE_IPUT
} gasnetc_ofi_op_type;

typedef struct gasnetc_ofi_op_ctxt {
  struct fi_context     ctxt;
  rdma_callback_fn      callback;
  gasnetc_ofi_op_type   type;
  int                   data_sent;
} gasnetc_ofi_op_ctxt_t;

/* ------------------------------------------------------------------------------------ */

/* FI_THREAD_DOMAIN providers in PAR mode */
#if GASNET_PAR && GASNETC_OFI_USE_THREAD_DOMAIN
    #define GASNETC_OFI_TRYLOCK(lock) gasneti_spinlock_trylock(&gasnetc_ofi_locks.big_lock)
    #define GASNETC_OFI_LOCK(lock) gasneti_spinlock_lock(&gasnetc_ofi_locks.big_lock)
    #define GASNETC_OFI_UNLOCK(lock) gasneti_spinlock_unlock(&gasnetc_ofi_locks.big_lock)
    /* When using DOMAIN, all locks use the centralized big_lock */
    #define GASNETC_OFI_PAR_TRYLOCK(lock) GASNETC_OFI_TRYLOCK(lock)
    #define GASNETC_OFI_PAR_LOCK(lock) GASNETC_OFI_LOCK(lock)
    #define GASNETC_OFI_PAR_UNLOCK(lock) GASNETC_OFI_UNLOCK(lock)
/* This is left here for the purpose of supporting future providers that require fine
 * grained locking. For now, all supported providers either support FI_THREAD_DOMAIN or
 * FI_THREAD_SAFE */
#elif 0 && GASNET_PAR
    #define GASNETC_OFI_TRYLOCK(lock) gasneti_spinlock_trylock(lock)
    #define GASNETC_OFI_LOCK(lock) gasneti_spinlock_lock(lock)
    #define GASNETC_OFI_UNLOCK(lock) gasneti_spinlock_unlock(lock)
    #define GASNETC_OFI_PAR_TRYLOCK(lock) gasneti_spinlock_trylock(lock)
    #define GASNETC_OFI_PAR_LOCK(lock) gasneti_spinlock_lock(lock)
    #define GASNETC_OFI_PAR_UNLOCK(lock) gasneti_spinlock_unlock(lock)
#elif GASNET_PAR /* For FI_THREAD_SAFE providers in PAR mode*/
#define GASNETC_OFI_TRYLOCK(lock) 0
    #define GASNETC_OFI_LOCK(lock) do{} while(0) 
    #define GASNETC_OFI_UNLOCK(lock) do{} while(0) 
    #define GASNETC_OFI_PAR_TRYLOCK(lock) gasneti_spinlock_trylock(lock)
    #define GASNETC_OFI_PAR_LOCK(lock) gasneti_spinlock_lock(lock)
    #define GASNETC_OFI_PAR_UNLOCK(lock) gasneti_spinlock_unlock(lock)
#else /* GASNET_SEQ or GASNET_PARSYNC */
    #define GASNETC_OFI_TRYLOCK(lock) 0
    #define GASNETC_OFI_LOCK(lock) do{} while(0) 
    #define GASNETC_OFI_UNLOCK(lock) do{} while(0) 
    #define GASNETC_OFI_PAR_TRYLOCK(lock) 0
    #define GASNETC_OFI_PAR_LOCK(lock) do{} while(0) 
    #define GASNETC_OFI_PAR_UNLOCK(lock) do{} while(0) 
#endif

#define GASNETC_OFI_LOCK_EXPR(lock, expr) do { GASNETC_OFI_LOCK(lock); \
                                               expr; \
                                               GASNETC_OFI_UNLOCK(lock); \
                                          } while(0)

/* ------------------------------------------------------------------------------------ */
/* handler table (temporary global impl) */
extern gex_AM_Entry_t *gasnetc_handler;

/* ------------------------------------------------------------------------------------ */
/* Unnamed struct to hold all the locks needed */
#if GASNET_PAR && GASNETC_OFI_USE_THREAD_DOMAIN
struct {
    gasneti_atomic_t big_lock;
} gasnetc_ofi_locks;
#elif GASNET_PAR
struct {
    gasneti_atomic_t rx_request_cq;
    char _pad0[GASNETI_CACHE_PAD(sizeof(gasneti_atomic_t))];
    gasneti_atomic_t rx_reply_cq;
} gasnetc_ofi_locks;
/* This is left here for the purpose of supporting future providers that require fine
 * grained locking. For now, all supported providers either support FI_THREAD_DOMAIN or
 * FI_THREAD_SAFE */
#elif 0 && GASNET_PAR && !GASNETC_OFI_USE_THREAD_DOMAIN
struct {
    gasneti_atomic_t rx_cq;
    char _pad0[GASNETI_CACHE_PAD(sizeof(gasneti_atomic_t))];
    gasneti_atomic_t tx_cq;
    char _pad1[GASNETI_CACHE_PAD(sizeof(gasneti_atomic_t))];
    gasneti_atomic_t rdma_tx;
    char _pad2[GASNETI_CACHE_PAD(sizeof(gasneti_atomic_t))];
    gasneti_atomic_t rdma_rx;
    char _pad3[GASNETI_CACHE_PAD(sizeof(gasneti_atomic_t))];
    gasneti_atomic_t am_tx;
    char _pad4[GASNETI_CACHE_PAD(sizeof(gasneti_atomic_t))];
    gasneti_atomic_t am_rx;

} gasnetc_ofi_locks;
#endif

/* These definitions are just to make the polling of the two different AM
 * networks easier to read and understand*/
#define OFI_POLL_ALL   1
#define OFI_POLL_REPLY 0

#define GASNETC_STAT_EVENT(name) \
    _GASNETI_STAT_EVENT(C,name)
#define GASNETC_STAT_EVENT_VAL(name,val) \
    _GASNETI_STAT_EVENT_VAL(C,name,val)

/* ------------------------------------------------------------------------------------ */
/* Job Spawn / Bootstrap */

extern gasneti_spawnerfn_t const *gasneti_spawner;

#define gasneti_bootstrapBarrier        (*(gasneti_spawner->Barrier))
#define gasneti_bootstrapExchange       (*(gasneti_spawner->Exchange))
#define gasneti_bootstrapBroadcast      (*(gasneti_spawner->Broadcast))
#define gasneti_bootstrapSNodeBroadcast (*(gasneti_spawner->SNodeBroadcast))
#define gasneti_bootstrapAlltoall       (*(gasneti_spawner->Alltoall))
#define gasneti_bootstrapAbort          (*(gasneti_spawner->Abort))
#define gasneti_bootstrapCleanup        (*(gasneti_spawner->Cleanup))
#define gasneti_bootstrapFini           (*(gasneti_spawner->Fini))

#endif
