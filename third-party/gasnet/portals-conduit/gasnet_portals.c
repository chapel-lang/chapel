/* MLW: Which of these includes do we really need? */
#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>
#include <gasnet_portals.h>

/* Needed for bootstrap */
#include <catamount/cnos_mpi_os.h>

/* We keep a pool of Request receive buffers on a linked match-list.
 * We maintain the set of handles and other vital data in an array of ReqRB_t objects.
 */
typedef struct {
  size_t nbytes;
  void*  start;
  ptl_handle_md_t  md_h;
} ReqRB_t;
static ReqRB_t *ReqRB_pool;

int    gasnetc_ReqRB_pool_size = 2;
size_t gasnetc_ReqRB_numchunk = 1024;
size_t gasnetc_ReqSB_numchunk = 1024;
size_t gasnetc_RplSB_numchunk = 10;        /* may only need one, for single-threaded implementation */

gasnetc_chunkalloc_t gasnetc_ReqSB = {0,};
gasnetc_chunkalloc_t gasnetc_RplSB = {0,};

ptl_handle_ni_t gasnetc_ni_h;              /* the network interface handle */
ptl_handle_md_t gasnetc_RAR_md_h;          /* Handle to RAR Memory Descriptor */
ptl_handle_md_t gasnetc_RARAM_md_h;        /* Handle to RARAM Memory Descriptor */
ptl_handle_md_t gasnetc_CB_md_h;           /* the catch-basin memory descriptor */
ptl_handle_eq_t gasnetc_EQ_h;              /* Handle to the combined Event Queue */

/* We limit the number of temporary memory descriptors in use at any time.
 * If over the limit, allocator will poll until the number of outstanding tmp mds
 * drops below the limit.
 */
int gasnetc_max_tmpmd = GASNETC_MAX_TMP_MDS;
gasneti_weakatomic_t gasnetc_tmpmd_count;
#if GASNETI_STATS_OR_TRACE
int gasnetc_tmpmd_hwm = 0;
#endif

ptl_process_id_t *gasnetc_procid_map = NULL;

gasnetc_conn_t *gasnetc_conn_state = NULL;

int gasnetc_use_AM_portals = 0;

/* ------------------------------------------------------------------------------------ */
/* The number of GASNet Put or Get operations that can occur before a poll to the
 * network is made.
 */
int gasnete_putget_poll = 6;
gasneti_weakatomic_t gasnete_putget_poll_cnt;

/* The number of outstanding Put/Get operations allowed and the Put/Get limit.
 * Note that we must limit so that we can insure no event queue overflow
 * for operations we initiate.  In general, each Put generates two local events
 * and each Get also generates two (although only should generate one).
 */
gasneti_weakatomic_t gasnete_putget_inflight;
int gasnete_putget_limit = 255;


int gasnetc_max_poll_events = GASNETC_MAX_POLL_EVENTS;

const char* gasnetc_md_name[] = {"RAR_MD","RARAM_MD","REQSB_MD","REQRB_MD","RPLSB_MD","CB_MD","TMP_MD"};

/* =================================================================================
 * This top portion of the file is where file-scope worker routines are located.
 * ================================================================================= */

/* ------------------------------------------------------------------------------------
 * Trivial chunk allocator for bounce buffer and Msg Send buffers.
 * MLW:
 * (1) WARNING WARNING WARNING!
 *     Not a thread-safe freelist implementation!!!
 *     Can easily have one thread pulling something off the list while a portals
 *     event handler, executing in another thread is putting a chunk back on the list.
 *     Must change for multi-threaded implementation.
 * (2) What we really should have is an efficient buddy-buffer implementation so that
 *     small messages dont have to allocate a full KB.  Concern that this will be expensive
 *     and even more expensive in multi-threaded environment.
 * --------------------------------------------------------------------------------- */
static void gasnetc_chunk_init(gasnetc_chunkalloc_t *allocator, const char *name, size_t nchunks)
{
  int i;
  size_t nbytes = nchunks * GASNETC_CHUNKSIZE;
  gasnetc_chunk_t *p;

  allocator->name = name;
  allocator->nbytes = nbytes;
  allocator->numchunks = nchunks;
  allocator->inuse = 0;
  allocator->hwm = 0;
  allocator->freelist = NULL;
  allocator->start = gasneti_malloc(nbytes);
  GASNETI_TRACE_PRINTF(C,("CHUNK_INIT: %s nchunks=%i, nbytes=%i, start=0x%p",name,(int)nchunks,(int)nbytes,allocator->start));
  p = (gasnetc_chunk_t*) allocator->start;
  if (p == NULL) {
    gasneti_fatalerror("failed to alloc %i bytes for chunk allocator %s at %s",(int)nbytes,name,gasneti_current_loc);
  }
  for (i = 0; i < nchunks; i++) {
    p->next = allocator->freelist;
    allocator->freelist = p;
    p++;
  }
}

