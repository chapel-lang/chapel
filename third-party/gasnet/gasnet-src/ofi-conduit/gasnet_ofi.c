/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ofi-conduit/gasnet_ofi.c $
 * Description: GASNet libfabric (OFI) conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2015-2017, Intel Corporation
 * Portions copyright 2018-2020, The Regents of the University of California.
 * Terms of use are as specified in license.txt
 */
#define GASNETI_NEED_GASNET_MK_H 1
#include <gasnet_core_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_hwloc_internal.h>
#include <gasnet_ofi.h>

#include <rdma/fabric.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_errno.h>

#if HAVE_SYS_UIO_H
#include <sys/uio.h> /* For struct iovec */
#endif

GASNETI_IDENT(gasnetc_IdentString_Providers,
              "$GASNetOfiProvidersSupported: " GASNETC_OFI_PROVIDER_LIST " $");
GASNETI_IDENT(gasnetc_IdentString_OfiUseThreadDomain,
              "$GASNetOfiUseThreadDomain: "_STRINGIFY(GASNETC_OFI_USE_THREAD_DOMAIN)" $");
GASNETI_IDENT(gasnetc_IdentString_OfiUseMultiCQ,
              "$GASNetOfiUseMultiCQ: "_STRINGIFY(GASNETC_OFI_USE_MULTI_CQ)" $");
GASNETI_IDENT(gasnetc_IdentString_OfiRetryRecvmsg,
              "$GASNetOfiRetryRecvmsg: "_STRINGIFY(GASNETC_OFI_RETRY_RECVMSG)" $");

struct fid_fabric*    gasnetc_ofi_fabricfd;
struct fid_domain*    gasnetc_ofi_domainfd;
struct fid_cq*        gasnetc_ofi_tx_cqfd;  // CQ, ideally for both AM and RDMA tx ops
#if GASNETC_OFI_USE_MULTI_CQ
struct fid_cq*        gasnetc_ofi_reqtx_cqfd = NULL; // CQ for AM Request tx ops, IFF cannot share
struct fid_cq*        gasnetc_ofi_reptx_cqfd = NULL; // CQ for AM Request tx ops, IFF cannot share
#endif
struct fid_ep*        gasnetc_ofi_rdma_epfd;
struct fid_ep*        gasnetc_ofi_request_epfd;
struct fid_ep*        gasnetc_ofi_reply_epfd;
struct fid_cq*        gasnetc_ofi_request_cqfd;
struct fid_cq*        gasnetc_ofi_reply_cqfd;
#if GASNET_SEGMENT_EVERYTHING
struct fid_mr*        gasnetc_segment_mrfd = NULL;
#endif
struct fid_mr*        gasnetc_auxseg_mrfd = NULL;
size_t gasnetc_ofi_bbuf_threshold;

#ifdef FI_MR_ENDPOINT
static int gasnetc_fi_mr_endpoint = 0;
#endif

#if GASNET_HAVE_MK_CLASS_MULTIPLE
static int gasnetc_fi_hmem = 0;
#endif

#define GASNETC_PROHIBIT_MODE_BIT(info,bit,scope) do { \
    if (info->mode & bit) {                            \
      gasneti_fatalerror("Provider '%s' has set unsupported mode bit " #bit " for %s", \
                         gasnetc_ofi_provider, scope); \
    }                                                  \
  } while (0)

size_t gasnetc_ofi_max_medium = GASNETC_OFI_MAX_MEDIUM_DFLT;
size_t gasnetc_ofi_max_long;

// Maximum size to use for RMA
size_t gasnetc_max_rma_size;

typedef struct gasnetc_ofi_recv_metadata {
    struct iovec iov;
    struct fi_msg am_buff_msg;
    gasnetc_ofi_recv_ctxt_t am_buff_ctxt;
} gasnetc_ofi_recv_metadata_t;

static short use_av_map = 0;
#ifdef GASNETC_OFI_USE_AV_MAP_CONFIGURE
  #define GASNETC_OFI_USE_AV_MAP_STATIC 1
  #define GASNETC_OFI_USE_AV_MAP (GASNETC_OFI_USE_AV_MAP_CONFIGURE[0] == '1')
  GASNETI_IDENT(gasnetc_IdentString_OfiUseAVMAP,
                "$GASNetOfiUseAVMAP: " GASNETC_OFI_USE_AV_MAP_CONFIGURE " $");
#else
  // cast prevents erroneous use in preprocessor directives
  #define GASNETC_OFI_USE_AV_MAP ((short)use_av_map)
  GASNETI_IDENT(gasnetc_IdentString_OfiUseAVMAP,
                "$GASNetOfiUseAVMAP: dynamic $");
#endif

// Must match order of fi_getname() calls in ofi_exchange_addresses(),
// where this is enforced via static assertions.
enum {
    GASNETC_FADDR_IDX_REQ = 0,
    GASNETC_FADDR_IDX_REP,
    GASNETC_FADDR_IDX_RDMA,
    NUM_OFI_ENDPOINTS
};

// One address vector per endpoint
static struct fid_av* gasnetc_ofi_avfd[NUM_OFI_ENDPOINTS];

// Entries non-NULL only for FI_AV_MAP
static fi_addr_t *gasnetc_addr_map[NUM_OFI_ENDPOINTS] = { NULL, };

// TODO: multi-ep with independent resources will require rewriting this
GASNETI_INLINE(gasnetc_fabric_addr_inner) GASNETI_PURE
fi_addr_t gasnetc_fabric_addr_inner(int idx, gex_Rank_t jobrank) {
  if (GASNETC_OFI_USE_AV_MAP) {
    return gasnetc_addr_map[idx][jobrank];
  } else {
    return (fi_addr_t)jobrank;
  }
}
GASNETI_PUREP(gasnetc_fabric_addr_inner)
#define gasnetc_fabric_addr(type, jobrank) \
        gasnetc_fabric_addr_inner(GASNETC_FADDR_IDX_##type, jobrank)


static short has_mr_virt_addr = 0;
#ifdef GASNETC_OFI_HAS_MR_VIRT_ADDR_CONFIGURE
  #define GASNETC_OFI_HAS_MR_VIRT_ADDR_STATIC 1
  #define GASNETC_OFI_HAS_MR_VIRT_ADDR (GASNETC_OFI_HAS_MR_VIRT_ADDR_CONFIGURE[0] == '1')
  GASNETI_IDENT(gasnetc_IdentString_OfiMRVirtAddr,
                "$GASNetOfiMRVirtAddr: " GASNETC_OFI_HAS_MR_VIRT_ADDR_CONFIGURE " $");
#else
  // cast prevents erroneous use in preprocessor directives
  #define GASNETC_OFI_HAS_MR_VIRT_ADDR ((short)has_mr_virt_addr)
  GASNETI_IDENT(gasnetc_IdentString_OfiMRVirtAddr,
                "$GASNetOfiMRVirtAddr: dynamic $");
#endif

static short has_mr_prov_key = 0;
#ifdef GASNETC_OFI_HAS_MR_PROV_KEY_CONFIGURE
  #define GASNETC_OFI_HAS_MR_PROV_KEY_STATIC 1
  #define GASNETC_OFI_HAS_MR_PROV_KEY (GASNETC_OFI_HAS_MR_PROV_KEY_CONFIGURE[0] == '1')
  GASNETI_IDENT(gasnetc_IdentString_OfiMRProvKey,
                "$GASNetOfiMRProvKey: " GASNETC_OFI_HAS_MR_PROV_KEY_CONFIGURE " $");
#else
  // cast prevents erroneous use in preprocessor directives
  #define GASNETC_OFI_HAS_MR_PROV_KEY ((short)has_mr_prov_key)
  GASNETI_IDENT(gasnetc_IdentString_OfiMRProvKey,
                "$GASNetOfiMRProvKey: dynamic $");
#endif

// Table of remote registration keys, used only when GASNETC_OFI_HAS_MR_PROV_KEY,
// and otherwise NULL.
// The leading dimension is EP index, with indices in [-1, GASNET_MAXEPS) to
// place the aux segment keys as index -1, and the second dimension is jobrank.
// The leading dimension is allocated at startup, and the rest is allocated
// lazily when the first entry for a given EP index is received.
// TODO: scalable storage
static uint64_t** gasnetc_remote_key_tbl;

static size_t tx_cq_size = 0;
static size_t rx_cq_size = 0;

// Determine if (jobrank,addr) is in the aux segment registration
GASNETI_INLINE(gasnetc_in_auxseg) GASNETI_PURE
int gasnetc_in_auxseg(gex_Rank_t jobrank, void *addr)
{
#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    // Reminder that with unsigned types, a negative offset is a very large positive value
    uintptr_t offset = (uintptr_t)addr - (uintptr_t)gasneti_seginfo_aux[jobrank].addr;
    return offset < gasneti_seginfo_aux[jobrank].size;
#else
    // For SEGMENT_EVERYTHING there is only ever a single memory registration.
    // So the addrres is NEVER "in the aux segment registration".
    return 0;
#endif
}
GASNETI_PUREP(gasnetc_in_auxseg)

// Lookup or compute correct key for RDMA
// NOTE: rem_epidx has type int (not gex_EP_Index_t) to allow -1 to name the aux seg
GASNETI_INLINE(gasnetc_remote_key) GASNETI_PURE
uint64_t gasnetc_remote_key(gex_Rank_t jobrank, int rem_epidx)
{
#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    gasneti_assert_int(rem_epidx ,>=, -1);
    gasneti_assert_int(rem_epidx ,<, GASNET_MAXEPS);
    if (GASNETC_OFI_HAS_MR_PROV_KEY) {
        gasneti_assert(gasnetc_remote_key_tbl[rem_epidx]);
        return gasnetc_remote_key_tbl[rem_epidx][jobrank];
    } else {
        return GASNETC_EPIDX_TO_KEY(rem_epidx);
    }
#else
    // For SEGMENT_EVERYTHING there is a single host memory registration
    gasneti_assert_int(rem_epidx ,>=, 0);
    gasneti_assert_int(rem_epidx ,<, GASNET_MAXEPS);
    return GASNETC_EPIDX_TO_KEY(0);
#endif
}
GASNETI_PUREP(gasnetc_remote_key)

// Lookup correct "address" (which may be an offset) for RDMA
// NOTE: rem_epidx has type int (not gex_EP_Index_t) to allow -1 to name the aux seg
GASNETI_INLINE(gasnetc_remote_addr) GASNETI_PURE
uintptr_t gasnetc_remote_addr(gex_Rank_t jobrank, void *addr, int rem_epidx)
{
#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    gasneti_assert_int(rem_epidx ,>=, -1);
    gasneti_assert_int(rem_epidx ,<, GASNET_MAXEPS);
    if (GASNETC_OFI_HAS_MR_VIRT_ADDR) {
        return (uintptr_t)addr;
    } else {
        // uses an offset rather than virtual address
        int in_auxseg = (rem_epidx < 0);
        gasnet_seginfo_t *si = in_auxseg ? gasneti_seginfo_aux
                                         : gasneti_seginfo_tbl[rem_epidx];
        gasneti_assert(si);
        return (uintptr_t)addr - (uintptr_t)si[jobrank].addr;
    }
#else
    gasneti_assert_int(rem_epidx ,>=, 0);
    gasneti_assert_int(rem_epidx ,<, GASNET_MAXEPS);
    // For EVERYTHING the base address (if any) is zero
    return (uintptr_t)addr;
#endif
}
GASNETI_PUREP(gasnetc_remote_addr)

// Statements which launch a fi_write or fi_read, setting "ret"
#define OFI_RMA(rw, c_ep, loc_addr, nbytes, jobrank, rem_epidx, rem_addr, ctxt_ptr, alc) \
    do { \
        fi_addr_t _peer = gasnetc_fabric_addr(RDMA, jobrank); \
        uintptr_t _addr = gasnetc_remote_addr(jobrank, rem_addr, rem_epidx); \
        uint64_t _key   = gasnetc_remote_key(jobrank, rem_epidx); \
        void *_op_ctxt  = gasnetc_rdma_ctxt_to_op_ctxt(ctxt_ptr, alc|GASNETC_CTXT_IS_RMA); \
        void *_desc     = gasneti_i_segment_kind_is_host(c_ep->_segment) ? NULL: fi_mr_desc(c_ep->mrfd); \
        struct fid_ep *_ofi_ep = gasnetc_ofi_rdma_epfd; /* TODO: ep isolation */ \
        ret = fi_##rw(_ofi_ep, loc_addr, nbytes, _desc, _peer, _addr, _key, _op_ctxt); \
    } while(0)
#define OFI_WRITE(c_ep, src_addr, nbytes, jobrank, rem_epidx, dest_addr, ctxt_ptr, alc) \
        OFI_RMA(write, c_ep, src_addr, nbytes, jobrank, rem_epidx, dest_addr, ctxt_ptr, alc)
#define OFI_READ(c_ep, dest_addr, nbytes, jobrank, rem_epidx, src_addr, ctxt_ptr, alc) \
        OFI_RMA(read, c_ep, dest_addr, nbytes, jobrank, rem_epidx, src_addr, ctxt_ptr, alc)

/* Poll periodically on RMA injection to ensure efficient progress.
 * This is a data race, but it is safe as polling here is unnecessary, it
 * simply improves performance in case of floods of RMA requests*/
static int rdma_poll_frequency = 0;
static int rdma_periodic_poll_threshold; /* Set via environment variable in init() */
#define PERIODIC_RMA_POLL() do{\
    if_pf(rdma_poll_frequency++ >= rdma_periodic_poll_threshold){\
        rdma_poll_frequency=0;\
        gasnetc_ofi_tx_poll();\
    }} while(0)

/* In this case, inject means "inject into the network". It is not specific
 * to the fi_inject/fi_inject_write functions. fxn must assign a return value
 * to an int named "ret" in the scope in which it is called.*/
#define OFI_INJECT_RETRY(lock, fxn, poll_type)\
    do {\
        GASNETC_OFI_LOCK_EXPR(lock, fxn);\
        GASNETI_SPIN_WHILE(ret == -FI_EAGAIN, {\
          GASNETC_OFI_POLL_SELECTIVE(poll_type);\
          GASNETC_OFI_LOCK_EXPR(lock, fxn);\
        });\
    }while(0)
// As above, except that if 'imm' is non-zero, then will jump to 'label'
// if first attempt returns -FI_EAGAIN
// TODO: support poll if GASNETC_IMMEDIATE_AMPOLLS?
#define OFI_INJECT_RETRY_IMM(lock, fxn, poll_type, imm, label)\
    do {\
        GASNETC_OFI_LOCK_EXPR(lock, fxn);\
        if (ret == -FI_EAGAIN) {\
          if (imm) goto label; \
          GASNETI_SPIN_DOWHILE(ret == -FI_EAGAIN, {\
            GASNETC_OFI_POLL_SELECTIVE(poll_type);\
            GASNETC_OFI_LOCK_EXPR(lock, fxn);\
          });\
        } \
    }while(0)

static gasneti_lifo_head_t ofi_am_request_pool = GASNETI_LIFO_INITIALIZER;
static gasneti_lifo_head_t ofi_am_reply_pool = GASNETI_LIFO_INITIALIZER;
static gasneti_lifo_head_t ofi_bbuf_pool = GASNETI_LIFO_INITIALIZER;
static gasneti_lifo_head_t ofi_bbuf_ctxt_pool = GASNETI_LIFO_INITIALIZER;

static size_t num_multirecv_buffs;
static size_t multirecv_buff_size;
static void* receive_region_start = NULL;
static size_t receive_region_size = 0;

/* Variables for bounce buffering of non-blocking, non-bulk puts.
 * The gasnetc_ofi_bbuf_threshold variable is defined in gasnet_ofi.h
 * as it is needed in other files */
static void* bounce_region_start = NULL;
static size_t bounce_region_size = 0;
static size_t ofi_num_bbufs; 
static size_t ofi_bbuf_size;
#define OFI_MAX_NUM_BOUNCE_BUFFERS 32

static void* am_buffers_region_start = NULL;
static size_t am_buffers_region_size = 0;
static gasneti_semaphore_t num_unallocated_request_buffers;
static gasneti_semaphore_t num_unallocated_reply_buffers;
static size_t max_am_request_buffs = 0;
static size_t max_am_reply_buffs = 0;
static size_t num_init_am_request_buffs = 0;
static size_t num_init_am_reply_buffs = 0;
static size_t long_rma_threshold = 0;

static uint64_t                 max_buffered_send;
static uint64_t                 max_buffered_write;
static uint64_t                 min_multi_recv;
static unsigned long long       maybe_multi_recv = FI_MULTI_RECV;

static int using_psm_provider = 0;

static char *gasnetc_ofi_device = NULL;
static const char *supported_providers = GASNETC_OFI_PROVIDER_LIST;

static int gasnetc_high_perf_prov = 0;
static char *gasnetc_ofi_provider = NULL;
static char *gasnetc_ofi_domain = NULL;

static struct fi_info *gasnetc_rma_info;
static struct fi_info *gasnetc_msg_info;

gasneti_spawnerfn_t const *gasneti_spawner = NULL;

static gasnetc_ofi_recv_metadata_t* metadata_array;

/* Being able to see if there are pending operations that are not
 * completing is useful for debugging purposes */
#if GASNET_DEBUG
static gasnetc_paratomic_t pending_rdma = gasnetc_paratomic_init(0);
static gasnetc_paratomic_t pending_am = gasnetc_paratomic_init(0);
#endif

static int gasnetc_ofi_inited = 0;

// OFI_CONDUIT_VERSION: API version to request in fi_getinfo()
// NOTE: we do NOT blindly chase the latest to avoid nasty surprises.
//
// FI_MR_HMEM and corresponding fields in `struct fi_mr_attr` first
// appear in API version 1.9.  So, memory kinds needs >= 1.9.
//
// API version 1.5 is the lowest we support
#if GASNET_HAVE_MK_CLASS_MULTIPLE
  // No need to check MAJOR,MINOR against FI_VERSION(1, 9) since configure has verified version 1.11+.
  #define OFI_CONDUIT_VERSION FI_VERSION(1, 9)
#elif FI_VERSION(FI_MAJOR_VERSION, FI_MINOR_VERSION) >= FI_VERSION(1, 5)
  #define OFI_CONDUIT_VERSION FI_VERSION(1, 5)
#else
  #error "ofi-conduit requires libfabric 1.5 or newer"
#endif

#if GASNET_PSHM
    #define gasnetc_AMPSHMPoll(repliesOnly) gasneti_AMPSHMPoll(repliesOnly GASNETI_THREAD_PASS)
#else
    #define gasnetc_AMPSHMPoll(repliesOnly) ((void)0)
#endif 

#define GASNETC_OFI_POLL_EVERYTHING() do { gasnetc_ofi_poll(); gasnetc_AMPSHMPoll(0);} while (0)
#define GASNETC_OFI_POLL_SELECTIVE(type) do {\
    if (type == OFI_POLL_ALL) {\
        gasnetc_ofi_am_recv_poll_cold(1);\
        gasnetc_AMPSHMPoll(0);\
    }\
    else {\
        gasnetc_AMPSHMPoll(1);\
    }\
    gasnetc_ofi_am_recv_poll_cold(0);\
    gasnetc_ofi_tx_poll();\
}while(0)


#if GASNET_PAR
static inline int gasnetc_is_exiting(void) {
    gasneti_sync_reads();
    return gasnetc_exit_in_progress;
}
#define gasnetc_is_exit_error(e) \
  (gasnetc_is_exiting() && ((e).err == FI_SUCCESS || (e).err == FI_ECANCELED || (e).err == EACCES))
