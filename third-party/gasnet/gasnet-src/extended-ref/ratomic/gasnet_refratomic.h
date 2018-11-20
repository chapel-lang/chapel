/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/ratomic/gasnet_refratomic.h $
 * Description: Reference implemetation of GASNet Remote Atomics, using Active Messages, Header
 * Copyright 2017, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_EXTENDED_REFRATOMIC_H
#define _GASNET_EXTENDED_REFRATOMIC_H

#if ! GASNETE_BUILD_AMRATOMIC_STUBS // otherwise using a stub w/o AM handlers
//
// AM Handler Table entries
//

/* conduits may override this to relocate the ref-ratomic handlers */
#ifndef GASNETE_RATOMIC_HANDLER_BASE
#define GASNETE_RATOMIC_HANDLER_BASE 98
#endif

#define _hidx_gasnete_amratomic_reqh  (GASNETE_RATOMIC_HANDLER_BASE+0)
#define _hidx_gasnete_amratomic_reph  (GASNETE_RATOMIC_HANDLER_BASE+1)

MEDIUM_HANDLER_DECL(gasnete_amratomic_reqh,3,5);
MEDIUM_HANDLER_DECL(gasnete_amratomic_reph,1,2);

#define GASNETE_AMRATOMIC_HANDLERS() \
    gasneti_handler_tableentry_with_bits(gasnete_amratomic_reqh,3,5,REQUEST,MEDIUM,0),   \
    gasneti_handler_tableentry_with_bits(gasnete_amratomic_reph,1,2,REPLY,MEDIUM,0),
#endif // ! GASNETE_BUILD_AMRATOMIC_STUBS

#endif
