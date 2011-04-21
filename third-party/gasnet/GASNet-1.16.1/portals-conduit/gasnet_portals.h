#ifndef GASNET_PORTALS_H
#define GASNET_PORTALS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <portals/portals3.h>
#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>
/* ------------------------------------------------------------------------------------ */
/* MLW:  Support for Portals 3.0 */
#if GASNET_SEGMENT_EVERYTHING
    #error "GASNET_SEGMENT_EVERYTHING is not yet supported by portals-conduit"
#endif

#ifndef GASNETC_DEBUG
#define GASNETC_DEBUG 0
#endif

/* default fraction of phys mem to assume is pinnable under CNL */
#ifndef GASNETC_DEFAULT_PHYSMEM_PINNABLE_RATIO
#define GASNETC_DEFAULT_PHYSMEM_PINNABLE_RATIO 0.75
#endif

/* set to 1 to compile in Sandia specific Accelerated Portals code */
#ifndef GASNETC_USE_SANDIA_ACCEL
#define GASNETC_USE_SANDIA_ACCEL 0
#endif

/* set to 1 when testing credit algorithm.  Forces credit values specified
 * in env vars and ignores extra credits due to fixed size ReqRB buffers
 * Also, includes additional credit assounting stats and ability to print
 * activity on scavenge list for node gasnetc_debug_node.
 */
#ifndef GASNETC_CREDIT_TESTING
#define GASNETC_CREDIT_TESTING 0
#endif

/* Set to 1 if want to use gasneti_spinlock_t rather than gasneti_mutex_t
 * to control the state data in the per-peer gasnetc_conn_state[] records.
 * By default, we will use mutex_t.
 */
#ifndef GASNETC_USE_SPINLOCK
#define GASNETC_USE_SPINLOCK 0
#endif

/* Set to 1 to enable use of PtlMDUpdate in the firehose move callback.
 * The code that this enables currently works around an apparent bug in
 * PtlMDUPdate in which it returns PTL_INVALID_EQ when passed PTL_EQ_NONE.
 * However, it does NOT currently work-around GASNet bug 2617, and so is
 * disabled by default.
 * Additionally, testing done while examining Bug 2617 suggests that the
 * cost of an PtlMDUpdate in PE-2.1.50HD is as much as 50% higher than the
 * corresponding PtlMDUnlink() + PtlMDBind().
 */
#ifndef GASNETC_USE_MDUPDATE
#define GASNETC_USE_MDUPDATE 0
#endif

/* Comments on the value of GASNETC_CHUNKSIZE:
 *
 *         ==> NOTE: This value set in gasnet_core_help.h <==
 *
 * We manage space in our Request and Reply Send Buffers using a simple chunk allocator
 * and all chunks are the same size.  Additionally, our AMRequest receive buffers are
 * also allocated as multiples of GASNETC_CHUNKSIZE.
 *
 * This number determines the following set of values:
 *  - The size of an AMMedium (see gasnet_core.h)
 *  - The number of credits required to send a maximal sized AMMedium and therefore
 *    the GASNETC_MIN_CREDITS value (minimum number of credits that can be allocated
 *    to a node).
 *  - the max payload size of a packed AMRequestLong/AMRreplyLong.
 *    (Note that these two values are not required to be compile time constants
 *     and will vary with the number of arguments in the AMLong.)
 */
#ifndef GASNETC_CHUNKSIZE
#error "GASNETC_CHUNKSIZE not defined in gasnet_portals.h"
#endif

/* Define the number of AM Receive buffer bytes that are controlled by a single
 * flow-control credit.  That is, if node X has one credit to send an AM to node Y
 * it can send at most this many bytes as the length of the Portals Put operation.
 * Note that one credit also guarantees the existance of an event queue entry in the
 * AM Event queue on the target node.  An EQ entry is 128 bytes, which is not included
 * in this value.
 */
#define GASNETC_BYTES_PER_CREDIT  256

/* Define the MINIMUM number of credits that must be allocated to each node in the job.
 * All AMShort messages will fit within one credit.
 * All non-packed AMLong messages will require two credits (two messages => two EQ entries).
 * A full sized AM Medium message will require 4 credit if CHUNKSIZE is 1024 and
 * 8 credits if 2048.  In general:
 * GASNETC_MIN_CREDITS =  cealing(GASNETC_CHUNKSIZE/GASNETC_BYTES_PER_CREDIT) , or
 * (GASNETC_CHUNKSIZE/GASNETC_BYTES_PER_CREDIT + (GASNETC_CHUNKSIZE%GASNETC_BYTES_PER_CREDIT?1:0))
 * but rather than this ugly mess, we just state its value, which MUST be kept consistent
 * GASNet will fail in an assertion at startup if not.
 */
#if ((GASNETC_CHUNKSIZE % GASNETC_BYTES_PER_CREDIT) != 0)
#error "GASNETC_CHUNKSIZE MUST BE A MULTIPLE OF GASNET_BYTES_PER_CREDIT"
#endif
#define GASNETC_MIN_CREDITS       (GASNETC_CHUNKSIZE/GASNETC_BYTES_PER_CREDIT)

/* The following constant is the cutoff size for out-of-segment Put/Get
 * messages that should be copied through the ReqSB buffer (acting as a bounce buffer).
 * Larger messages will allocate a Temporary MD around the data rather than doing
 * the copy.  This protocal switch value is determined by running benchmarks
 * over Portals and will most definately be compute node OS dependent.
 * Pinning costs under Catamount are minimal and the switch is at about 1KB.
 * Finally, this value must be <= GASNETC_CHUNKSIZE.
 * We check this in an assertion at job startup time.
 * This is the *default* value, which can be overridden by env var.
 */
#if PLATFORM_OS_CATAMOUNT
#define GASNETC_PUTGET_BOUNCE_LIMIT_DFLT 1024
#else
/* Under CNL, this value is not yet known.  Assume as large as a CHUNK */
#define GASNETC_PUTGET_BOUNCE_LIMIT_DFLT GASNETC_CHUNKSIZE
#endif
#if (GASNETC_PUTGET_BOUNCE_LIMIT_DFLT > GASNETC_CHUNKSIZE)
#error "GASNETC_PUTGET_BOUNCE_LIMIT_DFLT MUST BE <= GASNETC_CHUNKSIZE"
#endif

/* Radix of the N-ary tree for bootstrap broadcast */
#ifndef GASNETC_BOOTSTRAP_BCAST_RADIX
#define GASNETC_BOOTSTRAP_BCAST_RADIX 8
#endif

/* Do we register an EQ handler with a queue or just poll ourselves */
#if GASNETC_USE_EQ_HANDLER
  #define GASNETC_EQ_HANDLER gasnetc_event_handler
  #define GASNETC_CALL_EQ_HANDLER(ev) gasnetc_event_handler(&ev)
#else
  #define GASNETC_EQ_HANDLER NULL
  #define GASNETC_CALL_EQ_HANDLER(ev) (*(gasnetc_ptl_event_handler)(ev.md.user_ptr))(&(ev)) 
#endif

/* bug workaround for missing ptl_eq_handler_t typedef in some PE versions */
#ifdef GASNETC_PORTALS_MISSING_EQ_HANDLER_T
  typedef void (*ptl_eq_handler_t)( ptl_event_t *event );
#endif

/* Max transfer size SHOULD be defined by portals, but apparently is not */
#ifdef PTL_MAX_TRANS_SZ
#define GASNETC_PTL_MAX_TRANS_SZ PTL_MAX_TRANS_SZ
#else
#define GASNETC_PTL_MAX_TRANS_SZ 2147483648UL
#endif

