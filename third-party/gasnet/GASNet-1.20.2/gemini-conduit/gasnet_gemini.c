#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_handler.h>
#include <gasnet_gemini.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>

#define GASNETC_NETWORKDEPTH_DEFAULT 12

#ifdef GASNET_CONDUIT_GEMINI
  /* Use remote event + PI_FLUSH to get "proper" ordering w/ relaxed and default PI ordering */
  #define FIX_HT_ORDERING 1
#else
  #define FIX_HT_ORDERING 0
#endif

int      gasnetc_dev_id;
uint32_t gasnetc_cookie;
uint32_t gasnetc_address;
uint8_t  gasnetc_ptag;

static uint32_t gasnetc_memreg_flags;
static int gasnetc_mem_consistency;

static int bank_credits;

typedef union {
  volatile uint32_t full; /* is zero until filled */
  gasnetc_packet_t packet;
  uint8_t raw[GASNETC_MSG_MAXSIZE];
} gasnetc_mailbox_t;

typedef struct peer_struct {
  gni_ep_handle_t ep_handle;
  gni_mem_handle_t mem_handle;
  gasneti_weakatomic_t am_credit;
  gasneti_weakatomic_t am_credit_bank; /* single bit of credit accumulator for return to peer */
  struct {
    gasnetc_mailbox_t *loc_addr;
    gasnetc_mailbox_t *rem_addr;
    gni_mem_handle_t rem_hndl;
    /* "pos" variables are in range [1..mb_slots] and moving backwards */
    unsigned int recv_pos;
    unsigned int send_pos;
  } mb;
} peer_struct_t;

static gni_mem_handle_t my_smsg_handle;

static gni_cdm_handle_t cdm_handle;
static gni_cq_handle_t destination_cq_handle;

static void *smsg_mmap_ptr;
static size_t smsg_mmap_bytes;

static gasnet_seginfo_t gasnetc_bounce_buffers;
static gasnet_seginfo_t gasnetc_pd_buffers;

static unsigned int log2_remote;
static unsigned int mb_slots;
static unsigned int am_maxcredit;


/*------ Group the most commonly accessed variables together ------*/
/* TODO: could move gasneti_{mynode,nodes} here, but it is non-trivial */

/* read-only: */
static gni_nic_handle_t nic_handle;
static gni_mem_handle_t my_mem_handle;
static gni_cq_handle_t bound_cq_handle;
static gni_cq_handle_t smsg_cq_handle;
static int gasnetc_poll_burst = 10;
static peer_struct_t *peer_data;
#if FIX_HT_ORDERING
static uint16_t gasnetc_fma_put_cq_mode = GNI_CQMODE_GLOBAL_EVENT;
#endif
static size_t gasnetc_get_fma_rdma_cutover;
static size_t gasnetc_put_fma_rdma_cutover;
static size_t gasnetc_get_bounce_register_cutover;
static size_t gasnetc_put_bounce_register_cutover;
size_t gasnetc_max_get_unaligned;
size_t gasnetc_max_put_lc;

/* lock: */
static int8_t pad0[GASNETC_CACHELINE_SIZE];
gasnetc_gni_lock_t gasnetc_gni_lock;

/* read-write: */
static int8_t pad1[GASNETC_CACHELINE_SIZE];
static gasneti_weakatomic_t gasnetc_reg_credit;

/* lifo_head_t contains cache-line padding */
static gasneti_lifo_head_t post_descriptor_pool = GASNETI_LIFO_INITIALIZER;
static gasneti_lifo_head_t gasnetc_bounce_buffer_pool = GASNETI_LIFO_INITIALIZER;
#if !GASNET_CONDUIT_GEMINI
gasneti_lifo_head_t gasnetc_smsg_buffers = GASNETI_LIFO_INITIALIZER;
#endif

/*------ Convience functions for printing error messages ------*/

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


#if GASNET_TRACE
const char *gasnetc_type_string(int type)
{
  if (type == GC_CMD_NULL) return("GC_CMD_AM_NULL");
  if (type == GC_CMD_AM_SHORT) return ("GC_CMD_AM_SHORT");
  if (type == GC_CMD_AM_LONG) return ("GC_CMD_AM_LONG");
  if (type == GC_CMD_AM_MEDIUM) return ("GC_CMD_AM_MEDIUM");
  return("unknown");
}
#endif

const char *gasnetc_post_type_string(gni_post_type_t type)
{
  if (type == GNI_POST_RDMA_PUT) return("GNI_POST_RDMA_PUT");
  if (type == GNI_POST_RDMA_GET) return("GNI_POST_RDMA_GET");
  if (type == GNI_POST_FMA_PUT) return("GNI_POST_FMA_PUT");
  if (type == GNI_POST_FMA_GET) return("GNI_POST_FMA_GET");
  if (type == GNI_POST_FMA_PUT_W_SYNCFLAG) return("GNI_POST_FMA_PUT_W_SYNCFLAG");
  return("unknown");
}

/*------ Used to probe registration capabilities ------*/

#if 0 /* Currently unused */
int gasnetc_try_pin(void *addr, uintptr_t size)
{ 
  int i;

  for (i=0; i<100; ++i) {
    gni_mem_handle_t mem_handle;
    gni_return_t status;

    status = GNI_MemRegister(nic_handle, (uint64_t)addr, size, NULL,
                             gasnetc_memreg_flags, -1, &mem_handle);
    if_pt (status == GNI_RC_SUCCESS) {
      status = GNI_MemDeregister(nic_handle, &mem_handle);
      gasneti_assert(status == GNI_RC_SUCCESS);
      return 1;
    }
    if (status != GNI_RC_ERROR_RESOURCE) break; /* Fatal */
  }

  return 0;
}
#endif

/*------ Functions for dynamic memory registration ------*/

static gasneti_weakatomic_val_t gasnetc_reg_credit_max;
#define gasnetc_init_reg_credit(_val) \
	gasneti_weakatomic_set(&gasnetc_reg_credit,gasnetc_reg_credit_max=(_val),0)

/* Register local side of a pd, with unbounded retry */
static void gasnetc_register_pd(gni_post_descriptor_t *pd)
{
  const uint64_t addr = pd->local_addr;
  const size_t nbytes = pd->length;
  int trial = 0;
  gni_return_t status;

  if_pf (gasnetc_reg_credit_max &&
         !gasnetc_weakatomic_dec_if_positive(&gasnetc_reg_credit)) {
    /* We may simple not have polled the Cq recently.
       So, WAITHOOK and STALL tracing only if still nothing after first poll */
    GASNETC_TRACE_WAIT_BEGIN();
    int stall = 0;
    goto first;
    do {
      GASNETI_WAITHOOK();
      stall = 1;
first:
      gasnetc_poll_local_queue();
    } while (!gasnetc_weakatomic_dec_if_positive(&gasnetc_reg_credit));
    if_pf (stall) GASNETC_TRACE_WAIT_END(MEM_REG_STALL);
  }

  for (;;) {
    GASNETC_LOCK_GNI();
    status = GNI_MemRegister(nic_handle, addr, nbytes, NULL,
                             gasnetc_memreg_flags, -1, &pd->local_mem_hndl);
    GASNETC_UNLOCK_GNI();
    if_pt (status == GNI_RC_SUCCESS) {
      if (trial) GASNETC_STAT_EVENT_VAL(MEM_REG_RETRY, trial);
      return;
    }
    if (status != GNI_RC_ERROR_RESOURCE) break; /* Fatal */
    GASNETI_WAITHOOK();
    gasnetc_poll_local_queue();
    ++trial;
  }
  gasnetc_GNIT_Abort("MemRegister failed with %s", gni_return_string(status));
}

/* Deregister local side of a pd */
GASNETI_INLINE(gasnetc_deregister_pd)
void gasnetc_deregister_pd(gni_post_descriptor_t *pd)
{
  gni_return_t status = GNI_MemDeregister(nic_handle, &pd->local_mem_hndl);
  gasneti_assert_always (status == GNI_RC_SUCCESS);
  if (gasnetc_reg_credit_max) gasneti_weakatomic_increment(&gasnetc_reg_credit, 0);
}

/*-------------------------------------------------*/
/* We don't allocate resources for comms w/ self or PSHM-reachable peers */

#if GASNET_PSHM
  #define node_is_local(_i) gasneti_pshm_in_supernode(_i)
#else
  #define node_is_local(_i) ((_i) == gasneti_mynode)
#endif

