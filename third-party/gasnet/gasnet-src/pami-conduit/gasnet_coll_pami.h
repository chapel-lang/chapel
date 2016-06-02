/*   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/gasnet_coll_pami.h $
 * Description: GASNet extended collectives implementation on PAMI
 * Copyright 2012, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */


#ifndef _GASNET_COLL_PAMI_H
#define _GASNET_COLL_PAMI_H

#include <gasnet_core_internal.h>

#if GASNET_PAMI_NATIVE_COLL

#include <gasnet_extended_refcoll.h>
#include <gasnet_coll.h>
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

#if GASNET_PAR
  /* thread (image) barrier returning non-zero to exactly one caller */
  GASNETI_INLINE(gasnete_coll_pami_images_barrier)
  int gasnete_coll_pami_images_barrier(gasnet_team_handle_t team) {
    const int phase = team->pami.barrier_phase;
    gasneti_atomic_t * const counter = &team->pami.barrier_counter[phase];
    const int last = gasneti_atomic_decrement_and_test(counter, GASNETI_ATOMIC_REL);
    const int goal = phase ^ 1;

    if (last) {
      gasneti_atomic_set(counter, team->my_images, GASNETI_ATOMIC_MB_POST);
      team->pami.barrier_phase = goal;
    } else {
      gasneti_waituntil( team->pami.barrier_phase == goal );
    }

    return last;
  }
#else
  #define gasnete_coll_pami_images_barrier(_t) (1)
#endif

#endif /* GASNET_PAMI_NATIVE_COLL */

#endif /* _GASNET_COLL_PAMI_H */