#ifndef GASNETC_MIN
#define GASNETC_MIN(x,y) ((x)<(y)?(x):(y))
#endif
#ifndef GASNETC_MAX
#define GASNETC_MAX(x,y) ((x)>(y)?(x):(y))
#endif

/* types of polling */
typedef enum{GASNETC_NO_POLL=0, GASNETC_SAFE_POLL, GASNETC_FULL_POLL} gasnetc_pollflag_t;
/* max number of events to process per polling call */
extern unsigned gasnetc_safe_poll_limit;
extern unsigned gasnetc_am_poll_limit;
extern unsigned gasnetc_sys_poll_limit;

/* check for signal and call gasnet_exit */
#if GASNETC_USE_SANDIA_ACCEL
#define GASNETC_CHECKSIG() do {						\
    int sig = gasneti_weakatomic_read(&gasnetc_got_signum,0);		\
    if (sig) {								\
      gasneti_weakatomic_set(&gasnetc_got_signum,0,0);			\
      fprintf(stderr,"*** Caught a signal number %i on node %i/%i\n",	\
	      sig,(int)gasneti_mynode,(int)gasneti_nodes);		\
      gasnet_exit(1);							\
    }									\
  } while(0);
#else
#define GASNETC_CHECKSIG() ((void)0)
#endif

/* Macro that checks return code from a Portals calls */
#define GASNETC_PTLCHECK(_retcode, _message) do {			\
    if_pf (_retcode != (int)PTL_OK) {					\
      gasneti_fatalerror("\nGASNet Portals encountered an error: %s (%i)\n" \
			 "  %s\n  at %s",				\
			 ptl_err_str[_retcode], _retcode, _message, gasneti_current_loc); \
    }									\
 } while (0)

/* Macro that makes a Portals calls and checks return code */
#define GASNETC_PTLSAFE(fncall) do {					\
    int _retcode;							\
    GASNETC_CHECKSIG();							\
    _retcode = (fncall);						\
    GASNETC_PTLCHECK(_retcode, "while calling: " #fncall);		\
 } while (0)

/* Portals Access table not implemented on XT3 */
#define GASNETC_PTL_AC_ID  0

/* We need Cray to reserve two table entries for UPC/GASNET
 * We believe these two have been reserved for use by UPC but not positive.
 */
#define GASNETC_PTL_RAR_PTE 38
#define GASNETC_PTL_AM_PTE 39

/* Values that are encoded in the MBITs of Portals Data Transfer ops */
#define GASNETC_PTL_IGNORE_BITS  0xFFFFFFFFFFFFFFF0ULL
#define GASNETC_PTL_RAR_BITS     0x00
#define GASNETC_PTL_RARAM_BITS   0x01
#define GASNETC_PTL_RARSRC_BITS  0x02
#define GASNETC_PTL_REQRB_BITS   0x03
#define GASNETC_PTL_CB_BITS      0x03
#define GASNETC_PTL_REQSB_BITS   0x04
#define GASNETC_PTL_SYS_BITS     0x05
#define GASNETC_PTL_BOOT_BITS    0x06

/* Operation type */
#define GASNETC_PTL_MSG_PUT      0x10
#define GASNETC_PTL_MSG_GET      0x20
#define GASNETC_PTL_MSG_AM       0x40
#define GASNETC_PTL_MSG_AMDATA   0x80
#define GASNETC_PTL_MSG_DOLC     0x80
/* DOLC - Do Local Completion flag for Extended API Puts/Gets.
 * Can occupy same bit as AMDATA flag since is only used when PUT or GET
 * Indicates to event handler that it must decrement the local completion counter
 * for this gasnet operation
 */

/* Additional flags used for Active Messages */
#define GASNETC_PTL_AM_SHORT     0x01
#define GASNETC_PTL_AM_MEDIUM    0x02
#define GASNETC_PTL_AM_LONG      0x04
#define GASNETC_PTL_AM_REQUEST   0x08
#define GASNETC_PTL_AM_PACKED    0x10
#define GASNETC_PTL_AM_SYNC      0x20

/* Token flag values */
#define GASNETC_PTL_REPLY_SENT   0x01

/* Masks used in constructing 64-bit bit fields */
#define GASNETC_SELECT_UPPER32     0xFFFFFFFF00000000ULL
#define GASNETC_SELECT_LOWER32     0x00000000FFFFFFFFULL
#define GASNETC_SELECT_OPBITS      0x00000000FFFFFF00ULL
#define GASNETC_SELECT_BYTE0       0x00000000000000FFULL
#define GASNETC_SELECT_BYTE1       0x000000000000FF00ULL
#define GASNETC_SELECT_BYTE2       0x0000000000FF0000ULL
#define GASNETC_SELECT_BYTE3       0x00000000FF000000ULL
#define GASNETC_SELECT_BYTE4       0x000000FF00000000ULL
#define GASNETC_SELECT_BYTE5       0x0000FF0000000000ULL
#define GASNETC_SELECT_BYTE6       0x00FF000000000000ULL
#define GASNETC_SELECT_BYTE7       0xFF00000000000000ULL

#define GASNETC_PACK_UPPER(lhs,rhs) lhs = ((lhs) & GASNETC_SELECT_LOWER32) | ((uint64_t)(rhs) << 32)
#define GASNETC_PACK_LOWER(lhs,rhs) lhs = ((lhs) & GASNETC_SELECT_UPPER32) | ((uint64_t)(rhs) & GASNETC_SELECT_LOWER32)
#define GASNETC_PACK_2INT(x,up,low) x = GASNETI_MAKEWORD(up,low)

#define GASNETC_UNPACK_2INT(x,up,low) do { \
    (up) = GASNETI_HIWORD(x);	   \
    (low) = GASNETI_LOWORD(x);	   \
  } while (0)

#define gasnetc_alloc_ticket(semptr) gasneti_semaphore_trydown(semptr)
#define gasnetc_return_ticket(semptr) gasneti_semaphore_up(semptr)
#define gasnetc_return_tickets(semptr,n) gasneti_semaphore_up_n(semptr,n)
#define gasnetc_num_tickets(semptr) gasneti_semaphore_read(semptr)

/* poll until thread has cached requested number of send tickets
 * AMPoll will spend these so we safe poll here
 */
#define GASNETC_GET_SEND_TICKETS(th,nsend,pollcnt) do {			\
    if (gasnetc_msg_limit == 0) {					\
      th->snd_tickets = nsend; /* no limit */				\
    } else {								\
      while(th->snd_tickets < nsend) {					\
	if (gasnetc_alloc_ticket(&gasnetc_send_tickets)) {		\
	  th->snd_tickets++;						\
	} else {							\
	  pollcnt++;							\
	  GASNETI_TRACE_EVENT(C, MSG_THROTTLE);				\
	  /* must safe poll.  AMPoll could eat our send+tmpmd tickets */ \
	  gasnetc_portals_poll(GASNETC_SAFE_POLL);			\
	}								\
      }									\
    }									\
  } while(0)

/* poll until thread has cached requested number of tmpmd tickets
 * NOTE: call to AMPoll may use one of these.  Could safe poll
 *       but this should eventually work.
 */
#define GASNETC_GET_TMPMD_TICKETS(th,ntmpmd,pollcnt) do {		\
    while(th->tmpmd_tickets < ntmpmd) {					\
      if (gasnetc_alloc_ticket(&gasnetc_tmpmd_tickets)) {		\
	th->tmpmd_tickets++;						\
      } else {								\
	pollcnt++;							\
	GASNETI_TRACE_EVENT(C, TMPMD_THROTTLE);				\
	gasneti_AMPoll();						\
      }									\
    }									\
  } while(0)

#define GASNETC_SET_CREDIT_BYTE(cred_byte,end_epoch,nextra,ncredit) do { \
    cred_byte = (0x0F & ncredit) | ((0x07 & nextra) << 4) | (end_epoch << 7); \
  } while(0)

