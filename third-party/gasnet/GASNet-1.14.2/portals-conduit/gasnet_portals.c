#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>
#include <gasnet_portals.h>
#include <signal.h>
#ifdef HAVE_MMAP
#include <sys/mman.h> /* For MAP_FAILED */
#endif

#define GASNETC_DEBUG_RB_VERBOSE 0

#if HAVE_PMI_CNOS
   #define _CRAY_PORTALS
   #include <pmi.h>
   #define GASNETC_NEED_CNOS_NIDPID_MAP_T 1
#elif HAVE_CNOS_MPI_OS_H /* new CNL (bug 2472) */
   #include <cnos_mpi_os.h>
#elif HAVE_CATAMOUNT_CNOS_MPI_OS_H /* catamount and new CNL */
   #include <catamount/cnos_mpi_os.h>
#elif HAVE_PCTMBOX_H /* old CNL */
   #include <pctmbox.h>
#else /* backup declarations, since these headers seem to be in flux */
  extern int cnos_get_rank();
  extern int cnos_get_size();
  extern int cnos_get_nidpid_map(void *);
  extern void cnos_barrier_init(ptl_handle_ni_t ni_handle); /* NOOP function on Catamount */
  extern int cnos_barrier(void);
  #if PLATFORM_OS_CNL
    extern void cnos_pm_barrier(int);
    extern int cnos_register_ptlid(ptl_process_id_t);
  #endif
  #define GASNETC_NEED_CNOS_NIDPID_MAP_T 1
#endif
#if GASNETC_NEED_CNOS_NIDPID_MAP_T
  typedef struct {
      ptl_nid_t nid;
      ptl_pid_t pid;
      #ifdef STRIDER0
        int port;
      #endif
  } cnos_nidpid_map_t;
#endif

/* set to one for ReqRB Auto Unlink
 * We used to prefer Manual unlink, until bug 2461 revealed that it
 * was flawed.  Now AUTO_UNLINK is the only safe option.
 */
#define GASNETC_REQRB_AUTO_UNLINK 1
#define GASNETC_REQRB_UNLINK_VERBOSE 0

/* for all the MEs we create */
static const ptl_process_id_t gasnetc_any_id = {PTL_NID_ANY,PTL_PID_ANY};

/* Max number of events we will process per polling call */
unsigned gasnetc_safe_poll_limit = 12;
unsigned gasnetc_am_poll_limit = 8;
unsigned gasnetc_sys_poll_limit = 0;  /* 0 => infinite */

/* We maintain a single Request send buffer */
size_t gasnetc_ReqSB_numchunk = 2048;
gasnetc_PtlBuffer_t gasnetc_ReqSB;

/* We maintain an array of Request Receive Buffers */
int    gasnetc_ReqRB_pool_size = 8;
size_t gasnetc_ReqRB_numchunk = 1024;
gasnetc_PtlBuffer_t **gasnetc_ReqRB;          
#if GASNET_PAR
  #define GASNETC_REQRB_SPARES 2
  static gasneti_weakatomic_t gasnetc_spare_ReqRB = gasneti_weakatomic_init(GASNETC_REQRB_SPARES);
#else
  #define GASNETC_REQRB_SPARES 1
#endif

/* We maintain a single Reply send buffer.
 * Each threads is allowed to cache up to one buffer.
 */
size_t gasnetc_RplSB_numchunk = 256;  /* each thread allowed to cache up to one */
gasnetc_PtlBuffer_t gasnetc_RplSB;

/* The catch-basin buffer */
gasnetc_PtlBuffer_t gasnetc_CB;          

/* And the Remote Access Region, covered by two buffers */
gasnetc_PtlBuffer_t gasnetc_RAR;
gasnetc_PtlBuffer_t gasnetc_RARAM;
gasnetc_PtlBuffer_t gasnetc_RARSRC;

/* Max size of a bounced put or get (differ by space for bounce addr in the get) */
size_t gasnetc_put_bounce_limit;
size_t gasnetc_get_bounce_limit;

ptl_handle_ni_t gasnetc_ni_h;              /* the network interface handle */
gasnetc_eq_t *gasnetc_AM_EQ = NULL;        /* The AM Event Queue */
gasnetc_eq_t *gasnetc_SAFE_EQ = NULL;      /* The SAFE Event Queue */

#if GASNETC_USE_MDUPDATE
gasnetc_eq_t *gasnetc_EMPTY_EQ = NULL;     /* For MDUpdate, since it rejects PTL_EQ_NONE */
#endif

/* out of band MDs for sending system messages */
gasnetc_PtlBuffer_t gasnetc_SYS_Send;       /* out-of-band message send buffer */
gasnetc_PtlBuffer_t gasnetc_SYS_Recv;       /* out-of-band message recv buffer */
gasnetc_eq_t *gasnetc_SYS_EQ = NULL;        /* out-of-band system Event Queue */
/* a flag that is set to true after the network is initialized */
static int portals_sysqueue_initialized = 0;
double gasnetc_shutdown_seconds = 0.;
int gasnetc_shutdownInProgress = 0;
static gasneti_atomic_t gasnetc_exitcode = gasneti_atomic_init(0);
static uint32_t sys_exit_rcvd = 0;
static double shutdown_max = 360.;  /* 6 minutes ... just a guess */
static uint32_t sys_barrier_rcvd[2];
/* stores signal number when signal occurs */
gasneti_weakatomic_t gasnetc_got_signum;
/* indicates when portals resources have been initialized on all nodes */
int gasnetc_resource_init_complete = 0;

/* We limit the number of temporary memory descriptors in use at any time.
 * Each allocation of a temp MD requires a tmpmd ticket.
 * gasnetc_tmpmd_tickets is the number of tickets available.
 * When drops to zero, no more tmpmds can be allocated and thread must poll
 * until positive.
 */
int gasnetc_max_tmpmd = GASNETC_MAX_TMP_MDS;
gasneti_semaphore_t gasnetc_tmpmd_tickets;
#if GASNETI_STATS_OR_TRACE
int gasnetc_tmpmd_hwm = 0;
#endif

ptl_uid_t gasnetc_uid;
ptl_process_id_t gasnetc_myid;
gasnetc_procid_t *gasnetc_procid_map = NULL;

#if GASNETC_USE_SANDIA_ACCEL
/* use Sandia Accelerated Portals */
int gasnetc_use_accel = 0;
#endif

/* flow control and dynamic credit management variables */
int gasnetc_use_flow_control = 1;             /* turn on/off AM Request flow control */
int gasnetc_use_dynamic_credits = 1;          /* turn on/off credit redistribution algorithm */
long gasnetc_total_credits = 0;               /* total number of credits to distribute */
gasneti_semaphore_t gasnetc_banked_credits;   /* number of credits avail for redistribution */
int gasnetc_revoke_limit = 32;                /* max credits that can be revoked in one epoch */
int gasnetc_lender_limit = 32;                /* max number of credits that can given in one epoch */
int gasnetc_max_cpn = 400;                    /* max number of credits that can be given to a node */
gasneti_mutex_t gasnetc_epoch_lock = GASNETI_MUTEX_INITIALIZER;  /* epoch update lock */
gasneti_weakatomic_t gasnetc_AMRequest_count; /* Count of number of AMRequests we receive */
int gasnetc_epoch_duration = 1024;            /* Number of AMRequests we receive before end of epoch */
gasnet_node_t gasnetc_scavenge_list = GASNETC_DLL_NULL;    /* scavenge list */
gasneti_mutex_t gasnetc_scavenge_lock = GASNETI_MUTEX_INITIALIZER;  /* lock for scavenge list */
int gasnetc_num_scavenge = 6;                 /* Max number of nodes to hit-up each scavenge run */
gasneti_weakatomic_t gasnetc_scavenge_inflight;  /* number of outstanding revoke requests */
int gasnetc_dump_stats = 0;                   /* write some stats info to files */
int gasnetc_debug_node = -1;                  /* used in debugging */

gasnetc_conn_t *gasnetc_conn_state = NULL;
static gasneti_lifo_head_t gasnetc_lid_freelist = GASNETI_LIFO_INITIALIZER;

/* ------------------------------------------------------------------------------------ */
/* The number of available send tickets and the message limit.
 * Note that we must limit so that we can insure no event queue overflow
 * for operations we initiate.  In general, each Put generates two local events
 * and each Get also generates two (although only should generate one).
 */
gasneti_semaphore_t gasnetc_send_tickets;
int gasnetc_msg_limit = 250;
static int gasnetc_msg_minimum = 12; /* most likely deadlock with fewer */

/* by default, allow packed AMLong messages */
int gasnetc_allow_packed_long = 1;

const char* gasnetc_md_name[] = {"RAR_MD","RARAM_MD","RARSRC_MD","REQSB_MD","REQRB_MD","RPLSB_MD","CB_MD","TMP_MD","SYS_SEND","SYS_RECV"};

/* debugging aids */
uint32_t gasnetc_snd_seqno=0;
uint32_t gasnetc_rcv_seqno=0;
uint32_t gasnetc_amseqno = 0;

/* Firehose stuff */

size_t gasnetc_AMMaxLong;

#if !PLATFORM_OS_CATAMOUNT

/* XXX: Need dynamic discovery of limits, but bug 2053 makes that problematic.
 * For now we'll use 1024 regions of maximum length 128K.
 * That should be sufficiently small usage (128MB worst case) to not crash.
 */
#ifndef GASNETC_FIREHOSE_MAXREGIONS
  #define GASNETC_FIREHOSE_MAXREGIONS 1024
#endif
#ifndef GASNETC_FIREHOSE_MAXREGION_SIZE
  #define GASNETC_FIREHOSE_MAXREGION_SIZE (128*1024)
#endif

int gasnetc_use_firehose;
firehose_info_t gasnetc_firehose_info;

#endif /* !PLATFORM_OS_CATAMOUNT */

/* =================================================================================
 * This top portion of the file is where file-scope worker routines are located.
 * ================================================================================= */

/* Forward reference for ReqRB event handler */
static void ReqRB_event(ptl_event_t *ev);

/* Forward declarations for event queue alloc/free */
static gasnetc_eq_t* gasnetc_eq_alloc(long num_events, const char* name, ptl_eq_handler_t hndlr);
static void gasnetc_eq_free(gasnetc_eq_t *eq);

/* ------------------------------------------------------------------------------------
 * Search the lid cache for this object.
 * If not found, create one, add to list and return it, setting found = false.
 * If found, remove from list and return, setting found = true;
 * This is a worker function that should ONLY be called from 
 *        get_lid_object_from_data 
 *  -OR-  get_lid_object_from_header
 * Lid cache must be locked before this is called, caller must unlock.
 * --------------------------------------------------------------------------------- */
static int get_or_insert_lid(gasnet_node_t src, uint32_t lid, gasnetc_amlongcache_t **obj)
{
  gasnetc_amlongcache_t *p, *prev;
  int found = 0;
  prev = NULL;
  p = gasnetc_conn_state[src].lids;
  while (! found) {
    if (p == NULL) break;
    if (p->dest_lid == lid) {
      found = 1;
    } else { /* advance */
      prev = p;
      p = p->next;
    }
  }

  if (found) {
    /* remove from the list and return it */
    if (prev == NULL) {
      /* first in list */
      gasnetc_conn_state[src].lids = p->next;
    } else {
      prev->next = p->next;
    }
    p->next = NULL;
    *obj = p;
    return found;
  }

  /* not found, create new entry and add to list */
  p = gasneti_lifo_pop(&gasnetc_lid_freelist);
  if_pf (!p) p = (gasnetc_amlongcache_t*)gasneti_malloc(sizeof(gasnetc_amlongcache_t));
  p->dest_lid = lid;
#if GASNET_DEBUG
  p->flags = 0;
#endif
  /* prev is either NULL, or points to the end of the list */
  if (prev == NULL) {
    /* empty list */
    p->next = gasnetc_conn_state[src].lids;
    gasnetc_conn_state[src].lids = p;
  } else {
    /* add at end */
    p->next = prev->next;
    prev->next = p;
  }

  *obj = p;
  return 0;
}

/* ------------------------------------------------------------------------------------
 * Search the lid cache from this src node for a matching object.
 * This call is made when the data portion of an AM Long arrives in the RARAM.
 * If found
 *     - remove from cache and return it
 *     - Data fields not updated
 *     - calling routine must deallocate
 * If not found
 *     - alloc new object and insert into cache.
 *     - set data fields as per arguments.
 *     - return NULL.
 *
 * --------------------------------------------------------------------------------- */
static gasnetc_amlongcache_t* get_lid_obj_from_data(ptl_hdr_data_t hdr_data, void* dataptr, size_t datalen)
{
  uint32_t lid = GASNETI_LOWORD(hdr_data);
  gasnet_node_t src = GASNETI_HIWORD(hdr_data);
  gasnetc_amlongcache_t *obj;
  int found;

  GASNETC_LOCK_NODE(src);
  found = get_or_insert_lid(src, lid, &obj);
#if GASNET_DEBUG
  gasneti_assert( obj->flags == (found ? GASNETC_LID_HEADER_HERE : 0) );
  obj->flags |= GASNETC_LID_DATA_HERE;
#endif
  obj->data = dataptr;                  /* only data message writes this */
  obj->nbytes = datalen;                /* only data message writes this */
  GASNETC_UNLOCK_NODE(src);

  return found ? obj : NULL;
}

/* ------------------------------------------------------------------------------------
 * Search the lid cache from this src node for a matching object.
 * This call is made when the Header portion of an AM Long arrives in either ReqRB or ReqSB.
 * If found
 *     - remove from cache and return it
 *     - Data fields not updated
 *     - calling routine must deallocate
 * If not found
 *     - alloc new object and insert into cache.
 *     - set data fields as per arguments.
 *     - return NULL.
 * --------------------------------------------------------------------------------- */
static gasnetc_amlongcache_t* get_lid_obj_from_header(gasnet_node_t src, uint32_t lid, gasnetc_ptl_token_t *ptok)
{
  gasnetc_amlongcache_t *obj;
  int found;

  GASNETC_LOCK_NODE(src);
  found = get_or_insert_lid(src, lid, &obj);
#if GASNET_DEBUG
  gasneti_assert( obj->flags == (found ? GASNETC_LID_DATA_HERE : 0) );
  obj->flags |= GASNETC_LID_HEADER_HERE;
#endif
  obj->tok = *ptok;             /* only header message writes these */
  GASNETC_UNLOCK_NODE(src);

  return found ? obj : NULL;
}

/* ------------------------------------------------------------------------------------
 * Unpack the data from the event structure and execute the Request or Reply AM Short
 * handler function.
 * isReq is true if this is an AM Short Request, false for a Reply.
 *
 * NOTES:
 *   - message should be GASNETI_MEDBUF_ALIGNMENT aligned.
 * This function is called from exec_am_header on arrival of an AM Short
 * Returns TRUE if an implicit Reply must be sent
 * --------------------------------------------------------------------------------- */
static int exec_amshort_handler(gasnetc_ptl_token_t *ptok, uint32_t *data32, int numarg, int ghandler)
{
  gasnet_token_t token;
  int  isReq = ptok->need_reply;
#if GASNET_DEBUG || GASNETI_STATS_OR_TRACE
  gasnetc_threaddata_t *th = gasnetc_mythread();
#endif

  GASNETC_DEF_HARGS();     /* debug, must be first statement */
  GASNETC_GET_SEQNO(ptok); /* debug */

  /* AM Short Data Format:
   *     HD=[srcnode,cred] Data=[args][seqno][pad]
   * NOTE: seqno included only in debug mode, pad only in Request
   */

  if (isReq) GASNETC_MSGLEN_PAD(ptok->msg_bytes); /* Only Req is padded */

  #if GASNETI_STATS_OR_TRACE
  {
    uint8_t ee,nx,nc;
    GASNETC_READ_CREDIT_BYTE(ptok->credits,ee,nx,nc);
    GASNETI_TRACE_PRINTF(C,("CREDINFO: Short %s s=%d r=%d cred_byte=%x %d:%d:%d",(isReq?"Req":"Rpl"),ptok->srcnode,gasneti_mynode,ptok->credits,ee,nx,nc));
  }
  #endif

  /* Process credit info and prep credit byte for return (if isReq) */
  if (gasnetc_use_flow_control) {
    ptok->credits = gasnetc_credit_update(isReq,ptok->credits,ptok->srcnode,"Short");
  }
    
  GASNETC_DBGMSG(0,isReq,"S",ptok->srcnode,gasneti_mynode,ghandler,numarg,ptok->args,ptok->msg_bytes,ptok->credits,0,NULL,th);

  token = (gasnet_token_t)ptok;
  GASNETI_RUN_HANDLER_SHORT(isReq, ghandler, gasnetc_handler[ghandler], token, ptok->args, numarg);

  return ptok->need_reply;
}

/* ------------------------------------------------------------------------------------
 * Unpack the data from the event structure and execute the Request or Reply AM Medium
 * handler function.
 * isReq is true if this is an AM Medium Request, false for a Reply.
 *
 * NOTES:
 *   - message should be GASNETI_MEDBUF_ALIGNMENT aligned.
 *   - pad insures data payload is GASNETI_MEDBUF_ALIGNMENT aligned.
 * This function is called from exec_am_header on arrival of an AM Medium
 * Returns TRUE if an implicit Reply must be sent
 * --------------------------------------------------------------------------------- */
static int exec_ammedium_handler(gasnetc_ptl_token_t *ptok, uint32_t *data32,
                                 int numarg, int ghandler, int nbytes)
{
  gasnet_token_t token;
  uint32_t cred_len;
  int      isReq = ptok->need_reply;
#if GASNET_DEBUG || GASNETI_STATS_OR_TRACE
  gasnetc_threaddata_t *th = gasnetc_mythread();
#endif

  GASNETC_DEF_HARGS();     /* debug, must be first statement */
  GASNETC_GET_SEQNO(ptok); /* debug */

  /* Medium Data Format: 
   *     HD=[srcnode,cred] Data=[args][seqno][pad][data][pad]
   * NOTE: seqno included only in debug mode, trailing pad only in Request
   */

  /* Skip over any pad field so that handler payload is properly aligned */
  GASNETC_MSGLEN_PAD(ptok->msg_bytes);
  data32 = (uint32_t*)GASNETI_ALIGNUP(data32,GASNETI_MEDBUF_ALIGNMENT);

  /* accounting: this should be same as message length */
  GASNETC_MSGLEN_ADD(ptok->msg_bytes, nbytes);
  if (isReq) GASNETC_MSGLEN_PAD(ptok->msg_bytes); /* Only Req is padded */

  #if GASNETI_STATS_OR_TRACE
  {
    uint8_t ee,nx,nc;
    GASNETC_READ_CREDIT_BYTE(ptok->credits,ee,nx,nc);
    GASNETI_TRACE_PRINTF(C,("CREDINFO: Med %s s=%d r=%d cred_byte=%x %d:%d:%d",(isReq?"Req":"Rpl"),ptok->srcnode,gasneti_mynode,ptok->credits,ee,nx,nc));
  }
  #endif

  /* Process credit info and prep credit byte for return (if isReq) */
  if (gasnetc_use_flow_control) {
    ptok->credits = gasnetc_credit_update(isReq,ptok->credits,ptok->srcnode,"Med");
  }

  GASNETC_DBGMSG(0,isReq,"M",ptok->srcnode,gasneti_mynode,ghandler,numarg,ptok->args,ptok->msg_bytes,ptok->credits,nbytes,data32,th);

  token = (gasnet_token_t)ptok;
  GASNETI_RUN_HANDLER_MEDIUM(isReq, ghandler, gasnetc_handler[ghandler], token, ptok->args, numarg, data32, nbytes);

  return ptok->need_reply;
}

/* ------------------------------------------------------------------------------------
 * Unpack the data from the event structure and attempt to execute the AM Long handler.
 *
 * This routine will be called from both Request and Reply AMs, isReq=true of Request.
 * In general, AM Longs require two messages, a data payload sent directly to the RAR
 * and a header send to ReqRB (for a Request) or ReqSB (for a Reply).  This function
 * is called when the header arrives.  The data message may or may not have arrived.
 * The last to arrive will execute the requested handler.
 * The first to arrive will cache its metadata in a LID cache that the second can retrieve.
 * AMLong messages with small data payloads may have the payload packed with the header
 * message (and no data message).  isPacked=true if a packed message.
 *
 * This function is called from exec_am_header on arrival of an AM Long
 * Returns TRUE if an implicit Reply must be sent
 * --------------------------------------------------------------------------------- */
static int exec_amlong_header(gasnetc_ptl_token_t *ptok, int isPacked,
                              uint32_t *data32, int numarg, int ghandler, int len_or_lid)
{
  gasnet_token_t token;
  int      ran_handler = 1; /* assume the best */
  int      isReq = ptok->need_reply;
#if GASNET_DEBUG || GASNETI_STATS_OR_TRACE
  gasnetc_threaddata_t *th = gasnetc_mythread();
#endif

  GASNETC_DEF_HARGS();     /* debug, must be first statement */
  GASNETC_GET_SEQNO(ptok); /* debug */

  /* Long Data Formats:
   *     Regular Request Format: HD=[srcnode,cred]     data=[args][seqno][pad]
   *     Regular Reply   Format: HD=[srcnode,lid:cred] data=[args][seqno]
   *             Packed  Format: HD=[srcnode,len:cred] data=[args][seqno][destaddr][data]
   * NOTE: seqno only in debug mode
   */

  #if GASNETI_STATS_OR_TRACE
  {
    uint8_t ee,nx,nc;
    GASNETC_READ_CREDIT_BYTE(ptok->credits,ee,nx,nc);
    GASNETI_TRACE_PRINTF(C,("CREDINFO: Long%s %s s=%d r=%d cred_byte=%x %d:%d:%d",(isPacked?"Packed":""),(isReq?"Req":"Rpl"),ptok->srcnode,gasneti_mynode,ptok->credits,ee,nx,nc));
  }
  #endif

  if (gasnetc_use_flow_control) {
    /* In case of Request:  process credit update and store in token field for return.
     * In case of Reply:  ok to process returned credits here.
     */
    ptok->credits = gasnetc_credit_update(isReq,ptok->credits,ptok->srcnode,"Long");
  }

  if (isPacked) {
    
    void    *dest;
    size_t   nbytes = len_or_lid;

    /* extract the data payload destination, should be in local RAR */
    dest = (void*)GASNETI_MAKEWORD(data32[1],data32[0]);
    data32 += 2;
    GASNETC_MSGLEN_ADD(ptok->msg_bytes, 2*sizeof(uint32_t));
      
    /* copy the data payload to the specified destination */
    gasneti_assert(gasnetc_in_local_rar(dest,nbytes));
    memcpy(dest,data32,nbytes);
    GASNETC_MSGLEN_ADD(ptok->msg_bytes, nbytes);
    if (isReq) GASNETC_MSGLEN_PAD(ptok->msg_bytes);

    GASNETC_DBGMSG(0,isReq,"L",ptok->srcnode,gasneti_mynode,ghandler,numarg,ptok->args,ptok->msg_bytes,ptok->credits,nbytes,dest,th);
    token = (gasnet_token_t)ptok;
    GASNETI_RUN_HANDLER_LONG(isReq, ghandler, gasnetc_handler[ghandler], token, ptok->args, numarg, dest, nbytes);
  } else {

    /* called from Header packet, but not a packed message, check if data message has arrived */
    gasnetc_amlongcache_t *p;
    uint32_t lid;

    if (isReq) {
      lid = ptok->initiator_offset;
      /* message length accounting */
      GASNETC_MSGLEN_PAD(ptok->msg_bytes);
    } else {
      lid = len_or_lid;
    }

    /* Check lid is in proper Request or Reply space */
    gasneti_assert((lid & (1<<23)) == (!isReq << 23));

    p = get_lid_obj_from_header(ptok->srcnode, lid, ptok);
    if (p) {
      /* data has arrived, run handler */

      GASNETC_DBGMSG(0,isReq,"L",ptok->srcnode,gasneti_mynode,ghandler,numarg,ptok->args,ptok->msg_bytes,ptok->credits,p->nbytes,p->data,th);
      GASNETI_TRACE_PRINTF(C,("exec_amlong_header, second to arrive: isReq=%d, numarg=%d, hndlr=%d, nbytes=%d",isReq,numarg,ghandler,(int)p->nbytes));
      token = (gasnet_token_t)ptok;
      GASNETI_RUN_HANDLER_LONG(isReq, ghandler ,gasnetc_handler[ghandler], token, ptok->args, numarg, p->data, p->nbytes);

      /* free the lid object, it has already been removed from the list */
      gasneti_lifo_push(&gasnetc_lid_freelist, p);
    } else {

      /* first to arrive, cant run handler */
      GASNETI_TRACE_PRINTF(C,("exec_amlong_header, first to arrive: isReq=%d, numarg=%d, hndlr=%d",isReq,numarg,ghandler));
      ran_handler = 0;
    }
  }

  return ran_handler && ptok->need_reply;
}

