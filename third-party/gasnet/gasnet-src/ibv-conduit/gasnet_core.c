/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/gasnet_core.c $
 * Description: GASNet ibv conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_am.h>

#include <gasnet_event_internal.h> // access to eop and iop
#if GASNET_BLCR
#include <gasnet_blcr.h>
#endif

#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include <sys/time.h>
#include <sys/resource.h>

// ntohs() should be in one of these:
#include <netinet/in.h>
#include <arpa/inet.h>

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");
#if GASNETC_IBV_SRQ
  GASNETI_IDENT(gasnetc_IdentString_SRQ, "$GASNetIbvSRQ: 1 $");
#endif
#if GASNETC_IBV_XRC
  GASNETI_IDENT(gasnetc_IdentString_XRC, "$GASNetIbvXRC: 1 $");
#endif
#if GASNETC_IBV_ODP
  GASNETI_IDENT(gasnetc_IdentString_ODP, "$GASNetIbvODP: 1 $");
#endif
GASNETI_IDENT(gasneti_IdentString_AMMaxMedium,  "$GASNetAMMaxMedium: " _STRINGIFY(GASNETC_IBV_MAX_MEDIUM) " $");

gex_AM_Entry_t const *gasnetc_get_handlertable(void);

gasnetc_EP_t gasnetc_ep0; // First EP created.  Used by init, sys AMs, and shutdown.

/* ------------------------------------------------------------------------------------ */
/*
  Configuration
  ==============
*/

/* Minimum memory to reserve for firehoses in SEGMENT_FAST: */
#define GASNETC_MIN_FH_MEM		(16*1024*1024)

/*
  The following values can be overridden by environment variables.
  Variable names are formed by replacing GASNETC_DEFAULT_ by GASNET_
*/

/* Default is to open one physical port per HCA */
#define GASNETC_DEFAULT_IBV_PORTS		""

/* Limits on in-flight (queued but not reaped) RDMA Ops */
#define GASNETC_DEFAULT_NETWORKDEPTH_TOTAL	255	/* Max ops (RDMA + AM) outstanding at source */
#define GASNETC_DEFAULT_NETWORKDEPTH_PP		24	/* Max ops (RDMA + AM) outstanding to each peer */

/* Limits on in-flight (queued but not acknowledged) AM Requests */
#define GASNETC_DEFAULT_AM_CREDITS_TOTAL	MIN(256,(gasneti_nodes-1)*gasnetc_am_oust_pp)	/* Max AM requests outstanding at source, 0 = automatic */
#define GASNETC_DEFAULT_AM_CREDITS_PP		12	/* Max AM requests outstanding to each peer */
#define GASNETC_DEFAULT_AM_CREDITS_SLACK	1	/* Max AM credits delayed by coalescing */

/* Limit on prepinned send bounce buffers */
#define GASNETC_DEFAULT_BBUF_COUNT		1024	/* Max bounce buffers prepinned, 0 = automatic */

/* Limit on AM recv buffers when using SRQ */
#define GASNETC_DEFAULT_RBUF_COUNT		1024	/* Max SRQ receive buffers posted, 0 = automatic */

/* Use of multiple QPs */
#define GASNETC_DEFAULT_NUM_QPS			0	/* 0 = one per HCA */

/* Protocol switch points */
#define GASNETC_DEFAULT_INLINESEND_LIMIT	72
#define GASNETC_DEFAULT_NONBULKPUT_BOUNCE_LIMIT	(64*1024)
#define GASNETC_DEFAULT_PACKEDLONG_LIMIT	4012 // See bug 4044 for discussion re: increasing this
#if !GASNETC_PIN_SEGMENT
  #define GASNETC_DEFAULT_PUTINMOVE_LIMIT	GASNETC_PUTINMOVE_LIMIT_MAX
#endif

size_t gasnetc_am_gather_min;
#define GASNETC_DEFAULT_AM_GATHER_MIN           1500

#define GASNETC_DEFAULT_PUT_STRIPE_SZ           0 // Disabled by default
#define GASNETC_DEFAULT_GET_STRIPE_SZ           0 // Disabled by default
#define GASNETC_MINIMUM_PUT_STRIPE_SZ           4096
#define GASNETC_MINIMUM_GET_STRIPE_SZ           4096

/* Exit coordination timeouts */
#define GASNETC_DEFAULT_EXITTIMEOUT_MAX		360.0	/* 6 minutes! */
#define GASNETC_DEFAULT_EXITTIMEOUT_MIN		2	/* 2 seconds */
#define GASNETC_DEFAULT_EXITTIMEOUT_FACTOR	0.25	/* 1/4 second */
static double gasnetc_exittimeout = GASNETC_DEFAULT_EXITTIMEOUT_MAX;

/* Exit coordination setup */
static void gasnetc_exit_init(void);

/* Maximum MTU (will silently lower to port capability) */
enum ibv_mtu gasnetc_max_mtu;

/* HW level retry knobs */
int gasnetc_qp_timeout, gasnetc_qp_retry_count;
#define GASNETC_DEFAULT_QP_TIMEOUT		18	/* about 1s */
#define GASNETC_DEFAULT_QP_RETRY_COUNT		7

#ifndef MT_MELLANOX_IEEE_VENDOR_ID
  #define MT_MELLANOX_IEEE_VENDOR_ID      0x02c9
#endif
#ifndef PCI_DEVICE_ID_MELLANOX_TAVOR
  #define PCI_DEVICE_ID_MELLANOX_TAVOR    0x5a44
#endif

/* ------------------------------------------------------------------------------------ */

/* conduit-specific firehose region parameters
 * Note that these are kept to sane sizes rather than the HCA limit
 * 128kB is the peak of the bandwidth curve and thus a good size.
 * With 32k * 128k = 4G we can pin upto 4GB of physical memory with these.
 * We don't yet deal well with many small regions.
 * Note that GASNET_FIREHOSE_* env vars can override these.
 */
static unsigned int gasnetc_fh_maxregions = 32768;
static unsigned int gasnetc_fh_maxsize    = 131072;

/* ------------------------------------------------------------------------------------ */

int		gasnetc_num_hcas = 1;
gasnetc_hca_t	gasnetc_hca[GASNETC_IB_MAX_HCAS];
uintptr_t	gasnetc_max_msg_sz;
int		gasnetc_qp_rd_atom;

gasnetc_port_info_t      *gasnetc_port_tbl = NULL;
int                      gasnetc_num_ports = 0;

static uint64_t  gasnetc_pin_maxsz;
#if GASNETC_PIN_SEGMENT
  uintptr_t		gasnetc_seg_start;
  uintptr_t		gasnetc_seg_len;
#endif
firehose_info_t	gasnetc_firehose_info;
static uintptr_t gasnetc_firehose_mem;
static int       gasnetc_firehose_reg;
static uint32_t  gasnetc_firehose_flags;

int		gasnetc_op_oust_limit;
int		gasnetc_op_oust_pp;
int		gasnetc_am_oust_limit;
int		gasnetc_am_oust_pp;
int		gasnetc_bbuf_limit;
#if GASNETC_IBV_SRQ
  int		gasnetc_rbuf_limit;
  int		gasnetc_rbuf_set;
#endif

/* Maximum pinning capabilities of the HCA */
typedef struct gasnetc_pin_info_t_ {
    uint64_t    physmemsz;
    uintptr_t	memory;		/* How much pinnable (per proc) */
    uint32_t	regions;
    int		num_local;	/* How many procs */
} gasnetc_pin_info_t;
static gasnetc_pin_info_t gasnetc_pin_info;

static char *gasnetc_ibv_ports;

#if GASNET_TRACE
  static unsigned int	gasnetc_pinned_blocks = 0;
  static size_t		gasnetc_pinned_bytes = 0;
#endif

static int gasnetc_did_firehose_init = 0;

static const enum ibv_access_flags
    gasneti_seg_access_flags =
        (enum ibv_access_flags) (IBV_ACCESS_LOCAL_WRITE  |
                                 IBV_ACCESS_REMOTE_WRITE |
                                 IBV_ACCESS_REMOTE_READ  |
                                 IBV_ACCESS_REMOTE_ATOMIC);

/* ------------------------------------------------------------------------------------ */
/*
  Bootstrap collectives
*/

gasneti_spawnerfn_t const *gasneti_spawner = NULL;

static int gasneti_bootstrap_native_coll = 0;
static int gasnetc_bootstrapBarrier_phase = 0;
static int gasnetc_bootstrapExchange_phase = 0;
static gex_Rank_t gasnetc_dissem_peers = 0;
static gex_Rank_t *gasnetc_dissem_peer = NULL;
static gex_Rank_t *gasnetc_exchange_rcvd = NULL;
static gex_Rank_t *gasnetc_exchange_send = NULL;
#if GASNET_PSHM
static gex_Rank_t *gasnetc_exchange_permute = NULL;
#endif

static void gasnetc_sys_coll_init(void)
{
  int i;

#if GASNET_PSHM
  const gex_Rank_t size = gasneti_nodemap_global_count;
  const gex_Rank_t rank = gasneti_nodemap_global_rank;

  if (gasneti_nodemap_local_rank) {
    /* No network comms */
    goto done;
  }
#else
  const gex_Rank_t size = gasneti_nodes;
  const gex_Rank_t rank = gasneti_mynode;
#endif

  if (size == 1) {
    /* No network comms */
    goto done;
  }

  gasnetc_bootstrapBarrier_phase = 0;
  gasnetc_bootstrapExchange_phase = 0;

  /* Construct vector of the dissemination peers */
  gasnetc_dissem_peers = 0;
  for (i = 1; i < size; i *= 2) {
    ++gasnetc_dissem_peers;
  }
  if (NULL == gasnetc_dissem_peer) {
    gasnetc_dissem_peer = gasneti_malloc(gasnetc_dissem_peers * sizeof(gex_Rank_t));
    gasneti_leak(gasnetc_dissem_peer);
  }
  for (i = 0; i < gasnetc_dissem_peers; ++i) {
    const gex_Rank_t distance = 1 << i;
    const gex_Rank_t peer = (distance <= rank) ? (rank - distance) : (rank + (size - distance));
  #if GASNET_PSHM
    /* Convert supernode numbers to node numbers */
    gasnetc_dissem_peer[i] = gasneti_pshm_firsts[peer];
  #else
    gasnetc_dissem_peer[i] = peer;
  #endif
  }

  /* Compute the recv offset and send count for each step of exchange */
  gasnetc_exchange_rcvd = gasneti_malloc((gasnetc_dissem_peers+1) * sizeof(gex_Rank_t));
  gasnetc_exchange_send = gasneti_malloc(gasnetc_dissem_peers * sizeof(gex_Rank_t));
  { int step;
  #if GASNET_PSHM
    gex_Rank_t *width;
    gex_Rank_t sum1 = 0;
    gex_Rank_t sum2 = 0;
    gex_Rank_t distance, last;

    distance = 1 << (gasnetc_dissem_peers-1);
    last = (distance <= rank) ? (rank - distance) : (rank + (size - distance));

    /* Step 1: determine the "width" of each supernode */
    width = gasneti_calloc(size, sizeof(gex_Rank_t));
    for (i = 0; i < gasneti_nodes; ++i) {
      width[gasneti_nodeinfo[i].supernode] += 1;
    }
    /* Step 2: form the necessary partial sums */
    for (step = i = 0; step < gasnetc_dissem_peers; ++step) {
      distance = 1 << step;
      for (/*empty*/; i < distance; ++i) {
        sum1 += width[ (rank + i) % size ];
        sum2 += width[ (last + i) % size ];
      }
      gasnetc_exchange_rcvd[step] = gasnetc_exchange_send[step] = sum1;
    }
    gasnetc_exchange_send[step-1] = gasneti_nodes - sum2;
    gasnetc_exchange_rcvd[step] = gasneti_nodes;
    /* Step 3: construct the permutation vector, if necessary */
    {
      gex_Rank_t n;
    
      /* Step 3a. determine if we even need a permutation vector */
      int sorted = 1;
      gasneti_assert(0 == gasneti_nodeinfo[0].supernode);
      n = 0;
      for (i = 1; i < gasneti_nodes; ++i) {
        if (n > gasneti_nodeinfo[i].supernode) {
          sorted = 0;
          break;
        }
        n = gasneti_nodeinfo[i].supernode;
      }

      /* Step 3b. contstruct the vector if needed */
      if (!sorted) {
        gex_Rank_t *offset = gasneti_malloc(size * sizeof(gex_Rank_t));
        
        /* Form a sort of shifted prefix-reduction on width */
        sum1 = 0;
        n = rank;
        for (i = 0; i < size; ++i) {
          offset[n] = sum1;
          sum1 += width[n];
          n = (n == size-1) ? 0 : (n+1);
        }
        gasneti_assert(sum1 == gasneti_nodes);

        /* Scan nodeinfo to collect all the nodes in each supernode (in their order) */
        gasnetc_exchange_permute = gasneti_malloc(gasneti_nodes * sizeof(gex_Rank_t));
        for (i = 0; i < gasneti_nodes; ++i) {
          int index = offset[ gasneti_nodeinfo[i].supernode ]++;
          gasnetc_exchange_permute[index] = i;
        }
      }
    }
    gasneti_free(width);
  #else
    for (step = 0; step < gasnetc_dissem_peers; ++step) {
      gasnetc_exchange_rcvd[step] = gasnetc_exchange_send[step] = 1 << step;
    }
    gasnetc_exchange_send[step-1] = gasneti_nodes - gasnetc_exchange_send[step-1];
    gasnetc_exchange_rcvd[step] = gasneti_nodes;
  #endif
  }

done:
  gasneti_assert(! gasneti_bootstrap_native_coll);
  gasneti_bootstrap_native_coll = 1;
#if !GASNETC_IBV_SHUTDOWN
  gasneti_spawner->Cleanup(); /* No futher use of ssh/mpi/pmi collectives */
#endif
}

static void gasnetc_sys_coll_fini(void)
{
  gasneti_free(gasnetc_exchange_rcvd);
  gasneti_free(gasnetc_exchange_send);
#if GASNET_PSHM
  gasneti_free(gasnetc_exchange_permute);
#endif

#if GASNET_DEBUG
  gasnetc_exchange_rcvd = NULL;
  gasnetc_exchange_send = NULL;
 #if GASNET_PSHM
  gasnetc_exchange_permute = NULL;
 #endif
#endif

  gasneti_bootstrap_native_coll = 0;
}

#if GASNETC_USE_RCV_THREAD
  static gasneti_atomic_t gasnetc_sys_barrier_rcvd[2] =
                            {gasneti_atomic_init(0), gasneti_atomic_init(0)};
  #define gasnetc_sys_barrier_read(_phase) \
    gasneti_atomic_read(&gasnetc_sys_barrier_rcvd[_phase], 0)
  #define gasnetc_sys_barrier_reset(_phase) \
    gasneti_atomic_set(&gasnetc_sys_barrier_rcvd[_phase], 0, GASNETI_ATOMIC_WMB_POST)
#else
  static uint32_t gasnetc_sys_barrier_rcvd[2] = {0, 0};
  #define gasnetc_sys_barrier_read(_phase) \
    (gasnetc_sys_barrier_rcvd[_phase])
  #define gasnetc_sys_barrier_reset(_phase) \
    ((void)(gasnetc_sys_barrier_rcvd[_phase] = 0))
#endif

static void gasnetc_sys_barrier_reqh(gex_Token_t token, uint32_t arg)
{
    const int phase = arg & 1;
#if GASNETC_USE_RCV_THREAD
    gasneti_atomic_t *p = &gasnetc_sys_barrier_rcvd[phase];
  #if defined(GASNETI_HAVE_ATOMIC_ADD_SUB)
    /* atomic OR via ADD since no bit will be set more than once */
    const int flag = arg & ~1;
    gasneti_assert(GASNETI_POWEROFTWO(flag));
    gasneti_atomic_add(p, flag, 0);
  #elif defined(GASNETI_HAVE_ATOMIC_CAS)
    /* atomic OR via C-A-S */
    uint32_t old_val;
    do {
      old_val = gasneti_atomic32_read(p, 0);
    } while (!gasneti_atomic_compare_and_swap(p, old_val, old_val|arg, 0));
  #else
    #error "required atomic compare-and-swap is not yet implemented for your CPU/OS/compiler"
  #endif
#else
    gasnetc_sys_barrier_rcvd[phase] |= arg;
#endif
}

static void gasnetc_bootstrapBarrier_ib(void)
{
    int phase = gasnetc_bootstrapBarrier_phase;
    int i;

#if GASNET_PSHM
    gasneti_pshmnet_bootstrapBarrier();
#endif
    for (i = 0; i < gasnetc_dissem_peers; ++i) {
      const uint32_t mask = 2 << i; /* (distance << 1) */

      (void) gasnetc_RequestSysShort(gasnetc_dissem_peer[i], NULL,
                                     gasneti_handleridx(gasnetc_sys_barrier_reqh),
                                     1, phase | mask);

      /* wait for completion of the proper receive, which might arrive out of order */
      while (!(gasnetc_sys_barrier_read(phase) & mask)) {
         gasnetc_sndrcv_poll(0);
      }
    }
#if GASNET_PSHM
    gasneti_pshmnet_bootstrapBarrier();
#endif

    /* reset for next barrier */
    gasnetc_sys_barrier_reset(phase);
    gasnetc_bootstrapBarrier_phase ^= 1;
}

extern void gasneti_bootstrapBarrier(void)
{
  if (gasneti_bootstrap_native_coll) {
  #if GASNET_DEBUG
    if (!gasneti_mynode) {
      fprintf(stderr, "@ DEVWARN: Indirect native Barrier\n");
      /* gasnett_print_backtrace(2); */
    }
  #endif
    gasnetc_bootstrapBarrier_ib();
  } else {
    gasneti_spawner->Barrier();
  }
}

#if GASNET_MAXNODES > 65535
#error "Update gasneti_bootstrapExchange for > 16-bit node count"
#endif

#if GASNETC_USE_RCV_THREAD
  static gasneti_atomic_t gasnetc_sys_exchange_rcvd[2][16] =
  { { gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0) },
    { gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0) } };
  #define gasnetc_sys_exchange_inc(_phase, _step) \
    gasneti_atomic_increment(&gasnetc_sys_exchange_rcvd[_phase][_step], GASNETI_ATOMIC_REL)
  #define gasnetc_sys_exchange_read(_phase, _step) \
    gasneti_atomic_read(&gasnetc_sys_exchange_rcvd[_phase][_step], GASNETI_ATOMIC_NONE)
  #define gasnetc_sys_exchange_reset(_phase, _step) \
    gasneti_atomic_set(&gasnetc_sys_exchange_rcvd[_phase][_step], 0, GASNETI_ATOMIC_ACQ)
