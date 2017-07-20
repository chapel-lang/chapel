/*   $Source: bitbucket.org:berkeleylab/gasnet.git/psm-conduit/gasnet_core_internal.h $
 * Description: GASNet psm conduit header for internal definitions in Core API
 * Copyright (c) 2013-2015 Intel Corporation. All rights reserved.
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>
#include <gasnet_handler.h>

#include <psm2.h>
#include <psm2_am.h>
#include <psm2_mq.h>


/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 1

/* -------------------------------------------------------------------------- */
#define GASNETC_HANDLER_BASE  1 /* reserve 1-63 for the core API */
#define _hidx_gasnetc_auxseg_reqh             (GASNETC_HANDLER_BASE+0)
#define _hidx_gasnetc_handler_barrier2        (GASNETC_HANDLER_BASE+1)
#define _hidx_gasnetc_handler_exit2           (GASNETC_HANDLER_BASE+2)
/* add new core API handlers here and to the bottom of gasnet_core.c */

/* -------------------------------------------------------------------------- */
/* handler table (recommended impl) */
#define GASNETC_MAX_NUMHANDLERS   256
extern gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS];

/* -------------------------------------------------------------------------- */
/* AM category (recommended impl if supporting PSHM) */
typedef enum {
    gasnetc_Short=0,
    gasnetc_Medium=1,
    gasnetc_Long=2
} gasnetc_category_t;

/*
 * Multi-thread support
 * psm2 is not thread-safe, so all psm2 calls must be wrapped by a lock.
 */

#define GASNETC_PSM_LOCK() gasneti_spinlock_lock(&gasnetc_psm_state.psm_lock)
#define GASNETC_PSM_TRYLOCK() gasneti_spinlock_trylock(&gasnetc_psm_state.psm_lock)
#define GASNETC_PSM_UNLOCK() gasneti_spinlock_unlock(&gasnetc_psm_state.psm_lock)

#if GASNET_PSHM
/* When PSHM is enabled, the progress thread will poll PSHM to provide passive
   progress.  These lock macros protect calls to PSHM. */
#define GASNETC_PSM_PSHM_LOCK() gasneti_spinlock_lock(&gasnetc_psm_state.pshm_lock)
#define GASNETC_PSM_PSHM_TRYLOCK() gasneti_spinlock_trylock(&gasnetc_psm_state.pshm_lock)
#define GASNETC_PSM_PSHM_UNLOCK() gasneti_spinlock_unlock(&gasnetc_psm_state.pshm_lock)
#endif

/*
 * Bootstrap support
 */
extern gasneti_spawnerfn_t const *gasneti_spawner;

#define gasneti_bootstrapBarrier        (*(gasneti_spawner->Barrier))
#define gasneti_bootstrapExchange       (*(gasneti_spawner->Exchange))
#define gasneti_bootstrapBroadcast      (*(gasneti_spawner->Broadcast))
#define gasneti_bootstrapSNodeBroadcast (*(gasneti_spawner->SNodeBroadcast))
#define gasneti_bootstrapAlltoall       (*(gasneti_spawner->Alltoall))
#define gasneti_bootstrapAbort          (*(gasneti_spawner->Abort))
#define gasneti_bootstrapCleanup        (*(gasneti_spawner->Cleanup))
#define gasneti_bootstrapFini           (*(gasneti_spawner->Fini))

#define AM_HANDLER_SHORT    0 /* gasnetc_handler_short */
#define AM_HANDLER_MED      1 /* gasnetc_handler_med */
#define AM_HANDLER_LONG     2 /* gasnetc_handler_long */
#define AM_HANDLER_PUT      3 /* gasnete_handler_put */
#define AM_HANDLER_GET_REQUEST 4 /* gasnete_handler_get_request */
#define AM_HANDLER_GET_REPLY   5 /* gasnete_handler_get_reply */
#define AM_HANDLER_LONG_PUT 6 /* gasnete_handler_long_put */
#define AM_HANDLER_LONG_GET 7 /* gasnete_handler_long_get */
#define AM_HANDLER_LONG_PUT_REPLY 8 /* gasnete_handler_long_put_reply */
#define AM_HANDLER_NUM      9

