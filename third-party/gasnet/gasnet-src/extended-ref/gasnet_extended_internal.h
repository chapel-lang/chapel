/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/gasnet_extended_internal.h $
 * Description: GASNet header for internal definitions in Extended API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_EXTENDED_INTERNAL_H
#define _GASNET_EXTENDED_INTERNAL_H

#include <gasnet_event_internal.h> // TODO-EX: assumes Extended always needs this

/* ------------------------------------------------------------------------------------ */
/* called at startup to check configuration sanity if using any portion of AMRef */
extern void gasnete_check_config_amref(void);

/* ------------------------------------------------------------------------------------ */
// Subsystem initialization
extern void gasnete_barrier_init(void);
extern void gasnete_coll_init_subsystem(void);

/* ------------------------------------------------------------------------------------ */

#define _hidx_gasnete_amdbarrier_notify_reqh (GASNETE_HANDLER_BASE+0) 
#define _hidx_gasnete_amcbarrier_notify_reqh (GASNETE_HANDLER_BASE+1) 
#define _hidx_gasnete_amcbarrier_done_reqh   (GASNETE_HANDLER_BASE+2)
#define _hidx_gasnete_amref_get_reqh         (GASNETE_HANDLER_BASE+3)
#define _hidx_gasnete_amref_get_reph         (GASNETE_HANDLER_BASE+4)
#define _hidx_gasnete_amref_getlong_reqh     (GASNETE_HANDLER_BASE+5)
#define _hidx_gasnete_amref_getlong_reph     (GASNETE_HANDLER_BASE+6)
#define _hidx_gasnete_amref_put_reqh         (GASNETE_HANDLER_BASE+7)
#define _hidx_gasnete_amref_putlong_reqh     (GASNETE_HANDLER_BASE+8)
#define _hidx_gasnete_amref_markdone_reph    (GASNETE_HANDLER_BASE+9)
/* add new extended API handlers here and to the bottom of gasnet_extended.c */

#endif