/* From point-of-view of a remote node, what is MY index as an Smsg peer? */
GASNETI_INLINE(my_smsg_index)
int my_smsg_index(gasnet_node_t remote_node) {
#if GASNET_PSHM
  int i, result = 0;

  gasneti_assert(NULL != gasneti_nodemap);
  for (i = 0; i < gasneti_mynode; ++i) {
    /* counts nodes that are not local to remote_node */
    result += (gasneti_nodemap[i] != gasneti_nodemap[remote_node]);
  }
  return result;
#else
  /* we either fall before or after the remote node skips over itself */
  return gasneti_mynode - (gasneti_mynode > remote_node);
#endif
}

/*-------------------------------------------------*/

static uint32_t *gather_nic_addresses(void)
{
  uint32_t *result = gasneti_malloc(gasneti_nodes * sizeof(uint32_t));

  if (gasnetc_dev_id == -1) {
    /* no value was found in environment */
    gni_return_t status;
    uint32_t cpu_id;

    gasnetc_dev_id  = 0;
    status = GNI_CdmGetNicAddress(gasnetc_dev_id, &gasnetc_address, &cpu_id);
    if (status != GNI_RC_SUCCESS) {
      gasnetc_GNIT_Abort("GNI_CdmGetNicAddress failed: %s", gni_return_string(status));
    }
  } else {
    /* use gasnetc_address taken from the environment */
  }

  gasnetc_bootstrapExchange(&gasnetc_address, sizeof(uint32_t), result);

  return result;
}

/*-------------------------------------------------*/
/* called after segment init. See gasneti_seginfo */
/* allgather the memory handles for the segments */
/* create endpoints */
void gasnetc_init_segment(void *segment_start, size_t segment_size)
{
  gni_return_t status;
  /* Map the shared segment */

  /* Protocol switch points: FMA vs. RDMA */
  gasnetc_get_fma_rdma_cutover = 
    gasneti_getenv_int_withdefault("GASNET_GNI_GET_FMA_RDMA_CUTOVER",
				   GASNETC_GNI_GET_FMA_RDMA_CUTOVER_DEFAULT,1);
  gasnetc_get_fma_rdma_cutover = MIN(gasnetc_get_fma_rdma_cutover,
                                     GASNETC_GNI_FMA_RDMA_CUTOVER_MAX);

  gasnetc_put_fma_rdma_cutover = 
    gasneti_getenv_int_withdefault("GASNET_GNI_PUT_FMA_RDMA_CUTOVER",
				   GASNETC_GNI_PUT_FMA_RDMA_CUTOVER_DEFAULT,1);
  gasnetc_put_fma_rdma_cutover = MIN(gasnetc_put_fma_rdma_cutover,
                                     GASNETC_GNI_FMA_RDMA_CUTOVER_MAX);

  /* Protocol switch points: bounce buffer vs. registration */
  gasnetc_get_bounce_register_cutover = 
    gasneti_getenv_int_withdefault("GASNET_GNI_GET_BOUNCE_REGISTER_CUTOVER",
				   GASNETC_GNI_GET_BOUNCE_REGISTER_CUTOVER_DEFAULT,1);
  gasnetc_get_bounce_register_cutover = MIN(gasnetc_get_bounce_register_cutover,
                                            GASNETC_GNI_BOUNCE_REGISTER_CUTOVER_MAX);
  gasnetc_get_bounce_register_cutover = MIN(gasnetc_get_bounce_register_cutover,
                                            gasnetc_bounce_buffers.size);

  gasnetc_put_bounce_register_cutover = 
    gasneti_getenv_int_withdefault("GASNET_GNI_PUT_BOUNCE_REGISTER_CUTOVER",
				   GASNETC_GNI_PUT_BOUNCE_REGISTER_CUTOVER_DEFAULT,1);
  gasnetc_put_bounce_register_cutover = MIN(gasnetc_put_bounce_register_cutover,
                                            GASNETC_GNI_BOUNCE_REGISTER_CUTOVER_MAX);
  gasnetc_put_bounce_register_cutover = MIN(gasnetc_put_bounce_register_cutover,
                                            gasnetc_bounce_buffers.size);

  /* Derived limits used in extended API implementation: */
  gasnetc_max_get_unaligned = MAX(GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE,
                                  gasnetc_get_bounce_register_cutover);
#if GASNET_CONDUIT_GEMINI
  gasnetc_max_put_lc = MAX(gasnetc_put_fma_rdma_cutover,
                           gasnetc_put_bounce_register_cutover);
#else
  gasnetc_max_put_lc = MAX(GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE,
                           gasnetc_put_bounce_register_cutover);
#endif

  { int envval = gasneti_getenv_int_withdefault("GASNET_GNI_MEMREG", GASNETC_GNI_MEMREG_DEFAULT, 0);
    if (envval < 0) envval = 0;
    gasnetc_init_reg_credit(envval);
  }

  gasnetc_mem_consistency = GASNETC_DEFAULT_RDMA_MEM_CONSISTENCY;
  { char * envval = gasneti_getenv("GASNET_GNI_MEM_CONSISTENCY");
    if (!envval || !envval[0]) {
      /* No value given - keep default */
    } else if (!strcmp(envval, "strict") || !strcmp(envval, "STRICT")) {
      gasnetc_mem_consistency = GASNETC_STRICT_MEM_CONSISTENCY;
    } else if (!strcmp(envval, "relaxed") || !strcmp(envval, "RELAXED")) {
      gasnetc_mem_consistency = GASNETC_RELAXED_MEM_CONSISTENCY;
    } else if (!strcmp(envval, "default") || !strcmp(envval, "DEFAULT")) {
      gasnetc_mem_consistency = GASNETC_DEFAULT_MEM_CONSISTENCY;
    } else if (!gasneti_mynode) {
      fflush(NULL);
      fprintf(stderr, "WARNING: ignoring unknown value '%s' for environment "
                      "variable GASNET_GNI_MEM_CONSISTENCY\n", envval);
      fflush(NULL);
    }
  }

  switch (gasnetc_mem_consistency) {
    case GASNETC_STRICT_MEM_CONSISTENCY:
      gasnetc_memreg_flags = GNI_MEM_STRICT_PI_ORDERING;
      break;
    case GASNETC_RELAXED_MEM_CONSISTENCY:
      gasnetc_memreg_flags = GNI_MEM_RELAXED_PI_ORDERING;
      break;
    case GASNETC_DEFAULT_MEM_CONSISTENCY:
      gasnetc_memreg_flags = 0;
      break;
  }
  gasnetc_memreg_flags |= GNI_MEM_READWRITE;

#if FIX_HT_ORDERING
  if (gasnetc_mem_consistency != GASNETC_STRICT_MEM_CONSISTENCY) {
    gasnetc_memreg_flags |= GNI_MEM_PI_FLUSH; 
    gasnetc_fma_put_cq_mode |= GNI_CQMODE_REMOTE_EVENT;

    /* With 1 completion entry this queue is INTENDED to always overflow */
    status = GNI_CqCreate(nic_handle, 1, 0, GNI_CQ_NOBLOCK, NULL, NULL, &destination_cq_handle);
    gasneti_assert_always (status == GNI_RC_SUCCESS);
  }
#endif

  {
    int count = 0;
    for (;;) {
      status = GNI_MemRegister(nic_handle, (uint64_t) segment_start, 
			       (uint64_t) segment_size, destination_cq_handle,
			       gasnetc_memreg_flags, -1, 
			       &my_mem_handle);
      if (status == GNI_RC_SUCCESS) break;
      if (status == GNI_RC_ERROR_RESOURCE) {
	gasnetc_GNIT_Log("MemRegister segment fault %d at  %p %lx, code %s", 
		count, segment_start, segment_size, gni_return_string(status));
	count += 1;
	if (count >= 10) break;
      } else {
	break;
      }
    }
  }
			       
  assert (status == GNI_RC_SUCCESS);
  
  {
    gni_mem_handle_t *all_mem_handle = gasneti_malloc(gasneti_nodes * sizeof(gni_mem_handle_t));
    gasnet_node_t i;
    gasnetc_bootstrapExchange(&my_mem_handle, sizeof(gni_mem_handle_t), all_mem_handle);
    for (i = 0; i < gasneti_nodes; ++i) {
      peer_data[i].mem_handle = all_mem_handle[i];
    }
    gasneti_free(all_mem_handle);
  }
}

