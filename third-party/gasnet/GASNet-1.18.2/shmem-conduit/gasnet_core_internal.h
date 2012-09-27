/*   $Source: /var/local/cvs/gasnet/shmem-conduit/gasnet_core_internal.h,v $
 *     $Date: 2009/09/18 23:33:44 $
 * $Revision: 1.14 $
 * Description: GASNet shmem conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#if defined(CRAY_SHMEM) || defined(SGI_SHMEM)
#include <mpp/shmem.h>
#elif defined(QUADRICS_SHMEM)
#include <shmem.h>
#endif

extern intptr_t		*gasnetc_segment_shptr_off;

/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 1

/* -------------------------------------------------------------------- */
#define GASNETC_HANDLER_BASE  1 /* reserve 1-63 for the core API */
#define _hidx_gasnetc_auxseg_reqh             (GASNETC_HANDLER_BASE+0)
/* add new core API handlers here and to the bottom of gasnet_core.c */

/* ------------------------------------------------------------------------------------ */
/* handler table (recommended impl) */
#define GASNETC_MAX_NUMHANDLERS   256
extern gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS];

/*
 * Active Message header encoding (always 4 bytes)
 *
 * Currently scales to 65K PEs.  This may be too conservative in a not too
 * distance future.
 */
#define GASNETC_REQREP_M    0x80000000
#define GASNETC_TYPE_M	    0x60000000
#define GASNETC_NUMARGS_M   0x1f000000
#define GASNETC_HANDLER_M   0x00ff0000
#define GASNETC_NODEID_M    0x0000ffff

#define GASNETC_REQUEST_T   0x00000000
#define GASNETC_REPLY_T	    0x80000000

#define GASNETC_AMSHORT_T   0x00000000
#define GASNETC_AMMED_T	    0x20000000
#define GASNETC_AMLONG_T    0x40000000

#define GASNETC_HANDLER_SHIFT	16
#define GASNETC_NUMARGS_SHIFT	24

/*
 * Pack/Unpack AM Headers
 */
#define GASNETC_AMHEADER_PACK(req,type,numargs,handler,nodeid)		\
	    (gasneti_assert((numargs)<=GASNETC_MAX_ARGS),		\
	    ((req) | (type) | ((handler)<<GASNETC_HANDLER_SHIFT) |	\
	     ((numargs)<<GASNETC_NUMARGS_SHIFT) | (nodeid)))

#define GASNETC_AMHEADER_UNPACK(hdr,req,type,numargs,handler,nodeid) do {   \
	 (req) = (hdr) & GASNETC_REQREP_M;				    \
	 (type) = (hdr) & GASNETC_TYPE_M;				    \
	 (numargs) = ((hdr) & GASNETC_NUMARGS_M) >> GASNETC_NUMARGS_SHIFT;  \
	 (handler) = ((hdr) & GASNETC_HANDLER_M) >> GASNETC_HANDLER_SHIFT;  \
	 (nodeid) = (hdr) & GASNETC_NODEID_M;			            \
	} while (0)

#define GASNETC_MEDHEADER_PADARG(numargs) \
        (!((numargs & 0x1) ^ ((GASNETC_MED_HEADERSZ>>2) & 0x1)))

/*
 * Simpler header queries
 */
#define GASNETC_AMHEADER_ISREQUEST(hdr)			\
	    (((hdr) & GASNETC_REQREP_M) == GASNETC_REQUEST_T)

#define GASNETC_AMHEADER_ISREPLY(hdr)	!GASNETC_AMHEADER_ISREQUEST(hdr)

#define GASNETC_AMHEADER_NODEID(hdr)	((hdr) & GASNETC_NODEID_M)

#define GASNETC_ARGS_WRITE(buf, argptr, numargs)                        \
        do {    int _i; int32_t *_pbuf = (int32_t *) buf;               \
                for (_i = 0; _i < (numargs); _i++)      {               \
                        _pbuf[_i] = (int32_t) va_arg((argptr), int);    \
                }                                                       \
           } while (0)

/*
 * AMQUEUE DEPTH and maximum sizes
 */
#if PLATFORM_ARCH_CRAYX1
  #define GASNETC_AMQUEUE_DEFAULT_DEPTH 32 /* allow polling with a single bitfield load */
#else
  #define GASNETC_AMQUEUE_DEFAULT_DEPTH 64
#endif
#define GASNETC_AMQUEUE_MAX_DEPTH	256
#define GASNETC_AMQUEUE_MAX_FIELDS	(2*GASNETC_AMQUEUE_MAX_DEPTH/sizeof(uintptr_t))
#define GASNETC_AMQUEUE_FREE_S		0
#define GASNETC_AMQUEUE_USED_S		1
#define GASNETC_AMQUEUE_DONE_S		2

#define GASNETC_AMQUEUE_SIZE_VALID(q)	(GASNETI_POWEROFTWO(q) && (q)>1 && \
					    (q)<=GASNETC_AMQUEUE_MAX_DEPTH)