#define GASNETC_READ_CREDIT_BYTE(cred_byte,end_epoch,nextra,ncredit) do { \
    ncredit = cred_byte & 0x0F;						\
    nextra = (cred_byte >> 4) & 0x07;					\
    end_epoch = (cred_byte >> 7) & 0x01;				\
  } while(0)

/* debugging functions and macros */
#if GASNET_DEBUG
#define GASNETC_MSGLEN_PAD(n)   do { (n) = GASNETI_ALIGNUP((n),GASNETI_MEDBUF_ALIGNMENT); } while(0)
#define GASNETC_MSGLEN_ADD(n,l) do {	(n) += (l); } while(0)
#define GASNETC_INIT_HARGS do {			\
    int i;					\
    for (i = 0; i < 16; i++) hargs[i]=0;	\
  } while(0)
#define GASNETC_DEF_HARGS() gasnet_handlerarg_t hargs[16]; int hargcnt=0; uint32_t db_seqno=0; int db_cntr=0; GASNETC_INIT_HARGS
#define GASNETC_ADD_HARG(foo) hargs[hargcnt++] = foo
#define GASNETC_GET_SEQNO(ptoken) db_seqno = (ptoken)->seqno
#define GASNETC_EXTRACT_SEQNO(data32,tok) do {\
    tok.seqno = *(data32++);                  \
    tok.msg_bytes += sizeof(uint32_t);        \
  } while(0)
#define GASNETC_INJECT_NEXT_SEQNO(data32) do {       \
    *(data32++) = db_seqno = gasnetc_amseqno++; \
  } while(0)
#define GASNETC_SEQNO_MSGLEN(cntr) cntr += sizeof(uint32_t)
#define GASNETC_AMLONG_DEFSEQARG ,uint32_t db_seqno
#define GASNETC_AMLONG_SEQARG ,db_seqno

#if 1
#define GASNETC_CHECKSUM(addr,len) gasneti_checksum(addr,len)
#else
#define GASNETC_CHECKSUM(addr,len) 1
#endif
#define GASNETC_DBGMSG(snd,req,str,src,dest,handler,narg,a,mlen,cred,dlen,data,th) do { \
    const char *fmt_str = "AMD %s %s %s s=%d d=%d sq=%d sr=%d h=%d mlen=%d dlen=%d crc=%lu cred=%d:%d:%d th=%d narg=%d %x %x %x %x %x %x %x %x"; \
    uint8_t end_epoch,nextra,ncredit;					\
    uint32_t sr_seqno = (snd ? (gasnetc_snd_seqno++) : (gasnetc_rcv_seqno++)); \
    uint64_t crc = GASNETC_CHECKSUM(data,dlen);				\
    GASNETC_READ_CREDIT_BYTE(cred,end_epoch,nextra,ncredit);		\
    GASNETI_TRACE_PRINTF(C,(fmt_str,(snd?"S":"R"),str,(req?"Req":"Rpl"),src,dest,db_seqno,sr_seqno,handler,mlen,dlen,crc,end_epoch,nextra,ncredit,th->threadidx,narg,a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7])); \
  } while(0)
  #define GASNETC_ZERO_AMARGS(_args) memset((_args), 0, sizeof(_args))
    
#else
#define GASNETC_MSGLEN_PAD(n)   do {} while(0)
#define GASNETC_MSGLEN_ADD(n,l) do {} while(0)
#define GASNETC_DEF_HARGS() do {} while(0)
#define GASNETC_ADD_HARG(foo) do {} while(0)
#define GASNETC_GET_SEQNO(ptok) do {} while(0)
#define GASNETC_EXTRACT_SEQNO(data,tok) do {} while(0)
#define GASNETC_INJECT_NEXT_SEQNO(data) do {} while(0)
#define GASNETC_SEQNO_MSGLEN(cntr) do {} while(0)
#define GASNETC_AMLONG_DEFSEQARG
#define GASNETC_AMLONG_SEQARG
#define GASNETC_DBGMSG(snd,req,str,src,dest,handler,narg,a,mlen,cred,dlen,data,th) do{}while(0)
#define GASNETC_ZERO_AMARGS(args) do {} while(0)
#endif

#if GASNETC_CREDIT_TESTING
#define UPDATE_STALL_TOT(state) do {state->SendStalls_tot++;} while(0)
#define UPDATE_SEND_MAX_TOT(state) do {state->SendMax_tot = GASNETC_MAX(state->SendMax_tot,state->SendMax);} while(0)
#else
#define UPDATE_STALL_TOT(state) do {} while(0)
#define UPDATE_SEND_MAX_TOT(state) do {} while(0)
#endif

/* poll until thread has cached requested number of flow control credits
 * NOTE: polling will never spend these.
 */
#define GASNETC_GET_SEND_CREDITS(th,state,ncredit,cred_byte,pollcnt) do { \
    uint8_t need = 0;							\
    int navail;								\
    uint8_t end_epoch = 0;						\
    gasneti_assert(th->snd_credits == 0);				\
    gasneti_assert(pollcnt == 0);					\
    cred_byte = 0;							\
    if (gasnetc_use_flow_control) {					\
      GASNETC_TRACE_WAIT_BEGIN();					\
      do {								\
	GASNETC_LOCK_STATE(state);					\
	navail = state->SendCredits - state->SendInuse;			\
	if (ncredit <= navail) {					\
	  state->SendInuse += ncredit;					\
	  if (state->SendInuse > state->SendMax) state->SendMax = state->SendInuse; \
	  UPDATE_SEND_MAX_TOT(state);					\
	  if (gasnetc_use_dynamic_credits) {				\
	    if (state->flags & GASNETC_REACHED_EPOCH) {			\
	      state->flags &= ~GASNETC_REACHED_EPOCH;			\
	      end_epoch = 1;						\
	    }								\
	  }								\
	  GASNETC_UNLOCK_STATE(state);					\
	  th->snd_credits += ncredit;					\
	  if (!gasnetc_use_dynamic_credits) need = 0;			\
	  need = GASNETC_MIN(need,GASNETC_MAX_CREDIT_REQUEST);		\
	  GASNETC_SET_CREDIT_BYTE(cred_byte,end_epoch,need,ncredit);	\
	} else {							\
	  if (! need) {	/* only set these the first time */		\
	    need = (uint8_t)(ncredit - navail);				\
	    state->SendStalls++;					\
	    UPDATE_STALL_TOT(state);					\
	  }								\
	  GASNETC_UNLOCK_STATE(state);					\
	  GASNETI_TRACE_EVENT(C, CREDIT_THROTTLE);			\
	  pollcnt++;							\
	  gasneti_AMPoll();						\
	}								\
      } while(th->snd_credits < ncredit);				\
      if (pollcnt) GASNETC_TRACE_WAIT_END(CREDIT_STALL);		\
    }									\
  } while(0)

/* Before starting an AM Request, poll until certain conditions are met */
#define GASNETC_COMMON_AMREQ_START(state,offset,th,nsend,ncredit,cred_byte) do { \
    int pollcnt = 0;							\
    GASNETC_GET_SEND_CREDITS(th,state,ncredit,cred_byte,pollcnt);	\
    /* Allocate a send buffer */					\
    while (!gasnetc_chunk_alloc(&gasnetc_ReqSB, GASNETC_CHUNKSIZE, &(offset)) ) { \
      pollcnt++;							\
      gasneti_AMPoll();							\
    }									\
    GASNETC_GET_SEND_TICKETS(th,nsend,pollcnt);				\
    gasneti_assert( th->snd_tickets >= nsend );				\
    gasneti_assert( th->snd_credits >= ncredit );			\
  } while (0)