uintptr_t gasnetc_init_messaging(void)
{
  const gasnet_node_t remote_nodes = gasneti_nodes - (GASNET_PSHM ? gasneti_nodemap_local_count : 1);
  gni_return_t status;
  uint32_t *all_addr;
  uint32_t local_address;
  uint32_t i;
  unsigned int bytes_per_mbox;
  unsigned int bytes_needed;
  int modes = 0;

#if GASNET_DEBUG
  modes |= GNI_CDM_MODE_ERR_NO_KILL;
#endif

  GASNETC_INITLOCK_GNI();

  status = GNI_CdmCreate(gasneti_mynode,
			 gasnetc_ptag, gasnetc_cookie,
			 modes,
			 &cdm_handle);
  gasneti_assert_always (status == GNI_RC_SUCCESS);

  status = GNI_CdmAttach(cdm_handle,
			 gasnetc_dev_id,
			 &local_address,
			 &nic_handle);
  gasneti_assert_always (status == GNI_RC_SUCCESS);

  { /* Determine credits for AMs: GASNET_NETWORKDEPTH */
    int depth = gasneti_getenv_int_withdefault("GASNET_NETWORKDEPTH",
                                               GASNETC_NETWORKDEPTH_DEFAULT, 0);
    am_maxcredit = MAX(1,depth); /* Min is 1 */
    mb_slots = 2 * am_maxcredit; /* (req + reply) = 2 */
    bank_credits = (depth > 1) ? 1 : 0;
  }

  { /* Determine Cq size: GASNET_GNI_NUM_PD */
    int num_pd, cq_entries;
    num_pd = gasneti_getenv_int_withdefault("GASNET_GNI_NUM_PD",
                                            GASNETC_GNI_NUM_PD_DEFAULT,1);

    cq_entries = num_pd+2; /* XXX: why +2 ?? */

    status = GNI_CqCreate(nic_handle, cq_entries, 0, GNI_CQ_NOBLOCK, NULL, NULL, &bound_cq_handle);
    gasneti_assert_always (status == GNI_RC_SUCCESS);
  }

  /* create and bind endpoints */
  all_addr = gather_nic_addresses();
  peer_data = gasneti_malloc(gasneti_nodes * sizeof(peer_struct_t));
  for (i = 0; i < gasneti_nodes; i += 1) {
    if (node_is_local(i)) continue; /* no connection to self or PSHM-reachable peers */
    status = GNI_EpCreate(nic_handle, bound_cq_handle, &peer_data[i].ep_handle);
    gasneti_assert_always (status == GNI_RC_SUCCESS);
    status = GNI_EpBind(peer_data[i].ep_handle, all_addr[i], i);
    gasneti_assert_always (status == GNI_RC_SUCCESS);
  }
  gasneti_free(all_addr);

  /* Initialize the short message system */

  /* log2_remote = MAX(1, ceil(log_2(remote_nodes))) */
  log2_remote = 1;
  for (i=2; i < remote_nodes; i*=2) {
    log2_remote += 1;
  }

  /*
   * allocate a CQ in which to receive message notifications
   * include logarithmic space for shutdown messaging
   */
  i = log2_remote + remote_nodes*mb_slots;
  status = GNI_CqCreate(nic_handle,i,0,GNI_CQ_NOBLOCK,NULL,NULL,&smsg_cq_handle);
  if (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort("GNI_CqCreate returned error %s", gni_return_string(status));
  }
  
  /*
   * Set up an mmap region to contain all of my mailboxes.
   */

  bytes_per_mbox = mb_slots * sizeof(gasnetc_mailbox_t);

  /* TODO: remove MAX(1,) while still avoiding "issues" on single-(super)node runs */
  bytes_needed = MAX(1,remote_nodes) * bytes_per_mbox;
  
  smsg_mmap_ptr = gasneti_huge_mmap(NULL, bytes_needed);
  smsg_mmap_bytes = bytes_needed;
  if (smsg_mmap_ptr == (char *)MAP_FAILED) {
    gasnetc_GNIT_Abort("hugepage mmap failed: ");
  }
  
  {
    int count = 0;
    for (;;) {
      status = GNI_MemRegister(nic_handle, 
			       (unsigned long)smsg_mmap_ptr, 
			       bytes_needed,
			       smsg_cq_handle,
			       GNI_MEM_STRICT_PI_ORDERING | GNI_MEM_PI_FLUSH | GNI_MEM_READWRITE,
			       -1,
			       &my_smsg_handle);
      if (status == GNI_RC_SUCCESS) break;
      if (status == GNI_RC_ERROR_RESOURCE) {
	gasnetc_GNIT_Log("MemRegister smsg fault %d at  %p %x, code %s", 
		count, smsg_mmap_ptr, bytes_needed, gni_return_string(status));
	count += 1;
	if (count >= 10) break;
      } else {
	break;
      }
    }
  }
  if (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort("GNI_MemRegister returned error %s",gni_return_string(status));
  }

  /* exchange peer data and initialize smsg */
  { struct smsg_exchange { uint8_t *addr; gni_mem_handle_t handle; };
    struct smsg_exchange my_smsg_exchg = { smsg_mmap_ptr, my_smsg_handle };
    struct smsg_exchange *all_smsg_exchg = gasneti_malloc(gasneti_nodes * sizeof(struct smsg_exchange));
    uint8_t *local_buffer = smsg_mmap_ptr;

    gasnetc_bootstrapExchange(&my_smsg_exchg, sizeof(struct smsg_exchange), all_smsg_exchg);
  
    /* At this point all_smsg_exchg has the required information for everyone */
    for (i = 0; i < gasneti_nodes; i += 1) {
      if (node_is_local(i)) continue; /* no connection to self or PSHM-reachable peers */

      peer_data[i].mb.loc_addr = (gasnetc_mailbox_t*) local_buffer;
      peer_data[i].mb.rem_addr = (gasnetc_mailbox_t*) all_smsg_exchg[i].addr + (mb_slots * my_smsg_index(i));
      peer_data[i].mb.rem_hndl = all_smsg_exchg[i].handle;
      peer_data[i].mb.recv_pos = mb_slots;
      peer_data[i].mb.send_pos = mb_slots;
      gasneti_weakatomic_set(&peer_data[i].am_credit, am_maxcredit, 0);
      gasneti_weakatomic_set(&peer_data[i].am_credit_bank, 0, 0);
      local_buffer += bytes_per_mbox;
    }

    gasneti_free(all_smsg_exchg);
  }

  /* Create a temporary pool of post descriptors for uses prior to the aux seg attach.
   * NOTE: The immediate buffer WON'T be in registered memory - so no Put/Get.
   */
  for (i=0; i < log2_remote; ++i) {
    /* allocate individually to ease later destruction of this pool. */
    gasnetc_post_descriptor_t *gpd = gasneti_calloc(1, sizeof(gasnetc_post_descriptor_t));
    gasneti_lifo_push(&post_descriptor_pool, gpd);
  }

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

  /* for each connected rank */
  left = gasneti_nodes - (GASNET_PSHM ? gasneti_nodemap_local_count : 1);
  for (tries=0; tries<10; ++tries) {
    for (i = 0; i < gasneti_nodes; i += 1) {
      if (node_is_local(i)) continue; /* no connection to self or PSHM-reachable peers */
      if (peer_data[i].ep_handle != NULL) {
	status = GNI_EpUnbind(peer_data[i].ep_handle);
	status = GNI_EpDestroy(peer_data[i].ep_handle);
	if (status == GNI_RC_SUCCESS) {
	  peer_data[i].ep_handle = NULL;
	  left -= 1;
	}
      }
    }
    if (!left) break;
#if 0
    GASNETI_WAITHOOK();
    gasnetc_poll_local_queue();
#endif
  }
  if (left > 0) {
    gasnetc_GNIT_Log("at shutdown: %d endpoints left after 10 tries", left);
  }

  if (gasneti_attach_done) {
    status = GNI_MemDeregister(nic_handle, &my_mem_handle);
    if_pf (status != GNI_RC_SUCCESS) {
      gasnetc_GNIT_Abort("MemDeregister(segment) failed with %s", gni_return_string(status));
    }
  }

  gasneti_huge_munmap(smsg_mmap_ptr, smsg_mmap_bytes);

  status = GNI_MemDeregister(nic_handle, &my_smsg_handle);
  if_pf (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort("MemDeregister(smsg_mem) failed with %s", gni_return_string(status));
  }

  if (destination_cq_handle) {
    status = GNI_CqDestroy(destination_cq_handle);
    if_pf (status != GNI_RC_SUCCESS) {
      gasnetc_GNIT_Abort("CqDestroy(dest_cq) failed with %s", gni_return_string(status));
    }
  }

  status = GNI_CqDestroy(smsg_cq_handle);
  if_pf (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort("CqDestroy(smsg_cq) failed with %s", gni_return_string(status));
  }

  status = GNI_CqDestroy(bound_cq_handle);
  if_pf (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort("CqDestroy(bound_cq) failed with %s", gni_return_string(status));
  }

  status = GNI_CdmDestroy(cdm_handle);
  if_pf (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort("CdmDestroy(bound_cq) failed with %s", gni_return_string(status));
  }
}


