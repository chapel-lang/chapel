/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_bootstrap.c $
 * Description: Conduit-independent AM-based bootstrap collectives
 * Copyright 2022, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <coll/gasnet_coll_internal.h>
#include <gasnet_core_internal.h> // for handler indices

/* ------------------------------------------------------------------------------------ */
// Return a vector of peers for dissemination barrier or Bruck's GatherAll
// returns length, writes pointer to *out_p
// when length is 0, *out_p will be NULL
//
// Note that the result returned in *out_p remains owned by this function.
// Callers must not free or write to it.
//
// Note that the only thread-safety provisions are guards against concurrent
// handler execution if GASNETI_CONDUIT_THREADS is asserted.  This is under
// the assumption that the only callers are serial initialization code and
// AM handlers such as those in this file.
gex_Rank_t gasneti_get_dissem_peers(const gex_Rank_t **out_p)
{
  static gex_Rank_t result_len = 0;
  static gex_Rank_t *result_vec = NULL;
  static int is_init = 0;

#if GASNETI_CONDUIT_THREADS
  static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;
  gasneti_mutex_lock(&lock);
#endif

  if (!is_init) {
    is_init = 1;

    gex_Rank_t size = gasneti_nodes;
    gex_Rank_t rank = gasneti_mynode;

    if (size > 1) {
      gasneti_assert_uint(result_len ,==, 0);
      for (gex_Rank_t i = 1; i < size; i *= 2) result_len += 1;

      result_vec = gasneti_malloc(result_len * sizeof(gex_Rank_t));
      gasneti_leak(result_vec);
      for (gex_Rank_t i = 0; i < result_len; ++i) {
        gex_Rank_t distance = 1 << i;
        result_vec[i] = (distance <= rank) ? (rank - distance) : (rank + (size - distance));
      }
    }
  }

#if GASNETI_CONDUIT_THREADS
  gasneti_mutex_unlock(&lock);
#endif

  *out_p = result_vec;
  return result_len;
}

#if GASNET_PSHM
// As above but consisting of just one "leader" per nbrhd
gex_Rank_t gasneti_get_dissem_peers_pshm(const gex_Rank_t **out_p)
{
  static gex_Rank_t result_len = 0;
  static gex_Rank_t *result_vec = NULL;
  static int is_init = 0;

#if GASNETI_CONDUIT_THREADS
  static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;
  gasneti_mutex_lock(&lock);
#endif

  if (!is_init) {
    is_init = 1;

    gex_Rank_t size = gasneti_nodemap_global_count;
    gex_Rank_t rank = gasneti_nodemap_global_rank;

    if (size > 1) {
      gasneti_assert_uint(result_len ,==, 0);
      for (gex_Rank_t i = 1; i < size; i *= 2) result_len += 1;

      result_vec = gasneti_malloc(result_len * sizeof(gex_Rank_t));
      gasneti_leak(result_vec);
      for (gex_Rank_t i = 0; i < result_len; ++i) {
        gex_Rank_t distance = 1 << i;
        gex_Rank_t peer = (distance <= rank) ? (rank - distance) : (rank + (size - distance));
        result_vec[i] = gasneti_pshm_firsts[peer];
      }
    }
  }

#if GASNETI_CONDUIT_THREADS
  gasneti_mutex_unlock(&lock);
#endif

  *out_p = result_vec;
  return result_len;
}
#endif

/* ------------------------------------------------------------------------------------ */
// Host-scoped (potentially superset of supernode) barrier
//
// Note a lack explicit thread-safety provisions, under the assumption that the
// only callers are serial initialization.  The use of atomics in the handler
// are sufficient to guard against concurrent hander execution by a conduit thread.

static gasneti_weakatomic32_t gasneti_hbarr_rcvd[2][32]; // Implicitly zero-initialized

