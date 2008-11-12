/*   $Source: /var/local/cvs/gasnet/portals-conduit/gasnet_core_internal.h,v $
 *     $Date: 2008/09/10 01:56:58 $
 * $Revision: 1.4 $
 * Description: GASNet PORTALS conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>

/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 1

/* ------------------------------------------------------------------------------------ */
#define GASNETC_HANDLER_BASE  1 /* reserve 1-63 for the core API */
#define _hidx_gasnetc_auxseg_reqh             (GASNETC_HANDLER_BASE+0)
#define _hidx_gasnetc_noop_reph               (GASNETC_HANDLER_BASE+1)
/* add new core API handlers here and to the bottom of gasnet_core.c */

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


#endif