/* ------------------------------------------------------------------------------------
 * Unpack the data from the event structure and attempt to execute the AM handler.
 * --------------------------------------------------------------------------------- */
GASNETI_INLINE(exec_am_header)
void exec_am_header(int isReq, ptl_match_bits_t mbits, ptl_event_t *ev)
{
  uint8_t numarg, ghandler, amflag;
  gasnetc_ptl_token_t tok;
  int argcnt, need_reply;
  uint32_t *data32;
  uint32_t other;

#if GASNET_DEBUG
  gasnetc_threaddata_t *th = gasnetc_mythread();

  gasneti_assert(th->rplsb || !isReq);

  GASNETC_ZERO_AMARGS(tok.args);
  tok.msg_bytes = 0;
#endif

  gasneti_assert(ev->mlength == ev->rlength);

  tok.need_reply = isReq;
  tok.initiator = ev->initiator;
  tok.initiator_offset = GASNETI_HIWORD(mbits); /* only used if isReq, but no need to branch */

  GASNETC_GET_AM_LOWBITS(mbits, numarg, ghandler, amflag);

  /* Common Format for all AM categories
   *     HD=[srcnode,other:cred] data=[args][seqno]...
   * Where "other" is 24 bits and varies with category/format
   */

  /* unpack hdr_data */
  tok.srcnode = (gasnet_node_t)GASNETI_HIWORD(ev->hdr_data);
  tok.credits = (uint8_t)ev->hdr_data;
  other = GASNETI_LOWORD(ev->hdr_data) >> 8;

  /* set data pointer and verify alignment */
  data32 = (uint32_t*)((uintptr_t)ev->md.start + ev->offset);
  gasnetc_assert_aligned(data32,GASNETI_MEDBUF_ALIGNMENT);

  /* unpack args from message payload */
  for (argcnt = 0; argcnt < numarg; argcnt++) {
    tok.args[argcnt] = *(data32++);
    GASNETC_MSGLEN_ADD(tok.msg_bytes, sizeof(uint32_t));
  }
  GASNETC_EXTRACT_SEQNO(data32,tok);      /* debug */

  if (amflag & GASNETC_PTL_AM_SHORT) {
    gasneti_assert(other == 0);
    need_reply = exec_amshort_handler(&tok,data32,numarg,ghandler);
  } else if (amflag & GASNETC_PTL_AM_MEDIUM) {
    need_reply = exec_ammedium_handler(&tok,data32,numarg,ghandler,other);
  } else if (amflag & GASNETC_PTL_AM_LONG) {
    int is_packed = amflag & GASNETC_PTL_AM_PACKED;
    need_reply = exec_amlong_header(&tok,is_packed,data32,numarg,ghandler,other);
  } else {
    gasneti_fatalerror("Invalid amflag from mbits = %lx",(uint64_t)mbits);
  }

  gasneti_assert(tok.msg_bytes == ev->rlength);
  gasneti_assert(tok.msg_bytes <= GASNETC_CHUNKSIZE);

  if (isReq && need_reply) { /* isReq is redundent, but helps the optimizer drop for Reply */
    gasneti_assert(tok.need_reply);
    GASNETI_SAFE(
		 SHORT_REP(0,0, ((gasnet_token_t)&tok , gasneti_handleridx(gasnetc_noop_reph)) )
		 );
  }
}

/* ------------------------------------------------------------------------------------
 * Unpack the data from the event structure and attempt to execute the AM Long handler.
 * 
 * This routine will be called from both Request and Reply AMs, isReq=true of Request.
 * In general, AM Longs require two messages, a data payload sent directly to the RAR
 * and a header send to ReqRB (for a Request) or ReqSB (for a Reply).  This function
 * is called when the data message arrives.  The header message may or may not have arrived.
 * The last to arrive will execute the requested handler.
 * The first to arrive will cache its metadata in a LID cache that the second can retrieve.
 * This function is called from:
 * - RARAM_event:  in response to a Request AMLong data packet arrival.
 * - RARSRC_event: in response to a Reply AMLong data packet arrival.
 * --------------------------------------------------------------------------------- */
static void exec_amlong_data(int isReq, ptl_event_t *ev)
{
  uint8_t* dataaddr = (uint8_t*)ev->md.start + ev->offset;
  size_t   datalen = ev->mlength;
  gasnetc_amlongcache_t *p;
#if GASNET_DEBUG || GASNETI_STATS_OR_TRACE
  gasnetc_threaddata_t *th = gasnetc_mythread();
#endif

  GASNETC_DEF_HARGS();

  gasneti_assert(th->rplsb || !isReq);

  /* see if header message has arrived */
  p = get_lid_obj_from_data(ev->hdr_data, dataaddr, datalen);
  if (p) {
    /* Header has arrived, run handler */
    gasnet_token_t token = (gasnet_token_t)&p->tok;

    /* extract numarg and ghandler from match bits */
    int numarg = (ev->match_bits >> 56);
    gasnet_handler_t ghandler = (ev->match_bits >> 48) & 0xFF;

    GASNETC_DBGMSG(0,isReq,"L",p->tok.srcnode,gasneti_mynode,ghandler,numarg,p->tok.args,p->tok.msg_bytes,p->tok.credits,datalen,dataaddr,th);
    GASNETI_TRACE_PRINTF(C,("exec_amlong_data, second to arrive, running handler isReq=%d, lid=%d",isReq,p->dest_lid));
    GASNETI_RUN_HANDLER_LONG(isReq, ghandler ,gasnetc_handler[ghandler], token, p->tok.args, numarg, dataaddr, datalen);

    if (p->tok.need_reply) {
      /* must always issue a reply to dealloc ReqSB chunk.  If GASNet handler did
       * not reply, we reply here with a short no-op */
      GASNETI_TRACE_PRINTF(C,("exec_amlong_data, sending noop reply"));
      GASNETI_SAFE(
		   SHORT_REP(0,0, (token , gasneti_handleridx(gasnetc_noop_reph)) )
		   );
    } 

    /* free the lid object, it has already been removed from the list */
    gasneti_lifo_push(&gasnetc_lid_freelist, p);

  } else {
    GASNETI_TRACE_PRINTF(C,("exec_amlong_data, first to arrive, isReq=%d, lid=%d",isReq,GASNETI_LOWORD(ev->hdr_data)));
  }
}

/* ------------------------------------------------------------------------------------ */

#if HAVE_MMAP
  GASNETI_INLINE(gasnetc_malloc_aligned)
  void *gasnetc_malloc_aligned(size_t alignment, size_t nbytes) {
    void *result = gasneti_mmap(GASNETI_PAGE_ALIGNUP(nbytes));
    gasneti_assert_always(result != MAP_FAILED);
    gasneti_assert(alignment <= GASNET_PAGESIZE);
    return result;
  }
  GASNETI_INLINE(gasnetc_free_aligned)
  void gasnetc_free_aligned(void *addr, size_t nbytes) {
    gasneti_munmap(addr, GASNETI_PAGE_ALIGNUP(nbytes));
  }
#else
  #define gasnetc_malloc_aligned(_alignment,_nbytes) \
    gasneti_malloc_aligned(_alignment,_nbytes)
  #define gasnetc_free_aligned(_addr,_nbytes) \
    gasneti_free_aligned(_addr)
#endif

/* ------------------------------------------------------------------------------------
 * Initialize a buffer with the given size and address.
 * This buffer will NOT be managed by a chunk allocator
 * --------------------------------------------------------------------------------- */
static void gasnetc_buf_init(gasnetc_PtlBuffer_t *buf, const char *name, size_t nbytes, void *addr)
{
  GASNETI_TRACE_PRINTF(C,("gasnetc_buf_init for %s with length %lu at %p",name,(unsigned long)nbytes,addr));
  gasneti_assert((addr && nbytes) || (!addr && !nbytes));
  buf->name = gasneti_strdup(name);
  buf->nbytes = nbytes;
  buf->actual_start = buf->start = addr;
  buf->use_chunks = 0;
}

/* ------------------------------------------------------------------------------------
 * Trivial chunk allocator for bounce buffer and Msg Send buffers.
 * (1) Just a simple mutex-protected freelist implementation.
 * (2) What we really should have is an efficient buddy-buffer implementation so that
 *     small messages don't have to allocate a full KB.  Concern that this will be expensive
 *     and even more expensive in multi-threaded environment (larger critical section).
 * --------------------------------------------------------------------------------- */
static void gasnetc_chunk_init(gasnetc_PtlBuffer_t *buf, const char *name, size_t nchunks)
{
  int i;
  size_t nbytes = nchunks * GASNETC_CHUNKSIZE;
  void **p;

  GASNETI_TRACE_PRINTF(C,("gasnetc_chunk_init for %s with %lu chunks",name,(ulong)nchunks));
  buf->name = gasneti_strdup(name);
  buf->nbytes = nbytes;
  buf->actual_start = buf->start = gasnetc_malloc_aligned(GASNETC_CHUNKSIZE,nbytes);
  buf->use_chunks = 1;
#if GASNETI_STATS_OR_TRACE
  gasneti_mutex_init(&buf->lock);
  buf->numchunks = nchunks;
  buf->inuse = 0;
  buf->hwm = 0;
#endif

  GASNETI_TRACE_PRINTF(C,("CHUNK_INIT: %s nchunks=%i, nbytes=%i, start=0x%p",name,(int)nchunks,(int)nbytes,buf->start));

  p = (void **)buf->start;
  if (p == NULL) {
    gasneti_fatalerror("failed to alloc %i bytes for chunk allocator %s at %s",(int)nbytes,name,gasneti_current_loc);
  }
  gasneti_lifo_init(&buf->freelist);
  for (i = 0; i < nchunks-1; i++) {
    void *next = (void*)((uintptr_t)p + GASNETC_CHUNKSIZE);
    *p = next;
    p = (void**)next;
  }
  gasneti_lifo_push_many(&buf->freelist, buf->start, p);
}

/* ---------------------------------------------------------------------------------
 * delete the allocator memory.
 * --------------------------------------------------------------------------------- */
static void gasnetc_buf_free(gasnetc_PtlBuffer_t *buf)
{
  gasneti_free(buf->name);
  if (buf->actual_start != NULL) {
    gasnetc_free_aligned(buf->actual_start, buf->nbytes);
  }
  buf->start = buf->actual_start = NULL;
  buf->nbytes = 0;

  /* keep name and stats for trace_finish call */
}

/* ---------------------------------------------------------------------------------
 * Attach the MD for a ReqRB buffer
 * --------------------------------------------------------------------------------- */
static void ReqRB_attach(gasnetc_PtlBuffer_t *p)
{
  ptl_md_t md;

  md.start = p->start;
  md.length = p->nbytes;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = GASNETC_CHUNKSIZE;
  md.options = PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE | PTL_MD_MAX_SIZE;
#if GASNETC_REQRB_AUTO_UNLINK
  /* NOTE: these flags are Cray extensions to the spec */
  md.options |= PTL_MD_FLAG_AUTO_UNLINK | PTL_MD_EVENT_AUTO_UNLINK_ENABLE;
#endif

#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uintptr_t)GASNETC_REQRB_MD;
#else
  md.user_ptr = (void*)ReqRB_event;
#endif
  md.eq_handle = gasnetc_AM_EQ->eq_h;

#if GASNET_PAR
  gasneti_assert(!GASNETC_REQRB_BUSY(p));
  p->fresh = 1;
#endif

  GASNETC_PTLSAFE(PtlMEInsert(gasnetc_CB.me_h, gasnetc_any_id,
                              GASNETC_PTL_REQRB_BITS, GASNETC_PTL_IGNORE_BITS,
                              PTL_UNLINK, PTL_INS_BEFORE, &p->me_h));
  GASNETC_PTLSAFE(PtlMDAttach(p->me_h, md, PTL_UNLINK, &p->md_h));
}

/* ---------------------------------------------------------------------------------
 * "Find" the ReqRB with a memory starting address of start_addr
 * --------------------------------------------------------------------------------- */
GASNETI_ALWAYS_INLINE(ReqRB_getbuf) GASNETI_CONST
gasnetc_PtlBuffer_t* ReqRB_getbuf(uintptr_t start_addr)
{
  const size_t skip = GASNETI_ALIGNUP(sizeof(gasnetc_PtlBuffer_t),GASNETI_MEDBUF_ALIGNMENT);
  gasnetc_PtlBuffer_t *result = (gasnetc_PtlBuffer_t *)(start_addr - skip);
  gasneti_assert((uintptr_t)result->start == start_addr);
  return result;
}

/* ---------------------------------------------------------------------------------
 * This function is called when a Request Receive Buffer needs to be refreshed
 * and placed on the match list just before the catch-basin buffer.
 * --------------------------------------------------------------------------------- */
static void ReqRB_refresh(gasnetc_PtlBuffer_t *p)
{
  gasneti_assert(p);

  GASNETI_TRACE_PRINTF(C,("ReqRB_refresh called with start address %p",p->start));
#if GASNETC_DEBUG_RB_VERBOSE
  printf("[%d] ReqRB_refresh buffer %s at start address %lx\n",gasneti_mynode,p->name,p->start); fflush(stdout);
#endif

#if GASNET_PAR
  /* must wait until all other threads have completed work in this buffer before
   * re-threading back onto ME list.  
   */
  if_pf (GASNETC_REQRB_BUSY(p)) {
    GASNETC_TRACE_WAIT_BEGIN();
    gasneti_waitwhile(GASNETC_REQRB_BUSY(p));
    GASNETC_TRACE_WAIT_END(REFRESH_STALL);
  }
#endif

  ReqRB_attach(p);

#if GASNET_PAR
  gasneti_weakatomic_increment(&gasnetc_spare_ReqRB, 0);
#endif
}

/* ---------------------------------------------------------------------------------
 * Handle events on the local RARAM Memory Descriptor
 * Used as :
 *   - dest of AM_Long data segment.
 * Events:
 *   PUT_END => Data portion of AM_Long arrived.  Extract LID and see if header has
 *              also arrived.  If so, call GASNet handler, else insert LID in LID table.
 * --------------------------------------------------------------------------------- */
static void RARAM_event(ptl_event_t *ev)
{
  ptl_size_t offset;
  ptl_match_bits_t mbits;
  uint8_t msg_type;
  ptl_match_bits_t amflag;
  int isReq;

  gasneti_mutex_unlock(&gasnetc_AM_EQ->lock);

  offset = ev->offset;
  mbits = ev->match_bits;
  amflag = ((mbits & GASNETC_SELECT_BYTE1) >> 8);
  isReq = (amflag & GASNETC_PTL_AM_REQUEST);
  msg_type = GASNETC_GET_MSG_TYPE(mbits);

  GASNETI_TRACE_PRINTF(C,("RARAM event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x, amflag=%x",ptl_event_str[ev->type],(int)offset,(uint64_t)mbits,msg_type,(uint32_t)amflag));

  /* extract the lower bits based on message type */
  gasneti_assert(msg_type & GASNETC_PTL_MSG_AMDATA);

  gasneti_assert(!(amflag & GASNETC_PTL_AM_PACKED));

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  switch (ev->type) {

  case PTL_EVENT_PUT_END:
    /* Must be data packet of AM Long Request */
    gasneti_assert( isReq );
    exec_amlong_data(1, ev);
    break;

  default:
    gasneti_fatalerror("Invalid event %s on RARAM",ptl_event_str[ev->type]);
  }
}

/* ---------------------------------------------------------------------------------
 * Handle events on the local RARSRC Memory Descriptor
 * Used as :
 *   - source of GASNet Puts or dest of Gets when data region happens to lie in RAR.
 *   - source of AM_Long Request messages when data region happens to lie in RAR.
 *   - dest of AM_Long Reply messages
 * Events:
 *  SEND_END => Put (or Get) local completion
 *       ACK => GASNet Put completed remotely.  Mark operation as complete.
 *              NOTE: AMs dont request ACKs
 * REPLY_END => GASNet Get completed.  Mark operation as complete.
 * PUT_END   => AM Long Reply Data message has arrived, may execute handler.
 * --------------------------------------------------------------------------------- */
static void RARSRC_event(ptl_event_t *ev)
{
  ptl_match_bits_t   mbits = ev->match_bits;
  uint8_t msg_type;

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("RARSRC event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)ev->offset,(uint64_t)mbits,msg_type));

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  switch (ev->type) {
  case PTL_EVENT_SEND_END:
    /* InSegment Put (from local RAR) */
    if ( !(msg_type & GASNETC_PTL_MSG_GET) )
      gasnetc_return_ticket(&gasnetc_send_tickets);
    if ((msg_type & GASNETC_PTL_MSG_PUT) && (msg_type & GASNETC_PTL_MSG_DOLC)) {
      gasnete_threaddata_t *th = gasnete_mbits2td(mbits);
      gasneti_weakatomic_decrement(&(th->local_completion_count), 0);
    } else if (msg_type & GASNETC_PTL_MSG_AMDATA) {
      ptl_match_bits_t amflag = (mbits & GASNETC_SELECT_BYTE1) >> 8;
      if (amflag & GASNETC_PTL_AM_SYNC) {
	gasnetc_threaddata_t *th = gasnete_mbits2td(mbits)->gasnetc_threaddata;
	/* caller is AMLong (sync, not async), and is waiting for this counter to decrement */
	gasneti_weakatomic_t *counter = (amflag & GASNETC_PTL_AM_REQUEST) ? &th->amlongReq_data_inflight
									  : &th->amlongRep_data_inflight;
	GASNETC_DEC_INFLIGHT(counter);
      }
    }
    break;

  case PTL_EVENT_PUT_END:
    /* Must be a AM Long Reply data message */
    #if GASNET_DEBUG
    {
      ptl_match_bits_t amflag = (mbits & GASNETC_SELECT_BYTE1) >> 8;
      gasneti_assert( msg_type & GASNETC_PTL_MSG_AMDATA);
      gasneti_assert( !( amflag & GASNETC_PTL_AM_REQUEST) );
    }
    #endif
    exec_amlong_data(0, ev);
    break;

  case PTL_EVENT_ACK:
    /* InSegment Put (from local RAR) */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_PUT);
    /* mark the put (isget=0) operation complete */
    gasnete_op_markdone(gasnete_mbits2op(mbits), 0 /* !isget */);
    break;

  case PTL_EVENT_REPLY_END:
    /* InSegment Get (to local RAR) */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_GET);
    gasnetc_return_ticket(&gasnetc_send_tickets);
    /* mark the get (isget=1) operation complete */
    gasnete_op_markdone(gasnete_mbits2op(mbits), 1);
    break;

  default:
    gasneti_fatalerror("Invalid event %s on RARSRC",ptl_event_str[ev->type]);
  }
}

/* ---------------------------------------------------------------------------------
 * Handle events on Temporary Memory Descriptors and/or Firehoses.
 * Used as local md (source of Put or Long; or dest of Get) when not in
 * local RAR and even then only if either firehose is enabled, or if we can't
 * use bounce bufers (either size was too large or no chunks were available).
 *  SEND_END => Put, Get or Long payload completed locally.
 *       ACK => Put completed, mark operation as done and free MD.
 * REPLY_END => Get completed, mark operation done and free MD.
 * --------------------------------------------------------------------------------- */
static void TMPMD_event(ptl_event_t *ev)
{
  ptl_match_bits_t   mbits = ev->match_bits;
  uint8_t msg_type;

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("TMPMD event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)ev->offset,(uint64_t)mbits,msg_type));

#if GASNET_DEBUG
  if (msg_type & GASNETC_PTL_MSG_AM) {
    gasneti_fatalerror("Unexpected AM msg type on TMPMD, mbits = 0x%lx",(uint64_t)mbits);
  }
#endif

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  switch (ev->type) {
  case PTL_EVENT_SEND_END:
    if ( !(msg_type & GASNETC_PTL_MSG_GET) )
      gasnetc_return_ticket(&gasnetc_send_tickets);
    /* Put from TmpMD */
    if ((msg_type & GASNETC_PTL_MSG_PUT) && (msg_type & GASNETC_PTL_MSG_DOLC)) {
      gasnete_threaddata_t *th = gasnete_mbits2td(mbits);
      gasneti_weakatomic_decrement(&(th->local_completion_count), 0);
    } else if (msg_type & GASNETC_PTL_MSG_AMDATA) {
      ptl_match_bits_t amflag = (mbits & GASNETC_SELECT_BYTE1) >> 8;
      if (amflag & GASNETC_PTL_AM_SYNC) {
	gasnetc_threaddata_t *th = gasnete_mbits2td(mbits)->gasnetc_threaddata;
	/* caller is AMLong (sync, not async), and is waiting for this counter to decrement */
	gasneti_weakatomic_t *counter = (amflag & GASNETC_PTL_AM_REQUEST) ? &th->amlongReq_data_inflight
									  : &th->amlongRep_data_inflight;
	GASNETC_DEC_INFLIGHT(counter);
      }
      /* unlink the tmp MD or free the firehose used in the AM Long data put */
      GASNETC_IF_USE_FIREHOSE (
        gasnetc_fh_free((uint16_t)GASNETI_HIWORD(mbits));
      ) else {
        gasnetc_free_tmpmd(ev->md_handle);
      }
    }
    break;

  case PTL_EVENT_ACK:
    /* Put from Firehose or TmpMD */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_PUT);
    GASNETC_IF_USE_FIREHOSE (
      gasnetc_fh_free((uint16_t)GASNETI_HIWORD(mbits));
    ) else {
      gasnetc_free_tmpmd(ev->md_handle);
    }
    /* mark the put (isget=0) operation complete */
    gasnete_op_markdone(gasnete_mbits2op(mbits), 0 /* !isget */);
    break;

  case PTL_EVENT_REPLY_END:
    /* Get into Firehose or TmpMD */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_GET);
    gasnetc_return_ticket(&gasnetc_send_tickets);
    GASNETC_IF_USE_FIREHOSE (
      gasnetc_fh_free((uint16_t)GASNETI_HIWORD(mbits));
    ) else {
      gasnetc_free_tmpmd(ev->md_handle);
    }
    /* mark the get (isget=1) operation complete */
    gasnete_op_markdone(gasnete_mbits2op(mbits), 1);
    break;

  default:
    gasneti_fatalerror("Invalid event %s on TMPMD",ptl_event_str[ev->type]);
  }
}

/* ------------------------------------------------------------------------------------
 * Handle events on one of the Request Send Buffer
 *  SEND_END => Put (and Get) local completion.
 *                - Ignore for AM Request sends
 *                - Ignore for Gets (Cray Portals gens these in violation of spec)
 *                - If non-bulk Put, increment local completion counter and free chunk.
 *       ACK => Put through bounce buffer completed.  
 *                - NOTE: AMs will not generate ACKs.
 * REPLY_END => Get operation completed, data in bounce buffer must be copied
 *              to actual destination, mark op free, free chunk.
 *   PUT_END => Reply AM arrived in same chunk as Request was sent.
 *              Call GASNet handler then free chunk.
 *   GET_END => Catch-basin recovery underway.  Mark source node as in-recovery (NYI).
 * --------------------------------------------------------------------------------- */
