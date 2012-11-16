#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_handler.h>
#include <gasnet_gemini.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>

#define GASNETC_NETWORKDEPTH_DEFAULT 12
static unsigned int gasnetc_mb_maxcredit;

int gasnetc_poll_burst = 10;
static gasnetc_queue_t smsg_work_queue;

static double shutdown_max;
static uint32_t sys_exit_rcvd;


typedef struct peer_struct {
  gasnetc_queue_item_t qi;
  uint32_t rank;
  uint32_t nic_address;
  void *heap_base;
  gni_mem_handle_t heap_mem_handle;
  gni_smsg_attr_t smsg_attr;
  gasneti_weakatomic_t am_credit;
  gasneti_weakatomic_t fma_credit;
  gasneti_mutex_t lock;
} peer_struct_t;

typedef struct peer_segment_struct {
  void *segment_base;
  uint64_t segment_size;
  gni_mem_handle_t segment_mem_handle;
} peer_segment_struct_t;

static peer_struct_t mypeerdata;
static peer_struct_t *peer_data;
static peer_segment_struct_t mypeersegmentdata;
static peer_segment_struct_t *peer_segment_data;

uint32_t *gasnetc_UGNI_AllAddr;

static gni_cdm_handle_t cdm_handle;
static gni_nic_handle_t nic_handle;
static gni_ep_handle_t *bound_ep_handles;
static gni_cq_handle_t bound_cq_handle;
static gni_cq_handle_t smsg_cq_handle;

static void *smsg_mmap_ptr;
static size_t smsg_mmap_bytes;

static const char *gni_return_string(gni_return_t status)
{
  if (status == GNI_RC_SUCCESS) return ("GNI_RC_SUCCESS");
  if (status == GNI_RC_NOT_DONE) return ("GNI_RC_NOT_DONE");
  if (status == GNI_RC_INVALID_PARAM) return ("GNI_RC_INVALID_PARAM");
  if (status == GNI_RC_ERROR_RESOURCE) return ("GNI_RC_ERROR_RESOURCE");
  if (status == GNI_RC_TIMEOUT) return ("GNI_RC_TIMEOU");
  if (status == GNI_RC_PERMISSION_ERROR) return ("GNI_RC_PERMISSION_ERROR");
  if (status == GNI_RC_DESCRIPTOR_ERROR) return ("GNI_RC_DESCRIPTOR ERROR");
  if (status == GNI_RC_ALIGNMENT_ERROR) return ("GNI_RC_ALIGNMENT_ERROR");
  if (status == GNI_RC_INVALID_STATE) return ("GNI_RC_INVALID_STATE");
  if (status == GNI_RC_NO_MATCH) return ("GNI_RC_NO_MATCH");
  if (status == GNI_RC_SIZE_ERROR) return ("GNI_RC_SIZE_ERROR");
  if (status == GNI_RC_TRANSACTION_ERROR) return ("GNI_RC_TRANSACTION_ERROR");
  if (status == GNI_RC_ILLEGAL_OP) return ("GNI_RC_ILLEGAL_OP");
  if (status == GNI_RC_ERROR_NOMEM) return ("GNI_RC_ERROR_NOMEM");
  return("unknown");
}


const char *gasnetc_type_string(int type)
{
  if (type == GC_CMD_NULL) return("GC_CMD_AM_NULL");
  if (type == GC_CMD_AM_SHORT) return ("GC_CMD_AM_SHORT");
  if (type == GC_CMD_AM_LONG) return ("GC_CMD_AM_LONG");
  if (type == GC_CMD_AM_MEDIUM) return ("GC_CMD_AM_MEDIUM");
  if (type == GC_CMD_AM_SHORT_REPLY) return ("GC_CMD_AM_SHORT_REPLY");
  if (type == GC_CMD_AM_LONG_REPLY) return ("GC_CMD_AM_LONG_REPLY");
  if (type == GC_CMD_AM_MEDIUM_REPLY) return ("GC_CMD_AM_MEDIUM_REPLY");
  if (type == GC_CMD_AM_NOP_REPLY) return ("GC_CMD_AM_NOP_REPLY");
  if (type == GC_CMD_SYS_SHUTDOWN_REQUEST) return ("GC_CMD_SYS_SHUTDOWN_REQUEST");
  return("unknown");
}


/* called after segment init. See gasneti_seginfo */
/* allgather the memory handles for the segments */
/* create endpoints */
void gasnetc_init_segment(void *segment_start, size_t segment_size)
{
  gni_return_t status;
  /* Map the shared segment */

  mypeersegmentdata.segment_base = segment_start;
  mypeersegmentdata.segment_size = segment_size;
  {
    int count = 0;
    for (;;) {
      status = GNI_MemRegister(nic_handle, (uint64_t) segment_start, 
			       (uint64_t) segment_size, NULL,
			       GNI_MEM_STRICT_PI_ORDERING | GNI_MEM_PI_FLUSH 
			       | GNI_MEM_READWRITE, -1, 
			       &mypeersegmentdata.segment_mem_handle);
      if (status == GNI_RC_SUCCESS) break;
      if (status == GNI_RC_ERROR_RESOURCE) {
	fprintf(stderr, "MemRegister segment fault %d at  %p %lx, code %s\n", 
		count, segment_start, segment_size, gni_return_string(status));
	count += 1;
	if (count >= 10) break;
      } else {
	break;
      }
    }
  }
			       
  assert (status == GNI_RC_SUCCESS);
#if GASNETC_DEBUG
  gasnetc_GNIT_Log("segment mapped %p, %p", segment_start, (void *) segment_size);
#endif
  peer_segment_data = gasneti_malloc(gasneti_nodes * sizeof(peer_segment_struct_t));
  assert(peer_segment_data);
  
  gasnetc_GNIT_Allgather(&mypeersegmentdata, sizeof(peer_segment_struct_t), peer_segment_data);
}

