#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>
#include <gasnet_portals.h>
#include <signal.h>

#define GASNETC_DEBUG_RB_VERBOSE 0

#if HAVE_CATAMOUNT_CNOS_MPI_OS_H /* catamount and new CNL */
   #include <catamount/cnos_mpi_os.h>
#elif HAVE_PCTMBOX_H /* old CNL */
   #include <pctmbox.h>
#else /* backup declarations, since these headers seem to be in flux */
  extern int cnos_get_rank();
  extern int cnos_get_size();
  extern int cnos_get_nidpid_map(void *);
  typedef struct {
      ptl_nid_t nid;
      ptl_pid_t pid;
      #ifdef STRIDER0
        int port;
      #endif
  } cnos_nidpid_map_t;
  extern void cnos_barrier_init(ptl_handle_ni_t ni_handle); /* NOOP function on Catamount */
  extern int cnos_barrier(void);
  #if PLATFORM_OS_CNL
    extern void cnos_pm_barrier(int);
    extern int cnos_register_ptlid(ptl_process_id_t);
  #endif
#endif

/* set to one for ReqRB Auto Unlink
 * Not advised since unlink event may be reaped so late that all
 * receive buffers are filled and overflow before the first
 * unlink event is seen.  Manual unlink seems to be the best option.
 */
#define GASNETC_REQRB_AUTO_UNLINK 0
#define GASNETC_REQRB_UNLINK_VERBOSE 0

/* macros used for simple hash table lookup.  Only accessed in this file. */
#define HASHTABLE_SIZE 512
#define HASHVAL HASHTABLE_SIZE
#define HASHFUNC(procid) (((procid)->nid) % HASHVAL)

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
gasnetc_PtlBuffer_t *gasnetc_ReqRB;          

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

ptl_handle_ni_t gasnetc_ni_h;              /* the network interface handle */
gasnetc_eq_t *gasnetc_AM_EQ = NULL;        /* The AM Event Queue */
gasnetc_eq_t *gasnetc_SAFE_EQ = NULL;      /* The SAFE Event Queue */

/* out of band MDs for sending system messages */
gasnetc_PtlBuffer_t gasnetc_SYS_Send;       /* out-of-band message send buffer */
gasnetc_PtlBuffer_t gasnetc_SYS_Recv;       /* out-of-band message recv buffer */
gasnetc_eq_t *gasnetc_SYS_EQ = NULL;        /* out-of-band system Event Queue */
/* a flag that is set to true after the network is initialized */
static int portals_sysqueue_initialized = 0;
double gasnetc_shutdown_seconds = 0.;
int gasnetc_shutdownInProgress = 0;
static double shutdown_max = 360.;  /* 6 minutes ... just a guess */
static gasneti_weakatomic_t sys_barrier_cnt;
static gasneti_weakatomic_t sys_barrier_got;
static gasneti_weakatomic_t sys_barrier_checkin;
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

/* construct the hash table for reverse lookups */
static gasnetc_procid_t *gasnetc_addrtable[HASHTABLE_SIZE];

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
gasnetc_dll_index_t gasnetc_scavenge_list = GASNETC_DLL_NULL;    /* scavenge list */
gasneti_mutex_t gasnetc_scavenge_lock = GASNETI_MUTEX_INITIALIZER;  /* lock for scavenge list */
int gasnetc_num_scavenge = 6;                 /* Max number of nodes to hit-up each scavenge run */
gasneti_weakatomic_t gasnetc_scavenge_inflight;  /* number of outstanding revoke requests */
int gasnetc_dump_stats = 0;                   /* write some stats info to files */
int gasnetc_debug_node = -1;                  /* used in debugging */

gasnetc_conn_t *gasnetc_conn_state = NULL;


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

/* =================================================================================
 * This top portion of the file is where file-scope worker routines are located.
 * ================================================================================= */

/* Forward reference for ReqRB event handler */
static void ReqRB_event(ptl_event_t *ev);

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
  p = (gasnetc_amlongcache_t*)gasneti_malloc(sizeof(gasnetc_amlongcache_t));
  p->dest_lid = lid;
  p->flags = 0;
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
static gasnetc_amlongcache_t* get_lid_obj_from_data(gasnet_node_t src, uint32_t lid, void* dataptr, size_t datalen)
{
  gasnetc_amlongcache_t *obj;
  int found;

  GASNETC_LOCK_NODE(src);
  found = get_or_insert_lid(src, lid, &obj);
  gasneti_assert( ! (obj->flags & GASNETC_LID_DATA_HERE) );
  obj->flags |= GASNETC_LID_DATA_HERE;
  obj->data = dataptr;                  /* only data message writes this */
  obj->nbytes = datalen;                /* only data message writes this */
  GASNETC_UNLOCK_NODE(src);
  /* unlock the list */
  if (found) {
    /* second to arrive, return obj to caller */
    gasneti_assert( obj->flags & GASNETC_LID_HEADER_HERE );
    return obj;
  }
  /* we are the first to arrive, obj remains on the list */
  return NULL;
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

  /* lock the list here */
  GASNETC_LOCK_NODE(src);
  found = get_or_insert_lid(src, lid, &obj);
  gasneti_assert( !(obj->flags & GASNETC_LID_HEADER_HERE) );
  obj->flags |= GASNETC_LID_HEADER_HERE;
  obj->tok = *ptok;             /* only header message writes these */
  GASNETC_UNLOCK_NODE(src);

  if (found) {
    /* second to arrive, return obj to caller */
    gasneti_assert( obj->flags & GASNETC_LID_DATA_HERE );
    return obj;
  }

  /* we are the first to arrive, obj remains on the list */
  return NULL;
}

/* ------------------------------------------------------------------------------------
 * Unpack the data from the event structure and execute the Request or Reply AM Short
 * handler function.
 * isReq is true if this is an AM Short Request, false for a Reply.
 * Return TRUE if we executed a handler (should always be true for AM Short)
 *
 * NOTE: the lower 32 bits of the match_bits have already been unpacked.
 * For a Request: upper 32 bits of match_bits = offset in sender ReqSB.
 * For a   Reply: upper 32 bits of match_bits = arg2
 * For both a request and reply:
 *   hdr_data:      [arg0 << 32 | arg1]
 *   Data Payload:  [remaining args]
 * NOTES:
 *   - message should be sizeof(double) aligned.
 * This function is called from:
 * - ReqRB_event in response to the arrival of an AM Short Request
 * - ReqSB_event in response to the arrival of an AM Short Reply
 * --------------------------------------------------------------------------------- */
static int exec_amshort_handler(int isReq, ptl_event_t *ev, int numarg, int ghandler)
{
  ptl_match_bits_t   mbits = ev->match_bits;
  gasnetc_ptl_token_t tok;
  gasnet_token_t token = (gasnet_token_t)&tok;
  uint8_t *data;
  int  argcnt = 0;
  int  msg_bytes = 0;
  gasnetc_threaddata_t *th = gasnetc_mythread();

  GASNETC_DEF_HARGS();    /* debug, must be first statement */
  {
    int i;
    for (i = 0; i < gasnet_AMMaxArgs(); i++) tok.args[i] = 0;
  }

  tok.flags = 0;
  tok.initiator = ev->initiator;
  tok.srcnode = gasnetc_get_nodeid(&ev->initiator);
  tok.narg = numarg;
  tok.ghandler = ghandler;

  if (isReq) {
    gasneti_assert(th->flags & GASNETC_THREAD_HAVE_RPLSB);
    th->flags &= ~GASNETC_THREAD_HAVE_RPLSB;
    tok.rplsb_offset = (uint32_t)th->rplsb_off;
    tok.initiator_offset = (uint32_t)(mbits >> 32);
  }

  /* set data pointer */
  data = (uint8_t*)ev->md.start + ev->offset;

  /* insure our data pointer is aligned for a double */
  gasneti_assert( ((intptr_t)data % sizeof(double)) == 0 );

  /* AM Short Request Data Format:
   * numarg=0:   HD=[----,cred] MB=[off,XX] Data=[seqno][pad]
   * numarg=1:   HD=[arg1,cred] MB=[off,XX] Data=[seqno][pad]
   * numarg=2+:  HD=[arg1,arg2] MB=[off,XX] Data=[args][cred][seqno][pad]
   * NOTE: seqno included only in debug mode
   * NOTE: Reply is identical, except without trailing pad
   */
  if (numarg > 0) tok.args[argcnt++] = (gasnet_handlerarg_t)GASNETC_UNPACK_UPPER(ev->hdr_data);
  if (numarg < 2) {
    /* credit info is packed in LOWER bits of hdr_data */
    uint32_t cred = (uint32_t)GASNETC_UNPACK_LOWER(ev->hdr_data);
    tok.credits = (uint8_t)(cred & 0x000000FF);
  } else {
    /* second arg in LOWER bits of hdr_data */
    tok.args[argcnt++] = (gasnet_handlerarg_t)GASNETC_UNPACK_LOWER(ev->hdr_data);
  }
  /* unpack remaining args from data payload */
  for(; argcnt < numarg; argcnt++) {
    memcpy(&tok.args[argcnt], data, sizeof(gasnet_handlerarg_t));
    data += sizeof(gasnet_handlerarg_t);
    msg_bytes += sizeof(gasnet_handlerarg_t);
  }
  if (numarg > 1) {
    /* unpack the credit info */
    memcpy(&tok.credits,data,sizeof(uint8_t));
    data += sizeof(uint8_t);
    msg_bytes += sizeof(uint8_t);
  }
  GASNETC_EXTRACT_SEQNO(data,msg_bytes);
  if (isReq) { /* message length accounting: Request contains pad at end of message */
    int pad;
    GASNETC_COMPUTE_DOUBLE_PAD(msg_bytes,pad);
    msg_bytes += pad;
  }
  {
    uint8_t ee,nx,nc;
    GASNETC_READ_CREDIT_BYTE(tok.credits,ee,nx,nc);
    GASNETI_TRACE_PRINTF(C,("CREDINFO: Short %s s=%d r=%d cred_byte=%x %d:%d:%d",(isReq?"Req":"Rpl"),tok.srcnode,gasneti_mynode,tok.credits,ee,nx,nc));
  }

  /* Process credit info and prep credit byte for return (if isReq) */
  if (gasnetc_use_flow_control) {
    tok.credits = gasnetc_credit_update(isReq,tok.credits,tok.srcnode,"Short");
  }
    
  gasneti_assert(ev->mlength == ev->rlength);
  gasneti_assert(msg_bytes == ev->rlength);
  GASNETC_SAVE_SEQNO(&tok);
  GASNETC_DBGMSG(0,isReq,"S",tok.srcnode,gasneti_mynode,ghandler,numarg,tok.args,msg_bytes,tok.credits,0,NULL,th);

  GASNETI_RUN_HANDLER_SHORT(isReq, ghandler, gasnetc_handler[ghandler], token, tok.args, numarg);

  if (isReq && !(tok.flags & GASNETC_PTL_REPLY_SENT)) {
    GASNETI_SAFE(
		 SHORT_REP(0,0, (token , gasneti_handleridx(gasnetc_noop_reph)) )
		 );
  }

  return 1;
}

/* ------------------------------------------------------------------------------------
 * Unpack the data from the event structure and execute the Request or Reply AM Medium
 * handler function.
 * isReq is true if this is an AM Medium Request, false for a Reply.
 * Return TRUE if we executed a handler (always true for AM Medium)
 *
 * NOTE: the lower 32 bits of the match_bits have already been unpacked.
 * For a Request: upper 32 bits of match_bits = offset in sender ReqSB.
 * For a   Reply: upper 32 bits of match_bits = arg2
 * For both a request and reply:
 *   hdr_data:      [arg0 << 32 | arg1]
 *   Data Payload:  [remaining args][data payload length][pad][data payload]
 * NOTES:
 *   - message should be sizeof(double) aligned.
 *   - pad insures data payload is sizeof(double) aligned.
 * This function is called from:
 * - ReqRB_event in response to the arrival of an AM Medium Request
 * - ReqSB_event in response to the arrival of an AM Medium Reply
 * --------------------------------------------------------------------------------- */
