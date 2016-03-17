/*
 * Description: GASNet header for MXM conduit core (forward definitions)
 * Copyright (c)  2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright (c)  2012, Mellanox Technologies LTD. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
#error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_FWD_H
#define _GASNET_CORE_FWD_H

#define GASNET_CORE_VERSION      1.2
#define GASNET_CORE_VERSION_STR  _STRINGIFY(GASNET_CORE_VERSION)
#define GASNET_CORE_NAME         MXM
#define GASNET_CORE_NAME_STR     _STRINGIFY(GASNET_CORE_NAME)
#define GASNET_CONDUIT_NAME      GASNET_CORE_NAME
#define GASNET_CONDUIT_NAME_STR  _STRINGIFY(GASNET_CONDUIT_NAME)
#define GASNET_CONDUIT_MXM       1

/* GASNET_PSHM defined 1 if this conduit supports PSHM. leave undefined otherwise. */
#if GASNETI_PSHM_ENABLED
#define GASNET_PSHM 1
#endif

/*  defined to be 1 if gasnet_init guarantees that the remote-access memory segment will be aligned  */
/*  at the same virtual address on all nodes. defined to 0 otherwise */
#if GASNETI_DISABLE_ALIGNED_SEGMENTS || GASNET_PSHM
#define GASNET_ALIGNED_SEGMENTS   0 /* user or PSHM disabled segment alignment */
#else
/* (###)
 * MXM conduit doesn't guarantees that the remote-access memory segment will be aligned yet.
 */
#define GASNET_ALIGNED_SEGMENTS   0
#endif

/* conduits should define GASNETI_CONDUIT_THREADS to 1 if they have one or more
   "private" threads which may be used to run AM handlers, even under GASNET_SEQ
   this ensures locking is still done correctly, etc
 */
#if 1
/* (###)
 * MXM conduit doesn't have threads, but MXM has progress thread
 * that can receive messages and call AM callback.
 */
#define GASNETI_CONDUIT_THREADS 1
#endif

/* define to 1 if your conduit may interrupt an application thread
   (e.g. with a signal) to run AM handlers (interrupt-based handler dispatch)
 */
#if (0)
/* (###)
 * MXM conduit doesn't use interrupt-based dispatching.
 */
#define GASNETC_USE_INTERRUPTS 1
#endif

/* define these to 1 if your conduit supports PSHM, but cannot use the
   default interfaces. (see template-conduit/gasnet_core.c and gasnet_pshm.h)
 */
#if (0)
/* (###)
 * Using default interfaces for PSHM
 */
#define GASNETC_GET_HANDLER 1
typedef ### gasnetc_handler_t;
#endif

#if (0)
/* (###)
 * Using default interfaces for PSHM
 */
#define GASNETC_TOKEN_CREATE 1
#endif

/* this can be used to add conduit-specific
   statistical collection values (see gasnet_trace.h) */
#define GASNETC_CONDUIT_STATS(CNT,VAL,TIME)

#endif
