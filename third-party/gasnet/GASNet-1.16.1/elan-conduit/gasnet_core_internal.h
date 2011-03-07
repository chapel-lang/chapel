/*   $Source: /var/local/cvs/gasnet/elan-conduit/gasnet_core_internal.h,v $
 *     $Date: 2009/09/18 23:33:28 $
 * $Revision: 1.46 $
 * Description: GASNet elan conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>
#include <gasnet_handler.h>

#if PLATFORM_COMPILER_PGI || PLATFORM_COMPILER_SUN
  /* this workaround enables use of these compilers with the libelan headers */
  #ifndef __GNUC__
    #define __GNUC__ 3
    #define GASNETC_CC_ELAN_GNUC 1
  #endif
  #if PLATFORM_ARCH_X86_64 && !defined(__x86_64)
    #define __x86_64 1
    #define GASNETC_CC_ELAN_X8664 1
  #endif
  #include <qsnet/fence.h>
  #if GASNETC_CC_ELAN_GNUC
    #undef __GNUC__
  #endif
  #if GASNETC_CC_ELAN_X8664
    #undef __x86_64
  #endif
#endif

#include <elan/elan.h>

#if !defined(GASNETC_ELAN3) && !defined(GASNETC_ELAN4)
  #error Must define GASNETC_ELAN3 or GASNETC_ELAN4
#endif

#if !defined(ELAN_VERSION_MAJOR) || !defined(ELAN_VERSION_MINOR) || !defined(ELAN_VERSION_SUB)
  #error Must define ELAN_VERSION_MAJOR, ELAN_VERSION_MINOR and ELAN_VERSION_SUB
#endif
#ifndef QSNETLIBS_VERSION
  #define QSNETLIBS_VERSION(a,b,c)	(((a) << 16) + ((b) << 8) + (c))
#endif
#if ELAN_VERSION_MAJOR == 1 && ELAN_VERSION_MINOR == 2
  #define ELAN_VER_1_2
  #define ELAN_VERSION_CODE     QSNETLIBS_VERSION(1,2,0)
#elif ELAN_VERSION_MAJOR == 1 && ELAN_VERSION_MINOR == 3
  #define ELAN_VER_1_3
  #define ELAN_VERSION_CODE     QSNETLIBS_VERSION(1,3,0)
#elif ELAN_VERSION_MAJOR == 1 && ELAN_VERSION_MINOR >= 4
  #define ELAN_VERSION_CODE     QSNETLIBS_VERSION_CODE
#else
  #error unknown elan version
#endif

#define ELAN_VERSION_GE(a,b,c) (ELAN_VERSION_CODE >= QSNETLIBS_VERSION(a,b,c))
#define ELAN_VERSION_LT(a,b,c) (ELAN_VERSION_CODE <  QSNETLIBS_VERSION(a,b,c))

#if ELAN_VERSION_GE(1,4,8)
  #define ELAN_SIZE_T size_t
#else
  #define ELAN_SIZE_T int
#endif

/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 1

/* use a (small) statically-allocated shared segment */
#define GASNETC_USE_STATIC_SEGMENT 0

/* ------------------------------------------------------------------------------------ */
#define GASNETC_HANDLER_BASE  1 /* reserve 1-63 for the core API */
#define _hidx_gasnetc_auxseg_reqh             (GASNETC_HANDLER_BASE+0)
/* add new core API handlers here and to the bottom of gasnet_core.c */

/* ------------------------------------------------------------------------------------ */
/* handler table (recommended impl) */
#define GASNETC_MAX_NUMHANDLERS   256
extern gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS];

/* ------------------------------------------------------------------------------------ */
/* AM category (recommended impl if supporting PSHM) */
typedef enum {
  gasnetc_Short=0,
  gasnetc_Medium=1,
  gasnetc_Long=2
} gasnetc_category_t;

extern ELAN_BASE  *gasnetc_elan_base;
extern ELAN_STATE *gasnetc_elan_state;
extern ELAN_GROUP *gasnetc_elan_group;
extern void *gasnetc_elan_ctx;
extern ELAN_TPORT *gasnetc_elan_tport;