static int exec_ammedium_handler(int isReq, ptl_event_t *ev, int numarg, int ghandler)
{
  ptl_match_bits_t   mbits = ev->match_bits;
  gasnetc_ptl_token_t tok;
  gasnet_token_t token = (gasnet_token_t)&tok;
  uint8_t *data;
  int      pad;
  uint32_t payload_bytes;
  size_t   nbytes;
  int      argcnt = 0;
  int      msg_bytes = 0;
  gasnetc_threaddata_t *th = gasnetc_mythread();
  GASNETC_DEF_HARGS();    /* debug, must be first statement */
  {
    int i;
    for (i = 0; i < gasnet_AMMaxArgs(); i++) tok.args[i] = 0;
  }

  tok.flags = 0;
  tok.initiator = ev->initiator;
  tok.srcnode = gasnetc_get_nodeid(&ev->initiator);
  tok.narg = numarg;
  tok.ghandler = ghandler;

  /* set data pointer */
  data = (uint8_t*)ev->md.start + ev->offset;

  /* insure our data pointer is aligned for a double */
  gasneti_assert( ((intptr_t)data % sizeof(double)) == 0 );

  if (isReq) {
    gasneti_assert(th->flags & GASNETC_THREAD_HAVE_RPLSB);
    th->flags &= ~GASNETC_THREAD_HAVE_RPLSB;
    /* MLW: NOTE that rpl send buffer is small, offset never > 4GB */
    tok.rplsb_offset = (uint32_t)th->rplsb_off;
    tok.initiator_offset = (uint32_t)(mbits >> 32);
  }

  /* AM Medium Data Format:
   * HD=[cred:len,arg1] MB=[off,XX] Data=[args][seqno][pad][data][pad]
   * NOTE: seqno only included in debug mode
   */
  /* payload len and credit info in upper bits of hdr_data */
  payload_bytes = (uint32_t)GASNETC_UNPACK_UPPER(ev->hdr_data);
  tok.credits = (uint8_t)(payload_bytes >> 24);
  payload_bytes &= 0x00FFFFFF;     /* mask off credit_byte */
  nbytes = (size_t)payload_bytes;  /* type conversion */

  /* crack args out of hdr_data, mbits if available */
  if (numarg > 0) tok.args[argcnt++] = (gasnet_handlerarg_t)GASNETC_UNPACK_LOWER(ev->hdr_data);

  /* unpack remaining args from data payload */
  for(; argcnt < numarg; argcnt++) {
    memcpy(&tok.args[argcnt], data, sizeof(gasnet_handlerarg_t));
    data += sizeof(gasnet_handlerarg_t);
    msg_bytes += sizeof(gasnet_handlerarg_t);
  }

  GASNETC_EXTRACT_SEQNO(data,msg_bytes);            /* debug */
  GASNETC_SAVE_SEQNO(&tok);                         /* debug */

  /* Skip over any pad field so that handler payload is double-aligned */
  GASNETC_COMPUTE_DOUBLE_PAD(msg_bytes,pad);
  data += pad;
  msg_bytes += pad;
  gasneti_assert( ((intptr_t)data % sizeof(double)) == 0 );

  /* accounting: this should be same as message length */
  msg_bytes += nbytes;
  if (isReq) {  /* trailing pad only on Requests */
    GASNETC_COMPUTE_DOUBLE_PAD(msg_bytes,pad);
    msg_bytes += pad;
  }

  gasneti_assert(ev->mlength == ev->rlength);
  gasneti_assert(msg_bytes == ev->rlength);

  {
    uint8_t ee,nx,nc;
    GASNETC_READ_CREDIT_BYTE(tok.credits,ee,nx,nc);
    GASNETI_TRACE_PRINTF(C,("CREDINFO: Med %s s=%d r=%d cred_byte=%x %d:%d:%d",(isReq?"Req":"Rpl"),tok.srcnode,gasneti_mynode,tok.credits,ee,nx,nc));
  }

  /* Process credit info and prep credit byte for return (if isReq) */
  if (gasnetc_use_flow_control) {
    tok.credits = gasnetc_credit_update(isReq,tok.credits,tok.srcnode,"Med");
  }

  GASNETC_DBGMSG(0,isReq,"M",tok.srcnode,gasneti_mynode,ghandler,numarg,tok.args,msg_bytes,tok.credits,nbytes,data,th);

  GASNETI_RUN_HANDLER_MEDIUM(isReq, ghandler, gasnetc_handler[ghandler], token, tok.args, numarg, data, nbytes);

  if (isReq && !(tok.flags & GASNETC_PTL_REPLY_SENT)) {
    GASNETI_SAFE(
		 SHORT_REP(0,0, (token , gasneti_handleridx(gasnetc_noop_reph)) )
		 );
  }

  return 1;
}

/* ------------------------------------------------------------------------------------
 * Unpack the data from the event structure and attempt to execute the AM Long handler.
 * Return true if we executed the gasnet handler function
 *
 * This routine will be called from both Request and Reply AMs, isReq=true of Request.
 * In general, AM Longs require two messages, a data payload sent directly to the RAR
 * and a header send to ReqRB (for a Request) or ReqSB (for a Reply).  This function
 * is called when the header arrives.  The data message may or may not have arrived.
 * The last to arrive will execute the requested handler.
 * The first to arrive will cache its metadata in a LID cache that the second can retrieve.
 * AMLong messages with small data payloads may have the payload packed with the header
 * message (and no data message).  isPacked=true if a packed message.
 * In summary, this function is called from:
 * - RARAM_event: in response to a Request or Reply AMLong data packet arrival.
 * - ReqRB_event: in response to an AM Long Header Request message.
 * - ReqSB_event: in response to an AM Long Header Reply message.
 * --------------------------------------------------------------------------------- */
static int exec_amlong_header(int isReq, int isPacked,
			       ptl_event_t *ev, int numarg, int ghandler)
{
  ptl_match_bits_t   mbits = ev->match_bits;
  gasnetc_ptl_token_t tok;
  gasnet_token_t token = (gasnet_token_t)&tok;
  gasnet_handlerarg_t args[gasnet_AMMaxArgs()];
  uint32_t lid;
  uint8_t *data;
  int      pad;
  int32_t  payload_bytes;
  size_t   nbytes = -1;
  int      argcnt = 0;
  int      msg_bytes = 0;
  void    *dest;
  int      check_reply = isReq;  /* AM Request must reply for Portals Conduit */
  int      ran_handler = 0;
  gasnetc_threaddata_t *th = gasnetc_mythread();

  GASNETC_DEF_HARGS();           /* debug, must be first statement */
  {
    int i;
    for (i = 0; i < gasnet_AMMaxArgs(); i++) tok.args[i] = 0;
  }

  tok.flags = 0;
  tok.initiator = ev->initiator;
  tok.srcnode = gasnetc_get_nodeid(&ev->initiator);
  tok.credits = 0;
  tok.narg = numarg;
  tok.ghandler = ghandler;

  /* set data pointer */
  data = (uint8_t*)ev->md.start + ev->offset;

  /* insure our data pointer is aligned for a double */
  gasneti_assert( ((intptr_t)data % sizeof(double)) == 0 );

  /* Regular Format: hdr_dara=[arg0,lid]      data=[args][seqno][cred][pad] 
   * Packed  Format: hdr_data=[arg0,cred:len] data=[args][seqno][destaddr][data][pad]
   * NOTE: seqno only in debug mode, pad only for Req
   */

  /* extract LID and check if this is a packed AM Long */
  /* if this is a packed AM, the resulting LID is actually the data payload length */
  lid = (uint32_t)GASNETC_UNPACK_LOWER(ev->hdr_data);

  /* crack args out of hdr_data */
  if (numarg > 0) tok.args[argcnt++] = (gasnet_handlerarg_t)GASNETC_UNPACK_UPPER(ev->hdr_data);

  /* crack upper portion of match_bits */
  if (isReq) {
    tok.initiator_offset = (uint32_t)GASNETC_UNPACK_UPPER(mbits);
  } 

  /* unpack remaining args from data payload */
  for(; argcnt < numarg; argcnt++) {
    memcpy(&tok.args[argcnt], data, sizeof(gasnet_handlerarg_t));
    data += sizeof(gasnet_handlerarg_t);
    msg_bytes += sizeof(gasnet_handlerarg_t);
  }
  GASNETC_EXTRACT_SEQNO(data,msg_bytes);
  GASNETC_SAVE_SEQNO(&tok);

  if (isPacked) {
    tok.credits = (lid >> 24);
    nbytes = (lid & 0x00FFFFFF);
  } else {
    /* unpack credit byte */
    memcpy(&tok.credits,data,sizeof(uint8_t));
    data += sizeof(uint8_t);
    msg_bytes += sizeof(uint8_t);
  }

  {
    uint8_t ee,nx,nc;
    GASNETC_READ_CREDIT_BYTE(tok.credits,ee,nx,nc);
    GASNETI_TRACE_PRINTF(C,("CREDINFO: Long%s %s s=%d r=%d cred_byte=%x %d:%d:%d",(isPacked?"Packed":""),(isReq?"Req":"Rpl"),tok.srcnode,gasneti_mynode,tok.credits,ee,nx,nc));
  }
  if (gasnetc_use_flow_control) {
    /* In case of Request:  process credit update and store in token field for return.
     * In case of Reply:  ok to process returned credits here.
     */
    tok.credits = gasnetc_credit_update(isReq,tok.credits,tok.srcnode,"Long");
  }

  if (isPacked) {
    
    /* extract the data payload destination, shoud be in local RAR */
    memcpy(&dest, data, sizeof(void*));
    data += sizeof(void*);
    msg_bytes += sizeof(void*);
      
    /* copy the data payload to the specified destination */
    memcpy(dest,data,nbytes);
    msg_bytes += nbytes;

    if (isReq) {
      gasnetc_threaddata_t *th = gasnetc_mythread();
      gasneti_assert(th->flags & GASNETC_THREAD_HAVE_RPLSB);
      th->flags &= ~GASNETC_THREAD_HAVE_RPLSB;
      tok.rplsb_offset = (uint32_t)th->rplsb_off;
      /* message length accounting */
      GASNETC_COMPUTE_DOUBLE_PAD(msg_bytes,pad);
      msg_bytes += pad;
    }
    gasneti_assert(msg_bytes == ev->rlength);
    gasneti_assert(msg_bytes <= GASNETC_CHUNKSIZE);
    GASNETC_DBGMSG(0,isReq,"L",tok.srcnode,gasneti_mynode,ghandler,numarg,tok.args,msg_bytes,tok.credits,nbytes,dest,th);
    GASNETI_RUN_HANDLER_LONG(isReq, ghandler, gasnetc_handler[ghandler], token, tok.args, numarg, dest, nbytes);

    ran_handler = 1;

  } else {

    /* called from Header packet, but not a packed message, check if data message has arrived */
    gasnetc_amlongcache_t *p;

    if (isReq) { /* message length accounting */
      GASNETC_COMPUTE_DOUBLE_PAD(msg_bytes,pad);
      msg_bytes += pad;
    }
    gasneti_assert(msg_bytes == ev->rlength);
    gasneti_assert(msg_bytes <= GASNETC_CHUNKSIZE);

#if GASNET_DEBUG
    tok.msg_bytes = msg_bytes;  /* used in debug/tracing message */
#endif

    p = get_lid_obj_from_header(tok.srcnode, lid, &tok);
    if (p) {
      /* data has arrived, run handler */
      if (isReq) {  
	gasneti_assert(th->flags & GASNETC_THREAD_HAVE_RPLSB);
	th->flags &= ~GASNETC_THREAD_HAVE_RPLSB;
	tok.rplsb_offset = (uint32_t)th->rplsb_off;
      }

      GASNETC_DBGMSG(0,isReq,"L",tok.srcnode,gasneti_mynode,ghandler,numarg,tok.args,msg_bytes,tok.credits,p->nbytes,p->data,th);
      GASNETI_TRACE_PRINTF(C,("exec_amlong_header, second to arrive: isReq=%d, numarg=%d, hndlr=%d, nbytes=%d",isReq,numarg,ghandler,(int)p->nbytes));
      GASNETI_RUN_HANDLER_LONG(isReq, ghandler ,gasnetc_handler[ghandler], token, tok.args, numarg, p->data, p->nbytes);

      ran_handler = 1;

      /* free the lid object, it has already been removed from the list */
      gasneti_free(p);
    } else {

      /* first to arrive, cant run handler */
      GASNETI_TRACE_PRINTF(C,("exec_amlong_header, first to arrive: isReq=%d, numarg=%d, hndlr=%d",isReq,numarg,ghandler));
      check_reply = 0;
    }
  }


  if (check_reply && !(tok.flags & GASNETC_PTL_REPLY_SENT)) {
    /* must always issue a reply to dealloc ReqSB chunk.  If GASNet handler did
     * not reply, we reply here with a short no-op
     */
    GASNETI_TRACE_PRINTF(C,("exec_amlong_header, sending noop reply"));
    GASNETI_SAFE(
		 SHORT_REP(0,0, (token , gasneti_handleridx(gasnetc_noop_reph)) )
		 );
  }

  return ran_handler;
}

/* ------------------------------------------------------------------------------------
 * Unpack the data from the event structure and attempt to execute the AM Long handler.
 * Return TRUE if we ran the gasnet handler.
 * 
 * This routine will be called from both Request and Reply AMs, isReq=true of Request.
 * In general, AM Longs require two messages, a data payload sent directly to the RAR
 * and a header send to ReqRB (for a Request) or ReqSB (for a Reply).  This function
 * is called when the data message arrives.  The header message may or may not have arrived.
 * The last to arrive will execute the requested handler.
 * The first to arrive will cache its metadata in a LID cache that the second can retrieve.
 * This function is called from:
 * - RARAM_event: in response to a Request or Reply AMLong data packet arrival.
 * --------------------------------------------------------------------------------- */
