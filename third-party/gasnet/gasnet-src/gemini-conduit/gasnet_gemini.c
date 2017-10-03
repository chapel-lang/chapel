#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_handler.h>
#include <gasnet_gemini.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>

#ifndef container_of
/* Convert from address of a member to address of the containing structure
 * ptr = member pointer
 * type = container's type
 * field = member's field name
 */
  #define container_of(ptr,type,field) ((type*) ((uintptr_t)(ptr) - offsetof(type,field)))
#endif

#define GASNETC_NETWORKDEPTH_SPACE_DEFAULT (12*1024)
#define GASNETC_NETWORKDEPTH_TOTAL_DEFAULT 64

#ifdef GASNET_CONDUIT_GEMINI
  /* Use remote event + PI_FLUSH to get "proper" ordering w/ relaxed and default PI ordering */
  #define FIX_HT_ORDERING 1
#else
  #define FIX_HT_ORDERING 0
#endif

#if GASNET_DEBUG
  #define GASNETC_CDM_MODE GNI_CDM_MODE_FORK_FULLCOPY | GNI_CDM_MODE_ERR_NO_KILL
#else
  #define GASNETC_CDM_MODE GNI_CDM_MODE_FORK_FULLCOPY
#endif
static uint32_t gasnetc_cdm_mode = GASNETC_CDM_MODE;

int      gasnetc_dev_id;
uint32_t gasnetc_cookie;
uint32_t gasnetc_address;
uint8_t  gasnetc_ptag;

static uint32_t gasnetc_memreg_flags;
static int gasnetc_mem_consistency;

typedef struct peer_struct {
  unsigned int pe;
  gni_ep_handle_t ep_handle;
  gni_mem_handle_t mem_handle;
  gni_mem_handle_t am_handle;

  gasnetc_notify_t *remote_notify_base;
  gasnetc_notify_t *local_notify_base;
  uint8_t *remote_reply_base;
  uint8_t *local_request_base;
  uint8_t *remote_request_base;

#if GASNET_PAR
  volatile int remote_request_lock;
#endif
  uint64_t remote_request_map;    
  uint32_t remote_notify_write; /* covered by the gni lock, unbounded */
  uint32_t local_notify_read;   /* covered by the ampoll lock, bounded [0..notify_ring_size) */
  struct peer_struct *next;     /* covered by the ampoll lock */
  unsigned int event_count;     /* covered by the ampoll lock */
#if GASNETC_USE_MULTI_DOMAIN
  uint32_t nic_addr;
#endif
} peer_struct_t;

static gni_mem_handle_t am_handle;

static void *am_mmap_ptr;
static size_t am_mmap_bytes;

typedef struct reply_pool {
  gasnetc_packet_t  *packet;         /* Corresponding mailbox */
  union {
    struct reply_pool *next;         /* Next when on reply_freelist */
    uint64_t           request_bits; /* Request slots to be restored upon Reply */
  } u;
} reply_pool_t;

static reply_pool_t *reply_pool = NULL;
static reply_pool_t *reply_freelist = NULL;

static gasnet_seginfo_t gasnetc_bounce_buffers;
static gasnet_seginfo_t gasnetc_pd_buffers;

unsigned int gasnetc_log2_remote;
static unsigned int num_pd;
static uint32_t notify_ring_mask; /* ring size minus 1 */
static unsigned int am_slotsz;
static unsigned int am_slot_bits;

static int have_segment = 0;

static gni_cq_handle_t am_cq_handle;
static int gasnetc_poll_burst = 10;
#if FIX_HT_ORDERING
static uint16_t gasnetc_fma_put_cq_mode = GNI_CQMODE_GLOBAL_EVENT;
#endif
static size_t gasnetc_get_fma_rdma_cutover;
static size_t gasnetc_put_fma_rdma_cutover;
static size_t gasnetc_get_bounce_register_cutover;
static size_t gasnetc_put_bounce_register_cutover;
size_t gasnetc_max_get_unaligned;
size_t gasnetc_max_put_lc;

/* read-only: */
static gni_mem_handle_t my_mem_handle;

#if GASNETC_USE_MULTI_DOMAIN
static unsigned int gasnetc_domain_count;
static unsigned int gasnetc_poll_am_domain_mask;
#if (GASNETC_DOMAIN_THREAD_DISTRIBUTION == GASNETC_DOMAIN_THREAD_DISTRIBUTION_BULK)
static int gasnetc_threads_per_domain;
#endif

