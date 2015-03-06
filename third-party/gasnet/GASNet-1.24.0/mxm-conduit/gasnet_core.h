/*
 * Description: GASNet header for MXM conduit core
 * Copyright (c)  2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright (c)  2012, Mellanox Technologies LTD. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
#error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_H
#define _GASNET_CORE_H

#include <gasnet_core_help.h>

GASNETI_BEGIN_EXTERNC

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* gasnet_init not inlined or renamed because we use redef-name trick on
   it to ensure proper version linkage */
extern int gasnet_init(int *argc, char ***argv);

extern int gasnetc_attach(gasnet_handlerentry_t *table, int numentries,
                          uintptr_t segsize, uintptr_t minheapoffset);
#define gasnet_attach gasnetc_attach

extern void gasnetc_exit(int exitcode) GASNETI_NORETURN;
GASNETI_NORETURNP(gasnetc_exit)
#define gasnet_exit gasnetc_exit
/* ------------------------------------------------------------------------------------ */
/*
  No-interrupt sections
  =====================
*/
/* conduit may or may not need this based on whether interrupts are used for running handlers */
#if GASNETC_USE_INTERRUPTS
extern void gasnetc_hold_interrupts(void);
extern void gasnetc_resume_interrupts(void);

#define gasnet_hold_interrupts    gasnetc_hold_interrupts
#define gasnet_resume_interrupts  gasnetc_resume_interrupts
#else
#define gasnet_hold_interrupts()
#define gasnet_resume_interrupts()
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Handler-safe locks
  ==================
*/
typedef struct _gasnet_hsl_t {
    gasneti_mutex_t lock;

#if GASNETI_STATS_OR_TRACE
    gasneti_tick_t acquiretime;
#endif

#if GASNETC_USE_INTERRUPTS
    /* more state may be required for conduits using interrupts */
#error interrupts not implemented
#endif
} gasnet_hsl_t GASNETI_THREAD_TYPEDEF;

#if GASNETI_STATS_OR_TRACE
#define GASNETC_LOCK_STAT_INIT ,0
#else
#define GASNETC_LOCK_STAT_INIT
#endif

#if GASNETC_USE_INTERRUPTS
#error interrupts not implemented
#define GASNETC_LOCK_INTERRUPT_INIT
#else
#define GASNETC_LOCK_INTERRUPT_INIT
#endif

#define GASNET_HSL_INITIALIZER { \
  GASNETI_MUTEX_INITIALIZER      \
  GASNETC_LOCK_STAT_INIT         \
  GASNETC_LOCK_INTERRUPT_INIT    \
  }

/* decide whether we have "real" HSL's */
#if GASNETI_THREADS || GASNETC_USE_INTERRUPTS || /* need for safety */ \
    GASNET_DEBUG || GASNETI_STATS_OR_TRACE       /* or debug/tracing */
#ifdef GASNETC_NULL_HSL
#error bad defn of GASNETC_NULL_HSL
#endif
#else
#define GASNETC_NULL_HSL 1
#endif

#if GASNETC_NULL_HSL
/* HSL's unnecessary - compile away to nothing */
#define gasnet_hsl_init(hsl)
#define gasnet_hsl_destroy(hsl)
#define gasnet_hsl_lock(hsl)
#define gasnet_hsl_unlock(hsl)
#define gasnet_hsl_trylock(hsl)	GASNET_OK
#else
extern void gasnetc_hsl_init   (gasnet_hsl_t *hsl);
extern void gasnetc_hsl_destroy(gasnet_hsl_t *hsl);
extern void gasnetc_hsl_lock   (gasnet_hsl_t *hsl);
extern void gasnetc_hsl_unlock (gasnet_hsl_t *hsl);
extern int  gasnetc_hsl_trylock(gasnet_hsl_t *hsl) GASNETI_WARN_UNUSED_RESULT;

#define gasnet_hsl_init    gasnetc_hsl_init
#define gasnet_hsl_destroy gasnetc_hsl_destroy
#define gasnet_hsl_lock    gasnetc_hsl_lock
#define gasnet_hsl_unlock  gasnetc_hsl_unlock
#define gasnet_hsl_trylock gasnetc_hsl_trylock
#endif
/* ------------------------------------------------------------------------------------ */
/*
  Active Message Size Limits
  ==========================
*/

#define GASNETC_MAX_ARGS  16

/*
 * The maximal message size in MXM is practically
 * unlimited, so this should really be ((size_t)-1).
 * However, GASNet Extended API implementation uses
 * implicit casting to int here and there, so as
 * a workaround, use INT_MAX instead.
 */
#define GASNETC_MAX_LONG_REQ ((size_t)INT_MAX)
#define GASNETC_MAX_LONG_REP ((size_t)(GASNETC_MAX_LONG_REQ))

#define GASNETC_MXM_MAX_MSG_SIZE 0x40000000

/*
 * Medium messages are sent using SEND and adding MXM eager RDMA header
 * (unlike long messages that are sent with PUT and using MXM write header).
 * The actual data that is being sent is:
 *   - arguments buffer
 *   - user buffer
 *   - metadata is sent in immediate (part of MXM header for eager)
 *   - source node ID is sent in tag (part of MXM header for eager)
 *   - MXM header for eager RDMA (22 bytes)
 */
#define GASNETC_AM_MAX_MED_PACKETS 64
#define GASNETC_DEFAULT_AM_MAX_MED (size_t)                                          \
            (GASNETC_AM_MAX_MED_PACKETS * (2048 - 22) - \
             (GASNETC_MAX_ARGS * sizeof(gasnet_handlerarg_t)))


#if GASNET_PSHM
/*
 * PSHM uses GASNETC_MAX_MEDIUM as parameter in array size,
 * so it has to be compile-time constant
 */
#define GASNETC_MAX_MEDIUM   GASNETC_DEFAULT_AM_MAX_MED
#else
size_t gasneti_AMMaxMedium();
#define GASNETC_MAX_MEDIUM   (gasneti_AMMaxMedium())
#endif

#define gasnet_AMMaxArgs()          ((size_t)GASNETC_MAX_ARGS)
#if GASNET_PSHM
/* (###) If supporting PSHM a conduit must "negotiate" the maximum size of a
 * Medium message.  This can either be done by lowering the conduit's value to
 * the default PSHM value (as shown here), or GASNETI_MAX_MEDIUM_PSHM can be
 * defined in gasnet_core_fwd.h to give the conduit complete control. */
#define gasnet_AMMaxMedium()      ((size_t)MIN(GASNETC_MAX_MEDIUM, GASNETI_MAX_MEDIUM_PSHM))
#else
#define gasnet_AMMaxMedium()      ((size_t)GASNETC_MAX_MEDIUM)
#endif

#define gasnet_AMMaxLongRequest()   ((size_t)GASNETC_MAX_LONG_REQ)
#define gasnet_AMMaxLongReply()     ((size_t)GASNETC_MAX_LONG_REP)

/* ------------------------------------------------------------------------------------ */
/*
  Misc. Active Message Functions
  ==============================
*/
extern int gasnetc_AMGetMsgSource(gasnet_token_t token, gasnet_node_t *srcindex);

#define gasnet_AMGetMsgSource  gasnetc_AMGetMsgSource

#define GASNET_BLOCKUNTIL(cond) gasneti_polluntil(cond)

/* ------------------------------------------------------------------------------------ */
/*
 * Misc. auxiliary functions
 * =========================
 */
extern void gasnetc_barrier_fence(void);

/* ------------------------------------------------------------------------------------ */

GASNETI_END_EXTERNC

#endif

#include <gasnet_ammacros.h>