#else
  static gasneti_atomic_val_t gasnetc_sys_exchange_rcvd[2][16] =
  { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
  #define gasnetc_sys_exchange_inc(_phase, _step) \
    ((void)(++gasnetc_sys_exchange_rcvd[_phase][_step]))
  #define gasnetc_sys_exchange_read(_phase, _step) \
    (gasnetc_sys_exchange_rcvd[_phase][_step])
  #define gasnetc_sys_exchange_reset(_phase, _step) \
    ((void)(gasnetc_sys_exchange_rcvd[_phase][_step] = 0))
#endif

static uint8_t *gasnetc_sys_exchange_buf[2] = { NULL, NULL };
#if GASNET_DEBUG
static size_t gasnetc_sys_exchange_elemsz[2];
#endif

static uint8_t *gasnetc_sys_exchange_addr(int phase, size_t elemsz)
{
#if GASNETC_USE_RCV_THREAD
  static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;
  gasneti_mutex_lock(&lock);
#endif

  if (gasnetc_sys_exchange_buf[phase] == NULL) {
    gasnetc_sys_exchange_buf[phase] = gasneti_malloc(elemsz * gasneti_nodes);
  #if GASNET_DEBUG
    gasnetc_sys_exchange_elemsz[phase] = elemsz;
  } else {
    gasneti_assert(gasnetc_sys_exchange_elemsz[phase] == elemsz);
  #endif
  }

#if GASNETC_USE_RCV_THREAD
  gasneti_mutex_unlock(&lock);
#endif

  return gasnetc_sys_exchange_buf[phase];
}

#define GASNETC_SYS_EXCHANGE_MAX GASNETC_MAX_MEDIUM

static void gasnetc_sys_exchange_reqh(gex_Token_t token, void *buf,
                                 size_t nbytes, uint32_t arg0,
                                 uint32_t elemsz)
{
  const int phase = arg0 & 1;
  const int step = (arg0 >> 1) & 0x0f;
  const int seq = (arg0 >> 5);
  const size_t offset = elemsz * gasnetc_exchange_rcvd[step];
  uint8_t *dest = gasnetc_sys_exchange_addr(phase, elemsz)
                  + offset + (seq * GASNETC_SYS_EXCHANGE_MAX);

  GASNETI_MEMCPY(dest, buf, nbytes);
  gasnetc_sys_exchange_inc(phase, step);
}

static void gasnetc_bootstrapExchange_ib(void *src, size_t len, void *dest)
{
    int phase = gasnetc_bootstrapExchange_phase;

    uint8_t *temp = gasnetc_sys_exchange_addr(phase, len);
    int step;

#if GASNET_PSHM
    /* Construct supernode-local contribution */
    gasneti_pshmnet_bootstrapGather(gasneti_request_pshmnet, src, len, temp, 0);
    if (gasneti_nodemap_local_rank) goto end_network_comms;
#else
    /* Copy in local contribution */
    GASNETI_MEMCPY(temp, src, len);
#endif

    /* Bruck's concatenation algorithm: */
    for (step = 0; step < gasnetc_dissem_peers; ++step) {
      size_t nbytes = len * gasnetc_exchange_send[step];
      size_t offset = 0;
      uint32_t seq = 0;

      gasneti_assert(step < 16);

      /* Send payload using AMMedium(s) */
      do {
        const size_t to_xfer = MIN(nbytes, GASNETC_SYS_EXCHANGE_MAX);

        (void) gasnetc_RequestSysMedium(gasnetc_dissem_peer[step], NULL,
                                        gasneti_handleridx(gasnetc_sys_exchange_reqh),
                                        temp + offset, to_xfer,
                                        2, phase | (step << 1) | (seq << 5), len);

        ++seq;
        offset += to_xfer;
        nbytes -= to_xfer;
        gasneti_assert(seq < (1<<(32-5)));
      } while (nbytes);

      /* poll until correct number of messages have been received */
      nbytes = len * (gasnetc_exchange_rcvd[step+1] - gasnetc_exchange_rcvd[step]);
      seq = (nbytes + GASNETC_SYS_EXCHANGE_MAX - 1) / GASNETC_SYS_EXCHANGE_MAX;
      while (gasnetc_sys_exchange_read(phase, step) != seq) {
        gasnetc_sndrcv_poll(0);
      }
      gasnetc_sys_exchange_reset(phase, step); /* Includes the RMB, if any, for the data */
    }

    /* Copy to destination while performing the rotation or permutation */
#if GASNET_PSHM
    if (gasnetc_exchange_permute) {
      gex_Rank_t n;
      for (n = 0; n < gasneti_nodes; ++n) {
        const gex_Rank_t peer = gasnetc_exchange_permute[n];
        GASNETI_MEMCPY((uint8_t*) dest + len * peer, temp + len * n, len);
      }
    } else
#endif
    {
      GASNETI_MEMCPY_SAFE_EMPTY(dest, temp + len * (gasneti_nodes - gasneti_mynode), len * gasneti_mynode);
      GASNETI_MEMCPY((uint8_t*)dest + len * gasneti_mynode, temp, len * (gasneti_nodes - gasneti_mynode));
    }

#if GASNET_PSHM
end_network_comms:
    gasneti_pshmnet_bootstrapBroadcast(gasneti_request_pshmnet, dest, len*gasneti_nodes, dest, 0);
#endif

    /* Prepare for next */
    gasneti_free(temp);
    gasnetc_sys_exchange_buf[phase] = NULL;
    gasneti_sync_writes();
    gasnetc_bootstrapExchange_phase ^= 1;

#if GASNET_DEBUG
  /* verify own data as a sanity check */
  if (memcmp(src, (void *) ((uintptr_t ) dest + (gasneti_mynode * len)), len) != 0) {
    gasneti_fatalerror("exchange failed: self data on node %d is incorrect", gasneti_mynode);
  }
#endif
}

extern void gasneti_bootstrapExchange(void *src, size_t len, void *dest)
{
  if (gasneti_bootstrap_native_coll) {
  #if GASNET_DEBUG
    if (!gasneti_mynode) {
      fprintf(stderr, "@ DEVWARN: Indirect native Exchange\n");
      /* gasnett_print_backtrace(2); */
    }
  #endif
    gasnetc_bootstrapExchange_ib(src, len, dest);
  } else {
    gasneti_spawner->Exchange(src, len, dest);
  }
}

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnetc_check_config(void) {
  gasneti_check_config_preinit();

  gasneti_assert_always(offsetof(gasnetc_medmsg_t,args) == GASNETC_MEDIUM_HDRSZ);
  gasneti_assert_always(offsetof(gasnetc_longmsg_t,args) == GASNETC_LONG_HDRSZ);
  gasneti_assert_always(GASNETI_POWEROFTWO(GASNETC_BUFSZ));
}

extern void gasnetc_unpin(gasnetc_hca_t *hca, gasnetc_memreg_t *reg) {
  int rc = ibv_dereg_mr(reg->handle);
  GASNETC_IBV_CHECK(rc, "from ibv_dereg_mr()");
#if GASNET_TRACE
  gasnetc_pinned_blocks -= 1;
  gasnetc_pinned_bytes -= reg->len;
#endif
}

extern int gasnetc_pin(gasnetc_hca_t *hca, void *addr, size_t size, enum ibv_access_flags acl, gasnetc_memreg_t *reg) {
  gasneti_assert(((uintptr_t)addr % GASNET_PAGESIZE) == 0);
  gasneti_assert(((uintptr_t)size % GASNET_PAGESIZE) == 0);

  reg->handle = ibv_reg_mr(hca->pd, addr, size, acl);
  if_pf (reg->handle == NULL) return 1;

  reg->addr     = (uintptr_t)addr;
  reg->len      = size;

#if GASNET_TRACE
  gasnetc_pinned_blocks += 1;
  gasnetc_pinned_bytes += reg->len;
#endif

  return 0;
}

static void *gasnetc_try_pin_inner(size_t size, gasnetc_memreg_t *reg) {
  int vstat;
  void *addr;
  int h;

  addr = gasnetc_mmap(size);
  if (addr != GASNETC_MMAP_FAILED) {
    GASNETC_FOR_ALL_HCA_INDEX(h) {
      gasnetc_hca_t *hca = &gasnetc_hca[h];
      vstat = gasnetc_pin(hca, addr, size, (enum ibv_access_flags)0, &reg[h]);
      if (vstat != 0) {
	for (h -= 1; h >= 0; --h) {
          gasnetc_unpin(&gasnetc_hca[h], &reg[h]);
	}
        gasnetc_munmap(addr, size);
        return NULL;
      }
    }
  } else {
    addr = NULL;
  }

  return addr;
}

/* Try to pin up to 'limit' in chunks no larger than size 'step' */
static uintptr_t gasnetc_trypin(uintptr_t limit, uintptr_t step) {
  gasnetc_memreg_t reg[GASNETC_IB_MAX_HCAS];
  int h;

  while (limit >= GASNETI_MMAP_GRANULARITY) {
    /* step non-zero means try linear growth, else bisection */
    uintptr_t size = step ? MIN(limit, step) : limit;
    uintptr_t half = GASNETI_PAGE_ALIGNDOWN(size / 2);
    if (gasnetc_try_pin_inner(size, reg) != NULL) {
      /* Success - recurse to try another chunk */
      size += gasnetc_trypin(step ? (limit - size) : half, step);
      GASNETC_FOR_ALL_HCA_INDEX(h) {
        gasnetc_unpin(&gasnetc_hca[h], &reg[h]);
      }
      gasnetc_unmap(&reg[0]);
      return size;
    }
    limit = half;
    step = 0;
  }

  return 0;
}

#if GASNET_ALIGNED_SEGMENTS  /* Unused otherwise */
/* Reproduce the mmap()/munmap() steps to keep compatible VM spaces */
static void gasnetc_fakepin(uintptr_t limit, uintptr_t step) {
  if (limit != 0) {
    void *addr;
    step = MIN(limit, step);
    addr = gasnetc_mmap(step);
    if (addr != GASNETC_MMAP_FAILED) {
      gasnetc_fakepin(limit - step, step);
      gasnetc_munmap(addr, step);
    }
  }
}
#endif

#ifndef GASNETC_PHYSMEM_MIN
#define GASNETC_PHYSMEM_MIN (128*1024*1024)
#endif

static void gasnetc_physmem_check(const char *reason, uintptr_t limit) {
  if (limit < GASNETC_PHYSMEM_MIN) {
    char min_display[16];
    char limit_display[16];
    gasneti_format_number(GASNETC_PHYSMEM_MIN, min_display, sizeof(min_display), 1);
    gasneti_format_number(limit, limit_display, sizeof(limit_display), 1);
    gasneti_fatalerror(
            "%s yields GASNET_PHYSMEM_MAX of %"PRIuPTR" (%s), "
            "which is less than the minimum supported value of %s.",
            reason, limit, limit_display, min_display);
  }
}

static void gasnetc_physmem_report(double elapsed, gasnetc_pin_info_t *all_info)
{
  fprintf(stderr, "WARNING: Probe of max pinnable memory completed in %gs.\n", elapsed);
  char valstr1[80], valstr2[80], valstr3[80];
  double sum_frac, min_frac, max_frac;
  uintptr_t min_mem, max_mem;
  uint64_t sum_mem, sum_pin, min_pin, max_pin;
  sum_pin = min_pin = max_pin = all_info[0].memory;
  sum_mem = min_mem = max_mem = all_info[0].physmemsz;
  sum_frac = min_frac = max_frac = sum_pin / (double)sum_mem;
  for (gex_Rank_t i = 1; i < gasneti_nodes; ++i) {
    uintptr_t pin = all_info[i].memory;
    if (pin == ~((uintptr_t)0)) continue;  // Not probed
    sum_pin += pin;
    min_pin = MIN(min_pin, pin);
    max_pin = MAX(max_pin, pin);

    uintptr_t mem = all_info[i].physmemsz;
    sum_mem += mem;
    min_mem = MIN(min_mem, mem);
    max_mem = MAX(max_mem, mem);

    double frac = pin / (double)mem;
    sum_frac += frac;
    min_frac = MIN(min_frac, frac);
    max_frac = MAX(max_frac, frac);
  }
  int single_valued = 0;
  if ((max_pin - min_pin) < ((uintptr_t)1 << 30)) {
    // less than 1G difference in ABSOLUTE size
    gasneti_format_number(min_pin, valstr1, sizeof(valstr1), 1);
    single_valued = 1;
  } else if ((max_frac - min_frac) < 0.05) {
    // less than 5 percentage points difference in RELATIVE size
    snprintf(valstr1, sizeof(valstr1), "%.3g", min_frac);
    single_valued = 1;
  }
  if (single_valued) {
    fprintf(stderr, "WARNING:   Probe of max pinnable memory has yielded '%s'.\n", valstr1);
    fprintf(stderr, "WARNING:   If you have the same memory configuration on all nodes, then\n");
    fprintf(stderr, "WARNING:   to avoid this probe in the future either reconfigure using\n");
    fprintf(stderr, "WARNING:      --with-ibv-physmem-max='%s'\n", valstr1);
    fprintf(stderr, "WARNING:   or run with environment variable\n");
    fprintf(stderr, "WARNING:      GASNET_PHYSMEM_MAX='%s'.\n", valstr1);
  } else {
    fprintf(stderr, "WARNING:   Probe of max pinnable memory found varying results\n");
    gasneti_format_number(sum_mem/gasneti_nodemap_global_count, valstr1, sizeof(valstr1), 1);
    gasneti_format_number(min_mem, valstr2, sizeof(valstr2), 1);
    gasneti_format_number(max_mem, valstr3, sizeof(valstr3), 1);
    fprintf(stderr, "WARNING:   Physical memory   MEAN/MIN/MAX = %s / %s / %s\n",
                    valstr1, valstr2, valstr3);
    gasneti_format_number(sum_pin/gasneti_nodemap_global_count, valstr1, sizeof(valstr1), 1);
    gasneti_format_number(min_pin, valstr2, sizeof(valstr2), 1);
    gasneti_format_number(max_pin, valstr3, sizeof(valstr3), 1);
    fprintf(stderr, "WARNING:   Pinnable memory   MEAN/MIN/MAX = %s / %s / %s\n",
                    valstr1, valstr2, valstr3);
    snprintf(valstr1, sizeof(valstr1), "%.3g", sum_frac/gasneti_nodemap_global_count);
    snprintf(valstr2, sizeof(valstr2), "%.3g", min_frac);
    snprintf(valstr3, sizeof(valstr3), "%.3g", max_frac);
    fprintf(stderr, "WARNING:   Pinnable fraction MEAN/MIN/MAX = %s / %s / %s\n",
                    valstr1, valstr2, valstr3);

    // Report memory "lost" at min absolute size
    uintptr_t try_abs = min_pin;
    uintptr_t lost = all_info[0].memory - try_abs;
    sum_mem = max_mem = lost;
    sum_frac = max_frac = lost / (double)all_info[0].memory;
    for (gex_Rank_t i = 1; i < gasneti_nodes; ++i) {
      uintptr_t pin = all_info[i].memory;
      if (pin == ~((uintptr_t)0)) continue;  // Not probed
      lost = pin - try_abs;
      sum_mem += lost;
      max_mem = MAX(max_mem, lost);

      double frac = lost / (double)pin;
      sum_frac += frac;
      max_frac = MAX(max_frac, frac);
    }
    gasneti_format_number(try_abs, valstr1, sizeof(valstr1), 1);
    fprintf(stderr, "WARNING:   Unusable pinned memory with an absolute max of '%s':\n", valstr1);
    gasneti_format_number(sum_mem, valstr1, sizeof(valstr1), 1);
    gasneti_format_number(sum_mem/gasneti_nodemap_global_count, valstr2, sizeof(valstr2), 1);
    gasneti_format_number(max_mem, valstr3, sizeof(valstr3), 1);
    fprintf(stderr, "WARNING:     SUM/MEAN/MAX = %s / %s / %s\n", valstr1, valstr2, valstr3);

    // Report memory "lost" at min relative size
    double try_rel = min_frac;
    lost = all_info[0].memory - (try_rel * all_info[0].physmemsz);
    sum_mem = max_mem = lost;
    sum_frac = max_frac = lost / (double)all_info[0].memory;
    for (gex_Rank_t i = 1; i < gasneti_nodes; ++i) {
      uintptr_t pin = all_info[i].memory;
      if (pin == ~((uintptr_t)0)) continue;  // Not probed
      lost = pin - (try_rel * all_info[i].physmemsz);
      sum_mem += lost;
      max_mem = MAX(max_mem, lost);

      double frac = lost / (double)pin;
      sum_frac += frac;
      max_frac = MAX(max_frac, frac);
    }
    fprintf(stderr, "WARNING:   Unusable pinned memory with a relative max of '%.3g':\n", try_rel);
    gasneti_format_number(sum_mem, valstr1, sizeof(valstr1), 1);
    gasneti_format_number(sum_mem/gasneti_nodemap_global_count, valstr2, sizeof(valstr2), 1);
    gasneti_format_number(max_mem, valstr3, sizeof(valstr3), 1);
    fprintf(stderr, "WARNING:     SUM/MEAN/MAX = %s / %s / %s\n", valstr1, valstr2, valstr3);
  }
  fprintf(stderr, "WARNING: For more information see \"Slow PHYSMEM probe at start-up\"\n");
  fprintf(stderr, "WARNING: in ibv-conduit's README.\n");
}

/* Search for the total amount of memory we can pin per process.
 */
static void gasnetc_init_pin_info(int first_local, int num_local) {
  gasnetc_pin_info_t *all_info = gasneti_malloc(gasneti_nodes * sizeof(gasnetc_pin_info_t));
  int i;

  /* 
   * We bound our search by the smallest of:
   *   env(GASNET_PHYSMEM_MAX) as described in README
   *   User's current (soft) mlock limit (optional)
   *   if FIREHOSE_M and FIREHOSE_MAXVICTIM_M are both set:
   *     (SEGMENT_FAST ? MMAP_LIMIT : 0 ) + (FIREHOSE_M + FIREHOSE_MAXVICTIM_M + eplison)
   *
   * Unless env(GASNET_PHYSMEM_PROBE) is set to a "true" value, we will NOT verify any value
   * set by configure or the GASNET_PHYSMEM_MAX environment variable.
   */

  #ifdef GASNETC_IBV_PHYSMEM_MAX_CONFIGURE
    #define GASNETC_DEFAULT_PHYSMEM_MAX GASNETC_IBV_PHYSMEM_MAX_CONFIGURE
    int do_probe_default = 0;
  #else
    #define GASNETC_DEFAULT_PHYSMEM_MAX "2/3"
    int do_probe_default = ! gasneti_getenv("GASNET_PHYSMEM_MAX");
  #endif
  #ifdef GASNETC_IBV_PHYSMEM_PROBE_CONFIGURE
    do_probe_default = GASNETC_IBV_PHYSMEM_PROBE_CONFIGURE;
  #else
    // Will probe on request or if neither configure nor environment has provided a value
  #endif
  const int do_probe = gasneti_getenv_yesno_withdefault("GASNET_PHYSMEM_PROBE", do_probe_default);
  const int quiet = do_probe ? !gasneti_getenv_yesno_withdefault("GASNET_PHYSMEM_WARN", 1): 0;

  // We document that the behavior is undefined unless
  // GASNET_PHYSMEM_{PROBE,WARN} are single-valued.  However, as noted in bug
  // 3769, the case of non-equal values can lead to non-collective calls to
  // gasnetc_bootstrapExchange_ib() (not a clean failure mode).
  // So, we do some extra work here to ensure single-valued behavior.
  // However, we do are not documenting this specific behavior to reserve
  // the right to silently change it in the future.
  struct {  // TODO? pack into a single byte?
    int8_t do_probe;
    int8_t quiet;
  } *all_knobs, my_knobs = { do_probe, quiet };
  all_knobs = gasneti_malloc(gasneti_nodes * sizeof(my_knobs));
  gasnetc_bootstrapExchange_ib(&my_knobs, sizeof(my_knobs), all_knobs);
#if 1
  // Option 1: fatal error on mismatch
  if (!gasneti_mynode) {
    for (gex_Rank_t n = 0; n < gasneti_nodes; ++n) {
      if (do_probe != all_knobs[n].do_probe) {
      #ifdef GASNETC_IBV_PHYSMEM_MAX_CONFIGURE
        gasneti_fatalerror("GASNET_PHYSMEM_PROBE is not single-valued");
      #else
        gasneti_fatalerror("GASNET_PHYSMEM_PROBE is not single-valued (might be defaulted from GASNET_PHYSMEM_MAX)");
      #endif
      }
      if (quiet != all_knobs[n].quiet) {
        gasneti_fatalerror("GASNET_PHYSMEM_WARN is not single-valued");
      }
    }
  }
#else
  // Option 2: logical OR do_probe and AND of quiet
  // NOTE: if one pisks this option, one must also remove 'const' from decls
  for (gasnet_gex_Rank_t n = 0; n < gasneti_nodes; ++n) {
    do_probe |= all_knobs[n].do_probe;
    quiet    &= all_knobs[n].quiet;
  }
#endif
  gasneti_free(all_knobs);

  uint64_t physmemsz = gasneti_getPhysMemSz(1);
#if PLATFORM_ARCH_32
  uint64_t hardmax = 0xFFFFFFFF;
#else
  uint64_t hardmax = 0; // unlimited
#endif
  uint64_t limit = gasneti_getenv_memsize_withdefault(
                           "GASNET_PHYSMEM_MAX", GASNETC_DEFAULT_PHYSMEM_MAX,
                           GASNETC_PHYSMEM_MIN, hardmax, physmemsz, 0, 0);

  #if defined(RLIMIT_MEMLOCK) && GASNETC_HONOR_RLIMIT_MEMLOCK
  { /* Honor soft mlock limit (build-time option) */
    struct rlimit r;
    if ((getrlimit(RLIMIT_MEMLOCK, &r) == 0) && (r.rlim_cur != RLIM_INFINITY)) {
      limit = MIN(limit, r.rlim_cur);
      gasnetc_physmem_check("Application of RLIMIT_MEMLOCK", limit);
    }
  }
  #endif

  { /* Honor Firehose params if set */
    unsigned long fh_M = gasneti_parse_int(gasnet_getenv("GASNET_FIREHOSE_M"),(1<<20));
    unsigned long fh_VM = gasneti_parse_int(gasnet_getenv("GASNET_FIREHOSE_MAXVICTIM_M"),(1<<20));
    if (fh_M && fh_VM) {
      #if GASNETC_PIN_SEGMENT
	limit = MIN(limit, (fh_M + fh_VM + GASNETI_MMAP_LIMIT + GASNETI_MMAP_GRANULARITY));
      #else
	limit = MIN(limit, (fh_M + fh_VM + GASNETI_MMAP_GRANULARITY));
      #endif
      limit = GASNETI_PAGE_ALIGNDOWN(limit);
      gasnetc_physmem_check("Application of GASNET_FIREHOSE_M and GASNET_FIREHOSE_MAXVICTIM_M", limit);
    }
  }

  GASNETI_TRACE_PRINTF(I, ("Final/effective GASNET_PHYSMEM_MAX=%"PRIu64, limit));

  gasnetc_pin_info.physmemsz = physmemsz;
  gasnetc_pin_info.memory    = ~((uintptr_t)0);
  gasnetc_pin_info.num_local = num_local;
  gasnetc_pin_info.regions = gasnetc_fh_maxregions;
  GASNETC_FOR_ALL_HCA_INDEX(i) {
    if (! gasnetc_hca[i].hca_cap.max_mr) { // Treat zero as unbounded (e.g. Omni-Path)
      GASNETI_TRACE_PRINTF(I, ("HCA %d advertises hca_cap.max_mr == 0, treating as unbounded", i));
      continue;
    }
    gasnetc_pin_info.regions = MIN(gasnetc_pin_info.regions, gasnetc_hca[i].hca_cap.max_mr);
  }

  if (do_probe) {
    int did_warn = 0;
    gasneti_tick_t start_time = gasneti_ticks_now();
    // Warn if any node has more than 2G (unless QUIET)
    if (! quiet) {
      uint64_t *all_limits = gasneti_malloc(gasneti_nodes * sizeof(uint64_t));
      gasnetc_bootstrapExchange_ib(&limit, sizeof(uint64_t), all_limits);
      if (!gasneti_mynode) {
        uint64_t max_limit = all_limits[0];
        for (gex_Rank_t i = 1; i < gasneti_nodes; ++i) {
          max_limit = MAX(max_limit, all_limits[i]);
        }
        if (max_limit > ((uint64_t)2 << 30)) {
          fprintf(stderr, "WARNING: Beginning a potentially slow probe of max pinnable memory...\n");
          fflush(stderr);
          did_warn = 1;
        }
      }
      gasneti_free(all_limits);
    }
    /* Now search for largest pinnable memory, on one process per machine */
    uintptr_t step = ~(uintptr_t)0;
    GASNETC_FOR_ALL_HCA_INDEX(i) {
      step = MIN(step, gasnetc_hca[i].hca_cap.max_mr_size);
    }
    #if GASNETC_PIN_SEGMENT
    if (gasnetc_pin_maxsz && (step > gasnetc_pin_maxsz)) {
      step = MIN(step, gasnetc_pin_maxsz);
    }
    #endif
    step = GASNETI_PAGE_ALIGNDOWN(step);
    if (gasneti_mynode == first_local) {
      uintptr_t size = gasnetc_trypin(limit, step);
      if_pf (!size) {
        gasneti_fatalerror("ERROR: Failure to determine the max pinnable memory.  IBV may be misconfigured.");
      }
      gasnetc_pin_info.memory = size;
    }
    gasnetc_bootstrapExchange_ib(&gasnetc_pin_info, sizeof(gasnetc_pin_info_t), all_info);
    if (! quiet && ! gasneti_mynode) {
      // If warned above, or too slow, print the results and what to do with them.
      // We define "too slow" as 10s + log2(nodes) * 5s.
      double elapsed = 1.e-9 * gasneti_ticks_to_ns(gasneti_ticks_now() - start_time);
      double too_slow = 10.;
      for (gex_Rank_t i = gasneti_nodes/2; i; i >>= 1) too_slow += 5.;
      if (did_warn || (elapsed > too_slow)) {
        gasnetc_physmem_report(elapsed, all_info);
      }
    }
#if GASNET_ALIGNED_SEGMENTS  /* Just a waste of time otherwise */
    if (gasneti_mynode != first_local) {
      /* Extra mmap traffic to ensure compatible VM spaces */
      gasnetc_fakepin(all_info[first_local].memory, step);
    }
    gasnetc_bootstrapBarrier_ib(); /* Ensure fakepin completes unmap before continuing */
#endif
  } else {
    /* Note that README says PHYSMEM_NOPROBE must be equal on all nodes */
    gasnetc_pin_info.memory = limit;
    gasnetc_bootstrapExchange_ib(&gasnetc_pin_info, sizeof(gasnetc_pin_info_t), all_info);
  }

  /* Determine the global values (min of maxes) from the local values */
  for (i = 0; i < gasneti_nodes; i++) {
    gasnetc_pin_info_t *info = &all_info[i];

    limit = MIN(limit, info->memory);
    info->memory = GASNETI_PAGE_ALIGNDOWN(info->memory / info->num_local);
    info->regions /= info->num_local;

    gasnetc_pin_info.memory  = MIN(gasnetc_pin_info.memory,  info->memory );
    gasnetc_pin_info.regions = MIN(gasnetc_pin_info.regions, info->regions);
  }
  gasneti_free(all_info);

  gasnetc_physmem_check("Probing O/S limits and HCA capabilities", limit);
}

GASNETI_NORETURN
static void gasneti_segreg_failed(size_t size, const char *which, int why) {
  const char *hint1 = "";
  const char *hint2 = "";
#if !GASNETI_PSHM_POSIX
  // N/A
#elif PLATFORM_OS_LINUX || PLATFORM_OS_CNL || PLATFORM_OS_WSL
  #define GASNETC_PSHM_FS "/dev/shm"
#elif PLATFORM_OS_NETBSD
  #define GASNETC_PSHM_FS "/var/shm"
#elif PLATFORM_OS_FREEBSD || PLATFORM_OS_OPENBSD
  #define GASNETC_PSHM_FS "/tmp"
#else
  // Cygwin, macOS and Solaris are not believed to back with a filesystem
  // Others are unknown
#endif
#ifdef GASNETC_PSHM_FS
  if (why == EFAULT) {
    hint1 = "\n        This could be caused by insufficient space in " GASNETC_PSHM_FS " (or similar).";
  }
#endif
  if (! *which) { // empty string == NOT " aux"
    hint2 = "\n        Reducing the value of environment variable GASNET_MAX_SEGSIZE may help.";
  }
  char sizestr[16];
  gasneti_fatalerror("Unexpected error %s (errno=%d) when registering a %s%s segment%s%s",
                     strerror(why), why,
                     gasnett_format_number(size, sizestr, sizeof(sizestr), 1),
                     which, hint1, hint2);
}

//
// simple container of segments
//
static gasnetc_Segment_t *gasnetc_segment_table = NULL;
static int gasnetc_segment_count = 0;
static gasneti_mutex_t gasnetc_segment_lock = GASNETI_MUTEX_INITIALIZER;

static void gasnetc_add_segment(gasnetc_Segment_t seg) {
  gasneti_mutex_lock(&gasnetc_segment_lock);
  seg->idx = gasnetc_segment_count++;
  size_t space = gasnetc_segment_count * sizeof(gasnetc_Segment_t);
  gasnetc_segment_table = gasneti_realloc(gasnetc_segment_table, space);
  gasnetc_segment_table[seg->idx] = seg;
  gasneti_mutex_unlock(&gasnetc_segment_lock);
}
static void gasnetc_del_segment(gasnetc_Segment_t seg) {
  gasneti_mutex_lock(&gasnetc_segment_lock);
  gasnetc_Segment_t last = gasnetc_segment_table[gasnetc_segment_count--];
  last->idx = seg->idx;
  gasnetc_segment_table[last->idx] = last;
  // lack of realloc to shrink is harmless
  gasneti_mutex_unlock(&gasnetc_segment_lock);
}

#if GASNET_TRACE
static const char *mtu_to_str(enum ibv_mtu mtu) {
  switch (mtu) {
  case IBV_MTU_256 : return "256";
  case IBV_MTU_512 : return "512";
  case IBV_MTU_1024: return "1024";
  case IBV_MTU_2048: return "2048";
  case IBV_MTU_4096: return "4096";
  default: return (int)mtu ? "unknown" : "active_mtu";
  }
}
#endif

/* Process defaults and the environment to get configuration settings */
static int gasnetc_load_settings(void) {
  const char *tmp;
  int i;

  tmp =  gasneti_getenv("GASNET_HCA_ID");
  if (tmp && strlen(tmp)) {
    fprintf(stderr, "WARNING: GASNET_HCA_ID set in environment, but ignored.  See gasnet/ibv-conduit/README.\n");
  }
  tmp =  gasneti_getenv("GASNET_PORT_NUM");
  if (tmp && strlen(tmp) && atoi(tmp)) {
    fprintf(stderr, "WARNING: GASNET_PORT_NUM set in environment, but ignored.  See gasnet/ibv-conduit/README.\n");
  }

  gasnetc_ibv_ports = gasneti_getenv_withdefault("GASNET_IBV_PORTS", GASNETC_DEFAULT_IBV_PORTS);

  #define GASNETC_ENVINT(program_var, env_key, default_val, minval, is_mem) do {     \
      int64_t _tmp = gasneti_getenv_int_withdefault(#env_key, default_val, is_mem);  \
      if (_tmp < minval)                                                             \
        GASNETI_RETURN_ERRR(BAD_ARG, "("#env_key" < "#minval") in environment");     \
      program_var = _tmp;                                                            \
    } while (0)
  
  GASNETC_ENVINT(i, GASNET_MAX_MTU, 0, -1, 1);
  switch (i) {
    default: fprintf(stderr,
                     "WARNING: ignoring invalid GASNET_MAX_MTU value %d.\n",
                     i);
             /* fall through to "auto" case: */ GASNETI_FALLTHROUGH
  case    0: /* TODO: "automatic" might be more sophisticated than using the maximum */
  case   -1: gasnetc_max_mtu = (enum ibv_mtu)0; /* Use port's active_mtu */
             break;
  case  256: gasnetc_max_mtu = IBV_MTU_256;
             break;
  case  512: gasnetc_max_mtu = IBV_MTU_512;
             break;
  case 1024: gasnetc_max_mtu = IBV_MTU_1024;
             break;
  case 2048: gasnetc_max_mtu = IBV_MTU_2048;
             break;
  case 4096: gasnetc_max_mtu = IBV_MTU_4096;
             break;
  }

  GASNETC_ENVINT(gasnetc_qp_timeout, GASNET_QP_TIMEOUT, GASNETC_DEFAULT_QP_TIMEOUT, 0, 0);
  if_pf (gasnetc_qp_timeout > 31) {
    fprintf(stderr,
            "WARNING: GASNET_QP_TIMEOUT reduced from the requested value, %d, to the maximum supported value 31.\n", (int)gasnetc_qp_timeout);
    gasnetc_qp_timeout = 31;
  }
  GASNETC_ENVINT(gasnetc_qp_retry_count, GASNET_QP_RETRY_COUNT, GASNETC_DEFAULT_QP_RETRY_COUNT, 1, 0);
  if_pf (gasnetc_qp_retry_count > 7) {
    fprintf(stderr,
            "WARNING: GASNET_QP_RETRY_COUNT reduced from the requested value, %d, to the maximum supported value 7.\n", (int)gasnetc_qp_retry_count);
    gasnetc_qp_retry_count = 7;
  }
  GASNETC_ENVINT(gasnetc_qp_rd_atom, GASNET_QP_RD_ATOM, 0, 0, 0);
  GASNETC_ENVINT(gasnetc_op_oust_pp, GASNET_NETWORKDEPTH_PP, GASNETC_DEFAULT_NETWORKDEPTH_PP, 1, 0);
  GASNETC_ENVINT(gasnetc_op_oust_limit, GASNET_NETWORKDEPTH_TOTAL, GASNETC_DEFAULT_NETWORKDEPTH_TOTAL, 0, 0);
  GASNETC_ENVINT(gasnetc_am_oust_pp, GASNET_AM_CREDITS_PP, GASNETC_DEFAULT_AM_CREDITS_PP, 1, 0);
  GASNETC_ENVINT(gasnetc_am_oust_limit, GASNET_AM_CREDITS_TOTAL, GASNETC_DEFAULT_AM_CREDITS_TOTAL, 0, 0);
  GASNETC_ENVINT(gasnetc_am_credits_slack, GASNET_AM_CREDITS_SLACK, GASNETC_DEFAULT_AM_CREDITS_SLACK, 0, 0);
  GASNETC_ENVINT(gasnetc_bbuf_limit, GASNET_BBUF_COUNT, GASNETC_DEFAULT_BBUF_COUNT, 0, 0);
#if GASNETC_IBV_SRQ
  gasnetc_rbuf_set = (NULL != gasneti_getenv("GASNET_RBUF_COUNT"));
  GASNETC_ENVINT(gasnetc_rbuf_limit, GASNET_RBUF_COUNT, GASNETC_DEFAULT_RBUF_COUNT, 0, 0);
#endif
  GASNETC_ENVINT(gasnetc_num_qps, GASNET_NUM_QPS, GASNETC_DEFAULT_NUM_QPS, 0, 0);
  GASNETC_ENVINT(gasnetc_inline_limit, GASNET_INLINESEND_LIMIT, GASNETC_DEFAULT_INLINESEND_LIMIT, -1, 0);
  GASNETC_ENVINT(gasnetc_bounce_limit, GASNET_NONBULKPUT_BOUNCE_LIMIT, GASNETC_DEFAULT_NONBULKPUT_BOUNCE_LIMIT, 0, 1);
  GASNETC_ENVINT(gasnetc_packedlong_limit, GASNET_PACKEDLONG_LIMIT, GASNETC_DEFAULT_PACKEDLONG_LIMIT, 0, 1);
  GASNETC_ENVINT(gasnetc_am_gather_min, GASNET_AM_GATHER_MIN, GASNETC_DEFAULT_AM_GATHER_MIN, -1, 1);
  if (gasnetc_am_gather_min == -1) {
    // -1 is the documented value to disable this optimization
    gasnetc_am_gather_min = INT_MAX;
  }
  GASNETC_ENVINT(gasnetc_put_stripe_sz, GASNET_PUT_STRIPE_SZ, GASNETC_DEFAULT_PUT_STRIPE_SZ, 0, 1024);
  GASNETC_ENVINT(gasnetc_get_stripe_sz, GASNET_GET_STRIPE_SZ, GASNETC_DEFAULT_GET_STRIPE_SZ, 0, 1024);

  #if !GASNETC_PIN_SEGMENT
    GASNETC_ENVINT(gasnetc_putinmove_limit, GASNET_PUTINMOVE_LIMIT, GASNETC_DEFAULT_PUTINMOVE_LIMIT, 0, 1);
    if_pf (gasnetc_putinmove_limit > GASNETC_PUTINMOVE_LIMIT_MAX) {
      gasneti_fatalerror("GASNET_PUTINMOVE_LIMIT (%"PRIuPTR") is larger than the max permitted (%"PRIuPTR")", (uintptr_t)gasnetc_putinmove_limit, (uintptr_t)GASNETC_PUTINMOVE_LIMIT_MAX);
    }
  #endif
  gasnetc_use_rcv_thread = gasneti_getenv_yesno_withdefault("GASNET_RCV_THREAD", 0);

  /* Verify correctness/sanity of values */
  if (gasnetc_use_rcv_thread && !GASNETC_USE_RCV_THREAD) {
    gasneti_fatalerror("AM receive thread enabled by environment variable GASNET_RCV_THREAD, but was disabled at GASNet build time");
  }
#if GASNETC_FH_OPTIONAL
  gasnetc_use_firehose = gasneti_getenv_yesno_withdefault("GASNET_USE_FIREHOSE", 1);
  if (!GASNETC_PIN_SEGMENT && !gasnetc_use_firehose) {
    gasneti_fatalerror("Use of the 'firehose' dynamic pinning library disabled by environment variable GASNET_USE_FIREHOSE, but is required in a GASNET_SEGMENT_" _STRINGIFY(GASNETI_SEGMENT_CONFIG) " configuration");
  }
#else
  if (!gasneti_getenv_yesno_withdefault("GASNET_USE_FIREHOSE", 1)) {
    fprintf(stderr,
	    "WARNING: Environment variable GASNET_USE_FIREHOSE ignored.  It is only available in a DEBUG build of GASNet\n");
  }
#endif
#if GASNETC_IBV_SRQ
  /* Integer value becomes a boolean, though possibly not until gasnetc_sndrcv_init() */
  gasnetc_use_srq = gasneti_getenv_int_withdefault("GASNET_USE_SRQ", -1, 0);
  /* My disabling SRQ early when possible we can avoid allocating resources we might not use.
     XXX: If we can, we should move AM resource computation here so we can avoid ever
          allocating unused resources.
     As per README:
     GASNET_USE_SRQ = 0: Never use SRQ
     GASNET_USE_SRQ > 0: Use SRQ for nodes >= GASNET_USE_SRQ (so 1 == Always)
     GASNET_USE_SRQ < 0: Use SRQ is memory savings would result (done in gasnetc_sndrcv_init()).
   */
  if (gasnetc_use_srq > (int)gasneti_nodes) {
    /* Positive value of GASNET_USE_SRQ denotes a minumum node count */
    GASNETI_TRACE_PRINTF(I, ("SRQ disabled because GASNET_USE_SRQ = %d is greater than nodes = %d",
                             gasnetc_use_srq, gasneti_nodes));
    gasnetc_use_srq = 0;
  }
#endif
#if GASNETC_IBV_XRC
  gasnetc_use_xrc = gasneti_getenv_int_withdefault("GASNET_USE_XRC", 1, 0);
#endif

  /* XXX: Does SRQ make any of these invalid? */
  if_pf (gasnetc_am_credits_slack >= gasnetc_am_oust_pp) {
    fprintf(stderr,
            "WARNING: GASNET_AM_CREDITS_SLACK reduced to GASNET_AM_CREDITS_PP-1 (from %d to %d)\n",
            gasnetc_am_credits_slack, gasnetc_am_oust_pp-1);
    gasnetc_am_credits_slack = gasnetc_am_oust_pp - 1;
  }
  if_pf (gasnetc_packedlong_limit > GASNETC_MAX_PACKEDLONG) {
    fprintf(stderr,
            "WARNING: GASNET_PACKEDLONG_LIMIT reduced from %u to %u\n",
            (unsigned int)gasnetc_packedlong_limit, GASNETC_MAX_PACKEDLONG);
    gasnetc_packedlong_limit = GASNETC_MAX_PACKEDLONG;
  }


  /* Report */
  GASNETI_TRACE_PRINTF(I,("ibv-conduit build time configuration settings = {"));
  GASNETI_TRACE_PRINTF(I,("  AM receives in internal thread %sabled (GASNETC_IBV_RCV_THREAD)",
				GASNETC_USE_RCV_THREAD ? "en" : "dis"));
#if GASNETC_IBV_POLL_LOCK
  GASNETI_TRACE_PRINTF(I,("  Serialized CQ polls            YES (--enable-ibv-poll-lock)"));
#else
  GASNETI_TRACE_PRINTF(I,("  Serialized CQ polls            NO (default)"));
#endif
  GASNETI_TRACE_PRINTF(I,("  Max. snd completions per poll  %d (GASNETC_SND_REAP_LIMIT)",
				GASNETC_SND_REAP_LIMIT));
  GASNETI_TRACE_PRINTF(I,("  Max. rcv completions per poll  %d (GASNETC_RCV_REAP_LIMIT)",
				GASNETC_RCV_REAP_LIMIT));
  GASNETI_TRACE_PRINTF(I,  ("}"));

  GASNETI_TRACE_PRINTF(I,("ibv-conduit run time configuration settings = {"));
  if (gasnetc_ibv_ports && strlen(gasnetc_ibv_ports)) {
    GASNETI_TRACE_PRINTF(I,  ("  GASNET_IBV_PORTS                = '%s'", gasnetc_ibv_ports));
  } else {
    GASNETI_TRACE_PRINTF(I,  ("  GASNET_IBV_PORTS                = empty or unset (probe all)"));
  }
  if (gasnetc_num_qps) {
    GASNETI_TRACE_PRINTF(I,  ("  GASNET_NUM_QPS                  = %d", gasnetc_num_qps));
  } else {
    GASNETI_TRACE_PRINTF(I,  ("  GASNET_NUM_QPS                  = 0 (automatic)"));
  }
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_NETWORKDEPTH_PP          = %d", gasnetc_op_oust_pp));
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_NETWORKDEPTH_TOTAL       = %d%s",
			  	gasnetc_op_oust_limit, gasnetc_op_oust_limit ? "" : " (automatic)"));
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_AM_CREDITS_PP            = %d", gasnetc_am_oust_pp));
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_AM_CREDITS_TOTAL         = %d%s",
			  	gasnetc_am_oust_limit, gasnetc_am_oust_limit ? "" : " (automatic)"));
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_AM_CREDITS_SLACK         = %d", gasnetc_am_credits_slack));
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_BBUF_COUNT               = %d%s",
			  	gasnetc_bbuf_limit, gasnetc_bbuf_limit ? "": " (automatic)"));
#if GASNETC_IBV_SRQ
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_USE_SRQ                  = %d", gasnetc_use_srq));
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_RBUF_COUNT               = %d%s",
			  	gasnetc_rbuf_limit, gasnetc_rbuf_limit ? "": " (automatic)"));
#endif
#if GASNETC_IBV_XRC
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_USE_XRC                  = %d", gasnetc_use_xrc));
#endif
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_INLINESEND_LIMIT         = %d%s", (int)gasnetc_inline_limit,
				(gasnetc_inline_limit == (size_t)-1 ? " (automatic)" : "")));
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_NONBULKPUT_BOUNCE_LIMIT  = %u", (unsigned int)gasnetc_bounce_limit));
#if !GASNETC_PIN_SEGMENT
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_PUTINMOVE_LIMIT          = %u", (unsigned int)gasnetc_putinmove_limit));
#endif
#if GASNETC_USE_RCV_THREAD
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_RCV_THREAD               = %d (%sabled)", gasnetc_use_rcv_thread,
				gasnetc_use_rcv_thread ? "en" : "dis"));
