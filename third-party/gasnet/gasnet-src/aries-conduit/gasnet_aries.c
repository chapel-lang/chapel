#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_am.h>
#include <gasnet_aries.h>
#include <fcntl.h>
#include <sys/mman.h>
#ifdef GASNETI_USE_HUGETLBFS
#include <hugetlbfs.h>
#endif
#include <signal.h>
#include <string.h>

#define GASNETC_NETWORKDEPTH_SPACE_DEFAULT (12*1024)
#define GASNETC_NETWORKDEPTH_TOTAL_DEFAULT 64
#define GASNETC_NETWORKDEPTH_DEFAULT 64

#define GASNETC_GNI_AM_RVOUS_CUTOVER_DEFAULT 16384 // TODO-EX: this is a W.A.G.
#define GASNETC_GNI_AM_RVOUS_BUFFERS_DEFAULT 64

// How many times to retry a Post which fails with GNI_RC_ERROR_RESOURCE
// TODO: Should this be an env var?
#ifndef GASNETC_RESOURCE_RETRIES
#define GASNETC_RESOURCE_RETRIES 65536
#endif

// Use alloca()?  (e.g. to work-around bug 2079 or similar)
#ifdef GASNETI_USE_ALLOCA
  // Keep defn
#elif HAVE_ALLOCA && !PLATFORM_COMPILER_PGI
  #define GASNETI_USE_ALLOCA 1
#endif
#if GASNETI_USE_ALLOCA && HAVE_ALLOC_H
  #include <alloca.h>
#endif

// Max number of times to poll the AM mailboxes per entry
// TODO: distinct value for CQ events reaped vs service limit on "slow" list?
static int gasnetc_ampoll_burst; // Units of events
#define GASNETC_GNI_AMPOLL_BURST_DEFAULT 20
#if GASNETI_USE_ALLOCA
  static size_t gasnetc_ampoll_burst_sz; // Units of bytes
#elif !defined(GASNETC_GNI_AMPOLL_BURST_MAX)
  #define GASNETC_GNI_AMPOLL_BURST_MAX 100
#endif

#if GASNET_DEBUG
  #define GASNETC_CDM_MODE GNI_CDM_MODE_FORK_FULLCOPY | GNI_CDM_MODE_ERR_NO_KILL
#else
  #define GASNETC_CDM_MODE GNI_CDM_MODE_FORK_FULLCOPY
#endif
static uint32_t gasnetc_cdm_mode = GASNETC_CDM_MODE | GNI_CDM_MODE_DUAL_EVENTS;

int      gasnetc_dev_id;
uint32_t gasnetc_cookie;
uint32_t gasnetc_address;
uint8_t  gasnetc_ptag;

static uint32_t gasnetc_memreg_flags;
static int gasnetc_mem_consistency;

struct peer_struct_t_ {
  unsigned int pe;
  gni_ep_handle_t ep_handle;
  gni_mem_handle_t mem_handle; // client segment
  gni_mem_handle_t aux_handle;
  gni_mem_handle_t am_handle;

  gasnetc_notify_t *remote_notify_base;
  gasnetc_notify_t *local_notify_base;
  uint8_t *remote_reply_base;
  uint8_t *local_request_base;
  uint8_t *remote_request_base;

#if GASNET_PAR
  volatile int remote_request_lock;
#endif
  uint64_t remote_request_map;  /* allocation bitmap */
  uint32_t remote_notify_write; /* covered by the gni lock, unbounded */
  uint32_t local_notify_read;   /* covered by the ampoll lock, bounded [0..notify_ring_size) */
  peer_struct_t *next;          /* covered by the ampoll lock */
  unsigned int event_count;     /* covered by the ampoll lock */
  int long_credits;
#if GASNETC_USE_MULTI_DOMAIN
  uint32_t nic_addr;
#endif
};

static gni_mem_handle_t am_handle;

static void *am_mmap_ptr;
static size_t am_mmap_bytes;

typedef struct reply_pool {
  gasnetc_packet_t  *packet;         /* Corresponding mailbox */
  union {
    struct reply_pool *next;         /* Next when on reply_freelist */
    struct {                         /* Upon Reply this is slots or credits to release */
      peer_struct_t *peer;
      uint64_t value;
      int isLong;
    } credit;
  } u;
} reply_pool_t;

static size_t am_replysz = 0;
static reply_pool_t *reply_pool = NULL;
static reply_pool_t *reply_freelist = NULL;

static int am_rvous_enabled;
static gasneti_lifo_head_t am_rvous_pool = GASNETI_LIFO_INITIALIZER;

static gasnet_seginfo_t gasnetc_bounce_buffers;
static gasnet_seginfo_t gasnetc_pd_buffers;

unsigned int gasnetc_log2_remote;
static unsigned int num_pd;
static unsigned int num_cqe;
static uint32_t notify_ring_mask; /* ring size minus 1 */
static unsigned int am_slotsz;
static unsigned int am_slot_bits;
static unsigned int am_maxcredit;
static unsigned int request_bits;

static int have_auxseg = 0;
static int have_segment = 0;

static gni_cq_handle_t am_cq_handle;
static int gasnetc_poll_burst = 10;
static size_t gasnetc_get_fma_rdma_cutover;
static size_t gasnetc_put_fma_rdma_cutover;
static size_t gasnetc_get_bounce_register_cutover;
static size_t gasnetc_put_bounce_register_cutover;
size_t gasnetc_max_get_unaligned;

/* read-only: */
// TODO-EX: this needs to be more general for multi-segment support
static gni_mem_handle_t my_mem_handle;
static gni_mem_handle_t my_aux_handle;

#if GASNETC_BUILD_GNICE
// TODO: multi-CE and multi-EP ??
static gni_ep_handle_t my_ce_ep_handle;
static gni_ce_handle_t ce_handle;
int gasnete_ce_available = 0;
#endif

#if GASNETC_USE_MULTI_DOMAIN
static unsigned int gasnetc_domain_count;
static unsigned int gasnetc_domain_count_max;
static unsigned int gasnetc_poll_am_domain_mask;
#if (GASNETC_DOMAIN_THREAD_DISTRIBUTION == GASNETC_DOMAIN_THREAD_DISTRIBUTION_BULK)
static int gasnetc_threads_per_domain;
#endif

typedef struct {
  int8_t pads[GASNETC_CACHELINE_SIZE];
  gni_cdm_handle_t cdm_handle;
  gni_nic_handle_t nic_handle;
  gni_cq_handle_t bound_cq_handle;
  gasneti_lifo_head_t post_descriptor_pool;
  peer_struct_t *peer_data;
  int threads_per_domain;
  int initialized;
  unsigned int poll_idx;
  gasneti_lifo_head_t bounce_buffer_pool;
  int8_t padm[GASNETC_CACHELINE_SIZE];
  gasnetc_gni_lock_t gasnetc_gni_lock;
  int8_t pade[GASNETC_CACHELINE_SIZE];
} communication_domain_struct_t;

static communication_domain_struct_t * gasnetc_cdom_data;

#if (GASNETC_DOMAIN_THREAD_DISTRIBUTION == GASNETC_DOMAIN_THREAD_DISTRIBUTION_BULK)
int gasnetc_get_domain_idx(gasnete_threadidx_t tidx)
{
  int didx = (tidx / gasnetc_threads_per_domain) % gasnetc_domain_count;
  return didx;
}
GASNETI_INLINE(gasnetc_get_domain_first_thread_idx)
gasnete_threadidx_t gasnetc_get_domain_first_thread_idx(int didx)
{
  int tidx = didx * gasnetc_threads_per_domain;
  return tidx;
}
#else
int gasnetc_get_domain_idx(gasnete_threadidx_t tidx)
{
  int didx = tidx % gasnetc_domain_count;
  return didx;
}

GASNETI_INLINE(gasnetc_get_domain_first_thread_idx)
gasnete_threadidx_t gasnetc_get_domain_first_thread_idx(int didx)
{
  int tidx = didx;
  return tidx;
}
#endif

GASNETI_INLINE(reset_comm_data)
void reset_comm_data(communication_domain_struct_t * ddata)
{
  gasneti_lifo_init(&(ddata->post_descriptor_pool));
  gasneti_lifo_init(&(ddata->bounce_buffer_pool));
#if (GASNETC_DOMAIN_ALLOC_POLICY == GASNETC_STATIC_DOMAIN_ALLOC)
  ddata->threads_per_domain = -1;
#else
  ddata->threads_per_domain = 0;
#endif	
  ddata->initialized = 0;
  ddata->poll_idx = 0;
}

gasnetc_gni_lock_t *gasnetc_gni_lock() 
{
   return & gasnetc_cdom_data[GASNETC_DEFAULT_DOMAIN].gasnetc_gni_lock;
}

#else /* GASNETC_USE_MULTI_DOMAIN */

#define gasnetc_domain_count 1
#define gasnetc_domain_count_max 1

static gni_cdm_handle_t cdm_handle;

/* read-only: */
static gni_nic_handle_t nic_handle;
static gni_cq_handle_t bound_cq_handle;
static peer_struct_t *peer_data;

/* lock: */
gasnetc_gni_lock_t gasnetc_gni_lock;

/* lifo_head_t contains cache-line padding */
static gasneti_lifo_head_t post_descriptor_pool = GASNETI_LIFO_INITIALIZER;
static gasneti_lifo_head_t bounce_buffer_pool = GASNETI_LIFO_INITIALIZER;

#endif /* GASNETC_USE_MULTI_DOMAIN */

gasnetc_gni_lock_t gasnetc_am_buffer_lock;

// Op and Arg for a control msg are 8 and 16 bits, respectively.
#define gc_build_ctrl(op, arg) ((uint32_t)(op) | ((uint32_t)(arg) << 8))
#define gc_ctrl_op(x)  ((uint8_t)(x))
#define gc_ctrl_arg(x) ((uint16_t)((x) >> 8))

/*------ Convience functions for printing error messages ------*/

static const char *gasnetc_gni_rc_string(gni_return_t status)
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
  if (type == GC_CMD_AM_SHORT) return ("AM_SHORT");
  if (type == GC_CMD_AM_MEDIUM) return ("AM_MEDIUM");
  if (type == GC_CMD_AM_LONG) return ("AM_LONG");
  if (type == GC_CMD_AM_LONG_PACKED) return ("AM_LONG_PACKED");
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
                             gasnetc_memreg_flags|GNI_MEM_READWRITE, -1, &mem_handle);
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

/*------ Macros to acheive readable access to domain-specific variables ------*/

#if GASNETC_USE_MULTI_DOMAIN
#  define DOMAIN_SPECIFIC_VAL(_var)         (gasnetc_cdom_data[_domain_idx]._var)
#  define DOMAIN_SPECIFIC_VAR(_type, _var)  _type _var = DOMAIN_SPECIFIC_VAL(_var)
#else
#  define DOMAIN_SPECIFIC_VAL(_var)         (_var)
#  define DOMAIN_SPECIFIC_VAR(_type, _var)  char _dummy##_var = 0
#endif

/*------ Macros for lock operations */

#if GASNETC_USE_SPINLOCK
  #define GASNETC_INITLOCK_GNI() gasneti_spinlock_init(&DOMAIN_SPECIFIC_VAL(gasnetc_gni_lock))
  #define GASNETC_LOCK_GNI()     gasneti_spinlock_lock(&DOMAIN_SPECIFIC_VAL(gasnetc_gni_lock))
  #define GASNETC_UNLOCK_GNI()   gasneti_spinlock_unlock(&DOMAIN_SPECIFIC_VAL(gasnetc_gni_lock))
  #define GASNETC_INITLOCK_AM_BUFFER() gasneti_spinlock_init(&gasnetc_am_buffer_lock)
  #define GASNETC_LOCK_AM_BUFFER()     gasneti_spinlock_lock(&gasnetc_am_buffer_lock)
  #define GASNETC_UNLOCK_AM_BUFFER()   gasneti_spinlock_unlock(&gasnetc_am_buffer_lock)
  #define GASNETC_INITLOCK_UDREG()     gasneti_spinlock_init(&gasnetc_udreg_lock)
  #define GASNETC_LOCK_UDREG_()        gasneti_spinlock_lock(&gasnetc_udreg_lock)
  #define GASNETC_UNLOCK_UDREG()       gasneti_spinlock_unlock(&gasnetc_udreg_lock)
#else
  #define GASNETC_INITLOCK_GNI() gasneti_mutex_init(&DOMAIN_SPECIFIC_VAL(gasnetc_gni_lock))
  #define GASNETC_LOCK_GNI()     gasneti_mutex_lock(&DOMAIN_SPECIFIC_VAL(gasnetc_gni_lock))
  #define GASNETC_UNLOCK_GNI()   gasneti_mutex_unlock(&DOMAIN_SPECIFIC_VAL(gasnetc_gni_lock))
  #define GASNETC_INITLOCK_AM_BUFFER() gasneti_mutex_init(&gasnetc_am_buffer_lock)
  #define GASNETC_LOCK_AM_BUFFER()     gasneti_mutex_lock(&gasnetc_am_buffer_lock)
  #define GASNETC_UNLOCK_AM_BUFFER()   gasneti_mutex_unlock(&gasnetc_am_buffer_lock)
  #define GASNETC_INITLOCK_UDREG()     gasneti_mutex_init(&gasnetc_udreg_lock)
  #define GASNETC_LOCK_UDREG_()        gasneti_mutex_lock(&gasnetc_udreg_lock)
  #define GASNETC_UNLOCK_UDREG()       gasneti_mutex_unlock(&gasnetc_udreg_lock)
#endif

/*------ Functions for dynamic memory registration ------*/

#if GASNETC_GNI_UDREG
typedef union gasnetc_reg_data_{
  union gasnetc_reg_data_ *next;
  gni_mem_handle_t mem_handle;
} gasnetc_reg_data_t;
static gasnetc_reg_data_t *mem_reg_data_pool = NULL;

GASNETI_INLINE(mem_reg_data_alloc)
GASNETI_MALLOC gasnetc_reg_data_t *mem_reg_data_alloc(void)
{
  gasnetc_reg_data_t *result = mem_reg_data_pool;
  if_pf (NULL == result) {
    const int count = GASNET_PAGESIZE / sizeof(gasnetc_reg_data_t); /* TODO: env var to tune? */
    int i;
    result = gasneti_malloc(count * sizeof(gasnetc_reg_data_t));
    gasneti_leak(result);
    for (i = 0; i < count - 1; ++i) {
      result[i].next = &result[i + 1];
    }
    result[i].next = NULL;
  }
  mem_reg_data_pool = result->next;
  return result;
}

GASNETI_INLINE(mem_reg_data_free)
void mem_reg_data_free(gasnetc_reg_data_t *p)
{
  p->next = mem_reg_data_pool;
  mem_reg_data_pool = p;
}

static udreg_cache_handle_t gasnetc_udreg_hndl;
static gasnetc_gni_lock_t gasnetc_udreg_lock;

#if GASNETC_USE_MULTI_DOMAIN
  static int gasnetc_udreg_domain;
  #define GASNETC_LOCK_UDREG() do {                        \
      GASNETC_LOCK_UDREG_();                               \
      gasnetc_udreg_domain = GASNETC_DIDX_PASS_ALONE;      \
    } while (0)
#else
  #define GASNETC_LOCK_UDREG() GASNETC_LOCK_UDREG_()
#endif

/* Called w/ UDREG lock held */
static void *
gasnetc_udreg_register(void *addr, uint64_t len, void *context) {
  GASNETC_DIDX_POST(gasnetc_udreg_domain);
  DOMAIN_SPECIFIC_VAR(gni_nic_handle_t, nic_handle);
  gasnetc_reg_data_t *data = mem_reg_data_alloc();
  gni_return_t status;

  GASNETC_LOCK_GNI();
  status = GNI_MemRegister(nic_handle, (uint64_t)addr, len, NULL,
                           gasnetc_memreg_flags|GNI_MEM_READWRITE,
                           -1, &(data->mem_handle));
  GASNETC_UNLOCK_GNI();

  if_pf (GNI_RC_SUCCESS != status) {
    mem_reg_data_free(data);
    data = (GNI_RC_ERROR_RESOURCE == status) ? NULL : UDREG_DEVICE_REG_FAILED;
  }

  return data;
}

/* Called w/ UDREG lock held */
static uint32_t
gasnetc_udreg_deregister(void *data_arg, void *context) {
  GASNETC_DIDX_POST(gasnetc_udreg_domain);
  DOMAIN_SPECIFIC_VAR(gni_nic_handle_t, nic_handle);
  gasnetc_reg_data_t *data = (gasnetc_reg_data_t*)data_arg;
  gni_return_t status;
  GASNETC_LOCK_GNI();
  status = GNI_MemDeregister(nic_handle, &data->mem_handle);
  GASNETC_UNLOCK_GNI();
  gasneti_assert_always (status == GNI_RC_SUCCESS);
  mem_reg_data_free(data);
  return 0; /* TODO: what else might be returned, and when/why? */
}

/* Register local side of a pd, with unbounded retry on resource shortage.
   Returns 1 on success, or 0 on INVAL_PARAM */
static int gasnetc_register_udreg(gasnetc_post_descriptor_t *gpd, uint32_t memreg_flags)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  gni_post_descriptor_t * const pd = &gpd->pd;
  const uint64_t addr = pd->local_addr;
  const size_t nbytes = pd->length;
  udreg_entry_t *entry;
  udreg_return_t rc;
  GASNETC_TRACE_WAIT_BEGIN();
  int stall = 0;

  GASNETC_LOCK_UDREG();
  for (;;) {
    rc = UDREG_Register(gasnetc_udreg_hndl, (void*)addr, nbytes, &entry);
    if_pt (UDREG_RC_SUCCESS == rc) {
      GASNETC_UNLOCK_UDREG();
      if (stall) GASNETC_TRACE_WAIT_END(MEM_REG_STALL);
      gpd->u.udreg_entry = entry;
      pd->local_mem_hndl = ((gasnetc_reg_data_t*)(entry->device_data))->mem_handle;
      return 1;
    } else if (UDREG_RC_INVALID_PARAM == rc) {
      GASNETC_UNLOCK_UDREG();
      if (stall) GASNETC_TRACE_WAIT_END(MEM_REG_STALL);
      return 0;
    } else if (UDREG_RC_NO_SPACE == rc) {
      stall = 1;
      rc = UDREG_Evict(gasnetc_udreg_hndl);
      if_pt (UDREG_RC_SUCCESS == rc) {
        continue;
      } else if (UDREG_RC_NO_MATCH == rc) {
        GASNETC_UNLOCK_UDREG();
        GASNETI_WAITHOOK();
        gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
        GASNETC_LOCK_UDREG();
      } else {
        gasnetc_GNIT_Abort("UDREG_Evict failed with rc = %d", rc);
        break; /* NOT REACHED */
      }
    } else {
      /* Unknown failure is fatal */
      gasnetc_GNIT_Abort("UDREG_Register failed at %p + %p with rc = %d",
                         (void*)addr, (void*)nbytes, rc);
      break; /* NOT REACHED */
    }
  }
  /* NOT REACHED */
  GASNETC_UNLOCK_UDREG();
  return 0;
}

/* Deregister local side of a pd */
GASNETI_INLINE(gasnetc_deregister_udreg)
void gasnetc_deregister_udreg(gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  udreg_return_t rc;
  GASNETC_LOCK_UDREG();
  rc = UDREG_DecrRefcount(gasnetc_udreg_hndl, gpd->u.udreg_entry);
  GASNETC_UNLOCK_UDREG();
  gasneti_assert_always (UDREG_RC_SUCCESS == rc);
}
#endif /* GASNETC_GNI_UDREG */

/* read-write: (should cache pad) */
static gasneti_weakatomic_t gasnetc_reg_credit;

static gasneti_weakatomic_val_t gasnetc_reg_credit_max;
#define gasnetc_init_reg_credit(_val) \
	gasneti_weakatomic_set(&gasnetc_reg_credit,gasnetc_reg_credit_max=(_val),0)

/* Register local side of a pd, with unbounded retry on ERR_RESOURCE.
   Returns 1 on success, or 0 on ERR_INVAL_PARAM */
static int gasnetc_register_gni(gasnetc_post_descriptor_t *gpd, uint32_t memreg_flags)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(gni_nic_handle_t, nic_handle);
  gni_post_descriptor_t * const pd = &gpd->pd;
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
      gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
    } while (!gasnetc_weakatomic_dec_if_positive(&gasnetc_reg_credit));
    if_pf (stall) GASNETC_TRACE_WAIT_END(MEM_REG_STALL);
  }

  memreg_flags |= gasnetc_memreg_flags;
  for (;;) {
    GASNETC_LOCK_GNI();
    status = GNI_MemRegister(nic_handle, addr, nbytes, NULL,
                             memreg_flags, -1, &pd->local_mem_hndl);
    GASNETC_UNLOCK_GNI();
    if_pt (status == GNI_RC_SUCCESS) {
      GASNETC_STAT_EVENT_VAL(MEM_REG_RETRY, trial);
      return 1;
    } else if (status == GNI_RC_ERROR_RESOURCE) {
      GASNETI_WAITHOOK();
      gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
      ++trial;
    } else if (status == GNI_RC_INVALID_PARAM) {
      /* Registration failed (e.g. memory imported by XPMEM), but not fatal. */
      if (gasnetc_reg_credit_max) gasneti_weakatomic_increment(&gasnetc_reg_credit, 0);
      return 0;
    } else {
      /* Unknown failure is fatal */
      gasnetc_GNIT_Abort("MemRegister failed at %p + %p with %s",
                         (void*)addr, (void*)nbytes, gasnetc_gni_rc_string(status));
      break; /* NOT REACHED */
    }
  }
  return 0; /* NOT REACHED */
}

/* Deregister local side of a pd */
GASNETI_INLINE(gasnetc_deregister_gni)
void gasnetc_deregister_gni(gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(gni_nic_handle_t, nic_handle);
  gni_return_t status = GNI_MemDeregister(nic_handle, &gpd->pd.local_mem_hndl);
  gasneti_assert_always (status == GNI_RC_SUCCESS);
  if (gasnetc_reg_credit_max) gasneti_weakatomic_increment(&gasnetc_reg_credit, 0);
}

/* Register local side of a pd */
GASNETI_INLINE(gasnetc_register_gpd)
int gasnetc_register_gpd(gasnetc_post_descriptor_t *gpd, uint32_t memreg_flags)
{
#if GASNETC_GNI_UDREG
  if_pt (gasnetc_udreg_hndl) {
    return gasnetc_register_udreg(gpd, memreg_flags);
  } else
#endif
  return gasnetc_register_gni(gpd, memreg_flags);
}