extern void gasnetc_hbarr_reqh(gex_Token_t token, gex_AM_Arg_t arg0)
{
  const int phase = arg0 & 1;
  const int step = (arg0 >> 1) & 0x1f; // Max 2^5 steps => 2^32 proc/host!
  const int distance = (1 << step);
  gasneti_assert_uint(distance ,<, gasneti_myhost.node_count);
  gasneti_weakatomic32_increment(&gasneti_hbarr_rcvd[phase][step], GASNETI_ATOMIC_REL);
}

void gasneti_host_barrier(void)
{
  // Simple dissemination barrier with two phase
  static int phase = 0;
  const gex_Rank_t rank = gasneti_myhost.node_rank;
  const gex_Rank_t size = gasneti_myhost.node_count;
  for (unsigned int step = 0, distance = 1; distance < size; ++step, distance *= 2) {
    gex_Rank_t peer = (distance <= rank) ? rank - distance : rank + (size - distance);
    gex_AM_Arg_t arg0 = phase | (step << 1);

    gex_AM_RequestShort(gasneti_THUNK_TM, gasneti_myhost.nodes[peer],
                        gasneti_handleridx(gasnetc_hbarr_reqh), 0, arg0);

    // Poll until we have received the same phase we've just sent
    GASNET_BLOCKUNTIL((int)gasneti_weakatomic32_read(&gasneti_hbarr_rcvd[phase][step], 0));
    gasneti_assert_int((int)gasneti_weakatomic32_read(&gasneti_hbarr_rcvd[phase][step], 0) ,==, 1);
    gasneti_weakatomic32_set(&gasneti_hbarr_rcvd[phase][step], 0, 0);
  }

#if GASNET_PSHM
  // Cannot use AMPSHM and pshm bootstrap collectives in same pshmnet barrier phase
  gasneti_pshmnet_bootstrapBarrierPoll();
#endif

  phase ^= 1;
}

/* ------------------------------------------------------------------------------------ */
// Host-scoped (potentially superset of supernode) reduce-to-all(SUM, uint64_t)
//
// Note a lack explicit thread-safety provisions, under the assumption that the
// only callers are serial initialization.  The use of atomics in the handler
// are sufficient to guard against concurrent hander execution by a conduit thread.

// Following are both implicitly zero-initialized:
static gasneti_weakatomic32_t gasneti_hsumu64_rcvd;
static gasneti_weakatomic64_t gasneti_hsumu64_sum;

extern void gasnetc_hsumu64_reqh(gex_Token_t token, gex_AM_Arg_t arg0, gex_AM_Arg_t arg1)
{
  uint64_t operand = GASNETI_MAKEWORD(arg0, arg1);
  gasneti_weakatomic64_add(&gasneti_hsumu64_sum, operand, 0);
  gasneti_weakatomic32_increment(&gasneti_hsumu64_rcvd, GASNETI_ATOMIC_REL);
}

uint64_t gasneti_host_sumu64(uint64_t operand)
{
  // Sum on the way up a binimial tree, and then broadcast down the same tree
  const gex_Rank_t rank = gasneti_myhost.node_rank;
  const gex_Rank_t size = gasneti_myhost.node_count;
  gasneti_assert_uint(size ,<, 0x80000000); // otherwise some of the math below goes wrong
  const gex_Rank_t remain = size - rank;
  const gex_Rank_t fullsize = (rank & (-rank));
  const gex_Rank_t subsize = (!fullsize || (fullsize > remain)) ? remain : fullsize;
  const gex_Rank_t children = 1 + gasnete_coll_log2_rank(subsize - 1);
  const gex_Rank_t parent = rank - fullsize;

  gasneti_weakatomic32_t *rcvd_counter = &gasneti_hsumu64_rcvd;
  gasneti_weakatomic64_t *sum = &gasneti_hsumu64_sum;
  uint64_t result = operand;

  if (children) {
    // 1. wait for contributions from children
    GASNET_BLOCKUNTIL((gex_Rank_t)gasneti_weakatomic32_read(rcvd_counter,0) == children);
    result += gasneti_weakatomic64_read(sum,0);
    gasneti_weakatomic64_set(sum,0,0);
  }

  if (rank) {
    // 2. send partial result to parent
    gex_AM_RequestShort(gasneti_THUNK_TM, gasneti_myhost.nodes[parent],
                        gasneti_handleridx(gasnetc_hsumu64_reqh), 0,
                        GASNETI_HIWORD(result), GASNETI_LOWORD(result));
    // 3. wait for final result from parent
    GASNET_BLOCKUNTIL((gex_Rank_t)gasneti_weakatomic32_read(rcvd_counter,0) == children + 1);
    result = gasneti_weakatomic64_read(sum,0);
    gasneti_weakatomic64_set(sum,0,0);
  }

  // 4. reset state for next time
  gasneti_weakatomic32_set(rcvd_counter,0,0);

  if (children) {
    // 5. forward result to children
    uint32_t arg0 = GASNETI_HIWORD(result);
    uint32_t arg1 = GASNETI_LOWORD(result);
    for (int idx = children - 1; idx >= 0; --idx) { // Reverse order for deepest subtree first
      gex_Rank_t peer = gasneti_myhost.nodes[rank + (1 << idx)];
      gex_AM_RequestShort(gasneti_THUNK_TM, peer,
                          gasneti_handleridx(gasnetc_hsumu64_reqh), 0,
                          arg0, arg1);
    }
  }

  return result;
}

