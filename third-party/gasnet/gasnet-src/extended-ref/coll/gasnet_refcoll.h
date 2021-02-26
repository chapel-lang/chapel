/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_refcoll.h $
 * Description: GASNet Collectives conduit header
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_EXTENDED_REFCOLL_H
#define _GASNET_EXTENDED_REFCOLL_H

/*---------------------------------------------------------------------------------*/
/* ***  Parameters *** */
/*---------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------*/
/* ***  Handlers *** */
/*---------------------------------------------------------------------------------*/

#ifndef GASNETE_COLL_P2P_OVERRIDE

  MEDIUM_HANDLER_DECL(gasnete_coll_p2p_memcpy_reqh,4,5);
  SHORT_HANDLER_NOBITS_DECL(gasnete_coll_p2p_short_reqh, 5);
  MEDIUM_HANDLER_NOBITS_DECL(gasnete_coll_p2p_med_reqh,6);
MEDIUM_HANDLER_NOBITS_DECL(gasnete_coll_p2p_med_counting_reqh,5);
  LONG_HANDLER_NOBITS_DECL(gasnete_coll_p2p_long_reqh,5);

  MEDIUM_HANDLER_NOBITS_DECL(gasnete_coll_p2p_med_tree_reqh,2);
  SHORT_HANDLER_NOBITS_DECL(gasnete_coll_p2p_advance_reqh,3);
  LONG_HANDLER_NOBITS_DECL(gasnete_coll_p2p_put_and_advance_reqh,3);
  MEDIUM_HANDLER_NOBITS_DECL(gasnete_coll_p2p_med_tree_reqh,2);

  #define GASNETE_COLL_P2P_HANDLERS() \
      gasneti_handler_tableentry_with_bits(gasnete_coll_p2p_memcpy_reqh,4,5,REQUEST,MEDIUM,0), \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_short_reqh,5,REQUEST,SHORT,0),    \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_med_reqh,6,REQUEST,MEDIUM,0),      \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_long_reqh,5,REQUEST,LONG,0),     \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_med_tree_reqh,2,REQUEST,MEDIUM,0), \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_advance_reqh,3,REQUEST,SHORT,0), \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_put_and_advance_reqh,3,REQUEST,LONG,0), \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_med_counting_reqh,5,REQUEST,MEDIUM,0),

#elif !defined(GASNETE_COLL_P2P_HANDLERS)
  #define GASNETE_COLL_P2P_HANDLERS()
#endif

#ifndef GASNETE_COLL_SCRATCH_OVERRIDE
SHORT_HANDLER_NOBITS_DECL(gasnete_coll_scratch_update_reqh, 2);
#define GASNETE_COLL_SCRATCH_HANDLERS() gasneti_handler_tableentry_no_bits(gasnete_coll_scratch_update_reqh,2,REQUEST,SHORT,0),
#endif

MEDIUM_HANDLER_NOBITS_DECL(gasnete_subteam_op_reqh,4);

#define GASNETE_REFCOLL_HANDLERS()                           \
  GASNETE_COLL_P2P_HANDLERS() GASNETE_COLL_SCRATCH_HANDLERS() \
  gasneti_handler_tableentry_no_bits(gasnete_subteam_op_reqh,4,REQUEST,MEDIUM,0),

#endif
