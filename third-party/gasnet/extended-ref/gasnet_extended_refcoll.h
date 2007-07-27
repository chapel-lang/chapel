/*   $Source$
 *     $Date$
 * $Revision$
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
#define GASNETE_COLL_HANDLER_BASE 124
#endif

#define _hidx_gasnete_coll_p2p_memcpy_reqh  (GASNETE_COLL_HANDLER_BASE+0)
#define _hidx_gasnete_coll_p2p_short_reqh   (GASNETE_COLL_HANDLER_BASE+1)
#define _hidx_gasnete_coll_p2p_med_reqh	    (GASNETE_COLL_HANDLER_BASE+2)
#define _hidx_gasnete_coll_p2p_long_reqh    (GASNETE_COLL_HANDLER_BASE+3)

/*---------------------------------------------------------------------------------*/

#ifndef GASNETE_COLL_P2P_OVERRIDE

  MEDIUM_HANDLER_DECL(gasnete_coll_p2p_memcpy_reqh,4,5);
  SHORT_HANDLER_NOBITS_DECL(gasnete_coll_p2p_short_reqh, 5);
  MEDIUM_HANDLER_NOBITS_DECL(gasnete_coll_p2p_med_reqh,6);
  LONG_HANDLER_NOBITS_DECL(gasnete_coll_p2p_long_reqh,5);

  #define GASNETE_COLL_P2P_HANDLERS() \
      gasneti_handler_tableentry_with_bits(gasnete_coll_p2p_memcpy_reqh), \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_short_reqh),    \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_med_reqh),      \
      gasneti_handler_tableentry_no_bits(gasnete_coll_p2p_long_reqh),

#elif !defined(GASNETE_COLL_P2P_HANDLERS)
  #define GASNETE_COLL_P2P_HANDLERS()
#endif

#define GASNETE_REFCOLL_HANDLERS()                           \
  /* ptr-width independent handlers */                       \
  /*  gasneti_handler_tableentry_no_bits(gasnete__reqh) */   \
                                                             \
  /* ptr-width dependent handlers */                         \
  /*  gasneti_handler_tableentry_with_bits(gasnete__reqh) */ \
                                                             \
  GASNETE_COLL_P2P_HANDLERS()                        

/*---------------------------------------------------------------------------------*/

#endif
