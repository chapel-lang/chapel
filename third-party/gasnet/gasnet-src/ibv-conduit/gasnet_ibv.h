/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/gasnet_core_internal.h $
 * Description: GASNet ibv conduit header for internal defns common to Core & Extended APIs
 * Copyright 2016, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_IBV_H
#define _GASNET_IBV_H

#include <gasnet_event_internal.h>

/* ------------------------------------------------------------------------------------ *
 *  Common op completion logic
 * ------------------------------------------------------------------------------------ */

// TODO-EX: the iop values of this enum are currently unused
typedef enum {
  gasnetc_comptype_eop_alc,
  gasnetc_comptype_eop_get,
  gasnetc_comptype_eop_put,
  gasnetc_comptype_iop_alc,
  gasnetc_comptype_iop_get,
  gasnetc_comptype_iop_put,
} gasnetc_comptype_t;

GASNETI_INLINE(gasnetc_complete_eop)
int gasnetc_complete_eop(gasnete_eop_t *eop, gasnetc_comptype_t type)
{ // Advance and test the proper counter
  gasnete_op_t *op = (gasnete_op_t*)eop;
  gasnetc_atomic_val_t completed;
  gasnetc_atomic_val_t initiated;

  switch (type) {
    case gasnetc_comptype_eop_alc:
      completed = gasnetc_atomic_add(&eop->completed_alc, 1, GASNETI_ATOMIC_ACQ);
      initiated = eop->initiated_alc;
      break;
    case gasnetc_comptype_eop_put:
      completed = gasnetc_atomic_add(&eop->completed_cnt, 1, GASNETI_ATOMIC_ACQ);
      initiated = eop->initiated_cnt;
      break;
    case gasnetc_comptype_eop_get:
      completed = gasnetc_atomic_add(&eop->completed_cnt, 1, GASNETI_ATOMIC_ACQ | GASNETI_ATOMIC_REL);
      initiated = eop->initiated_cnt;
      break;
    default:
      gasneti_unreachable();
  }

  if (completed == (initiated & GASNETI_ATOMIC_MAX)) {
    switch (type) {
      case gasnetc_comptype_eop_alc:
        GASNETE_EOP_LC_FINISH(op);
        break;
      case gasnetc_comptype_eop_put:
      case gasnetc_comptype_eop_get:
        GASNETE_EOP_MARKDONE(op);
        break;
      default:
        gasneti_unreachable();
    }
    return 1;
  }
  return 0;
}

#endif
