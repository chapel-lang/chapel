/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/gasnet_core.c $
 * Description: GASNet ibv conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include <sys/time.h>
#include <sys/resource.h>

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

#if HAVE_SSH_SPAWNER
  GASNETI_IDENT(gasnetc_IdentString_HaveSSHSpawner, "$GASNetSSHSpawner: 1 $");
#endif
#if HAVE_MPI_SPAWNER
  GASNETI_IDENT(gasnetc_IdentString_HaveMPISpawner, "$GASNetMPISpawner: 1 $");
#endif
#if HAVE_PMI_SPAWNER
  GASNETI_IDENT(gasnetc_IdentString_HavePMISpawner, "$GASNetPMISpawner: 1 $");
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Configuration
  ==============
*/

/* Minimum number of pages to reserve for firehoses in SEGMENT_FAST: */
#define GASNETC_MIN_FH_PAGES		4096

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
#define GASNETC_DEFAULT_AM_CREDITS_TOTAL	MIN(256,gasneti_nodes*gasnetc_am_oust_pp)	/* Max AM requests outstanding at source, 0 = automatic */
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
#define GASNETC_DEFAULT_PACKEDLONG_LIMIT	GASNETC_MAX_PACKEDLONG
#if !GASNETC_PIN_SEGMENT
  #define GASNETC_DEFAULT_PUTINMOVE_LIMIT	GASNETC_PUTINMOVE_LIMIT_MAX
#endif

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

#if GASNETC_PIN_SEGMENT
  int			gasnetc_max_regs;
  uintptr_t		gasnetc_seg_start;
  uintptr_t		gasnetc_seg_len;
  uint64_t 		gasnetc_pin_maxsz;
  uint64_t 		gasnetc_pin_maxsz_mask;
  unsigned int		gasnetc_pin_maxsz_shift;
#endif
firehose_info_t	gasnetc_firehose_info;

gasnet_handlerentry_t const *gasnetc_get_handlertable(void);

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
    uintptr_t	memory;		/* How much pinnable (per proc) */
    uint32_t	regions;
    int		num_local;	/* How many procs */
} gasnetc_pin_info_t;
static gasnetc_pin_info_t gasnetc_pin_info;

gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS]; /* handler table */
static int gasnetc_reghandlers(gasnet_handlerentry_t *table, int numentries,
                               int lowlimit, int highlimit,
                               int dontcare, int *numregistered);

static char *gasnetc_ibv_ports;

#if GASNET_TRACE
  static unsigned int	gasnetc_pinned_blocks = 0;
  static size_t		gasnetc_pinned_bytes = 0;
#endif

static int gasnetc_did_firehose_init = 0;

/* ------------------------------------------------------------------------------------ */
/*
  Bootstrap collectives
*/

static void (*gasneti_bootstrapBarrier_p)(void) = NULL;
static void (*gasneti_bootstrapExchange_p)(void *src, size_t len, void *dest) = NULL;
void (*gasneti_bootstrapFini_p)(void) = NULL;
void (*gasneti_bootstrapAbort_p)(int exitcode) = NULL;
void (*gasneti_bootstrapAlltoall_p)(void *src, size_t len, void *dest) = NULL;
void (*gasneti_bootstrapBroadcast_p)(void *src, size_t len, void *dest, int rootnode) = NULL;
void (*gasneti_bootstrapCleanup_p)(void) = NULL;

static int gasneti_bootstrap_native_coll = 0;
static gasnet_node_t gasnetc_dissem_peers = 0;
static gasnet_node_t *gasnetc_dissem_peer = NULL;
static gasnet_node_t *gasnetc_exchange_rcvd = NULL;
static gasnet_node_t *gasnetc_exchange_send = NULL;
#if GASNET_PSHM
static gasnet_node_t *gasnetc_exchange_permute = NULL;
#endif

