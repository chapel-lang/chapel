/*   $Source: /var/local/cvs/gasnet/lapi-conduit/gasnet_core_internal.h,v $
 *     $Date: 2008/03/09 09:38:31 $
 * $Revision: 1.53 $
 * Description: GASNet lapi conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

/* =======================================================================
 * LAPI Conduit Implementation for IBM SP.
 * Michael Welcome
 * Lawrence Berkeley National Laboratory
 * mlwelcome@lbl.gov
 * November, 2002
 * =======================================================================
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>
#if GASNETC_LAPI_RDMA
  #include <firehose.h>
#endif

/* LAPI Specific decls */
#include <stddef.h>

extern lapi_info_t        gasnetc_lapi_info;
extern volatile int gasnetc_got_exit_signal;
extern int                gasnetc_max_lapi_uhdr_size;
extern unsigned long      gasnetc_max_lapi_data_size;
extern void**             gasnetc_remote_req_hh;
extern void**             gasnetc_remote_reply_hh;

/* Enable loopback by setting to 1, disable by setting to 0 */
#define GASNETC_ENABLE_LOOPBACK 1

#define GASNETC_MAX_NUMHANDLERS   256

#define GASNETC_LCHECK(func) do {                                 \
    int lapi_errno;                                               \
    if_pf ((lapi_errno = func) != LAPI_SUCCESS) {                 \
       char gasnetc_lapi_msg[LAPI_MAX_ERR_STRING];                \
       if (gasnetc_got_exit_signal) {                             \
         int i;                                                   \
         /* a shutdown is in progress, and likely caused the */   \
         /* LAPI failure - silently ignore it and wait to die */  \
         for (i=0; i < 5; i++) sleep(1);                          \
         gasneti_killmyprocess(-1); /* prevent zombies */         \
       }                                                          \
       LAPI_Msg_string(lapi_errno,gasnetc_lapi_msg);              \
       gasneti_fatalerror("LAPI Error on node %d in file %s"      \
                          " at line %d, [%s] return code = %d\n", \
                          gasneti_mynode,__FILE__,__LINE__,       \
                          gasnetc_lapi_msg,lapi_errno);           \
    }                                                             \
  } while(0)

/* Define how to poll in LAPI.
 * Should use LAPI_Msgpoll when available since it
 * temporarily disables LAPI interrupt mode (if used).  Not available
 * in older versions of LAPI.  Note that the info structure contains
 * a status field to determine if the dispatcher did poll, if another
 * thread was already polling, or if a message operation completed
 * before the requested number of polling operations completed.
 */
#if (GASNETC_LAPI_VERSION > 1)
#define GASNETC_LAPI_POLL(context) do {          \
    lapi_msg_info_t info;                        \
    GASNETC_LCHECK(LAPI_Msgpoll((context),1,&info));    \
 } while (0)                          
#else
#define GASNETC_LAPI_POLL(context) GASNETC_LCHECK(LAPI_Probe(context))
#endif

typedef void (*gasnetc_handler_fn_t)();  /* prototype for handler function */
extern gasnetc_handler_fn_t gasnetc_handler[]; /* handler table */

extern void gasnetc_lapi_exchange(void *src, size_t len, void *dest);

extern void gasnetc_lapi_err_handler(lapi_handle_t *context, int *error_code,
				     lapi_err_t  *error_type, int *taskid, int *src);

extern void* gasnetc_lapi_AMreq_hh(lapi_handle_t *context, void *uhdr, uint *uhdr_len,
				   ulong *msg_len, compl_hndlr_t **comp_h, void **uinfo);
extern void* gasnetc_lapi_AMreply_hh(lapi_handle_t *context, void *uhdr, uint *uhdr_len,
				     ulong *msg_len, compl_hndlr_t **comp_h, void **uinfo);
extern void gasnetc_lapi_AMch(lapi_handle_t *context, void *uinfo);