typedef struct {
  int8_t pads[GASNETC_CACHELINE_SIZE];
  gni_cdm_handle_t cdm_handle;
  gni_cq_handle_t destination_cq_handle;
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

int gasnetc_my_domain_idx(void) {
    const gasnete_threaddata_t * const mythread = gasnete_mythread();
    return mythread->domain_idx;
}
#else /* GASNETC_USE_MULTI_DOMAIN */

#define gasnetc_domain_count 1

static gni_cdm_handle_t cdm_handle;
static gni_cq_handle_t destination_cq_handle;

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

/* NOTE: notify_type is "pre shifted" by 24 bits */
enum notify_type {
  notify_request = 0x01000000,
  notify_reply   = 0x02000000,
  notify_credit  = 0x03000000,
  notify_ctrl    = 0x04000000
}; 

#define build_notify(_type, _initiator, _target)\
  ((uint64_t)(_type) |  ((uint64_t)(_initiator) << 8) |  ((uint64_t)(_target)))

#define notify_get_type(n) ((n) & 0xff000000)
#define notify_get_target_slot(n) ((uint8_t)((n) & 255)) /* actual range 0..63 */
#define notify_get_initiator_slot(n) ((uint16_t)(((n) >> 8) & 65535))

/* Op and Arg for a control msg are also 8 and 16 bits, respectively.
   However we could use at total of 56 bits (keeping 8 for type) if ever needed.
*/
#define build_ctrl_notify(_op, _arg) build_notify(notify_ctrl, _arg, _op)
#define notify_ctrl_op(n) notify_get_target_slot(n)
#define notify_ctrl_arg(n) notify_get_initiator_slot(n)

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
static int gasnetc_register_udreg(gasnetc_post_descriptor_t *gpd, uint32_t flags)
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
static int gasnetc_register_gni(gasnetc_post_descriptor_t *gpd, uint32_t flags)
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

  flags |= gasnetc_memreg_flags;
  for (;;) {
    GASNETC_LOCK_GNI();
    status = GNI_MemRegister(nic_handle, addr, nbytes, NULL,
                             flags, -1, &pd->local_mem_hndl);
    GASNETC_UNLOCK_GNI();
    if_pt (status == GNI_RC_SUCCESS) {
      if (trial) GASNETC_STAT_EVENT_VAL(MEM_REG_RETRY, trial);
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
int gasnetc_register_gpd(gasnetc_post_descriptor_t *gpd, uint32_t flags)
{
#if GASNETC_GNI_UDREG
  if_pt (gasnetc_udreg_hndl) {
    return gasnetc_register_udreg(gpd, flags);
  } else
#endif
  return gasnetc_register_gni(gpd, flags);
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

#if GASNETC_GNI_FIREHOSE
/* Acquire firehose covering (at least some leading portion of) the xfer given by gdp */
GASNETI_INLINE(gasnetc_firehose_acquire)
size_t gasnetc_firehose_acquire(gasnetc_post_descriptor_t *gpd)
{
  gni_post_descriptor_t * const pd = &gpd->pd;
  const uintptr_t loc_addr = pd->local_addr;
  const size_t aligned = (2 * gasnetc_fh_align) - (loc_addr & gasnetc_fh_align_mask);
  const firehose_request_t * fh_loc;
  fh_loc = firehose_local_pin(loc_addr, MIN(pd->length, aligned), &gpd->u.fh_req);
  gasneti_assert(fh_loc == &gpd->u.fh_req);
  pd->local_mem_hndl = gpd->u.fh_req.client;
  if_pf (! gasneti_valid_client_t(& pd->local_mem_hndl)) {
    /* Failed memory registration */
    firehose_release(&fh_loc, 1);
    return 0;
  }
  return MIN(pd->length, fh_loc->addr + fh_loc->len - loc_addr);
}

/* Release firehose linked to gpd */
GASNETI_INLINE(gasnetc_firehose_release)
void gasnetc_firehose_release(gasnetc_post_descriptor_t *gpd)
{
  const firehose_request_t *fh_req = &gpd->u.fh_req;
  firehose_release(&fh_req, 1);
}
#endif

/*-------------------------------------------------*/
/* We don't allocate resources for comms w/ self or PSHM-reachable peers */

#if GASNET_PSHM
  #define node_is_local(_i) gasneti_pshm_in_supernode(_i)
#else
  #define node_is_local(_i) ((_i) == gasneti_mynode)
#endif

/* From point-of-view of a remote node, what is MY index in the mailbox array */
GASNETI_INLINE(my_mb_index)
int my_mb_index(gasnet_node_t remote_node) {
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
/* called after segment init. See gasneti_seginfo */
/* allgather the memory handles for the segments */
/* create endpoints */
void gasnetc_init_segment(void *segment_start, size_t segment_size)
{
  gni_return_t status;
  /* Map the shared segment */
#if GASNETC_USE_MULTI_DOMAIN
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  DOMAIN_SPECIFIC_VAR(gni_nic_handle_t, nic_handle);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  gni_cq_handle_t  destination_cq_handle = NULL;
#endif
  size_t bb_size = gasnetc_bounce_buffers.size / gasnetc_domain_count;
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
#if GASNET_CONDUIT_GEMINI
  gasnetc_max_put_lc = MAX(gasnetc_put_fma_rdma_cutover,
                           gasnetc_put_bounce_register_cutover);
#else
  gasnetc_max_put_lc = MAX(GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE,
                           gasnetc_put_bounce_register_cutover);
#endif

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
			       gasnetc_memreg_flags|GNI_MEM_READWRITE, -1,
			       &my_mem_handle);
      if (status == GNI_RC_SUCCESS) break;
      if (status == GNI_RC_ERROR_RESOURCE) {
	gasnetc_GNIT_Log("MemRegister segment fault %d at  %p %"PRIxPTR", code %s", 
		count, segment_start, (uintptr_t)segment_size, gasnetc_gni_rc_string(status));
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
    gasnet_node_t i;
    gasnetc_bootstrapExchange_gni(&my_mem_handle, sizeof(gni_mem_handle_t), all_mem_handle);
    for (i = 0; i < gasneti_nodes; ++i) {
      peer_data[i].mem_handle = all_mem_handle[i];
    }
    gasneti_free(all_mem_handle);
  }

#if GASNETC_USE_MULTI_DOMAIN
  DOMAIN_SPECIFIC_VAL(destination_cq_handle) = destination_cq_handle;
  
 #if(GASNETC_DOMAIN_ALLOC_POLICY == GASNETC_STATIC_DOMAIN_ALLOC)
  {
    int i;
    for(i=1;i<gasnetc_domain_count;i++)
      gasnetc_create_parallel_domain(gasnetc_get_domain_first_thread_idx(i));
  }
 #endif
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

  status = GNI_CqCreate(DOMAIN_SPECIFIC_VAL(nic_handle), num_pd + 2, 0, GNI_CQ_NOBLOCK, 
                        NULL, NULL, &DOMAIN_SPECIFIC_VAL(bound_cq_handle));
  gasneti_assert_always (status == GNI_RC_SUCCESS);
  /* create and bind endpoints */
#if GASNET_DEBUG
  DOMAIN_SPECIFIC_VAL(peer_data) = gasneti_calloc(gasneti_nodes, sizeof(peer_struct_t));
#else
  DOMAIN_SPECIFIC_VAL(peer_data) = gasneti_malloc(gasneti_nodes * sizeof(peer_struct_t));
#endif
  for (i = 0; i < gasneti_nodes; i += 1) {
  #if !GASNETC_GNI_FETCHOP
    if (node_is_local(i)) continue; /* no connection to self or PSHM-reachable peers */
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
#if FIX_HT_ORDERING
  if (gasnetc_mem_consistency != GASNETC_STRICT_MEM_CONSISTENCY) {
    /* With 1 completion entry this queue is INTENDED to always overflow */
    status = GNI_CqCreate(DOMAIN_SPECIFIC_VAL(nic_handle), 1, 0, GNI_CQ_NOBLOCK, NULL, NULL, &DOMAIN_SPECIFIC_VAL(destination_cq_handle));
    gasneti_assert_always (status == GNI_RC_SUCCESS);
  }
#else
  DOMAIN_SPECIFIC_VAL(destination_cq_handle) = NULL;
#endif

  /* TODO: this replication is unnecessary, but cache-friendly: */
  for (i = 0; i < gasneti_nodes; ++i) {
    DOMAIN_SPECIFIC_VAL(peer_data[i]).mem_handle = gasnetc_cdom_data[0].peer_data[i].mem_handle;
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
  const gasnet_node_t remote_nodes = gasneti_nodes - (GASNET_PSHM ? gasneti_nodemap_local_count : 1);
  gni_return_t status;
  uint32_t local_address;
  uint32_t i;
  uint64_t request_map;
  size_t request_region_length;
  size_t reply_region_length;
  size_t peer_stride;
  int am_maxcredit;
  int notify_ring_size;
  int reply_count;

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

  /* Determine space for AM Requests: GASNET_NETWORKDEPTH_SPACE */
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
  request_map = (am_maxcredit == 64) ? ~(uint64_t)0 : (((uint64_t)1 << am_maxcredit) - 1);
 
  /* Determine number of Reply buffers */
  reply_count = gasneti_getenv_int_withdefault("GASNET_NETWORKDEPTH_TOTAL",
                                               GASNETC_NETWORKDEPTH_TOTAL_DEFAULT, 0);
  reply_count = MAX(1, reply_count); /* Min is 1 */

  /* Max number of AM Requests outstanding may be constrained by available Reply buffers: */
  am_maxcredit = MIN(am_maxcredit, reply_count);

  { /* Determine Cq size: GASNET_GNI_NUM_PD */
    int cq_entries;
    num_pd = gasneti_getenv_int_withdefault("GASNET_GNI_NUM_PD",
                                            GASNETC_GNI_NUM_PD_DEFAULT,0);
    num_pd = MAX(32, num_pd); /* Min is 32 (XXX: should be cores+1) */

    cq_entries = num_pd+2; /* XXX: why +2 ?? */

    status = GNI_CqCreate(nic_handle, cq_entries, 0, GNI_CQ_NOBLOCK, NULL, NULL, &bound_cq_handle);
    gasneti_assert_always (status == GNI_RC_SUCCESS);
  }

  /* Determine size of per-peer notify ring: Req + Rep + shutdown */
  notify_ring_size = GASNETI_ALIGNUP(gasnetc_next_power_of_2(2*am_maxcredit + 1),
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
  i = gasnetc_log2_remote + 2*remote_nodes*am_maxcredit; /* 2 = Request + Reply */
  status = GNI_CqCreate(nic_handle,i,0,GNI_CQ_NOBLOCK,NULL,NULL,&am_cq_handle);
  if (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort("GNI_CqCreate returned error %s", gasnetc_gni_rc_string(status));
  }
  
  /*
   * Set up an mmap region to contain all of my mailboxes.
   */

  reply_region_length = reply_count * GASNETC_MSG_MAXSIZE;
  peer_stride = request_region_length + notify_ring_size * sizeof(gasnetc_notify_t);

  /* TODO: remove MAX(1,) while still avoiding "issues" on single-(super)node runs */
  am_mmap_bytes = reply_region_length + MAX(1,remote_nodes) * peer_stride;
  
#if defined(GASNETI_USE_HUGETLBFS)
  am_mmap_ptr = gasneti_huge_mmap(NULL, am_mmap_bytes);
#else
  am_mmap_ptr = gasneti_mmap(am_mmap_bytes);
#endif
  if (am_mmap_ptr == (char *)MAP_FAILED) {
    gasnetc_GNIT_Abort("am mmap failed: ");
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

  reply_pool = gasneti_malloc(reply_count * sizeof(reply_pool_t));
  for (i = 0; i < reply_count; i++) {
    reply_pool[i].u.next = &reply_pool[i + 1];
    reply_pool[i].packet = (gasnetc_packet_t *)((uintptr_t)am_mmap_ptr + i * GASNETC_MSG_MAXSIZE);
  }
  reply_freelist = reply_pool;
  reply_pool[reply_count - 1].u.next = NULL;

#if GASNET_DEBUG
  peer_data = gasneti_calloc(gasneti_nodes, sizeof(peer_struct_t));
#else
  peer_data = gasneti_malloc(gasneti_nodes * sizeof(peer_struct_t));
#endif

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
    uint8_t *local_peer_base = (uint8_t *)am_mmap_ptr + reply_region_length;

    gasneti_spawner->Exchange(&my_am_exchg, sizeof(struct am_exchange), all_am_exchg);
  
    /* At this point all_am_exchg has the required information for everyone */
    for (i = 0; i < gasneti_nodes; i += 1) {
    #if GASNETC_USE_MULTI_DOMAIN
      peer_data[i].nic_addr = all_am_exchg[i].nic_addr;
    #endif

    #if !GASNETC_GNI_FETCHOP
      if (!node_is_local(i)) /* no connection to self or PSHM-reachable peers */
    #endif
      {
        status = GNI_EpCreate(nic_handle, bound_cq_handle, &peer_data[i].ep_handle);
        gasneti_assert_always (status == GNI_RC_SUCCESS);
        status = GNI_EpBind(peer_data[i].ep_handle, all_am_exchg[i].nic_addr, i);
        gasneti_assert_always (status == GNI_RC_SUCCESS);
      }

      if (!node_is_local(i)) { /* no AMs to self or PSHM-reachable peers */
        peer_struct_t * const peer = &peer_data[i];
        uint8_t *remote_peer_base = all_am_exchg[i].addr + peer_stride * my_mb_index(i) + reply_region_length;

        peer->pe = i;
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
  if (have_segment && gasnetc_udreg_hndl) {
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
    DOMAIN_SPECIFIC_VAR(gni_cq_handle_t, destination_cq_handle);
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
      #if !GASNETC_GNI_FETCHOP
          if_pf (node_is_local(i)) continue; /* no connection to self or PSHM-reachable peers */
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
#if GASNETC_USE_MULTI_DOMAIN
    }
#endif

    if (destination_cq_handle) {
      status = GNI_CqDestroy(destination_cq_handle);
      if_pf (status != GNI_RC_SUCCESS) {
        gasnetc_GNIT_Log("CqDestroy(dest_cq) failed with %s", gasnetc_gni_rc_string(status));
      }
    }

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
void *gasnetc_alloc_bounce_buffer(GASNETC_DIDX_FARG_ALONE)
{
  gasneti_lifo_head_t * const pool_p = &DOMAIN_SPECIFIC_VAL(bounce_buffer_pool);
  void *buf = gasneti_lifo_pop(pool_p);
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
      gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
      buf = gasneti_lifo_pop(pool_p);
    } while (!buf);
    if_pf (stall) GASNETC_TRACE_WAIT_END(ALLOC_BB_STALL);
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
  const int max_trials = 4;
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

    if_pf (++trial == max_trials) {
      return GASNET_ERR_RESOURCE;
    }

    GASNETI_WAITHOOK();
    gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
    GASNETC_LOCK_GNI();
  }

  if_pf (trial) GASNETC_STAT_EVENT_VAL(AM_SEND_RETRY, trial);
  return GASNET_OK;
}

extern int
gasnetc_send_am(gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  gni_post_descriptor_t *pd = &gpd->pd;
  peer_struct_t * const peer = (peer_struct_t *)gpd->gpd_am_peer;
#if GASNET_TRACE
  gasnetc_notify_t n = pd->sync_flag_value;
#endif
  unsigned int slot;

  GASNETI_TRACE_PRINTF(D, ("msg to %d type %s/%s\n", peer->pe,
                           gasnetc_type_string(gasnetc_am_command(n)),
                           (notify_get_type(n) == notify_request) ? "REQ" : "REP"));

  GASNETC_LOCK_GNI();
  
  slot = fetch_inc_notify_pointer(peer->remote_notify_write);
  pd->sync_flag_addr = (uint64_t)(peer->remote_notify_base + slot);
  return(gasnetc_send_am_common(peer, pd));
}

GASNETI_INLINE(gasnetc_send_notify)
int gasnetc_send_notify(peer_struct_t * const peer, gasnetc_notify_t notify, gasneti_weakatomic_t *cntr)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor(GASNETC_DIDX_PASS_ALONE);
  gni_post_descriptor_t *pd = &gpd->pd;
  unsigned int slot;

  if (cntr) {
    gpd->gpd_completion = (uintptr_t)cntr;
    gpd->flags = GC_POST_COMPLETION_CNTR;
  }
  gpd->u.notify = notify;
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT | GNI_CQMODE_REMOTE_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->type = GNI_POST_FMA_PUT;
  pd->length = sizeof(gasnetc_notify_t);
  pd->local_addr = (uint64_t)&gpd->u.notify;
  pd->remote_mem_hndl = peer->am_handle;

  GASNETC_LOCK_GNI();

  slot = fetch_inc_notify_pointer(peer->remote_notify_write);
  pd->remote_addr = (uint64_t)(peer->remote_notify_base + slot);
  return(gasnetc_send_am_common(peer, pd));
}

GASNETI_INLINE(gasnetc_send_credit)
int gasnetc_send_credit(peer_struct_t * const peer, gasnetc_notify_t notify)
{
  GASNETI_TRACE_PRINTF(D, ("msg to %d type AM_CREDIT\n", peer->pe));
  gasneti_assert(notify_get_type(notify) == notify_request);
  notify += build_notify((notify_credit - notify_request),0,0); /* just modify the notify type */
  return(gasnetc_send_notify(peer, notify, NULL));
}

/* Send a 3-byte control message (could have us much as 7 bytes if ever needed) */
/* Current ARBITRARILY managed as 8-bit op and 16-bit arg */
GASNETI_INLINE(gasnetc_send_control)
int gasnetc_send_control(gasnet_node_t dest, uint8_t op, uint16_t arg, gasneti_weakatomic_t *cntr)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  peer_struct_t * const peer = &peer_data[dest];
  GASNETI_TRACE_PRINTF(D, ("msg to %d type CONTROL op=%d arg=0x%x\n",
                           (int)dest, (int)op, (int)arg));
  return(gasnetc_send_notify(peer, build_ctrl_notify(op, arg), cntr));
}

GASNETI_INLINE(gasnetc_format_am_gpd) 
void gasnetc_format_am_gpd(gasnetc_post_descriptor_t *gpd, 
                           gasnetc_packet_t *p,
                           peer_struct_t *peer,
                           size_t length, uint32_t flags)
{
  gni_post_descriptor_t *pd = &gpd->pd;

  gpd->flags = flags;
  gpd->gpd_am_peer = (uint64_t) peer; 
  pd->length = length;
  pd->local_addr = (uint64_t)p;
  pd->remote_mem_hndl = peer->am_handle;
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT | GNI_CQMODE_REMOTE_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->type = GNI_POST_FMA_PUT_W_SYNCFLAG;
}

gasnetc_post_descriptor_t *gasnetc_alloc_reply_post_descriptor(gasnet_token_t t,
                                                               size_t length)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  gasnetc_token_t *token = (gasnetc_token_t *)t;
  peer_struct_t * const peer = &peer_data[token->source];
  gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor(GASNETC_DIDX_PASS_ALONE);
  gni_post_descriptor_t *pd = &gpd->pd;
  gasnetc_notify_t notify = token->notify;
  gasnetc_packet_t *packet;
  uint32_t flags = 0;

  /* Find space to construct the Reply.
   * NOTE that we don't *need* registered memory.
   * However all the memory pools we manage happen to be registered.
   */
  if (length <= GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE) {
    /* Use in-gpd buffer */
    packet = (gasnetc_packet_t *) gpd->u.immediate;
  } else {
    /* Try to reuse the Request buffer for the Reply */
    const int numargs = gasnetc_am_numargs(notify);
    uint32_t target_slot = notify_get_target_slot(notify);
    unsigned int req_len = 0;
    packet = (gasnetc_packet_t *) (peer->local_request_base + (target_slot << am_slot_bits));

    switch (gasnetc_am_command(notify)) {
      case GC_CMD_AM_SHORT:
        req_len = GASNETC_HEADLEN(short, numargs);
        req_len = req_len ? req_len : 1; /* request never allocates zero */
        break;
      case GC_CMD_AM_MEDIUM: {
        if_pt (0 != gasnetc_am_nbytes(notify)) {
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
      packet = (gasnetc_packet_t *) gasnetc_alloc_bounce_buffer(GASNETC_DIDX_PASS_ALONE);
      flags = GC_POST_UNBOUNCE;
    }
  }

  /* modify the notify type and clear its AM header bits */
  gasneti_assert(notify_get_type(notify) == notify_request);
  pd->sync_flag_value = (notify & 0xffffffffUL) + build_notify((notify_reply - notify_request),0,0);
  
  pd->remote_addr = (uint64_t) (peer->remote_reply_base +
                                GASNETC_MSG_MAXSIZE * notify_get_initiator_slot(notify));
  gasnetc_format_am_gpd(gpd, packet, peer, length, flags);
  gasneti_assert(token->need_reply);
  token->need_reply = 0;
  /* If Medium payload is in-use, then defer sending Reply until Request returns (avoids overwrite race) */
  if (0 != gasnetc_am_nbytes(notify)) {
    gasneti_assert(GC_CMD_AM_MEDIUM == gasnetc_am_command(notify));
    token->deferred_reply = gpd;
  }
  return(gpd);
}

#define BUSYWAIT(_condition, _poll, _trace)  \
    if_pf (_condition) {                     \
      GASNETC_TRACE_WAIT_BEGIN();            \
      do {                                   \
        GASNETC_UNLOCK_AM_BUFFER();          \
        GASNETI_WAITHOOK();                  \
        _poll();                             \
        GASNETC_LOCK_AM_BUFFER();            \
      } while (_condition);                  \
      GASNETC_TRACE_WAIT_END(_trace);        \
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

gasnetc_post_descriptor_t *gasnetc_alloc_request_post_descriptor(gasnet_node_t dest, 
                                                                 size_t length)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  peer_struct_t * const peer = &peer_data[dest];
  unsigned int my_slot;
  unsigned int remote_slot;
  const unsigned int slots = MAX(1, ((length + am_slotsz - 1) >> am_slot_bits));
  uint64_t mask = (slots == 64) ? ~(uint64_t)0 : (((uint64_t)1 << slots) - 1);
  reply_pool_t *r;

  GASNETC_LOCK_AM_BUFFER();

#if GASNET_PAR
  /* Prevent starvation of large allocations by small ones. */
  while (peer->remote_request_lock) {
    GASNETC_UNLOCK_AM_BUFFER();
    while (peer->remote_request_lock) GASNETI_WAITHOOK();
    GASNETC_LOCK_AM_BUFFER();
  }
  peer->remote_request_lock = 1;
#endif

  BUSYWAIT(((remote_slot = gasnetc_remote_slot(peer, mask)) == 64),
           gasnetc_AMPoll,
           GET_AM_REM_BUFFER_STALL);
  mask <<= remote_slot;
  peer->remote_request_map ^= mask;
#if GASNET_PAR
  peer->remote_request_lock = 0;
#endif

  BUSYWAIT(((r = reply_freelist) == NULL), 
           gasnetc_AMPoll,
           GET_AM_LOC_BUFFER_STALL);
  reply_freelist = r->u.next;

  GASNETC_UNLOCK_AM_BUFFER();

  r->u.request_bits = mask;

  gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor(GASNETC_DIDX_PASS_ALONE);
  gni_post_descriptor_t *pd = &gpd->pd;
  pd->remote_addr = (uint64_t) peer->remote_request_base + (remote_slot << am_slot_bits);
  pd->sync_flag_value = build_notify(notify_request, r - reply_pool, remote_slot);
  
  gasnetc_format_am_gpd(gpd, r->packet, peer, length, 0);
  
  return(gpd);
}

/* Choice to inline or not is left to the compiler */
void gasnetc_recv_am(peer_struct_t * const peer, gasnetc_packet_t * const packet, gasnetc_notify_t notify)
{
  int is_req = (notify_get_type(notify) == notify_request);
  const int numargs = gasnetc_am_numargs(notify);
  const int handlerindex = gasnetc_am_handler(notify);
  gasneti_handler_fn_t handler = gasnetc_handler[handlerindex];
  gasnetc_token_t the_token = { peer->pe, is_req, notify, NULL };
  gasnet_token_t token = (gasnet_token_t)&the_token; /* RUN macros need an lvalue */

  gasneti_mutex_unlock(&ampoll_lock);

  gasneti_assert(numargs <= gasnet_AMMaxArgs());
  GASNETI_TRACE_PRINTF(D, ("msg from %d type %s/%s\n", peer->pe,
                           gasnetc_type_string(gasnetc_am_command(notify)),
                           is_req ? "REQ" : "REP"));
  
  switch (gasnetc_am_command(notify)) {
  case GC_CMD_AM_SHORT:
      GASNETI_RUN_HANDLER_SHORT(is_req, handlerindex, handler,
                                token, packet->gasp.args, numargs);
      break;
      
  case GC_CMD_AM_MEDIUM: {
      const size_t head_len = GASNETC_HEADLEN(medium, numargs);
      uint8_t * data = (uint8_t *)packet + head_len;
      gasneti_assert(0 == (((uintptr_t) data) % GASNETI_MEDBUF_ALIGNMENT));
      gasneti_assert(gasnetc_am_nbytes(notify) <= gasnet_AMMaxMedium());
      GASNETI_RUN_HANDLER_MEDIUM(is_req, handlerindex, handler,
                                 token, packet->gamp.args, numargs,
                                 data, gasnetc_am_nbytes(notify));
      break;
  }
      
  case GC_CMD_AM_LONG_PACKED:
      { /* payload follows args - copy it into place */
          const size_t head_len = GASNETC_HEADLEN(long, numargs);
          uint8_t * data = (uint8_t *)packet + head_len;
          gasneti_assert(packet->galp.data_length <= GASNETC_MAX_PACKED_LONG(numargs));
          memcpy(packet->galp.data, data, packet->galp.data_length);
      }
      /* fall through... */
  case GC_CMD_AM_LONG:
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

  gasneti_mutex_lock(&ampoll_lock);
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

static void gasnetc_handle_sys_shutdown_packet(uint32_t source, uint16_t arg);

GASNETI_NEVER_INLINE(dispatch_ctrl,
static
void dispatch_ctrl(peer_struct_t * const peer,gasnetc_notify_t notify))
{
  const uint16_t arg = notify_ctrl_arg(notify);
  const uint8_t op = notify_ctrl_op(notify);
      
  switch (op) {
    case GC_CTRL_SHUTDOWN:
      gasnetc_handle_sys_shutdown_packet(peer->pe, arg);
      break;

  #if GASNET_DEBUG
    default:
      gasnetc_GNIT_Abort("unknown control message %d", (int)op);
  #endif
  }
}

GASNETI_INLINE(poll_for_message)
int poll_for_message(peer_struct_t * const peer, int is_slow)
{
  volatile gasnetc_notify_t * const notify = peer->local_notify_base + peer->local_notify_read;
  const gasnetc_notify_t n = *notify;

  if (n) { 
    uint32_t target_slot = notify_get_target_slot(n);
    uint32_t initiator_slot = notify_get_initiator_slot(n);
    uint32_t type = notify_get_type(n);

    *notify = 0;
    advance_notify_pointer(peer->local_notify_read);
    if (is_slow) ampoll_del(peer);

    gasneti_compiler_fence(); /* prevent compiler from prefetching over dependency on n!=0 */
    
    if (type == notify_request) {
      gasnetc_packet_t *packet = (gasnetc_packet_t *) (peer->local_request_base + (target_slot << am_slot_bits));
      gasnetc_recv_am(peer, packet, n);
    } else if_pf (type == notify_ctrl) {
      dispatch_ctrl(peer, n);
    } else {
      reply_pool_t *reply = reply_pool + initiator_slot;

      if (type == notify_reply) {
        gasnetc_recv_am(peer, reply->packet, n);
      } else {
        gasneti_assert(type == notify_credit);
        GASNETI_TRACE_PRINTF(D, ("msg from %d type AM_CREDIT\n", peer->pe));
      }

      GASNETC_LOCK_AM_BUFFER();
      peer->remote_request_map |= reply->u.request_bits;
      reply->u.next = reply_freelist;
      reply_freelist = reply;
      GASNETC_UNLOCK_AM_BUFFER();
    }
    return 1;
  }
  return 0;
}

/* Max number of times to poll the AM mailboxes per entry */
/* TODO: control via env var */
/* TODO: distinct value for CQ events reaped vs service limit on "slow" list? */
#define AM_BURST 20

static
void gasnetc_poll_am_queue(void)
{
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  gni_cq_entry_t event_data[AM_BURST];
  int count = 0;
  int i;

  /* Reap Cq entries until our queue is full, or Cq is empty */
#if GASNET_PAR
  /* Cray has assured us that GNI_CqTestEvent() requires no serialization */
  if (GNI_RC_NOT_DONE != GNI_CqTestEvent(am_cq_handle))
#endif
  {
  GASNETC_LOCK_GNI();
    for (count = 0; count < AM_BURST; ++count) {
      gni_return_t status = GNI_CqGetEvent(am_cq_handle, &event_data[count]);
      if (status != GNI_RC_SUCCESS) break; /* TODO: check for fatal errors */
      gasneti_assert(!GNI_CQ_OVERRUN(event_data[count]));
    }
  GASNETC_UNLOCK_GNI();
  }

  if (count) {
    /* Must take the lock to process new events */
    gasneti_mutex_lock(&ampoll_lock);

    for (i = 0; i < count; ++i) {
    #ifdef GNI_CQ_GET_REM_INST_ID
      /* Mar 2013 (S-2446-5002) docs introduces this call for use on recv CQs ... */
      uint32_t source = GNI_CQ_GET_REM_INST_ID(event_data[i]);
    #else
      /* ... while prior versions say this is used on both send and recv CQs */
      uint32_t source = GNI_CQ_GET_INST_ID(event_data[i]);
    #endif
      peer_struct_t * const peer = &peer_data[source];
      if (!poll_for_message(peer, 0)) {
        ampoll_ins(peer);
      }
    }
  } else if ((NULL == ampoll_head) || (EBUSY == gasneti_mutex_trylock(&ampoll_lock))) {
    /* Either there is no work to be done, or another thread is already doing it */
    return;
  }

  /* Poll "slow" sources, starting with the oldest */
  for (i = 0; ampoll_head && (i < AM_BURST); ++i) {
    peer_struct_t * const peer = ampoll_head;
    if (!poll_for_message(peer, 1)) {
      if (peer == ampoll_tail) break; /* don't spin on singleton peer */
      ampoll_last(peer);
    }
  }

  gasneti_mutex_unlock(&ampoll_lock);
}

/* Poll the bound_ep completion queue */
GASNETI_INLINE(gasnetc_poll_bound_cq)
gasnetc_post_descriptor_t *gasnetc_poll_bound_cq(GASNETC_DIDX_FARG_ALONE)
{
  DOMAIN_SPECIFIC_VAR(gni_cq_handle_t, bound_cq_handle);
  gni_post_descriptor_t * result = NULL;
  gni_cq_entry_t event_data;
  gni_return_t status;

#if GASNETC_USE_MULTI_DOMAIN
  if_pf (!DOMAIN_SPECIFIC_VAL(initialized)) return NULL;
#endif

  GASNETC_LOCK_GNI();
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
    gasnetc_GNIT_Abort("bound CqGetEvent %s", gasnetc_gni_rc_string(status));
  }
  GASNETC_UNLOCK_GNI();

  return result ? container_of(result, gasnetc_post_descriptor_t, pd) : NULL;
}

GASNETI_NEVER_INLINE(gasnetc_poll_local_queue,
void gasnetc_poll_local_queue(GASNETC_DIDX_FARG_ALONE))
{
  int i;

  for (i = 0; i < gasnetc_poll_burst; i += 1) {
    gasnetc_post_descriptor_t * const gpd = gasnetc_poll_bound_cq(GASNETC_DIDX_PASS_ALONE);

    if_pt (! gpd) { /* empty Cq is common case */
      break;
    } else {
      const uint32_t flags = gpd->flags; /* see note w/ GC_POST_COMPLETION_FLAG */

      /* handle remaining work */
      if (flags & GC_POST_COPY_IMM) {
        memcpy((void *) gpd->gpd_get_dst, (void *) gpd->u.immediate, gpd->pd.length);
        gasneti_sync_writes(); /* sync memcpy */
      } else
      if (flags & GC_POST_COPY) {
        const size_t length = gpd->pd.length - (flags & GC_POST_COPY_TRIM);
        memcpy((void *) gpd->gpd_get_dst, (void *) gpd->gpd_get_src, length);
        gasneti_sync_writes(); /* sync memcpy */
      }

      /* indicate completion */
      if (flags & GC_POST_COMPLETION_FLAG) {
        *(volatile int *) gpd->gpd_completion = 1;
        /* NOTE: if (flags & GC_POST_KEEP_GPD) then caller might free gpd now */
      } else if(flags & GC_POST_COMPLETION_CNTR) {
        gasneti_weakatomic_increment((gasneti_weakatomic_t *) gpd->gpd_completion, 0);
      } else if (flags & GC_POST_COMPLETION_SEND) {
        gasnetc_post_descriptor_t *next = (gasnetc_post_descriptor_t *) gpd->gpd_completion;
        if (gasneti_weakatomic_decrement_and_test(&next->u.counter, 0)) {
          int rc = gasnetc_send_am(next);
          gasneti_assert_always (rc == GASNET_OK);
        }
      } 

      /* release resources */
      if (flags & GC_POST_UNREGISTER) {
        gasnetc_deregister_gpd(gpd);
      } else if (flags & GC_POST_UNBOUNCE) {
        gasnetc_free_bounce_buffer(gpd);
      }
#if GASNETC_GNI_FIREHOSE
      else if (flags & GC_POST_FIREHOSE) {
        gasnetc_firehose_release(gpd);
      }
#endif

      if (!(flags & GC_POST_KEEP_GPD)) {
        gasnetc_free_post_descriptor(gpd);
      }
    }
  }
}

void gasnetc_poll(GASNETC_DIDX_FARG_ALONE)
{
#if GASNETC_USE_MULTI_DOMAIN
 #if 1
  /* There is NO use of GASNETC_ALL_DOMAINS in the current code */
  gasneti_assert(GASNETC_DIDX != GASNETC_ALL_DOMAINS);
 #else
  if_pf (GASNETC_DIDX == GASNETC_ALL_DOMAINS) {
    int d;
    gasnetc_poll_am_queue();
    for (d = 0; d < gasnetc_domain_count; d++) {
      gasnetc_poll_local_queue(d);
    }
  } else
 #endif
  {
    if ((GASNETC_DIDX == GASNETC_DEFAULT_DOMAIN) ||
        /* Every now and then poll for AMs even from non-default domains: */
        GASNETT_PREDICT_FALSE((DOMAIN_SPECIFIC_VAL(poll_idx)++ & gasnetc_poll_am_domain_mask) == 0)) {
       gasnetc_poll_am_queue();
    }
    gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
  }
#else
  gasnetc_poll_am_queue();
  gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
#endif
}

GASNETI_NEVER_INLINE(print_post_desc,
static void print_post_desc(const char *title, gni_post_descriptor_t *cmd)) {
  const int in_seg = gasneti_in_segment(gasneti_mynode, (void *) cmd->local_addr, cmd->length);
  printf("r %d %s-segment %s, desc addr %p\n", gasneti_mynode, (in_seg?"in":"non"), title, cmd);
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

static gni_return_t myPostRdma(gni_ep_handle_t ep, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  gni_post_descriptor_t * const pd = &gpd->pd;
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
      gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
  } while (++trial < max_trials);
  if (status == GNI_RC_ERROR_RESOURCE) {
    gasnetc_GNIT_Log("PostRdma retry failed");
  }
  return status;
}

static gni_return_t myPostFma(gni_ep_handle_t ep, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  gni_post_descriptor_t * const pd = &gpd->pd;
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
      gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
  } while (++trial < max_trials);
  if (status == GNI_RC_ERROR_RESOURCE) {
    gasnetc_GNIT_Log("PostFma retry failed");
  }
  return status;
}

#if GASNETC_GNI_FIREHOSE
/* Perform an fma/rdma Pet with out-of-segment source.
 * Returns length of the request issued to GNI, which may be less
 * than nbytes due to memory registration boundaries.
 * Legal only for out-of-segment source_addr.
 */
size_t gasnetc_rdma_put_fh(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  peer_struct_t * const peer = &peer_data[node];
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;

  gasneti_assert(!node_is_local(node));
  gasneti_boundscheck(node, dest_addr, nbytes);
  gasneti_assert(!gasneti_in_segment(gasneti_mynode, source_addr, nbytes));

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) dest_addr;
  pd->remote_mem_hndl = peer->mem_handle;
  pd->local_addr = (uint64_t) source_addr;
  pd->length = nbytes;

  if (nbytes <= gasnetc_put_fma_rdma_cutover) {
    /* Small enough for FMA - no local memory registration is required */
    pd->type = GNI_POST_FMA_PUT;
#if FIX_HT_ORDERING
    pd->cq_mode = gasnetc_fma_put_cq_mode;
#endif
    pd->local_mem_hndl = my_mem_handle;
    status = myPostFma(peer->ep_handle, gpd);
  } else {
    /* TODO: look at ibv-conduit logic which works to align registrations for better reuse */
    pd->type = GNI_POST_RDMA_PUT;
    nbytes = gasnetc_firehose_acquire(gpd);
    if_pt (nbytes) {
      gpd->flags |= GC_POST_FIREHOSE;
    } else {
      /* Memory could not be registered - fall back to bounce buffers */
      pd->local_addr = (uint64_t) gasnetc_alloc_bounce_buffer(GASNETC_DIDX_PASS_ALONE);
      pd->local_mem_hndl = my_mem_handle;
      gpd->flags |= GC_POST_UNBOUNCE;
      nbytes = MIN(pd->length, gasnetc_put_bounce_register_cutover);
      memcpy((void *) pd->local_addr, source_addr, nbytes);
    }
    pd->length = nbytes;
    status = myPostRdma(peer->ep_handle, gpd);
  }

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Put", pd);
    gasnetc_GNIT_Abort("Put failed with %s", gasnetc_gni_rc_string(status));
  }

  return nbytes;
}
#endif

/* Perform an rdma/fma Put with no concern for local completion.
 * Returns length of the request issued to GNI, which may be less
 * than nbytes (for instance due to a failed call to MemRegister).
 */
size_t gasnetc_rdma_put_bulk(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
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
    status = myPostFma(peer->ep_handle, gpd);
  } else { /* Using RDMA, which requires local memory registration */
    if_pf (!gasneti_in_segment(gasneti_mynode, source_addr, nbytes)) {
      /* Use a bounce buffer or mem-reg according to size.
       * Use of gpd->u.immedate would only be reachable if
       *     (put_fma_rdma_cutover < IMMEDIATE_BOUNCE_SIZE),
       * which is not the default (nor recommended).
       */
      gasneti_assert(!gasnetc_use_firehose);
      if ((nbytes <= gasnetc_put_bounce_register_cutover) ||
          /* Also use bounce buffer (setting nbytes to max size) if MemRegister fails: */
          (!gasnetc_register_gpd(gpd, GNI_MEM_READ_ONLY) &&
           ((pd->length = nbytes = gasnetc_put_bounce_register_cutover),
            (pd->local_mem_hndl = my_mem_handle),1))) {
        void * const buffer = gasnetc_alloc_bounce_buffer(GASNETC_DIDX_PASS_ALONE);
        pd->local_addr = (uint64_t) memcpy(buffer, source_addr, nbytes);
        gpd->flags |= GC_POST_UNBOUNCE;
      } else {
        gpd->flags |= GC_POST_UNREGISTER;
      }
    }
    pd->type = GNI_POST_RDMA_PUT;
    status = myPostRdma(peer->ep_handle, gpd);
  }

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Put", pd);
    gasnetc_GNIT_Abort("Put failed with %s", gasnetc_gni_rc_string(status));
  }

  return nbytes;
}

/* Perform an rdma/fma Put for which the caller requires local completion
 * NOTE: be sure to update gasnetc_max_put_lc if the logic here changes
 */
void
gasnetc_rdma_put_lc(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  peer_struct_t * const peer = &peer_data[node];
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;

  gasneti_assert(!node_is_local(node));
  gasneti_assert(nbytes <= gasnetc_max_put_lc);

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
    status = myPostFma(peer->ep_handle, gpd);
  } else
#endif
  { /* Use bounce buffers */
  #if !GASNET_CONDUIT_GEMINI /* On Gemini the FMA path above would be selected instead */
    if (nbytes <= GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE) {
      void * const buffer = gpd->u.immediate;
      pd->local_addr = (uint64_t) memcpy(buffer, source_addr, nbytes);
    } else
  #endif
    {
      void * const buffer = gasnetc_alloc_bounce_buffer(GASNETC_DIDX_PASS_ALONE);
      pd->local_addr = (uint64_t) memcpy(buffer, source_addr, nbytes);
      gpd->flags |= GC_POST_UNBOUNCE;
      gasneti_assert(nbytes <= gasnetc_put_bounce_register_cutover);
    }
 
#if !GASNET_CONDUIT_GEMINI
    if (nbytes <= gasnetc_put_fma_rdma_cutover) {
      pd->type = GNI_POST_FMA_PUT;
    #if FIX_HT_ORDERING
      pd->cq_mode = gasnetc_fma_put_cq_mode;
    #endif
      status = myPostFma(peer->ep_handle, gpd);
    } else
#endif
    {
      pd->type = GNI_POST_RDMA_PUT;
      status = myPostRdma(peer->ep_handle, gpd);
    }
  }

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Put", pd);
    gasnetc_GNIT_Abort("Put failed with %s", gasnetc_gni_rc_string(status));
  }
}

/* FMA Put from a specified buffer */
void gasnetc_rdma_put_buff(gasnet_node_t node,
		void *dest_addr, void *source_addr,
		size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
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
  status = myPostFma(peer->ep_handle, gpd);

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
      status = myPostFma(ep, gpd);
  } else {
      pd->type = GNI_POST_RDMA_GET;
      status = myPostRdma(ep, gpd);
  }

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Get", pd);
    gasnetc_GNIT_Abort("Get failed with %s", gasnetc_gni_rc_string(status));
  }
}