static void ReqSB_event(ptl_event_t *ev)
{
  ptl_size_t offset = ev->offset;
  ptl_match_bits_t   mbits = ev->match_bits;
  uint8_t msg_type;
  uint8_t *pdata, *q;
  void *dest;
  ptl_size_t local_offset;

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("ReqSB event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)offset,(uint64_t)mbits,msg_type));

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  switch (ev->type) {
  case PTL_EVENT_SEND_END:
    if ( !(msg_type & GASNETC_PTL_MSG_GET) )
      gasnetc_return_ticket(&gasnetc_send_tickets);
    if (msg_type & GASNETC_PTL_MSG_PUT) {
      /* Put bounced through ReqSB, can free chunk now */
      gasneti_assert(!(msg_type & GASNETC_PTL_MSG_DOLC));
      local_offset = GASNETI_HIWORD(mbits);
      gasnetc_chunk_free(&gasnetc_ReqSB,local_offset);
    }
    break;

  case PTL_EVENT_ACK:
    /* Put bounced through ReqSB, mark op complete */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_PUT);
    /* mark the put (isget=0) operation complete */
    gasnete_op_markdone(gasnete_mbits2op(mbits), 0 /* !isget */);
    break;

  case PTL_EVENT_REPLY_END:
    /* Get bouncing through ReqSB, copy to dest and complete */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_GET);
    gasnetc_return_ticket(&gasnetc_send_tickets);
    local_offset = GASNETI_HIWORD(mbits);
    q = ((uint8_t*)ev->md.start + local_offset);
    pdata = q + sizeof(void*);
    /* q points to location where real destination address is stored */
    dest = *(void**)q;
    memcpy(dest,pdata,ev->mlength);
    /* free the bounce buffer */
    gasnetc_chunk_free(&gasnetc_ReqSB,local_offset);
    /* mark the get (isget=1) operation complete */
    /* Do we need membar here?  Above chunk free required lock/unlock
     * of mutex => membar, right?
     */
    gasnete_op_markdone(gasnete_mbits2op(mbits), 1);
    break;

  case PTL_EVENT_PUT_END:
    /* This is an AM reply from a previous request */
    exec_am_header(0,mbits,ev);

    /* dealloc the chunk */
    gasnetc_chunk_free(&gasnetc_ReqSB,offset);

    break;

  default:
    gasneti_fatalerror("Invalid event %s on ReqSB",ptl_event_str[ev->type]);
  }
}

/* ------------------------------------------------------------------------------------
 * Handle events on one of the Reply Send Buffer
 * SEND_END => reply message sent, free the chunk
 * NOTE: since this is an event from a local action (not an ACK), ev->offset is valid
 * --------------------------------------------------------------------------------- */
static void RplSB_event(ptl_event_t *ev)
{
  ptl_match_bits_t   mbits = ev->match_bits;
  ptl_size_t local_offset = GASNETI_HIWORD(mbits);
  uint8_t msg_type;

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("RplSB event %s offset = %i, loc_offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)ev->offset,(int)local_offset,(uint64_t)mbits,msg_type));

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  /* MLW: debug check */
  gasneti_assert(ev->offset == local_offset);
  
  switch (ev->type) {
  case PTL_EVENT_SEND_END:
    gasnetc_return_ticket(&gasnetc_send_tickets);
    /* reclaim the chunk */
    gasnetc_chunk_free(&gasnetc_RplSB, local_offset);
    break;

  default:
    gasneti_fatalerror("Invalid event %s on RplSB",ptl_event_str[ev->type]);
  }

}

/* ------------------------------------------------------------------------------------
 * Handle events on one of the ReqRB buffers
 * PUT_END => Arrival of new AM Request
 * UNLINK  => spent buffer, recycle and link at end of list
 * --------------------------------------------------------------------------------- */
static void ReqRB_event(ptl_event_t *ev)
{
  ptl_match_bits_t   mbits = ev->match_bits;
  uint8_t msg_type;
  gasnetc_PtlBuffer_t *bufptr = ReqRB_getbuf((uintptr_t)ev->md.start);

#if GASNET_PAR
  /* flow control work */
  if (ev->type == PTL_EVENT_PUT_END) {
    /* increment ref counter on this buffer, atomic w.r.t. poll of the AM_EQ */
    GASNETC_REQRB_START(bufptr);

    /* release AM_EQ mutex so that "fresh stall" can't block unrelated events */
    gasneti_mutex_unlock(&gasnetc_AM_EQ->lock);

    /* stall if too few "fresh" ReqRBs would remain to cover the advertised credits (bug 2462)
     * the ref we hold protects against ReqRB_refresh accessing bufptr->fresh */
    if_pf (bufptr->fresh) {
      gasneti_mutex_lock(&bufptr->lock);
      if (bufptr->fresh) {
        GASNETI_TRACE_EVENT(C, FRESH_REQRB);
        if_pf (!gasneti_weakatomic_read(&gasnetc_spare_ReqRB, 0)) {
          GASNETC_TRACE_WAIT_BEGIN();
          gasneti_waituntil(gasneti_weakatomic_read(&gasnetc_spare_ReqRB, 0));
          GASNETC_TRACE_WAIT_END(FRESH_STALL);
        }
        gasneti_weakatomic_decrement(&gasnetc_spare_ReqRB, 0);
        bufptr->fresh = 0;
      }
      gasneti_mutex_unlock(&bufptr->lock);
    }

    /* on zero-byte payload we don't need to keep a reference */
    if (!ev->mlength) GASNETC_REQRB_FINISH(bufptr);
  } else
#endif
    gasneti_mutex_unlock(&gasnetc_AM_EQ->lock);

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("ReqRB event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)ev->offset,(uint64_t)mbits,msg_type));

  /* extract the lower bits based on message type */
#if GASNET_DEBUG
  if (!(msg_type & GASNETC_PTL_MSG_AM)) {
    gasneti_fatalerror("Invalid event msg type on ReqRB, mbits = 0x%lx",(uint64_t)mbits);
  }
#endif

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  switch (ev->type) {
  case PTL_EVENT_PUT_END:
    exec_am_header(1,mbits,ev);

    /* decrement ref counter on this buffer if we held one */
    if (ev->mlength) GASNETC_REQRB_FINISH(bufptr);

#if GASNETC_REQRB_AUTO_UNLINK
#if GASNETC_REQRB_UNLINK_VERBOSE
    { /* testing */
      ptl_size_t space_left = ev->md.length - (ev->offset + ev->mlength);
      if (space_left < GASNETC_CHUNKSIZE) {
	printf("[%d] ReqRB: AUTO_UNLINK and only %ld bytes left in buffer with handle %ld\n",
	       gasneti_mynode,(long)space_left,(ulong)ev->md_handle);
      }
    }
#endif
#else
    #error "!GASNETC_REQRB_AUTO_UNLINK is known to be broken (bug 2461)"
    {
      ptl_size_t space_left = ev->md.length - (ev->offset + ev->mlength);
      if (space_left < GASNETC_CHUNKSIZE) {
	/* attempt an unlink.  If successful, refresh the buffer */
	int rc = PtlMDUnlink(ev->md_handle);
	GASNETI_TRACE_PRINTF(C,("ReqRB_event: manual unlink returned %d",rc));
#if GASNETC_REQRB_UNLINK_VERBOSE
	printf("[%d] ReqRB: MANUAL_UNLINK and only %ld bytes left in buffer with handle %ld\n",
	       gasneti_mynode,(long)space_left,(ulong)ev->md_handle);
#endif
	switch (rc) {
	case PTL_OK:
	case PTL_MD_INVALID:
	  /* MLW: implementation error: spec does not even list this
	   * as possible return code for PtlMDUnlink, but it seems to indicate
	   * the MD has been unlinked, so refresh it */
	  /* put it back on the end of the list */
	  /* printf("[%d] Manual Unlink of ReqRB with handle %lu, rc=%d\n",gasneti_mynode,(ulong)ev->md_handle,rc); */
	  ReqRB_refresh(bufptr);
	  break;
	case PTL_MD_IN_USE:
	  /* do nothing, will unlink later */
	  break;
	default:
	  gasneti_fatalerror("ReqRB_event: unlink attempt returned error %d",rc);
	}
      }
    }
#endif
    break;

  case PTL_EVENT_UNLINK:
    /* buffer was auto-unlinked, refresh and relink at end of buffer list. */

#if GASNETC_REQRB_UNLINK_VERBOSE
    printf("[%d] Got Unlink event of ReqRB with handle %lu\n",gasneti_mynode,(ulong)ev->md_handle);
#endif
    ReqRB_refresh(bufptr);
    break;

  default:
    gasneti_fatalerror("Invalid event %s on ReqRB",ptl_event_str[ev->type]);
  }
}

/* ------------------------------------------------------------------------------------
 * Handle events on the Catch-Basin memory descriptor.
 * - PUT_END => dropped AM Request
 * --------------------------------------------------------------------------------- */
static void CB_event(ptl_event_t *ev)
{
  ptl_match_bits_t   mbits = ev->match_bits;
  uint8_t msg_type;

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("CB event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)ev->offset,(uint64_t)mbits,msg_type));

  /* extract the lower bits based on message type */
#if GASNET_DEBUG
  if (!(msg_type & GASNETC_PTL_MSG_AM)) {
    gasneti_fatalerror("Invalid event msg type on CB, mbits = 0x%lx",(uint64_t)mbits);
  }
#endif

  /* we always truncate on this MD */
  gasneti_assert(ev->mlength == 0);

  switch (ev->type) {
  case PTL_EVENT_PUT_END:
    gasneti_fatalerror("PUT_END event on CB indicates flow-control failure");
    break;

  default:
    gasneti_fatalerror("Invalid event %s on CB",ptl_event_str[ev->type]);
  }
}

/* ---------------------------------------------------------------------------------
 * Construct the memory descriptors that cover the Remote Access Region.
 * There are three Memory Descriptors that cover this region:
 *   - RAR_MD covers the region but has no event queue.  It is the target of
 *     remote Put and Get operations.
 *   - RARSRC_MD is a free floating MD used in the following cases:
 *     (1) Src of Extended Put when src region lies in local RAR
 *     (2) Dest of Extended Get
 *     (3) Src of AMLong Data Put when src region lies in local RAR.
 *   - RARAM_MD also covers the RAR.  This MD is associated with an event
 *     queue and is used as the target of AMLong Request and Reply Data messages.
 *     Receipt of a PUT_END on this MD causes the execution of an AMLong handler.
 * Both RAR_MD and RARAM_MD are linked on the GASNETC_RAR_PTE portals table entry list.
 * --------------------------------------------------------------------------------- */
static void RAR_init(void)
{
  ptl_md_t md;
  void* rar_start   = gasneti_seginfo[gasneti_mynode].addr;
  size_t rar_len    = gasneti_seginfo[gasneti_mynode].size;

  GASNETI_TRACE_PRINTF(C,("RAR_init with len = %lu at %p",(unsigned long)rar_len,rar_start));

  gasnetc_RAR.start = rar_start;
  gasnetc_RAR.nbytes = rar_len;
  gasnetc_RAR.actual_start = NULL;      /* this gets lost in gasneti_segmentattach, so cant free */
  gasnetc_RAR.name = gasneti_strdup("RAR");
  gasnetc_RAR.use_chunks = 0;

  /* Insert a MLE at the head of the list */
  GASNETC_PTLSAFE(PtlMEAttach(gasnetc_ni_h, GASNETC_PTL_RAR_PTE, gasnetc_any_id, GASNETC_PTL_RAR_BITS,
			      GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_BEFORE, &gasnetc_RAR.me_h));

  /* The RAR does not generate events, but will produce ACKs */
  md.start = rar_start;
  md.length = rar_len;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_OP_PUT | PTL_MD_OP_GET | PTL_MD_MANAGE_REMOTE |
    PTL_MD_EVENT_START_DISABLE | PTL_MD_EVENT_END_DISABLE;
  md.user_ptr = 0;
  md.eq_handle = PTL_EQ_NONE;

  /* Attach this as first Match-list entry in portals table at index RAR_PTE */
  GASNETC_PTLSAFE(PtlMDAttach(gasnetc_RAR.me_h, md, PTL_RETAIN, &gasnetc_RAR.md_h));

  /* We create another md to cover the same RAR region but this one will have
   * receive AM Long Request Data messages and be on the AM_EQ event queue.
   */
  gasnetc_RARAM.start = rar_start;
  gasnetc_RARAM.nbytes = rar_len;
  gasnetc_RARAM.actual_start = NULL;      /* this gets lost in gasneti_segmentattach, so cant free */
  gasnetc_RARAM.name = gasneti_strdup("RARAM");
  gasnetc_RARAM.use_chunks = 0;

  md.start = rar_start;
  md.length = rar_len;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_OP_PUT | PTL_MD_MANAGE_REMOTE | PTL_MD_EVENT_START_DISABLE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uintptr_t)GASNETC_RARAM_MD;
#else
  md.user_ptr = (void*)RARAM_event;
#endif
  md.eq_handle = gasnetc_AM_EQ->eq_h;

  GASNETC_PTLSAFE(PtlMEInsert(gasnetc_RAR.me_h, gasnetc_any_id, GASNETC_PTL_RARAM_BITS,
			      GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_AFTER,
			      &gasnetc_RARAM.me_h));
  GASNETC_PTLSAFE(PtlMDAttach(gasnetc_RARAM.me_h, md, PTL_RETAIN, &gasnetc_RARAM.md_h));

  /* We create a third md to cover the same RAR region but this one will 
   * be used in the following cases:
   *   - Source of Put or AM Long Request/Reply when data happens to lie in local RAR
   *   - Dest of Get when happens to lie in local RAR
   *   - Destination of AM Long Reply Data message.
   * All events on this MD will not consume additional resources, and will serve to
   * reclaim resources, and so it is put on the SAFE_EQ.
   */
  gasnetc_RARSRC.start = rar_start;
  gasnetc_RARSRC.nbytes = rar_len;
  gasnetc_RARSRC.actual_start = NULL;      /* this gets lost in gasneti_segmentattach, so cant free */
  gasnetc_RARSRC.name = gasneti_strdup("RARSRC");
  gasnetc_RARSRC.use_chunks = 0;

  md.start = rar_start;
  md.length = rar_len;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_OP_PUT | PTL_MD_MANAGE_REMOTE | PTL_MD_EVENT_START_DISABLE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uintptr_t)GASNETC_RARSRC_MD;
#else
  md.user_ptr = (void*)RARSRC_event;
#endif
  md.eq_handle = gasnetc_SAFE_EQ->eq_h;

  GASNETC_PTLSAFE(PtlMEInsert(gasnetc_RARAM.me_h, gasnetc_any_id, GASNETC_PTL_RARSRC_BITS,
			      GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_AFTER,
			      &gasnetc_RARSRC.me_h));
  GASNETC_PTLSAFE(PtlMDAttach(gasnetc_RARSRC.me_h, md, PTL_RETAIN, &gasnetc_RARSRC.md_h));
}

/* ------------------------------------------------------------------------------------
 * Remove the memory descriptors associated with the Remote Access Region, but dont
 * deallocate the memory.
 * --------------------------------------------------------------------------------- */
static void RAR_exit(void)
{
  /* these will automatically unlink the match-list entries as well */
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_RAR.md_h));
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_RARAM.md_h));
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_RARSRC.md_h));
}

/* ---------------------------------------------------------------------------------
 * Construct the Reply Send Buffer.
 * Replies to AM Requests are issued from this buffer.  It is never the target of
 * a remote operation so it is allocated a free-floating memory descriptor.
 * --------------------------------------------------------------------------------- */
static void RplSB_init(void)
{
  ptl_md_t md;

  gasnetc_chunk_init(&gasnetc_RplSB, "RplSB", gasnetc_RplSB_numchunk);

  md.start = gasnetc_RplSB.start;
  md.length = gasnetc_RplSB.nbytes;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_EVENT_START_DISABLE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uintptr_t)GASNETC_REQSB_MD;
#else
  md.user_ptr = (void*)RplSB_event;
#endif
  md.eq_handle = gasnetc_SAFE_EQ->eq_h;

  GASNETC_PTLSAFE(PtlMDBind(gasnetc_ni_h, md, PTL_RETAIN, &gasnetc_RplSB.md_h));
  GASNETI_TRACE_PRINTF(C,("RplSB_init: %s %lu chunks md=%lu",gasnetc_RplSB.name,(ulong)gasnetc_RplSB_numchunk,(ulong)gasnetc_RplSB.md_h));

}

/* ------------------------------------------------------------------------------------
 * Clean up the Reply Send Buffer.  Unlink and delete the memory.
 * --------------------------------------------------------------------------------- */
static void RplSB_exit(void)
{
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_RplSB.md_h));
  gasnetc_buf_free(&gasnetc_RplSB);
}

/* ------------------------------------------------------------------------------------
 * Construct a pool of Request Receive Buffers along with Memory Descriptors for
 * each.  Link the buffers at the head of the GASNERC_AM_PTE portals table entry.
 * This is where AM Request messages will be placed (these are the only unexpected
 * messages in this implementation).  We do not implement any strict flow-control
 * so a flood of AM Requests into a node can overflow these buffers.  They are implemented
 * as a double or triple buffer scheme so that when one buffer it exhausted, it can
 * be reset and added back onto the end of the list.
 * We also allocate the Catch-Basin Memory descriptor here.  It lives at the end of
 * the list and uses the same match-bits as the Receive buffers.  
 * --------------------------------------------------------------------------------- */
static void ReqRB_init(void)
{
  int i;
  ptl_md_t md;
  size_t nbytes = gasnetc_ReqRB_numchunk * GASNETC_CHUNKSIZE;
  char name[32];

  /* First add the Catch-Basin MD */
  gasnetc_buf_init(&gasnetc_CB,"Catch_Basin",0,NULL);
  md.start = NULL;
  md.length = 0;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE | PTL_MD_ACK_DISABLE | PTL_MD_TRUNCATE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uintptr_t)GASNETC_CB_MD;
#else
  md.user_ptr = (void*)CB_event;
#endif
  md.eq_handle = gasnetc_SAFE_EQ->eq_h;
  GASNETC_PTLSAFE(PtlMEAttach(gasnetc_ni_h, GASNETC_PTL_AM_PTE, gasnetc_any_id, GASNETC_PTL_REQRB_BITS, GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_AFTER, &gasnetc_CB.me_h));
  GASNETC_PTLSAFE(PtlMDAttach(gasnetc_CB.me_h, md, PTL_RETAIN, &gasnetc_CB.md_h));

  GASNETI_TRACE_PRINTF(C,("CB_init: %s me=%lu md=%lu",gasnetc_CB.name,(ulong)gasnetc_CB.me_h,(ulong)gasnetc_CB.md_h));

  /* Then add the ReqRB MDs */
  gasnetc_ReqRB = (gasnetc_PtlBuffer_t**)gasneti_malloc(gasnetc_ReqRB_pool_size*sizeof(gasnetc_PtlBuffer_t*));
  for (i = 0; i < gasnetc_ReqRB_pool_size; i++) {
    gasnetc_PtlBuffer_t *p;
    size_t skip = GASNETI_ALIGNUP(sizeof(gasnetc_PtlBuffer_t),GASNETI_MEDBUF_ALIGNMENT);

    sprintf(&name[0],"ReqRB_%02d",i);

    p = gasnetc_ReqRB[i] = gasnetc_malloc_aligned(GASNETI_MEDBUF_ALIGNMENT,nbytes + skip);
    gasnetc_buf_init(p,name,nbytes,(void *)((uintptr_t)p + skip));
#if GASNET_PAR
    gasneti_weakatomic_set(&p->threads_active, 0, 0);
    gasneti_mutex_init(&p->lock);
#endif

    ReqRB_attach(p);

    GASNETI_TRACE_PRINTF(C,("ReqRB_init[%d]: %s %lu bytes me=%lu md=%lu",i,p->name,(ulong)nbytes,(ulong)p->me_h,(ulong)p->md_h));

#if GASNETC_DEBUG_RB_VERBOSE
    printf("[%d] ReqRB_Init: buffer %s at start address %lx\n",gasneti_mynode,p->name,(uintptr_t)p->start); fflush(stdout);
#endif
  }
}

/* ---------------------------------------------------------------------------------
 * Cleanup Request Receive Buffer resources.
 * --------------------------------------------------------------------------------- */
static void ReqRB_exit(void)
{
  int i;

  for (i = 0; i < gasnetc_ReqRB_pool_size; i++) {
    /* This should also unlink the associated MEs */
    GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_ReqRB[i]->md_h));
    gasnetc_buf_free(gasnetc_ReqRB[i]);
  }
  gasneti_free(gasnetc_ReqRB);

  /* no data buffer for the CB */
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_CB.md_h));
}

/* ---------------------------------------------------------------------------------
 * Allocate the Request Send Buffer (also used as a Bounce Buffer) and
 * Construct a Memory Descriptor for it.
 * Note:  Current Implementation is for free-floating MD.  Must put this on
 *        Match-list for AM Replys and for Catch-Basin algorithm to work.  
 *        Put it on GASNETC_AM_PTE table entry.
 * --------------------------------------------------------------------------------- */
static void ReqSB_init(void)
{
  ptl_md_t md;
  gasnetc_PtlBuffer_t *p = &gasnetc_ReqSB;

  gasnetc_chunk_init(p, "ReqSB", gasnetc_ReqSB_numchunk);
  gasneti_assert(gasnetc_ReqSB.nbytes - GASNETC_CHUNKSIZE < 0x7FFFFF); /* limit offset to 23-bits for lid */

  /* construct a memory descriptor for the Request Send Buffer and attach to AM PTE */
  md.start = gasnetc_ReqSB.start;
  md.length = gasnetc_ReqSB.nbytes;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_EVENT_START_DISABLE | PTL_MD_OP_PUT | PTL_MD_MANAGE_REMOTE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uintptr_t)GASNETC_REQSB_MD;
#else
  md.user_ptr = (void*)ReqSB_event;
#endif
  md.eq_handle = gasnetc_SAFE_EQ->eq_h;

  /* Insert this after the Catch-Basin ME entry (at end of list) */
  GASNETC_PTLSAFE(PtlMEInsert(gasnetc_CB.me_h, gasnetc_any_id, GASNETC_PTL_REQSB_BITS, GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_AFTER, &p->me_h));
  GASNETC_PTLSAFE(PtlMDAttach(p->me_h, md, PTL_UNLINK, &p->md_h ));

  GASNETI_TRACE_PRINTF(C,("ReqSB_init: %s %lu chunks me=%lu md=%lu",p->name,(ulong)gasnetc_ReqSB_numchunk,(ulong)p->me_h,(ulong)p->md_h));

}

/* ---------------------------------------------------------------------------------
 * Cleanup Request Send Buffer Resources
 * --------------------------------------------------------------------------------- */
static void ReqSB_exit(void)
{
  /* unlink the MD */
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_ReqSB.md_h));

  /* free the memory */
  gasnetc_buf_free(&gasnetc_ReqSB);
}

#if GASNETC_CREDIT_TESTING
void gasnetc_dump_credits(int epoch_count)
{
  static int first_dump_credits = 1;
  char filename[128];
  FILE *fh;
  int i;
  /* if opening the first time, over-write old file, otherwise append */
  const char *mode = (first_dump_credits ? "w" : "a");
  sprintf(&filename[0],"credits.%03d",gasneti_mynode);
  if ( (fh = fopen(filename,mode)) == NULL) {
    gasneti_fatalerror("Failed to create or append file named %s",filename);
  }
  first_dump_credits = 0;
  fprintf(fh,"Node: %d  Epoch Counter = %d\n",gasneti_mynode,epoch_count);
  fprintf(fh,"%4s %7s %7s %7s %7s %7s %7s %7s %7s",
	  "Node","L_Cred","S_Cred","S_Stall","S_Inuse","S_Max","S_Revok","L_Req","L_Given");
  fprintf(fh," %7s %7s %7s %7s %7s %7s\n",
	  "TS_Stal","TS_Max","TL_Give","TL_Req","TL_Rev","TL_RR");
  for (i = 0; i < gasneti_nodes; i++) {
    gasnetc_conn_t *s = &gasnetc_conn_state[i];
    fprintf(fh,"%4d %7d %7d %7d %7d %7d %7d %7d %7d",
	    i,s->LoanCredits,s->SendCredits,s->SendStalls,s->SendInuse,
	    s->SendMax,s->SendRevoked,s->LoanRequested,s->LoanGiven);
    fprintf(fh," %7d %7d %7d %7d %7d %7d\n",
	    s->SendStalls_tot,s->SendMax_tot,s->LoanGiven_tot,
	    s->LoanRequested_tot,s->LoanRevoked_tot,s->LoanReqRevoke_tot);
  }
  fclose(fh);
}
#endif

/* ---------------------------------------------------------------------------------
 * Credit update code executed on both AM target node and AM Requestor node (when Reply arrives)
 * On AM Target Node:
 *   - Decay SEND vars if remote node as Lender reached their end of epoch
 *   - Attempt to grant additional credits if requested
 *   - Inform Requester if we have reached our end of epoch
 *   - NOTE: return updated cred_byte to be sent to Requester.
 * On original AM Requester node when AM Reply arrives:
 *   - Decay SEND vars if remote node as Lender reached their end of epoch
 *   - update SendInuse variables with reply credits
 *   - update SendCredits variables with any additional credits we have been granted
 * --------------------------------------------------------------------------------- */