#define GASNETC_PACK_AM_MBITS(mbits, offset, numarg, hndlr, amflag, targ_mbits) \
    (mbits) = ((uint64_t)(offset) << 32) | ( ((uint64_t)(numarg) & GASNETC_SELECT_BYTE0) << 24) \
      | ((uint64_t)(hndlr) << 16) | ((uint64_t)(amflag) << 8) | (uint64_t)(targ_mbits)

#define GASNETC_UNPACK_AM_MBITS(mbits, offset, numarg, hndlr, amflag, targ_mbits) do { \
    offset     = GASNETI_HIWORD(mbits);					\
    numarg     = ((mbits)&GASNETC_SELECT_BYTE3)>>24;			\
    hndlr      = ((mbits)&GASNETC_SELECT_BYTE2)>>16;			\
    amflag     = ((mbits)&GASNETC_SELECT_BYTE1)>>8;			\
    targ_mbits =  (mbits)&GASNETC_SELECT_BYTE0;				\
  } while(0)

#define GASNETC_GET_AM_LOWBITS(mbits,numarg,ghndlr,amflag) do { \
    numarg = ((mbits)&GASNETC_SELECT_BYTE3) >> 24;		\
    ghndlr = ((mbits)&GASNETC_SELECT_BYTE2) >> 16;		\
    amflag = ((mbits)&GASNETC_SELECT_BYTE1) >>  8;		\
  } while(0)

#define GASNETC_GET_MSG_TYPE(mbits) ((mbits) & 0xF0)
#define GASNETC_SET_MSG_TYPE(mbits,mtyp) (((mbits) & 0xFFFFFFFFFFFFFF0F) | ((mtyp) & 0xF0))

#define GASNETC_PTL_OFFSET(n,s) ((uint8_t*)(s) - (uint8_t*)gasneti_seginfo[n].addr)

/* AM tokens data structure.  This is never sent over the wire and generally exists
 * only as a stack variable when processing an AM.  The exception is in the case
 * of a non-packed AMLong, where a token record is contained in the amlongcache
 * object, retaining its state until both header and data payload arrives.
 */
typedef struct token_rec {
  uint8_t           need_reply;
  uint8_t           credits;             /* credit info: end_epoch flag, nextra, ncredit */
  uint32_t          initiator_offset;    /* offset in senders ReqSB, where Reply is sent */
  ptl_process_id_t  initiator;           /* process ID of requestor */
  gasnet_node_t     srcnode;             /* gasnet node ID of requestor */
  gasnet_handlerarg_t args[gasnet_AMMaxArgs()]; /* handler arguments [expands to constant] */
#if GASNET_DEBUG
  size_t            msg_bytes;           /* Entire length of header message (for debugging) */
  uint32_t          seqno;               /* Debug Seq number for AM Req/Reply */
#endif
} gasnetc_ptl_token_t;

/* Metadata cached by Long Put or AM Long Header
 * These data structures are allocated dynamically and updated atomically.
 * They will be created when the first of the AM Long Header or data packet arrives
 * and deallocated after the handler is run.
 */
typedef struct gasnetc_amlongcache_rec {
#if GASNET_DEBUG
  #define GASNETC_LID_DATA_HERE    0x1
  #define GASNETC_LID_HEADER_HERE  0x2
  uint8_t             flags;              /* indicates if HEADER and/or DATA has arrived */
#endif
  uint32_t            dest_lid;           /* ID of this Long AM */
  gasnetc_ptl_token_t tok;                /* token for this AM, only gets written by header */
  void*               data;               /* location of data packet payload */
  size_t              nbytes;             /* data payload size */
  struct gasnetc_amlongcache_rec *next;   /* link into conn_state list of active AMLong lid objs */
} gasnetc_amlongcache_t;

#if GASNETC_USE_SPINLOCK
typedef gasneti_atomic_t gasnetc_statelock_t;
#define GASNETC_INITLOCK_STATE(ptr) gasneti_spinlock_init(&ptr->lock)
#define GASNETC_TRYLOCK_STATE(ptr) gasneti_spinlock_trylock(&ptr->lock)
#define GASNETC_LOCK_STATE(ptr) gasneti_spinlock_lock(&ptr->lock)
#define GASNETC_UNLOCK_STATE(ptr) gasneti_spinlock_unlock(&ptr->lock)
#define GASNETC_INITLOCK_NODE(srcnode)			\
  gasneti_spinlock_init(&gasnetc_conn_state[srcnode].lock)
#define GASNETC_TRYLOCK_NODE(srcnode)				\
  gasneti_spinlock_trylock(&gasnetc_conn_state[srcnode].lock)
#define GASNETC_LOCK_NODE(srcnode)				\
  gasneti_spinlock_lock(&gasnetc_conn_state[srcnode].lock)
#define GASNETC_UNLOCK_NODE(srcnode)			\
  gasneti_spinlock_unlock(&gasnetc_conn_state[srcnode].lock)
#else
typedef gasneti_mutex_t gasnetc_statelock_t;
#define GASNETC_INITLOCK_STATE(ptr) gasneti_mutex_init(&ptr->lock)
#define GASNETC_TRYLOCK_STATE(ptr) gasneti_mutex_trylock(&ptr->lock)
#define GASNETC_LOCK_STATE(ptr) gasneti_mutex_lock(&ptr->lock)
#define GASNETC_UNLOCK_STATE(ptr) gasneti_mutex_unlock(&ptr->lock)
#define GASNETC_INITLOCK_NODE(srcnode)			\
  gasneti_mutex_init(&gasnetc_conn_state[srcnode].lock)
#define GASNETC_TRYLOCK_NODE(srcnode)				\
  gasneti_mutex_trylock(&gasnetc_conn_state[srcnode].lock)
#define GASNETC_LOCK_NODE(srcnode)				\
  gasneti_mutex_lock(&gasnetc_conn_state[srcnode].lock)
#define GASNETC_UNLOCK_NODE(srcnode)			\
  gasneti_mutex_unlock(&gasnetc_conn_state[srcnode].lock)
#endif

/* GASNet connection state.  This is where we put any per-node state info.
 * It includes the src_lid, whose value is incremented each time we send a (non-packed)
 * AMLong Request or Reply to the corresponding target.
 * It also includes list where we attach amlong cache objects for AM Long Requests
 * and replies from this corresponding node.
 * But mostly, it contains flow control and dynamic credit re-distribution variables.
 * We distribute our LOAN credits to remote nodes so they can send AMs to us.
 * We are granted SEND credits from other nodes to send AMs to them.
 * Only AM Requests require credits, AM Replies do not.
 * Notes on variaous fields:
 * SendInuse:  records the number of SEND credits currently in use. Max value = 2^16
 * SendMax:    records the max number of SEND credits in use during this epoch.  This value
 *              will decay at the start of each epoch so that it will go to zero after
 *              several epochs if no AMs are sent.
 * SendRevoked: The number of SendCredits I gave up this epoch.  This value will be limited
 *              each epoch so that SEND credits are not drained rapidly.  It will decay at the
 *              start of each epoch and go to zero after several epochs if none revoked.
 * SendStalls: Count the number of times (this epoch) we stalled, polling the network, 
 *              until we had enough SendCredits available to issue the AMRequest.
 *              This value will be decayed at each epoch, going to zero after several
 *              epochs without stalls.
 * LoanRequested: Number of additional LOAN credits (which we manage) a remote node
 *              has requested this epoch.  This value also decays over time.
 * LoanGiven:  Number of (our) LOAN credits we gave to this remote node this epoch.  Decays.
 * The values of num_stalls, SendMax and SendRevoked will be used when 
 * asked by the remote node to give up some of our SendCredits.
 * RECV_requested will help determining how many, if any, RECV_credits to grant
 * the remote node.
 */