/* ---------------------------------------------------------------------------------
 * delete the allocator memory.
 * --------------------------------------------------------------------------------- */
static void gasnetc_chunk_delete(gasnetc_chunkalloc_t *allocator)
{
  int nchunk = 0;
  gasnetc_chunk_t *p = allocator->freelist;

#if GASNET_DEBUG
  /* check for allocated chunks */
  while (p != NULL) {
    nchunk++;
    p = p->next;
  }
  GASNETI_TRACE_PRINTF(C,("Chunk_Delete: %d free chunks, expected %d for %s",nchunk,allocator->numchunks,allocator->name));
  gasneti_assert(nchunk == allocator->numchunks);
#endif

  gasneti_free(allocator->start);

  /* keep name and stats for trace_finish call */
}

/* ---------------------------------------------------------------------------------
 * Handle events on the local RARAM Memory Descriptor
 * Used as :
 *   - source of GASNet Puts or dest of Gets when data region happens to lie in RAR.
 *   - source of AM_Long when data region happens to lie in RAR.
 *   - dest of AM_Long data segment.
 * Events:
 *  SEND_END => Put (or Get) local completion
 *       ACK => GASNet Put completed remotely.  Mark operation as complete.
 *              NOTE: AMs dont request ACKs
 * REPLY_END => GASNet Get completed.  Mark operation as complete.
 *   PUT_END => Data portion of AM_Long arrived.  Extract LID and see if header has
 *              also arrived.  If so, call GASNet handler, else insert LID in LID table.
 * --------------------------------------------------------------------------------- */
static void RARAM_event(ptl_event_t *ev)
{
  ptl_size_t offset = ev->offset;
  ptl_match_bits_t   mbits = ev->match_bits;
  gasnete_threadidx_t threadid;
  gasnete_opaddr_t addr;
  uint8_t msg_type, amflag, numarg, ghndlr;
  gasnete_op_t *op;

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("RARAM event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)offset,(uint64_t)mbits,msg_type));

  /* extract the lower bits based on message type */
  if (msg_type & GASNETC_PTL_MSG_AM) {
    gasnetc_get_am_lowbits(mbits, &amflag, &numarg, &ghndlr);
  } else {
    gasnete_get_op_lowbits(mbits, &threadid, &addr);
  }

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  switch (ev->type) {
  case PTL_EVENT_SEND_END:
    /* InSegment Put (from local RAR) */
    if ((msg_type & GASNETC_PTL_MSG_PUT) && (msg_type & GASNETC_PTL_MSG_DOLC)) {
      gasnete_threaddata_t *th = gasnete_threadtable[GASNETE_THREADID(threadid)];
      gasneti_weakatomic_decrement(&(th->local_completion_count), 0);
    }
    break;
  case PTL_EVENT_ACK:
    /* InSegment Put (from local RAR) */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_PUT);
    gasneti_weakatomic_decrement(&gasnete_putget_inflight, 0);
    op = gasnete_opaddr_to_ptr(threadid, addr);
    /* mark the put (isget=0) operation complete */
    gasnete_op_markdone(op, 0 /* !isget */);
    break;

  case PTL_EVENT_REPLY_END:
    /* InSegment Get (to local RAR) */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_GET);
    gasneti_weakatomic_decrement(&gasnete_putget_inflight, 0);
    op = gasnete_opaddr_to_ptr(threadid, addr);
    /* mark the get (isget=1) operation complete */
    gasnete_op_markdone(op, 1);
    break;

  case PTL_EVENT_PUT_END:
    gasneti_fatalerror("Event %s on RARAM not implemented",ptl_event_str[ev->type]);

  default:
    gasneti_fatalerror("Invalid event %s on RARAM",ptl_event_str[ev->type]);
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
    /* Put from TmpMD */
    if ((msg_type & GASNETC_PTL_MSG_PUT) && (msg_type & GASNETC_PTL_MSG_DOLC)) {
      gasnete_threaddata_t *th = gasnete_threadtable[GASNETE_THREADID(threadid)];
      gasneti_weakatomic_decrement(&(th->local_completion_count), 0);
    }
    break;
  case PTL_EVENT_ACK:
    /* Put from TmpMD */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_PUT);
    gasneti_weakatomic_decrement(&gasnete_putget_inflight, 0);
    gasnetc_free_tmpmd(ev->md_handle);
    op = gasnete_opaddr_to_ptr(threadid, addr);
    /* mark the put (isget=0) operation complete */
    gasnete_op_markdone(op, 0 /* !isget */);
    break;

  case PTL_EVENT_REPLY_END:
    /* Get into TmpMD */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_GET);
    gasneti_weakatomic_decrement(&gasnete_putget_inflight, 0);
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
  uint8_t msg_type, amflag, numarg, ghndlr;
  gasnete_op_t *op;
  uint8_t *pdata, *q;
  void *dest;

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("ReqSB event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)offset,(uint64_t)mbits,msg_type));

  /* extract the lower bits based on message type */
  if (msg_type & GASNETC_PTL_MSG_AM) {
    gasnetc_get_am_lowbits(mbits, &amflag, &numarg, &ghndlr);
  } else {
    gasnete_get_op_lowbits(mbits, &threadid, &addr);
  }

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  switch (ev->type) {
  case PTL_EVENT_SEND_END:
    if (msg_type & GASNETC_PTL_MSG_PUT) {
      /* Put bounced through ReqSB, can free chunk now */
      if (msg_type & GASNETC_PTL_MSG_DOLC) {
	gasnete_threaddata_t *th = gasnete_threadtable[GASNETE_THREADID(threadid)];
	gasneti_weakatomic_decrement(&(th->local_completion_count), 0);
      }
      gasnetc_chunk_free(&gasnetc_ReqSB,offset);
    }

    break;
  case PTL_EVENT_ACK:
    /* Put bounced through ReqSB, mark op complete */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_PUT);
    gasneti_weakatomic_decrement(&gasnete_putget_inflight, 0);
    op = gasnete_opaddr_to_ptr(threadid, addr);
    /* mark the put (isget=0) operation complete */
    gasnete_op_markdone(op, 0 /* !isget */);
    break;

  case PTL_EVENT_REPLY_END:
    /* Get bouncing through ReqSB, copy to dest and complete */
    gasneti_assert(msg_type & GASNETC_PTL_MSG_GET);
    gasneti_weakatomic_decrement(&gasnete_putget_inflight, 0);
    pdata = ((uint8_t*)ev->md.start + offset);
    q = pdata - sizeof(void*);
    /* q points to location where real destination address is stored */
    dest = (void*)*(uintptr_t*)q;
    GASNETI_TRACE_PRINTF(C,("EV_handler copying %i bytes from bb 0x%lx to 0x%lx",ev->mlength,(uintptr_t)pdata,(uintptr_t)dest));
    memcpy(dest,pdata,ev->mlength);
    /* free the bounce buffer */
    offset -= sizeof(void*);
    gasnetc_chunk_free(&gasnetc_ReqSB,offset);
    op = gasnete_opaddr_to_ptr(threadid, addr);
    /* mark the get (isget=1) operation complete */
    gasnete_op_markdone(op, 1);
    break;

  default:
    gasneti_fatalerror("Invalid event %s on ReqSB",ptl_event_str[ev->type]);
  }
}