uintptr_t gasnetc_init_messaging(void)
{
  gni_return_t status;
  uint32_t remote_addr;
  uint32_t i;
  unsigned int bytes_per_mbox;
  unsigned int bytes_needed;
  int modes = 0;

#if GASNETC_DEBUG
  gasnetc_GNIT_Log("entering");
#endif

  GASNETC_INITLOCK_GNI();

  gasnetc_queue_init(&smsg_work_queue);

  status = GNI_CdmCreate(gasneti_mynode, gasnetc_GNIT_Ptag(), gasnetc_GNIT_Cookie(), 
			 modes,
			 &cdm_handle);
  gasneti_assert_always (status == GNI_RC_SUCCESS);

  status = GNI_CdmAttach(cdm_handle, gasnetc_GNIT_Device_Id(),
			 &mypeerdata.nic_address,
			 &nic_handle);

  gasneti_assert_always (status == GNI_RC_SUCCESS);

#if GASNETC_DEBUG
  gasnetc_GNIT_Log("cdmattach");
#endif

  status = GNI_CqCreate(nic_handle, 1024, 0, GNI_CQ_NOBLOCK, NULL, NULL, &bound_cq_handle);

  gasneti_assert_always (status == GNI_RC_SUCCESS);


  bound_ep_handles = (gni_ep_handle_t *)gasneti_malloc(gasneti_nodes * sizeof(gni_ep_handle_t));
  gasneti_assert_always(bound_ep_handles != NULL);
  gasnetc_UGNI_AllAddr = (uint32_t *)gasnetc_gather_nic_addresses();

  for (i=0; i<gasneti_nodes; i++) {
    status = GNI_EpCreate(nic_handle, bound_cq_handle, &bound_ep_handles[i]);
    gasneti_assert_always (status == GNI_RC_SUCCESS);
    remote_addr = gasnetc_UGNI_AllAddr[i];
    status = GNI_EpBind(bound_ep_handles[i], remote_addr, i);
    gasneti_assert_always (status == GNI_RC_SUCCESS);
#if GASNETC_DEBUG
    gasnetc_GNIT_Log("ep bound to %d, addr %d", i, remote_addr);
#endif
  }


  /* Initialize the short message system */

  { int tmp = gasneti_getenv_int_withdefault("GASNET_NETWORKDEPTH", GASNETC_NETWORKDEPTH_DEFAULT, 0);
    gasnetc_mb_maxcredit = 2 * MAX(1,tmp); /* silently "fix" zero or negative values */
  }

  /*
   * allocate a CQ in which to receive message notifications
   */
  /* TODO: is "* 2" still correct given gasnetc_mb_maxcredit has been halved since the original code? */
  status = GNI_CqCreate(nic_handle,gasneti_nodes * gasnetc_mb_maxcredit * 2,0,GNI_CQ_NOBLOCK,NULL,NULL,&smsg_cq_handle);
  if (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort("GNI_CqCreate returned error %s\n", gni_return_string(status));
  }
  
  /*
   * Set up an mmap region to contain all of my mailboxes.
   * The GNI_SmsgBufferSizeNeeded is used to determine how
   * much memory is needed for each mailbox.
   */

  mypeerdata.smsg_attr.msg_type = GNI_SMSG_TYPE_MBOX;
  mypeerdata.smsg_attr.mbox_maxcredit = gasnetc_mb_maxcredit;
  mypeerdata.smsg_attr.msg_maxsize = GASNETC_MSG_MAXSIZE;
#if GASNETC_DEBUG
  fprintf(stderr,"r %d maxcredit %d msg_maxsize %d\n", gasneti_mynode, gasnetc_mb_maxcredit, (int)GASNETC_MSG_MAXSIZE);
#endif

  status = GNI_SmsgBufferSizeNeeded(&mypeerdata.smsg_attr,&bytes_per_mbox);
  if (status != GNI_RC_SUCCESS){
    gasnetc_GNIT_Abort("GNI_GetSmsgBufferSize returned error %s\n",gni_return_string(status));
  }
#if GASNETC_DEBUG
  fprintf(stderr,"r %d GetSmsgBufferSize says %d bytes for each mailbox\n", gasneti_mynode, bytes_per_mbox);
#endif
  bytes_per_mbox = GASNETI_ALIGNUP(bytes_per_mbox, GASNETC_CACHELINE_SIZE);
  /* test */
  bytes_per_mbox += mypeerdata.smsg_attr.mbox_maxcredit 
    * mypeerdata.smsg_attr.msg_maxsize;
  bytes_needed = gasneti_nodes * bytes_per_mbox;
  
#if GASNETC_DEBUG
  fprintf(stderr,"Allocating %d bytes for each mailbox\n",bytes_per_mbox);
  fprintf(stderr,"max medium %d, sizeof medium %d\n",(int)gasnet_AMMaxMedium(),
	  (int)sizeof(gasnetc_am_medium_packet_t));
#endif
  smsg_mmap_ptr = gasneti_huge_mmap(NULL, bytes_needed);
  smsg_mmap_bytes = bytes_needed;
  if (smsg_mmap_ptr == (char *)MAP_FAILED) {
    gasnetc_GNIT_Abort("hugepage mmap failed: ");
  }
  
#if GASNETC_DEBUG
  gasnetc_GNIT_Log("mmap %p", smsg_mmap_ptr);
#endif
  {
    int count = 0;
    for (;;) {
      status = GNI_MemRegister(nic_handle, 
			       (unsigned long)smsg_mmap_ptr, 
			       bytes_needed,
			       smsg_cq_handle,
			       GNI_MEM_STRICT_PI_ORDERING | GNI_MEM_PI_FLUSH 
			       |GNI_MEM_READWRITE,
			       -1,
			       &mypeerdata.smsg_attr.mem_hndl);
      if (status == GNI_RC_SUCCESS) break;
      if (status == GNI_RC_ERROR_RESOURCE) {
	fprintf(stderr, "MemRegister smsg fault %d at  %p %x, code %s\n", 
		count, smsg_mmap_ptr, bytes_needed, gni_return_string(status));
	count += 1;
	if (count >= 10) break;
      } else {
	break;
      }
    }
  }
  if (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort("GNI_MemRegister returned error %s\n",gni_return_string(status));
  }
  
  mypeerdata.smsg_attr.msg_type = GNI_SMSG_TYPE_MBOX;
  mypeerdata.smsg_attr.msg_buffer = smsg_mmap_ptr;
  mypeerdata.smsg_attr.buff_size = bytes_per_mbox;
  mypeerdata.smsg_attr.mbox_maxcredit = gasnetc_mb_maxcredit;
  mypeerdata.smsg_attr.msg_maxsize = GASNETC_MSG_MAXSIZE;
  

#if GASNETC_DEBUG
  gasnetc_GNIT_Log("segment registered");
#endif

    /* exchange peer data */
  mypeerdata.rank = gasneti_mynode;
  mypeerdata.nic_address = gasnetc_UGNI_AllAddr[gasneti_mynode];
  
  peer_data = gasneti_malloc(gasneti_nodes * sizeof(peer_struct_t));
  gasneti_assert_always(peer_data);
  
  gasnetc_GNIT_Allgather(&mypeerdata, sizeof(peer_struct_t), peer_data);
  
  /* At this point, peer_data has information for everyone */
  /* We need to patch up the smsg data, fixing the remote start addresses */
  for (i = 0; i < gasneti_nodes; i += 1) {
    /* each am takes 2 credits (req + reply) */
    gasneti_mutex_init(&peer_data[i].lock);
    gasneti_weakatomic_set(&peer_data[i].am_credit, gasnetc_mb_maxcredit / 2, 0);
    gasneti_weakatomic_set(&peer_data[i].fma_credit, 1, 0);
    gasnetc_queue_item_init(&peer_data[i].qi);
    peer_data[i].smsg_attr.mbox_offset = bytes_per_mbox * gasneti_mynode;
    mypeerdata.smsg_attr.mbox_offset = bytes_per_mbox * i;
    status = GNI_SmsgInit(bound_ep_handles[i],
			  &mypeerdata.smsg_attr,
			  &peer_data[i].smsg_attr);
    if (status != GNI_RC_SUCCESS) {
      gasnetc_GNIT_Abort("GNI_SmsgInit returned error %s\n", gni_return_string(status));
    }
  }

  /* Now make sure everyone is ready */
#if GASNETC_DEBUG
  gasnetc_GNIT_Log("finishing");
#endif
  /* set the number of seconds we poll until forceful shutdown.
   * May be over-ridden by env-vars.
   */
  gasnetc_shutdown_seconds = gasneti_get_exittimeout(shutdown_max, 3., 0.125, 0.);

  gasnetc_fma_rdma_cutover = 
    gasneti_getenv_int_withdefault("GASNETC_GNI_FMA_RDMA_CUTOVER",
				   GASNETC_GNI_FMA_RDMA_CUTOVER_DEFAULT,1);
  if (gasnetc_fma_rdma_cutover > GASNETC_GNI_FMA_RDMA_CUTOVER_MAX)
    gasnetc_fma_rdma_cutover = GASNETC_GNI_FMA_RDMA_CUTOVER_MAX;
  if (gasnetc_fma_rdma_cutover > GASNETC_GNI_FMA_RDMA_CUTOVER_MAX)
    gasnetc_fma_rdma_cutover = GASNETC_GNI_FMA_RDMA_CUTOVER_MAX;

  gasnetc_GNIT_Barrier();

  return bytes_needed;
}

