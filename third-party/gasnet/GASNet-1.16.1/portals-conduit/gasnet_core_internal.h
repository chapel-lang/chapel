/*   $Source: /var/local/cvs/gasnet/portals-conduit/gasnet_core_internal.h,v $
 *     $Date: 2009/09/18 23:33:40 $
 * $Revision: 1.9 $
 * Description: GASNet PORTALS conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>
#include <gasnet_handler.h>

/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 1

/* ------------------------------------------------------------------------------------ */
#define GASNETC_HANDLER_BASE  1 /* reserve 1-63 for the core API */
#define _hidx_gasnetc_auxseg_reqh             (GASNETC_HANDLER_BASE+0)
#define _hidx_gasnetc_noop_reph               (GASNETC_HANDLER_BASE+1)
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
/* Unconditionally evalute second arg.
 * When debugging, also assert that it equals the first argument.
 * NOTE: first arg is only evaluated contitionally.
 */
#if GASNET_DEBUG
  #define gasnetc_assert_value(_val, _expr)	gasneti_assert((_val) == (_expr))
#else
  #define gasnetc_assert_value(_val, _expr)	_expr
#endif

/* Assert that a value is aligned to at least the given size */
#define gasnetc_assert_aligned(_val,_align)	gasneti_assert(!((uintptr_t)(_val) % (_align)))

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

/* ------------------------------------------------------------------------------------ */

#if HAVE_ALLOCA && !PLATFORM_COMPILER_PGI
  #define gasnetc_alloc_tmp(_nbytes) alloca(_nbytes)
  #define gasnetc_free_tmp(_ptr)     ((void)0)
#else
  #define gasnetc_alloc_tmp(_nbytes) gasneti_malloc(_nbytes)
  #define gasnetc_free_tmp(_ptr)     gasneti_free(_ptr)
#endif

/* ------------------------------------------------------------------------------------ */

#endif