/* what type of message are we sending/receiving */
typedef enum {
    gasnetc_Short=0, 
    gasnetc_Medium=1, 
    gasnetc_Long=2,
    gasnetc_AsyncLong=3
} gasnetc_category_t;

static const char *gasnetc_catname[] = {"Short","Medium","Long","AsyncLong"};

#if GASNETC_USE_INTERRUPTS
#define GASNETC_MAX_THREAD 20
extern volatile int gasnetc_interrupt_held[];
#endif

/* the important contents of a gasnet token */
typedef unsigned int gasnetc_flag_t;
typedef struct {
    gasnetc_flag_t       flags;
    gasnet_handler_t     handlerId;
    gasnet_node_t        sourceId;
    uintptr_t            destLoc;
    size_t               dataLen;
    uintptr_t            uhdrLoc;    /* only used on AsyncLong messages */
    gasnet_handlerarg_t  args[GASNETC_AM_MAX_ARGS];
} gasnetc_msg_t;

#define GASNETC_MSG_SETFLAGS(pmsg, isreq, cat, packed, numargs) \
  ((pmsg)->flags = (gasnetc_flag_t) (                   \
                   (((numargs) & 0xFF) << 16)           \
                 | (((isreq) & 0x1)    << 8)            \
                 | (((packed) & 0x1)   << 3)            \
                 |  ((cat) & 0x3)                       \
                   ))

#define GASNETC_MSG_NUMARGS(pmsg)   ( ( ((unsigned int)(pmsg)->flags) >> 16 ) & 0xFF)
#define GASNETC_MSG_ISREQUEST(pmsg) ( ( ((unsigned int)(pmsg)->flags) >>  8 ) & 0x1)
#define GASNETC_MSG_CATEGORY(pmsg)  ((gasnetc_category_t)((pmsg)->flags & 0x3))
#define GASNETC_MSG_ISPACKED(pmsg)  ((unsigned int)((pmsg)->flags & 0x8))
#define GASNETC_MSG_SET_PACKED(pmsg) (pmsg)->flags |= 0x8

#define GASNETC_DOUBLEWORD 8
/* align pointer to nearest (forward) 8-byte boundary */
#define GASNETC_ALIGN_PTR(p) (void*)( ((uintptr_t)(p) + 0x7) & ~0x7 )
/* Round integer up to next multiple of 8 */
#define GASNETC_ROUND_DOUBLEWORD(x) (int)( ((unsigned int)(x) + 0x7) & ~0x7 )
/* --------------------------------------------------------------------
 * the following structure is use as a LAPI-conduit gasnet_token_t.
 * It is also the uhdr structure used in all CORE LAPI Amsend calls.
 *
 * The next pointer allow for the re-use of allocated tokens
 * by stringing them on a free list.  Also, it is used to
 * place tokens on a queue for fast AM request processing in polling
 * mode.
 *
 * --------------------------------------------------------------------
 */
typedef struct gasnetc_token_rec {
    struct gasnetc_token_rec  *next;
    gasnetc_msg_t    msg;
} gasnetc_token_t;
/*
 * We currently set the token length at compile time.
 * Will change this in the future.
 * On Federation systems the packet size if 2KB, whereas
 * its 1KB on older, switch2-based systems like Seaborg.
 * Dont know how to detect this at compile time, so for now
 * we just use the lapi version number.  All federation systems
 * use the new version of LAPI and seaborg uses the older version
 * (at least for now).
 */
#if (GASNETC_LAPI_VERSION > 1)
#define GASNETC_TOKEN_SIZE 2048
#else
#define GASNETC_TOKEN_SIZE 1024
#endif

#define TOKEN_LEN(narg) offsetof(gasnetc_token_t,msg) \
                      + offsetof(gasnetc_msg_t,args) \
                      + (narg)*sizeof(gasnet_handlerarg_t)

#ifndef GASNET_LAPI_MODE_DEFAULT
#define GASNET_LAPI_MODE_DEFAULT POLLING
#endif

/* --------------------------------------------------------------------
 * A simple spinlock implementation
 * Implemented as either a true spinlock or a mutex
 * --------------------------------------------------------------------
 */