/* ------------------------------------------------------------------------------------ */
// Bootstrap collectives using AM Requests
// These require Short and/or Medium Requests, plus GASNET_BLOCKUNTIL.
// No use is currently made of Reply or Long.
//
// Currently Barrier and Exchange are provided.
// TODO: more operations as needed
//
// Note that the only thread-safety provisions are those needed to allow for
// concurrent handler execution, and only if GASNETI_CONDUIT_THREADS is
// asserted.  This is under the assumption that the only callers are serial
// initialization code.

//
// BARRIER
//

static int gasneti_am_barrier_phase = 0;

static uint32_t gasneti_am_barrier_rcvd[2] = {0, 0};
#if GASNETI_CONDUIT_THREADS
  // MUTEX
  static gasneti_mutex_t gasneti_am_barrier_lock = GASNETI_MUTEX_INITIALIZER;
  static void gasneti_am_barrier_arrival(int phase, uint32_t bit) {
    gasneti_mutex_lock(&gasneti_am_barrier_lock);
    gasneti_am_barrier_rcvd[phase] |= bit;
    gasneti_mutex_unlock(&gasneti_am_barrier_lock);
  }
  static uint32_t gasneti_am_barrier_read(int phase) {
    gasneti_mutex_lock(&gasneti_am_barrier_lock);
    uint32_t result = gasneti_am_barrier_rcvd[phase];
    gasneti_mutex_unlock(&gasneti_am_barrier_lock);
    return result;
  }
  static void gasneti_am_barrier_reset(int phase) {
    gasneti_mutex_lock(&gasneti_am_barrier_lock);
    gasneti_am_barrier_rcvd[phase] = 0;
    gasneti_mutex_unlock(&gasneti_am_barrier_lock);
  }
#else
  // SERIAL
  #define gasneti_am_barrier_arrival(phase,bit) \
    ((void)(gasneti_am_barrier_rcvd[phase] |= (bit)))
  #define gasneti_am_barrier_read(phase) \
    (gasneti_am_barrier_rcvd[phase])
  #define gasneti_am_barrier_reset(phase) \
    ((void)(gasneti_am_barrier_rcvd[phase] = 0))
#endif

extern void gasnetc_am_barrier_reqh(gex_Token_t token, gex_AM_Arg_t arg)
{
  uint32_t phase = arg & 1;
  uint32_t bit = arg ^ phase;
  gasneti_assert(GASNETI_POWEROFTWO(bit));
  gasneti_am_barrier_arrival(phase,bit);
}