#else
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_RCV_THREAD               disabled at build time"));
#endif
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_QP_TIMEOUT               = %d (%g sec)", gasnetc_qp_timeout, 4.096e-6*(1<<gasnetc_qp_timeout)));
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_QP_RETRY_COUNT           = %d", gasnetc_qp_retry_count));
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_QP_RD_ATOM               = %d%s", gasnetc_qp_rd_atom,
				!gasnetc_qp_rd_atom ? " (automatic)" : ""));
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_MAX_MTU                  = %s", mtu_to_str(gasnetc_max_mtu)));
  GASNETI_TRACE_PRINTF(I,  ("}"));

  gasnetc_exittimeout = gasneti_get_exittimeout(GASNETC_DEFAULT_EXITTIMEOUT_MAX,
						GASNETC_DEFAULT_EXITTIMEOUT_MIN,
						GASNETC_DEFAULT_EXITTIMEOUT_FACTOR,
						GASNETC_DEFAULT_EXITTIMEOUT_MIN);

  return GASNET_OK;
}

/* Info used while probing for HCAs/ports */
typedef struct gasnetc_port_list_ {
  struct gasnetc_port_list_	*next;
  char				*id;
  int				port;
  int				matched;
} gasnetc_port_list_t;

static gasnetc_port_list_t *gasnetc_port_list = NULL;

static void
gasnetc_add_port(char *id, int port) {
  gasnetc_port_list_t *result = gasneti_malloc(sizeof(gasnetc_port_list_t));
  result->next = gasnetc_port_list;
  gasnetc_port_list = result;
  result->id = id;
  result->port = port;
  result->matched = 0;
}

static int
gasnetc_match_port(const char *id, int port, int mark) {
  gasnetc_port_list_t *curr;
  int found;

  if (gasnetc_port_list != NULL) {
    found = 0;
    for (curr = gasnetc_port_list; curr && !found; curr = curr->next) {
      if (!curr->matched && !strcmp(id, curr->id)) {
	if (!port) { /* match HCA only */
	  found = 1;
	} else if (!curr->port || (curr->port == port)) { /* 0 is a wild card */
	  found = 1;
	  curr->matched = mark;
        }
      }
    }
  } else {
    found = 1;
  }

  return found;
}

static void
gasnetc_clear_ports(void) {
  gasnetc_port_list_t *curr;

  curr = gasnetc_port_list;
  while(curr) {
    gasnetc_port_list_t *next = curr->next;
    if (curr->matched == 0) {
      if (curr->port) {
        GASNETI_TRACE_PRINTF(C,("Probe left element '%s:%d' of GASNET_IBV_PORTS unused", curr->id, curr->port));
      } else {
        GASNETI_TRACE_PRINTF(C,("Probe left element '%s' of GASNET_IBV_PORTS unused", curr->id));
      }
    }
    gasneti_free(curr->id);
    gasneti_free(curr);
    curr = next;
  }
  gasnetc_port_list = NULL;
}

/* Return length of initial substring of S not containing C,
 * limited to LEN or unlimited if LEN is 0.
 */
static int
gasnetc_scan(const char *s, int c, int len) {
  int i;
  for (i = 0; !len || (i < len); ++i) {
    if ((s[i] == '\0') || (s[i] == c)) return i;
  }
  return len;
}

static int
gasnetc_parse_ports(const char *p) {
  int len = strlen(p);
  while (len > 0) {
    int a = gasnetc_scan(p, '+', len);
    int b = gasnetc_scan(p, ':', a);
    char *hca = gasneti_strndup(p, b);
    if (b >= a-1) {
      gasnetc_add_port(gasneti_strndup(p, b), 0);
    } else {
      const char *r = p + b + 1;
      while (r < p + a) {
	int port = atoi(r);
	if (port) {
	  gasnetc_add_port(gasneti_strndup(p, b), port);
	  r += gasnetc_scan(r, ',', 0) + 1;
	} else {
          gasneti_free(hca);
	  return 1;
	}
      }
    }
    gasneti_free(hca);
    p += a + 1;
    len -= a + 1;
  }

  return 0;
}

static const char* port_state_name(enum ibv_port_state port_state)
{
  switch (port_state) {
    case IBV_PORT_DOWN:  return "DOWN";
    case IBV_PORT_INIT:  return "INITIALIZE";
    case IBV_PORT_ARMED: return "ARMED";
    default:             return "unknown";
  }
}

static int16_t get_pkey(void)
{
  static int16_t pkey = -1;
  static int first = 1;
  if (first) {
    first = 0;
    int64_t tmp = gasnett_getenv_int_withdefault("GASNET_IBV_PKEY", -1, 0);
    if (tmp == -1) {
      // Nothing to do
    } else {
      tmp &= ~(uint64_t)0x8000;  // strip membership bit (but not sign bit)
      if ((tmp > 0x7fff) || (tmp < 2)) {
        gasneti_fatalerror("Invalid GASNET_IBV_PKEY '%s'", gasnett_getenv("GASNET_IBV_PKEY"));
      }
    }
    pkey = (int16_t)tmp;
  }
  return pkey;
}

// Print the ports
static void gasnetc_list_ports(void) {
  gasneti_console_message("INFO", "Detected the following devices:ports");

  struct ibv_device **hca_list;
  int num_hcas = 0;
  hca_list = ibv_get_device_list(&num_hcas);

  int good_count = 0;
  for (int hca_num = 0; hca_num < num_hcas; ++hca_num) {
    const char *hca_name = ibv_get_device_name(hca_list[hca_num]);

#if HAVE_IBV_TRANSPORT_TYPE
    if (hca_list[hca_num]->transport_type != IBV_TRANSPORT_IB) {
      gasneti_console_message("INFO", "    %s BAD - identifies as NON InfiniBand device\n", hca_name);
      continue;
    }
#endif
    struct ibv_context *hca_handle = ibv_open_device(hca_list[hca_num]);
    if (! hca_handle) {
      gasneti_console_message("INFO", "    %s BAD - failed to open device\n", hca_name);
      continue;
    }

    struct ibv_device_attr hca_attr;
    if (ibv_query_device(hca_handle, &hca_attr)) {
      gasneti_console_message("INFO", "    %s BAD - failed to query device capabilities\n", hca_name);
      (void) ibv_close_device(hca_handle);
      continue;
    }

    // Loop over ports on the HCA (numbering starts at 1)
    for (int port_num = 1; port_num <= hca_attr.phys_port_cnt; ++port_num) {
      struct ibv_port_attr port_attr;
      if (ibv_query_port(hca_handle, port_num, &port_attr)) {
        gasneti_console_message("INFO", "    %s:%d BAD - failed to query port capabilities\n", hca_name, port_num);
        continue;
      }
      if (port_attr.state != IBV_PORT_ACTIVE) {
        gasneti_console_message("INFO", "    %s:%d BAD - reports state=%s\n",
                                hca_name, port_num, port_state_name(port_attr.state));
        continue;
      }
      if (!port_attr.lid) {
        gasneti_console_message("INFO", "    %s:%d BAD - reports LID=0\n", hca_name, port_num);
        continue;
      }

      int16_t pkey = get_pkey();
      if (pkey >= 0) {
        int idx = -1;
        for (int i = 0; i < hca_attr.max_pkeys; ++i) {
          uint16_t pkey_val;
          if (ibv_query_pkey(hca_handle, port_num, i, &pkey_val)) {
            gasneti_console_message("INFO", "    %s:%d BAD - failed to query pkeys\n", hca_name, port_num);
            idx = i;
            break;
          }
          if (pkey == (ntohs(pkey_val) & 0x7fff)) {
            idx = i;
            break;
          }
        }
        if (idx < 0) {
          gasneti_console_message("INFO", "    %s:%d BAD - not associated with user-specified pkey 0x%x\n",
                                  hca_name, port_num, (unsigned int)pkey);
          continue;
        }
      }

      gasneti_console_message("INFO", "    %s:%d GOOD\n", hca_name, port_num);
      ++good_count;
    }
    (void) ibv_close_device(hca_handle);
  }
  if (good_count) {
    gasneti_console_message("INFO", "Found %d potentially usable InfiniBand ports\n", good_count);
  } else if (num_hcas) {
    gasneti_console_message("INFO", "Found %d devices, but no usable InfiniBand ports\n", num_hcas);
  } else {
    gasneti_console_message("INFO", "No IBV-compatible devices found\n");
  }
}

/* Try to find up to *port_count_p ACTIVE ports, replacing w/ the actual count */
static void gasnetc_probe_ports(int max_ports) {
  struct ibv_device	**hca_list;
  int			num_hcas;
  int			ib_hcas;
  gasnetc_port_info_t	*port_tbl;
  int			port_count = 0;
  int			hca_count = 0;
  int			curr_hca;

  if (gasneti_getenv_yesno_withdefault("GASNET_IBV_LIST_PORTS", 0) &&
      gasneti_check_node_list("GASNET_IBV_LIST_PORTS_NODES")) {
    gasnetc_list_ports();
  }

  if (gasnetc_parse_ports(gasnetc_ibv_ports)) {
    GASNETI_TRACE_PRINTF(C,("Failed to parse GASNET_IBV_PORTS='%s'", gasnetc_ibv_ports));
    gasnetc_clear_ports();
    return;
  }

  if (max_ports) {
    GASNETI_TRACE_PRINTF(C,("Probing HCAs for active ports (max %d)", max_ports));
  } else {
    GASNETI_TRACE_PRINTF(C,("Probing HCAs for active ports"));
    max_ports = 128;	/* If you have more than 128 IB ports per node, then I owe you $20 :-) */
  }

  port_tbl = gasneti_calloc(max_ports, sizeof(gasnetc_port_info_t));
  num_hcas = 0; /* call will overwrite with actual count */
  hca_list = ibv_get_device_list(&num_hcas);
  if ((hca_list == NULL) || (num_hcas == 0)) {
    GASNETI_TRACE_PRINTF(C,("Probe failed to locate any HCAs"));
    gasnetc_clear_ports();
    return;
  }

  ib_hcas = num_hcas;
#if HAVE_IBV_TRANSPORT_TYPE
  for (curr_hca = 0; curr_hca < num_hcas; ++curr_hca) {
    ib_hcas -= (hca_list[curr_hca]->transport_type != IBV_TRANSPORT_IB);
  }
#endif

  if ((ib_hcas > GASNETC_IB_MAX_HCAS) && (gasnetc_port_list == NULL)) {
#if GASNETC_IBV_MAX_HCAS
    const char *current = "with '--with-ibv-max-hcas=" _STRINGIFY(GASNETC_IB_MAX_HCAS) "'";
    const char *enable = "";
#else
    const char *current = "without multi-rail support";
    const char *enable = "--enable-ibv-multirail ";
#endif
    fprintf(stderr, "WARNING: Found %d IB HCAs, but GASNet was configured %s.  "
		    "To utilize all your HCAs, you should "
		    "reconfigure GASNet with '%s--with-ibv-max-hcas=%d'.  You can silence this warning "
		    "by setting the environment variable GASNET_IBV_PORTS as described in the file "
		    "'gasnet/ibv-conduit/README'.\n", num_hcas, current, enable, num_hcas);
  }

  int16_t pkey = get_pkey();

  /* Loop over list of HCAs */
  for (curr_hca = 0;
       (hca_count < GASNETC_IB_MAX_HCAS) && (port_count < max_ports) && (curr_hca < num_hcas);
       ++curr_hca) {
    const char *hca_name = ibv_get_device_name(hca_list[curr_hca]);
    int rc;
    struct ibv_context *	hca_handle;
    struct ibv_device_attr	hca_cap;
    int found = 0;
    int curr_port;

#if HAVE_IBV_TRANSPORT_TYPE
    if (hca_list[curr_hca]->transport_type != IBV_TRANSPORT_IB) {
      GASNETI_TRACE_PRINTF(C,("Probe skipping non-InfiniBand HCA '%s'", hca_name));
      continue;
    }
#endif

    if (!gasnetc_match_port(hca_name, 0, 0)) {
      GASNETI_TRACE_PRINTF(C,("Probe skipping HCA '%s' - no match in GASNET_IBV_PORTS", hca_name));
      continue;
    }

    hca_handle = ibv_open_device(hca_list[curr_hca]);
    if (hca_handle != NULL) {
      GASNETI_TRACE_PRINTF(C,("Probe found HCA '%s'", hca_name));
    } else {
      GASNETI_TRACE_PRINTF(C,("Probe failed to open HCA '%s'", hca_name));
      continue;	/* OK, keep trying HCAs */
    }
    rc = ibv_query_device(hca_handle, &hca_cap);
    GASNETC_IBV_CHECK(rc, "from ibv_query_device()");

    /* Loop over ports on the HCA (they start numbering at 1) */
    for (curr_port = 1;
	 (port_count < max_ports) && (curr_port <= hca_cap.phys_port_cnt);
	 ++curr_port) {
      gasnetc_port_info_t *this_port = &port_tbl[port_count];

      if (!gasnetc_match_port(hca_name, curr_port, 0)) {
        GASNETI_TRACE_PRINTF(C,("Probe skipping HCA '%s', port %d - no match in GASNET_IBV_PORTS", hca_name, curr_port));
	continue;
      }

      (void) ibv_query_port(hca_handle, curr_port, &this_port->port);

      if (!this_port->port.lid) {
        GASNETI_TRACE_PRINTF(C,("Probe skipping HCA '%s', port %d - LID is zero", hca_name, curr_port));
	continue;
      }

      if (this_port->port.state == IBV_PORT_ACTIVE) {
        ++port_count;
        ++found;
        this_port->port_num = curr_port;
        this_port->hca_index = hca_count;
        if (pkey < 0) {
          GASNETI_TRACE_PRINTF(C,("Using default pkey_index=0 for HCA '%s', port %d",
                                  hca_name, curr_port));
          this_port->pkey_index = 0;
        } else {
          int i;
          for (i = 0; i < hca_cap.max_pkeys; ++i) {
            uint16_t pkey_val;
            if (ibv_query_pkey(hca_handle, curr_port, i, &pkey_val)) {
              gasneti_fatalerror("Failed to query pkeys for HCA '%s', port %d", hca_name, curr_port);
            }
            pkey_val = ntohs(pkey_val) & 0x7fff;
            if (pkey_val == pkey) {
              GASNETI_TRACE_PRINTF(C,("Using pkey_index %d for HCA '%s', port %d",
                                      i, hca_name, curr_port));
              this_port->pkey_index = i;
              break;
            }
          }
          if (i == hca_cap.max_pkeys) {
            gasneti_fatalerror("Failed to locate index of requested pkey 0x%04x for HCA '%s', port %d",
                               (unsigned int)pkey, hca_name, curr_port);
          }
        }
        if (gasnetc_qp_rd_atom) { /* Zero means use HCA/port limit */
          int limit = MIN(hca_cap.max_qp_init_rd_atom, hca_cap.max_qp_rd_atom);
          if (gasnetc_qp_rd_atom > limit) {
	    fprintf(stderr,
		"WARNING: Requested GASNET_QP_RD_ATOM %d reduced to HCA limit %d\n",
		gasnetc_qp_rd_atom, limit);
            gasnetc_qp_rd_atom = limit;
          }
          this_port->rd_atom = gasnetc_qp_rd_atom;
        } else {
          this_port->rd_atom = MIN(hca_cap.max_qp_init_rd_atom, hca_cap.max_qp_rd_atom);
        }
        GASNETI_TRACE_PRINTF(C,("Probe found HCA '%s', port %d", hca_name, curr_port));
        (void)gasnetc_match_port(hca_name, curr_port, 1);
	if (gasnetc_port_list == NULL) {
	  /* By default one at most 1 port per HCA */
	  break;
	}
      }
#if GASNET_TRACE
      else {
        GASNETI_TRACE_PRINTF(C,("Probe skipping HCA '%s', port %d - state = %s",
                                hca_name, curr_port, port_state_name(this_port->port.state)));
      }
#endif
    }

    /* Install or release the HCA */
    if (found) {
      gasnetc_hca_t *hca = &gasnetc_hca[hca_count];

      if (gasneti_getenv_yesno_withdefault("GASNET_IBV_MODEL_WARN", 1)) {
        if (!strncmp(hca_name, "hfi1_", 5)) {
          fprintf(stderr,
                  "WARNING: Use of ibv-conduit with Omni-Path NIC %s is not recommended.\n"
                  "         See GASNet's ibv-conduit README for more information.\n"
                  "         Alternatively, you may set environment variable\n"
                  "         GASNET_IBV_MODEL_WARN=0 to silence this message.\n",
                  hca_name);
        }
      }

      memset(hca, 0, sizeof(gasnetc_hca_t));
      hca->handle	= hca_handle;
      hca->hca_index	= hca_count;
      hca->hca_id	= gasneti_strdup(hca_name);
      hca->hca_cap	= hca_cap;
      gasneti_leak((/*non-const*/ void *)hca->hca_id);

      hca_count++;
    } else {
      (void) ibv_close_device(hca_handle);
    }
  }
  GASNETI_TRACE_PRINTF(C,("Probe found %d active port(s) on %d HCA(s)", port_count, hca_count));
  gasnetc_clear_ports();
  ibv_free_device_list(hca_list);

  gasnetc_num_hcas = hca_count;
  gasnetc_num_ports = port_count;
  gasnetc_port_tbl  = gasneti_realloc(port_tbl, port_count * sizeof(gasnetc_port_info_t));
  gasneti_leak(gasnetc_port_tbl);
}

static int gasnetc_hca_report(void) {
  gasnetc_hca_t         *hca;
  int                   h, i;

  /* Report/check hca and port properties */
  GASNETC_FOR_ALL_HCA_INDEX(h) {
    hca = &gasnetc_hca[h];
    GASNETI_TRACE_PRINTF(I,("ibv-conduit HCA properties (%d of %d) = {", h+1, gasnetc_num_hcas));
    GASNETI_TRACE_PRINTF(I,("  HCA id                   = '%s'", hca->hca_id));
    GASNETI_TRACE_PRINTF(I,("  HCA vendor id            = 0x%x", (unsigned int)hca->hca_cap.vendor_id));
    GASNETI_TRACE_PRINTF(I,("  HCA vendor part id       = 0x%x", (unsigned int)hca->hca_cap.vendor_part_id));
    GASNETI_TRACE_PRINTF(I,("  HCA hardware version     = 0x%x", (unsigned int)hca->hca_cap.hw_ver));
    GASNETI_TRACE_PRINTF(I,("  HCA firmware version     = %64s", hca->hca_cap.fw_ver));
    GASNETI_TRACE_PRINTF(I,("  max_num_qp               = %u", (unsigned int)hca->hca_cap.max_qp));
    GASNETI_TRACE_PRINTF(I,("  max_qp_ous_wr            = %u", (unsigned int)hca->hca_cap.max_qp_wr));
    GASNETI_TRACE_PRINTF(I,("  max_num_sg_ent           = %u", (unsigned int)hca->hca_cap.max_sge));
    gasneti_assert_always(hca->hca_cap.max_sge >= GASNETC_SND_SG);
    gasneti_assert_always(hca->hca_cap.max_sge >= 1);
    #if 1 /* QP end points */
      GASNETI_TRACE_PRINTF(I,("  max_qp_init_rd_atom      = %u", (unsigned int)hca->hca_cap.max_qp_init_rd_atom));
      gasneti_assert_always(hca->hca_cap.max_qp_init_rd_atom >= 1);	/* RDMA Read support required */
      GASNETI_TRACE_PRINTF(I,("  max_qp_ous_rd_atom       = %u", (unsigned int)hca->hca_cap.max_qp_rd_atom));
      gasneti_assert_always(hca->hca_cap.max_qp_rd_atom >= 1);	/* RDMA Read support required */
    #else
      GASNETI_TRACE_PRINTF(I,("  max_ee_init_rd_atom      = %u", (unsigned int)hca->hca_cap.max_ee_init_rd_atom));
      gasneti_assert_always(hca->hca_cap.max_ee_init_rd_atom >= 1);	/* RDMA Read support required */
      GASNETI_TRACE_PRINTF(I,("  max_ee_ous_rd_atom       = %u", (unsigned int)hca->hca_cap.max_ee_rd_atom));
      gasneti_assert_always(hca->hca_cap.max_ee_rd_atom >= 1);	/* RDMA Read support required */
    #endif
    GASNETI_TRACE_PRINTF(I,("  max_num_cq               = %u", (unsigned int)hca->hca_cap.max_cq));
    gasneti_assert_always(hca->hca_cap.max_cq >= 2);
    GASNETI_TRACE_PRINTF(I,("  max_num_ent_cq           = %u", (unsigned int)hca->hca_cap.max_cqe));
  
  
    GASNETI_TRACE_PRINTF(I,("  max_mr                   = %u", (unsigned int)hca->hca_cap.max_mr));
    GASNETI_TRACE_PRINTF(I,("  max_mr_sz                = %u", (unsigned int)hca->hca_cap.max_mr_size));

    /* Vendor/device-specific firmware checks */
    /* NONE OF OUR KNOWN DEFECTS ARE PRESENT IN IBV-CAPABLE FW */
      
    /* Per-port: */
    for (i = 0; i < gasnetc_num_ports; ++i) {
      if (gasnetc_port_tbl[i].hca_index == h) {
        GASNETI_TRACE_PRINTF(I,("  port %d properties = {", (int)gasnetc_port_tbl[i].port_num));
        GASNETI_TRACE_PRINTF(I,("    LID                      = %u", (unsigned int)gasnetc_port_tbl[i].port.lid));
        GASNETI_TRACE_PRINTF(I,("    max_msg_sz               = %u", (unsigned int)gasnetc_port_tbl[i].port.max_msg_sz));
        GASNETI_TRACE_PRINTF(I,("    active_mtu               = %s", mtu_to_str(gasnetc_port_tbl[i].port.active_mtu)));
        GASNETI_TRACE_PRINTF(I,("  }"));
      }
    }

    GASNETI_TRACE_PRINTF(I,("}")); /* end of HCA report */
  }

  return GASNET_OK;
}

#if GASNETC_IBV_ODP
static void gasneti_odp_init(void) {
  struct gasneti_odp_support {
    uint8_t missing;
    char    hca_id[15];
  } my_odp_support[GASNETC_IB_MAX_HCAS];
  enum gasneti_odp_missing {
    missing_none = 0, // not missing anything == OK
    missing_general,
    missing_implicit,
    missing_xrc_read,
    missing_xrc_write,
    missing_rc_read,
    missing_rc_write
  };
  const char *message[] = {
      "",
      "general ODP",
      "Implicit ODP",
      "XRC READ",
      "XRC WRITE"
      "RC READ",
      "RC WRITE"
  };
  // TODO: support heterogenous multi-rail with some HCAs having ODP and others not.
  // This is not trivial because the RDMA logic picks ODP vs FH before picking an HCA.
  gasnetc_hca_t	*hca;
  GASNETC_FOR_ALL_HCA(hca) {
    enum gasneti_odp_missing missing = missing_none;
    struct ibv_exp_device_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.comp_mask = IBV_EXP_DEVICE_ATTR_ODP | IBV_EXP_DEVICE_ATTR_EXP_CAP_FLAGS;
    int ret = ibv_exp_query_device(hca->handle, &attr);
    if (! (attr.exp_device_cap_flags & IBV_EXP_DEVICE_ODP)) {
      missing = missing_general;
    } else if (! (attr.odp_caps.general_odp_caps & IBV_EXP_ODP_SUPPORT_IMPLICIT)) {
      missing = missing_implicit;
      // TODO-EX: maybe support older systems lacking this bit?
      // Prior to ConnectX-4, implicit ODP was done in s/w and
      //  + This can be identified because this caps bit was not set
      //  + Implicit ODP emulation had 128MB limit
      //  + Implicit ODP was valid for local only (invalid rkey)
    } else if (gasnetc_use_xrc) {
      uint32_t odp_caps = gasnetc_use_xrc ? attr.odp_caps.per_transport_caps.xrc_odp_caps
                                          : attr.odp_caps.per_transport_caps.rc_odp_caps;
      if (! (odp_caps & IBV_EXP_ODP_SUPPORT_READ)) {
        missing = gasnetc_use_xrc? missing_xrc_read : missing_rc_read;
      } else if (! (odp_caps & IBV_EXP_ODP_SUPPORT_WRITE)) {
        missing = gasnetc_use_xrc? missing_xrc_write : missing_rc_write;
      }
    }
    if (missing != missing_none) {
      GASNETI_TRACE_PRINTF(C,("Disabled ODP - %s: %s support is missing.",
                              hca->hca_id, message[(int)missing]));
      gasnetc_use_odp = 0;
      size_t max_len = sizeof(my_odp_support[0].hca_id) - 1;
      strncpy(my_odp_support[hca->hca_index].hca_id, hca->hca_id, max_len);
      my_odp_support[hca->hca_index].hca_id[max_len] = '\0';
    }
    my_odp_support[hca->hca_index].missing = missing;
  }
  if (gasnetc_use_odp) {
    // Create implict ODP registrations (currently only used locally)
    GASNETC_FOR_ALL_HCA(hca) {
      struct ibv_exp_reg_mr_in in;
      memset(&in, 0, sizeof(in));
      in.pd = hca->pd;
      in.exp_access = (enum ibv_exp_access_flags)( IBV_EXP_ACCESS_ON_DEMAND |
                                                   IBV_EXP_ACCESS_LOCAL_WRITE );
      in.length = IBV_EXP_IMPLICIT_MR_SIZE;
      hca->implicit_odp.handle = ibv_exp_reg_mr(&in);
      GASNETC_IBV_CHECK_PTR(hca->implicit_odp.handle, "from ibv_exp_reg_mr(implicit)");
      hca->implicit_odp.lkey = hca->implicit_odp.handle->lkey; // flatten for quick access
    }
  }
  // Results by value of GASNET_ODP_VERBOSE:
  //  0: No output (and no comms either)
  //  1: Reports count of proceses missing support
  //  2: Reports detail, w/ HCA ids IFF multi-hca
  //  3: Reports detail w/ HCA ids unconditionally
  int verbose = gasneti_getenv_int_withdefault("GASNET_ODP_VERBOSE", 1, 0);
  if (verbose) {
    // Will exchange just a single byte when not reporting HCA ids
    // TODO: Only one process reports, so gather to 0 would be sufficient
    int show_ids = (verbose > 2) || (gasnetc_num_hcas > 1);
    size_t exchg_len = show_ids ? gasnetc_num_hcas * sizeof(struct gasneti_odp_support) : 1;
    size_t stride = show_ids ? sizeof(struct gasneti_odp_support) : 1;
    struct gasneti_odp_support *all_odp_support = gasneti_malloc(gasneti_nodes * exchg_len);
    gasneti_bootstrapExchange(my_odp_support, exchg_len, all_odp_support);
    if (! gasneti_mynode) {
      struct gasneti_odp_support *p = all_odp_support;
      // First just count the number of procs w/o OPD
      int non_odp_procs = 0;
      int with_xrc_procs = 0;
      for (gex_Rank_t i = 0; i < gasneti_nodes; ++i) {
        int non_odp = 0;
        int with_xrc = 0;
        GASNETC_FOR_ALL_HCA(hca) {
          non_odp |= (p->missing != missing_none);
          with_xrc |= (p->missing == missing_xrc_read) || (p->missing == missing_xrc_write);
          p = (struct gasneti_odp_support *)((uintptr_t)p + stride);
        }
        non_odp_procs += non_odp;
        with_xrc_procs += with_xrc;
      }
      if (non_odp_procs) {
        int only_xrc_problem = (non_odp_procs == with_xrc_procs);
        const char *why = only_xrc_problem ? "due to conflict with XRC"
                                           : "which are missing support";
        const char *less_msg =
                "         To suppress this message set environment variable\n"
                "         GASNET_ODP_VERBOSE=0 or reconfigure with --disable-ibv-odp\n"
                "         (see ibv-conduit's README for more information).\n";
        const char *more_msg = "";
        if (only_xrc_problem) {
            more_msg =
                "         Alternatively, one may set GASNET_USE_ODP or GASNET_USE_XRC\n"
                "         to '0' to disable the corresponding feature.\n";
        } else if (verbose) {
            more_msg =
                "         To see additional details set environment variable\n"
                "         GASNET_ODP_VERBOSE=2 (or higher).\n";
        }
        // report the summary information (verbose > 0)
        fprintf(stderr,
                "WARNING: ODP disabled on %d of %d processes %s.\n%s%s",
                (int)non_odp_procs, (int)gasneti_nodes, why, less_msg, more_msg);
        // report detailed information (verbose > 1)
        if (verbose > 1) {
          p = all_odp_support;
          for (gex_Rank_t i = 0; i < gasneti_nodes; ++i) {
            GASNETC_FOR_ALL_HCA(hca) {
              if (p->missing != missing_none) {
                const char *msg = message[(int)p->missing];
                if (show_ids) {
                   fprintf(stderr, "    Process %d (hca '%s'): %s support is missing.\n",
                           i, p->hca_id, msg);
                } else {
                   fprintf(stderr, "    Process %d: %s support is missing.\n",
                           i, msg);
                }
              }
              p = (struct gasneti_odp_support *)((uintptr_t)p + stride);
            }
          }
        }
      }
    }
    gasneti_free(all_odp_support);
  }
}

static void gasnetc_odp_dereg(gasnetc_hca_t *hca) {
  struct ibv_mr *handle;
#if PLATFORM_ARCH_32
  handle = (struct ibv_mr *) gasneti_atomic32_swap((gasneti_atomic32_t *) &hca->implicit_odp.handle, 0, 0);
#else
  handle = (struct ibv_mr *) gasneti_atomic64_swap((gasneti_atomic64_t *) &hca->implicit_odp.handle, 0, 0);
#endif
  if (handle) {
    ibv_dereg_mr(handle);
  }
}

// Testing shows that exiting without releasing the implicit ODP registration
// leads to an (eventually fatal!) irreversible system memory leak.
// So, we *must* do this for both normal and abnormal exits.
static void gasnetc_odp_shutdown(void) {
  if (gasnetc_use_odp) {
    gasnetc_hca_t *hca;
    GASNETC_FOR_ALL_HCA(hca) {
      gasnetc_odp_dereg(hca);
    }
  }
}
extern void gasnetc_fatalsignal_cleanup_callback(int sig) {
  // Note: caller has set SIGALRM handler
  alarm(30);
  gasnetc_odp_shutdown();
  alarm(0);
}
#if HAVE_ON_EXIT
  static void gasnetc_odp_on_exit(int exitcode, void *arg) {
    gasnetc_odp_shutdown();
  }
#else
  static void gasnetc_odp_atexit(void) {
    gasnetc_odp_shutdown();
  }
#endif
#endif // GASNETC_IBV_ODP