uint8_t gasnetc_credit_update(int isReq, uint8_t cred_byte, gasnet_node_t remotenode, const char* name)
{
  uint8_t ncredit, nextra, end_epoch;
  gasnetc_conn_t *state = &gasnetc_conn_state[remotenode];
  int scavenge = 0;

  /* did we just reach an end of epoch? */
  if (isReq && gasnetc_use_dynamic_credits) {
    /* this gets executed exactly once per receipt of an AMRequest */
    int count = gasneti_weakatomic_add(&gasnetc_AMRequest_count,1,0);
    if ((count % gasnetc_epoch_duration) == 0) gasnetc_end_epoch(count);
  }

  /* extract the fields from the cred_byte */
  GASNETC_READ_CREDIT_BYTE(cred_byte,end_epoch,nextra,ncredit);
  if (!gasnetc_use_dynamic_credits) {
    gasneti_assert(end_epoch == 0);
    gasneti_assert(nextra == 0);
  }
  
  GASNETC_LOCK_STATE(state);

  /* if remote node reached end of epoch, decay our SEND vars */
  if (end_epoch) {
    GASNETC_DECAY_SENDVARS(state);
  }

  if (isReq) {  /* excuted when AMRequest arrives on target node */
    uint8_t ee_return = 0;
    uint8_t nextra_return = 0;
    uint8_t ncredit_return = ncredit;
    int     link_to_revoke_list = 0;

    if (nextra > 0) {
      gasneti_assert(gasnetc_use_dynamic_credits);
      /* remote node request additional credits */
      state->LoanRequested++;
#if GASNETC_CREDIT_TESTING
      state->LoanRequested_tot += nextra;
#endif
      if ((state->LoanGiven + nextra <= gasnetc_lender_limit) &&
	  (state->LoanCredits + nextra <= gasnetc_max_cpn) ) {
	/* NOTE: guaranteed to be less than GASNETC_MAX_CREDITS */
	nextra_return = gasneti_semaphore_trydown_n(&gasnetc_banked_credits,nextra);
	state->LoanGiven += nextra_return;    /* this will decay */
	if ((state->LoanCredits == GASNETC_MIN_CREDITS) && nextra_return) link_to_revoke_list=1;
	state->LoanCredits += nextra_return;  /* record total we have given them */
#if GASNETC_CREDIT_TESTING
	state->LoanGiven_tot += nextra_return;
#endif
	if (nextra_return == 0) scavenge = 1;
      }
    }
    /* inform target (original requester) node if we have reached end of our epoch */
    if (gasnetc_use_dynamic_credits && (state->flags & GASNETC_REACHED_EPOCH)) {
      state->flags &= ~GASNETC_REACHED_EPOCH;
      ee_return = 1;
    }
    GASNETC_UNLOCK_STATE(state);
    GASNETC_SET_CREDIT_BYTE(cred_byte, ee_return, nextra_return, ncredit_return);
    GASNETI_TRACE_PRINTF(C,("CREDINFO: %sReq Recv: [P%d->P%d] Got=%d:%d:%d Update=%d:%d:%d lc=%d",name,remotenode,gasneti_mynode,end_epoch,nextra,ncredit,ee_return,nextra_return,ncredit_return,state->LoanCredits));
    if (scavenge) gasnetc_scavenge_for_credits();
    if (link_to_revoke_list) gasnetc_scavenge_list_add(remotenode,0);
    
  } else {
    /* excuted when AMReply arrives on original requestor node */
    gasneti_assert(state->SendInuse >= ncredit);
    state->SendInuse -= ncredit;
    state->SendCredits += nextra;   /* additional credits granted by Lender */
    GASNETC_UNLOCK_STATE(state);
    GASNETI_TRACE_PRINTF(C,("CREDINFO: %sRpl Recv: [P%d->P%d] Got=%d:%d:%d sc=%d su=%d",name,remotenode,gasneti_mynode,end_epoch,nextra,ncredit,state->SendCredits, state->SendInuse));
  }
  return cred_byte;
}

void gasnetc_scavenge_list_add(gasnet_node_t node, int locked)
{
  gasnetc_conn_t *s = &gasnetc_conn_state[node];
  gasneti_assert(gasnetc_use_dynamic_credits);
  if (! locked) gasneti_mutex_lock(&gasnetc_scavenge_lock);
  /* make sure we are not already on the list */
  gasneti_assert(s->link.next == GASNETC_DLL_NULL);
  gasneti_assert(s->link.prev == GASNETC_DLL_NULL);
  {
    gasnet_node_t head = gasnetc_scavenge_list;
    if (head == GASNETC_DLL_NULL) {
      /* empty list */
      s->link.next = s->link.prev = node;
      gasnetc_scavenge_list = node;
    } else {
      /* add to the end of the list */
      gasnet_node_t prev = gasnetc_conn_state[head].link.prev;
      s->link.next = head;
      s->link.prev = prev;
      gasnetc_conn_state[head].link.prev = node;
      gasnetc_conn_state[prev].link.next = node;
    }
  }
  if (!locked) gasneti_mutex_unlock(&gasnetc_scavenge_lock);
#if GASNETC_CREDIT_TESTING
  if (!locked && (gasnetc_debug_node >= 0) && (gasneti_mynode == gasnetc_debug_node)) {
    printf("After adding node %d to Scavenge List:\n",node);
    gasnetc_print_scavenge_list();
  }
#endif
}

void gasnetc_scavenge_list_remove(gasnet_node_t node)
{
  gasnetc_conn_t *s = &gasnetc_conn_state[node];
  gasneti_assert(gasnetc_use_dynamic_credits);
  gasneti_mutex_lock(&gasnetc_scavenge_lock);
  /* make sure we are not already on the list */
  gasneti_assert(s->link.next != GASNETC_DLL_NULL);
  gasneti_assert(s->link.prev != GASNETC_DLL_NULL);
  {
    gasnet_node_t next = s->link.next;
    gasnet_node_t prev = s->link.prev;
    gasneti_assert( gasnetc_scavenge_list != GASNETC_DLL_NULL );
    if (next == node) {
      /* this is the only node in the list */
      gasneti_assert(gasnetc_scavenge_list == node);
      gasnetc_scavenge_list = GASNETC_DLL_NULL;
    } else {
      /* more than one element on the list, make sure list head does not point to us */
      if (gasnetc_scavenge_list == node) gasnetc_scavenge_list = next;
      gasnetc_conn_state[prev].link.next = next;
      gasnetc_conn_state[next].link.prev = prev;
    }
    s->link.next = GASNETC_DLL_NULL;
    s->link.prev = GASNETC_DLL_NULL;
  }
  gasneti_mutex_unlock(&gasnetc_scavenge_lock);
#if GASNETC_CREDIT_TESTING
  if ((gasnetc_debug_node >= 0) && (gasneti_mynode == gasnetc_debug_node)) {
    printf("After removing node %d from Scavenge List:\n",node);
    gasnetc_print_scavenge_list();
  }
#endif
}

void gasnetc_print_scavenge_list(void)
{
  gasneti_assert(gasnetc_use_dynamic_credits); 
  gasneti_mutex_lock(&gasnetc_scavenge_lock);
  {
    gasnet_node_t node = gasnetc_scavenge_list;
    int finished = (node == GASNETC_DLL_NULL);
    printf("SCAVENGE_List[%d]:",gasneti_mynode);
    while (! finished) {
      printf(" %d",node);
      node = gasnetc_conn_state[node].link.next;
      if (node == gasnetc_scavenge_list) finished = 1;
    }
    printf("\n");
  }
  gasneti_mutex_unlock(&gasnetc_scavenge_lock);
}

/* ---------------------------------------------------------------------------------
 * At end of each credit epoch, we 
 * - clear credit flags
 * - set end_of_epoch bit as signal for senders to age their SEND vars.
 * - Age LOAN vars
 * --------------------------------------------------------------------------------- */
void gasnetc_end_epoch(int epoch_count)
{
  int i;
  if (!gasnetc_use_dynamic_credits) return;
  if ( gasneti_mutex_trylock(&gasnetc_epoch_lock) ) {
    /* could not get lock, another thread must still be doing epoch update */
#if GASNET_DEBUG
    fprintf(stderr,"Node [%d]: Epoch lock not available at count %d, skipping...\n",
	    gasneti_mynode,epoch_count);
#endif
    return;
  }
  GASNETI_TRACE_EVENT(C, END_EPOCH);
  if (gasnetc_dump_stats) GASNETC_DUMP_CREDITS(epoch_count);
  for (i = 0; i < gasneti_nodes; i++) {
    gasnetc_conn_t *state = &gasnetc_conn_state[i];
    GASNETC_LOCK_STATE(state);
    state->flags &= ~GASNETC_CREDIT_REVOKE_ZERO_REPLY;
    state->flags |= GASNETC_REACHED_EPOCH;
    GASNETC_CREDIT_DECAY(state->LoanRequested);
    GASNETC_CREDIT_DECAY(state->LoanGiven);
    GASNETC_UNLOCK_STATE(state);
  }
  gasneti_mutex_unlock(&gasnetc_epoch_lock);
}

/* ---------------------------------------------------------------------------------
 * Walk list of nodes, hitting them up for credits to be returned to us
 * Walk in circular list, always re-start from where we left off last time.
 * --------------------------------------------------------------------------------- */
void gasnetc_scavenge_for_credits(void)
{
  /* Simply walk list of remote nodes, requesting that they return credits */
  int node, start;
  int finished = 0;
  int hit = 0;
  int target_hits = gasnetc_num_scavenge - gasneti_weakatomic_read(&gasnetc_scavenge_inflight,0);

  gasneti_assert(gasnetc_use_dynamic_credits);
  if (gasneti_nodes == 1) return;
  target_hits = GASNETC_MAX(target_hits,0);
  if (target_hits == 0) return;

  gasneti_mutex_lock(&gasnetc_scavenge_lock);
  if (gasnetc_scavenge_list == GASNETC_DLL_NULL) finished = 1;  /* empty list */
  start = gasnetc_scavenge_list;
  node = start;
  
  while (!finished) {
    /* should we hit-up this node? */
    gasnetc_conn_t *state = &gasnetc_conn_state[node];
    int epoch_end = 0;
    if (node == gasneti_mynode) goto nextnode;
    if (GASNETC_TRYLOCK_STATE(state)) goto nextnode;
    if (state->flags & GASNETC_SYS_MSG_INFLIGHT) goto unlock;
    /* if we know they only have min credits, dont ask */
    if (state->LoanCredits <= GASNETC_MIN_CREDITS) goto unlock;
    /* We recently asked for additional credits and they did not have any to spare */
    if (state->flags & GASNETC_CREDIT_REVOKE_ZERO_REPLY) goto unlock;
    /* if they recently asked us for more credits, dont bother asking for any back */
    if (state->LoanRequested) goto unlock;
    state->flags |= GASNETC_SYS_MSG_INFLIGHT;
    if (state->flags & GASNETC_REACHED_EPOCH) {
      state->flags &= ~GASNETC_REACHED_EPOCH;
      epoch_end = 1;
    }
#if GASNETC_CREDIT_TESTING
    state->LoanReqRevoke_tot++;
#endif
    gasneti_weakatomic_increment(&gasnetc_scavenge_inflight,0);
    gasnetc_sys_SendMsg(node, GASNETC_SYS_CREDIT_REVOKE, gasneti_mynode, epoch_end, 0);
    hit++;
  unlock:
    GASNETC_UNLOCK_STATE(state);
  nextnode:
    node = gasnetc_conn_state[node].link.next;
    if (node == start) finished = 1;  /* wrapped around to start */
    if (hit >= target_hits) finished = 1;  
  }
  gasnetc_scavenge_list = node;  /* set head of list to where we left off */
  gasneti_mutex_unlock(&gasnetc_scavenge_lock);
}

/* ---------------------------------------------------------------------------------
 * Got a request from this node to revoke or return some is its credits, must reply
 * --------------------------------------------------------------------------------- */
static void process_credit_revoke(gasnet_node_t node, int epoch_end)
{
  gasnetc_conn_t *state = &gasnetc_conn_state[node];
  int give_back = 0;
  int navail;
  gasneti_assert( gasnetc_use_dynamic_credits );
  GASNETC_LOCK_STATE(state);
  if (epoch_end) {
    /* remote node reached epoch end, decay our send vars */
    GASNETC_DECAY_SENDVARS(state);
  }
  if (state->SendStalls) goto send_it;  /* I dont have enough, so cant give any up */
  if (state->SendRevoked >= gasnetc_revoke_limit) goto send_it;
  navail = state->SendCredits - GASNETC_MAX(GASNETC_MIN_CREDITS,state->SendMax);
  give_back = GASNETC_MIN(navail,GASNETC_MAX(0,gasnetc_revoke_limit - state->SendRevoked));
  state->SendCredits -= give_back;
  state->SendRevoked += give_back;
  send_it:
  epoch_end = 0;
  if (state->flags & GASNETC_REACHED_EPOCH) {
    /* we reached our epoch, inform target to decay SEND vars */
    state->flags &= ~GASNETC_REACHED_EPOCH;
    epoch_end = 1;
  }
  GASNETC_UNLOCK_STATE(state);
  gasnetc_sys_SendMsg(node, GASNETC_SYS_CREDIT_RETURN, gasneti_mynode, give_back, epoch_end);
}

/* ---------------------------------------------------------------------------------
 * Got a revoke reply from this node, returning some of (our) LOAN credits
 * --------------------------------------------------------------------------------- */
static void process_credit_return(gasnet_node_t node, int ncredit, int epoch_end)
{
  gasnetc_conn_t *state = &gasnetc_conn_state[node];
  int remove_from_list = 0;
  gasneti_assert( gasnetc_use_dynamic_credits );
  gasneti_weakatomic_decrement(&gasnetc_scavenge_inflight,0);
  GASNETC_LOCK_STATE(state);
  gasneti_assert(state->flags & GASNETC_SYS_MSG_INFLIGHT);
  state->flags &= ~GASNETC_SYS_MSG_INFLIGHT;
  if (epoch_end) {
    /* remote node reached epoch end, decay our send vars */
    GASNETC_DECAY_SENDVARS(state);
  }
  if (ncredit == 0) {
    state->flags |= GASNETC_CREDIT_REVOKE_ZERO_REPLY;
  } else {
    state->flags &= ~GASNETC_CREDIT_REVOKE_ZERO_REPLY;
    state->LoanCredits -= ncredit;  /* number of our credits they have */
    if (state->LoanCredits == GASNETC_MIN_CREDITS) remove_from_list = 1;
#if GASNETC_CREDIT_TESTING
    state->LoanRevoked_tot += ncredit;
#endif

  }
  GASNETC_UNLOCK_STATE(state);
  /* put the returned credits in the bank for distribution at a later time */
  if (ncredit > 0) gasneti_semaphore_up_n(&gasnetc_banked_credits,ncredit);
  if (remove_from_list) gasnetc_scavenge_list_remove(node);
}

/* ---------------------------------------------------------------------------------
 * Figure out which SYS System message to execute
 * --------------------------------------------------------------------------------- */
static void exec_sys_msg(gasnetc_sys_t msg_id, int32_t arg0, int32_t arg1, int32_t arg2)
{
  /*  GASNETI_TRACE_PRINTF(C,("sys_msg %u, arg0=%d, arg1=%d, arg2=%d",(unsigned)msg_id,arg0,arg1,arg2)); */

  switch (msg_id) {
  case GASNETC_SYS_SHUTDOWN_REQUEST:
    {
      uint32_t distance = arg0;
      int exitcode = arg1;
      int oldcode;
    #if GASNET_DEBUG || GASNETI_STATS_OR_TRACE
      int sender = arg2;
      gasneti_assert(((uint32_t)sender + distance) % gasneti_nodes == gasneti_mynode);
      GASNETI_TRACE_PRINTF(C,("Got SHUTDOWN Request from node %d w/ exitcode %d",sender,exitcode));
    #endif
      oldcode = gasneti_atomic_read(&gasnetc_exitcode, 0);
      if (exitcode > oldcode) {
        gasneti_atomic_set(&gasnetc_exitcode, exitcode, 0);
      } else {
        exitcode = oldcode;
      }
      sys_exit_rcvd |= distance;
      if (!gasnetc_shutdownInProgress) {
        gasneti_sighandlerfn_t handler = gasneti_reghandler(SIGQUIT, SIG_IGN);
        if ((handler != gasneti_defaultSignalHandler) &&
#ifdef SIG_HOLD
            (handler != (gasneti_sighandlerfn_t)SIG_HOLD) &&
#endif
            (handler != (gasneti_sighandlerfn_t)SIG_ERR) &&
            (handler != (gasneti_sighandlerfn_t)SIG_IGN) &&
            (handler != (gasneti_sighandlerfn_t)SIG_DFL)) {
          (void)gasneti_reghandler(SIGQUIT, handler);
          raise(SIGQUIT);
        }
        gasnetc_exit(exitcode);
      }
    }
    break;

  case GASNETC_SYS_BARRIER:
    {
      /* barrier notify message - never multithreaded */
      int phase = arg0;
      uint32_t distance = arg1;
    #if GASNET_DEBUG || GASNETI_STATS_OR_TRACE
      int sender = arg2;
      GASNETI_TRACE_PRINTF(C,("Got BARRIER from node %d phase=%d distance=%d",sender,phase,(int)distance));
      gasneti_assert(((uint32_t)sender + distance) % gasneti_nodes == gasneti_mynode);
    #endif
      sys_barrier_rcvd[phase] |= distance;
    }
    break;

  case GASNETC_SYS_CREDIT_REVOKE:
    /* got request to revoke some credits */
    {
      int sender = arg0;
      int epoch_end = arg1;
      process_credit_revoke(sender,epoch_end);
    }
    break;

  case GASNETC_SYS_CREDIT_RETURN:
    /* got reply to my request to revoke credits */
    {
      int sender = arg0;
      int ncredit = arg1;
      int epoch_end = arg2;
      process_credit_return(sender,ncredit,epoch_end);
    }
    break;

  default:
    gasneti_fatalerror("[%d] unknown sys_msg %u, arg0=%d, arg1=%d, arg2=%d",
		       gasneti_mynode, (unsigned)msg_id, arg0, arg1, arg2);
  }
}

/* ---------------------------------------------------------------------------------
 * Process system SYS events
 * --------------------------------------------------------------------------------- */
static void sys_event(ptl_event_t *ev)
{
  ptl_match_bits_t   mbits = ev->match_bits;

  switch (ev->type) {

  case PTL_EVENT_PUT_END:
    /* Must be a system message */
    {
      gasnetc_sys_t msg_id = (gasnetc_sys_t) ((mbits & GASNETC_SELECT_BYTE1)>>8);
      int32_t arg0 = (int32_t) GASNETI_HIWORD(mbits);
      int32_t arg1 = (int32_t) GASNETI_HIWORD(ev->hdr_data);
      int32_t arg2 = (int32_t) GASNETI_LOWORD(ev->hdr_data);
      exec_sys_msg(msg_id, arg0, arg1, arg2);
    }
    
    break;

  case PTL_EVENT_SEND_END:   /* should not happen */
  default:
    gasneti_fatalerror("Invalid event %s on SYS",ptl_event_str[ev->type]);
  }
}

/* ---------------------------------------------------------------------------------
 * Init the system SYS MDs and Event Queue
 * --------------------------------------------------------------------------------- */
static void sys_init(void)
{
  ptl_size_t eq_len = 2*gasneti_nodes + 10;
  ptl_md_t   md;

  /*  printf("[%d] SYS_init: allocated %ld events on SYS_EQ\n",(int)gasneti_mynode,(long)eq_len); */
  gasnetc_SYS_EQ = gasnetc_eq_alloc(eq_len,"SYS_EQ",NULL);

  /* allocate the SYS send buffer */
  gasnetc_buf_init(&gasnetc_SYS_Send,"SYS_Send",0,NULL);
  md.start = NULL;
  md.length = 0;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_EVENT_START_DISABLE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uintptr_t)GASNETC_SYS_SEND_MD;
#else
  md.user_ptr = (void*)sys_event;
#endif
  md.eq_handle = PTL_EQ_NONE;

  GASNETC_PTLSAFE(PtlMDBind(gasnetc_ni_h, md, PTL_RETAIN, &gasnetc_SYS_Send.md_h));
  GASNETI_TRACE_PRINTF(C,("SYS_init: %s initialized, md=%lu",gasnetc_SYS_Send.name,(ulong)gasnetc_SYS_Send.md_h));

  /* allocate the SYS receive buffer */
  gasnetc_buf_init(&gasnetc_SYS_Recv,"SYS_Recv",0,NULL);
  md.start = NULL;
  md.length = 0;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE | PTL_MD_ACK_DISABLE | PTL_MD_TRUNCATE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uintptr_t)GASNETC_SYS_RECV_MD;
#else
  md.user_ptr = (void*)sys_event;
#endif
  md.eq_handle = gasnetc_SYS_EQ->eq_h;

  /* Insert a MLE at the head of the list */
  GASNETC_PTLSAFE(PtlMEAttach(gasnetc_ni_h, GASNETC_PTL_AM_PTE, gasnetc_any_id, GASNETC_PTL_SYS_BITS,
			      GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_BEFORE, &gasnetc_SYS_Recv.me_h));
  GASNETC_PTLSAFE(PtlMDAttach(gasnetc_SYS_Recv.me_h, md, PTL_RETAIN, &gasnetc_SYS_Recv.md_h));

  GASNETI_TRACE_PRINTF(C,("SYS_init: %s me=%lu md=%lu",gasnetc_SYS_Recv.name,(ulong)gasnetc_SYS_Recv.me_h,(ulong)gasnetc_SYS_Recv.md_h));

  sys_barrier_rcvd[0] = 0;
  sys_barrier_rcvd[1] = 0;

  /* make sure everyone has done this before proceeding */
  gasnetc_bootstrapBarrier();

  portals_sysqueue_initialized = 1;
  /* write barrier here? */

}

static void gasnetc_eq_destroy(gasnetc_eq_t *eq)
{
  ptl_event_t ev;
  gasneti_mutex_lock(&eq->lock);
  while (gasnetc_get_event(eq,&ev,GASNETC_EQ_NOLOCK)) {};
  gasneti_mutex_unlock(&eq->lock);
  gasnetc_eq_free(eq);
}

/* ---------------------------------------------------------------------------------
 * Remove the system SYS resources
 * --------------------------------------------------------------------------------- */
static void sys_exit(void)
{
  /* these will automatically unlink the match-list entries as well */
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_SYS_Send.md_h));
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_SYS_Recv.md_h));
  gasnetc_eq_destroy(gasnetc_SYS_EQ);
}

/* ---------------------------------------------------------------------------------
 * Send an SYS message to another gasnet node
 * --------------------------------------------------------------------------------- */
extern void gasnetc_sys_SendMsg(gasnet_node_t node, gasnetc_sys_t msg_id,
				int32_t arg0, int32_t arg1, int32_t arg2)
{
  ptl_size_t         local_offset = 0;
  ptl_size_t         remote_offset = 0;
  ptl_size_t         msg_bytes = 0;
  ptl_handle_md_t    md_h = gasnetc_SYS_Send.md_h;
  ptl_process_id_t   target_id = gasnetc_procid_map[node].ptl_id;
  ptl_ac_index_t     ac_index = GASNETC_PTL_AC_ID;
  ptl_match_bits_t   match_bits;
  uint64_t           hdr_data;

  GASNETI_TRACE_PRINTF(C,("SYS_SendMsg: Sending msg_id=%u to node %d",(unsigned)msg_id,node));
  match_bits = GASNETI_MAKEWORD(arg0,((ptl_match_bits_t)msg_id << 8) | GASNETC_PTL_SYS_BITS);
  hdr_data = GASNETI_MAKEWORD(arg1, arg2);
  GASNETC_PTLSAFE(PtlPutRegion(md_h, local_offset, msg_bytes, PTL_NOACK_REQ, target_id, GASNETC_PTL_AM_PTE, ac_index, match_bits, remote_offset, hdr_data));

}

extern void gasnetc_sys_barrier(void)
{
  static int phase = 0;
  uint32_t goal = 0;
  uint32_t distance;

#if GASNET_DEBUG || GASNETI_STATS_OR_TRACE
  static int barr_cnt = 0;
  GASNETI_TRACE_PRINTF(C,("Entering SYS BARRIER cnt=%d",barr_cnt));
  barr_cnt += 1;
#endif

  gasneti_assert(portals_sysqueue_initialized);

  for (distance = 1; distance < gasneti_nodes; distance *= 2) {
    gasnet_node_t peer;

    if (distance >= gasneti_nodes - gasneti_mynode) {
      peer = gasneti_mynode - (gasneti_nodes - distance);
    } else {
      peer = gasneti_mynode + distance;
    }

    gasnetc_sys_SendMsg(peer,GASNETC_SYS_BARRIER,phase,distance,gasneti_mynode);

    /* wait for completion of the proper receive, which might arrive out of order */
    goal |= distance;
    while ((sys_barrier_rcvd[phase] & goal) != goal) {
      gasnetc_sys_poll(GASNETC_EQ_LOCK);
    }
  }

  /* reset for next barrier */
  sys_barrier_rcvd[phase] = 0;
  phase ^= 1;
}