/* GASNETC_MAX_CREDIT_REQUEST = max incremental credit request, must fit in 3 bits */
#define GASNETC_MAX_CREDIT_REQUEST         0x7U
#define GASNETC_SYS_GOT_SHUTDOWN_MSG       0x01U
#define GASNETC_SYS_MSG_INFLIGHT           0x02U
#define GASNETC_CREDIT_REVOKE_ZERO_REPLY   0x04U
#define GASNETC_REACHED_EPOCH              0x08U
/* assumes lock is held */
#define GASNETC_DECAY_SENDVARS(state) do {	\
    GASNETC_CREDIT_DECAY(state->SendStalls);	\
    GASNETC_CREDIT_DECAY(state->SendMax);	\
    GASNETC_CREDIT_DECAY(state->SendRevoked);	\
  } while(0)

/* compact representation of a doubly linked list by using node ids rather than pointers */
#define GASNETC_DLL_NULL GASNET_MAXNODES
typedef struct _gasnetc_dll_link {
  gasnet_node_t  prev;
  gasnet_node_t  next;
} gasnetc_dll_link_t;

typedef struct gconrec {
  gasnetc_statelock_t    lock;              /* spinlock/mutex for atomic update of this record */
  uint16_t               LoanCredits;       /* number of my credits allocated to this remote node */
  uint16_t               SendCredits;       /* number of remote node credits allocated to me */
  uint16_t               SendStalls;        /* number of times we stall on credits trying to send AMs */
  uint16_t               SendInuse;         /* current number of SEND credits in flight to them */
  uint16_t               SendMax;           /* max number of SEND credits in flight this epoch */
  uint16_t               SendRevoked;       /* number of SEND credits I give back this epoch */
  uint16_t               LoanRequested;     /* number of times additional credits are requested */
  uint16_t               LoanGiven;         /* number of additional LOAN credits given to them */
#if GASNETC_CREDIT_TESTING
  uint32_t               SendStalls_tot;    /* total send stalls, not decayed */
  uint32_t               SendMax_tot;       /* max credits in use at a time, not decayed */
  uint32_t               LoanGiven_tot;     /* total extra credits given, not decayed */
  uint32_t               LoanRequested_tot; /* total extra credits requested, not decayed */
  uint32_t               LoanRevoked_tot;   /* total credits returned from this node, not decayed */
  uint32_t               LoanReqRevoke_tot; /* total number of times we requested cred revoke */
#endif
  gasnetc_dll_link_t     link;              /* double link list of nodes to scavenge
					     * this field controlled by gasnetc_scavenge_lock */
  uint8_t                flags;             /* maintain binary state values */
  gasneti_weakatomic_t   src_lid;           /* must be 32 bit unsigned so will roll after 2^32 */
  gasnetc_amlongcache_t *lids;              /* list of lid cache objects of AM Longs from this node */
} gasnetc_conn_t;

/* array of connection states */
extern gasnetc_conn_t *gasnetc_conn_state;

/* Per-node credit vars are stored in 16 bit ints */
#define GASNETC_MAX_CREDITS       ((int)(uint16_t)(-1))
extern int gasnetc_use_flow_control;                /* flow control switch, must be same on all nodes */
extern int gasnetc_use_dynamic_credits;
extern long gasnetc_total_credits;                  /* total number of AM credits this nodes has to distribute */
extern gasneti_semaphore_t gasnetc_banked_credits;  /* number of banked credits available for distribution */
extern int gasnetc_revoke_limit;                    /* max number of credits revoked in an epoch */
extern int gasnetc_lender_limit;                    /* max number of credits loaned in an epoch */
extern int gasnetc_max_cpn;                         /* max number of credits allocated to a node */
extern gasneti_mutex_t gasnetc_epoch_lock;          /* lock to control epoch update */
extern gasneti_weakatomic_t gasnetc_AMRequest_count;/* counter of number of AMRequests to age epoch */
extern int gasnetc_epoch_duration;                  /* number of AMReq before epoch ends */
extern int gasnetc_num_scavenge;                    /* number of nodes to hit-up for credits */
extern gasneti_weakatomic_t gasnetc_scavenge_inflight;  /* number of outstanding scavange requests */
extern gasnet_node_t gasnetc_scavenge_list;       /* list of nodes to scavenge, those that have
						   * more than min number of credits allocated */
extern gasneti_mutex_t   gasnetc_scavenge_lock;   /* lock to control the scavenge list */
extern int gasnetc_debug_node;                    /* used in debugging */
#define GASNETC_CURRENT_TIME() gasneti_ticks_to_ns(gasneti_ticks_now())
/* decay variables by dividing by 4 */
#define GASNETC_CREDIT_DECAY(val) val = ((val) >> 2)
extern uintptr_t gasnetc_segbase, gasnetc_segend; /* local segment bounds */

#if GASNETC_USE_SANDIA_ACCEL
extern int gasnetc_use_accel;
#endif

/* Types of GASNET Portals Memory Descriptors */
enum { GASNETC_RAR_MD,
       GASNETC_RARAM_MD,
       GASNETC_RARSRC_MD,
       GASNETC_REQSB_MD,
       GASNETC_REQRB_MD,
       GASNETC_RPLSB_MD,
       GASNETC_CB_MD,
       GASNETC_TMP_MD,
       GASNETC_SYS_SEND_MD,
       GASNETC_SYS_RECV_MD,
       GASNETC_NUM_MD
};

/* an array of MD names for diagnostics */
extern const char* gasnetc_md_name[];

/* pointer to event handler functions */
typedef void (*gasnetc_ptl_event_handler)(ptl_event_t *ev);

/* Number of temporary Portals MDs in use at any time */
#define GASNETC_MAX_TMP_MDS 1024
extern int gasnetc_max_tmpmd;
extern gasneti_semaphore_t gasnetc_tmpmd_tickets;
#if GASNETI_STATS_OR_TRACE
int gasnetc_tmpmd_hwm;
#endif

extern int gasnetc_dump_stats;
extern int gasnetc_io_buffer_size;
extern void* gasnetc_flush_buffer;

/* An array of Portals Proc IDs used to determine network address of nodes
 * Also used as elements in a hash table, for reverse lookup of ptl_process_id_t
 * structures to gasnet_node_t.
 */
typedef struct gasnetc_procrec {
  ptl_process_id_t  ptl_id;
  struct gasnetc_procrec *next;  /* linked list for hash table reverse lookup */
} gasnetc_procid_t;

extern ptl_process_id_t    gasnetc_myid;
extern ptl_uid_t           gasnetc_uid;
extern gasnetc_procid_t   *gasnetc_procid_map;

/* An array of strings that name the Portals events
 * MLW: Not defined in API but exists in Portals implementation
 */
extern char* ptl_event_str[];


#ifndef GASNET_PAR
  /* No accounting required */
  #define GASNETC_REQRB_START(bufptr)      do {} while(0)
  #define GASNETC_REQRB_FINISH(bufptr)     do {} while(0)
  #define GASNETC_REQRB_BUSY(bufptr)       0
#else
  /* "threads_active" counts threads referencing the buffer */
  #define GASNETC_REQRB_START(bufptr) \
      gasneti_weakatomic_increment(&(bufptr)->threads_active, 0)
  #define GASNETC_REQRB_FINISH(bufptr) \
      gasneti_weakatomic_decrement(&(bufptr)->threads_active, 0)
  #define GASNETC_REQRB_BUSY(bufptr) \
      (gasneti_weakatomic_read(&(bufptr)->threads_active, 0) != 0)
#endif

/* ----------------------------------------------------------------------------------- */