void gasnetc_shutdown(void)
{
  int i;
  int tries;
  int left;
  gni_return_t status;
  /* seize gni lock and hold it  */
  GASNETC_LOCK_GNI();
  /* Do other threads need to be killed off here?
     release resources in the reverse order of acquisition
   */

  /* for each rank */
  tries = 0;
  left = gasneti_nodes;
  while (left > 0) {
    tries += 1;
    for (i = 0; i < gasneti_nodes; i += 1) {
      if (bound_ep_handles[i] != NULL) {
	status = GNI_EpUnbind(bound_ep_handles[i]);
	if (status != GNI_RC_SUCCESS) {
	  fprintf(stderr, "node %d shutdown epunbind %d try %d  got %s\n",
		  gasneti_mynode, i, tries, gni_return_string(status));
	} 
	status = GNI_EpDestroy(bound_ep_handles[i]);
	if (status != GNI_RC_SUCCESS) {
	  fprintf(stderr, "node %d shutdown epdestroy %d try %d  got %s\n",
		  gasneti_mynode, i, tries, gni_return_string(status));
	} else {
	  bound_ep_handles[i] = NULL;
	  left -= 1;
	}
      }
    }
    if (tries >= 10) break;
  }
  if (left > 0) {
    fprintf(stderr, "node %d %d endpoints left after 10 tries\n", 
	    gasneti_mynode, left);
  }

  if (gasneti_attach_done) {
    status = GNI_MemDeregister(nic_handle,
			     &mypeersegmentdata.segment_mem_handle);
    gasneti_assert_always (status == GNI_RC_SUCCESS);
  }

  gasneti_huge_munmap(smsg_mmap_ptr, smsg_mmap_bytes);

  status = GNI_MemDeregister(nic_handle,
			     &mypeerdata.smsg_attr.mem_hndl);
  gasneti_assert_always (status == GNI_RC_SUCCESS);


  status = GNI_CqDestroy(smsg_cq_handle);
  gasneti_assert_always (status == GNI_RC_SUCCESS);

  status = GNI_CqDestroy(bound_cq_handle);
  gasneti_assert_always (status == GNI_RC_SUCCESS);


  status = GNI_CdmDestroy(cdm_handle);
  gasneti_assert_always (status == GNI_RC_SUCCESS);
  /*  fprintf(stderr, "node %d gasnetc_shutdown done\n", gasneti_mynode); */
}


GASNETI_INLINE(gasnetc_handle_am_short_packet)
void gasnetc_handle_am_short_packet(int req, gasnet_node_t source, 
			       gasnetc_am_short_packet_t *am)
{
  int handlerindex = am->header.handler;
  gasneti_handler_fn_t handler = gasnetc_handler[handlerindex];
  gasnetc_token_t the_token = { source, req };
  gasnet_token_t token = (gasnet_token_t)&the_token; /* RUN macro needs al lvalue */
  gasnet_handlerarg_t *pargs = (gasnet_handlerarg_t *) am->args;
  int numargs = am->header.numargs;
  GASNETI_RUN_HANDLER_SHORT(req, 
			    handlerindex,
			    handler,
			    token,
			    pargs,
			    numargs);
  if (!req) {
    /* TODO: would returning credit before running handler help or hurt? */
    gasnetc_return_am_credit(source);
  } else if (the_token.need_reply) {
    gasnetc_send_am_nop(source);
  }
}

GASNETI_INLINE(gasnetc_handle_am_medium_packet)
void gasnetc_handle_am_medium_packet(int req, gasnet_node_t source, 
				gasnetc_am_medium_packet_t *am, void* data)
{
  int handlerindex = am->header.handler;
  gasneti_handler_fn_t handler = gasnetc_handler[handlerindex];
  gasnetc_token_t the_token = { source, req };
  gasnet_token_t token = (gasnet_token_t)&the_token; /* RUN macro needs al lvalue */
  gasnet_handlerarg_t *pargs = (gasnet_handlerarg_t *) am->args;
  int numargs = am->header.numargs;
  GASNETI_RUN_HANDLER_MEDIUM(req, 
			     handlerindex,
			     handler,
			     token,
			     pargs,
			     numargs,
			     data,
			     am->header.misc);
  if (!req) {
    /* TODO: would returning credit before running handler help or hurt? */
    gasnetc_return_am_credit(source);
  } else if (the_token.need_reply) {
    gasnetc_send_am_nop(source);
  }
}

GASNETI_INLINE(gasnetc_handle_am_long_packet)
void gasnetc_handle_am_long_packet(int req, gasnet_node_t source, 
			      gasnetc_am_long_packet_t *am)
{
  int handlerindex = am->header.handler;
  gasneti_handler_fn_t handler = gasnetc_handler[handlerindex];
  gasnetc_token_t the_token = { source, req };
  gasnet_token_t token = (gasnet_token_t)&the_token; /* RUN macro needs al lvalue */
  gasnet_handlerarg_t *pargs = (gasnet_handlerarg_t *) am->args;
  int numargs = am->header.numargs;
  GASNETI_RUN_HANDLER_LONG(req, 
			   handlerindex,
			   handler,
			   token,
			   pargs,
			   numargs,
			   am->data,
			   am->data_length);
  if (!req) {
    /* TODO: would returning credit before running handler help or hurt? */
    gasnetc_return_am_credit(source);
  } else if (the_token.need_reply) {
    gasnetc_send_am_nop(source);
  }
}


extern void gasnetc_handle_sys_shutdown_packet(uint32_t source, gasnetc_sys_shutdown_packet_t *sys);
extern void  gasnetc_poll_smsg_completion_queue(void);

void gasnetc_process_smsg_q(gasnet_node_t pe)
{
  gni_return_t status;
  GC_Header_t *recv_header;
  union {
    gasnetc_packet_t packet;
    uint8_t raw[GASNETC_HEADLEN(medium, gasnet_AMMaxArgs()) + gasnet_AMMaxMedium()];
    uint64_t dummy_for_alignment;
  } buffer;
  size_t head_length;
  size_t length;
  uint32_t numargs;
  int is_req;
  for (;;) {
    GASNETC_LOCK_GNI();
    status = GNI_SmsgGetNext(bound_ep_handles[pe], 
			     (void **) &recv_header);
    GASNETC_UNLOCK_GNI_IF_SEQ();
    if (status == GNI_RC_SUCCESS) {
      gasneti_assert((((uintptr_t) recv_header) & 7) == 0);
      numargs = recv_header->numargs;
      if (numargs > gasnet_AMMaxArgs()) {
	gasnetc_GNIT_Abort("numargs %d, max is %ld\n", numargs, gasnet_AMMaxArgs());
      }
      GASNETI_TRACE_PRINTF(A, ("smsg r from %d to %d type %s\n", pe, gasneti_mynode, gasnetc_type_string(recv_header->command)));
      is_req = (1 & recv_header->command); /* Requests have ODD values */
      switch (recv_header->command) {
      case GC_CMD_AM_NOP_REPLY: {
	status = GNI_SmsgRelease(bound_ep_handles[pe]);
        GASNETC_UNLOCK_GNI_IF_PAR();
	gasnetc_return_am_credit(pe);
	break;
      }
      case GC_CMD_AM_SHORT:
      case GC_CMD_AM_SHORT_REPLY: {
	head_length = GASNETC_HEADLEN(short, numargs);
	memcpy(&buffer, recv_header, head_length);
	status = GNI_SmsgRelease(bound_ep_handles[pe]);
	GASNETC_UNLOCK_GNI_IF_PAR();
	gasnetc_handle_am_short_packet(is_req, pe, &buffer.packet.gasp);
	break;
      }
      case GC_CMD_AM_MEDIUM:
      case GC_CMD_AM_MEDIUM_REPLY: {
	head_length = GASNETC_HEADLEN(medium, numargs);
	length = head_length + recv_header->misc;
	memcpy(&buffer, recv_header, length);
	gasneti_assert(recv_header->misc <= gasnet_AMMaxMedium());
	status = GNI_SmsgRelease(bound_ep_handles[pe]);
	GASNETC_UNLOCK_GNI_IF_PAR();
	gasnetc_handle_am_medium_packet(is_req, pe, &buffer.packet.gamp, &buffer.raw[head_length]);
	break;
      }
      case GC_CMD_AM_LONG:
      case GC_CMD_AM_LONG_REPLY: {
	head_length = GASNETC_HEADLEN(long, numargs);
	memcpy(&buffer, recv_header, head_length);
	if (buffer.packet.galp.header.misc) { /* payload follows header - copy it into place */
	  void *im_data = (void *) (((uintptr_t) recv_header) + head_length);
	  memcpy(buffer.packet.galp.data, im_data, buffer.packet.galp.data_length);
	}
	status = GNI_SmsgRelease(bound_ep_handles[pe]);
	GASNETC_UNLOCK_GNI_IF_PAR();
	gasnetc_handle_am_long_packet(is_req, pe, &buffer.packet.galp);
	break;
      }
      case GC_CMD_SYS_SHUTDOWN_REQUEST: {
	memcpy(&buffer, recv_header, sizeof(buffer.packet.gssp));
	status = GNI_SmsgRelease(bound_ep_handles[pe]);
	GASNETC_UNLOCK_GNI_IF_PAR();
	gasnetc_handle_sys_shutdown_packet(pe, &buffer.packet.gssp);
	break;
      }
      default: {
	gasnetc_GNIT_Abort("unknown packet type");
      }
      }
      /* now check the SmsgRelease status */      
      if (status == GNI_RC_SUCCESS) {
	/* LCS nothing to do */
      } else {
	/* LCS SmsgRelease Failed */
	/* GNI_RC_INVALID_PARAM here means bad endpoint */
	/* GNI_RC_NOT_DONE here means there was no smsg */
	gasnetc_GNIT_Log("SmsgRelease from pe %d fail with %s\n",
		   pe, gni_return_string(status));
      }
    } else if (status == GNI_RC_NOT_DONE) {
      GASNETC_UNLOCK_GNI_IF_PAR();
      break;  /* GNI_RC_NOT_DONE here means there was no smsg */
    } else {
      gasnetc_GNIT_Abort("SmsgGetNext from pe %d fail with %s", 
		 pe, gni_return_string(status));
    }
    gasnetc_poll_smsg_completion_queue();
  }
}


