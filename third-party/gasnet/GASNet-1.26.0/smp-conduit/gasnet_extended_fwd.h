/*   $Source: bitbucket.org:berkeleylab/gasnet.git/smp-conduit/gasnet_extended_fwd.h $
 * Description: GASNet Extended API Header (forward decls)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_EXTENDED_FWD_H
#define _GASNET_EXTENDED_FWD_H

#define GASNET_EXTENDED_VERSION      1.2
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#define GASNET_EXTENDED_NAME         SMP
#define GASNET_EXTENDED_NAME_STR     _STRINGIFY(GASNET_EXTENDED_NAME)


#define _GASNET_HANDLE_T
/*  an opaque type representing a non-blocking operation in-progress initiated using the extended API */
struct _gasnete_op_t;
typedef struct _gasnete_op_t *gasnet_handle_t;
#define GASNET_INVALID_HANDLE ((gasnet_handle_t)0)
#define GASNETI_EOP_IS_HANDLE 1
#define GASNETI_DISABLE_EOP_INTERFACE 1

  /* if conduit-internal threads may call the Extended API and/or they may run
     progress functions, then define GASNETE_CONDUIT_THREADS_USING_TD to the
     maximum COUNT of such threads to allocate space for their threaddata
   */
#if 0
  #define GASNETE_CONDUIT_THREADS_USING_TD ###
#endif

#if GASNET_PSHM
#define GASNETE_COLL_CONDUIT_BARRIERS GASNETE_COLL_BARRIER_PSHM
#define GASNETE_BARRIER_DEFAULT "PSHM"
#define GASNETE_BARRIER_READENV() do { \
    if(GASNETE_ISBARRIER("PSHM")) gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_PSHM; \
  } while (0)
#define GASNETE_BARRIER_INIT(TEAM, TYPE, NODES, SUPERNODES) do { \
    if ((TYPE) == GASNETE_COLL_BARRIER_PSHM &&              \
        (TEAM) == GASNET_TEAM_ALL) {                        \
      gasnete_pshmbarrier_init(TEAM);                       \
    }                                                       \
  } while (0)
#endif /* GASNET_PSHM */

  /* this can be used to add statistical collection values 
     specific to the extended API implementation (see gasnet_help.h) */
#define GASNETE_CONDUIT_STATS(CNT,VAL,TIME)  \
        GASNETI_VIS_STATS(CNT,VAL,TIME)      \
        GASNETI_COLL_STATS(CNT,VAL,TIME)     \
        CNT(C, DYNAMIC_THREADLOOKUP, cnt)    

#define GASNETE_AUXSEG_DECLS \
    extern gasneti_auxseg_request_t gasnete_barr_auxseg_alloc(gasnet_seginfo_t *auxseg_info);
#define GASNETE_AUXSEG_FNS() gasnete_barr_auxseg_alloc, 

/* NOT using the AM-based implementation of get/put/memset */
#define GASNETE_USING_REF_EXTENDED_GET_BULK 0
#define GASNETE_USING_REF_EXTENDED_PUT_BULK 0
#define GASNETE_USING_REF_EXTENDED_PUT      0
#define GASNETE_USING_REF_EXTENDED_MEMSET   0

/* All the "real work" is in gasnet_extended_help_extra.h */
#define GASNETE_HAVE_EXTENDED_HELP_EXTRA_H 1

#endif

