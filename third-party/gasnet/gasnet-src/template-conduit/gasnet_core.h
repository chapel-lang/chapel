/*   $Source: bitbucket.org:berkeleylab/gasnet.git/template-conduit/gasnet_core.h $
 * Description: GASNet header for <conduitname> conduit core
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_CORE_H
#define _GASNET_CORE_H

#include <gasnet_core_help.h>

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/

extern void gasnetc_exit(int _exitcode) GASNETI_NORETURN;
GASNETI_NORETURNP(gasnetc_exit)
#define gasnet_exit gasnetc_exit

/* Some conduits permit gasnet_init(NULL,NULL).
   Define to 1 if this conduit supports this extension, or to 0 otherwise.  */
#define GASNET_NULL_ARGV_OK ###
/* ------------------------------------------------------------------------------------ */
extern int gasnetc_Client_Init(
                gex_Client_t           *_client_p,
                gex_EP_t               *_ep_p,
                gex_TM_t               *_tm_p,
                const char             *_clientName,
                int                    *_argc,
                char                   ***_argv,
                gex_Flags_t            _flags);
// gasnetex.h handles name-shifting of gex_Client_Init()

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
  extern void gasnetc_hsl_init   (gex_HSL_t *_hsl);
  extern void gasnetc_hsl_destroy(gex_HSL_t *_hsl);
  extern void gasnetc_hsl_lock   (gex_HSL_t *_hsl);
  extern void gasnetc_hsl_unlock (gex_HSL_t *_hsl);
  extern int  gasnetc_hsl_trylock(gex_HSL_t *_hsl) GASNETI_WARN_UNUSED_RESULT;

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

#define gex_AM_MaxArgs()          ((unsigned int)###)

  // Define least-upper-bound (worst case) limits on payload sizes
  //
  // Conduit must also provide the NBRHD logic with the max (best case) sizes of
  // messages, and by default these are used for that purpose.  If the best and
  // worst cases are *not* the same (such as larger limits with fewer arguments)
  // then the conduit must define GASNETC_MAX_{MEDIUM,LONG}_NBRHD.  For more
  // information see the comments in gasnetex.h which describe those two macros.
#define gex_AM_LUBRequestMedium() ((size_t)###)
#define gex_AM_LUBReplyMedium()   ((size_t)###)
#define gex_AM_LUBRequestLong()   ((size_t)###)
#define gex_AM_LUBReplyLong()     ((size_t)###)

  /* Provide tigher bounds based on parameters (*/
/* Example for closed-form macros:
 * Note the need to ensure every argument is evaluated *exactly once*.
#define gasnetc_AM_MaxRequestMedium(tm,rank,lc_opt,flags,nargs)  \
        (GASNETI_UNUSED_ARGS5(tm,rank,lc_opt,flags,nargs),(size_t)###)
#define gasnetc_AM_MaxReplyMedium(tm,rank,lc_opt,flags,nargs)    \
        (GASNETI_UNUSED_ARGS5(tm,rank,lc_opt,flags,nargs),(size_t)###)
#define gasnetc_Token_MaxReplyMedium(token,lc_opt,flags,nargs)   \
        (GASNETI_UNUSED_ARGS4(token,lc_opt,flags,nargs),(size_t)###)

#define gasnetc_AM_MaxRequestLong(tm,rank,lc_opt,flags,nargs)    \
        (GASNETI_UNUSED_ARGS4(tm,rank,lc_opt,nargs), \
         ((flags) & GEX_FLAG_AM_PREPARE_LEAST_ALLOC  \
                  ? GASNETC_REF_NPAM_MAX_ALLOC       \
                  : (size_t)###))
#define gasnetc_AM_MaxReplyLong(tm,rank,lc_opt,flags,nargs)      \
        (GASNETI_UNUSED_ARGS4(tm,rank,lc_opt,nargs), \
         ((flags) & GEX_FLAG_AM_PREPARE_LEAST_ALLOC  \
                  ? GASNETC_REF_NPAM_MAX_ALLOC       \
                  : (size_t)###))
#define gasnetc_Token_MaxReplyLong(token,lc_opt,flags,nargs)     \
        (GASNETI_UNUSED_ARGS3(token,lc_opt,nargs),   \
         ((flags) & GEX_FLAG_AM_PREPARE_LEAST_ALLOC  \
                  ? GASNETC_REF_NPAM_MAX_ALLOC       \
                  : (size_t)###))
 */
/* Example for true functions:
extern GASNETI_PURE size_t gasnetc_AM_MaxRequestMedium(
           gex_TM_t _tm, gex_Rank_t _rank,
           const gex_Event_t *_lc_opt, gex_Flags_t _flags, unsigned int _nargs);
GASNETI_PUREP(gasnetc_AM_MaxRequestMedium)
extern GASNETI_PURE size_t gasnetc_AM_MaxReplyMedium(
           gex_TM_t _tm, gex_Rank_t _rank,
           const gex_Event_t *_lc_opt, gex_Flags_t _flags, unsigned int _nargs);
GASNETI_PUREP(gasnetc_AM_MaxReplyMedium)
extern GASNETI_PURE size_t gasnetc_AM_MaxRequestLong(
           gex_TM_t _tm, gex_Rank_t _rank,
           const gex_Event_t *_lc_opt, gex_Flags_t _flags, unsigned int _nargs);
GASNETI_PUREP(gasnetc_AM_MaxRequestLong)
extern GASNETI_PURE size_t gasnetc_AM_MaxReplyLong(
           gex_TM_t _tm, gex_Rank_t _rank,
           const gex_Event_t *_lc_opt, gex_Flags_t _flags, unsigned int _nargs);
GASNETI_PUREP(gasnetc_AM_MaxReplyLong)
extern GASNETI_PURE size_t gasnetc_Token_MaxReplyMedium(
           gex_Token_t _token,
           const gex_Event_t *_lc_opt, gex_Flags_t _flags, unsigned int nargs_);
GASNETI_PUREP(gasnetc_Token_MaxReplyMedium)
extern GASNETI_PURE size_t gasnetc_Token_MaxReplyLong(
           const gex_Token_t _token,
           gex_Event_t *_lc_opt, gex_Flags_t _flags, unsigned int _nargs);
GASNETI_PUREP(gasnetc_Token_MaxReplyLong)
 */

/* ------------------------------------------------------------------------------------ */
/*
  Misc. Active Message Functions
  ==============================
*/

#define GASNET_BLOCKUNTIL(cond) gasneti_polluntil(cond)

/* ------------------------------------------------------------------------------------ */

#endif

#include <gasnet_ammacros.h>