/* Deregister local side of a pd */
GASNETI_INLINE(gasnetc_deregister_gpd)
void gasnetc_deregister_gpd(gasnetc_post_descriptor_t *gpd)
{
#if GASNETC_GNI_UDREG
  if_pt (gasnetc_udreg_hndl) {
    gasnetc_deregister_udreg(gpd);
  } else
#endif
  gasnetc_deregister_gni(gpd);
}

/*-------------------------------------------------*/

/* From point-of-view of a remote node, what is MY index in the mailbox array */
GASNETI_INLINE(my_mb_index)
int my_mb_index(gex_Rank_t remote_node) {
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

// Forward declarations
static void gasnetc_init_post_descriptor_pool(GASNETC_DIDX_FARG_ALONE);
static void gasnetc_init_bounce_buffer_pool(GASNETC_DIDX_FARG_ALONE);

/*-------------------------------------------------*/
/* called after aux segment init.
 * compute settings
 * setup udreg (if applicable)
 * register the auxseg
 * allgather the memory handles for the auxseg
 * create parallen domains (if applicable)
 */
void gasnetc_init_gni(gasnet_seginfo_t seginfo)
{
  gni_return_t status;
#if GASNETC_USE_MULTI_DOMAIN
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  DOMAIN_SPECIFIC_VAR(gni_nic_handle_t, nic_handle);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
#endif
  size_t bb_size = gasnetc_bounce_buffers.size / gasnetc_domain_count_max;
  int max_memreg = gasneti_getenv_int_withdefault("GASNET_GNI_MEMREG", GASNETC_GNI_MEMREG_DEFAULT, 0);

  if (bb_size < GASNET_PAGESIZE) {
    gasneti_fatalerror("GASNET_GNI_BOUNCE_SIZE must be %d or larger", (int)GASNET_PAGESIZE);
  }

  if (bb_size < GASNETC_MSG_MAXSIZE) {
    gasneti_fatalerror("GASNET_GNI_BOUNCE_SIZE must be %d or larger", (int)GASNETC_MSG_MAXSIZE);
  }

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
  gasnetc_get_bounce_register_cutover = MAX(gasnetc_get_bounce_register_cutover,
                                            GASNET_PAGESIZE);
  gasnetc_get_bounce_register_cutover = MIN(gasnetc_get_bounce_register_cutover,
                                            GASNETC_GNI_BOUNCE_REGISTER_CUTOVER_MAX);
  gasnetc_get_bounce_register_cutover = MIN(gasnetc_get_bounce_register_cutover,
                                            bb_size);
  gasneti_assert(gasnetc_get_bounce_register_cutover >= GASNET_PAGESIZE);

  gasnetc_put_bounce_register_cutover = 
    gasneti_getenv_int_withdefault("GASNET_GNI_PUT_BOUNCE_REGISTER_CUTOVER",
				   GASNETC_GNI_PUT_BOUNCE_REGISTER_CUTOVER_DEFAULT,1);
  gasnetc_put_bounce_register_cutover = MAX(gasnetc_put_bounce_register_cutover,
                                            GASNET_PAGESIZE);
  gasnetc_put_bounce_register_cutover = MIN(gasnetc_put_bounce_register_cutover,
                                            GASNETC_GNI_BOUNCE_REGISTER_CUTOVER_MAX);
  gasnetc_put_bounce_register_cutover = MIN(gasnetc_put_bounce_register_cutover,
                                            bb_size);
  gasneti_assert(gasnetc_put_bounce_register_cutover >= GASNET_PAGESIZE);

  /* Derived limits used in extended API implementation: */
  gasnetc_max_get_unaligned = MAX(GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE,
                                  gasnetc_get_bounce_register_cutover);

#if GASNETC_GNI_UDREG
  if (gasneti_getenv_yesno_withdefault("GASNET_USE_UDREG", 1)) {
    char name[] = "gasnet";
    struct udreg_cache_attr attr;
    udreg_return_t rc;

    GASNETC_INITLOCK_UDREG();

    strncpy (attr.cache_name, name, UDREG_MAX_CACHENAME_LEN);
    attr.max_entries = MAX(max_memreg, 1);
    attr.modes = UDREG_CC_MODE_USE_KERNEL_CACHE |
                 UDREG_CC_MODE_USE_LAZY_DEREG;
    attr.debug_mode = 0;
    attr.debug_rank = 0;
    attr.reg_context         = NULL;
    attr.dreg_context        = NULL;
    attr.destructor_context  = NULL;
    attr.device_reg_func     = gasnetc_udreg_register;
    attr.device_dereg_func   = gasnetc_udreg_deregister;
    attr.destructor_callback = NULL;

    rc = UDREG_CacheCreate(&attr); /* TODO: error detect/recover? */
    if (UDREG_RC_SUCCESS != rc) {
      gasnetc_GNIT_Abort("UDREG_CacheCreate() failed with rc=%d", rc);
    }

    rc = UDREG_CacheAccess(name, &gasnetc_udreg_hndl);
    if (UDREG_RC_SUCCESS != rc) {
      gasnetc_GNIT_Abort("UDREG_CacheAccess() failed with rc=%d", rc);
    }
  } else
#endif
  {
    gasnetc_init_reg_credit(MAX(max_memreg, 0));
  }

  { const char * envval = gasneti_getenv_withdefault("GASNET_GNI_MEM_CONSISTENCY","relaxed");
    gasnetc_mem_consistency = GASNETC_RELAXED_MEM_CONSISTENCY;
    if (!strcmp(envval, "strict") || !strcmp(envval, "STRICT")) {
      gasnetc_mem_consistency = GASNETC_STRICT_MEM_CONSISTENCY;
    } else if (!strcmp(envval, "relaxed") || !strcmp(envval, "RELAXED")) {
      gasnetc_mem_consistency = GASNETC_RELAXED_MEM_CONSISTENCY;
    } else if (!strcmp(envval, "none") || !strcmp(envval, "NONE")) {
      gasnetc_mem_consistency = GASNETC_NEITHER_MEM_CONSISTENCY;
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
    case GASNETC_NEITHER_MEM_CONSISTENCY:
      gasnetc_memreg_flags = 0;
      break;
  }

  {
    int count = 0;
    for (;;) {
      status = GNI_MemRegister(nic_handle, (uint64_t) seginfo.addr,
			       (uint64_t) seginfo.size, am_cq_handle,
			       gasnetc_memreg_flags|GNI_MEM_READWRITE, -1,
			       &my_aux_handle);
      if (status == GNI_RC_SUCCESS) break;
      if (status == GNI_RC_ERROR_RESOURCE) {
	gasnetc_GNIT_Log("MemRegister auxseg fault %d at  %p %"PRIxPTR", code %s",
		count, seginfo.addr, seginfo.size, gasnetc_gni_rc_string(status));
	count += 1;
	if (count >= 10) break;
      } else {
	break;
      }
    }
  }
  gasneti_assert_always (status == GNI_RC_SUCCESS);

  gasnetc_init_post_descriptor_pool(GASNETC_DIDX_PASS_ALONE);
  gasnetc_init_bounce_buffer_pool(GASNETC_DIDX_PASS_ALONE);

  {
    gni_mem_handle_t *all_mem_handle = gasneti_malloc(gasneti_nodes * sizeof(gni_mem_handle_t));
    gasnetc_bootstrapExchange_gni(&my_aux_handle, sizeof(gni_mem_handle_t), all_mem_handle);
    for (gex_Rank_t i = 0; i < gasneti_nodes; ++i) {
      peer_data[i].aux_handle = all_mem_handle[i];
    }
    gasneti_free(all_mem_handle);
  }

  have_auxseg = 1;

#if GASNETC_USE_MULTI_DOMAIN
 #if(GASNETC_DOMAIN_ALLOC_POLICY == GASNETC_STATIC_DOMAIN_ALLOC)
  for (int i = 1; i < gasnetc_domain_count; i++) {
    gasnetc_create_parallel_domain(gasnetc_get_domain_first_thread_idx(i));
  }
 #endif
#endif
}

/*-------------------------------------------------*/
/* called after client segment init. */
/* allgather the memory handles for the segments */
void gasnetc_init_segment(gasnet_seginfo_t seginfo)
{
  gni_return_t status;
#if GASNETC_USE_MULTI_DOMAIN
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  DOMAIN_SPECIFIC_VAR(gni_nic_handle_t, nic_handle);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
#endif

  {
    int count = 0;
    for (;;) {
      status = GNI_MemRegister(nic_handle, (uint64_t) seginfo.addr,
			       (uint64_t) seginfo.size, am_cq_handle,
			       gasnetc_memreg_flags|GNI_MEM_READWRITE, -1,
			       &my_mem_handle);
      if (status == GNI_RC_SUCCESS) break;
      if (status == GNI_RC_ERROR_RESOURCE) {
	gasnetc_GNIT_Log("MemRegister segment fault %d at  %p %lx, code %s",
		count, seginfo.addr, seginfo.size, gasnetc_gni_rc_string(status));
	count += 1;
	if (count >= 10) break;
      } else {
	break;
      }
    }
  }
  have_segment = 1;

  gasneti_assert_always (status == GNI_RC_SUCCESS);

  {
    gni_mem_handle_t *all_mem_handle = gasneti_malloc(gasneti_nodes * sizeof(gni_mem_handle_t));
  #if 0// Cannot use gni-specific bootstrap collectives this late
    gasnetc_bootstrapExchange_gni(&my_mem_handle, sizeof(gni_mem_handle_t), all_mem_handle);
  #else
    // TODO-EX: but we want real collectives here eventually anyway
    gasneti_defaultExchange(&my_mem_handle, sizeof(gni_mem_handle_t), all_mem_handle);
  #endif
    for (gex_Rank_t i = 0; i < gasneti_nodes; ++i) {
      peer_data[i].mem_handle = all_mem_handle[i];
    }
    gasneti_free(all_mem_handle);
  }

#if GASNETC_USE_MULTI_DOMAIN && (GASNETC_DOMAIN_ALLOC_POLICY == GASNETC_STATIC_DOMAIN_ALLOC)
  /* Replicate mem handle - not stricty necessary, but cache-friendly: */
  for (int d = 1; d < gasnetc_domain_count; d++) {
    gasnete_threadidx_t tidx = gasnetc_get_domain_first_thread_idx(d);
    GASNETC_DIDX_POST(gasnetc_get_domain_idx(tidx));

    for (gex_Rank_t n = 0; n < gasneti_nodes; ++n) {
      DOMAIN_SPECIFIC_VAL(peer_data[n]).mem_handle = gasnetc_cdom_data[0].peer_data[n].mem_handle;
    }
  }
#endif
}


#if GASNETC_USE_MULTI_DOMAIN

void  gasnetc_create_parallel_domain(gasnete_threadidx_t tidx)
{
  GASNETC_DIDX_POST(gasnetc_get_domain_idx(tidx));
  gni_return_t status;
  uint32_t local_address;
  uint32_t i;
  unsigned int bytes_per_mbox;
  unsigned int bytes_needed;
  uint32_t inst_id;

  DOMAIN_SPECIFIC_VAL(threads_per_domain)++;
  if(DOMAIN_SPECIFIC_VAL(initialized))
    return;

  GASNETC_INITLOCK_GNI();
  inst_id = gasneti_mynode + GASNETC_DIDX * gasneti_nodes;
  status = GNI_CdmCreate(inst_id,
                        gasnetc_ptag, gasnetc_cookie,
                        gasnetc_cdm_mode,
                        &DOMAIN_SPECIFIC_VAL(cdm_handle));
  gasneti_assert_always (status == GNI_RC_SUCCESS);

  status = GNI_CdmAttach(DOMAIN_SPECIFIC_VAL(cdm_handle),
                        gasnetc_dev_id,
                        &local_address,
                        &DOMAIN_SPECIFIC_VAL(nic_handle));
#if defined(GNI_CDM_MODE_FMA_DEDICATED)
  if_pf ((GNI_RC_ERROR_RESOURCE == status) &&
         (gasnetc_cdm_mode & GNI_CDM_MODE_FMA_DEDICATED)) {
    // TODO: supernode-scoped reduction followed by *automatic* enable of FMA sharing if needed
    gasneti_fatalerror("GNI_CdmAttach failed: please try setting environment variable GASNET_GNI_FMA_SHARING=1");
  }
#endif
  gasneti_assert_always (status == GNI_RC_SUCCESS);

  status = GNI_CqCreate(DOMAIN_SPECIFIC_VAL(nic_handle), num_cqe, 0, GNI_CQ_NOBLOCK, 
                        NULL, NULL, &DOMAIN_SPECIFIC_VAL(bound_cq_handle));
  gasneti_assert_always (status == GNI_RC_SUCCESS);
  /* create and bind endpoints */
  DOMAIN_SPECIFIC_VAL(peer_data) = gasneti_malloc(gasneti_nodes * sizeof(peer_struct_t));
  for (i = 0; i < gasneti_nodes; i += 1) {
  #if !GASNETC_LOCAL_GNI_EP
    if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(i)) continue; /* no connection to self or PSHM-reachable peers */
  #endif
   status = GNI_EpCreate(DOMAIN_SPECIFIC_VAL(nic_handle), DOMAIN_SPECIFIC_VAL(bound_cq_handle), 
                        &DOMAIN_SPECIFIC_VAL(peer_data[i].ep_handle));
   gasneti_assert_always (status == GNI_RC_SUCCESS);
#if 0 /* The following is the "proper" binding to "same-domain" peers... */
   status = GNI_EpBind(DOMAIN_SPECIFIC_VAL(peer_data[i].ep_handle),
                       gasnetc_cdom_data[0].peer_data[i].nic_addr,
                       i + GASNETC_DIDX * gasneti_nodes);
#else /* ...but bind to remote domain=0 is insensitive to having fewer domains on some nodes */
   status = GNI_EpBind(DOMAIN_SPECIFIC_VAL(peer_data[i].ep_handle),
                       gasnetc_cdom_data[0].peer_data[i].nic_addr,
                       i);
#endif
   gasneti_assert_always (status == GNI_RC_SUCCESS);
  }

  /* TODO: this replication is unnecessary, but cache-friendly: */
  for (i = 0; i < gasneti_nodes; ++i) {
    DOMAIN_SPECIFIC_VAL(peer_data[i]).aux_handle = gasnetc_cdom_data[0].peer_data[i].aux_handle;
    DOMAIN_SPECIFIC_VAL(peer_data[i]).mem_handle = gasnetc_cdom_data[0].peer_data[i].mem_handle;
    DOMAIN_SPECIFIC_VAL(peer_data[i]).pe = i;
  }

  gasnetc_init_post_descriptor_pool(GASNETC_DIDX_PASS_ALONE);
  gasnetc_init_bounce_buffer_pool(GASNETC_DIDX_PASS_ALONE);
 /* This should remain as the last statement to indicate that the domain is initialized. */
  gasneti_local_wmb();
  DOMAIN_SPECIFIC_VAL(initialized) = 1;
}
#endif

uintptr_t gasnetc_init_messaging(void)
{
  const gex_Rank_t remote_nodes = gasneti_nodes - (GASNET_PSHM ? gasneti_nodemap_local_count : 1);
  gni_return_t status;
  uint32_t local_address;
  uint32_t i;
  uint64_t request_map;
  size_t request_region_length = 0;
  size_t peer_stride;
  int notify_ring_size;
  int reply_count;
  int rvous_count = 0;

#if GASNETC_USE_MULTI_DOMAIN
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  gni_cdm_handle_t cdm_handle;
  gni_nic_handle_t nic_handle;
  gni_cq_handle_t bound_cq_handle;
  peer_struct_t *peer_data;
  gasnetc_domain_count = gasneti_getenv_int_withdefault("GASNET_DOMAIN_COUNT",
               GASNETC_DOMAIN_COUNT_DEFAULT,0);
  gasnetc_poll_am_domain_mask = gasneti_getenv_int_withdefault("GASNET_AM_DOMAIN_POLL_MASK",
               GASNETC_AM_DOMAIN_POLL_MASK_DEFAULT,0);
  if (gasnetc_poll_am_domain_mask) {
    for (i=2; i<gasnetc_domain_count; i<<=1) {
      gasnetc_poll_am_domain_mask = (gasnetc_poll_am_domain_mask << 1) | 1;
    }
  }
  unsigned int *all_domain_counts = gasneti_malloc(gasneti_nodes * sizeof(unsigned int));
  gasneti_spawner->Exchange(&gasnetc_domain_count, sizeof(unsigned int), all_domain_counts);
  gasnetc_domain_count_max = gasnetc_domain_count;
  for (i = 0; i < gasneti_nodes; ++i) {
    gasnetc_domain_count_max = MAX(gasnetc_domain_count_max, all_domain_counts[i]);
  }
  gasneti_free(all_domain_counts);
 #if (GASNETC_DOMAIN_THREAD_DISTRIBUTION == GASNETC_DOMAIN_THREAD_DISTRIBUTION_BULK)
  gasnetc_threads_per_domain =  gasneti_getenv_int_withdefault("GASNET_GNI_PTHREADS_PER_DOMAIN",
               GASNETC_PTHREADS_PER_DOMAIN_DEFAULT,0);
 #endif
  gasnetc_cdom_data = gasneti_malloc(gasnetc_domain_count * sizeof(communication_domain_struct_t));
  for(i=0;i<gasnetc_domain_count;i++)
    reset_comm_data(gasnetc_cdom_data+i);
#endif

  // Process GASNET_GNI_FMA_SHARING, if supported
  // Default is NO, unless we project resource exhaustion otherwise
  #if defined(GNI_CDM_MODE_FMA_SHARED) && defined(GNI_CDM_MODE_FMA_DEDICATED)
  {
    int my_cdm_count = gasneti_myhost.node_count * gasnetc_domain_count;
    int avail_cmd_count = 123; // TODO: reduce by any used by MPI?
    int fma_sharing = gasneti_getenv_yesno_withdefault("GASNET_GNI_FMA_SHARING",
                                                       my_cdm_count > avail_cmd_count);
    gasnetc_cdm_mode |= fma_sharing ? GNI_CDM_MODE_FMA_SHARED: GNI_CDM_MODE_FMA_DEDICATED;
  }
  #endif

  // Process GASNET_GNI_MDD_SHARING, if supported
  // Default is YES: use shared MDD to avoid likely resource exhaustion
  #if defined(GNI_CDM_MODE_MDD_SHARED) && defined(GNI_CDM_MODE_MDD_DEDICATED)
  {
    int mdd_sharing = gasneti_getenv_yesno_withdefault("GASNET_GNI_MDD_SHARING", 1);
    gasnetc_cdm_mode |= mdd_sharing ? GNI_CDM_MODE_MDD_SHARED: GNI_CDM_MODE_MDD_DEDICATED;
  }
  #endif

  // Process GASNET_GNI_BTE_MULTI_CHANNEL, if supported
  // Default is NO: use all available BTE channels
  #if defined(GNI_CDM_MODE_BTE_SINGLE_CHANNEL)
  {
    int bte_multi_channel = gasneti_getenv_yesno_withdefault("GASNET_GNI_BTE_MULTI_CHANNEL", 1);
    gasnetc_cdm_mode |= bte_multi_channel ? 0: GNI_CDM_MODE_BTE_SINGLE_CHANNEL;
  }
  #endif

  // Process GASNET_GNI_PACKEDLONG_CUTOVER
  // Default is arch-dependent
  gasnetc_packedlong_cutover = gasneti_getenv_int_withdefault("GASNET_GNI_PACKEDLONG_CUTOVER",
                                                              GASNETC_GNI_PACKEDLONG_CUTOVER_DEFAULT, 0);
  gasnetc_packedlong_cutover = MIN(GASNETC_MSG_MAXSIZE, gasnetc_packedlong_cutover);

  GASNETC_INITLOCK_GNI();
  GASNETC_INITLOCK_AM_BUFFER();

  status = GNI_CdmCreate(gasneti_mynode,
			 gasnetc_ptag, gasnetc_cookie,
			 gasnetc_cdm_mode,
			 &cdm_handle);
  gasneti_assert_always (status == GNI_RC_SUCCESS);

  status = GNI_CdmAttach(cdm_handle,
			 gasnetc_dev_id,
			 &local_address,
			 &nic_handle);
#if defined(GNI_CDM_MODE_FMA_DEDICATED)
  if_pf ((GNI_RC_ERROR_RESOURCE == status) &&
         (gasnetc_cdm_mode & GNI_CDM_MODE_FMA_DEDICATED)) {
    // TODO: supernode-scoped reduction followed by *automatic* enable of FMA sharing if needed
    gasneti_fatalerror("GNI_CdmAttach failed: please try setting environment variable GASNET_GNI_FMA_SHARING=1");
  }
#endif
  gasneti_assert_always (status == GNI_RC_SUCCESS);

  /* Determine number of Reply buffers */
  reply_count = gasneti_getenv_int_withdefault("GASNET_NETWORKDEPTH_TOTAL",
                                               GASNETC_NETWORKDEPTH_TOTAL_DEFAULT, 0);
  reply_count = MAX(1, reply_count); /* Min is 1 */
  reply_count = MIN(GASNETC_AM_INITIATOR_SLOTS, reply_count); /* Max determined by count of 'initiator_slot' */

  /* Select Eager or Rendezvous protocol for AM Requests */
  int am_rvous_val = gasneti_getenv_int_withdefault("GASNET_GNI_AM_RVOUS_CUTOVER",
                                                    GASNETC_GNI_AM_RVOUS_CUTOVER_DEFAULT, 0);
  am_rvous_enabled = (am_rvous_val && (gasneti_nodes >= am_rvous_val));

  /* Determine space/credits for AM Requests */
  if (am_rvous_enabled) {
    /* Rendezvous: GASNET_NETWORKDEPTH */
    GASNETI_TRACE_PRINTF(I, ("Using Rendezvous protocol for AM Requests"));
    am_maxcredit = gasneti_getenv_int_withdefault("GASNET_NETWORKDEPTH",
                                                  GASNETC_NETWORKDEPTH_DEFAULT, 0);
    am_maxcredit = MAX(1, am_maxcredit); /* Min is 1 */
    if (am_maxcredit > reply_count) {
      if (gasneti_mynode) {
        fprintf(stderr,
                "WARNING: Requested GASNET_NETWORKDEPTH %d reduced to GASNET_NETWORKDEPTH_TOTAL of %d\n",
                am_maxcredit, reply_count);
      }
      am_maxcredit = reply_count;
    }
    rvous_count = gasneti_getenv_int_withdefault("GASNET_GNI_AM_RVOUS_BUFFERS",
                                                 GASNETC_GNI_AM_RVOUS_BUFFERS_DEFAULT, 0);
    rvous_count = MAX(1, rvous_count); /* Min is 1 */

    request_bits = am_maxcredit;

    am_replysz = GASNETI_ALIGNUP(GASNETC_MSG_MAXSIZE, GASNETC_CACHELINE_SIZE); // No-op??
  } else {
    /* Eager: GASNET_NETWORKDEPTH_SPACE */
    GASNETI_TRACE_PRINTF(I, ("Using Eager protocol for AM Requests"));
    request_region_length = gasneti_getenv_int_withdefault("GASNET_NETWORKDEPTH_SPACE",
                                                           GASNETC_NETWORKDEPTH_SPACE_DEFAULT, 1);
    request_region_length = GASNETI_ALIGNUP(request_region_length, 64);
    request_region_length = MAX(request_region_length,  2*GASNETC_MSG_MAXSIZE);
    request_region_length = MIN(request_region_length, 64*GASNETC_MSG_MAXSIZE);
    am_slotsz = gasnetc_next_power_of_2(request_region_length / 64);
    am_slot_bits = ffs(am_slotsz) - 1;
    am_maxcredit = request_region_length / am_slotsz;
    request_region_length = am_maxcredit * am_slotsz;
    /* NOTE: 1<<64 is undefined and indeed icc yields 1.  So, we special case 64 credits */
    gasneti_assert(am_maxcredit >= 32);
    gasneti_assert(am_maxcredit <= 64);
    request_bits = am_maxcredit;

    // Clip credits to NETWORKDEPTH_TOTAL for use in computing size of Cq and notify ring
    am_maxcredit = MIN(am_maxcredit, reply_count);
    /* reply destination is also request source.  So, must fit largest *outgoing* message */
    am_replysz = GASNETI_ALIGNUP(GASNETC_MSG_MAXSIZE, am_slotsz);
  }

  // NOTE: 1<<64 is undefined and indeed icc yields 1.  So, we special case 64.
  request_map = (request_bits == 64) ? ~(uint64_t)0 : (((uint64_t)1 << request_bits) - 1);

  // Maximum number of Long requests outstanding per peer
  int am_long_depth = gasneti_getenv_int_withdefault("GASNET_LONG_DEPTH", am_maxcredit, 0);
  am_long_depth = MAX(am_long_depth, 1);            // Min is 1
  am_long_depth = MIN(am_long_depth, am_maxcredit); // Max is all AMs are Long

  { /* Determine Cq size: GASNET_GNI_NUM_PD */
    num_pd = gasneti_getenv_int_withdefault("GASNET_GNI_NUM_PD",
                                            GASNETC_GNI_NUM_PD_DEFAULT,0);
    num_pd = MAX(32, num_pd); /* Min is 32 (XXX: should be cores+1) */

    num_cqe = 2 * (num_pd + rvous_count + 1); /* XXX: why the +1 ?? */

    status = GNI_CqCreate(nic_handle, num_cqe, 0, GNI_CQ_NOBLOCK, NULL, NULL, &bound_cq_handle);
    gasneti_assert_always (status == GNI_RC_SUCCESS);
  }

  // Size per-peer notify ring: (Requests + Replies) rounded up to a power-of-two
  notify_ring_size = am_maxcredit + MIN(am_maxcredit, reply_count);
  notify_ring_size = GASNETI_ALIGNUP(gasnetc_next_power_of_2(notify_ring_size),
                                     (GASNETC_CACHELINE_SIZE / sizeof(gasnetc_notify_t)));
  notify_ring_mask = notify_ring_size - 1;

  /* Initialize the active message resources */

  /* gasnetc_log2_remote = MAX(1, ceil(log_2(remote_nodes))) */
  gasnetc_log2_remote = 1;
  for (i=2; i < remote_nodes; i*=2) {
    gasnetc_log2_remote += 1;
  }

  /*
   * allocate a CQ in which to receive message notifications
   * include logarithmic space for shutdown messaging
   */
  int am_num_cqe =
      GASNETI_ALIGNUP(gasnetc_log2_remote +         // for shutdown ctrl messages
                      2 * reply_count +             // for Replies (2 = header + Long payload)
                      remote_nodes * am_maxcredit + // for Request headers
                      remote_nodes * am_long_depth, // for Request Long payloads
                      2);                           // need it to be even
  status = GNI_CqCreate(nic_handle,am_num_cqe,0,GNI_CQ_NOBLOCK,NULL,NULL,&am_cq_handle);
  if (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort("GNI_CqCreate returned error %s", gasnetc_gni_rc_string(status));
  }
  
  /*
   * Set up an mmap region to contain all of my mailboxes.
   */

  const size_t rvous_region_length = rvous_count * am_replysz;
  const size_t reply_region_length = reply_count * am_replysz;
  const size_t peer_region_offset = reply_region_length + rvous_region_length;
  peer_stride = request_region_length + notify_ring_size * sizeof(gasnetc_notify_t);

  am_mmap_bytes = peer_region_offset + remote_nodes * peer_stride;
  gasneti_assert(am_mmap_ptr != (char *)MAP_FAILED);

  int report_to_stderr =
          (!gasneti_mynode && gasneti_getenv_yesno_withdefault("GASNET_GNI_AM_MEMORY_REPORT", 0));
am_memory_report:
  if (report_to_stderr || GASNETI_TRACE_ENABLED(I)) {
    #define DO_PRINT(...) do { \
        GASNETI_TRACE_PRINTF(I, (__VA_ARGS__));             \
        if (report_to_stderr) fprintf(stderr, __VA_ARGS__); \
      } while (0);
    char valstr1[32], valstr2[32];
    DO_PRINT("Fixed AM Memory used by this process:\n");
    gasnett_format_number(reply_region_length, valstr1, sizeof(valstr1), 1);
    DO_PRINT("  Outgoing buffers:    %s\n", valstr1);
    gasnett_format_number(rvous_region_length, valstr1, sizeof(valstr1), 1);
    DO_PRINT("  Rendezvous buffers:  %s\n", valstr1);
    DO_PRINT("Per-peer AM Memory used by this process:\n");
    gasnett_format_number(request_region_length, valstr1, sizeof(valstr1), 1);
    gasnett_format_number(request_region_length*remote_nodes, valstr2, sizeof(valstr2), 1);
    DO_PRINT("  Eager buffers:       %s\t(%s)\n", valstr1, valstr2);
    gasnett_format_number(sizeof(gasnetc_notify_t)*notify_ring_size, valstr1, sizeof(valstr1), 1);
    gasnett_format_number(sizeof(gasnetc_notify_t)*notify_ring_size*remote_nodes, valstr2, sizeof(valstr2), 1);
    DO_PRINT("  Notify ring:         %s\t(%s)\n", valstr1, valstr2);
    gasnett_format_number(am_mmap_bytes, valstr1, sizeof(valstr1), 1);
    DO_PRINT("TOTAL AM Memory used by this process: %s\n", valstr1);
    #undef DO_PRINT
  }

  if (am_mmap_ptr == (char *)MAP_FAILED) {
    gasnetc_GNIT_Abort("am mmap failed: ");
  }

#if defined(GASNETI_USE_HUGETLBFS)
  am_mmap_bytes = GASNETI_ALIGNUP(am_mmap_bytes, gethugepagesize());
  am_mmap_ptr = gasneti_huge_mmap(NULL, am_mmap_bytes);
#else
  am_mmap_bytes = GASNETI_PAGE_ALIGNUP(am_mmap_bytes);
  am_mmap_ptr = gasneti_mmap(am_mmap_bytes);
#endif

  if (am_mmap_ptr == (char *)MAP_FAILED) {
    // print report, aborting right after
    report_to_stderr = 1;
    goto am_memory_report;
  }
  
  {
    int count = 0;
    for (;;) {
      status = GNI_MemRegister(nic_handle, 
			       (unsigned long)am_mmap_ptr, 
			       am_mmap_bytes,
			       am_cq_handle,
			       GNI_MEM_STRICT_PI_ORDERING | GNI_MEM_PI_FLUSH | GNI_MEM_READWRITE,
			       -1,
			       &am_handle);
      if (status == GNI_RC_SUCCESS) break;
      if (status == GNI_RC_ERROR_RESOURCE) {
	gasnetc_GNIT_Log("MemRegister am fault %d at  %p %"PRIxPTR", code %s",
		count, am_mmap_ptr, (uintptr_t)am_mmap_bytes, gasnetc_gni_rc_string(status));
	count += 1;
	if (count >= 10) break;
      } else {
	break;
      }
    }
  }
  if (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort("GNI_MemRegister returned error %s",gasnetc_gni_rc_string(status));
  }

  /*
   * Initialize Reply pool
   */
  reply_pool = gasneti_malloc(reply_count * sizeof(reply_pool_t));
  for (i = 0; i < reply_count; i++) {
    reply_pool[i].u.next = &reply_pool[i + 1];
    reply_pool[i].packet = (gasnetc_packet_t *)((uintptr_t)am_mmap_ptr + i * am_replysz);
  }
  reply_freelist = reply_pool;
  reply_pool[reply_count - 1].u.next = NULL;

  /*
   * Initialize RVous pool
   */
  if (am_rvous_enabled) {
    uintptr_t packet_addr = (uintptr_t)am_mmap_ptr + reply_region_length;
    gasnetc_post_descriptor_t *gpd = (gasnetc_post_descriptor_t *)
                                     gasneti_calloc(rvous_count, sizeof(gasnetc_post_descriptor_t));
    for (i = 0; i < rvous_count; ++i, ++gpd) {
      gasneti_lifo_push(&am_rvous_pool, gpd);
      /* Init constant fields: */
      gpd->gpd_flags = GC_POST_COMPLETION_AMRV | GC_POST_KEEP_GPD;
    #if GASNETC_USE_MULTI_DOMAIN
      gpd->domain_idx = GASNETC_DEFAULT_DOMAIN;
    #endif
      gpd->pd.cq_mode = GNI_CQMODE_GLOBAL_EVENT;
      gpd->pd.dlvr_mode = GNI_DLVMODE_PERFORMANCE;
      gpd->pd.local_addr = (uint64_t) packet_addr;
      gpd->pd.local_mem_hndl = am_handle;
      packet_addr += am_replysz;
    }
    gasneti_assert(packet_addr == (uintptr_t)am_mmap_ptr + peer_region_offset);
  }

  peer_data = gasneti_malloc(gasneti_nodes * sizeof(peer_struct_t));

  /* Get my NIC address */
  if (gasnetc_dev_id == -1) {
    /* no value was found in environment */
    uint32_t cpu_id;

    gasnetc_dev_id  = 0;
    status = GNI_CdmGetNicAddress(gasnetc_dev_id, &gasnetc_address, &cpu_id);
    if (status != GNI_RC_SUCCESS) {
      gasnetc_GNIT_Abort("GNI_CdmGetNicAddress failed: %s", gasnetc_gni_rc_string(status));
    }
  } else {
    /* use gasnetc_address taken from the environment */
  }

  /* exchange peer data, create and bind endpoints and initialize am */
  { struct am_exchange { uint8_t *addr; gni_mem_handle_t handle; uint32_t nic_addr; };
    struct am_exchange my_am_exchg = { am_mmap_ptr, am_handle, gasnetc_address };
    struct am_exchange *all_am_exchg = gasneti_malloc(gasneti_nodes * sizeof(struct am_exchange));
    uint8_t *local_peer_base = (uint8_t *)am_mmap_ptr + peer_region_offset;

    gasneti_spawner->Exchange(&my_am_exchg, sizeof(struct am_exchange), all_am_exchg);
  
    /* At this point all_am_exchg has the required information for everyone */
    for (i = 0; i < gasneti_nodes; i += 1) {
    #if GASNETC_USE_MULTI_DOMAIN
      peer_data[i].nic_addr = all_am_exchg[i].nic_addr;
    #endif

    #if !GASNETC_LOCAL_GNI_EP
      if (!GASNETI_NBRHD_JOBRANK_IS_LOCAL(i)) /* no connection to self or PSHM-reachable peers */
    #endif
      {
        status = GNI_EpCreate(nic_handle, bound_cq_handle, &peer_data[i].ep_handle);
        gasneti_assert_always (status == GNI_RC_SUCCESS);
        status = GNI_EpBind(peer_data[i].ep_handle, all_am_exchg[i].nic_addr, i);
        gasneti_assert_always (status == GNI_RC_SUCCESS);
        peer_data[i].pe = i;
      }

      if (!GASNETI_NBRHD_JOBRANK_IS_LOCAL(i)) { /* no AMs to self or PSHM-reachable peers */
        peer_struct_t * const peer = &peer_data[i];
        uint8_t *remote_peer_base = all_am_exchg[i].addr + peer_stride * my_mb_index(i) + peer_region_offset;

        peer->event_count = 0;

        peer->am_handle = all_am_exchg[i].handle;
        peer->remote_reply_base = all_am_exchg[i].addr;
        peer->local_request_base = local_peer_base;
        peer->remote_request_base = remote_peer_base;
        peer->remote_notify_base = (gasnetc_notify_t *)(remote_peer_base + request_region_length);
        peer->local_notify_base = (gasnetc_notify_t *)(local_peer_base + request_region_length);
        peer->local_notify_read = 0;
        peer->remote_notify_write = 0;

      #if PLATFORM_COMPILER_CRAY
        /* Sigh.  Work around an apparent bug in cce-8.4.0.x.
         * This reference appears to prevent the compiler from discarding the initialization.
         */
        gasneti_assert_always (peer->local_notify_base);
      #endif

      #if GASNET_PAR
        peer->remote_request_lock = 0;
      #endif
        peer->remote_request_map = request_map;
        peer->long_credits = am_long_depth;
        local_peer_base += peer_stride;
      }
    }
    gasneti_free(all_am_exchg);
  }

  /* Create a temporary pool of post descriptors for uses prior to the aux seg attach.
   * NOTE: The immediate buffer WON'T be in registered memory - so no Put/Get.
   */
  for (i=0; i < gasnetc_log2_remote; ++i) {
    /* allocate individually to ease later destruction of this pool. */
    gasnetc_post_descriptor_t *gpd = gasneti_calloc(1, sizeof(gasnetc_post_descriptor_t));
    gasneti_lifo_push(&DOMAIN_SPECIFIC_VAL(post_descriptor_pool), gpd);
  }

#if GASNETC_USE_MULTI_DOMAIN 
  DOMAIN_SPECIFIC_VAL(cdm_handle) = cdm_handle;
  DOMAIN_SPECIFIC_VAL(nic_handle) = nic_handle;
  DOMAIN_SPECIFIC_VAL(bound_cq_handle) = bound_cq_handle;
  DOMAIN_SPECIFIC_VAL(peer_data) = peer_data;
  DOMAIN_SPECIFIC_VAL(threads_per_domain) = 1;
  DOMAIN_SPECIFIC_VAL(initialized) = 1;
#endif

  gasnetc_ampoll_burst = MAX(1,gasneti_getenv_int_withdefault("GASNET_GNI_AMPOLL_BURST",
                                                              GASNETC_GNI_AMPOLL_BURST_DEFAULT, 0));
#if GASNETI_USE_ALLOCA
  // precompute size of dynamic allocation
  gasnetc_ampoll_burst_sz = gasnetc_ampoll_burst * sizeof(gni_cq_entry_t);
#else
  // bound size of static allocation
  gasnetc_ampoll_burst = MIN(gasnetc_ampoll_burst, GASNETC_GNI_AMPOLL_BURST_MAX);
#endif

  return am_mmap_bytes;
}

