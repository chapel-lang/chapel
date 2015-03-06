/*
 * Description: GASNet Extended API Implementation for MXM conduit
 * Copyright (c)  2012, Mellanox Technologies LTD. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_EXTENDED_FWD_H
#define _GASNET_EXTENDED_FWD_H

#define GASNET_EXTENDED_VERSION      1.2
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#define GASNET_EXTENDED_NAME         MXM
#define GASNET_EXTENDED_NAME_STR     _STRINGIFY(GASNET_EXTENDED_NAME)


#define _GASNET_HANDLE_T
/*  an opaque type representing a non-blocking operation in-progress initiated using the extended API */
struct _gasnete_op_t;
typedef struct _gasnete_op_t *gasnet_handle_t;
#define GASNET_INVALID_HANDLE ((gasnet_handle_t)0)
#define GASNETI_EOP_IS_HANDLE 1

/* this can be used to add statistical collection values
 *      specific to the extended API implementation (see gasnet_help.h) */
#define GASNETE_CONDUIT_STATS(CNT,VAL,TIME)  \
            GASNETI_VIS_STATS(CNT,VAL,TIME)      \
        GASNETI_COLL_STATS(CNT,VAL,TIME)     \
        CNT(C, DYNAMIC_THREADLOOKUP, cnt)


#define GASNETI_DIRECT_PUT      1
#define GASNETI_DIRECT_PUT_BULK 1
#define GASNETI_DIRECT_GET      1
#define GASNETI_DIRECT_GET_BULK 1
#define GASNETI_DIRECT_GET_NB   1
#define GASNETI_DIRECT_GET_NBI  1
#define GASNETI_DIRECT_PUT_NBI  1

/* Configure use of AM-based implementation of get/put/memset */
/* NOTE: Barriers, Collectives, VIS may use GASNETE_USING_REF_* in algorithm selection */
#define GASNETE_USING_REF_EXTENDED_MEMSET   1

/* Conduit implements memset directly via amref: */
#define gasnete_amref_memset_nb     gasnete_memset_nb
#define gasnete_amref_memset_nbi    gasnete_memset_nbi

/* XXX: RDMADISSEM leads to error which need to be investigated! */
#define GASNETE_BARRIER_DEFAULT "AMDISSEM"

#define GASNETE_AUXSEG_DECLS \
                   extern gasneti_auxseg_request_t gasnete_barr_auxseg_alloc(gasnet_seginfo_t *auxseg_info);
#define GASNETE_AUXSEG_FNS() gasnete_barr_auxseg_alloc,

#endif /* _GASNET_EXTENDED_FWD_H */
