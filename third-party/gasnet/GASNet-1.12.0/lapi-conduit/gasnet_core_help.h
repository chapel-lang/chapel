/*   $Source: /var/local/cvs/gasnet/lapi-conduit/gasnet_core_help.h,v $
 *     $Date: 2008/03/13 04:34:21 $
 * $Revision: 1.27 $
 * Description: GASNet lapi conduit core Header Helpers (Internal code, not for client use)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_HELP_H
#define _GASNET_CORE_HELP_H

GASNETI_BEGIN_EXTERNC

#include <gasnet_help.h>
#include <lapi.h>

/* Determine which "version" of LAPI we are using.
 * LAPI does not define this anywhere, so we will use a
 * hack that will distinguish between the "origional" lapi
 * and the "new" version, upon which MPI is based.
 */
#ifdef LAPI_DGSM_COPY_SIZE
/* This only appears in the new version of the LAPI header files */
#define GASNETC_LAPI_VERSION 2
#else
#define GASNETC_LAPI_VERSION 1
#endif

/* NOTE: this should be dependent on whether we compile in
 * 32 or 64 bit mode
 */
#define GASNETC_AM_MAX_ARGS 16

/* The max size of a medium message.  Can really be arbitrary size
 * but the receiving task must malloc space for the incoming
 * message.  Note that messages that fit into a single token
 * are optimized.
 */
#define GASNETC_AM_MAX_MEDIUM 262144

/* In 32 bit mode, this is 2^31 - 1 bytes.  */
#define GASNETC_AM_MAX_LONG 2147483647

/* stuff needed for the BLOCKUNTIL macro */
typedef enum {
    gasnetc_Interrupt = 0,
    gasnetc_Polling
} gasnetc_lapimode_t;

extern lapi_handle_t      gasnetc_lapi_context;
extern gasnetc_lapimode_t gasnetc_lapi_default_mode;

/* --------------------------------------------------------------------
 * A simple spinlock implementation
 * --------------------------------------------------------------------
 */
#ifndef GASNETC_USE_SPINLOCKS
  /* default to using spinlocks over pthread mutex */
  #define GASNETC_USE_SPINLOCKS 1
#endif

#if GASNETC_USE_SPINLOCKS
  #if !GASNETI_HAVE_SPINLOCK
    #error Missing required spinlock support
  #endif
  typedef gasneti_atomic_t gasnetc_spinlock_t;
  #define GASNETC_SPINLOCK_INITIALIZER	GASNETI_SPINLOCK_INITIALIZER
#else  /* Use pthread mutex for spinlock */
  typedef gasneti_mutex_t gasnetc_spinlock_t;
  #define GASNETC_SPINLOCK_INITIALIZER	GASNETI_MUTEX_INITIALIZER
#endif


GASNETI_END_EXTERNC

#endif