/* ------------------------------------------------------------------------------------
 * Handle events on one of the Reply Send Buffer
 * SEND_END => reply message sent, free the chunk
 * --------------------------------------------------------------------------------- */
static void RplSB_event(ptl_event_t *ev)
{
  ptl_size_t offset = ev->offset;
  ptl_match_bits_t   mbits = ev->match_bits;
  uint8_t msg_type, amflag, numarg, ghndlr;

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("RplSB event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)offset,(uint64_t)mbits,msg_type));

  /* extract the lower bits based on message type */
  if (msg_type & GASNETC_PTL_MSG_AM) {
    gasnetc_get_am_lowbits(mbits, &amflag, &numarg, &ghndlr);
  } else {
    gasneti_fatalerror("Invalid event msg type on RplSB, mbits = 0x%lx",(uint64_t)mbits);
  }

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  switch (ev->type) {
  case PTL_EVENT_SEND_END:

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
  ptl_size_t offset = ev->offset;
  ptl_match_bits_t   mbits = ev->match_bits;
  uint8_t msg_type, amflag, numarg, ghndlr;

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("RARAM event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)offset,(uint64_t)mbits,msg_type));

  /* extract the lower bits based on message type */
  if (msg_type & GASNETC_PTL_MSG_AM) {
    gasnetc_get_am_lowbits(mbits, &amflag, &numarg, &ghndlr);
  } else {
    gasneti_fatalerror("Invalid event msg type on ReqRB, mbits = 0x%lx",(uint64_t)mbits);
  }

  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  switch (ev->type) {
  case PTL_EVENT_PUT_END:

  case PTL_EVENT_UNLINK:

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
  ptl_size_t offset = ev->offset;
  ptl_match_bits_t   mbits = ev->match_bits;
  uint8_t msg_type, amflag, numarg, ghndlr;

  msg_type = GASNETC_GET_MSG_TYPE(mbits);
  GASNETI_TRACE_PRINTF(C,("CB event %s offset = %i, mbits = 0x%lx, msg_type = 0x%x",ptl_event_str[ev->type],(int)offset,(uint64_t)mbits,msg_type));

  /* extract the lower bits based on message type */
  if (msg_type & GASNETC_PTL_MSG_AM) {
    gasnetc_get_am_lowbits(mbits, &amflag, &numarg, &ghndlr);
  } else {
    gasneti_fatalerror("Invalid event msg type on CB, mbits = 0x%lx",(uint64_t)mbits);
  }
  /* we never truncate on this MD */
  gasneti_assert(ev->rlength == ev->mlength);

  switch (ev->type) {
  case PTL_EVENT_PUT_END:

  default:
    gasneti_fatalerror("Invalid event %s on CB",ptl_event_str[ev->type]);
  }
}

/* ---------------------------------------------------------------------------------
 * Construct the memory descriptors that cover the Remote Access Region.
 * There are two Memory Descriptors:
 *   - RAR_MD covers the region but has no event queue.  It is the target of
 *     remote Put and Get operations.
 *   - RARAM_MD also covers the RAR, but this MD is associated with an event
 *     queue.  This MD is used for the data payload Puts of AM_Long operations
 *     as well as the src/dest of Put/Get operations when the src/dest happen
 *     to lie in the local RAR (and we need the events to complete the ops).
 * Both are linked on the GASNETC_RAR_PTE portals table entry list.
 * --------------------------------------------------------------------------------- */
static void RAR_init()
{
  ptl_md_t md;
  ptl_handle_me_t me1_h, me2_h;
  ptl_process_id_t  match_id;
  void* rar_start   = gasneti_seginfo[gasneti_mynode].addr;
  size_t rar_len    = gasneti_seginfo[gasneti_mynode].size;

  match_id.nid = PTL_NID_ANY;
  match_id.pid = PTL_PID_ANY;

  /* Insert a MLE at the head of the list */
  GASNETC_PTLSAFE(PtlMEAttach(gasnetc_ni_h, GASNETC_PTL_RAR_PTE, match_id, GASNETC_PTL_RAR_BITS,
			      GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_BEFORE, &me1_h));

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
  GASNETC_PTLSAFE(PtlMDAttach(me1_h, md, PTL_RETAIN, &gasnetc_RAR_md_h));

  /* We create another md to cover the same RAR region but this one will have
   * an EQ.  We will use it as the source MD of a Put or dest MD of a get
   * in the case when the src/dest happens to lie within the RAR.
   * We could make this free-floating but will need it attached to an MLE
   * in the future, when used as the data target of an AM Long
   */
  md.start = rar_start;
  md.length = rar_len;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  md.options = PTL_MD_OP_PUT | PTL_MD_OP_GET | PTL_MD_MANAGE_REMOTE |
    PTL_MD_EVENT_START_DISABLE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uint64_t)GASNETC_RARAM_MD;
#else
  md.user_ptr = (void*)RARAM_event;
#endif
  md.eq_handle = gasnetc_EQ_h;

  GASNETC_PTLSAFE(PtlMEInsert(me1_h, match_id, GASNETC_PTL_RARAM_BITS,
			      GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_AFTER,
			      &me2_h));
  GASNETC_PTLSAFE(PtlMDAttach(me2_h, md, PTL_RETAIN, &gasnetc_RARAM_md_h));

}

/* ------------------------------------------------------------------------------------
 * Remove the memory descriptors associated with the Remote Access Region, but dont
 * deallocate the memory.
 * --------------------------------------------------------------------------------- */
static void RAR_exit()
{
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_RAR_md_h));
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_RARAM_md_h));
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
  md.eq_handle = gasnetc_EQ_h;

  GASNETC_PTLSAFE(PtlMDBind(gasnetc_ni_h, md, PTL_RETAIN, &gasnetc_RplSB.md_h));

}