#if GASNETC_USE_SPINLOCKS
  #define gasnetc_spinlock_init(lock)     gasneti_spinlock_init((lock))
  #define gasnetc_spinlock_destroy(lock)  gasneti_spinlock_destroy((lock))
  #define gasnetc_spinlock_lock(lock)     gasneti_spinlock_lock((lock))
  #define gasnetc_spinlock_unlock(lock)   gasneti_spinlock_unlock((lock))
  #define gasnetc_spinlock_trylock(lock)  gasneti_spinlock_trylock((lock))
#else  /* Use pthread mutex for spinlock */
  #define gasnetc_spinlock_init(lock)     gasneti_mutex_init((lock))
  #define gasnetc_spinlock_destroy(lock)  gasneti_mutex_destroy((lock))
  #define gasnetc_spinlock_lock(lock)     gasneti_mutex_lock((lock))
  #define gasnetc_spinlock_unlock(lock)   gasneti_mutex_unlock((lock))
  #define gasnetc_spinlock_trylock(lock)  gasneti_mutex_trylock((lock))
#endif

/* --------------------------------------------------------------------
 * A freelist structure for the re-use of gasnetc_buf_t structures.
 * --------------------------------------------------------------------
 */
#define GASNETC_UHDR_INIT_CNT 256
#define GASNETC_UHDR_ADDITIONAL 256
typedef struct {
    int    high_water_mark;
    int    numfree;
    int    numalloc;
    gasnetc_token_t *head;
    gasnetc_spinlock_t  lock;
} gasnetc_uhdr_freelist_t;

extern gasnetc_uhdr_freelist_t gasnetc_uhdr_freelist;
extern void   gasnetc_uhdr_init(int want);
extern gasnetc_token_t*  gasnetc_uhdr_alloc(void);
extern void   gasnetc_uhdr_free(gasnetc_token_t* uhdr);
extern int    gasnetc_uhdr_more(int want);

/* --------------------------------------------------------------------
 * Fast mutial exclusion queue using spinlocks
 * --------------------------------------------------------------------
 */
typedef struct {
    gasnetc_token_t *head;
    gasnetc_token_t *tail;
    gasnetc_spinlock_t lock;
    int              schedule;
} gasnetc_token_queue_t;
extern void gasnetc_token_queue_init(gasnetc_token_queue_t *q);
extern gasnetc_token_t* gasnetc_token_dequeue(gasnetc_token_queue_t *q, int update_schedule);
extern void gasnetc_token_enqueue(gasnetc_token_queue_t *q, gasnetc_token_t *p, int *schedule);

/* return the AIX page size for a given memory address */
extern size_t gasnetc_get_pagesize(void *addr);