/* The RAR, RARAM, and the AM request/reply send/receive buffers are described by */
typedef struct {
  size_t nbytes;                       /* number of bytes in buffer after alignment */
  void*  actual_start;                 /* returned by allocator */
  void*  start;                        /* aligned start */
  ptl_handle_md_t  md_h;               /* The Portals memory descriptor handler */
  ptl_handle_me_t  me_h;               /* The Portals match-list entry handle (if used) */
  char *name;                          /* string used for diagnostics */
  int use_chunks;                      /* Is the buffer under control of a chunk allocator? */

  /* Used only in ReqRB: */
  gasneti_weakatomic_t threads_active; /* counts number of threads actively using buffer */
  volatile int fresh;                  /* non-zero if no AMs have been processed since MDAttach */

  /* The following fields are only used in the case of a chunk allocator */
  /* NOTE: Only ReqSB and RplSB objects are controlled by chunk allocation, others are not. */
#if GASNETI_STATS_OR_TRACE
  int numchunks;                       /* number of chunks in buffer */
  int inuse;                           /* number of chunks currently in use */
  int hwm;                             /* High water mark of chunk use */
#endif
  gasneti_lifo_head_t freelist;        /* chunk freelist */

  /* locks access to "fresh" (if !use_chunks), or "inuse" and "hwm" (if use_chunks) */
  gasneti_mutex_t      lock;
} gasnetc_PtlBuffer_t;

/* Thread local data
 * This is attached to the gasnetc_threaddata hook in gasnete_threaddata_t
 */
typedef struct _gasnetc_threaddata_t {
  gasnete_threadidx_t threadidx;     /* must be identical to corresponding gasnete value */

  /* holding place for tickets and credits that we have already allocated */
  uint8_t snd_tickets;
  uint8_t tmpmd_tickets;
  uint8_t snd_credits;

  /* this is set when sending a non-async amlong request.  Issuing thread will
   * poll on this variable until cleared.  Thread processing the SEND_END event
   * will decrement the count.  Issuing thread ID must be sent in match_bits.
   * Each thread allowed to issue one non-async amlong at a time.  */
  gasneti_weakatomic_t amlongReq_data_inflight;

  /* like amlongReq_data_inflight, but for replies (which are always non-async).
   * A separate counter is required because both may be in flight simultaneously */
  gasneti_weakatomic_t amlongRep_data_inflight;

  /* rplsb is cached request send buffer, or NULL */
  void *rplsb;
} gasnetc_threaddata_t;

/* ----------------------------------------------------------------------------
 * Encapsulate event queues into structures so that we can associate a lock
 * with each.  Needed because only one thread can poll/wait/get on an
 * event queue at one time.
 */
typedef struct _gasnetc_eq {
  long num_events;               /* number of events allocated in queue */
  ptl_handle_eq_t eq_h;          /* Handle to portals event queue */
  gasneti_mutex_t lock;          /* enforce sequential access to queue */
  char *name;                    /* event queue name */
} gasnetc_eq_t;

/* configurable sizes for Portals buffers */
extern int gasnetc_ReqRB_pool_size;
extern size_t gasnetc_ReqRB_numchunk;         /* Number of chunks in each ReqRB */
extern size_t gasnetc_ReqSB_numchunk;         /* Number of chunks to alloc for ReqSB */
extern size_t gasnetc_RplSB_numchunk;         /* Number of chunks to alloc for RplSB */

extern gasnetc_PtlBuffer_t gasnetc_ReqSB;
extern gasnetc_PtlBuffer_t gasnetc_RplSB;    /* MLW: Can elim this, and alloc a per-thread buffer and MD
					      * No need for an EQ since will only use it to send */
extern gasnetc_PtlBuffer_t **gasnetc_ReqRB;  /* an array of buffers */
extern gasnetc_PtlBuffer_t gasnetc_RAR;
extern gasnetc_PtlBuffer_t gasnetc_RARAM;
extern gasnetc_PtlBuffer_t gasnetc_RARSRC;
extern gasnetc_PtlBuffer_t gasnetc_CB;

/* handles to Portals network interface, memory descriptors and event queues */
extern ptl_handle_ni_t gasnetc_ni_h;              /* the network interface handle */
extern gasnetc_eq_t   *gasnetc_AM_EQ;             /* The AM Event Queue */
extern gasnetc_eq_t   *gasnetc_SAFE_EQ;           /* The SAFE Event Queue */

/* out of band MDs for sending system messages */
extern gasnetc_PtlBuffer_t gasnetc_SYS_Send;       /* out-of-band message send buffer */
extern gasnetc_PtlBuffer_t gasnetc_SYS_Recv;       /* out-of-band message recv buffer */
extern gasnetc_eq_t *gasnetc_SYS_EQ;               /* out-of-band system Event Queue */
extern double gasnetc_shutdown_seconds;            /* number of seconds to poll before forceful shutdown */
extern int gasnetc_shutdownInProgress;             /* set upon entry to gasnetc_exit */
typedef enum{GASNETC_SYS_SHUTDOWN_REQUEST=0,
	     GASNETC_SYS_BARRIER,
	     GASNETC_SYS_CREDIT_REVOKE,
	     GASNETC_SYS_CREDIT_RETURN,
	     GASNETC_SYS_NUM} gasnetc_sys_t;
extern int gasnetc_resource_init_complete;         /* After this is set, should be ok to tolerate
						    * dropped events on SYS queue */

#if GASNETC_USE_SANDIA_ACCEL
/* did we get a signal, and if so, what signal number */
extern gasneti_weakatomic_t gasnetc_got_signum;
#endif

/* flag set at init time on whether we allow packed am_long messages or not */
extern int gasnetc_allow_packed_long;


/* Vars that limit total number of Portals operations in flight at any time
 * originating from this node.
 * Performance decreases when too many messages are inflight so we try to
 * limit the number we initiate.
 * The send_tickets counter represents the number of tickets available.
 * It is initialized to be the msg_limit and decremented each time a send_ticket
 * is allocated.  It is incremented each time an operation completes.
 * NOTE: gasnetc_msg_limit == 0 means there is no limit
 */
extern gasneti_semaphore_t gasnetc_send_tickets;
extern int gasnetc_msg_limit;

/* debugging aid */
extern uint32_t gasnetc_snd_seqno, gasnetc_rcv_seqno;
extern uint32_t gasnetc_amseqno;

/* Functions we export to the core and extended API */
extern void *gasnetc_chunk_alloc_no_off(gasnetc_PtlBuffer_t *buf, size_t nbytes);
extern void *gasnetc_chunk_alloc(gasnetc_PtlBuffer_t *buf, size_t nbytes, ptl_size_t *offset);
extern void *gasnetc_chunk_alloc_withpoll(gasnetc_PtlBuffer_t *buf, size_t nbytes, ptl_size_t *offset,
					int pollcnt, gasnetc_pollflag_t poll_type);
extern void gasnetc_chunk_free(gasnetc_PtlBuffer_t *buf, ptl_size_t offset);
extern ptl_handle_md_t gasnetc_alloc_tmpmd(void* dest, size_t nbytes);
extern void gasnetc_free_tmpmd(ptl_handle_md_t md_h);
extern void gasnetc_init_portals_network(int *argc, char ***argv);
extern uintptr_t gasnetc_portalsMaxPinMem(void);
extern void gasnetc_bootstrapBarrier(void);
extern void gasnetc_bootstrapBroadcast(void *src, size_t len, void *dest, int rootnode);
extern void gasnetc_bootstrapExchange(void *src, size_t len, void *dest);
extern void gasnetc_init_portals_resources(void);
extern void gasnetc_portals_exit(void);
extern void gasnetc_portals_poll(gasnetc_pollflag_t poll_type);
extern void gasnetc_event_handler(ptl_event_t *ev);
extern void gasnetc_ptl_trace_finish(void);
extern size_t gasnetc_getmsg(void *dest, gasnet_node_t node, void *src, size_t nbytes,
			   ptl_match_bits_t match_bits, gasnetc_pollflag_t pollflag);
