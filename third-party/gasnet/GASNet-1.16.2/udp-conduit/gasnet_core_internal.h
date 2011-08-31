/*   $Source: /var/local/cvs/gasnet/udp-conduit/gasnet_core_internal.h,v $
 *     $Date: 2009/09/21 01:05:38 $
 * $Revision: 1.11 $
 * Description: GASNet MPI conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>

/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 1

extern ep_t gasnetc_endpoint;

extern gasneti_mutex_t gasnetc_AMlock; /*  protect access to AMUDP */
extern volatile int gasnetc_AMLockYield;
#if GASNETI_THREADS
  #define _AMLOCKYIELD() do {      \
    if_pf(gasnetc_AMLockYield) {   \
      int _i;                      \
      for (_i = 0; _i < 10; _i++)  \
        gasneti_sched_yield();     \
    } } while (0)
#else
  #define _AMLOCKYIELD() ((void)0)
#endif
#define AMLOCK()        do {             \
    _AMLOCKYIELD();                      \
    gasneti_mutex_lock(&gasnetc_AMlock); \
} while (0)
#define AMLOCK_TOSEND() do {             \
    _AMLOCKYIELD();                      \
    gasneti_suspend_spinpollers();       \
    gasneti_mutex_lock(&gasnetc_AMlock); \
    gasneti_resume_spinpollers();        \
  } while (0)
#define AMUNLOCK()           gasneti_mutex_unlock(&gasnetc_AMlock)
#define AM_ASSERT_LOCKED()   gasneti_mutex_assertlocked(&gasnetc_AMlock)
#define AM_ASSERT_UNLOCKED() gasneti_mutex_assertunlocked(&gasnetc_AMlock)

/* AMLOCK_CAUTIOUS is only for use in very limited contexts where lock status
   is unknown, eg exit-time processing */
#if GASNET_DEBUG
  /* ignore recursive lock attempts */
  #define _AMLOCK_CAUTIOUS_HELPER() if (gasnetc_AMlock.owner == GASNETI_THREADIDQUERY()) break
#else
  #define _AMLOCK_CAUTIOUS_HELPER() ((void)0)
#endif
#define AMLOCK_CAUTIOUS()    do {                         \
    int _i;                                               \
    gasnetc_AMLockYield = 1;                              \
    for (_i=0; _i < 50; _i++) {                           \
      _AMLOCK_CAUTIOUS_HELPER();                          \
      if (!gasneti_mutex_trylock(&gasnetc_AMlock)) break; \
      gasneti_sched_yield();                              \
    }                                                     \
    gasnetc_AMLockYield = 0;                              \
} while (0)

/* ------------------------------------------------------------------------------------
 *  AM Error Handling
 * ------------------------------------------------------------------------------------ */
GASNETI_INLINE(gasneti_AMErrorName)
const char *gasneti_AMErrorName(int errval) {
  switch (errval) {
    case AM_OK:           return "AM_OK";      
    case AM_ERR_NOT_INIT: return "AM_ERR_NOT_INIT";      
    case AM_ERR_BAD_ARG:  return "AM_ERR_BAD_ARG";       
    case AM_ERR_RESOURCE: return "AM_ERR_RESOURCE";      
    case AM_ERR_NOT_SENT: return "AM_ERR_NOT_SENT";      
    case AM_ERR_IN_USE:   return "AM_ERR_IN_USE";       
    default: return "*unknown*";
    }
  }

/* ------------------------------------------------------------------------------------ */
/* make an AM call - if it fails, print error message and return */
#define GASNETI_AM_SAFE(fncall) do {                            \
   int const _retcode = (fncall);                               \
   if_pf (_retcode != AM_OK) {                                  \
     char msg[128];                                             \
     sprintf(msg, "\nGASNet encountered an AM Error: %s(%i)\n", \
        gasneti_AMErrorName(_retcode), _retcode);               \
     GASNETI_RETURN_ERRFR(RESOURCE, fncall, msg);               \
   }                                                            \
 } while (0)

/* ------------------------------------------------------------------------------------ */
/* make an AM call - if it fails, print error message and set retval to non-zero errcode
 * else, set retval to zero
 */
#define GASNETI_AM_SAFE_NORETURN(retval,fncall) do {                   \
   gasneti_assert(AM_OK == 0);                                         \
   retval = (fncall);                                                  \
   if_pf (retval) {                                                    \
     if (gasneti_VerboseErrors) {                                      \
       fprintf(stderr, "\nGASNet %s encountered an AM Error: %s(%i)\n" \
                       "  at %s:%i\n",                                 \
         GASNETI_CURRENT_FUNCTION,                                     \
         gasneti_AMErrorName(retval),                                  \
         retval, __FILE__, __LINE__);                                  \
       fflush(stderr);                                                 \
     }                                                                 \
   }                                                                   \
 } while (0)

/* ------------------------------------------------------------------------------------ */
#define GASNETC_HANDLER_BASE  1 /* reserve 1-63 for the core API */
#define _hidx_gasnetc_auxseg_reqh             (GASNETC_HANDLER_BASE+0)
/* add new core API handlers here and to the bottom of gasnet_core.c */

/* ------------------------------------------------------------------------------------ */
#if GASNET_PSHM
#define GASNETC_MAX_NUMHANDLERS 256
extern gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS];
#endif

/* ------------------------------------------------------------------------------------ */
/* AM category (recommended impl if supporting PSHM) */
typedef enum {
  gasnetc_Short=0,
  gasnetc_Medium=1,
  gasnetc_Long=2
} gasnetc_category_t;

#endif
