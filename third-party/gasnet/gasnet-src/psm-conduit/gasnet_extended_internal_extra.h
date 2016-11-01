/*
 * Description: PSM extended API Op management
 * Copyright (c) 2015 Intel Corporation. All rights reserved.
 * Terms of use are as specified in license.txt
 */


#ifndef _GASNET_EXTENDED_EXTRA_H
#define _GASNET_EXTENDED_EXTRA_H

/* -------------------------------------------------------------------------- */

#if GASNETE_EOP_COUNTED && !GASNETE_AMREF_USE_MARKDONE
  #define PSM_PACK_EOP_DONE(_eop)         (&(_eop)->completed_cnt)
  #define PSM_PACK_IOP_DONE(_iop,_getput) (&(_iop)->completed_##_getput##_cnt)
  #define PSM_MARK_DONE(_ptr,_isget)      gasneti_weakatomic_increment((gasneti_weakatomic_t *)(_ptr), \
                                                                   (_isget) ? GASNETI_ATOMIC_REL : 0)
#else
  #define PSM_PACK_EOP_DONE(_eop)         (_eop)
  #define PSM_PACK_IOP_DONE(_iop,_getput) (_iop)
  #define PSM_MARK_DONE(_ptr,_isget)      do {                                                      \
                                         if (_isget) gasneti_sync_writes();                     \
                                         gasnete_op_markdone((gasnete_op_t *)(_ptr), (_isget)); \
                                      } while (0)
#endif

void gasnete_op_markdone(gasnete_op_t *op, int isget);

#endif