/* ------------------------------------------------------------------------------------
 * Clean up the Reply Send Buffer.  Unlink and delete the memory.
 * --------------------------------------------------------------------------------- */
static void RplSB_exit()
{
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_RplSB.md_h));
  gasnetc_chunk_delete(&gasnetc_RplSB);
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
  ReqRB_t *p;
  ptl_handle_me_t me_h;
  ptl_process_id_t  match_id;

  match_id.nid = PTL_NID_ANY;
  match_id.pid = PTL_PID_ANY;

  p = ReqRB_pool = (ReqRB_t*)gasneti_malloc(gasnetc_ReqRB_pool_size*sizeof(ReqRB_t));
  gasneti_assert(ReqRB_pool != NULL);
  for (i = 0; i < gasnetc_ReqRB_pool_size; i++) {
    p->nbytes = nbytes;
    p->start = gasneti_malloc(nbytes);
    gasneti_assert(p->start);
    md.start = p->start;
    md.length = nbytes;
    md.threshold = PTL_MD_THRESH_INF;
    md.max_size = sizeof(GASNETC_CHUNKSIZE);
    md.options = PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE | PTL_MD_MAX_SIZE;
#if GASNETC_USE_EQ_HANDLER
    md.user_ptr = (void*)(uint64_t)GASNETC_REQRB_MD;
#else
    md.user_ptr = (void*)ReqRB_event;
#endif
    md.eq_handle = gasnetc_EQ_h;

    GASNETC_PTLSAFE(PtlMEAttach(gasnetc_ni_h, GASNETC_PTL_AM_PTE, match_id, GASNETC_PTL_REQRB_BITS, GASNETC_PTL_IGNORE_BITS,PTL_UNLINK, PTL_INS_AFTER, &me_h));
    GASNETC_PTLSAFE(PtlMDAttach(me_h, md, PTL_UNLINK, &(p->md_h)));

    p++;
  }

  /* Now add the Catch-Basin MD */
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
  md.eq_handle = gasnetc_EQ_h;
  GASNETC_PTLSAFE(PtlMEAttach(gasnetc_ni_h, GASNETC_PTL_AM_PTE, match_id, GASNETC_PTL_REQRB_BITS, GASNETC_PTL_IGNORE_BITS, PTL_UNLINK, PTL_INS_AFTER, &me_h));
  GASNETC_PTLSAFE(PtlMDAttach(me_h, md, PTL_RETAIN, &gasnetc_CB_md_h));

}