GASNETI_INLINE(recv_credits)
void recv_credits(peer_struct_t *peer, int n) {
  GASNETI_UNUSED_UNLESS_DEBUG int newval = 
    gasneti_weakatomic_add(&peer->am_credit, n, GASNETI_ATOMIC_NONE);
  gasneti_assert(newval <= am_maxcredit);
}

static void gasnetc_handle_sys_shutdown_packet(uint32_t source, uint16_t arg);
static void gasnetc_send_credit(uint32_t pe);

static
void gasnetc_recv_am(gasnet_node_t pe, gasnetc_mailbox_t * const mb, const GC_Header_t header)
{
  peer_struct_t * const peer = &peer_data[pe];

  {
      const int numargs = header.numargs;
      const int misc = header.misc;
      const int is_req = header.is_req;
      const int handlerindex = header.handler;
      gasneti_handler_fn_t handler = gasnetc_handler[handlerindex];

      gasnetc_token_t the_token = { pe, is_req };
      gasnet_token_t token = (gasnet_token_t)&the_token; /* RUN macros need an lvalue */

      /* NOTE: Mailbox is already marked free.
       * Peer cannot possibly consume it until we send back a credit (Request case)
       * or we send a Request using a newly acquired credit (Reply case).
       * We've copied out mb->packet.header already and args are read before calling
       * the handler.  So, if the handler does Reply (and thus return a credit) to a
       * peer, these fields are safe to over-write.  Only the payload of a Medium
       * Request needs to be copied out of the mailbox before its handler can run.
       */

      gasneti_assert(numargs <= gasnet_AMMaxArgs());
      GASNETI_TRACE_PRINTF(D, ("smsg r from %d type %s_%s%s\n", pe,
                               gasnetc_type_string(header.command),
                               is_req ? "REQUEST" : "REPLY",
                               header.credit ? " (+credit)" : ""));

      switch (header.command) {
      case GC_CMD_AM_SHORT:
        GASNETI_RUN_HANDLER_SHORT(is_req, handlerindex, handler,
                                  token, mb->packet.gasp.args, numargs);
	break;

      case GC_CMD_AM_MEDIUM: {
        uint8_t buffer[gasnet_AMMaxMedium()];
        const size_t head_len = GASNETC_HEADLEN(medium, numargs);
        uint8_t * data = &mb->raw[head_len];
        if (is_req) { /* Req cannot run with payload in-place */
          /* TODO: special case for non-replying requests (internal only for now) */
          data = memcpy(&buffer, data, misc);
        }
        gasneti_assert(0 == (((uintptr_t) data) % GASNETI_MEDBUF_ALIGNMENT));
        gasneti_assert(misc <= gasnet_AMMaxMedium());
        GASNETI_RUN_HANDLER_MEDIUM(is_req, handlerindex, handler,
                                   token, mb->packet.gamp.args, numargs,
                                   data, misc);
	break;
      }

      case GC_CMD_AM_LONG:
        if (misc) { /* payload follows header - copy it into place */
          const size_t head_len = GASNETC_HEADLEN(long, numargs);
          gasneti_assert(mb->packet.galp.data_length <= GASNETC_MAX_PACKED_LONG(numargs));
          memcpy(mb->packet.galp.data, &mb->raw[head_len], mb->packet.galp.data_length);
        }
        GASNETI_RUN_HANDLER_LONG(is_req, handlerindex, handler,
        		         token, mb->packet.galp.args, numargs,
        		         mb->packet.galp.data, mb->packet.galp.data_length);
	break;

#if GASNET_DEBUG
      default:
	gasnetc_GNIT_Abort("unknown packet type");
#endif
      }
      if (the_token.need_reply) {
        gasnetc_send_credit(pe);
      }
      { const int credits = header.credit + !is_req;
        if (credits) recv_credits(peer, credits);
      }
  }
}


/* Max number of times to poll the AM mailboxes per entry */
/* TODO: control via env var (requires dynamic allocation of queue) */
#define SMSG_BURST 20

static
void gasnetc_poll_smsg_queue(void)
{
  /* FIFO queue of sources for which we have reaped a Cq entry,
   * but have not yet completed the corresponding receive.
   * A source may potentially appear multiple time.
   * The +1 in sizing prevents head==tail and its full/empty ambiguity
   */
  static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;
  static gasnet_node_t queue[SMSG_BURST+1];
  static int head = SMSG_BURST; /* Runs backward over range [0:SMSG_BURST] */
  static int tail = SMSG_BURST; /* Runs backward over range [0:SMSG_BURST] */
  static int free_space = SMSG_BURST;

  int i;

  gasneti_mutex_lock(&lock);

  /* Reap Cq entries until our queue is full, or Cq is empty */
  while (free_space) {
    gni_cq_entry_t event_data[SMSG_BURST];
    gni_return_t status;
    int count;

    GASNETC_LOCK_GNI();
    for (count = 0; count < free_space; ++count) {
      status = GNI_CqGetEvent(smsg_cq_handle, &event_data[count]);
      if (status != GNI_RC_SUCCESS) break; /* TODO: check for fatal errors */
    }
    GASNETC_UNLOCK_GNI();
    if (!count) break;

    for (i = 0; i < count; ++i) {
      uint32_t source = GNI_CQ_GET_INST_ID(event_data[i]);
      gasneti_assert(0 == ((GASNET_MAXNODES - 1) & (source ^ GNI_CQ_GET_DATA(event_data[i]))));

      if (source & GASNET_MAXNODES) { /* Control message */
        const uint32_t control = GNI_CQ_GET_DATA(event_data[i]) >> 24;
        const uint16_t     arg = control >> 8;
        const uint8_t       op = control;
        source &= (GASNET_MAXNODES - 1);
        gasneti_assert((source < gasneti_nodes) && !node_is_local(source));

        switch (op) {
        case GC_CTRL_CREDIT:
          gasneti_assert((arg == 1) || (arg == 2));
          recv_credits(&peer_data[source], arg);
          break;

        case GC_CTRL_SHUTDOWN:
	  gasnetc_handle_sys_shutdown_packet(source, arg);
          break;

#if GASNET_DEBUG
        default:
	  gasnetc_GNIT_Abort("unknown control message %d", (int)op);
#endif
        }
      } else {
        gasneti_assert((source < gasneti_nodes) && !node_is_local(source));
        queue[tail] = source;
        tail = tail ? (tail-1) : SMSG_BURST;
        free_space -= 1;
      }
    }
  }

  /* Poll all "live" sources, starting with any that were not ready last time */
  if (head != tail) {
    for (i = 0; i < SMSG_BURST; ++i) {
      /* dequeue one source and poll it, then we either run the
         handler (w/o lock held) or requeue the source for later service */
      const gasnet_node_t source = queue[head];
      peer_struct_t * const peer = &peer_data[source];
      unsigned int slot = peer->mb.recv_pos - 1;
      gasnetc_mailbox_t * const mb = &peer->mb.loc_addr[slot];
      GC_Header_t header;

      head = head ? (head-1) : SMSG_BURST;

      if (mb->full) { /* First word is zero until mailbox is filled */
        gasneti_local_rmb(); /* Acquire */
        header = mb->packet.header; /* before over-written via 'full' */
        mb->full = 0;
        peer->mb.recv_pos = slot ? slot : mb_slots;
        ++free_space;
        gasneti_mutex_unlock(&lock);
          gasnetc_recv_am(source, mb, header);
        gasneti_mutex_lock(&lock);
      } else {
        queue[tail] = source;
        tail = tail ? (tail-1) : SMSG_BURST;
      }

      if (head == tail) break; /* queue is empty */
    }
  }

  gasneti_mutex_unlock(&lock);
}

