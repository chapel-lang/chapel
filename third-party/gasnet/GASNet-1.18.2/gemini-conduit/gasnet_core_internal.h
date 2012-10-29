/*   $Source: /var/local/cvs/gasnet/gemini-conduit/gasnet_core_internal.h,v $
 *     $Date: 2011/06/18 19:56:17 $
 * $Revision: 1.1.1.1 $
 * Description: GASNet <conduitname> conduit header for internal definitions in Core API
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

/* from portals-conduit */
/* Assert that a value is aligned to at least the given size */
#define gasnetc_assert_aligned(_val,_align)	gasneti_assert(!((uintptr_t)(_val) % (_align)))



/* AuxSeg setup for registered bounce  and post descriptors*/

extern gasneti_auxseg_request_t gasnetc_bounce_auxseg_alloc(gasnet_seginfo_t *auxseg_info);
extern gasneti_auxseg_request_t gasnetc_pd_auxseg_alloc(gasnet_seginfo_t *auxseg_info);


#define GASNETC_AUXSEG_FNS() gasnetc_bounce_auxseg_alloc, \
    gasnetc_pd_auxseg_alloc,

#endif