static int  exec_amlong_data(int isReq, ptl_event_t *ev)
{
  uint32_t lid;
  uint8_t *data;
  int      pad;
  size_t   nbytes;
  void    *dest;
  uint8_t* dataaddr = (uint8_t*)ev->md.start + ev->offset;
  size_t   datalen = ev->mlength;
  int      ran_handler = 0;
  gasnetc_amlongcache_t *p;
  gasnet_node_t srcnode = gasnetc_get_nodeid(&ev->initiator);
  gasnetc_threaddata_t *th = gasnetc_mythread();

  GASNETC_DEF_HARGS();

  /* extract LID and check if this is a packed AM Long */
  /* if this is a packed AM, the resulting LID is actually the data payload length */
  lid = GASNETC_UNPACK_LOWER(ev->hdr_data);

  /* see if header message has arrived */
  p = get_lid_obj_from_data(srcnode, lid, dataaddr, datalen);
  if (p) {
    /* Header has arrived, run handler */
    gasnet_token_t token = (gasnet_token_t)&p->tok;

    if (isReq) {
      gasneti_assert(th->flags & GASNETC_THREAD_HAVE_RPLSB);
      th->flags &= ~GASNETC_THREAD_HAVE_RPLSB;

      p->tok.rplsb_offset = (uint32_t)th->rplsb_off;
    }

    GASNETC_DBGMSG(0,isReq,"L",p->tok.srcnode,gasneti_mynode,p->tok.ghandler,p->tok.narg,p->tok.args,p->tok.msg_bytes,p->tok.credits,datalen,dataaddr,th);
    GASNETI_TRACE_PRINTF(C,("exec_amlong_data, second to arrive, running handler isReq=%d, lid=%d",isReq,lid));
    GASNETI_RUN_HANDLER_LONG(isReq, p->tok.ghandler ,gasnetc_handler[p->tok.ghandler], token, p->tok.args, p->tok.narg, dataaddr, datalen);

    ran_handler = 1;

    if (isReq && !(p->tok.flags & GASNETC_PTL_REPLY_SENT)) {
      /* must always issue a reply to dealloc ReqSB chunk.  If GASNet handler did
       * not reply, we reply here with a short no-op */
      GASNETI_TRACE_PRINTF(C,("exec_amlong_data, sending noop reply"));
      GASNETI_SAFE(
		   SHORT_REP(0,0, (token , gasneti_handleridx(gasnetc_noop_reph)) )
		   );
    } 

    /* free the lid object, it has already been removed from the list */
    gasneti_free(p);

  } else {
    GASNETI_TRACE_PRINTF(C,("exec_amlong_data, first to arrive, isReq=%d, lid=%d",isReq,lid));
  }

  return ran_handler;
}

/* ------------------------------------------------------------------------------------
 * Allocate memory with a given byte alignment.
 *  -- The aligned memory is the function return value
 *  -- The actual start of the memory (for freeing it) is returned in allocated_start
 *  -- The alignment MUST be a power of 2
 * --------------------------------------------------------------------------------- */
static void* gasnetc_aligned_alloc(size_t nbytes, uint32_t alignment, void **allocated_start)
{
  size_t bytes;
  void *loc;
  uintptr_t ptr, mask;

  bytes = nbytes + (alignment > 0 ? alignment - 1 : 0);
  loc = gasneti_malloc(bytes);
  *allocated_start = loc;

  if (alignment == 0) {
    /* no alignment constraint */
    return loc;
  }

  /* insure alignment is power of 2 (contains exactly one non-zero bit) */
  {
    uintptr_t bits = alignment;
    int cnt = 0;
    while (bits > 0) {
      if (bits & 0x1) cnt++;
      bits = bits >> 1;
    }
    if (cnt != 1) {
      gasneti_fatalerror("gasnetc_aligned_alloc with non-power-of-2 alignment %d",(int)alignment);
    }
  }
    
  /* finally, do the alignment by zeroing the low order bits */
  mask = alignment-1;
  ptr = ((uintptr_t)( (uint8_t*)loc + alignment - 1)) & ~mask;
  return (void*)ptr;
}

/* ------------------------------------------------------------------------------------
 * Allocate a buffer with the given alignment.
 * This buffer will NOT be managed by a chunk allocator
 * --------------------------------------------------------------------------------- */
static void gasnetc_buf_init(gasnetc_PtlBuffer_t *buf, const char *name, size_t nbytes, uint32_t alignment)
{
  buf->name = gasneti_strdup(name);
  buf->alignment = alignment;
  buf->nbytes = nbytes;
  if (nbytes > 0) {
    buf->start = gasnetc_aligned_alloc(nbytes,alignment,&buf->actual_start);
    GASNETI_TRACE_PRINTF(C,("gasnetc_buf_init for %s alignment %u at %p, start=%p",name,alignment,buf->start,buf->actual_start));
  } else {
    buf->start = buf->actual_start = NULL;
    buf->alignment = 0;
  }
  buf->use_chunks = 0;
#ifdef GASNET_PAR
  gasneti_weakatomic_set(&buf->threads_active, 0, 0);
#endif
}

/* ------------------------------------------------------------------------------------
 * Trivial chunk allocator for bounce buffer and Msg Send buffers.
 * (1) WARNING WARNING WARNING!
 *     Not a thread-safe freelist implementation!!!
 *     Can easily have one thread pulling something off the list while a portals
 *     event handler, executing in another thread is putting a chunk back on the list.
 *     Must change for multi-threaded implementation.
 * (2) What we really should have is an efficient buddy-buffer implementation so that
 *     small messages dont have to allocate a full KB.  Concern that this will be expensive
 *     and even more expensive in multi-threaded environment.
 * --------------------------------------------------------------------------------- */
static void gasnetc_chunk_init(gasnetc_PtlBuffer_t *buf, const char *name, size_t nchunks)
{
  int i;
  size_t nbytes = nchunks * GASNETC_CHUNKSIZE;
  gasnetc_chunk_t *p;

  GASNETI_TRACE_PRINTF(C,("gasnetc_chunk_init for %s with %lu chunks",name,(ulong)nchunks));
  buf->name = gasneti_strdup(name);
  buf->alignment = GASNETC_CHUNKSIZE;
  buf->nbytes = nbytes;
  buf->start = gasnetc_aligned_alloc(nbytes,buf->alignment,&buf->actual_start);
  buf->use_chunks = 1;
  gasneti_mutex_init(&buf->lock);
  buf->numchunks = nchunks;
  buf->inuse = 0;
  buf->hwm = 0;
  buf->freelist = NULL;
  GASNETI_TRACE_PRINTF(C,("CHUNK_INIT: %s nchunks=%i, nbytes=%i, start=0x%p",name,(int)nchunks,(int)nbytes,buf->start));
  p = (gasnetc_chunk_t*) buf->start;
  if (p == NULL) {
    gasneti_fatalerror("failed to alloc %i bytes for chunk allocator %s at %s",(int)nbytes,name,gasneti_current_loc);
  }
  for (i = 0; i < nchunks; i++) {
    p->next = buf->freelist;
    buf->freelist = p;
    p++;
  }
}

/* ---------------------------------------------------------------------------------
 * delete the allocator memory.
 * --------------------------------------------------------------------------------- */
static void gasnetc_buf_free(gasnetc_PtlBuffer_t *buf)
{
  gasneti_free(buf->name);
  if (buf->actual_start != NULL) {
    gasneti_free(buf->actual_start);
  }
  buf->start = buf->actual_start = NULL;
  buf->nbytes = 0;

  /* keep name and stats for trace_finish call */
}

/* ---------------------------------------------------------------------------------
 * Find the ReqRB with a memory starting address of start_addr
 * --------------------------------------------------------------------------------- */
static gasnetc_PtlBuffer_t* ReqRB_getbuf(uintptr_t start_addr)
{
  int i;
  gasnetc_PtlBuffer_t *p = NULL;

  for (i = 0; i < gasnetc_ReqRB_pool_size; i++) {
    uintptr_t buf_start = (uintptr_t)gasnetc_ReqRB[i].start;
    if (buf_start == start_addr) {
      return &gasnetc_ReqRB[i];
    }
  }
  gasneti_fatalerror("ReqRB_getbuf: Unable to find ReqRB with starting address 0x%llx",(unsigned long long)start_addr);
}

/* ---------------------------------------------------------------------------------
 * This function is called when a Request Receive Buffer needs to be refreshed
 * and placed on the match list just before the catch-basin buffer.
 * The start_addr is the starting address of the memory buffer.  We use this
 * to determine which ReqRB from the pool needs to be re-cycled
 * --------------------------------------------------------------------------------- */
static void ReqRB_refresh(uintptr_t start_addr)
{
  int i;
  gasnetc_PtlBuffer_t *p = NULL;
  ptl_md_t md;
  ptl_process_id_t match_id;


  GASNETI_TRACE_PRINTF(C,("ReqRB_refresh called with start address %lx",start_addr));
  p = ReqRB_getbuf(start_addr);
#if GASNETC_DEBUG_RB_VERBOSE
  printf("[%d] ReqRB_refresh buffer %s at start address %lx\n",gasneti_mynode,p->name,start_addr); fflush(stdout);
#endif
#ifdef GASNET_PAR 
  /* must wait until all other threads have completed work in this buffer before
   * re-threading back onto ME list.  
   */
  while (gasneti_weakatomic_read(&p->threads_active, 0) > 0) {
    /* probably should do-nothing poll since if other threads are active they
     * should be done soon and should not block
     */
    gasneti_sched_yield();
  }
#endif
  md.start = p->start;
  md.length = p->nbytes;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = GASNETC_CHUNKSIZE;
  md.options = PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE | PTL_MD_MAX_SIZE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uint64_t)GASNETC_REQRB_MD;
#else
  md.user_ptr = (void*)ReqRB_event;
#endif
  md.eq_handle = gasnetc_AM_EQ->eq_h;

  match_id.nid = PTL_NID_ANY;
  match_id.pid = PTL_PID_ANY;

  GASNETC_PTLSAFE(PtlMEInsert(gasnetc_CB.me_h, match_id, GASNETC_PTL_REQRB_BITS, GASNETC_PTL_IGNORE_BITS,PTL_UNLINK, PTL_INS_BEFORE, &p->me_h));

  GASNETC_PTLSAFE(PtlMDAttach(p->me_h, md, PTL_UNLINK, &p->md_h));
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
  ptl_size_t offset = ev->offset;
  ptl_match_bits_t   mbits = ev->match_bits;
  uint8_t msg_type;
  uint64_t amflag = ((mbits & GASNETC_SELECT_BYTE1) >> 8);
  int isReq = (amflag & GASNETC_PTL_AM_REQUEST);
  int ran_handler;

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
  ptl_size_t offset = ev->offset;
  ptl_match_bits_t   mbits = ev->match_bits;
  gasnete_threadidx_t threadid;
  gasnete_opaddr_t addr;
  uint8_t msg_type;
  gasnete_op_t *op;

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("RARSRC event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)offset,(uint64_t)mbits,msg_type));

  /* extract the lower bits based on message type */
  gasnete_get_op_lowbits(mbits, &threadid, &addr);

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  switch (ev->type) {
  case PTL_EVENT_SEND_END:
    /* InSegment Put (from local RAR) */
    if ( !(msg_type & GASNETC_PTL_MSG_GET) )
      gasnetc_return_ticket(&gasnetc_send_tickets);
    if ((msg_type & GASNETC_PTL_MSG_PUT) && (msg_type & GASNETC_PTL_MSG_DOLC)) {
      gasnete_threaddata_t *th = gasnete_threadtable[GASNETE_THREADID(threadid)];
      gasneti_weakatomic_decrement(&(th->local_completion_count), 0);
    } else if (msg_type & GASNETC_PTL_MSG_AMDATA) {
      uint64_t amflag = (mbits & GASNETC_SELECT_BYTE1) >> 8;
      if (amflag & GASNETC_PTL_AM_SYNC) {
	gasnetc_threaddata_t *th = gasnete_threadtable[GASNETE_THREADID(threadid)]->gasnetc_threaddata;
	/* caller is AMLong (sync, not async), and is waiting for this counter to decrement */
	gasneti_weakatomic_decrement(&th->amlong_data_inflight, 0);
      }
    }
    break;

  case PTL_EVENT_PUT_END:
    /* Must be a AM Long Reply data message */
    {
      int ran_handler;
      uint64_t amflag = (mbits & GASNETC_SELECT_BYTE1) >> 8;
      gasneti_assert( msg_type & GASNETC_PTL_MSG_AMDATA);
      gasneti_assert( !( amflag & GASNETC_PTL_AM_REQUEST) );
      exec_amlong_data(0, ev);
    }
    break;

  case PTL_EVENT_ACK:
    /* InSegment Put (from local RAR) */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_PUT);
    op = gasnete_opaddr_to_ptr(threadid, addr);
    /* mark the put (isget=0) operation complete */
    gasnete_op_markdone(op, 0 /* !isget */);
    break;

  case PTL_EVENT_REPLY_END:
    /* InSegment Get (to local RAR) */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_GET);
    gasnetc_return_ticket(&gasnetc_send_tickets);
    op = gasnete_opaddr_to_ptr(threadid, addr);
    /* mark the get (isget=1) operation complete */
    gasnete_op_markdone(op, 1);
    break;

  default:
    gasneti_fatalerror("Invalid event %s on RARSRC",ptl_event_str[ev->type]);
  }
}