/* Set this bit in the handler index argument to indicate reply. */
#define REQUEST_BIT 0x100

/* -------------------------------------------------------------------------- */
/* Generic, thread-safe single-linked queue */

typedef struct _gasnetc_item {
    struct _gasnetc_item *next;
} gasnetc_item_t;

typedef struct _gasnetc_list {
    gasneti_atomic_t lock;
    gasnetc_item_t head;
    gasnetc_item_t *tail;
} gasnetc_list_t;


typedef struct _gasnete_transfer {
    void *context;
    uint32_t frags_remaining;
    uint32_t optype;
} gasnete_transfer_t;

typedef struct _gasnetc_posted_mq_reqs {
    psm2_mq_req_t posted_reqs;
    uint64_t label;
    uint8_t completion;
    int32_t optype;
    psm2_epaddr_t peer;
    int transfer_id;
} gasnetc_posted_mq_reqs_t;

/* -------------------------------------------------------------------------- */
/* General psm conduit state */

typedef struct _gasnetc_psm_state {
    psm2_ep_t ep;
    psm2_mq_t mq;
    psm2_epid_t epid;

    gasneti_atomic_t psm_lock;

    int periodic_poll;
    uint32_t long_msg_threshold;

    void* getreq_slab;        /* Slab of memory for get items */
    int getreq_alloc;        /* Items allocated for get item slab */
    gasnetc_list_t getreqs;    /* Free get items in slab */

    gasnetc_list_t avail_mq_ops;
    /* Queue of pending MQ send/receives to be posted */
    gasnetc_list_t pending_mq_ops;

    /* List of outstanding MQ requests to be completed */
    gasnetc_posted_mq_reqs_t *posted_mq_reqs;
    int posted_reqs_length;
    int posted_reqs_alloc;

    /* Queue of pending MQ ACKS recieved to be completed */
    gasnetc_list_t pending_ack;

    /* List of transfers to be completed */
    gasnete_transfer_t *transfers;
    int transfers_count;
    int transfers_alloc;

    uint64_t mq_op_id;
    int am_handlers[AM_HANDLER_NUM];
    psm2_epaddr_t* peer_epaddrs;

    int exit_in_progress;
    int should_exit;
    int exit_code;
    double exit_timeout;

#if GASNET_PSHM
    gasneti_atomic_t pshm_lock;
#endif
} gasnetc_psm_state_t;

extern gasnetc_psm_state_t gasnetc_psm_state;

typedef struct _gasnetc_token {
    psm2_am_token_t token;
    psm2_epaddr_t source;
} gasnetc_token_t;


/* -------------------------------------------------------------------------- */
/* Internal progress routines */

/* Periodic polling:
   psm2 should be called to process incoming messages, but not too frequently
   nor too infrequently.  If psm2 is polled too frequently, the poll calls just
   become overhead when there are rarely messages waiting to be processed.  On
   the other hand, not polling often enough can result in long delays for
   waiting peers.  Calling psm2 only once for every so many RMA operations
   strikes a balance between these two tradeoffs.  Informal testing resulted in
   the choice of once every 32 calls.
*/
GASNETI_HOT
GASNETI_INLINE(gasnetc_psm_poll_periodic)
void gasnetc_psm_poll_periodic(void)
{
    /* Use of ">= 32" below renders any multi-threaded race on "+=" harmless */
    gasnetc_psm_state.periodic_poll += 1;
    if(gasnetc_psm_state.periodic_poll >= 32) {
        gasnetc_psm_state.periodic_poll = 0;
        gasnetc_AMPoll();
    }
}


int gasnetc_progress_thread_init(void);

GASNETI_COLD GASNETI_NORETURN
void gasnetc_do_exit(void);

int gasnete_long_msg_init(void);

void gasnete_post_pending_ack(void);

void gasnete_post_pending_mq_ops(void);

/* This routine assumes that psm2_poll() has recently been called. */
void gasnete_finish_mq_reqs(void);

#endif