/*
 * Each queue slot requires some payload area to store AM arguments and
 * AMMedium payloads.
 */
typedef
struct _gasnetc_am_packet
{
	int	    state;  /* One of FREE, USED, DONE */
	uint32_t    header;
	char	    payload[GASNETC_MAX_MEDIUM_TOTAL];
}
gasnetc_am_packet_t;

/*
 * Senders really only need stubs that are large enough to hold a header and up
 * to 16 args
 */
typedef
struct _gasnetc_am_stub
{
	uint32_t    args[GASNETC_MAX_ARGS+4];
	/* 
	 * arg[0] holds the AM header
	 *
	 * Short use arg[1-16] for args
	 *
	 * Longs use arg[1]    for length
	 *   64-bit:
	 *           arg[2-3]  for ptr to payload
	 *           arg[4-19] for args
	 *   32-bit:
	 *           arg[2]    for ptr to payload
	 *           arg[3-18] for args
	 *
	 * Meds  use arg[1]    for length
	 *           arg[2-17] for args
	 *           arg[18... for payload
	 *           (payload may be earlier if <16 args are used)
	 */
}
gasnetc_am_stub_t;

/*
 * The header is sometimes useful in its unpacked form
 */
typedef
struct _gasnetc_am_header
{
	uint32_t	reqrep;
	uint32_t	type;
	uint32_t	numargs;
	uint32_t	handler;
	gasnet_node_t	pe;
}
gasnetc_am_header_t;

extern int  gasnetc_amq_idx[2]; /* request, reply */
extern int  gasnetc_amq_depth;
extern int  gasnetc_amq_mask;

extern gasnetc_am_packet_t  gasnetc_amq_reqs[2*GASNETC_AMQUEUE_MAX_DEPTH];

#ifdef CRAY_SHMEM
extern volatile long	gasnetc_amq_reqfields[GASNETC_AMQUEUE_MAX_FIELDS];
extern long		gasnetc_amq_numfields;
#endif

GASNETI_INLINE(gasnetc_AMQueueAcquire)
int gasnetc_AMQueueAcquire(gasnet_node_t pe, int reqrep) {
    int	idx;
    const int isreply = (reqrep == GASNETC_REPLY_T);

    #ifdef QUADRICS_SHMEM
      idx = random() & gasnetc_amq_mask;
    #else
      idx = shmem_int_finc(&gasnetc_amq_idx[isreply], (int) pe) & gasnetc_amq_mask;
    #endif
    if (isreply) idx += gasnetc_amq_depth;

    /* Once we have the ID, cswap until the selected slot is free  */

    while (shmem_int_cswap(&gasnetc_amq_reqs[idx].state, 
	    GASNETC_AMQUEUE_FREE_S, GASNETC_AMQUEUE_USED_S, (int) pe) 
            != GASNETC_AMQUEUE_FREE_S) {
        /* while queuing a reply, we must drain incoming replies to prevent deadlock
           also, it is *only* safe to run reply handlers here, otherwise the 
           progress engine might erroneously try to re-execute the request already being run
         */
	_gasnetc_AMPoll(isreply);
    }
    return idx;
}

GASNETI_INLINE(gasnetc_AMQueueRelease)
void gasnetc_AMQueueRelease(gasnet_node_t pe, int idx)
{
  #ifdef CRAY_SHMEM
    /*
     * We are trying to find to which id in the bitvector the current idx will
     * map to.  For a long of 64 bits, bits map to bit vector indeces as
     * follows:
     *   0- 63 => bit vector id 0
     *  64-127 => bit vector id 1
     *  . .
     *
     * The intuition is to get the number of leading zeros in idx, which should
     * be at the least (64-9=55) for indexes between 256 and 511 and 64 if the
     * index is 0.  
     *
     * If we subtract the number of leading zeros from 64, the value is between
     * 0 and 9.  Since we really want a value between 0 and 4, we simply make
     * sure that the smallest idx passed into _leadz has its sixth bit set.
     *
     */
    int  field_no; 
    long field_mask;

    field_no = (unsigned long) idx >> 6;
    field_mask = 0x8000000000000000ul >> (idx & 63);

    gasneti_assert(idx >= 0 && idx < 2*gasnetc_amq_depth);
    gasneti_assert(field_no >= 0 && field_no < GASNETC_AMQUEUE_MAX_FIELDS);

    shmem_long_mswap((long *) &gasnetc_amq_reqfields[field_no], 
		     field_mask, field_mask, pe);

  #else /* ! CRAY_SHMEM */
    gasneti_assert(idx >= 0 && idx < 2*gasnetc_amq_depth);
    gasneti_assert(sizeof(int) == sizeof(uint32_t));
    shmem_int_p(&(gasnetc_amq_reqs[idx].state), 
		GASNETC_AMQUEUE_DONE_S, (int) pe);
  #endif

    return;
}

/* -------------------------------------------------------------------------- */

#endif