/* =================================================================================
 * This lower portion of the file is where exported functions are located.
 * These are exported to both the Core and Extended API implementations.
 * ================================================================================= */

/* Reduction (op=MAX) over exitcodes using dissemination pattern.
   Returns 0 on sucess, or non-zero on error (timeout).
 */
extern int gasnetc_sys_exit(int *exitcode_p)
{
  uint32_t goal = 0;
  uint32_t distance;
  int result = 0; /* success */
  int exitcode = *exitcode_p;
  int oldcode;
  gasneti_tick_t timeout_us = 1e6 * gasnetc_shutdown_seconds;
  gasneti_tick_t starttime = gasneti_ticks_now();

  GASNETI_TRACE_PRINTF(C,("Entering SYS EXIT"));

  gasneti_assert(portals_sysqueue_initialized);

  for (distance = 1; distance < gasneti_nodes; distance *= 2) {
    gasnet_node_t peer;

    if (distance >= gasneti_nodes - gasneti_mynode) {
      peer = gasneti_mynode - (gasneti_nodes - distance);
    } else {
      peer = gasneti_mynode + distance;
    }

    oldcode = gasneti_atomic_read(&gasnetc_exitcode, 0);
    exitcode = MAX(exitcode, oldcode);
    gasnetc_sys_SendMsg(peer,GASNETC_SYS_SHUTDOWN_REQUEST,distance,exitcode,gasneti_mynode);

    /* wait for completion of the proper receive, which might arrive out of order */
    goal |= distance;
    while ((sys_exit_rcvd & goal) != goal) {
      gasnetc_sys_poll(GASNETC_EQ_LOCK);
      if (gasneti_ticks_to_us(gasneti_ticks_now() - starttime) > timeout_us) {
        result = 1; /* failure */
        goto out;
      }
    }
  }

out:
  oldcode = gasneti_atomic_read(&gasnetc_exitcode, 0);
  *exitcode_p = MAX(exitcode, oldcode);

  return result;
}

/* ---------------------------------------------------------------------------------
 * Setup the portals network so that we can begin communicating
 *   - Get the Portals network-interface handle.  
 *   - Get the Portals proc_id map so we know how to talk to all other nodes.
 * --------------------------------------------------------------------------------- */
static char* gasnetc_flush_buf = NULL;
int gasnetc_io_buffer_size = 0;  /* was 1024 */
extern void gasnetc_init_portals_network(int *argc, char ***argv)
{
  ptl_interface_t   ptl_iface;
#if PLATFORM_OS_CNL
  int               use_bridge = PTL_BRIDGE_UK;
#else
  int               use_bridge = PTL_BRIDGE_QK;
#endif
  int               use_nal = PTL_IFACE_SS;
  ptl_ni_limits_t   ni_limits;
  cnos_nidpid_map_t *cnos_map;
  int               rc, i, node;
  int               num_interfaces;
  int               pid_offset = 0;

#if HAVE_PMI_CNOS
  if (PMI_SUCCESS != PMI_Init(&rc)) {
    gasneti_fatalerror("PMI_Init() failed");
  }
  if (PMI_SUCCESS != PMI_Get_rank(&i)) {
    gasneti_fatalerror("PMI_Get_rank() failed");
  }
  gasneti_mynode = i;
  if (PMI_SUCCESS != PMI_Get_size(&i)) {
    gasneti_fatalerror("PMI_Get_size() failed");
  }
  gasneti_nodes = i;
#else
  gasneti_mynode = cnos_get_rank();
  gasneti_nodes = cnos_get_size();
#endif

  /* init tracing as early as possible */
  gasneti_trace_init(argc, argv);

  if (gasneti_nodes > GASNET_MAXNODES) {
    gasneti_fatalerror("GASNet Portals conduit designed to work for up to %d nodes,"
		       " this job uses %d nodes.  Modify size of gasnet_node_t "
		       " and rebuild library",GASNET_MAXNODES,gasneti_nodes);
  }

  /* Set up buffered IO for STDOUT */
  if (gasnetc_io_buffer_size > 0) {
    gasnetc_flush_buf = (char*)gasneti_malloc(gasnetc_io_buffer_size);
    setvbuf(stdout, gasnetc_flush_buf, _IOFBF, gasnetc_io_buffer_size);
  }

  /* First, init portals */
  GASNETC_PTLSAFE(PtlInit(&num_interfaces));

  /* construct the interface */
  ptl_iface = IFACE_FROM_BRIDGE_AND_NALID(use_bridge,use_nal);

#if GASNETC_USE_SANDIA_ACCEL
  if (gasneti_getenv_yesno_withdefault("GASNET_PORTAL_ACCEL",0)) {
    gasnetc_use_accel = 1;
    pid_offset = 4096;
    ptl_iface = CRAY_ACCEL;
    if (gasneti_mynode == 0) printf("Using SANDIA ACCELERATED mode\n");
  }
#endif

  /* Get the network handle */
  rc = PtlNIInit(ptl_iface, PTL_PID_ANY, NULL, &ni_limits, &gasnetc_ni_h);
  switch (rc) {
  case PTL_OK:
  case PTL_IFACE_DUP:
    break;
  default:
    gasneti_fatalerror("GASNet Portals failed on call to PtlNiInit:\n"
		       "  iface_type = %x error=%s (%i)\n"
		       "  at: %s\n",ptl_iface,ptl_err_str[rc],rc,gasneti_current_loc);
  }

  /* Get my process info */
  GASNETC_PTLSAFE(PtlGetUid(gasnetc_ni_h,&gasnetc_uid));
  GASNETC_PTLSAFE(PtlGetId(gasnetc_ni_h,&gasnetc_myid));

#if HAVE_PMI_CNOS
  /* Not using the CNOS barrier */
#elif PLATFORM_OS_CNL
  /* must init the CNOS barrier under CNL (this is a noop for Catamount)
   * This MUST be done before calls to
   *      cnos_register_ptlid() AND cnos_get_nidpid_map()
   * which for a split-phase barrier in CNL job startup.
   * Cannot call cnos_barrier() until all three have been called.
   */
  cnos_barrier_init(gasnetc_ni_h);

  /* Assume APRUN launcher */
  if ((rc=cnos_register_ptlid(gasnetc_myid)) != 0) {
    gasneti_fatalerror("cnos_register_ptlid returned %d\n",rc);
  }
#else
  /* Under Catamount, dont have to do anything */
#endif

  /* get process to portals address mapping */
#if HAVE_PMI_CNOS
  if (PMI_SUCCESS != PMI_CNOS_Get_nidpid_map((void **)&cnos_map)) {
    gasneti_fatalerror("PMI_CNOS_Get_nidpid_map failed");
  }
#else
  if(gasneti_nodes != cnos_get_nidpid_map(&cnos_map)) {
    gasneti_fatalerror("cnos_get_nidpid_map size != %d",gasneti_nodes);
  }
#endif

  gasneti_assert_always(cnos_map[gasneti_mynode].nid == gasnetc_myid.nid);
  gasneti_assert_always(cnos_map[gasneti_mynode].pid == (gasnetc_myid.pid - pid_offset));

  gasnetc_procid_map = (gasnetc_procid_t*)gasneti_malloc(gasneti_nodes * sizeof(gasnetc_procid_t));
  for (node = 0; node < gasneti_nodes; node++) {
    gasnetc_procid_map[node].ptl_id.nid = cnos_map[node].nid;
    gasnetc_procid_map[node].ptl_id.pid = cnos_map[node].pid + pid_offset;
    gasnetc_procid_map[node].next = NULL;
  }

#if PLATFORM_OS_CATAMOUNT
  /* gasneti_nodemap is unused - no mmap(), no shared memory */
#else
  /* Build nodemap from cnos_map w/o need for a bootstrapExchange */
  gasneti_nodemapInit(NULL, &cnos_map[0].nid,
                      sizeof(cnos_map[0].nid),
                      sizeof(cnos_map[0]));
#endif

  /* Allocate and init (part of) the connection state array */
  gasnetc_conn_state = (gasnetc_conn_t*)gasneti_malloc(gasneti_nodes*sizeof(gasnetc_conn_t));
  for (i = 0; i < gasneti_nodes; i++) {
    gasnetc_conn_t *s = &gasnetc_conn_state[i];
    s->flags = 0;
    gasneti_weakatomic_set(&s->src_lid, 0, 0);
    s->lids = NULL;
    GASNETC_INITLOCK_STATE(s);
  }

  /* set the number of seconds we poll until forceful shutdown.  May be over-ridden
   * by env-var when they are processed as part of gasnetc_attach
   */
  gasnetc_shutdown_seconds = 3. + gasneti_nodes/8.;
  gasnetc_shutdown_seconds = (gasnetc_shutdown_seconds > shutdown_max ? shutdown_max : gasnetc_shutdown_seconds);

  /* setup system SYS Send/Recv resources */
  sys_init();

  /* setup an empty EQ for firehose */
  #if GASNETC_FIREHOSE_LOCAL && GASNETC_USE_MDUPDATE
    gasnetc_EMPTY_EQ = gasnetc_eq_alloc(1,"EMPTY_EQ",NULL);
  #endif
}

/* ---------------------------------------------------------------------------------
 * Bootstrap barrier function.
 * Just use cnos_barrier on XT3, but might have to init it first.
 * --------------------------------------------------------------------------------- */
extern void gasnetc_bootstrapBarrier(void) {
  static int gasnetc_bootstrapBarrierCnt = 0;

  gasnetc_bootstrapBarrierCnt++;

  /* check the system queue if after network init */
  if (portals_sysqueue_initialized) {
    gasnetc_sys_barrier();
  } else {
    GASNETI_TRACE_PRINTF(C,("bootstrapBarrier count = %d",gasnetc_bootstrapBarrierCnt));
#if HAVE_PMI_CNOS
    PMI_Barrier();
#else
    cnos_barrier();
#endif
  }
}

/* ---------------------------------------------------------------------------------
 * Bootstrap broadcast function over portals
 * After the network has been initialized, but before all the conduit resources
 * have been allocated, can use this function to perform the equivelent of
 * an MPI_Broadcast.  The root node will broadcast its info to all other nodes.
 * --------------------------------------------------------------------------------- */
extern void gasnetc_bootstrapBroadcast(void *src, size_t len, void *dest, int rootnode)
{
  gasnet_node_t left_child_rank, local_rank;
  int children;
  ptl_md_t src_md, dest_md;
  ptl_handle_me_t dest_me_h;
  ptl_handle_md_t src_h, dest_h;
  ptl_handle_eq_t eq_h;
  ptl_event_t ev;
  int eq_len = GASNETC_BOOTSTRAP_BCAST_RADIX + 2;
  int i, rc;

  GASNETI_TRACE_PRINTF(C,("bootBroadcast from %d len = %d, src=%p dest=%p",rootnode,(int)len,src,dest));
  gasneti_assert(GASNETC_BOOTSTRAP_BCAST_RADIX > 1);

  /* messy wrap stuff for rootnode != 0 */
  local_rank = gasneti_mynode + gasneti_nodes - rootnode;
  if (local_rank >= gasneti_nodes) local_rank -= gasneti_nodes; /* local_rank %= gasneti_nodes */
  left_child_rank = GASNETC_BOOTSTRAP_BCAST_RADIX * local_rank + 1;
  children = (left_child_rank >= gasneti_nodes)
                  ? 0 : GASNETC_MIN(GASNETC_BOOTSTRAP_BCAST_RADIX, gasneti_nodes - left_child_rank);

  /* alloc an event queue for the action */
  GASNETC_PTLSAFE(PtlEQAlloc(gasnetc_ni_h, eq_len, NULL, &eq_h));

  /* register the dest md with an EQ on a match list */
  if (local_rank != 0) {
    dest_md.start = dest;
    dest_md.length = len;
    dest_md.threshold = PTL_MD_THRESH_INF;
    dest_md.max_size = 0;
    dest_md.options = PTL_MD_EVENT_START_DISABLE | PTL_MD_OP_PUT | PTL_MD_MANAGE_REMOTE;
    dest_md.user_ptr = 0;
    dest_md.eq_handle = eq_h;

    /* construct the match entry */
    GASNETC_PTLSAFE(PtlMEAttach(gasnetc_ni_h, GASNETC_PTL_AM_PTE, gasnetc_any_id, GASNETC_PTL_BOOT_BITS, GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_AFTER, &dest_me_h));

    /* attach the dest memory descriptor */
    GASNETC_PTLSAFE(PtlMDAttach(dest_me_h, dest_md, PTL_RETAIN, &dest_h));
  }

  /* wait for all dest_md registrations */
  /* XXX: point-to-point sync would be preferable, but this code is unused anyway */
  gasnetc_bootstrapBarrier();

  /* step 1 - wait for data to arrive from parent, if any */
  if (local_rank != 0) {
    rc = PtlEQWait(eq_h, &ev);
    switch (rc) {
    case PTL_OK:
      gasneti_assert_always( ev.type == PTL_EVENT_PUT_END );
      break;
    default:
      gasneti_fatalerror("GASNet Portals Error in bootBroadcast waiting for PUT_END event: %s (%i)\n at %s\n",
			 ptl_err_str[rc],rc,gasneti_current_loc);
    };
  
    /* now remove the dest MD, which will also unlink the match-list entry */
    GASNETC_PTLSAFE(PtlMDUnlink(dest_h));
  }

  /* step 2 - put data to children, if any */
  if (children) {
    /* register the src md */
    src_md.start = local_rank ? dest : src;
    src_md.length = len;
    src_md.threshold = PTL_MD_THRESH_INF;
    src_md.max_size = 0;
    src_md.options = PTL_MD_EVENT_START_DISABLE;
    src_md.user_ptr = 0;
    src_md.eq_handle = eq_h;
    GASNETC_PTLSAFE(PtlMDBind(gasnetc_ni_h, src_md, PTL_RETAIN, &src_h));

    /* issue puts */
    for (i=0; i<children; ++i) {
      gasnet_node_t child = left_child_rank + rootnode + i;
      if (child >= gasneti_nodes) child -= gasneti_nodes; /* child %= gasneti_nodes */
      GASNETC_PTLSAFE(PtlPut(src_h,PTL_NOACK_REQ, gasnetc_procid_map[child].ptl_id,GASNETC_PTL_AM_PTE,GASNETC_PTL_AC_ID,GASNETC_PTL_BOOT_BITS,0,0));
    }

    /* wait for completion of puts */
    for (i=0; i<children; ++i) {
      rc = PtlEQWait(eq_h, &ev);
      switch (rc) {
      case PTL_OK:
        gasneti_assert_always( ev.type == PTL_EVENT_SEND_END );
        break;
      default:
        gasneti_fatalerror("GASNet Portals Error in bootBroadcast waiting for SEND_END event: %s (%i)\n at %s\n",
			   ptl_err_str[rc],rc,gasneti_current_loc);
      };
    }

    /* now remove the src MD */
    GASNETC_PTLSAFE(PtlMDUnlink(src_h));
  }

  /* reclaim the event queue */
  rc = PtlEQFree(eq_h);
  if_pf (rc == PTL_EQ_IN_USE) { /* XXX: Not sure what causes this - it is not in Portals Spec */
    int i;
    for (i = 0; i < 4; ++i) { /* bounded retry */
      rc = PtlEQGet(eq_h, &ev);
      if (rc == PTL_OK) {
        gasneti_fatalerror("Unexpected event at end of bootBroadcast w/ mbits=%p\n",(void*)ev.match_bits);
      }
      if (PtlEQFree(eq_h) == PTL_OK) break;
    }
  }

  if (!local_rank && (dest != src)) {
    memcpy(dest, src, len);
  }

  /* should not need a barrier here */
  GASNETI_TRACE_PRINTF(C,("bootBroadcast exit"));
}

/* ---------------------------------------------------------------------------------
 * Bootstrap exchange function over portals
 * After the network has been initialized, but before all the conduit resources
 * have been allocated, can use this function to perform the equivelent of
 * an MPI_Allgather.  Bruck's concatenation algorithm is used here.
 * --------------------------------------------------------------------------------- */
extern void gasnetc_bootstrapExchange(void *src, size_t len, void *dest)
{
  ptl_md_t temp_md;
  ptl_handle_me_t temp_me_h;
  ptl_handle_md_t temp_h;
  ptl_handle_eq_t eq_h;
  ptl_event_t ev;
  const int eq_len = 64; /* 32 PUT_END + 32 SEND_END worst case */
  ptl_hdr_data_t rcvd = 0;
  ptl_hdr_data_t goal = 0;
  ptl_hdr_data_t hdr_data = 1;
  ptl_size_t offset = len;
  void *temp;
  uint32_t distance;
  int rc, sends = 0;

  GASNETI_TRACE_PRINTF(C,("bootExch with len = %d, src = %p dest = %p",(int)len,src,dest));

  temp = gasneti_malloc(len * gasneti_nodes);

  /* alloc an event queue for the action */
  GASNETC_PTLSAFE(PtlEQAlloc(gasnetc_ni_h, eq_len, NULL, &eq_h));

  /* register the temp md with an EQ on a match list */
  temp_md.start = temp;
  temp_md.length = len * gasneti_nodes;
  temp_md.threshold = PTL_MD_THRESH_INF;
  temp_md.max_size = 0;
  temp_md.options = PTL_MD_EVENT_START_DISABLE | PTL_MD_OP_PUT | PTL_MD_MANAGE_REMOTE;
  temp_md.user_ptr = 0;
  temp_md.eq_handle = eq_h;

  /* construct the match entry */
  GASNETC_PTLSAFE(PtlMEAttach(gasnetc_ni_h, GASNETC_PTL_AM_PTE, gasnetc_any_id, GASNETC_PTL_BOOT_BITS, GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_AFTER, &temp_me_h));

  /* attach the temp memory descriptor */
  GASNETC_PTLSAFE(PtlMDAttach(temp_me_h, temp_md, PTL_RETAIN, &temp_h));
  gasnetc_sys_barrier();

  /* Bruck's Concatenation Algorithm */
  memcpy(temp, src, len);
  for (distance = 1; distance < gasneti_nodes; distance *= 2) {
    ptl_size_t to_xfer;
    gasnet_node_t peer;

    if (gasneti_mynode >= distance) {
      peer = gasneti_mynode - distance;
    } else {
      peer = gasneti_mynode + (gasneti_nodes - distance);
    }

    to_xfer = len * MIN(distance, gasneti_nodes - distance);
    GASNETC_PTLSAFE(PtlPutRegion(temp_h, 0, to_xfer, PTL_NOACK_REQ, gasnetc_procid_map[peer].ptl_id,
                                 GASNETC_PTL_AM_PTE, GASNETC_PTL_AC_ID, GASNETC_PTL_BOOT_BITS, offset, hdr_data));
    sends += 1;

    /* wait for completion of the proper receive, and keep count of uncompleted sends.
       "rcvd" is an accumulator to deal with out-of-order receives, which are IDed by the hdr_data */
    goal |= hdr_data;
    while ((rcvd & goal) != goal) {
      rc = PtlEQWait(eq_h, &ev);
      switch (rc) {
      case PTL_OK:
        gasneti_assert(( ev.type == PTL_EVENT_PUT_END ) || ( ev.type == PTL_EVENT_SEND_END ));
        if (ev.type == PTL_EVENT_SEND_END) {
          sends -= 1;
        } else {
          rcvd |= ev.hdr_data;
          gasneti_assert(ev.rlength == ev.mlength);
          gasneti_assert((ev.rlength == to_xfer) || (ev.hdr_data != hdr_data));
        }
        break;
      default:
        gasneti_fatalerror("GASNet Portals Error in bootExchange waiting for event: %s (%i)\n at %s\n",
                           ptl_err_str[rc],rc,gasneti_current_loc);
      }
    }

    hdr_data <<= 1;
    offset += to_xfer;
  }

  /* now remove the temp MD, which will also unlink the match-list entry */
  GASNETC_PTLSAFE(PtlMDUnlink(temp_h));

  /* wait for any SEND_END events not yet seen */
  while (sends) {
    rc = PtlEQWait(eq_h, &ev);
    switch (rc) {
    case PTL_OK:
      gasneti_assert( ev.type == PTL_EVENT_SEND_END );
      sends -= 1;
      break;
    default:
      gasneti_fatalerror("GASNet Portals Error in bootExchange waiting for event: %s (%i)\n at %s\n",
                         ptl_err_str[rc],rc,gasneti_current_loc);
    }
  }

  /* reclaim the event queue */
  rc = PtlEQFree(eq_h);
  if_pf (rc == PTL_EQ_IN_USE) { /* XXX: Not sure what causes this - it is not in Portals Spec */
    int i;
    for (i = 0; i < 4; ++i) { /* bounded retry */
      rc = PtlEQGet(eq_h, &ev);
      if (rc == PTL_OK) {
        gasneti_fatalerror("Unexpected event at end of bootExchange w/ mbits=%p\n",(void*)ev.match_bits);
      }
      if (PtlEQFree(eq_h) == PTL_OK) break;
    }
  }

  /* now rotate into final position */
  memcpy(dest, (uint8_t*)temp + len * (gasneti_nodes - gasneti_mynode), len * gasneti_mynode);
  memcpy((uint8_t*)dest + len * gasneti_mynode, temp, len * (gasneti_nodes - gasneti_mynode));
  gasneti_free(temp);

  /* should not need a barrier here */
  GASNETI_TRACE_PRINTF(C,("bootExch exit"));
}

#if 0
extern void gasnetc_testBootExch(void)
{
  char *src = (char*)gasneti_malloc(gasneti_nodes*4*sizeof(char));
  char *dest = (char*)gasneti_malloc(gasneti_nodes*4*sizeof(char));
  int i;
  int failed = 0;
  char *probe;

  for (i = 0; i < 4*gasneti_nodes; i++) dest[i] = 'z';
  src[0] = 'a'; 
  src[1] = 'b'; 
  src[2] = 'c'; 
  src[3] = 'd'; 
  src[4] = 'e'; 
  src[5] = 'f'; 
  src[6] = 'g'; 
  src[7] = 'h';
  printf("[%d] testboot src = %p  dest = %p\n",gasneti_mynode,src,dest);

  probe = src;
  printf("[%d] testboot src data is [%d]-[%d]-[%d]-[%d]  [%d]-[%d]-[%d]-[%d]\n",
	 gasneti_mynode,
	 (int)probe[0],(int)probe[1],(int)probe[2],(int)probe[3],
	 (int)probe[4],(int)probe[5],(int)probe[6],(int)probe[7]);
  probe = dest;
  printf("[%d] testboot dest data is [%d]-[%d]-[%d]-[%d]  [%d]-[%d]-[%d]-[%d]\n",
	 gasneti_mynode,
	 (int)probe[0],(int)probe[1],(int)probe[2],(int)probe[3],
	 (int)probe[4],(int)probe[5],(int)probe[6],(int)probe[7]);

  gasnetc_bootstrapExchange((gasneti_mynode ? (src+4) : src),4,dest);

  probe = src;
  printf("[%d] testboot src data is [%d]-[%d]-[%d]-[%d]  [%d]-[%d]-[%d]-[%d]\n",
	 gasneti_mynode,
	 (int)probe[0],(int)probe[1],(int)probe[2],(int)probe[3],
	 (int)probe[4],(int)probe[5],(int)probe[6],(int)probe[7]);
  probe = dest;
  printf("[%d] testboot dest data is [%d]-[%d]-[%d]-[%d]  [%d]-[%d]-[%d]-[%d]\n",
	 gasneti_mynode,
	 (int)probe[0],(int)probe[1],(int)probe[2],(int)probe[3],
	 (int)probe[4],(int)probe[5],(int)probe[6],(int)probe[7]);

  gasnetc_bootstrapBarrier();

  gasneti_free(src);
  gasneti_free(dest);

}
#endif

/* ---------------------------------------------------------------------------------
 * Function to determine if a chunk of memory of the given size can be allocated
 * and registered as a portals memory descriptor (pinned).
 * Returns true if can be, false if the memory either cannot be allocated or
 * if pinning fails.
 * Cleans up after itself.
 * --------------------------------------------------------------------------------- */