extern int
gasnetc_send_smsg(gasnet_node_t dest, gasnetc_post_descriptor_t *gpd,
                  gasnetc_packet_t *msg, size_t length)
{
  peer_struct_t * const peer = &peer_data[dest];
  gni_return_t status;
  const int max_trials = 4;
  int trial = 0;

  gasnetc_mailbox_t * mb;
  gni_post_descriptor_t * pd;
  const uint64_t *buffer;

  gasneti_assert(length >= 8);
  gasneti_assert(length <= GASNETC_MSG_MAXSIZE);

  gasneti_assert(!node_is_local(dest));

  msg->header.credit = gasnetc_weakatomic_swap(&peer->am_credit_bank, 0);

  GASNETI_TRACE_PRINTF(D, ("smsg to %d type %s_%s%s\n", dest,
                           gasnetc_type_string(msg->header.command),
                           msg->header.is_req ? "REQUEST" : "REPLY",
                           msg->header.credit ? " (+credit)" : ""));

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd = &gpd->pd;
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT | GNI_CQMODE_REMOTE_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->type = GNI_POST_FMA_PUT_W_SYNCFLAG;

  /* First word will be written to the remote sync flag, while rest is the Put payload */
  buffer = (uint64_t *)msg;
  pd->sync_flag_value = buffer[0];
  pd->length = length - 8;
  pd->local_addr = (uint64_t) &buffer[1];
  pd->remote_mem_hndl = peer->mb.rem_hndl;

  GASNETC_LOCK_GNI();
  { unsigned int slot = peer->mb.send_pos - 1;
    mb = &peer->mb.rem_addr[slot];
    peer->mb.send_pos = slot ? slot : mb_slots;
  }

  pd->sync_flag_addr = (uint64_t) mb;
  pd->remote_addr = (uint64_t) mb + 8;
  for (;;) {
    status = GNI_PostFma(peer->ep_handle, pd);

    GASNETC_UNLOCK_GNI();
    if_pt (status == GNI_RC_SUCCESS) {
      if_pf (trial) GASNETC_STAT_EVENT_VAL(SMSG_SEND_RETRY, trial);
      return GNI_RC_SUCCESS;
    }

    if (status != GNI_RC_ERROR_RESOURCE) {
      gasnetc_GNIT_Abort("PostFma for AM returned error %s", gni_return_string(status));
    }

    if_pf (++trial == max_trials) {
      return GASNET_ERR_RESOURCE;
    }

    GASNETI_WAITHOOK();
    gasnetc_poll_local_queue();
    GASNETC_LOCK_GNI();
  }

  return GASNET_OK;
}


/* Poll the bound_ep completion queue */
GASNETI_INLINE(gasnetc_poll_bound_cq)
gasnetc_post_descriptor_t *gasnetc_poll_bound_cq(void)
{
  gni_post_descriptor_t * result = NULL;
  gni_cq_entry_t event_data;
  gni_return_t status;

  GASNETC_LOCK_GNI();
  status = GNI_CqGetEvent(bound_cq_handle,&event_data);
  if (status == GNI_RC_NOT_DONE) { /* empty queue is most common case */
    /* nothing */
  } else if_pt (status == GNI_RC_SUCCESS) {
    gasneti_assert(!GNI_CQ_OVERRUN(event_data));

    status = GNI_GetCompleted(bound_cq_handle, event_data, &result);
    if_pf (status != GNI_RC_SUCCESS) {
      gasnetc_GNIT_Abort("GetCompleted(%p) failed %s",
                         (void *) event_data, gni_return_string(status));
    }
  } else if (!gasnetc_shutdownInProgress) {
    gasnetc_GNIT_Abort("bound CqGetEvent %s", gni_return_string(status));
  }
  GASNETC_UNLOCK_GNI();

  /* NOTE: we rely here on the fact that pd is first member of gpd */
  return (gasnetc_post_descriptor_t *) result;
}

GASNETI_NEVER_INLINE(gasnetc_poll_local_queue,
void gasnetc_poll_local_queue(void))
{
  int i;

  for (i = 0; i < gasnetc_poll_burst; i += 1) {
    gasnetc_post_descriptor_t * const gpd = gasnetc_poll_bound_cq();
    
    if_pt (! gpd) { /* empty Cq is common case */
      break;
    } else {
      const uint32_t flags = gpd->flags; /* see note w/ GC_POST_COMPLETION_FLAG */

      /* handle remaining work */
      if (flags & GC_POST_COPY) {
        const size_t length = gpd->pd.length - (flags & GC_POST_COPY_TRIM);
        memcpy((void *) gpd->gpd_get_dst, (void *) gpd->gpd_get_src, length);
        gasneti_sync_writes(); /* sync memcpy */
      }

      /* indicate completion */
      if (flags & GC_POST_COMPLETION_FLAG) {
        *(volatile int *) gpd->gpd_completion = 1;
        /* NOTE: if (flags & GC_POST_KEEP_GPD) then caller might free gpd now */
      } else if(flags & GC_POST_COMPLETION_OP) {
        gasnete_op_markdone((gasnete_op_t *) gpd->gpd_completion, (flags & GC_POST_GET));
      }

      /* release resources */
      if (flags & GC_POST_UNREGISTER) {
	gasnetc_deregister_pd(&gpd->pd);
      } else if (flags & GC_POST_UNBOUNCE) {
	gasnetc_free_bounce_buffer((void *) gpd->pd.local_addr);
      }
    #if !GASNET_CONDUIT_GEMINI
      else if (flags & GC_POST_SMSG_BUF) {
        gasneti_lifo_push(&gasnetc_smsg_buffers, (void *) (gpd->pd.local_addr - 8));
      }
    #endif

      if (flags & GC_POST_SEND) {
        /* repost this gpd, which already carries the msg in place */
        gasnetc_packet_t * const msg = &gpd->u.packet;
        int rc;
        gpd->flags = 0;
        rc = gasnetc_send_smsg(gpd->dest, gpd, msg,
                               GASNETC_HEADLEN(long, msg->header.numargs));
        gasneti_assert_always (rc == GASNET_OK);
      } else
      if (!(flags & GC_POST_KEEP_GPD)) {
        gasnetc_free_post_descriptor(gpd);
      }
    }
  }
}
  
void gasnetc_poll(void)
{
  gasnetc_poll_smsg_queue();
  gasnetc_poll_local_queue();
}

/* Send a 3-byte control message */
/* Current ARBITRARILY managed as 8-bit op and 16-bit arg */
void gasnetc_send_control(uint32_t dest, uint8_t op, uint16_t arg)
{
  peer_struct_t * const peer = &peer_data[dest];
  gasnetc_post_descriptor_t * const gpd = gasnetc_alloc_post_descriptor();
  gni_return_t status;
  const int max_trials = 4;
  int trial = 0;

  /*  bzero(&gpd->pd, sizeof(gni_post_descriptor_t)); */
  gpd->flags = 0;
  gpd->pd.cq_mode = GNI_CQMODE_GLOBAL_EVENT | GNI_CQMODE_REMOTE_EVENT;
  gpd->pd.dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  gpd->pd.type = GNI_POST_CQWRITE;
  gpd->pd.remote_mem_hndl = peer->mb.rem_hndl;

  gpd->pd.cqwrite_value = ((uint64_t) arg << 32)
                        | ((uint64_t) op  << 24)
                        | GASNET_MAXNODES | gasneti_mynode;

  for (;;) {
    GASNETC_LOCK_GNI();
    status = GNI_PostCqWrite(peer->ep_handle, &gpd->pd);
    GASNETC_UNLOCK_GNI();
    if_pt (status == GNI_RC_SUCCESS) return;
    if (status != GNI_RC_ERROR_RESOURCE) break; /* Fatal */
    GASNETI_WAITHOOK();
    gasnetc_poll_local_queue();
  } while (++trial < max_trials);
  if (status == GNI_RC_ERROR_RESOURCE) {
    gasnetc_GNIT_Abort("PostCqWrite retry failed");
  }
}

GASNETI_INLINE(gasnetc_send_credit)
void gasnetc_send_credit(uint32_t pe)
{
  uint32_t credits_to_send = 1;

  /* bank_credits = 0: all calls result in a single credit sent
   * bank_credits = 1:
   *    bank = 0: deposit one and send none
   *    bank = 1: withdraw one and send two
   */
  if (bank_credits) {
    gasneti_weakatomic_val_t oldval = (1 ^ gasneti_weakatomic_add(&peer_data[pe].am_credit_bank, 1, 0));
    credits_to_send = (oldval & 1) << 1;
  }

  if (credits_to_send) {
    gasneti_assert((credits_to_send == 1) || (credits_to_send == 2));
    gasnetc_send_control(pe, GC_CTRL_CREDIT, credits_to_send);
  }
}