/* ---------------------------------------------------------------------------------
 * Cleanup Request Receive Buffer resources.
 * --------------------------------------------------------------------------------- */
static void ReqRB_exit()
{
  int i;

  for (i = 0; i < gasnetc_ReqRB_pool_size; i++) {
    /* This should also unlink the associated MEs */
    GASNETC_PTLSAFE(PtlMDUnlink(ReqRB_pool[i].md_h));
    gasneti_free(ReqRB_pool[i].start);
  }
  gasneti_free(ReqRB_pool);
  /* no data buffer for the CB */
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_CB_md_h));
}

/* ---------------------------------------------------------------------------------
 * Allocate the Request Send Buffer (also used as a Bounce Buffer) and
 * Construct a Memory Descriptor for it.
 * Note:  Current Implementation is for free-floating MD.  Must put this on
 *        Match-list for Catch-Basin algorithm to work.  Put it at end of
 *        GASNETC_AM_PTE table entry since this is an unlikely situation.
 * --------------------------------------------------------------------------------- */
static void ReqSB_init()
{
  ptl_md_t md;
  gasnetc_chunk_init(&gasnetc_ReqSB, "ReqSB", gasnetc_ReqSB_numchunk);

  /* construct a memory descriptor for the bounce buffer */
  md.start = gasnetc_ReqSB.start;
  md.length = gasnetc_ReqSB.nbytes;
  md.threshold = PTL_MD_THRESH_INF;
  md.max_size = 0;
  /* free-floating md that is src of put or dest of get.  Disable start ops */
  md.options = PTL_MD_EVENT_START_DISABLE;
#if GASNETC_USE_EQ_HANDLER
  md.user_ptr = (void*)(uint64_t)GASNETC_REQSB_MD;
#else
  md.user_ptr = (void*)ReqSB_event;
#endif
  md.eq_handle = gasnetc_EQ_h;

  /* register the md and get handle */
  GASNETC_PTLSAFE(PtlMDBind(gasnetc_ni_h, md, PTL_RETAIN, &gasnetc_ReqSB.md_h));
}

/* ---------------------------------------------------------------------------------
 * Cleanup Request Send Buffer Resources
 * --------------------------------------------------------------------------------- */
static void ReqSB_exit()
{
  /* unlink the MD */
  GASNETC_PTLSAFE(PtlMDUnlink(gasnetc_ReqSB.md_h));

  /* free the memory */
  gasnetc_chunk_delete(&gasnetc_ReqSB);
}

/* =================================================================================
 * This lower portion of the file is where exported functions are located.
 * These are exported to both the Core and Extended API implementations.
 * ================================================================================= */

/* ---------------------------------------------------------------------------------
 * get a chunk from the allocator.
 * returns 1=TRUE on success, 0=FAIL if not able to satisfly request.
 * Will poll network a bounded number of times if necessary to alloc chunk.
 * MLW: may want input flag to control this.
 * --------------------------------------------------------------------------------- */
extern int gasnetc_chunk_alloc(gasnetc_chunkalloc_t *allocator, size_t nbytes, ptl_size_t *offset)
{
    gasnetc_chunk_t *p;
    int poll_max = 2;
    int cnt = 0;
    
    if (nbytes > GASNETC_CHUNKSIZE) return 0;
    while ((allocator->freelist == NULL) && cnt < poll_max) {
      /* poll npoll times, to see if slot frees up */
      GASNETI_SAFE(gasneti_AMPoll());
      cnt++;
    }
    if (allocator->freelist == NULL) return 0;
    p = allocator->freelist;
    allocator->freelist = p->next;
    *offset = ((uint8_t*)p - (uint8_t*)allocator->start);
#if GASNETI_STATS_OR_TRACE
    allocator->inuse++;
    if (allocator->inuse > allocator->hwm) allocator->hwm = allocator->inuse;
    GASNETI_TRACE_PRINTF(C,("CHUNK_ALLOC: name %s, inuse = %d, hwm = %d, offset=%lu",allocator->name,allocator->inuse,allocator->hwm,(unsigned long)*offset));
    GASNETI_TRACE_EVENT(C, CHUNK_ALLOC);
#endif

    return 1;
}