#if GASNETC_GNI_FIREHOSE
/* Perform an fma/rdma Get with out-of-segment destination.
 * Returns length of the request issued to GNI, which may be less
 * than nbytes due to memory registration boundaries.
 * Legal only for out-of-segment dest_addr.
 */
size_t gasnetc_rdma_get_fh(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  peer_struct_t * const peer = &peer_data[node];
  gni_post_descriptor_t * const pd = &gpd->pd;

  gasneti_assert(!node_is_local(node));
  gasneti_boundscheck(node, source_addr, nbytes);
  gasneti_assert(!gasneti_in_segment(gasneti_mynode, dest_addr, nbytes));

  /*  bzero(&pd, sizeof(gni_post_descriptor_t)); */
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) source_addr;
  pd->remote_mem_hndl = peer->mem_handle;
  pd->length = nbytes;

#if GASNET_PAR
  /* Exactly when is IMMEDIATE_BOUNCE cheaper than a firehose lookup?
     Results from testsmall-seq runs show the FH hit is cheaper.  However,
     testcontend-par results slightly favor IMMEDIATE_BOUNCE (due to lock
     contention in FH).  The testcontend-par results with multi-domain are even
     more favorable (since FH becomes the only serialization).
     TODO: revisit if/when anything changes that could impact these timings.
  */
  if (nbytes < GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE) {
    pd->local_mem_hndl = my_mem_handle;
    gpd->flags |= GC_POST_COPY_IMM;
    pd->local_addr = (uint64_t) gpd->u.immediate;
    gpd->gpd_get_dst = (uint64_t) dest_addr;
  } else