extern void gasneti_bootstrapBarrier_am(void)
{
    const gex_Rank_t *peer;
#if GASNET_PSHM
    gasneti_pshmnet_bootstrapBarrier();
    gex_Rank_t size = gasneti_nodemap_local_rank
                    ? 0 // not leader -> no network comms
                    : gasneti_get_dissem_peers_pshm(&peer);
#else
    gex_Rank_t size = gasneti_get_dissem_peers(&peer);
#endif

    int phase = gasneti_am_barrier_phase;
    for (gex_Rank_t i = 0; i < size; ++i) { // empty for PSHM/non-leader
      const uint32_t bit = 2 << i; // (distance << 1), since phase is low bit

      (void) gex_AM_RequestShort1(gasneti_THUNK_TM, peer[i],
                                  gasneti_handleridx(gasnetc_am_barrier_reqh),
                                  0, phase | bit);

      // wait for completion of the proper receive, which might arrive out of order
      GASNET_BLOCKUNTIL(gasneti_am_barrier_read(phase) & bit);
    }

#if GASNET_PSHM
    gasneti_pshmnet_bootstrapBarrier();
#endif

    // reset for next barrier
    gasneti_am_barrier_reset(phase);
    gasneti_am_barrier_phase = !phase;
}

//
// EXCHANGE
//

static int gasneti_am_exchange_phase = 0;

static gex_Rank_t *gasneti_am_exchange_rcvd = NULL;
static gex_Rank_t *gasneti_am_exchange_send = NULL;
#if GASNET_PSHM
static gex_Rank_t *gasneti_am_exchange_permute = NULL;
#endif
static uint8_t *gasneti_am_exchange_buf[2] = { NULL, NULL };

#if GASNETI_CONDUIT_THREADS
  static gasneti_atomic_t gasneti_am_exchange_cntr[2][32] =
  { { gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0) },
    { gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0),
      gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0), gasneti_atomic_init(0) } };
  #define gasneti_am_exchange_inc(_phase, _step) \
    gasneti_atomic_increment(&gasneti_am_exchange_cntr[_phase][_step], GASNETI_ATOMIC_REL)
  #define gasneti_am_exchange_read(_phase, _step) \
    gasneti_atomic_read(&gasneti_am_exchange_cntr[_phase][_step], GASNETI_ATOMIC_NONE)
  #define gasneti_am_exchange_reset(_phase, _step) \
    gasneti_atomic_set(&gasneti_am_exchange_cntr[_phase][_step], 0, GASNETI_ATOMIC_ACQ)