static int gasnetc_init( gex_Client_t            *client_p,
                         gex_EP_t                *ep_p,
                         const char              *clientName,
                         int                     *argc,
                         char                    ***argv,
                         gex_Flags_t             flags)
{
#if GASNET_PSHM
  void                  *shared_mem;
#endif
  gasnetc_hca_t		*hca;
  uint16_t		*local_lid;
  uint16_t		*remote_lid;
  gex_Rank_t	node;
  int 			i;

  /*  check system sanity */
  gasnetc_check_config();

  if (gasneti_init_done) 
    GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already initialized");

  /* Initialize the bootstrapping support. */
  /* Must come very early to get the global ENV. */
  #if GASNET_DEBUG_VERBOSE
    /* note - can't call trace macros during gasnet_init because trace system not yet initialized */
    fprintf(stderr,"gasnetc_init(): about to spawn...\n"); fflush(stderr);
  #endif
  gasneti_spawner = gasneti_spawnerInit(argc, argv, NULL, &gasneti_nodes, &gasneti_mynode);
  if (!gasneti_spawner) GASNETI_RETURN_ERRR(NOT_INIT, "GASNet job spawn failed");

  gasneti_init_done = 1; /* enable early to allow tracing */

  gasneti_freezeForDebugger();

  /* Must init timers after global env, and preferably before tracing */
  GASNETI_TICKS_INIT();

  /* Now enable tracing of all the following steps */
  gasneti_trace_init(argc, argv);

  /* bootstrapInit may set gasneti_nodes==0 if would overflow 16-bit field */
  if (!gasneti_nodes || (gasneti_nodes > GASNET_MAXNODES)) {
    GASNETI_RETURN_ERRR(RESOURCE, "job size exceeds ibv-conduit capabilities");
  }

  /* Process the environment for configuration/settings */
  i = gasnetc_load_settings();
  if (i != GASNET_OK) {
    return i;
  }

  /* Find the port(s) to use */
  gasnetc_probe_ports(gasnetc_num_qps);
  if (!gasnetc_num_qps) {
    /* Let the probe determine gasnetc_num_qps */
    gasnetc_num_qps = gasnetc_num_ports;
  }
  if (!gasnetc_num_ports || (gasnetc_port_tbl == NULL)) {
    if (gasnetc_ibv_ports && strlen(gasnetc_ibv_ports)) {
      GASNETI_RETURN_ERRR(RESOURCE, "unable to open any HCA ports given in GASNET_IBV_PORTS");
    } else {
      GASNETI_RETURN_ERRR(RESOURCE, "unable to open any HCA ports");
    }
  }

#if GASNETC_IB_MAX_HCAS > 1
  gasnetc_use_fenced_puts = gasneti_getenv_yesno_withdefault("GASNET_USE_FENCED_PUTS", 0);
  if (gasnetc_use_fenced_puts && (gasnetc_num_hcas == 1)) {
    if (!gasneti_mynode) {
      fprintf(stderr,
              "WARNING: GASNET_USE_FENCED_PUTS requested, but ignored because only a single\n"
              "         HCA was detected and/or enabled.  To suppress this message, you may\n"
              "         either unset this environment variable or set it to '0'.\n"
              "         Alternatively, you may configure using '--disable-ibv-multirail'\n"
              "         if all nodes have only a single InfiniBand HCA.\n");
    }
    gasnetc_use_fenced_puts = 0;
  }
#else
  if (!gasneti_mynode && gasneti_getenv_yesno_withdefault("GASNET_USE_FENCED_PUTS", 0)) {
    fprintf(stderr,
            "WARNING: GASNET_USE_FENCED_PUTS requested, but ignored because GASNet was\n"
            "         configured without multi-rail support.  To suppress this message,\n"
            "         you may either unset this environment variable or set it to '0'.\n"
            "         Alternatively, you may configure using '--enable-ibv-multirail'.\n");
  }
#endif

  /* report hca/port properties */
  gasnetc_hca_report();

  /* get a pd for the QPs, SRQ and memory registration */
  GASNETC_FOR_ALL_HCA(hca) {
    hca->pd = ibv_alloc_pd(hca->handle);
    GASNETC_IBV_CHECK_PTR(hca->pd, "from ibv_alloc_pd()");
  }

  /* Check each HCA for feature support. */
#if GASNETC_IBV_SRQ
  if (gasnetc_use_srq) {
    GASNETC_FOR_ALL_HCA(hca) {
      struct ibv_srq_init_attr attr;
      struct ibv_srq *my_srq;

      memset(&attr, 0, sizeof(attr));
      attr.attr.max_wr = 4; /* Arbitrary */
      attr.attr.max_sge = 1;

      if (!hca->hca_cap.max_srq ||
          !hca->hca_cap.max_srq_wr ||
          (NULL == (my_srq = ibv_create_srq(hca->pd, &attr))) ||
          ibv_destroy_srq(my_srq)) {
        gasnetc_use_srq = 0;
        break;
      }
    }
    
    if (!gasnetc_use_srq && !gasneti_mynode) {
      fprintf(stderr,
              "WARNING: GASNET_USE_SRQ disabled because HCA lacks support.\n"
              "         To suppress this message set environment variable\n"
              "         GASNET_USE_SRQ=0 or reconfigure with --disable-ibv-srq.\n"
             );
    }
  }
#endif /* GASNETC_IBV_SRQ */
#if GASNETC_IBV_XRC
  if (gasnetc_use_xrc && !gasnetc_use_srq) {
    gasnetc_use_xrc =0 ;
    if (!gasneti_mynode) {
      fprintf(stderr,
              "WARNING: GASNET_USE_XRC disabled because SRQ is unavailable.\n"
              "         To suppress this message set environment variable\n"
              "         GASNET_USE_XRC=0 or reconfigure with --disable-ibv-xrc.\n"
             );
    }
  } else if (gasnetc_use_xrc) {
    GASNETC_FOR_ALL_HCA(hca) {
      if (0 == (hca->hca_cap.device_cap_flags & IBV_DEVICE_XRC)) {
        gasnetc_use_xrc =0 ;
        break;
      }
    }

    if (!gasnetc_use_xrc && !gasneti_mynode) {
      fprintf(stderr,
              "WARNING: GASNET_USE_XRC disabled because HCA lacks support.\n"
              "         To suppress this message set environment variable\n"
              "         GASNET_USE_XRC=0 or reconfigure with --disable-ibv-xrc.\n"
             );
    }
  }
#endif /* GASNETC_IBV_XRC */

  // Detect configuration differences (likely to be) due to heterogeneous clusters
  {
    const uint32_t srq_flag = 0x80000000;
    const uint32_t xrc_flag = 0x40000000;
    uint32_t config_word =
        (gasnetc_use_srq ? srq_flag : 0) |
        (gasnetc_use_xrc ? xrc_flag : 0) |
        ((gasnetc_num_hcas & 0xff) << 8) |
        (gasnetc_num_ports & 0xff);
    int srq_squashed = 0;
    uint32_t *all_configs = gasneti_malloc(gasneti_nodes * sizeof(config_word));
    gasneti_bootstrapExchange(&config_word, sizeof(config_word), all_configs);
#if GASNETC_IBV_SRQ
    // Auto-disable (with warning) SRQ if inhomogeneous
    for (gex_Rank_t i = 0; i < gasneti_nodes; ++i) {
      if (srq_flag & (config_word ^ all_configs[i])) {
        gasnetc_use_srq = 0;
      #if GASNETC_IBV_XRC
        gasnetc_use_xrc = 0;
        srq_squashed = 1;
      #endif
        GASNETI_TRACE_PRINTF(I, ("SRQ disabled because availability differs across nodes"));
        if (!gasneti_mynode) {
          fprintf(stderr,
                  "WARNING: SRQ disabled because availability differs across nodes.\n"
                  "         To suppress this message set environment variable\n"
                  "         GASNET_USE_SRQ=0 or reconfigure with --disable-ibv-srq.\n");
        }
      }
    }
#endif // GASNETC_IBV_SRQ
#if GASNETC_IBV_XRC
    // Auto-disable (with warning) XRC if inhomogeneous
    // Note that if SRQ was "squashed" above, then we won't complain about both
    for (gex_Rank_t i = 0; i < gasneti_nodes; ++i) {
      if (xrc_flag & (config_word ^ all_configs[i])) {
        gasnetc_use_xrc = 0;
        if (!srq_squashed) {
          GASNETI_TRACE_PRINTF(I, ("XRC disabled because availability differs across nodes"));
          if (!gasneti_mynode) {
            fprintf(stderr,
                    "WARNING: XRC disabled because availability differs across nodes.\n"
                    "         To suppress this message set environment variable\n"
                    "         GASNET_USE_XRC=0 or reconfigure with --disable-ibv-xrc.\n");
          }
        }
      }
    }
#endif // GASNETC_IBV_XRC
    // Fail gracefully if HCA or port counts are inhomogeneous
    if (!gasneti_mynode) {
      for (gex_Rank_t i = 1; i < gasneti_nodes; ++i) {
        if (0xff00 & (config_word ^ all_configs[i])) {
          gasneti_fatalerror("Inhomogeneous IB HCA count - cannot continue.\n"
                             "See README for GASNet's ibv-conduit for more information, especially\n"
                             "on multi-rail support and the GASNET_IBV_PORTS environment variable.\n"
                             "First detected mismatch: proc 0: %d vs. proc %d: %d\n",
                             gasnetc_num_hcas, (int)i, (int)(0xff & (all_configs[i] >> 8)));
        }
        if (0xff & (config_word ^ all_configs[i])) {
          gasneti_fatalerror("Inhomogeneous IB PORT count - cannot continue.\n"
                             "See README for GASNet's ibv-conduit for more information, especially\n"
                             "on multi-rail support and the GASNET_IBV_PORTS environment variable.\n"
                             "First detected mismatch: proc 0: %d vs. proc %d: %d\n",
                             gasnetc_num_ports, (int)i, (int)(0xff & all_configs[i]));
        }
      }
    }
    gasneti_free(all_configs);
  }

  /* Determine gasnetc_max_msg_sz and dependent variables */
  gasnetc_max_msg_sz = gasnetc_port_tbl[0].port.max_msg_sz;
  for (i = 1; i < gasnetc_num_ports; ++i) {
    gasnetc_max_msg_sz = MIN(gasnetc_max_msg_sz, gasnetc_port_tbl[i].port.max_msg_sz);
  }
  gasnetc_bounce_limit = MIN(gasnetc_max_msg_sz, gasnetc_bounce_limit);
  // Determine striping params used in gasnet_core_sndrcv.c:gasnetc_zerocp_common()
  if (1 == gasnetc_num_ports) {
    // single path: no striping
    gasnetc_put_stripe_sz = gasnetc_max_msg_sz;
    gasnetc_get_stripe_sz = gasnetc_max_msg_sz;
    GASNETI_TRACE_PRINTF(I, ("RMA striping: disabled for single-path network"));
  } else {
    if (! gasnetc_put_stripe_sz) {
      gasnetc_put_stripe_sz = gasnetc_max_msg_sz;
    } else if (gasnetc_put_stripe_sz > gasnetc_max_msg_sz) {
      GASNETI_TRACE_PRINTF(I, ("RMA striping: reduced GASNET_PUT_STRIPE_SZ to HCA max message size"));
      gasnetc_put_stripe_sz = gasnetc_max_msg_sz;
    } else if (gasnetc_put_stripe_sz < GASNETC_MINIMUM_PUT_STRIPE_SZ) {
      if (!gasneti_mynode) {
        gasneti_console_message("WARNING", "increased GASNET_PUT_STRIPE_SZ to minimum size %d", GASNETC_MINIMUM_PUT_STRIPE_SZ);
      }
      gasnetc_put_stripe_sz = GASNETC_MINIMUM_PUT_STRIPE_SZ;
    }
    GASNETI_TRACE_PRINTF(I, ("RMA striping: final GASNET_PUT_STRIPE_SZ = %"PRIuSZ, gasnetc_put_stripe_sz));
    if (! gasnetc_get_stripe_sz) {
      gasnetc_get_stripe_sz = gasnetc_max_msg_sz;
    } else if (gasnetc_get_stripe_sz > gasnetc_max_msg_sz) {
      GASNETI_TRACE_PRINTF(I, ("RMA striping: reduced GASNET_GET_STRIPE_SZ to HCA max message size"));
      gasnetc_get_stripe_sz = gasnetc_max_msg_sz;
    } else if (gasnetc_get_stripe_sz < GASNETC_MINIMUM_GET_STRIPE_SZ) {
      if (!gasneti_mynode) {
        gasneti_console_message("WARNING", "increased GASNET_GET_STRIPE_SZ to minimum size %d", GASNETC_MINIMUM_GET_STRIPE_SZ);
      }
      gasnetc_get_stripe_sz = GASNETC_MINIMUM_GET_STRIPE_SZ;
    }
    GASNETI_TRACE_PRINTF(I, ("RMA striping: final GASNET_GET_STRIPE_SZ = %"PRIuSZ, gasnetc_get_stripe_sz));
  }
  gasnetc_put_stripe_split = MIN(2*gasnetc_put_stripe_sz, gasnetc_max_msg_sz);
  gasnetc_get_stripe_split = MIN(2*gasnetc_get_stripe_sz, gasnetc_max_msg_sz);

  /* Exchange LIDs */
  local_lid = gasneti_calloc(gasnetc_num_ports, sizeof(uint16_t));
  remote_lid = gasneti_calloc(gasnetc_num_ports * gasneti_nodes, sizeof(uint16_t));
  for (i = 0; i < gasnetc_num_ports; ++i) {
    local_lid[i] = gasnetc_port_tbl[i].port.lid;
  }
  gasneti_bootstrapExchange(local_lid, gasnetc_num_ports * sizeof(uint16_t), remote_lid);
  gasneti_free(local_lid);

#if PLATFORM_ARCH_MIC
  /* In the case of multiple MICs in a host, the LIDs will be the same.
   * So, use the default nodemap.
   * TODO: distinguish single-MIC and "self hosted" MIC systems.
   */
  gasneti_nodemapInit(&gasneti_bootstrapExchange, NULL, 0, 0);
#else
  /* Derive nodemap from the LID info we have just exchanged */
  gasneti_nodemapInit(NULL, &remote_lid[0],
                      sizeof(remote_lid[0]),
                      sizeof(remote_lid[0]) * gasnetc_num_ports);
#endif

  /* compute various snd/rcv resource limits (requires node map) */
  i = gasnetc_sndrcv_limits();
  if (i != GASNET_OK) {
    return i;
  }
#if GASNETC_IBV_XRC
  if ((gasneti_nodemap_global_count == 1) ||
      (gasneti_nodemap_global_count == gasneti_nodes)) {
    /* No warning.  Eliminates case(s) that would SEGV otherwise */
    gasnetc_use_xrc = 0;
  }
#endif

#if GASNETC_IBV_ODP
  gasnetc_use_odp = gasneti_getenv_int_withdefault("GASNET_USE_ODP", 1, 0);
  if (gasnetc_use_odp) {
    gasneti_odp_init();
  }
#elif !GASNETC_IBV_ODP_DISABLED
  if (gasneti_getenv_int_withdefault("GASNET_ODP_VERBOSE", 1, 0)) {
    uint8_t found_odp_hca = 0;
    GASNETC_FOR_ALL_HCA(hca) {
      // Assume hca_id starting with "mlx5" (or higher) has ODP support
      if (!strncmp(hca->hca_id, "mlx", 3) && (atoi(hca->hca_id+3) >= 5)) {
        found_odp_hca = 1;
        break;
      }
    }
    // TODO: Only one process reports, so gather to 0 would be sufficient and
    //       a SUM reduction even would be even better.
    uint8_t *all = gasneti_malloc(gasneti_nodes);
    gasneti_bootstrapExchange(&found_odp_hca, 1, all);
    if (!gasneti_mynode) {
      gex_Rank_t count = 0;
      for (gex_Rank_t i = 0; i < gasneti_nodes; ++i) {
        count += all[i];
      }
      if (count) {
        fprintf(stderr,
                "WARNING: %d of %d processes have HCAs believed to support ODP.  However, the\n"
                "         corresponding software support was not found at configure time.\n"
                "         Please see the README for GASNet's ibv-conduit for more info on ODP.\n"
                "         To suppress this message set environment variable\n"
                "         GASNET_ODP_VERBOSE=0 or reconfigure with --disable-ibv-odp\n"
                "         (see ibv-conduit's README for more information).\n",
                (int)count, (int)gasneti_nodes);
      }
    }
    gasneti_free(all);
  }
#endif // GASNETC_IBV_ODP

  #if GASNET_PSHM
  {
    size_t shared_size = 0;

    /* global lid table: */
    shared_size += gasneti_nodes * gasnetc_num_ports * sizeof(uint16_t);
    shared_size = GASNETI_ALIGNUP(shared_size, GASNETI_CACHE_LINE_BYTES);

#if GASNETC_IBV_XRC
    /* shared qpn table: */
    shared_size += gasneti_nodes * gasnetc_alloc_qps * sizeof(uint32_t);
    shared_size = GASNETI_ALIGNUP(shared_size, GASNETI_CACHE_LINE_BYTES);
#endif

    shared_mem = gasneti_pshm_init(&gasneti_bootstrapSNodeBroadcast, shared_size);
  }
  #endif

  /* transpose remote lids into port_tbl */
#if GASNET_PSHM
  {
    uint16_t *tmp = (uint16_t *)shared_mem;
    for (i = 0; i < gasnetc_num_ports; ++i) {
      gasnetc_port_tbl[i].remote_lids = tmp;
      if (0 == gasneti_nodemap_local_rank) {
        for (node = 0; node < gasneti_nodes; ++node) {
          *(tmp++) = remote_lid[node * gasnetc_num_ports + i];
        }
      } else {
        tmp += gasneti_nodes;
      }
    }
    shared_mem = (void *)GASNETI_ALIGNUP(tmp, GASNETI_CACHE_LINE_BYTES);
  }
#else
  for (i = 0; i < gasnetc_num_ports; ++i) {
    gasnetc_port_tbl[i].remote_lids = gasneti_malloc(gasneti_nodes * sizeof(uint16_t));
    gasneti_leak(gasnetc_port_tbl[i].remote_lids);
    for (node = 0; node < gasneti_nodes; ++node) {
      gasnetc_port_tbl[i].remote_lids[node] = remote_lid[node * gasnetc_num_ports + i];
    }
  }
#endif
  gasneti_free(remote_lid);

  //  Create first Client and EP *here*, for use in subsequent bootstrap collectives
  gasneti_Client_t client;
  gasneti_EP_t ep;
  {
    //  allocate the client object
    client = gasneti_alloc_client(clientName, flags, 0);
    *client_p = gasneti_export_client(client);

    //  create the initial endpoint with internal handlers
    if (gasnetc_EP_Create(ep_p, *client_p, flags))
      GASNETI_RETURN_ERRR(RESOURCE,"Error creating initial endpoint");
    ep = gasneti_import_ep(*ep_p);
    gasnetc_ep0 = (gasnetc_EP_t)ep; // TODO-EX: this global variable to be removed
  }

#if GASNETC_IBV_XRC
  /* allocate/initialize XRC resources, if any */
  if (gasnetc_use_xrc) {
    i = gasnetc_xrc_init(&shared_mem);
    if (i != GASNET_OK) {
      return i;
    }
  }
#endif

  /* allocate/initialize transport resources */
  i = gasnetc_sndrcv_init(gasnetc_ep0);
  if (i != GASNET_OK) {
    return i;
  }

  /* Establish static connections and prepare for dynamic ones */
  i = gasnetc_connect_init(gasnetc_ep0);
  if (i != GASNET_OK) {
    return i;
  }

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): spawn successful - node %i/%i starting...\n", 
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif

  /* From this point forward gasneti_bootstrap*() can safely be implemented
   * via AMs or "raw" IB if desired for efficiency (but no segment for RDMA).
   * Currently only Exchange (aka GatherAll) and Barrier are used beyond this point.
   */
  gasnetc_sys_coll_init();

  // Determine largest allowable single memory registration supported by the HCA(s)
  gasnetc_pin_maxsz = ~(uint64_t)0;
  GASNETC_FOR_ALL_HCA(hca) {
    gasnetc_pin_maxsz = MIN(gasnetc_pin_maxsz, hca->hca_cap.max_mr_size);
  }

  /* Find max pinnable size before we start carving up memory w/ mmap()s.
   *
   * Take care that only one process per LID (node) performs the probe.
   * The result is then divided by the number of processes on the adapter,
   * which is easily determined from the connection information we exchanged above.
   */
  {
    GASNETI_TRACE_PRINTF(C,("I am node %d of %d on-node peers",
                            gasneti_myhost.node_rank, gasneti_myhost.node_count));

    /* Query the pinning limits of the HCA */
    gasnetc_init_pin_info(gasneti_myhost.nodes[0], gasneti_myhost.node_count);

    gasneti_assert(gasnetc_pin_info.memory != 0);
    gasneti_assert(gasnetc_pin_info.memory != (uintptr_t)(-1));
    gasneti_assert(gasnetc_pin_info.regions != 0);
  }
 
  uintptr_t mmap_limit;
  {
    uintptr_t local_limit;
  #if GASNET_SEGMENT_FAST
    /* Reserved memory needed by firehose on each node */
    /* NOTE: We reserve this memory even when firehose is disabled, since the disable
     * is only made available for debugging. */
    size_t reserved_mem = GASNETC_MIN_FH_MEM;

    if_pf (gasnetc_pin_info.memory < reserved_mem) {
      gasneti_fatalerror("Pinnable memory (%"PRIuPTR") is less than reserved minimum %"PRIuPTR, 
                         (uintptr_t)gasnetc_pin_info.memory, (uintptr_t)reserved_mem);
    }
    local_limit = (gasnetc_pin_info.memory - reserved_mem);
  #else
    local_limit = (uintptr_t)-1;
  #endif

  #ifdef GASNETI_MMAP_OR_PSHM
    // Bound per-host (sharedLimit) argument to gasneti_segmentLimit()
    // while properly reserving space for aux segments.
    uint64_t sharedLimit = gasneti_sharedLimit();
    uint64_t hostAuxSegs = gasneti_myhost.node_count * gasneti_auxseg_preinit();
    if (sharedLimit <= hostAuxSegs) {
      gasneti_fatalerror("per-host segment limit %"PRIu64" is too small to accommodate %i aux segments, "
                         "total size %"PRIu64". You may need to adjust OS shared memory limits.",
                         sharedLimit, gasneti_myhost.node_count, hostAuxSegs);
    }
    sharedLimit -= hostAuxSegs;

    mmap_limit = gasneti_segmentLimit(
                                  local_limit, sharedLimit,
                                  &gasnetc_bootstrapExchange_ib,
                                  &gasnetc_bootstrapBarrier_ib);
  #else
    mmap_limit = local_limit; // No better info available
  #endif
  }

  /* allocate and attach an aux segment */

  gasnet_seginfo_t auxseg = gasneti_auxsegAttach(gasnetc_pin_maxsz, &gasnetc_bootstrapExchange_ib);

  void *auxbase = auxseg.addr;
  uintptr_t auxsize = auxseg.size;

  /* The auxseg will be statically pinned even if the segment is not */
  
  GASNETC_FOR_ALL_HCA(hca) {
    if (0 != gasnetc_pin(hca, auxbase, auxsize, gasneti_seg_access_flags, &hca->aux_reg)) {
      gasneti_segreg_failed(auxsize, " aux", errno);
    }
    // TODO_EX: need scalable and/or lazy storage of aux segments and their rkeys
    hca->aux_rkeys = gasneti_malloc(gasneti_nodes*sizeof(uint32_t));
    gasnetc_bootstrapExchange_ib(&hca->aux_reg.handle->rkey, sizeof(uint32_t), hca->aux_rkeys);
  }
  gasneti_assert(gasnetc_pin_info.memory > auxsize);
  gasneti_assert(gasnetc_pin_info.regions > 1);
  gasnetc_pin_info.memory -= auxsize;
  gasnetc_pin_info.regions -= 1;

  /* determine Max{Local,GLobal}SegmentSize */
  gasneti_segmentInit(mmap_limit, &gasnetc_bootstrapExchange_ib, flags);

#if GASNET_BLCR
  gasneti_checkpoint_init(gasneti_bootstrapBroadcast);
#endif

  gasnetc_exit_init();

  #if 0
    /* Done earlier to allow tracing */
    gasneti_init_done = 1;  
  #endif
  gasnetc_bootstrapBarrier_ib();

  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
/* Helper for firehose init
 * Returns address (in static storage) of an array of preregistered regions
 */
static firehose_region_t *
gasnetc_prereg_list(int *count_p) {
  static firehose_region_t prereg[2];
  int c = 0;
  int h;

  /* Currently "snd_reg" and "aux_reg" are added to the firehose tables */

  prereg[c].addr             = gasnetc_hca[0].snd_reg.addr;
  prereg[c].len              = gasnetc_hca[0].snd_reg.len;
  GASNETC_FOR_ALL_HCA_INDEX(h) {
    prereg[c].client.handle[h] = NULL;        /* unreg must fail */
    prereg[c].client.lkey[h]   = gasnetc_hca[h].snd_reg.handle->lkey;
    prereg[c].client.rkey[h]   = gasnetc_hca[h].snd_reg.handle->rkey;
  }
  ++c;

  prereg[c].addr             = gasnetc_hca[0].aux_reg.addr;
  prereg[c].len              = gasnetc_hca[0].aux_reg.len;
  GASNETC_FOR_ALL_HCA_INDEX(h) {
    prereg[c].client.handle[h] = NULL;        /* unreg must fail */
    prereg[c].client.lkey[h]   = gasnetc_hca[h].aux_reg.handle->lkey;
    prereg[c].client.rkey[h]   = gasnetc_hca[h].aux_reg.handle->rkey;
  }
  ++c;

  *count_p = c;
  return prereg;
}

/* ------------------------------------------------------------------------------------ */
static int gasnetc_attach_primary(void) {
  /* ------------------------------------------------------------------------------------ */
  /*  register fatal signal handlers */

  /* catch fatal signals and convert to SIGQUIT */
  gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);

  /*  (###) register any custom signal handlers required by your conduit 
   *        (e.g. to support interrupt-based messaging)
   */

  /* ------------------------------------------------------------------------------------ */
  /* Initialize firehose */
  int using_firehose = GASNETC_USE_FIREHOSE && (gasneti_nodes > 1);
#if GASNETC_IBV_ODP && GASNETC_PIN_SEGMENT
  // If using Impicit ODP for local addrs, then local firehose is unused
  using_firehose &= ! gasnetc_use_odp;
#endif
  if (using_firehose) {
    int reg_count;
    firehose_region_t *prereg = gasnetc_prereg_list(&reg_count);
    size_t maxsz;

    uint64_t temp_fh_mem = gasnetc_pin_info.memory; // Math can exceed 4G and later fall below
    gasnetc_firehose_reg = gasnetc_pin_info.regions;

    /* Adjust for prepinned regions (they were pinned before init_pin_info probe) */
    for (int i = 0; i < reg_count; ++i) {
      temp_fh_mem += prereg[i].len;
    }

    /* Now initialize firehose */
    {
      gasnetc_firehose_flags = 0;

      #if GASNETC_PIN_SEGMENT
        /* Adjust for the pinned segment (which is not advertised to firehose as prepinned) */
        // TODO-EX: Need a replacement for use of gasneti_MaxLocalSegmentSize
        //          which is grossly inaccurate until the client segment has been registered.
        // TODO: shouldn't we use *local* values rather than max ones?
        gasneti_assert_always(temp_fh_mem > gasneti_MaxLocalSegmentSize);
        temp_fh_mem -= gasneti_MaxLocalSegmentSize;
        gasnetc_firehose_reg -= 1;

        gasnetc_firehose_flags |= FIREHOSE_INIT_FLAG_LOCAL_ONLY;
      #endif
      #if PLATFORM_OS_DARWIN
        gasnetc_firehose_flags |= FIREHOSE_INIT_FLAG_UNPIN_ON_FINI;
      #endif
      #if GASNETC_IBV_SHUTDOWN
        gasnetc_firehose_flags |= FIREHOSE_INIT_FLAG_UNPIN_ON_FINI
                               |  FIREHOSE_INIT_FLAG_MAY_REINIT;
      #endif

      #if PLATFORM_ARCH_32
        gasnetc_firehose_mem = GASNETI_PAGE_ALIGNDOWN(MIN(temp_fh_mem, 0xFFFFFFFF));
      #else
        gasnetc_firehose_mem = temp_fh_mem;
      #endif
      firehose_init(gasnetc_firehose_mem, gasnetc_firehose_reg, gasnetc_fh_maxsize,
                    prereg, reg_count, gasnetc_firehose_flags, &gasnetc_firehose_info);
      gasnetc_did_firehose_init = 1;
    }

    /* Determine alignment (and max size) for fh requests - a power-of-two <= max_region/2 */
    maxsz = MIN(gasnetc_max_msg_sz, gasnetc_firehose_info.max_LocalPinSize);
    #if !GASNETC_PIN_SEGMENT
      maxsz = MIN(maxsz, gasnetc_firehose_info.max_RemotePinSize);
    #endif
    gasneti_assert_always(maxsz >= (GASNET_PAGESIZE + gasnetc_inline_limit));
    gasnetc_fh_align = GASNET_PAGESIZE;
    while ((gasnetc_fh_align * 2) <= (maxsz / 2)) {
      gasnetc_fh_align *= 2;
    }
    gasnetc_fh_align_mask = gasnetc_fh_align - 1;
  }

  /* ------------------------------------------------------------------------------------ */
  /*  primary attach complete */
  gasneti_attach_done = 1;
  gasnetc_bootstrapBarrier_ib();

  GASNETI_TRACE_PRINTF(C,("gasnetc_attach_primary(): primary attach complete"));

  gasnete_init(); /* init the extended API */

  gasneti_nodemapFini();

  /* ensure extended API is initialized across nodes */
  gasnetc_bootstrapBarrier_ib();

#if GASNETC_USE_RCV_THREAD
  /* Start AM receive thread, if applicable */
  gasnetc_sndrcv_start_thread();
#endif

  // tear down conduit-specific bootstrap collectives (not used after attach)
  gasnetc_sys_coll_fini();
#if GASNET_DEBUG
  /* Ensure fini-init-fini works (required for checkpoint/restart) */
  gasnetc_sys_coll_init();
  gasneti_spawner->Barrier();
  gasnetc_bootstrapBarrier_ib();
  gasnetc_sys_coll_fini();
#endif

  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