#else
#define gasnetc_is_exit_error(e) 0
#endif

// Conversion between conduit's AM context types and operation context
// The "to" operations include type checking that simple casts would not

GASNETI_INLINE(gasnetc_send_ctxt_to_op_ctxt)
void *gasnetc_send_ctxt_to_op_ctxt(gasnetc_ofi_send_ctxt_t *p)
{ return &p->ctxt; }

GASNETI_INLINE(gasnetc_op_ctxt_to_send_ctxt)
gasnetc_ofi_send_ctxt_t *gasnetc_op_ctxt_to_send_ctxt(void *p)
{ return gasneti_container_of(p, gasnetc_ofi_send_ctxt_t, ctxt); }

GASNETI_INLINE(gasnetc_recv_ctxt_to_op_ctxt)
void *gasnetc_recv_ctxt_to_op_ctxt(gasnetc_ofi_recv_ctxt_t *p)
{ return &p->ctxt; }

GASNETI_INLINE(gasnetc_op_ctxt_to_recv_ctxt)
gasnetc_ofi_recv_ctxt_t *gasnetc_op_ctxt_to_recv_ctxt(void *p)
{ return gasneti_container_of(p, gasnetc_ofi_recv_ctxt_t, ctxt); }


// We reserve low 2 bits to hold per-operation "aux" data for callbacks
// Assumes pointers have at least 4-bytes alignment in structs
#define GASNETC_CTXT_MASK (~(uintptr_t)3)
#define GASNETC_CTXT_IS_ALC ((uintptr_t)1)
#define GASNETC_CTXT_IS_RMA ((uintptr_t)2)

// Conversion from conduit's RDMA contexts to fi operation context.
// This is for use with gasnetc_ofi_{nb,bounce,blocking}_op_ctxt_t,
// where we use the callback function as the operation context.
// The callbacks perform the reverse using gasneti_container_of().
GASNETI_INLINE(gasnetc_rdma_ctxt_to_op_ctxt_inner) GASNETT_PURE
void *gasnetc_rdma_ctxt_to_op_ctxt_inner(void *p, unsigned int aux)
{
  uintptr_t raw = (uintptr_t)p;
  gasneti_assert(0 == (raw & ~GASNETC_CTXT_MASK));
  gasneti_assert(0 == (aux &  GASNETC_CTXT_MASK));
  return (void *)(raw | aux);
}
GASNETT_PUREP(gasnetc_rdma_ctxt_to_op_ctxt_inner)
#define gasnetc_rdma_ctxt_to_op_ctxt(p,aux) \
        gasnetc_rdma_ctxt_to_op_ctxt_inner(&(p)->callback,aux)

// Convert fi operation context to rdma callback function and call
GASNETI_INLINE(gasnetc_op_ctxt_run_rdma_callback)
void gasnetc_op_ctxt_run_rdma_callback(void *raw, uintptr_t aux)
{
  void *ctxt = (void *)((uintptr_t)raw & GASNETC_CTXT_MASK);
  gasnetc_rdma_callback_fn callback = *(gasnetc_rdma_callback_fn *)ctxt;
  callback(ctxt, aux);
}

/*-------------------------------------------------
 * Function Declarations
 *-------------------------------------------------*/
GASNETI_INLINE(gasnetc_ofi_handle_am)
void gasnetc_ofi_handle_am(gasnetc_ofi_am_send_buf_t *header, int isreq, size_t msg_len, uint64_t cq_data);
void gasnetc_ofi_am_send_complete(gasnetc_ofi_send_ctxt_t *header);
void gasnetc_ofi_tx_poll(void);
GASNETI_INLINE(gasnetc_ofi_am_recv_poll)
void gasnetc_ofi_am_recv_poll(int is_request);
void gasnetc_ofi_am_recv_poll_cold(int is_request) { // non-inline wrapper to avoid forced inlining on "cold" paths
  gasnetc_ofi_am_recv_poll(is_request);
}
GASNETI_NEVER_INLINE(gasnetc_fi_cq_readerr, // this wrapper silences a warning on gcc 4.8.5
ssize_t gasnetc_fi_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf, uint64_t flags)) {
  return fi_cq_readerr(cq, buf, flags);
}

// ofi-conduit should not be considered "portable" when using
// a high-performance provider (unless used w/ inappropriate h/w)
int gasnetc_check_portable_conduit(void) {
  gasneti_assert(gasnetc_ofi_inited);
  if (! strcmp(gasnetc_ofi_provider, "verbs;ofi_rxm")) {
    // extension of bug 3609: some verbs-compatible networks need special handling
    // TODO: warn specifically about the right providers
    if (!strncmp(gasnetc_ofi_domain, "hfi1_", 5)) return 1; // psm2
    if (!strncmp(gasnetc_ofi_domain, "qib", 3))   return 1; // psm
  }
  return !gasnetc_high_perf_prov;
}

// Check if the argument macthes the configured provider(s).
// Considers the value of FI_PROVIDER (if any) in the generic case.
// return:
//   NO:    0 if the argument cannot be the eventually selected provider
//   MAYBE: 1 if the argument might be the eventually selected provider
//   YES:   2 if the argument must be the eventually selected provider
#define GASNETC_EARLY_PROVIDER_NO    0
#define GASNETC_EARLY_PROVIDER_MAYBE 1
#define GASNETC_EARLY_PROVIDER_YES   2
static int gasnetc_early_provider_check(const char *prov_name) {
  // Argument cannot be "generic" nor may it contain a semi-colon
  gasneti_assert(gasneti_strcasecmp(prov_name, "generic"));
  gasneti_assert(strchr(prov_name,';') == NULL);

  const char *provider = gasneti_getenv("FI_PROVIDER");
  const char *provider_ident = _STRINGIFY(GASNETC_OFI_PROVIDER_IDENT);

  if (gasneti_strcasecmp(provider_ident, "generic")) {
    // Single-provider build
    return gasneti_strcasecmp(prov_name, provider_ident) ? GASNETC_EARLY_PROVIDER_NO
                                                         : GASNETC_EARLY_PROVIDER_YES;
  } else if (provider && provider[0]) {
    // Generic build, constrained to one provider by FI_PROVIDER
    char *delim = strchr(provider,';');
    size_t len = delim ? (delim - provider) : strlen(provider);
    return gasneti_strncasecmp(prov_name, provider, len) ? GASNETC_EARLY_PROVIDER_NO
                                                         : GASNETC_EARLY_PROVIDER_YES;
  } else {
    // Generic build, unconstrained
    return GASNETC_EARLY_PROVIDER_MAYBE;
  }
}

// Compare library version (according to header) against a given value.
// returns
//   0 for equal
//   1 for build is newer than given
//  -1 for build is older than given
static int gasnetc_header_version_cmp(unsigned int major, unsigned int minor, unsigned int revision) {
  gasneti_assert_uint(major ,<=, 0xffff);
  gasneti_assert_uint(minor ,<=, 0xffff);
  gasneti_assert_uint(revision ,<=, 0xffff);
  uint64_t given = FI_VERSION(major, minor);
  uint64_t build = FI_VERSION(FI_MAJOR_VERSION, FI_MINOR_VERSION);
#ifdef FI_REVISION_VERSION
  given = (given << 16) | revision;
  build = (build << 16) | FI_REVISION_VERSION;
#endif
  return (build == given) ? 0 : ((build < given) ? -1 : 1);
}

// Reads any user-provided settings from the environment to avoid clogging up
// the gasnetc_ofi_init() function with this code.
// Runs after provier and domain selection to allow for provider-specific defaults
static void gasnetc_ofi_read_env_vars(const char *provider, const char *domain) {
    const char* am_max_medium_env =  "GASNET_OFI_MAX_MEDIUM";
    const char* max_am_request_buffs_env =  "GASNET_OFI_MAX_REQUEST_BUFFS";
    const char* max_am_reply_buffs_env =  "GASNET_OFI_MAX_REPLY_BUFFS";
    const char* num_init_request_buffs_env = "GASNET_OFI_NUM_INITIAL_REQUEST_BUFFS";
    const char* num_init_reply_buffs_env = "GASNET_OFI_NUM_INITIAL_REPLY_BUFFS";
    const char* max_err_string =  "%s must be greater than or equal to\n"
                                  "%s, which is set to %d in this run.\n";
    const char* init_err_string = "%s must be greater than or equal to 2.\n";

    // Maxiumum size of an AM Medium payload
    gasnetc_ofi_max_medium = gasneti_getenv_int_withdefault(am_max_medium_env, GASNETC_OFI_MAX_MEDIUM_DFLT, 1);
    if (gasnetc_ofi_max_medium < 512) {
      gasneti_fatalerror("%s setting (%lu) is below the minimum value of 512.",
                         am_max_medium_env, (unsigned long)gasnetc_ofi_max_medium);
    }

    // Maximum and initial number of buffers to allocate for AM Requests
    max_am_request_buffs = gasneti_getenv_int_withdefault(max_am_request_buffs_env, 1024, 0);
    size_t dflt = MIN(256, max_am_request_buffs);
    num_init_am_request_buffs = gasneti_getenv_int_withdefault(num_init_request_buffs_env, dflt, 0);
    if (num_init_am_request_buffs < 2) {
        gasneti_fatalerror(init_err_string, num_init_request_buffs_env);
    }
    if (max_am_request_buffs < num_init_am_request_buffs) {
        gasneti_fatalerror(max_err_string, max_am_request_buffs_env, num_init_request_buffs_env, 
                (int)num_init_am_request_buffs);
    }

    // Maximum and initial number of buffers to allocate for AM Replies
    max_am_reply_buffs = gasneti_getenv_int_withdefault(max_am_reply_buffs_env, 1024, 0);
    dflt = MIN(256, max_am_reply_buffs);
    num_init_am_reply_buffs = gasneti_getenv_int_withdefault(num_init_reply_buffs_env, dflt, 0);
    if (num_init_am_reply_buffs < 2) {
        gasneti_fatalerror(init_err_string, num_init_reply_buffs_env);
    }
    if (max_am_reply_buffs < num_init_am_reply_buffs) {
        gasneti_fatalerror(max_err_string, max_am_reply_buffs_env, num_init_reply_buffs_env, 
                (int)num_init_am_reply_buffs);
    }

    /* The number of RMA requests to be issued before a tx_poll takes place */
    rdma_periodic_poll_threshold = gasneti_getenv_int_withdefault("GASNET_OFI_RMA_POLL_FREQ", 32, 0);

    ofi_num_bbufs = gasneti_getenv_int_withdefault("GASNET_OFI_NUM_BBUFS", 64, 0);
    ofi_bbuf_size = gasneti_getenv_int_withdefault("GASNET_OFI_BBUF_SIZE", GASNET_PAGESIZE, 1);
    gasnetc_ofi_bbuf_threshold = gasneti_getenv_int_withdefault("GASNET_OFI_BBUF_THRESHOLD", 4*ofi_bbuf_size, 1);

    if (ofi_num_bbufs < gasnetc_ofi_bbuf_threshold/ofi_bbuf_size)
        gasneti_fatalerror("The number of bounce buffers must be greater than or equal to the bounce\n"
                "buffer threshold divided by the bounce buffer size. See the ofi-conduit README.\n");

    if (gasnetc_ofi_bbuf_threshold/ofi_bbuf_size > OFI_MAX_NUM_BOUNCE_BUFFERS) {
        gasneti_fatalerror("The ofi-conduit limits the max number of bounce buffers used in the non-blocking\n"
                "put path to %d. Your selections for the bounce buffer tuning parameters exceed this. If you\n" 
                "truly need more than %d bounce buffers, edit the OFI_MAX_NUM_BOUNCE_BUFFERS macro in\n" 
                "gasnet_ofi.c and recompile.\n", OFI_MAX_NUM_BOUNCE_BUFFERS, OFI_MAX_NUM_BOUNCE_BUFFERS);
    }

    // What is the largest ofi-level message we can receive?
    min_multi_recv = MAX(offsetof(gasnetc_ofi_am_send_buf_t, buf.long_buf.data),
                         offsetof(gasnetc_ofi_am_send_buf_t, buf.medium_buf.data))
                     + OFI_AM_MAX_DATA_LENGTH;
    min_multi_recv = GASNETI_ALIGNUP(min_multi_recv, GASNETI_MEDBUF_ALIGNMENT);

    const char* num_multirecv_buffs_env = "GASNET_OFI_NUM_RECEIVE_BUFFS";
    const char* multirecv_size_env = "GASNET_OFI_RECEIVE_BUFF_SIZE";
    const char *env_val = gasnet_getenv(multirecv_size_env);
    if (! gasneti_strcasecmp("SINGLE", env_val) || ! gasneti_strcasecmp("RECV", env_val)) {
        const char *value = gasneti_dynsprintf("%s => %d", env_val, (int)min_multi_recv);
        gasneti_envstr_display(multirecv_size_env, value, 0);
        multirecv_buff_size = min_multi_recv;
        // See Bug 4478 for information leading to the selection of 450 as a default.
        // TODO: at least consider scaling this with PPN
        num_multirecv_buffs = gasneti_getenv_int_withdefault(num_multirecv_buffs_env, 450, 0);
        if (! gasneti_strcasecmp("RECV", env_val)) {
          // single-message buffers are posted *without* FI_MULTI_RECV
          maybe_multi_recv = 0;
        }
    } else {
        multirecv_buff_size = gasneti_getenv_int_withdefault(multirecv_size_env, 1024*1024, 1);
        num_multirecv_buffs = gasneti_getenv_int_withdefault(num_multirecv_buffs_env, 8, 0);
    }
    gasneti_assert((maybe_multi_recv == FI_MULTI_RECV) || ! gasneti_strcasecmp("RECV", env_val));

    if (num_multirecv_buffs < 2)
        gasneti_fatalerror("%s must be at least 2.\n", num_multirecv_buffs_env);

    if (multirecv_buff_size < min_multi_recv) {
        gasneti_fatalerror("%s must be at least %d bytes on this build.\n"
                "This is the size of the largest AM, including its message header.\n", \
                multirecv_size_env, (int)min_multi_recv);
    }

    const char* long_rma_threshold_env = "GASNET_OFI_LONG_AM_RMA_THRESH";
    long_rma_threshold = gasneti_getenv_int_withdefault(long_rma_threshold_env, OFI_AM_MAX_DATA_LENGTH, 1);
    if (long_rma_threshold > OFI_AM_MAX_DATA_LENGTH) {
            gasneti_fatalerror(
                "The value given for %s exceeds the amount\n"
                "of data which can be packed into a medium message (%d bytes on this build).\n"
                "Use a lower value or reconfigure GASNet for a bigger medium message size using\n"
                "--with-ofi-max-medium=<new size>.\n",
                long_rma_threshold_env, (int)OFI_AM_MAX_DATA_LENGTH);
    }

    tx_cq_size = gasneti_getenv_int_withdefault("GASNET_OFI_TX_CQ_SIZE", 0, 0);
    rx_cq_size = gasneti_getenv_int_withdefault("GASNET_OFI_RX_CQ_SIZE", 0, 0);
}

/* The intention of separating this logic from gasnetc_ofi_init() is
 * to contain the complexity of supporting scalable endpoints in the future to
 * this function and the relevant get-address macros.
 */
static void ofi_setup_address_vector(void) {
  int ret = FI_SUCCESS;
  struct fi_av_attr     av_attr     = {0};

#if USE_AV_MAP
  av_attr.type        = FI_AV_MAP;
#else
  av_attr.type        = FI_AV_TABLE;
#endif
  av_attr.count       = gasneti_nodes;

  // TODO:
  //  + set av_attr.ep_per_node to improve provider resource allocation?
  //  + set av_attr.name and mmap_addr to share address vectors within a host?

  // Create an AV per endpoint
  for (int i = 0; i < NUM_OFI_ENDPOINTS; ++i) {
    if (GASNETC_OFI_USE_AV_MAP) {
      gasnetc_addr_map[i] = (fi_addr_t*)gasneti_malloc(sizeof(fi_addr_t) * av_attr.count);
    }
    ret = fi_av_open(gasnetc_ofi_domainfd, &av_attr, &gasnetc_ofi_avfd[i], NULL);
    GASNETC_OFI_CHECK_RET(ret, "fi_av_open failed");
  }

  // Bind AVs to distinct endpoints
  ret = fi_ep_bind(gasnetc_ofi_rdma_epfd, &gasnetc_ofi_avfd[GASNETC_FADDR_IDX_RDMA]->fid, 0);
  GASNETC_OFI_CHECK_RET(ret, "fi_ep_bind for avfd to rdma_epfd failed");

  ret = fi_ep_bind(gasnetc_ofi_request_epfd, &gasnetc_ofi_avfd[GASNETC_FADDR_IDX_REQ]->fid, 0);
  GASNETC_OFI_CHECK_RET(ret, "fi_ep_bind for avfd to am request epfd failed");

  ret = fi_ep_bind(gasnetc_ofi_reply_epfd, &gasnetc_ofi_avfd[GASNETC_FADDR_IDX_REP]->fid, 0);
  GASNETC_OFI_CHECK_RET(ret, "fi_ep_bind for avfd to am reply epfd failed");
}

static void ofi_exchange_addresses(void) {
  size_t reqnamelen = 0, repnamelen = 0, rdmanamelen = 0;
  char* on_node_addresses;
  int ret = FI_SUCCESS;

  /* Query each endpoint for its address length. While in most cases, these
   * lengths will be equal, there are some cases where they might not be. For
   * example, when using both IPv4 and IPv6. */
  ret = fi_getname(&gasnetc_ofi_request_epfd->fid, NULL, &reqnamelen);
  gasneti_assert(ret == -FI_ETOOSMALL);
  ret = fi_getname(&gasnetc_ofi_reply_epfd->fid, NULL, &repnamelen);
  gasneti_assert(ret == -FI_ETOOSMALL);
  ret = fi_getname(&gasnetc_ofi_rdma_epfd->fid, NULL, &rdmanamelen);
  gasneti_assert(ret == -FI_ETOOSMALL);

  size_t total_len = reqnamelen + repnamelen + rdmanamelen;
  on_node_addresses = gasneti_malloc(total_len);

  char* alladdrs = gasneti_malloc(gasneti_nodes*total_len);
  char* p = on_node_addresses;
  size_t offsets[NUM_OFI_ENDPOINTS];

  gasneti_static_assert(GASNETC_FADDR_IDX_REQ == 0);
  ret = fi_getname(&gasnetc_ofi_request_epfd->fid, p, &reqnamelen);
  GASNETC_OFI_CHECK_RET(ret, "fi_getname failed for the AM request endpoint");
  offsets[GASNETC_FADDR_IDX_REQ] = p - on_node_addresses;
  p += reqnamelen;

  gasneti_static_assert(GASNETC_FADDR_IDX_REP == 1);
  ret = fi_getname(&gasnetc_ofi_reply_epfd->fid, p, &repnamelen);
  GASNETC_OFI_CHECK_RET(ret, "fi_getname failed for the AM reply endpoint");
  offsets[GASNETC_FADDR_IDX_REP] = p - on_node_addresses;
  p += repnamelen;

  gasneti_static_assert(GASNETC_FADDR_IDX_RDMA == 2);
  ret = fi_getname(&gasnetc_ofi_rdma_epfd->fid, p, &rdmanamelen);
  GASNETC_OFI_CHECK_RET(ret, "fi_getname failed for the RDMA endpoint");
  offsets[GASNETC_FADDR_IDX_RDMA] = p - on_node_addresses;
  p += rdmanamelen;

  gasneti_assert_ptr(p ,==, on_node_addresses + total_len);

  gasneti_bootstrapExchange(on_node_addresses, total_len, alladdrs);

  // Interleaving of three endpoints' addresses leads to this
  // transpose-order traversal and "round-robin" insertion
  for (int i = 0; i < NUM_OFI_ENDPOINTS; ++i) {
    p = alladdrs + offsets[i];
    for (gex_Rank_t j = 0; j < gasneti_nodes; ++j) {
      uint64_t flags = (j == gasneti_nodes - 1) ? 0 : FI_MORE;
      fi_addr_t *addr_p = GASNETC_OFI_USE_AV_MAP ? gasnetc_addr_map[i] + j : NULL;
      ret = fi_av_insert(gasnetc_ofi_avfd[i], p, 1, addr_p, flags , NULL);
      if (ret != 1) {
        gasneti_fatalerror("fi_av_insert(rank=%u, ep=%d) failed %d\n", j, i, ret);
      }
      p += total_len;
    }
  }

  gasneti_free(alladdrs);
  gasneti_free(on_node_addresses);
}