#define BASE()  (gasnetc_elan_base)
#define STATE() (gasnetc_elan_state)
#define GROUP() (gasnetc_elan_group)
#ifdef GASNETC_ELAN4
#define CTX()   ((ELAN4_CTX *)gasnetc_elan_ctx)
#else
#define CTX()   ((ELAN3_CTX *)gasnetc_elan_ctx)
#endif
#define TPORT() (gasnetc_elan_tport)

#define GASNETI_EADDRFMT "0x%08x"
#define GASNETI_EADDRSTR(ptr) ((uint32_t)(uintptr_t)(ptr))

/* GASNet-elan system configuration parameters */
#define GASNETC_MAX_RECVMSGS_PER_POLL 10  /* max number of waiting messages serviced per poll (0 for unlimited) */
#define GASNETC_PREPOST_RECVS         1   /* pre-post non-blocking tport recv's */
#ifdef GASNETC_ELAN4
#define GASNETC_ELAN_MAX_QUEUEMSG  2048   /* max message in a mainqueue */
#else
#define GASNETC_ELAN_MAX_QUEUEMSG   320   /* max message in a mainqueue */
#endif
#define GASNETC_ELAN_SMALLPUTSZ      64   /* max put that elan_put copies to an elan buffer */

/* number of iterations to use when testing a put/get elan event with elan_poll */
#ifndef GASNETC_ELAN_POLLITERS
  #ifdef GASNETC_ELAN4
    /* TODO: using 0 should be faster, but testsmall shows it reduces put/get performance */
    #define GASNETC_ELAN_POLLITERS 1
  #else
    #define GASNETC_ELAN_POLLITERS 0
  #endif
#endif
/* number of iterations to use when testing an AM-related elan event with elan_poll */
#ifndef GASNETC_ELAN_POLLITERS_AM
#define GASNETC_ELAN_POLLITERS_AM 0
#endif

#ifdef ELAN_GLOBAL_DEST
  #define GASNETC_ELAN_GLOBAL_DEST ELAN_GLOBAL_DEST
#else
  #define GASNETC_ELAN_GLOBAL_DEST 1
#endif

#ifndef GASNETC_PREALLOC_AMLONG_BOUNCEBUF
  #ifdef GASNETC_ELAN4
    /* elan4 should never need an AMLong bounce buffer */
    #define GASNETC_PREALLOC_AMLONG_BOUNCEBUF 0
  #else
    #define GASNETC_PREALLOC_AMLONG_BOUNCEBUF 1
  #endif
#endif

/* whether our PGCTRL pool supports non-contiguous put/gets */
#ifndef GASNETE_PGCTRL_PGVSUPPORT
#define GASNETE_PGCTRL_PGVSUPPORT 0
#endif

/* libelan's tuning knobs for multi-rail NIC striping (TODO: are these good vals?) */
#ifndef GASNETC_PGCTRL_SPLITPUTSZ
#define GASNETC_PGCTRL_SPLITPUTSZ BASE()->putget_stripeputsize
#endif
#ifndef GASNETC_PGCTRL_SPLITGETSZ
#define GASNETC_PGCTRL_SPLITGETSZ BASE()->putget_stripegetsize
#endif
#ifndef GASNETC_PGCTRL_RAIL
  #ifdef ELAN_RAIL_ALL
    #define GASNETC_PGCTRL_RAIL ELAN_RAIL_ALL
  #else
    #define GASNETC_PGCTRL_RAIL 0
  #endif
#endif

/* max concurrent puts that can be issued to a pgctrl without software throttling 
   libelan currently requires this to be <= 64  */
#ifndef GASNETC_PGCTRL_THROTTLE
#define GASNETC_PGCTRL_THROTTLE 64
#endif

#ifndef GASNETC_ALLOW_ELAN_VERSION_MISMATCH
  #ifdef GASNETC_ELAN4
    /* elan4 reports libelan version mismatches, not sure why... */
    #define GASNETC_ALLOW_ELAN_VERSION_MISMATCH 1
  #else
    #define GASNETC_ALLOW_ELAN_VERSION_MISMATCH 1
  #endif