/* ---------------------------------------------------------------------------------
 * release a chunk back to the allocator.
 * MLW: Programmer error to return chunk to wrong allocator since just giving
 *      the offset.  We could hand in full address and compute offset here for
 *      better error checking, but would have to compute full address from offset
 *      at point of call, and unwind that here.  
 * --------------------------------------------------------------------------------- */
extern void gasnetc_chunk_free(gasnetc_chunkalloc_t *allocator, ptl_size_t offset)
{
    gasnetc_chunk_t *p = (gasnetc_chunk_t*)((uint8_t*)allocator->start + offset);
    p->next = allocator->freelist;
    allocator->freelist = p;
#if GASNETI_STATS_OR_TRACE
    allocator->inuse--;
    GASNETI_TRACE_PRINTF(C,("CHUNK_FREE: name %s, inuse = %d, hwm = %d, offset=%lu",allocator->name,allocator->inuse,allocator->hwm,(unsigned long)offset));
    GASNETI_TRACE_EVENT(C, CHUNK_FREE);
#endif
}

/* ---------------------------------------------------------------------------------
 * Allocate a temp md to be used as the source of a Put or destination
 * of a Get operation.  MD to be free floating, not target of remote op.
 * MLW: Can make more general by specifying OPTIONS as arg.  May need this
 *      for CB recovery.
 * --------------------------------------------------------------------------------- */
