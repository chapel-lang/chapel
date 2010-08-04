/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_extended_refcoll.h,v $
 *     $Date: 2010/05/07 03:14:25 $
 * $Revision: 1.4.2.1 $
 * Description: GASNet Collectives conduit header
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_EXTENDED_REFCOLL_H
#define _GASNET_EXTENDED_REFCOLL_H

#include <gasnet_handler.h>

/*---------------------------------------------------------------------------------*/
/* ***  Parameters *** */
/*---------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------*/
/* ***  Handlers *** */
/*---------------------------------------------------------------------------------*/
/* conduits may override this to relocate the ref-coll handlers */
#ifndef GASNETE_COLL_HANDLER_BASE
#define GASNETE_COLL_HANDLER_BASE 119
#endif

#define _hidx_gasnete_coll_p2p_memcpy_reqh  (GASNETE_COLL_HANDLER_BASE+0)
#define _hidx_gasnete_coll_p2p_short_reqh   (GASNETE_COLL_HANDLER_BASE+1)
#define _hidx_gasnete_coll_p2p_med_reqh	    (GASNETE_COLL_HANDLER_BASE+2)
#define _hidx_gasnete_coll_p2p_long_reqh    (GASNETE_COLL_HANDLER_BASE+3)
#define _hidx_gasnete_coll_p2p_med_tree_reqh (GASNETE_COLL_HANDLER_BASE+4)
#define _hidx_gasnete_coll_p2p_advance_reqh (GASNETE_COLL_HANDLER_BASE+5)
#define _hidx_gasnete_coll_p2p_put_and_advance_reqh (GASNETE_COLL_HANDLER_BASE+6)
#define _hidx_gasnete_coll_p2p_med_counting_reqh (GASNETE_COLL_HANDLER_BASE+7)
#define _hidx_gasnete_coll_p2p_seg_put_reqh (GASNETE_COLL_HANDLER_BASE+8)

/*---------------------------------------------------------------------------------*/
/*four args: team id, node id, seq number, head, tail*/
#define GASNETE_COLL_NUM_SCRATCH_HANDLERS 1
#ifndef GASNETE_COLL_SCRATCH_HANDLER_BASE
#define GASNETE_COLL_SCRATCH_HANDLER_BASE (GASNETE_COLL_HANDLER_BASE-GASNETE_COLL_NUM_SCRATCH_HANDLERS)
#endif

#define _hidx_gasnete_coll_scratch_update_reqh (GASNETE_COLL_SCRATCH_HANDLER_BASE+0)

#define GASNETE_COLL_NUM_TEAM_HANDLERS 1
#ifndef GASNETE_COLL_TEAM_HANDLER_BASE
#define GASNETE_COLL_TEAM_HANDLER_BASE (GASNETE_COLL_SCRATCH_HANDLER_BASE-GASNETE_COLL_NUM_SCRATCH_HANDLERS)
#endif
#define _hidx_gasnete_coll_teamid_reqh (GASNETE_COLL_TEAM_HANDLER_BASE+0)

#ifndef GASNETE_COLL_P2P_OVERRIDE

  MEDIUM_HANDLER_DECL(gasnete_coll_p2p_memcpy_reqh,4,5);
  SHORT_HANDLER_NOBITS_DECL(gasnete_coll_p2p_short_reqh, 5);
  MEDIUM_HANDLER_NOBITS_DECL(gasnete_coll_p2p_med_reqh,6);
MEDIUM_HANDLER_NOBITS_DECL(gasnete_coll_p2p_med_counting_reqh,5);
  LONG_HANDLER_NOBITS_DECL(gasnete_coll_p2p_long_reqh,5);

  MEDIUM_HANDLER_NOBITS_DECL(gasnete_coll_p2p_med_tree_reqh,2);
  SHORT_HANDLER_NOBITS_DECL(gasnete_coll_p2p_advance_reqh,3);
  LONG_HANDLER_NOBITS_DECL(gasnete_coll_p2p_put_and_advance_reqh,3);
  LONG_HANDLER_NOBITS_DECL(gasnete_coll_p2p_seg_put_reqh,3);

  #define GASNETE_COLL_P2P_HANDLERS() \
      gasneti_handler_tableentry_with_bits(gasnete_coll_p2p_memcpy_reqh), \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_short_reqh),    \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_med_reqh),      \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_long_reqh),     \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_med_tree_reqh), \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_advance_reqh), \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_put_and_advance_reqh), \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_med_counting_reqh), \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_seg_put_reqh), 

#elif !defined(GASNETE_COLL_P2P_HANDLERS)
  #define GASNETE_COLL_P2P_HANDLERS()
#endif

#ifndef GASNETE_COLL_SCRATCH_OVERRIDE
SHORT_HANDLER_NOBITS_DECL(gasnete_coll_scratch_update_reqh, 2);
#define GASNETE_COLL_SCRATCH_HANDLERS() gasneti_handler_tableentry_no_bits(gasnete_coll_scratch_update_reqh),
#endif

#ifndef GASNETE_COLL_TEAM_OVERRIDE
SHORT_HANDLER_NOBITS_DECL(gasnete_coll_teamid_reqh, 1);
#define GASNETE_COLL_TEAM_HANDLERS() gasneti_handler_tableentry_no_bits(gasnete_coll_teamid_reqh),
#endif

#define GASNETE_REFCOLL_HANDLERS()                           \
  /* ptr-width independent handlers */                       \
  /*  gasneti_handler_tableentry_no_bits(gasnete__reqh) */   \
                                                             \
  /* ptr-width dependent handlers */                         \
  /*  gasneti_handler_tableentry_with_bits(gasnete__reqh) */ \
                                                             \
  GASNETE_COLL_P2P_HANDLERS() GASNETE_COLL_SCRATCH_HANDLERS() GASNETE_COLL_TEAM_HANDLERS()

extern int gasnete_coll_init_done;
extern int gasnete_coll_opt_enabled;
#endif