#endif

#ifndef GASNETC_ELAN_MAPS_ENTIRE_VM
  #if defined(GASNETC_ELAN4) || PLATFORM_ARCH_32
    /* elan4 has a 64-bit thread processor and always maps the entire host VM space.
       Quadrics confirms elan_addressable() should always return true on elan4.
       (although the elan/main VA's may differ).  
       We should also get the same effect on elan3 with 32-bit hosts
     */
    #define GASNETC_ELAN_MAPS_ENTIRE_VM  1
  #else
    #define GASNETC_ELAN_MAPS_ENTIRE_VM  0
  #endif
#endif

#if GASNETC_ELAN_MAPS_ENTIRE_VM
  #if GASNET_DEBUG
    #define gasnetc_elan_addressable(base, sz) \
      (gasneti_assert(elan_addressable(STATE(), (base), (sz))), 1)
  #else
    #define gasnetc_elan_addressable(base, sz) 1
  #endif
#else
  #define gasnetc_elan_addressable(base, sz) (elan_addressable(STATE(), (base), (sz)))
#endif

#ifndef GASNETC_ALLOW_ELAN_PERM_REMAP
  #if GASNETC_ELAN_MAPS_ENTIRE_VM
    /* memory remapping should never be necessary when we map the entire host VM space 
     */
    #define GASNETC_ALLOW_ELAN_PERM_REMAP 0
  #else
    #define GASNETC_ALLOW_ELAN_PERM_REMAP 1
  #endif
#endif

#ifndef GASNETC_USE_SIGNALING_EXIT
  #if HAVE_RMS_KILLRESOURCE || HAVE_SLURM_KILL_JOB
    #define GASNETC_USE_SIGNALING_EXIT 1
  #else
    #define GASNETC_USE_SIGNALING_EXIT 0
  #endif
#endif

#ifdef GASNETI_USE_GENERIC_ATOMICOPS
  /* need real atomic ops for signalling exit - force it off */
  #undef GASNETC_USE_SIGNALING_EXIT
  #define GASNETC_USE_SIGNALING_EXIT 0 
#endif

#if GASNETC_USE_SIGNALING_EXIT
  extern gasneti_atomic_t gasnetc_remoteexitflag;
  extern gasneti_atomic_t gasnetc_remoteexitrecvd; 
  #define GASNETC_EXITINPROGRESS()       (gasneti_atomic_read(&gasnetc_remoteexitflag, 0) != 1)
  #define GASNETC_REMOTEEXITINPROGRESS() (gasneti_atomic_read(&gasnetc_remoteexitrecvd, GASNETI_ATOMIC_RMB_PRE) != 0)
#else 
  #define GASNETC_EXITINPROGRESS() 0
  #define GASNETC_REMOTEEXITINPROGRESS() 0
#endif

#ifndef GASNETC_USE_MAINQUEUE
  #if HAVE_ELAN_QUEUETXINIT && defined(GASNETC_ELAN4) /* lemieux has elan_queuetxinit, but it doesn't work */
    #if GASNET_PAR
      /* ELAN_QUEUE_TX/RX are apparently not thread-safe, at least not as of 1.8.13 */
      #define GASNETC_USE_MAINQUEUE 1
    #else
      #define GASNETC_USE_MAINQUEUE 0
    #endif
  #else
    #define GASNETC_USE_MAINQUEUE 1
  #endif
#endif

#ifndef GASNETC_OVERLAP_AMQUEUE
  #if GASNETC_USE_MAINQUEUE
    #define GASNETC_OVERLAP_AMQUEUE 0
  #else
    /* define to 1 to enable overlap of elan_queuetx for AM's */
    #define GASNETC_OVERLAP_AMQUEUE 1
  #endif
#elif GASNETC_OVERLAP_AMQUEUE && GASNETC_USE_MAINQUEUE
  #error cannot overlap AMs when using main queue
#endif