// Visit available info structs, grouped by provider from most to least preferred.
// Invokes callback for each struct, terminates early if the callback returns non-zero.
// This is a helper for other functions
typedef int (*gasnetc_info_visitor_t)(const struct fi_info *p, void *context);
static void gasnetc_info_foreach(struct fi_info *info, gasnetc_info_visitor_t callback, void *context)
{
  for (const char *q = supported_providers; *q; /*empty*/) {
      while (*q == ' ') ++q;
      const char *r = strchr(q, ' ');
      int prov_name_len = r ? r - q : strlen(q);
      for (struct fi_info *p = info; p; p = p->next) {
          if (! gasneti_strncasecmp(p->fabric_attr->prov_name, q, prov_name_len)) {
              if (callback(p, context)) return;
          }
      }
      q += prov_name_len;
  }
}

// List available devices, grouped by provider from most to least preferred
static int gasnetc_list_devices_visitor(const struct fi_info *p, void *context)
{
  // This visitor adds one line to a multi-line message for each struct visted, with de-duplication
  char **msg_p = (char **)context;
  char *msg = *msg_p;
  const char *prov_name = p->fabric_attr->prov_name;
  const char *dev_name = p->domain_attr->name;
  char line[128];
  snprintf(line, sizeof(line)-1, "\n        %-16s %s", prov_name, dev_name);
  if (!msg) {
    *msg_p = gasneti_strdup(line);
  } else if (!strstr(msg, line)) {
    // append 'line' to 'msg'
    size_t old_len = strlen(msg);
    size_t new_len = old_len + strlen(line) + 1;
    *msg_p = msg = gasneti_realloc(msg, new_len);
    strcpy(msg + old_len, line);
  }
  return 0; // continue traversal
}
static void gasnetc_list_devices(struct fi_info *hints)
{
  struct fi_info *info = NULL;
  int ret = fi_getinfo(OFI_CONDUIT_VERSION, NULL, NULL, 0ULL, hints, &info);
  if (FI_SUCCESS != ret) {
    gasneti_console_message("INFO", "Failed to detect any supported providers and/or devices");
    return;
  }

  char *msg = NULL;
  gasnetc_info_foreach(info, gasnetc_list_devices_visitor, &msg);
  gasneti_console_message("INFO", "Detected the following provider and device pair(s)%s", msg);
  gasneti_free(msg);
  fi_freeinfo(info);
  return;
}

// Find the first entry for the most-preferred provider offered, if any.
static int gasnetc_getinfo_visitor(const struct fi_info *p, void *context)
{
  // This visitor simply dups the first match
  *(struct fi_info **)context = fi_dupinfo(p);
  return 1; // end traversal
}
static struct fi_info *gasnetc_ofi_getinfo(struct fi_info *hints)
{
  struct fi_info *info = NULL;
  int ret = fi_getinfo(OFI_CONDUIT_VERSION, NULL, NULL, 0ULL, hints, &info);
  if (FI_SUCCESS != ret) {
    return NULL;
  }

  struct fi_info *result = NULL;
  gasnetc_info_foreach(info, gasnetc_getinfo_visitor, &result);
  fi_freeinfo(info);
  return result; // caller will notice if this is the wrong provider
}

// Utility function to set an environment variable with proper tracing/logging
// Returns zero if the variable was already set, non-zero otherwise.
// Returns zero if !replace and the variable was already set to a different
// (case-insensitive) value, non-zero otherwise.
int gasnetc_setenv_string(const char *key, const char *val, int replace)
{
    const char *prev = gasneti_getenv(key);
    int same = prev && !gasneti_strcasecmp(prev, val);
    if (same) {
        GASNETI_TRACE_PRINTF(I, ("Preserving default %s='%s' in environment", key, prev));
        gasneti_envstr_display(key, prev, 1);
        return 1;
    } else if (!replace && prev) {
        GASNETI_TRACE_PRINTF(I, ("Not overwriting %s='%s' in environment", key, prev));
        gasneti_envstr_display(key, prev, 0);
        return 0;
    } else {
        GASNETI_TRACE_PRINTF(I, ("Setting %s='%s' in environment", key, val));
        gasneti_envstr_display(key, val, 1);
        gasneti_setenv(key, val);
        return 1;
    }
}

// Wrapper for case of gasnetc_setenv_string for an unsigned int key
int gasnetc_setenv_uint(const char *key, unsigned int val, int replace)
{
      char valstr[16];
      snprintf(valstr, sizeof(valstr), "%u", val);
      return gasnetc_setenv_string(key, valstr, replace);
}

// Helper for (large) page-aligned allocation
#include <sys/mman.h> // for MAP_FAILED
static void *gasnetc_alloc_pages(size_t len, const char *desc)
{
    void *result = gasneti_mmap(GASNETI_PAGE_ALIGNUP(len));
    if (MAP_FAILED == result) {
        gasneti_fatalerror("Failed to allocate %"PRIuSZ " bytes %s", len, desc);
    }
    return result;
}

static void gasnetc_check_version(const char *prov_name, unsigned int major, unsigned int minor)
{
  if (strcmp(prov_name,gasnetc_ofi_provider)) return;

  uint32_t have = fi_version();
  uint32_t want = FI_VERSION(major,minor);
  if (FI_VERSION_GE(have, want)) return;

  const char *msg = gasneti_dynsprintf(
              "Provider '%s' is untrusted in libfabric version %d.%d "
              "(minimum trusted version is %d.%d).",
              gasnetc_ofi_provider, FI_MAJOR(have), FI_MINOR(have), major, minor);
  // UNDOCUMENTED mechanism to suppress the error:
  if (gasneti_getenv_yesno_withdefault("GASNET_OFI_ALLOW_UNTRUSTED_PROVIDER", 0)) {
    gasneti_console0_message("WARNING","%s",msg);
  } else {
    gasneti_fatalerror("%s",msg);
  }
}

/*------------------------------------------------
 * Initialize OFI conduit
 * ----------------------------------------------*/