static int gasnetc_attach_segment(gex_Segment_t                 *segment_p,
                                  gex_TM_t                      tm,
                                  uintptr_t                     segsize,
                                  gasneti_bootstrapExchangefn_t exchangefn,
                                  gex_Flags_t                   flags) {
  /* ------------------------------------------------------------------------------------ */
  /*  register client segment  */

  gasnetc_Segment_t segment;
  gasnet_seginfo_t myseg = gasneti_segmentAttach(segment_p, sizeof(*segment), tm, segsize, exchangefn, flags);
  segment = (gasnetc_Segment_t) gasneti_import_tm(tm)->_ep->_segment;

  // Register client segment with NIC

  #if GASNETC_PIN_SEGMENT
  {
    gasnetc_add_segment(segment);

    gasnetc_seg_start = (uintptr_t)myseg.addr;
    gasnetc_seg_len   = myseg.size;

    /* pin the segment and exchange the RKeys, once per HCA */
    gasnetc_hca_t *hca;
    GASNETC_FOR_ALL_HCA(hca) {
      hca->rkeys = gasneti_calloc(gasneti_nodes, sizeof(uint32_t));
      gasneti_leak(hca->rkeys);

      gasnetc_memreg_t memreg;
      if (0 != gasnetc_pin(hca, myseg.addr, myseg.size, gasneti_seg_access_flags, &memreg)) {
        gasneti_segreg_failed(segsize, "", errno);
      }
      segment->seg_lkey[hca->hca_index] = memreg.handle->lkey;
    #if GASNETC_IBV_SHUTDOWN
      segment->seg_reg[hca->hca_index] = memreg;
    #endif

      GASNETI_TRACE_PRINTF(I, ("Attach registered %"PRIuPTR" bytes on HCA %d", segsize, hca->hca_index));

      /* XXX: hca->rkeys is one of the O(N) storage requirements we might reduce/eliminate.
       * + When using PSHM we could store rkeys just once per supernode
       * + When not fully connected, we could utilize sparse storage
       */
      (*exchangefn)(&memreg.handle->rkey, sizeof(uint32_t), hca->rkeys);
    }
  }
  #endif

  /* Per-endpoint work */
  for (gex_Rank_t i = 0; i < gasneti_nodes; i++) {
    gasnetc_cep_t *cep = GASNETC_NODE2CEP(gasnetc_ep0, i);
    if (cep) {
      gasnetc_sndrcv_attach_peer(i, cep);
    }
  }

  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
// TODO-EX: this is a candidate for factorization (once we understand the per-conduit variations)
extern int gasnetc_attach( gex_TM_t               _tm,
                           gasnet_handlerentry_t  *table,
                           int                    numentries,
                           uintptr_t              segsize)
{
  GASNETI_TRACE_PRINTF(C,("gasnetc_attach(table (%i entries), segsize=%"PRIuPTR")",
                          numentries, segsize));
  gasneti_TM_t tm = gasneti_import_tm(_tm);
  gasneti_EP_t ep = tm->_ep;

  if (!gasneti_init_done) 
    GASNETI_RETURN_ERRR(NOT_INIT, "GASNet attach called before init");
  if (gasneti_attach_done) 
    GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already attached");

  /*  check argument sanity */
  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    if ((segsize % GASNET_PAGESIZE) != 0) 
      GASNETI_RETURN_ERRR(BAD_ARG, "segsize not page-aligned");
    if (segsize > gasneti_MaxLocalSegmentSize) 
      GASNETI_RETURN_ERRR(BAD_ARG, "segsize too large");
  #else
    segsize = 0;
  #endif

  /*  primary attach  */
  if (GASNET_OK != gasnetc_attach_primary())
    GASNETI_RETURN_ERRR(RESOURCE,"Error in primary attach");

  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    /*  register client segment  */
    gex_Segment_t seg; // g2ex segment is automatically saved by a hook
    if (GASNET_OK != gasnetc_attach_segment(&seg, _tm, segsize, gasneti_defaultExchange, GASNETI_FLAG_INIT_LEGACY))
      GASNETI_RETURN_ERRR(RESOURCE,"Error attaching segment");
  #endif

  /*  register client handlers */
  if (table && gasneti_amregister_legacy(ep->_amtbl, table, numentries) != GASNET_OK)
    GASNETI_RETURN_ERRR(RESOURCE,"Error registering handlers");

  /* ensure everything is initialized across all nodes */
  gasnet_barrier(0, GASNET_BARRIERFLAG_UNNAMED);

  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
// TODO-EX: this is a candidate for factorization (once we understand the per-conduit variations)
extern int gasnetc_Client_Init(
                               gex_Client_t            *client_p,
                               gex_EP_t                *ep_p,
                               gex_TM_t                *tm_p,
                               const char              *clientName,
                               int                     *argc,
                               char                    ***argv,
                               gex_Flags_t             flags)
{
  gasneti_assert(client_p);
  gasneti_assert(ep_p);
  gasneti_assert(tm_p);
  gasneti_assert(clientName);
#if !GASNET_NULL_ARGV_OK
  gasneti_assert(argc);
  gasneti_assert(argv);
#endif

  //  main init
  // TODO-EX: must split off per-client and per-endpoint portions
  if (!gasneti_init_done) { // First client
    // NOTE: gasnetc_init() creates the first Client and EP for use in bootstrap comms
    int retval = gasnetc_init(client_p, ep_p, clientName, argc, argv, flags);
    if (retval != GASNET_OK) GASNETI_RETURN(retval);
  #if 0
    /* called within gasnetc_init to allow init tracing */
    gasneti_trace_init(argc, argv);
  #endif
  } else { // NOT first client
    //  allocate the client object
    gasneti_Client_t client = gasneti_alloc_client(clientName, flags, 0);
    *client_p = gasneti_export_client(client);

    //  create the initial endpoint with internal handlers
    if (gasnetc_EP_Create(ep_p, *client_p, flags))
      GASNETI_RETURN_ERRR(RESOURCE,"Error creating initial endpoint");
  }
  gasneti_EP_t ep = gasneti_import_ep(*ep_p);

  // TODO-EX: create team
  gasneti_TM_t tm = gasneti_alloc_tm(ep, gasneti_mynode, gasneti_nodes, flags, 0);
  *tm_p = gasneti_export_tm(tm);

  if (0 == (flags & GASNETI_FLAG_INIT_LEGACY)) {
    /*  primary attach  */
    if (GASNET_OK != gasnetc_attach_primary())
      GASNETI_RETURN_ERRR(RESOURCE,"Error in primary attach");

    /* ensure everything is initialized across all nodes */
    gasnet_barrier(0, GASNET_BARRIERFLAG_UNNAMED);
  }

  return GASNET_OK;
}

extern int gasnetc_Segment_Attach(
                gex_Segment_t          *segment_p,
                gex_TM_t               tm,
                uintptr_t              length)
{
  gasneti_assert(segment_p);

  // TODO-EX: remove when this limitation is removed
  static int once = 1;
  if (once) once = 0;
  else gasneti_fatalerror("gex_Segment_Attach: current implementation can be called at most once");

  #if GASNET_SEGMENT_EVERYTHING
    *segment_p = GEX_SEGMENT_INVALID;
    gex_Event_Wait(gex_Coll_BarrierNB(tm, 0));
    return GASNET_OK; 
  #endif

  /* create a segment collectively */
  // TODO-EX: this implementation only works *once*
  // TODO-EX: should be using the team's exchange function if possible
  // TODO-EX: need to pass proper flags (e.g. pshm and bind) instead of 0
  if (GASNET_OK != gasnetc_attach_segment(segment_p, tm, length, gasneti_defaultExchange, 0))
    GASNETI_RETURN_ERRR(RESOURCE,"Error attaching segment");

  return GASNET_OK;
}

extern int gasnetc_EP_Create(gex_EP_t           *ep_p,
                             gex_Client_t       client,
                             gex_Flags_t        flags) {
  /* (###) add code here to create an endpoint belonging to the given client */
#if 1 // TODO-EX: This is a stub, which assumes 1 implicit call from ClientCreate
  static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;
  gasneti_mutex_lock(&lock);
    static int once = 0;
    int prev = once;
    once = 1;
  gasneti_mutex_unlock(&lock);
  if (prev) gasneti_fatalerror("Multiple endpoints are not yet implemented");
#endif

  gasnetc_EP_t conduit_ep;
  gasneti_EP_t ep = gasneti_alloc_ep(gasneti_import_client(client), flags, sizeof(*conduit_ep));
  *ep_p = gasneti_export_ep(ep);

  { /*  core API handlers */
    gex_AM_Entry_t *ctable = (gex_AM_Entry_t *)gasnetc_get_handlertable();
    int len = 0;
    int numreg = 0;
    gasneti_assert(ctable);
    while (ctable[len].gex_fnptr) len++; /* calc len */
    if (gasneti_amregister(ep->_amtbl, ctable, len, GASNETC_HANDLER_BASE, GASNETE_HANDLER_BASE, 0, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering core API handlers");
    gasneti_assert(numreg == len);
  }

  #if !GASNETC_PIN_SEGMENT
  { /*  firehose handlers */
    gex_AM_Entry_t *ftable = firehose_get_handlertable();
    int len = 0;
    int numreg = 0;
    gasneti_assert(ftable);
    while (ftable[len].gex_fnptr) len++; /* calc len */
    if (gasneti_amregister(ep->_amtbl, ftable, len, GASNETC_HANDLER_BASE, GASNETE_HANDLER_BASE, 1, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE, "Error registering firehose handlers");
    gasneti_assert(numreg == len);
  }
  #endif

  { /*  extended API handlers */
    gex_AM_Entry_t *etable = (gex_AM_Entry_t *)gasnete_get_handlertable();
    int len = 0;
    int numreg = 0;
    gasneti_assert(etable);
    while (etable[len].gex_fnptr) len++; /* calc len */
    if (gasneti_amregister(ep->_amtbl, etable, len, GASNETE_HANDLER_BASE, GASNETI_CLIENT_HANDLER_BASE, 0, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering extended API handlers");
    gasneti_assert(numreg == len);
  }

  return GASNET_OK;
}

extern int gasnetc_EP_RegisterHandlers(gex_EP_t                ep,
                                       gex_AM_Entry_t          *table,
                                       size_t                  numentries) {
  return gasneti_amregister_client(gasneti_import_ep(ep)->_amtbl, table, numentries);
}
/* ------------------------------------------------------------------------------------ */
/* Shutdown code - not always used */

#if GASNETC_IBV_SHUTDOWN
void
gasnetc_shutdown(void) {
  gasnetc_hca_t *hca;
  int rc, i;

  gasnetc_connect_shutdown(gasnetc_ep0);

  rc = gasnetc_sndrcv_shutdown();
  if (rc != GASNET_OK) {
    gasneti_fatalerror("gasnetc_sndrcv_shutdown() failed");
  }

  GASNETC_FOR_ALL_HCA(hca) {
  #if GASNETC_PIN_SEGMENT
    gasneti_mutex_lock(&gasnetc_segment_lock);
      for (int i = 0; i < gasnetc_segment_count; ++i) {
        gasnetc_Segment_t seg = gasnetc_segment_table[i];
        gasnetc_unpin(hca, &seg->seg_reg[hca->hca_index]);
      }
    gasneti_mutex_unlock(&gasnetc_segment_lock);
  #endif
  #if GASNETC_IBV_ODP
    if (gasnetc_use_odp) {
      gasnetc_odp_dereg(hca);
    }
  #endif

    if (hca->aux_reg.len) {
      gasnetc_unpin(hca, &hca->aux_reg);
    }

    rc = ibv_dealloc_pd(hca->pd);
    GASNETC_IBV_CHECK(rc, "from ibv_dealloc_pd()");

    rc = ibv_close_device(hca->handle);
    GASNETC_IBV_CHECK(rc, "from ibv_close_device()");
  }
}
#endif

/* ------------------------------------------------------------------------------------ */
/* Checkpoint/restart support code (BLCR) */

#if GASNET_BLCR

/* For open(), stat(), O_CREAT, etc.: */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* For time() */
#include <time.h>

void gasnetc_pre_checkpoint(void) {

  #if GASNETC_USE_RCV_THREAD
  /* TODO: could pause/resume instead of start/stop */
  gasnetc_sndrcv_stop_thread(1);
  #endif

  #if 0
  /* TODO: this just reports stats - do we want to preserve the info across checkpoints? */
  gasnetc_connect_fini();
  #endif

  gasnetc_sndrcv_quiesce();

  if (gasnetc_did_firehose_init) {
    firehose_fini();
  }

  gasnetc_shutdown();
}

void gasnetc_post_checkpoint(int is_restart) {
  gasnetc_hca_t *hca;
  int rc, i;

  /* Reopen each HCA by the original name */
  {
    struct ibv_device   **hca_list;
    int                 hca_count;

    hca_list = ibv_get_device_list(&hca_count);
    if ((hca_list == NULL) || (hca_count == 0)) {
      gasneti_fatalerror("Unable to find any HCAs");
    }
    GASNETC_FOR_ALL_HCA(hca) {
      for (i = 0; i < hca_count; ++i) {
        const char *hca_name = ibv_get_device_name(hca_list[i]);
        if (0 == strcmp(hca_name, hca->hca_id)) {
          hca->handle = ibv_open_device(hca_list[i]);
          hca->num_qps = 0;
          rc = ibv_query_device(hca->handle, &hca->hca_cap);
          GASNETC_IBV_CHECK(rc, "from ibv_query_device()");
          break;
        }
      }
      if (i == hca_count) {
        gasneti_fatalerror("Unable to reopen HCA '%s'", hca->hca_id);
      }
    }
    ibv_free_device_list(hca_list);
  }

  GASNETC_FOR_ALL_HCA(hca) {
    hca->pd = ibv_alloc_pd(hca->handle);
    GASNETC_IBV_CHECK_PTR(hca->pd, "from ibv_alloc_pd()");
  }

#if GASNET_PSHM
#error BLCR integration does not YET support PSHM - configure with --disable-blcr or --disable-pshm
  /* Need to pointer to conduit-specific shared mem for lid table and xrc */
#endif

  /* Query and exchange new LIDs */
  if (is_restart) {
    uint16_t *local_lid, *remote_lid;

    /* Query each port (to obtain the LID) */
    for (i = 0; i < gasnetc_num_ports; ++i) {
      gasnetc_port_info_t *this_port = &gasnetc_port_tbl[i];
      hca = &gasnetc_hca[this_port->hca_index];
      (void) ibv_query_port(hca->handle, this_port->port_num, &this_port->port);
    }

    /* Exchange LIDs */
    local_lid = gasneti_calloc(gasnetc_num_ports, sizeof(uint16_t));
    remote_lid = gasneti_calloc(gasnetc_num_ports * gasneti_nodes, sizeof(uint16_t));
    for (i = 0; i < gasnetc_num_ports; ++i) {
      local_lid[i] = gasnetc_port_tbl[i].port.lid;
    }
    gasneti_bootstrapExchange(local_lid, gasnetc_num_ports * sizeof(uint16_t), remote_lid);
    gasneti_free(local_lid);

    /* transpose remote lids into port_tbl */
    /* BLCR-TODO: factor this step once PSHM support ready */
  #if GASNET_PSHM
    // BLCR-TODO: NOT implemented
  #else
    for (i = 0; i < gasnetc_num_ports; ++i) {
      gex_Rank_t node;
      for (node = 0; node < gasneti_nodes; ++node) {
        gasnetc_port_tbl[i].remote_lids[node] = remote_lid[node * gasnetc_num_ports + i];
      }
    }
  #endif
    gasneti_free(remote_lid);
  }

#if GASNETC_IBV_XRC && 0 // BLCR integration does not YET include PSHM (and thus XRC)
  // BLCR-TODO: Skipped for now as we are initially not supporting PSHM */
  if (gasnetc_use_xrc) {
    rc = gasnetc_xrc_init(&shared_mem);
    if (i != GASNET_OK) {
      gasneti_fatalerror("Failed post-checkpoint call to gasnetc_xrc_init");
    }
  }
#endif

  /* allocate/initialize transport resources */
  rc = gasnetc_sndrcv_init(gasnetc_ep0);
  if (rc != GASNET_OK) {
    gasneti_fatalerror("Failed post-checkpoint call to gasnetc_sndrcv_init");
  }

  /* Establish static connections and prepare for dynamic ones */
  rc = gasnetc_connect_init(gasnetc_ep0);
  if (rc != GASNET_OK) {
    gasneti_fatalerror("Failed post-checkpoint call to gasnetc_connect_init");
  }

  /* Now safe to use native collectives again */
  gasnetc_sys_coll_init();

#if GASNET_DEBUG_VERBOSE
  gasnetc_bootstrapBarrier_ib();
  fprintf(stderr, "@%d> AM-based barrier worked!\n", gasneti_mynode);
#endif

  /* REregister the segment and exchange the new rkeys */
#if GASNETC_PIN_SEGMENT
  if (gasnetc_hca[0].rkeys) {
    gasneti_mutex_lock(&gasnetc_segment_lock);
    GASNETC_FOR_ALL_HCA(hca) {
        for (int i = 0; i < gasnetc_segment_count; ++i) {
          gasnetc_Segment_t seg = gasnetc_segment_table[i];
          gasnetc_memreg_t *reg = &seg->seg_reg[hca->hca_index];
          if (0 != gasnetc_pin(hca, (void *)reg->addr, reg->len, gasneti_seg_access_flags, reg)) {
            gasneti_fatalerror("Unexpected error %s (errno=%d) when (re)registering the segment",
                               strerror(errno), errno);
          }
          seg->seg_lkey[hca->hca_index] = reg->handle->lkey;
        }
      // TODO-EX: following is insufficient for multi-segment
      gasnetc_memreg_t *reg = &gasnetc_segment_table[0]->seg_reg[hca->hca_index];
      gasneti_assert_int(gasnetc_segment_count ,==, 1);
      gasnetc_bootstrapExchange_ib(&reg->handle->rkey, sizeof(uint32_t), hca->rkeys);
    }
    gasneti_mutex_unlock(&gasnetc_segment_lock);
  }
#endif

  /* Per-endpoint attach work */
  for (i = 0; i < gasneti_nodes; i++) {
    gasnetc_cep_t *cep = GASNETC_NODE2CEP(gasnetc_ep0, i);
    if (cep) {
      gasnetc_sndrcv_attach_peer(i, cep);
    }
  }

#if GASNETC_PIN_SEGMENT &&  GASNET_DEBUG_VERBOSE
  GASNETI_SAFE(gasnet_barrier(0,0));
  fprintf(stderr, "@%d> native barrier worked!\n", gasneti_mynode);
#endif

  /* Re-initialize firehose */
  if (gasnetc_did_firehose_init) {
    int reg_count;
    firehose_region_t *prereg = gasnetc_prereg_list(&reg_count);

    firehose_init(gasnetc_firehose_mem, gasnetc_firehose_reg, gasnetc_fh_maxsize,
                  prereg, reg_count, gasnetc_firehose_flags, &gasnetc_firehose_info);
  }

#if GASNET_DEBUG_VERBOSE
  /* Exercise put/get and firehose via out-of-segment xfers
   * If !SEGMENT_FAST this is also the first test of the native barrier.
   */
  {
    #define THE_TEST_LEN 16384
  #if GASNET_SEGMENT_EVERYTHING
    static char buffer0[THE_TEST_LEN];
  #endif
    static char buffer1[THE_TEST_LEN];
    static char buffer2[THE_TEST_LEN];
    const size_t nbytes = THE_TEST_LEN;
    void *loc_addr, *rem_addr;
    #undef THE_TEST_LEN

    gex_Rank_t peer = gasneti_mynode ^ 1;
    if (peer == gasneti_nodes) peer = gasneti_mynode;

  #if GASNET_SEGMENT_EVERYTHING
    {
      void **exchg_tmp = gasneti_malloc(gasneti_nodes * sizeof(void*));
      loc_addr = buffer0;
      gasnetc_bootstrapExchange_ib(&loc_addr, sizeof(void*), exchg_tmp);
      rem_addr = exchg_tmp[peer];
      gasneti_free(exchg_tmp);
      for (i=0; i<nbytes; ++i) buffer0[i] = (char)(i ^ gasneti_mynode);
    }
  #else
    loc_addr = gasneti_client_seginfo(gasneti_THUNK_TM, gasneti_mynode)->addr;
    rem_addr = gasneti_client_seginfo(gasneti_THUNK_TM, peer)->addr;
  #endif

    GASNETI_MEMCPY(buffer1, loc_addr, nbytes);
    // TODO-EX: replace or remove:
    //gasnet_ get(buffer2, peer, rem_addr, nbytes);

    GASNETI_SAFE(gasnet_barrier(0, GASNET_BARRIERFLAG_ANONYMOUS));

    // TODO-EX: replace or remove:
    //gasnet_ put(peer, rem_addr, buffer2, nbytes);

    GASNETI_SAFE(gasnet_barrier(0, GASNET_BARRIERFLAG_UNNAMED));

    gasneti_assert_always(memcmp(buffer1, loc_addr, nbytes) == 0);

    GASNETI_SAFE(gasnet_barrier(5551212, 0));

  #if GASNETC_PIN_SEGMENT
    fprintf(stderr, "@%d> RDMA-based xfers worked!\n", gasneti_mynode);
  #else
    fprintf(stderr, "@%d> RDMA-based barriers and xfers worked!\n", gasneti_mynode);
  #endif
  }
#endif

#if GASNETC_USE_RCV_THREAD
  /* TODO: could pause/resume instead of start/stop */
  gasnetc_sndrcv_start_thread();
#endif

  gasnetc_sys_coll_fini();
}

int gasnet_all_rollback(const char *dir) {
    /* BLCR-TODO: remove the barrier or use a distinct team? */
    gasnet_barrier(0, GASNET_BARRIERFLAG_UNNAMED);
    gasnetc_pre_checkpoint();

    /* BLCR-TODO: error reporting/recovery */
    if (gasneti_spawner->Rollback) {
      (void) gasneti_spawner->Rollback(dir);
      /* BLCR-TODO: error checking */
    }

    gasnetc_post_checkpoint(1);
    gasneti_bootstrapBarrier();

    return GASNET_OK;
}
#endif /* GASNET_BLCR */

/* PROOF-OF-CONCEPT
 * Collective checkpoint request */
int gasnet_all_checkpoint(const char *dir_arg) {
  #if GASNET_BLCR
    int rc;

    /* BLCR-TODO: remove the barrier or use a distinct team? */
    gasnet_barrier(0, GASNET_BARRIERFLAG_UNNAMED);
    gasnetc_pre_checkpoint();

    {
      const char *dir = gasneti_checkpoint_dir(dir_arg);
      int fd = fd = gasneti_checkpoint_create(dir);
      /* BLCR-TODO: error handling (curently _create() dies on error) */

      if (gasneti_spawner->PreCheckpoint) {
        (void) gasneti_spawner->PreCheckpoint(fd);
        /* BLCR-TODO: error checking */
      }

      rc = gasneti_checkpoint_write(fd);
      if (rc < 0) {
        gasneti_fatalerror("Checkpoint failed rc=%d errno=%d\n", rc, errno);
      }
      /* BLCR-TODO: better error handling/recovery */

      if (gasneti_spawner->PostCheckpoint) {
        (void) gasneti_spawner->PostCheckpoint(fd, rc);
        /* BLCR-TODO: error checking */
      }

      if (!rc) (void)close(fd); /* Continue case */
      if (!dir_arg) gasneti_free((void*)dir);
    }

    gasnetc_post_checkpoint(rc);
    gasneti_bootstrapBarrier();

    return GASNET_OK;
  #else
    fprintf(stderr, "WARNING: checkpoint requested but not configured\n");
    return GASNET_ERR_RESOURCE;
  #endif
}

/* ------------------------------------------------------------------------------------ */
/*
  Exit handling code
*/

#ifndef GASNETI_HAVE_ATOMIC_CAS
  #error "required atomic compare-and-swap is not yet implemented for your CPU/OS/compiler"
#endif

gasneti_atomic_t gasnetc_exit_running = gasneti_atomic_init(0);		/* boolean used by GASNETC_IS_EXITING */

static gasneti_atomic_t gasnetc_exit_code = gasneti_atomic_init(0);	/* value to _exit() with */
static gasneti_atomic_t gasnetc_exit_reds = gasneti_atomic_init(0);	/* count of reduce requests */
static gasneti_atomic_t gasnetc_exit_dist = gasneti_atomic_init(0);	/* OR of reduce distances */
static gasneti_atomic_t gasnetc_exit_reqs = gasneti_atomic_init(0);	/* count of remote exit requests */
static gasneti_atomic_t gasnetc_exit_reps = gasneti_atomic_init(0);	/* count of remote exit replies */
static gasneti_atomic_t gasnetc_exit_done = gasneti_atomic_init(0);	/* flag to show exit coordination done */
static gasnetc_counter_t gasnetc_exit_repl_oust = GASNETC_COUNTER_INITIALIZER; /* track send of our AM reply */

static int gasnetc_exit_in_signal = 0;  /* to avoid certain things in signal context */
extern void gasnetc_fatalsignal_callback(int sig) {
  gasnetc_exit_in_signal = 1;
}

#define GASNETC_ROOT_NODE 0

enum {
  GASNETC_EXIT_ROLE_UNKNOWN,
  GASNETC_EXIT_ROLE_MASTER,
  GASNETC_EXIT_ROLE_SLAVE
};

static gasneti_atomic_t gasnetc_exit_role = gasneti_atomic_init(GASNETC_EXIT_ROLE_UNKNOWN);

#if GASNET_DEBUG_VERBOSE
  static const char * volatile gasnetc_exit_state = "UNKNOWN STATE";
  #define GASNETC_EXIT_STATE(st) do {                                    \
	gasnetc_exit_state = st;                                         \
	fprintf(stderr, "%d> EXIT STATE %s\n", (int)gasneti_mynode, st); \
        fflush(NULL);                                                    \
  } while (0)
#elif GASNET_DEBUG
  static const char * volatile gasnetc_exit_state = "UNKNOWN STATE";
  #define GASNETC_EXIT_STATE(st) gasnetc_exit_state = st
#else
  #define GASNETC_EXIT_STATE(st) do {} while (0)
#endif

/*
 * Code to disable user's AM handlers when exiting.  We need this because we must call
 * AMPoll to run core handlers, including ACKs for flow control.
 *
 * We do it this way because it adds absolutely nothing the normal execution path.
 * Thanks to Dan for the suggestion.
 */
static void gasnetc_noop(void) { return; }
static void gasnetc_disable_AMs(void) {
  int i;

  for (i = GASNETE_HANDLER_BASE; i < GASNETC_MAX_NUMHANDLERS; ++i) {
    gasnetc_ep0->_amtbl[i].gex_fnptr = (gex_AM_Fn_t)&gasnetc_noop;
  }
}

#if GASNET_PSHM
static gex_Rank_t *gasnetc_exit_child = NULL;
static gex_Rank_t gasnetc_exit_children = 0;
static gex_Rank_t gasnetc_exit_parent = 0;
#endif

static int gasnetc_exit_reduce(int exitcode, int64_t timeout_us)
{
  gasneti_tick_t start_time = gasneti_ticks_now();
  int rc, i;

  GASNETC_EXIT_STATE("exitcode reduction");

  gasneti_assert(timeout_us > 0); 

  /* If the remote request has arrived then we've already failed */
  if (gasneti_atomic_read(&gasnetc_exit_reqs, 0)) return -1;

#if GASNET_PSHM
  if (gasnetc_exit_children == gasneti_nodes) { /* Non-lead node */
    GASNETC_EXIT_STATE("exitcode reduction: send to parent");
    rc = gasnetc_RequestSysShort(gasnetc_exit_parent, NULL,
                                 gasneti_handleridx(gasnetc_exit_reduce_reqh),
                                 2, exitcode, 0);
    if (rc != GASNET_OK) return -1;
    if (gasneti_atomic_read(&gasnetc_exit_reqs, 0)) return -1;

    GASNETC_EXIT_STATE("exitcode reduction: wait for parent");
    do {
      if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) return -1;
      gasnetc_sndrcv_poll(0);
      if (gasneti_atomic_read(&gasnetc_exit_reqs, 0)) return -1;
    } while (gasneti_atomic_read(&gasnetc_exit_reds, 0) == 0);
    return 0;
  } else { /* Lead node */
    GASNETC_EXIT_STATE("exitcode reduction: wait for children");
    while (gasneti_atomic_read(&gasnetc_exit_reds, 0) < gasnetc_exit_children) {
      if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) return -1;
      gasnetc_sndrcv_poll(0);
      if (gasneti_atomic_read(&gasnetc_exit_reqs, 0)) return -1;
    }
    exitcode = gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE);
  }
#endif

  GASNETC_EXIT_STATE("exitcode reduction: dissemination");
  for (i = 0; i < gasnetc_dissem_peers; ++i) {
    const uint32_t distance = 1 << i;
    rc = gasnetc_RequestSysShort(gasnetc_dissem_peer[i], NULL,
                                 gasneti_handleridx(gasnetc_exit_reduce_reqh),
                                 2, exitcode, distance);
    if (rc != GASNET_OK) return -1;
    do { /* wait for completion of the proper receive, which might arrive out of order */
      if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) return -1;
      gasnetc_sndrcv_poll(0); 
      if (gasneti_atomic_read(&gasnetc_exit_reqs, 0)) return -1;
    } while (!(distance & gasneti_atomic_read(&gasnetc_exit_dist, 0)));
    exitcode = gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE);
  }

#if GASNET_PSHM
  GASNETC_EXIT_STATE("exitcode reduction: send to children");
  for (i = 0; i < gasnetc_exit_children; ++i) {
    rc = gasnetc_RequestSysShort(gasnetc_exit_child[i], NULL,
                                 gasneti_handleridx(gasnetc_exit_reduce_reqh),
                                 2, exitcode, 0);
    if (rc != GASNET_OK) return -1;
    gasnetc_sndrcv_poll(0);
    if (gasneti_atomic_read(&gasnetc_exit_reqs, 0)) return -1;
    if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) return -1;
  }
#endif

  return 0;
}

/* gasnetc_exit_reduce_reqh: reduction on exitcode */
static void gasnetc_exit_reduce_reqh(gex_Token_t token,
                                     gex_AM_Arg_t arg0,
                                     gex_AM_Arg_t arg1) {
  gasneti_atomic_val_t exitcode = arg0;
  gasneti_atomic_val_t distance = arg1;
  gasneti_atomic_val_t prevcode;
  do {
    prevcode = gasneti_atomic_read(&gasnetc_exit_code, 0);
  } while ((exitcode > prevcode) &&
           !gasneti_atomic_compare_and_swap(&gasnetc_exit_code, prevcode, exitcode, 0));
  if (distance) {
  #if defined(GASNETI_HAVE_ATOMIC_ADD_SUB)
    /* atomic OR via ADD since no bit will be set more than once */
    gasneti_assert(GASNETI_POWEROFTWO(distance));
    gasneti_atomic_add(&gasnetc_exit_dist, distance, GASNETI_ATOMIC_REL);
  #elif defined(GASNETI_HAVE_ATOMIC_CAS)
    /* atomic OR via C-A-S */
    uint32_t old_val;
    do {
      old_val = gasneti_atomic_read(&gasnetc_exit_dist, 0);
    } while (!gasneti_atomic_compare_and_swap(&gasnetc_exit_dist,
                                               old_val, old_val|distance,
                                               GASNETI_ATOMIC_REL));
  #else
    #error "required atomic compare-and-swap is not yet implemented for your CPU/OS/compiler"
  #endif
  } else {
    gasneti_atomic_increment(&gasnetc_exit_reds, GASNETI_ATOMIC_REL);
  }
}

/*
 * gasnetc_exit_role_reqh()
 *
 * This request handler (invoked only on the "root" node) handles the election
 * of a single exit "master", who will coordinate an orderly shutdown.
 */
static void gasnetc_exit_role_reqh(gex_Token_t token) {
  gex_Rank_t src = gasnetc_msgsource(token);
  int local_role, result;

  gasneti_assert(gasneti_mynode == GASNETC_ROOT_NODE);	/* May only send this request to the root node */

  
  /* What role would the local node get if the requester is made the master? */
  
  local_role = (src == GASNETC_ROOT_NODE) ? GASNETC_EXIT_ROLE_MASTER : GASNETC_EXIT_ROLE_SLAVE;

  /* Try atomically to assume the proper role.  Result determines role of requester */
  result = gasneti_atomic_compare_and_swap(&gasnetc_exit_role, GASNETC_EXIT_ROLE_UNKNOWN, local_role, 0)
                ? GASNETC_EXIT_ROLE_MASTER : GASNETC_EXIT_ROLE_SLAVE;

  /* Inform the requester of the outcome. */
  GASNETI_SAFE(gasnetc_ReplySysShort(token, NULL, gasneti_handleridx(gasnetc_exit_role_reph),
				   1, (gex_AM_Arg_t)result));
}

/*
 * gasnetc_exit_role_reph()
 *
 * This reply handler receives the result of the election of an exit "master".
 * The reply contains the exit "role" this node should assume.
 */
static void gasnetc_exit_role_reph(gex_Token_t token, gex_AM_Arg_t arg0) {
  int role;

  /* May only receive this reply from the root node */
  gasneti_assert(gasnetc_msgsource(token) == GASNETC_ROOT_NODE);

  /* What role has this node been assigned? */
  role = (int)arg0;
  gasneti_assert((role == GASNETC_EXIT_ROLE_MASTER) || (role == GASNETC_EXIT_ROLE_SLAVE));

  /* Set the role if not yet set.  Then assert that the assigned role has been assumed.
   * This way the assertion is checking that if the role was obtained by other means
   * (namely by receiving an exit request) it must match the election result. */
  gasneti_atomic_compare_and_swap(&gasnetc_exit_role, GASNETC_EXIT_ROLE_UNKNOWN, role, 0);
  gasneti_assert(gasneti_atomic_read(&gasnetc_exit_role, 0) == role);
}

/*
 * gasnetc_get_exit_role()
 *
 * This function returns the exit role immediately if known.  Otherwise it sends an AMRequest
 * to determine its role and then polls the network until the exit role is determined, either
 * by the reply to that request, or by a remote exit request.
 *
 * Should be called with an alarm timer in-force in case we get hung sending or the root node
 * is not responsive.
 *
 * Note that if we get here as a result of a remote exit request then our role has already been
 * set to "slave" and we won't touch the network from inside the request handler.
 */