GASNETI_NEVER_INLINE(print_post_desc,
static void print_post_desc(const char *title, gni_post_descriptor_t *cmd)) {
  const int in_seg = gasneti_in_segment(gasneti_mynode, (void *) cmd->local_addr, cmd->length);
  printf("r %d %s-segment %s, desc addr %p\n", gasneti_mynode, (in_seg?"in":"non"), title, cmd);
  printf("r %d status: %ld\n", gasneti_mynode, cmd->status);
  printf("r %d cq_mode_complete: 0x%x\n", gasneti_mynode, cmd->cq_mode_complete);
  printf("r %d type: %d (%s)\n", gasneti_mynode, cmd->type, gasnetc_post_type_string(cmd->type));
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

static gni_return_t myPostRdma(gni_ep_handle_t ep, gni_post_descriptor_t *pd)
{
  gni_return_t status;
  const int max_trials = 1000;
  int trial = 0;

  do {
      GASNETC_LOCK_GNI();
      status = GNI_PostRdma(ep, pd);
      GASNETC_UNLOCK_GNI();
      if_pt (status == GNI_RC_SUCCESS) {
        if (trial) GASNETC_STAT_EVENT_VAL(POST_RDMA_RETRY, trial);
        return GNI_RC_SUCCESS;
      }
      if (status != GNI_RC_ERROR_RESOURCE) break; /* Fatal */
      GASNETI_WAITHOOK();
      gasnetc_poll_local_queue();
  } while (++trial < max_trials);
  if (status == GNI_RC_ERROR_RESOURCE) {
    gasnetc_GNIT_Log("PostRdma retry failed");
  }
  return status;
}

static gni_return_t myPostFma(gni_ep_handle_t ep, gni_post_descriptor_t *pd)
{
  gni_return_t status;
  const int max_trials = 1000;
  int trial = 0;

  do {
      GASNETC_LOCK_GNI();
      status = GNI_PostFma(ep, pd);
      GASNETC_UNLOCK_GNI();
      if_pt (status == GNI_RC_SUCCESS) {
        if (trial) GASNETC_STAT_EVENT_VAL(POST_FMA_RETRY, trial);
        return GNI_RC_SUCCESS;
      }
      if (status != GNI_RC_ERROR_RESOURCE) break; /* Fatal */
      GASNETI_WAITHOOK();
      gasnetc_poll_local_queue();
  } while (++trial < max_trials);
  if (status == GNI_RC_ERROR_RESOURCE) {
    gasnetc_GNIT_Log("PostFma retry failed");
  }
  return status;
}

/* Perform an rdma/fma Put with no concern for local completion */
void gasnetc_rdma_put_bulk(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  peer_struct_t * const peer = &peer_data[node];
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;

  gasneti_assert(!node_is_local(node));

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) dest_addr;
  pd->remote_mem_hndl = peer->mem_handle;
  pd->length = nbytes;

  /* confirm that the destination is in-segment on the far end */
  gasneti_boundscheck(node, dest_addr, nbytes);

  /* Start with defaults suitable for FMA or in-segment case */
  pd->local_addr = (uint64_t) source_addr;
  pd->local_mem_hndl = my_mem_handle;

  if (nbytes <= gasnetc_put_fma_rdma_cutover) {
    /* Small enough for FMA - no local memory registration is required */
    pd->type = GNI_POST_FMA_PUT;
#if FIX_HT_ORDERING
    pd->cq_mode = gasnetc_fma_put_cq_mode;
#endif
    status = myPostFma(peer->ep_handle, pd);
  } else { /* Using RDMA, which requires local memory registration */
    if_pf (!gasneti_in_segment(gasneti_mynode, source_addr, nbytes)) {
      /* Use a bounce buffer or mem-reg according to size.
       * Use of gpd->u.immedate would only be reachable if
       *     (put_fma_rdma_cutover < IMMEDIATE_BOUNCE_SIZE),
       * which is not the default (nor recommended).
       */
      if (nbytes <= gasnetc_put_bounce_register_cutover) {
        void * const buffer = gasnetc_alloc_bounce_buffer();
        pd->local_addr = (uint64_t) memcpy(buffer, source_addr, nbytes);
        gpd->flags |= GC_POST_UNBOUNCE;
      } else {
        gpd->flags |= GC_POST_UNREGISTER;
        gasnetc_register_pd(pd);
      }
    }
    pd->type = GNI_POST_RDMA_PUT;
    status = myPostRdma(peer->ep_handle, pd);
  }

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Put", pd);
    gasnetc_GNIT_Abort("Put failed with %s", gni_return_string(status));
  }
}

/* Perform an rdma/fma Put which favors rapid local completion
 * The return value is boolean, where 1 means locally complete.
 * NOTE: be sure to update gasnetc_max_put_lc if the logic here changes
 */
int gasnetc_rdma_put(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  peer_struct_t * const peer = &peer_data[node];
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;
  int result = 1; /* assume local completion */

  gasneti_assert(!node_is_local(node));

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) dest_addr;
  pd->remote_mem_hndl = peer->mem_handle;
  pd->length = nbytes;

  /* confirm that the destination is in-segment on the far end */
  gasneti_boundscheck(node, dest_addr, nbytes);

  /* Start with defaults suitable for FMA or in-segment case */
  pd->local_addr = (uint64_t) source_addr;
  pd->local_mem_hndl = my_mem_handle;

#if GASNET_CONDUIT_GEMINI
  /* On Gemini (only) return from PostFma follows local completion. */
  if (nbytes <= gasnetc_put_fma_rdma_cutover) {
    /* Small enough for FMA - no local memory registration is required */
    pd->type = GNI_POST_FMA_PUT;
  #if FIX_HT_ORDERING
    pd->cq_mode = gasnetc_fma_put_cq_mode;
  #endif
    status = myPostFma(peer->ep_handle, pd);
  } else
#endif
  { /* Favor bounce buffers if possible */
  #if !GASNET_CONDUIT_GEMINI /* On Gemini the FMA path above would be selected instead */
    if (nbytes <= GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE) {
      void * const buffer = gpd->u.immediate;
      pd->local_addr = (uint64_t) memcpy(buffer, source_addr, nbytes);
    } else
  #endif
    if (nbytes <= gasnetc_put_bounce_register_cutover) {
      void * const buffer = gasnetc_alloc_bounce_buffer();
      pd->local_addr = (uint64_t) memcpy(buffer, source_addr, nbytes);
      gpd->flags |= GC_POST_UNBOUNCE;
    } else {
      result = 0;
      if_pf (!gasneti_in_segment(gasneti_mynode, source_addr, nbytes)) {
        /* source not in segment */
        gpd->flags |= GC_POST_UNREGISTER;
        gasnetc_register_pd(pd);
      }
    }
 
#if !GASNET_CONDUIT_GEMINI
    if (nbytes <= gasnetc_put_fma_rdma_cutover) {
      pd->type = GNI_POST_FMA_PUT;
    #if FIX_HT_ORDERING
      pd->cq_mode = gasnetc_fma_put_cq_mode;
    #endif
      status = myPostFma(peer->ep_handle, pd);
    } else
#endif
    {
      pd->type = GNI_POST_RDMA_PUT;
      status = myPostRdma(peer->ep_handle, pd);
    }
  }

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Put", pd);
    gasnetc_GNIT_Abort("Put failed with %s", gni_return_string(status));
  }

  gasneti_assert((result == 0) || (result == 1)); /* ensures caller can use "&=" or "+=" */
  return result;
}

/* FMA Put from a specified buffer */
void gasnetc_rdma_put_buff(gasnet_node_t node,
		void *dest_addr, void *source_addr,
		size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  peer_struct_t * const peer = &peer_data[node];
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;

  gasneti_assert(!node_is_local(node));

  /* confirm that the destination is in-segment on the far end */
  gasneti_boundscheck(node, dest_addr, nbytes);

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) dest_addr;
  pd->remote_mem_hndl = peer->mem_handle;
  pd->length = nbytes;
  pd->local_addr = (uint64_t) source_addr;

  /* now initiate always using FMA */
  pd->type = GNI_POST_FMA_PUT;
#if FIX_HT_ORDERING
  pd->cq_mode = gasnetc_fma_put_cq_mode;
#endif
  status = myPostFma(peer->ep_handle, pd);

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Put", pd);
    gasnetc_GNIT_Abort("Put failed with %s", gni_return_string(status));
  }
}

/* initiate a Get according to fma/rdma cutover */
GASNETI_INLINE(gasnetc_post_get)
void gasnetc_post_get(gni_ep_handle_t ep, gni_post_descriptor_t *pd)
{
  gni_return_t status;
  const size_t nbytes = pd->length;

  if (nbytes <= gasnetc_get_fma_rdma_cutover) {
      pd->type = GNI_POST_FMA_GET;
      status = myPostFma(ep, pd);
  } else {
      pd->type = GNI_POST_RDMA_GET;
      status = myPostRdma(ep, pd);
  }

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Get", pd);
    gasnetc_GNIT_Abort("Get failed with %s", gni_return_string(status));
  }
}