void gasnetc_shutdown(void)
{
  int i;
  int tries;
  int left;
  gni_return_t status;
#if GASNETC_USE_MULTI_DOMAIN
  int didx;
#endif

  /* Do other threads need to be killed off here?
     release resources in the reverse order of acquisition
   */

#if GASNETC_GNI_UDREG
  if (have_auxseg && gasnetc_udreg_hndl) {
    udreg_return_t rc;
    rc = UDREG_CacheRelease(gasnetc_udreg_hndl);
    if (UDREG_RC_SUCCESS != rc) {
      gasnetc_GNIT_Log("UDREG_CacheRelease() failed with rc=%d", rc);
    }
    rc = UDREG_CacheDestroy(gasnetc_udreg_hndl);
    if (UDREG_RC_SUCCESS != rc) {
      gasnetc_GNIT_Log("UDREG_CacheDestroy() failed with rc=%d", rc);
    }
  }
#endif

  /* seize gni lock and hold it  */
  {  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
     GASNETC_LOCK_GNI();
  }

#if GASNETC_USE_MULTI_DOMAIN
  for (didx = 0; didx < gasnetc_domain_count; didx++) {
    GASNETC_DIDX_POST(didx);
    DOMAIN_SPECIFIC_VAR(gni_cdm_handle_t, cdm_handle);
    DOMAIN_SPECIFIC_VAR(gni_cq_handle_t, bound_cq_handle);
    DOMAIN_SPECIFIC_VAR(gni_nic_handle_t, nic_handle);
    DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);

    if (!DOMAIN_SPECIFIC_VAL(initialized)) continue;
    DOMAIN_SPECIFIC_VAL(initialized) = 0;
#endif

    /* for each connected rank */
    left = gasneti_nodes - (GASNET_PSHM ? gasneti_nodemap_local_count : 1);
    for (tries=0; tries<10; ++tries) {
      for (i = 0; i < gasneti_nodes; i += 1) {
      #if !GASNETC_LOCAL_GNI_EP
          if_pf (GASNETI_NBRHD_JOBRANK_IS_LOCAL(i)) continue; /* no connection to self or PSHM-reachable peers */
      #endif
          if_pt (peer_data[i].ep_handle != NULL) {
            status = GNI_EpUnbind( peer_data[i].ep_handle);
            status = GNI_EpDestroy( peer_data[i].ep_handle);
            if (status == GNI_RC_SUCCESS) {
              peer_data[i].ep_handle = NULL;
              left -= 1;
          }
        }
      }
      if (!left) break;
#if 0
      GASNETI_WAITHOOK();
      gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
#endif
    }
    if_pf (left > 0) {
      gasnetc_GNIT_Log("at shutdown: %d endpoints left after 10 tries", left);
    }

#if GASNETC_USE_MULTI_DOMAIN
    if (didx == GASNETC_DEFAULT_DOMAIN) {
#endif
#if defined(GASNETI_USE_HUGETLBFS)
      gasneti_huge_munmap(am_mmap_ptr, am_mmap_bytes);
#else
      gasneti_munmap(am_mmap_ptr, am_mmap_bytes);
#endif

      status = GNI_MemDeregister(nic_handle, &am_handle);
      if_pf (status != GNI_RC_SUCCESS) {
        gasnetc_GNIT_Log("MemDeregister(am_mem) failed with %s", gasnetc_gni_rc_string(status));
      }

      status = GNI_CqDestroy(am_cq_handle);
      if_pf (status != GNI_RC_SUCCESS) {
        gasnetc_GNIT_Log("CqDestroy(am_cq) failed with %s", gasnetc_gni_rc_string(status));
      }

      if_pt (have_segment) {
        status = GNI_MemDeregister(nic_handle, &my_mem_handle);
        if_pf (status != GNI_RC_SUCCESS) {
          gasnetc_GNIT_Log("MemDeregister(segment) failed with %s", gasnetc_gni_rc_string(status));
        }
      }

      if_pt (have_auxseg) {
        status = GNI_MemDeregister(nic_handle, &my_aux_handle);
        if_pf (status != GNI_RC_SUCCESS) {
          gasnetc_GNIT_Log("MemDeregister(auxseg) failed with %s", gasnetc_gni_rc_string(status));
        }
      }

    #if GASNETC_BUILD_GNICE
      if (gasnete_ce_available && ce_handle) {
        status = GNI_CeDestroy(ce_handle);
        if_pf (status != GNI_RC_SUCCESS) {
          gasnetc_GNIT_Log("CeDestroy() failed with %s", gasnetc_gni_rc_string(status));
        }
      }
    #endif
#if GASNETC_USE_MULTI_DOMAIN
    }
#endif

    status = GNI_CqDestroy(bound_cq_handle);
    if_pf (status != GNI_RC_SUCCESS) {
      gasnetc_GNIT_Log("CqDestroy(bound_cq) failed with %s", gasnetc_gni_rc_string(status));
    }

    status = GNI_CdmDestroy(cdm_handle);
    if_pf (status != GNI_RC_SUCCESS) {
      gasnetc_GNIT_Log("CdmDestroy(bound_cq) failed with %s", gasnetc_gni_rc_string(status));
    }
#if GASNETC_USE_MULTI_DOMAIN
  }
#endif

  {  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
     GASNETC_UNLOCK_GNI();
  }
}

extern void gasnetc_trace_finish(void) {
#if GASNETC_GNI_UDREG
  if (GASNETI_STATS_ENABLED(C) && gasnetc_udreg_hndl) {
    int max_memreg = MAX(1,gasneti_getenv_int_withdefault("GASNET_GNI_MEMREG", GASNETC_GNI_MEMREG_DEFAULT, 0));
    uint64_t hit = 0, miss = 0, evict = 0;
    (void)UDREG_GetStat(gasnetc_udreg_hndl, UDREG_STAT_CACHE_HIT, &hit);
    (void)UDREG_GetStat(gasnetc_udreg_hndl, UDREG_STAT_CACHE_MISS, &miss);
    (void)UDREG_GetStat(gasnetc_udreg_hndl, UDREG_STAT_CACHE_EVICTED, &evict);
    GASNETI_STATS_PRINTF(C,("UDREG size=%d hit/miss/evict: %"PRIu64"/%"PRIu64"/%"PRIu64"\n", max_memreg,
                            hit, miss, evict));
  }
#endif
}

static GASNETI_MALLOC
void *gasnetc_alloc_bounce_buffer(gex_Flags_t flags GASNETC_DIDX_FARG)
{
  gasneti_lifo_head_t * const pool_p = &DOMAIN_SPECIFIC_VAL(bounce_buffer_pool);
  void *buf = gasneti_lifo_pop(pool_p);
  if_pf (!buf) {
    /* We may simple not have polled the Cq recently.
       So, WAITHOOK and STALL tracing only if still nothing after first poll */
    GASNETC_TRACE_WAIT_BEGIN();
    gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
    buf = gasneti_lifo_pop(pool_p);
    if_pf (!buf && !(flags & GEX_FLAG_IMMEDIATE)) {
      do {
        GASNETI_WAITHOOK();
        gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
        buf = gasneti_lifo_pop(pool_p);
      } while (!buf);
      GASNETC_TRACE_WAIT_END(ALLOC_BB_STALL);
    }
  }
  return(buf);
}