typedef struct {
  ELAN_EVENT  **evt_lst; 
  uint16_t      evt_cnt;
  uint16_t      evt_sz;
} gasnete_evtbin_t;
extern int gasnete_evtbin_done(gasnete_evtbin_t *bin);
extern void gasnete_evtbin_save(gasnete_evtbin_t *bin, ELAN_EVENT *evt);
/* init evtbin with space of sizeof(ELAN_EVENT *)*sz */
GASNETI_INLINE(gasnete_evtbin_init)
void gasnete_evtbin_init(gasnete_evtbin_t *bin, uint16_t sz, ELAN_EVENT **space) {
  bin->evt_cnt = 0;
  bin->evt_sz = sz;
  bin->evt_lst = space;
}

/* add a define that was missing from many versions of elan4 */
#ifndef LIBELAN_QUEUEREUSEBUF
#define LIBELAN_QUEUEREUSEBUF 0x10000 
#endif

/* whether or not to stripe elan puts across multiple PGCTRL objects,
   to avoid software backpressure from libelan throttling during 
   message injection
*/
#ifndef GASNETE_MULTI_PGCTRL
  #ifdef ELAN_VER_1_2
    #define GASNETE_MULTI_PGCTRL 0 /* pgctrl not available on 1.2 */
  #else
    #define GASNETE_MULTI_PGCTRL 1
  #endif
#endif

#if GASNETE_MULTI_PGCTRL
  /* distribute puts round-robin onto different pgctrls to provide 
     a deeper effective put queue before software backpressure is applied
     this algorithm works probablistically under the assumption that put
     sizes are randomply distributed and drain fairly from all pgctrls - 
     for degenerate access patterns with multiple different put sizes 
     or unfair queue drainage, we might hit a full pgctrl queue backpressure 
     when some pgctrls still have available slots - a smarter algorithm 
     would try to query or predict the available slots and choose the 
     least loaded pgctrl
   */
  #ifndef GASNETE_NUMPGCTRL_CNTMAX
  #define GASNETE_NUMPGCTRL_CNTMAX  1024
  #endif
  extern int gasnete_elan_pgctrl_cnt;
  extern int _gasnete_elan_pgctrl_cur;
  extern ELAN_PGCTRL *gasnete_elan_pgctrl[GASNETE_NUMPGCTRL_CNTMAX];
  /* there's a race here for PAR mode, but it's benign - 
     this is a probabalistic heuristic anyhow 
     TODO: does assigning per-thread ELAN_PGCTRL's reduce locking contention in libelan?
   */
  GASNETI_INLINE(gasnetc_next_PGCTRL)
  ELAN_PGCTRL *gasnetc_next_PGCTRL(void) {
    int myidx = _gasnete_elan_pgctrl_cur;
    int newidx = myidx+1;
    gasneti_assert(gasnete_elan_pgctrl_cnt);
    gasneti_assert(gasnete_elan_pgctrl_cnt < GASNETE_NUMPGCTRL_CNTMAX);
    gasneti_assert(myidx < gasnete_elan_pgctrl_cnt);
    gasneti_assert(gasnete_elan_pgctrl[myidx]);
    if (newidx == gasnete_elan_pgctrl_cnt) newidx = 0;
    _gasnete_elan_pgctrl_cur = newidx;
    return gasnete_elan_pgctrl[myidx];
  }

  #define gasnete_elan_put(src, dest, nbytes, node) ( \
          ASSERT_ELAN_LOCKED_WEAK(),                  \
          elan_doput(gasnetc_next_PGCTRL(), src, dest, 0, nbytes, node, GASNETC_PGCTRL_RAIL))
  /* elan gets are not software-throttled, so currently always 
     assign them to the default PGCTRL, to avoid interference with puts
   */
  #define gasnete_elan_get(src, dest, nbytes, node) ( \
          ASSERT_ELAN_LOCKED_WEAK(),                  \
          elan_get(STATE(), src, dest, nbytes, node))

  #define GASNETC_IS_SMALLPUT(sz) (sz <= GASNETC_ELAN_SMALLPUTSZ)