int gasnetc_ofi_init(void)
{
  int ret = GASNET_OK;
  int result = GASNET_ERR_NOT_INIT;
  struct fi_info        *hints, *info;
  struct fi_cq_attr     tx_cq_attr  = {0};
  struct fi_cq_attr     rx_cq_attr  = {0};
  size_t optval;
  int num_locks; 
  int i;
  
  // Ensure uniform FI_* env vars
  gasneti_propagate_env("FI_", GASNETI_PROPAGATE_ENV_PREFIX);

  // Ensure uniform MLX5_* env vars for verbs provider
  // Especially important for work-arounds like MLX5_SCATTER_TO_CQE
  gasneti_propagate_env("MLX5_", GASNETI_PROPAGATE_ENV_PREFIX);

  // Ensure uniform HFI_* env vars for psm2 provider
  // Especially important for work-arounds like HFI_NO_CPUAFFINITY
  gasneti_propagate_env("HFI_", GASNETI_PROPAGATE_ENV_PREFIX);

  // TODO: other providers?

#if GASNETC_OFI_USE_THREAD_DOMAIN && GASNET_PAR
  gasneti_spinlock_init(&gasnetc_ofi_locks.big_lock);
#elif GASNET_PAR
  /* This lock is needed in PAR mode to protect the AM reference counting */
  gasneti_spinlock_init(&gasnetc_ofi_locks.rx_request_cq);
  gasneti_spinlock_init(&gasnetc_ofi_locks.rx_reply_cq);
#endif
#if 0
  gasneti_spinlock_init(&gasnetc_ofi_locks.tx_cq);
  gasneti_spinlock_init(&gasnetc_ofi_locks.am_rx);
  gasneti_spinlock_init(&gasnetc_ofi_locks.am_tx);
  gasneti_spinlock_init(&gasnetc_ofi_locks.rdma_rx);
  gasneti_spinlock_init(&gasnetc_ofi_locks.rdma_tx);
#endif

  /* OFI initialization */

  /* Alloc hints*/
  hints = fi_allocinfo();
  if (!hints) gasneti_fatalerror("fi_allocinfo for hints failed\n");

  // constrain the device/domain if provided by the user
  gasnetc_ofi_device = gasneti_getenv_hwloc_withdefault("GASNET_OFI_DEVICE", "", "Socket");
  if (!strlen(gasnetc_ofi_device)) gasnetc_ofi_device = NULL;
  hints->domain_attr->name = gasnetc_ofi_device;

  /* caps: fabric interface capabilities */
  hints->caps           = FI_RMA | FI_MSG | FI_MULTI_RECV;
#if GASNET_HAVE_MK_CLASS_MULTIPLE
  hints->caps          |= FI_HMEM;
#endif
  /* mode: convey requirements for application to use fabric interfaces */
  hints->mode           = FI_CONTEXT;   /* fi_context is used for per
                                           operation context parameter */
  /* addr_format: expected address format for AV/CM calls */
  hints->addr_format        = FI_FORMAT_UNSPEC;
  hints->ep_attr->type      = FI_EP_RDM; /* Reliable datagram */
  /* Threading mode is set by the configure script to FI_THREAD_DOMAIN if
   * using the psm2 provider and FI_THREAD_SAFE otherwise*/
#if GASNETC_OFI_USE_THREAD_DOMAIN || !GASNET_PAR
  hints->domain_attr->threading         = FI_THREAD_DOMAIN;
#else
  hints->domain_attr->threading         = FI_THREAD_SAFE;
#endif

  hints->domain_attr->control_progress  = FI_PROGRESS_MANUAL;
  /* resource_mgmt: FI_RM_ENABLED - provider protects against overrunning 
     local and remote resources. */
  hints->domain_attr->resource_mgmt     = FI_RM_ENABLED;

  // av_type: type of address vectors that are usable with this domain
#if !GASNETC_OFI_USE_AV_MAP_STATIC
  hints->domain_attr->av_type           = FI_AV_UNSPEC;
#else
  hints->domain_attr->av_type           = GASNETC_OFI_USE_AV_MAP ? FI_AV_MAP : FI_AV_TABLE;
#endif

  // The four bits we compose here are basically FI_MR_BASIC decomposed plus FI_MR_ENDPOINT:
  hints->domain_attr->mr_mode = FI_MR_ENDPOINT;
#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
  // We expect all segment registrations to be of allocated virtual addresses
  hints->domain_attr->mr_mode |= FI_MR_ALLOCATED;
#else
  // EVERYTHING fundamentally cannot satisfy the "ALLOCATED" constraint.
  // Note that some providers will set this bit on output even if clear on input.
#endif
#if GASNETC_OFI_HAS_MR_VIRT_ADDR_STATIC
  // Set FI_MR_VIRT_ADDR according to configure-time selection
  hints->domain_attr->mr_mode |= GASNETC_OFI_HAS_MR_VIRT_ADDR ? FI_MR_VIRT_ADDR : 0;
#else
  // Try with FI_MR_VIRT_ADDR bit set.  The provider may clear it.
  hints->domain_attr->mr_mode |= FI_MR_VIRT_ADDR;
#endif
#if GASNETC_OFI_HAS_MR_PROV_KEY_STATIC
  // Set FI_MR_PROV_KEY according to configure-time selection
  hints->domain_attr->mr_mode |= GASNETC_OFI_HAS_MR_PROV_KEY ? FI_MR_PROV_KEY : 0;
#else
  // Try with FI_MR_PROV_KEY bit set.  Either we or the provider may clear it
  hints->domain_attr->mr_mode |= FI_MR_PROV_KEY;
#endif

  // If user has requested, list devices prior to adding FI_MR_HMEM which
  // is optional (we eventually retry w/o it if it leads to no matches).
  if (gasneti_getenv_yesno_withdefault("GASNET_OFI_LIST_DEVICES", 0) &&
      gasneti_check_node_list("GASNET_OFI_LIST_DEVICES_NODES")) {
      gasnetc_list_devices(hints);
  }

#if GASNET_HAVE_MK_CLASS_MULTIPLE
  hints->domain_attr->mr_mode |= FI_MR_HMEM;
#endif

  // Setup various environment variables quite early, before the provider may
  // have been determined.  This is necessary because fi_getinfo() may read them.
  // NOTE: spawn via an ofi-based MPI may have read these even earlier!

  // Provider-independent FI_MR_CACHE_MAX_{SIZE,COUNT}:
  // The defaults are known to have a negative impact on many applications
  // when using cxi or verbs providers (bug 4676).
  // To avoid unforeseen problems on conduits not known to demonstrate the
  // performance issue, we set these variables only for these two providers.
  // In particular, we do not set these for the "generic" provider case unless
  // `FI_PROVIDER` has been set to ensure one of those two will be selected.
  if (gasnetc_early_provider_check("cxi")   == GASNETC_EARLY_PROVIDER_YES ||
      gasnetc_early_provider_check("verbs") == GASNETC_EARLY_PROVIDER_YES) {
    gasnetc_setenv_string("FI_MR_CACHE_MAX_SIZE",  "-1", 0 /* = no replacement */);
    gasnetc_setenv_string("FI_MR_CACHE_MAX_COUNT", "-1", 0 /* = no replacement */);
  }
  // Implement our "opt-out" behavior of converting empty values for these two
  // variables to unset.  Otherwise libfabric use of `strtol()` parses empty
  // strings as `0`, resulting in disabling the cache!
  // We do this unconditionally, to avoid letting a user's "defensive" setting
  // do harm for a provider using the cache but not in our allow-list.
  { const char *vars[] =  { "FI_MR_CACHE_MAX_SIZE",
                            "FI_MR_CACHE_MAX_COUNT", };
    size_t count = sizeof(vars) / sizeof(vars[0]);
    for (size_t i = 0; i < count; ++i) {
      const char *var = vars[i];
      const char *val = gasneti_getenv(var);
      if (val && !val[0]) {
        gasneti_unsetenv(var);
        GASNETI_TRACE_PRINTF(I, ("Converting empty %s in environment to unset", var));
      }
    }
  }

  // Provider-independent FI_UNIVERSE_SIZE:
  // Ideally, FI_UNIVERSE_SIZE should always match our process count unless is
  // has already been set.
  // However, due to bug 4413, we currently only set it on an "opt-in" basis if
  // we may be using cxi provider prior to libfabric 1.15.2.0.
  {
    int dflt = 1;
#if GASNETI_ARCH_CRAYEX
    if (gasnetc_early_provider_check("cxi") && (gasnetc_header_version_cmp(1,15,2) < 0)) {
      // default should be 0 when cxi < 1.15.2.0 may be running
      dflt = 0;
    }
#endif
    if (gasneti_getenv_yesno_withdefault("GASNET_OFI_SET_UNIVERSE_SIZE", dflt)) {
      gasnetc_setenv_uint("FI_UNIVERSE_SIZE", gasneti_nodes, 0 /* = no replacement */);
    }
  }

  // PSM2 provider:
  // In libfabric v1.6, the psm2 provider transitioned to using separate
  // psm2 endpoints for each ofi endpoint, whereas in the past all communication
  // was multiplexed over a single psm2 endpoint. Setting this variable ensures
  // that unnecessary connections between remote endpoints which never communicate
  // are not made, which can cause slow tear-down.
  int set_psm2_lazy_conn = 0;
  if (FI_VERSION(FI_MAJOR_VERSION, FI_MINOR_VERSION) >= FI_VERSION(1, 6)) {
      set_psm2_lazy_conn = gasnetc_setenv_string("FI_PSM2_LAZY_CONN", "1", 1);
  }

  // CXI provider:
  // To handle bursty AM traffic, enable hybrid receive mode with reasonable default parameters.
  // If FI_CXI_RX_MATCH_MODE is already set to something else, we make NO changes (or risk an
  // inconsistent mess).
  const char *initial_CXI_RX_MATCH_MODE  = gasnet_getenv("FI_CXI_RX_MATCH_MODE");
  const char *initial_CXI_RDZV_THRESHOLD = gasnet_getenv("FI_CXI_RDZV_THRESHOLD");
  const char *initial_CXI_RDZV_GET_MIN   = gasnet_getenv("FI_CXI_RDZV_GET_MIN");
  int set_cxi_match_mode = gasnetc_setenv_string("FI_CXI_RX_MATCH_MODE", "hybrid", 0);
  if (set_cxi_match_mode) {
    // Always try to set both RDZV parameters but will warn if either conflicts
    // Don't use && or || due to short-circuit evaluation
    if (! gasnetc_setenv_string("FI_CXI_RDZV_THRESHOLD", "256", 0)) set_cxi_match_mode = 0;
    if (! gasnetc_setenv_string("FI_CXI_RDZV_GET_MIN", "256", 0)) set_cxi_match_mode = 0;
  } // else: warning deferred until provider selection confirms use of CXI

  info = gasnetc_ofi_getinfo(hints);
  if (!info) {
  #if GASNET_HAVE_MK_CLASS_MULTIPLE
      hints->caps ^= FI_HMEM; // retry w/o FI_HMEM
      info = gasnetc_ofi_getinfo(hints);
      if (info) {
          // fall through
          // gasnetc_fi_hmem will get set to zero later, leading to
          // deferred failure in the first call to gex_MK_Create(), if any
      } else
  #endif
      GASNETI_RETURN_ERRR(RESOURCE,
              "No OFI providers found that could support the OFI conduit");
  }

  // Balk if provider was explicitly chosen at configure time and is not available now
  if (!strchr(supported_providers,' ') && strcmp(supported_providers, info->fabric_attr->prov_name)) {
      if (gasnetc_ofi_device) {
        // Retry to rule out invalid device choice
        hints->domain_attr->name = NULL;
        info = gasnetc_ofi_getinfo(hints);
        if (info && !strcmp(supported_providers, info->fabric_attr->prov_name)) {
          gasneti_fatalerror("Specifed device '%s' is not available or not usable", gasnetc_ofi_device);
        }
      }
      char *envvar = gasneti_getenv("FI_PROVIDER");
      gasneti_fatalerror(
          "OFI provider '%s' selected at configure time is not available at run time%s%s%s.",
          supported_providers,
          envvar ? " and/or has been overridden by FI_PROVIDER='" : "",
          envvar ? envvar : "",
          envvar ? "' in the environment" : "");
  }

  // Check if this provider is one we consider "high performance"
  const char *high_perf_providers[] = { "psm2", "cxi", "verbs;ofi_rxm" };
  for (i = 0; i < sizeof(high_perf_providers)/sizeof(high_perf_providers[0]); ++i) {
    if (!strcmp(info->fabric_attr->prov_name, high_perf_providers[i])) {
      gasnetc_high_perf_prov = 1;
      break;
    }
  }

  // psm2 provider needs some special handling
  if (!strcmp(info->fabric_attr->prov_name, "psm2")){
      using_psm_provider = 1;
  } else if (set_psm2_lazy_conn) {
      /* If we set this variable and are not using psm2, unset it in the
       * unlikely case that another library in the current application will
       * use ofi/psm2 */
      unsetenv("FI_PSM2_LAZY_CONN");
  }

  if (!strcmp(info->fabric_attr->prov_name, "cxi") && !set_cxi_match_mode) {
    const char *str0 = (initial_CXI_RX_MATCH_MODE && initial_CXI_RX_MATCH_MODE[0])
                     ? gasneti_dynsprintf("='%s'", initial_CXI_RX_MATCH_MODE)
                     : "";
    const char *str1 = (initial_CXI_RDZV_THRESHOLD && initial_CXI_RDZV_THRESHOLD[0])
                     ? gasneti_dynsprintf("='%s'", initial_CXI_RDZV_THRESHOLD)
                     : "";
    const char *str2 = (initial_CXI_RDZV_GET_MIN && initial_CXI_RDZV_GET_MIN[0])
                     ? gasneti_dynsprintf("='%s'", initial_CXI_RDZV_GET_MIN)
                     : "";
    gasneti_console0_message("WARNING", "ofi-conduit failed to configure FI_CXI_* "
                             "environment variables due to prior conflicting settings. "
                             "This may lead to unstable behavior and/or degraded "
                             "performance.  If you did not intentionally set "
                             "FI_CXI_RX_MATCH_MODE%s, FI_CXI_RDZV_THRESHOLD%s, or "
                             "FI_CXI_RDZV_GET_MIN%s then this condition may have resulted "
                             "from initializing MPI prior to initialization of GASNet. "
                             "For more information on that scenario, please see \"Limits "
                             "to MPI interoperability\" in the ofi-conduit README. ",
                             str0, str1, str2
                            );
  }

  int quiet = gasneti_getenv_yesno_withdefault("GASNET_QUIET", 0);
#if GASNET_PAR
  if (!using_psm_provider && GASNETC_OFI_USE_THREAD_DOMAIN) {
      const char * msg =
            "Using OFI provider \"%s\" when the ofi-conduit was configured for FI_THREAD_DOMAIN\n"
            "(possibly because the psm or psm2 provider was detected at configure time). In GASNET_PAR mode,\n"
            "this has the effect of using a global lock instead of fine-grained locking. If this causes \n"
            "undesirable performance in PAR, reconfigure GASNet using: --with-ofi-provider=%s --disable-thread-domain";
      if (quiet) GASNETI_TRACE_PRINTF(I,(msg, info->fabric_attr->prov_name, info->fabric_attr->prov_name));
      else gasneti_console0_message("WARNING", msg, info->fabric_attr->prov_name, info->fabric_attr->prov_name);
  }
#endif

  if (!gasnetc_high_perf_prov) {
      const char * msg = 
          "Using OFI provider (%s), which has not been validated to provide\n"
          "    WARNING: acceptable GASNet performance. You should consider using a more\n"
          "    WARNING: hardware-appropriate GASNet conduit. See ofi-conduit/README.";
      if (quiet) GASNETI_TRACE_PRINTF(I,(msg, info->fabric_attr->prov_name));
      else gasneti_console0_message("WARNING", msg, info->fabric_attr->prov_name);
  }

  gasneti_leak( gasnetc_ofi_provider = gasneti_strdup(info->fabric_attr->prov_name) );
  gasneti_leak( gasnetc_ofi_domain = gasneti_strdup(info->domain_attr->name) );

  // Ensure that subsequent calls to fi_getinfo() won't ever give us a
  // different provider.  This is necessary when more than one provider matches
  // the other hints, and the first match is not the one we want.
  hints->fabric_attr->prov_name = gasnetc_ofi_provider;
  hints->domain_attr->name = gasnetc_ofi_domain;

  // Check provider-specific minimum library versions (before checking modes)
  // REMINDER: these are documented in README and also enforced in configure.in
  gasnetc_check_version("udp;ofi_rxd"    , 1,7 );
#if HAVE_PMI_CRAY_H
  // HPE's libfabric 1.11.x.y.z for Slingshot-10
  gasnetc_check_version("verbs;ofi_rxm"  , 1,11);
#else
  gasnetc_check_version("verbs;ofi_rxm"  , 1,12);
#endif
  gasnetc_check_version("gni"            , 1,14);
  gasnetc_check_version("tcp;ofi_rxm"    , 1,15);

#if !GASNETC_OFI_HAS_MR_PROV_KEY_STATIC
  // We offered FI_MR_PROV_KEY, but would rather not support it if not required.
  // So, query again without FI_MR_PROV_KEY if present.
  if (info->domain_attr->mr_mode & FI_MR_PROV_KEY) {
    hints->domain_attr->mr_mode ^= FI_MR_PROV_KEY;
    struct fi_info *alt_info = gasnetc_ofi_getinfo(hints);
    int accept = (alt_info != NULL);
  #if GASNETC_OFI_HAS_MR_VIRT_ADDR_STATIC
    // Must preserve statically selected FI_MR_VIRT_ADDR, if any
    if (alt_info && (FI_MR_VIRT_ADDR & (alt_info->domain_attr->mr_mode ^ info->domain_attr->mr_mode))) {
      accept = 0;
    }
  #endif
    if (accept) {
      fi_freeinfo(info);
      info = alt_info;
    } else {
      if (alt_info) fi_freeinfo(alt_info);
      hints->domain_attr->mr_mode ^= FI_MR_PROV_KEY;
    }
  }
#endif

  has_mr_virt_addr = !!(info->domain_attr->mr_mode & FI_MR_VIRT_ADDR);
  has_mr_prov_key = !!(info->domain_attr->mr_mode & FI_MR_PROV_KEY);
  gasnetc_fi_mr_endpoint = (info->domain_attr->mr_mode & FI_MR_ENDPOINT);

#if GASNETC_OFI_HAS_MR_VIRT_ADDR_STATIC
  if (GASNETC_OFI_HAS_MR_VIRT_ADDR != has_mr_virt_addr) {
      gasneti_fatalerror("The statically-determined value for GASNETC_OFI_HAS_MR_VIRT_ADDR=%i does\n"
                         "  not match the memory registration support that the (%s) provider reported.\n"
                         "  This could happen if a provider has changed behavior between versions.\n"
                         "  Use configure option --%s-ofi-mr-virt-addr to correct this.",
                         GASNETC_OFI_HAS_MR_VIRT_ADDR,
                         info->fabric_attr->prov_name,
                         (has_mr_virt_addr ? "enable" : "disable"));
  }
#endif
#if GASNETC_OFI_HAS_MR_PROV_KEY_STATIC
  if (GASNETC_OFI_HAS_MR_PROV_KEY != has_mr_prov_key) {
      gasneti_fatalerror("The statically-determined value for GASNETC_OFI_HAS_MR_PROV_KEY=%i does\n"
                         "  not match the memory registration support that the (%s) provider reported.\n"
                         "  This could happen if a provider has changed behavior between versions.\n"
                         "  Use configure option --%s-ofi-mr-prov-key to correct this.",
                         GASNETC_OFI_HAS_MR_PROV_KEY,
                         info->fabric_attr->prov_name,
                         (has_mr_prov_key ? "enable" : "disable"));
  }
#endif
#if GASNET_SEGMENT_EVERYTHING
  if (info->domain_attr->mr_mode & FI_MR_ALLOCATED) {
      gasneti_fatalerror("GASNET_SEGMENT_EVERYTHING requires a provider which does NOT require FI_MR_ALLOCATED.  "
                         "You must pick either a different provider or a different GASNet segment mode.");
  }
#endif
#if GASNET_HAVE_MK_CLASS_MULTIPLE
  gasnetc_fi_hmem = !!(info->caps & FI_HMEM);
#endif

  use_av_map = (info->domain_attr->av_type == FI_AV_MAP);
#if GASNETC_OFI_USE_AV_MAP_STATIC
  if (GASNETC_OFI_USE_AV_MAP != use_av_map) {
      gasneti_fatalerror("The statically-determined value for GASNETC_OFI_USE_AV_MAP=%i does\n"
                         "  not match the address vector type that the (%s) provider reported.\n"
                         "  This could happen if a provider has changed behavior between versions.\n"
                         "  Use configure option --%s-ofi-av-map to correct this.",
                         GASNETC_OFI_USE_AV_MAP,
                         info->fabric_attr->prov_name,
                         (GASNETC_OFI_USE_AV_MAP ? "disable" : "enable"));
  }
#endif

  /* Open the fabric provider */
  ret = fi_fabric(info->fabric_attr, &gasnetc_ofi_fabricfd, NULL);
  GASNETC_OFI_CHECK_RET(ret, "fi_fabric failed");
  GASNETI_TRACE_PRINTF(I, ("Opened provider '%s' version %u.%u",
                           info->fabric_attr->prov_name,
                           (unsigned int)FI_MAJOR(info->fabric_attr->prov_version),
                           (unsigned int)FI_MINOR(info->fabric_attr->prov_version)));
  gasneti_assert(! strcmp(gasnetc_ofi_provider, info->fabric_attr->prov_name));

  /* Open a fabric access domain, also referred to as a resource domain */
  ret = fi_domain(gasnetc_ofi_fabricfd, info, &gasnetc_ofi_domainfd, NULL);
  GASNETC_OFI_CHECK_RET(ret, "fi_domain failed");
  GASNETI_TRACE_PRINTF(I, ("Opened domain '%s'", info->domain_attr->name));
  gasneti_assert(! strcmp(gasnetc_ofi_domain, info->domain_attr->name));

  if (gasneti_spawn_verbose) {
      gasneti_console_message("INFO", "provider '%s' version %u.%u, domain '%s', hostname '%s'",
                                      gasnetc_ofi_provider,
                                      (unsigned int)FI_MAJOR(info->fabric_attr->prov_version),
                                      (unsigned int)FI_MINOR(info->fabric_attr->prov_version),
                                      gasnetc_ofi_domain,
                                      gasneti_gethostname());
  }

  fi_freeinfo(info);

#if GASNETC_OFI_HAS_MR_VIRT_ADDR_STATIC
  GASNETI_TRACE_PRINTF(I, ("FI_MR_VIRT_ADDR support: "GASNETC_OFI_HAS_MR_VIRT_ADDR_CONFIGURE" (static)"));
#else
  GASNETI_TRACE_PRINTF(I, ("FI_MR_VIRT_ADDR support: %d (dynamic)", GASNETC_OFI_HAS_MR_VIRT_ADDR));
#endif
#if GASNETC_OFI_HAS_MR_PROV_KEY_STATIC
  GASNETI_TRACE_PRINTF(I, ("FI_MR_PROV_KEY support: "GASNETC_OFI_HAS_MR_PROV_KEY_CONFIGURE" (static)"));
#else
  GASNETI_TRACE_PRINTF(I, ("FI_MR_PROV_KEY support: %d (dynamic)", GASNETC_OFI_HAS_MR_PROV_KEY));
#endif
#if GASNET_HAVE_MK_CLASS_MULTIPLE
  GASNETI_TRACE_PRINTF(I, ("FI_HMEM support: %d (dynamic)", gasnetc_fi_hmem));
#else
  GASNETI_TRACE_PRINTF(I, ("FI_HMEM support: 0 (static)"));
#endif
#if GASNETC_OFI_USE_AV_MAP_STATIC
  GASNETI_TRACE_PRINTF(I, ("Use of FI_AV_MAP: "GASNETC_OFI_USE_AV_MAP_CONFIGURE" (static)"));
#else
  GASNETI_TRACE_PRINTF(I, ("Use of FI_AV_MAP: %d (dynamic)", GASNETC_OFI_USE_AV_MAP));
#endif

  // Now read user-provided environment settings
  gasnetc_ofi_read_env_vars(gasnetc_ofi_provider, gasnetc_ofi_domain);

  /* Allocate a new active endpoint for RDMA operations */
  hints->caps = FI_RMA;
#if GASNET_HAVE_MK_CLASS_MULTIPLE
  if (gasnetc_fi_hmem) {
    hints->caps |= FI_HMEM;
  }
#endif
  hints->tx_attr->op_flags = FI_DELIVERY_COMPLETE;

  // We do not support FI_CONTEXT for an RMA endpoint due to many-to-one iop.
  // However, we must set the bit as a work-around for psm2 provider in libfabric < 1.10 (bug 4567)
  int have_bug_4567 = using_psm_provider &&
                      (FI_VERSION(FI_MAJOR_VERSION, FI_MINOR_VERSION) < FI_VERSION(1, 10));
  hints->mode = have_bug_4567 ? FI_CONTEXT : 0;
  GASNETI_TRACE_PRINTF(I,("Work-around for bug 4567 is %sabled.", have_bug_4567?"en":"dis"));

  ret = fi_getinfo(OFI_CONDUIT_VERSION, NULL, NULL, 0ULL, hints, &gasnetc_rma_info);
  GASNETC_OFI_CHECK_RET(ret, "fi_getinfo() failed querying for RMA endpoint");

  // Sanity checks for bits we cannot support
  if (!have_bug_4567) GASNETC_PROHIBIT_MODE_BIT(gasnetc_rma_info, FI_CONTEXT, "RMA endpoints");
  GASNETC_PROHIBIT_MODE_BIT(gasnetc_rma_info, FI_CONTEXT2, "RMA endpoints");
  GASNETC_PROHIBIT_MODE_BIT(gasnetc_rma_info, FI_MSG_PREFIX, "RMA endpoints");
  GASNETC_PROHIBIT_MODE_BIT(gasnetc_rma_info, FI_RESTRICTED_COMP, "RMA endpoints");

  ret = fi_endpoint(gasnetc_ofi_domainfd, gasnetc_rma_info, &gasnetc_ofi_rdma_epfd, NULL);
  GASNETC_OFI_CHECK_RET(ret, "fi_endpoint for rdma failed");

  GASNETI_TRACE_PRINTF(I,("RMA EP: max_msg_size=%"PRIuSZ" inject_size=%"PRIuSZ" rma_iov_limit=%"PRIuSZ,
                          gasnetc_rma_info->ep_attr->max_msg_size,
                          gasnetc_rma_info->tx_attr->inject_size,
                          gasnetc_rma_info->tx_attr->rma_iov_limit));

  // Maximum size to use for fi_read() and fi_write() calls
  gasnetc_max_rma_size = gasnetc_rma_info->ep_attr->max_msg_size;
  gasneti_assert_always_uint( ofi_bbuf_size ,<=, gasnetc_max_rma_size );

  // Maximum size supported for an AMLong payload
  gasnetc_ofi_max_long = MIN(gasnetc_max_rma_size, 0x7fffffff);

  // Maximum size to use for RMA with FI_INJECT
  {
    const char *env_var = "GASNET_OFI_RMA_INJECT_LIMIT";
    uint64_t dflt = gasnetc_rma_info->tx_attr->inject_size;
    uint64_t value = gasneti_getenv_int_withdefault(env_var, dflt, 1);
    if (value > dflt) { // enforce dflt as the maximum
      if (!gasneti_mynode)  {
        gasneti_console_message("WARNING",
                                "%s reduced from the requested value %"PRIu64
                                " to the maximum supported value %"PRIu64,
                                env_var, value, dflt);
      }
      value = dflt;
    }
    max_buffered_write = value;
  }
  GASNETI_TRACE_PRINTF(I, ("Max buffered write size is %"PRIu64, max_buffered_write));
  gasneti_assert_always_uint( max_buffered_write ,<=, gasnetc_max_rma_size );

  /* Allocate a new active endpoint for AM operations buffer */
  hints->caps     = FI_MSG | FI_MULTI_RECV;
  hints->mode     = FI_CONTEXT;
  // FI_INJECT_COMPLETE is the better semantic fit for sending AMs.
  // However, we provide an undocumented means to restore the legacy
  // use of FI_DELIVERY_COMPLETE.
  if (gasneti_getenv_yesno_withdefault("GASNET_OFI_AM_USE_DELIVERY_COMPLETE", 0)) {
    hints->tx_attr->op_flags = FI_DELIVERY_COMPLETE;
  } else {
    hints->tx_attr->op_flags = FI_INJECT_COMPLETE;
  }

  ret = fi_getinfo(OFI_CONDUIT_VERSION, NULL, NULL, 0ULL, hints, &gasnetc_msg_info);
  GASNETC_OFI_CHECK_RET(ret, "fi_getinfo() failed querying for MSG endpoints");

  // Sanity checks for bits we cannot support
  GASNETC_PROHIBIT_MODE_BIT(gasnetc_msg_info, FI_CONTEXT2, "MSG endpoints");
  GASNETC_PROHIBIT_MODE_BIT(gasnetc_msg_info, FI_MSG_PREFIX, "MSG endpoints");
  GASNETC_PROHIBIT_MODE_BIT(gasnetc_msg_info, FI_RESTRICTED_COMP, "MSG endpoints");

  ret = fi_endpoint(gasnetc_ofi_domainfd, gasnetc_msg_info, &gasnetc_ofi_request_epfd, NULL);
  GASNETC_OFI_CHECK_RET(ret, "fi_endpoint for am request endpoint failed");

  ret = fi_endpoint(gasnetc_ofi_domainfd, gasnetc_msg_info, &gasnetc_ofi_reply_epfd, NULL);
  GASNETC_OFI_CHECK_RET(ret, "fi_endpoint for am reply endpoint failed");

  GASNETI_TRACE_PRINTF(I,("MSG EP: max_msg_size=%"PRIuSZ" inject_size=%"PRIuSZ" iov_limit=%"PRIuSZ,
                          gasnetc_msg_info->ep_attr->max_msg_size,
                          gasnetc_msg_info->tx_attr->inject_size,
                          gasnetc_msg_info->tx_attr->iov_limit));

  // Maximum size to use for fi_inject
  {
    const char *new_env_var = "GASNET_OFI_AM_INJECT_LIMIT";
    const char *old_env_var = "GASNET_OFI_INJECT_LIMIT";
    // Perfer the new variable name unless only the legacy one is set
    const char *env_var = (gasneti_getenv(new_env_var) || !gasneti_getenv(old_env_var))
                        ? new_env_var : old_env_var;
    uint64_t dflt = gasnetc_msg_info->tx_attr->inject_size;
    uint64_t value = gasneti_getenv_int_withdefault(env_var, dflt, 1);
    if (value > dflt) { // enforce dflt as the maximum
      if (!gasneti_mynode)  {
        gasneti_console_message("WARNING",
                                "%s reduced from the requested value %"PRIu64
                                " to the maximum supported value %"PRIu64,
                                env_var, value, dflt);
      }
      value = dflt;
    }
    max_buffered_send = value;
  }
  GASNETI_TRACE_PRINTF(I, ("Max buffered send size is %"PRIu64, max_buffered_send));

  // Allocate a CQ that will ideally be shared for both RDMA and AM tx ops
  memset(&tx_cq_attr, 0, sizeof(tx_cq_attr));
  tx_cq_attr.format    = FI_CQ_FORMAT_CONTEXT;
  tx_cq_attr.size      = tx_cq_size;
  tx_cq_attr.wait_obj  = FI_WAIT_NONE;
  ret = fi_cq_open(gasnetc_ofi_domainfd, &tx_cq_attr, &gasnetc_ofi_tx_cqfd, NULL);
  GASNETC_OFI_CHECK_RET(ret, "fi_cq_open for tx_cqfd failed");

  /* Allocate recv completion queues for AMs */
  memset(&rx_cq_attr, 0, sizeof(rx_cq_attr));
  rx_cq_attr.format    = FI_CQ_FORMAT_DATA;
  rx_cq_attr.size      = rx_cq_size;
  rx_cq_attr.wait_obj  = FI_WAIT_NONE;
  ret = fi_cq_open(gasnetc_ofi_domainfd, &rx_cq_attr, &gasnetc_ofi_request_cqfd, NULL);
  GASNETC_OFI_CHECK_RET(ret, "fi_cq_open for am request cq failed");

  ret = fi_cq_open(gasnetc_ofi_domainfd, &rx_cq_attr, &gasnetc_ofi_reply_cqfd, NULL);
  GASNETC_OFI_CHECK_RET(ret, "fi_cq_open for am reply cq failed");

  /* Bind CQs to endpoints */
  ret = fi_ep_bind(gasnetc_ofi_rdma_epfd, &gasnetc_ofi_tx_cqfd->fid, FI_TRANSMIT | FI_RECV);
  GASNETC_OFI_CHECK_RET(ret, "fi_ep_bind for tx_cq to rdma_epfd failed");

  ret = fi_ep_bind(gasnetc_ofi_request_epfd, &gasnetc_ofi_tx_cqfd->fid, FI_TRANSMIT);
#if GASNETC_OFI_USE_MULTI_CQ
  if (ret == -FI_EINVAL) { // Provider doesn't want to let us share CQ
    GASNETI_TRACE_PRINTF(I, ("Allocating distinct reqtx_cqfd"));
    ret = fi_cq_open(gasnetc_ofi_domainfd, &tx_cq_attr, &gasnetc_ofi_reqtx_cqfd, NULL);
    GASNETC_OFI_CHECK_RET(ret, "fi_cq_open for reqtx_cqfd failed");
    ret = fi_ep_bind(gasnetc_ofi_request_epfd, &gasnetc_ofi_reqtx_cqfd->fid, FI_TRANSMIT);
  }
#endif
  GASNETC_OFI_CHECK_RET(ret, "fi_ep_bind for tx_cq to am request CQ failed");

  ret = fi_ep_bind(gasnetc_ofi_reply_epfd, &gasnetc_ofi_tx_cqfd->fid, FI_TRANSMIT);
#if GASNETC_OFI_USE_MULTI_CQ
  if (ret == -FI_EINVAL) { // Provider doesn't want to let us share CQ
    GASNETI_TRACE_PRINTF(I, ("Allocating distinct reptx_cqfd"));
    ret = fi_cq_open(gasnetc_ofi_domainfd, &tx_cq_attr, &gasnetc_ofi_reptx_cqfd, NULL);
    GASNETC_OFI_CHECK_RET(ret, "fi_cq_open for reptx_cqfd failed");
    ret = fi_ep_bind(gasnetc_ofi_reply_epfd, &gasnetc_ofi_reptx_cqfd->fid, FI_TRANSMIT);
  }
#endif
  GASNETC_OFI_CHECK_RET(ret, "fi_ep_bind for tx_cq to am reply CQ failed");

  ret = fi_ep_bind(gasnetc_ofi_request_epfd, &gasnetc_ofi_request_cqfd->fid, FI_RECV);
  GASNETC_OFI_CHECK_RET(ret, "fi_ep_bind for am request cq to am_request_epfd failed");

  ret = fi_ep_bind(gasnetc_ofi_reply_epfd, &gasnetc_ofi_reply_cqfd->fid, FI_RECV);
  GASNETC_OFI_CHECK_RET(ret, "fi_ep_bind for am reply cq to am_reply_epfd failed");

  // Low-water mark for multi-receive buffer, if any
  if (maybe_multi_recv) {
    GASNETI_TRACE_PRINTF(I, ("Setting multi-recv low-water mark to %"PRIuSZ, min_multi_recv));
    optval = min_multi_recv;
    ret    = fi_setopt(&gasnetc_ofi_request_epfd->fid, FI_OPT_ENDPOINT, FI_OPT_MIN_MULTI_RECV,
                       &optval, sizeof(optval));
    GASNETC_OFI_CHECK_RET(ret, "fi_setopt for am request epfd failed");
    gasneti_assert_uint(optval ,==, min_multi_recv); // documented as IN
    ret    = fi_setopt(&gasnetc_ofi_reply_epfd->fid, FI_OPT_ENDPOINT, FI_OPT_MIN_MULTI_RECV,
                      &optval, sizeof(optval));
    GASNETC_OFI_CHECK_RET(ret, "fi_setopt for am reply epfd failed");
    gasneti_assert_uint(optval ,==, min_multi_recv); // documented as IN
  }

  ofi_setup_address_vector();

  /* Enable endpoints */
  ret = fi_enable(gasnetc_ofi_rdma_epfd);
  GASNETC_OFI_CHECK_RET(ret, "fi_enable for rdma failed");
  ret = fi_enable(gasnetc_ofi_request_epfd);
  GASNETC_OFI_CHECK_RET(ret, "fi_enable for am request ep failed");
  ret = fi_enable(gasnetc_ofi_reply_epfd);
  GASNETC_OFI_CHECK_RET(ret, "fi_enable for am reply ep failed");

  ofi_exchange_addresses();

  // Don't allow libfabric to free() our strings
  hints->domain_attr->name = NULL;
  hints->fabric_attr->prov_name = NULL;

  fi_freeinfo(hints);

  if (GASNETC_OFI_HAS_MR_PROV_KEY) {
    // 'static' ensures valgrind does not consider this allocation "possibly lost"
    static uint64_t **tmp;
    tmp = gasneti_calloc(1+GASNET_MAXEPS, sizeof(uint64_t*));
    gasnetc_remote_key_tbl = tmp + 1; // places aux seg keys at gasnetc_remote_key_tbl[-1]
  }

  receive_region_size = multirecv_buff_size*num_multirecv_buffs;
  receive_region_start = gasnetc_alloc_pages(receive_region_size, "for multi-recv buffers");
  metadata_array = gasneti_malloc(sizeof(gasnetc_ofi_recv_metadata_t)*num_multirecv_buffs);
  { char valstr[16];
    gasneti_format_number(multirecv_buff_size*num_multirecv_buffs, valstr, sizeof(valstr), 1);
    GASNETI_TRACE_PRINTF(I, ("Allocated %s for %"PRIuSZ " multi-recv buffers",
                              valstr, num_multirecv_buffs));
  }

  for(i = 0; i < num_multirecv_buffs; i++) {
        gasnetc_ofi_recv_metadata_t* metadata = metadata_array + i;
        metadata->iov.iov_base = ((char*)receive_region_start) + multirecv_buff_size*i;
        metadata->iov.iov_len = multirecv_buff_size;
        metadata->am_buff_msg.msg_iov = &metadata->iov;
        metadata->am_buff_msg.iov_count = 1;
        metadata->am_buff_msg.addr = FI_ADDR_UNSPEC;
        metadata->am_buff_msg.desc = NULL;
        metadata->am_buff_msg.context = gasnetc_recv_ctxt_to_op_ctxt(&metadata->am_buff_ctxt);
        metadata->am_buff_msg.data = 0;
        metadata->am_buff_ctxt.final_cntr = 0;
        metadata->am_buff_ctxt.event_cntr = 0;
        gasnetc_paratomic_set(&metadata->am_buff_ctxt.consumed_cntr, 0, 0);
        // Post multi-recv buffers for Active Messages
        struct fid_ep *epfd = (i % 2 == 0)
                            ? gasnetc_ofi_request_epfd
                            : gasnetc_ofi_reply_epfd;
        ret = fi_recvmsg(epfd, &metadata->am_buff_msg, maybe_multi_recv);

        GASNETC_OFI_CHECK_RET(ret, "fi_recvmsg failed");
    }

  
  /* Allocate bounce buffers*/
  bounce_region_size = GASNETI_PAGE_ALIGNUP(ofi_num_bbufs * ofi_bbuf_size);
  bounce_region_start = gasnetc_alloc_pages(bounce_region_size, "for bounce buffers");
  { char valstr[16];
    gasneti_format_number(bounce_region_size, valstr, sizeof(valstr), 1);
    GASNETI_TRACE_PRINTF(I, ("Allocated %s for %"PRIuSZ " bounce buffers",
                              valstr, ofi_num_bbufs));
  }

  /* Progress backwards so that when these buffers are added to the stack, they
   * will come off of it in order by address */
  char* buf = (char*)bounce_region_start + (ofi_num_bbufs-1)*ofi_bbuf_size;
  for (i = 0; i < (int)ofi_num_bbufs; i++) {
      gasneti_assert(buf);
      gasnetc_ofi_bounce_buf_t* container = gasneti_malloc(sizeof(gasnetc_ofi_bounce_buf_t));
      container->buf = buf;
      gasneti_lifo_push(&ofi_bbuf_pool, container);
      buf -= ofi_bbuf_size;
  }

  // Accounting to prevent dynamic over-allocation
  gasneti_semaphore_init(&num_unallocated_request_buffers, max_am_request_buffs - num_init_am_request_buffs, 0);
  gasneti_semaphore_init(&num_unallocated_reply_buffers, max_am_reply_buffs - num_init_am_reply_buffs, 0);

  size_t total_init = num_init_am_request_buffs + num_init_am_reply_buffs;
  am_buffers_region_size = GASNETI_PAGE_ALIGNUP(total_init*GASNETC_SIZEOF_AM_BUF_T);
  am_buffers_region_start = gasnetc_alloc_pages(am_buffers_region_size, "for AM send buffers");
  { char valstr[16];
    gasneti_format_number(am_buffers_region_size, valstr, sizeof(valstr), 1);
    GASNETI_TRACE_PRINTF(I, ("Allocated %s for %"PRIuSZ " (out of max %"PRIuSZ ") AM send buffers",
                              valstr, total_init, max_am_request_buffs + max_am_reply_buffs));
  }

  /* Add the buffers to the stack in reverse order to be friendly to the cache. */
  gasnetc_ofi_send_ctxt_t * bufp = (gasnetc_ofi_send_ctxt_t*)
      ((uintptr_t)am_buffers_region_start + GASNETC_SIZEOF_AM_BUF_T*(total_init - 1));

  GASNETC_STAT_EVENT_VAL(ALLOC_REQ_BUFF, num_init_am_request_buffs);
  for (i = 0; i < (int)num_init_am_request_buffs; i++) {
     bufp->pool = &ofi_am_request_pool;
     gasneti_lifo_push(bufp->pool, bufp);
     bufp = (gasnetc_ofi_send_ctxt_t*)((uintptr_t)bufp - GASNETC_SIZEOF_AM_BUF_T);
  }  

  GASNETC_STAT_EVENT_VAL(ALLOC_REP_BUFF, num_init_am_reply_buffs);
  for (i = 0; i < (int)num_init_am_reply_buffs; i++) {
      bufp->pool = &ofi_am_reply_pool;
      gasneti_lifo_push(bufp->pool, bufp);
      bufp = (gasnetc_ofi_send_ctxt_t*)((uintptr_t)bufp - GASNETC_SIZEOF_AM_BUF_T);
  }

  gasnetc_ofi_inited = 1;
  return GASNET_OK;
}