#endif
  {
    pd->local_addr = (uint64_t) dest_addr;
    nbytes = gasnetc_firehose_acquire(gpd);
    if_pt (nbytes) {
      gpd->flags |= GC_POST_FIREHOSE;
    } else {
      /* Memory could not be registered - fall back to bounce buffers */
      pd->local_addr = (uint64_t) gasnetc_alloc_bounce_buffer(GASNETC_DIDX_PASS_ALONE);
      pd->local_mem_hndl = my_mem_handle;
      gpd->flags |= GC_POST_UNBOUNCE | GC_POST_COPY;
      gpd->gpd_get_src = pd->local_addr;
      gpd->gpd_get_dst = (uint64_t) dest_addr;
      nbytes = MIN(pd->length, gasnetc_get_bounce_register_cutover);
    }
    pd->length = nbytes;
  }

  gasnetc_post_get(peer->ep_handle, gpd);

  return nbytes;
}
#endif

/* Perform an rdma/fma Get.
 * Returns length of the request issued to GNI, which may be less
 * than nbytes (for instance due to a failed call to MemRegister).
 */
size_t gasnetc_rdma_get(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  peer_struct_t * const peer = &peer_data[node];
  gni_post_descriptor_t * const pd = &gpd->pd;

  gasneti_assert(!node_is_local(node));

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
    gasneti_assert(!gasnetc_use_firehose);
    if (nbytes <= GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE) {
      gpd->flags |= GC_POST_COPY_IMM;
      pd->local_addr = (uint64_t) gpd->u.immediate;
      gpd->gpd_get_dst = (uint64_t) dest_addr;
    } else if ((nbytes <= gasnetc_get_bounce_register_cutover) ||
               /* Also use bounce buffer (setting nbytes to max size) if MemRegister fails: */
               (!gasnetc_register_gpd(gpd, GNI_MEM_READWRITE) &&
                ((pd->length = nbytes = gasnetc_get_bounce_register_cutover),
                 (pd->local_mem_hndl = my_mem_handle),1))) {
      gpd->flags |= GC_POST_UNBOUNCE | GC_POST_COPY;
      gpd->gpd_get_src = pd->local_addr = (uint64_t) gasnetc_alloc_bounce_buffer(GASNETC_DIDX_PASS_ALONE);
      gpd->gpd_get_dst = (uint64_t) dest_addr;
    } else {
      gpd->flags |= GC_POST_UNREGISTER;
    }
  }

  gasnetc_post_get(peer->ep_handle, gpd);

  return nbytes;
}