GASNETI_INLINE(gasnetc_free_bounce_buffer)
void gasnetc_free_bounce_buffer(gasnetc_post_descriptor_t *gpd)
{
  void *gcb = (void *) gpd->pd.local_addr;
  GASNETC_DIDX_POST(gpd->domain_idx);
  gasneti_lifo_push(&DOMAIN_SPECIFIC_VAL(bounce_buffer_pool), gcb);
}

static gasneti_mutex_t ampoll_lock = GASNETI_MUTEX_INITIALIZER;

/* for local_notify_read which is kept in range [0..notify_ring_size) */
#define advance_notify_pointer(__x) (__x) = ((__x + 1) & notify_ring_mask)

/* for remote_notify_write which is not bounded */
#define fetch_inc_notify_pointer(__x) ((__x)++ & notify_ring_mask)

GASNETI_INLINE(gasnetc_send_am_common)
int gasnetc_send_am_common(peer_struct_t *peer, gni_post_descriptor_t *pd)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  int trial = 0;
  gni_return_t status;

  for (;;) {
    status = GNI_PostFma(peer->ep_handle, pd);
    GASNETC_UNLOCK_GNI();

    if_pt (status == GNI_RC_SUCCESS) {
      break;
    }

    if_pf (status != GNI_RC_ERROR_RESOURCE) {
      gasnetc_GNIT_Abort("PostFma for AM returned error %s", gasnetc_gni_rc_string(status));
    }

    if_pf (++trial == GASNETC_RESOURCE_RETRIES) {
      gasnetc_GNIT_Log("PostFma retry for AM failed");
      return GASNET_ERR_RESOURCE;
    }

    GASNETI_WAITHOOK();
    gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
    GASNETC_LOCK_GNI();
  }

  GASNETC_STAT_EVENT_VAL(AM_SEND_RETRY, trial);
  return GASNET_OK;
}

extern int
gasnetc_send_am(gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  gni_post_descriptor_t *pd = &gpd->pd;
  peer_struct_t * const peer = (peer_struct_t *)gpd->gpd_am_peer;
  gasnetc_notify_t notify = pd->sync_flag_value;
  unsigned int slot;

  GASNETI_TRACE_PRINTF(D, ("msg to %d type %s/%s\n", peer->pe,
                           gasnetc_type_string(gasnetc_am_command(notify)),
                           (gc_notify_get_type(notify) == gc_notify_request) ? "REQ" : "REP"));

  GASNETC_LOCK_GNI();
  
  slot = fetch_inc_notify_pointer(peer->remote_notify_write);
  gasnetc_notify_t *notify_addr = peer->remote_notify_base + slot;
  if (am_rvous_enabled && (gc_notify_get_type(notify) == gc_notify_request)) {
    gpd->u.notify = notify ^ (gc_notify_request ^ gc_notify_rvous);
    pd->type = GNI_POST_FMA_PUT;
    pd->length = sizeof(gasnetc_notify_t);
    pd->local_addr = (uint64_t) &gpd->u.notify;
    pd->remote_addr = (uint64_t) notify_addr;
  } else {
    gasneti_assert(pd->type == GNI_POST_FMA_PUT_W_SYNCFLAG);
    pd->sync_flag_addr = (uint64_t) notify_addr;
  }
  return(gasnetc_send_am_common(peer, pd));
}

GASNETI_INLINE(send_ctrl)
int send_ctrl(peer_struct_t * const peer, uint32_t value, gasneti_weakatomic_t *cntr)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor(0 GASNETC_DIDX_PASS);
  gni_post_descriptor_t *pd = &gpd->pd;

  if (cntr) {
    gpd->gpd_completion = (uintptr_t)cntr;
    gpd->gpd_flags = GC_POST_COMPLETION_CNTR;
  }
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT | GNI_CQMODE_REMOTE_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_mem_hndl = peer->am_handle;

  pd->type = GNI_POST_CQWRITE;
  pd->cqwrite_value = (uint64_t) value | gc_instid_ctrl;

  int trial = 0;
  gni_return_t status;

  for (;;) {
    GASNETC_LOCK_GNI();
    status = GNI_PostCqWrite(peer->ep_handle, pd);
    GASNETC_UNLOCK_GNI();

    if_pt (status == GNI_RC_SUCCESS) {
      break;
    }

    if_pf (status != GNI_RC_ERROR_RESOURCE) {
      gasnetc_GNIT_Abort("PostCqWrite for Ctrl returned error %s", gasnetc_gni_rc_string(status));
    }

    if_pf (++trial == GASNETC_RESOURCE_RETRIES) {
      gasnetc_GNIT_Log("PostCqWrite retry for Ctrl failed");
      return GASNET_ERR_RESOURCE;
    }

    GASNETI_WAITHOOK();
    gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
  }

  GASNETC_STAT_EVENT_VAL(CTRL_SEND_RETRY, trial);
  return GASNET_OK;
}

// Credit is a specific control message
GASNETI_INLINE(gasnetc_send_credit)
int gasnetc_send_credit(peer_struct_t * const peer, gasnetc_notify_t notify)
{
  GASNETI_TRACE_PRINTF(D, ("msg to %d type AM_CREDIT\n", peer->pe));
  gasneti_assert(gc_notify_get_type(notify) == gc_notify_request);
  uint32_t ctrl = gc_build_ctrl(GC_CTRL_CREDIT, gc_notify_get_initiator_slot(notify));
  return send_ctrl(peer, ctrl, NULL);
}

// Send a 3-byte control message (8-bit op and 16-bit arg)
GASNETI_INLINE(gasnetc_send_control)
int gasnetc_send_control(gex_Rank_t dest, uint8_t op, uint16_t arg, gasneti_weakatomic_t *cntr)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  peer_struct_t * const peer = &peer_data[dest];
  GASNETI_TRACE_PRINTF(D, ("msg to %d type CONTROL op=%d arg=0x%x\n",
                           (int)dest, (int)op, (int)arg));
  return(send_ctrl(peer, gc_build_ctrl(op, arg), cntr));
}

GASNETI_INLINE(gasnetc_format_am_gpd) 
void gasnetc_format_am_gpd(gasnetc_post_descriptor_t *gpd, 
                           gasnetc_packet_t *p,
                           peer_struct_t *peer,
                           size_t length, uint32_t gpd_flags)
{
  gni_post_descriptor_t *pd = &gpd->pd;

  gpd->gpd_flags = gpd_flags;
  gpd->gpd_am_peer = (uint64_t) peer; 
  pd->length = length;
  pd->local_addr = (uint64_t)p;
  pd->remote_mem_hndl = peer->am_handle;
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT | GNI_CQMODE_REMOTE_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->type = GNI_POST_FMA_PUT_W_SYNCFLAG;
}

gasnetc_post_descriptor_t *gasnetc_alloc_reply_post_descriptor(gex_Token_t t,
                                                               size_t length,
                                                               gex_Flags_t flags)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  gasnetc_token_t *token = (gasnetc_token_t *)t;
  peer_struct_t * const peer = &peer_data[token->source];
  gasnetc_notify_t notify = token->notify;
  gasnetc_packet_t *packet;
  uint32_t gpd_flags = 0;

 { // Start of scope: 'gpd'
  // Unlike the AMRequest path, it is safe (and easier) to acquire gpd first
  gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor(flags GASNETC_DIDX_PASS);
  if_pf (!gpd) goto out_immediate_1;

  /* Find space to construct the Reply.
   * NOTE that we don't *need* registered memory.
   * However all the memory pools we manage happen to be registered.
   */
  if (length <= GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE) {
    /* Use in-gpd buffer */
    packet = (gasnetc_packet_t *) gpd->u.immediate;
  } else if (am_rvous_enabled) {
    /* need a bounce buffer */
    packet = (gasnetc_packet_t *) gasnetc_alloc_bounce_buffer(flags GASNETC_DIDX_PASS);
    if_pf (!packet) goto out_immediate_2;
    gpd_flags = GC_POST_UNBOUNCE;
  } else {
    unsigned int req_len = 0;
    /* Try to reuse the Request buffer for the Reply */
    const int numargs = gasnetc_am_numargs(notify);
    uint32_t target_slot = gc_notify_get_target_slot(notify);
    packet = (gasnetc_packet_t *) (peer->local_request_base + (target_slot << am_slot_bits));

    switch (gasnetc_am_command(notify)) {
      case GC_CMD_AM_SHORT:
        req_len = GASNETC_HEADLEN(short, numargs);
        req_len = req_len ? req_len : 1; /* request never allocates zero */
        break;
      case GC_CMD_AM_MEDIUM: {
        if_pt (0 == gasnetc_am_nbytes(notify)) {
          /* We can reuse the Request buffer, since the Medium had no payload */
          /* TODO: also safe for "TAIL_REPLY" when implemented */
          req_len = GASNETC_HEADLEN(medium, numargs);
          req_len = req_len ? req_len : 1; /* request never allocates zero */
        }
        break;
      }
      case GC_CMD_AM_LONG:
        req_len = GASNETC_HEADLEN(long, numargs);
        break;
      case GC_CMD_AM_LONG_PACKED:
        req_len = GASNETC_HEADLEN(long, numargs) + packet->galp.data_length;
        break;
    }
    req_len = GASNETI_ALIGNUP(req_len, am_slotsz);

    if (length > req_len) {
      /* need a bounce buffer */
      packet = (gasnetc_packet_t *) gasnetc_alloc_bounce_buffer(flags GASNETC_DIDX_PASS);
      if_pf (!packet) goto out_immediate_2;
      gpd_flags = GC_POST_UNBOUNCE;
    }
  }

  /* modify the notify type and clear its AM header bits */
 { // Start of scope: 'pd'
  gni_post_descriptor_t *pd = &gpd->pd;
  gasneti_assert(gc_notify_get_type(notify) == gc_notify_request);
  pd->sync_flag_value = (notify & 0xffffffffUL) + gc_build_notify((gc_notify_reply - gc_notify_request),0,0);
  
  pd->remote_addr = (uint64_t) (peer->remote_reply_base +
                                am_replysz * gc_notify_get_initiator_slot(notify));
  gasnetc_format_am_gpd(gpd, packet, peer, length, gpd_flags);
  gasneti_assert(token->need_reply);
  token->need_reply = 0;
  /* If Medium payload is in-use, then defer sending Reply until Request returns (avoids overwrite race) */
  if (gasnetc_am_nbytes(notify) && (GC_CMD_AM_MEDIUM == gasnetc_am_command(notify))) {
    token->deferred_reply = gpd;
  }
  return gpd;
 } // End of scope: 'pd'

out_immediate_2:
  gasnetc_free_post_descriptor(gpd);
 } // End of scope: 'gpd'
out_immediate_1:
  return NULL;
}

// Spin poll until (!_condition), but with some added complications.
// The idea is that if _condition is initally true, in may become false after
// a single _poll if the resource(s) it requires are released by pending CQ
// events.  Only if the first _poll does *not* lead to a false (_condition)
// is this considered a "stall" which leads to calls to GASNETI_WAITHOOK() and
// tracing output giving the length of the stall once _condition is false.
// Additionally, _escape is an expression (with a goto) that allows for the
// premature exit in the case of GEX_FLAG_IMMEDIATE.
//
//  + The AM buffer lock is held on entry and exit
//    - Must hold AM buffer lock to evaluate _condution
//    - Must release AM buffer lock to call _poll
//  + Must evaluate _escapeN to allow for IMMEDIATE support
//    - _escape1 when _condition is first known to be false
//    - _escape2 if _condition is still false after an initial _poll
//  + If first _poll did not satisfy (!_condition) this is a stall event
//    - Will call WAITHOOK before each additional _poll
//    - Will _trace the stall time at end
//
#define BUSYWAIT(_condition, _escape1, _escape2, _poll, _trace)  \
    if_pf (_condition) {                     \
      _escape1;                              \
      GASNETC_TRACE_WAIT_BEGIN();            \
      GASNETC_UNLOCK_AM_BUFFER();            \
      _poll;                                 \
      GASNETC_LOCK_AM_BUFFER();              \
      if_pf (_condition) {                   \
        _escape2;                            \
        do {                                 \
          GASNETC_UNLOCK_AM_BUFFER();        \
          GASNETI_WAITHOOK();                \
          _poll;                             \
          GASNETC_LOCK_AM_BUFFER();          \
        } while (_condition);                \
        GASNETC_TRACE_WAIT_END(_trace);      \
      }                                      \
    }                   

// Length of longest run of 1s in peer's remote_request_map
static int
gasnetc_remote_slots_avail(peer_struct_t * const peer)
{
  int result;
  uint64_t map = peer->remote_request_map;
  // Each iteration reduces by one the length of every run of 1s
  for (result = 0; map; ++result) { map &= map << 1; }
  return result;
}

static int
gasnetc_remote_slot(peer_struct_t * const peer, const uint64_t mask)
{
  uint64_t map = peer->remote_request_map;
  int shift;

  for (shift = 0; map >= mask; ++shift, map >>= 1) {
    if ((map & mask) == mask) return shift;
  }
  return 64;
}

// Subject to specialization on:
//   isFixed: min_length == max_length from fixed-payload AM injection call
//   isLong:  A Long (not packed) request which must honor GASNET_LONG_DEPTH
GASNETI_INLINE(request_post_descriptor_inner)
gasnetc_post_descriptor_t *request_post_descriptor_inner(gex_Rank_t dest,
                                                         const int isFixed,
                                                         const int isLong,
                                                         size_t min_length,
                                                         size_t max_length,
                                                         gex_Flags_t flags
                                                         GASNETI_THREAD_FARG)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  peer_struct_t * const peer = &peer_data[dest];
  unsigned int my_slot;
  unsigned int remote_slot;
  reply_pool_t *r;
  gex_Flags_t imm_flag = flags & GEX_FLAG_IMMEDIATE;
#if GASNET_PAR
  int request_lock_held;
#endif

#if GASNETC_IMMEDIATE_AMPOLLS
  // BUSYWAIT may AMPoll at most once when IMMEDIATE flag is set
  int did_poll = 0;
  #define ESCAPE1(label) do { if (imm_flag && did_poll++) goto label; } while(0)
  #define ESCAPE2(label) do { if (imm_flag) goto label; } while(0)
#else
  // BUSYWAIT will never AMPoll when IMMEDIATE flag is set
  #define ESCAPE1(label) do { if (imm_flag) goto label; } while(0)
  #define ESCAPE2(label) gasneti_assert(!imm_flag)
#endif

  GASNETC_LOCK_AM_BUFFER();

#if GASNET_PAR
  /* Prevent starvation of large allocations by small ones. */
  /* Note that we spin here (*not* polling) IFF another thread *IS* spin polling */
  if_pf (peer->remote_request_lock) {
    if (imm_flag) goto out_immediate_1;
    do {
      GASNETC_UNLOCK_AM_BUFFER();
      while (peer->remote_request_lock) GASNETI_WAITHOOK();
      GASNETC_LOCK_AM_BUFFER();
    } while (peer->remote_request_lock);
  }
  peer->remote_request_lock = 1;
  request_lock_held = 1;
#endif

  if (isLong) { // Honor LONG_DEPTH
    gasneti_assert(isLong == 1); // must be 0 or 1 , since added later
    BUSYWAIT(!peer->long_credits,
           ESCAPE1(out_immediate_1_5),
           ESCAPE2(out_immediate_1_5),
           gasnetc_AMPoll(GASNETI_THREAD_PASS_ALONE),
           GET_AM_LONG_CREDIT_STALL);
    peer->long_credits -= 1;
    gasneti_assert(peer->long_credits >= 0);
  }

  uint64_t mask;
  size_t length;
 { // Start of scope: 'slots'
  unsigned int slots = MAX(1, ((min_length + am_slotsz - 1) >> am_slot_bits));
  if (am_rvous_enabled) {
    // All we count is credits (not size), but we need a remote slot number
    mask = 1;

    BUSYWAIT(((remote_slot = gasnetc_remote_slot(peer, mask)) == 64),
           ESCAPE1(out_immediate_2),
           ESCAPE2(out_immediate_2),
           gasnetc_AMPoll(GASNETI_THREAD_PASS_ALONE),
           GET_AM_REM_BUFFER_STALL);

    length = max_length;
  } else if (isFixed || (min_length == max_length)) { // Fixed Payload (or effectively so)
    gasneti_assert_uint(slots ,<=, request_bits/2);
    mask = (((uint64_t)1 << slots) - 1);

    BUSYWAIT(((remote_slot = gasnetc_remote_slot(peer, mask)) == 64),
           ESCAPE1(out_immediate_2),
           ESCAPE2(out_immediate_2),
           gasnetc_AMPoll(GASNETI_THREAD_PASS_ALONE),
           GET_AM_REM_BUFFER_STALL);

    length = min_length;
  } else {
    unsigned int avail_slots;

    BUSYWAIT(((avail_slots = gasnetc_remote_slots_avail(peer)) < slots),
           ESCAPE1(out_immediate_2),
           ESCAPE2(out_immediate_2),
           gasnetc_AMPoll(GASNETI_THREAD_PASS_ALONE),
           GET_AM_REM_BUFFER_STALL);

    unsigned int max_slots = MAX(1, ((max_length + am_slotsz - 1) >> am_slot_bits));
    slots = MIN(avail_slots, max_slots);
    length = slots << am_slot_bits;

    mask = (slots == 64) ? ~(uint64_t)0 : (((uint64_t)1 << slots) - 1);
    remote_slot = gasnetc_remote_slot(peer, mask);
    gasneti_assert(remote_slot != 64);
  }
 } // End of scope: 'slots'

  mask <<= remote_slot;
  peer->remote_request_map ^= mask; // Claim slot(s)

#if GASNET_PAR
  peer->remote_request_lock = 0;
  request_lock_held = 0;
#endif

  BUSYWAIT(((r = reply_freelist) == NULL), 
           ESCAPE1(out_immediate_3),
           ESCAPE2(out_immediate_3),
           gasnetc_AMPoll(GASNETI_THREAD_PASS_ALONE),
           GET_AM_LOC_BUFFER_STALL);
  reply_freelist = r->u.next;

  GASNETC_UNLOCK_AM_BUFFER();

 { // Start of scope: 'gpd'
  gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor(flags GASNETC_DIDX_PASS);
  if_pf (!gpd) goto out_immediate_4;
  gasnetc_format_am_gpd(gpd, r->packet, peer, length, 0);

  gni_post_descriptor_t *pd = &gpd->pd;
  pd->remote_addr = (uint64_t) peer->remote_request_base + (remote_slot << am_slot_bits);
  pd->sync_flag_value = gc_build_notify(gc_notify_request, r - reply_pool, remote_slot);

  r->u.credit.peer = peer;
  r->u.credit.value = mask;
  r->u.credit.isLong = isLong;
  
  return gpd;
 } // End of scope: 'gpd'

out_immediate_4:
  GASNETC_LOCK_AM_BUFFER();
    // Return reply state to freelist
    r->u.next = reply_freelist;
    reply_freelist = r;
out_immediate_3:
    // Restore bits corresponding to remote buffer allocation
    peer->remote_request_map ^= mask;
out_immediate_2:
    // LONG_DEPTH credit, if any
    peer->long_credits += isLong;
out_immediate_1_5:
  #if GASNET_PAR
    // Possibly release our lock on the per-peer remote buffer allocator
    if (request_lock_held) peer->remote_request_lock = 0;
  #endif
out_immediate_1:
  GASNETC_UNLOCK_AM_BUFFER();

  return NULL;
}

gasnetc_post_descriptor_t *
gasnetc_alloc_request_post_descriptor(
                        gex_Rank_t dest,
                        size_t length,
                        gex_Flags_t flags
                        GASNETI_THREAD_FARG)
{
  return request_post_descriptor_inner(dest, 1, 0, length, length, flags GASNETI_THREAD_PASS);
}

gasnetc_post_descriptor_t *
gasnetc_alloc_request_post_descriptor_long(
                        gex_Rank_t dest,
                        size_t length,
                        gex_Flags_t flags,
                        int is_packed
                        GASNETI_THREAD_FARG)
{
  return request_post_descriptor_inner(dest, 1, !is_packed, length, length, flags GASNETI_THREAD_PASS);
}

#if GASNETC_NP_MEDXL // NP Medium beyond MaxMedium - disabled by default
static gasneti_lifo_head_t medxl_descriptor_pool = GASNETI_LIFO_INITIALIZER;
#endif

gasnetc_post_descriptor_t *
gasnetc_alloc_request_post_descriptor_np(
                        gex_Rank_t dest,
                        size_t min_length,
                        size_t max_length,
                        gex_Flags_t flags
                        GASNETI_THREAD_FARG)
{
#if GASNETC_NP_MEDXL
  gasnetc_post_descriptor_t *gpd =
    request_post_descriptor_inner(dest, 0, 0, min_length, max_length, flags GASNETI_THREAD_PASS);
  if (gpd && (gpd->pd.length > GASNETC_MSG_MAXSIZE)) {
    // We have a "extra large" landing zone on the peer, but the gpd has a
    // source buffer of at most GASNETC_MSG_MAXSIZE.  We need an alternate.
    void *buf = gasneti_lifo_pop(&medxl_descriptor_pool);
    if_pf (! buf) buf = gasneti_malloc(am_maxcredit << am_slot_bits);
    gpd->pd.local_addr = (uint64_t) buf;
    gpd->gpd_flags |= GC_POST_UNPREPARE;
  }
  return gpd;
#else
  // TODO-EX: cannot negotiate larger than MaxMedium until/unless reply_pool is over-sized too
  // We cannot send 65536 bytes in a 16-bit field (bug 4042)
  max_length = MIN(max_length, MIN(GASNETC_MSG_MAXSIZE,65535));
  return request_post_descriptor_inner(dest, 0, 0, min_length, max_length, flags GASNETI_THREAD_PASS);
#endif
}