#define SMSG_BURST 20
#define SMSG_PEER_BURST 4

/* algorithm, imagining that several threads call this at once
 * seize the GNI lock
 *   call CqGetEvent BURST times, into a local array
 * release the GNI lock
 * if any messages found:
 * seize the smsq_work_queue lock
 *   scan the local array, adding peers to the work queue if needed
 * release the smsg work queue lock
 * check the final status from the CqGetEvents
 *   if queue overflow
 *   seize the GNI lock and drain the cq
 *   seize the smsg_work_queue_lock and enqueue everyone not already there
 * up to PEER_BURST times
 *  try to dequeue a peer and process messages from them
 */

void gasnetc_poll_smsg_completion_queue(void)
{
  gni_return_t status;
  gasnet_node_t source;
  gni_cq_entry_t event_data[SMSG_BURST];
  int messages;
  int i;

  /* grab the gni lock, then spin through SMSG_BURST calls to
   * CqGetEvent as fast as possible, saving the interpretation
   * for later.  The GNI lock is global, for all GNI api activity.
   */
  messages = 0;
  GASNETC_LOCK_GNI();
  for (;;) {
    status = GNI_CqGetEvent(smsg_cq_handle,&event_data[messages]);
    if (status != GNI_RC_SUCCESS) break;
    messages += 1;
    if (messages >= SMSG_BURST) break;
  }
  GASNETC_UNLOCK_GNI();
  /* Now run through what you found */
  if (messages > 0) {
    GASNETC_LOCK_QUEUE(&smsg_work_queue);
    for (i = 0; i < messages; i += 1) {
      source = gni_cq_get_inst_id(event_data[i]);
      gasneti_assert(source < gasneti_nodes);
      /* atomically enqueue the peer on the smsg queue if it isn't
	 already there.  */
      if (peer_data[source].qi.queue == NULL)
	gasnetc_queue_enqueue_no_lock(&smsg_work_queue, 
				 (gasnetc_queue_item_t *) &peer_data[source]);
    }
    GASNETC_UNLOCK_QUEUE(&smsg_work_queue);
  }
  /* Now check the final status from the CqGetEvent */
  /* the most likely case is no more messages */
  /* The next most likely case is a full BURST, ending in success */
  /* Next is queue overflow, which shouldn't happen but is recoverable */
  if (status == GNI_RC_NOT_DONE) {
    /* nothing to do */
  } else if (status == GNI_RC_SUCCESS) {
    /* nothing to do */
  } else if (status == GNI_RC_ERROR_RESOURCE) {
    /* drain the cq completely */
    GASNETC_LOCK_GNI();
    for (;;) {
      status = GNI_CqGetEvent(smsg_cq_handle,&event_data[0]);
      if (status == GNI_RC_SUCCESS) continue;
      if (status == GNI_RC_ERROR_RESOURCE) continue;
      if (status == GNI_RC_NOT_DONE) break;
      gasnetc_GNIT_Abort("CqGetEvent(smsg_cq) drain returns error %s", gni_return_string(status));
    }
    GASNETC_UNLOCK_GNI();
    /* and enqueue everyone on the work queue, who isn't already */
    GASNETC_LOCK_QUEUE(&smsg_work_queue);
    for (source = 0; source < gasneti_nodes; source += 1) {
      if (peer_data[source].qi.queue == NULL)
	gasnetc_queue_enqueue_no_lock(&smsg_work_queue, 
				 (gasnetc_queue_item_t *) &peer_data[source]);
    }
    GASNETC_UNLOCK_QUEUE(&smsg_work_queue);
    gasnetc_GNIT_Log("smsg queue overflow");
  } else {
    /* anything else is a fatal error */
    gasnetc_GNIT_Abort("CqGetEvent(smsg_cq) returns error %s", gni_return_string(status));
  }
}

void gasnetc_poll_smsg_queue(void)
{
  int i;
  gasnet_node_t source;
  gasnetc_poll_smsg_completion_queue();
  /* Now see about processing some peers off the smsg_work_queue */
  for (i = 0; i < SMSG_PEER_BURST; i += 1) {
    peer_struct_t *peer = (peer_struct_t *) gasnetc_queue_pop(&smsg_work_queue);
    if (peer == NULL) break;
    source = peer->rank;
    gasnetc_process_smsg_q(source);
  }
}


void gasnetc_poll_local_queue(void)
{
  gni_return_t status;
  gni_cq_entry_t event_data;
  int i;
  gni_post_descriptor_t *pd;
  gasnetc_post_descriptor_t *gpd;

  GASNETC_LOCK_GNI();
  for (i = 0; i < gasnetc_poll_burst; i += 1) {
    /* Poll the bound_ep completion queue */
    status = GNI_CqGetEvent(bound_cq_handle,&event_data);
    if (status == GNI_RC_SUCCESS) {
      status = GNI_GetCompleted(bound_cq_handle, event_data, &pd);
      if (status != GNI_RC_SUCCESS)
	gasnetc_GNIT_Abort("GetCompleted(%p) failed %s\n",
		   (void *) event_data, gni_return_string(status));
      gpd = gasnetc_get_struct_addr_from_field_addr(gasnetc_post_descriptor_t, pd, pd);
      

      /* handle remaining work */
      if (gpd->flags & GC_POST_COPY) {
	memcpy(gpd->get_target, gpd->bounce_buffer, gpd->get_nbytes);
      }
      if (gpd->flags & GC_POST_UNREGISTER) {
	status = GNI_MemDeregister(nic_handle, &gpd->mem_handle);
	gasneti_assert_always (status == GNI_RC_SUCCESS);
      }
      if (gpd->flags & GC_POST_UNBOUNCE) {
	gasnetc_free_bounce_buffer(gpd->bounce_buffer);
      }
      if (gpd->flags & GC_POST_SEND) {
	status = GNI_SmsgSend(bound_ep_handles[gpd->dest], &gpd->u.galp, 
			      GASNETC_HEADLEN(long, gpd->u.galp.header.numargs),
			      NULL, 0, 0);
	gasneti_assert_always (status == GNI_RC_SUCCESS);
      }
      /* atomic int of the completion pointer suffices for explicit
	 and implicit nb operations in GASNet */

      if (gpd->flags & GC_POST_COMPLETION_FLAG) {
	gasneti_atomic_set((gasneti_atomic_t *) gpd->completion, 1, 0);
      }
      if (gpd->flags & GC_POST_COMPLETION_EOP) {
	gasnete_op_markdone(gpd->completion, (gpd->flags & GC_POST_GET) != 0);
      }
      gasnetc_free_post_descriptor(gpd);
    } else if (status == GNI_RC_NOT_DONE) {
      break;
    } else {
      gasnetc_GNIT_Log("bound CqGetEvent %s\n", gni_return_string(status));
    }
  }
  GASNETC_UNLOCK_GNI();
}
  