/* ------------------------------------------------------------------------------------ */
#if GASNETI_THROTTLE_POLLERS 
  #ifndef GASNETC_LAPIWAIT_SPIN
    /* spinning with LAPI_GetCntr performs better under contention than LAPI_WaitCntr */
    #define GASNETC_LAPIWAIT_SPIN 1
  #endif
  /* next two only affect behavior if GASNETC_LAPIWAIT_SPIN is enabled */
  #ifndef GASNETC_LAPIWAIT_SPIN_SUSPRESM
    /* suspend/resume_spinpollers currently never seem to improve lapi performance, 
       so make them no-ops for now */
    #define GASNETC_LAPIWAIT_SPIN_SUSPRESM 0
  #endif
  #ifndef GASNETC_LAPIWAIT_SPIN_TOGGLEINTR
    /* toggling interrupt mode while spinning - 
       produces crazy performance due to lack of thread safety in toggle */
    #define GASNETC_LAPIWAIT_SPIN_TOGGLEINTR 0
  #endif
  
  #if !GASNETC_LAPIWAIT_SPIN_SUSPRESM
    #undef gasneti_suspend_spinpollers
    #undef gasneti_resume_spinpollers
    #define gasneti_suspend_spinpollers() gasneti_suspend_spinpollers_check()
    #define gasneti_resume_spinpollers()  gasneti_resume_spinpollers_check()
  #endif
  #if GASNETC_LAPIWAIT_SPIN_TOGGLEINTR
    #define gasnetc_hold_lapiinterrupts()   GASNETC_PAUSE_INTERRUPT_MODE()
    #define gasnetc_resume_lapiinterrupts() GASNETC_RESUME_INTERRUPT_MODE()
  #else
    #define gasnetc_hold_lapiinterrupts()
    #define gasnetc_resume_lapiinterrupts()
  #endif

  #if GASNETC_LAPIWAIT_SPIN
    #define GASNETC_WAITCNTR(cntr, val, result) do {                        \
      int tmp;                                                              \
      GASNETC_LCHECK(LAPI_Getcntr(gasnetc_lapi_context, (cntr), &tmp));     \
      if (tmp < (val)) {                                                    \
        gasnetc_hold_lapiinterrupts();              \
        do {                                                                \
          gasneti_AMPoll();                                                 \
          GASNETC_LCHECK(LAPI_Getcntr(gasnetc_lapi_context, (cntr), &tmp)); \
        } while (tmp < (val));                                              \
        gasnetc_resume_lapiinterrupts();              \
      }                                                                     \
      tmp -= (val);                                                         \
      GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context, (cntr), tmp));      \
      if (result != NULL) *(int*)(result) = tmp;                            \
    } while (0)
  #else
    #define GASNETC_WAITCNTR(cntr, val, result) \
      GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context, (cntr), (val), (result)))
  #endif
#else
  #define GASNETC_WAITCNTR(cntr, val, result) \
    GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context, (cntr), (val), (result)))
#endif

/* Define a special version of WAITCNTR for selected places in the
 * code where we know the problem is exhimited
 */
#if GASNETC_LAPI_FED_FLOWCONTROL_BUG
    #define GASNETC_WAITCNTR_FBW(cntr, val, result) \
    GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context, (cntr), (val), (result)))
#else
    #define GASNETC_WAITCNTR_FBW(cntr, val, result) GASNETC_WAITCNTR((cntr),(val),(result))
#endif

/* -------------------------------------------------------------------- */
#define GASNETC_HANDLER_BASE  1 /* reserve 1-63 for the core API */
#define _hidx_gasnetc_auxseg_reqh             (GASNETC_HANDLER_BASE+0)
/* add new core API handlers here and to the bottom of gasnet_core.c */

/* ------------------------------------------------------------------------------------ */

#if GASNETC_LAPI_RDMA
/* PVOs of 16M length, where 2^24 = 16*1024*1024 */
#define GASNETC_LAPI_PVO_EXTENT_BITS	24
#define GASNETC_LAPI_PVO_EXTENT		(1UL << GASNETC_LAPI_PVO_EXTENT_BITS)
#define GASNETC_LAPI_PVO_EXTENT_MASK	(GASNETC_LAPI_PVO_EXTENT - 1)

#define GASNETC_MAX_PVOS_PER_THREAD 16
extern int gasnetc_use_firehose;

extern lapi_remote_cxt_t **gasnetc_remote_ctxts;
extern gasneti_weakatomic_t *gasnetc_lapi_current_rctxt;
extern gasneti_weakatomic_val_t gasnetc_rctxts_per_node_mask;
extern lapi_user_pvo_t **gasnetc_pvo_table;
extern lapi_long_t *gasnetc_segbase_table;
extern lapi_long_t gasnetc_my_segbase;
extern lapi_long_t gasnetc_my_segtop;
extern int *gasnetc_lapi_local_target_counters;
extern lapi_cntr_t **gasnetc_lapi_completion_ptrs;
extern lapi_long_t *gasnetc_lapi_target_counter_directory;
extern int gasnetc_lapi_done;
extern int gasnetc_lapi_empty;
extern int gasnetc_lapi_occupied;
#endif
#endif