/* Choice to inline or not is left to the compiler */
void gasnetc_recv_am_unlocked(peer_struct_t * const peer, gasnetc_packet_t * const packet,
                     gasnetc_notify_t notify GASNETI_THREAD_FARG)
{
  gasneti_mutex_assertunlocked(&ampoll_lock);

  int is_req = (gc_notify_get_type(notify) == gc_notify_request);
  const int numargs = gasnetc_am_numargs(notify);
  const int handlerindex = gasnetc_am_handler(notify);
  const gex_AM_Entry_t * const handler_entry = &gasnetc_handler[handlerindex];
  gex_AM_Fn_t handler = handler_entry->gex_fnptr;
  gasnetc_token_t the_token = { peer->pe, handler_entry, is_req, notify, NULL };
#if GASNETI_THREADINFO_OPT
  the_token.threadinfo = GASNETI_MYTHREAD;
#endif
  gex_Token_t token = (gex_Token_t)&the_token; /* RUN macros need an lvalue */

  gasneti_assert(numargs <= gex_AM_MaxArgs());
  GASNETI_TRACE_PRINTF(D, ("msg from %d type %s/%s\n", peer->pe,
                           gasnetc_type_string(gasnetc_am_command(notify)),
                           is_req ? "REQ" : "REP"));

  switch (gasnetc_am_command(notify)) {
  case GC_CMD_AM_SHORT:
      gasneti_amtbl_check(handler_entry, numargs, gasneti_Short, is_req);
      GASNETI_RUN_HANDLER_SHORT(is_req, handlerindex, handler,
                                token, packet->gasp.args, numargs);
      break;
      
  case GC_CMD_AM_MEDIUM: {
      gasneti_amtbl_check(handler_entry, numargs, gasneti_Medium, is_req);
      const size_t head_len = GASNETC_HEADLEN(medium, numargs);
      uint8_t * data = (uint8_t *)packet + head_len;
      gasneti_assert(0 == (((uintptr_t) data) % GASNETI_MEDBUF_ALIGNMENT));
      GASNETI_RUN_HANDLER_MEDIUM(is_req, handlerindex, handler,
                                 token, packet->gamp.args, numargs,
                                 data, gasnetc_am_nbytes(notify));
      break;
  }
      
  case GC_CMD_AM_LONG_PACKED:
      { /* payload follows args - copy it into place */
          const size_t head_len = GASNETC_HEADLEN(long, numargs);
          uint8_t * data = (uint8_t *)packet + head_len;
          gasneti_assert(head_len + packet->galp.data_length <= GASNETC_MSG_MAXSIZE);
          GASNETI_MEMCPY(packet->galp.data, data, packet->galp.data_length);
      }
      GASNETI_FALLTHROUGH
  case GC_CMD_AM_LONG:
      gasneti_amtbl_check(handler_entry, numargs, gasneti_Long, is_req);
      GASNETI_RUN_HANDLER_LONG(is_req, handlerindex, handler,
                               token, packet->galp.args, numargs,
                               packet->galp.data, packet->galp.data_length);
      break;
      
#if GASNET_DEBUG
  default:
      gasnetc_GNIT_Abort("unknown packet type");
#endif
  }

  /* TODO: gasneti_suspend_spinpollers()? */
  if (the_token.need_reply) {
      gasnetc_send_credit(peer, notify);
  } else if (the_token.deferred_reply) {
      gasnetc_send_am(the_token.deferred_reply);
  }
}

GASNETI_INLINE(gasnetc_recv_am)
void gasnetc_recv_am(peer_struct_t * const peer, gasnetc_packet_t * const packet,
                     gasnetc_notify_t notify, reply_pool_t *reply
                     GASNETI_THREAD_FARG)
{
  gasneti_mutex_unlock(&ampoll_lock);
  gasnetc_recv_am_unlocked(peer, packet, notify GASNETI_THREAD_PASS);
  gasneti_mutex_lock(&ampoll_lock);
  if (reply) {
    GASNETC_LOCK_AM_BUFFER();
    peer->remote_request_map ^= reply->u.credit.value;
    peer->long_credits += reply->u.credit.isLong;
    reply->u.next = reply_freelist;
    reply_freelist = reply;
    GASNETC_UNLOCK_AM_BUFFER();
  }
}

static peer_struct_t *ampoll_head = NULL;
static peer_struct_t *ampoll_tail = NULL;

/* Move peer (which must be at head now), to be tail */
GASNETI_INLINE(ampoll_last)
void ampoll_last(peer_struct_t *peer)
{
  gasneti_assert(ampoll_head == peer);
  gasneti_assert(0 != peer->event_count);
  ampoll_head = peer->next;
  ampoll_tail = peer;
}

/* Remove peer (which must be at head now) from polling set */
GASNETI_INLINE(ampoll_del)
void ampoll_del(peer_struct_t *peer)
{
  gasneti_assert(ampoll_head == peer);
  if (0 == --peer->event_count) {
    ampoll_head = ampoll_tail->next = (peer == ampoll_tail) ? NULL : peer->next;
    /* NOTE: tail is now undefined if head became NULL */
  }
}

/* Add peer to polling set */
GASNETI_INLINE(ampoll_ins)
void ampoll_ins(peer_struct_t *peer)
{
  GASNETI_TRACE_EVENT(D, AMPOLL_INS);
  if (0 == peer->event_count++) {
    if (NULL == ampoll_head) {
      ampoll_head = peer;
    } else {
      ampoll_tail->next = peer;
    }
    ampoll_tail = peer;
    peer->next = ampoll_head;
  }
}

static void gasnetc_handle_sys_shutdown_packet(uint16_t arg);

GASNETI_INLINE(dispatch_ctrl)
void dispatch_ctrl(uint32_t value)
{
  const uint16_t arg = gc_ctrl_arg(value);
  const uint8_t op = gc_ctrl_op(value);
      
  switch (op) {
    case GC_CTRL_CREDIT: {
      GASNETI_TRACE_PRINTF(D, ("AM_CREDIT for slot %d\n", arg));
      reply_pool_t *reply = reply_pool + arg;
      peer_struct_t *peer = reply->u.credit.peer;

      GASNETC_LOCK_AM_BUFFER();
      peer->remote_request_map ^= reply->u.credit.value;
      peer->long_credits += reply->u.credit.isLong;
      reply->u.next = reply_freelist;
      reply_freelist = reply;
      GASNETC_UNLOCK_AM_BUFFER();
      break;
    }

    case GC_CTRL_SHUTDOWN:
      gasnetc_handle_sys_shutdown_packet(arg);
      break;

    default:
      gasneti_unreachable();
  }
}

// Process an incoming AM rendezvous
static void am_rvous_get(peer_struct_t * const peer, gasnetc_notify_t notify GASNETI_THREAD_FARG);

//
// Matching of Long payload and header
//
// Data structures are protected by ampoll_lock, in the same critical
// section protecting other polling related data.
//

typedef struct long_match_t_ {
  struct long_match_t_ *next;
  uint32_t longid; // The key
  peer_struct_t * peer;
  gasnetc_packet_t * packet;
  gasnetc_notify_t notify;
} long_match_t;

// Chained-bucket hash table
// Access serialized by ampoll_lock
#ifndef LONG_MATCH_TBL_SZ
#define LONG_MATCH_TBL_SZ 64
#endif
static long_match_t * long_match_tbl[LONG_MATCH_TBL_SZ];

// Free list of buckets
// Access serialized by ampoll_lock
static long_match_t * long_match_pool = NULL;

// TODO: cache-align when PAR?
GASNETI_NEVER_INLINE(long_match_grow,
static long_match_t *long_match_grow(void))
{
  const int n = 8; // entries allocated per call
  long_match_t *p = gasneti_malloc(n * sizeof(long_match_t));
  p[0].next = NULL;
  for (int i = 1; i < n-1; ++i) {
    p[i].next = &p[i + 1];
  }
  p[n-1].next = NULL;
  long_match_pool = &p[1];
  return p;
}

GASNETI_INLINE(long_match_alloc)
long_match_t *long_match_alloc(void)
{
  long_match_t *result = long_match_pool;
  if (result) {
    long_match_pool = result->next;
  } else {
    result = long_match_grow();
  }
  return result;
}

GASNETI_INLINE(long_match_free)
void long_match_free(long_match_t *p)
{
  p->next = long_match_pool;
  long_match_pool = p;
}

GASNETI_INLINE(long_match_list_head)
long_match_t **long_match_list_head(uint32_t longid)
{
  uint32_t jobrank = longid & (GASNET_MAXNODES - 1);
  uint32_t nonce   = longid >> 24;
  uint32_t slot    = (jobrank ^ nonce) % LONG_MATCH_TBL_SZ;
  return long_match_tbl + slot;
}

#define make_longid(jobrank, n) (jobrank | (uint32_t)gc_notify_get_nonce(n))

// If header entry exists, removes the entry and runs the handler (Eager)
// or initiates the Get of the full header (RVous)
// Otherwise creates a new entry
static void long_match_payload(uint32_t longid GASNETI_THREAD_FARG)
{
  gasneti_mutex_assertlocked(&ampoll_lock);
  long_match_t **prev_p = long_match_list_head(longid);
  long_match_t *curr = *prev_p;
  while (curr) {
    if (curr->longid == longid) {
      gasneti_assert(curr->peer); // detect payload matched to payload
      peer_struct_t * const peer = curr->peer;
      gasnetc_packet_t * const packet = curr->packet;
      gasnetc_notify_t const notify = curr->notify;
      *prev_p = curr->next;
      long_match_free(curr);

      reply_pool_t *reply = NULL;
      if (gc_notify_get_type(notify) == gc_notify_reply) {
        reply = reply_pool + gc_notify_get_initiator_slot(notify);
      } else if (am_rvous_enabled) {
        am_rvous_get(peer, notify GASNETI_THREAD_PASS);
        return;
      }

      gasnetc_recv_am(peer, packet, notify, reply GASNETI_THREAD_PASS);
      return;
    }
    prev_p = &curr->next;
    curr = curr->next;
  }
  *prev_p = curr = long_match_alloc();
  curr->next = NULL;
  curr->longid = longid;
#if GASNET_DEBUG
  curr->peer = NULL;
#endif
}

// If payload entry exists, removes the entry, and returns 1
// Otherwise creates a new entry and returns 0
static int long_match_header(
                peer_struct_t * const peer,
                gasnetc_packet_t * const packet,
                gasnetc_notify_t notify
                GASNETI_THREAD_FARG)
{
  gasneti_mutex_assertlocked(&ampoll_lock);
  uint32_t longid = make_longid(peer->pe, notify);
  long_match_t **prev_p = long_match_list_head(longid);
  long_match_t *curr = *prev_p;
  while (curr) {
    if (curr->longid == longid) {
      gasneti_assert(! curr->peer); // detect header matched to header
      *prev_p = curr->next;
      long_match_free(curr);
      return 1;
    }
    prev_p = &curr->next;
    curr = curr->next;
  }
  *prev_p = curr = long_match_alloc();
  curr->next   = NULL;
  curr->longid = longid;
  curr->peer   = peer;
  curr->packet = packet;
  curr->notify = notify;
  return 0;
}

// Data for AM rendevous
static gasneti_mutex_t am_rvous_lock = GASNETI_MUTEX_INITIALIZER;
// Protected by am_rvous_lock:
static am_rvous_t *am_rvous_head;
static am_rvous_t **am_rvous_tail_p = &am_rvous_head;
// Not lock protected:
static volatile int am_rvous_ready;

// Run ready AMs, if any
GASNETI_INLINE(am_rvous_run)
void am_rvous_run(GASNETI_THREAD_FARG_ALONE)
{
  if (!am_rvous_ready) return; // Unlocked check (the only check in SEQ mode)
  gasneti_assert(am_rvous_enabled);

#if GASNET_PAR
  if (gasneti_mutex_trylock(&am_rvous_lock)) return; // Busy
  if (!am_rvous_ready) {
    gasneti_mutex_unlock(&am_rvous_lock);
    return;
  }
#endif

  am_rvous_ready = 0;  // Clear flag first (so race favors extra polls, not lost ones)
  gasneti_sync_reads();

  // Traverse entire list with lock held, removing ready ones
  am_rvous_t *ready_list;
  am_rvous_t **ready_tail_p = &ready_list;
  am_rvous_t *curr = am_rvous_head;
  am_rvous_t **prev_p = &am_rvous_head;
  while (curr) {
    am_rvous_t *next = curr->next;
    if (curr->ready) {
      // Remove from live list
      (*prev_p) = next;
      // Append to ready list
      (*ready_tail_p) = curr;
      ready_tail_p = &curr->next;
    } else {
      prev_p = &curr->next;
    }
    curr = next;
  }
  *ready_tail_p = NULL;     // Terminate ready list
  am_rvous_tail_p = prev_p; // Correct the tail of live list
#if GASNET_PAR
  gasneti_mutex_unlock(&am_rvous_lock);
#endif

  if (ready_list) {
    // Run ready ones (without lock held)
    curr = ready_list;
    do {
      gasnetc_post_descriptor_t *gpd = gasneti_container_of(curr, gasnetc_post_descriptor_t, u.am_rvous);
      gasneti_assert(gpd->gpd_flags == (GC_POST_COMPLETION_AMRV | GC_POST_KEEP_GPD));
      gasnetc_recv_am_unlocked(curr->peer, (void*) gpd->pd.local_addr, curr->notify GASNETI_THREAD_PASS);
      curr = curr->next;
      gasneti_lifo_push(&am_rvous_pool, gpd);
    } while (curr);
  }
}

GASNETI_INLINE(poll_for_message)
int poll_for_message(peer_struct_t * const peer, int is_slow GASNETI_THREAD_FARG)
{
  volatile gasnetc_notify_t * const notify = peer->local_notify_base + peer->local_notify_read;
  gasnetc_notify_t n = *notify;

  if (n) { 
    uint32_t target_slot = gc_notify_get_target_slot(n);
    uint32_t initiator_slot = gc_notify_get_initiator_slot(n);
    uint32_t type = gc_notify_get_type(n);

    *notify = 0;
    advance_notify_pointer(peer->local_notify_read);
    if (is_slow) ampoll_del(peer);

    gasneti_compiler_fence(); /* prevent compiler from prefetching over dependency on n!=0 */
    
    if (type == gc_notify_rvous) {
      // Replace the notify by its "effective" value:
      n ^= (gc_notify_request ^ gc_notify_rvous);
      if ((gasnetc_am_command(n) != GC_CMD_AM_LONG) ||
          long_match_header(peer, NULL, n GASNETI_THREAD_PASS)) {
        am_rvous_get(peer, n GASNETI_THREAD_PASS);
      }
    } else {
      reply_pool_t *reply;
      gasnetc_packet_t *packet;

      if (type == gc_notify_request) {
        reply = NULL;
        packet = (gasnetc_packet_t *) (peer->local_request_base + (target_slot << am_slot_bits));
      } else {
        gasneti_assert(type == gc_notify_reply);
        reply = reply_pool + initiator_slot;
        packet = reply->packet;
      }

      if ((gasnetc_am_command(n) != GC_CMD_AM_LONG) ||
          long_match_header(peer, packet, n GASNETI_THREAD_PASS)) {
        gasnetc_recv_am(peer, packet, n, reply GASNETI_THREAD_PASS);
      }
    }
    return 1;
  }
  return 0;
}

static
void gasnetc_poll_am_queue(GASNETI_THREAD_FARG_ALONE)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
#if GASNETI_USE_ALLOCA
  gni_cq_entry_t *event_data = alloca(gasnetc_ampoll_burst_sz);
#else
  gni_cq_entry_t event_data[GASNETC_GNI_AMPOLL_BURST_MAX];
#endif
  int count = 0;
  int i;

  // Run handlers for completed rvous-gets, if any
  am_rvous_run(GASNETI_THREAD_PASS_ALONE);

  /* Reap Cq entries until our queue is full, or Cq is empty */
#if GASNET_PAR
  /* Cray has assured us that GNI_CqTestEvent() requires no serialization */
  if (GNI_RC_NOT_DONE != GNI_CqTestEvent(am_cq_handle))
#endif
  {
  GASNETC_LOCK_GNI();
    for (count = 0; count < gasnetc_ampoll_burst; ++count) {
      gni_return_t status = GNI_CqGetEvent(am_cq_handle, &event_data[count]);
      if (status != GNI_RC_SUCCESS) break; /* TODO: check for fatal errors */
      gasneti_assert(!GNI_CQ_OVERRUN(event_data[count]));
    }
  GASNETC_UNLOCK_GNI();
  }

  if (count) {
    GASNETI_TRACE_EVENT_VAL(D,AMPOLL_EVENTS,count);

    /* Must take the lock to process new events */
    gasneti_mutex_lock(&ampoll_lock);

    for (i = 0; i < count; ++i) {
    #ifdef GNI_CQ_GET_REM_INST_ID
      /* Mar 2013 (S-2446-5002) docs introduces this call for use on recv CQs ... */
      uint32_t inst_id = GNI_CQ_GET_REM_INST_ID(event_data[i]);
    #else
      /* ... while prior versions say this is used on both send and recv CQs */
      uint32_t inst_id = GNI_CQ_GET_INST_ID(event_data[i]);
    #endif
      switch (inst_id & GC_INSTID_MASK) {
        case gc_instid_ctrl:
          // CQWRITE of a control message
          dispatch_ctrl(inst_id);
          break;

        case gc_instid_header:
        { // FMA_PUT_W_SYNCFLAG of an AM Header
          gasneti_assert(inst_id < gasneti_nodes);
          peer_struct_t * const peer = &peer_data[inst_id];
          if (!poll_for_message(peer, 0 GASNETI_THREAD_PASS)) {
            ampoll_ins(peer);
          }
          break;
        }

        default:
          // FMA_PUT or RDMA_PUT of Long payload
          long_match_payload(inst_id GASNETI_THREAD_PASS);
      }
    }
  } else if ((NULL == ampoll_head) || (EBUSY == gasneti_mutex_trylock(&ampoll_lock))) {
    /* Either there is no work to be done, or another thread is already doing it */
    return;
  }

  /* Poll "slow" sources, starting with the oldest */
  for (i = 0; ampoll_head && (i < gasnetc_ampoll_burst); ++i) {
    peer_struct_t * const peer = ampoll_head;
    if (!poll_for_message(peer, 1 GASNETI_THREAD_PASS)) {
      if (peer == ampoll_tail) break; /* don't spin on singleton peer */
      ampoll_last(peer);
    }
  }

  gasneti_mutex_unlock(&ampoll_lock);
}

/* Poll the bound_ep completion queue */
GASNETI_INLINE(gasnetc_poll_bound_cq)
gasnetc_post_descriptor_t *gasnetc_poll_bound_cq(gni_cq_handle_t bound_cq_handle)
{
  gni_post_descriptor_t * result = NULL;
  gni_cq_entry_t event_data;
  gni_return_t status;

  status = GNI_CqGetEvent(bound_cq_handle,&event_data);
  if (status == GNI_RC_NOT_DONE) { /* empty queue is most common case */
    /* nothing */
  } else if_pt (status == GNI_RC_SUCCESS) {
    gasneti_assert(!GNI_CQ_OVERRUN(event_data));

    status = GNI_GetCompleted(bound_cq_handle, event_data, &result);
    if_pf (status != GNI_RC_SUCCESS) {
      gasnetc_GNIT_Abort("GetCompleted(%p) failed %s",
                         (void *) event_data, gasnetc_gni_rc_string(status));
    }
  } else if (!gasnetc_shutdownInProgress) {
    gasneti_assert(!GNI_CQ_OVERRUN(event_data));
    gasnetc_GNIT_Abort("bound CqGetEvent %s", gasnetc_gni_rc_string(status));
  }

  return result ? gasneti_container_of(result, gasnetc_post_descriptor_t, pd) : NULL;
}