extern size_t gasnetc_putmsg(void *dest, gasnet_node_t node, void *src, size_t nbytes,
			   ptl_match_bits_t match_bits, gasneti_weakatomic_t *lcc,
			   gasnetc_pollflag_t pollflag);
extern void gasnetc_sys_SendMsg(gasnet_node_t node, gasnetc_sys_t msg_id,
				int32_t arg0, int32_t arg1, int32_t arg2);
extern void gasnetc_sys_barrier(void);
extern int gasnetc_sys_exit(int *exitcode);
/* need a special signal handler for Portals */
extern void gasnetc_portalsSignalHandler(int sig);
extern int gasnetc_issue_credit_request(gasnet_node_t node, int ncredit);
extern int gasnetc_issue_credit_revoke(gasnet_node_t node);
extern uint8_t gasnetc_credit_update(int isReq, uint8_t cred_byte, gasnet_node_t remotenode, const char* name);
extern void gasnetc_end_epoch(int epoch_count);
extern void gasnetc_scavenge_for_credits(void);
#if GASNETC_CREDIT_TESTING
#define GASNETC_DUMP_CREDITS(epoch_count) gasnetc_dump_credits(epoch_count)
extern void gasnetc_dump_credits(int epoch_count);
#else
#define GASNETC_DUMP_CREDITS(epoch_count) do{}while(0)
#endif
extern void gasnetc_print_scavenge_list(void);
extern void gasnetc_scavenge_list_remove(gasnet_node_t node);
extern void gasnetc_scavenge_list_add(gasnet_node_t node, int locked);

/* Inline Function Definitions */

GASNETI_ALWAYS_INLINE(get_rplsb) 
void *claim_rplsb(gasnetc_threaddata_t *th)
{
  void *retval = th->rplsb;
  th->rplsb = NULL;
  gasneti_assert(retval != NULL);
  return retval;
}

GASNETI_INLINE(gasnetc_compute_credits)
unsigned long gasnetc_compute_credits(unsigned long nbytes)
{
  if (gasnetc_use_flow_control) {
    /* eq: if bpc=256 then 0-256 is 1 credit, 257-512 is 2, etc */
    return (nbytes == 0 ? 1 : ((nbytes + GASNETC_BYTES_PER_CREDIT - 1) / GASNETC_BYTES_PER_CREDIT));
  }
  return 0;
}

/* return current number of credits avail to send AMs target node */
GASNETI_INLINE(gasnetc_avail_credits)
int gasnetc_avail_credits(gasnet_node_t node)
{
  int avail;
  gasnetc_conn_t *state = &gasnetc_conn_state[node];
  GASNETC_LOCK_STATE(state);
  avail = state->SendCredits - state->SendInuse;
  gasneti_assert(avail >= 0);
  GASNETC_UNLOCK_STATE(state);
  return avail;
}

GASNETI_INLINE(gasnete_set_mbits_lowbits)
void gasnete_set_mbits_lowbits(ptl_match_bits_t *mbits, uint8_t msg_type, gasnete_op_t *op)
{
  /* The format is 0x00000000TTAAAAMM
   * Where TT   = threadid with eop/iop encoding in most significant bit
   *       AAAA = EOP/IOP opaddr bits
   *       MM   = message type and match bits
   */
    uint32_t th_b   = ( ((uint32_t)op->threadidx) << 24)    & 0xFF000000;
    uint32_t addr_b = ( ((uint32_t)op->addr.fulladdr) << 8) & 0x00FFFF00;
    uint32_t m_b    = ( (uint32_t)msg_type )                & 0x000000FF;
    *mbits = (GASNETC_SELECT_UPPER32 & *mbits) | (GASNETC_SELECT_LOWER32 & (ptl_match_bits_t)(th_b | addr_b | m_b));
    GASNETI_TRACE_PRINTF(C,("set lowbits th = 0x%x, addr = 0x%x, type = 0x%x, bits = 0x%llx",th_b,addr_b,m_b,*mbits));
}

GASNETI_INLINE(gasnete_mbits2op)
gasnete_op_t* gasnete_mbits2op(ptl_match_bits_t mbits)
{
  uint32_t lo = GASNETI_LOWORD(mbits);
  uint8_t tid = lo >> 24;
  gasnete_opaddr_t addr;
  addr.fulladdr = lo >> 8;
  return gasnete_opaddr_to_ptr(tid, addr);
}

GASNETI_INLINE(gasnete_mbits2td)
gasnete_threaddata_t* gasnete_mbits2td(ptl_match_bits_t mbits)
{
  uint32_t lo = GASNETI_LOWORD(mbits);
  uint8_t tid = lo >> 24;
  return gasnete_threadtable[GASNETE_THREADID(tid)];
}

GASNETI_INLINE(gasnetc_in_local_rar)
int gasnetc_in_local_rar(uint8_t* pstart, size_t n)
{
  uintptr_t addr = (uintptr_t)pstart;
  return ((addr >= gasnetc_segbase) && ((addr + n) <= gasnetc_segend));
}

GASNETI_INLINE(gasnetc_alloc_tmpmd_withpoll)
ptl_handle_md_t gasnetc_alloc_tmpmd_withpoll(void* start, size_t nbytes)
{
  while (! gasnetc_alloc_ticket(&gasnetc_tmpmd_tickets)) {
    gasnetc_portals_poll(GASNETC_SAFE_POLL);
  }
  return gasnetc_alloc_tmpmd(start, nbytes);
}

enum { /* lock_op argument to gasnetc_get_event and gasnetc_sys_poll */
  GASNETC_EQ_NOLOCK = 0, /* Caller must hold the lock */
  GASNETC_EQ_LOCK,       /* Obtain and release the lock */
  GASNETC_EQ_TRYLOCK     /* trylock and return 0 on failure to acquire */
};

GASNETI_INLINE(gasnetc_get_event)
int gasnetc_get_event(gasnetc_eq_t *eq, ptl_event_t *ev, int lock_op)
{
  int rc;
  int retcode = 0;

  switch (lock_op) {
  case GASNETC_EQ_NOLOCK:
    gasneti_mutex_assertlocked(&eq->lock);
    break;
  case GASNETC_EQ_LOCK:
    gasneti_mutex_lock(&eq->lock);
    break;
  case GASNETC_EQ_TRYLOCK:
    if (gasneti_mutex_trylock(&eq->lock)) return 0;
    break;
  }
  rc = PtlEQGet( eq->eq_h, ev);
  if (lock_op != GASNETC_EQ_NOLOCK) gasneti_mutex_unlock(&eq->lock);
  switch (rc) {
  case PTL_OK:
    retcode = 1;
    break;
  case PTL_EQ_EMPTY:
    break;
  default:
    gasneti_fatalerror("gasnetc_get_event Portals Error in PtlEQGet: %s (%i)\n on Queue %s at %s\n",
		       ptl_err_str[rc],rc,eq->name,gasneti_current_loc);
    break;
  }

  return retcode;
}

GASNETI_INLINE(gasnetc_mythread)
gasnetc_threaddata_t *gasnetc_mythread(void)
{
  gasnete_threaddata_t *th = gasnete_mythread();
  return th->gasnetc_threaddata;
}

static void gasnetc_free_threaddata(void *_td) {
  gasnetc_threaddata_t *td = (gasnetc_threaddata_t *)_td;
  gasneti_free(td);
}