static int gasnetc_get_exit_role(void)
{
  int role;

  role = gasneti_atomic_read(&gasnetc_exit_role, 0);
  if (role == GASNETC_EXIT_ROLE_UNKNOWN) {
    /* Don't know our role yet.  So, send an AM Request to determine our role */
    GASNETI_SAFE(gasnetc_RequestSysShort(GASNETC_ROOT_NODE, NULL,
			    	       gasneti_handleridx(gasnetc_exit_role_reqh), 0));

    /* Now spin until somebody tells us what our role is */
    do {
      gasnetc_sndrcv_poll(0); /* works even before _attach */
      role = gasneti_atomic_read(&gasnetc_exit_role, 0);
    } while (role == GASNETC_EXIT_ROLE_UNKNOWN);
  }

  return role;
}

/* gasnetc_exit_head
 *
 * All exit paths pass through here as the first step.
 * This function ensures that gasnetc_exit_code is written only once
 * by the first call.
 * It also lets the handler for remote exit requests know if a local
 * request has already begun.
 *
 * returns non-zero on the first call only
 * returns zero on all subsequent calls
 */
static int gasnetc_exit_head(int exitcode) {
  static gasneti_atomic_t once = gasneti_atomic_init(1);
  int retval;

  gasneti_atomic_set(&gasnetc_exit_running, 1, GASNETI_ATOMIC_WMB_POST);

  retval = gasneti_atomic_decrement_and_test(&once, 0);

  if (retval) {
    /* Store the exit code for later use */
    gasneti_atomic_set(&gasnetc_exit_code, exitcode, GASNETI_ATOMIC_WMB_POST);
  }

  return retval;
}

/* gasnetc_exit_now
 *
 * First we set the atomic variable gasnetc_exit_done to allow the exit
 * of any threads which are spinning on it in gasnetc_exit().
 * Then this function tries hard to actually terminate the calling thread.
 * If for some unlikely reason gasneti_killmyprocess() returns, we abort().
 *
 * DOES NOT RETURN
 */
GASNETI_NORETURN
static void gasnetc_exit_now(int exitcode) {
  /* If anybody is still waiting, let them go */
  gasneti_atomic_set(&gasnetc_exit_done, 1, GASNETI_ATOMIC_WMB_POST);

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_exit(): node %i/%i calling killmyprocess...\n", 
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif
  gasneti_killmyprocess(exitcode);
  /* NOT REACHED */

  gasneti_reghandler(SIGABRT, SIG_DFL);
  gasneti_fatalerror("gasnetc_exit_now aborting...");
  /* NOT REACHED */
}

/* gasnetc_exit_tail
 *
 * This the final exit code for the cases of local or remote requested exits.
 * It is not used for the return-from-main case.  Nor is this code used if a fatal
 * signal (including SIGALRM on timeout) is encountered while trying to shutdown.
 *
 * Just a wrapper around gasnetc_exit_now() to actually terminate.
 *
 * DOES NOT RETURN
 */
static void gasnetc_exit_tail(void) GASNETI_NORETURN;
static void gasnetc_exit_tail(void) {
  gasnetc_exit_now((int)gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE));
  /* NOT REACHED */
}

/* gasnetc_exit_sighandler
 *
 * This signal handler is for a last-ditch exit when a signal arrives while
 * attempting the graceful exit.  That includes SIGALRM if we get wedged.
 *
 * Just a (verbose) signal-handler wrapper for gasnetc_exit_now().
 *
 * DOES NOT RETURN
 */
static void gasnetc_exit_sighandler(int sig) {
  int exitcode = (int)gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE);
  static gasneti_atomic_t once = gasneti_atomic_init(1);

#if GASNET_DEBUG || GASNETC_IBV_ODP
  // protect until we reach reentrance check
  GASNETC_EXIT_STATE("in exit sighandler");
  gasneti_reghandler(SIGALRM, _exit);
  gasneti_unblocksig(SIGALRM);
  alarm(30);
#endif

#if GASNETC_IBV_ODP
  gasnetc_odp_shutdown(); // Avoid possible system memory leak
#endif

  #if GASNET_DEBUG
  /* note - can't call trace macros here, or even sprintf */
  if (sig == SIGALRM) {
    static const char msg[] = "gasnet_exit(): WARNING: timeout during exit... goodbye.  [";
    const char * state = gasnetc_exit_state;
    (void) write(STDERR_FILENO, msg, sizeof(msg) - 1);
    (void) write(STDERR_FILENO, state, strlen(state));
    (void) write(STDERR_FILENO, "]\n", 2);
  } else {
    static const char msg1[] = "gasnet_exit(): ERROR: signal ";
    static const char msg2[] = " received during exit... goodbye.  [";
    const char * state = gasnetc_exit_state;
    char digit;

    (void) write(STDERR_FILENO, msg1, sizeof(msg1) - 1);

    /* assume sig < 100 */
    if (sig > 9) {
      digit = '0' + ((sig / 10) % 10);
      (void) write(STDERR_FILENO, &digit, 1);
    }
    digit = '0' + (sig % 10);
    (void) write(STDERR_FILENO, &digit, 1);
    
    (void) write(STDERR_FILENO, msg2, sizeof(msg2) - 1);
    (void) write(STDERR_FILENO, state, strlen(state));
    (void) write(STDERR_FILENO, "]\n", 2);
  }
  #endif

  if (gasneti_atomic_decrement_and_test(&once, 0)) {
    /* We ask the bootstrap support to kill us, but only once */
    GASNETC_EXIT_STATE("in suicide timer");
    gasneti_reghandler(SIGALRM, gasnetc_exit_sighandler);
    gasneti_unblocksig(SIGALRM);
    alarm(5);
    gasneti_bootstrapAbort(exitcode);
  } else {
    gasnetc_exit_now(exitcode);
  }

  /* NOT REACHED */
}

/* gasnetc_exit_master
 *
 * We say a polite goodbye to our peers and then listen for their replies.
 * This forms the root node's portion of a barrier for graceful shutdown.
 *
 * The "goodbyes" are just an AM containing the desired exit code.
 * The AM helps ensure that on non-collective exits the "other" nodes know to exit.
 * If we see a "goodbye" from all of our peers we know we've managed to coordinate
 * an orderly shutdown.  If not, then in gasnetc_exit_body() we can ask the bootstrap
 * support to kill the job in a less graceful way.
 *
 * Takes the exitcode and a timeout in us as arguments
 *
 * Returns 0 on success, non-zero on any sort of failure including timeout.
 */
static int gasnetc_exit_master(int exitcode, int64_t timeout_us) {
  int i, rc;
  gasneti_tick_t start_time;

  gasneti_assert(timeout_us > 0); 

  start_time = gasneti_ticks_now();

  /* Notify phase */
  for (i = 0; i < gasneti_nodes; ++i) {
    if (i == gasneti_mynode) continue;

    if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) return -1;

    rc = gasnetc_RequestSysShort(i, NULL,
		    	       gasneti_handleridx(gasnetc_exit_reqh),
			       1, (gex_AM_Arg_t)exitcode);
    if (rc != GASNET_OK) return -1;
  }

  /* Wait phase - wait for replies from our N-1 peers */
  while (gasneti_atomic_read(&gasnetc_exit_reps, 0) < (gasneti_nodes - 1)) {
    if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) return -1;

    gasnetc_sndrcv_poll(0); /* works even before _attach */
  }

  return 0;
}

/* gasnetc_exit_slave
 *
 * We wait for a polite goodbye from the exit master.
 *
 * Takes a timeout in us as an argument
 *
 * Returns 0 on success, non-zero on timeout.
 */
static int gasnetc_exit_slave(int64_t timeout_us) {
  gasneti_tick_t start_time;

  gasneti_assert(timeout_us > 0); 

  start_time = gasneti_ticks_now();

  /* wait until the exit request is received from the master */
  while (gasneti_atomic_read(&gasnetc_exit_reqs, 0) == 0) {
    if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) return -1;

    gasnetc_sndrcv_poll(0); /* works even before _attach */
  }

  /* wait until our reply has been placed on the wire */
  gasneti_sync_reads(); /* For non-atomic portion of gasnetc_exit_repl_oust */
  GASNET_BEGIN_FUNCTION(); // OK - not a critical-path
  gasnetc_counter_wait(&gasnetc_exit_repl_oust, 1 GASNETI_THREAD_PASS);

  return 0;
}

/* gasnetc_exit_body
 *
 * This code is common to all the exit paths and is used to perform a hopefully graceful exit in
 * all cases.  In the normal case of a collective call to gasnet_exit() (or return from main()),
 * we perform a MAX() reduction over the exitcodes and all processes will exit with the result.
 * If the exitcode reduction does not complete within gasnetc_exittimeout seconds, then we
 * assume that this is a NON-collective exit and that additional coordination is needed.  In that
 * case, to coordinate a graceful shutdown gasnetc_get_exit_role() will select one node as
 * the "master".  That master node will then send a remote exit request to each of its peers to
 * ensure they know that it is time to exit.  If we fail to coordinate the shutdown, we ask the
 * bootstrap to shut us down agressively.  Otherwise we return to our caller.  Unless our caller
 * is the at-exit handler, we are typically followed by a call to gasnetc_exit_tail() to perform
 * the actual termination.  Note also that this function will block all calling threads other than
 * the first until the shutdown code has been completed.
 *
 * XXX: timeouts other than gasnetc_exittimeout are hard-coded and entirely arbitrary
 */
static void gasnetc_exit_body(void) {
  int role, exitcode;
  int graceful = 0;
  int64_t timeout_us = gasnetc_exittimeout * 1.0e6;

  /* once we start a shutdown, ignore all future SIGQUIT signals or we risk reentrancy */
  (void)gasneti_reghandler(SIGQUIT, SIG_IGN);

  /* Ensure only one thread ever continues past this point.
   * Others will spin here until time to die.
   * We can't/shouldn't use mutex code here since it is not signal-safe.
   */
  #ifdef GASNETI_USE_GENERIC_ATOMICOPS
    #error "We need real atomic ops with signal-safety for gasnet_exit..."
  #endif
  {
    static gasneti_atomic_t exit_lock = gasneti_atomic_init(1);
    if (!gasneti_atomic_decrement_and_test(&exit_lock, 0)) {
      /* poll until it is time to exit */
      while (!gasneti_atomic_read(&gasnetc_exit_done, GASNETI_ATOMIC_RMB_PRE)) {
        gasneti_sched_yield(); /* NOT safe to use sleep() here - conflicts with alarm() */
      }
      gasnetc_exit_tail();
      /* NOT REACHED */
    }
  }

#if GASNETC_USE_RCV_THREAD
  /* Stop AM receive thread, if applicable (won't kill self) */
  gasnetc_sndrcv_stop_thread(0);
#endif

  /* read exit code, stored by first caller to gasnetc_exit_head() */
  exitcode = gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE);

  /* Establish a last-ditch signal handler in case of failure. */
  alarm(0);
  gasneti_reghandler(SIGALRM, gasnetc_exit_sighandler);
  #if GASNET_DEBUG
    gasneti_reghandler(SIGABRT, SIG_DFL);
  #else
    gasneti_reghandler(SIGABRT, gasnetc_exit_sighandler);
  #endif
  gasneti_reghandler(SIGILL,  gasnetc_exit_sighandler);
  gasneti_reghandler(SIGSEGV, gasnetc_exit_sighandler);
  gasneti_reghandler(SIGFPE,  gasnetc_exit_sighandler);
  gasneti_reghandler(SIGBUS,  gasnetc_exit_sighandler);

  /* Disable processing of AMs, except core-specific ones */
  gasnetc_disable_AMs();

  GASNETI_TRACE_PRINTF(C,("gasnet_exit(%i)\n", exitcode));

  /* Timed MAX(exitcode) reduction to clearly distinguish collective exit */
  alarm(2 + (int)gasnetc_exittimeout);
  graceful = (gasnetc_exit_reduce(exitcode, timeout_us) == 0);
  alarm(0);

  GASNETC_EXIT_STATE("dumping final stats");
  alarm(60);
#if GASNET_TRACE
  { gasneti_heapstats_t stats;
    gasneti_getheapstats(&stats);
    GASNETI_TRACE_PRINTF(I, ("Conduit-internal memory use (%scludes segment):",
                             GASNETC_PIN_SEGMENT ? "in" : "ex"));
    GASNETI_TRACE_PRINTF(I, ("  allocated: %12"PRIu64" bytes in %8"PRIu64" objects",
                             stats.live_bytes, stats.live_objects));
    GASNETI_TRACE_PRINTF(I, ("     pinned: %12"PRIu64" bytes in %8"PRIu64" objects",
                             (uint64_t)gasnetc_pinned_bytes,
                             (uint64_t)gasnetc_pinned_blocks));
    GASNETI_TRACE_PRINTF(I, ("      total: %12"PRIu64" bytes in %8"PRIu64" objects",
                             (uint64_t)(stats.live_bytes + gasnetc_pinned_bytes),
                             (uint64_t)(stats.live_objects + gasnetc_pinned_blocks)));
  }
 #if PLATFORM_OS_LINUX
  { FILE *fp;
    char line[256];
    if (NULL != (fp = fopen("/proc/self/status","r"))) {
      while (fgets(line, sizeof(line)-1, fp)) {
        if (!strncmp(line, "Vm", 2)) {
          GASNETI_TRACE_PRINTF(I, ("%s", line));
        }
      }
      fclose(fp);
    }
  }
 #endif
#endif
  gasnetc_connect_fini(gasnetc_ep0); /* just stats reporting */
  alarm(0);

  /* Try to flush out all the output, allowing upto 60s */
  GASNETC_EXIT_STATE("flushing output");
  alarm(60);
  {
    gasneti_flush_streams();
    gasneti_trace_finish();
    alarm(0);
    gasneti_sched_yield();
  }

 if (!graceful) { /* Skip the complex case unless the reduction timed-out */
#if GASNET_DEBUG_VERBOSE
  fprintf(stderr, "Exitcode reduction timed-out on node %d\n", (int)gasneti_mynode);
#endif

  exitcode = gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE);

  /* Determine our role (master or slave) in the coordination of this shutdown */
  GASNETC_EXIT_STATE("performing non-collective exit");
  alarm(10);
  role = gasnetc_get_exit_role();

  /* Attempt a coordinated shutdown */
  GASNETC_EXIT_STATE("coordinating shutdown");
  alarm(1 + (int)gasnetc_exittimeout);
  switch (role) {
  case GASNETC_EXIT_ROLE_MASTER:
    /* send all the remote exit requests and wait for the replies */
    graceful = (gasnetc_exit_master(exitcode, timeout_us) == 0);
    break;

  case GASNETC_EXIT_ROLE_SLAVE:
    /* wait for the exit request and reply before proceeding */
    graceful = (gasnetc_exit_slave(timeout_us) == 0);
    break;

  default:
      gasneti_fatalerror("invalid exit role");
  }
 }

  /* Note we skip cleanly shutdown on non-colective exit or exit via signal */
  if (graceful && !gasnetc_exit_in_signal) {
  #if GASNETC_IBV_SHUTDOWN
    GASNETC_EXIT_STATE("ibv quiesce");
    alarm(30);
    gasneti_bootstrapBarrier();
    gasnetc_sndrcv_quiesce();
  #endif
    if (gasnetc_did_firehose_init) {
      GASNETC_EXIT_STATE("in firehose_fini()");
      alarm(10);
      firehose_fini();
    }
  #if GASNETC_IBV_SHUTDOWN
    GASNETC_EXIT_STATE("ibv shutdown");
    alarm(30);
    gasnetc_shutdown();
  #endif
    alarm(0);
  }

#if GASNETC_IBV_ODP
  // Always need to shutdown ODP (safe no-op if we did full shutdown above)
  GASNETC_EXIT_STATE("odp shutdown");
  alarm(30);
  gasnetc_odp_shutdown(); // Avoid possible system memory leak
#endif

  /* Try again to flush out any recent output, allowing upto 30s */
  GASNETC_EXIT_STATE("closing output");
  alarm(30);
  {
    gasneti_flush_streams();
    #if !GASNET_DEBUG_VERBOSE
      gasneti_close_streams();
    #endif
  }

  /* XXX potential problems here if exiting from the "Wrong" thread, or from a signal handler */
  alarm(60);
  {
    if (graceful) {
      #if GASNET_DEBUG_VERBOSE
	fprintf(stderr, "Graceful exit initiated by node %d\n", (int)gasneti_mynode);
      #endif
      GASNETC_EXIT_STATE("in gasneti_bootstrapFini()");
      gasneti_bootstrapFini();
    } else {
      /* We couldn't reach our peers, so hope the bootstrap code can kill the entire job */
      #if GASNET_DEBUG_VERBOSE
	fprintf(stderr, "Ungraceful exit initiated by node %d\n", (int)gasneti_mynode);
      #endif
      GASNETC_EXIT_STATE("in gasneti_bootstrapAbort()");
      gasneti_bootstrapAbort(exitcode);
      /* NOT REACHED */
    }
  }

  alarm(0);
}

/* gasnetc_exit_reqh
 *
 * This is a core AM handler and is therefore available as soon as gasnet_init()
 * returns, even before gasnet_attach().  This handler is responsible for receiving the
 * remote exit requests from the master node and replying.  We call gasnetc_exit_head()
 * with the exitcode seen in the remote exit request.  If this remote request is seen before
 * any local exit requests (normal or signal), then we are also responsible for starting the
 * exit procedure, via gasnetc_exit_{body,tail}().  Additionally, we are responsible for
 * firing off a SIGQUIT to let the user's handler, if any, run before we begin to exit.
 */
static void gasnetc_exit_reqh(gex_Token_t token, gex_AM_Arg_t arg0) {
  /* The master will send this AM, but should _never_ receive it */
  gasneti_assert(gasneti_atomic_read(&gasnetc_exit_role, 0) != GASNETC_EXIT_ROLE_MASTER);

  /* We should never receive this AM multiple times */
  gasneti_assert(gasneti_atomic_read(&gasnetc_exit_reqs, 0) == 0);

  /* If we didn't already know, we are now certain our role is "slave" */
  (void)gasneti_atomic_compare_and_swap(&gasnetc_exit_role, GASNETC_EXIT_ROLE_UNKNOWN, GASNETC_EXIT_ROLE_SLAVE, 0);

  /* Send a reply so the master knows we are reachable */
  gasnetc_counter_inc(&gasnetc_exit_repl_oust);
  GASNETI_SAFE(gasnetc_ReplySysShort(token, &gasnetc_exit_repl_oust,
				   gasneti_handleridx(gasnetc_exit_reph), /* no args */ 0));
  gasneti_sync_writes(); /* For non-atomic portion of gasnetc_exit_repl_oust */

  /* Count the exit requests, so gasnetc_exit_slave() knows when to return */
  gasneti_atomic_increment(&gasnetc_exit_reqs, 0);

  /* Initiate an exit IFF this is the first we've heard of it */
  if (gasnetc_exit_head(arg0)) {
    gasneti_sighandlerfn_t handler;
    /* IMPORTANT NOTE
     * When we reach this point we are in a request handler which will never return.
     * Care should be taken to ensure this doesn't wedge the AM recv logic.
     *
     * This is currently safe because:
     * 1) request handlers are run w/ no locks held
     * 2) we poll for AMs in all the places we need them
     */

    /* To try and be reasonably robust, want to avoid performing the shutdown and exit from signal
     * context if we can avoid it.  However, we must raise SIGQUIT if the user has registered a handler.
     * Therefore we inspect what is registered before calling raise().
     *
     * XXX we don't do this atomically w.r.t the signal
     * XXX we don't do the right thing w/ SIG_ERR and SIG_HOLD
     */
    handler = gasneti_reghandler(SIGQUIT, SIG_IGN);
    if ((handler != gasneti_defaultSignalHandler) &&
#ifdef SIG_HOLD
	(handler != (gasneti_sighandlerfn_t)SIG_HOLD) &&
#endif
	(handler != (gasneti_sighandlerfn_t)SIG_ERR) &&
	(handler != (gasneti_sighandlerfn_t)SIG_IGN) &&
	(handler != (gasneti_sighandlerfn_t)SIG_DFL)) {
      (void)gasneti_reghandler(SIGQUIT, handler);
      #if 1
        raise(SIGQUIT);
        /* Note: Both ISO C and POSIX assure us that raise() won't return until after the signal handler
         * (if any) has executed.  However, if that handler calls gasnetc_exit(), we'll never return here. */
      #elif 0
	kill(getpid(),SIGQUIT);
      #else
	handler(SIGQUIT);
      #endif
    } else {
      /* No need to restore the handler, since _exit_body will set it to SIG_IGN anyway. */
    }

    gasnetc_exit_body();
    gasnetc_exit_tail();
    /* NOT REACHED */
  }

  return;
}

/* gasnetc_exit_reph
 *
 * Simply count replies
 */
static void gasnetc_exit_reph(gex_Token_t token) {
  gasneti_atomic_increment(&gasnetc_exit_reps, 0);
}
  
/* gasnetc_atexit
 *
 * This is a simple (at,on_}exit() handler to achieve a hopefully graceful exit.
 * We use the functions gasnetc_exit_{head,body}() to coordinate the shutdown.
 * Note that we don't call gasnetc_exit_tail() since we anticipate the normal
 * exit() procedures to shutdown the multi-threaded process nicely and also
 * because with atexit() we don't have access to the exit code!
 *
 * With atexit(), we don't have access to the exit code to send to the other
 * nodes in the event this is a non-collective exit.  However, experience with at
 * lease one MPI suggests that when using MPI for bootstrap a non-zero return from
 * at least one executable is sufficient to produce that non-zero exit code from
 * the parallel job.  Therefore, we can "safely" pass 0 to our peers and still
 * expect to preserve a non-zero exit code for the GASNet job as a whole.  Of course
 * there is no _guarantee_ this will work with all bootstraps.
 */
static void gasnetc_atexit(int exitcode) {
  /* Check return from _head to avoid reentrance */
  if (gasnetc_exit_head(exitcode)) {
    gasnetc_exit_body();
  }
  return;
}

static void gasnetc_exit_init(void) {
  // register an exit-time callback for ODP (needed for GASNET_CATCH_EXIT=0 case)
#if GASNETC_IBV_ODP
  if (gasnetc_use_odp) {
  #if HAVE_ON_EXIT
    on_exit(gasnetc_odp_on_exit, NULL);
  #else
    atexit(gasnetc_odp_atexit);
  #endif
  }
#endif

  /* Handler for non-collective returns from main() */
  // register process exit-time hook
  gasneti_registerExitHandler(gasnetc_atexit);

#if GASNET_PSHM
  /* Extract info from nodemap that we'll need at exit */
  if (gasneti_nodemap_local_rank) {
    gasnetc_exit_parent = gasneti_nodemap[gasneti_mynode];
    gasnetc_exit_children = gasneti_nodes;
  } else {
    const gex_Rank_t children = gasneti_nodemap_local_count - 1;

    if (children) {
      const size_t len = children * sizeof(gex_Rank_t);
      gasnetc_exit_children = children;
      gasnetc_exit_child = gasneti_malloc(len);
      gasneti_leak(gasnetc_exit_child);
      GASNETI_MEMCPY(gasnetc_exit_child, gasneti_nodemap_local+1, len);
    }
  }
#endif
}

/* gasnetc_exit
 *
 * This is the start of a locally requested exit from GASNet.
 * The caller might be the user, some part of the conduit which has detected an error,
 * or possibly gasneti_defaultSignalHandler() responding to a termination signal.
 */
extern void gasnetc_exit(int exitcode) {
  gasnetc_exit_head(exitcode);
  gasnetc_exit_body();
  gasnetc_exit_tail();
  /* NOT REACHED */
}

/* ------------------------------------------------------------------------------------ */
/*
  Misc. Active Message Functions
  ==============================
*/
#if GASNET_PSHM
/* (###) GASNETC_GET_HANDLER
 *   If your conduit will support PSHM, then there needs to be a way
 *   for PSHM to see your handler table.  If you use the recommended
 *   implementation then you don't need to do anything special.
 *   Othwerwise, #define GASNETC_GET_HANDLER in gasnet_core_fwd.h and
 *   implement gasnetc_get_handler() as a macro in
 *   gasnet_core_internal.h
 */
#endif

// NOTE: unlike other conduits this gets used outside the file, and w/ AMPSHM tokens too!
gex_Rank_t gasnetc_msgsource(gex_Token_t token) {
  gex_Rank_t sourceid;
  gasneti_assert(token);

  if (gasnetc_token_in_nbrhd(token)) {
    gex_Token_Info_t info;
    unsigned int rc = gasnetc_nbrhd_Token_Info(token, &info, GEX_TI_SRCRANK);
    gasneti_assert(rc & GEX_TI_SRCRANK);
    sourceid = info.gex_srcrank;
  } else {
    uint32_t flags = ((gasnetc_rbuf_t *)token)->rbuf_flags;

    if (GASNETC_MSG_HANDLERID(flags) >= GASNETE_HANDLER_BASE) {
      GASNETI_CHECKATTACH();
    }

    sourceid = GASNETC_MSG_SRCIDX(flags);
  }

  gasneti_assert(sourceid < gasneti_nodes);
  return sourceid;
}

extern gex_TI_t gasnetc_Token_Info(
                gex_Token_t         token,
                gex_Token_Info_t    *info,
                gex_TI_t            mask)
{
  gasneti_assert(token);
  gasneti_assert(info);

  if (gasnetc_token_in_nbrhd(token)) {
    return gasnetc_nbrhd_Token_Info(token, info, mask);
  }

  const gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
  uint32_t flags = rbuf->rbuf_flags;
  gex_TI_t result = 0;

  if (GASNETC_MSG_HANDLERID(flags) >= GASNETE_HANDLER_BASE) GASNETI_CHECKATTACH();

  info->gex_srcrank = GASNETC_MSG_SRCIDX(flags);
  result |= GEX_TI_SRCRANK;

#if GASNET_TRACE
  // TRACE of source of bootstrap AMs can reach here before gasneti_THUNK_TM is set
  info->gex_ep = gasneti_THUNK_TM ? gasneti_THUNK_EP : NULL;
#else
  info->gex_ep = gasneti_THUNK_EP;
#endif
  result |= GEX_TI_EP;

  info->gex_entry = &rbuf->rr_ep->_amtbl[GASNETC_MSG_HANDLERID(flags)];
  result |= GEX_TI_ENTRY;

  info->gex_is_req = GASNETC_MSG_ISREQUEST(flags);
  result |= GEX_TI_IS_REQ;

  info->gex_is_long = (gasneti_Long == GASNETC_MSG_CATEGORY(flags));
  result |= GEX_TI_IS_LONG;

  return GASNETI_TOKEN_INFO_RETURN(result, info, mask);
}

extern int gasnetc_AMPoll(GASNETI_THREAD_FARG_ALONE) {
  GASNETI_CHECKATTACH();
  gasnetc_poll_both();

  return GASNET_OK;
}


/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/

// Helper to select an IB-level cep for AM Request injection
static
gasnetc_cep_t *gasnetc_am_select_cep(gasnetc_EP_t ep, gasnetc_epid_t dest)
{
#if GASNETC_IBV_SHUTDOWN
  /* Currently only the shutdown code uses dest to specify a "bound" value */
  gasnetc_cep_t *cep = gasnetc_get_cep(ep, gasnetc_epid2node(dest));
  const gasnetc_epid_t dest_qpi = gasnetc_epid2qpi(dest);
#else
  gasnetc_cep_t *cep = gasnetc_get_cep(ep, dest);
  gasneti_assume(gasnetc_epid2qpi(dest) == 0);
#endif

#if GASNETC_IBV_SHUTDOWN
  if_pf (dest_qpi) {
    cep += (dest_qpi - 1);
  } else
#endif
  {
    // SRQ requires shift to upper half of table
    cep += gasnetc_use_srq ? gasnetc_num_qps : 0;

#if 0
    /* TODO: could bind by largest avail credits (or at least favor non-zero over zero) */
    int qpi = 0;
    if (gasnetc_num_qps > 1) {
      uint32_t best_credits;
      int i;
      /* gasnetc_poll_snd(); here? */
      best_credits = gasnetc_sema_read(&cep[0].am_rem);
      for (i = 1; i < gasnetc_num_qps; ++i) {
        const uint32_t tmp = gasnetc_sema_read(&cep[i].am_rem);
        if (tmp > best_credits) {
          best_credits = tmp;
          qpi = i;
        }
      }
    }
    cep += qpi;
#else
    cep += gasnetc_epid_select_qpi(cep, dest);
#endif
  }

  return cep;
}

size_t gasnetc_am_inline_limit_sndrcv = 0;

// Common logic for immediate allocation failures
#if GASNETC_IMMEDIATE_AMPOLLS
  // Perform a full Poll, but only once and only the selected HCA
  // TODO-EX: stats/trace for this as distinct from ..._STALL
  static int gasnetc_imm_alloc_failure(
                          gasnetc_sema_t * const sema,
                          gasnetc_EP_t ep,
                          gasnetc_cep_t *cep
                          GASNETI_THREAD_FARG)
  {
    gasnetc_poll_rcv_hca(ep, cep->hca, GASNETC_RCV_REAP_LIMIT GASNETI_THREAD_PASS);
    if (gasnetc_sema_trydown(sema)) return 0;
    return 1;
  }
#else
  // Never Poll
  #define gasnetc_imm_alloc_failure(sema,ep,cep_and_TI) 1
#endif

// Common code for spinning on allocation failures
GASNETI_NEVER_INLINE(gasnetc_am_sema_poll,
static
void gasnetc_am_sema_poll(gasnetc_sema_t * const sema,
                          gasnetc_EP_t ep
                          GASNETI_THREAD_FARG))
{
  do {
    GASNETI_WAITHOOK();
    gasnetc_poll_rcv_all(ep, 1 GASNETI_THREAD_PASS);
  } while (!gasnetc_sema_trydown(sema));
}

// Helper for allocation of flow-control credit
// and (if needed) SRQ reply buffer accounting
static
int gasnetc_am_get_credit(gasnetc_EP_t ep,
                          gasnetc_cep_t *cep,
                          gex_Flags_t immediate
                          GASNETI_THREAD_FARG)
{
  GASNETC_STAT_EVENT(GET_AMREQ_CREDIT);
  {
    gasnetc_sema_t * const sema = &(cep->am_rem);
    if_pf (!gasnetc_sema_trydown(sema)) {
      if (immediate) {
        return gasnetc_imm_alloc_failure(sema, ep, cep GASNETI_THREAD_PASS);
      } else {
        GASNETC_TRACE_WAIT_BEGIN();
        gasnetc_am_sema_poll(sema, ep GASNETI_THREAD_PASS);
        GASNETC_TRACE_WAIT_END(GET_AMREQ_CREDIT_STALL);
      }
    }
  }

#if GASNETC_IBV_SRQ
  if (gasnetc_use_srq) {
    gasnetc_sema_t * const sema = &(cep->hca->am_sema);
    if_pf (!gasnetc_sema_trydown(sema)) {
      if (immediate) {
        if (gasnetc_imm_alloc_failure(sema, ep, cep GASNETI_THREAD_PASS)) {
          gasnetc_sema_up(&cep->am_rem); // Restore the allocated credit
          return 1;
        }
      } else {
        GASNETC_TRACE_WAIT_BEGIN();
        gasnetc_am_sema_poll(sema, ep GASNETI_THREAD_PASS);
        GASNETC_TRACE_WAIT_END(GET_AMREQ_BUFFER_STALL);
      }
    }
  }
#endif

  return 0;
}


// Helper for DEallocation of flow-control credit, and (if needed)
// SRQ reply buffer accounting, upon IMMEDIATE failure. 
static
void gasnetc_am_put_credit(gasnetc_cep_t *cep)
{
#if GASNETC_IBV_SRQ
  if (gasnetc_use_srq) {
    // Account for the posted-but-not-consumed reply buffer
    gasnetc_sema_up(&cep->hca->am_sema);
  }
#endif
  // Restore the allocated credit
  gasnetc_sema_up(&cep->am_rem);
}