#else
  #define gasnete_elan_put(src, dest, nbytes, node) ( \
          ASSERT_ELAN_LOCKED_WEAK(),                  \
          elan_put(STATE(), src, dest, nbytes, node))
  #define gasnete_elan_get(src, dest, nbytes, node) ( \
          ASSERT_ELAN_LOCKED_WEAK(),                  \
          elan_get(STATE(), src, dest, nbytes, node))

  #if ELAN_VERSION_GE(1,4,8)
    #define GASNETC_IS_SMALLPUT(sz) (sz <= BASE()->putget_smallputsize)
  #else
    #define GASNETC_IS_SMALLPUT(sz) (sz <= GASNETC_ELAN_SMALLPUTSZ)
  #endif
#endif


/* message flags */
 /* 0-1: category
  * 2:   request vs. reply 
  * 3-7: numargs
  */
typedef unsigned char gasnetc_flag_t;

#define GASNETC_MSG_SETFLAGS(pmsg, isreq, cat, numargs) \
  ((pmsg)->flags = (gasnetc_flag_t) (                   \
                   (((numargs) & 0x1F) << 3)            \
                 | (((isreq) & 0x1) << 2)               \
                 |  ((cat) & 0x3)                       \
                   ))
#define GASNETC_MSG_NUMARGS(pmsg)   ( ( ((unsigned char)(pmsg)->flags) >> 3 ) & 0x1F)
#define GASNETC_MSG_ISREQUEST(pmsg) (!!(((unsigned char)(pmsg)->flags) & 0x4))
#define GASNETC_MSG_CATEGORY(pmsg)  ((gasnetc_category_t)((pmsg)->flags & 0x3))

/* active message header & meta info fields */
typedef struct {
  gasnetc_flag_t    flags;
  gasnet_handler_t  handlerId;
  uint16_t          sourceId;
} gasnetc_msg_t;
typedef gasnetc_msg_t gasnetc_shortmsg_t;

typedef struct {
  gasnetc_flag_t    flags;
  gasnet_handler_t  handlerId;
  uint16_t          sourceId;

  uint16_t          nBytes;
  uint16_t          _pad;
} gasnetc_medmsg_t;

typedef struct {
  gasnetc_flag_t    flags;
  gasnet_handler_t  handlerId;
  uint16_t          sourceId;

  uint32_t          nBytes;
  uintptr_t	    destLoc;
} gasnetc_longmsg_t;


/* active message buffer, including message and space for data payload */
typedef struct gasnetc_buf {
  union {
    gasnetc_msg_t     msg;
    gasnetc_medmsg_t  medmsg;
    gasnetc_longmsg_t longmsg;
  } m;
  uint8_t     _Data[(4*GASNETC_MAX_ARGS)+GASNETC_MAX_MEDIUM]; /* holds args and data */
} gasnetc_buf_t;

/* buffer descriptor and bookkeeping info 
 */
typedef struct _gasnetc_bufdesc_t {
  gasnetc_buf_t *buf;        /* buf currently associated w/ descriptor (may be a system buf) */
  gasnetc_buf_t *buf_owned;  /* buf permanently owned by this desc (rx bufs only) */
  ELAN_EVENT *event;       /* packets in tport rx or tx queue, or NULL */
  struct _gasnetc_bufdesc_t *next;       /* tport rx or tx queue list */
  int8_t   handlerRunning; /* received packets only */
  int8_t   replyIssued;    /* received packets only */
} gasnetc_bufdesc_t;

extern int gasnetc_RequestGeneric(gasnetc_category_t category, 
                         int dest, gasnet_handler_t handler, 
                         void *source_addr, int nbytes, void *dest_ptr, 
                         int numargs, va_list argptr);
extern int gasnetc_ReplyGeneric(gasnetc_category_t category, 
                         gasnet_token_t token, gasnet_handler_t handler, 
                         void *source_addr, int nbytes, void *dest_ptr, 
                         int numargs, va_list argptr);

extern void gasnetc_initbufs(void);