static int try_pin(const uintptr_t size)
{
  ptl_md_t md;
  ptl_handle_md_t md_h;
  int rc, ok;
#if HAVE_MMAP
  void *mem = gasneti_mmap(size);
  if (mem == MAP_FAILED) return 0;
#else
  void *mem = gasneti_malloc_allowfail(size);
  if (mem == NULL) return 0;
#endif

  /* poll system queue here since these operations can take some time */
  gasnetc_sys_poll(GASNETC_EQ_LOCK);

#if GASNETC_DEBUG
  printf("[%d] try_pin with %lu bytes\n",gasneti_mynode,(unsigned long)size);
#endif

  /* Now try to pin by creating a free floating MD for this memory */
  md.start = mem;
  md.length = size;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_EVENT_START_DISABLE | PTL_MD_EVENT_END_DISABLE;
  md.user_ptr = 0;
  md.eq_handle = PTL_EQ_NONE;
  rc = PtlMDBind(gasnetc_ni_h, md, PTL_RETAIN, &md_h);
  switch(rc) {
  case PTL_OK:
    ok = 1;
    break;
  case PTL_NO_SPACE:
  case PTL_VAL_FAILED:   /* this error condition not in Portals spec, added by Cray */
    ok = 0;
    break;
  default:
    gasneti_fatalerror("try_pin::PltMDBind returned error %d = %s for mem size %ld",rc,ptl_err_str[rc],(long)size);
  }
  /* release the memory descriptor */
  if (ok) {
    GASNETC_PTLSAFE(PtlMDUnlink(md_h));
  }
#if HAVE_MMAP
  gasneti_munmap(mem, size);
#else
  gasneti_free(mem);
#endif
  GASNETI_TRACE_PRINTF(C,("try_pin of %lu bytes %s",(unsigned long)size,(ok?"successful":"failed")));
  return ok;
}

/* ---------------------------------------------------------------------------------
 * Determine the largest amount of memory that can be pinned on the node.
 * --------------------------------------------------------------------------------- */
extern uintptr_t gasnetc_portalsMaxPinMem(void)
{
#define MBYTE 1048576ULL
  uint64_t granularity = 16ULL * MBYTE;
  uint64_t low = granularity;
  uint64_t high;
  uint64_t limit = 16ULL * 1024ULL * MBYTE;
  uint64_t prev;
#undef MBYTE

#if PLATFORM_OS_CNL
  /* On CNL, if we try to pin beyond what the OS will allow, the job is killed.
   * So, there is really no way (that we know of) to determine the EXACT maximum
   * pinnable memory under CNL without dire consequences.
   * For this platform, we will simply try a large fraction of the physical
   * memory.  If that is too big, then the job will be killed at startup.
   */
  double pm_ratio = gasneti_getenv_dbl_withdefault(
                        "GASNET_PHYSMEM_PINNABLE_RATIO", 
                        GASNETC_DEFAULT_PHYSMEM_PINNABLE_RATIO);

  limit = gasneti_mmapLimit(limit, pm_ratio * gasneti_getPhysMemSz(1),
                            &gasnetc_bootstrapExchange,
                            &gasnetc_bootstrapBarrier);
#endif

  /* make sure we can pin at least the initial low watermark of memory */
  if (! try_pin(low)) {
    gasneti_fatalerror("Unable to alloc and pin minimal memory of size %d bytes",(int)low);
  }
  high = low;

  /* move high boundary up (exponentially) until it will no longer pin, or we hit the limit */
  prev = low;
  high = prev*2;
  if (high > limit) high = limit;
  while (try_pin(high)) {
    prev = high;
    if (high >= limit) {
	break;
    }
    high *= 2;
    if (high > limit) high = limit;
  }
  low = prev;

  /* Now bisect until difference is within the granularity */
  while ((high - low) > granularity) {
    uint64_t mid = (low + high)/2;
    if (try_pin(mid)) {
      low = mid;
    } else {
      high = mid;
    }
  }
  if ((low != high) && try_pin(high)) {
    low = high;
  }
  GASNETI_TRACE_PRINTF(C,("MaxPinMem = %lu",(unsigned long)low));
  return (uintptr_t)low;
}

/* ---------------------------------------------------------------------------------
 * get a chunk from the allocator, or NULL if not available.
 * --------------------------------------------------------------------------------- */
extern void *gasnetc_chunk_alloc_no_off(gasnetc_PtlBuffer_t *buf, size_t nbytes)
{
    void *p;
    
#if GASNET_DEBUG
    gasneti_assert(buf->use_chunks);
    if (nbytes > GASNETC_CHUNKSIZE) {
      gasneti_fatalerror("gasnetc_chunk_alloc*() requested %lu bytes, limit is %lu",(ulong)nbytes,(ulong)GASNETC_CHUNKSIZE);
    }
#endif

    p = gasneti_lifo_pop(&buf->freelist);

#if GASNETI_STATS_OR_TRACE
    if (p != NULL) {
      gasneti_mutex_lock(&buf->lock);
      buf->inuse++;
      if (buf->inuse > buf->hwm) buf->hwm = buf->inuse;
      GASNETI_TRACE_PRINTF(C,("CHUNK_ALLOC: name %s, inuse = %d, hwm = %d, offset=%lu",buf->name,buf->inuse,buf->hwm,(unsigned long)((uintptr_t)p - (uintptr_t)buf->start)));
      GASNETI_TRACE_EVENT(C, CHUNK_ALLOC);
      gasneti_mutex_unlock(&buf->lock);
    }
#endif

    return p;
}

/* ---------------------------------------------------------------------------------
 * get a chunk from the allocator.
 * returns the address on success, NULL if not able to satisfy request.
 * --------------------------------------------------------------------------------- */
extern void *gasnetc_chunk_alloc(gasnetc_PtlBuffer_t *buf, size_t nbytes, ptl_size_t *offset)
{
    void *p = gasnetc_chunk_alloc_no_off(buf, nbytes);
    
    /* offset is garbage on failure, but we avoid a branch */
    *offset = ((uintptr_t)p - (uintptr_t)buf->start);

    return p;
}

/* ---------------------------------------------------------------------------------
 * get a chunk from the allocator.
 * returns the address on success, NULL if not able to satisfy request.
 * May poll network at most pollmax times.
 * --------------------------------------------------------------------------------- */
extern void *gasnetc_chunk_alloc_withpoll(gasnetc_PtlBuffer_t *buf, size_t nbytes, ptl_size_t *offset,
					int pollmax, gasnetc_pollflag_t poll_type)
{
    int cnt = 0;
    void *p;
    
    gasneti_assert(pollmax > 0);
    gasneti_assert(poll_type != GASNETC_NO_POLL);

    p = gasnetc_chunk_alloc_no_off(buf,nbytes);

    /* poll up to pollmax times, waiting for chunk to free-up */
    while (!p && (cnt++ < pollmax)) {
      if (poll_type == GASNETC_FULL_POLL) {
	gasneti_AMPoll();
      } else if (poll_type == GASNETC_SAFE_POLL) {
	gasnetc_portals_poll(poll_type);
      }

      p = gasnetc_chunk_alloc_no_off(buf,nbytes);
    }

    /* offset is garbage on failure, but we avoid a branch */
    *offset = ((uintptr_t)p - (uintptr_t)buf->start);

    return p;
}

/* ---------------------------------------------------------------------------------
 * release a chunk back to the allocator.
 * MLW: Programmer error to return chunk to wrong allocator since just giving
 *      the offset.  We could hand in full address and compute offset here for
 *      better error checking, but would have to compute full address from offset
 *      at point of call, and unwind that here.  
 * --------------------------------------------------------------------------------- */
extern void gasnetc_chunk_free(gasnetc_PtlBuffer_t *buf, ptl_size_t offset)
{
    void *p = (void*)((uintptr_t)buf->start + offset);
    gasneti_assert(buf->use_chunks);
    
    gasneti_lifo_push(&buf->freelist, p);

#if GASNETI_STATS_OR_TRACE
    gasneti_mutex_lock(&buf->lock);
    buf->inuse--;
    GASNETI_TRACE_PRINTF(C,("CHUNK_FREE: name %s, inuse = %d, hwm = %d, offset=%lu",buf->name,buf->inuse,buf->hwm,(unsigned long)offset));
    GASNETI_TRACE_EVENT(C, CHUNK_FREE);
    gasneti_mutex_unlock(&buf->lock);
#endif
}

/* bug 2102: PtlEQAlloc/PtlEQFree are not thread-safe.
 * However, we no longer perform these dynamically (only at startup and cleanup now).
 * So, we don't need the mutex introduced for bug 2102.
 */

static gasnetc_eq_t* gasnetc_eq_alloc(long num_events, const char* name, ptl_eq_handler_t hndlr)
{
  gasnetc_eq_t *eq = (gasnetc_eq_t*)gasneti_malloc(sizeof(gasnetc_eq_t));
  eq->num_events = num_events;
  GASNETC_PTLSAFE(PtlEQAlloc(gasnetc_ni_h, num_events, hndlr, &eq->eq_h));
  gasneti_mutex_init(&eq->lock);
  eq->name = gasneti_strdup(name);
  GASNETI_TRACE_PRINTF(C,("gasnetc_eq_alloc %s with %ld events (%i)",eq->name,num_events,(int)eq->eq_h));
  return eq;
}
static void gasnetc_eq_free(gasnetc_eq_t *eq)
{
  GASNETC_PTLSAFE(PtlEQFree(eq->eq_h));
  GASNETI_TRACE_PRINTF(C,("gasnetc_eq_free %s with %ld events (%i)",eq->name,eq->num_events,(int)eq->eq_h));
  gasneti_free(eq->name);
  gasneti_free(eq);
}

  

/* ---------------------------------------------------------------------------------
 * Allocate a temp md to be used as the source of a Put or destination
 * of a Get operation.  MD to be free floating, not target of remote op.
 * Associated with eq_h Event Queue (usually the SAFE_EQ).
 * NOTE:  Assumes caller has already allocated a tmpmd ticket
 * Use gasnetc_alloc_tmpmd_withpoll for a caller that does not already have a ticket.
 * --------------------------------------------------------------------------------- */
extern ptl_handle_md_t gasnetc_alloc_tmpmd(void* start, size_t nbytes)
{
  ptl_md_t md;
  ptl_handle_md_t md_h;

  gasneti_assert(!gasnetc_use_firehose);
  GASNETI_TRACE_PRINTF(C,("Alloc_Tmpmd: num available TmpMDs = %d",gasnetc_num_tickets(&gasnetc_tmpmd_tickets)));

  md.start = start;
  md.length = nbytes;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_EVENT_START_DISABLE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uintptr_t)GASNETC_TMP_MD;
#else
  md.user_ptr = (void*)TMPMD_event;
#endif
  md.eq_handle = gasnetc_SAFE_EQ->eq_h;

  GASNETC_PTLSAFE(PtlMDBind(gasnetc_ni_h, md, PTL_UNLINK, &md_h));

#if GASNETI_STATS_OR_TRACE
  {
	int inuse = gasnetc_max_tmpmd - (int)gasnetc_num_tickets(&gasnetc_tmpmd_tickets);
	if (gasnetc_tmpmd_hwm < inuse) gasnetc_tmpmd_hwm = inuse;
	GASNETI_TRACE_PRINTF(C,("ALLOC TMPMD at 0x%p, len=%d, inuse=%d, hwm=%d",start,(int)nbytes,inuse,gasnetc_tmpmd_hwm));
	GASNETI_TRACE_EVENT(C, TMPMD_ALLOC);
  }
#endif

  return md_h;
}

/* ---------------------------------------------------------------------------------
 * Release the TMP memory descriptor (unpin the region), but dont dealloc the
 * memory space (since we did not alloc it).
 * --------------------------------------------------------------------------------- */
extern void gasnetc_free_tmpmd(ptl_handle_md_t md_h)
{
  gasneti_assert(!gasnetc_use_firehose);
  gasnetc_return_ticket(&gasnetc_tmpmd_tickets);
  GASNETC_PTLSAFE(PtlMDUnlink(md_h));
#if GASNETI_STATS_OR_TRACE
      {
	int inuse = gasnetc_max_tmpmd - (int)gasnetc_num_tickets(&gasnetc_tmpmd_tickets);
	GASNETI_TRACE_PRINTF(C,("FREE TMPMD, inuse=%d",inuse));
	GASNETI_TRACE_EVENT(C, TMPMD_FREE);
      }
#endif
}

/* Table defining the number of credits_per_node and banked credits we assign by
 * default based on the number of gasnet nodes in the job.
 * As the node count increases, we rely more on dynamic credit management
 * to keep the ReqRB and AM Event Queue memory usage within reason.
 * Credits_per_node and banked credits for node counts between
 * table values are linearly interpolated.
 */
typedef struct _credit_record {
  gasnet_node_t nodes;
  int           credits_per_node;
  int           banked_credits;
} credit_record_t;
#if (GASNETC_CHUNKSIZE == 1024)
/* 51200 banked credits based on 4 cred_per_node minimum assuming 128 active
 * communication partners allocating 400 credits per partner
 */
static credit_record_t credit_table[] = {
  {1,      200,     400},    /*  2.50 MB  ReqRB + AM space */
  {64,     170,   25600},    /* 14.50 MB */
  {128,    170,   51200},    /* 27.99 MB */
  {512,     64,   51200},    /* 32.49 MB */
  {1024,    32,   51200},    /* 32.49 MB */
  {2048,    16,   51200},    /* 32.49 MB */
  {3072,    12,   51200},    /* 33.99 MB */
  {4096,     8,   51200},    /* 32.49 MB */
  {5120,     4,   51200},    /* 27.99 MB */
  {20480,    4,   51200},    /* 50.48 MB */
};
#elif (GASNETC_CHUNKSIZE == 2048)
/* 25600 banked credits based on 8 cred_per_node minimum assuming 128 active
 * communication partners allocating 200 credits per partner
 */
static credit_record_t credit_table[] = {
  {1,      200,     200},    /*  5.00 MB  ReqRB + AM space */
  {64,     170,   12800},    /* 11.00 MB */
  {128,    170,   25600},    /* 19.99 MB */
  {512,     64,   25600},    /* 25.99 MB */
  {1024,    32,   25600},    /* 25.99 MB */
  {2048,    16,   25600},    /* 25.99 MB */
  {3072,    12,   25600},    /* 25.99 MB */
  {4096,     8,   25600},    /* 25.99 MB */
  {20480,    8,   25600},    /* 73.98 MB */
};
#else
#error "MUST DEFINE default credit_table for this value of GASNETC_CHUNKSIZE"
#endif
/* Using credit_table, computes the default number of credits_per_node and
 * banked_credits.  Also estimate size of ReqRB space
 */
static void compute_default_credits(int *cpn, int64_t *banked, int64_t *rb_space)
{
  int i = 0;
  int i_max = sizeof(credit_table)/sizeof(credit_record_t);
  int found = 0;
  int64_t cred_per_node = 0;
  int64_t num_banked =  0;
  int64_t tot_credits;

  /* search table for this job size */
  for (i = 0; i < i_max; i++) {
    if (credit_table[i].nodes > gasneti_nodes) {
      found = 1;
      break;
    }
  }
  if (found) {
    double dnode;
    double frac;
    gasneti_assert(i>0);
    /* interpolate values between indicies i-1 and i */
    dnode = (double)(credit_table[i].nodes - credit_table[i-1].nodes);
    frac = ((double)(gasneti_nodes - credit_table[i-1].nodes))/dnode;
    cred_per_node = credit_table[i-1].credits_per_node +
      (int)(frac*(double)(credit_table[i].credits_per_node - credit_table[i-1].credits_per_node));
    num_banked = credit_table[i-1].banked_credits +
      (int)(frac*(double)(credit_table[i].banked_credits - credit_table[i-1].banked_credits));
  } else {
    /* just use the last entry in the table */
    cred_per_node = credit_table[i_max-1].credits_per_node;
    num_banked = credit_table[i_max-1].banked_credits;
  }

  /* adjust if not using dynamic credit redistribution */
  if (!gasnetc_use_dynamic_credits && (gasneti_nodes > 1)) {
    /* changed banked credits to cred_per_node */
    cred_per_node += num_banked/(gasneti_nodes-1);
    num_banked = num_banked % (gasneti_nodes-1);  /* can just set to zero */
  }

  cred_per_node = GASNETC_MAX(cred_per_node,GASNETC_MIN_CREDITS);

  /* compute an estimate of the total number of credits */
  tot_credits = num_banked + ((int64_t)(gasneti_nodes-1))*cred_per_node;

  /* now, compute ReqRB space based on this number of credits */
  *rb_space = tot_credits * GASNETC_BYTES_PER_CREDIT;
  *banked = num_banked;
  *cpn = cred_per_node;
}


/* given credit values of banked and cpn (cred_per_node), determine the number
 * of ReqRB buffers that will be required to hold these credits.
 * Also, since the buffers come in a discrete size, adjust the cpn and banked
 * values to account for the exact number of credits these buffers will hold.
 */
static void adjust_bufspace_from_cred(int64_t *banked, int *cpn, int64_t *total_cred, int* nbuf)
{
  int64_t tot_cred = *banked + (gasneti_nodes - 1)*(*cpn);
  int64_t cred_bytes_per_buffer =  (gasnetc_ReqRB_numchunk-1)*GASNETC_CHUNKSIZE;
  int64_t cred_per_buffer = cred_bytes_per_buffer/GASNETC_BYTES_PER_CREDIT;
  int nb = tot_cred/cred_per_buffer;
#if GASNETC_CREDIT_TESTING
  if (gasneti_mynode == 0) {
    printf("Adjust: banked = %d, cpn = %d, tot = %d, cred_per_buf = %d, nbuf = %d\n",
	   (int)*banked,*cpn,(int)tot_cred,(int)cred_per_buffer,nb);
  }
#endif
  if (nb*cred_per_buffer < tot_cred) nb++;  /* another for the fractional credits */
  tot_cred = nb*cred_per_buffer;
#if GASNETC_CREDIT_TESTING
  if (gasneti_mynode == 0) printf("Adjust: adjusted nbuf = %d, tot_cred = %d\n",(int)tot_cred,nb);
#endif
  if (! gasnetc_use_dynamic_credits) {
    /* increase cpn as high as possible */
    *cpn = tot_cred/(gasneti_nodes - 1);
    gasneti_assert(*cpn >= GASNETC_MIN_CREDITS);
  }
  if (*cpn > GASNETC_MAX_CREDITS) {
    *cpn = GASNETC_MAX_CREDITS;
  }
  /* add remainder to the bank */
  *banked = tot_cred - (gasneti_nodes-1)*(*cpn);
  *total_cred = tot_cred;
  *nbuf = nb + GASNETC_REQRB_SPARES;   /* always more than credit buffer space (see bug 2462) */
#if GASNETC_CREDIT_TESTING
  if (gasneti_mynode == 0) printf("Adjust: final banked = %d, cpn = %d, tot_cred = %d, nbuf = %d\n",
				  (int)*banked,*cpn,(int)tot_cred,*nbuf);
#endif
}

/* Junk code to peek at memory layout of Catamount process
 * does not return values that make sense.
 */
#if 1
#define PRINT_ADDRESS_VALS() do{}while(0)
#else
#define PRINT_ADDRESS_VALS() if (gasneti_mynode == 0) print_address_vals()
#include <qk/process_pcb_type.h>
extern user_pcb_t *_my_pcb;
static void print_address_vals(void)
{
  int foo = 25;
  printf("My CPU                = %d\n",_my_pcb->upcb_sig_cpu_id);
  printf("My Process state      = %d\n",(int)_my_pcb->upcb_state);
  printf("My Process mode       = %d\n",(int)_my_pcb->upcb_proc_mode);
  printf("Process Start Address = %ld\n",(intptr_t)_my_pcb->upcb_start_addr);
  printf("Process Text  Address = %ld\n",(intptr_t)_my_pcb->upcb_text_base);
  printf("Process Text  Length  = %ld\n",(intptr_t)_my_pcb->upcb_text_length);
  printf("Process Data  Address = %ld\n",(intptr_t)_my_pcb->upcb_data_base);
  printf("Process Data  Length  = %ld\n",(intptr_t)_my_pcb->upcb_data_length);
  printf("Process IData Length  = %ld\n",(intptr_t)_my_pcb->upcb_idata_length);
  printf("Process Stack Address = %ld\n",(intptr_t)_my_pcb->upcb_stack_base);
  printf("Process Stack Length  = %ld\n",(intptr_t)_my_pcb->upcb_stack_length);
  printf("Process Heap  Address = %ld\n",(intptr_t)_my_pcb->upcb_heap_base);
  printf("Process Heap  Length  = %ld\n",(intptr_t)_my_pcb->upcb_heap_length);
  printf("Address of stack var foo = %ld\n", (intptr_t)&foo);
  printf("Address of Initialized Data var gasnetc_dump_stats = %ld\n",(intptr_t)&gasnetc_dump_stats);
  printf("Address of Heap var gasnetc_conn_state = %ld\n",(intptr_t)gasnetc_conn_state);
}
#endif

/* ---------------------------------------------------------------------------------
 * Initialize all the Portals resources for GASNet:
 *   - Read portals-related env vars to adjust buffer sizes
 *   - Allocate the event queues
 *   - Allocate all the Portals buffers and match list entries.
 * --------------------------------------------------------------------------------- */
extern void gasnetc_init_portals_resources(void)
{
  ptl_size_t   num_safe_events, num_am_events;
  int          i;
  int          val;
  int forced_cpn = 0;
  int forced_banked = 0;
  int forced_bufspace = 0;
  int default_cred_per_node, cred_per_node;
  int num_reqRB;
  int64_t default_banked, banked;
  int64_t default_bufspace, bufspace;
  int64_t val64;
  int64_t total_credits;
  int64_t cred_bytes_per_buffer =  (gasnetc_ReqRB_numchunk-1)*GASNETC_CHUNKSIZE;
  int64_t bytes_per_buffer = gasnetc_ReqRB_numchunk*GASNETC_CHUNKSIZE;
  int64_t cred_per_buffer = cred_bytes_per_buffer/GASNETC_BYTES_PER_CREDIT;
  
  /* read Portals specific env vars */
  #if GASNETC_FIREHOSE_LOCAL
  gasnetc_use_firehose = gasneti_getenv_yesno_withdefault("GASNET_USE_FIREHOSE", 1);
  #endif
  val64 = (int64_t)gasneti_getenv_int_withdefault("GASNET_PORTAL_PUTGET_BOUNCE_LIMIT",
                         (gasnetc_use_firehose ? 0 : (int64_t)GASNETC_PUTGET_BOUNCE_LIMIT_DFLT), 1);
  gasnetc_put_bounce_limit = (int64_t)gasneti_getenv_int_withdefault("GASNET_PORTAL_PUT_BOUNCE_LIMIT",
                                                                     val64, 1);
  if (gasnetc_put_bounce_limit > GASNETC_CHUNKSIZE) {
    if (!gasneti_mynode) {
      fprintf(stderr,
		"WARNING: Requested GASNET_PORTAL_PUT_BOUNCE_LIMIT %u reduced to %u\n",
		(unsigned int)gasnetc_put_bounce_limit, (unsigned int)GASNETC_CHUNKSIZE);
    }
    gasnetc_put_bounce_limit = GASNETC_CHUNKSIZE;
  }
  gasnetc_get_bounce_limit = (int64_t)gasneti_getenv_int_withdefault("GASNET_PORTAL_GET_BOUNCE_LIMIT",
                                                                     val64, 1);
  if (gasnetc_get_bounce_limit > GASNETC_CHUNKSIZE) { /* Don't complain about last sizeof(void *) */
    if (!gasneti_mynode) {
      fprintf(stderr,
		"WARNING: Requested GASNET_PORTAL_GET_BOUNCE_LIMIT %u reduced to %u\n",
		(unsigned int)gasnetc_get_bounce_limit, (unsigned int)(GASNETC_CHUNKSIZE - sizeof(void *)));
    }
  }
  gasnetc_get_bounce_limit = GASNETC_MIN(gasnetc_get_bounce_limit, GASNETC_CHUNKSIZE - sizeof(void *));

  gasnetc_dump_stats = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_STATS",
				 (int64_t)gasnetc_dump_stats,0);
  gasnetc_ReqSB_numchunk = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_SB_CHUNKS",
				 (int64_t)gasnetc_ReqSB_numchunk,0);
  if ((gasnetc_ReqSB_numchunk - 1) * GASNETC_CHUNKSIZE >= 0x800000) {
    if (!gasneti_mynode) {
      fprintf(stderr,
		"WARNING: Requested GASNET_PORTAL_SB_CHUNKS %u reduced to %u\n",
		(unsigned int)gasnetc_ReqSB_numchunk, (unsigned int)(0x800000/GASNETC_CHUNKSIZE));
    }
    gasnetc_ReqSB_numchunk = 0x800000 / GASNETC_CHUNKSIZE;
  }
  gasnetc_RplSB_numchunk = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_RPL_CHUNKS",
				 (int64_t)gasnetc_RplSB_numchunk,0);
  gasnetc_max_tmpmd = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_NUM_TMPMD",
				 (int64_t)GASNETC_MAX_TMP_MDS,0);
  gasnetc_msg_limit = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_MSG_LIMIT",
				 (int64_t)gasnetc_msg_limit,0);
  gasnetc_allow_packed_long = gasneti_getenv_yesno_withdefault("GASNET_PORTAL_PACKED_LONG",
				 gasnetc_allow_packed_long);
  if (gasnetc_msg_limit < gasnetc_msg_minimum) {
    gasnetc_msg_limit = gasnetc_msg_minimum;
  }
  gasnetc_epoch_duration = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_EPOCH_DURATION",
 		                (int64_t)gasnetc_epoch_duration,0);
  val = gasneti_getenv_yesno_withdefault("GASNET_PORTAL_FLOW_CONTROL",
					    gasnetc_use_flow_control);
  gasnetc_use_flow_control = val;
  val = gasneti_getenv_yesno_withdefault("GASNET_PORTAL_DYNAMIC_CREDITS",
					    gasnetc_use_dynamic_credits);
  gasnetc_use_dynamic_credits = val;
  if (val) gasnetc_use_flow_control = 1;  /* implied by use of dynamic credits */
  val = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_MAX_CRED_PER_NODE",
					    (int64_t)gasnetc_max_cpn,0);
  gasnetc_max_cpn = val;
  val = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_SAFE_LIMIT",
					    (int64_t)gasnetc_safe_poll_limit,0);
  if (val >= 0) gasnetc_safe_poll_limit = val;
  val = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_AM_LIMIT",
					    (int64_t)gasnetc_am_poll_limit,0);
  if (val >= 0) gasnetc_am_poll_limit = val;
  val = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_SYS_LIMIT",
					    (int64_t)gasnetc_sys_poll_limit,0);
  if (val >= 0) gasnetc_sys_poll_limit = val;
  gasnetc_shutdown_seconds = gasneti_get_exittimeout(shutdown_max, 5., 0.25, 0.);

