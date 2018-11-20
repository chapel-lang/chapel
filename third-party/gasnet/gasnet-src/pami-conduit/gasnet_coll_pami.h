/*   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/gasnet_coll_pami.h $
 * Description: GASNet extended collectives implementation on PAMI
 * Copyright 2012, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */


#ifndef _GASNET_COLL_PAMI_H
#define _GASNET_COLL_PAMI_H

#define GASNETI_NEED_GASNET_COLL_H 1
#include <gasnet_core_internal.h>

#if GASNET_PAMI_NATIVE_COLL

#include <gasnet_refcoll.h>
#include <gasnet_coll_internal.h>

/* Flags for enable/disable each operation: */
extern int gasnete_use_pami_allga;
extern int gasnete_use_pami_allto;
extern int gasnete_use_pami_bcast;
extern int gasnete_use_pami_gathr;
extern int gasnete_use_pami_scatt;

/* Partially initialized pami_xfer_t each operation: */
extern pami_xfer_t gasnete_op_template_allga;
extern pami_xfer_t gasnete_op_template_allto;
extern pami_xfer_t gasnete_op_template_bcast;
extern pami_xfer_t gasnete_op_template_gathr;
extern pami_xfer_t gasnete_op_template_scatt;
#if GASNET_PAR
extern pami_xfer_t gasnete_op_template_allgavi;
extern pami_xfer_t gasnete_op_template_alltovi;
extern pami_xfer_t gasnete_op_template_gathrvi;
extern pami_xfer_t gasnete_op_template_scattvi;
#endif

#endif /* GASNET_PAMI_NATIVE_COLL */

#endif /* _GASNET_COLL_PAMI_H */