void gasnetc_poll(void)
{
  gasnetc_poll_smsg_queue();
  gasnetc_poll_local_queue();
}

void gasnetc_send_am_nop(uint32_t pe)
{
  gasnetc_am_nop_packet_t m;
  m.header.command = GC_CMD_AM_NOP_REPLY;
  m.header.misc    = 0;
  m.header.numargs = 0;
  m.header.handler = 0;
  gasnetc_send(pe, &m, sizeof(gasnetc_am_nop_packet_t), NULL, 0);
}


int gasnetc_send(gasnet_node_t dest, 
	    void *header, int header_length, 
	    void *data, int data_length)
{
  gni_return_t status;
  const int max_trial = 4;
  int trial;
  GASNETI_TRACE_PRINTF(A, ("smsg s from %d to %d type %s\n", gasneti_mynode, dest, gasnetc_type_string(((GC_Header_t *) header)->command)));
  for (trial = 0; trial < max_trial; ++trial) {
    GASNETC_LOCK_GNI();

    status = GNI_SmsgSend(bound_ep_handles[dest], header, 
			  header_length, data, data_length, 0);
    GASNETC_UNLOCK_GNI();
    if_pt (status == GNI_RC_SUCCESS) break;
    if (status != GNI_RC_NOT_DONE) {
      gasnetc_GNIT_Abort("GNI_SmsgSend returned error %s\n", gni_return_string(status));
    }

    /* XXX: GNI_RC_NOT_DONE should NOT happen due to our flow control.
       However, it DOES rarely happen, expecially when using all the cores.
       Of course the fewer credits we allocate the mote likely it is.
       So, we retry a finite number of times.  -PHH 2012.05.12
       TODO: Determine why/how we see NOT_DONE.
       TODO: trace/stats for this unfortunate event.
     */
    usleep(1 << trial); /* exponential back-off: 1us, 2us, 4us... */
  }
  return(GASNET_OK);
}


static void print_post_desc(char *title, gni_post_descriptor_t *cmd) {
  printf("r %d %s, desc addr %p\n", gasneti_mynode, title, cmd);
  printf("r %d status: %ld\n", gasneti_mynode, cmd->status);
  printf("r %d cq_mode_complete: 0x%x\n", gasneti_mynode, cmd->cq_mode_complete);
  printf("r %d cq_mode_type: %d\n", gasneti_mynode, cmd->type);
  printf("r %d cq_mode: 0x%x\n", gasneti_mynode, cmd->cq_mode);
  printf("r %d dlvr_mode: 0x%x\n", gasneti_mynode, cmd->dlvr_mode);
  printf("r %d local_address: %p(0x%lx, 0x%lx)\n", gasneti_mynode, (void *) cmd->local_addr, 
	 cmd->local_mem_hndl.qword1, cmd->local_mem_hndl.qword2);
  printf("r %d remote_address: %p(0x%lx, 0x%lx)\n", gasneti_mynode, (void *) cmd->remote_addr, 
	 cmd->remote_mem_hndl.qword1, cmd->remote_mem_hndl.qword2);
  printf("r %d length: 0x%lx\n", gasneti_mynode, cmd->length);
  printf("r %d rdma_mode: 0x%x\n", gasneti_mynode, cmd->rdma_mode);
  printf("r %d src_cq_hndl: %p\n", gasneti_mynode, cmd->src_cq_hndl);
  printf("r %d sync: (0x%lx,0x%lx)\n", gasneti_mynode, cmd->sync_flag_value, cmd->sync_flag_addr);
  printf("r %d amo_cmd: %d\n", gasneti_mynode, cmd->amo_cmd);
  printf("r %d amo: 0x%lx, 0x%lx\n", gasneti_mynode, cmd->first_operand, cmd->second_operand);
  printf("r %d cqwrite_value: 0x%lx\n", gasneti_mynode, cmd->cqwrite_value);
}

/* These are here due to transient resource problems in PostRdma
 *  They are not expected to happen (often)
 */

static gni_return_t myPostRdma(gni_ep_handle_t ep, gni_post_descriptor_t *pd)
{
  gni_return_t status;
  int i;
  i = 0;
  for (;;) {
      status = GNI_PostRdma(ep, pd);
      if (status == GNI_RC_SUCCESS) break;
      if (status != GNI_RC_ERROR_RESOURCE) break;
      if (i >= 1000) {
	fprintf(stderr, "postrdma retry failed\n");
	break;
      }
      GASNETC_UNLOCK_GNI();
      gasnetc_poll_local_queue();
      GASNETC_LOCK_GNI();
  }
  return (status);
}

static gni_return_t myPostFma(gni_ep_handle_t ep, gni_post_descriptor_t *pd)
{
  gni_return_t status;
  int i;
  i = 0;
  for (;;) {
      status = GNI_PostFma(ep, pd);
      if (status == GNI_RC_SUCCESS) break;
      if (status != GNI_RC_ERROR_RESOURCE) break;
      if (i >= 1000) {
	fprintf(stderr, "postrdma retry failed\n");
	break;
      }
      GASNETC_UNLOCK_GNI();
      gasnetc_poll_local_queue();
      GASNETC_LOCK_GNI();
  }
  return (status);
}