/* for get in which one or more of dest_addr, source_addr or nbytes is NOT divisible by 4
 * NOTE: be sure to update gasnetc_max_get_unaligned if the logic here changes
 */
void gasnetc_rdma_get_unaligned(gasnet_node_t node,
		 void *dest_addr, void *source_addr,
		 size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  peer_struct_t * const peer = &peer_data[node];
  gni_post_descriptor_t * const pd = &gpd->pd;
  uint8_t * buffer;

  /* Compute length of "overfetch" required, if any */
  unsigned int pre = (uintptr_t) source_addr & 3;
  size_t       length = GASNETI_ALIGNUP(nbytes + pre, 4);
  unsigned int overfetch = length - nbytes;

  gasneti_assert(!node_is_local(node));
  gasneti_assert(length <= gasnetc_max_get_unaligned);

  gasneti_assert(0 == (overfetch & ~GC_POST_COPY_TRIM));
  gpd->flags |= GC_POST_COPY | overfetch;

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
  if (length <= GASNETC_GNI_IMMEDIATE_BOUNCE_SIZE) {
    buffer = gpd->u.immediate;
  } else {
    gasneti_assert(length <= gasnetc_get_bounce_register_cutover);
    gpd->flags |= GC_POST_UNBOUNCE;
    buffer = gasnetc_alloc_bounce_buffer(GASNETC_DIDX_PASS_ALONE);
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
int gasnetc_rdma_get_buff(gasnet_node_t node,
		void *dest_addr, void *source_addr,
		size_t nbytes, gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
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
  status = myPostFma(peer->ep_handle, gpd);

  if_pf (status != GNI_RC_SUCCESS) {
    print_post_desc("Get", pd);
    gasnetc_GNIT_Abort("Get failed with %s", gasnetc_gni_rc_string(status));
  }

  return pre;
}

#if GASNETC_GNI_FETCHOP
/* Perform an 8-byte fetch-and-op */
void gasnetc_fetchop_u64(
                gasnet_node_t node, void *source_addr,
                gni_fma_cmd_type_t cmd, uint64_t operand,
                gasnetc_post_descriptor_t *gpd)
{
  GASNETC_DIDX_POST(gpd->domain_idx);
  DOMAIN_SPECIFIC_VAR(peer_struct_t * const, peer_data);
  peer_struct_t * const peer = &peer_data[node];
  gni_post_descriptor_t * const pd = &gpd->pd;
  gni_return_t status;

  gasneti_boundscheck(node, source_addr, 8);

  pd->type = GNI_POST_AMO;
  pd->amo_cmd = cmd;
  pd->first_operand = operand;
  pd->cq_mode = GNI_CQMODE_GLOBAL_EVENT;
  pd->dlvr_mode = GNI_DLVMODE_PERFORMANCE;
  pd->remote_addr = (uint64_t) source_addr;
  pd->remote_mem_hndl = peer->mem_handle;
  pd->local_addr = (uint64_t) gpd->u.immediate;
  pd->local_mem_hndl = my_mem_handle;
  pd->length = 8;

  status = myPostFma(peer->ep_handle, gpd);
  if_pf (status != GNI_RC_SUCCESS) {
    gasnetc_GNIT_Abort("GNI_POST_AMO failed with %s", gasnetc_gni_rc_string(status));
  }
}
#endif


/* Needs no lock because it is called only from the init code */
void gasnetc_init_post_descriptor_pool(GASNETC_DIDX_FARG_ALONE) 
{
  int i;
  const int count = gasnetc_pd_buffers.size / GASNETC_SIZEOF_GDP /  gasnetc_domain_count;
  uintptr_t addr;

  gasneti_assert_always(gasnetc_pd_buffers.addr != NULL);

#if GASNETC_USE_MULTI_DOMAIN
  /* must first destroy the temporary pool of post descriptors (only first domain) */
  if_pf (GASNETC_DIDX == GASNETC_DEFAULT_DOMAIN) {
    for (i=0; i < gasnetc_log2_remote; ++i) {
      gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor(GASNETC_DIDX_PASS_ALONE);
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
    gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor(GASNETC_DIDX_PASS_ALONE);
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
gasnetc_post_descriptor_t *gasnetc_alloc_post_descriptor(GASNETC_DIDX_FARG_ALONE)
{
  gasneti_lifo_head_t * const pool_p = &DOMAIN_SPECIFIC_VAL(post_descriptor_pool);
  gasnetc_post_descriptor_t *gpd = (gasnetc_post_descriptor_t *) gasneti_lifo_pop(pool_p);
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
      gasnetc_poll_local_queue(GASNETC_DIDX_PASS_ALONE);
      gpd = (gasnetc_post_descriptor_t *) gasneti_lifo_pop(pool_p);
    } while (!gpd);
    if_pf (stall) GASNETC_TRACE_WAIT_END(ALLOC_PD_STALL);
  }
  gpd->flags = 0;
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
void gasnetc_handle_sys_shutdown_packet(uint32_t source, uint16_t arg)
{
  uint32_t distance = 1 << (arg >> 8);
  uint8_t exitcode = arg & 0xff;
  gasneti_weakatomic_val_t readval;

  GASNETI_TRACE_PRINTF(C,("Got SHUTDOWN Request from node %d w/ exitcode %d",(int)source,exitcode));

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
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);
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
      GASNETI_WAITHOOK();
      gasnetc_poll(GASNETC_DIDX_PASS_ALONE);
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
        gasnetc_poll(GASNETC_DIDX_PASS_ALONE);
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
    gasnet_node_t peeridx = (distance >= size - rank) ? rank - (size - distance)
                                                      : rank + distance;
  #if GASNET_PSHM
    gasnet_node_t dest = gasneti_pshm_firsts[peeridx];
  #else
    gasnet_node_t dest = peeridx;
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
      gasnetc_poll(GASNETC_DIDX_PASS_ALONE);
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
  GASNETC_DIDX_POST(GASNETC_DEFAULT_DOMAIN);

  /* Drain completions for sent exitcode-reduction messages */
  if (gasneti_weakatomic_read(&sys_exit_sent_fini, 0) != sys_exit_sent_init) {
    gasnetc_poll(GASNETC_DIDX_PASS_ALONE);
    while (gasneti_weakatomic_read(&sys_exit_sent_fini, 0) != sys_exit_sent_init) {
      GASNETI_WAITHOOK();
      gasnetc_poll(GASNETC_DIDX_PASS_ALONE);
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
        gasnetc_poll(GASNETC_DIDX_PASS_ALONE);
      }
    }
  }
#endif
}

/* AuxSeg setup for registered bounce buffer space*/
#if GASNETC_USE_MULTI_DOMAIN
  GASNETI_IDENT(gasneti_bounce_auxseg_IdentString,
                "$GASNetAuxSeg_bounce: "
                "(GASNET_DOMAIN_COUNT:" _STRINGIFY(GASNETC_DOMAIN_COUNT_DEFAULT) ") *" 
                "(GASNET_GNI_BOUNCE_SIZE:" _STRINGIFY(GASNETC_GNI_BOUNCE_SIZE_DEFAULT)") $");
#else
  GASNETI_IDENT(gasneti_bounce_auxseg_IdentString,
                "$GASNetAuxSeg_bounce: "
                "GASNET_GNI_BOUNCE_SIZE:" _STRINGIFY(GASNETC_GNI_BOUNCE_SIZE_DEFAULT)" $");
#endif
gasneti_auxseg_request_t gasnetc_bounce_auxseg_alloc(gasnet_seginfo_t *auxseg_info) {
  gasneti_auxseg_request_t retval;

  retval.minsz =
  retval.optimalsz = gasnetc_domain_count *
                     gasneti_getenv_int_withdefault("GASNET_GNI_BOUNCE_SIZE",
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
 */
#if GASNETC_USE_MULTI_DOMAIN
  GASNETI_IDENT(gasneti_pd_auxseg_IdentString,
              "$GASNetAuxSeg_pd: " _STRINGIFY(GASNETC_SIZEOF_GDP) "*"
              "(GASNET_DOMAIN_COUNT:" _STRINGIFY(GASNETC_DOMAIN_COUNT_DEFAULT) ") *" 
              "(GASNET_GNI_NUM_PD:" _STRINGIFY(GASNETC_GNI_NUM_PD_DEFAULT) ") $");
#else
  GASNETI_IDENT(gasneti_pd_auxseg_IdentString,
              "$GASNetAuxSeg_pd: " _STRINGIFY(GASNETC_SIZEOF_GDP) "*"
              "(GASNET_GNI_NUM_PD:" _STRINGIFY(GASNETC_GNI_NUM_PD_DEFAULT) ") $");
#endif
gasneti_auxseg_request_t gasnetc_pd_auxseg_alloc(gasnet_seginfo_t *auxseg_info) {
  gasneti_auxseg_request_t retval;
  
  retval.minsz =
  retval.optimalsz = gasnetc_domain_count * num_pd * GASNETC_SIZEOF_GDP;
  gasneti_assert_always(GASNETC_SIZEOF_GDP >= sizeof(gasnetc_post_descriptor_t));

  if (auxseg_info != NULL) { /* auxseg granted */
    /* The only one we care about is our own node */
    gasnetc_pd_buffers = auxseg_info[gasneti_mynode];
  }

  return retval;
}

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

  num_bounce = gasnetc_bounce_buffers.size / buffer_size / gasnetc_domain_count;

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


/* ============================================================== */
#if GASNETC_GNI_FIREHOSE
/* Implement client-specific callbacks for use by firehose-region */

#if GASNETI_STATS_OR_TRACE
  #define GASNETC_TRACE_MR(_event, _verb, _region) do {                  \
	const firehose_region_t *_reg = (_region);                       \
	int _pages = (int)(_reg->len/GASNET_PAGESIZE);                   \
	GASNETI_TRACE_PRINTF(D, ("FIREHOSE_MOVE: " _STRINGIFY(_verb)     \
				 " %d page(s) at " GASNETI_LADDRFMT,     \
				 _pages, GASNETI_LADDRSTR(_reg->addr))); \
	GASNETC_STAT_EVENT_VAL(_event, _pages);                          \
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
#if GASNETC_USE_MULTI_DOMAIN
  GASNETC_DIDX_POST((gasnete_mythread())->domain_idx);
  DOMAIN_SPECIFIC_VAR(gni_nic_handle_t, nic_handle);
#endif
  GASNETC_TRACE_WAIT_BEGIN();
  gni_return_t status;
  int i;

  GASNETC_LOCK_GNI();
  {
    /* Take care of any unpins first */
    for (i = 0; i < unpin_num; i++) {
      const firehose_region_t *region = unpin_list + i;
      if_pt (gasneti_valid_client_t(&region->client)) {
        gni_mem_handle_t handle = region->client; /* Copy due to const */
        status = GNI_MemDeregister(nic_handle, &handle);
        gasneti_assert_always(status == GNI_RC_SUCCESS);
      }
      GASNETC_TRACE_UNPIN(&unpin_list[i]);
    }

    /* Take care of any pins */
    for (i = 0; i < pin_num; i++) {
      firehose_region_t *region = pin_list + i;
      status = GNI_MemRegister(nic_handle, region->addr, region->len, NULL,
                               gasnetc_memreg_flags|GNI_MEM_READWRITE, -1, &region->client);
      if_pf (GNI_RC_SUCCESS != status) {
        if (GNI_RC_INVALID_PARAM == status || GNI_RC_PERMISSION_ERROR == status) {
          /* Memory could not be registered (e.g. memory imported by XPMEM or r/o data). */
          gasneti_invalidate_client_t(&region->client);
        } else {
          /* Any other failure mode is fatal */
          gasnetc_GNIT_Abort("MemRegister failed at %p + %p with %s",
                             (void*)region->addr, (void*)region->len,
                             gasnetc_gni_rc_string(status));
        }
      }
      GASNETC_TRACE_PIN(&pin_list[i]);
    }
  }
  GASNETC_UNLOCK_GNI();

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
#endif