// Decide if Medium (or Packed Long) payload should be sent using gather-on-send
GASNETI_INLINE(gasnetc_am_use_gather)
int gasnetc_am_use_gather(gasnetc_EP_t ep,
                          void *src_addr, int nbytes,
                          gasnetc_cb_t local_cb)
{
#if GASNETC_PIN_SEGMENT
  return ((nbytes >= gasnetc_am_gather_min) &&   // Big enough to benefit
          (local_cb != gasnetc_cb_counter) &&    // Caller did NOT require synchronous LC
          (gasnetc_use_odp ||                    // Registered via OPD ...
           gasnetc_in_bound_segment(ep, (uintptr_t)src_addr, nbytes))); // ... or in-segment
#else
  return 0;
#endif
}

// Helper to allocate a local buffer
// INLINE because we don't want to always force (multiple) outputs to memory
GASNETI_INLINE(gasnetc_am_get_buffer)
int gasnetc_am_get_buffer(size_t buf_len,
                          gex_Flags_t immediate,
                          void *inline_buf,
                          // Outputs:
                          gasnetc_buffer_t **buf_p,
                          gasnetc_buffer_t **buf_alloc_p

                          GASNETI_THREAD_FARG)
{
  void *buf;
  if (buf_len <= gasnetc_am_inline_limit_sndrcv) {
    // Inline send/put using the on-stack buffer
    buf = (gasnetc_buffer_t *)GASNETI_ALIGNUP(inline_buf, 8);
    gasneti_assert(*buf_alloc_p == NULL);
  } else {
    // Allocation of a pinned bounce buffer
    buf = gasnetc_get_bbuf(!immediate GASNETI_THREAD_PASS);
    if (!buf) {
      // TODO-EX: stats/trace for this as distinct from ..._STALL
      gasneti_assert(immediate);
      return 1;
    }
    *buf_alloc_p = buf;
  }
  *buf_p = buf;

  return 0;
}

// Common logic to "commit" (construct and inject)
// INLINE due to specialization opportunity on (at least) category and is_reply
GASNETI_INLINE(gasnetc_am_commit)
void gasnetc_am_commit(   gasnetc_buffer_t *buf, gasnetc_buffer_t *buf_alloc,
                          const gasneti_category_t category, const int is_reply,
                          gasnetc_EP_t ep, gasnetc_cep_t *cep,
                          gex_AM_Index_t handler,
                          void *src_addr, size_t nbytes, void *dst_addr,
                          size_t head_len, size_t copy_len, size_t gath_len,
                          int in_place, const int have_flow, int numargs,
                          gasnetc_atomic_val_t *local_cnt,
                          gasnetc_cb_t local_cb,
                          gasnetc_counter_t *counter, va_list argptr
                          GASNETI_THREAD_FARG)
{
    // Set header fields and locate arguments
    gex_AM_Arg_t *args;
    switch (category) {
    case gasneti_Short:
      args = buf->shortmsg.args;
      break;
  
    case gasneti_Medium:
      buf->medmsg.nBytes = nbytes;
      args = buf->medmsg.args;
      if (in_place) {
        gasneti_assert_ptr(src_addr ,==, GASNETC_MSG_MED_DATA(buf, numargs + have_flow));
        gasneti_assert_uint(copy_len ,==, nbytes);
      } else if (copy_len) {
        void *data = (void*)((uintptr_t)buf + head_len);
        gasneti_assert_ptr(data ,==, GASNETC_MSG_MED_DATA(buf, numargs + have_flow));
        gasneti_assert_uint(copy_len ,==, nbytes);
        GASNETI_MEMCPY_SAFE(data, src_addr, copy_len);
      }
      break;
  
    case gasneti_Long:
      buf->longmsg.destLoc = (uintptr_t)dst_addr;
      buf->longmsg.nBytes  = nbytes;
      if (copy_len + gath_len) { // Packed Long optimization
        gasneti_assume(nbytes <= GASNETC_MAX_PACKEDLONG);
        buf->longmsg.nBytes |= 0x80000000; /* IDs the packedlong case */
        if (copy_len) {
          void *data = (void*)((uintptr_t)buf + head_len);
          gasneti_assert_ptr(data ,==, GASNETC_MSG_LONG_DATA(buf, numargs + have_flow));
          gasneti_assert_uint(copy_len ,==, nbytes);
          GASNETI_MEMCPY_SAFE(data, src_addr, copy_len);
        } else {
          gasneti_assert_uint(gath_len ,==, nbytes);
        }
      }
      args = buf->longmsg.args;
      break;

    default: gasneti_unreachable_error(("Invalid AM category: 0x%x",(int)category));
    }
   
    /* Assemble an array of arguments. */
    if (have_flow) {
      /* credits travel packed in a "prefixed" argument, remaining args are shifted */

      /* "Grab" info w/ atomic load-and-clear: */
      const uint32_t credits = gasnetc_atomic_swap(&cep->am_flow.credit, 0, 0);
      gasneti_assume(credits <= 255);

      args[0] = credits | ((numargs + 1) << 16);

      GASNETI_TRACE_PRINTF(C,("SND_AM_CREDITS credits=%d\n", credits));
    }
    for (int i = have_flow; i < (numargs + have_flow); ++i) {
      args[i] = va_arg(argptr, gex_AM_Arg_t);
    }
  
    /* send the AM */
    {
      GASNETC_DECL_SR_DESC(sr_desc, 2);
      gasnetc_sreq_t *sreq;
      int numargs_field = have_flow ? GASNETC_MAX_ARGS : numargs;

      sr_desc->imm_data   = GASNETC_MSG_GENFLAGS(!is_reply, category, numargs_field, handler,
						 gasneti_mynode);
      sr_desc->opcode     = IBV_WR_SEND_WITH_IMM;
      sr_desc->num_sge    = 1;
      sr_desc->sg_list[0].addr   = (uintptr_t)buf;
      sr_desc->sg_list[0].length = head_len + (in_place ? nbytes : copy_len);
      sr_desc->sg_list[0].lkey   = GASNETC_SND_LKEY(cep);
  
      sreq = gasnetc_get_sreq(GASNETC_OP_AM GASNETI_THREAD_PASS);
      sreq->am_buff = buf_alloc;

      if_pf (counter) { // Caller requires remote completion indication
        gasneti_assert(!gath_len);
        sreq->comp.cb = gasnetc_cb_counter;
        sreq->comp.data = &counter->initiated;
      }
      #if GASNETC_PIN_SEGMENT
      else if (gath_len) { // Gather-on-send to concatenate header and payload
        sr_desc->num_sge = 2;
        sr_desc->sg_list[1].length = gath_len;
        sr_desc->sg_list[1].addr   = (uintptr_t)src_addr;
        #if GASNETC_IBV_ODP
          sr_desc->sg_list[1].lkey = gasnetc_use_odp
                                         ? cep->hca->implicit_odp.lkey
                                         : GASNETC_SEG_LKEY(ep, cep);
        #else
          sr_desc->sg_list[1].lkey = GASNETC_SEG_LKEY(ep, cep);
        #endif
        sreq->comp.cb = local_cb;
        sreq->comp.data = local_cnt;
        ++(*local_cnt);
      }
      #endif
  
      (void)gasnetc_bind_cep_am(ep, cep->epid, sreq, is_reply);

      gasnetc_snd_post_common(sreq, sr_desc, !buf_alloc GASNETI_THREAD_PASS);
    }
}

// Subject to specialization upon inlining (e.g. category and is_reply)
GASNETI_INLINE(gasnetc_ReqRepGeneric)
int gasnetc_ReqRepGeneric(gasnetc_EP_t ep,
                          const gasneti_category_t category, const int is_reply,
			  gasnetc_cep_t *cep,
                          gex_AM_Index_t handler,
			  void *src_addr, size_t nbytes, void *dst_addr,
			  gex_Flags_t flags, int numargs,
			  gasnetc_atomic_val_t *local_cnt,
                          gasnetc_cb_t local_cb,
			  gasnetc_counter_t *counter, va_list argptr
                          GASNETI_THREAD_FARG)
{
    gex_Flags_t immediate = flags & GEX_FLAG_IMMEDIATE;
    int fail_type = GASNETC_FAIL_IMM;
   
    /* Reserve space for 1 extra argument if we *might* carry flow control
     * data.  We need to know numargs before we allocate a large enough
     * buffer, which could block and thus delay the credit update.  So, we
     * allow a race where we allocate space, but later send only zeros.
     */
    const int have_flow = gasnetc_atomic_read(&(cep)->am_flow.credit, 0) ? 1 : 0;

    // Figure out lengths so we know if we can use inline or not.
    // Also starts Long payload Put if possible/necessary
    size_t head_len;     // Length of header (padded in case of Medium)
    size_t copy_len = 0; // Length of payload to be copied (if any)
    size_t gath_len = 0; // Length of payload to be sent using gather-on-send (if any)
    switch (category) {
    case gasneti_Short:
      head_len = GASNETC_MSG_SHORT_ARGSEND(numargs + have_flow);
#if !GASNETC_ALLOW_0BYTE_MSG
      if (!head_len) head_len = 4; /* Mellanox bug (zero-length sends) work-around */
#endif
      break;

    case gasneti_Medium:
      /* XXX: When nbytes == 0 GASNETC_MSG_MED_ARGSEND still rounds up to 8-byte boundary */
      head_len = GASNETC_MSG_MED_ARGSEND(numargs + have_flow);
      if (gasnetc_am_use_gather(ep, src_addr, nbytes, local_cb)) {
        gath_len = nbytes;
      } else {
        copy_len = nbytes;
      }
      break;

    case gasneti_Long:
      head_len = GASNETC_MSG_LONG_ARGSEND(numargs + have_flow);
      if ((nbytes <= gasnetc_packedlong_limit) || (!GASNETC_PIN_SEGMENT && is_reply)) {
        /* Small enough to send like a Medium (always true of Reply when using remote firehose) */
        if (gasnetc_am_use_gather(ep, src_addr, nbytes, local_cb)) {
          gath_len = nbytes;
        } else {
          copy_len = nbytes;
        }
      } else {
        /* When using firehose, Long Requests may need AMs for firehose moves and
         * so we MUST do any RDMA before getting credits.  It can't hurt to queue
         * the Long RDMA as early as possible even when firehose is not in use.
         */
        // Firehose replies MUST take the packedlong path:
        gasneti_assert(GASNETC_PIN_SEGMENT || !is_reply);
        // TODO-EX: should we pass flags other than 'immediate' to the payload Put
        int rc = gasnetc_rdma_long_put(ep, cep, src_addr, dst_addr, nbytes, immediate,
                                       local_cnt, local_cb GASNETI_THREAD_PASS);
        if (rc) {
          // TODO-EX: stats/trace for FAIL_IMM case
          fail_type = rc;
          goto out_no_put;
        }
        immediate = 0; // No turing back now!
        // TODO-EX: can we delay the Put to avoid disabling 'immediate'?
      }
      break;
  
    default: gasneti_unreachable_error(("Invalid AM category: 0x%x",(int)category));
    }

    // Get the flow-control credit needed for Requests before a buffer.
    // This order ensures that we never hold the last pinned buffer
    // while spinning on the rcv queue waiting for credits.
    // If using SRQ, will also account for the rbuf needed for the Reply.
    if (!is_reply) {
      if (gasnetc_am_get_credit(ep, cep, immediate GASNETI_THREAD_PASS)) {
        goto out_no_credit;
      }
    }

    // If we can be certain that the message will be small enough for an
    // inline send/put, then we'll use this stack buffer (saving us from
    // accessing the shared pool of bounce buffers, which might block).
    char inline_buf[sizeof(gasnetc_am_tmp_buf_t) + 8];

   { // Start of scope: 'buf_alloc'

    // Obtain an appropriate buffer in which to build the message
    gasnetc_buffer_t *buf, *buf_alloc = NULL;
    if (gasnetc_am_get_buffer(head_len + copy_len + gath_len, immediate, inline_buf,
                              &buf, &buf_alloc GASNETI_THREAD_PASS)) {
      gasneti_assert(immediate);
      goto out_no_buffer;
    }

    // Build and send the message
    gasnetc_am_commit(buf, buf_alloc, category, is_reply, ep, cep,
                      handler, src_addr, nbytes, dst_addr,
                      head_len, copy_len, gath_len, 0, have_flow, numargs,
                      local_cnt, local_cb, counter, argptr
                      GASNETI_THREAD_PASS);

   } // End of scope: 'buf_alloc'

    GASNETI_RETURN(GASNET_OK); // Normal return

  // "unwind" logic for failure returns:

  out_no_buffer:

    if (!is_reply) {
      gasnetc_am_put_credit(cep);
    }

  out_no_credit:

  out_no_put:

    return fail_type; // TODO-EX: this will eventually distinguish IMMEDIATE vs (resilient) failure
}

GASNETI_INLINE(gasnetc_RequestGeneric)
int gasnetc_RequestGeneric(gasnetc_EP_t ep,
                          const gasneti_category_t category,
                          gasnetc_epid_t dest, gex_AM_Index_t handler,
                          void *src_addr, size_t nbytes, void *dst_addr,
                          gex_Flags_t flags, int numargs,
                          gasnetc_atomic_val_t *local_cnt,
                          gasnetc_cb_t local_cb,
                          gasnetc_counter_t *counter, va_list argptr
                          GASNETI_THREAD_FARG)
{
  gasneti_assert(!GASNETI_NBRHD_JOBRANK_IS_LOCAL(gasnetc_epid2node(dest))); // never local

  return gasnetc_ReqRepGeneric(
                        ep, category, 0,  gasnetc_am_select_cep(ep, dest),
                        handler, src_addr, nbytes, dst_addr, flags, numargs,
                        local_cnt, local_cb, counter, argptr GASNETI_THREAD_PASS);
}

GASNETI_INLINE(gasnetc_ReplyGeneric)
int gasnetc_ReplyGeneric(gasnetc_EP_t ep,
                          const gasneti_category_t category,
                          gasnetc_rbuf_t * const token,
                          gex_AM_Index_t handler,
                          void *src_addr, size_t nbytes, void *dst_addr,
                          gex_Flags_t flags, int numargs,
                          gasnetc_atomic_val_t *local_cnt,
                          gasnetc_cb_t local_cb,
                          gasnetc_counter_t *counter, va_list argptr
                          GASNETI_THREAD_FARG)
{
  gasneti_assert(token);
  gasneti_assert(token->rbuf_handlerRunning);
  gasneti_assert(GASNETC_MSG_ISREQUEST(token->rbuf_flags));
  gasneti_assert(token->rbuf_needReply);

  int rc = gasnetc_ReqRepGeneric(
                        ep, category, 1, token->cep,
                        handler, src_addr, nbytes, dst_addr, flags, numargs,
                        local_cnt, local_cb, counter, argptr GASNETI_THREAD_PASS);

  token->rbuf_needReply = rc; // 0 on success, 1 on immediate-failure (no branch)

  return rc;
}

extern int gasnetc_RequestSysShort(gasnetc_epid_t dest,
                                 gasnetc_counter_t *counter,
                                 gex_AM_Index_t handler,
                                 int numargs, ...) {
  GASNET_BEGIN_FUNCTION();
  int retval;
  va_list argptr;

  const gex_Rank_t jobrank = gasnetc_epid2node(dest);
  GASNETI_TRACE_AMREQUESTSHORT(gasneti_THUNK_TM,jobrank,handler,/*flags*/0,numargs);

  /* ensure AM progress, but NOT progress functions */
  gasnetc_poll_rcv();

  gasnetc_EP_t ep = gasnetc_ep0;
  va_start(argptr, numargs);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    retval = gasnetc_nbrhd_RequestGeneric ( gasneti_Short, jobrank, handler,
                                            NULL, 0, NULL,
                                            0, numargs, argptr GASNETI_THREAD_PASS);
    if_pf (counter) gasnetc_atomic_increment(&counter->completed, 0);
  } else {
    retval = gasnetc_RequestGeneric(ep, gasneti_Short, dest, handler,
                                   NULL, 0, NULL, 0, numargs,
                                   &counter->initiated, gasnetc_cb_counter, counter,
                                   argptr GASNETI_THREAD_PASS);
  }
  va_end(argptr);
  return retval;
}

extern int gasnetc_RequestSysMedium(gasnetc_epid_t dest,
                                    gasnetc_counter_t *counter,
                                    gex_AM_Index_t handler,
                                    void *source_addr, size_t nbytes,
                                    int numargs, ...) {
  GASNET_BEGIN_FUNCTION();
  int retval;
  va_list argptr;

  const gex_Rank_t jobrank = gasnetc_epid2node(dest);
  GASNETI_TRACE_AMREQUESTMEDIUM(gasneti_THUNK_TM,jobrank,handler,source_addr,nbytes,/*flags*/0,numargs);

  /* ensure AM progress, but NOT progress functions */
  gasnetc_poll_rcv();

  gasnetc_EP_t ep = gasnetc_ep0;
  va_start(argptr, numargs);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    retval = gasnetc_nbrhd_RequestGeneric ( gasneti_Medium, jobrank, handler,
                                            source_addr, nbytes, NULL,
                                            0, numargs, argptr GASNETI_THREAD_PASS);
    if_pf (counter) gasnetc_atomic_increment(&counter->completed, 0);
  } else {
    retval = gasnetc_RequestGeneric(ep, gasneti_Medium, dest, handler,
                                   source_addr, nbytes, NULL, 0, numargs,
                                   &counter->initiated, gasnetc_cb_counter, counter,
                                   argptr GASNETI_THREAD_PASS);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_ReplySysShort(gex_Token_t token,
                               gasnetc_counter_t *counter,
                               gex_AM_Index_t handler,
                               int numargs, ...) {
  int retval;
  va_list argptr;
  gasneti_assert(token);
  GASNETI_TRACE_AMREPLYSHORT(token,handler,/*flags*/0,numargs);
  va_start(argptr, numargs);
  if_pt (gasnetc_token_in_nbrhd(token)) {
    retval = gasnetc_nbrhd_ReplyGeneric ( gasneti_Short, token, handler,
                                          NULL, 0, NULL,
                                          0, numargs, argptr);
    if_pf (counter) gasnetc_atomic_increment(&counter->completed, 0);
  } else {
    gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
    GASNET_POST_THREADINFO(rbuf->rbuf_threadinfo);
    gasnetc_EP_t ep = rbuf->rr_ep;
    gasneti_assert(ep == gasnetc_ep0);
    retval = gasnetc_ReplyGeneric(ep, gasneti_Short, rbuf, handler,
                                   NULL, 0, NULL, 0, numargs,
                                   &counter->initiated, gasnetc_cb_counter, counter,
                                   argptr GASNETI_THREAD_PASS);
  }
  va_end(argptr);
  return retval;
}

extern int gasnetc_ReplySysMedium(gex_Token_t token,
                                  gasnetc_counter_t *counter,
                                  gex_AM_Index_t handler,
                                  void *source_addr, size_t nbytes,
                                  int numargs, ...) {
  int retval;
  va_list argptr;
  gasneti_assert(token);
  GASNETI_TRACE_AMREPLYMEDIUM(token,handler,source_addr,nbytes,/*flags*/0,numargs); 
  va_start(argptr, numargs);
  if_pt (gasnetc_token_in_nbrhd(token)) {
    retval = gasnetc_nbrhd_ReplyGeneric ( gasneti_Medium, token, handler,
                                          source_addr, nbytes, NULL,
                                          0, numargs, argptr);
    if_pf (counter) gasnetc_atomic_increment(&counter->completed, 0);
  } else {
    gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
    GASNET_POST_THREADINFO(rbuf->rbuf_threadinfo);
    gasnetc_EP_t ep = rbuf->rr_ep;
    gasneti_assert(ep == gasnetc_ep0);
    retval = gasnetc_ReplyGeneric(ep, gasneti_Medium, rbuf, handler,
                                   source_addr, nbytes, NULL, 0, numargs,
                                   &counter->initiated, gasnetc_cb_counter, counter,
                                   argptr GASNETI_THREAD_PASS);
  }
  va_end(argptr);
  return retval;
}

/* ------------------------------------------------------------------------------------ */
// AM "cores" - no poll or progress fns

GASNETI_INLINE(gasnetc_AMRequestShort)
int gasnetc_AMRequestShort( gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval;
  gasneti_assert(tm);
  gasnetc_EP_t ep = (gasnetc_EP_t)gasneti_import_ep(gex_TM_QueryEP(tm));
  gasneti_assert(ep == gasnetc_ep0);
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    retval = gasnetc_nbrhd_RequestGeneric ( gasneti_Short, jobrank, handler,
                                            NULL, 0, NULL,
                                            0, numargs, argptr GASNETI_THREAD_PASS);
  } else {
    retval = gasnetc_RequestGeneric(ep, gasneti_Short, jobrank, handler,
                                   NULL, 0, NULL,
                                   flags, numargs, NULL, NULL, NULL,
                                   argptr GASNETI_THREAD_PASS);
  }
  return (retval == GASNETC_FAIL_IMM);
}

GASNETI_INLINE(gasnetc_AMRequestMedium)
int gasnetc_AMRequestMedium(gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, 
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval;
  gasneti_assert(tm);
  gasnetc_EP_t ep = (gasnetc_EP_t)gasneti_import_ep(gex_TM_QueryEP(tm));
  gasneti_assert(ep == gasnetc_ep0);
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    gasneti_leaf_finish(lc_opt); // Always synchronous local completion
    retval = gasnetc_nbrhd_RequestGeneric ( gasneti_Medium, jobrank, handler,
                                            source_addr, nbytes, NULL,
                                            flags, numargs, argptr GASNETI_THREAD_PASS);
  } else {
    gasnetc_counter_t    counter = GASNETC_COUNTER_INITIALIZER;
    gasnetc_atomic_val_t *local_cnt, start_cnt;
    gasnetc_cb_t         local_cb;
    gasnete_eop_t        *eop = NULL;

    if (gasneti_leaf_is_pointer(lc_opt)) {
      eop = _gasnete_eop_new(GASNETI_MYTHREAD);
      *lc_opt = (gex_Event_t)eop;
      GASNETE_EOP_LC_START(eop);
      start_cnt = eop->initiated_alc;
      local_cnt = &eop->initiated_alc;
      local_cb = gasnetc_cb_eop_alc;
    } else if (lc_opt == GEX_EVENT_NOW) {
      local_cnt = &counter.initiated;
      local_cb = gasnetc_cb_counter;
    } else if (lc_opt == GEX_EVENT_GROUP) {
      gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
      gasnete_iop_t *op = mythread->current_iop;
      local_cnt = &op->initiated_alc_cnt;
      local_cb = op->next ? gasnetc_cb_nar_alc : gasnetc_cb_iop_alc;
    } else {
      gasneti_fatalerror("Invalid lc_opt argument to RequestMedium");
    }

    retval = gasnetc_RequestGeneric(ep, gasneti_Medium, jobrank, handler,
                                   source_addr, nbytes, NULL,
                                   flags, numargs, local_cnt, local_cb, NULL,
                                   argptr GASNETI_THREAD_PASS);

    if (eop) {
      gasneti_assume(gasneti_leaf_is_pointer(lc_opt)); // avoid maybe-uninitialized warning (bug 3756)
      if (start_cnt == eop->initiated_alc) {
        // Synchronous LC - reset LC state and pass-back INVALID_HANDLE as result
        GASNETE_EOP_LC_FINISH(eop);
        *lc_opt = GEX_EVENT_INVALID;
        gasnete_eop_free(eop GASNETI_THREAD_PASS);
      }
    } else if (lc_opt == GEX_EVENT_NOW) {
#if 0 // Currently always synchronous LC when (local_cb == gasnetc_cb_counter)
      /* block for local completion of payload transfer */
      gasnetc_counter_wait(&counter, 0 GASNETI_THREAD_PASS);
#else
      gasneti_assert(counter.initiated == 0);
      gasneti_assert(gasnetc_atomic_read(&counter.completed,0) == 0);
#endif
    }
  }
  return (retval == GASNETC_FAIL_IMM);
}

GASNETI_INLINE(gasnetc_AMRequestLong)
int gasnetc_AMRequestLong(  gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, void *dest_addr,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  int retval;
  gasneti_assert(tm);
  gasnetc_EP_t ep = (gasnetc_EP_t)gasneti_import_ep(gex_TM_QueryEP(tm));
  gasneti_assert(ep == gasnetc_ep0);
  gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);
  if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
    gasneti_leaf_finish(lc_opt); // Always synchronous local completion
    retval = gasnetc_nbrhd_RequestGeneric ( gasneti_Long, jobrank, handler,
                                            source_addr, nbytes, dest_addr,
                                            flags, numargs, argptr GASNETI_THREAD_PASS);
  } else {
    gasnetc_counter_t    counter = GASNETC_COUNTER_INITIALIZER;
    gasnetc_atomic_val_t *local_cnt, start_cnt;
    gasnetc_cb_t         local_cb;
    gasnete_eop_t        *eop = NULL;

    if (gasneti_leaf_is_pointer(lc_opt)) {
      if (flags & GASNETI_FLAG_LC_OPT_IN) {
        gasnete_op_t *op = gasneti_event_op(*lc_opt);
        if (OPTYPE(op) == OPTYPE_IMPLICIT) {
          gasnete_iop_t *iop = (gasnete_iop_t*)op;
          gasneti_assert(gasneti_event_idx(*lc_opt) == gasnete_iop_event_alc);
          gasneti_assert(iop->next); // Within an NBI access region
          local_cnt = &iop->initiated_alc_cnt;
          local_cb = gasnetc_cb_nar_alc;
        } else {
          eop = (gasnete_eop_t*)op;
          gasneti_assert(gasneti_event_idx(*lc_opt) == gasnete_eop_event_alc);
          GASNETE_EOP_LC_START(eop);
          start_cnt = eop->initiated_alc;
          local_cnt = &eop->initiated_alc;
          local_cb = gasnetc_cb_eop_alc;
        }
      } else {
        eop = _gasnete_eop_new(GASNETI_MYTHREAD);
        *lc_opt = (gex_Event_t)eop;
        GASNETE_EOP_LC_START(eop);
        start_cnt = eop->initiated_alc;
        local_cnt = &eop->initiated_alc;
        local_cb = gasnetc_cb_eop_alc;
      }
    } else if (lc_opt == GEX_EVENT_NOW) {
      local_cnt = &counter.initiated;
      local_cb = gasnetc_cb_counter;
    } else if (lc_opt == GEX_EVENT_GROUP) {
      gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
      gasnete_iop_t *op = mythread->current_iop;
      local_cnt = &op->initiated_alc_cnt;
      local_cb = op->next ? gasnetc_cb_nar_alc : gasnetc_cb_iop_alc;
    } else {
      gasneti_fatalerror("Invalid lc_opt argument to RequestLong");
    }

    retval = gasnetc_RequestGeneric(ep, gasneti_Long, jobrank, handler,
                                   source_addr, nbytes, dest_addr,
                                   flags, numargs, local_cnt, local_cb, NULL,
                                   argptr GASNETI_THREAD_PASS);

    if (lc_opt == GEX_EVENT_NOW) {
      /* block for local completion of RDMA transfer */
      gasnetc_counter_wait(&counter, 0 GASNETI_THREAD_PASS);
    } else if (eop) {
      gasneti_assume(gasneti_leaf_is_pointer(lc_opt)); // avoid maybe-uninitialized warning (bug 3756)
      if (start_cnt == eop->initiated_alc) {
        // Synchronous LC - reset LC state and pass-back INVALID_HANDLE as result
        GASNETE_EOP_LC_FINISH(eop);
        *lc_opt = GEX_EVENT_INVALID;
        if (!(flags & GASNETI_FLAG_LC_OPT_IN)) {
          gasnete_eop_free(eop GASNETI_THREAD_PASS);
        }
      }
    }
  }
  return (retval == GASNETC_FAIL_IMM);
}

GASNETI_INLINE(gasnetc_AMReplyShort)
int gasnetc_AMReplyShort(   gex_Token_t token, gex_AM_Index_t handler,
                            gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  int retval;
  gasneti_assert(token);
  if_pt (gasnetc_token_in_nbrhd(token)) {
    retval = gasnetc_nbrhd_ReplyGeneric ( gasneti_Short, token, handler,
                                          NULL, 0, NULL,
                                          flags, numargs, argptr);
  } else {
    gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
    GASNET_POST_THREADINFO(rbuf->rbuf_threadinfo);
    gasnetc_EP_t ep = rbuf->rr_ep;
    gasneti_assert(ep == gasnetc_ep0);

    retval = gasnetc_ReplyGeneric(ep, gasneti_Short, rbuf, handler,
                                   NULL, 0, NULL, 
                                   flags, numargs, NULL, NULL,
                                   NULL, argptr GASNETI_THREAD_PASS);
    gasneti_assert(!rbuf->rbuf_needReply);
  }
  return (retval == GASNETC_FAIL_IMM);
}

GASNETI_INLINE(gasnetc_AMReplyMedium)
int gasnetc_AMReplyMedium(  gex_Token_t token, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  int retval;
  gasneti_assert(token);
  if_pt (gasnetc_token_in_nbrhd(token)) {
    gasneti_leaf_finish(lc_opt); // Always synchronous local completion
    retval = gasnetc_nbrhd_ReplyGeneric ( gasneti_Medium, token, handler,
                                          source_addr, nbytes, NULL,
                                          flags, numargs, argptr);
  } else {
    gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
    GASNET_POST_THREADINFO(rbuf->rbuf_threadinfo);
    gasnetc_EP_t ep = rbuf->rr_ep;
    gasneti_assert(ep == gasnetc_ep0);

    gasnetc_counter_t    counter = GASNETC_COUNTER_INITIALIZER;
    gasnetc_atomic_val_t *local_cnt, start_cnt;
    gasnetc_cb_t         local_cb;
    gasnete_eop_t        *eop = NULL;

    if (gasneti_leaf_is_pointer(lc_opt)) {
      eop = _gasnete_eop_new(GASNETI_MYTHREAD);
      *lc_opt = (gex_Event_t)eop;
      GASNETE_EOP_LC_START(eop);
      start_cnt = eop->initiated_alc;
      local_cnt = &eop->initiated_alc;
      local_cb = gasnetc_cb_eop_alc;
    } else if (lc_opt == GEX_EVENT_NOW) {
      local_cnt = &counter.initiated;
      local_cb = gasnetc_cb_counter;
    } else {
      gasneti_fatalerror("Invalid lc_opt argument to ReplyMedium");
    }
  
    retval = gasnetc_ReplyGeneric(ep, gasneti_Medium, rbuf, handler,
                                   source_addr, nbytes, NULL,
                                   flags, numargs, local_cnt, local_cb, NULL,
                                   argptr GASNETI_THREAD_PASS);
    gasneti_assert(!rbuf->rbuf_needReply || (flags & GEX_FLAG_IMMEDIATE));

    if (eop) {
      gasneti_assume(gasneti_leaf_is_pointer(lc_opt)); // avoid maybe-uninitialized warning (bug 3756)
      if (start_cnt == eop->initiated_alc) {
        // Synchronous LC - reset LC state and pass-back INVALID_HANDLE as result
        GASNETE_EOP_LC_FINISH(eop);
        *lc_opt = GEX_EVENT_INVALID;
        gasnete_eop_free(eop GASNETI_THREAD_PASS);
      }
    } else if (lc_opt == GEX_EVENT_NOW) {
    #if 0 // Currently always synchronous LC when (local_cb == gasnetc_cb_counter)
      /* block for local completion of payload transfer */
      gasnetc_counter_wait(&counter, 1 /* handler context */ GASNETI_THREAD_PASS);
    #else
      gasneti_assert(counter.initiated == 0);
      gasneti_assert(gasnetc_atomic_read(&counter.completed,0) == 0);
    #endif
    }
  }
  return (retval == GASNETC_FAIL_IMM);
}