void gasnetc_rdma_put(gasnet_node_t dest,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  gni_post_descriptor_t *pd;
  gni_return_t status;

  /*   if (nbytes == 0) return;  shouldn't happen */
  pd = &gpd->pd;

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  /* confirm that the destination is in-segment on the far end */
  gasneti_boundscheck(dest, dest_addr, nbytes);
  if (!gasneti_in_segment(gasneti_mynode, source_addr, nbytes)) {
    /* source not (entirely) in segment */
    /* if (nbytes < gasnetc_bounce_register_cutover)  then use bounce buffer
     * else mem-register
     */
    pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
    pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
    pd->remote_addr = (uint64_t) dest_addr;
    pd->remote_mem_hndl = peer_segment_data[dest].segment_mem_handle;
    pd->length = nbytes;
    /* first deal with the memory copy and bounce buffer assignment */
    if (nbytes <= GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE) {
      gpd->bounce_buffer = gpd->u.immediate;
      memcpy(gpd->bounce_buffer, source_addr, nbytes);
      pd->local_addr = (uint64_t) gpd->bounce_buffer;
      pd->local_mem_hndl = mypeersegmentdata.segment_mem_handle;
    } else if (nbytes <  gasnetc_bounce_register_cutover) {
      gpd->flags |= GC_POST_UNBOUNCE;
      gpd->bounce_buffer = gasnetc_alloc_bounce_buffer();
      memcpy(gpd->bounce_buffer, source_addr, nbytes);
      pd->local_addr = (uint64_t) gpd->bounce_buffer;
      pd->local_mem_hndl = mypeersegmentdata.segment_mem_handle;
    } else {
      gpd->flags |= GC_POST_UNREGISTER;
      pd->local_addr = (uint64_t) source_addr;
      GASNETC_LOCK_GNI();
      {
	int count = 0;
	for (;;) {
	  status = GNI_MemRegister(nic_handle, (uint64_t) source_addr, 
				   (uint64_t) nbytes, NULL,
				   GNI_MEM_STRICT_PI_ORDERING | GNI_MEM_PI_FLUSH 
				   |GNI_MEM_READWRITE, -1, &gpd->mem_handle);
	  if (status == GNI_RC_SUCCESS) break;
	  if (status == GNI_RC_ERROR_RESOURCE) {
	    fprintf(stderr, "MemRegister fault %d at  %p %lx, code %s\n", count, source_addr, nbytes,
		    gni_return_string(status));
	    count += 1;
	    if (count >= 10) break;
	  } else {
	    break;
	  }
	}
      }
      GASNETC_UNLOCK_GNI();
      gasneti_assert_always (status == GNI_RC_SUCCESS);
      pd->local_mem_hndl = gpd->mem_handle;
    }
    /* now initiate the transfer according to fma/rdma cutover */
    if (nbytes <= gasnetc_fma_rdma_cutover) {
      pd->type = GNI_POST_FMA_PUT;
      GASNETC_LOCK_GNI();
      status = myPostFma(bound_ep_handles[dest], pd);
      GASNETC_UNLOCK_GNI();
      if (status != GNI_RC_SUCCESS) {
	print_post_desc((char *) "non-segment-postfma", pd);
	gasnetc_GNIT_Abort("PostFMA failed with %s\n", gni_return_string(status));
      }
    } else {
      pd->type = GNI_POST_RDMA_PUT;
      GASNETC_LOCK_GNI();
      status = myPostRdma(bound_ep_handles[dest], pd);
      GASNETC_UNLOCK_GNI();
      if (status != GNI_RC_SUCCESS) {
	print_post_desc((char *) "non-segment-postrdma", pd);
	gasnetc_GNIT_Abort("PostRdma failed with %s\n", gni_return_string(status));
      }
    }


  } else {
    if (nbytes <= gasnetc_fma_rdma_cutover) {
      pd->type = GNI_POST_FMA_PUT;
      pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
      pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
      pd->local_addr = (uint64_t) source_addr;
      pd->local_mem_hndl = mypeersegmentdata.segment_mem_handle;
      pd->remote_addr = (uint64_t) dest_addr;
      pd->remote_mem_hndl = peer_segment_data[dest].segment_mem_handle;
      pd->length = nbytes;
      GASNETC_LOCK_GNI();
      status = myPostFma(bound_ep_handles[dest], pd);
      GASNETC_UNLOCK_GNI();
      if (status != GNI_RC_SUCCESS) {
	print_post_desc((char *) "in-segment-postfma", pd);
	gasnetc_GNIT_Abort("Postfma failed with %s\n", gni_return_string(status));
      }
    } else {
      pd->type = GNI_POST_RDMA_PUT;
      pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
      pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
      pd->local_addr = (uint64_t) source_addr;
      pd->local_mem_hndl = mypeersegmentdata.segment_mem_handle;
      pd->remote_addr = (uint64_t) dest_addr;
      pd->remote_mem_hndl = peer_segment_data[dest].segment_mem_handle;
      pd->length = nbytes;
      GASNETC_LOCK_GNI();
      status =myPostRdma(bound_ep_handles[dest], pd);
      GASNETC_UNLOCK_GNI();
      if (status != GNI_RC_SUCCESS) {
	print_post_desc((char *) "in-segment-postrdma", pd);
	gasnetc_GNIT_Abort("PostRdma failed with %s\n", gni_return_string(status));
      }
    }
  }
}

/* Algorithm

   get reuqires 4 byte alignment for source, dst, and length

   if ((src | dst | len) & 3) == 0 {  // case aligned 
      if (in segment) {
        if (length < YYY) PostFMA
	else PostRdma
      } else { // out of semgnt
        if (length < XXX) use bounce buffer
        else call memregister
      }
      } else { //unaligned
        if (length < XXX) use bounce buffer
	   if (length < YYY) PostFMA
	   else PostRdma
	else use active message
      }
 */


/* for get, source_addr is remote */
void gasnetc_rdma_get(gasnet_node_t dest,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  gni_post_descriptor_t *pd;
  gni_return_t status;

  /*  if (nbytes == 0) return; */
  gasneti_assert(gpd);
  pd = &gpd->pd;
  gpd->flags |= GC_POST_GET;
  /* confirm that the destination is in-segment on the far end */
  gasneti_boundscheck(dest, source_addr, nbytes);
  /* check where the local addr is */
  if (!gasneti_in_segment(gasneti_mynode, dest_addr, nbytes)) {
    /* dest not (entirely) in segment */
    /* if (nbytes < gasnetc_bounce_register_cutover)  then use bounce buffer
     * else mem-register
     */
    pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
    pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
    pd->remote_addr = (uint64_t) source_addr;
    pd->remote_mem_hndl = peer_segment_data[dest].segment_mem_handle;

    pd->length = nbytes;
    if (nbytes < GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE) {
      gpd->flags |= GC_POST_COPY;
      gpd->bounce_buffer = gpd->u.immediate;
      gpd->get_target = dest_addr;
      gpd->get_nbytes = nbytes;
      pd->local_addr = (uint64_t) gpd->bounce_buffer;
      pd->local_mem_hndl = mypeersegmentdata.segment_mem_handle;
    } else if (nbytes < gasnetc_bounce_register_cutover) {
      gpd->flags |= GC_POST_UNBOUNCE | GC_POST_COPY;
      gpd->bounce_buffer = gasnetc_alloc_bounce_buffer();
      gpd->get_target = dest_addr;
      gpd->get_nbytes = nbytes;
      pd->local_addr = (uint64_t) gpd->bounce_buffer;
      pd->local_mem_hndl = mypeersegmentdata.segment_mem_handle;
    } else {
      gpd->flags |= GC_POST_UNREGISTER;
      pd->local_addr = (uint64_t) dest_addr;
      GASNETC_LOCK_GNI();
      {
	int count = 0;
	for (;;) {
	  status = GNI_MemRegister(nic_handle, (uint64_t) dest_addr, 
				   (uint64_t) nbytes, NULL,
				   GNI_MEM_STRICT_PI_ORDERING | GNI_MEM_PI_FLUSH 
				   |GNI_MEM_READWRITE, -1, &gpd->mem_handle);
	  if (status == GNI_RC_SUCCESS) break;
	  if (status == GNI_RC_ERROR_RESOURCE) {
	    fprintf(stderr, "MemRegister fault %d at  %p %lx, code %s\n",
		    count, dest_addr, nbytes,
		    gni_return_string(status));
	    count += 1;
	    if (count >= 10) break;
	  } else {
	    break;
	  }
	}
      }
      GASNETC_UNLOCK_GNI();
      gasneti_assert_always (status == GNI_RC_SUCCESS);
      pd->local_mem_hndl = gpd->mem_handle;
    }
    if (nbytes <= gasnetc_fma_rdma_cutover) {
      pd->type = GNI_POST_FMA_GET;
      GASNETC_LOCK_GNI();
      status = myPostFma(bound_ep_handles[dest], pd);
      GASNETC_UNLOCK_GNI();
      if (status != GNI_RC_SUCCESS) {
	print_post_desc((char *) "non-segment-postfma", pd);
	gasnetc_GNIT_Abort("PostFMA failed with %s\n", gni_return_string(status));
      }
    } else {
      pd->type = GNI_POST_RDMA_GET;
      GASNETC_LOCK_GNI();
      status = myPostRdma(bound_ep_handles[dest], pd);
      GASNETC_UNLOCK_GNI();
      if (status != GNI_RC_SUCCESS) {
	print_post_desc((char *) "non-segment-postrdma", pd);
	gasnetc_GNIT_Abort("PostRdma failed with %s\n", gni_return_string(status));
      }
    }
  } else {
    pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
    pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
    pd->local_addr = (uint64_t) dest_addr;
    pd->local_mem_hndl = mypeersegmentdata.segment_mem_handle;
    pd->remote_addr = (uint64_t) source_addr;
    pd->remote_mem_hndl = peer_segment_data[dest].segment_mem_handle;
    pd->length = nbytes;
    if (nbytes <= gasnetc_fma_rdma_cutover) {
      pd->type = GNI_POST_FMA_GET;
      GASNETC_LOCK_GNI();
      status = myPostFma(bound_ep_handles[dest], pd);
      GASNETC_UNLOCK_GNI();
      if (status != GNI_RC_SUCCESS) {
	print_post_desc((char *) "in-segment-postfma", pd);
	gasnetc_GNIT_Abort("PostFMA failed with %s\n", gni_return_string(status));
      }
    } else {
      pd->type = GNI_POST_RDMA_GET;
      GASNETC_LOCK_GNI();
      status = myPostRdma(bound_ep_handles[dest], pd);
      GASNETC_UNLOCK_GNI();
      if (status != GNI_RC_SUCCESS) {
	print_post_desc((char *) "in-segment-postrdma", pd);
	gasnetc_GNIT_Abort("PostRdma failed with %s\n", gni_return_string(status));
      }
    }
  }
}