/* for get, source_addr is remote */
void gasnetc_rdma_get(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  peer_struct_t * const peer = &peer_data[node];
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;

  gasneti_assert(!node_is_local(node));

  gpd->flags |= GC_POST_GET;

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) source_addr;
  pd->remote_mem_hndl = peer->mem_handle;
  pd->length = nbytes;

  /* confirm that the source is in-segment on the far end */
  gasneti_boundscheck(node, source_addr, nbytes);

  /* Start with defaults suitable for in-segment case */
  pd->local_addr = (uint64_t) dest_addr;
  pd->local_mem_hndl = my_mem_handle;

  /* check where the local addr is */
  if_pf (!gasneti_in_segment(gasneti_mynode, dest_addr, nbytes)) {
    /* dest not (entirely) in segment */
    /* if (nbytes <= gasnetc_get_bounce_register_cutover)  then use bounce buffer
     * else mem-register
     */
    if (nbytes < GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE) {
      gpd->flags |= GC_POST_COPY;
      gpd->gpd_get_src = pd->local_addr = (uint64_t) gpd->u.immediate;
      gpd->gpd_get_dst = (uint64_t) dest_addr;
    } else if (nbytes <= gasnetc_get_bounce_register_cutover) {
      gpd->flags |= GC_POST_UNBOUNCE | GC_POST_COPY;
      gpd->gpd_get_src = pd->local_addr = (uint64_t) gasnetc_alloc_bounce_buffer();
      gpd->gpd_get_dst = (uint64_t) dest_addr;
    } else {
      gpd->flags |= GC_POST_UNREGISTER;
      gasnetc_register_pd(pd);
    }
  }

  gasnetc_post_get(peer->ep_handle, pd);
}

/* for get in which one or more of dest_addr, source_addr or nbytes is NOT divisible by 4
 * NOTE: be sure to update gasnetc_max_get_unaligned if the logic here changes
 */
void gasnetc_rdma_get_unaligned(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  peer_struct_t * const peer = &peer_data[node];
  gni_post_descriptor_t * const pd = &gpd->pd;
  uint8_t * buffer;

  /* Compute length of "overfetch" required, if any */
  unsigned int pre = (uintptr_t) source_addr & 3;
  size_t       length = GASNETI_ALIGNUP(nbytes + pre, 4);
  unsigned int overfetch = length - nbytes;

  gasneti_assert(!node_is_local(node));

  gasneti_assert(0 == (overfetch & ~GC_POST_COPY_TRIM));
  gpd->flags |= GC_POST_GET | GC_POST_COPY | overfetch;

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) source_addr - pre;
  pd->remote_mem_hndl = peer->mem_handle;
  pd->length = length;
  pd->local_mem_hndl = my_mem_handle;

  /* confirm that the source is in-segment on the far end */
  gasneti_boundscheck(node, (void*)pd->remote_addr, pd->length);

  /* must always use immediate or bounce buffer */
  if (length < GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE) {
    buffer = gpd->u.immediate;
  } else if_pt (length <= gasnetc_get_bounce_register_cutover) {
    gpd->flags |= GC_POST_UNBOUNCE;
    buffer = gasnetc_alloc_bounce_buffer();
  } else {
    gasneti_fatalerror("get_unaligned called with nbytes too large for bounce buffers");
  }

  pd->local_addr = (uint64_t) buffer;
  gpd->gpd_get_src = (uint64_t) buffer + pre;
  gpd->gpd_get_dst = (uint64_t) dest_addr;

  gasnetc_post_get(peer->ep_handle, pd);
}

/* Get into a specified in-full-segment buffer
   Will overfetch if source_addr or nbytes are not 4-byte aligned
   but expects (does not check) that the buffer is aligned.
   Caller must be allow for space (upto 6 bytes) for the overfetch.
   Returns offset to start of data after adjustment for overfetch
 */
int gasnetc_rdma_get_buff(gasnet_node_t node,
		void *dest_addr, void *source_addr,
		size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  peer_struct_t * const peer = &peer_data[node];
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;

  /* Compute length of "overfetch" required, if any */
  unsigned int pre = (uintptr_t) source_addr & 3;
  size_t       length = GASNETI_ALIGNUP(nbytes + pre, 4);

  gasneti_assert(!node_is_local(node));
  gasneti_assert(nbytes  <= GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE);

  /* confirm that the source is in-segment on the far end */
  gasneti_boundscheck(node, source_addr, nbytes);

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) source_addr - pre;
  pd->remote_mem_hndl = peer->mem_handle;
  pd->length = length;
  pd->local_addr = (uint64_t) dest_addr;
  pd->local_mem_hndl = my_mem_handle;

  /* now initiate - *always* FMA for now */
  pd->type = GNI_POST_FMA_GET;
  status = myPostFma(peer->ep_handle, pd);

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Get", pd);
    gasnetc_GNIT_Abort("Get failed with %s", gni_return_string(status));
  }

  return pre;
}


void gasnetc_get_am_credit(uint32_t pe)
{
  gasneti_weakatomic_t *p = &peer_data[pe].am_credit;
  if_pf (!gasnetc_weakatomic_dec_if_positive(p)) {
    GASNETC_TRACE_WAIT_BEGIN();
    do {
      GASNETI_WAITHOOK();
      gasneti_AMPoll();
    } while (!gasnetc_weakatomic_dec_if_positive(p));
    GASNETC_TRACE_WAIT_END(GET_AM_CREDIT_STALL);
  }
}

/* Needs no lock because it is called only from the init code */
void gasnetc_init_post_descriptor_pool(void)
{
  int i;
  const int count = gasnetc_pd_buffers.size / sizeof(gasnetc_post_descriptor_t);
  gasnetc_post_descriptor_t *gpd;

  /* must first destroy the temporary pool of post descriptors */
  for (i=0; i < log2_remote; ++i) {
    gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor();
    gasneti_free(gpd);
  }

  /* now create the new pool */
  gpd = gasnetc_pd_buffers.addr;
  gasneti_assert_always(gpd);
  memset(gpd, 0, gasnetc_pd_buffers.size); /* Just in case */
  for (i = 0; i < count; i += 1) {
    gasneti_lifo_push(&post_descriptor_pool, gpd + i);
  }
}