GASNETI_NEVER_INLINE(gasnetc_poll_local_queue,
void gasnetc_poll_local_queue(GASNETC_DIDX_FARG_ALONE))
{
#if GASNETC_USE_MULTI_DOMAIN
  if_pf (!DOMAIN_SPECIFIC_VAL(initialized)) return;
#endif

  DOMAIN_SPECIFIC_VAR(gni_cq_handle_t, bound_cq_handle);

  for (int i = 0; i < gasnetc_poll_burst; i += 1) {
    gasnetc_post_descriptor_t *gpd;
    uint32_t gpd_flags;

    GASNETC_LOCK_GNI();
again:
      gpd = gasnetc_poll_bound_cq(bound_cq_handle);

      // Handle local events with lock still held
      if (gpd && (gpd->pd.cq_mode & GNI_CQMODE_LOCAL_EVENT)) {
        // TODO-EX: Could handle GC_POST_UNREGISTER here, earlier than the GLOBAL_EVENT.
        // However, that would probably require dropping the lock.
        gasneti_assert(gpd->pd.type == GNI_POST_RDMA_PUT);
        gasneti_assert(gpd->gpd_put_lc);

        gpd_flags = gpd->gpd_flags;
        if (gpd_flags & GC_POST_COMPLETION_FLAG) {
          *(volatile int *) gpd->gpd_put_lc = 1;
        } else
        if (gpd_flags & (GC_POST_COMPLETION_EOP | GC_POST_COMPLETION_EAM)) {
          GASNETC_EOP_ALC_FINISH((gasnete_eop_t *) gpd->gpd_put_lc);
        } else
        if (gpd_flags & (GC_POST_COMPLETION_IPUT | GC_POST_COMPLETION_IAM)) {
          if (gpd_flags & GC_POST_LC_NOW) { // GEX_EVENT_NOW for IOP (non-atomic)
            * (volatile gasneti_weakatomic_val_t *) gpd->gpd_put_lc += 1;
          } else {
            GASNETE_IOP_CNT_FINISH((gasnete_iop_t *) gpd->gpd_put_lc, alc, 1, 0);
          }
        }

        if (gpd->pd.cq_mode & GNI_CQMODE_GLOBAL_EVENT) {
          // disambiguate the following global event
          gpd->pd.cq_mode = GNI_CQMODE_GLOBAL_EVENT;
          goto again; // LC is so cheap that we don't count it against gasnetc_poll_burst
        } else {
          // no global event requested (AM Long payload)
          // leap ahead to release resources
          GASNETC_UNLOCK_GNI();
          goto release;
        }
      }
    GASNETC_UNLOCK_GNI();

    if_pt (! gpd) { /* empty Cq is common case */
      break;
    } else {
      gpd_flags = gpd->gpd_flags; /* see note w/ GC_POST_COMPLETION_FLAG */

      /* handle remaining work */
      if (gpd_flags & GC_POST_COPY_AMO4) {
        * (uint32_t *) gpd->gpd_amo_result = gpd->u.u32;
        gasneti_sync_writes(); /* sync memcpy */
      } else
      if (gpd_flags & GC_POST_COPY_AMO8) {
        * (uint64_t *) gpd->gpd_amo_result = gpd->u.u64;
        gasneti_sync_writes(); /* sync memcpy */
      } else
      if (gpd_flags & GC_POST_COPY_IMM) {
        GASNETI_MEMCPY((void *) gpd->gpd_get_dst, (void *) gpd->u.immediate, gpd->pd.length);
        gasneti_sync_writes(); /* sync memcpy */
      } else
      if (gpd_flags & GC_POST_COPY) {
        const size_t length = gpd->pd.length - (gpd_flags & GC_POST_COPY_TRIM);
        GASNETI_MEMCPY((void *) gpd->gpd_get_dst, (void *) gpd->gpd_get_src, length);
        gasneti_sync_writes(); /* sync memcpy */
      }

      /* indicate completion */
      // TODO-EX: separate iop inside and outside accesregion?

     { // Start of scope: 'comp'
      const uint32_t comp = (gpd_flags & GC_POST_COMPLETION_MASK);
      gasneti_assert(GASNETI_POWEROFTWO(comp)); // Zero or one bit is set

      switch (comp) {
        case GC_POST_COMPLETION_FLAG:
          *(volatile int *) gpd->gpd_completion = 1;
          /* NOTE: if (gpd_flags & GC_POST_KEEP_GPD) then caller might free gpd now */
          break;
        case GC_POST_COMPLETION_CNTR:
          gasneti_weakatomic_increment((gasneti_weakatomic_t *) gpd->gpd_completion, 0);
          break;
        case GC_POST_COMPLETION_EOP:
          GASNETC_EOP_CNT_FINISH((gasnete_eop_t *) gpd->gpd_completion);
          break;
        case GC_POST_COMPLETION_EAM:
          GASNETC_EOP_ALC_FINISH((gasnete_eop_t *) gpd->gpd_completion);
          break;
        case GC_POST_COMPLETION_IAM:
          GASNETE_IOP_CNT_FINISH((gasnete_iop_t *) gpd->gpd_completion, alc, 1, 0);
          break;
        case GC_POST_COMPLETION_IPUT:
          GASNETE_IOP_CNT_FINISH((gasnete_iop_t *) gpd->gpd_completion, put, 1, 0);
          break;
        case GC_POST_COMPLETION_IGET:
          GASNETE_IOP_CNT_FINISH((gasnete_iop_t *) gpd->gpd_completion, get, 1, 0);
          break;
        case GC_POST_COMPLETION_IRMW:
          GASNETE_IOP_CNT_FINISH((gasnete_iop_t *) gpd->gpd_completion, rmw, 1, 0);
          break;
        case GC_POST_COMPLETION_AMRV:
          gpd->u.am_rvous.ready = 1;
          gasneti_sync_writes();
          am_rvous_ready = 1;
          break;
      } 
     } // End of scope: 'comp'

release:
      /* release resources */
      if (gpd_flags & GC_POST_UNREGISTER) {
        gasnetc_deregister_gpd(gpd);
      } else if (gpd_flags & GC_POST_UNBOUNCE) {
        gasnetc_free_bounce_buffer(gpd);
      }
    #if GASNETC_NP_MEDXL
      else if (gpd_flags & GC_POST_UNPREPARE) {
        gasneti_lifo_push(&medxl_descriptor_pool, (void *) gpd->pd.local_addr);
      }
    #endif

      if (!(gpd_flags & GC_POST_KEEP_GPD)) {
        gasnetc_free_post_descriptor(gpd);
      }
    }
  }
}

// Poll default (possibly only) domain, which always includes incoming AMs
// For multi-domain this is used only at init/exit
// Otherwise, this is gasnet_AMpoll()
void gasnetc_poll(GASNETI_THREAD_FARG_ALONE)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  gasnetc_poll_am_queue(GASNETI_THREAD_PASS_ALONE);
  gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
}

#if GASNETC_USE_MULTI_DOMAIN
// Poll caller's domain (but sometimes incoming AMs too)
// For multi-domain this is gasnet_AMPoll()
void gasnetc_poll_single_domain(GASNETI_THREAD_FARG_ALONE)
{
  GASNETC_DIDX_POST(GASNETI_MYTHREAD->domain_idx);
 #if 1
  /* There is NO use of GASNETC_ALL_DOMAINS in the current code */
  gasneti_assert(GASNETC_DIDX != GASNETC_ALL_DOMAINS);
 #else
  if_pf (GASNETC_DIDX == GASNETC_ALL_DOMAINS) {
    int d;
    gasnetc_poll_am_queue(GASNETI_THREAD_PASS_ALONE);
    for (d = 0; d < gasnetc_domain_count; d++) {
      gasnetc_poll_local_queue(d);
    }
  } else
 #endif
  {
    if (GASNETC_DIDX == GASNETC_DEFAULT_DOMAIN) {
       gasnetc_poll_am_queue(GASNETI_THREAD_PASS_ALONE);
    } else if_pf ((DOMAIN_SPECIFIC_VAL(poll_idx)++ & gasnetc_poll_am_domain_mask) == 0) {
      /* Every now and then poll for AMs even from non-default domains: */
      if (am_rvous_head) {
        gasnetc_poll_local_queue(GASNETC_DEFAULT_DOMAIN);
      }
      gasnetc_poll_am_queue(GASNETI_THREAD_PASS_ALONE);
    }
    gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
  }
}
#endif // GASNETC_USE_MULTI_DOMAIN

GASNETI_NEVER_INLINE(print_post_desc,
static void print_post_desc(const char *title, gni_post_descriptor_t *cmd)) {
  gasneti_EP_t i_ep = gasneti_import_ep(gasneti_THUNK_EP);
  const int in_seg = gasneti_in_local_clientsegment(i_ep, (void *) cmd->local_addr, cmd->length);
  const int in_aux = gasneti_in_local_auxsegment(i_ep, (void *) cmd->local_addr, cmd->length);
  printf("r %d %s-segment %s, desc addr %p\n", gasneti_mynode, (in_seg?"in":(in_aux?"aux":"non")), title, cmd);
  printf("r %d status: %"PRIu64"\n", gasneti_mynode, cmd->status);
  printf("r %d cq_mode_complete: 0x%x\n", gasneti_mynode, cmd->cq_mode_complete);
  printf("r %d type: %d (%s)\n", gasneti_mynode, cmd->type, gasnetc_post_type_string(cmd->type));
  printf("r %d cq_mode: 0x%x\n", gasneti_mynode, cmd->cq_mode);
  printf("r %d dlvr_mode: 0x%x\n", gasneti_mynode, cmd->dlvr_mode);
  printf("r %d local_address: %p(0x%"PRIx64", 0x%"PRIx64")\n", gasneti_mynode, (void *) cmd->local_addr, 
	 cmd->local_mem_hndl.qword1, cmd->local_mem_hndl.qword2);
  printf("r %d remote_address: %p(0x%"PRIx64", 0x%"PRIx64")\n", gasneti_mynode, (void *) cmd->remote_addr, 
	 cmd->remote_mem_hndl.qword1, cmd->remote_mem_hndl.qword2);
  printf("r %d length: 0x%"PRIx64"\n", gasneti_mynode, cmd->length);
  printf("r %d rdma_mode: 0x%x\n", gasneti_mynode, cmd->rdma_mode);
  printf("r %d src_cq_hndl: %p\n", gasneti_mynode, cmd->src_cq_hndl);
  printf("r %d sync: (0x%"PRIx64",0x%"PRIx64")\n", gasneti_mynode, cmd->sync_flag_value, cmd->sync_flag_addr);
  printf("r %d amo_cmd: %d\n", gasneti_mynode, cmd->amo_cmd);
  printf("r %d amo: 0x%"PRIx64", 0x%"PRIx64"\n", gasneti_mynode, cmd->first_operand, cmd->second_operand);
  printf("r %d cqwrite_value: 0x%"PRIx64"\n", gasneti_mynode, cmd->cqwrite_value);
}

GASNETI_INLINE(myPostRdma)
gni_return_t myPostRdma(gni_ep_handle_t ep, gasnetc_post_descriptor_t *gpd, int last_eop_chunk)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;
  int trial = 0;

  do {
      GASNETC_LOCK_GNI();
      status = GNI_PostRdma(ep, pd);
      if (last_eop_chunk && (status == GNI_RC_SUCCESS)) {
        GASNETC_EOP_ALC_FINISH((gasnete_eop_t *) gpd->gpd_put_lc);
      }
      GASNETC_UNLOCK_GNI();
      if_pt (status == GNI_RC_SUCCESS) {
        GASNETC_STAT_EVENT_VAL(POST_RDMA_RETRY, trial);
        return GNI_RC_SUCCESS;
      }
      if (status != GNI_RC_ERROR_RESOURCE) break; /* Fatal */
      GASNETI_WAITHOOK();
      gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
  } while (++trial < GASNETC_RESOURCE_RETRIES);
  if (status == GNI_RC_ERROR_RESOURCE) {
    gasnetc_GNIT_Log("PostRdma retry failed");
  }
  return status;
}

GASNETI_INLINE(myPostFma)
gni_return_t myPostFma(gni_ep_handle_t ep, gasnetc_post_descriptor_t *gpd, int last_eop_chunk)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;
  int trial = 0;

  do {
      GASNETC_LOCK_GNI();
      status = GNI_PostFma(ep, pd);
      if (last_eop_chunk && (status == GNI_RC_SUCCESS)) {
        gasneti_assert(gpd->gpd_flags & GC_POST_COMPLETION_EOP);
        GASNETC_EOP_ALC_FINISH((gasnete_eop_t *) gpd->gpd_put_lc);
      }
      GASNETC_UNLOCK_GNI();
      if_pt (status == GNI_RC_SUCCESS) {
        GASNETC_STAT_EVENT_VAL(POST_FMA_RETRY, trial);
        return GNI_RC_SUCCESS;
      }
      if (status != GNI_RC_ERROR_RESOURCE) break; /* Fatal */
      GASNETI_WAITHOOK();
      gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
  } while (++trial < GASNETC_RESOURCE_RETRIES);
  if (status == GNI_RC_ERROR_RESOURCE) {
    gasnetc_GNIT_Log("PostFma retry failed");
  }
  return status;
}

// TODO-EX: this is our auxseg support until real multi-segment support arrives
//
// Note len=1 is sufficient since the full (addr,len) will have already passed
// gasneti_in_{,local_}fullsegment().  While len=0 might seem cheaper, it is not
// permitted by gasneti_in_*segment().
GASNETI_INLINE(gasnetc_local_mh)
gni_mem_handle_t gasnetc_local_mh(gasneti_EP_t i_ep, void *addr) {
  return  gasneti_in_local_auxsegment(i_ep,addr,1) ? my_aux_handle : my_mem_handle;
}
GASNETI_INLINE(gasnetc_remote_mh)
gni_mem_handle_t gasnetc_remote_mh(peer_struct_t * const peer, void *addr) {
  return  gasneti_in_auxsegment(gasneti_THUNK_TM,peer->pe,addr,1) ? peer->aux_handle : peer->mem_handle;
}

/* Perform an rdma/fma Put with no concern for local completion.
 * Returns length of the request issued to GNI, which may be less
 * than nbytes (for instance due to a failed call to MemRegister).
 */
size_t gasnetc_rdma_put_bulk(gex_TM_t tm, gex_Rank_t rank,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  gasneti_EP_t i_ep = gasneti_import_tm(tm)->_ep;
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  peer_struct_t * const peer = &peer_data[jobrank];
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;

  gasneti_assert(!GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank));

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) dest_addr;
  pd->remote_mem_hndl = gasnetc_remote_mh(peer, dest_addr);
  pd->length = nbytes;

  /* confirm that the destination is in-segment on the far end */
  gasneti_boundscheck(tm, rank, dest_addr, nbytes);

  /* Start with defaults suitable for FMA or in-segment case */
  pd->local_addr = (uint64_t) source_addr;
  pd->local_mem_hndl = gasnetc_local_mh(i_ep, source_addr);

  if (nbytes <= gasnetc_put_fma_rdma_cutover) {
    /* Small enough for FMA - no local memory registration is required */
    pd->type = GNI_POST_FMA_PUT;
    status = myPostFma(peer->ep_handle, gpd, 0);
  } else { /* Using RDMA, which requires local memory registration */
    if_pf (!gasneti_in_local_fullsegment(i_ep, source_addr, nbytes)) {
      /* Use a bounce buffer or mem-reg according to size.
       * Use of gpd->u.immedate would only be reachable if
       *     (put_fma_rdma_cutover < IMMEDIATE_BOUNCE_SIZE),
       * which is not the default (nor recommended).
       */
      if (// Note short-circuit evaluation: cases 1 and 3 lead to THEN body and case 2 to ELSE body.
          // Case 1: nbytes at or below bounce-to-register cutover.  Use bounce buffer.
          (nbytes <= gasnetc_put_bounce_register_cutover) ||
          // Case 2: nbytes larger than cutover.  Attempt to register the local memory.
          (!gasnetc_register_gpd(gpd, GNI_MEM_READ_ONLY) &&
          // Case 3: Registration failed.  Use bounce buffer, reducing xfer length accordingly.
           (pd->length = nbytes = gasnetc_put_bounce_register_cutover))) {
        void * const buffer = gasnetc_alloc_bounce_buffer(0 GASNETC_DIDX_PASS);
        GASNETI_MEMCPY(buffer, source_addr, nbytes);
        pd->local_addr = (uint64_t) buffer;
        pd->local_mem_hndl = my_aux_handle;
        gpd->gpd_flags |= GC_POST_UNBOUNCE;
      } else {
        gpd->gpd_flags |= GC_POST_UNREGISTER;
      }
    }
    pd->type = GNI_POST_RDMA_PUT;
    status = myPostRdma(peer->ep_handle, gpd, 0);
  }

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Put", pd);
    gasnetc_GNIT_Abort("Put failed with %s", gasnetc_gni_rc_string(status));
  }

  return nbytes;
}

/* Perform an rdma/fma Put with signalling of local completion.
 * Returns length of the request issued to GNI, which may be less
 * than nbytes (for instance due to a failed call to MemRegister).
 */
size_t
gasnetc_rdma_put_lc(gex_TM_t tm, gex_Rank_t rank,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasneti_weakatomic_val_t *initiated_lc,
		 int last_eop_chunk, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  gasneti_EP_t i_ep = gasneti_import_tm(tm)->_ep;
  peer_struct_t * const peer = &peer_data[jobrank];
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;

  gasneti_assert(!GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank));

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) dest_addr;
  pd->remote_mem_hndl = gasnetc_remote_mh(peer, dest_addr);
  pd->length = nbytes;

  /* confirm that the destination is in-segment on the far end */
  gasneti_boundscheck(tm, rank, dest_addr, nbytes);

  /* Start with defaults suitable for FMA or in-segment case */
  pd->local_addr = (uint64_t) source_addr;
  pd->local_mem_hndl = gasnetc_local_mh(i_ep, source_addr);

  /* If small enough for FMA then no local memory registration is required */
  if (nbytes <= gasnetc_put_fma_rdma_cutover) {
    /* Favor immediate buffer or bounce-buffers upto the FMA limit. */
    // TODO-EX: when indication of LC is requested we are currently favoring a
    // FMA+copy over the alternative in which LC is not signalled until RC.
    // This gives synchronous LC up to the FMA/RDMA cutover (4K by default).
    // While this does simplify the completion logic (by avoiding any LC
    // signalling options when handling a GLOBAL_EVENT) it may not always
    // be the best option - especially given that 4K memcpy() is not free.
    // So, GEX_FLAG_LC_COPY_{YES,NO} eventually should be applied here.
    void * buffer;
    if (nbytes <= GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE) {
       buffer = gpd->u.immediate;
    } else {
       pd->length = nbytes = MIN(nbytes, gasnetc_put_bounce_register_cutover);
       gpd->gpd_flags |= GC_POST_UNBOUNCE;
       buffer = gasnetc_alloc_bounce_buffer(0 GASNETC_DIDX_PASS);
    }
    GASNETI_MEMCPY(buffer, source_addr, nbytes);
    pd->local_addr = (uint64_t) buffer;
    pd->type = GNI_POST_FMA_PUT;
    status = myPostFma(peer->ep_handle, gpd, last_eop_chunk);
  } else {
    /* Using RDMA, which requires local memory registration */
    if_pf (!gasneti_in_local_fullsegment(i_ep, source_addr, nbytes)) {
      /* Use a bounce buffer or mem-reg according to size.
       */
      if (// Note short-circuit evaluation: cases 1 and 3 lead to THEN body and case 2 to ELSE body.
          // Case 1: nbytes at or below bounce-to-register cutover.  Use bounce buffer.
          (nbytes <= gasnetc_put_bounce_register_cutover) ||
          // Case 2: nbytes larger than cutover.  Attempt to register the local memory.
          (!gasnetc_register_gpd(gpd, GNI_MEM_READ_ONLY) &&
          // Case 3: Registration failed.  Use bounce buffer, reducing xfer length accordingly.
          ((last_eop_chunk = 0), (pd->length = nbytes = gasnetc_put_bounce_register_cutover)))) {
        void * const buffer = gasnetc_alloc_bounce_buffer(0 GASNETC_DIDX_PASS);
        GASNETI_MEMCPY(buffer, source_addr, nbytes);
        pd->local_addr = (uint64_t) buffer;
        gpd->gpd_flags |= GC_POST_UNBOUNCE;
        pd->local_mem_hndl = my_aux_handle;
        goto post_rdma;
      } else {
        gpd->gpd_flags |= GC_POST_UNREGISTER;
      }
    }
    // Request LC event if registered (in segment or dynamic mem-reg)
    pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT | GNI_CQMODE_LOCAL_EVENT;
    *initiated_lc += 1;
post_rdma:
    pd->type = GNI_POST_RDMA_PUT;
    status = myPostRdma(peer->ep_handle, gpd, last_eop_chunk);
  }

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Put", pd);
    gasnetc_GNIT_Abort("Put failed with %s", gasnetc_gni_rc_string(status));
  }

  return nbytes;
}

/* FMA Put from a specified buffer */
void gasnetc_rdma_put_buff(gex_TM_t tm, gex_Rank_t rank,
		void *dest_addr, void *source_addr,
		size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  peer_struct_t * const peer = &peer_data[jobrank];
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;

#if !GASNETC_LOCAL_GNI_EP
  gasneti_assert(!GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank));
#endif

  /* confirm that the destination is in-segment on the far end */
  gasneti_boundscheck(tm, rank, dest_addr, nbytes);

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) dest_addr;
  pd->remote_mem_hndl = gasnetc_remote_mh(peer, dest_addr);
  pd->length = nbytes;
  pd->local_addr = (uint64_t) source_addr;

  /* now initiate always using FMA */
  pd->type = GNI_POST_FMA_PUT;
  status = myPostFma(peer->ep_handle, gpd, 0);

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Put", pd);
    gasnetc_GNIT_Abort("Put failed with %s", gasnetc_gni_rc_string(status));
  }
}

/* initiate a Get according to fma/rdma cutover */
GASNETI_INLINE(gasnetc_post_get)
void gasnetc_post_get(gni_ep_handle_t ep, gasnetc_post_descriptor_t *gpd)
{
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;
  const size_t nbytes = pd->length;

  if (nbytes <= gasnetc_get_fma_rdma_cutover) {
      pd->type = GNI_POST_FMA_GET;
      status = myPostFma(ep, gpd, 0);
  } else {
      pd->type = GNI_POST_RDMA_GET;
      status = myPostRdma(ep, gpd, 0);
  }

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Get", pd);
    gasnetc_GNIT_Abort("Get failed with %s", gasnetc_gni_rc_string(status));
  }
}

/* Perform an rdma/fma Get.
 * Returns length of the request issued to GNI, which may be less
 * than nbytes (for instance due to a failed call to MemRegister).
 */
size_t gasnetc_rdma_get(gex_TM_t tm, gex_Rank_t rank,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  gasneti_EP_t i_ep = gasneti_import_tm(tm)->_ep;
  peer_struct_t * const peer = &peer_data[jobrank];
  gni_post_descriptor_t * const pd = &gpd->pd;

#if !GASNETC_LOCAL_GNI_EP
  gasneti_assert(!GASNETI_NBRHD_JOBRANK_IS_LOCAL(node));
#endif

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) source_addr;
  pd->remote_mem_hndl = gasnetc_remote_mh(peer, source_addr);
  pd->length = nbytes;

  /* confirm that the source is in-segment on the far end */
  gasneti_boundscheck(tm, rank, source_addr, nbytes);

  /* Start with defaults suitable for in-segment case */
  pd->local_addr = (uint64_t) dest_addr;
  pd->local_mem_hndl = gasnetc_local_mh(i_ep, dest_addr);

  /* check where the local addr is */
  if_pf (!gasneti_in_local_fullsegment(i_ep, dest_addr, nbytes)) {
    /* dest not (entirely) in segment */
    /* if (nbytes <= gasnetc_get_bounce_register_cutover)  then use bounce buffer
     * else mem-register
     */
    if (nbytes <= GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE) {
      gpd->gpd_flags |= GC_POST_COPY_IMM;
      pd->local_addr = (uint64_t) gpd->u.immediate;
      pd->local_mem_hndl = my_aux_handle;
      gpd->gpd_get_dst = (uint64_t) dest_addr;
    } else if (// Note short-circuit evaluation: cases 1 and 3 lead to THEN body and case 2 to ELSE body.
               // Case 1: nbytes at or below bounce-to-register cutover.  Use bounce buffer.
               (nbytes <= gasnetc_get_bounce_register_cutover) ||
               // Case 2: nbytes larger than cutover.  Attempt to register the local memory.
               (!gasnetc_register_gpd(gpd, GNI_MEM_READWRITE) &&
               // Case 3: Registration failed.  Use bounce buffer, reducing xfer length accordingly.
                (pd->length = nbytes = gasnetc_get_bounce_register_cutover))) {
      gpd->gpd_flags |= GC_POST_UNBOUNCE | GC_POST_COPY;
      pd->local_addr = (uint64_t) gasnetc_alloc_bounce_buffer(0 GASNETC_DIDX_PASS);
      pd->local_mem_hndl = my_aux_handle;
      gpd->gpd_get_src = pd->local_addr;
      gpd->gpd_get_dst = (uint64_t) dest_addr;
    } else {
      gpd->gpd_flags |= GC_POST_UNREGISTER;
    }
  }

  gasnetc_post_get(peer->ep_handle, gpd);

  return nbytes;
}

/* for get in which one or more of dest_addr, source_addr or nbytes is NOT divisible by 4
 * NOTE: be sure to update gasnetc_max_get_unaligned if the logic here changes
 */
