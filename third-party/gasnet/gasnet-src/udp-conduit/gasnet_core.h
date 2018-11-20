/*   $Source: bitbucket.org:berkeleylab/gasnet.git/udp-conduit/gasnet_core.h $
 * Description: GASNet header for UDP conduit core
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_CORE_H
#define _GASNET_CORE_H

#include <amudp_const.h>

#include <gasnet_core_help.h>

/*  TODO enhance AMUDP to support thread-safety */
/*  TODO add UDP bypass to loopback messages */

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/

extern void gasnetc_exit(int exitcode) GASNETI_NORETURN;
GASNETI_NORETURNP(gasnetc_exit)
#define gasnet_exit gasnetc_exit

/* Some conduits permit gasnet_init(NULL,NULL).
   Define to 1 if this conduit supports this extension, or to 0 otherwise.  */
#define GASNET_NULL_ARGV_OK 1
/* ------------------------------------------------------------------------------------ */
extern int gasnetc_Client_Init(
                gex_Client_t           *client_p,
                gex_EP_t               *ep_p,
                gex_TM_t               *tm_p,
                const char             *clientName,
                int                    *argc,
                char                   ***argv,
                gex_Flags_t            flags);
// gasnetex.h handles name-shifting of gex_Client_Init()

extern int gasnetc_Segment_Attach(
                gex_Segment_t          *segment_p,
                gex_TM_t               tm,
                uintptr_t              length);
#define gex_Segment_Attach gasnetc_Segment_Attach

extern int gasnetc_EP_Create(
                gex_EP_t                *ep_p,
                gex_Client_t            client,
                gex_Flags_t             flags);
#define gex_EP_Create gasnetc_EP_Create

extern int gasnetc_EP_RegisterHandlers(
                gex_EP_t                ep,
                gex_AM_Entry_t          *table,
                size_t                  numentries);
#define gex_EP_RegisterHandlers gasnetc_EP_RegisterHandlers
/* ------------------------------------------------------------------------------------ */
/*
  Handler-safe locks
  ==================
*/
typedef struct {
  gasneti_mutex_t lock;

  #if GASNETI_STATS_OR_TRACE
    gasneti_tick_t acquiretime;
  #endif
} gex_HSL_t;

#if GASNETI_STATS_OR_TRACE
  #define GASNETC_LOCK_STAT_INIT ,0 
#else
  #define GASNETC_LOCK_STAT_INIT  
#endif

#define GEX_HSL_INITIALIZER { \
  GASNETI_MUTEX_INITIALIZER      \
  GASNETC_LOCK_STAT_INIT         \
  }

/* decide whether we have "real" HSL's */
#if GASNETI_THREADS ||                           /* need for safety */ \
    GASNET_DEBUG || GASNETI_STATS_OR_TRACE       /* or debug/tracing */
  #ifdef GASNETC_NULL_HSL 
    #error bad defn of GASNETC_NULL_HSL
  #endif
#else
  #define GASNETC_NULL_HSL 1
#endif

#if GASNETC_NULL_HSL
  /* HSL's unnecessary - compile away to nothing */
  #define gex_HSL_Init(hsl)
  #define gex_HSL_Destroy(hsl)
  #define gex_HSL_Lock(hsl)
  #define gex_HSL_Unlock(hsl)
  #define gex_HSL_Trylock(hsl)	GASNET_OK
#else
  extern void gasnetc_hsl_init   (gex_HSL_t *hsl);
  extern void gasnetc_hsl_destroy(gex_HSL_t *hsl);
  extern void gasnetc_hsl_lock   (gex_HSL_t *hsl);
  extern void gasnetc_hsl_unlock (gex_HSL_t *hsl);
  extern int  gasnetc_hsl_trylock(gex_HSL_t *hsl) GASNETI_WARN_UNUSED_RESULT;

  #define gex_HSL_Init    gasnetc_hsl_init
  #define gex_HSL_Destroy gasnetc_hsl_destroy
  #define gex_HSL_Lock    gasnetc_hsl_lock
  #define gex_HSL_Unlock  gasnetc_hsl_unlock
  #define gex_HSL_Trylock gasnetc_hsl_trylock
#endif
/* ------------------------------------------------------------------------------------ */
/*
  Active Message Size Limits
  ==========================
*/

#define gex_AM_MaxArgs()            ((unsigned int)AM_MaxShort())
#define gex_AM_LUBRequestMedium()   ((size_t)MIN(AM_MaxMedium(), GASNETC_MAX_MEDIUM_NBRHD_DFLT))
#define gex_AM_LUBReplyMedium()     ((size_t)MIN(AM_MaxMedium(), GASNETC_MAX_MEDIUM_NBRHD_DFLT))
#define gex_AM_LUBRequestLong()     ((size_t)AM_MaxLong())
#define gex_AM_LUBReplyLong()       ((size_t)AM_MaxLong())

  // TODO-EX: Can these be improved upon, at least for PSHM case
#define gasnetc_AM_MaxRequestMedium(tm,rank,lc_opt,flags,nargs)  gex_AM_LUBRequestMedium()
#define gasnetc_AM_MaxReplyMedium(tm,rank,lc_opt,flags,nargs)    gex_AM_LUBReplyMedium()
#define gasnetc_AM_MaxRequestLong(tm,rank,lc_opt,flags,nargs)    gex_AM_LUBRequestLong()
#define gasnetc_AM_MaxReplyLong(tm,rank,lc_opt,flags,nargs)      gex_AM_LUBReplyLong()
#define gasnetc_Token_MaxReplyMedium(token,lc_opt,flags,nargs)   gex_AM_LUBReplyMedium()
#define gasnetc_Token_MaxReplyLong(token,lc_opt,flags,nargs)     gex_AM_LUBReplyLong()

/* ------------------------------------------------------------------------------------ */
/*
  Misc. Active Message Functions
  ==============================
*/

#define GASNET_BLOCKUNTIL(cond) gasneti_polluntil(cond)

/* ------------------------------------------------------------------------------------ */
#ifdef GASNETI_BLCR_ENABLED
/* Collective and non-collective checkpoints */
extern int gasnet_all_checkpoint(const char *dir);
extern int gasnet_checkpoint(const char *dir);
#define GASNET_BLCR 1
#endif
/* ------------------------------------------------------------------------------------ */

#endif

#include <gasnet_ammacros.h>