#if GASNETC_CREDIT_TESTING
  gasnetc_debug_node = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_DEBUG_NODE",
				 (int64_t)gasnetc_debug_node,0);
#endif

  PRINT_ADDRESS_VALS();

  /* now determine default values of credit/bufsapce parameters based on gasneti_nodes */
  compute_default_credits(&default_cred_per_node, &default_banked, &default_bufspace);

  val = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_CRED_PER_NODE",
					    (int64_t)-1,0);
  if (val >= 0) {
    forced_cpn = 1;
    cred_per_node = val;
    if ((cred_per_node < GASNETC_MIN_CREDITS) && (gasneti_mynode == 0)) {
      gasneti_fatalerror("GASNET_PORTAL_CRED_PER_NODE=%d but MIN cred_per_node = %d",
			 cred_per_node,GASNETC_MIN_CREDITS);
    }
  } 
  val64 = gasneti_getenv_int_withdefault("GASNET_PORTAL_BANKED_CREDITS",
					 (int64_t)-1,0);
  if (val64 >= 0) {
    forced_banked = 1;
    banked = val64;
  }
  val64 = gasneti_getenv_int_withdefault("GASNET_PORTAL_AMRECV_SPACE",
					 (int64_t)-1,1);
  if (val64 >= 0) {
    forced_bufspace = 1;
    bufspace = val64;
  }
#if GASNETC_CREDIT_TESTING
  if (gasneti_mynode == 0) {
    printf("USE_FLOW_CONTROL=%d, DYNAMIC_CREDITS=%d\n",
	   gasnetc_use_flow_control,gasnetc_use_dynamic_credits);
    printf("DEFAULT: banked = %ld, cpn = %d, bufspace = %ld\n",
	   default_banked,default_cred_per_node,default_bufspace);
    if (forced_cpn)      printf("FORCED cpn      = %d\n",cred_per_node);
    if (forced_banked)   printf("FORCED banked   = %ld\n",banked);
    if (forced_bufspace) printf("FORCED bufspace = %ld\n",bufspace);
  }
#endif

  /* logic nightmare on how to init per-node & banked credits and ReqRB space */
  /* at end, have cred_per_node, banked, total_credits and num_reqRB set */
  if (gasneti_nodes == 1) {
    /* will never receive an AM, just provide dummy values */
    cred_per_node = 0;
    banked = 0;
    gasnetc_use_flow_control = 0;
    gasnetc_use_dynamic_credits = 0;
    num_reqRB = 1;       /* always alloc one */
    total_credits = 10;  /* will determine number of events in AM Queue */
    
  } else if (! gasnetc_use_flow_control) {
    cred_per_node = 0;
    banked = 0;
    if (!forced_bufspace) bufspace = default_bufspace;
    num_reqRB = (int)(bufspace/((double)bytes_per_buffer)) + 1;
    bufspace = num_reqRB * bytes_per_buffer;
    total_credits = 500 + num_reqRB*cred_per_buffer;  /* size of AM event queue */

  } else if (! gasnetc_use_dynamic_credits) {
    /* use flow control, but not dynamic credit management */
    /* ignore forced_banked, dont bank credits, distribute all to remote nodes, fixed for runtime */
    banked = 0;
    if (forced_cpn) {
#if GASNETC_CREDIT_TESTING
      int cpn_saved = cred_per_node;
#endif
      adjust_bufspace_from_cred(&banked, &cred_per_node, &total_credits, &num_reqRB);

#if GASNETC_CREDIT_TESTING
      /* MLW: hack to force credits to what we specify in env vars and ignore extras */
      cred_per_node = cpn_saved;
#endif

    } else if (forced_bufspace) {
      /* first, use bufspace provided to compute cred_per_node */
      total_credits = (int)(bufspace/((double)cred_bytes_per_buffer));
      cred_per_node = total_credits/(gasneti_nodes - 1);
      cred_per_node = GASNETC_MAX(cred_per_node,GASNETC_MIN_CREDITS); /* this is required */
      /* compute number of buffers and adjust credits */
      adjust_bufspace_from_cred(&banked,&cred_per_node,&total_credits,&num_reqRB);

    } else {
      /* use default credits per node from table */
      cred_per_node = default_cred_per_node;
      adjust_bufspace_from_cred(&banked,&cred_per_node,&total_credits,&num_reqRB);
      gasneti_assert(cred_per_node >= GASNETC_MIN_CREDITS);
    }

  } else {
    /* using flow control with dynamic credit distribution */
    if (forced_cpn) {
      if (forced_banked) {
#if GASNETC_CREDIT_TESTING
	int64_t banked_save = banked;
#endif
	/* cpn && banked, ignore forced_bufspace if set in env */
	/* NOTE: ignore forced_banked if set, overspecified */
	adjust_bufspace_from_cred(&banked,&cred_per_node,&total_credits,&num_reqRB);
	gasneti_assert(cred_per_node >= GASNETC_MIN_CREDITS);

#if GASNETC_CREDIT_TESTING
	/* WARNING WARNING: Remove this for final checkin ! ! ! */
	/* DEBUG: force banked to be what we requested if less than actual */
	if (banked > banked_save) banked = banked_save;
#endif

      } else {
	/* ! forced_banked */
	if (forced_bufspace) {
	  /* cpn && bufspace - dont use default banked, just let banked be remainder */
	  int64_t c_space;
	  total_credits = cred_per_node * (gasneti_nodes-1);
	  c_space = total_credits*GASNETC_BYTES_PER_CREDIT;
	  if (c_space > bufspace) {
	    gasneti_fatalerror("With GASNET_PORTAL_CRED_PER_NODE=%d, require bufspace = %ld greater than GASNET_PORTAL_BUFFER_SPACE_MB=%ld",
			       cred_per_node,c_space,bufspace);
	  }
	  banked = 0;
	  adjust_bufspace_from_cred(&banked,&cred_per_node,&total_credits,&num_reqRB);

	} else {
	  /* cpn (only), do we used default banked credits? */
	  banked = default_banked;
	  adjust_bufspace_from_cred(&banked,&cred_per_node,&total_credits,&num_reqRB);
	  
	}
      }
    } else {
      /* cpn not specified in this section */
      if (forced_banked) {
	if (forced_bufspace) {
	  /* banked && bufspace */
	  /* first, compute total credits given bufspace, then cpn */
	  total_credits = bufspace / GASNETC_BYTES_PER_CREDIT;
	  if (total_credits < banked) {
	    gasneti_fatalerror("GASNET_PORTAL_BUFFER_SPACE=%ld => %ld Credits, less than GASNET_PORTAL_BANKED_CREDITS=%ld",bufspace,total_credits,banked);
	  }
	  cred_per_node = (total_credits - banked)/(gasneti_nodes-1);
	  if (cred_per_node < GASNETC_MIN_CREDITS) {
	    gasneti_fatalerror("GASNET_PORTAL_BUFFER_SPACE=%ld => %ld Total Credits.  With GASNET_PORTAL_BANKED_CREDITS=%ld the remaining credits imply credit_per_node=%d < 4 (MINIMUM)",bufspace,total_credits,banked,cred_per_node);
	  }
	  adjust_bufspace_from_cred(&banked,&cred_per_node,&total_credits,&num_reqRB);

	} else {
	  /* banked (only), use default cred_per_node */
	  cred_per_node = default_cred_per_node;
	  adjust_bufspace_from_cred(&banked,&cred_per_node,&total_credits,&num_reqRB);
	}
      } else {
	if (forced_bufspace) {
	  /* bufspace (only) */
	  /* compute cred_per_node from bufspace */
	  total_credits = bufspace / GASNETC_BYTES_PER_CREDIT;
	  if (total_credits < GASNETC_MIN_CREDITS) {
	    gasneti_fatalerror("GASNET_PORTAL_BUFFER_SPACE=%ld => %ld Total Credits => credit_per_node=%d < 4 (MINIMUM)",bufspace,total_credits,cred_per_node);
	  }
	  cred_per_node = total_credits/(gasneti_nodes - 1);
	  banked = 0;
	  adjust_bufspace_from_cred(&banked,&cred_per_node,&total_credits,&num_reqRB);
	  
	} else {
	  /* nothing forced, use defaults */
	  banked = default_banked;
	  cred_per_node = default_cred_per_node;
	  adjust_bufspace_from_cred(&banked,&cred_per_node,&total_credits,&num_reqRB);

	}
      }
    }
  }
  if (gasnetc_use_flow_control) {
    gasneti_assert_always(cred_per_node <= GASNETC_MAX_CREDITS);
    gasneti_assert_always(cred_per_node >= GASNETC_MIN_CREDITS);
  }
  gasneti_assert_always(gasnetc_lender_limit < GASNETC_MAX_CREDITS);
  gasneti_assert_always(gasnetc_revoke_limit < GASNETC_MAX_CREDITS);
  gasnetc_ReqRB_pool_size = num_reqRB;
  gasnetc_total_credits = total_credits;
  num_am_events = total_credits;
  gasneti_weakatomic_set(&gasnetc_AMRequest_count, 0, 0);
  gasneti_weakatomic_set(&gasnetc_scavenge_inflight, 0, 0);

  /* init the credit vars of the connection state */
  {
    /* if not using dynamic credits, distribute the extra credits
     * in banked to the first banked nodes, remembering that we
     * never grant credits to ourselves.
     */
    int extra_cutoff = (gasneti_mynode < banked ? banked+1 : banked);
    gasneti_mutex_lock(&gasnetc_scavenge_lock);
    for (i = 0; i < gasneti_nodes; i++) {
      gasnetc_conn_t *state = &gasnetc_conn_state[i];
      if (i == gasneti_mynode) {
	/* dont loan yourself any, nor grant yourself any */
	state->LoanCredits = 0;
	state->SendCredits = 0;
      } else {
	state->LoanCredits = cred_per_node;
	state->SendCredits = cred_per_node;
	if (!gasnetc_use_dynamic_credits) {
	  /* distribute extra credits */
	  if (i < extra_cutoff) state->LoanCredits++;
	  /* was I given an extra credit on node i? */
	  if ((i < banked) && (gasneti_mynode <= banked)) {
	    state->SendCredits++;
	  } else if ((i >= banked) && (gasneti_mynode < banked)) {
	    state->SendCredits++;
	  }
	}
      }
      state->link.next = state->link.prev = GASNETC_DLL_NULL;
      if (gasnetc_use_dynamic_credits &&
	  (state->LoanCredits > GASNETC_MIN_CREDITS)) gasnetc_scavenge_list_add(i,1);
      state->SendStalls = 0;
      state->SendInuse = 0;
      state->SendMax = 0;
      state->SendRevoked = 0;
      state->LoanRequested = 0;
      state->LoanGiven = 0;
#if GASNETC_CREDIT_TESTING
      state->SendStalls_tot = 0;
      state->SendMax_tot = 0;
      state->LoanGiven_tot = 0;
      state->LoanRequested_tot = 0;
      state->LoanRevoked_tot = 0;
      state->LoanReqRevoke_tot = 0;
#endif    
    }
    gasneti_mutex_unlock(&gasnetc_scavenge_lock);
  }
#if GASNETC_CREDIT_TESTING
  if (gasnetc_use_dynamic_credits && (gasnetc_debug_node >= 0) && (gasneti_mynode == gasnetc_debug_node)) {
    printf("After Scavenge List Initialization:\n");
    gasnetc_print_scavenge_list();
  }
#endif
  if (!gasnetc_use_dynamic_credits) {
    /* banked value was a remainder and was distribute to first banked nodes */
    gasneti_assert_always(banked < gasneti_nodes);
    banked = 0;
  }
  gasneti_semaphore_init(&gasnetc_banked_credits, banked, 0);

  if (gasnetc_dump_stats) GASNETC_DUMP_CREDITS(0);
				
#if GASNETC_CREDIT_TESTING
  if (gasneti_mynode == 0) {
    const char *filename = "initial_vals";
    FILE *fh = fopen(filename,"w");
    if (fh == NULL) {
      gasneti_fatalerror("Could not open %s for writing",filename);
    }
    fprintf(fh,"Static Segsize      = %f10.2 MB\n", ((double)gasnetc_static_segsize/(1024.0*1024.0)));
    fprintf(fh,"ReqRB_Pool_Size     = %d\n", gasnetc_ReqRB_pool_size);
    fprintf(fh,"ReqRB_numchunk      = %d\n", (int)gasnetc_ReqRB_numchunk);
    fprintf(fh,"ReqSB_numchunk      = %d\n", (int)gasnetc_ReqSB_numchunk);
    fprintf(fh,"RplSB_numchunk      = %d\n", (int)gasnetc_RplSB_numchunk);
    fprintf(fh,"Max_tmpmd           = %d\n", gasnetc_max_tmpmd);
    fprintf(fh,"Msg_limit           = %d\n", gasnetc_msg_limit);
    fprintf(fh,"Sys_poll_limit      = %d\n", gasnetc_sys_poll_limit);
    fprintf(fh,"Safe_poll_limit     = %d\n", gasnetc_safe_poll_limit);
    fprintf(fh,"AM_poll_limit       = %d\n", gasnetc_am_poll_limit);
    fprintf(fh,"Use Flow control    = %d\n", gasnetc_use_flow_control);
    fprintf(fh,"Use Dynamic credits = %d\n", gasnetc_use_dynamic_credits);
    fprintf(fh,"Total credits       = %ld\n", total_credits);
    fprintf(fh,"Banked credits      = %ld\n", banked);
    fprintf(fh,"Credits per Node    = %d\n", cred_per_node);
    fprintf(fh,"Epoch Duration      = %d\n", gasnetc_epoch_duration);
    fprintf(fh,"Epoch Lender Limit  = %d\n", gasnetc_lender_limit);
    fprintf(fh,"Epoch Revoke Limit  = %d\n", gasnetc_revoke_limit);
    fprintf(fh,"Allow Packed AMLong = %d\n", gasnetc_allow_packed_long);
    fprintf(fh,"Shutdown Seconds    = %f\n", gasnetc_shutdown_seconds);
    fclose(fh);
  }
#endif

  GASNETI_TRACE_PRINTF(C,("Portals_Init: ReqRB_Pool_size = %d",gasnetc_ReqRB_pool_size));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: ReqRB_numchunk  = %d",(int)gasnetc_ReqRB_numchunk));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: ReqSB_numchunk  = %d",(int)gasnetc_ReqSB_numchunk));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: RplSB_numchunk  = %d",(int)gasnetc_RplSB_numchunk));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: max_tmpmd       = %d",gasnetc_max_tmpmd));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: msg_limit       = %d",gasnetc_msg_limit));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: safe_poll_limit = %d",gasnetc_safe_poll_limit));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: am_poll_limit   = %d",gasnetc_am_poll_limit));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: sys_poll_limit  = %d",gasnetc_sys_poll_limit));

  GASNETI_TRACE_PRINTF(C,("Portals_Init: flow_control    = %d",gasnetc_use_flow_control));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: dynamic_credits = %d",gasnetc_use_dynamic_credits));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: epoch_duration  = %d",gasnetc_epoch_duration));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: total_credits   = %ld",total_credits));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: banked_credits  = %ld",banked));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: credits_per_node= %d",cred_per_node));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: lender_limit    = %d",gasnetc_lender_limit));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: revoke_limit    = %d",gasnetc_revoke_limit));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: shutdown seconds= %f",gasnetc_shutdown_seconds));
#if GASNETC_USE_SANDIA_ACCEL
  GASNETI_TRACE_PRINTF(C,("Portals_Init: use_accelerated = %d",gasnetc_use_accel));
#endif
  
  /* Init the temp md counter to max allowed */
  gasneti_semaphore_init(&gasnetc_tmpmd_tickets, gasnetc_max_tmpmd, gasnetc_max_tmpmd);

  /* keep a counter of number of send tickets available */
  gasneti_semaphore_init(&gasnetc_send_tickets, gasnetc_msg_limit, gasnetc_msg_limit);

  /* Create two EQs:
   * gasnetc_SAFE_EQ:  Used to reclaim buffer space.  Always safe to poll on this
   *    since it never consumes additional resources and never resursively polls.
   *    Bound to the following MDs:  RplSB, TMPMDs, RARSRC and ReqSB.
   *    Puts/Gets limited by msg_limit: 2 events each (SE+ACK or SE+RE)
   *    Number of AMs limited by number of ReqSB chunks: 3 events each (SE+PE) + possibly
   *    PE event as data segment of AM Reply in RARSRC.
   *    Replies limited by RplSB: 1 event (SE).
   *    In reality, SE events get generated almost immediately and should get processed
   *    rapidly.  Can probably get by with 75% of this number of events.
   * gasnetc_AM_EQ:   Used to process AM Request messages.  Cannot poll on this
   *    eq unless there are at least one RplSB and one TMPMD buffer available.
   *    Bound to the following MDs:  ReqRB, RARAM.
   *    If using flow control
   *       Size = total number of credits available + change.
   *    else, we guess:
   *       Size = Num AMLong data puts + Num AM Requests
   */

  num_safe_events = 2*gasnetc_msg_limit + gasnetc_RplSB_numchunk + 3*gasnetc_ReqSB_numchunk;
  if (num_safe_events > 10000) {
    /* should never need this many events, since SE events are always generated and SHOULD
     * be processed almost immediately, and this accounts for 1/2 all of the possible
     * events generated.
     */
    num_safe_events = (int)((double)num_safe_events * 0.75);
  }

  gasnetc_SAFE_EQ = gasnetc_eq_alloc(num_safe_events, "SAFE_EQ", GASNETC_EQ_HANDLER);
  gasnetc_AM_EQ = gasnetc_eq_alloc(num_am_events, "AM_EQ", GASNETC_EQ_HANDLER);

  /* Finally, init the Remote Access Region and allocate the AM Buffer space */
  RAR_init();
  ReqRB_init();
  ReqSB_init();  /* required to init ReqSB after ReqRB, since it must go on end of list */
  RplSB_init();

  /* Initialize firehose */
  #if GASNETC_FIREHOSE_LOCAL
  if (gasnetc_use_firehose) {
    size_t firehose_mem = GASNETC_FIREHOSE_MAXREGIONS * GASNETC_FIREHOSE_MAXREGION_SIZE;

    firehose_init(firehose_mem, GASNETC_FIREHOSE_MAXREGIONS, GASNETC_FIREHOSE_MAXREGION_SIZE,
                  NULL, 0, FIREHOSE_INIT_FLAG_LOCAL_ONLY, &gasnetc_firehose_info);
    gasnetc_AMMaxLong = gasnetc_firehose_info.max_LocalPinSize - GASNET_PAGESIZE;
  } else {
    gasnetc_AMMaxLong = 1024 * 1024 * 1024; /* 1GB */
  }
  #elif GASNETC_FIREHOSE_REMOTE
    #error "Firehose REMOTE is not yet supported"
  #endif

#if 0
#ifndef GASNETC_USE_EQ_HANDLER
  /* Enable the progress function */
  GASNETI_TRACE_PRINTF(C,("Enabling Portals polling function (No EQ Handler)"));
  GASNETI_PROGRESSFNS_ENABLE(gasnete_pf_portals_poll,BOOLEAN);
#endif
#endif
}

#if 0  /* Note: this is currently unused -PHH */
/* ---------------------------------------------------------------------------------
 * Pre-exit function
 * Attempts to poll until all local resources have been reclaimed,
 * otherwise errors can occur when the resources are released.
 * MLW: Currently not in use.
 * --------------------------------------------------------------------------------- */
extern void gasnetc_portals_preexit(int do_trace)
{
  /* we poll until all resources are freed */
  int inuse = 1;
  int iter = 0;
  while ((iter < 1000) && (inuse > 0)) {
    int rplsb_cnt, reqsb_cnt, tmpmd_cnt;
    gasneti_AMPoll();
    rplsb_cnt = gasnetc_RplSB.inuse;
    reqsb_cnt = gasnetc_ReqSB.inuse;
    tmpmd_cnt  = gasnetc_max_tmpmd - gasnetc_num_tickets(&gasnetc_tmpmd_tickets);
    if (do_trace) {
      GASNETI_TRACE_PRINTF(C,("PRE_EXIT: iter %d RplSB_count = %d ReqSB_count = %d tmpmd_count = %d",iter,rplsb_cnt,reqsb_cnt,tmpmd_cnt));
    }
    inuse = rplsb_cnt + reqsb_cnt + tmpmd_cnt;
    iter++;
  } 
}
#endif

/* ---------------------------------------------------------------------------------
 * Release Portals resources
 *   - The proc_id map
 *   - Remove MDs and match-list entries
 *   - Free the buffers used for bounce, send/recv
 * --------------------------------------------------------------------------------- */
extern void gasnetc_portals_exit(void)
{

#define DO_CLEANUP_PORTALS 0
#if DO_CLEANUP_PORTALS
  {
    ptl_event_t ev;

    #if GASNETC_FIREHOSE_LOCAL && GASNETC_USE_MDUPDATE
      firehose_fini();
      gasnetc_eq_free(gasnetc_EMPTY_EQ);
    #endif

    sys_exit();

    RplSB_exit();
    ReqRB_exit();
    ReqSB_exit();
    RAR_exit();

    /* remove the event queues */
    gasnetc_eq_destroy(gasnetc_SAFE_EQ);
    gasnetc_SAFE_EQ = NULL;
    gasnetc_eq_destroy(gasnetc_AM_EQ);
    gasnetc_AM_EQ = NULL;

    /* free the proc id map */
    gasneti_free(gasnetc_procid_map);

    /* free the AM connection state array */
    gasneti_free(gasnetc_conn_state);

    GASNETC_PTLSAFE(PtlNIFini(gasnetc_ni_h));
  }
#endif

#if HAVE_PMI_CNOS
  if (PMI_SUCCESS != PMI_Finalize()) {
    gasneti_fatalerror("Error in PMI_Finalize");
  }
#elif PLATFORM_OS_CNL
  /* inform cnos of clean exit
   * MLW: dont understand the args to this yet!!!
   */
  cnos_pm_barrier(1);
#endif
}

/* ------------------------------------------------------------------------------------
 * --------------------------------------------------------------------------------- */