/* returns 1 if-and-only-if value was decremented.  based on gasneti_semaphore_trydown() */
GASNETI_INLINE(gasnetc_atomic_dec_if_positive)
int gasnetc_atomic_dec_if_positive(gasneti_weakatomic_t *p)
{
  int swapped;
  do {
    const gasneti_weakatomic_val_t old = gasneti_weakatomic_read(p, 0);
    if_pf (old == 0) {
      return 0;       /* Note: "break" here generates infinite loop w/ pathcc 2.4 (bug 1620) */
    }
    swapped = gasneti_weakatomic_compare_and_swap(p, old, old - 1, GASNETI_ATOMIC_ACQ_IF_TRUE);
  } while (GASNETT_PREDICT_FALSE(!swapped));
  return 1;
}
void gasnetc_get_am_credit(uint32_t pe)
{
  gasneti_weakatomic_t *p = &peer_data[pe].am_credit;
#if GASNETC_DEBUG
  fprintf(stderr, "r %d get am credit for %d, before is %d\n",
	 gasneti_mynode, pe, (int)gasneti_weakatomic_read(&peer_data[pe].am_credit, 0));
#endif
  while (gasnetc_atomic_dec_if_positive(p) == 0) {
    gasneti_AMPoll();
    gasneti_spinloop_hint();
  }
}

void gasnetc_return_am_credit(uint32_t pe)
{
  gasneti_weakatomic_increment(&peer_data[pe].am_credit, GASNETI_ATOMIC_NONE);
#if GASNETC_DEBUG
  fprintf(stderr, "r %d return am credit for %d, after is %d\n",
	 gasneti_mynode, pe, (int)gasneti_weakatomic_read(&peer_data[pe].am_credit, 0));
#endif
}

void gasnetc_get_fma_credit(uint32_t pe)
{
  gasneti_weakatomic_t *p = &peer_data[pe].fma_credit;
  while (gasnetc_atomic_dec_if_positive(p) == 0) gasnetc_poll_local_queue();
}

void gasnetc_return_fma_credit(uint32_t pe)
{
  gasneti_weakatomic_increment(&peer_data[pe].fma_credit, GASNETI_ATOMIC_NONE);
}


void gasnetc_queue_init(gasnetc_queue_t *q) 
{
  q->head = NULL;
  q->tail = NULL;
  GASNETC_INITLOCK_QUEUE(q);
}

void gasnetc_queue_item_init(gasnetc_queue_item_t *qi) 
{
  qi->next = NULL;
  qi->queue = NULL;
}

#if 0
gasnetc_queue_item_t *gasnetc_dequeue(gasnetc_queue_t *q, gasnetc_queue_item_t *qi)
{
  gasneti_assert(q != NULL);
  GASNETC_LOCK_QUEUE(q);
  if (qi != NULL) {
    if (qi->prev == NULL) q->head = qi->next;
    else qi->prev->next = qi->next;
    if (qi->next == NULL) q->tail = qi->prev;
    else qi->next->prev = qi->prev;
    GC_DEBUG(qi->next = NULL);
    GC_DEBUG(qi->prev = NULL);
  }
  GASNETC_UNLOCK_QUEUE(q);
  return(qi);
}
#endif 

gasnetc_queue_item_t *gasnetc_queue_pop(gasnetc_queue_t *q)
{
  gasnetc_queue_item_t *qi;
  gasneti_assert(q != NULL);
  GASNETC_LOCK_QUEUE(q);
  qi = q->head;
  if (qi != NULL) {
    gasneti_assert(qi->queue == q);
    q->head = qi->next;
    if (q->head == NULL) q->tail = NULL;
    qi->queue = NULL;
    GC_DEBUG(qi->next = NULL);
  }
  GASNETC_UNLOCK_QUEUE(q);
  return(qi);
}

void gasnetc_queue_push(gasnetc_queue_t *q, gasnetc_queue_item_t *qi)
{
  gasneti_assert(q != NULL);
  gasneti_assert(qi != NULL);
  gasneti_assert(qi->queue == NULL);
  GASNETC_LOCK_QUEUE(q);
  qi->next = q->head;
  q->head = qi;
  if (q->tail == NULL) q->tail = qi;
  qi->queue = q;
  GASNETC_UNLOCK_QUEUE(q);
}

void gasnetc_queue_enqueue(gasnetc_queue_t *q, gasnetc_queue_item_t *qi)
{
  gasneti_assert(q != NULL);
  gasneti_assert(qi != NULL);
  gasneti_assert(qi->queue == NULL);
  GASNETC_LOCK_QUEUE(q);
  if (q->head == NULL) {
    q->head = qi;
  } else {
    gasneti_assert(q->tail != NULL);
    q->tail->next = qi;
  }
  q->tail = qi;
  qi->next = NULL;
  qi->queue = q;
  GASNETC_UNLOCK_QUEUE(q);
}

/* This version is used from smsg_poll inside a loop that holds the lock
 * to amortize locking costs
 */

void gasnetc_queue_enqueue_no_lock(gasnetc_queue_t *q, gasnetc_queue_item_t *qi)
{
  gasneti_assert(q != NULL);
  gasneti_assert(qi != NULL);
  gasneti_assert(qi->queue == NULL);
  /* GASNETC_LOCK_QUEUE(q); */
  if (q->head == NULL) {
    q->head = qi;
  } else {
    gasneti_assert(q->tail != NULL);
    q->tail->next = qi;
  }
  q->tail = qi;
  qi->next = NULL;
  qi->queue = q;
  /* GASNETC_UNLOCK_QUEUE(q); */
}

int gasnetc_queue_on_queue(gasnetc_queue_item_t *qi)
{
  gasneti_assert(qi != NULL);
  return (qi->queue != NULL);
}

static gasneti_lifo_head_t post_descriptor_pool = GASNETI_LIFO_INITIALIZER;

/* Needs no lock because it is called only from the init code */
void gasnetc_init_post_descriptor_pool(void)
{
  int i;
  gasnetc_post_descriptor_t *data = gasnetc_pd_buffers.addr;
  gasneti_assert_always(data);
  for (i = 0; i < (gasnetc_pd_buffers.size / sizeof(gasnetc_post_descriptor_t)); i += 1) {
    gasneti_lifo_push(&post_descriptor_pool, &data[i]);
  }
}

/* This needs no lock because there is an internal lock in the queue */
gasnetc_post_descriptor_t *gasnetc_alloc_post_descriptor(void)
{
  gasnetc_post_descriptor_t *gpd;
  while ((gpd = (gasnetc_post_descriptor_t *) 
	  gasneti_lifo_pop(&post_descriptor_pool)) == NULL)
    gasnetc_poll_local_queue();
  gpd->completion = NULL;
  return(gpd);
}


/* This needs no lock because there is an internal lock in the queue */
/* LCS inline this */
void gasnetc_free_post_descriptor(gasnetc_post_descriptor_t *gpd)
{
  gasneti_lifo_push(&post_descriptor_pool, gpd);
}

/* exit related */
volatile int gasnetc_shutdownInProgress = 0;
double gasnetc_shutdown_seconds = 0.0;
static double shutdown_max = 120.;  /* 2 minutes */
static uint32_t sys_exit_rcvd = 0;


/* XXX: probably need to obtain am_credit or otherwise guard against
   the possibility of GNI_SmsgSend() returning GNI_RC_NOT_DONE. */