/* ---------------------------------------------------------------------------------
 * Handle events on Temporary Memory Descriptors.
 * Used only as local source of GASNet Put or dest of GASNet Get when not
 * in local RAR and too large to use bounce buffer (on no chunks available).
 *  SEND_END => Put (or Get) completed locally.
 *       ACK => Put completed, mark operation as done and free MD.
 * REPLY_END => Get completed, mark operation done and free MD.
 * --------------------------------------------------------------------------------- */
static void TMPMD_event(ptl_event_t *ev)
{
  ptl_size_t offset = ev->offset;
  ptl_match_bits_t   mbits = ev->match_bits;
  gasnete_threadidx_t threadid;
  gasnete_opaddr_t addr;
  uint8_t msg_type;
  gasnete_op_t *op;

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("TMPMD event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)offset,(uint64_t)mbits,msg_type));

  /* extract the lower bits based on message type */
  if (msg_type & GASNETC_PTL_MSG_AM) {
    gasneti_fatalerror("Unexpected AM msg type on TMPMD, mbits = 0x%lx",(uint64_t)mbits);
  } else {
    gasnete_get_op_lowbits(mbits, &threadid, &addr);
  }

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  switch (ev->type) {
  case PTL_EVENT_SEND_END:
    if ( !(msg_type & GASNETC_PTL_MSG_GET) )
      gasnetc_return_ticket(&gasnetc_send_tickets);
    /* Put from TmpMD */
    if ((msg_type & GASNETC_PTL_MSG_PUT) && (msg_type & GASNETC_PTL_MSG_DOLC)) {
      gasnete_threaddata_t *th = gasnete_threadtable[GASNETE_THREADID(threadid)];
      gasneti_weakatomic_decrement(&(th->local_completion_count), 0);
    } else if (msg_type & GASNETC_PTL_MSG_AMDATA) {
      uint64_t amflag = (mbits & GASNETC_SELECT_BYTE1) >> 8;
      if (amflag & GASNETC_PTL_AM_SYNC) {
	gasnetc_threaddata_t *th = gasnete_threadtable[GASNETE_THREADID(threadid)]->gasnetc_threaddata;
	/* caller is AMLong (sync, not async), and is waiting for this counter to decrement */
	gasneti_weakatomic_decrement(&th->amlong_data_inflight, 0);
      }
      /* unlink the tmp MD used in the AM Long data put */
      gasnetc_free_tmpmd(ev->md_handle);
    }
    break;

  case PTL_EVENT_ACK:
    /* Put from TmpMD */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_PUT);
    gasnetc_free_tmpmd(ev->md_handle);
    op = gasnete_opaddr_to_ptr(threadid, addr);
    /* mark the put (isget=0) operation complete */
    gasnete_op_markdone(op, 0 /* !isget */);
    break;

  case PTL_EVENT_REPLY_END:
    /* Get into TmpMD */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_GET);
    gasnetc_return_ticket(&gasnetc_send_tickets);
    gasnetc_free_tmpmd(ev->md_handle);
    op = gasnete_opaddr_to_ptr(threadid, addr);
    /* mark the get (isget=1) operation complete */
    gasnete_op_markdone(op, 1);
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
 *   GET_END => Catch-basin recovery underway.  Mark source node as in-recovery.
 * --------------------------------------------------------------------------------- */
static void ReqSB_event(ptl_event_t *ev)
{
  ptl_size_t offset = ev->offset;
  ptl_match_bits_t   mbits = ev->match_bits;
  gasnete_threadidx_t threadid;
  gasnete_opaddr_t addr;
  uint8_t msg_type, amflag, numarg, ghandler;
  gasnete_op_t *op;
  uint8_t *pdata, *q;
  void *dest;
  gasnetc_conn_t      *state;
  int pending;
  gasnet_node_t srcnode;
  int ran_handler = 0;
  ptl_size_t local_offset;


  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("ReqSB event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)offset,(uint64_t)mbits,msg_type));

  /* extract the lower bits based on message type */
  if (msg_type & GASNETC_PTL_MSG_AM) {
    GASNETC_GET_AM_LOWBITS(mbits, numarg, ghandler, amflag);
  } else {
    gasnete_get_op_lowbits(mbits, &threadid, &addr);
  }

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  switch (ev->type) {
  case PTL_EVENT_SEND_END:
    if ( !(msg_type & GASNETC_PTL_MSG_GET) )
      gasnetc_return_ticket(&gasnetc_send_tickets);
    if (msg_type & GASNETC_PTL_MSG_PUT) {
      /* Put bounced through ReqSB, can free chunk now */
      if (msg_type & GASNETC_PTL_MSG_DOLC) {
	gasnete_threaddata_t *th = gasnete_threadtable[GASNETE_THREADID(threadid)];
	gasneti_weakatomic_decrement(&(th->local_completion_count), 0);
      }
      local_offset = mbits>>32;
      gasnetc_chunk_free(&gasnetc_ReqSB,local_offset);
    }
    break;

  case PTL_EVENT_ACK:
    /* Put bounced through ReqSB, mark op complete */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_PUT);
    op = gasnete_opaddr_to_ptr(threadid, addr);
    /* mark the put (isget=0) operation complete */
    gasnete_op_markdone(op, 0 /* !isget */);
    break;

  case PTL_EVENT_REPLY_END:
    /* Get bouncing through ReqSB, copy to dest and complete */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_GET);
    gasnetc_return_ticket(&gasnetc_send_tickets);
    local_offset = (mbits >> 32);
    pdata = ((uint8_t*)ev->md.start + local_offset);
    q = pdata - sizeof(void*);
    /* q points to location where real destination address is stored */
    dest = (void*)*(uintptr_t*)q;
    memcpy(dest,pdata,ev->mlength);
    /* free the bounce buffer */
    local_offset -= sizeof(void*);
    gasnetc_chunk_free(&gasnetc_ReqSB,local_offset);
    op = gasnete_opaddr_to_ptr(threadid, addr);
    /* mark the get (isget=1) operation complete */
    /* Do we need membar here?  Above chunk free required lock/unlock
     * of mutex => membar, right?
     */
    gasnete_op_markdone(op, 1);
    break;

  case PTL_EVENT_GET_END:
    /* CB Recovery of dropped AM Request, stop all further AMs to this node */
    srcnode = gasnetc_get_nodeid(&ev->initiator);
    state = &gasnetc_conn_state[srcnode];
    /* dealloc the chunk */
    gasnetc_chunk_free(&gasnetc_ReqSB,offset);

    /* CB Recovery not implemented, better fail */
    gasneti_fatalerror("ReqSB got GET_END event, but CB not implemented");
    
    break;

  case PTL_EVENT_PUT_END:
    /* This is an AM reply from a previous request */
    if (amflag & GASNETC_PTL_AM_SHORT) {
      ran_handler = exec_amshort_handler(0,ev,numarg,ghandler);
    } else if (amflag & GASNETC_PTL_AM_MEDIUM) {
      ran_handler = exec_ammedium_handler(0,ev,numarg,ghandler);
    } else if (amflag & GASNETC_PTL_AM_LONG) {
      int is_packed = amflag & GASNETC_PTL_AM_PACKED;
      gasneti_assert(! (amflag & GASNETC_PTL_AM_REQUEST) );
      /* isReq = 0, only Replies come into ReqSB */
      ran_handler = exec_amlong_header(0,is_packed,ev,numarg,ghandler);
    } else {
      gasneti_fatalerror("ReqSB: Invalid amflag from mbits = %lx",(uint64_t)mbits);
    }

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
  ptl_size_t offset = ev->offset;
  ptl_size_t local_offset = (mbits >> 32);
  uint8_t msg_type;

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("RplSB event %s offset = %i, loc_offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)offset,(int)local_offset,(uint64_t)mbits,msg_type));

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  /* MLW: debug check */
  gasneti_assert(offset == local_offset);
  
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
  uint8_t msg_type, amflag, numarg, ghandler;

  /* increment ref counter on this buffer */
  GASNETC_REQRB_START(ev->md.start);

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("ReqRB event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)ev->offset,(uint64_t)mbits,msg_type));

  /* extract the lower bits based on message type */
  if (msg_type & GASNETC_PTL_MSG_AM) {
    GASNETC_GET_AM_LOWBITS(mbits, numarg, ghandler, amflag);
  } else {
    gasneti_fatalerror("Invalid event msg type on ReqRB, mbits = 0x%lx",(uint64_t)mbits);
  }

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  switch (ev->type) {
  case PTL_EVENT_PUT_END:

    if (amflag & GASNETC_PTL_AM_SHORT) {
      exec_amshort_handler(1,ev,numarg,ghandler);
    } else if (amflag & GASNETC_PTL_AM_MEDIUM) {
      exec_ammedium_handler(1,ev,numarg,ghandler);
    } else if (amflag & GASNETC_PTL_AM_LONG) {
      int is_packed = amflag & GASNETC_PTL_AM_PACKED;
      /* isReq = true */
      exec_amlong_header(1,is_packed,ev,numarg,ghandler);
    } else {
      gasneti_fatalerror("ReqRB: Invalid amflag from mbits = %lx",(uint64_t)mbits);
    }

    /* decrement ref counter on this buffer */
    GASNETC_REQRB_FINISH(ev->md.start);

    /* Should we check if this buffer can be recycled here as well as below? */
    /* THREAD SAFETY ISSUE: multiple threads could be executing handlers that
     * reference data in this MD.  Cant zero memory and should not re-link
     * into list until all threads have completed.
     * In practice, it will be a very low probability event that, after
     * being added back into the match list, an incoming message will have
     * over-written data that one of the threads is still reading.
     */
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
	  ReqRB_refresh((intptr_t)ev->md.start);
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
    /* decrement ref counter on this buffer */
    GASNETC_REQRB_FINISH(ev->md.start);
    ReqRB_refresh((intptr_t)ev->md.start);
    break;

  default:
    /* decrement ref counter on this buffer */
    GASNETC_REQRB_FINISH(ev->md.start);
    gasneti_fatalerror("Invalid event %s on ReqRB",ptl_event_str[ev->type]);
  }
}

/* ------------------------------------------------------------------------------------
 * Handle events on the Catch-Basin memory descriptor.
 * - PUT_END => dropped AM Request
 * --------------------------------------------------------------------------------- */
static void CB_event(ptl_event_t *ev)
{
  ptl_size_t offset = ev->offset;
  ptl_match_bits_t   mbits = ev->match_bits;
  uint8_t msg_type, amflag, numarg, ghandler;

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("CB event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)offset,(uint64_t)mbits,msg_type));

  /* extract the lower bits based on message type */
  if (msg_type & GASNETC_PTL_MSG_AM) {
    GASNETC_GET_AM_LOWBITS(mbits, numarg, ghandler, amflag);
  } else {
    gasneti_fatalerror("Invalid event msg type on CB, mbits = 0x%lx",(uint64_t)mbits);
  }
  /* we always truncate on this MD */
  gasneti_assert(ev->mlength == 0);

  switch (ev->type) {
  case PTL_EVENT_PUT_END:

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
static void RAR_init()
{
  ptl_md_t md;
  ptl_handle_me_t me1_h, me2_h;
  ptl_process_id_t  match_id;
  void* rar_start   = gasneti_seginfo[gasneti_mynode].addr;
  size_t rar_len    = gasneti_seginfo[gasneti_mynode].size;

  GASNETI_TRACE_PRINTF(C,("RAR_init with len = %lu at %p",(unsigned long)rar_len,rar_start));

  match_id.nid = PTL_NID_ANY;
  match_id.pid = PTL_PID_ANY;

  gasnetc_RAR.start = rar_start;
  gasnetc_RAR.nbytes = rar_len;
  gasnetc_RAR.alignment = GASNET_PAGESIZE;
  gasnetc_RAR.actual_start = NULL;      /* this gets lost in gasneti_segmentattach, so cant free */
  gasnetc_RAR.name = gasneti_strdup("RAR");
  gasnetc_RAR.use_chunks = 0;

  /* Insert a MLE at the head of the list */
  GASNETC_PTLSAFE(PtlMEAttach(gasnetc_ni_h, GASNETC_PTL_RAR_PTE, match_id, GASNETC_PTL_RAR_BITS,
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
  gasnetc_RARAM.alignment = GASNET_PAGESIZE;
  gasnetc_RARAM.actual_start = NULL;      /* this gets lost in gasneti_segmentattach, so cant free */
  gasnetc_RARAM.name = gasneti_strdup("RARAM");
  gasnetc_RARAM.use_chunks = 0;

  md.start = rar_start;
  md.length = rar_len;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_OP_PUT | PTL_MD_MANAGE_REMOTE | PTL_MD_EVENT_START_DISABLE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uint64_t)GASNETC_RARAM_MD;
#else
  md.user_ptr = (void*)RARAM_event;
#endif
  md.eq_handle = gasnetc_AM_EQ->eq_h;

  GASNETC_PTLSAFE(PtlMEInsert(gasnetc_RAR.me_h, match_id, GASNETC_PTL_RARAM_BITS,
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
  gasnetc_RARSRC.alignment = GASNET_PAGESIZE;
  gasnetc_RARSRC.actual_start = NULL;      /* this gets lost in gasneti_segmentattach, so cant free */
  gasnetc_RARSRC.name = gasneti_strdup("RARSRC");
  gasnetc_RARSRC.use_chunks = 0;

  md.start = rar_start;
  md.length = rar_len;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_OP_PUT | PTL_MD_MANAGE_REMOTE | PTL_MD_EVENT_START_DISABLE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uint64_t)GASNETC_RARSRC_MD;
#else
  md.user_ptr = (void*)RARSRC_event;
#endif
  md.eq_handle = gasnetc_SAFE_EQ->eq_h;

  GASNETC_PTLSAFE(PtlMEInsert(gasnetc_RARAM.me_h, match_id, GASNETC_PTL_RARSRC_BITS,
			      GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_AFTER,
			      &gasnetc_RARSRC.me_h));
  GASNETC_PTLSAFE(PtlMDAttach(gasnetc_RARSRC.me_h, md, PTL_RETAIN, &gasnetc_RARSRC.md_h));
}

/* ------------------------------------------------------------------------------------
 * Remove the memory descriptors associated with the Remote Access Region, but dont
 * deallocate the memory.
 * --------------------------------------------------------------------------------- */
static void RAR_exit()
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
static void RplSB_init()
{
  ptl_md_t md;

  gasnetc_chunk_init(&gasnetc_RplSB, "RplSB", gasnetc_RplSB_numchunk);

  md.start = gasnetc_RplSB.start;
  md.length = gasnetc_RplSB.nbytes;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_EVENT_START_DISABLE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uint64_t)GASNETC_REQSB_MD;
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
static void RplSB_exit()
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
static void ReqRB_init()
{
  int i;
  ptl_md_t md;
  size_t nbytes = gasnetc_ReqRB_numchunk * GASNETC_CHUNKSIZE;
  gasnetc_PtlBuffer_t *p;
  ptl_handle_me_t me_h;
  ptl_process_id_t  match_id;
  char name[32];

  match_id.nid = PTL_NID_ANY;
  match_id.pid = PTL_PID_ANY;

  p = gasnetc_ReqRB = (gasnetc_PtlBuffer_t*)gasneti_malloc(gasnetc_ReqRB_pool_size*sizeof(gasnetc_PtlBuffer_t));
  gasneti_assert(gasnetc_ReqRB != NULL);
  for (i = 0; i < gasnetc_ReqRB_pool_size; i++) {
    sprintf(&name[0],"ReqRB_%02d",i);

    gasnetc_buf_init(p,name,nbytes,sizeof(double));

    md.start = p->start;
    md.length = p->nbytes;
    md.threshold = PTL_MD_THRESH_INF;
    md.max_size = GASNETC_CHUNKSIZE;
    md.options = PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE | PTL_MD_MAX_SIZE;
#if GASNETC_REQRB_AUTO_UNLINK
    /* Not advised.  See notes above where GASNETC_REQRB_AUTO_UNLINK is defined */
    /* NOTE: these flags are Cray extensions to the spec */
    md.options |= PTL_MD_FLAG_AUTO_UNLINK | PTL_MD_EVENT_AUTO_UNLINK_ENABLE;
#endif

#if GASNETC_USE_EQ_HANDLER
    md.user_ptr = (void*)(uint64_t)GASNETC_REQRB_MD;
#else
    md.user_ptr = (void*)ReqRB_event;
#endif
    md.eq_handle = gasnetc_AM_EQ->eq_h;

    if (i == 0) {
      /* make first in list */
      GASNETC_PTLSAFE(PtlMEAttach(gasnetc_ni_h, GASNETC_PTL_AM_PTE, match_id, GASNETC_PTL_REQRB_BITS, GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_BEFORE, &p->me_h));
    } else {
      /* insert after i-1 */
      GASNETC_PTLSAFE(PtlMEInsert(gasnetc_ReqRB[i-1].me_h, match_id, GASNETC_PTL_REQRB_BITS, GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_AFTER, &p->me_h));
    }
    GASNETC_PTLSAFE(PtlMDAttach(p->me_h, md, PTL_UNLINK, &(p->md_h)));

    GASNETI_TRACE_PRINTF(C,("ReqRB_init[%d]: %s %lu bytes me=%lu md=%lu",i,p->name,(ulong)nbytes,(ulong)p->me_h,(ulong)p->md_h));

#if GASNETC_DEBUG_RB_VERBOSE
    printf("[%d] ReqRB_Init: buffer %s at start address %lx\n",gasneti_mynode,p->name,(uintptr_t)p->start); fflush(stdout);
#endif

    p++;
  }

  /* Now add the Catch-Basin MD */
  gasnetc_buf_init(&gasnetc_CB,"Catch_Basin",0,0);
  md.start = NULL;
  md.length = 0;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE | PTL_MD_ACK_DISABLE | PTL_MD_TRUNCATE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uint64_t)GASNETC_CB_MD;
#else
  md.user_ptr = (void*)CB_event;
#endif
  md.eq_handle = gasnetc_SAFE_EQ->eq_h;
  GASNETC_PTLSAFE(PtlMEInsert(gasnetc_ReqRB[gasnetc_ReqRB_pool_size-1].me_h, match_id, GASNETC_PTL_REQRB_BITS, GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_AFTER, &gasnetc_CB.me_h));
  GASNETC_PTLSAFE(PtlMDAttach(gasnetc_CB.me_h, md, PTL_RETAIN, &gasnetc_CB.md_h));

  GASNETI_TRACE_PRINTF(C,("CB_init: %s me=%lu md=%lu",gasnetc_CB.name,(ulong)gasnetc_CB.me_h,(ulong)gasnetc_CB.md_h));

}

/* ---------------------------------------------------------------------------------
 * Cleanup Request Receive Buffer resources.
 * --------------------------------------------------------------------------------- */
static void ReqRB_exit()
{
  int i;

  for (i = 0; i < gasnetc_ReqRB_pool_size; i++) {
    /* This should also unlink the associated MEs */
    GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_ReqRB[i].md_h));
    gasnetc_buf_free(&gasnetc_ReqRB[i]);
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
static void ReqSB_init()
{
  ptl_md_t md;
  gasnetc_PtlBuffer_t *p = &gasnetc_ReqSB;
  ptl_process_id_t  match_id;

  match_id.nid = PTL_NID_ANY;
  match_id.pid = PTL_PID_ANY;

  gasnetc_chunk_init(p, "ReqSB", gasnetc_ReqSB_numchunk);

  /* construct a memory descriptor for the Request Send Buffer and attach to AM PTE */
  md.start = gasnetc_ReqSB.start;
  md.length = gasnetc_ReqSB.nbytes;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_EVENT_START_DISABLE | PTL_MD_OP_PUT | PTL_MD_MANAGE_REMOTE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uint64_t)GASNETC_REQSB_MD;
#else
  md.user_ptr = (void*)ReqSB_event;
#endif
  md.eq_handle = gasnetc_SAFE_EQ->eq_h;

  /* Insert this after the Catch-Basin ME entry (at end of list) */
  GASNETC_PTLSAFE(PtlMEInsert(gasnetc_CB.me_h, match_id, GASNETC_PTL_REQSB_BITS, GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_AFTER, &p->me_h));
  GASNETC_PTLSAFE(PtlMDAttach(p->me_h, md, PTL_UNLINK, &p->md_h ));

  GASNETI_TRACE_PRINTF(C,("ReqSB_init: %s %lu chunks me=%lu md=%lu",p->name,(ulong)gasnetc_ReqSB_numchunk,(ulong)p->me_h,(ulong)p->md_h));

}

/* ---------------------------------------------------------------------------------
 * Cleanup Request Send Buffer Resources
 * --------------------------------------------------------------------------------- */
static void ReqSB_exit()
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
    gasnetc_dll_index_t head = gasnetc_scavenge_list;
    if (head == GASNETC_DLL_NULL) {
      /* empty list */
      s->link.next = s->link.prev = node;
      gasnetc_scavenge_list = node;
    } else {
      /* add to the end of the list */
      gasnetc_dll_index_t prev = gasnetc_conn_state[head].link.prev;
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
    gasnetc_dll_index_t next = s->link.next;
    gasnetc_dll_index_t prev = s->link.prev;
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
    gasnetc_dll_index_t node = gasnetc_scavenge_list;
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
  FILE *fh;
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
void gasnetc_scavenge_for_credits()
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
      gasnet_node_t sender = (gasnet_node_t)arg0;
      int exitcode = arg1;
      gasneti_assert(sender >=0 && sender < gasneti_nodes);
      /* mark that we got a shutdown message from this node */
      gasnetc_conn_state[sender].flags |= GASNETC_SYS_GOT_SHUTDOWN_MSG;
      GASNETI_TRACE_PRINTF(C,("Got SHUTDOWN Request from node %d",sender));
      if (!gasnetc_shutdownInProgress) gasnetc_exit(exitcode);
    }
    break;

  case GASNETC_SYS_BARRIER_ARRIVE:
    {
      /* we are root and message that a node has arrived at a barrier */
      int sender = arg0;
      int b_cnt = arg1;
      gasneti_assert(gasneti_mynode == 0);
      gasneti_weakatomic_increment(&sys_barrier_checkin,0);
      GASNETI_TRACE_PRINTF(C,("Got BARRIER_ARRIVE from node %d, cnt=%d",sender,b_cnt));
    }
    break;

  case GASNETC_SYS_BARRIER_GO:
    {
      /* we are root and message that a node has arrived at a barrier */
      int sender = arg0;
      int b_cnt = arg1;
      gasneti_assert(sender == 0);
      gasneti_assert(b_cnt == gasneti_weakatomic_read(&sys_barrier_cnt,0));
      GASNETI_TRACE_PRINTF(C,("Got BARRIER_GO from node %d, cnt=%d",sender,b_cnt));
      /* let poller know its ok to proceed */
      gasneti_weakatomic_set(&sys_barrier_got,b_cnt,0);
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
  ptl_size_t offset = ev->offset;
  ptl_match_bits_t   mbits = ev->match_bits;

  switch (ev->type) {

  case PTL_EVENT_PUT_END:
    /* Must be a system message */
    {
      gasnetc_sys_t msg_id = (gasnetc_sys_t) ((mbits & GASNETC_SELECT_BYTE1)>>8);
      int32_t arg0 = (int32_t) ((mbits & GASNETC_SELECT_UPPER32) >> 32);
      int32_t arg1 = (int32_t) ((ev->hdr_data & GASNETC_SELECT_UPPER32) >> 32);
      int32_t arg2 = (int32_t) (ev->hdr_data & GASNETC_SELECT_LOWER32);
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
static void sys_init()
{
  ptl_size_t eq_len = 2*gasneti_nodes + 10;
  ptl_md_t   md;
  ptl_process_id_t  match_id;

  match_id.nid = PTL_NID_ANY;
  match_id.pid = PTL_PID_ANY;

  /*  printf("[%d] SYS_init: allocated %ld events on SYS_EQ\n",(int)gasneti_mynode,(long)eq_len); */
  gasnetc_SYS_EQ = gasnetc_eq_alloc(eq_len,"SYS_EQ",NULL);

  /* allocate the SYS send buffer */
  gasnetc_buf_init(&gasnetc_SYS_Send,"SYS_Send",0,0);
  md.start = NULL;
  md.length = 0;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_EVENT_START_DISABLE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uint64_t)GASNETC_SYS_SEND_MD;
#else
  md.user_ptr = (void*)sys_event;
#endif
  md.eq_handle = PTL_EQ_NONE;

  GASNETC_PTLSAFE(PtlMDBind(gasnetc_ni_h, md, PTL_RETAIN, &gasnetc_SYS_Send.md_h));
  GASNETI_TRACE_PRINTF(C,("SYS_init: %s initialized, md=%lu",gasnetc_SYS_Send.name,(ulong)gasnetc_SYS_Send.md_h));

  /* allocate the SYS receive buffer */
  gasnetc_buf_init(&gasnetc_SYS_Recv,"SYS_Recv",0,0);
  md.start = NULL;
  md.length = 0;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE | PTL_MD_ACK_DISABLE | PTL_MD_TRUNCATE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uint64_t)GASNETC_SYS_RECV_MD;
#else
  md.user_ptr = (void*)sys_event;
#endif
  md.eq_handle = gasnetc_SYS_EQ->eq_h;

  /* Insert a MLE at the head of the list */
  GASNETC_PTLSAFE(PtlMEAttach(gasnetc_ni_h, GASNETC_PTL_AM_PTE, match_id, GASNETC_PTL_SYS_BITS,
			      GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_BEFORE, &gasnetc_SYS_Recv.me_h));
  GASNETC_PTLSAFE(PtlMDAttach(gasnetc_SYS_Recv.me_h, md, PTL_RETAIN, &gasnetc_SYS_Recv.md_h));

  GASNETI_TRACE_PRINTF(C,("SYS_init: %s me=%lu md=%lu",gasnetc_SYS_Recv.name,(ulong)gasnetc_SYS_Recv.me_h,(ulong)gasnetc_SYS_Recv.md_h));

  gasneti_weakatomic_set(&sys_barrier_cnt, 0, 0);
  gasneti_weakatomic_set(&sys_barrier_got, 0, 0);
  gasneti_weakatomic_set(&sys_barrier_checkin, 0, 0);

  /* make sure everyone has done this before proceeding */
  gasnetc_bootstrapBarrier();

  portals_sysqueue_initialized = 1;
  /* write barrier here? */

}

/* ---------------------------------------------------------------------------------
 * Remove the system SYS resources
 * --------------------------------------------------------------------------------- */
static void sys_exit()
{
  ptl_event_t ev;
  /* these will automatically unlink the match-list entries as well */
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_SYS_Send.md_h));
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_SYS_Recv.md_h));
  /* drain the queue */
  while (gasnetc_get_event(gasnetc_SYS_EQ,&ev)) {};
  gasnetc_eq_free(gasnetc_SYS_EQ);
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
  match_bits = ((uint64_t)arg0 << 32) | ((uint64_t)msg_id << 8) | GASNETC_PTL_SYS_BITS;
  hdr_data = ((uint64_t)arg1 << 32) | (uint64_t)arg2;
  GASNETC_PTLSAFE(PtlPutRegion(md_h, local_offset, msg_bytes, PTL_NOACK_REQ, target_id, GASNETC_PTL_AM_PTE, GASNETC_PTL_AC_ID, match_bits, remote_offset, hdr_data));

}

extern void gasnetc_sys_barrier(void)
{
  gasnet_node_t node;
  int barr_cnt;
  gasneti_assert(portals_sysqueue_initialized);

  gasneti_weakatomic_increment(&sys_barrier_cnt,0);
  barr_cnt = gasneti_weakatomic_read(&sys_barrier_cnt, 0);
  GASNETI_TRACE_PRINTF(C,("Entering SYS BARRIER cnt=%d",barr_cnt));
  if (gasneti_mynode == 0) {
    /* wait for all other nodes to check in */
    while (gasneti_weakatomic_read(&sys_barrier_checkin,0) < gasneti_nodes-1) gasnetc_sys_poll();

    /* reset this for next barrier */
    gasneti_weakatomic_set(&sys_barrier_checkin, 0, 0);

    /* send message to all other nodes */
    for (node = 1; node < gasneti_nodes; node++)
      gasnetc_sys_SendMsg(node,GASNETC_SYS_BARRIER_GO,0,barr_cnt,0);
  } else {

    /* reply to this message will set this variable to 1 */
    gasneti_weakatomic_set(&sys_barrier_got, 0, 0);
    /* send signal to node 0 */
    gasnetc_sys_SendMsg(0,GASNETC_SYS_BARRIER_ARRIVE,gasneti_mynode,barr_cnt,0);

    /* wait for node 0 to reply */
    while (!gasneti_weakatomic_read(&sys_barrier_got,0)) gasnetc_sys_poll();
  }
}

/* =================================================================================
 * This lower portion of the file is where exported functions are located.
 * These are exported to both the Core and Extended API implementations.
 * ================================================================================= */

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
  uint32_t          maxnodes = (uint32_t)((gasnetc_dll_index_t)-1);
 
  gasneti_mynode = cnos_get_rank();
  gasneti_nodes = cnos_get_size();

  /* init tracing as early as possible */
  gasneti_trace_init(argc, argv);

  if (gasneti_nodes >= maxnodes) {
    gasneti_fatalerror("GASNet Portals conduit designed to work for up to %d nodes,"
		       " this job uses %d nodes.  Modify size of gasnetc_dll_index_t "
		       " and rebuild library",maxnodes-1,gasneti_nodes);
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
  /* MLW: can we use gasneti_getenv here? */
  if (getenv("GASNET_PORTAL_ACCEL") != NULL) {
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

#if PLATFORM_OS_CNL
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
  if(gasneti_nodes != cnos_get_nidpid_map(&cnos_map)) {
    gasneti_fatalerror("cnos_get_nidpid_map size != %d",gasneti_nodes);
  }

  gasneti_assert_always(cnos_map[gasneti_mynode].nid == gasnetc_myid.nid);
  gasneti_assert_always(cnos_map[gasneti_mynode].pid == (gasnetc_myid.pid - pid_offset));

  gasnetc_procid_map = (gasnetc_procid_t*)gasneti_malloc(gasneti_nodes * sizeof(gasnetc_procid_t));
  for (node = 0; node < gasneti_nodes; node++) {
    gasnetc_procid_map[node].node_id = node;
    gasnetc_procid_map[node].ptl_id.nid = cnos_map[node].nid;
    gasnetc_procid_map[node].ptl_id.pid = cnos_map[node].pid + pid_offset;
    gasnetc_procid_map[node].next = NULL;
  }

  /* init the table to a list of null pointers */
  for (i = 0; i < HASHTABLE_SIZE; i++) {
    gasnetc_addrtable[i] = NULL;
  }

  /* now populate the table */
  for (node = 0; node < gasneti_nodes; node++) {
    gasnetc_procid_t *proc = &gasnetc_procid_map[node];
    int indx = HASHFUNC(&proc->ptl_id);
    proc->next = gasnetc_addrtable[indx];
    gasnetc_addrtable[indx] = proc;
  }

#ifdef GASNET_DEBUG
  {
    int i;
    int numzero = 0;
    int sum = 0;
    int mincnt = gasneti_nodes+1;
    int maxcnt = 0;
    double avg;
    for (i = 0; i < HASHTABLE_SIZE; i++) {
      int cnt = 0;
      gasnetc_procid_t *p = gasnetc_addrtable[i];
      if (p == NULL) numzero++;
      while (p != NULL) {
	GASNETI_TRACE_PRINTF(C,("Table[%d][%d] :: Node=%d, Nid=%d, Pid=%d",i,cnt,p->node_id,p->ptl_id.nid,p->ptl_id.pid));
	cnt++;
	p = p->next;
      }
      mincnt = MIN(cnt,mincnt);
      maxcnt = MAX(cnt,maxcnt);
      sum += cnt;
    }
    gasneti_assert(sum == gasneti_nodes);
    avg = ((double)sum)/((double)HASHTABLE_SIZE);
    GASNETI_TRACE_PRINTF(C,("Table stats: NumZero=%d, AvgLen=%6.2f, MinLen=%d, MaxLen=%d",numzero,avg,mincnt,maxcnt));
  }
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

}

/* Function to convert a ptl_process_id_t to a GASNet Node id */
extern gasnet_node_t gasnetc_get_nodeid(ptl_process_id_t *proc)
{
  int indx = HASHFUNC(proc);
  gasnetc_procid_t *p = gasnetc_addrtable[indx];
  while (p != NULL) {
    if ((p->ptl_id.nid == proc->nid) && (p->ptl_id.pid == proc->pid)) {
      return p->node_id;
    }
    p = p->next;
  }
  gasneti_fatalerror("gasnetc_get_nodeid failed with nid=%d,pid=%d, table index=%d",proc->nid,proc->pid,indx);
  return -1;
}

/* ---------------------------------------------------------------------------------
 * Bootstrap barrier function.
 * Just use cnos_barrier on XT3, but might have to init it first.
 * --------------------------------------------------------------------------------- */
extern void gasnetc_bootstrapBarrier() {
  static int gasnetc_bootstrapBarrierCnt = 0;

  gasnetc_bootstrapBarrierCnt++;

  /* check the system queue if after network init */
  if (portals_sysqueue_initialized) {
    gasnetc_sys_barrier();
  } else {
    GASNETI_TRACE_PRINTF(C,("bootstrapBarrier count = %d",gasnetc_bootstrapBarrierCnt));
    cnos_barrier();
  }
}

/* ---------------------------------------------------------------------------------
 * Bootstrap exchange function over portals
 * After the network has been initialized, but before all the conduit resources
 * have been allocated, can use this function to perform the equivelent of
 * an MPI_Broadcast.  The root node will broadcast its info to all other nodes.
 * --------------------------------------------------------------------------------- */
extern void gasnetc_bootstrapBroadcast(void *src, size_t len, void *dest, int rootnode)
{
  ptl_md_t src_md, dest_md;
  ptl_handle_me_t dest_me_h;
  ptl_handle_md_t src_h, dest_h;
  ptl_handle_eq_t eq_h;
  ptl_process_id_t  match_id;
  ptl_event_t ev;
  ptl_match_bits_t match_bits  = 0x0F0F0F0F0F0F0F0F;
  ptl_match_bits_t ignore_bits = 0x0000000000000000;
  int eq_len;
  int i, rc;

  match_id.nid = PTL_NID_ANY;
  match_id.pid = PTL_PID_ANY;

  GASNETI_TRACE_PRINTF(C,("bootBroadcast from %d len = %d, src=%p dest=%p",rootnode,(int)len,src,dest));

  if (gasneti_mynode != rootnode) {
    /* alloc an event queue for the action */
    GASNETC_PTLSAFE(PtlEQAlloc(gasnetc_ni_h, eq_len, NULL, &eq_h));

    /* register the dest md with an EQ on a match list */
    dest_md.start = dest;
    dest_md.length = len;
    dest_md.threshold = PTL_MD_THRESH_INF;
    dest_md.max_size = 0;
    dest_md.options = PTL_MD_EVENT_START_DISABLE | PTL_MD_OP_PUT | PTL_MD_MANAGE_REMOTE;
    dest_md.user_ptr = 0;
    dest_md.eq_handle = eq_h;

    /* construct the match entry */
    GASNETC_PTLSAFE(PtlMEAttach(gasnetc_ni_h, GASNETC_PTL_AM_PTE, match_id, match_bits, ignore_bits, PTL_UNLINK, PTL_INS_AFTER, &dest_me_h));

    /* attach the dest memory descriptor */
    GASNETC_PTLSAFE(PtlMDAttach(dest_me_h, dest_md, PTL_RETAIN, &dest_h));
  }

  /* need barrier here to insure everyone is ready to go */
  gasnetc_bootstrapBarrier();

  if (gasneti_mynode == rootnode) {
    int found = 0;
    /* alloc an event queue for the action */
    eq_len = 2*gasneti_nodes;

    GASNETC_PTLSAFE(PtlEQAlloc(gasnetc_ni_h, eq_len, NULL, &eq_h));

    /* register the src md with no event queue */
    src_md.start = src;
    src_md.length = len;
    src_md.threshold = PTL_MD_THRESH_INF;
    src_md.max_size = 0;
    src_md.options = PTL_MD_EVENT_START_DISABLE;
    src_md.user_ptr = 0;
    src_md.eq_handle = eq_h;
    GASNETC_PTLSAFE(PtlMDBind(gasnetc_ni_h, src_md, PTL_RETAIN, &src_h));

    /* spray out the message */
    for (i = 0; i < gasneti_nodes; i++) {
      if (i != gasneti_mynode) {
	GASNETC_PTLSAFE(PtlPut(src_h,PTL_NOACK_REQ, gasnetc_procid_map[i].ptl_id,GASNETC_PTL_AM_PTE,GASNETC_PTL_AC_ID,match_bits,0,0));
      }
    }

    while (found < (gasneti_nodes - 1)) {
      rc = PtlEQWait(eq_h, &ev);
      switch (rc) {
      case PTL_OK:
	gasneti_assert_always( ev.type == PTL_EVENT_SEND_END );
	break;
      default:
	gasneti_fatalerror("GASNet Portals Error in bootExchange waiting for event: %s (%i)\n at %s\n",
			   ptl_err_str[rc],rc,gasneti_current_loc);
      };
    }

    /* now remove the src MD */
    GASNETC_PTLSAFE(PtlMDUnlink(src_h));
    /* reclaim the event queue */
    GASNETC_PTLSAFE(PtlEQFree(eq_h));

  } else {
    eq_len = 10;
    /* wait the the message that data has arrived */
    rc = PtlEQWait(eq_h, &ev);
    switch (rc) {
    case PTL_OK:
      gasneti_assert_always( ev.type == PTL_EVENT_PUT_END );
      break;
    default:
      gasneti_fatalerror("GASNet Portals Error in bootExchange waiting for event: %s (%i)\n at %s\n",
			 ptl_err_str[rc],rc,gasneti_current_loc);
    };
  
    /* now remove the dest MD, which will also unlink the match-list entry */
    GASNETC_PTLSAFE(PtlMDUnlink(dest_h));

    /* reclaim the event queue */
    GASNETC_PTLSAFE(PtlEQFree(eq_h));
  }

  /* should not need a barrier here */
  GASNETI_TRACE_PRINTF(C,("bootBroadcast exit"));
}


/* ---------------------------------------------------------------------------------
 * Bootstrap exchange function over portals
 * After the network has been initialized, but before all the conduit resources
 * have been allocated, can use this function to perform the equivelent of
 * an MPI_Allgather.  Each node will broadcast its info to all other nodes.
 * --------------------------------------------------------------------------------- */
extern void gasnetc_bootstrapExchange(void *src, size_t len, void *dest)
{
  ptl_md_t src_md, dest_md;
  ptl_handle_me_t dest_me_h;
  ptl_handle_md_t src_h, dest_h;
  ptl_handle_eq_t eq_h;
  int eq_len = gasneti_nodes * 4;
  ptl_process_id_t  match_id;
  int found = 0;
  ptl_event_t ev;
  ptl_match_bits_t match_bits  = 0xF0F0F0F0F0F0F0F0;
  ptl_match_bits_t ignore_bits = 0x0000000000000000;
  int dest_offset = gasneti_mynode*len;
  int i;

  match_id.nid = PTL_NID_ANY;
  match_id.pid = PTL_PID_ANY;

  GASNETI_TRACE_PRINTF(C,("bootExch with len = %d, src = %p dest = %p",(int)len,src,dest));

  /* alloc an event queue for the action */
  GASNETC_PTLSAFE(PtlEQAlloc(gasnetc_ni_h, eq_len, NULL, &eq_h));

  /* register the src md with EQ */
  src_md.start = src;
  src_md.length = len;
  src_md.threshold = PTL_MD_THRESH_INF;
  src_md.max_size = 0;
  src_md.options = PTL_MD_EVENT_START_DISABLE;
  src_md.user_ptr = 0;
  src_md.eq_handle = PTL_EQ_NONE;
  src_md.eq_handle = eq_h;
  GASNETC_PTLSAFE(PtlMDBind(gasnetc_ni_h, src_md, PTL_RETAIN, &src_h));

  /* register the dest md with an EQ on a match list */
  dest_md.start = dest;
  dest_md.length = len*gasneti_nodes;
  dest_md.threshold = PTL_MD_THRESH_INF;
  dest_md.max_size = 0;
  dest_md.options = PTL_MD_EVENT_START_DISABLE | PTL_MD_OP_PUT | PTL_MD_MANAGE_REMOTE;
  dest_md.user_ptr = 0;
  dest_md.eq_handle = eq_h;

  /* construct the match entry */
  GASNETC_PTLSAFE(PtlMEAttach(gasnetc_ni_h, GASNETC_PTL_AM_PTE, match_id, match_bits, ignore_bits, PTL_UNLINK, PTL_INS_AFTER, &dest_me_h));

  /* attach the dest memory descriptor */
  GASNETC_PTLSAFE(PtlMDAttach(dest_me_h, dest_md, PTL_RETAIN, &dest_h));

  /* need barrier here to insure everyone is ready to go */
  gasnetc_bootstrapBarrier();

  /* spray out the message */
  for (i = 0; i < gasneti_nodes; i++) {
    if (i == gasneti_mynode) {
      memcpy(((uint8_t*)dest+dest_offset),src,len); 
    } else {
      GASNETC_PTLSAFE(PtlPut(src_h,PTL_NOACK_REQ, gasnetc_procid_map[i].ptl_id,GASNETC_PTL_AM_PTE,GASNETC_PTL_AC_ID,match_bits,dest_offset,0));
    }
  }

  /* now poll EQ until we see 2*(N-1) PUT_END/SEND_END events */
  while (found < 2*(gasneti_nodes-1)) {
    int rc = PtlEQWait(eq_h, &ev);
    switch (rc) {
    case PTL_OK:
      gasneti_assert_always( (ev.type == PTL_EVENT_PUT_END) || (ev.type == PTL_EVENT_SEND_END) );
      found++;
      break;
    default:
      gasneti_fatalerror("GASNet Portals Error in bootExchange waiting for event: %s (%i)\n at %s\n",
			 ptl_err_str[rc],rc,gasneti_current_loc);
    };
  }

  gasnetc_bootstrapBarrier(); /* MLW not needed */
  
  /* now remove the src MD */
  GASNETC_PTLSAFE(PtlMDUnlink(src_h));

  /* now remove the dest MD, which will also unlink the match-list entry */
  GASNETC_PTLSAFE(PtlMDUnlink(dest_h));

  /* reclaim the event queue */
  GASNETC_PTLSAFE(PtlEQFree(eq_h));

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
static int try_pin(uintptr_t size)
{
  ptl_md_t md;
  ptl_handle_md_t md_h;
  int rc, ok;
  void *mem = gasneti_malloc_allowfail(size);

  if (mem == NULL) return 0;

  /* poll system queue here since these operations can take some time */
  gasnetc_sys_poll();

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
  gasneti_free(mem);
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
  void *mem = NULL;
#undef MBYTE

#if PLATFORM_OS_CNL
  /* On CNL, if we try to pin beyond what the OS will allow, the job is killed.
   * So, there is really no way (that we know of) to determine the maximum
   * pinnable memory under CNL without dire consequences.
   * For this platform, we will simply return a large value and if the user
   * requests a value larger than what can be pinned, the job will be killed.
   */
  return (uintptr_t)-1;
#endif

  /* make sure we can pin at least the initial low watermark of memory */
  if (! try_pin(low)) {
    gasneti_fatalerror("Unable to alloc and pin minimal memory of size %d bytes",(int)low);
  }
  high = low;

  /* move high boundary up (exponentially) intil it will no longer pin */
  prev = low;
  high = prev*2;
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
  do {
    uint64_t mid = (low + high)/2;
    if (try_pin(mid)) {
      low = mid;
    } else {
      high = mid;
    }
  } while ((high - low) > granularity);
  if (try_pin(high)) {
    low = high;
  }
  GASNETI_TRACE_PRINTF(C,("MaxPinMem = %lu",(unsigned long)low));
  return (uintptr_t)low;
}

/* ---------------------------------------------------------------------------------
 * get a chunk from the allocator.
 * returns 1=TRUE on success, 0=FAIL if not able to satisfly request.
 * --------------------------------------------------------------------------------- */
extern int gasnetc_chunk_alloc(gasnetc_PtlBuffer_t *buf, size_t nbytes, ptl_size_t *offset)
{
    gasnetc_chunk_t *p;
    
    gasneti_assert(buf->use_chunks);

    if (nbytes > GASNETC_CHUNKSIZE) {
      gasneti_fatalerror("gasnetc_chunk_alloc requested %lu bytes, limit is %lu",(ulong)nbytes,(ulong)GASNETC_CHUNKSIZE);
    }
    /* MLW: Could have an atomic var, or just a regular volitile "isempty" var that is set
     * when empty and unset when items are on the free list.  That would prevent having to
     * gain lock to check if freelist is empty.  It would be last var set before unlock
     * and membar would insure reads would reflect it.  Of course, would still have to
     * check freelist condition when lock is gotten
     */
    gasneti_mutex_lock(&buf->lock);
    if (buf->freelist == NULL) {
      gasneti_mutex_unlock(&buf->lock);
      return 0;
    }
    p = buf->freelist;
    buf->freelist = p->next;
    *offset = ((uint8_t*)p - (uint8_t*)(buf->start));
#if GASNETI_STATS_OR_TRACE
    buf->inuse++;
    if (buf->inuse > buf->hwm) buf->hwm = buf->inuse;
    GASNETI_TRACE_PRINTF(C,("CHUNK_ALLOC: name %s, inuse = %d, hwm = %d, offset=%lu",buf->name,buf->inuse,buf->hwm,(unsigned long)*offset));
    GASNETI_TRACE_EVENT(C, CHUNK_ALLOC);
#endif
    gasneti_mutex_unlock(&buf->lock);

    return 1;
}
/* ---------------------------------------------------------------------------------
 * get a chunk from the allocator.
 * returns 1=TRUE on success, 0=FAIL if not able to satisfly request.
 * May poll network at most pollmax times.
 * --------------------------------------------------------------------------------- */
extern int gasnetc_chunk_alloc_withpoll(gasnetc_PtlBuffer_t *buf, size_t nbytes, ptl_size_t *offset,
					int pollmax, gasnetc_pollflag_t poll_type)
{
    gasnetc_chunk_t *p;
    int cnt = 0;
    int gotone = 0;
    
    gasneti_assert(buf->use_chunks);
    gasneti_assert(pollmax > 0);
    gasneti_assert(poll_type != GASNETC_NO_POLL);

    gotone = gasnetc_chunk_alloc(buf,nbytes,offset);
    if (gotone) return gotone;

    /* poll up to pollmax times, waiting for chunk to free-up */
    while (cnt < pollmax) {
      if (poll_type == GASNETC_FULL_POLL) {
	gasneti_AMPoll();
      } else if (poll_type == GASNETC_SAFE_POLL) {
	gasnetc_portals_poll(poll_type);
      }
      cnt++;

      gotone = gasnetc_chunk_alloc(buf,nbytes,offset);
      if (gotone) break;
    }

    return gotone;
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
    gasnetc_chunk_t *p = (gasnetc_chunk_t*)((uint8_t*)buf->start + offset);
    gasneti_assert(buf->use_chunks);
    
    gasneti_mutex_lock(&buf->lock);
    p->next = buf->freelist;
    buf->freelist = p;
#if GASNETI_STATS_OR_TRACE
    buf->inuse--;
    GASNETI_TRACE_PRINTF(C,("CHUNK_FREE: name %s, inuse = %d, hwm = %d, offset=%lu",buf->name,buf->inuse,buf->hwm,(unsigned long)offset));
    GASNETI_TRACE_EVENT(C, CHUNK_FREE);
#endif
    gasneti_mutex_unlock(&buf->lock);
}

/* bug 2102: PtlEQAlloc/PtlEQFree are not thread-safe */
static gasneti_mutex_t gasnetc_eqalloc_lock = GASNETI_MUTEX_INITIALIZER;

extern gasnetc_eq_t* gasnetc_eq_alloc(long num_events, const char* name, ptl_eq_handler_t hndlr)
{
  gasnetc_eq_t *eq = (gasnetc_eq_t*)gasneti_malloc(sizeof(gasnetc_eq_t));
  eq->num_events = num_events;
  gasneti_mutex_lock(&gasnetc_eqalloc_lock);
    GASNETC_PTLSAFE(PtlEQAlloc(gasnetc_ni_h, num_events, hndlr, &eq->eq_h));
  gasneti_mutex_unlock(&gasnetc_eqalloc_lock);
  gasneti_mutex_init(&eq->lock);
  eq->name = gasneti_strdup(name);
  GASNETI_TRACE_PRINTF(C,("gasnetc_eq_alloc %s with %ld events (%i)",eq->name,num_events,(int)eq->eq_h));
  return eq;
}
extern void gasnetc_eq_free(gasnetc_eq_t *eq)
{
  gasneti_mutex_lock(&gasnetc_eqalloc_lock);
    GASNETC_PTLSAFE(PtlEQFree(eq->eq_h));
  gasneti_mutex_unlock(&gasnetc_eqalloc_lock);
  GASNETI_TRACE_PRINTF(C,("gasnetc_eq_free %s with %ld events (%i)",eq->name,eq->num_events,(int)eq->eq_h));
  gasneti_free(eq->name);
  gasneti_free(eq);
}

  

/* ---------------------------------------------------------------------------------
 * Allocate a temp md to be used as the source of a Put or destination
 * of a Get operation.  MD to be free floating, not target of remote op.
 * Associated with eq_h Event Queue (usually the SAFE_EQ).
 * NOTE:  Assumes caller has already allocated a tmpmd ticket
 * See gasnetc_try_alloc_tmpmd or gasnetc_alloc_tmpmd_withpoll for interfaces
 * that do not already have a ticket.
 * --------------------------------------------------------------------------------- */
extern ptl_handle_md_t gasnetc_alloc_tmpmd(void* start, size_t nbytes, ptl_handle_eq_t eq_h)
{
  ptl_md_t md;
  ptl_handle_md_t md_h;

  GASNETI_TRACE_PRINTF(C,("Alloc_Tmpmd: num available TmpMDs = %d",gasnetc_num_tickets(&gasnetc_tmpmd_tickets)));

  md.start = start;
  md.length = nbytes;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_EVENT_START_DISABLE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uint64_t)GASNETC_TMP_MD;
#else
  md.user_ptr = (void*)TMPMD_event;
#endif
  md.eq_handle = eq_h;

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
  int64_t cred_bytes_per_buffer = (gasnetc_ReqRB_numchunk-1)*GASNETC_CHUNKSIZE;

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
  *nbuf = nb + 1;   /* always one more than credit buffer space */
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
  int          i, rc;
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
  gasnetc_dump_stats = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_STATS",
				 (int64_t)gasnetc_dump_stats,0);
  gasnetc_ReqSB_numchunk = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_SB_CHUNKS",
				 (int64_t)gasnetc_ReqSB_numchunk,0);
  gasnetc_RplSB_numchunk = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_RPL_CHUNKS",
				 (int64_t)gasnetc_RplSB_numchunk,0);
  gasnetc_max_tmpmd = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_NUM_TMPMD",
				 (int64_t)GASNETC_MAX_TMP_MDS,0);
  gasnetc_msg_limit = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_MSG_LIMIT",
				 (int64_t)gasnetc_msg_limit,0);
  gasnetc_allow_packed_long = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_PACKED_LONG",
				 (int64_t)gasnetc_allow_packed_long,0);
  if (gasnetc_msg_limit < gasnetc_msg_minimum) {
    gasnetc_msg_limit = gasnetc_msg_minimum;
  }
  gasnetc_epoch_duration = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_EPOCH_DURATION",
 		                (int64_t)gasnetc_epoch_duration,0);
  val = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_FLOW_CONTROL",
					    (int64_t)gasnetc_use_flow_control,0);
  gasnetc_use_flow_control = val;
  val = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_DYNAMIC_CREDITS",
					    (int64_t)gasnetc_use_dynamic_credits,0);
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
  gasnetc_shutdown_seconds = gasneti_get_exittimeout(shutdown_max, 3., 0.125, 0.);

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
      int cpn_saved = cred_per_node;
      adjust_bufspace_from_cred(&banked, &cred_per_node, &total_credits, &num_reqRB);