#else
  static gasneti_atomic_val_t gasneti_am_exchange_cntr[2][32] =
  { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
  #define gasneti_am_exchange_inc(_phase, _step) \
    ((void)(++gasneti_am_exchange_cntr[_phase][_step]))
  #define gasneti_am_exchange_read(_phase, _step) \
    (gasneti_am_exchange_cntr[_phase][_step])
  #define gasneti_am_exchange_reset(_phase, _step) \
    ((void)(gasneti_am_exchange_cntr[_phase][_step] = 0))
#endif

GASNETI_NEVER_INLINE(gasneti_am_exchange_init,
static void gasneti_am_exchange_init(void))
{
#if GASNETI_CONDUIT_THREADS
  static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;
  gasneti_mutex_lock(&lock);
#endif

  static int is_init = 0;
  if (is_init) goto done;
  is_init = 1;

#if GASNET_PSHM
  const gex_Rank_t size = gasneti_nodemap_global_count;
  const gex_Rank_t rank = gasneti_nodemap_global_rank;

  if (gasneti_nodemap_local_rank || size == 1) goto done; // No network comms
#else
  const gex_Rank_t size = gasneti_nodes;
  const gex_Rank_t rank = gasneti_mynode;

  if (size == 1) goto done; // No network comms
#endif

  // Fetch vector of the dissemination peers (only need length)
  const gex_Rank_t *peers;
  gex_Rank_t num_peers;
  #if GASNET_PSHM
    num_peers = gasneti_get_dissem_peers_pshm(&peers);
  #else
    num_peers = gasneti_get_dissem_peers(&peers);
  #endif

  // Without the following hint, some gcc versions warn about massive malloc sizes below
  gasneti_assume(num_peers <= 8*sizeof(gex_Rank_t));

  // Compute the recv offset and send count for each step of exchange
  gasneti_am_exchange_rcvd = gasneti_malloc((num_peers+1) * sizeof(gex_Rank_t));
  gasneti_am_exchange_send = gasneti_malloc(num_peers * sizeof(gex_Rank_t));
  { gex_Rank_t i, step;
  #if GASNET_PSHM
    gex_Rank_t *width;
    gex_Rank_t sum1 = 0;
    gex_Rank_t sum2 = 0;
    gex_Rank_t distance, last;

    distance = 1 << (num_peers-1);
    last = (distance <= rank) ? (rank - distance) : (rank + (size - distance));

    // Step 1: determine the "width" of each nbrhd
    width = gasneti_calloc(size, sizeof(gex_Rank_t));
    for (i = 0; i < gasneti_nodes; ++i) {
      width[gasneti_nodeinfo[i].supernode] += 1;
    }
    // Step 2: form the necessary partial sums
    for (step = i = 0; step < num_peers; ++step) {
      distance = 1 << step;
      for (/*empty*/; i < distance; ++i) {
        sum1 += width[ (rank + i) % size ];
        sum2 += width[ (last + i) % size ];
      }
      gasneti_am_exchange_rcvd[step] = gasneti_am_exchange_send[step] = sum1;
    }
    gasneti_am_exchange_send[step-1] = gasneti_nodes - sum2;
    gasneti_am_exchange_rcvd[step] = gasneti_nodes;
    // Step 3: construct the permutation vector, if necessary
    {
      // Step 3a. determine if we even need a permutation vector
      int sorted = 1;
      gasneti_assert(0 == gasneti_nodeinfo[0].supernode);
      gex_Rank_t n = 0;
      for (i = 1; i < gasneti_nodes; ++i) {
        if (n > gasneti_nodeinfo[i].supernode) {
          sorted = 0;
          break;
        }
        n = gasneti_nodeinfo[i].supernode;
      }

      // Step 3b. contstruct the vector if needed
      if (!sorted) {
        gex_Rank_t *offset = gasneti_malloc(size * sizeof(gex_Rank_t));

        // Form a sort of shifted prefix-reduction on width
        sum1 = 0;
        n = rank;
        for (i = 0; i < size; ++i) {
          offset[n] = sum1;
          sum1 += width[n];
          n = (n == size-1) ? 0 : (n+1);
        }
        gasneti_assert(sum1 == gasneti_nodes);

        // Scan nodeinfo to collect all the nodes in each nbrhd (in their order)
        gasneti_am_exchange_permute = gasneti_malloc(gasneti_nodes * sizeof(gex_Rank_t));
        for (i = 0; i < gasneti_nodes; ++i) {
          int index = offset[ gasneti_nodeinfo[i].supernode ]++;
          gasneti_am_exchange_permute[index] = i;
        }

        gasneti_free(offset);
      }
    }
    gasneti_free(width);
  #else
    for (step = 0; step < num_peers; ++step) {
      gasneti_am_exchange_rcvd[step] = gasneti_am_exchange_send[step] = 1 << step;
    }
    gasneti_am_exchange_send[step-1] = gasneti_nodes - gasneti_am_exchange_send[step-1];
    gasneti_am_exchange_rcvd[step] = gasneti_nodes;
  #endif
  }

done:
#if GASNETI_CONDUIT_THREADS
  gasneti_mutex_unlock(&lock);
#else
  return; // avoids warnings about label not followed by a statement
#endif
}

static uint8_t *gasneti_am_exchange_addr(int phase, size_t elemsz)
{
#if GASNETI_CONDUIT_THREADS
  static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;
  gasneti_mutex_lock(&lock);
#endif

  if (gasneti_am_exchange_buf[phase] == NULL) {
    gasneti_am_exchange_buf[phase] = gasneti_malloc(elemsz * gasneti_nodes);
  }

#if GASNETI_CONDUIT_THREADS
  gasneti_mutex_unlock(&lock);
#endif

  return gasneti_am_exchange_buf[phase];
}

#ifndef GASNETI_AM_EXCHANGE_CHUNK
#define GASNETI_AM_EXCHANGE_CHUNK \
        gex_AM_MaxRequestMedium(gasneti_THUNK_TM,GEX_RANK_INVALID,GEX_EVENT_NOW,0,2)
#endif

extern void gasnetc_am_exchange_reqh(gex_Token_t token, void *buf,
                                     size_t nbytes, uint32_t arg0,
                                     uint32_t elemsz)
{
  gasneti_am_exchange_init();

  const int phase = arg0 & 1;
  const int step = (arg0 >> 1) & 0x0f;
  const int seq = (arg0 >> 5);
  const size_t offset = elemsz * gasneti_am_exchange_rcvd[step];
  uint8_t *dest = gasneti_am_exchange_addr(phase, elemsz)
                  + offset + (seq * GASNETI_AM_EXCHANGE_CHUNK);

  GASNETI_MEMCPY(dest, buf, nbytes);
  gasneti_am_exchange_inc(phase, step);
}

extern void gasneti_bootstrapExchange_am(void *src, size_t len, void *dest)
{
  gasneti_am_exchange_init();

  int phase = gasneti_am_exchange_phase;
  uint8_t *temp = gasneti_am_exchange_addr(phase, len);

  const gex_Rank_t *peer;
#if GASNET_PSHM
  // Construct nbrhd-local contribution
  gasneti_pshmnet_bootstrapGather(gasneti_request_pshmnet, src, len, temp, 0);

  if (gasneti_nodemap_local_rank) goto end_network_comms; // nbrhd non-leader case

  gex_Rank_t size = gasneti_get_dissem_peers_pshm(&peer);
#else
  // Copy in local contribution
  GASNETI_MEMCPY(temp, src, len);

  gex_Rank_t size = gasneti_get_dissem_peers(&peer);
#endif

  // Bruck's concatenation algorithm:
  gasneti_assert_uint(size ,<=, 32);
  size_t chunk_size = GASNETI_AM_EXCHANGE_CHUNK;
  for (gex_Rank_t step = 0; step < size; ++step) {
    size_t nbytes = len * gasneti_am_exchange_send[step];
    size_t offset = 0;
    uint32_t seq = 0;

    // Send payload using AMMedium(s)
    do {
      const size_t to_xfer = MIN(nbytes, chunk_size);

      (void) gex_AM_RequestMedium2(gasneti_THUNK_TM, peer[step],
                                   gasneti_handleridx(gasnetc_am_exchange_reqh),
                                   temp + offset, to_xfer,
                                   GEX_EVENT_NOW, 0,
                                   phase | (step << 1) | (seq << 5), len);

      ++seq;
      offset += to_xfer;
      nbytes -= to_xfer;
      gasneti_assert(seq < (1<<(32-5)));
    } while (nbytes);

    // poll until correct number of messages have been received
    nbytes = len * (gasneti_am_exchange_rcvd[step+1] - gasneti_am_exchange_rcvd[step]);
    seq = (nbytes + chunk_size - 1) / chunk_size;
    GASNET_BLOCKUNTIL(gasneti_am_exchange_read(phase, step) == seq);
    gasneti_am_exchange_reset(phase, step); // Includes the RMB, if any, for the data
  }

  // Copy to destination while performing the rotation or permutation
#if GASNET_PSHM
  if (gasneti_am_exchange_permute) {
    for (gex_Rank_t n = 0; n < gasneti_nodes; ++n) {
      const gex_Rank_t peer = gasneti_am_exchange_permute[n];
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

  // Prepare for next */
  gasneti_free(temp);
  gasneti_am_exchange_buf[phase] = NULL;
  gasneti_sync_writes();
  gasneti_am_exchange_phase = !phase;

#if GASNET_DEBUG
  // verify own data as a sanity check */
  if (memcmp(src, (void *) ((uintptr_t ) dest + (gasneti_mynode * len)), len) != 0) {
    gasneti_fatalerror("exchange failed: self data on node %d is incorrect", gasneti_mynode);
  }
#endif
}

/* ------------------------------------------------------------------------------------ */
