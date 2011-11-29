/*   $Source: /var/local/cvs/gasnet/shmem-conduit/gasnet_extended_fwd.h,v $
 *     $Date: 2009/10/28 00:39:43 $
 * $Revision: 1.19 $
 * Description: GASNet Extended API Header (forward decls)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_EXTENDED_FWD_H
#define _GASNET_EXTENDED_FWD_H

#define GASNET_EXTENDED_VERSION      1.8
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#define GASNET_EXTENDED_NAME         SHMEM
#define GASNET_EXTENDED_NAME_STR     _STRINGIFY(GASNET_EXTENDED_NAME)

#define _GASNET_HANDLE_T
typedef int * gasnet_handle_t;
#define GASNET_INVALID_HANDLE ((gasnet_handle_t)0)
#define GASNETI_DISABLE_EOP_INTERFACE

/* shmem-conduit has a custom valget implementation */
#define _GASNET_VALGET_HANDLE_T
typedef uintptr_t gasnet_valget_handle_t;

#define _GASNET_REGISTER_VALUE_T
#define SIZEOF_GASNET_REGISTER_VALUE_T SIZEOF_VOID_P
typedef uintptr_t gasnet_register_value_t;

  /* this can be used to add statistical collection values 
     specific to the extended API implementation (see gasnet_help.h) */
#define GASNETE_CONDUIT_STATS(CNT,VAL,TIME)  \
        CNT(C, DYNAMIC_THREADLOOKUP, cnt)    \
	GASNETI_VIS_STATS(CNT,VAL,TIME)      \
	GASNETI_COLL_STATS(CNT,VAL,TIME)

#define GASNETE_HAVE_EXTENDED_HELP_EXTRA_H

#ifdef GASNETE_GLOBAL_ADDRESS
  /* tweaks required for bounds checking on clients who lie about node number*/
  extern int _gasneti_in_segment_bc(gasnet_node_t node, const void *ptr, size_t nbytes);
  #define gasneti_in_segment_bc _gasneti_in_segment_bc
  #define gasneti_in_nodes_bc(node) (node == (gasnet_node_t)-1 || node < gasneti_nodes)
  #define gasneti_in_segment_allowoutofseg_bc gasneti_in_segment_bc
#endif

#define _GASNETE_MYTHREAD
struct _gasnete_threaddata_t;
extern struct _gasnete_threaddata_t * const gasnete_threaddata_ptr;
#define gasnete_mythread() (gasnete_threaddata_ptr)
#define GASNETI_MAX_THREADS 1

#define GASNETE_COLL_CONDUIT_BARRIERS GASNETE_COLL_BARRIER_SHMEM

#endif