void gasnetc_rdma_get_unaligned(gex_TM_t tm, gex_Rank_t rank,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  peer_struct_t * const peer = &peer_data[jobrank];
  gni_post_descriptor_t * const pd = &gpd->pd;
  uint8_t * buffer;

  /* Compute length of "overfetch" required, if any */
  unsigned int pre = (uintptr_t) source_addr & 3;
  size_t       length = GASNETI_ALIGNUP(nbytes + pre, 4);
  unsigned int overfetch = length - nbytes;

  gasneti_assert(!GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank));
  gasneti_assert(length <= gasnetc_max_get_unaligned);

  gasneti_assert(0 == (overfetch & ~GC_POST_COPY_TRIM));
  gpd->gpd_flags |= GC_POST_COPY | overfetch;

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) source_addr - pre;
  pd->remote_mem_hndl = gasnetc_remote_mh(peer, source_addr);
  pd->length = length;
  pd->local_mem_hndl = my_aux_handle;

  /* confirm that the source is in-segment on the far end */
  gasneti_boundscheck(tm, rank, (void*)pd->remote_addr, pd->length);

  /* must always use immediate or bounce buffer */
  if (length <= GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE) {
    buffer = gpd->u.immediate;
  } else {
    gasneti_assert(length <= gasnetc_get_bounce_register_cutover);
    gpd->gpd_flags |= GC_POST_UNBOUNCE;
    buffer = gasnetc_alloc_bounce_buffer(0 GASNETC_DIDX_PASS);
  }

  pd->local_addr = (uint64_t) buffer;
  gpd->gpd_get_src = (uint64_t) buffer + pre;
  gpd->gpd_get_dst = (uint64_t) dest_addr;

  gasnetc_post_get(peer->ep_handle, gpd);
}

/* Get into a specified in-full-segment buffer
   Will overfetch if source_addr or nbytes are not 4-byte aligned
   but expects (does not check) that the buffer is aligned.
   Caller must be allow for space (upto 6 bytes) for the overfetch.
   Returns offset to start of data after adjustment for overfetch
 */
int gasnetc_rdma_get_buff(gex_TM_t tm, gex_Rank_t rank,
		void *dest_addr, void *source_addr,
		size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  gasneti_EP_t i_ep = gasneti_import_tm(tm)->_ep;
  peer_struct_t * const peer = &peer_data[jobrank];
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;

  /* Compute length of "overfetch" required, if any */
  unsigned int pre = (uintptr_t) source_addr & 3;
  size_t       length = GASNETI_ALIGNUP(nbytes + pre, 4);

  gasneti_assert(!GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank));
  gasneti_assert(nbytes  <= GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE);

  /* confirm that the source is in-segment on the far end */
  gasneti_boundscheck(tm, rank, source_addr, nbytes);

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) source_addr - pre;
  pd->remote_mem_hndl = gasnetc_remote_mh(peer, source_addr);
  pd->length = length;
  pd->local_addr = (uint64_t) dest_addr;
  pd->local_mem_hndl = gasnetc_local_mh(i_ep, dest_addr);

  /* now initiate - *always* FMA for now */
  pd->type = GNI_POST_FMA_GET;
  status = myPostFma(peer->ep_handle, gpd, 0);

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Get", pd);
    gasnetc_GNIT_Abort("Get failed with %s", gasnetc_gni_rc_string(status));
  }

  return pre;
}

static
void am_rvous_get(peer_struct_t * const peer, gasnetc_notify_t notify GASNETI_THREAD_FARG)
{
  gasneti_mutex_unlock(&ampoll_lock);

  gasneti_assert(gc_notify_get_type(notify) != gc_notify_rvous);

  // TODO-EX: if/when we use CQWrite we'll need a different encoding, which will
  // carry the len (likely in units such as cacheline), instead of this code
  size_t length;
  const int numargs = gasnetc_am_numargs(notify);
  switch (gasnetc_am_command(notify)) {
    case GC_CMD_AM_SHORT:
      length = GASNETC_HEADLEN(short, numargs);
      break;
    case GC_CMD_AM_MEDIUM:
      length = GASNETC_HEADLEN(medium, numargs) + gasnetc_am_nbytes(notify);
      break;
    case GC_CMD_AM_LONG:
      length = GASNETC_HEADLEN(long, numargs);
      break;
    case GC_CMD_AM_LONG_PACKED:
      length = GASNETC_HEADLEN(long, numargs) + gasnetc_am_nbytes(notify);
      break;
    default: gasneti_unreachable();
  }

  if (! length) {
    // Short or Medium w/ 0 args and 0 payload.  Nothing to Get
    gasnetc_recv_am_unlocked(peer, NULL, notify GASNETI_THREAD_PASS);
  } else {
    // Allocate gpd with embeded rendezvous metadata
    gasnetc_post_descriptor_t *gpd = gasneti_lifo_pop(&am_rvous_pool);
    if_pf (!gpd) {
      // progress outstanding gets to free a gpd
      GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
      GASNETC_TRACE_WAIT_BEGIN();
      int stall = 0;
      goto first;
      do {
        GASNETI_WAITHOOK();
        stall = 1;
        // Poll until a gpd is free or one or more Gets are completed
        do {
          gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
          gpd = gasneti_lifo_pop(&am_rvous_pool);
        } while (!gpd && !am_rvous_ready);
        if (gpd) break;
first:
        // Run any ready handlers
        am_rvous_run(GASNETI_THREAD_PASS_ALONE);
        gpd = gasneti_lifo_pop(&am_rvous_pool);
      } while (!gpd);
      if (stall) GASNETC_TRACE_WAIT_END(ALLOC_AMRV_STALL);
    }
    am_rvous_t *rvous = &gpd->u.am_rvous;
    rvous->ready  = 0;
    rvous->peer   = peer;
    rvous->notify = notify;

    gni_post_descriptor_t * const pd = &gpd->pd;
    pd->remote_addr = (uint64_t) (peer->remote_reply_base +
                                  am_replysz * gc_notify_get_initiator_slot(notify));
    pd->remote_mem_hndl = peer->am_handle;
    pd->length = GASNETI_ALIGNUP(length, 8); // TODO: 4 is minimum, what is optimal?

    // Honors same fma/rma cutover as Get
    gasnetc_post_get(peer->ep_handle, gpd);

    // Add to live list
    rvous->next = NULL;
    gasneti_mutex_lock(&am_rvous_lock);
    *am_rvous_tail_p = rvous;
    am_rvous_tail_p = &rvous->next;
    // TODO: profitable to run "ready" entry here instead of enqueueing?
    if_pf (rvous->ready) am_rvous_ready = 1;  // completed by another racing thread
    gasneti_mutex_unlock(&am_rvous_lock);
  }

  gasneti_mutex_lock(&ampoll_lock);
}

/*------ Post Put of Long Payload */

// gasnetc_rdma_put_long()
//
// For the uncommon case that the source is out-of-segment, larger than a
// single bounce-buffer, and cannot be dynamically registered, all but the
// last bounce-buffer is completed globally before the final piece.
void gasnetc_rdma_put_long(
                          gex_TM_t tm, gex_Rank_t rank,
                          void *dest_addr, void *source_addr,
                          size_t nbytes,
                          uint32_t gpd_flags,
                          void *completion,
                          uint32_t nonce
                          GASNETC_DIDX_FARG)
{
  /* confirm that the destination is in-segment on the far end */
  gasneti_boundscheck(tm, rank, dest_addr, nbytes);

  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  gasneti_EP_t i_ep = gasneti_import_tm(tm)->_ep;
  peer_struct_t * const peer = &peer_data[jobrank];
  gni_return_t status;

  gasneti_assert(!GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank));

  gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor(0 GASNETC_DIDX_PASS);

  gni_post_descriptor_t * pd = &gpd->pd;
  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) dest_addr;
  pd->remote_mem_hndl = gasnetc_remote_mh(peer, dest_addr);
  pd->length = nbytes;

  /* Start with defaults suitable for FMA or in-segment RDMA */
  gpd->gpd_flags = gpd_flags;
  pd->local_addr = (uint64_t) source_addr;
  pd->local_mem_hndl = gasnetc_local_mh(i_ep, source_addr);

  if (nbytes <= gasnetc_put_fma_rdma_cutover) {
    /* Small enough for FMA - no local memory registration is required */
    gpd->gpd_completion = (uintptr_t) completion;
    pd->type = GNI_POST_FMA_PUT;
    pd->cq_mode = GNI_CQMODE_REMOTE_EVENT | GNI_CQMODE_GLOBAL_EVENT;
  } else { /* Using RDMA, which requires local memory registration */
    if_pf (!gasneti_in_local_fullsegment(i_ep, source_addr, nbytes)) {
      /* Use a bounce buffer or mem-reg according to size.
       * Use of gpd->u.immedate would only be reachable if
       *     (put_fma_rdma_cutover < IMMEDIATE_BOUNCE_SIZE),
       * which is not the default (nor recommended).
       */
      if (nbytes <= gasnetc_put_bounce_register_cutover) {
        // Single bounce-buffer case
        void * buffer = gasnetc_alloc_bounce_buffer(0 GASNETC_DIDX_PASS);
        GASNETI_MEMCPY(buffer, source_addr, pd->length);
        pd->local_addr = (uint64_t) buffer;
        pd->local_mem_hndl = my_aux_handle;
        gpd->gpd_flags = GC_POST_UNBOUNCE;
        // signal synchronous local completion
        switch (gpd_flags & GC_POST_COMPLETION_MASK) {
          case GC_POST_COMPLETION_FLAG:
            *(volatile int *) completion = 1;
            break;
          case GC_POST_COMPLETION_EAM:
            GASNETC_EOP_ALC_FINISH((gasnete_eop_t *) completion);
            break;
          case GC_POST_COMPLETION_IAM:
            GASNETE_IOP_CNT_FINISH((gasnete_iop_t *) completion, alc, 1, 0);
            break;
          default:
            gasneti_unreachable_error(("Invalid completion flags 0x%x",
                                      gpd_flags & GC_POST_COMPLETION_MASK));
        }
      } else if (! gasnetc_register_gpd(gpd, GNI_MEM_READ_ONLY)) {
        // Multiple bounce-buffer case (due to dynamic registration failure).
        // Caller needs exactly one xfer on which to block for completion,
        // so we implement "puts-sync-put".
        gasneti_weakatomic_t completed = gasneti_weakatomic_init(0);
        int initiated = 0;

        const gni_mem_handle_t remote_mem_hndl = pd->remote_mem_hndl;
        const size_t chunk = gasnetc_put_bounce_register_cutover;
        void * buffer = gasnetc_alloc_bounce_buffer(0 GASNETC_DIDX_PASS);

        do {
          gasneti_assert(nbytes > chunk);

          // Inject a chunk (never the last one) tied to local counter of global completion events
          GASNETI_MEMCPY(buffer, source_addr, chunk);
          pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
          pd->type = GNI_POST_RDMA_PUT;
          pd->length = chunk;
          pd->local_addr = (uint64_t) buffer;
          pd->local_mem_hndl = my_aux_handle;
          gpd->gpd_completion = (uintptr_t) &completed;
          gpd->gpd_flags = GC_POST_COMPLETION_CNTR | GC_POST_UNBOUNCE;
          status = myPostRdma(peer->ep_handle, gpd, 0);
          if_pf (status) goto error;

          initiated += 1;
          nbytes -= chunk;
          source_addr = (void*)(chunk + (uintptr_t)source_addr);
          dest_addr   = (void*)(chunk + (uintptr_t)dest_addr);

          // Prepare for next chunk (might be last one)
          gpd = gasnetc_alloc_post_descriptor(0 GASNETC_DIDX_PASS);
          pd = &gpd->pd;
          /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
          pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
          pd->remote_addr = (uint64_t) dest_addr;
          pd->remote_mem_hndl = remote_mem_hndl;
          buffer = gasnetc_alloc_bounce_buffer(0 GASNETC_DIDX_PASS);
        } while (nbytes > chunk);

        // Prep final chunk (overlapped w/ prior xfers)
        GASNETI_MEMCPY(buffer, source_addr, nbytes);
        pd->length = nbytes;
        pd->local_addr = (uint64_t) buffer;
        pd->local_mem_hndl = my_aux_handle;
        gpd->gpd_completion = (uintptr_t) completion;
        gpd->gpd_flags = gpd_flags | GC_POST_UNBOUNCE;

        // Stall
        while (initiated != gasneti_weakatomic_read(&completed, 0)) {
          GASNETI_WAITHOOK();
          gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
        }

        // Fall through to inject final chunk
      } else {
        gpd->gpd_flags = gpd_flags | GC_POST_UNREGISTER;
      }
    }
    gpd->gpd_put_lc = (uintptr_t) completion;
    pd->type = GNI_POST_RDMA_PUT;
    pd->cq_mode = GNI_CQMODE_REMOTE_EVENT | GNI_CQMODE_LOCAL_EVENT;
  }

 { // Start of scope: 'trial', 'ep' and 'instid'
  int trial = 0;
  gni_ep_handle_t ep = peer->ep_handle;
  const uint32_t instid = gasneti_mynode | nonce;

  do {
    GASNETC_LOCK_GNI();
    gasneti_assert_zeroret( GNI_EpSetEventData(ep, 0, instid) );
    status = (pd->type == GNI_POST_RDMA_PUT) ? GNI_PostRdma(ep, pd)
                                             : GNI_PostFma(ep, pd);
    gasneti_assert_zeroret( GNI_EpSetEventData(ep, 0, gasneti_mynode) );
    GASNETC_UNLOCK_GNI();
    if_pt (status == GNI_RC_SUCCESS) {
      if (pd->type == GNI_POST_RDMA_PUT) {
        GASNETC_STAT_EVENT_VAL(POST_RDMA_RETRY, trial);
      } else {
        GASNETC_STAT_EVENT_VAL(POST_FMA_RETRY, trial);
      }
      return; // Normal exit path
    }
    if (status != GNI_RC_ERROR_RESOURCE) break; /* Fatal */
    GASNETI_WAITHOOK();
    gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
  } while (++trial < GASNETC_RESOURCE_RETRIES);
 } // End of scope: 'trial', 'ep' and 'instid'

error:
  gasneti_assert (status != GNI_RC_SUCCESS);
  print_post_desc("Payload Put", pd);
  gasnetc_GNIT_Abort("Payload Put failed with %s", gasnetc_gni_rc_string(status));
}



#if GASNETC_BUILD_GNIRATOMIC
/*------ Post Fma for NIC atomic */
void gasnetc_post_amo(
                gasneti_TM_t i_tm, gex_Rank_t tgt_rank, void *tgt_addr,
                gasnetc_post_descriptor_t *gpd, gex_Flags_t flags)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  gex_Rank_t jobrank = gasneti_i_tm_rank_to_jobrank(i_tm, tgt_rank);
  peer_struct_t * const peer = &peer_data[jobrank];
  gni_post_descriptor_t * const pd = &gpd->pd;

  gasneti_boundscheck(gasneti_export_tm(i_tm), tgt_rank, tgt_addr, pd->length);

  pd->type = GNI_POST_AMO;
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->local_addr = (uint64_t) gpd->u.immediate;
  pd->local_mem_hndl = my_aux_handle;
  pd->remote_addr = (uint64_t) tgt_addr;
  pd->remote_mem_hndl = gasnetc_remote_mh(peer, tgt_addr);

  gni_return_t status = myPostFma(peer->ep_handle, gpd, 0);
  if_pf (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort("GNI_POST_AMO failed with %s", gasnetc_gni_rc_string(status));
  }
}
#endif

#if GASNETC_BUILD_GNICE
// AuxSeg setup for CE result buffer space
#define GASNETC_CE_RESULT_COUNT 1
static gasnete_ce_result_t *gasnete_ce_results;
gasneti_auxseg_request_t gasnetc_ce_auxseg_alloc(gasnet_seginfo_t *auxseg_info) {
  gasneti_auxseg_request_t retval;

  // TODO: replace w/ env var if/when we have logic to pipeline multiple
  int count = GASNETC_CE_RESULT_COUNT;

  retval.minsz =
  retval.optimalsz = count * sizeof(gasnete_ce_result_t);
  gasneti_assert_always(retval.optimalsz == count * 2 * GASNETC_CACHELINE_SIZE);

  if (auxseg_info != NULL) { /* auxseg granted */
    gasneti_assert_always(auxseg_info[gasneti_mynode].size >= count * sizeof(gasnete_ce_result_t));
    gasnete_ce_results = auxseg_info[gasneti_mynode].addr;
  }

  return retval;
}

static void ce_gate_init(void); // Fwd decl for use in gasnete_init_ce

// Return job rank of first process on a given host
static gex_Rank_t host_to_jobrank(const gex_Rank_t host) {
  gasneti_assert_always(gasneti_nodeinfo);
  if (host == GEX_RANK_INVALID) return GEX_RANK_INVALID;
  for (gex_Rank_t i = 0; i < gasneti_nodes; ++i) {
    if (host == gasneti_nodeinfo[i].host) return i;
  }
  gasneti_fatalerror("Invalid host number %d", (int)host);
}

#if GASNET_PSHM
// Return 1 if jobrank is first process in a given neighborhood, otherwise 0
static int jobrank_leads_nbrhd(const gex_Rank_t jobrank) {
  gasneti_assert_always(gasneti_pshm_firsts);
  for (gex_Rank_t i = 0; i < gasneti_mysupernode.grp_count; ++i) {
    if (jobrank == gasneti_pshm_firsts[i]) return 1;
  }
  return 0;
}
#endif

// TODO: generalize to other than TEAM_ALL??
static uint32_t *gather_ce_ids(uint32_t my_ce_id) {
  uint32_t *result = gasneti_calloc(gasneti_nodes, sizeof(uint32_t));
  gasnetc_bootstrapExchange_gni(&my_ce_id, sizeof(my_ce_id), result);

  // Scan result for failure (-1 value) from any host-leader
  // TODO: there must be a better way to iterate over the leaders
  gex_Rank_t num_hosts = gasneti_myhost.grp_count;
  for (gex_Rank_t host = 0; host < num_hosts; ++host) {
    gex_Rank_t jobrank = host_to_jobrank(host);
    gasneti_assert(jobrank != GEX_RANK_INVALID);
    if ((uint32_t)-1 == result[jobrank]) {
      if (my_ce_id != (uint32_t)-1) {
        GASNETI_TRACE_PRINTF(I,("Aries CE disabled: one or more remote hosts failed to init."));
      }
      gasneti_free(result);
      return NULL;
    }
  }

  return result;
}

static gni_ep_handle_t myEpSetCeAttr(
                gex_Rank_t jobrank, uint32_t ce_id,
                uint32_t child_id, gni_ce_child_t child_type)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);

  gni_ep_handle_t ep_handle = peer_data[jobrank].ep_handle;
  gni_return_t status = GNI_EpSetCeAttr(ep_handle, ce_id, child_id, child_type);
  if_pf (status) {
    gasnetc_GNIT_Abort("EpSetCeAttr() failed with %s", gasnetc_gni_rc_string(status));
  }
  return ep_handle;
}