/*------------------------------------------------
 * OFI conduit exit function
 * ----------------------------------------------*/
void gasnetc_ofi_exit(void)
{
  if (!gasnetc_ofi_inited)
      return;

#if GASNET_PAR && GASNETC_OFI_USE_THREAD_DOMAIN
  // Attempt to obtain (and *never* release) the big_lock in bounded time
  const uint64_t timeout_ns = 10 * 1000000000L; // TODO: arbitrary 10s
  const gasneti_tick_t t_start = gasneti_ticks_now();
  GASNETI_SPIN_WHILE(EBUSY == GASNETC_OFI_TRYLOCK(&gasnetc_ofi_locks.big_lock), {
    if (timeout_ns < gasneti_ticks_to_ns(gasneti_ticks_now() - t_start)) return;
  });
#endif

    // (attempt to) cancel multi-recv operations for Active Messages
    for (int i = 0; i < num_multirecv_buffs; i++) {
        gasnetc_ofi_recv_ctxt_t *am_buff_ctxt = &metadata_array[i].am_buff_ctxt;
        struct fid_ep *epfd = (i % 2 == 0)
                            ? gasnetc_ofi_request_epfd
                            : gasnetc_ofi_reply_epfd;
        (void) fi_cancel(&epfd->fid, gasnetc_recv_ctxt_to_op_ctxt(am_buff_ctxt));
    }

  #if GASNETI_CLIENT_THREADS
    /* Unsafe to free resources if other threads may be using them */
  #else
    gasneti_free(metadata_array);
    gasneti_munmap(receive_region_start, receive_region_size);
    // TODO: when/if the following are proven safe
    //gasneti_munmap(bounce_region_start, bounce_region_size);
    //gasneti_munmap(am_buffers_region_start, am_buffers_region_size);
  #endif

  if(fi_close(&gasnetc_ofi_reply_epfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am reply epfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_request_epfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am request epfd failed\n");
  }

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
   { // TODO: loop over clients
     gasneti_Client_t i_client = gasneti_import_client(gasneti_THUNK_CLIENT);
     for (gex_EP_Index_t ep_idx = 0; ep_idx < GASNET_MAXEPS; ++ep_idx) {
       gasneti_EP_t i_ep = i_client->_ep_tbl[ep_idx];
       if (i_ep) gasnetc_ep_unbindsegment(i_ep);
     }
   }
#else
  if(gasnetc_segment_mrfd && (fi_close(&gasnetc_segment_mrfd->fid)!=FI_SUCCESS)) {
    gasneti_fatalerror("close mrfd failed\n");
  }
#endif

  if (gasnetc_auxseg_mrfd && (fi_close(&gasnetc_auxseg_mrfd->fid) != FI_SUCCESS)) {
    gasneti_fatalerror("close auxseg mrfd failed\n");
  }

  // This must follow closing MRs if bound due to FI_MR_ENDPOINT
  if(fi_close(&gasnetc_ofi_rdma_epfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close rdma epfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_tx_cqfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am tx_cqfd failed\n");
  }
#if GASNETC_OFI_USE_MULTI_CQ
  if(gasnetc_ofi_reqtx_cqfd && fi_close(&gasnetc_ofi_reqtx_cqfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am reqtx_cqfd failed\n");
  }
  if(gasnetc_ofi_reptx_cqfd && fi_close(&gasnetc_ofi_reptx_cqfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am reptx_cqfd failed\n");
  }
#endif

  if(fi_close(&gasnetc_ofi_reply_cqfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am reply cqfd failed\n");
  }
  if(fi_close(&gasnetc_ofi_request_cqfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close am request cqfd failed\n");
  }
  for (int i = 0; i < NUM_OFI_ENDPOINTS; ++i) {
    if(fi_close(&gasnetc_ofi_avfd[i]->fid)!=FI_SUCCESS) {
      gasneti_fatalerror("close av[%d] failed", i);
    }
    gasneti_free(gasnetc_addr_map[i]);
  }

  if(fi_close(&gasnetc_ofi_domainfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close domainfd failed\n");
  }

  if(fi_close(&gasnetc_ofi_fabricfd->fid)!=FI_SUCCESS) {
    gasneti_fatalerror("close fabricfd failed\n");
  }

  if (GASNETC_OFI_HAS_MR_PROV_KEY) {
    // table indices run [-1, GASNET_MAXEPS)
    for (int epidx = -1; epidx < GASNET_MAXEPS; ++epidx) {
      gasneti_free(gasnetc_remote_key_tbl[epidx]);
    }
    gasneti_free(gasnetc_remote_key_tbl - 1);
  }
}

/*------------------------------------------------
 * OFI conduit callback functions
 * ----------------------------------------------*/

/* Handle Active Messages */
GASNETI_INLINE(gasnetc_ofi_handle_am)
void gasnetc_ofi_handle_am(gasnetc_ofi_am_send_buf_t *header, int isreq, size_t msg_len, uint64_t cq_data)
{
    uint8_t *addr;
    int handler = header->handler;
    const gex_AM_Entry_t * const handler_entry = &gasnetc_handler[handler];
    gex_AM_Fn_t handler_fn = handler_entry->gex_fnptr;
    gex_AM_Arg_t *args;
    int numargs = header->argnum;
    int data_offset;
    gex_Token_t token = (gex_Token_t)header;
    size_t nbytes;
    gasneti_assert_uint(isreq ,==, header->isreq);
    switch(header->type) {
        case OFI_AM_SHORT:
            args = (gex_AM_Arg_t *)header->buf.short_buf.data;
            GASNETI_RUN_HANDLER_SHORT(isreq, handler, handler_fn, token, args, numargs);
            break;
        case OFI_AM_MEDIUM:
            data_offset = GASNETI_ALIGNUP(sizeof(gex_AM_Arg_t)*numargs, GASNETI_MEDBUF_ALIGNMENT);
            args = (gex_AM_Arg_t *)header->buf.medium_buf.data;
            addr = header->buf.medium_buf.data + data_offset;
            nbytes = msg_len - header->overhead;
            GASNETI_RUN_HANDLER_MEDIUM(isreq, handler, handler_fn, token, args, numargs, addr, nbytes);
            break;
        case OFI_AM_LONG:
            data_offset = sizeof(gex_AM_Arg_t)*numargs;
            args = (gex_AM_Arg_t *)header->buf.long_buf.data;
            addr = header->buf.long_buf.dest_ptr;
            nbytes = cq_data;
            GASNETI_RUN_HANDLER_LONG(isreq, handler, handler_fn, token, args, numargs, addr, nbytes);
            break;
        case OFI_AM_LONG_MEDIUM:
            data_offset = sizeof(gex_AM_Arg_t)*numargs;
            args = (gex_AM_Arg_t *)header->buf.long_buf.data;
            addr = header->buf.long_buf.dest_ptr;
            nbytes = cq_data;
            GASNETI_MEMCPY_SAFE_EMPTY(addr, header->buf.long_buf.data + data_offset, nbytes);
            GASNETI_RUN_HANDLER_LONG(isreq, handler, handler_fn, token, args, numargs, addr, nbytes);
            break;
        default:
            gasneti_unreachable_error(("undefined header type in gasnetc_ofi_handle_am: %d",
                                       header->type));
    }
}

// Handle completion of a simple blocking operation
void gasnetc_ofi_handle_blocking(void *op_context, unsigned int aux)
{
    gasnetc_ofi_blocking_op_ctxt_t *ptr = gasneti_container_of(op_context, gasnetc_ofi_blocking_op_ctxt_t, callback);
    ptr->complete = 1;
}

// Handle RDMA completion as the initiator
void gasnetc_ofi_handle_rdma(void *op_context, unsigned int aux)
{
    gasnetc_ofi_nb_op_ctxt_t *ptr = gasneti_container_of(op_context, gasnetc_ofi_nb_op_ctxt_t, callback);
    int alc = aux & GASNETC_CTXT_IS_ALC;

    switch (ptr->type) {
        case OFI_TYPE_EGET:
            {
                gasnete_eop_t *eop = gasneti_container_of(ptr, gasnete_eop_t, ofi);
                gasnete_eop_check(eop);
                GASNETE_EOP_MARKDONE(eop);
            }
            break;
        case OFI_TYPE_EPUT:
            {
                gasnete_eop_t *eop = gasneti_container_of(ptr, gasnete_eop_t, ofi);
                gasnete_eop_check(eop);
                if (alc) GASNETE_EOP_LC_FINISH(eop);
                GASNETE_EOP_MARKDONE(eop);
            }
            break;
        case OFI_TYPE_IGET:
            {
                gasnete_iop_t *iop = gasneti_container_of(ptr, gasnete_iop_t, get_ofi);
                gasnete_iop_check(iop);
                GASNETE_IOP_CNT_FINISH(iop, get, 1, GASNETI_ATOMIC_REL);
            }
            break;
        case OFI_TYPE_IPUT:
            {
                gasnete_iop_t *iop = gasneti_container_of(ptr, gasnete_iop_t, put_ofi);
                gasnete_iop_check(iop);
                if (alc) GASNETE_IOP_LC_FINISH(iop);
                GASNETE_IOP_CNT_FINISH(iop, put, 1, GASNETI_ATOMIC_NONE);
            }
            break;
        default:
            gasneti_unreachable_error(("undefined OP type in gasnetc_ofi_rdma_poll: %d",
                                       ptr->type));

    }
}

// Allocate an AM send buffer, spin-polling if necessary/allowed
// TODO: should Reply be permitted to borrow from Request pool?
GASNETI_INLINE(gasnetc_ofi_get_am_header)
gasnetc_ofi_send_ctxt_t *gasnetc_ofi_get_am_header(int isreq, gex_Flags_t flags GASNETI_THREAD_FARG)
{
    const gex_Flags_t imm = flags & GEX_FLAG_IMMEDIATE;
    gasneti_lifo_head_t* pool = isreq ? &ofi_am_request_pool
                                      : &ofi_am_reply_pool;

    gasnetc_ofi_send_ctxt_t *header = gasneti_lifo_pop(pool);
#if GASNETC_IMMEDIATE_AMPOLLS
    if (header) return header;
#else
    if (header || imm) return header;
#endif

    // Poll only the tx queue and retry the pool before (maybe) allocating another buffer
    gasnetc_ofi_tx_poll();
    header = gasneti_lifo_pop(pool);
    if (header) return header;

    // Allocate another unless doing so would exceed the max
    gasneti_semaphore_t* sema = isreq ? &num_unallocated_request_buffers
                                      : &num_unallocated_reply_buffers;
    if (gasneti_semaphore_trydown(sema)) {
        // TODO: cache-align and allocate more than one at a time
        header = gasneti_malloc(GASNETC_SIZEOF_AM_BUF_T);
        gasneti_leak(header);
        header->pool = pool;
        if (isreq) {
            GASNETC_STAT_EVENT_VAL(ALLOC_REQ_BUFF, 1);
        } else {
            GASNETC_STAT_EVENT_VAL(ALLOC_REP_BUFF, 1);
        }
        return header;
    }

#if GASNETC_IMMEDIATE_AMPOLLS
    if (imm) return NULL;
#else
    gasneti_assert(!imm);
#endif

    // Spin-poll until a buffer is free
    // These are "DOUNTIL" since already know buffer pool is empty
    if (isreq) {
        GASNETI_SPIN_DOUNTIL((header = gasneti_lifo_pop(pool)),
                             GASNETC_OFI_POLL_SELECTIVE(OFI_POLL_ALL));
    } else {
        GASNETI_SPIN_DOUNTIL((header = gasneti_lifo_pop(pool)),
                             GASNETC_OFI_POLL_SELECTIVE(OFI_POLL_REPLY));
    }

    return header;
}

// Release unused AM send buffer
#define gasnetc_ofi_free_am_header(header) \
        gasneti_lifo_push(header->pool, header)

// Process completed AM send
// TODO: Async LC handling goes here
void gasnetc_ofi_am_send_complete(gasnetc_ofi_send_ctxt_t *header)
{
    gasnetc_ofi_free_am_header(header);
}

void gasnetc_ofi_handle_bounce_rdma(void *op_context, unsigned int aux)
{
    gasnetc_ofi_bounce_op_ctxt_t *op = gasneti_container_of(op_context, gasnetc_ofi_bounce_op_ctxt_t, callback);

    if (gasnetc_paratomic_decrement_and_test(&op->cntr, 0)) {
        gasnetc_ofi_bounce_buf_t * bbuf_to_return;
        while (NULL != (bbuf_to_return = gasneti_lifo_pop(&op->bbuf_list)))
            gasneti_lifo_push(&ofi_bbuf_pool, bbuf_to_return);
        /* These completions will always be RDMA, so call the callback directly */
        gasnetc_ofi_handle_rdma(gasnetc_rdma_ctxt_to_op_ctxt(op->orig_op,0),0);
        gasneti_lifo_push(&ofi_bbuf_ctxt_pool, op);
    }
}

GASNETI_INLINE(gasnetc_ofi_get_bounce_ctxt)
gasnetc_ofi_bounce_op_ctxt_t* gasnetc_ofi_get_bounce_ctxt(void)
{
    gasnetc_ofi_bounce_op_ctxt_t* ctxt = gasneti_lifo_pop(&ofi_bbuf_ctxt_pool);
    if (NULL == ctxt) {
        ctxt = gasneti_calloc(1,sizeof(gasnetc_ofi_bounce_op_ctxt_t));
        ctxt->callback = gasnetc_ofi_handle_bounce_rdma;
        gasneti_lifo_init(&ctxt->bbuf_list);
        gasneti_leak(ctxt);
    }
    return ctxt;
}

/*------------------------------------------------
 * Pre-post or pin-down memory
 * ----------------------------------------------*/

// Local registration of segment memory
int gasnetc_ep_bindsegment(gasneti_EP_t i_ep, gasneti_Segment_t segment)
{
    GASNETI_TRACE_PRINTF(C,("Binding segment [%p, %p) to EP %d",
                            segment->_addr, segment->_ub, i_ep->_index));

    void *segbase;
    uintptr_t segsize;
    struct fid_mr** mrfd_p;
    gasnetc_EP_t c_ep = (gasnetc_EP_t)i_ep;

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    gasneti_assert(segment);
    segbase = segment->_addr;
    segsize = segment->_size;
    gasneti_assert(c_ep);
    mrfd_p = &c_ep->mrfd;
#else
    if (!segment) {
        segbase = (void *)0;
        segsize = UINT64_MAX;
        mrfd_p = &gasnetc_segment_mrfd;
    } else if (gasneti_i_segment_kind_is_host((gasneti_Segment_t) segment)) {
        // No additional host memory registration required
        return GASNET_OK;
    } else {
        gasneti_unreachable_error(("ofi-conduit does not yet support non-host memory kinds in GASNET_SEGMENT_EVERYTHING mode"));
    }
#endif

    // TBD: do we want local READ or WRITE in access?  will atomics need them, for instance?
    struct iovec iov = { segbase, segsize };
    uint64_t access = FI_REMOTE_READ | FI_REMOTE_WRITE;
    uint64_t key = GASNETC_EPIDX_TO_KEY(c_ep->_index);
    uint64_t flags = 0;
    struct fi_mr_attr attr = {0};
    attr.mr_iov        = &iov;
    attr.iov_count     = 1;
    attr.access        = access;
    attr.requested_key = key;
    attr.context       = NULL;
#if GASNET_HAVE_MK_CLASS_MULTIPLE
    gex_MK_Class_t mk_class;
    if (segment->_kind == GEX_MK_HOST) {
      gasneti_static_assert(FI_HMEM_SYSTEM == 0);
      mk_class = GEX_MK_CLASS_HOST;
      c_ep->device_only_segment = 0;
    } else {
      gasneti_MK_t i_mk = gasneti_import_mk(segment->_kind);
      mk_class = i_mk->_mk_class;
      switch (mk_class) {
        #if GASNET_HAVE_MK_CLASS_CUDA_UVA
        case GEX_MK_CLASS_CUDA_UVA:
          attr.iface = FI_HMEM_CUDA;
          attr.device.cuda = (int)(uintptr_t)i_mk->_mk_conduit;
          c_ep->device_only_segment = 1;
          #ifdef FI_HMEM_DEVICE_ONLY
            flags |= FI_HMEM_DEVICE_ONLY;
          #endif
          break;
        #endif

        #if GASNET_HAVE_MK_CLASS_HIP
        case GEX_MK_CLASS_HIP:
          attr.iface = FI_HMEM_ROCR;
          c_ep->device_only_segment = 0;
          break;
        #endif

        #if GASNET_HAVE_MK_CLASS_ZE
        case GEX_MK_CLASS_ZE:
          attr.iface = FI_HMEM_ZE;
          attr.device.ze = (int)(uintptr_t)i_mk->_mk_conduit;
          c_ep->device_only_segment = 1;
          #ifdef FI_HMEM_DEVICE_ONLY
            flags |= FI_HMEM_DEVICE_ONLY;
          #endif
          break;
        #endif

        default:
          gasneti_unreachable_error(("undefined or unsupported gex_MK_Class_t value: %d", mk_class));
          break;
      }
    }
#endif
#if GASNETC_HAVE_FI_MR_REG_ATTR
    const char *reg_fn = "fi_mr_regattr";
    int ret = fi_mr_regattr(gasnetc_ofi_domainfd, &attr, flags, mrfd_p);
#else
    const char *reg_fn = "fi_mr_reg";
    int ret = fi_mr_reg(gasnetc_ofi_domainfd,
                        attr.mr_iov->iov_base, attr.mr_iov->iov_len,
                        attr.access, attr.offset, attr.requested_key,
                        flags, mrfd_p, attr.context);
#endif
    if (ret) {
        if (gasneti_VerboseErrors) {
            gasneti_console_message("WARNING",
                                    "Unexpected error %d (%s) from %s() when binding segment [%p, %p) to EP %d",
                                    ret, fi_strerror(-ret), reg_fn, segment->_addr, segment->_ub, i_ep->_index);
          #if GASNET_HAVE_MK_CLASS_ZE
            if ((attr.iface == FI_HMEM_ZE) && (ret == -EFAULT) && (segsize > 32768)) {
              gasneti_console_message("NOTICE",
                                      "This failure looks like a known issue in ZE memory kinds support.  "
                                      "See docs/memory_kinds_implementation.md in the GASNet-EX sources for more information.");
            }
          #endif
        }
        // TODO: can we do better sorting out failure modes?
        return GASNET_ERR_RESOURCE;
    }
    if (segment && ! GASNETC_OFI_HAS_MR_PROV_KEY) {
      gasneti_assert_uint(key ,==, fi_mr_key(*mrfd_p));
    }

#ifdef FI_MR_ENDPOINT
    if (gasnetc_fi_mr_endpoint) {
      ret = fi_mr_bind(*mrfd_p, &gasnetc_ofi_rdma_epfd->fid, 0);
      GASNETC_OFI_CHECK_RET(ret, "fi_mr_bind failed");
      ret = fi_mr_enable(*mrfd_p);
      GASNETC_OFI_CHECK_RET(ret, "fi_mr_enable failed");
    }
#endif

    return GASNET_OK;
}

int gasnetc_ep_unbindsegment(gasneti_EP_t i_ep)
{
    gasneti_assert(i_ep);
    GASNETI_TRACE_PRINTF(C,("Unbinding segment from EP %d", i_ep->_index));

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    gasnetc_EP_t c_ep = (gasnetc_EP_t)i_ep;
    if (c_ep->mrfd) {
      int ret = fi_close(&c_ep->mrfd->fid);
      GASNETC_OFI_CHECK_RET(ret, "fi_close(ep->mrfd) failed");
      c_ep->mrfd = NULL;
      c_ep->_segment = NULL;
    }
#endif
    return GASNET_OK;
}

// Exchange memory keys with other nodes, if needed
void gasnetc_segment_exchange(gex_TM_t tm, gex_EP_t *eps, size_t num_eps)
{
  if (!GASNETC_OFI_HAS_MR_PROV_KEY) return;  // nothing to be done

  // Exchange a 64-bit mr key
  struct exchg_data {
    gex_EP_Location_t loc;
    uint64_t mr_key;
  } *local, *global, *p;

  size_t elem_sz = sizeof(struct exchg_data);
  local = gasneti_malloc(num_eps * elem_sz);

  // Pack
  p = local;
  for (gex_Rank_t i = 0; i < num_eps; ++i) {
    gasnetc_EP_t c_ep = (gasnetc_EP_t) gasneti_import_ep(eps[i]);
    if (! c_ep->_segment) continue;
    p->loc.gex_rank = gasneti_mynode;
    p->loc.gex_ep_index = c_ep->_index;
    gasneti_assert(c_ep->mrfd);
    p->mr_key = fi_mr_key(c_ep->mrfd);
    ++p;
  }

  size_t local_bytes = elem_sz * (p - local);
  size_t total_bytes = gasneti_blockingRotatedExchangeV(tm, local, local_bytes, (void**)&global, NULL);
  size_t total_eps = total_bytes / elem_sz;
  gasneti_free(local);

  // Unpack
  p = global;
  for (size_t i = 0; i < total_eps; ++i, ++p) {
    gex_EP_Index_t idx = p->loc.gex_ep_index;
    uint64_t *key_array = gasnetc_remote_key_tbl[idx];
    if_pf (!key_array) {
      static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;
      gasneti_mutex_lock(&lock);
      key_array = gasnetc_remote_key_tbl[idx];
      if (!key_array) {
        key_array = gasneti_calloc(gasneti_nodes, sizeof(uint64_t));
        gasneti_local_wmb();
        gasnetc_remote_key_tbl[idx] = key_array;
      }
      gasneti_mutex_unlock(&lock);
    } else {
      gasneti_local_rmb();
    }
    gex_Rank_t jobrank = p->loc.gex_rank;
    uint64_t key = p->mr_key;
    gasneti_assert(key_array[jobrank] == 0 || key_array[jobrank] == p->mr_key);
    key_array[jobrank] = p->mr_key;
  }
  gasneti_free(global);
}

void gasnetc_auxseg_register(gasnet_seginfo_t si)
{
  int ret = fi_mr_reg(gasnetc_ofi_domainfd, si.addr, si.size,
                      FI_REMOTE_READ | FI_REMOTE_WRITE,
                      0ULL, GASNETC_AUX_KEY, 0ULL,
                      &gasnetc_auxseg_mrfd, NULL);
  GASNETC_OFI_CHECK_RET(ret, "fi_mr_reg for aux_seg failed");

#ifdef FI_MR_ENDPOINT
  if (gasnetc_fi_mr_endpoint) {
    ret = fi_mr_bind(gasnetc_auxseg_mrfd, &gasnetc_ofi_rdma_epfd->fid, 0);
    GASNETC_OFI_CHECK_RET(ret, "fi_mr_bind failed for aux_seg");
    ret = fi_mr_enable(gasnetc_auxseg_mrfd);
    GASNETC_OFI_CHECK_RET(ret, "fi_mr_enable failed for aux_seg");
  }
#endif

  if (GASNETC_OFI_HAS_MR_PROV_KEY) {
    // Provider was not required to honor our key, so we exchange them
    uint64_t mr_key = fi_mr_key(gasnetc_auxseg_mrfd);
    uint64_t *aux_keys = gasneti_malloc(gasneti_nodes * sizeof(uint64_t));
    gasneti_bootstrapExchange(&mr_key, sizeof(mr_key), aux_keys);
    gasneti_assert(! gasnetc_remote_key_tbl[-1]);
    gasnetc_remote_key_tbl[-1] = aux_keys;
  } else {
    gasneti_assert_uint(GASNETC_AUX_KEY ,==, fi_mr_key(gasnetc_auxseg_mrfd));
  }
}

/*------------------------------------------------
 * OFI conduit network poll function
 * ----------------------------------------------*/

// TX progress function: Handles either AM or RDMA outgoing operations
// Returns the number of completions processed
GASNETI_INLINE(gasnetc_ofi_tx_poll_one)
int gasnetc_ofi_tx_poll_one(struct fid_cq* cqfd)
{
    struct fi_cq_entry re[GASNETC_OFI_NUM_COMPLETIONS];
    struct fi_cq_err_entry e;

    /* Read from Completion Queue */
    /* In the case of using one global lock, a try-lock could prevent progress from
     * occurring if the big lock is being held often by another thread. Just lock in
     * this case */
#if GASNETC_OFI_USE_THREAD_DOMAIN
    GASNETC_OFI_LOCK(&gasnetc_ofi_locks.tx_cq);
#else
    /* If another thread already has the queue lock, return as it is already
     * processing the queue */
    if(EBUSY == GASNETC_OFI_TRYLOCK(&gasnetc_ofi_locks.tx_cq)) return 0;
#endif
    int ret = fi_cq_read(cqfd, (void *)&re, GASNETC_OFI_NUM_COMPLETIONS);
    GASNETC_OFI_UNLOCK(&gasnetc_ofi_locks.tx_cq);
    if (ret != -FI_EAGAIN)
    {
        if_pf (ret < 0) {
            if (-FI_EAVAIL == ret) {
                GASNETC_OFI_LOCK_EXPR(&gasnetc_ofi_locks.tx_cq,
                   gasnetc_fi_cq_readerr(cqfd, &e ,0));
                if_pf (gasnetc_is_exit_error(e)) return 0;
                gasnetc_ofi_fatalerror("fi_cq_read for tx_poll failed with error", -e.err);
            } 
            else
                gasnetc_ofi_fatalerror("fi_cq_read for tx_poll returned unexpected error", ret);
        } 
        else {
            for (int i = 0; i < ret; i++) {
                void *op_context = re[i].op_context;
                if (! ((uintptr_t)op_context & GASNETC_CTXT_IS_RMA)) {
#if GASNET_DEBUG
                    gasnetc_paratomic_decrement(&pending_am, 0);
#endif
                    gasnetc_ofi_send_ctxt_t *header = gasnetc_op_ctxt_to_send_ctxt(op_context);
                    gasnetc_ofi_am_send_complete(header);
                }
                else {
#if GASNET_DEBUG
                    gasnetc_paratomic_decrement(&pending_rdma, 0);
#endif
                    uintptr_t aux = (uintptr_t)op_context & ~GASNETC_CTXT_MASK;
                    gasnetc_op_ctxt_run_rdma_callback(op_context, aux);
                }
            }
        }
        return ret;
    }

    gasneti_assert(ret == -FI_EAGAIN);
    return 0;
}

void gasnetc_ofi_tx_poll(void)
{
  int rc = gasnetc_ofi_tx_poll_one(gasnetc_ofi_tx_cqfd);
  GASNETI_TRACE_EVENT_VAL(X, CQ_READ_TX, rc);

#if GASNETC_OFI_USE_MULTI_CQ
  // TODO: use poll sets for providers/platforms which support them
  if (gasnetc_ofi_reqtx_cqfd) {
    rc = gasnetc_ofi_tx_poll_one(gasnetc_ofi_reqtx_cqfd);
    GASNETI_TRACE_EVENT_VAL(X, CQ_READ_REQTX, rc);
  }
  if (gasnetc_ofi_reptx_cqfd) {
    rc = gasnetc_ofi_tx_poll_one(gasnetc_ofi_reptx_cqfd);
    GASNETI_TRACE_EVENT_VAL(X, CQ_READ_REPTX, rc);
  }
#endif
}

GASNETI_INLINE(gasnetc_ofi_am_recv_poll)
void gasnetc_ofi_am_recv_poll(int is_request)
{
#if GASNETC_OFI_RETRY_RECVMSG
    static gasnetc_ofi_recv_ctxt_t *buffs_to_retry[2] = { NULL, NULL };
#endif
    struct fid_ep * ep;
    struct fid_cq * cq;
    gasneti_atomic_t * lock_p;
    if (is_request) {
        ep = gasnetc_ofi_request_epfd;
        cq = gasnetc_ofi_request_cqfd;
#if GASNET_PAR && !GASNETC_OFI_USE_THREAD_DOMAIN
        lock_p = &gasnetc_ofi_locks.rx_request_cq;
#endif
    }
    else {
        ep = gasnetc_ofi_reply_epfd;
        cq = gasnetc_ofi_reply_cqfd;
#if GASNET_PAR && !GASNETC_OFI_USE_THREAD_DOMAIN
        lock_p = &gasnetc_ofi_locks.rx_reply_cq;
#endif
    }

    int count;
    for (count = 0; count < GASNETC_OFI_EVENTS_PER_POLL; ++count) {
        if(EBUSY == GASNETC_OFI_PAR_TRYLOCK(lock_p)) goto out;

        /* Read from Completion Queue */
        struct fi_cq_data_entry re = {0};
        int ret = fi_cq_read(cq, (void *)&re, 1);

        if (ret == -FI_EAGAIN) {
            GASNETC_OFI_PAR_UNLOCK(lock_p);
            goto out;
        } 
        if_pf (ret < 0) {
            struct fi_cq_err_entry e = {0};
            gasnetc_fi_cq_readerr(cq, &e ,0);
            GASNETC_OFI_PAR_UNLOCK(lock_p);
            if_pf (gasnetc_is_exit_error(e)) goto out;
            gasnetc_ofi_fatalerror("fi_cq_read for am_recv_poll failed with error", -e.err);
        }

        gasnetc_ofi_recv_ctxt_t *header = gasnetc_op_ctxt_to_recv_ctxt(re.op_context);
        gasnetc_ofi_recv_metadata_t* metadata =
                    gasneti_container_of(header, gasnetc_ofi_recv_metadata_t, am_buff_ctxt);

        /* Count number of completions read for this posted buffer */
        header->event_cntr++;

#if GASNET_TRACE
        {
          int buffer_num = metadata - metadata_array;
          uint64_t event_num = header->event_cntr - 1;
          uintptr_t offset = (uintptr_t)re.buf - (uintptr_t)metadata->iov.iov_base;
          GASNETI_TRACE_PRINTF(D,("fi_cq_read(%s) %d:%"PRIu64"%s%s",
                                   is_request?"req":"rep", buffer_num, event_num,
                                   (re.flags & FI_RECV)?gasneti_dynsprintf(" RECV@%"PRIuPTR"+%"PRIuSZ,
                                                                           offset, re.len)
                                                       :"",
                                   (re.flags & FI_MULTI_RECV)?" UNLINK":""));
        }
#endif

        /* Record the total number of completions read */
        if_pf (re.flags & FI_MULTI_RECV) {
            header->final_cntr = header->event_cntr;
        }
        GASNETC_OFI_PAR_UNLOCK(lock_p);

        if_pt (re.flags & FI_RECV) {
            void *buf = maybe_multi_recv ? re.buf : metadata->iov.iov_base;
            // re.data contains the payload length for a Long
            gasnetc_ofi_handle_am(buf, is_request, re.len, re.data);
        }

        // Repost if either not using FI_MULTI_RECV
        // OR matched "final" and "consumed" counters indicate last AM handler has completed
        if (!maybe_multi_recv ||
            ((GASNETI_ATOMIC_MAX & header->final_cntr) ==
             (uint64_t) gasnetc_paratomic_add(&header->consumed_cntr, 1, GASNETI_ATOMIC_ACQ))) {
            struct fi_msg* am_buff_msg = &metadata->am_buff_msg;
            GASNETC_OFI_LOCK(&gasnetc_ofi_locks.am_rx);
            int post_ret = fi_recvmsg(ep, am_buff_msg, maybe_multi_recv);
            GASNETC_OFI_UNLOCK(&gasnetc_ofi_locks.am_rx);
#if GASNETC_OFI_RETRY_RECVMSG
            if_pf (post_ret == -FI_EAGAIN) {
                GASNETC_OFI_PAR_LOCK(lock_p);
                header->next = buffs_to_retry[is_request];
                buffs_to_retry[is_request] = header;
                post_ret = FI_SUCCESS;
                if (is_request) {
                    GASNETI_TRACE_EVENT(C, RECVMSG_REQ_EAGAIN);
                } else {
                    GASNETI_TRACE_EVENT(C, RECVMSG_REP_EAGAIN);
                }
                GASNETC_OFI_PAR_UNLOCK(lock_p);
            }
#endif
            GASNETC_OFI_CHECK_RET(post_ret, "fi_recvmsg failed inside am_recv_poll");
            if (is_request) {
                GASNETI_TRACE_EVENT(C, RECVMSG_REQ);
            } else {
                GASNETI_TRACE_EVENT(C, RECVMSG_REP);
            }
        }
    }

#if GASNETC_OFI_RETRY_RECVMSG
    if_pf (buffs_to_retry[is_request]) {
        GASNETC_OFI_PAR_LOCK(lock_p);
        gasnetc_ofi_recv_ctxt_t **prev_p = &buffs_to_retry[is_request];
        gasnetc_ofi_recv_ctxt_t *curr = *prev_p;
        while (curr) {
            gasnetc_ofi_recv_ctxt_t *next = curr->next;
            gasnetc_ofi_recv_metadata_t* metadata =
                    gasneti_container_of(curr, gasnetc_ofi_recv_metadata_t, am_buff_ctxt);
            struct fi_msg* am_buff_msg = &metadata->am_buff_msg;
        #if GASNET_PAR && GASNETC_OFI_USE_THREAD_DOMAIN // avoid recursive acquire of big_lock
            int post_ret = fi_recvmsg(ep, am_buff_msg, maybe_multi_recv);
        #else
            GASNETC_OFI_LOCK(&gasnetc_ofi_locks.am_rx);
            int post_ret = fi_recvmsg(ep, am_buff_msg, maybe_multi_recv);
            GASNETC_OFI_UNLOCK(&gasnetc_ofi_locks.am_rx);
        #endif
            if (post_ret == -FI_EAGAIN) {
                prev_p = &curr->next; // retain curr in the list
            } else {
                GASNETC_OFI_CHECK_RET(post_ret, "deferred fi_recvmsg failed");
                if (is_request) {
                    GASNETI_TRACE_EVENT(C, RECVMSG_REQ_REPOST);
                } else {
                    GASNETI_TRACE_EVENT(C, RECVMSG_REP_REPOST);
                }
                *prev_p = next; // remove curr from the list
            }
            curr = next;
        }
        GASNETC_OFI_PAR_UNLOCK(lock_p);
    }
#endif

out:
    if (is_request) {
        GASNETI_TRACE_EVENT_VAL(X, CQ_READ_REQ, count);
    } else {
        GASNETI_TRACE_EVENT_VAL(X, CQ_READ_REP, count);
    }
}

/* General progress function */
void gasnetc_ofi_poll(void)
{
    gasnetc_ofi_tx_poll();
    gasnetc_ofi_am_recv_poll(1); /* requests */
    gasnetc_ofi_am_recv_poll(0); /* replies */
}

/*------------------------------------------------
 * OFI conduit am send functions
 * ----------------------------------------------*/

int gasnetc_ofi_am_send_short(gex_Rank_t dest, gex_AM_Index_t handler,
                     int numargs, va_list argptr, int isreq, gex_Flags_t flags GASNETI_THREAD_FARG)
{
    // Get a send buffer
    gasnetc_ofi_send_ctxt_t *header = gasnetc_ofi_get_am_header(isreq, flags GASNETI_THREAD_PASS);
    if (!header) {
        gasneti_assert(flags & GEX_FLAG_IMMEDIATE);
        return 1;
    }
    gasnetc_ofi_am_send_buf_t *sendbuf = &header->sendbuf;

    int ret = 0;
    struct fid_ep* ep;
    fi_addr_t am_dest;
    int poll_type;
    if (isreq) {
        ep = gasnetc_ofi_request_epfd;
        am_dest = gasnetc_fabric_addr(REQ, dest);
        poll_type = OFI_POLL_ALL;
    } 
    else {
        ep = gasnetc_ofi_reply_epfd;
        am_dest = gasnetc_fabric_addr(REP, dest);
        poll_type = OFI_POLL_REPLY;
    }

    size_t len = sizeof(gex_AM_Arg_t) * numargs + offsetof(gasnetc_ofi_am_send_buf_t, buf.short_buf);
    len = GASNETI_ALIGNUP(len, GASNETI_MEDBUF_ALIGNMENT); // ensure multi-recv buffer alignment

    // Initialize metadata (handler, args, etc.)
    sendbuf->handler = (uint8_t) handler;
    sendbuf->sourceid = gasneti_mynode;
    sendbuf->type = OFI_AM_SHORT;
    sendbuf->argnum = numargs;
    sendbuf->isreq = isreq;
    gex_AM_Arg_t *arglist = (gex_AM_Arg_t*) sendbuf->buf.short_buf.data;
    for (int i = 0 ; i < numargs ; ++i) {
        arglist[i] = va_arg(argptr, gex_AM_Arg_t);
    }

    // Send
    if(len <= max_buffered_send) {
        OFI_INJECT_RETRY_IMM(&gasnetc_ofi_locks.am_tx,
                             ret = fi_inject(ep, sendbuf, len, am_dest),
                             poll_type, flags & GEX_FLAG_IMMEDIATE, out_imm);
        GASNETC_OFI_CHECK_RET(ret, "fi_inject for short am failed");
        gasnetc_ofi_free_am_header(header);
    } else {
        struct fi_context *op_cxtx = gasnetc_send_ctxt_to_op_ctxt(header);
        OFI_INJECT_RETRY_IMM(&gasnetc_ofi_locks.am_tx,
                             ret = fi_send(ep, sendbuf, len, NULL, am_dest, op_cxtx),
                             poll_type, flags & GEX_FLAG_IMMEDIATE, out_imm);
        GASNETC_OFI_CHECK_RET(ret, "fi_send for short am failed");
#if GASNET_DEBUG
        gasnetc_paratomic_increment(&pending_am,0);
#endif
    }
    return 0;

out_imm:
    gasnetc_ofi_free_am_header(header);
    return 1;
}

GASNETI_INLINE(gasnetc_medium_prep)
gasnetc_ofi_send_ctxt_t *gasnetc_medium_prep(
                int numargs,
                int isreq,
                gex_Flags_t flags
                GASNETI_THREAD_FARG)
{
    // Get a send buffer
    gasnetc_ofi_send_ctxt_t *header = gasnetc_ofi_get_am_header(isreq, flags GASNETI_THREAD_PASS);
    if (!header) {
        gasneti_assert(flags & GEX_FLAG_IMMEDIATE);
        goto out_imm;
    }

    // Initialize available metadata
    gasnetc_ofi_am_send_buf_t *sendbuf = &header->sendbuf;
    sendbuf->sourceid = gasneti_mynode;
    sendbuf->type = OFI_AM_MEDIUM;
    sendbuf->isreq = isreq;
    sendbuf->argnum = numargs;

out_imm:
    return header;
}

GASNETI_INLINE(gasnetc_medium_commit)
int gasnetc_medium_commit(
                gasnetc_ofi_send_ctxt_t *header, const int fixed,
                gex_Rank_t dest, gex_AM_Index_t handler,
                const void *client_buf, size_t nbytes,   /* data payload */
                unsigned int numargs, va_list argptr, int isreq,
                gex_Flags_t flags GASNETI_THREAD_FARG)
{
    gasnetc_ofi_am_send_buf_t *sendbuf = &header->sendbuf;

    size_t args_len = GASNETI_ALIGNUP(sizeof(gex_AM_Arg_t)*numargs, GASNETI_MEDBUF_ALIGNMENT);
    if (fixed || client_buf) {
      GASNETI_MEMCPY_SAFE_EMPTY((uint8_t *)(sendbuf->buf.medium_buf.data) + args_len, client_buf, nbytes);
    }

    size_t len = offsetof(gasnetc_ofi_am_send_buf_t, buf.medium_buf) + args_len + nbytes;
    len = GASNETI_ALIGNUP(len, GASNETI_MEDBUF_ALIGNMENT); // ensure multi-recv buffer alignment

    // Remainder of metadata (handler, args, nbytes)
    sendbuf->handler = (uint8_t) handler;
    gex_AM_Arg_t *arglist = (gex_AM_Arg_t*) sendbuf->buf.medium_buf.data;
    for (int i = 0 ; i < numargs ; ++i) {
        arglist[i] = va_arg(argptr, gex_AM_Arg_t);
    }

    // Enable reconstruction of nbytes from message length
    size_t overhead = len - nbytes;
    sendbuf->overhead = overhead;
    gasneti_assert_uint(overhead ,<, 256);

    struct fid_ep* ep;
    fi_addr_t am_dest;
    int poll_type;
    if (isreq) {
        ep = gasnetc_ofi_request_epfd;
        am_dest = gasnetc_fabric_addr(REQ, dest);
        poll_type = OFI_POLL_ALL;
    }
    else {
        ep = gasnetc_ofi_reply_epfd;
        am_dest = gasnetc_fabric_addr(REP, dest);
        poll_type = OFI_POLL_REPLY;
    }

    // Send
    int ret;
    if(len <= max_buffered_send) {
        OFI_INJECT_RETRY_IMM(&gasnetc_ofi_locks.am_tx,
                             ret = fi_inject(ep, sendbuf, len, am_dest),
                             poll_type, flags & GEX_FLAG_IMMEDIATE, out_imm);
        GASNETC_OFI_CHECK_RET(ret, "fi_inject for medium am failed");
        gasnetc_ofi_free_am_header(header);
    } else {
        struct fi_context *op_cxtx = gasnetc_send_ctxt_to_op_ctxt(header);
        OFI_INJECT_RETRY_IMM(&gasnetc_ofi_locks.am_tx,
                             ret = fi_send(ep, sendbuf, len, NULL, am_dest, op_cxtx),
                             poll_type, flags & GEX_FLAG_IMMEDIATE, out_imm);
        GASNETC_OFI_CHECK_RET(ret, "fi_send for medium am failed");
#if GASNET_DEBUG
        gasnetc_paratomic_increment(&pending_am,0);
#endif
    }

    return 0;

out_imm:
    gasneti_assert(flags & GEX_FLAG_IMMEDIATE);
    gasnetc_ofi_free_am_header(header);
    return 1;
}

int gasnetc_ofi_am_send_medium(gex_Rank_t dest, gex_AM_Index_t handler, 
                     void *source_addr, size_t nbytes,   /* data payload */
                     int numargs, va_list argptr, int isreq, gex_Flags_t flags GASNETI_THREAD_FARG)
{
    gasnetc_ofi_send_ctxt_t *header;
    header = gasnetc_medium_prep(numargs, isreq, flags GASNETI_THREAD_PASS);
    if (!header) return 1;
    gasneti_assume((source_addr != NULL) || !nbytes);
    return gasnetc_medium_commit(header, /*fixed*/1, dest, handler, source_addr, nbytes,
                                 numargs, argptr, isreq, flags GASNETI_THREAD_PASS);
}


#if GASNET_NATIVE_NP_ALLOC_REQ_MEDIUM || GASNET_NATIVE_NP_ALLOC_REP_MEDIUM

extern gasneti_AM_SrcDesc_t gasnetc_ofi_PrepareMedium(
                        gasneti_AM_SrcDesc_t  sd,
                        int                   isreq,
                        gex_Rank_t            jobrank,
                        const void           *client_buf,
                        size_t                size,
                        gex_Flags_t           flags,
                        unsigned int          numargs
                        GASNETI_THREAD_FARG)
{
    const gex_Flags_t immediate = flags & GEX_FLAG_IMMEDIATE;
    gasnetc_ofi_send_ctxt_t *header =
        gasnetc_medium_prep(numargs, isreq, /*flags*/immediate GASNETI_THREAD_PASS);
    if (!header) goto out_immediate;

    sd->_void_p = header;
    sd->_is_nbrhd = 0;
    sd->_dest._request._rank = jobrank; // yes, same for request and reply paths
    sd->_size = size;

    if (client_buf) {
        sd->_addr = (/*non-const*/ void *)client_buf;
    } else {
        size_t args_len = GASNETI_ALIGNUP(sizeof(gex_AM_Arg_t)*numargs, GASNETI_MEDBUF_ALIGNMENT);
        sd->_addr = sd->_gex_buf =
            (void *)((uintptr_t)&header->sendbuf.buf.medium_buf.data + args_len);
        gasneti_init_sd_poison(sd);
    }

    return sd;

out_immediate:
    gasneti_assert(immediate);
    gasneti_reset_srcdesc(sd);
    return NULL;
}

extern void gasnetc_ofi_CommitMedium(
                gasneti_AM_SrcDesc_t sd,
                int isreq,
                gex_AM_Index_t handler,
                size_t nbytes,
                va_list argptr
                GASNETI_THREAD_FARG)
{
    gasnetc_ofi_send_ctxt_t *header = (gasnetc_ofi_send_ctxt_t *)sd->_void_p;
    unsigned int numargs = header->sendbuf.argnum;
    gex_Rank_t jobrank = sd->_dest._request._rank;
    const void *source_addr = sd->_gex_buf ? NULL : sd->_addr;
    gasneti_assert_zeroret(
        gasnetc_medium_commit(header, /*fixed*/0, jobrank, handler,
                              source_addr, nbytes,
                              numargs, argptr, isreq, /*flags*/0
                              GASNETI_THREAD_PASS));
}

#endif // GASNET_NATIVE_NP_ALLOC_REQ_MEDIUM || GASNET_NATIVE_NP_ALLOC_REP_MEDIUM

int gasnetc_ofi_am_send_long(gex_Rank_t dest, gex_AM_Index_t handler,
                       void *source_addr, size_t nbytes,   /* data payload */
                       void *dest_addr,
                       int numargs, va_list argptr, int isreq, gex_Flags_t flags
                       GASNETI_THREAD_FARG)
{
    // Get a send buffer
    gasnetc_ofi_send_ctxt_t *header = gasnetc_ofi_get_am_header(isreq, flags GASNETI_THREAD_PASS);
    if (!header) {
        gasneti_assert(flags & GEX_FLAG_IMMEDIATE);
        return 1;
    }
    gasnetc_ofi_am_send_buf_t *sendbuf = &header->sendbuf;

    int ret = 0;
    struct fid_ep* ep;
    fi_addr_t am_dest;
    int poll_type;
    if (isreq) {
        ep = gasnetc_ofi_request_epfd;
        am_dest = gasnetc_fabric_addr(REQ, dest);
        poll_type = OFI_POLL_ALL;
    } 
    else {
        ep = gasnetc_ofi_reply_epfd;
        am_dest = gasnetc_fabric_addr(REP, dest);
        poll_type = OFI_POLL_REPLY;
    }

    // Copy or Put the payload
    size_t len = sizeof(gex_AM_Arg_t)*numargs;
    if(len + nbytes < long_rma_threshold) {
        // Pack the payload if it's small enough
        GASNETI_MEMCPY_SAFE_EMPTY(sendbuf->buf.long_buf.data + len, source_addr, nbytes);
        len += nbytes;
        sendbuf->type = OFI_AM_LONG_MEDIUM;
    } else if (flags & GEX_FLAG_IMMEDIATE) {
        // As long as we are stalling for remote completion of a Put, must "fail" IMMEDIATE
        goto out_imm;
    } else {
        // Launch the long data payload transfer with RMA operation
        gasnetc_ofi_blocking_op_ctxt_t lam_ctxt;
        lam_ctxt.complete = 0;
        lam_ctxt.callback = gasnetc_ofi_handle_blocking;

        // TODO: following line is only correct until AM supported on non-primordial EP
        const int rem_epidx = gasnetc_in_auxseg(dest, dest_addr) ? -1 : 0;
        gasnetc_EP_t c_ep = (gasnetc_EP_t)gasneti_import_ep(gasneti_THUNK_EP);
        OFI_INJECT_RETRY(&gasnetc_ofi_locks.rdma_tx,
                         OFI_WRITE(c_ep, source_addr, nbytes,
                                   dest, rem_epidx, dest_addr, &lam_ctxt, 0),
                         poll_type);
        GASNETC_OFI_CHECK_RET(ret, "fi_write failed for AM long");
#if GASNET_DEBUG
        gasnetc_paratomic_increment(&pending_rdma,0);
#endif

        /* Because the order is not guaranteed between different ep, */
        /* we send the am part after confirming the large rdma operation */
        /* is successful. */
        GASNETI_SPIN_WHILE(!lam_ctxt.complete,
                           GASNETC_OFI_POLL_SELECTIVE(poll_type));
        sendbuf->type = OFI_AM_LONG;
    }
    len += offsetof(gasnetc_ofi_am_send_buf_t, buf.long_buf.data);
    len = GASNETI_ALIGNUP(len, GASNETI_MEDBUF_ALIGNMENT); // ensure multi-recv buffer alignment

    // Initialize metadata (handler, args, etc.)
    sendbuf->handler = (uint8_t) handler;
    sendbuf->sourceid = gasneti_mynode;
    sendbuf->argnum = numargs;
    sendbuf->isreq = isreq;
    sendbuf->buf.long_buf.dest_ptr = dest_addr;
    gex_AM_Arg_t *arglist = (gex_AM_Arg_t*) sendbuf->buf.long_buf.data;
    for (int i = 0 ; i < numargs ; ++i) {
        arglist[i] = va_arg(argptr, gex_AM_Arg_t);
    }

    if(len <= max_buffered_send) {
        OFI_INJECT_RETRY_IMM(&gasnetc_ofi_locks.am_tx,
                             ret = fi_injectdata(ep, sendbuf, len, nbytes, am_dest),
                             poll_type, flags & GEX_FLAG_IMMEDIATE, out_imm);
        GASNETC_OFI_CHECK_RET(ret, "fi_inject for long am failed");
        gasnetc_ofi_free_am_header(header);
    } else {
        struct fi_context *op_cxtx = gasnetc_send_ctxt_to_op_ctxt(header);
        OFI_INJECT_RETRY_IMM(&gasnetc_ofi_locks.am_tx,
                             ret = fi_senddata(ep, sendbuf, len, NULL, nbytes, am_dest, op_cxtx),
                             poll_type, flags & GEX_FLAG_IMMEDIATE, out_imm);
        GASNETC_OFI_CHECK_RET(ret, "fi_send for long am failed");
#if GASNET_DEBUG
        gasnetc_paratomic_increment(&pending_am,0);
#endif
    }

    return 0;

out_imm:
    gasnetc_ofi_free_am_header(header);
    return 1;
}

GASNETI_INLINE(get_bounce_bufs)
int get_bounce_bufs(int n, gasnetc_ofi_bounce_buf_t ** arr) {
    int i,j;
    gasnetc_ofi_bounce_buf_t* buf_container;

    for (i = 0; i < n; i++) {
        buf_container = gasneti_lifo_pop(&ofi_bbuf_pool);
        if (!buf_container) {
            for (j = i -1; j >= 0; j--) {
                gasneti_lifo_push(&ofi_bbuf_pool, arr[j]);
            }
            return 0;
        }
        arr[i] = buf_container;
    }
    return 1;
}

/*------------------------------------------------
 * OFI conduit one-sided put/get functions
 * ----------------------------------------------*/

/* There is not a good semantic match between GASNet and OFI in the non-blocking,
 * non-bulk puts due to local completion requirements. This function handles this 
 * special case. 
 *
 * Returns a valid event if necessary to block for remote completion.
 * Otherwise returns GEX_EVENT_INVALID
 */
gex_Event_t
gasnetc_rdma_put_non_bulk(gex_TM_t tm, gex_Rank_t rank, void* dest_addr, void* src_addr, 
        size_t nbytes, gasnetc_ofi_nb_op_ctxt_t* ctxt_ptr, gex_Flags_t flags GASNETI_THREAD_FARG)
{
    const gex_EP_Location_t loc = gasneti_e_tm_rank_to_location(tm, rank, 0);
    const gex_Rank_t jobrank = loc.gex_rank;
    const int rem_epidx = gasnetc_in_auxseg(jobrank, dest_addr) ? -1 : loc.gex_ep_index;
    gasnetc_EP_t c_ep = (gasnetc_EP_t)gasneti_e_tm_to_i_ep(tm);
    int i;
    int ret = FI_SUCCESS;
    uintptr_t src_ptr = (uintptr_t)src_addr;

    gasnetc_assert_callback_eq(ctxt_ptr, gasnetc_ofi_handle_rdma);

    PERIODIC_RMA_POLL();

#if GASNET_HAVE_MK_CLASS_CUDA_UVA || GASNET_HAVE_MK_CLASS_ZE
    // CUDA and ZE device memory preclude bounce buffers and (at least currently) use of FI_INJECT
    if (c_ep->device_only_segment) goto block_anyways;
#endif

    /* The payload can be injected without need for a bounce buffer */
    if (nbytes <= max_buffered_write) {
        struct fi_msg_rma msg;
        msg.desc = 0;
        msg.addr = gasnetc_fabric_addr(RDMA, jobrank);
        struct iovec iovec;
        struct fi_rma_iov rma_iov;
        iovec.iov_base = src_addr;
        iovec.iov_len = nbytes;

        rma_iov.addr = gasnetc_remote_addr(jobrank, dest_addr, rem_epidx);
        rma_iov.key = gasnetc_remote_key(jobrank, rem_epidx);
        rma_iov.len = nbytes;

        msg.context = gasnetc_rdma_ctxt_to_op_ctxt(ctxt_ptr,GASNETC_CTXT_IS_RMA);
        msg.msg_iov = &iovec;
        msg.iov_count = 1;
        msg.rma_iov = &rma_iov;
        msg.rma_iov_count = 1;
        
        OFI_INJECT_RETRY_IMM(&gasnetc_ofi_locks.rdma_tx,
                             ret = fi_writemsg(gasnetc_ofi_rdma_epfd, &msg, FI_INJECT | FI_DELIVERY_COMPLETE),
                             OFI_POLL_ALL, flags & GEX_FLAG_IMMEDIATE, out_imm_inject);
        GASNETC_OFI_CHECK_RET(ret, "fi_writemsg with FI_INJECT failed");

#if GASNET_DEBUG
        gasnetc_paratomic_increment(&pending_rdma,0);
#endif
        GASNETC_STAT_EVENT(NB_PUT_INJECT);
        return GEX_EVENT_INVALID;

out_imm_inject:
        return GEX_EVENT_NO_OP;
    } 
    /* Bounce buffers are needed */
    else if (nbytes <= gasnetc_ofi_bbuf_threshold) {
        uintptr_t dest_ptr = (uintptr_t)dest_addr;
        int num_bufs_needed = (nbytes + ofi_bbuf_size - 1) / ofi_bbuf_size;
        size_t bytes_to_copy;
        gasnetc_ofi_bounce_buf_t* buffs[OFI_MAX_NUM_BOUNCE_BUFFERS];

        /* If there are not enough bounce buffers available, simply block as
         * we don't know when more will become available */
        ret = get_bounce_bufs(num_bufs_needed, buffs);
        if (!ret) goto block_anyways;

        gasnetc_ofi_bounce_op_ctxt_t * bbuf_ctxt = gasnetc_ofi_get_bounce_ctxt();
        gasnetc_assert_callback_eq(bbuf_ctxt, gasnetc_ofi_handle_bounce_rdma);
        bbuf_ctxt->orig_op = ctxt_ptr;
        gasnetc_paratomic_set(&bbuf_ctxt->cntr, num_bufs_needed, 0);

        i = 0;
        gasnetc_ofi_bounce_buf_t* buf_container;

        gex_Flags_t imm = flags & GEX_FLAG_IMMEDIATE;

        while (num_bufs_needed > 0) {
            bytes_to_copy = num_bufs_needed != 1 ? ofi_bbuf_size : nbytes;
            gasneti_assert(bytes_to_copy <= ofi_bbuf_size);
            buf_container = buffs[i];
            gasneti_lifo_push(&bbuf_ctxt->bbuf_list, buf_container);
            GASNETI_MEMCPY(buf_container->buf, (void*)src_ptr, bytes_to_copy);

            OFI_INJECT_RETRY_IMM(&gasnetc_ofi_locks.rdma_tx,
                                 OFI_WRITE(c_ep, buf_container->buf, bytes_to_copy,
                                           jobrank, rem_epidx, (void *)dest_ptr, bbuf_ctxt, 0),
                                 OFI_POLL_ALL, imm, out_imm_bounce);
            imm = 0; // no going back once first buffer has been written

            GASNETC_OFI_CHECK_RET(ret, "fi_write for bounce buffered data failed");

#if GASNET_DEBUG
            gasnetc_paratomic_increment(&pending_rdma,0);
#endif

            /* Update our pointers to locations in memory */
            nbytes -= ofi_bbuf_size; 
            dest_ptr += ofi_bbuf_size;
            src_ptr += ofi_bbuf_size;
            num_bufs_needed--;
            i++;
        }

        GASNETC_STAT_EVENT(NB_PUT_BOUNCE);
        return GEX_EVENT_INVALID;

out_imm_bounce:
        gasneti_assume(i == 0);
        for (/*empty*/; i < num_bufs_needed; ++i) {
            gasneti_lifo_push(&ofi_bbuf_pool, buffs[i]);
        }
        gasneti_lifo_init(&bbuf_ctxt->bbuf_list);
        gasneti_lifo_push(&ofi_bbuf_ctxt_pool, bbuf_ctxt);
        return GEX_EVENT_NO_OP;
    }
    /* We tried our best to optimize this. Just wait for remote completion */
    else {
block_anyways:
      GASNETC_STAT_EVENT(NB_PUT_BLOCK);
      if (nbytes > gasnetc_max_rma_size) {
          // Will require multiple fi_write() calls, for which gasnetc_rdma_put()
          // only supports NBI (iop) callers.  So, use an access region.
          gasnete_begin_nbi_accessregion(0, 1 GASNETI_THREAD_PASS);
            gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
            gasnete_iop_t *iop = mythread->current_iop;
            gasneti_assert(iop->put_ofi.type == OFI_TYPE_IPUT);
            iop->initiated_put_cnt++;
            gasneti_assert_zeroret( gasnetc_rdma_put(tm, rank, dest_addr, src_addr, nbytes,
                                                     &iop->put_ofi, 0, flags GASNETI_THREAD_PASS) );
          return gasnete_end_nbi_accessregion(0 GASNETI_THREAD_PASS);
      }
      // Small enough for a single fi_write()
      gasnete_eop_t *eop = gasnete_eop_new(GASNETI_MYTHREAD);
      eop->ofi.type = OFI_TYPE_EPUT;
      if (gasnetc_rdma_put(tm, rank, dest_addr, src_addr, nbytes, &eop->ofi, 0, flags GASNETI_THREAD_PASS)) {
          gasneti_assert(flags & GEX_FLAG_IMMEDIATE);
          GASNETE_EOP_MARKDONE(eop);
          gasnete_eop_free(eop GASNETI_THREAD_PASS);
          return GEX_EVENT_NO_OP;
      }
      return (gex_Event_t)eop;
    }
}

int
gasnetc_rdma_put(gex_TM_t tm, gex_Rank_t rank, void *dst_ptr, void *src_ptr, size_t nbytes,
                 gasnetc_ofi_nb_op_ctxt_t *ctxt_ptr, int alc, gex_Flags_t flags GASNETI_THREAD_FARG)
{
    const gex_EP_Location_t loc = gasneti_e_tm_rank_to_location(tm, rank, 0);
    const gex_Rank_t jobrank = loc.gex_rank;
    const int rem_epidx = gasnetc_in_auxseg(jobrank, dst_ptr) ? -1 : loc.gex_ep_index;
    gasnetc_EP_t c_ep = (gasnetc_EP_t)gasneti_e_tm_to_i_ep(tm);
    int ret = FI_SUCCESS;

    gasnetc_assert_callback_eq(ctxt_ptr, gasnetc_ofi_handle_rdma);
    gasneti_assert((alc == 0) || (alc == 1));
    gasneti_static_assert(GASNETC_CTXT_IS_ALC == 1);

    PERIODIC_RMA_POLL();

    size_t remain = nbytes;
    const size_t chunksz = gasnetc_max_rma_size;
    uintptr_t dst_addr = (uintptr_t) dst_ptr;
    uintptr_t src_addr = (uintptr_t) src_ptr;

    if (nbytes > chunksz) {
      // Chunking logic is for IOP only (NB -> NBI xform done by caller if needed)
      gasneti_assert(ctxt_ptr->type == OFI_TYPE_IPUT);
      gasnete_iop_t *iop = gasneti_container_of(ctxt_ptr, gasnete_iop_t, put_ofi);
      gasnete_iop_check(iop);

      flags &= ~GEX_FLAG_IMMEDIATE; // multi-chunk precludes IMMEDIATE support

      // TODO: is there any advantage to using the first chunk to achieve "good" alignment?
      do {
        iop->initiated_put_cnt++;
        if (alc) GASNETE_IOP_LC_START(iop);
        OFI_INJECT_RETRY(&gasnetc_ofi_locks.rdma_tx,
                         OFI_WRITE(c_ep, (void*)src_addr, chunksz,
                                   jobrank, rem_epidx, (void*)dst_addr, ctxt_ptr, alc),
                         OFI_POLL_ALL);
        GASNETC_OFI_CHECK_RET(ret, "fi_write failed");
#if GASNET_DEBUG
        gasnetc_paratomic_increment(&pending_rdma,0);
#endif

        dst_addr += chunksz;
        src_addr += chunksz;
        remain -= chunksz;
      } while (remain > chunksz);
    }

    // Might honor GEX_FLAG_IMMEDIATE *only* if this is the first fi_write():
    OFI_INJECT_RETRY_IMM(&gasnetc_ofi_locks.rdma_tx,
                         OFI_WRITE(c_ep, (void*)src_addr, remain,
                                   jobrank, rem_epidx, (void*)dst_addr, ctxt_ptr, alc),
                         OFI_POLL_ALL, flags & GEX_FLAG_IMMEDIATE, out_imm);
    GASNETC_OFI_CHECK_RET(ret, "fi_write failed");
#if GASNET_DEBUG
    gasnetc_paratomic_increment(&pending_rdma,0);
#endif

    return 0;

out_imm:
    gasneti_assert(remain == nbytes); // IMM failure only possible in single-chunk case
    return 1;
}

int
gasnetc_rdma_get(void *dst_ptr, gex_TM_t tm, gex_Rank_t rank, void *src_ptr, size_t nbytes,
                 gasnetc_ofi_nb_op_ctxt_t *ctxt_ptr, gex_Flags_t flags GASNETI_THREAD_FARG)
{
    const gex_EP_Location_t loc = gasneti_e_tm_rank_to_location(tm, rank, 0);
    const gex_Rank_t jobrank = loc.gex_rank;
    const int rem_epidx = gasnetc_in_auxseg(jobrank, src_ptr) ? -1 : loc.gex_ep_index;
    gasnetc_EP_t c_ep = (gasnetc_EP_t)gasneti_e_tm_to_i_ep(tm);
    int ret = FI_SUCCESS;

    gasnetc_assert_callback_eq(ctxt_ptr, gasnetc_ofi_handle_rdma);

    PERIODIC_RMA_POLL();

    size_t remain = nbytes;
    const size_t chunksz = gasnetc_max_rma_size;
    uintptr_t dst_addr = (uintptr_t) dst_ptr;
    uintptr_t src_addr = (uintptr_t) src_ptr;

    if (nbytes > chunksz) {
      // Chunking logic is for IOP only (NB -> NBI xform done by caller if needed)
      gasneti_assert(ctxt_ptr->type == OFI_TYPE_IGET);
      gasnete_iop_t *iop = gasneti_container_of(ctxt_ptr, gasnete_iop_t, get_ofi);
      gasnete_iop_check(iop);

      flags &= ~GEX_FLAG_IMMEDIATE; // multi-chunk precludes IMMEDIATE support

      // TODO: is there any advantage to using the first chunk to achieve "good" alignment?
      do {
        iop->initiated_get_cnt++;
        OFI_INJECT_RETRY(&gasnetc_ofi_locks.rdma_tx,
                         OFI_READ(c_ep, (void*)dst_addr, chunksz,
                                  jobrank, rem_epidx, (void*)src_addr, ctxt_ptr, 0),
                         OFI_POLL_ALL);
        GASNETC_OFI_CHECK_RET(ret, "fi_read failed");
#if GASNET_DEBUG
        gasnetc_paratomic_increment(&pending_rdma,0);
#endif

        dst_addr += chunksz;
        src_addr += chunksz;
        remain -= chunksz;
      } while (remain > chunksz);
    }

    // Might honor GEX_FLAG_IMMEDIATE *only* if this is the first fi_read():
    OFI_INJECT_RETRY_IMM(&gasnetc_ofi_locks.rdma_tx,
                         OFI_READ(c_ep, (void*)dst_addr, remain,
                                  jobrank, rem_epidx, (void*)src_addr, ctxt_ptr, 0),
                         OFI_POLL_ALL, flags & GEX_FLAG_IMMEDIATE, out_imm);

    GASNETC_OFI_CHECK_RET(ret, "fi_read failed");
#if GASNET_DEBUG
    gasnetc_paratomic_increment(&pending_rdma,0);
#endif

    return 0;

out_imm:
    gasneti_assert(remain == nbytes); // IMM failure only possible in single-chunk case
    return 1;
}

#if GASNET_HAVE_MK_CLASS_MULTIPLE
int gasnetc_mk_create_hook(
                    gasneti_MK_t                     kind,
                    gasneti_Client_t                 client,
                    const gex_MK_Create_args_t       *args,
                    gex_Flags_t                      flags)
{
    // Fail if the FI_HMEM capability was not found at initialization
    // TODO: fall back to reference implementation when we have one
    if (!gasnetc_fi_hmem) {
        GASNETI_RETURN_ERRR(RESOURCE,
            gasneti_dynsprintf("Provider '%s' reports no support for FI_HMEM needed for memory kinds",
                               gasnetc_ofi_provider));
    }

    // TODO: Fail (later fall back to ref) if the given device support is not present.
    //
    // The libfabric maintainers say that attempting a memory registration with
    // a given iface value is the only reliable way to determine if the device
    // support is present (see https://github.com/ofiwg/libfabric/issues/7973 ).
    // However, libfabric releases through (at least) 1.15.2 incorrectly return
    // success from fi_mr_regattr() when called with an iface value which is not
    // supported (see https://github.com/ofiwg/libfabric/issues/7977 ).
    // Once that is resolved, we can/should attempt a small registration
    // here and look for `-FI_ENOSYS` as an indication that the requested
    // device support is missing.

    // Capture the user's device argument for use in memory registration
    switch (args->gex_class) {
      #if GASNET_HAVE_MK_CLASS_CUDA_UVA
      case GEX_MK_CLASS_CUDA_UVA:
        kind->_mk_conduit = (void*)(uintptr_t)args->gex_args.gex_class_cuda_uva.gex_CUdevice;
        break;
      #endif

      #if GASNET_HAVE_MK_CLASS_HIP
      case GEX_MK_CLASS_HIP:
        // No device needed for HIP
        break;
      #endif

      #if GASNET_HAVE_MK_CLASS_ZE
      case GEX_MK_CLASS_ZE:
        kind->_mk_conduit = (void*)(uintptr_t)gasneti_mk_ze_device_ordinal(args->gex_args.gex_class_ze.gex_zeDevice, 0);
        break;
      #endif

      default:
        gasneti_unreachable_error(("unknown or unsupported gex_MK_Class_t value: %d", args->gex_class));
        break;
    }

    return GASNET_OK;
}

#endif
