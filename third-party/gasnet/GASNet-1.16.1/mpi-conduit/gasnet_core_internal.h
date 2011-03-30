/*   $Source: /var/local/cvs/gasnet/mpi-conduit/gasnet_core_internal.h,v $
 *     $Date: 2009/09/18 23:33:34 $
 * $Revision: 1.21 $
 * Description: GASNet MPI conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>
#include <gasnet_handler.h>

/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 1
#define GASNETC_NISTIMEOUT_WARNING_THRESHOLD 1000000 /* us for timeout warning under HSL_ERRCHECK */
#define GASNETC_DEFAULT_NETWORKDEPTH 4

extern ep_t gasnetc_endpoint;

extern gasneti_mutex_t gasnetc_AMlock; /*  protect access to AMMPI */
#define AMLOCK()             gasneti_mutex_lock(&gasnetc_AMlock)
#define AMLOCK_TOSEND() do {             \
    gasneti_suspend_spinpollers();       \
    gasneti_mutex_lock(&gasnetc_AMlock); \
    gasneti_resume_spinpollers();        \
  } while (0)
#define AMUNLOCK()           gasneti_mutex_unlock(&gasnetc_AMlock)
#define AM_ASSERT_LOCKED()   gasneti_mutex_assertlocked(&gasnetc_AMlock)
#define AM_ASSERT_UNLOCKED() gasneti_mutex_assertunlocked(&gasnetc_AMlock)

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