GASNETI_INLINE(gasnetc_new_threaddata)
gasnetc_threaddata_t* gasnetc_new_threaddata(gasnete_threadidx_t idx)
{
  gasnetc_threaddata_t *th = (gasnetc_threaddata_t*)gasneti_malloc(sizeof(gasnetc_threaddata_t));
  gasneti_assert_always(th);
  th->threadidx = idx;       /* keep this consistent with gasnete_threaddata */
  th->snd_tickets = 0;
  th->tmpmd_tickets = 0;
  th->snd_credits = 0;
  th->rplsb = NULL;
  gasneti_weakatomic_set(&th->amlongReq_data_inflight, 0, 0);
  gasneti_weakatomic_set(&th->amlongRep_data_inflight, 0, 0);
  gasnete_register_threadcleanup(gasnetc_free_threaddata, th);
  return th;
}

GASNETI_INLINE(gasnetc_sys_poll)
void gasnetc_sys_poll(int lock_op)
{
  ptl_event_t ev;
  unsigned sys_cnt = 0;

  /* always check for receipt of signal before polling since we do not protect get_event
   * with _SAFE wrapper */
  GASNETC_CHECKSIG();

  /* limit number of sys events to process at a time ? */
  while ((gasnetc_sys_poll_limit == 0) || (sys_cnt < gasnetc_sys_poll_limit)) {
    /* attempt to get an event, ok if EQ overflowed (but not until after sys initialization) */
    int rc;
    switch (lock_op) {
    case GASNETC_EQ_NOLOCK:
      gasneti_mutex_assertlocked(&gasnetc_SYS_EQ->lock);
      break;
    case GASNETC_EQ_LOCK:
      gasneti_mutex_lock(&gasnetc_SYS_EQ->lock);
      break;
    case GASNETC_EQ_TRYLOCK:
      if (gasneti_mutex_trylock(&gasnetc_SYS_EQ->lock)) return;
      break;
    }
    rc = PtlEQGet(gasnetc_SYS_EQ->eq_h, &ev);
    if (lock_op != GASNETC_EQ_NOLOCK) gasneti_mutex_unlock(&gasnetc_SYS_EQ->lock);
    switch (rc) {
    case PTL_EQ_EMPTY:
      /* no work, return to caller */
      return;
      break;

    case PTL_EQ_DROPPED:
      GASNETI_TRACE_EVENT(C,SYSQ_DROPPED);
      if (!gasnetc_resource_init_complete) {
	/* could have been a bootstrap barrier message, this is fatal */
	gasneti_fatalerror("Dropped Event On SYS EQ prior to resource init");
      }
      /* fall through to process this event */

    case PTL_OK:
      GASNETI_TRACE_PRINTF(C,("Got event %s from SYS_EQ, md=%lu, mbits=0x%lx",ptl_event_str[ev.type],(ulong)ev.md_handle,(unsigned long)ev.match_bits));
      GASNETC_CALL_EQ_HANDLER(ev);
      sys_cnt++;
      break;

    default:
      gasneti_fatalerror("gasnetc_sys_poll Portals Error in PtlEQGet: %s (%i)\n at %s\n",
			 ptl_err_str[rc],rc,gasneti_current_loc);
      break;
    }
  }
}

/*
 * Encapsulate access to 
 *   th->amlong{Req,Rep}_data_inflight
 */
#if 0
  /* General case, allows multiple ops in-flight */
  #define GASNETC_INC_INFLIGHT(_p)	gasneti_weakatomic_increment((_p), 0)
  #define GASNETC_DEC_INFLIGHT(_p)	gasneti_weakatomic_decrement((_p), 0)
  #define GASNETC_TEST_INFLIGHT(_p)	gasneti_weakatomic_read((_p), 0)
#else
  /* Single-op case.  We allow only 1 in-flight op.
   * This allows use of set(p,1) and set(p,0), in place of inc(p) and dec(p).
   * We include assertions for a sanity check.
   */
  #define GASNETC_INC_INFLIGHT(_p) do {                              \
	    gasneti_weakatomic_t *_tmp = (_p);                       \
	    gasneti_assert(gasneti_weakatomic_read((_tmp), 0) == 0); \
	    gasneti_weakatomic_set((_tmp), 1, 0);                    \
	} while (0)
  #define GASNETC_DEC_INFLIGHT(_p) do {                              \
	    gasneti_weakatomic_t *_tmp = (_p);                       \
	    gasneti_assert(gasneti_weakatomic_read((_tmp), 0) == 1); \
	    gasneti_weakatomic_set((_tmp), 0, 0);                    \
	} while (0)
  #define GASNETC_TEST_INFLIGHT(_p)	gasneti_weakatomic_read((_p), 0)
#endif

/* ---------------------------------------------------------------------------------
 * Allocate a new 24-bit LID = "Long ID" for a new AMLong Reply operation
 * Bit 23 is set to prevent accidental collision w/ offset-based lids of Requests
 * --------------------------------------------------------------------------------- */
GASNETI_INLINE(gasnetc_new_lid)
uint32_t gasnetc_new_lid(gasnet_node_t dest)
{
  /* use _add rather than _incr since it returns the new value */
  uint32_t cntr = gasneti_weakatomic_add(&gasnetc_conn_state[dest].src_lid,1,0);
  return ((1<<23) | (cntr & 0x7FFFFF));
}

/* ----------------------------------------------------------------------------
 * Firehose bits
 */

#if PLATFORM_OS_CATAMOUNT
 #define GASNETC_IF_USE_FIREHOSE(X) if (0) { /*empty*/ }
 #define gasnetc_use_firehose       0
#else

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
  #define GASNETC_FIREHOSE_LOCAL  1
  #define GASNETC_FIREHOSE_REMOTE 0
  #define GASNETC_FH_PER_OP	  1
#else
  #define GASNETC_FIREHOSE_LOCAL  0
  #define GASNETC_FIREHOSE_REMOTE 1
  #define GASNETC_FH_PER_OP	  2
#endif

#define GASNETC_IF_USE_FIREHOSE(X) if_pt (gasnetc_use_firehose) {X}

#include <firehose.h>
extern int gasnetc_use_firehose;
extern firehose_info_t gasnetc_firehose_info;

/* A "handle" on a firehose request.
 * Used for compact encoding in the upper match bits
 */
typedef struct _gasnetc_fh_op_t {
  firehose_request_t		fh[GASNETC_FH_PER_OP]; /* shared w/ freelist's next ptr */
#if (GASNETC_FH_PER_OP != 1)
  int				count;  /* Since might need just local or remote, need a count */
#endif
  gasnete_opaddr_t		addr;
} gasnetc_fh_op_t;

extern gasnetc_fh_op_t *gasnetc_fh_new(void);
extern void gasnetc_fh_free(uint16_t fulladdr);

/* This limits the amount we ask for in a firehose_{local,remote}_pin() call,
 * to ensure that after rounding up to page boundaries, we don't exceed the max.
 */
GASNETI_INLINE(gasnetc_fh_aligned_len)
size_t gasnetc_fh_aligned_len(const void* start, size_t len) {
  size_t limit = gasnetc_firehose_info.max_LocalPinSize - ((uintptr_t)start & (GASNET_PAGESIZE - 1));
  return MIN(len, limit);
}

GASNETI_INLINE(gasnetc_fh_aligned_local_pin)
gasnetc_fh_op_t *gasnetc_fh_aligned_local_pin(const void* start, size_t len) {
  gasnetc_fh_op_t *op = gasnetc_fh_new();
  size_t ask_bytes = gasnetc_fh_aligned_len(start, len);
  (void) firehose_local_pin((uintptr_t)start, ask_bytes, &op->fh[0]);
#if (GASNETC_FH_PER_OP != 1)
  op->count++;
#endif
  return op;
}
#endif /* !PLATFORM_OS_CATAMOUNT */

#endif