#if GASNETC_CREDIT_TESTING
      // MLW: hack to force credits to what we specify in env vars and ignore extras
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
	int64_t banked_save = banked;
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
  gasneti_semaphore_init(&gasnetc_tmpmd_tickets, gasnetc_max_tmpmd, 0);

  /* keep a counter of number of send tickets available */
  gasneti_semaphore_init(&gasnetc_send_tickets, gasnetc_msg_limit, 0);

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

#if 0
#ifndef GASNETC_USE_EQ_HANDLER
  /* Enable the progress function */
  GASNETI_TRACE_PRINTF(C,("Enabling Portals polling function (No EQ Handler)"));
  GASNETI_PROGRESSFNS_ENABLE(gasnete_pf_portals_poll,BOOLEAN);
#endif
#endif
}

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

/* ---------------------------------------------------------------------------------
 * Release Portals resources
 *   - The proc_id map
 *   - Remove MDs and match-list entries
 *   - Free the buffers used for bounce, send/recv
 * --------------------------------------------------------------------------------- */
extern void gasnetc_portals_exit()
{

#define DO_CLEANUP_PORTALS 0
#if DO_CLEANUP_PORTALS
  {
    ptl_event_t ev;
    sys_exit();

    RplSB_exit();
    ReqRB_exit();
    ReqSB_exit();
    RAR_exit();

    /* remove the event queues */
    while (gasnetc_get_event(gasnetc_SAFE_EQ,&ev)) {};
    gasnetc_eq_free(gasnetc_SAFE_EQ);
    gasnetc_SAFE_EQ = NULL;
    while (gasnetc_get_event(gasnetc_AM_EQ,&ev)) {};
    gasnetc_eq_free(gasnetc_AM_EQ);
    gasnetc_AM_EQ = NULL;

    /* free the proc id map */
    gasneti_free(gasnetc_procid_map);

    /* free the AM connection state array */
    gasneti_free(gasnetc_conn_state);

    GASNETC_PTLSAFE(PtlNIFini(gasnetc_ni_h));
  }
#endif
#if PLATFORM_OS_CNL
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
  gasnetc_threaddata_t *th = gasnetc_mythread();

#if defined(GASNET_DEBUG) || defined(GASNETI_STATS_OR_TRACE)
  static int poll_level = 0;
  poll_level++;
  GASNETI_TRACE_PRINTF(C,("Enter Poll with %s, level %d",poll_name[poll_type],poll_level));
#endif

  GASNETI_TRACE_PRINTF(C,("POLL: SendTickets=%d  TmpMD_Tickets=%d",gasnetc_num_tickets(&gasnetc_send_tickets),gasnetc_num_tickets(&gasnetc_tmpmd_tickets)));

  /* should never be called with NO_POLL */
  gasneti_assert(poll_type != GASNETC_NO_POLL);

  /* always poll on the system queue, adds .074 usec to poll, cost of extra PtlEQGet call */
  gasnetc_sys_poll();

  /* always try to get a few events from the SAFE eq first 
   * all puts and gets generate two events so need to reap these queues faster
   * to prevent send_ticket starvation
   */
  while (safe_cnt < gasnetc_safe_poll_limit) {
    if ( gasnetc_get_event(gasnetc_SAFE_EQ, &ev) ) {
      GASNETI_TRACE_PRINTF(C,("Got event %s from SAFE_EQ, md=%lu, mbits=0x%lx, th_id=%d",ptl_event_str[ev.type],(ulong)ev.md_handle,(unsigned long)ev.match_bits,th->threadidx));
      GASNETC_CALL_EQ_HANDLER(ev);
      processed++;
      safe_cnt++;
    } else {
      /* no ready events, stop trying */
      break;
    }
  }

  if (poll_type == GASNETC_FULL_POLL) {
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
       * a ticket cached
       */
      if (th->tmpmd_tickets == 0) {
	if (! gasnetc_alloc_ticket(&gasnetc_tmpmd_tickets)) {
	  goto out;
	} 
	th->tmpmd_tickets++;
      }

      /* Finally, we will need a RplSB chunk, if not already cached, try to alloc one */
      if (! (th->flags & GASNETC_THREAD_HAVE_RPLSB)) {
	if (!gasnetc_chunk_alloc(&gasnetc_RplSB,GASNETC_CHUNKSIZE,&th->rplsb_off)) {
	  goto out;
	} 
	th->flags |= GASNETC_THREAD_HAVE_RPLSB;
      }

      GASNETI_TRACE_PRINTF(C,("PtlPoll: FULL thread=0x%p flags=0x%x",th,th->flags));

      /* if we got here, we have enough resources to poll the AM queue */
      if (gasnetc_get_event(gasnetc_AM_EQ, &ev) ) {
	GASNETI_TRACE_PRINTF(C,("Got event %s from AM_EQ, md=%lu, mbits=0x%lx th_id=%d",ptl_event_str[ev.type],(ulong)ev.md_handle,(ulong)ev.match_bits,th->threadidx));
	GASNETC_CALL_EQ_HANDLER(ev);
	processed++;
	am_cnt++;
      } else {
	goto out;
      }
    } /* end while */
  }

  out:
#if defined(GASNET_DEBUG) || defined(GASNETI_STATS_OR_TRACE)
  GASNETI_TRACE_PRINTF(C,("Leave Poll with %s level %d",poll_name[poll_type],poll_level));
  poll_level--;
#endif

  GASNETI_TRACE_EVENT_VAL(C, EVENT_CNT, processed);
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

  case GASNETC_SYS_SEND:
  case GASNETC_SYS_RECV:
    SYS_event(ev);
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

/* ------------------------------------------------------------------------------------
 * This function does the actual Portals Get operation for the extended API Get
 * operations.
 * If we have reached the put/get limit, we poll as directed.
 * --------------------------------------------------------------------------------- */
void gasnetc_getmsg(void *dest, gasnet_node_t node, void *src, size_t nbytes,
		    ptl_match_bits_t match_bits, gasnetc_pollflag_t pollflag)
{
  ptl_process_id_t target_id = gasnetc_procid_map[node].ptl_id;
  ptl_handle_md_t md_h;
  ptl_ac_index_t ac_index = GASNETC_PTL_AC_ID;
  ptl_size_t local_offset;
  ptl_size_t remote_offset = GASNETC_PTL_OFFSET(node,src);
  
  gasneti_assert(remote_offset >= 0 && remote_offset < gasneti_seginfo[node].size);

  /* stall here if too many puts/gets in progress */
  while( !gasnetc_alloc_ticket(&gasnetc_send_tickets) ) {
    switch (pollflag) {
    case GASNETC_NO_POLL:
      gasneti_fatalerror("gasnetc_getmsg: msg limit but NO_POLL allowed");
      break;
    case GASNETC_SAFE_POLL:
      gasnetc_portals_poll(pollflag);
      break;
    case GASNETC_FULL_POLL:
      gasneti_AMPoll();
      break;
    }
  }

  /* Determine destination MD for Ptl Get */
  if (gasnetc_in_local_rar(dest,nbytes)) {
    md_h = gasnetc_RARSRC.md_h;
    local_offset = GASNETC_PTL_OFFSET(gasneti_mynode,dest);
    GASNETI_TRACE_EVENT(C, GET_RAR);
  } else if ( (nbytes <= (GASNETC_PUTGET_BOUNCE_SIZE - (sizeof(void*))))  &&
	      gasnetc_chunk_alloc_withpoll(&gasnetc_ReqSB, nbytes, &local_offset, 1, GASNETC_SAFE_POLL) ) {
    /* Encode dest addr in BB chunk for later copy */
    void* bb;
    md_h = gasnetc_ReqSB.md_h;
    /* get the addr of the start of the chunk */
    bb = ((uint8_t*)gasnetc_ReqSB.start + local_offset);
    /* store the dest address at this location */
    *(uintptr_t*)bb = (uintptr_t)dest;
    /* Let portals use the rest of the chunk */
    local_offset += sizeof(void*);
    match_bits |= ((uint64_t)local_offset << 32);
    GASNETI_TRACE_EVENT(C, GET_BB);
  } else {
    /* alloc a temp md for the destination region */
    md_h = gasnetc_alloc_tmpmd_withpoll(dest, nbytes, gasnetc_SAFE_EQ->eq_h);
    local_offset = 0;
    GASNETI_TRACE_EVENT(C, GET_TMPMD);
  }

  /* Issue Ptl Get operation */
  GASNETI_TRACE_PRINTF(C,("getmsg: match_bits = 0x%lx, local_off=%ld, remote_off=%ld, nbytes=%ld",(uint64_t)match_bits,(long)local_offset,(long)remote_offset,(long)nbytes));

  GASNETC_PTLSAFE(PtlGetRegion(md_h, local_offset, nbytes, target_id, GASNETC_PTL_RAR_PTE, ac_index, match_bits, remote_offset));

}