GASNETI_INLINE(gasnetc_AMReplyLong)
int gasnetc_AMReplyLong(    gex_Token_t token, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, void *dest_addr,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  int retval;
  gasneti_assert(token);
  if_pt (gasnetc_token_in_nbrhd(token)) {
    gasneti_leaf_finish(lc_opt); // Always synchronous local completion
    retval = gasnetc_nbrhd_ReplyGeneric ( gasneti_Long, token, handler,
                                          source_addr, nbytes, dest_addr,
                                          flags, numargs, argptr);
  } else {
    gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
    GASNET_POST_THREADINFO(rbuf->rbuf_threadinfo);
    gasnetc_EP_t ep = rbuf->rr_ep;
    gasneti_assert(ep == gasnetc_ep0);

  #if GASNETC_PIN_SEGMENT
    gasnetc_counter_t    counter = GASNETC_COUNTER_INITIALIZER;
    gasnetc_atomic_val_t *local_cnt, start_cnt;
    gasnetc_cb_t         local_cb;
    gasnete_eop_t        *eop = NULL;

    if (gasneti_leaf_is_pointer(lc_opt)) {
      if (flags & GASNETI_FLAG_LC_OPT_IN) {
        gasnete_op_t *op = gasneti_event_op(*lc_opt);
        if (OPTYPE(op) == OPTYPE_IMPLICIT) {
          gasnete_iop_t *iop = (gasnete_iop_t*)op;
          gasneti_assert(gasneti_event_idx(*lc_opt) == gasnete_iop_event_alc);
          gasneti_assert(iop->next); // Within an NBI access region
          local_cnt = &iop->initiated_alc_cnt;
          local_cb = gasnetc_cb_nar_alc;
          start_cnt = 0; // DEAD STORE: avoid a warning from gcc-7.3.0/aarch64
        } else {
          eop = (gasnete_eop_t*)op;
          gasneti_assert(gasneti_event_idx(*lc_opt) == gasnete_eop_event_alc);
          GASNETE_EOP_LC_START(eop);
          start_cnt = eop->initiated_alc;
          local_cnt = &eop->initiated_alc;
          local_cb = gasnetc_cb_eop_alc;
        }
      } else {
        eop = _gasnete_eop_new(GASNETI_MYTHREAD);
        *lc_opt = (gex_Event_t)eop;
        GASNETE_EOP_LC_START(eop);
        start_cnt = eop->initiated_alc;
        local_cnt = &eop->initiated_alc;
        local_cb = gasnetc_cb_eop_alc;
      }
    } else if (lc_opt == GEX_EVENT_NOW) {
      local_cnt = &counter.initiated;
      local_cb = gasnetc_cb_counter;
      start_cnt = 0; // DEAD STORE: avoid a warning from gcc-7.3.0/aarch64
    } else {
      gasneti_fatalerror("Invalid lc_opt argument to ReplyLong");
    }

    retval = gasnetc_ReplyGeneric(ep, gasneti_Long, rbuf, handler,
                                   source_addr, nbytes, dest_addr,
                                   flags, numargs, local_cnt, local_cb, NULL,
                                   argptr GASNETI_THREAD_PASS);

    if (lc_opt == GEX_EVENT_NOW) {
      /* block for local completion of RDMA transfer */
      gasnetc_counter_wait(&counter, 1 /* handler context */ GASNETI_THREAD_PASS);
    } else if (eop) {
      gasneti_assume(gasneti_leaf_is_pointer(lc_opt)); // avoid maybe-uninitialized warning (bug 3756)
      if (start_cnt == eop->initiated_alc) {
        // Synchronous LC - reset LC state and pass-back INVALID_HANDLE as result
        GASNETE_EOP_LC_FINISH(eop);
        *lc_opt = GEX_EVENT_INVALID;
        if (!(flags & GASNETI_FLAG_LC_OPT_IN)) {
          gasnete_eop_free(eop GASNETI_THREAD_PASS);
        }
      }
    }
  #else
    gasneti_leaf_finish(lc_opt); // Always "packed long", and thus locally-complete
    retval = gasnetc_ReplyGeneric(ep, gasneti_Long, rbuf, handler,
                                   source_addr, nbytes, dest_addr,
                                   flags, numargs, NULL, NULL, NULL,
                                   argptr GASNETI_THREAD_PASS);
  #endif

    gasneti_assert(!rbuf->rbuf_needReply || (flags & GEX_FLAG_IMMEDIATE));
  }
  return (retval == GASNETC_FAIL_IMM);
}

/* ------------------------------------------------------------------------------------ */

// ---- NPAM common ----

GASNETI_INLINE(gasnetc_prepare_medium)
int gasnetc_prepare_medium(
                       gasneti_AM_SrcDesc_t    sd,
                       gasnetc_cep_t          *cep,
                       const void             *client_buf,
                       size_t                  least_payload,
                       size_t                  most_payload,
                       gex_Event_t            *lc_opt,
                       gex_Flags_t             flags,
                       unsigned int            nargs
                       GASNETI_THREAD_FARG)
{
  const size_t nbytes = MIN(most_payload, GASNETC_MAX_MEDIUM_(nargs));
   
  // See gasnetc_ReqRepGeneric() for details
  const int have_flow = gasnetc_atomic_read(&(cep)->am_flow.credit, 0) ? 1 : 0;
  const size_t head_len = GASNETC_MSG_MED_ARGSEND(nargs + have_flow);

  // Obtain an appropriate buffer in which to build the message
  // If an inline send is to be used, the buffer is in the sd itself
  // TODO: *might* "prove" use of gather-on-send to eliminate buffer allocation
  gasneti_static_assert(sizeof(gasnetc_am_tmp_buf_t) <= 128);
  const gex_Flags_t immediate = flags & GEX_FLAG_IMMEDIATE;
  gasnetc_buffer_t *buf, *buf_alloc = NULL;
  if (gasnetc_am_get_buffer(head_len + nbytes, immediate, &sd->_inline_buf,
                            &buf, &buf_alloc GASNETI_THREAD_PASS)) {
    gasneti_assert(immediate);
    return 1;
  }

  sd->_void_p = buf;
  sd->_lc_opt = lc_opt;
  sd->_size = nbytes;
  sd->_buf_alloc = buf_alloc;
  sd->_have_flow = have_flow;
  sd->_cep = cep;
  if (client_buf) {
    sd->_addr = (/*non-const*/void *)client_buf;
    gasneti_leaf_finish(lc_opt);
  } else {
    sd->_gex_buf = sd->_addr = (void*)((uintptr_t)sd->_void_p + head_len);
  }

  return 0;
}

static
void gasnetc_commit_medium(
                       gasneti_AM_SrcDesc_t    sd,
                       const int               is_reply,
                       gex_AM_Index_t          handler,
                       size_t                  nbytes,
                       unsigned int            nargs,
                       va_list                 argptr
                       GASNETI_THREAD_FARG)
{
  gasnetc_counter_t    counter = GASNETC_COUNTER_INITIALIZER;
  gasnetc_atomic_val_t *local_cnt, start_cnt;
  gasnetc_cb_t         local_cb;
  gasnete_eop_t        *eop = NULL;

  int is_Fixed = sd->_gex_buf == NULL;
  gex_Event_t *lc_opt = sd->_lc_opt;
  size_t copy_len = 0; // Length of payload to be copied (if any)
  size_t gath_len = 0; // Length of payload to be sent using gather-on-send (if any)
  if (is_Fixed) {
    gasneti_assert(lc_opt);
    if (gasneti_leaf_is_pointer(lc_opt)) {
      eop = _gasnete_eop_new(GASNETI_MYTHREAD);
      *lc_opt = (gex_Event_t)eop;
      GASNETE_EOP_LC_START(eop);
      start_cnt = eop->initiated_alc;
      local_cnt = &eop->initiated_alc;
      local_cb = gasnetc_cb_eop_alc;
    } else if (lc_opt == GEX_EVENT_NOW) {
      local_cnt = &counter.initiated;
      local_cb = gasnetc_cb_counter;
    } else if (lc_opt == GEX_EVENT_GROUP) {
      gasneti_assert(! is_reply); // Request only
      gasneti_threaddata_t * const mythread = GASNETI_MYTHREAD;
      gasnete_iop_t *op = mythread->current_iop;
      local_cnt = &op->initiated_alc_cnt;
      local_cb = op->next ? gasnetc_cb_nar_alc : gasnetc_cb_iop_alc;
    } else {
      gasneti_fatalerror("Invalid lc_opt argument to Prepare/Commit %sMedium",
                         is_reply?"Reply":"Request");
    }
    if (gasnetc_am_use_gather(sd->_ep, sd->_addr, nbytes, local_cb)) {
      gath_len = nbytes;
    } else {
      copy_len = nbytes;
    }
  } else {
    gasneti_assert(!lc_opt);
    local_cb = NULL;
    local_cnt = NULL;
    copy_len = nbytes;
  }

  size_t head_len = GASNETC_MSG_MED_ARGSEND(nargs + sd->_have_flow);
  gasnetc_am_commit( sd->_void_p, sd->_buf_alloc,
                     gasneti_Medium, is_reply,
                     sd->_ep,  sd->_cep,
                     handler, sd->_addr, nbytes, NULL,
                     head_len, copy_len, gath_len,
                     !is_Fixed, sd->_have_flow, nargs,
                     local_cnt, local_cb, NULL, argptr
                     GASNETI_THREAD_PASS);

  if (eop) {
    gasneti_assume(gasneti_leaf_is_pointer(lc_opt)); // avoid maybe-uninitialized warning (bug 3756)
    if (start_cnt == eop->initiated_alc) {
      // Synchronous LC - reset LC state and pass-back INVALID_HANDLE as result
      GASNETE_EOP_LC_FINISH(eop);
      *lc_opt = GEX_EVENT_INVALID;
      gasnete_eop_free(eop GASNETI_THREAD_PASS);
    }
  } else if (lc_opt == GEX_EVENT_NOW) {
#if 0 // Currently always synchronous LC when (local_cb == gasnetc_cb_counter)
    /* block for local completion of payload transfer */
    gasnetc_counter_wait(&counter, 0 GASNETI_THREAD_PASS);
#else
    gasneti_assert(counter.initiated == 0);
    gasneti_assert(gasnetc_atomic_read(&counter.completed,0) == 0);
#endif
  }
}

// ---- external FPAM requests ----

extern int gasnetc_AMRequestShortM( 
                            gex_TM_t tm,/* local context */
                            gex_Rank_t rank,       /* with tm, defines remote context */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            gex_Flags_t flags
                            GASNETI_THREAD_FARG,
                            int numargs, ...)
{
  GASNETI_COMMON_AMREQUESTSHORT(tm,rank,handler,flags,numargs);

  /* ensure progress */
  if (GASNETC_IMMEDIATE_WOULD_POLL(flags)) {
    gasnetc_poll_rcv();
    GASNETI_PROGRESSFNS_RUN();
  }

  va_list argptr;
  va_start(argptr, numargs);
  int retval = gasnetc_AMRequestShort(tm,rank,handler,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

#if !GASNETC_HAVE_NP_REQ_MEDIUM
extern int gasnetc_AMRequestMediumV(
                            gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  return gasnetc_AMRequestMedium(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
}
#endif

extern int gasnetc_AMRequestMediumM( 
                            gex_TM_t tm,/* local context */
                            gex_Rank_t rank,       /* with tm, defines remote context */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            gex_Event_t *lc_opt,       /* local completion of payload */
                            gex_Flags_t flags
                            GASNETI_THREAD_FARG,
                            int numargs, ...)
{
  GASNETI_COMMON_AMREQUESTMEDIUM(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs);

  /* ensure progress */
  if (GASNETC_IMMEDIATE_WOULD_POLL(flags)) {
    gasnetc_poll_rcv();
    GASNETI_PROGRESSFNS_RUN();
  }

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMRequestMedium(tm,rank,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return (retval == GASNETC_FAIL_IMM);
}

extern int gasnetc_AMRequestLongV(
                            gex_TM_t tm, gex_Rank_t rank, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, void *dest_addr,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr GASNETI_THREAD_FARG)
{
  return gasnetc_AMRequestLong(tm,rank,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
}

extern int gasnetc_AMRequestLongM(
                            gex_TM_t tm,/* local context */
                            gex_Rank_t rank,       /* with tm, defines remote context */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            gex_Event_t *lc_opt,       /* local completion of payload */
                            gex_Flags_t flags
                            GASNETI_THREAD_FARG,
                            int numargs, ...)
{
  GASNETI_COMMON_AMREQUESTLONG(tm,rank,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs);

  /* ensure progress */
  if (GASNETC_IMMEDIATE_WOULD_POLL(flags)) {
    gasnetc_poll_rcv();
    GASNETI_PROGRESSFNS_RUN();
  }

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMRequestLong(tm,rank,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr GASNETI_THREAD_PASS);
  va_end(argptr);
  return retval;
}

// ---- external NPAM requests ----

#if GASNETC_HAVE_NP_REQ_MEDIUM

extern gex_AM_SrcDesc_t gasnetc_AM_PrepareRequestMedium(
                       gex_TM_t           tm,
                       gex_Rank_t         rank,
                       const void        *client_buf,
                       size_t             least_payload,
                       size_t             most_payload,
                       gex_Event_t       *lc_opt,
                       gex_Flags_t        flags
                       GASNETI_THREAD_FARG,
                       unsigned int       nargs)
{
    gasneti_AM_SrcDesc_t sd = gasneti_init_request_srcdesc(GASNETI_THREAD_PASS_ALONE);
    GASNETI_COMMON_PREP_REQ(sd,tm,rank,client_buf,least_payload,most_payload,NULL,lc_opt,flags,nargs,Medium);

    GASNETC_IMMEDIATE_MAYBE_POLL(flags); // Ensure at least one poll upon Request injection

    flags &= ~(GEX_FLAG_AM_PREPARE_LEAST_CLIENT | GEX_FLAG_AM_PREPARE_LEAST_ALLOC);

    gex_Rank_t jobrank = gasneti_e_tm_rank_to_jobrank(tm, rank);

    if (GASNETI_NBRHD_JOBRANK_IS_LOCAL(jobrank)) {
        sd = gasnetc_nbrhd_PrepareRequest(sd, gasneti_Medium, jobrank,
                                          client_buf, least_payload, most_payload,
                                          NULL, lc_opt, flags, nargs);
    } else {
        const gex_Flags_t immediate = flags & GEX_FLAG_IMMEDIATE;

        gasnetc_EP_t ep = (gasnetc_EP_t)gasneti_import_tm(tm)->_ep;
        gasneti_assert(ep == gasnetc_ep0);
        gasnetc_cep_t *cep = gasnetc_am_select_cep(ep, jobrank);
        if (gasnetc_am_get_credit(ep, cep, immediate GASNETI_THREAD_PASS)) {
            goto out_immediate;
        } else if (gasnetc_prepare_medium(sd, cep, client_buf,
                                          least_payload, most_payload,
                                          lc_opt, flags, nargs
                                          GASNETI_THREAD_PASS)) {
            gasnetc_am_put_credit(cep);
            goto out_immediate;
        } else {
            gasneti_init_sd_poison(sd);
            sd->_is_nbrhd = 0;
            sd->_ep = ep;
        }
    }

    GASNETI_TRACE_PREP_RETURN(REQUEST_MEDIUM, sd);
    return gasneti_export_srcdesc(sd);

out_immediate:
    gasneti_assert(flags & GEX_FLAG_IMMEDIATE);
    gasneti_reset_srcdesc(sd);
    GASNETI_TRACE_PREP_RETURN(REQUEST_MEDIUM, NULL);
    return gasneti_export_srcdesc(NULL); // GEX_AM_SRCDESC_NO_OP
}

extern void gasnetc_AM_CommitRequestMediumM(
                       gex_AM_Index_t          handler,
                       size_t                  nbytes
                       GASNETI_THREAD_FARG,
                       unsigned int            nargs,
                       gex_AM_SrcDesc_t        sd_arg, ...)
{
    gasneti_AM_SrcDesc_t sd = gasneti_import_srcdesc(sd_arg);

    GASNETI_COMMON_COMMIT_REQ(sd,handler,nbytes,NULL,nargs,Medium);

    va_list argptr;
    va_start(argptr, sd_arg);
    if (sd->_is_nbrhd) {
        gasnetc_nbrhd_CommitRequest(sd, gasneti_Medium, handler, nbytes, NULL, argptr);
    } else {
        gasnetc_commit_medium(sd,0,handler,nbytes,nargs,argptr GASNETI_THREAD_PASS);
    }
    va_end(argptr);

    gasneti_reset_srcdesc(sd);
}

#endif // GASNETC_HAVE_NP_REQ_MEDIUM

// ---- external FPAM replies ----

extern int gasnetc_AMReplyShortM( 
                            gex_Token_t token,     /* token provided on handler entry */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            gex_Flags_t flags,
                            int numargs, ...)
{
  gasneti_assert(token);
  GASNETI_COMMON_AMREPLYSHORT(token,handler,flags,numargs);

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMReplyShort(token,handler,flags,numargs,argptr);
  va_end(argptr);
  return retval;
}

#if !GASNETC_HAVE_NP_REP_MEDIUM
extern int gasnetc_AMReplyMediumV(
                            gex_Token_t token, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  return gasnetc_AMReplyMedium(token,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr);
}
#endif

extern int gasnetc_AMReplyMediumM( 
                            gex_Token_t token,     /* token provided on handler entry */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            gex_Event_t *lc_opt,       /* local completion of payload */
                            gex_Flags_t flags,
                            int numargs, ...)
{
  gasneti_assert(token);
  GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,lc_opt,flags,numargs);

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMReplyMedium(token,handler,source_addr,nbytes,lc_opt,flags,numargs,argptr);
  va_end(argptr);
  return retval;
}

extern int gasnetc_AMReplyLongV(
                            gex_Token_t token, gex_AM_Index_t handler,
                            void *source_addr, size_t nbytes, void *dest_addr,
                            gex_Event_t *lc_opt, gex_Flags_t flags,
                            int numargs, va_list argptr)
{
  return gasnetc_AMReplyLong(token,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr);
}

extern int gasnetc_AMReplyLongM( 
                            gex_Token_t token,     /* token provided on handler entry */
                            gex_AM_Index_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            gex_Event_t *lc_opt,       /* local completion of payload */
                            gex_Flags_t flags,
                            int numargs, ...)
{
  gasneti_assert(token);
  GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs);

  va_list argptr;
  va_start(argptr, numargs); /*  pass in last argument */
  int retval = gasnetc_AMReplyLong(token,handler,source_addr,nbytes,dest_addr,lc_opt,flags,numargs,argptr);
  va_end(argptr);
  return retval;
}

// ---- external NPAM replies ----

#if GASNETC_HAVE_NP_REP_MEDIUM

extern gex_AM_SrcDesc_t gasnetc_AM_PrepareReplyMedium(
                       gex_Token_t        token,
                       const void        *client_buf,
                       size_t             least_payload,
                       size_t             most_payload,
                       gex_Event_t       *lc_opt,
                       gex_Flags_t        flags,
                       unsigned int       nargs)
{
    gasneti_AM_SrcDesc_t sd;
    flags &= ~(GEX_FLAG_AM_PREPARE_LEAST_CLIENT | GEX_FLAG_AM_PREPARE_LEAST_ALLOC);

    if (gasnetc_token_in_nbrhd(token)) {
        sd = gasnetc_nbrhd_PrepareReply(gasneti_Medium, token,
                                        client_buf, least_payload, most_payload,
                                        NULL, lc_opt, flags, nargs);
    } else {
        gasnetc_rbuf_t *rbuf = (gasnetc_rbuf_t *)token;
        gasneti_assert(rbuf);
        gasneti_assert(rbuf->rbuf_handlerRunning);
        gasneti_assert(GASNETC_MSG_ISREQUEST(rbuf->rbuf_flags));
        gasneti_assert(rbuf->rbuf_needReply);
        GASNET_POST_THREADINFO(rbuf->rbuf_threadinfo);

        sd = gasneti_init_reply_srcdesc(GASNETI_THREAD_PASS_ALONE);
        GASNETI_COMMON_PREP_REP(sd,token,client_buf,least_payload,most_payload,NULL,lc_opt,flags,nargs,Medium);

        if (gasnetc_prepare_medium(sd, rbuf->cep, client_buf,
                                   least_payload, most_payload,
                                   lc_opt, flags, nargs
                                   GASNETI_THREAD_PASS)) {
            gasneti_reset_srcdesc(sd);
            sd = NULL; // GEX_AM_SRCDESC_NO_OP
        } else {
            gasneti_init_sd_poison(sd);
            sd->_is_nbrhd = 0;
            sd->_ep = rbuf->rr_ep;
            rbuf->rbuf_needReply = 0;
        }
    }

    GASNETI_TRACE_PREP_RETURN(REPLY_MEDIUM, sd);
    return gasneti_export_srcdesc(sd);
}

extern void gasnetc_AM_CommitReplyMediumM(
                       gex_AM_Index_t          handler,
                       size_t                  nbytes,
                       unsigned int            nargs,
                       gex_AM_SrcDesc_t        sd_arg, ...)
{
    gasneti_AM_SrcDesc_t sd = gasneti_import_srcdesc(sd_arg);

    GASNETI_COMMON_COMMIT_REP(sd,handler,nbytes,NULL,nargs,Medium);

    va_list argptr;
    va_start(argptr, sd_arg);
    if (sd->_is_nbrhd) {
        gasnetc_nbrhd_CommitReply(sd, gasneti_Medium, handler, nbytes, NULL, argptr);
    } else {
        GASNET_POST_THREADINFO(sd->_thread);
        gasnetc_commit_medium(sd,1,handler,nbytes,nargs,argptr GASNETI_THREAD_PASS);
    }
    va_end(argptr);

    gasneti_reset_srcdesc(sd);
}

#endif // GASNETC_HAVE_NP_REP_MEDIUM

/* ------------------------------------------------------------------------------------ */
/*
  Handler-safe locks
  ==================
*/
#if !GASNETC_NULL_HSL
extern void gasnetc_hsl_init   (gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();
  gasneti_mutex_init(&(hsl->lock));
}

extern void gasnetc_hsl_destroy(gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();
  gasneti_mutex_destroy(&(hsl->lock));
}

extern void gasnetc_hsl_lock   (gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();

  {
    #if GASNETI_STATS_OR_TRACE
      gasneti_tick_t startlock = GASNETI_TICKS_NOW_IFENABLED(L);
    #endif
    #if GASNETC_HSL_SPINLOCK
      if_pf (gasneti_mutex_trylock(&(hsl->lock)) == EBUSY) {
        if (gasneti_wait_mode == GASNET_WAIT_SPIN) {
          while (gasneti_mutex_trylock(&(hsl->lock)) == EBUSY) {
            gasneti_compiler_fence();
            gasneti_spinloop_hint();
          }
        } else {
          gasneti_mutex_lock(&(hsl->lock));
        }
      }
    #else
      gasneti_mutex_lock(&(hsl->lock));
    #endif
    #if GASNETI_STATS_OR_TRACE
      hsl->acquiretime = GASNETI_TICKS_NOW_IFENABLED(L);
      GASNETI_TRACE_EVENT_TIME(L, HSL_LOCK, hsl->acquiretime-startlock);
    #endif
  }
}

extern void gasnetc_hsl_unlock (gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();

  GASNETI_TRACE_EVENT_TIME(L, HSL_UNLOCK, GASNETI_TICKS_NOW_IFENABLED(L)-hsl->acquiretime);

  gasneti_mutex_unlock(&(hsl->lock));
}

extern int  gasnetc_hsl_trylock(gex_HSL_t *hsl) {
  GASNETI_CHECKATTACH();

  {
    int locked = (gasneti_mutex_trylock(&(hsl->lock)) == 0);

    GASNETI_TRACE_EVENT_VAL(L, HSL_TRYLOCK, locked);
    if (locked) {
      #if GASNETI_STATS_OR_TRACE
        hsl->acquiretime = GASNETI_TICKS_NOW_IFENABLED(L);
      #endif
    }

    return locked ? GASNET_OK : GASNET_ERR_NOT_READY;
  }
}
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Private Handlers:
  ================
  see mpi-conduit and extended-ref for examples on how to declare AM handlers here
  (for internal conduit use in bootstrapping, job management, etc.)
*/
static gex_AM_Entry_t const gasnetc_handlers[] = {
  #ifdef GASNETC_COMMON_HANDLERS
    GASNETC_COMMON_HANDLERS(),
  #endif

  /* ptr-width independent handlers */
  gasneti_handler_tableentry_no_bits(gasnetc_exit_reduce_reqh,2,REQUEST,SHORT,0),
  gasneti_handler_tableentry_no_bits(gasnetc_exit_role_reqh,0,REQUEST,SHORT,0),
  gasneti_handler_tableentry_no_bits(gasnetc_exit_role_reph,1,REPLY,SHORT,0),
  gasneti_handler_tableentry_no_bits(gasnetc_exit_reqh,1,REQUEST,SHORT,0),
  gasneti_handler_tableentry_no_bits(gasnetc_exit_reph,0,REPLY,SHORT,0),
  gasneti_handler_tableentry_no_bits(gasnetc_sys_barrier_reqh,1,REQUEST,SHORT,0),
  gasneti_handler_tableentry_no_bits(gasnetc_sys_exchange_reqh,2,REQUEST,MEDIUM,0),
  #if GASNETC_IBV_SHUTDOWN
    gasneti_handler_tableentry_no_bits(gasnetc_sys_flush_reph,1,REPLY,SHORT,0),
    gasneti_handler_tableentry_no_bits(gasnetc_sys_close_reqh,0,REQUEST,SHORT,0),
  #endif

  /* ptr-width dependent handlers */
  // NONE currently

  GASNETI_HANDLER_EOT
};

gex_AM_Entry_t const *gasnetc_get_handlertable(void) {
  return gasnetc_handlers;
}

/* ------------------------------------------------------------------------------------ */

/* 
  Pthread_create wrapper:
  ======================
  On Darwin we see that if the stacks of pthreads become dynamically pinned,
  then bad things are happening when they are later unmapped.  To deal with
  this we are wrapping pthread_create() to provide our own stacks, which will
  not be unmapped automatically, and additionally we are passing the flag
  FIREHOSE_INIT_FLAG_UNPIN_ON_FINI to unpin all memory before it gets unmapped.
  XXX: Eventually we should be able to firehose_local_invalidate() the stacks
  we create and unmap them rather than leaking them as we do now.
 */

#if defined(GASNETC_PTHREAD_CREATE_OVERRIDE)
  /* Configuration */
  #if PLATFORM_OS_DARWIN
    #include <sys/mman.h>	/* For mprotect */
    #define GASNETC_DEFAULT_PTHREAD_STACKSZ (512*1024)
    #define GASNETC_PTHREAD_STACK_DIR (-1)
  #else
    #error "Override of pthread_create() attempted on unsupported platform"
  #endif

  struct gasnetc_pthread_args {
    void			*real_arg;
    void			*(*real_fn)(void *);
    void			*stackaddr;	/* exclusive of guard pages */
    size_t			stacksize;	/* exclusive of guard pages */
    struct gasnetc_pthread_args	*next;		/* once stacks are unused */
  };

  static struct {
    gasneti_mutex_t		lock;
    struct gasnetc_pthread_args	*list;
  } gasnetc_pthread_dead_stacks = {GASNETI_MUTEX_INITIALIZER, NULL};

  static void gasnetc_pthread_cleanup_fn(void *arg)
  {
    /* Here we form a linked list of the "dead" stacks.
     * However, we can't reuse them until the threads are join()ed since
     * the threads are STILL RUNNING (this very function) when linked in.
     * XXX: Resolve this by overriding pthread_join() and linking there?
     */
    struct gasnetc_pthread_args *args = arg;

    gasneti_mutex_lock(&gasnetc_pthread_dead_stacks.lock);
    args->next = gasnetc_pthread_dead_stacks.list;
    gasnetc_pthread_dead_stacks.list = args;
    gasneti_mutex_unlock(&gasnetc_pthread_dead_stacks.lock);
  }

  static void *gasnetc_pthread_start_fn(void *arg)
  {
    struct gasnetc_pthread_args *args = arg;
    void *retval = NULL;

    pthread_cleanup_push(&gasnetc_pthread_cleanup_fn, arg);
    retval = (*args->real_fn)(args->real_arg);
    pthread_cleanup_pop(1);

    return retval;
  }

  extern int gasnetc_pthread_create(gasneti_pthread_create_fn_t *create_fn, pthread_t *thread, const pthread_attr_t *attr, void * (*fn)(void *), void * arg) {
    pthread_attr_t my_attr = *attr; /* Copy it to maintain 'const' */
    struct gasnetc_pthread_args *args;
    size_t stacksize;
    void *stackaddr;

    /* Get caller's stack size and address */
    gasneti_assert_zeroret(pthread_attr_getstackaddr(&my_attr, &stackaddr));
    gasneti_assert_zeroret(pthread_attr_getstacksize(&my_attr, &stacksize));

    /* Use system's default stacksize if caller passed zero */
    if (!stacksize) {
      #ifdef GASNETC_DEFAULT_PTHREAD_STACKSZ
        stacksize = GASNETC_DEFAULT_PTHREAD_STACKSZ;
      #else
        pthread_attr_t tmp_attr;
        pthread_attr_init(&tmp_attr);
        pthread_attr_getstacksize(&tmp_attr, &stacksize);
        pthread_attr_destroy(&tmp_attr);
        if (!stacksize) {
          gasneti_fatalerror("Failed to determine stack size in gasnetc_pthread_create()");
        }
      #endif
    }

    if (stackaddr) {
      /* XXX: should we just assume they know what they are doing? */
      gasneti_fatalerror("gasnetc_pthread_create() does not support caller provided stack address");
    } else {
      /* Allocate memory w/ room for guard pages at both ends.
       * Note that these are not just for debugging/protection.
       * They also ensure no coalescing with adjacent pinned regions.
       * XXX: Coalescing doesn't matter yet, since we aren't doing
       * a firehose_local_invalidate() on them (we leak them instead).
       */
      stackaddr = gasnetc_mmap(stacksize + 2 * GASNET_PAGESIZE);
      gasneti_assert_zeroret(mprotect(stackaddr, GASNET_PAGESIZE, PROT_NONE));
      gasneti_assert_zeroret(mprotect((void *)((uintptr_t)stackaddr + stacksize + GASNET_PAGESIZE), GASNET_PAGESIZE, PROT_NONE));
      #if (GASNETC_PTHREAD_STACK_DIR < 0)	/* stack grows down */
        stackaddr = (void *)((uintptr_t)stackaddr + stacksize + GASNET_PAGESIZE);
      #elif (GASNETC_PTHREAD_STACK_DIR > 0)	/* stack grows up */
        stackaddr = (void *)((uintptr_t)stackaddr + GASNET_PAGESIZE);
      #else
	#error "Don't know which way stacks grow"
      #endif
    }

    /* Set stack size/addr */
    gasneti_assert_zeroret(pthread_attr_setstackaddr(&my_attr, stackaddr));
    gasneti_assert_zeroret(pthread_attr_setstacksize(&my_attr, stacksize));

    /* Build args structure (leaked by design) */;
    args = gasneti_malloc(sizeof(*args));
    args->real_fn = fn;
    args->real_arg = arg;
    args->stackaddr = stackaddr;
    args->stacksize = stacksize;
    args->next = NULL;

    return (*create_fn)(thread, &my_attr, &gasnetc_pthread_start_fn, args);
  }
#endif /* defined(GASNETC_PTHREAD_CREATE_OVERRIDE) */

/* ------------------------------------------------------------------------------------ */
/* If operating w/o mmap() we need access to a non-wraped free() */

#if !HAVE_MMAP
#undef free
extern int gasnetc_munmap(void *addr, size_t size) {
  free(addr);
  return 0;
}
#endif

/* ------------------------------------------------------------------------------------ */