static void gasnetc_sys_coll_init(void)
{
  int i;

#if GASNET_PSHM
  const gasnet_node_t size = gasneti_nodemap_global_count;
  const gasnet_node_t rank = gasneti_nodemap_global_rank;

  if (gasneti_nodemap_local_rank) {
    /* No network comms */
    goto done;
  }
#else
  const gasnet_node_t size = gasneti_nodes;
  const gasnet_node_t rank = gasneti_mynode;
#endif

  if (size == 1) {
    /* No network comms */
    goto done;
  }

  /* Construct vector of the dissemination peers */
  for (i = 1; i < size; i *= 2) {
    ++gasnetc_dissem_peers;
  }
  gasnetc_dissem_peer = gasneti_malloc(gasnetc_dissem_peers * sizeof(gasnet_node_t));
  gasneti_leak(gasnetc_dissem_peer);
  for (i = 0; i < gasnetc_dissem_peers; ++i) {
    const gasnet_node_t distance = 1 << i;
    const gasnet_node_t peer = (distance <= rank) ? (rank - distance) : (rank + (size - distance));
  #if GASNET_PSHM
    /* Convert supernode numbers to node numbers */
    gasnetc_dissem_peer[i] = gasneti_pshm_firsts[peer];
  #else
    gasnetc_dissem_peer[i] = peer;
  #endif
  }

  /* Compute the recv offset and send count for each step of exchange */
  gasnetc_exchange_rcvd = gasneti_malloc((gasnetc_dissem_peers+1) * sizeof(gasnet_node_t));
  gasnetc_exchange_send = gasneti_malloc(gasnetc_dissem_peers * sizeof(gasnet_node_t));
  { int step;
  #if GASNET_PSHM
    gasnet_node_t *width;
    gasnet_node_t sum1 = 0;
    gasnet_node_t sum2 = 0;
    gasnet_node_t distance, last;

    distance = 1 << (gasnetc_dissem_peers-1);
    last = (distance <= rank) ? (rank - distance) : (rank + (size - distance));

    /* Step 1: determine the "width" of each supernode */
    width = gasneti_calloc(size, sizeof(gasnet_node_t));
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
      gasnet_node_t n;
    
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
        gasnet_node_t *offset = gasneti_malloc(size * sizeof(gasnet_node_t));
        
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
        gasnetc_exchange_permute = gasneti_malloc(gasneti_nodes * sizeof(gasnet_node_t));
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
  gasneti_bootstrap_native_coll = 1;
  gasneti_bootstrapCleanup(); /* No futher use of ssh/mpi/pmi collectives */
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

static void gasnetc_sys_barrier_reqh(gasnet_token_t token, uint32_t arg)
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

extern void gasneti_bootstrapBarrier(void)
{
  if (gasneti_bootstrap_native_coll) {
    static int phase = 0;
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
    phase ^= 1;
  } else {
    (*gasneti_bootstrapBarrier_p)();
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

static uint8_t *gasnetc_sys_exchange_addr(int phase, size_t elemsz)
{
  if (gasnetc_sys_exchange_buf[phase] == NULL) {
  #if GASNETC_USE_RCV_THREAD
    static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;
    gasneti_mutex_lock(&lock);
  #endif
    gasnetc_sys_exchange_buf[phase] = gasneti_malloc(elemsz * gasneti_nodes);
  #if GASNETC_USE_RCV_THREAD
    gasneti_mutex_unlock(&lock);
  #endif
  }
  return gasnetc_sys_exchange_buf[phase];
}

#define GASNETC_SYS_EXCHANGE_MAX GASNETC_MAX_MEDIUM

static void gasnetc_sys_exchange_reqh(gasnet_token_t token, void *buf,
                                 size_t nbytes, uint32_t arg0,
                                 uint32_t elemsz)
{
  const int phase = arg0 & 1;
  const int step = (arg0 >> 1) & 0x0f;
  const int seq = (arg0 >> 5);
  const size_t offset = elemsz * gasnetc_exchange_rcvd[step];
  uint8_t *dest = gasnetc_sys_exchange_addr(phase, elemsz)
                  + offset + (seq * GASNETC_SYS_EXCHANGE_MAX);

  memcpy(dest, buf, nbytes);
  gasnetc_sys_exchange_inc(phase, step);
}

extern void gasneti_bootstrapExchange(void *src, size_t len, void *dest)
{
  if (gasneti_bootstrap_native_coll) {
    static int phase = 0;

    uint8_t *temp = gasnetc_sys_exchange_addr(phase, len);
    int step;

#if GASNET_PSHM
    /* Construct supernode-local contribution */
    gasneti_pshmnet_bootstrapGather(gasneti_request_pshmnet, src, len, temp, 0);
    if (gasneti_nodemap_local_rank) goto end_network_comms;
#else
    /* Copy in local contribution */
    memcpy(temp, src, len);
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
      gasnet_node_t n;
      for (n = 0; n < gasneti_nodes; ++n) {
        const gasnet_node_t peer = gasnetc_exchange_permute[n];
        memcpy((uint8_t*) dest + len * peer, temp + len * n, len);
      }
    } else
#endif
    {
      memcpy(dest, temp + len * (gasneti_nodes - gasneti_mynode), len * gasneti_mynode);
      memcpy((uint8_t*)dest + len * gasneti_mynode, temp, len * (gasneti_nodes - gasneti_mynode));
    }

#if GASNET_PSHM
end_network_comms:
    gasneti_pshmnet_bootstrapBroadcast(gasneti_request_pshmnet, dest, len*gasneti_nodes, dest, 0);
#endif

    /* Prepare for next */
    gasneti_free(temp);
    gasnetc_sys_exchange_buf[phase] = NULL;
    gasneti_sync_writes();
    phase ^= 1;
  } else {
    (*gasneti_bootstrapExchange_p)(src, len, dest);
  }

#if GASNET_DEBUG
  /* verify own data as a sanity check */
  if (memcmp(src, (void *) ((uintptr_t ) dest + (gasneti_mynode * len)), len) != 0) {
    gasneti_fatalerror("exchange failed: self data on node %d is incorrect", gasneti_mynode);
  }
#endif
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

  addr = gasneti_mmap(size);
  if (addr != MAP_FAILED) {
    GASNETC_FOR_ALL_HCA_INDEX(h) {
      gasnetc_hca_t *hca = &gasnetc_hca[h];
      vstat = gasnetc_pin(hca, addr, size, (enum ibv_access_flags)0, &reg[h]);
      if (vstat != 0) {
	for (h -= 1; h >= 0; --h) {
          gasnetc_unpin(&gasnetc_hca[h], &reg[h]);
	}
        gasneti_munmap(addr, size);
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
    addr = gasneti_mmap(step);
    if (addr != MAP_FAILED) {
      gasnetc_fakepin(limit - step, step);
      gasneti_munmap(addr, step);
    }
  }
}
#endif

/* Search for the total amount of memory we can pin per process.
 */
static void gasnetc_init_pin_info(int first_local, int num_local) {
  gasnetc_pin_info_t *all_info = gasneti_malloc(gasneti_nodes * sizeof(gasnetc_pin_info_t));
  unsigned long limit;
  int do_probe = 1;
  int i;

  /* 
   * We bound our search by the smallest of:
   *   2/3 of physical memory (1/4 or 1GB for Darwin)
   *   User's current (soft) mlock limit (optional)
   *   env(GASNET_PHYSMEM_MAX)
   *   if FIREHOSE_M and FIREHOSE_MAXVICTIM_M are both set:
   *     (SEGMENT_FAST ? MMAP_LIMIT : 0 ) + (FIREHOSE_M + FIREHOSE_MAXVICTIM_M + eplison)
   */

#if PLATFORM_OS_DARWIN
  /* Note bug #532: Pin requests >= 1GB kill Cluster X nodes */
  limit = MIN((gasneti_getPhysMemSz(1) / 4) - 1, 0x3fffffff /*1GB-1*/);
#else
  limit = 2 * (gasneti_getPhysMemSz(1) / 3);
#endif
  #if defined(RLIMIT_MEMLOCK) && GASNETC_HONOR_RLIMIT_MEMLOCK
  { /* Honor soft mlock limit (build-time option) */
    struct rlimit r;
    if ((getrlimit(RLIMIT_MEMLOCK, &r) == 0) && (r.rlim_cur != RLIM_INFINITY)) {
      limit = MIN(limit, r.rlim_cur);
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
    }
  }
  { /* Honor PHYSMEM_MAX if set */
    unsigned long tmp = gasneti_getenv_int_withdefault("GASNET_PHYSMEM_MAX", 0, 1);
    if (tmp) {
      limit = MIN(limit, tmp);
      if_pf (gasneti_getenv_yesno_withdefault("GASNET_PHYSMEM_NOPROBE", 0)) {
	/* Force use of PHYSMEM_MAX w/o probing */
	limit = tmp;
	do_probe = 0;
      }
    }
  }
  limit = GASNETI_PAGE_ALIGNDOWN(limit);

  if_pf (limit == 0) {
    gasneti_fatalerror("Failed to determine the available physical memory");
  }

  gasnetc_pin_info.memory    = ~((uintptr_t)0);
  gasnetc_pin_info.num_local = num_local;
  gasnetc_pin_info.regions = gasnetc_fh_maxregions;
  GASNETC_FOR_ALL_HCA_INDEX(i) {
    gasnetc_pin_info.regions = MIN(gasnetc_pin_info.regions, gasnetc_hca[i].hca_cap.max_mr);
  }

  if (do_probe) {
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
    gasneti_bootstrapExchange(&gasnetc_pin_info, sizeof(gasnetc_pin_info_t), all_info);
#if GASNET_ALIGNED_SEGMENTS  /* Just a waste of time otherwise */
    if (gasneti_mynode != first_local) {
      /* Extra mmap traffic to ensure compatible VM spaces */
      gasnetc_fakepin(all_info[first_local].memory, step);
    }
    gasneti_bootstrapBarrier(); /* Ensure fakepin completes unmap before continuing */
#endif
  } else {
    /* Note that README says PHYSMEM_NOPROBE must be equal on all nodes */
    gasnetc_pin_info.memory = limit;
    gasneti_bootstrapExchange(&gasnetc_pin_info, sizeof(gasnetc_pin_info_t), all_info);
  }

  /* Determine the global values (min of maxes) from the local values */
  for (i = 0; i < gasneti_nodes; i++) {
    gasnetc_pin_info_t *info = &all_info[i];

    info->memory = GASNETI_PAGE_ALIGNDOWN(info->memory / info->num_local);
    info->regions /= info->num_local;

    gasnetc_pin_info.memory  = MIN(gasnetc_pin_info.memory,  info->memory );
    gasnetc_pin_info.regions = MIN(gasnetc_pin_info.regions, info->regions);
  }
  gasneti_free(all_info);
}

#if GASNET_TRACE
static const char *mtu_to_str(enum ibv_mtu mtu) {
  switch (mtu) {
  case IBV_MTU_256 : return "256";
  case IBV_MTU_512 : return "512";
  case IBV_MTU_1024: return "1024";
  case IBV_MTU_2048: return "2048";
  case IBV_MTU_4096: return "4096";
  default                  : return "unknown";
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
  
  GASNETC_ENVINT(i, GASNET_MAX_MTU, 0, 0, 1);
  switch (i) {
    default: fprintf(stderr,
                     "WARNING: ignoring invalid GASNET_MAX_MTU value %d.\n",
                     i);
             /* fall through to "auto" case: */
  case    0: /* TODO: "automatic" might be more sophisticated */
             /* Our historic default is 1k, which is a good latency-vs-bandwidth compromise */
             gasnetc_max_mtu = IBV_MTU_1024;
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
  GASNETC_ENVINT(gasnetc_amrdma_max_peers, GASNET_AMRDMA_MAX_PEERS, GASNETC_DEFAULT_AMRDMA_MAX_PEERS, 0, 0);
  GASNETC_ENVINT(gasnetc_amrdma_limit, GASNET_AMRDMA_LIMIT, GASNETC_DEFAULT_AMRDMA_LIMIT, 0, 1);
  if_pf (gasnetc_amrdma_limit > GASNETC_AMRDMA_LIMIT_MAX) {
    fprintf(stderr,
            "WARNING: GASNET_AMRDMA_LIMIT reduced from the requested value, %d, to the maximum supported value, %d.\n",
            (int)gasnetc_amrdma_limit, (int)GASNETC_AMRDMA_LIMIT_MAX);
    gasnetc_amrdma_limit = GASNETC_AMRDMA_LIMIT_MAX;
  }
  GASNETC_ENVINT(gasnetc_amrdma_depth, GASNET_AMRDMA_DEPTH, GASNETC_DEFAULT_AMRDMA_DEPTH, 0, 0);
  if_pf (!GASNETI_POWEROFTWO(gasnetc_amrdma_depth)) {
    gasneti_fatalerror("GASNET_AMRDMA_DEPTH (%d) is not a power of 2", gasnetc_amrdma_depth);
  }
  if_pf (gasnetc_amrdma_depth > GASNETC_AMRDMA_DEPTH_MAX) {
    fprintf(stderr,
            "WARNING: GASNET_AMRDMA_DEPTH reduced from the requested value, %d, to the maximum supported value, %d.\n",
            (int)gasnetc_amrdma_depth, (int)GASNETC_AMRDMA_DEPTH_MAX);
    gasnetc_amrdma_depth = GASNETC_AMRDMA_DEPTH_MAX;
  }
  gasnetc_amrdma_slot_mask = (gasnetc_amrdma_depth - 1);

  GASNETC_ENVINT(gasnetc_amrdma_cycle, GASNET_AMRDMA_CYCLE, GASNETC_DEFAULT_AMRDMA_CYCLE, 0, 0);
  if_pf (gasnetc_amrdma_cycle && !GASNETI_POWEROFTWO(gasnetc_amrdma_cycle)) {
    gasneti_fatalerror("GASNET_AMRDMA_CYCLE (%d) is neither 0 nor a power of 2", gasnetc_amrdma_cycle);
  }
  if_pf (gasnetc_amrdma_cycle > (GASNETI_ATOMIC_MAX >> 2)) {
    fprintf(stderr,
            "WARNING: GASNET_AMRDMA_CYCLE reduced from the requested value, 0x%lx, to the maximum supported value, 0x%lx.\n",
            (unsigned long)gasnetc_amrdma_cycle, (unsigned long)(GASNETI_ATOMIC_MAX >> 2));
    gasnetc_amrdma_cycle = (GASNETI_ATOMIC_MAX >> 2);
  }

  #if GASNETC_PIN_SEGMENT
    GASNETC_ENVINT(gasnetc_pin_maxsz, GASNET_PIN_MAXSZ, 0, 0, 1);
    if (!gasnetc_pin_maxsz) {
      /* 0=automatic (default).  Will setup later */
    } else if (!GASNETI_POWEROFTWO(gasnetc_pin_maxsz)) {
      gasneti_fatalerror("GASNET_PIN_MAXSZ (%llu) is not a power of 2", (unsigned long long)gasnetc_pin_maxsz);
    } else if (gasnetc_pin_maxsz < GASNET_PAGESIZE) {
      gasneti_fatalerror("GASNET_PIN_MAXSZ (%lu) is less than GASNET_PAGESIZE (%lu)",
                         (unsigned long)gasnetc_pin_maxsz, (unsigned long)GASNET_PAGESIZE);
    }
  #else
    GASNETC_ENVINT(gasnetc_putinmove_limit, GASNET_PUTINMOVE_LIMIT, GASNETC_DEFAULT_PUTINMOVE_LIMIT, 0, 1);
    if_pf (gasnetc_putinmove_limit > GASNETC_PUTINMOVE_LIMIT_MAX) {
      gasneti_fatalerror("GASNET_PUTINMOVE_LIMIT (%lu) is larger than the max permitted (%lu)", (unsigned long)gasnetc_putinmove_limit, (unsigned long)GASNETC_PUTINMOVE_LIMIT_MAX);
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
  if (gasnetc_use_srq > gasneti_nodes) {
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
#if GASNETC_PIN_SEGMENT
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_PIN_MAXSZ                = %lu%s", (unsigned long)gasnetc_pin_maxsz,
				(!gasnetc_pin_maxsz ? " (automatic)" : "")));
#endif
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_INLINESEND_LIMIT         = %d%s", (int)gasnetc_inline_limit,
				(gasnetc_inline_limit == (size_t)-1 ? " (automatic)" : "")));
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_NONBULKPUT_BOUNCE_LIMIT  = %u", (unsigned int)gasnetc_bounce_limit));
#if !GASNETC_PIN_SEGMENT
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_PUTINMOVE_LIMIT          = %u", (unsigned int)gasnetc_putinmove_limit));
#endif
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_AMRDMA_MAX_PEERS         = %u", (unsigned int)gasnetc_amrdma_max_peers));
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_AMRDMA_DEPTH             = %u", (unsigned int)gasnetc_amrdma_depth));
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_AMRDMA_LIMIT             = %u", (unsigned int)gasnetc_amrdma_limit));
  GASNETI_TRACE_PRINTF(I,  ("  GASNET_AMRDMA_CYCLE             = %lu", (unsigned long)gasnetc_amrdma_cycle));
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

static int  gasneti_bootstrapInit(int *argc_p, char ***argv_p,
				  gasnet_node_t *nodes_p, gasnet_node_t *mynode_p) {
  char *spawner = gasneti_getenv_withdefault("GASNET_IB_SPAWNER", "(not set)");
  int res = GASNET_ERR_NOT_INIT;

#if HAVE_SSH_SPAWNER
  /* Sigh.  We can't assume GASNET_IB_SPAWNER has been set except in the master.
   * However, gasneti_bootstrapInit_ssh() verifies the command line args and
   * returns GASNET_ERR_NOT_INIT on failure witout any noise on stderr.
   * So, we try ssh-based spawn first.
   */
  if (GASNET_OK != res &&
      GASNET_OK == (res = gasneti_bootstrapInit_ssh(argc_p, argv_p, nodes_p, mynode_p))) {
    gasneti_bootstrapFini_p	= &gasneti_bootstrapFini_ssh;
    gasneti_bootstrapAbort_p	= &gasneti_bootstrapAbort_ssh;
    gasneti_bootstrapBarrier_p	= &gasneti_bootstrapBarrier_ssh;
    gasneti_bootstrapExchange_p	= &gasneti_bootstrapExchange_ssh;
    gasneti_bootstrapAlltoall_p	= &gasneti_bootstrapAlltoall_ssh;
    gasneti_bootstrapBroadcast_p= &gasneti_bootstrapBroadcast_ssh;
    gasneti_bootstrapCleanup_p  = &gasneti_bootstrapCleanup_ssh;
  }
#endif

#if HAVE_MPI_SPAWNER
  /* Only try MPI-based spawn when spawner == "mpi".
   * Otherwise things could hang or fail in "messy" ways here.
   */
  if (GASNET_OK != res && !strcmp(spawner, "mpi") && 
      GASNET_OK == (res = gasneti_bootstrapInit_mpi(argc_p, argv_p, nodes_p, mynode_p))) {
    gasneti_bootstrapFini_p	= &gasneti_bootstrapFini_mpi;
    gasneti_bootstrapAbort_p	= &gasneti_bootstrapAbort_mpi;
    gasneti_bootstrapBarrier_p	= &gasneti_bootstrapBarrier_mpi;
    gasneti_bootstrapExchange_p	= &gasneti_bootstrapExchange_mpi;
    gasneti_bootstrapAlltoall_p	= &gasneti_bootstrapAlltoall_mpi;
    gasneti_bootstrapBroadcast_p= &gasneti_bootstrapBroadcast_mpi;
    gasneti_bootstrapCleanup_p  = &gasneti_bootstrapCleanup_mpi;
  }
#endif

#if HAVE_PMI_SPAWNER
  /* Don't expect GASNET_IB_SPAWNER set if launched directly by srun, mpirun, yod, etc.
   * So, we try pmi-based spawn last.
   */
  if (GASNET_OK != res &&
      GASNET_OK == (res = gasneti_bootstrapInit_pmi(argc_p, argv_p, nodes_p, mynode_p))) {
    gasneti_bootstrapFini_p	= &gasneti_bootstrapFini_pmi;
    gasneti_bootstrapAbort_p	= &gasneti_bootstrapAbort_pmi;
    gasneti_bootstrapBarrier_p	= &gasneti_bootstrapBarrier_pmi;
    gasneti_bootstrapExchange_p	= &gasneti_bootstrapExchange_pmi;
    gasneti_bootstrapAlltoall_p	= &gasneti_bootstrapAlltoall_pmi;
    gasneti_bootstrapBroadcast_p= &gasneti_bootstrapBroadcast_pmi;
    gasneti_bootstrapCleanup_p  = &gasneti_bootstrapCleanup_pmi;
  }
#endif

  if (GASNET_OK != res
#if HAVE_SSH_SPAWNER
      && strcmp(spawner, "ssh")
#endif
#if HAVE_MPI_SPAWNER
      && strcmp(spawner, "mpi")
#endif
#if HAVE_PMI_SPAWNER
      && strcmp(spawner, "pmi")
#endif
      )
  {
    gasneti_fatalerror("Requested spawner \"%s\" is unknown or not supported in this build", spawner);
  }

  return res;
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

/* Try to find up to *port_count_p ACTIVE ports, replacing w/ the actual count */
static void gasnetc_probe_ports(int max_ports) {
  struct ibv_device	**hca_list;
  int			num_hcas;
  int			ib_hcas;
  gasnetc_port_info_t	*port_tbl;
  int			port_count = 0;
  int			hca_count = 0;
  int			curr_hca;

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
    fprintf(stderr, "WARNING: Found %d IB HCAs, but GASNet was configured with '--with-ibv-max-hcas="
		    _STRINGIFY(GASNETC_IB_MAX_HCAS) "'.  To utilize all your HCAs, you should "
		    "reconfigure GASNet with '--with-ibv-max-hcas=%d'.  You can silence this warning "
		    "by setting the environment variable GASNET_IBV_PORTS as described in the file "
		    "'gasnet/ibv-conduit/README'.\n", num_hcas, num_hcas);
  }

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
	const char *state;

	switch (this_port->port.state) {
	case IBV_PORT_DOWN:
		state = "DOWN";
		break;
	case IBV_PORT_INIT:
		state = "INITIALIZE";
		break;
	case IBV_PORT_ARMED:
		state = "ARMED";
		break;
	default:
		state = "unknown";
        }
        GASNETI_TRACE_PRINTF(C,("Probe skipping HCA '%s', port %d - state = %s", hca_name, curr_port, state));
      }
#endif
    }

    /* Install or release the HCA */
    if (found) {
      gasnetc_hca_t *hca = &gasnetc_hca[hca_count];

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

static int gasnetc_init(int *argc, char ***argv) {
  gasnetc_hca_t		*hca;
  uint16_t		*local_lid;
  uint16_t		*remote_lid;
  gasnet_node_t		node;
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
  i = gasneti_bootstrapInit(argc, argv, &gasneti_nodes, &gasneti_mynode);
  if (i != GASNET_OK) {
    return i;
  }

  gasneti_init_done = 1; /* enable early to allow tracing */

  gasneti_freezeForDebugger();

  /* Now enable tracing of all the following steps */
  gasneti_trace_init(argc, argv);

  /* bootstrapInit may set gasneti_nodes==0 if would overflow gasnet_node_t */
  if (!gasneti_nodes /* || (gasneti_nodes > GASNET_MAXNODES) */) {
    GASNETI_RETURN_ERRR(RESOURCE, "gasnet_nodes exceeds ibv-conduit capabilities");
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

  /* Determine gasnetc_max_msg_sz and dependent variables */
  gasnetc_max_msg_sz = gasnetc_port_tbl[0].port.max_msg_sz;
  for (i = 1; i < gasnetc_num_ports; ++i) {
    gasnetc_max_msg_sz = MIN(gasnetc_max_msg_sz, gasnetc_port_tbl[i].port.max_msg_sz);
  }
  gasnetc_bounce_limit = MIN(gasnetc_max_msg_sz, gasnetc_bounce_limit);

  /* Exchange LIDs */
  local_lid = gasneti_calloc(gasnetc_num_ports, sizeof(uint16_t));
  remote_lid = gasneti_calloc(gasnetc_num_ports * gasneti_nodes, sizeof(uint16_t));
  for (i = 0; i < gasnetc_num_ports; ++i) {
    local_lid[i] = gasnetc_port_tbl[i].port.lid;
  }
  gasneti_bootstrapExchange(local_lid, gasnetc_num_ports * sizeof(uint16_t), remote_lid);
  gasneti_free(local_lid);

  /* Derive nodemap from the LID info we have just exchanged */
  gasneti_nodemapInit(NULL, &remote_lid[0],
                      sizeof(remote_lid[0]),
                      sizeof(remote_lid[0]) * gasnetc_num_ports);

  #if GASNET_PSHM
    gasneti_pshm_init(&gasneti_bootstrapExchange, 0);
  #endif

  /* early registration of core API handlers */
  for (i = 0; i < GASNETC_MAX_NUMHANDLERS; i++) {
      gasnetc_handler[i] = (gasneti_handler_fn_t)&gasneti_defaultAMHandler;
  }
  {
    gasnet_handlerentry_t *ctable = (gasnet_handlerentry_t *)gasnetc_get_handlertable();
    int len = 0;
    int numreg = 0;
    gasneti_assert(ctable);
    while (ctable[len].fnptr) len++; /* calc len */
    if (gasnetc_reghandlers(ctable, len, 1, 63, 0, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering core API handlers");
    gasneti_assert(numreg == len);
  }

  /* record remote lids */
  for (i = 0; i < gasnetc_num_ports; ++i) {
    gasnetc_port_tbl[i].remote_lids = gasneti_malloc(gasneti_nodes * sizeof(uint16_t));
    gasneti_leak(gasnetc_port_tbl[i].remote_lids);
    for (node = 0; node < gasneti_nodes; ++node) {
      gasnetc_port_tbl[i].remote_lids[node] = remote_lid[node * gasnetc_num_ports + i];
    }
  }
  gasneti_free(remote_lid);

  /* compute various snd/rcv resource limits */
  i = gasnetc_sndrcv_limits();
  if (i != GASNET_OK) {
    return i;
  }
  
#if GASNETC_IBV_XRC
  /* allocate/initialize XRC resources, if any */
  if ((gasneti_nodemap_global_count == 1) ||
      (gasneti_nodemap_global_count == gasneti_nodes)) {
    /* No warning.  Includs case(s) that would SEGV otherwise */
    gasnetc_use_xrc = 0;
  } else
  if (gasnetc_use_xrc) {
    i = gasnetc_xrc_init();
    if (i != GASNET_OK) {
      return i;
    }
  }
#endif

  /* allocate/initialize transport resources */
  i = gasnetc_sndrcv_init();
  if (i != GASNET_OK) {
    return i;
  }

  /* Establish static connections and prepare for dynamic ones */
  i = gasnetc_connect_init();
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

  /* Find max pinnable size before we start carving up memory w/ mmap()s.
   *
   * Take care that only one process per LID (node) performs the probe.
   * The result is then divided by the number of processes on the adapter,
   * which is easily determined from the connection information we exchanged above.
   */
  {
    GASNETI_TRACE_PRINTF(C,("I am node %d of %d on-node peers",
                            gasneti_nodemap_local_rank, gasneti_nodemap_local_count));

    /* Query the pinning limits of the HCA */
    gasnetc_init_pin_info(gasneti_nodemap_local[0], gasneti_nodemap_local_count);

    gasneti_assert(gasnetc_pin_info.memory != 0);
    gasneti_assert(gasnetc_pin_info.memory != (uintptr_t)(-1));
    gasneti_assert(gasnetc_pin_info.regions != 0);
  }
 
  #if GASNET_SEGMENT_FAST
  {
    /* Reserved memory needed by firehose on each node */
    /* NOTE: We reserve this memory even when firehose is disabled, since the disable
     * is only made available for debugging. */
    size_t reserved_mem = GASNETC_MIN_FH_PAGES * GASNET_PAGESIZE;

    if_pf (gasnetc_pin_info.memory < reserved_mem) {
      gasneti_fatalerror("Pinnable memory (%lu) is less than reserved minimum %lu\n", (unsigned long)gasnetc_pin_info.memory, (unsigned long)reserved_mem);
    }
    gasneti_segmentInit((gasnetc_pin_info.memory - reserved_mem), &gasneti_bootstrapExchange);
  }
  #elif GASNET_SEGMENT_LARGE
  {
    uintptr_t limit = gasneti_mmapLimit((uintptr_t)-1, (uint64_t)-1,
                                  &gasneti_bootstrapExchange,
                                  &gasneti_bootstrapBarrier);
    gasneti_segmentInit(limit, &gasneti_bootstrapExchange);
  }
  #elif GASNET_SEGMENT_EVERYTHING
    /* segment is everything - nothing to do */
  #endif

  gasnetc_exit_init();

  #if 0
    /* Done earlier to allow tracing */
    gasneti_init_done = 1;  
  #endif
  gasneti_bootstrapBarrier();

  gasneti_auxseg_init(); /* adjust max seg values based on auxseg */

  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
extern int gasnet_init(int *argc, char ***argv) {
  int retval = gasnetc_init(argc, argv);
  if (retval != GASNET_OK) GASNETI_RETURN(retval);
  #if 0
    /* Already done in gasnetc_init() to allow tracing of init steps */
    gasneti_trace_init(argc, argv);
  #endif
  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
static char checkuniqhandler[256] = { 0 };
static int gasnetc_reghandlers(gasnet_handlerentry_t *table, int numentries,
                               int lowlimit, int highlimit,
                               int dontcare, int *numregistered) {
  int i;
  *numregistered = 0;
  for (i = 0; i < numentries; i++) {
    int newindex;

    if ((table[i].index == 0 && !dontcare) || 
        (table[i].index && dontcare)) continue;
    else if (table[i].index) newindex = table[i].index;
    else { /* deterministic assignment of dontcare indexes */
      for (newindex = lowlimit; newindex <= highlimit; newindex++) {
        if (!checkuniqhandler[newindex]) break;
      }
      if (newindex > highlimit) {
        char s[255];
        snprintf(s, sizeof(s), "Too many handlers. (limit=%i)", highlimit - lowlimit + 1);
        GASNETI_RETURN_ERRR(BAD_ARG, s);
      }
    }

    /*  ensure handlers fall into the proper range of pre-assigned values */
    if (newindex < lowlimit || newindex > highlimit) {
      char s[255];
      snprintf(s, sizeof(s), "handler index (%i) out of range [%i..%i]", newindex, lowlimit, highlimit);
      GASNETI_RETURN_ERRR(BAD_ARG, s);
    }

    /* discover duplicates */
    if (checkuniqhandler[newindex] != 0) 
      GASNETI_RETURN_ERRR(BAD_ARG, "handler index not unique");
    checkuniqhandler[newindex] = 1;

    /* register the handler */
    gasnetc_handler[newindex] = table[i].fnptr;

    /* The check below for !table[i].index is redundant and present
     * only to defeat the over-aggressive optimizer in pathcc 2.1
     */
    if (dontcare && !table[i].index) table[i].index = newindex;

    (*numregistered)++;
  }
  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int gasnetc_attach(gasnet_handlerentry_t *table, int numentries,
                          uintptr_t segsize, uintptr_t minheapoffset) {
  gasnetc_hca_t *hca;
  void *segbase = NULL;
  size_t maxsize = 0;
  int numreg = 0;
  gasnet_node_t i;
  
  GASNETI_TRACE_PRINTF(C,("gasnetc_attach(table (%i entries), segsize=%lu, minheapoffset=%lu)",
                          numentries, (unsigned long)segsize, (unsigned long)minheapoffset));

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
    if ((minheapoffset % GASNET_PAGESIZE) != 0) /* round up the minheapoffset to page sz */
      minheapoffset = ((minheapoffset / GASNET_PAGESIZE) + 1) * GASNET_PAGESIZE;
  #elif GASNET_SEGMENT_EVERYTHING
    segsize = 0;
    minheapoffset = 0;
  #endif

  segsize = gasneti_auxseg_preattach(segsize); /* adjust segsize for auxseg reqts */

  /* ------------------------------------------------------------------------------------ */
  /*  register handlers */
#if 0 /* Was done early in gasnetc_init() */
  { int i;
    for (i = 0; i < GASNETC_MAX_NUMHANDLERS; i++) 
      gasnetc_handler[i] = (gasneti_handler_fn_t)&gasneti_defaultAMHandler;
  }
  { /*  core API handlers */
    gasnet_handlerentry_t *ctable = (gasnet_handlerentry_t *)gasnetc_get_handlertable();
    int len = 0;
    gasneti_assert(ctable);
    while (ctable[len].fnptr) len++; /* calc len */
    if (gasnetc_reghandlers(ctable, len, 1, GASNETE_HANDLER_BASE-1, 0, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering core API handlers");
    gasneti_assert(numreg == len);
  }
#endif

  { /*  extended API handlers */
    gasnet_handlerentry_t *etable = (gasnet_handlerentry_t *)gasnete_get_handlertable();
    int len = 0;
    gasneti_assert(etable);
    while (etable[len].fnptr) len++; /* calc len */
    if (gasnetc_reghandlers(etable, len, GASNETE_HANDLER_BASE, 127, 0, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering extended API handlers");
    gasneti_assert(numreg == len);
  }

  #if GASNETC_PIN_SEGMENT
    /* No firehose AMs should ever be sent in this configuration */
  #else
  { /* firehose handlers */
    gasnet_handlerentry_t *ftable = (gasnet_handlerentry_t *)firehose_get_handlertable();
    int len = 0;
    int base = GASNETE_HANDLER_BASE + numreg;	/* start right after etable */
    gasneti_assert(ftable);
    while (ftable[len].fnptr) len++; /* calc len */
    gasneti_assert(base + len <= 128);	/* enough space remaining after etable? */
    if (gasnetc_reghandlers(ftable, len, base, 127, 1, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE, "Error registering firehose handlers");
    gasneti_assert(numreg == len);
  }
  #endif

  if (table) { /*  client handlers */
    int numreg1 = 0;
    int numreg2 = 0;

    /*  first pass - assign all fixed-index handlers */
    if (gasnetc_reghandlers(table, numentries, 128, 255, 0, &numreg1) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering fixed-index client handlers");

    /*  second pass - fill in dontcare-index handlers */
    if (gasnetc_reghandlers(table, numentries, 128, 255, 1, &numreg2) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering variable-index client handlers");

    gasneti_assert(numreg1 + numreg2 == numentries);
  }

  /* ------------------------------------------------------------------------------------ */
  /*  register fatal signal handlers */

  /* catch fatal signals and convert to SIGQUIT */
  gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);

  /*  (###) register any custom signal handlers required by your conduit 
   *        (e.g. to support interrupt-based messaging)
   */

  /* ------------------------------------------------------------------------------------ */
  /*  register segment  */

  gasneti_seginfo = (gasnet_seginfo_t *)gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t));
  gasneti_leak(gasneti_seginfo);

  #if GASNET_SEGMENT_EVERYTHING
  {
    for (i=0;i<gasneti_nodes;i++) {
      gasneti_seginfo[i].addr = (void *)0;
      gasneti_seginfo[i].size = (uintptr_t)-1;
    }
    segbase = (void *)0;
    segsize = (uintptr_t)-1;
    /* After local segment is attached, call optional client-provided hook
       (###) should call BEFORE any conduit-specific pinning/registration of the segment
     */
    if (gasnet_client_attach_hook) {
      gasnet_client_attach_hook(segbase, segsize);
    }
  }
  #elif GASNETC_PIN_SEGMENT
  {
    /* allocate the segment and exchange seginfo */
    gasneti_segmentAttach(segsize, minheapoffset, gasneti_seginfo, &gasneti_bootstrapExchange);
    segbase = gasneti_seginfo[gasneti_mynode].addr;
    segsize = gasneti_seginfo[gasneti_mynode].size;

    /* After local segment is attached, call optional client-provided hook
       (###) should call BEFORE any conduit-specific pinning/registration of the segment
     */
    if (gasnet_client_attach_hook) {
      gasnet_client_attach_hook(segbase, segsize);
    }

    gasnetc_seg_start = (uintptr_t)segbase;
    gasnetc_seg_len   = segsize;

    /* Find largest the segment requested */
    for (i=0; i<gasneti_nodes; ++i) {
      maxsize = MAX(maxsize, gasneti_seginfo[i].size);
    }

    /* Setup gasnetc_pin_maxsz{,_shift,_mask} and gasnetc_max_regs.
     * If possible, a single registration will be used and these variables will
     * enforce the max message size instead of max region length.
     * Otherwise they enforce gasnetc_pin_maxsz (rounded down to a power of two).
     */
    if (!gasnetc_pin_maxsz) {
      /* Default to largest registration supported by the HCA(a) */
      gasnetc_pin_maxsz = ~(uint64_t)0;
      GASNETC_FOR_ALL_HCA(hca) {
        gasnetc_pin_maxsz = MIN(gasnetc_pin_maxsz, hca->hca_cap.max_mr_size);
      }
    } else if (gasnetc_pin_maxsz > gasnetc_max_msg_sz) {
      char oldval[16], newval[16];
      gasneti_format_number(gasnetc_pin_maxsz, oldval, sizeof(oldval), 1);
      gasneti_format_number(gasnetc_max_msg_sz, newval, sizeof(newval), 1);
      fprintf(stderr,
              "WARNING: Requested GASNET_PIN_MAXSZ %s reduced to HCA's max_msg_sz of %s\n",
              oldval, newval);
      gasnetc_pin_maxsz = gasnetc_max_msg_sz;
    }
    if (gasnetc_pin_maxsz >= maxsize) {
      /* Single registration */
      gasnetc_pin_maxsz_shift = (8 * SIZEOF_VOID_P) - 1;
      gasnetc_pin_maxsz_mask = 0;
      gasnetc_pin_maxsz = gasnetc_max_msg_sz;
      gasnetc_max_regs = 1;
    } else {
      /* Multiple registration */
      size_t size = MIN(gasnetc_pin_maxsz, gasnetc_max_msg_sz);
      gasneti_assert(size != 0);
      size >>= 1;
      for (gasnetc_pin_maxsz_shift=0; size != 0; ++gasnetc_pin_maxsz_shift) { size >>= 1; }
      gasnetc_pin_maxsz = ((uint64_t)1) << gasnetc_pin_maxsz_shift;
      gasnetc_pin_maxsz_mask = (gasnetc_pin_maxsz - 1);
      gasnetc_max_regs = (maxsize + gasnetc_pin_maxsz - 1) >> gasnetc_pin_maxsz_shift;
    }
    { uint64_t value = gasnetc_pin_maxsz_mask ? gasnetc_pin_maxsz : maxsize;
      char valstr[16];
      gasneti_format_number(value, valstr, sizeof(valstr), 1);
      GASNETI_TRACE_PRINTF(I, ("Final/effective GASNET_PIN_MAXSZ = %s", valstr));
    }

    /* pin the segment and exchange the RKeys, once per HCA */
    { uint32_t	*my_rkeys;
      gasnetc_memreg_t  memreg;
      int		vstat;
      int		j;

      my_rkeys = gasneti_calloc(gasnetc_max_regs, sizeof(uint32_t));
      GASNETC_FOR_ALL_HCA(hca) {
        size_t		remain;
        uintptr_t		addr;
        hca->rkeys = gasneti_calloc(gasneti_nodes*gasnetc_max_regs, sizeof(uint32_t));
        gasneti_leak(hca->rkeys);
        hca->seg_lkeys = gasnett_malloc_aligned(GASNETI_CACHE_LINE_BYTES,
                                                gasnetc_max_regs * sizeof(uint32_t));
        gasneti_leak_aligned(hca->seg_lkeys);

        for (j = 0, addr = gasnetc_seg_start, remain = segsize; remain != 0; ++j) {
	  size_t len = (gasnetc_max_regs == 1) ? remain : MIN(remain, gasnetc_pin_maxsz);
          if (0 != gasnetc_pin(hca, (void *)addr, len,
			      (enum ibv_access_flags)(IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_REMOTE_READ),
			      &memreg)) {
             gasneti_fatalerror("Unexpected error %s (errno=%d) when registering the segment",
                                strerror(errno), errno);
          }
	  my_rkeys[j] = memreg.handle->rkey;
	  hca->seg_lkeys[j] = memreg.handle->lkey;
	  addr += len;
	  remain -= len;
          gasneti_assert(j <= gasnetc_max_regs);
        }
        GASNETI_TRACE_PRINTF(I, ("Attach registered %p bytes in %d regions",
                                 (void*)segsize, (int)gasnetc_max_regs));

        /* XXX: hca->rkeys is one of the O(N) storage requirements we might reduce/eliminate.
         * + When using PSHM we could store rkeys just once per supernode
         * + When not fully connected, we could utilize sparse storage
         */
        gasneti_bootstrapExchange(my_rkeys, gasnetc_max_regs*sizeof(uint32_t), hca->rkeys);
      }
      gasneti_free(my_rkeys);
    }
  }
  #else	/* just allocate the segment but don't pin it */
  {
    /* allocate the segment and exchange seginfo */
    gasneti_segmentAttach(segsize, minheapoffset, gasneti_seginfo, gasneti_bootstrapExchange);
    segbase = gasneti_seginfo[gasneti_mynode].addr;
    segsize = gasneti_seginfo[gasneti_mynode].size;
  }
  #endif

  /* Per-endpoint work */
  for (i = 0; i < gasneti_nodes; i++) {
    gasnetc_cep_t *cep = GASNETC_NODE2CEP(i);
    if (cep) {
      gasnetc_sndrcv_attach_peer(i, cep);
    }
  }

  /* Initialize firehose */
  if (GASNETC_USE_FIREHOSE && (gasneti_nodes > 1)) {
    uintptr_t firehose_mem = gasnetc_pin_info.memory;
    int firehose_reg = gasnetc_pin_info.regions;
    int reg_count, h;
    firehose_region_t prereg[1];
    size_t reg_size, maxsz;

    /* Setup prepinned regions list */
    prereg[0].addr             = gasnetc_hca[0].snd_reg.addr;
    prereg[0].len              = gasnetc_hca[0].snd_reg.len;
    GASNETC_FOR_ALL_HCA_INDEX(h) {
      prereg[0].client.handle[h] = NULL;	/* unreg must fail */
      prereg[0].client.lkey[h]   = gasnetc_hca[h].snd_reg.handle->lkey;
      prereg[0].client.rkey[h]   = gasnetc_hca[h].snd_reg.handle->rkey;
    }
    reg_size = prereg[0].len;
    reg_count = 1;

    /* Adjust for prepinned regions (they were pinned before init_pin_info probe) */
    firehose_mem += reg_size;

    /* Now initialize firehose */
    {
      uint32_t flags = 0;

      #if GASNETC_PIN_SEGMENT
        /* Adjust for the pinned segment (which is not advertised to firehose as prepinned) */
        gasneti_assert_always(firehose_mem > maxsize);
        firehose_mem -= maxsize;
        gasneti_assert_always(firehose_reg > gasnetc_max_regs);
        firehose_reg -= gasnetc_max_regs;

	flags |= FIREHOSE_INIT_FLAG_LOCAL_ONLY;
      #endif
      #if PLATFORM_OS_DARWIN
	flags |= FIREHOSE_INIT_FLAG_UNPIN_ON_FINI;
      #endif

      firehose_init(firehose_mem, firehose_reg, gasnetc_fh_maxsize,
		    prereg, reg_count, flags, &gasnetc_firehose_info);
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
  gasneti_bootstrapBarrier();

  GASNETI_TRACE_PRINTF(C,("gasnetc_attach(): primary attach complete"));

  gasneti_assert(gasneti_seginfo[gasneti_mynode].addr == segbase &&
         gasneti_seginfo[gasneti_mynode].size == segsize);

  gasneti_auxseg_attach(); /* provide auxseg */

  gasnete_init(); /* init the extended API */

  gasneti_nodemapFini();

#if GASNETC_USE_RCV_THREAD
  /* Start AM receive thread, if applicable */
  gasnetc_sndrcv_start_thread();
#endif

  /* ensure extended API is initialized across nodes */
  gasneti_bootstrapBarrier();
  gasnetc_sys_coll_fini();

  return GASNET_OK;
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
    gasnetc_handler[i] = (gasneti_handler_fn_t)&gasnetc_noop;
  }
}

#if GASNET_PSHM
static gasnet_node_t *gasnetc_exit_child = NULL;
static gasnet_node_t gasnetc_exit_children = 0;
static gasnet_node_t gasnetc_exit_parent = 0;
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
static void gasnetc_exit_reduce_reqh(gasnet_token_t token,
                                     gasnet_handlerarg_t arg0,
                                     gasnet_handlerarg_t arg1) {
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
      old_val = gasneti_atomic32_read(&gasnetc_exit_dist, 0);
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
static void gasnetc_exit_role_reqh(gasnet_token_t token) {
  gasnet_node_t src;
  int local_role, result;

  gasneti_assert(gasneti_mynode == GASNETC_ROOT_NODE);	/* May only send this request to the root node */

  
  /* What role would the local node get if the requester is made the master? */
  GASNETI_SAFE(gasnet_AMGetMsgSource(token, &src));
  local_role = (src == GASNETC_ROOT_NODE) ? GASNETC_EXIT_ROLE_MASTER : GASNETC_EXIT_ROLE_SLAVE;

  /* Try atomically to assume the proper role.  Result determines role of requester */
  result = gasneti_atomic_compare_and_swap(&gasnetc_exit_role, GASNETC_EXIT_ROLE_UNKNOWN, local_role, 0)
                ? GASNETC_EXIT_ROLE_MASTER : GASNETC_EXIT_ROLE_SLAVE;

  /* Inform the requester of the outcome. */
  GASNETI_SAFE(gasnetc_ReplySysShort(token, NULL, gasneti_handleridx(gasnetc_exit_role_reph),
				   1, (gasnet_handlerarg_t)result));
}

/*
 * gasnetc_exit_role_reph()
 *
 * This reply handler receives the result of the election of an exit "master".
 * The reply contains the exit "role" this node should assume.
 */
static void gasnetc_exit_role_reph(gasnet_token_t token, gasnet_handlerarg_t arg0) {
  int role;

  #if GASNET_DEBUG
  {
    gasnet_node_t src;
    GASNETI_SAFE(gasnet_AMGetMsgSource(token, &src));
    gasneti_assert(src == GASNETC_ROOT_NODE);	/* May only receive this reply from the root node */
  }
  #endif

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

  #if GASNET_DEBUG
  /* note - can't call trace macros here, or even sprintf */
  if (sig == SIGALRM) {
    static const char msg[] = "gasnet_exit(): WARNING: timeout during exit... goodbye.  [";
    const char * state = gasnetc_exit_state;
    write(STDERR_FILENO, msg, sizeof(msg) - 1);
    write(STDERR_FILENO, state, strlen(state));
    write(STDERR_FILENO, "]\n", 2);
  } else {
    static const char msg1[] = "gasnet_exit(): ERROR: signal ";
    static const char msg2[] = " received during exit... goodbye.  [";
    const char * state = gasnetc_exit_state;
    char digit;

    write(STDERR_FILENO, msg1, sizeof(msg1) - 1);

    /* assume sig < 100 */
    if (sig > 9) {
      digit = '0' + ((sig / 10) % 10);
      write(STDERR_FILENO, &digit, 1);
    }
    digit = '0' + (sig % 10);
    write(STDERR_FILENO, &digit, 1);
    
    write(STDERR_FILENO, msg2, sizeof(msg2) - 1);
    write(STDERR_FILENO, state, strlen(state));
    write(STDERR_FILENO, "]\n", 2);
  }
  #endif

  if (gasneti_atomic_decrement_and_test(&once, 0)) {
    /* We ask the bootstrap support to kill us, but only once */
    gasneti_reghandler(SIGALRM, gasnetc_exit_sighandler);
    GASNETC_EXIT_STATE("in suicide timer");
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
			       1, (gasnet_handlerarg_t)exitcode);
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
  gasnetc_counter_wait(&gasnetc_exit_repl_oust, 1);

  return 0;
}

/* gasnetc_exit_body
 *
 * This code is common to all the exit paths and is used to perform a hopefully graceful exit in
 * all cases.  To coordinate a graceful shutdown gasnetc_get_exit_role() will select one node as
 * the "master".  That master node will then send a remote exit request to each of its peers to
 * ensure they know that it is time to exit.  If we fail to coordinate the shutdown, we ask the
 * bootstrap to shut us down agressively.  Otherwise we return to our caller.  Unless our caller
 * is the at-exit handler, we are typically followed by a call to gasnetc_exit_tail() to perform
 * the actual termination.  Note also that this function will block all calling threads other than
 * the first until the shutdown code has been completed.
 *
 * XXX: timeouts contained here are entirely arbitrary
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
  gasnetc_sndrcv_stop_thread();
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

  /* Timed MIN(exitcode) reduction to clearly distinguish collective exit */
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
    GASNETI_TRACE_PRINTF(I, ("  allocated: %12llu bytes in %8llu objects",
                             (long long unsigned)stats.live_bytes,
                             (long long unsigned)stats.live_objects));
    GASNETI_TRACE_PRINTF(I, ("     pinned: %12llu bytes in %8llu objects",
                             (long long unsigned)gasnetc_pinned_bytes,
                             (long long unsigned)gasnetc_pinned_blocks));
    GASNETI_TRACE_PRINTF(I, ("      total: %12llu bytes in %8llu objects",
                             (long long unsigned)(stats.live_bytes + gasnetc_pinned_bytes),
                             (long long unsigned)(stats.live_objects + gasnetc_pinned_blocks)));
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
  gasnetc_connect_fini();
  if (gasnetc_did_firehose_init && !gasnetc_exit_in_signal) {
    /* Note we skip firehose_fini() on exit via a signal */
    GASNETC_EXIT_STATE("in firehose_fini()");
    firehose_fini();
  }
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
static void gasnetc_exit_reqh(gasnet_token_t token, gasnet_handlerarg_t arg0) {
  /* The master will send this AM, but should _never_ receive it */
  gasneti_assert(gasneti_atomic_read(&gasnetc_exit_role, 0) != GASNETC_EXIT_ROLE_MASTER);

  /* We should never receive this AM multiple times */
  gasneti_assert(gasneti_atomic_read(&gasnetc_exit_reqs, 0) == 0);

  /* If we didn't already know, we are now certain our role is "slave" */
  (void)gasneti_atomic_compare_and_swap(&gasnetc_exit_role, GASNETC_EXIT_ROLE_UNKNOWN, GASNETC_EXIT_ROLE_SLAVE, 0);

  /* Send a reply so the master knows we are reachable */
  gasnetc_counter_inc(&gasnetc_exit_repl_oust);
  GASNETI_SAFE(gasnetc_ReplySysShort(token, &gasnetc_exit_repl_oust.completed,
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
static void gasnetc_exit_reph(gasnet_token_t token) {
  gasneti_atomic_increment(&gasnetc_exit_reps, 0);
}
  
/* gasnetc_atexit OR gasnetc_on_exit
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
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int exitcode, void *arg) {
  /* Check return from _head to avoid reentrance */
  if (gasnetc_exit_head(exitcode)) {
    gasnetc_exit_body();
  }
  return;
}
#else
static void gasnetc_atexit(void) {
  /* Check return from _head to avoid reentrance */
  if (gasnetc_exit_head(0)) { /* real exit code is outside our control */
    gasnetc_exit_body();
  }
  return;
}
#endif

static void gasnetc_exit_init(void) {
  /* Handler for non-collective returns from main() */
  #if HAVE_ON_EXIT
    on_exit(gasnetc_on_exit, NULL);
  #else
    atexit(gasnetc_atexit);
  #endif

#if GASNET_PSHM
  /* Extract info from nodemap that we'll need at exit */
  if (gasneti_nodemap_local_rank) {
    gasnetc_exit_parent = gasneti_nodemap[gasneti_mynode];
    gasnetc_exit_children = gasneti_nodes;
  } else {
    const gasnet_node_t children = gasneti_nodemap_local_count - 1;

    if (children) {
      const size_t len = children * sizeof(gasnet_node_t);
      gasnetc_exit_children = children;
      gasnetc_exit_child = gasneti_malloc(len);
      gasneti_leak(gasnetc_exit_child);
      memcpy(gasnetc_exit_child, gasneti_nodemap_local+1, len);
    }
  }
#endif

  /* No warm-up needed because bootstrapBarrier uses same connections */
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

GASNETI_INLINE(gasnetc_amrdma_grant_reqh_inner)
void gasnetc_amrdma_grant_reqh_inner(gasnet_token_t token, int qpi, uint32_t rkey, void *addr) {
  gasnetc_cep_t *cep;
  gasnet_node_t node;

  GASNETI_SAFE(gasnet_AMGetMsgSource(token, &node));

  cep = GASNETC_NODE2CEP(node) + (qpi - 1);
  gasneti_assert(cep->amrdma_send == NULL);
  cep->amrdma_send = gasnetc_amrdma_send_alloc(rkey, addr);

  GASNETI_TRACE_PRINTF(C,("AMRDMA_GRANT_RCV from node=%d qp=%d\n", (int)node, qpi-1));
}
SHORT_HANDLER(gasnetc_amrdma_grant_reqh,3,4,
              (token, a0, a1, UNPACK(a2)    ),
              (token, a0, a1, UNPACK2(a2, a3)));

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/

extern int gasnetc_AMRequestShortM( 
                            gasnet_node_t dest,       /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTSHORT(dest,handler,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
  retval = gasnetc_RequestGeneric(gasnetc_Short, dest, handler,
		  		  NULL, 0, NULL,
				  numargs, NULL, NULL, argptr);
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestMediumM( 
                            gasnet_node_t dest,      /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTMEDIUM(dest,handler,source_addr,nbytes,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
  retval = gasnetc_RequestGeneric(gasnetc_Medium, dest, handler,
		  		  source_addr, nbytes, NULL,
				  numargs, NULL, NULL, argptr);
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestLongM( gasnet_node_t dest,        /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  gasnetc_counter_t mem_oust = GASNETC_COUNTER_INITIALIZER;
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTLONG(dest,handler,source_addr,nbytes,dest_addr,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
    retval = gasnetc_RequestGeneric(gasnetc_Long, dest, handler,
		  		  source_addr, nbytes, dest_addr,
				  numargs, &mem_oust, NULL, argptr);

    /* block for completion of RDMA transfer */
    gasnetc_counter_wait(&mem_oust, 0);
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestLongAsyncM( gasnet_node_t dest,        /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTLONGASYNC(dest,handler,source_addr,nbytes,dest_addr,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
  retval = gasnetc_RequestGeneric(gasnetc_Long, dest, handler,
		  		  source_addr, nbytes, dest_addr,
				  numargs, NULL, NULL, argptr);
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyShortM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREPLYSHORT(token,handler,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
  retval = gasnetc_ReplyGeneric(gasnetc_Short, token, handler,
		  		NULL, 0, NULL,
				numargs, NULL, NULL, argptr);
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyMediumM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
  retval = gasnetc_ReplyGeneric(gasnetc_Medium, token, handler,
		  		source_addr, nbytes, NULL,
				numargs, NULL, NULL, argptr);
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyLongM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,numargs); 
  va_start(argptr, numargs); /*  pass in last argument */
  #if GASNETC_PIN_SEGMENT
  {
    gasnetc_counter_t mem_oust = GASNETC_COUNTER_INITIALIZER;

    retval = gasnetc_ReplyGeneric(gasnetc_Long, token, handler,
		  		  source_addr, nbytes, dest_addr,
				  numargs, &mem_oust, NULL, argptr);

    /* block for completion of RDMA transfer */
    gasnetc_counter_wait(&mem_oust, 1 /* calling from a request handler */);
  }
  #else
  retval = gasnetc_ReplyGeneric(gasnetc_Long, token, handler,
		  		source_addr, nbytes, dest_addr,
				numargs, NULL, NULL, argptr);

  #endif
  va_end(argptr);
  GASNETI_RETURN(retval);
}

/* ------------------------------------------------------------------------------------ */
/*
  No-interrupt sections
  =====================
  This section is only required for conduits that may use interrupt-based handler dispatch
  See the GASNet spec and http://www.cs.berkeley.edu/~bonachea/upc/gasnet.html for
    philosophy and hints on efficiently implementing no-interrupt sections
  Note: the extended-ref implementation provides a thread-specific void* within the 
    gasnete_threaddata_t data structure which is reserved for use by the core 
    (and this is one place you'll probably want to use it)
*/
#if GASNETC_USE_INTERRUPTS
  #error interrupts not implemented
  extern void gasnetc_hold_interrupts(void) {
    GASNETI_CHECKATTACH();
    /* add code here to disable handler interrupts for _this_ thread */
  }
  extern void gasnetc_resume_interrupts(void) {
    GASNETI_CHECKATTACH();
    /* add code here to re-enable handler interrupts for _this_ thread */
  }
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Handler-safe locks
  ==================
*/
#if !GASNETC_NULL_HSL
extern void gasnetc_hsl_init   (gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();
  gasneti_mutex_init(&(hsl->lock));

  #if GASNETC_USE_INTERRUPTS
    /* add code here to init conduit-specific HSL state */
    #error interrupts not implemented
  #endif
}

extern void gasnetc_hsl_destroy(gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();
  gasneti_mutex_destroy(&(hsl->lock));

  #if GASNETC_USE_INTERRUPTS
    /* add code here to cleanup conduit-specific HSL state */
    #error interrupts not implemented
#endif
}

extern void gasnetc_hsl_lock   (gasnet_hsl_t *hsl) {
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

  #if GASNETC_USE_INTERRUPTS
    /* conduits with interrupt-based handler dispatch need to add code here to 
       disable handler interrupts on _this_ thread, (if this is the outermost
       HSL lock acquire and we're not inside an enclosing no-interrupt section)
     */
    #error interrupts not implemented
  #endif
}

extern void gasnetc_hsl_unlock (gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();

  #if GASNETC_USE_INTERRUPTS
    /* conduits with interrupt-based handler dispatch need to add code here to 
       re-enable handler interrupts on _this_ thread, (if this is the outermost
       HSL lock release and we're not inside an enclosing no-interrupt section)
     */
    #error interrupts not implemented
  #endif

  GASNETI_TRACE_EVENT_TIME(L, HSL_UNLOCK, GASNETI_TICKS_NOW_IFENABLED(L)-hsl->acquiretime);

  gasneti_mutex_unlock(&(hsl->lock));
}

extern int  gasnetc_hsl_trylock(gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();

  {
    int locked = (gasneti_mutex_trylock(&(hsl->lock)) == 0);

    GASNETI_TRACE_EVENT_VAL(L, HSL_TRYLOCK, locked);
    if (locked) {
      #if GASNETI_STATS_OR_TRACE
        hsl->acquiretime = GASNETI_TICKS_NOW_IFENABLED(L);
      #endif
      #if GASNETC_USE_INTERRUPTS
        /* conduits with interrupt-based handler dispatch need to add code here to 
           disable handler interrupts on _this_ thread, (if this is the outermost
           HSL lock acquire and we're not inside an enclosing no-interrupt section)
         */
        #error interrupts not implemented
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
static gasnet_handlerentry_t const gasnetc_handlers[] = {
  #ifdef GASNETC_AUXSEG_HANDLERS
    GASNETC_AUXSEG_HANDLERS(),
  #endif

  /* ptr-width independent handlers */
  gasneti_handler_tableentry_no_bits(gasnetc_exit_reduce_reqh),
  gasneti_handler_tableentry_no_bits(gasnetc_exit_role_reqh),
  gasneti_handler_tableentry_no_bits(gasnetc_exit_role_reph),
  gasneti_handler_tableentry_no_bits(gasnetc_exit_reqh),
  gasneti_handler_tableentry_no_bits(gasnetc_exit_reph),
  gasneti_handler_tableentry_no_bits(gasnetc_sys_barrier_reqh),
  gasneti_handler_tableentry_no_bits(gasnetc_sys_exchange_reqh),

  /* ptr-width dependent handlers */
  gasneti_handler_tableentry_with_bits(gasnetc_amrdma_grant_reqh),

  { 0, NULL }
};

gasnet_handlerentry_t const *gasnetc_get_handlertable(void) {
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
      stackaddr = gasneti_mmap(stacksize + 2 * GASNET_PAGESIZE);
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