extern ptl_handle_md_t gasnetc_alloc_tmpmd(void* dest, size_t nbytes, ptl_handle_eq_t eq_h)
{
  ptl_md_t md;
  ptl_handle_md_t md_h;

  /* Want to limit the number of tmp MDs in operation at once.
   * Poll until number of outstanding TMP MDs is less than limit.
   */
  GASNETI_TRACE_PRINTF(C,("Alloc_Tmpmd: num TmpMD outstanding = %d",gasneti_weakatomic_read(&gasnetc_tmpmd_count,0)));
  gasneti_pollwhile( (gasneti_weakatomic_read(&gasnetc_tmpmd_count,0) >= gasnetc_max_tmpmd) );

  gasneti_weakatomic_increment(&gasnetc_tmpmd_count,0);
  md.start = dest;
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

  GASNETC_PTLSAFE(PtlMDBind(gasnetc_ni_h, md, PTL_RETAIN, &md_h));

#if GASNETI_STATS_OR_TRACE
      {
	int inuse = (int)gasneti_weakatomic_read(&gasnetc_tmpmd_count,0);
	if (gasnetc_tmpmd_hwm < inuse) gasnetc_tmpmd_hwm = inuse;
	GASNETI_TRACE_PRINTF(C,("ALLOC TMPMD at 0x%p, len=%d, inuse=%d, hwm=%d",dest,(int)nbytes,inuse,gasnetc_tmpmd_hwm));
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
  gasneti_weakatomic_decrement(&gasnetc_tmpmd_count,0);
  GASNETC_PTLSAFE(PtlMDUnlink(md_h));
#if GASNETI_STATS_OR_TRACE
      {
	int inuse = (int)gasneti_weakatomic_read(&gasnetc_tmpmd_count,0);
	GASNETI_TRACE_PRINTF(C,("FREE TMPMD"));
	GASNETI_TRACE_EVENT(C, TMPMD_FREE);
      }
#endif
}

/* ---------------------------------------------------------------------------------
 * Initialize all the Portals resources for GASNet:
 *   - Read portals-related env vars to adjust buffer sizes and polling counts
 *   - Recover the Portals network-interface handle.  This was initialized in the
 *     MPI code but calling PtlNIInit a second time just returns the handle.
 *   - Get the Portals proc_id map so we know how to talk to all other nodes.
 *   - Allocate the event queues:  Only using one for now
 *   - Allocate the buffers for bounce, send/recv regions
 *   - Construct match-list entries and memory descriptors for bounce, send/recv space
 *     and RAR (Segment already allocated and initialized).
 * --------------------------------------------------------------------------------- */
extern void gasnetc_portals_init(void)
{
  /* Set up my RAR MDs */
  ptl_interface_t   ptl_iface;
  int               use_bridge = PTL_BRIDGE_QK;
  int               use_nal = PTL_IFACE_SS;
  ptl_size_t        eq_len;
  int               rc;
  cnos_nidpid_map_t *cnos_map;
  int               my_rank, my_size;
  ptl_process_id_t  my_id;
  int               i, num_chunk;

  /* read Portals specific env vars */
  gasnetc_ReqRB_pool_size = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_POOLSZ",
   			         (int64_t)gasnetc_ReqRB_pool_size,0);
  gasnetc_ReqRB_numchunk = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_RB_CHUNKS",
				(int64_t)gasnetc_ReqRB_numchunk,0);
  gasnetc_ReqSB_numchunk = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_SB_CHUNKS",
				(int64_t)gasnetc_ReqSB_numchunk,0);
  gasnetc_RplSB_numchunk = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_RPL_CHUNKS",
				(int64_t)gasnetc_RplSB_numchunk,0);
  gasnetc_max_tmpmd = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_NUM_TMPMD",
			   (int64_t)GASNETC_MAX_TMP_MDS,0);
  gasnetc_max_poll_events = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_MAX_POLL_EVENTS",
				 (int64_t)GASNETC_MAX_POLL_EVENTS,0);
  gasnete_putget_poll = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_PUTGET_POLL",
				(int64_t)gasnete_putget_poll,0);
  gasnete_putget_limit = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_PUTGET_LIMIT",
				(int64_t)gasnete_putget_limit,0);
  gasnetc_use_AM_portals = (int)gasneti_getenv_int_withdefault("GASNET_PORTAL_DO_AM",(int64_t)gasnetc_use_AM_portals,0);
				
  GASNETI_TRACE_PRINTF(C,("Portals_Init: ReqRB_Pool_size = %d",gasnetc_ReqRB_pool_size));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: ReqRB_numchunk  = %d",(int)gasnetc_ReqRB_numchunk));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: ReqSB_numchunk  = %d",(int)gasnetc_ReqSB_numchunk));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: RplSB_numchunk  = %d",(int)gasnetc_RplSB_numchunk));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: max_tmpmd       = %d",gasnetc_max_tmpmd));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: max_poll_events = %d",gasnetc_max_poll_events));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: putget_per_poll = %d",gasnete_putget_poll));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: putget_limit    = %d",gasnete_putget_limit));
  GASNETI_TRACE_PRINTF(C,("Portals_Init: use_AM_portals  = %d",gasnetc_use_AM_portals));

  /* Init the temp md counter to zero */
  gasneti_weakatomic_set(&gasnetc_tmpmd_count, 0, 0);

  /* keep a counter of number of puts/gets since last poll */
  gasneti_weakatomic_set(&gasnete_putget_poll_cnt, 0, 0);
  gasneti_weakatomic_set(&gasnete_putget_inflight, 0, 0);

  /* construct the interface */
  /* Hmm, how was it constructed for MPI? Will I get different ni? */
  ptl_iface = IFACE_FROM_BRIDGE_AND_NALID(use_bridge,use_nal);

  /* Get the network handle */
  rc = PtlNIInit(ptl_iface, PTL_PID_ANY, NULL, NULL, &gasnetc_ni_h);
  switch (rc) {
  case PTL_OK:
  case PTL_IFACE_DUP:
    break;
  default:
    gasneti_fatalerror("GASNet Portals failed on call to PtlNiInit:\n"
		       "  iface_type = %x error=%s (%i)\n"
		       "  at: %s\n",ptl_iface,ptl_err_str[rc],rc,gasneti_current_loc);
  }

  /* Get my process info, does it match? */
  GASNETC_PTLSAFE(PtlGetId(gasnetc_ni_h,&my_id));
  my_rank = cnos_get_rank();
  my_size = cnos_get_size();
  gasneti_assert_always(my_rank == gasneti_mynode);
  gasneti_assert_always(my_size == gasneti_nodes);
  
  /* get process to portals address mapping */
  if (my_size != cnos_get_nidpid_map(&cnos_map)) {
    gasneti_fatalerror("cnos_get_nidpid_map size != %d",my_size);
  }
  gasneti_assert_always(cnos_map[my_rank].nid == my_id.nid);
  gasneti_assert_always(cnos_map[my_rank].pid == my_id.pid);
  gasnetc_procid_map = (ptl_process_id_t*)gasneti_malloc(my_size * sizeof(ptl_process_id_t));
  for (i = 0; i < my_size; i++) {
    gasnetc_procid_map[i].nid = cnos_map[i].nid;
    gasnetc_procid_map[i].pid = cnos_map[i].pid;
  }

  /* Allocate and init the connection state array */
  gasnetc_conn_state = (gasnetc_conn_t*)gasneti_calloc(gasneti_nodes, sizeof(gasnetc_conn_t));

  /* On first cut, lets just create a single EQ for all of the MDs that will
   * generate events.  This includes:
   * BounceBuffer:  number = 2*num_chunks;
   * RARAM:         unknown ... scale with num procs? what scaling factor?
   * TMPMD:         2*max number of tmpmds
   */
  if (gasnetc_use_AM_portals) {
    num_chunk = gasnetc_ReqSB_numchunk + gasnetc_RplSB_numchunk 
      + gasnetc_ReqRB_pool_size*gasnetc_ReqRB_numchunk;
  } else {
    num_chunk = gasnetc_ReqSB_numchunk;
  }
  eq_len = 2*(num_chunk + gasnetc_max_tmpmd);
  /* two events per outstanding put or get */
  if (gasnete_putget_limit) {
    eq_len += 2*gasnete_putget_limit;
  } else {
    /* Allow unlimited number of Put/Get ops in flight.
     * Living dangerously, may not poll in time to prevent EQ overflow */
    eq_len += 50*gasneti_nodes;
  }
  /* for good measure */
  eq_len += 100;

  GASNETI_TRACE_PRINTF(C,("Constructing EQ with %d entries",eq_len));