/* ------------------------------------------------------------------------------------
 * This function does the actual Portals Put operation for the extended API Get
 * operations.
 * If we have reached the put/get limit, we poll as directed.
 * dest       => Address of destination, must be in remote RAR
 * node       => Which GASNet node to send message to
 * src        => Address of message source
 * nbytes     => Length of message
 * match_bits => Destination MD, may be modified in case of wait_lcc
 * isbulk     => Is this an extended API BULK Put?
 * wait_lcc   => Tells caller to wait for local completion flag
 *               Important: initialized by sender, only modify if must wait for local compl.
 * lcc        => Pointer to weakatomic that we increment before posting Put
 * pollflag   => What type of polling to allow before Put operation is posted.
 * --------------------------------------------------------------------------------- */
void gasnetc_putmsg(void *dest, gasnet_node_t node, void *src, size_t nbytes,
		    ptl_match_bits_t match_bits, int isbulk, int *wait_lcc, gasneti_weakatomic_t *lcc,
		    gasnetc_pollflag_t pollflag)
{
  ptl_size_t local_offset = 0;
  ptl_size_t remote_offset = GASNETC_PTL_OFFSET(node,dest);
  ptl_handle_md_t md_h;
  ptl_process_id_t target_id = gasnetc_procid_map[node].ptl_id;
  ptl_ac_index_t ac_index = GASNETC_PTL_AC_ID;
  ptl_hdr_data_t hdr_data = 0;
  
  gasneti_assert(remote_offset >= 0 && remote_offset < gasneti_seginfo[node].size);

  /* stall here if too many puts/gets in progress */
  while( !gasnetc_alloc_ticket(&gasnetc_send_tickets) ) {
    switch (pollflag) {
    case GASNETC_NO_POLL:
      gasneti_fatalerror("gasnetc_getmsg: msg limit but NO_POLL allowed");
      break;
    case GASNETC_SAFE_POLL:
      gasnetc_portals_poll(pollflag);
      break;
    case GASNETC_FULL_POLL:
      gasneti_AMPoll();
      break;
    }
  }

  /* Determine source MD for Ptl Put */
  if (gasnetc_in_local_rar(src,nbytes)) {
    md_h = gasnetc_RARSRC.md_h;
    local_offset = GASNETC_PTL_OFFSET(gasneti_mynode,src);
    if (! isbulk) *wait_lcc = 1;
    GASNETI_TRACE_EVENT(C, PUT_RAR);
  } else if ( (nbytes <= GASNETC_PUTGET_BOUNCE_SIZE)  &&
	      gasnetc_chunk_alloc_withpoll(&gasnetc_ReqSB,nbytes, &local_offset, 1, GASNETC_SAFE_POLL) ) {
    void* bb;
    md_h = gasnetc_ReqSB.md_h;
    /* get the addr of the start of the chunk */
    bb = ((uint8_t*)gasnetc_ReqSB.start + local_offset);
    /* copy the src data to the bounce buffer */
    memcpy(bb,src,nbytes);
    /* store the local offset in the upper bits of the match bits */
    match_bits |= ((uint64_t)local_offset << 32);
    GASNETI_TRACE_EVENT(C, PUT_BB);
  } else {
    /* alloc a temp md for the source region */
    md_h = gasnetc_alloc_tmpmd_withpoll(src, nbytes, gasnetc_SAFE_EQ->eq_h);
    local_offset = 0;
    if (! isbulk) *wait_lcc = 1;
    GASNETI_TRACE_EVENT(C, PUT_TMPMD);
  }
  if (*wait_lcc) {
    /* increment local completion flag and indicate to event handler to decrement */
    gasneti_weakatomic_increment(lcc, 0);
    match_bits |= GASNETC_PTL_MSG_DOLC;
  }

  GASNETI_TRACE_PRINTF(C,("putmsg: match_bits = 0x%lx, local_off=%ld, remote_off=%ld, bytes=%ld",(uint64_t)match_bits,(long)local_offset,(long)remote_offset,(long)nbytes));

  /* Issue Ptl Put operation */
  GASNETC_PTLSAFE(PtlPutRegion(md_h, local_offset, nbytes, PTL_ACK_REQ, target_id, GASNETC_PTL_RAR_PTE, ac_index, match_bits, remote_offset, hdr_data));

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

