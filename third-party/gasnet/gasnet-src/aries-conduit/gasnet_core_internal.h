/*   $Source: bitbucket.org:berkeleylab/gasnet.git/aries-conduit/gasnet_core_internal.h $
 * Description: GASNet aries conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
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
#define _hidx_gasnetc_sys_barrier_reqh        (GASNETC_HANDLER_BASE+2)
#define _hidx_gasnetc_sys_exchange_reqh       (GASNETC_HANDLER_BASE+3)
/* add new core API handlers here and to the bottom of gasnet_core.c */

/* ------------------------------------------------------------------------------------ */
/* handler table (temporary global impl) */
extern gex_AM_Entry_t *gasnetc_handler;

/* ------------------------------------------------------------------------------------ */
/* Configure gasnet_event_internal.h and gasnet_event.c */
// TODO-EX: prefix needs to move from "extended" to "core"

#define GASNETE_HAVE_LC

#define GASNETE_CONDUIT_EOP_FIELDS \
  gasneti_weakatomic_val_t initiated_cnt; \
  gasneti_weakatomic_t     completed_cnt; \
  gasneti_weakatomic_val_t initiated_alc; \
  gasneti_weakatomic_val_t completed_alc;

#define GASNETE_EOP_ALLOC_EXTRA(_eop) do { \
    gasneti_weakatomic_set(&(_eop)->completed_cnt, 0 , 0); \
    (_eop)->completed_alc = 0; \
  } while (0)

#if GASNET_DEBUG
#define GASNETC_EOP_CNT_DONE(_eop) \
    (gasneti_weakatomic_read(&(_eop)->completed_cnt, 0) \
        == ((_eop)->initiated_cnt & GASNETI_ATOMIC_MAX))
#define GASNETC_EOP_ALC_DONE(_eop) \
    ((_eop)->completed_alc == (_eop)->initiated_alc)
#endif

#define GASNETC_EOP_CNT_FINISH(_eop) do { \
    gasneti_assert(!GASNETC_EOP_CNT_DONE(_eop));                               \
    gasneti_weakatomic_val_t _completed =                                      \
        gasneti_weakatomic_add(&(_eop)->completed_cnt, 1, GASNETI_ATOMIC_ACQ); \
    if (_completed == ((_eop)->initiated_cnt & GASNETI_ATOMIC_MAX)) {          \
      GASNETE_EOP_MARKDONE(_eop);                                              \
    }                                                                          \
  } while (0)
#define GASNETC_EOP_ALC_FINISH(_eop) do { \
    gasneti_assert(!GASNETC_EOP_ALC_DONE(_eop));                               \
    gasneti_weakatomic_val_t _completed = ((_eop)->completed_alc += 1);        \
    if (_completed == (_eop)->initiated_alc) {                                 \
      GASNETE_EOP_LC_FINISH(_eop);                                             \
    }                                                                          \
  } while (0)

#define GASNETE_EOP_PREP_FREE_EXTRA(_eop) do { \
    gasneti_assert(GASNETC_EOP_CNT_DONE(_eop)); \
    gasneti_assert(GASNETC_EOP_ALC_DONE(_eop)); \
  } while (0)

#define _GASNETE_EOP_NEW_EXTRA GASNETE_EOP_PREP_FREE_EXTRA

/* ------------------------------------------------------------------------------------ */

#if GASNETI_STATS_OR_TRACE
  #define GASNETC_TRACE_WAIT_BEGIN() \
    gasneti_tick_t _waitstart = GASNETI_TICKS_NOW_IFENABLED(C)
#else
  #define GASNETC_TRACE_WAIT_BEGIN() \
    static char _dummy = (char)sizeof(_dummy)
#endif

#define GASNETC_TRACE_WAIT_END(name) \
  GASNETI_TRACE_EVENT_TIME(C,name,gasneti_ticks_now() - _waitstart)

#define GASNETC_STAT_EVENT(name) \
  _GASNETI_STAT_EVENT(C,name)
#define GASNETC_STAT_EVENT_VAL(name,val) \
  _GASNETI_STAT_EVENT_VAL(C,name,val)

/* ------------------------------------------------------------------------------------ */
/* intra-node exit coordination */

#if GASNET_PSHM
typedef struct {
    int exitcode;
    volatile int present;
} gasnetc_exitcode_t;
extern gasnetc_exitcode_t *gasnetc_exitcodes;
#endif

/* ------------------------------------------------------------------------------------ */
/* Bootstrap collective operations */

extern gasneti_spawnerfn_t const *gasneti_spawner;

void gasnetc_bootstrapBarrier_gni(void);
void gasnetc_bootstrapExchange_gni(void *src, size_t len, void *dest);

/* ------------------------------------------------------------------------------------ */
// Aries CE support

// Build Aries CE support by default
#if defined(GASNETC_BUILD_GNICE) && !GASNETC_BUILD_GNICE
  #undef GASNETC_BUILD_GNICE
#else
  #undef GASNETC_BUILD_GNICE
  #define GASNETC_BUILD_GNICE 1
#endif

// Hook consensus barrier w/ Aries CE support
#if GASNETC_BUILD_GNICE
  #define GASNETE_COLL_CONSENSUS_DEFNS          \
    extern void gasnete_cebarrier_notify(void); \
    extern int gasnete_cebarrier_try(void);     \
    extern int gasnete_ce_available;
  #define GASNETE_COLL_CONSENSUS_NOTIFY(team) do {           \
    if ((team) == GASNET_TEAM_ALL && gasnete_ce_available) { \
      gasnete_cebarrier_notify();                            \
    } else {                                                 \
      GASNETE_COLL_CONSENSUS_DEFAULT_NOTIFY(team);           \
    }                                                        \
  } while (0)
  #define GASNETE_COLL_CONSENSUS_TRY(team)                   \
    (((team) == GASNET_TEAM_ALL && gasnete_ce_available)     \
        ? gasnete_cebarrier_try()                            \
        : GASNETE_COLL_CONSENSUS_DEFAULT_TRY(team))
#endif

/* ------------------------------------------------------------------------------------ */
/* AuxSeg setup for registered bounce buffers, post descriptors, and Aries CE */

extern gasneti_auxseg_request_t gasnetc_bounce_auxseg_alloc(gasnet_seginfo_t *auxseg_info);
extern gasneti_auxseg_request_t gasnetc_pd_auxseg_alloc(gasnet_seginfo_t *auxseg_info);

#if GASNETC_BUILD_GNICE
  extern gasneti_auxseg_request_t gasnetc_ce_auxseg_alloc(gasnet_seginfo_t *auxseg_info);
  #define GASNETC_CE_AUXSEG_ALLOC gasnetc_ce_auxseg_alloc,
#else
  #define GASNETC_CE_AUXSEG_ALLOC /*empty*/
#endif

#define GASNETC_AUXSEG_FNS() \
    gasnetc_bounce_auxseg_alloc, \
    gasnetc_pd_auxseg_alloc, \
    GASNETC_CE_AUXSEG_ALLOC

/* ------------------------------------------------------------------------------------ */
#endif