#if 0
  if (gasneti_mynode == 0) {
    printf("MAX_POLL_EVENTS = %i\n",gasnetc_max_poll_events);
    printf("PUTGET_MAXPOLL  = %i\n",gasnete_putget_poll);
    printf("PUTGET_LIMIT    = %i\n",gasnete_putget_limit);
    printf("TOTAL EVENTS    = %i\n",eq_len);
  }
#endif

  GASNETC_PTLSAFE(PtlEQAlloc(gasnetc_ni_h, eq_len, GASNETC_EQ_HANDLER, &gasnetc_EQ_h));

  RAR_init();
  ReqSB_init();

  if (gasnetc_use_AM_portals) {
    ReqRB_init();
    RplSB_init();
  }

#ifndef GASNETC_USE_EQ_HANDLER
  /* Enable the progress function */
  GASNETI_TRACE_PRINTF(C,("Enabling Portals polling function (No EQ Handler)"));
  GASNETI_PROGRESSFNS_ENABLE(gasnete_pf_portals_poll,BOOLEAN);
#endif
}

/* ---------------------------------------------------------------------------------
 * Release Portals resources
 *   - The proc_id map
 *   - Remove MDs and match-list entries
 *   - Free the buffers used for bounce, send/recv
 * --------------------------------------------------------------------------------- */
extern void gasnetc_portals_exit()
{

  if (gasnetc_use_AM_portals) {
    RplSB_exit();
    ReqRB_exit();
  }
  ReqSB_exit();
  RAR_exit();

  /* remove the event queues */
  GASNETC_PTLSAFE(PtlEQFree(gasnetc_EQ_h));

  /* free the proc id map */
  gasneti_free(gasnetc_procid_map);

  /* free the AM connection state array */
  gasneti_free(gasnetc_conn_state);

}

/* ------------------------------------------------------------------------------------
 * --------------------------------------------------------------------------------- */
/* Portals polling function.  Process the event queue.
 * This gets called by gasneti_AMPoll() after checking for incoming MPI messages 
 * In the current implementation, we only have one event queue,
 * so just check it and run appropriate handler
 */

extern void gasnetc_portals_poll(void)
{
  ptl_event_t ev;
  int rc;
  int processed = 0;
  int finished = 0;

  /* Reset the putget counter, since we are about to poll... */
  gasneti_weakatomic_set(&gasnete_putget_poll_cnt, 0, 0);

  /* Note that PtlEQGet is just a user-space call and will see if anything is
   * on the list without diving into the kernel to see if any unprocessed events
   * are waiting.  Try it first.  If multiple exist, run them back to back since they
   * release resources.
   */
  while (! finished) {
    rc = PtlEQGet( gasnetc_EQ_h, &ev);
    switch (rc) {
    case PTL_OK:
      GASNETI_TRACE_PRINTF(C,("Q Handler: Got event %s from PtlEQGet",ptl_event_str[ev.type]));
      GASNETC_CALL_EQ_HANDLER(ev); 
      processed++;
      if (gasnetc_max_poll_events && (processed > gasnetc_max_poll_events)) finished = 1;
      break;
    case PTL_EQ_EMPTY:
      finished = 1;
      break;
    default:
      gasneti_fatalerror("GASNet Portals Error in PtlEQGet: %s (%i)\n at %s\n",
			 ptl_err_str[rc],rc,gasneti_current_loc);
      break;
    }
  }

  if (processed == 0) {
    /* No easy pickings... try polling, which may enter the kernel */
    int which = 0;
    int timeout = 0;       /* number of usec to wait */
    rc = PtlEQPoll(&gasnetc_EQ_h,1,timeout,&ev,&which);
    switch (rc) {
    case PTL_OK:
      GASNETI_TRACE_PRINTF(C,("Q Handler: Got event %s from PtlEQPoll",ptl_event_str[ev.type]));
      GASNETC_CALL_EQ_HANDLER(ev);
      processed++;
      break;
    case PTL_EQ_EMPTY:
      break;
    default:
      gasneti_fatalerror("GASNet Portals Error in PtlEQPoll: %s (%i)\n at %s\n",
			 ptl_err_str[rc],rc,gasneti_current_loc);
      break;
    }
  }
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