#if GASNETI_CLIENT_THREADS
  #define gasnetc_mythread() ((void**)(gasnete_mythread()))
#else
  extern void **_gasnetc_mythread;
  #define gasnetc_mythread() _gasnetc_mythread
#endif

/* status dumping functions */
extern void gasnetc_dump_base(void);
extern void gasnetc_dump_state(void);
extern void gasnetc_dump_group(void);
extern void gasnetc_dump_envvars(void);
extern void gasnetc_dump_tportstats(void);
extern void gasnetc_dump_groupstats(void);
/* ------------------------------------------------------------------------------------ */
/* Elan conduit locks:
    elan lock - protects all elan calls and tport rx fifo
    sendfifo lock - protects tport tx fifo
   if you need both, must acquire sendfifo lock first
 */
extern gasneti_mutex_t gasnetc_elanLock;
extern gasneti_mutex_t gasnetc_sendfifoLock;
#define LOCK_ELAN()       do { gasneti_suspend_spinpollers(); gasneti_mutex_lock(&gasnetc_elanLock); } while (0)
#define UNLOCK_ELAN()     do { gasneti_mutex_unlock(&gasnetc_elanLock); gasneti_resume_spinpollers(); } while (0)
#define LOCK_SENDFIFO()   gasneti_mutex_lock(&gasnetc_sendfifoLock)
#define UNLOCK_SENDFIFO() gasneti_mutex_unlock(&gasnetc_sendfifoLock)

#define ASSERT_ELAN_LOCKED()       gasneti_mutex_assertlocked(&gasnetc_elanLock)
#define ASSERT_ELAN_UNLOCKED()     gasneti_mutex_assertunlocked(&gasnetc_elanLock)
#define ASSERT_SENDFIFO_LOCKED()   gasneti_mutex_assertlocked(&gasnetc_sendfifoLock)
#define ASSERT_SENDFIFO_UNLOCKED() gasneti_mutex_assertunlocked(&gasnetc_sendfifoLock)

/* (UN)LOCK_ELAN_WEAK is used when we only need mutual exclusion for the
   purposes of an elan call (which quadrics claims are all thread-safe)
   OK - so apparently the elan library is only threadsafe starting in v1.4
 */
#if GASNETI_THREADS
  #if defined(ELAN_VER_1_2) || defined(ELAN_VER_1_3)
    /* use real locks to provide thread-safety */
    #define LOCK_ELAN_WEAK()   LOCK_ELAN()
    #define UNLOCK_ELAN_WEAK() UNLOCK_ELAN()
    #define ASSERT_ELAN_LOCKED_WEAK() gasneti_mutex_assertlocked(&gasnetc_elanLock)
  #else
    /* elan library v1.4+ thread-safe - no weak locking required */
    #define LOCK_ELAN_WEAK()   gasneti_suspend_spinpollers()
    #define UNLOCK_ELAN_WEAK() gasneti_resume_spinpollers()
    #define ASSERT_ELAN_LOCKED_WEAK() ((void)0)
  #endif
#else
  /* doesn't actually lock anything - just preserves debug checking */
  #define LOCK_ELAN_WEAK()    LOCK_ELAN()
  #define UNLOCK_ELAN_WEAK()  UNLOCK_ELAN()
  #define ASSERT_ELAN_LOCKED_WEAK() gasneti_mutex_assertlocked(&gasnetc_elanLock)
#endif

#define UNLOCKRELOCK_ELAN_WEAK(cmd) do { \
    UNLOCK_ELAN_WEAK();                  \
    cmd;                                 \
    LOCK_ELAN_WEAK();                    \
  } while (0)

#if GASNETI_STATS_OR_TRACE
  /* wrap around trace calls in locked sections to prevent 
     weak lock violation on elan_clock() */
  #define UNLOCKRELOCK_ELAN_WEAK_IFTRACE(cmd) UNLOCKRELOCK_ELAN_WEAK(cmd)
#else
  #define UNLOCKRELOCK_ELAN_WEAK_IFTRACE(cmd) 
#endif

#endif