/* This needs no lock because there is an internal lock in the queue */
GASNETI_MALLOC
gasnetc_post_descriptor_t *gasnetc_alloc_post_descriptor(void)
{
  gasnetc_post_descriptor_t *gpd =
            (gasnetc_post_descriptor_t *) gasneti_lifo_pop(&post_descriptor_pool);
  if_pf (!gpd) {
    /* We may simple not have polled the Cq recently.
       So, WAITHOOK and STALL tracing only if still nothing after first poll */
    GASNETC_TRACE_WAIT_BEGIN();
    int stall = 0;
    goto first;
    do {
      GASNETI_WAITHOOK();
      stall = 1;
first:
      gasnetc_poll_local_queue();
      gpd = (gasnetc_post_descriptor_t *) gasneti_lifo_pop(&post_descriptor_pool);
    } while (!gpd);
    if_pf (stall) GASNETC_TRACE_WAIT_END(ALLOC_PD_STALL);
  }
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
static gasneti_weakatomic_t sys_exit_rcvd = gasneti_weakatomic_init(0);
static gasneti_weakatomic_t sys_exit_code = gasneti_weakatomic_init(0);

#if GASNET_PSHM
gasnetc_exitcode_t *gasnetc_exitcodes = NULL;
#endif

/* this is called from poll when a shutdown packet arrives */
static
void gasnetc_handle_sys_shutdown_packet(uint32_t source, uint16_t arg)
{
  uint32_t distance = 1 << (arg >> 8);
  uint8_t exitcode = arg & 0xff;
  gasneti_weakatomic_val_t readval;

#if GASNET_DEBUG || GASNETI_STATS_OR_TRACE
  GASNETI_TRACE_PRINTF(C,("Got SHUTDOWN Request from node %d w/ exitcode %d",(int)source,exitcode));
#endif

#if GASNETI_THREADS || defined(GASNETI_FORCE_TRUE_WEAKATOMICS)
  /* Atomic MAX via C-A-S: */
  do {
    readval = gasneti_atomic_read(&sys_exit_code, 0);
  } while ((exitcode > readval) &&
           !gasneti_atomic_compare_and_swap(&sys_exit_code, readval, exitcode, 0));

  /* Atomic OR via C-A-S: */
  do {
    readval = gasneti_atomic_read(&sys_exit_rcvd, 0);
  } while (! gasneti_atomic_compare_and_swap(&sys_exit_rcvd, readval, (distance|readval), 0));
#else
  readval = gasneti_weakatomic_read(&sys_exit_code, 0);
  gasneti_weakatomic_set(&sys_exit_code, MAX(readval, exitcode), 0);

  readval = gasneti_weakatomic_read(&sys_exit_rcvd, 0);
  gasneti_weakatomic_set(&sys_exit_rcvd, (distance|readval), 0);
#endif
}


/* Reduction (op=MAX) over exitcodes using dissemination pattern.
   Returns 0 on sucess, or non-zero on error (timeout).
 */
extern int gasnetc_sys_exit(int *exitcode_p)
{
#if GASNET_PSHM                  
  const gasnet_node_t size = gasneti_nodemap_global_count;
  const gasnet_node_t rank = gasneti_nodemap_global_rank;
#else
  const gasnet_node_t size = gasneti_nodes;
  const gasnet_node_t rank = gasneti_mynode;
#endif
  uint32_t goal = 0;
  uint32_t distance;
  int result = 0; /* success */
  int exitcode = *exitcode_p;
  int oldcode;
  int shift;
  gasneti_tick_t timeout_us = 1e6 * gasnetc_shutdown_seconds;
  gasneti_tick_t starttime = gasneti_ticks_now();

  GASNETI_TRACE_PRINTF(C,("Entering SYS EXIT"));

#if GASNET_PSHM
  if (gasneti_nodemap_local_rank) {
    gasnetc_exitcode_t * const self = &gasnetc_exitcodes[gasneti_nodemap_local_rank];
    gasnetc_exitcode_t * const lead = &gasnetc_exitcodes[0];

    /* publish our exit code for leader to read */
    self->exitcode = exitcode;
    gasneti_local_wmb(); /* Release */
    self->present = 1;

    /* wait for leader to publish final result */
    while (! lead->present) {
      gasnetc_poll();
      if (gasneti_ticks_to_us(gasneti_ticks_now() - starttime) > timeout_us) {
        result = 1; /* failure */
        goto out;
      }
    }
    gasneti_local_rmb(); /* Acquire */
    exitcode = lead->exitcode;

    goto out;
  } else {
    int i;

    /* collect exit codes */
    for (i = 1; i < gasneti_nodemap_local_count; ++i) {
      gasnetc_exitcode_t * const peer = &gasnetc_exitcodes[i];

      while (! peer->present) {
        gasnetc_poll();
        if (gasneti_ticks_to_us(gasneti_ticks_now() - starttime) > timeout_us) {
          result = 1; /* failure */
          goto out;
        }
      }
      gasneti_local_rmb(); /* Acquire */
      exitcode = MAX(exitcode, peer->exitcode);
    }
  }
#endif

  for (distance = 1, shift = 0; distance < size; distance *= 2, ++shift) {
    gasnet_node_t peeridx = (distance >= size - rank) ? rank - (size - distance)
                                                      : rank + distance;
  #if GASNET_PSHM
    gasnet_node_t dest = gasneti_pshm_firsts[peeridx];
  #else
    gasnet_node_t dest = peeridx;
  #endif

    GASNETI_TRACE_PRINTF(C,("Send SHUTDOWN Request to node %d w/ shift %d, exitcode %d",
                            dest,shift,exitcode));
    gasnetc_send_control(dest, GC_CTRL_SHUTDOWN, (shift << 8) | (exitcode & 0xff));

    /* wait for completion of the proper receive, which might arrive out of order */
    goal |= distance;
    while ((gasneti_weakatomic_read(&sys_exit_rcvd, 0) & goal) != goal) {
      gasnetc_poll();
      if (gasneti_ticks_to_us(gasneti_ticks_now() - starttime) > timeout_us) {
        result = 1; /* failure */
        goto out;
      }
    }

    oldcode = gasneti_weakatomic_read(&sys_exit_code, 0);
    exitcode = MAX(exitcode, oldcode);
  }

#if GASNET_PSHM
  { /* publish final exit code */
    gasnetc_exitcode_t * const self = &gasnetc_exitcodes[0];
    self->exitcode = exitcode;
    gasneti_local_wmb(); /* Release */
    self->present = 1;
  }
#endif

out:
  *exitcode_p = exitcode;

  return result;
}






/* AuxSeg setup for registered bounce buffer space*/
GASNETI_IDENT(gasneti_bounce_auxseg_IdentString,
              "$GASNetAuxSeg_bounce: GASNET_GNI_BOUNCE_SIZE:" _STRINGIFY(GASNETC_GNI_BOUNCE_SIZE_DEFAULT)" $");
gasneti_auxseg_request_t gasnetc_bounce_auxseg_alloc(gasnet_seginfo_t *auxseg_info) {
  gasneti_auxseg_request_t retval;
  
  retval.minsz =
  retval.optimalsz = gasneti_getenv_int_withdefault("GASNET_GNI_BOUNCE_SIZE",
                                                    GASNETC_GNI_BOUNCE_SIZE_DEFAULT,1);
  if (auxseg_info != NULL) { /* auxseg granted */
    /* The only one we care about is our own node */
    gasnetc_bounce_buffers = auxseg_info[gasneti_mynode];
  }

  return retval;
}

/* AuxSeg setup for registered post descriptors*/
/* This ident string is used by upcrun (and potentially by other tools) to estimate
 * the auxseg requirements, and gets rounded up.
 * So, this doesn't need to be an exact value.
 * As of 2013.03.06 I have systems with
 *     Gemini = 304 bytes
 *     Aries  = 320 bytes
 */
#if GASNET_CONDUIT_GEMINI
  #define GASNETC_SIZEOF_GDP 304
#else
  #define GASNETC_SIZEOF_GDP 320
#endif
GASNETI_IDENT(gasneti_pd_auxseg_IdentString, /* XXX: update if gasnetc_post_descriptor_t changes */
              "$GASNetAuxSeg_pd: " _STRINGIFY(GASNETC_SIZEOF_GDP) "*"
              "(GASNET_GNI_NUM_PD:" _STRINGIFY(GASNETC_GNI_NUM_PD_DEFAULT) ") $");
gasneti_auxseg_request_t gasnetc_pd_auxseg_alloc(gasnet_seginfo_t *auxseg_info) {
  gasneti_auxseg_request_t retval;
  
  retval.minsz =
  retval.optimalsz = gasneti_getenv_int_withdefault("GASNET_GNI_NUM_PD",
                                                    GASNETC_GNI_NUM_PD_DEFAULT,1) 
    * sizeof(gasnetc_post_descriptor_t);
  if (auxseg_info != NULL) { /* auxseg granted */
    /* The only one we care about is our own node */
    gasnetc_pd_buffers = auxseg_info[gasneti_mynode];
  }

  return retval;
}

void gasnetc_init_bounce_buffer_pool(void)
{
  int i;
  int num_bounce;
  size_t buffer_size;
  gasneti_assert_always(gasnetc_bounce_buffers.addr != NULL);

  buffer_size = MAX(gasnetc_get_bounce_register_cutover,
                    gasnetc_put_bounce_register_cutover);

  num_bounce = gasnetc_bounce_buffers.size / buffer_size;
  for(i = 0; i < num_bounce; i += 1) {
    gasneti_lifo_push(&gasnetc_bounce_buffer_pool,
                      (char *) gasnetc_bounce_buffers.addr + (buffer_size * i));
  }
}

GASNETI_MALLOC
void *gasnetc_alloc_bounce_buffer(void)
{
  void *buf = gasneti_lifo_pop(&gasnetc_bounce_buffer_pool);
  if_pf (!buf) {
    /* We may simple not have polled the Cq recently.
       So, WAITHOOK and STALL tracing only if still nothing after first poll */
    GASNETC_TRACE_WAIT_BEGIN();
    int stall = 0;
    goto first;
    do {
      GASNETI_WAITHOOK();
      stall = 1;
first:
      gasnetc_poll_local_queue();
      buf = gasneti_lifo_pop(&gasnetc_bounce_buffer_pool);
    } while (!buf);
    if_pf (stall) GASNETC_TRACE_WAIT_END(ALLOC_BB_STALL);
  }
  return(buf);
}

void gasnetc_free_bounce_buffer(void *gcb)
{
  gasneti_lifo_push(&gasnetc_bounce_buffer_pool, gcb);
}
