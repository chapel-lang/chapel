/* $Source: /var/local/cvs/gasnet/gm-conduit/gasnet_core_fwd.h,v $
 * $Date: 2009/10/28 00:09:11 $
 * $Revision: 1.38 $
 * Description: GASNet header for GM conduit core (forward definitions)
 * Copyright 2002, Christian Bell <csbell@cs.berkeley.edu>
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_FWD_H
#define _GASNET_CORE_FWD_H

#define GASNET_CORE_VERSION      1.10
#define GASNET_CORE_VERSION_STR  _STRINGIFY(GASNET_CORE_VERSION)
#define GASNET_CORE_NAME         GM
#define GASNET_CORE_NAME_STR     _STRINGIFY(GASNET_CORE_NAME)
#define GASNET_CONDUIT_NAME      GASNET_CORE_NAME
#define GASNET_CONDUIT_NAME_STR  _STRINGIFY(GASNET_CONDUIT_NAME)
#define GASNET_CONDUIT_GM        1

  /*  defined to be 1 if gasnet_init guarantees that the remote-access memory segment will be aligned  */
  /*  at the same virtual address on all nodes. defined to 0 otherwise */
#if GASNETI_DISABLE_ALIGNED_SEGMENTS || GASNET_PSHM
  #define GASNET_ALIGNED_SEGMENTS   0 /* user or PSHM disabled segment alignment */
#else
  #define GASNET_ALIGNED_SEGMENTS   1
#endif
#define GASNET_MAXNODES		1024

  /* conduits should define GASNETI_CONDUIT_THREADS to 1 if they have one or more 
     "private" threads which may be used to run AM handlers, even under GASNET_SEQ
     this ensures locking is still done correctly, etc
   */
/* #define GASNETI_CONDUIT_THREADS 1 */

  /* define to 1 if your conduit may interrupt an application thread 
     (e.g. with a signal) to run AM handlers (interrupt-based handler dispatch)
   */
/* #define GASNETC_USE_INTERRUPTS 1 */

/* only have firehose for now */
#define GASNETC_FIREHOSE

/* Default board number */
#define GASNETC_DEFAULT_GM_BOARD_NUM	0

  /* define these to 1 if your conduit supports PSHM, but cannot use the
     default interfaces. (see template-conduit/gasnet_core.c and gasnet_pshm.h)
   */
#define GASNETC_GET_HANDLER 1 /* Not using the default handler table impl */
typedef uint16_t gasnetc_handler_t;
/* #define GASNETC_TOKEN_CREATE 1 */

  /* this can be used to add conduit-specific 
     statistical collection values (see gasnet_help.h) */
#define GASNETC_CONDUIT_STATS(CNT,VAL,TIME)                \
        VAL(C, HIGHRECV_FAST, packet bytes)                \
        VAL(C, HIGHRECV_NORMAL, packet bytes)              \
        VAL(C, LOWRECV_FAST, packet bytes)                 \
        VAL(C, LOWRECV_NORMAL, packet bytes)               \
        VAL(C, AMREQUESTLONGASYNC_ZEROCOPY, payload bytes) \
        VAL(C, AMREQUESTLONGASYNC_ONECOPY, payload bytes)  \
        VAL(C, AMREQUESTLONGASYNC_TWOCOPY, payload bytes)  \
        VAL(C, AMREQUESTLONG_ONECOPY, payload bytes)       \
        VAL(C, AMREQUESTLONG_TWOCOPY, payload bytes)       \
        VAL(C, AMREPLYLONG_ONECOPY, payload bytes)         \
        VAL(C, AMREPLYLONG_TWOCOPY, payload bytes)         \
        VAL(C, AMREPLYLONGASYNC_ZEROCOPY, payload bytes)      

#define _GASNET_NODE_T
typedef uint16_t	gasnet_node_t;
#define _GASNET_HANDLER_T
typedef uint8_t		gasnet_handler_t;

#define _GASNET_TOKEN_T
struct gasnetc_bufdesc;
typedef struct gasnetc_bufdesc *gasnet_token_t;

#endif /* _GASNET_CORE_FWD_H */