extern void gasnetc_sys_SendShutdownMsg(gasnet_node_t node, int shift, int exitcode)
{
  gasnetc_sys_shutdown_packet_t shutdown;
  GASNETI_TRACE_PRINTF(C,("Send SHUTDOWN Request to node %d w/ shift %d, exitcode %d",node,shift,exitcode));
  shutdown.header.command = GC_CMD_SYS_SHUTDOWN_REQUEST;
  shutdown.header.misc    = exitcode; /* only 15 bits, but exit() only preserves low 8-bits anyway */
  shutdown.header.numargs = 0;
  shutdown.header.handler = shift; /* log(distance) */
  gasnetc_send(node, &shutdown, sizeof(gasnetc_sys_shutdown_packet_t), NULL, 0);
}


/* this is called from poll when a shutdown packet arrives */
void gasnetc_handle_sys_shutdown_packet(uint32_t source, gasnetc_sys_shutdown_packet_t *sys)
{
  uint32_t distance = 1 << sys->header.handler;
  uint8_t exitcode = sys->header.misc;
  uint8_t oldcode;
#if GASNET_DEBUG || GASNETI_STATS_OR_TRACE
  int sender = source;
  gasneti_assert_always(((uint32_t)sender + distance) % gasneti_nodes == gasneti_mynode);
  GASNETI_TRACE_PRINTF(C,("Got SHUTDOWN Request from node %d w/ exitcode %d",sender,exitcode));
#endif
  oldcode = gasneti_atomic_read((gasneti_atomic_t *) &gasnetc_exitcode, 0);
  if (exitcode > oldcode) {
    gasneti_atomic_set((gasneti_atomic_t *) &gasnetc_exitcode, exitcode, 0);
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
    if (!gasnetc_shutdownInProgress) gasnetc_exit(exitcode);
  }
}


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
  int shift;
  gasneti_tick_t timeout_us = 1e6 * gasnetc_shutdown_seconds;
  gasneti_tick_t starttime = gasneti_ticks_now();

  GASNETI_TRACE_PRINTF(C,("Entering SYS EXIT"));

  /*  gasneti_assert(portals_sysqueue_initialized); */

  for (distance = 1, shift = 0; distance < gasneti_nodes; distance *= 2, ++shift) {
    gasnet_node_t peer;

    if (distance >= gasneti_nodes - gasneti_mynode) {
      peer = gasneti_mynode - (gasneti_nodes - distance);
    } else {
      peer = gasneti_mynode + distance;
    }

    oldcode = gasneti_atomic_read((gasneti_atomic_t *) &gasnetc_exitcode, 0);
    exitcode = MAX(exitcode, oldcode);

    gasnetc_sys_SendShutdownMsg(peer, shift, exitcode);

    /* wait for completion of the proper receive, which might arrive out of order */
    goal |= distance;
    while ((sys_exit_rcvd & goal) != goal) {
      gasnetc_poll();
      if (gasneti_ticks_to_us(gasneti_ticks_now() - starttime) > timeout_us) {
        result = 1; /* failure */
        goto out;
      }
    }
  }

out:
  oldcode = gasneti_atomic_read((gasneti_atomic_t *) &gasnetc_exitcode, 0);
  *exitcode_p = MAX(exitcode, oldcode);

  return result;
}






/* AuxSeg setup for registered bounce buffer space*/
gasneti_auxseg_request_t gasnetc_bounce_auxseg_alloc(gasnet_seginfo_t *auxseg_info) {
  gasneti_auxseg_request_t retval;
  
  retval.minsz = gasneti_getenv_int_withdefault("GASNETC_GNI_MIN_BOUNCE_SIZE",
                                                GASNETC_GNI_MIN_BOUNCE_SIZE_DEFAULT,1);
  retval.optimalsz = gasneti_getenv_int_withdefault("GASNETC_GNI_BOUNCE_SIZE",

                                                    GASNETC_GNI_BOUNCE_SIZE_DEFAULT,1);
  if (retval.optimalsz < retval.minsz) retval.optimalsz = retval.minsz;
#if GASNET_DEBUG_VERBOSE
  fprintf(stderr, "auxseg asking for min  %ld opt %ld\n", retval.minsz, retval.optimalsz);
#endif
  if (auxseg_info == NULL){
    return retval; /* initial query */
  }	
  else { /* auxseg granted */
    /* The only one we care about is our own node */
    gasnetc_bounce_buffers = auxseg_info[gasneti_mynode];
#if GASNET_DEBUG_VERBOSE
    fprintf(stderr, "got auxseg %p size %ld\n", gasnetc_bounce_buffers.addr,
	    gasnetc_bounce_buffers.size);
#endif
  }

  return retval;
}

/* AuxSeg setup for registered post descriptors*/
gasneti_auxseg_request_t gasnetc_pd_auxseg_alloc(gasnet_seginfo_t *auxseg_info) {
  gasneti_auxseg_request_t retval;
  
  retval.minsz = gasneti_getenv_int_withdefault("GASNETC_GNI_MIN_NUM_PD",
                                                GASNETC_GNI_MIN_NUM_PD_DEFAULT,1)
    * sizeof(gasnetc_post_descriptor_t);
  retval.optimalsz = gasneti_getenv_int_withdefault("GASNETC_GNI_NUM_PD",

                                                    GASNETC_GNI_NUM_PD_DEFAULT,1) 
    * sizeof(gasnetc_post_descriptor_t);
  if (retval.optimalsz < retval.minsz) retval.optimalsz = retval.minsz;
#if GASNET_DEBUG_VERBOSE
  fprintf(stderr, "auxseg post descriptor asking for min  %ld opt %ld\n", retval.minsz, retval.optimalsz);
#endif
  if (auxseg_info == NULL){
    return retval; /* initial query */
  }	
  else { /* auxseg granted */
    /* The only one we care about is our own node */
    gasnetc_pd_buffers = auxseg_info[gasneti_mynode];
#if GASNET_DEBUG_VERBOSE
    fprintf(stderr, "got pd auxseg %p size %ld\n", gasnetc_pd_buffers.addr,
	    gasnetc_pd_buffers.size);
#endif
  }

  return retval;
}

gasneti_lifo_head_t gasnetc_bounce_buffer_pool = GASNETI_LIFO_INITIALIZER;

void gasnetc_init_bounce_buffer_pool(void)
{
  int i;
  int num_bounce;
  gasneti_assert_always(gasnetc_bounce_buffers.addr != NULL);
  gasneti_assert_always(gasnetc_bounce_buffers.size >= GASNETC_GNI_MIN_BOUNCE_SIZE_DEFAULT);
  gasnetc_bounce_register_cutover = 
    gasneti_getenv_int_withdefault("GASNETC_GNI_BOUNCE_REGISTER_CUTOVER",
				   GASNETC_GNI_BOUNCE_REGISTER_CUTOVER_DEFAULT,1);
  if (gasnetc_bounce_register_cutover > GASNETC_GNI_BOUNCE_REGISTER_CUTOVER_MAX)
    gasnetc_bounce_register_cutover = GASNETC_GNI_BOUNCE_REGISTER_CUTOVER_MAX;
  if (gasnetc_bounce_register_cutover > gasnetc_bounce_buffers.size)
    gasnetc_bounce_register_cutover = gasnetc_bounce_buffers.size;
  num_bounce = gasnetc_bounce_buffers.size / gasnetc_bounce_register_cutover;
  for(i = 0; i < num_bounce; i += 1) {
    gasneti_lifo_push(&gasnetc_bounce_buffer_pool, (char *) gasnetc_bounce_buffers.addr + 
		      (gasnetc_bounce_register_cutover * i));
  }
}

void *gasnetc_alloc_bounce_buffer(void)
{
  void *buf;
  while ((buf = gasneti_lifo_pop(&gasnetc_bounce_buffer_pool)) == NULL) 
	 gasnetc_poll_local_queue();
  return(buf);
}

void gasnetc_free_bounce_buffer(void *gcb)
{
  gasneti_lifo_push(&gasnetc_bounce_buffer_pool, gcb);
}