// Collectively initialize the Aries Collective Engine
// TODO: generalize to other than TEAM_ALL??
void gasnete_init_ce(void) {
  if (gasnete_ce_available) return;

  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  gni_return_t status;

  int enable = gasneti_getenv_yesno_withdefault("GASNET_USE_CE", 1);
  if (!enable) {
    GASNETI_TRACE_PRINTF(I,("Aries CE disabled: user request"));
    return;
  }

  unsigned int radix = gasneti_getenv_int_withdefault("GASNET_GNI_CE_RADIX",
                                                      GASNETC_GNI_CE_RADIX_DEFAULT, 0);
  if_pf (radix < 1 || radix >= GNI_CE_MAX_CHILDREN) {
    gasneti_fatalerror("GASNET_GNI_CE_RADIX must be between 1 and %d, inclusive.",
                       (int)(GNI_CE_MAX_CHILDREN-1));
  }

  // Check that auxseg memory is available and suitably aligned
  gasneti_assert_always(NULL != gasnete_ce_results);
  gasneti_assert_always(! ((uintptr_t)gasnete_ce_results % 32));

  // Check that local process or neighborhood count does not exceed limits
  // This is a purely local computation, but if any host cannot fit within
  // the limits imposed by (GNI_CE_MAX_CHILDREN + radix), then communication
  // will disable CE use.
  int avail_local_degree = GNI_CE_MAX_CHILDREN - radix;
#if GASNET_PSHM
  int allow_psm = gasneti_getenv_yesno_withdefault("GASNET_USE_CE_PSHM", 1);
  int use_pshm = 0;
#endif
  if (avail_local_degree >= gasneti_myhost.node_count) {
    // Default/desired case: every process talks to VCE directly
    GASNETI_TRACE_PRINTF(I,("Aries CE: using processes"));
  } else {
#if GASNET_PSHM
    if (!allow_psm) {
      GASNETI_TRACE_PRINTF(I,("Aries CE disabled: processes per host(%d) + radix(%u) "
                              "exceeds GNI_CE_MAX_CHILDREN(%d) and shared-memory hybrid is disabled",
                            (int)gasneti_myhost.node_count, radix, GNI_CE_MAX_CHILDREN));
      (void) gather_ce_ids((uint32_t)-1); // induce collective decision to disable CE
      return;
    }

    int nph = 0; // neighborhoods per (this) host
    for (unsigned int i = 0; i < gasneti_myhost.node_count; ++i) {
      nph += jobrank_leads_nbrhd(gasneti_myhost.nodes[i]);
    }
    if (avail_local_degree >= nph) {
      // Too many processes per (this) host, but *can* fit using VCE leaf per nbrhd
      // So enable use of shared-memory within each neighborhood
      GASNETI_TRACE_PRINTF(I,("Aries CE: using neighborhoods"));
      use_pshm = 1;
    } else {
      // TODO: eliminate this restriction using a hierarchical barrier which uses the network within a host.
      GASNETI_TRACE_PRINTF(I,("Aries CE disabled: shared-memory neighborhoods per host(%d) + radix(%u) exceeds GNI_CE_MAX_CHILDREN(%d)",
                              (int)nph, radix, GNI_CE_MAX_CHILDREN));
      (void) gather_ce_ids((uint32_t)-1); // induce collective decision to disable CE
      return;
    }
#else
    // Too many processes per (this) host
    // TODO: eliminate this restriction using a hierarchical barrier which uses the network within a host.
    GASNETI_TRACE_PRINTF(I,("Aries CE disabled: processes per host(%d) + radix(%u) exceeds GNI_CE_MAX_CHILDREN(%d)",
                            (int)gasneti_myhost.node_count, radix, GNI_CE_MAX_CHILDREN));
    (void) gather_ce_ids((uint32_t)-1); // induce collective decision to disable CE
    return;
#endif
  }

  const gex_Rank_t node_rank = gasneti_myhost.node_rank;
  const gex_Rank_t tree_rank  = gasneti_myhost.grp_rank;
  const gex_Rank_t tree_size  = gasneti_myhost.grp_count;
  const gex_Rank_t tree_lo_ch = MIN(tree_size, tree_rank * radix + 1);
  const gex_Rank_t tree_hi_ch = MIN(tree_size, tree_lo_ch + radix);
  const gex_Rank_t children   = tree_hi_ch - tree_lo_ch;

  // One leader per host tries to allocate a single VCE instance
  uint32_t my_ce_id = (uint32_t)-1;
  if (!node_rank) {
  #if GASNET_PSHM
    gasneti_assert(jobrank_leads_nbrhd(gasneti_mynode)); // sanity check
  #endif
    status = GNI_CeCreate(DOMAIN_SPECIFIC_VAL(nic_handle), &ce_handle);
    if_pf (status) {
      GASNETI_TRACE_PRINTF(I,("Aries CE disabled: CeCreate failed with %s",
                              gasnetc_gni_rc_string(status)));
    } else {
      status = GNI_CeGetId(ce_handle, &my_ce_id);
      gasneti_assert_always (status == GNI_RC_SUCCESS);
      GASNETI_TRACE_PRINTF(I,("Aries CE: allocated VCE with id %d", my_ce_id));
    }
  }

  // All ranks collect ce_ids, checking for any failure to allocate
  uint32_t *all_ce_id = gather_ce_ids(my_ce_id);
  if (!all_ce_id) {
    if (!node_rank && ce_handle) (void) GNI_CeDestroy(ce_handle);
    return; // This failure is collective
  }

  // The per-host leader configures the VCE instance
  if (!node_rank) {
    // EPs needed to configure the VCE
    gni_ep_handle_t child_eps[GNI_CE_MAX_CHILDREN]; // lazy over-allocation
    gni_ep_handle_t parent_ep = NULL;
    // Configure EP connecting parent VCE
    if (tree_rank) {
      unsigned int cidx = (tree_rank - 1) % radix;
      gex_Rank_t parent = host_to_jobrank((tree_rank - 1) / radix);
      parent_ep = myEpSetCeAttr(parent, all_ce_id[parent], cidx, GNI_CE_CHILD_VCE);
    }
    int n = 0;
    // Configure EPs connecting child VCEs
    for (unsigned int i = 0; i < children; ++i, ++n) {
      gex_Rank_t child = host_to_jobrank(tree_lo_ch + i);
      child_eps[n] = myEpSetCeAttr(child, all_ce_id[child], n, GNI_CE_CHILD_VCE);
    }
    // Configure EPs connecting leaves (rank processes, including self)
    for (unsigned int i = 0; i < gasneti_myhost.node_count; ++i) {
      gex_Rank_t leaf = gasneti_myhost.nodes[i];
    #if GASNET_PSHM
      // Conditionally filter to connect only the nbrhd leaders
      if (use_pshm && !jobrank_leads_nbrhd(leaf)) continue;
    #endif
      child_eps[n] = myEpSetCeAttr(leaf, /*unused*/0, n, GNI_CE_CHILD_PE);
      n += 1;
    }
    // Configure the VCE
    gasneti_assert(n <= GNI_CE_MAX_CHILDREN);
    status = GNI_CeConfigure(ce_handle, child_eps, n,
                             parent_ep, DOMAIN_SPECIFIC_VAL(bound_cq_handle),
                             GNI_CE_MODE_CQE_ONERR | GNI_CE_MODE_ROUND_ZERO);
    if_pf (status) {
      // TODO: should we fall-back for this failure?
      gasnetc_GNIT_Abort("CeConfigure() failed with %s", gasnetc_gni_rc_string(status));
    }
  }

  // Configure the leaf->VCE connection, if any
#if GASNET_PSHM
  if (!use_pshm || !gasneti_pshm_mynode)
#endif
  {
    unsigned int cidx = children; // will hold my index in parent's ep list
  #if GASNET_PSHM
    if (use_pshm) {
      // Count nbrhd leaders, on same host, which proceed me
      for (unsigned int i = 0; i < gasneti_myhost.node_count; ++i) {
        gex_Rank_t jobrank = gasneti_myhost.nodes[i];
        if (jobrank == gasneti_mynode) break;
        cidx += jobrank_leads_nbrhd(jobrank);
      }
    } else
  #endif
    cidx += node_rank;
    gex_Rank_t vce_jobrank = host_to_jobrank(tree_rank);
    my_ce_ep_handle = myEpSetCeAttr(vce_jobrank, all_ce_id[vce_jobrank], cidx, GNI_CE_CHILD_PE);
  }

  gasneti_free(all_ce_id);

#if GASNET_PSHM
  if (use_pshm) ce_gate_init();
#endif

  gasnete_ce_available = 1;
  GASNETI_TRACE_PRINTF(I,("Aries CE: available, inter-host radix = %d", radix));

  gasnetc_bootstrapBarrier_gni();

#if GASNET_DEBUG && !GASNET_PSHM // TODO: remove or update for PSHM?
  for (int i = 0; i < 4; ++i) {
    gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor(0 GASNETC_DIDX_PASS);

    // The operation: 2-field SUM reduction over uint64_t
    gpd->gpd_ce_cmd  = GNI_FMA_CE_IADD;
    gpd->gpd_ce_mode = GNI_CEMODE_TWO_OP;
    gpd->gpd_ce_op1  = i;
    gpd->gpd_ce_op2  = gasneti_mynode;

    gasnete_ce_result_t *result = gasnetc_post_ce(gpd);

    gasneti_polluntil((status = gasnete_test_ce(result)) != GNI_RC_NOT_DONE);
    gasneti_assert(! status);

    gasneti_assert_uint(result->output.result1 ,==, i * gasneti_nodes);
    gasneti_assert_uint(result->output.result2 ,==, ((uint64_t)gasneti_nodes * (gasneti_nodes - 1)) / 2);
  }
#endif
}

/*------ Post Fma for Aries CE */
gasnete_ce_result_t *gasnetc_post_ce(gasnetc_post_descriptor_t *gpd)
{
  gasneti_assert(gasnete_ce_available);

  // TODO: multiple such buffers?
  gasnete_ce_result_t *result = &gasnete_ce_results[0];

  // Completion via flag
  result->done = 0;
  gpd->gpd_completion = (uintptr_t) &result->done;
  gpd->gpd_flags = GC_POST_COMPLETION_FLAG;

  gni_post_descriptor_t * const pd = &gpd->pd;
  pd->type = GNI_POST_CE;
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->local_addr = (uint64_t) &result->output;
  pd->local_mem_hndl = my_aux_handle;

  // CE reduction ID currently unused
  gasneti_assert(! pd->ce_red_id);

  gni_return_t status = myPostFma(my_ce_ep_handle, gpd, 0);
  if_pf (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort("GNI_POST_CE failed with %s", gasnetc_gni_rc_string(status));
  }

  return result;
}

#if GASNET_PSHM
// Shared-memory "gate" to be used in gasnete_cebarrier_{notify,try}
// if/when {PPN + radix) is greater than GNI_CE_MAX_CHILDREN.

// This is like a split-phase barrier except that it allows for a (statically
// chosen) distinguished "parent" rank to perform work between the last arrival
// and the signalling of the children (releasing them from the "gate").

// TBD: if this is every used to synchronize consumption of a reduction
// accumulator, then the memory fence properties should be examined carefully
// (and documented in comments).

gasnete_ce_gate_t *gasnete_ce_gate = NULL; // The only shared-memory data
// Rank-local:
static gasneti_atomic_val_t ce_gate_size; // will be 0 when unused
static int ce_gate_my_phase;
static volatile int *ce_gate_my_phase_p;
static volatile int *ce_gate_done_phase_p;

static void ce_gate_init(void) {
  gasneti_assert(gasnete_ce_gate);

  if (1 == gasneti_pshm_nodes) {
    // Singleton - no need for synchronization
    ce_gate_size = 0;
    return;
  }

  ce_gate_my_phase = 0;
  ce_gate_size     = gasneti_pshm_nodes;
  if (!gasneti_pshm_mynode) {
    gasnete_ce_gate->done_phase = 0;
    for (int i = 0; i < ce_gate_size; ++i) {
      gasnete_ce_gate->rank[i].phase = 0;
    }
  }

  // Pull some address arithmetic out of critical path
  ce_gate_my_phase_p = &gasnete_ce_gate->rank[gasneti_pshm_mynode].phase;
  ce_gate_done_phase_p = &gasnete_ce_gate->done_phase;
}

// Notify and Try and Release to be called by "parent"
#define ce_gate_parent_notify() ((ce_gate_my_phase ^= 1), ce_gate_parent_try())
GASNETI_INLINE(ce_gate_parent_try)
int ce_gate_parent_try(void)
{
  // 'rank[0]' is self and thus unused
  for (int i = 1; i < ce_gate_size; ++i) {
    if (ce_gate_my_phase != gasnete_ce_gate->rank[i].phase) return 0;
  }
  return 1;
}
#define ce_gate_release()       (*ce_gate_done_phase_p = ce_gate_my_phase)

// Notify and Try to be called by "child"
#define ce_gate_child_notify()  (void)(*ce_gate_my_phase_p = (ce_gate_my_phase ^= 1))
#define ce_gate_child_try()     (*ce_gate_done_phase_p == ce_gate_my_phase)
#endif // GASNET_PSHM

// Simple offloaded consensus barrier
// This uses the Aries CE reduce-to-all for its barrier side-effect
static gasnete_ce_result_t *cebarrier_result;
GASNETI_INLINE(gasnete_cebarrier_notify_inner)
void gasnete_cebarrier_notify_inner(void)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor(0 GASNETC_DIDX_PASS);
  gpd->gpd_ce_cmd  = GNI_FMA_CE_AND;
  gpd->gpd_ce_mode = 0;
  cebarrier_result = gasnetc_post_ce(gpd);
}
void gasnete_cebarrier_notify(void)
{
#if GASNET_PSHM
  if (ce_gate_size) {
    if (gasneti_pshm_mynode) {
      ce_gate_child_notify();
      return;
    } else {
      if (! ce_gate_parent_notify()) {
        // we are parent, but not last arrival
        cebarrier_result = NULL; // read by _try
        return;
      }
      // otherwise we are parent and last arrival - fall through
    }
  }
#endif
  gasnete_cebarrier_notify_inner();
}
int gasnete_cebarrier_try(void)
{
#if GASNET_PSHM
  if (ce_gate_size) {
    if (gasneti_pshm_mynode) {
      // child simply waits for Release by parent
      return ce_gate_child_try() ? GASNET_OK : GASNET_ERR_NOT_READY;
    } else if (!cebarrier_result) {
      // have not yet called passed the "gate" and initiated comms
      if (ce_gate_parent_try()) {
        gasnete_cebarrier_notify_inner();
        // no point falling through since test cannot succeed w/o a poll
      }
      return GASNET_ERR_NOT_READY;
    }
    // otherwise fall through
  }
#endif
  int status = gasnete_test_ce(cebarrier_result);
  if_pt (status == GNI_RC_SUCCESS) {
  #if GASNET_PSHM
    if (ce_gate_size) ce_gate_release();
  #endif
    return GASNET_OK;
  }
  gasneti_assert(status == GNI_RC_NOT_DONE);
  return GASNET_ERR_NOT_READY;
}
#endif // GASNETC_BUILD_GNICE

// Padded size, used to retain cache alignment
#define GASNETC_SIZEOF_GDP GASNETI_ALIGNUP(sizeof(gasnetc_post_descriptor_t), GASNETC_CACHELINE_SIZE)

/* Needs no lock because it is called only from the init code */
static
void gasnetc_init_post_descriptor_pool(GASNETC_DIDX_FARG_ALONE) 
{
  int i;
  const int count = gasnetc_pd_buffers.size / GASNETC_SIZEOF_GDP /  gasnetc_domain_count_max;
  uintptr_t addr;

  gasneti_assert_always(gasnetc_pd_buffers.addr != NULL);

#if GASNETC_USE_MULTI_DOMAIN
  /* must first destroy the temporary pool of post descriptors (only first domain) */
  if_pf (GASNETC_DIDX == GASNETC_DEFAULT_DOMAIN) {
    for (i=0; i < gasnetc_log2_remote; ++i) {
      gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor(0 GASNETC_DIDX_PASS);
      gasneti_free(gpd);
    }
  }

  addr = (uintptr_t) gasnetc_pd_buffers.addr;
  addr += count * GASNETC_DIDX * GASNETC_SIZEOF_GDP;
  memset((void*)addr, 0, count * GASNETC_SIZEOF_GDP); /* Just in case */
  /* sacrifice the first one to work as a padding */
  for (i = 1; i < count; i += 1) {
    addr += GASNETC_SIZEOF_GDP;
    gasneti_lifo_push(&DOMAIN_SPECIFIC_VAL(post_descriptor_pool), (void*)addr);
  }
#else
  /* must first destroy the temporary pool of post descriptors */
  for (i=0; i < gasnetc_log2_remote; ++i) {
    gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor(0 GASNETC_DIDX_PASS);
    gasneti_free(gpd);
  }

  /* now create the new pool */
  addr = (uintptr_t) gasnetc_pd_buffers.addr;
  memset((void*)addr, 0, count * GASNETC_SIZEOF_GDP); /* Just in case */
  for (i = 0; i < count; i += 1) {
    gasneti_lifo_push(&post_descriptor_pool, (void*)addr);
    addr += GASNETC_SIZEOF_GDP;
  }
#endif
}

/* This needs no lock because there is an internal lock in the queue */
GASNETI_MALLOC
gasnetc_post_descriptor_t *gasnetc_alloc_post_descriptor(gex_Flags_t flags GASNETC_DIDX_FARG)
{
  gasneti_lifo_head_t * const pool_p = &DOMAIN_SPECIFIC_VAL(post_descriptor_pool);
  gasnetc_post_descriptor_t *gpd = (gasnetc_post_descriptor_t *) gasneti_lifo_pop(pool_p);
  if_pf (!gpd) {
    /* We may simple not have polled the Cq recently.
       So, WAITHOOK and STALL tracing only if still nothing after first poll */
    GASNETC_TRACE_WAIT_BEGIN();
    gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
    gpd = (gasnetc_post_descriptor_t *) gasneti_lifo_pop(pool_p);
    if (gpd) {
      /* nothing to do */
    } else if (flags & GEX_FLAG_IMMEDIATE) {
      return NULL;
    } else {
      do {
        GASNETI_WAITHOOK();
        gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
        gpd = (gasnetc_post_descriptor_t *) gasneti_lifo_pop(pool_p);
      } while (!gpd);
      GASNETC_TRACE_WAIT_END(ALLOC_PD_STALL);
    }
  }
  gpd->gpd_flags = 0;
#if GASNETC_USE_MULTI_DOMAIN
  gpd->domain_idx = GASNETC_DIDX;
#endif

  return(gpd);
}


/* This needs no lock because there is an internal lock in the queue */
/* LCS inline this */
void gasnetc_free_post_descriptor(gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  gasneti_lifo_push(&DOMAIN_SPECIFIC_VAL(post_descriptor_pool), gpd);
}

/* exit related */
volatile int gasnetc_shutdownInProgress = 0;
double gasnetc_shutdown_seconds = 0.0;
static             uint32_t sys_exit_sent_mask = 0;
static                  int sys_exit_sent_init = 0;
static gasneti_weakatomic_t sys_exit_sent_fini = gasneti_weakatomic_init(0);
static gasneti_weakatomic_t sys_exit_rcvd = gasneti_weakatomic_init(0);
static gasneti_weakatomic_t sys_exit_code = gasneti_weakatomic_init(0);

#if GASNET_PSHM
gasnetc_exitcode_t *gasnetc_exitcodes = NULL;
#endif

/* this is called from poll when a shutdown packet arrives */
static
void gasnetc_handle_sys_shutdown_packet(uint16_t arg)
{
  uint32_t distance = 1 << (arg >> 8);
  uint8_t exitcode = arg & 0xff;
  gasneti_weakatomic_val_t readval;

  GASNETI_TRACE_PRINTF(C,("Got SHUTDOWN Request from distance %d w/ exitcode %d",(int)distance,exitcode));

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
  GASNET_BEGIN_FUNCTION();
#if GASNET_PSHM                  
  const gex_Rank_t size = gasneti_nodemap_global_count;
  const gex_Rank_t rank = gasneti_nodemap_global_rank;
#else
  const gex_Rank_t size = gasneti_nodes;
  const gex_Rank_t rank = gasneti_mynode;
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
      GASNETI_WAITHOOK();
      gasnetc_poll(GASNETI_THREAD_PASS_ALONE);
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
        GASNETI_WAITHOOK();
        gasnetc_poll(GASNETI_THREAD_PASS_ALONE);
        if (gasneti_ticks_to_us(gasneti_ticks_now() - starttime) > timeout_us) {
          result = 2; /* failure */
          goto out;
        }
      }
      gasneti_local_rmb(); /* Acquire */
      exitcode = MAX(exitcode, peer->exitcode);
    }
  }
#endif

  for (distance = 1, shift = 0; distance < size; distance *= 2, ++shift) {
    gex_Rank_t peeridx = (distance >= size - rank) ? rank - (size - distance)
                                                      : rank + distance;
  #if GASNET_PSHM
    gex_Rank_t dest = gasneti_pshm_firsts[peeridx];
  #else
    gex_Rank_t dest = peeridx;
  #endif

   if (0 == (sys_exit_sent_mask & distance)) {
      GASNETI_TRACE_PRINTF(C,("Send SHUTDOWN Request to node %d w/ shift %d, exitcode %d",
                              dest,shift,exitcode));
      gasnetc_send_control(dest, GC_CTRL_SHUTDOWN, (shift << 8) | (exitcode & 0xff), &sys_exit_sent_fini);
      sys_exit_sent_init += 1;
      sys_exit_sent_mask |= distance;
    }

    /* wait for completion of the proper receive, which might arrive out of order */
    goal |= distance;
    while ((gasneti_weakatomic_read(&sys_exit_rcvd, 0) & goal) != goal) {
      GASNETI_WAITHOOK();
      gasnetc_poll(GASNETI_THREAD_PASS_ALONE);
      if (gasneti_ticks_to_us(gasneti_ticks_now() - starttime) > timeout_us) {
        result = 3; /* failure */
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

  GASNETI_TRACE_PRINTF(C,("Leaving SYS EXIT with result=%d", result));
  return result;
}

/* Clean ups prior to "bottom half" of gasnetc_exit() */
extern void gasnetc_sys_fini(void) {
  GASNET_BEGIN_FUNCTION();
  /* Drain completions for sent exitcode-reduction messages */
  if (gasneti_weakatomic_read(&sys_exit_sent_fini, 0) != sys_exit_sent_init) {
    gasnetc_poll(GASNETI_THREAD_PASS_ALONE);
    while (gasneti_weakatomic_read(&sys_exit_sent_fini, 0) != sys_exit_sent_init) {
      GASNETI_WAITHOOK();
      gasnetc_poll(GASNETI_THREAD_PASS_ALONE);
    }
  }

#if GASNET_PSHM
  /* Coordinate release of of PSHM aux segment */
  if (gasneti_nodemap_local_rank) {
    gasnetc_exitcodes[gasneti_nodemap_local_rank].present = 0;
  } else {
    /* await acknowledgements */
    for (int i = 1; i < gasneti_nodemap_local_count; ++i) {
      gasnetc_exitcode_t * const peer = &gasnetc_exitcodes[i];
      while (peer->present) {
        GASNETI_WAITHOOK();
        gasnetc_poll(GASNETI_THREAD_PASS_ALONE);
      }
    }
  }
#endif
}

/* AuxSeg setup for registered bounce buffer space*/
gasneti_auxseg_request_t gasnetc_bounce_auxseg_alloc(gasnet_seginfo_t *auxseg_info) {
  gasneti_auxseg_request_t retval;

  retval.minsz =
  retval.optimalsz = gasnetc_domain_count_max *
                     gasneti_getenv_int_withdefault("GASNET_GNI_BOUNCE_SIZE",
                                                    GASNETC_GNI_BOUNCE_SIZE_DEFAULT,1);
  if (auxseg_info != NULL) { /* auxseg granted */
    /* The only one we care about is our own node */
    gasnetc_bounce_buffers = auxseg_info[gasneti_mynode];
  }

  return retval;
}

/* AuxSeg setup for registered post descriptors*/
gasneti_auxseg_request_t gasnetc_pd_auxseg_alloc(gasnet_seginfo_t *auxseg_info) {
  gasneti_auxseg_request_t retval;
  
  retval.minsz =
  retval.optimalsz = gasnetc_domain_count_max * num_pd * GASNETC_SIZEOF_GDP;

  if (auxseg_info != NULL) { /* auxseg granted */
    /* The only one we care about is our own node */
    gasnetc_pd_buffers = auxseg_info[gasneti_mynode];
  }

  return retval;
}

static
void gasnetc_init_bounce_buffer_pool(GASNETC_DIDX_FARG_ALONE)
{
  int i;
  int num_bounce;
  size_t buffer_size;
  gasneti_assert_always(gasnetc_bounce_buffers.addr != NULL);

  buffer_size = MAX(gasnetc_get_bounce_register_cutover,
                    gasnetc_put_bounce_register_cutover);
  buffer_size = MAX(buffer_size, GASNETC_MSG_MAXSIZE);
  buffer_size = GASNETI_ALIGNUP(buffer_size, GASNETC_CACHELINE_SIZE);

  num_bounce = gasnetc_bounce_buffers.size / buffer_size / gasnetc_domain_count_max;

#if GASNETC_USE_MULTI_DOMAIN
  /* sacrifice one bounce buffer to work as a padding */
  for(i = 1; i < num_bounce; i += 1) {
    gasneti_lifo_push(&DOMAIN_SPECIFIC_VAL(bounce_buffer_pool),
                      (char *) gasnetc_bounce_buffers.addr + (buffer_size *
                                                              (i+GASNETC_DIDX*num_bounce)));
  }
#else
  for(i = 0; i < num_bounce; i += 1) {
    gasneti_lifo_push(&bounce_buffer_pool,
                      (char *) gasnetc_bounce_buffers.addr + (buffer_size * i));
  }
#endif
}