/* Portals polling function.  Process the event queues.
 * 
 */

static const char* poll_name[] = {"NO_POLL","SAFE_POLL","FULL_POLL"};
extern void gasnetc_portals_poll(gasnetc_pollflag_t poll_type)
{
  int processed = 0;
  ptl_event_t ev;
  unsigned safe_cnt = 0;
  unsigned am_cnt = 0;

#if defined(GASNET_DEBUG) || defined(GASNETI_STATS_OR_TRACE)
  static int poll_level = 0;
  poll_level++;
  GASNETI_TRACE_PRINTF(C,("Enter Poll with %s, level %d",poll_name[poll_type],poll_level));
#endif

  GASNETI_TRACE_PRINTF(C,("POLL: SendTickets=%d  TmpMD_Tickets=%d",gasnetc_num_tickets(&gasnetc_send_tickets),gasnetc_num_tickets(&gasnetc_tmpmd_tickets)));

  /* should never be called with NO_POLL */
  gasneti_assert(poll_type != GASNETC_NO_POLL);

  /* always poll on the system queue, adds .074 usec to poll, cost of extra PtlEQGet call */
  gasnetc_sys_poll(GASNETC_EQ_TRYLOCK);

  /* always try to get a few events from the SAFE eq first 
   * all puts and gets generate two events so need to reap these queues faster
   * to prevent send_ticket starvation
   */
  while (safe_cnt < gasnetc_safe_poll_limit) {
    if ( gasnetc_get_event(gasnetc_SAFE_EQ, &ev, GASNETC_EQ_TRYLOCK) ) {
#if GASNETI_STATS_OR_TRACE
      gasnete_threaddata_t *td = gasnete_mythread();
      GASNETI_TRACE_PRINTF(C,("Got event %s from SAFE_EQ, md=%lu, mbits=0x%lx, th_id=%d",ptl_event_str[ev.type],(ulong)ev.md_handle,(unsigned long)ev.match_bits,td->threadidx));
#endif
      GASNETC_CALL_EQ_HANDLER(ev);
      processed++;
      safe_cnt++;
    } else {
      /* no ready events or another poller holds the lock, stop trying */
      break;
    }
  }

  if (poll_type == GASNETC_FULL_POLL) {
    gasnetc_threaddata_t *th = gasnetc_mythread();

    /* Accumulate all resources needed to execute an AM Request handler and reply.
     * If we fail, just return to caller and assume they will continue polling.
     * We do not release resources that we have acquired since the number of
     * threads is small compared to the number of resources so its ok to hold
     * onto them.  Others will free up soon.
     */

    while (am_cnt < gasnetc_am_poll_limit) {
      /* First, in worst case we will need two send tickets to reply with LONG */
      while (th->snd_tickets < 2) {
	if (!gasnetc_alloc_ticket(&gasnetc_send_tickets)) {
	  goto out;
	} 
	th->snd_tickets++;
      }

      /* Second, we may need a tmpmd to complete a long reply, make sure we have
       * a ticket cached, unless using firehose
       */
      if (!gasnetc_use_firehose && (th->tmpmd_tickets == 0)) {
	if (! gasnetc_alloc_ticket(&gasnetc_tmpmd_tickets)) {
	  goto out;
	} 
	th->tmpmd_tickets++;
      }

      /* Finally, we will need a RplSB chunk, if not already cached, try to alloc one */
      if (th->rplsb == NULL) {
        th->rplsb = gasnetc_chunk_alloc_no_off(&gasnetc_RplSB,GASNETC_CHUNKSIZE);
        if (th->rplsb == NULL) {
	  goto out;
	} 
      }

      GASNETI_TRACE_PRINTF(C,("PtlPoll: FULL thread=0x%p have_rplsb=%d",th,(th->rplsb != NULL)));

      /* if we got here, we have enough resources to poll the AM queue */
      if (gasneti_mutex_trylock(&gasnetc_AM_EQ->lock)) goto out;
      if (gasnetc_get_event(gasnetc_AM_EQ, &ev, GASNETC_EQ_NOLOCK) ) {
	GASNETI_TRACE_PRINTF(C,("Got event %s from AM_EQ, md=%lu, mbits=0x%lx th_id=%d",ptl_event_str[ev.type],(ulong)ev.md_handle,(ulong)ev.match_bits,th->threadidx));
	GASNETC_CALL_EQ_HANDLER(ev);
        gasneti_mutex_assertunlocked(&gasnetc_AM_EQ->lock);
	processed++;
	am_cnt++;
      } else {
        gasneti_mutex_unlock(&gasnetc_AM_EQ->lock);
	goto out;
      }
    } /* end while */
  }

  out:
#if defined(GASNET_DEBUG) || defined(GASNETI_STATS_OR_TRACE)
  GASNETI_TRACE_PRINTF(C,("Leave Poll with %s level %d",poll_name[poll_type],poll_level));
  poll_level--;
#endif

  GASNETI_TRACE_EVENT_VAL(C, EVENT_REAP, processed);
}

/* ------------------------------------------------------------------------------------
 * Jump table to determine which event handler to run.
 * Only used in case when we register a Portals event handler with an EQ, otherwise
 * functions are called through a function pointer stored in ev->md.user_prt
 * --------------------------------------------------------------------------------- */
extern void gasnetc_event_handler(ptl_event_t *ev)
{
#if GASNETC_USE_EQ_HANDLER
  unsigned int which_md = (unsigned int)(uintptr_t)ev->md.user_ptr;

  gasneti_assert(which_md < GASNETC_NUM_MD);

  switch (which_md) {
  case GASNETC_RARAM_MD:
    RARAM_event(ev);
    break;

  case GASNETC_RARSRC_MD:
    RARSRC_event(ev);
    break;

  case GASNETC_REQSB_MD:
    ReqSB_event(ev);
    break;

  case GASNETC_TMP_MD:
    TMPMD_event(ev);
    break;

  case GASNETC_REQRB_MD:
    ReqRB_event(ev);
    break;

  case GASNETC_RPLSB_MD:
    RplSB_event(ev);
    break;
    
  case GASNETC_CB_MD:
    CB_event(ev);
    break;

  case GASNETC_SYS_SEND_MD:
  case GASNETC_SYS_RECV_MD:
    sys_event(ev);
    break;

  default:
    gasneti_fatalerror("Invalid MD [%i] for event %s",(int)which_md,ptl_event_str[ev->type]);
  }
#endif
}

/* ------------------------------------------------------------------------------------
 * Called when the gasnet_core exits.
 * Dump locally collected statistics.
 * --------------------------------------------------------------------------------- */
extern void gasnetc_ptl_trace_finish(void)
{
  GASNETI_STATS_PRINTF(C,("TMPMD HWM:                         %i",gasnetc_tmpmd_hwm));
  GASNETI_STATS_PRINTF(C,("ReqSB CHUNK HWM:                   %i/%i",gasnetc_ReqSB.hwm,gasnetc_ReqSB.numchunks));
  GASNETI_STATS_PRINTF(C,("RplSB CHUNK HWM:                   %i/%i",gasnetc_RplSB.hwm,gasnetc_RplSB.numchunks));
}

GASNETI_INLINE(gasnetc_send_ticket_stall)
void gasnetc_send_ticket_stall(gasnetc_pollflag_t pollflag) {
  while( !gasnetc_alloc_ticket(&gasnetc_send_tickets) ) {
#if 1 /* Currently all callers use GASNETC_FULL_POLL */
    gasneti_assert(pollflag == GASNETC_FULL_POLL);
    gasneti_AMPoll();
#else    
    switch (pollflag) {
    case GASNETC_NO_POLL:
      gasneti_fatalerror("gasnetc_(get|put)msg: msg limit but NO_POLL allowed");
      break;
    case GASNETC_SAFE_POLL:
      gasnetc_portals_poll(pollflag);
      break;
    case GASNETC_FULL_POLL:
      gasneti_AMPoll();
      break;
    }
#endif
  }
}

/* ------------------------------------------------------------------------------------
 * This function does the actual Portals Get operation for the extended API Get
 * operations.
 * dest       => Address of destination
 * node       => Which GASNet node to send message to
 * src        => Address of source, must be in remote RAR
 * nbytes     => Length of message
 * match_bits => Destination MD, may be modified in case of bb or fh
 * pollflag   => What type of polling to allow before Get operation is posted.
 * If we have reached the put/get limit, we poll as directed.
 *
 * Returns the number of bytes actually initiated
 * --------------------------------------------------------------------------------- */
size_t gasnetc_getmsg(void *dest, gasnet_node_t node, void *src, size_t nbytes,
		    ptl_match_bits_t match_bits, gasnetc_pollflag_t pollflag)
{
  ptl_process_id_t target_id = gasnetc_procid_map[node].ptl_id;
  ptl_handle_md_t md_h;
  ptl_ac_index_t ac_index = GASNETC_PTL_AC_ID;
  ptl_size_t local_offset;
  ptl_size_t remote_offset = GASNETC_PTL_OFFSET(node,src);
  void* bb;
  
  gasneti_assert(remote_offset < gasneti_seginfo[node].size);

  /* stall here if too many puts/gets in progress */
  gasnetc_send_ticket_stall(pollflag);

  /* Determine destination MD for Ptl Get */
  if (gasnetc_in_local_rar(dest,nbytes)) {
    md_h = gasnetc_RARSRC.md_h;
    local_offset = GASNETC_PTL_OFFSET(gasneti_mynode,dest);
    nbytes = MIN(nbytes,GASNETC_PTL_MAX_TRANS_SZ);
    GASNETI_TRACE_EVENT(C, GET_RAR);
  } else if ( (nbytes <= gasnetc_get_bounce_limit)  &&
	      (bb = gasnetc_chunk_alloc_withpoll(&gasnetc_ReqSB, nbytes, &local_offset, 1, GASNETC_SAFE_POLL) )) {
    /* Encode dest addr in BB chunk for later copy */
    md_h = gasnetc_ReqSB.md_h;
    /* store the dest address at this location */
    *(uintptr_t*)bb = (uintptr_t)dest;
    match_bits |= ((ptl_match_bits_t)local_offset << 32); /* W/O the shift by sizeof(void*) */
    /* Let portals use the rest of the chunk */
    local_offset += sizeof(void*);
    GASNETI_TRACE_EVENT(C, GET_BB);
  } else GASNETC_IF_USE_FIREHOSE (
    /* alloc a firehose for the destination region */
    gasnetc_fh_op_t *op = gasnetc_fh_aligned_local_pin(dest, nbytes);
    const firehose_request_t *fh_loc = &op->fh[0];
    md_h = fh_loc->client;
    local_offset = (uintptr_t)dest - fh_loc->addr;
    nbytes = MIN(nbytes, (fh_loc->len - local_offset));
    match_bits |= ((ptl_match_bits_t)(op->addr.fulladdr) << 32); /* encode "op" for later release */
    GASNETI_TRACE_EVENT(C, GET_FH);
  ) else {
    /* alloc a temp md for the destination region */
    nbytes = MIN(nbytes,GASNETC_PTL_MAX_TRANS_SZ);
    md_h = gasnetc_alloc_tmpmd_withpoll(dest, nbytes);
    local_offset = 0;
    GASNETI_TRACE_EVENT(C, GET_TMPMD);
  }

  /* Issue Ptl Get operation */
  GASNETI_TRACE_PRINTF(C,("getmsg: match_bits = 0x%lx, local_off=%ld, remote_off=%ld, nbytes=%ld",(uint64_t)match_bits,(long)local_offset,(long)remote_offset,(long)nbytes));

  GASNETC_PTLSAFE(PtlGetRegion(md_h, local_offset, nbytes, target_id, GASNETC_PTL_RAR_PTE, ac_index, match_bits, remote_offset));

  return nbytes;
}

/* ------------------------------------------------------------------------------------
 * This function does the actual Portals Put operation for the extended API Put
 * operations.
 * If we have reached the put/get limit, we poll as directed.
 * dest       => Address of destination, must be in remote RAR
 * node       => Which GASNet node to send message to
 * src        => Address of message source
 * nbytes     => Length of message
 * match_bits => Destination MD, may be modified in case of wait_lcc or fh
 * lcc        => Pointer to weakatomic counter of pending local completions (or NULL).
 * pollflag   => What type of polling to allow before Put operation is posted.
 *
 * Returns the number of bytes actually initiated
 * --------------------------------------------------------------------------------- */
size_t gasnetc_putmsg(void *dest, gasnet_node_t node, void *src, size_t nbytes,
		    ptl_match_bits_t match_bits, gasneti_weakatomic_t *lcc,
		    gasnetc_pollflag_t pollflag)
{
  ptl_size_t local_offset = 0;
  ptl_size_t remote_offset = GASNETC_PTL_OFFSET(node,dest);
  ptl_handle_md_t md_h;
  ptl_process_id_t target_id = gasnetc_procid_map[node].ptl_id;
  ptl_ac_index_t ac_index = GASNETC_PTL_AC_ID;
  ptl_hdr_data_t hdr_data = 0;
  int inc_lcc = (lcc != NULL);
  void* bb;
  
  gasneti_assert(remote_offset < gasneti_seginfo[node].size);

  /* stall here if too many puts/gets in progress */
  gasnetc_send_ticket_stall(pollflag);

  /* Determine source MD for Ptl Put */
  if (gasnetc_in_local_rar(src,nbytes)) {
    md_h = gasnetc_RARSRC.md_h;
    local_offset = GASNETC_PTL_OFFSET(gasneti_mynode,src);
    nbytes = MIN(nbytes,GASNETC_PTL_MAX_TRANS_SZ);
    GASNETI_TRACE_EVENT(C, PUT_RAR);
  } else if ( (nbytes <= gasnetc_put_bounce_limit)  &&
	      (bb = gasnetc_chunk_alloc_withpoll(&gasnetc_ReqSB,nbytes, &local_offset, 1, GASNETC_SAFE_POLL)) ) {
    md_h = gasnetc_ReqSB.md_h;
    /* copy the src data to the bounce buffer */
    memcpy(bb,src,nbytes);
    /* store the local offset in the upper bits of the match bits */
    match_bits |= ((ptl_match_bits_t)local_offset << 32);
    inc_lcc = 0; /* Already completed locally */ 
    GASNETI_TRACE_EVENT(C, PUT_BB);
  } else GASNETC_IF_USE_FIREHOSE (
    /* alloc a firehose for the source region */
    gasnetc_fh_op_t *op = gasnetc_fh_aligned_local_pin(src, nbytes);
    const firehose_request_t *fh_loc = &op->fh[0];
    md_h = fh_loc->client;
    local_offset = (uintptr_t)src - fh_loc->addr;
    nbytes = MIN(nbytes, (fh_loc->len - local_offset));
    match_bits |= ((ptl_match_bits_t)(op->addr.fulladdr) << 32); /* encode "op" for later release */
    GASNETI_TRACE_EVENT(C, PUT_FH);
  ) else {
    /* alloc a temp md for the source region */
    nbytes = MIN(nbytes,GASNETC_PTL_MAX_TRANS_SZ);
    md_h = gasnetc_alloc_tmpmd_withpoll(src, nbytes);
    local_offset = 0;
    GASNETI_TRACE_EVENT(C, PUT_TMPMD);
  }
  if (inc_lcc) {
    /* increment local completion flag and indicate to event handler to decrement */
    gasneti_assert(lcc != NULL);
    gasneti_weakatomic_increment(lcc, 0);
    match_bits |= GASNETC_PTL_MSG_DOLC;
  }

  GASNETI_TRACE_PRINTF(C,("putmsg: match_bits = 0x%lx, local_off=%ld, remote_off=%ld, bytes=%ld",(uint64_t)match_bits,(long)local_offset,(long)remote_offset,(long)nbytes));

  /* Issue Ptl Put operation */
  GASNETC_PTLSAFE(PtlPutRegion(md_h, local_offset, nbytes, PTL_ACK_REQ, target_id, GASNETC_PTL_RAR_PTE, ac_index, match_bits, remote_offset, hdr_data));

  return nbytes;
}

/* Need a special signal handler for Catamount, cant even do I/O in signal context.
 * just bump atomic var that will be checked periodically to see if shutdown
 * message has arrived
 */
void gasnetc_portalsSignalHandler(int sig) {
  /* just indicate we got a signal */
  switch (sig) {
  case SIGABRT:
  case SIGILL:
  case SIGSEGV:
  case SIGBUS:
  case SIGFPE:
    /* die immediately on these */
    signal(sig, SIG_DFL);
    raise(sig);
    break;

  default:
    /* let gasnet kill itself naturally, or just die if second time signal arrived */
    { static int sigquit_raised = 0;
      if (sigquit_raised) {
	_exit(1);
      } else {
	sigquit_raised = 1;
      }
      gasneti_weakatomic_set(&gasnetc_got_signum,sig,0);
    }
    break;
  }
}

#if !PLATFORM_OS_CATAMOUNT
/* ------------------------------------------------------------------------------------ */
/* Firehose bits */

#if GASNETI_STATS_OR_TRACE
  #define GASNETC_TRACE_MR(_event, _verb, _region) do {                  \
	const firehose_region_t *_reg = (_region);                       \
	int _pages = (int)(_reg->len/GASNET_PAGESIZE);                   \
	GASNETI_TRACE_PRINTF(D, ("FIREHOSE_MOVE: " _STRINGIFY(_verb)     \
				 " %d page(s) at " GASNETI_LADDRFMT,     \
				 _pages, GASNETI_LADDRSTR(_reg->addr))); \
	_GASNETI_STAT_EVENT_VAL(C, _event, _pages);                         \
  } while(0)
  #define GASNETC_TRACE_PIN(_region)	GASNETC_TRACE_MR(FIREHOSE_PIN, pin, (_region))
  #define GASNETC_TRACE_UNPIN(_region)	GASNETC_TRACE_MR(FIREHOSE_UNPIN, unpin, (_region))
#else
  #define GASNETC_TRACE_PIN(_region) 	((void)0)
  #define GASNETC_TRACE_UNPIN(_region) 	((void)0)
#endif

extern int
firehose_move_callback(gasnet_node_t node,
                       const firehose_region_t *unpin_list,
                       size_t unpin_num,
                       firehose_region_t *pin_list,
                       size_t pin_num)
{
  GASNETC_TRACE_WAIT_BEGIN();
#if GASNETC_USE_MDUPDATE
  int updates = MIN(unpin_num, pin_num);
#else
  const int updates = 0;
#endif
  int i;

  /* Step 1: unpaired unpins */
  for (i = updates; i < unpin_num; i++) {
    GASNETC_TRACE_UNPIN(unpin_list+i);
    GASNETC_PTLSAFE(PtlMDUnlink(unpin_list[i].client));
  }

  /* Step 2: pins */
  for (i = 0; i < pin_num; i++) {
    firehose_region_t *region = pin_list + i;
    ptl_md_t md;

    gasnetc_assert_aligned(region->addr, GASNET_PAGESIZE);
    gasnetc_assert_aligned(region->len,  GASNET_PAGESIZE);

    md.start = (void *)(region->addr);
    md.length = region->len;
    md.threshold = PTL_MD_THRESH_INF;
    md.max_size = 0;
    md.options = PTL_MD_EVENT_START_DISABLE;
#if GASNETC_USE_EQ_HANDLER
    md.user_ptr = (void*)(uintptr_t)GASNETC_TMP_MD;
#else
    md.user_ptr = (void*)TMPMD_event;
#endif
    md.eq_handle = gasnetc_SAFE_EQ->eq_h;

#if GASNETC_USE_MDUPDATE
    if (i < updates) {
      /* PTL_EQ_NONE gets flagged as invalid.  So I've created an EMPTY_EQ. */
      GASNETC_TRACE_UNPIN(unpin_list+i);
      region->client = unpin_list[i].client;
      GASNETC_PTLSAFE(PtlMDUpdate(region->client, NULL, &md, gasnetc_EMPTY_EQ->eq_h));
    } else
#endif
    {
      GASNETC_PTLSAFE(PtlMDBind(gasnetc_ni_h, md, PTL_RETAIN, &region->client));
    }
    GASNETC_TRACE_PIN(region);
  }

  GASNETC_TRACE_WAIT_END(FIREHOSE_MOVE);
  return 0;
}

extern int
firehose_remote_callback(gasnet_node_t node,
                         const firehose_region_t *pin_list, size_t num_pinned,
                         firehose_remotecallback_args_t *args)
{
    /* DO NOTHING.  IF WE GET CALLED WE COMPLAIN. */
    gasneti_fatalerror("invalid attempted to call firehose_remote_callback()");
    return -1;
}


/* Freelist of fh ops (lockfree when arch support available) */
static gasneti_lifo_head_t gasnetc_fh_freelist = GASNETI_LIFO_INITIALIZER;

/* The allocation table for fh ops.
 * The mutex protects only allocation of additional buffers, not the
 * critical path alloc/free operations (which use the freelist head).
 */
static gasneti_mutex_t gasnetc_fh_buffer_lock = GASNETI_MUTEX_INITIALIZER;
static gasnetc_fh_op_t *gasnetc_fh_buffer_tbl[256];
static int gasnetc_fh_buffer_cnt = 0;

/*  get a new fh op (parts ripped off from eop_new) */
gasnetc_fh_op_t *gasnetc_fh_new(void) {
  gasnetc_fh_op_t *result;

  GASNETI_TRACE_EVENT(C, FH_OP_ALLOC);

  result = gasneti_lifo_pop(&gasnetc_fh_freelist);

  if_pf (result == NULL) { /*  free list empty - try polling ONCE */
    gasnetc_portals_poll(GASNETC_SAFE_POLL);
    result = gasneti_lifo_pop(&gasnetc_fh_freelist);
  }

  if_pf (result == NULL) { /*  free list empty - need more fh ops */
    gasneti_mutex_lock(&gasnetc_fh_buffer_lock);

    /* Recheck, in case list was refilled while waiting on the lock */
    result = gasneti_lifo_pop(&gasnetc_fh_freelist);

    if (result == NULL) {
      int bufidx = gasnetc_fh_buffer_cnt;
      gasnetc_fh_op_t *buf;
      int i;

      GASNETI_TRACE_EVENT(C, FH_OP_ALLOC_BUF);

      if (bufidx != 256) {
        ++gasnetc_fh_buffer_cnt;

        buf = (gasnetc_fh_op_t *)gasneti_calloc(256,sizeof(gasnetc_fh_op_t));
        for (i=0; i < 256; i++) {
          gasnete_opaddr_t addr;
          addr.bufferidx = bufidx;
          addr.opidx = i;
          buf[i].addr = addr;
          gasneti_lifo_link(buf+i, buf+i+1); /* bogus final link ptr is harmless */
        }
        gasneti_lifo_push_many(&gasnetc_fh_freelist, buf+1, buf+255); /* push all but 1st */
        gasnetc_fh_buffer_tbl[bufidx] = buf;
        result = buf+0; /* keep 1st for ourselves */
      }
    }

    gasneti_mutex_unlock(&gasnetc_fh_buffer_lock);
  }

  if_pf (result == NULL) { /*  free list at max size but STILL empty - poll forever */
    do {
      gasnetc_portals_poll(GASNETC_SAFE_POLL);
      result = gasneti_lifo_pop(&gasnetc_fh_freelist);
    } while (result == NULL);
  }

#if (GASNETC_FH_PER_OP != 1)
  op->count = 0;
#endif
  return result;
}

/*  free a fh op */
void gasnetc_fh_free(uint16_t fulladdr) {
  gasnete_opaddr_t addr;
  gasnetc_fh_op_t *op;

  GASNETI_TRACE_EVENT(C, FH_OP_FREE);
  addr.fulladdr = fulladdr;
  op = gasnetc_fh_buffer_tbl[addr.bufferidx] + addr.opidx;
#if (GASNETC_FH_PER_OP == 1)
  {
    const firehose_request_t *tmp = &op->fh[0];
    firehose_release(&tmp, 1);
  }
#else
  {
    const firehose_request_t *tmp[GASNETC_FH_PER_OP];
    int i;
    for (i = 0; i < op->count; ++i) {
      tmp[i] = &op->fh[i];
    }
    gasneti_assert(op->count != 0); /* Never allocated w/o use */
    firehose_release(tmp, op->count);
  }
#endif
  gasneti_lifo_push(&gasnetc_fh_freelist, op);
}

/* Safe poll entry point for FIREHOSE_AMPOLL: */
void gasnetc_firehose_ampoll(void) {
  gasnetc_portals_poll(GASNETC_SAFE_POLL);
}

#endif /* !PLATFORM_OS_CATAMOUNT */
