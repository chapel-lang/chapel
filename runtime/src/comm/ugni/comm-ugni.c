/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// GNI-based implementation of Chapel communication interface.
//

#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <limits.h>
#include <pmi.h>
#include <pthread.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>

#include <gni_pub.h>    // <stddef.h> and <stdint.h> must come first
#include <hugetlbfs.h>  // <sys/types.h> must come first

#if defined(GNI_VERSION_FMA_CHAIN_TRANSACTIONS) \
      && GNI_VERSION >= GNI_VERSION_FMA_CHAIN_TRANSACTIONS
#define HAVE_GNI_FMA_CHAIN_TRANSACTIONS 1
#else
#define HAVE_GNI_FMA_CHAIN_TRANSACTIONS 0
#endif

#include "chplcgfns.h"
#include "chpl-gen-includes.h"
#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-comm-diags.h"
#include "chpl-comm-callbacks.h"
#include "chpl-comm-callbacks-internal.h"
#include "chpl-comm-internal.h"
#include "chpl-comm-strd-xfer.h"
#include "chpl-env.h"
#include "chplexit.h"
#include "chpl-format.h"
#include "chpl-mem.h"
#include "chpl-mem-desc.h"
#include "chpl-mem-sys.h"
#include "chplsys.h"
#include "chpl-tasks.h"
#include "chpl-atomics.h"
#include "chplcast.h"
#include "chpl-linefile-support.h"
#include "comm-ugni-heap-pages.h"
#include "comm-ugni-mem.h"
#include "config.h"
#include "error.h"

// Don't get warning macros for chpl_comm_get etc
#include "chpl-comm-no-warning-macros.h"


#ifdef CHPL_COMM_DEBUG
#include <sys/types.h>
#include <fcntl.h>

#include <inttypes.h>
#include <sys/stat.h>

#include "chpl-env.h"

#define DEBUG_STATS 1

static uint64_t debug_flag = 0;

#define DBGF_IFACE         0x1          // interface calls
#define DBGF_GETPUT        0x2          // GET and PUT
#define DBGF_AMO           0x4          // AMOs
#define DBGF_RF            0x8          // remote forks
#define DBGF_MEMREG       0x10          // memory registration
#define DBGF_MEMREG_BCAST 0x20          // memory registration
#define DBGF_HUGEPAGES    0x40          // hugepage usage
#define DBGF_MEMMAPS     0x100          // memory maps
#define DBGF_BARRIER    0x1000          // barriers
#define DBGF_IN_FILE   0x10000          // output debug info to a file
#define DBGF_1_NODE    0x20000          // only produce debug for one node 
#define DBGF_NODENAME  0x40000          // produce chpl_nodeID <-> nodename map

static c_nodeid_t debug_nodeID = 0;

static FILE* debug_file;

static chpl_bool debug_exiting = false;

#define _DBG_DO(flg)  (((flg) & debug_flag) != 0                        \
                       && ((DBGF_1_NODE & debug_flag) == 0              \
                           || chpl_nodeID == debug_nodeID))
#define _DBG_THIS_NODE()  ((DBGF_1_NODE & debug_flag) == 0              \
                           || chpl_nodeID == debug_nodeID)

#define DBG_INIT()  dbg_init()

static pthread_t proc_thread_id;

static __thread uint32_t thread_idx      = ~(uint32_t) 0;
static atomic_uint_least32_t next_thread_idx;
#define _DBG_NEXT_THREAD_IDX() \
        atomic_fetch_add_uint_least32_t(&next_thread_idx, 1)

#define _DBG_P(dbg_do, f, ...)                                          \
        do {                                                            \
          if (dbg_do) {                                                 \
            if (thread_idx == ~(uint32_t) 0) {                          \
              thread_idx = _DBG_NEXT_THREAD_IDX();                      \
              fprintf(debug_file,                                       \
                      "DBG: %s:%d (%d): thread_id %" PRIu32 "\n",       \
                      __FILE__, __LINE__, chpl_nodeID, thread_idx);     \
            }                                                           \
            fprintf(debug_file, "DBG: %s:%d" f "\n",                    \
                    __FILE__, __LINE__, ## __VA_ARGS__);                \
          }                                                             \
        } while (0)

#define DBG_P(flg, f, ...)                                              \
        _DBG_P(_DBG_DO(flg), ": " f, ## __VA_ARGS__)
#define DBG_P_L(flg, f, ...)                                            \
        _DBG_P(_DBG_DO(flg), " (%d): " f, chpl_nodeID, ## __VA_ARGS__)
#define DBG_P_LP(flg, f, ...)                                           \
        _DBG_P(_DBG_DO(flg),                                            \
               " (%d/%s/%d): " f,                                       \
               chpl_nodeID, task_id(cd != NULL && cd->firmly_bound),    \
               (int) thread_idx, ## __VA_ARGS__)
#define DBG_P_LPS(flg, f, li, cdi, rbi, seq, ...)                       \
        _DBG_P(_DBG_DO(flg),                                            \
               " (%d/%s/%d) %d/%d/%d <%" PRIu64 ">: " f,                \
               chpl_nodeID, task_id(cd != NULL && cd->firmly_bound),    \
               (int) thread_idx,                                        \
               (int) li, cdi, rbi, seq, ## __VA_ARGS__)

#define CHPL_INTERNAL_ERROR(msg)                                        \
        do {                                                            \
          DBG_P_LP(1, "%s:%d: internal error: %s",                      \
                   __FILE__, (int) __LINE__, msg);                      \
          fflush(debug_file);                                           \
          abort();                                                      \
        } while (0)

static const char* task_id(int);
static const char* task_id(int firmly_bound)
{
  if (pthread_self() == proc_thread_id)
    return "PROC";
  if (debug_exiting)
    return "EXIT";
  if (firmly_bound)
    return "POLL";
  static __thread char buf[20];
  snprintf(buf, sizeof(buf), "%" PRId64, (int64_t) chpl_task_getId());
  return buf;
}
#else
#undef DEBUG_STATS

#define DBG_INIT()

#define DBG_P(flg, f, ...)
#define DBG_P_L(flg, f, ...)
#define DBG_P_LP(flg, f, ...)
#define DBG_P_LPS(flg, f, b, ...)

#define CHPL_INTERNAL_ERROR(msg)                                        \
        do {                                                            \
          static __thread char _cieBuf[1000];                           \
          (void) snprintf(_cieBuf, sizeof(_cieBuf),                     \
                          "%d:%s:%d: %s",                               \
                          (int) chpl_nodeID, __FILE__, (int) __LINE__,  \
                          msg);                                         \
          chpl_internal_error(_cieBuf);                                 \
        } while (0)
#endif

#ifdef DEBUG_STATS
static uint64_t debug_stats_flag = 0;

#define DBGSF_ANY 1

#define _DBGSTAT_DO(flg)      (((flg) & debug_stats_flag) != 0)

#define DBGSTAT_P_L(flg, f, ...)                                        \
        _DBG_P(_DBGSTAT_DO(flg),                                        \
               " (%d): " f, chpl_nodeID, ## __VA_ARGS__)
#else
#define DBGSTAT_P_L(flg, f, ...)
#endif


// not generally true, but should be for XE/XC
#define CACHE_LINE_SIZE 64
#define CACHE_LINE_ALIGN __attribute__((aligned(CACHE_LINE_SIZE)))


////////////////////////////////////////
//
// Statistics gathering
//
//#define PERFSTATS_COMM_UGNI 1

#ifdef PERFSTATS_COMM_UGNI

#define PERFSTATS_VARS_EPHEMERAL(MACRO)                                 \
        MACRO(put_cnt)                                                  \
        MACRO(put_byte_cnt)                                             \
        MACRO(put_strd_cnt)                                             \
        MACRO(get_cnt)                                                  \
        MACRO(get_byte_cnt)                                             \
        MACRO(get_strd_cnt)                                             \
        MACRO(get_nb_cnt)                                               \
        MACRO(get_nb_b_cnt)                                             \
        MACRO(test_nb_cnt)                                              \
        MACRO(wait_nb_cnt)                                              \
        MACRO(try_nb_cnt)                                               \
        MACRO(amo_cnt)                                                  \
        MACRO(fork_call_cnt)                                            \
        MACRO(fork_call_nb_cnt)                                         \
        MACRO(fork_call_fast_cnt)                                       \
        MACRO(fork_call_small_cnt)                                      \
        MACRO(fork_call_large_cnt)                                      \
        MACRO(fork_put_cnt)                                             \
        MACRO(fork_get_cnt)                                             \
        MACRO(fork_free_cnt)                                            \
        MACRO(fork_amo_cnt)                                             \
        MACRO(regMemAlloc_cnt)                                          \
        MACRO(regMemPostAlloc_cnt)                                      \
        MACRO(regMemFree_cnt)                                           \
        MACRO(regMem_bCast_cnt)                                         \
        MACRO(regMem_locks)                                             \
        MACRO(regMem_lock_nsecs)                                        \
        MACRO(regMem_critsec_nsecs)                                     \
        MACRO(regMem_alloc_nsecs)                                       \
        MACRO(regMem_kpage_nsecs)                                       \
        MACRO(regMem_defaultInit_nsecs)                                 \
        MACRO(regMem_reg_nsecs)                                         \
        MACRO(regMem_dereg_nsecs)                                       \
        MACRO(regMem_free_nsecs)                                        \
        MACRO(local_mreg_cnt)                                           \
        MACRO(local_mreg_cmps)                                          \
        MACRO(local_mreg_nsecs)                                         \
        MACRO(remote_mreg_cnt)                                          \
        MACRO(remote_mreg_cmps)                                         \
        MACRO(remote_mreg_nsecs)                                        \
        MACRO(sent_bytes)                                               \
        MACRO(rcvd_bytes)                                               \
        MACRO(acq_cd_cnt)                                               \
        MACRO(acq_cd_na_cnt)                                            \
        MACRO(acq_cd_cq_cnt)                                            \
        MACRO(acq_cd_rb_cnt)                                            \
        MACRO(acq_cd_rb_na_cnt)                                         \
        MACRO(acq_cd_rb_cq_cnt)                                         \
        MACRO(acq_cd_rb_frf_cnt)                                        \
        MACRO(reacq_cd_cnt)                                             \
        MACRO(wait_rfork_cnt)                                           \
        MACRO(wait_glb_cq_ev_cnt)                                       \
        MACRO(lyield_cnt)                                               \
        MACRO(tskyield_in_lyield_no_cd_cnt)                             \
        MACRO(tskyield_in_lyield_with_cd_cnt)                           \
        MACRO(lyield_in_bar_1_cnt)                                      \
        MACRO(lyield_in_bar_2_cnt)                                      \
        MACRO(lyield_in_acq_cd_cnt)                                     \
        MACRO(lyield_in_acq_cd_rb_cnt)                                  \
        MACRO(lyield_in_wait_rfork_cnt)                                 \
        MACRO(lyield_in_wait_glb_cq_ev_cnt)                             \
        MACRO(timer_nsecs)

#define PERFSTATS_VARS_ALL(MACRO)                                       \
        PERFSTATS_VARS_EPHEMERAL(MACRO)

//
// Define this to measure the kernel's page creation+zero time for
// regMemAlloc memory and also the Chapel default initialization
// time for such memory.  There are caveats:
// - Measuring the page creation+zero time will set the memory's NUMA
//   locality (to the thread's default) and thus nullify any later code
//   (such as default initialization) which tries to do the same.
// - The default initialization measurement assumes that defaultInit
//   occurs between the regMemAlloc() return and the regMemPostAlloc()
//   call, i.e., we continue with the current alloc-defaultInit-register
//   sequence.
// 
//#define PERFSTATS_TIME_ZERO_INIT 1

#define _PSV_C_TYPE      int_least64_t
#define _PSV_FMT         PRId64

#define _PSV_SYM(x)      _PSV_SYM2(x, _PSV_C_TYPE)
#define _PSV_SYM2(x, t)  _PSV_SYM3(x, t)
#define _PSV_SYM3(x, t)  x##_##t

#define _PSV_ATOMIC_TYPE _PSV_SYM(atomic)
#define _PSV_LD_FUNC     _PSV_SYM(atomic_load)
#define _PSV_ST_FUNC     _PSV_SYM(atomic_store)
#define _PSV_ADD_FUNC    _PSV_SYM(atomic_fetch_add)
#define _PSV_ADD_FUNC_E  _PSV_SYM(atomic_fetch_add_explicit)

#define _PSV_DECL(psv)  _PSV_ATOMIC_TYPE CACHE_LINE_ALIGN psv;
typedef struct chpl_comm_pstats {
  PERFSTATS_VARS_ALL(_PSV_DECL)
} chpl_comm_pstats_t;
#undef _PSV_DECL

chpl_comm_pstats_t chpl_comm_pstats;

#define PERFSTATS_DO_EPHEMERAL(MACRO) \
        do { PERFSTATS_VARS_EPHEMERAL(MACRO) } while (0)
#define PERFSTATS_DO_ALL(MACRO)       \
        do { PERFSTATS_VARS_ALL(MACRO) } while (0)

#define _PSV_VAR(cnt)  chpl_comm_pstats.cnt

#define PERFSTATS_LD(cnt)         _PSV_LD_FUNC(&_PSV_VAR(cnt))
#define PERFSTATS_ST(cnt, val)    _PSV_ST_FUNC(&_PSV_VAR(cnt), val)
#define PERFSTATS_STZ(cnt)        PERFSTATS_ST(cnt, 0)
#define PERFSTATS_ADD(cnt, val)   (void) _PSV_ADD_FUNC_E(&_PSV_VAR(cnt), val, memory_order_relaxed)
#define PERFSTATS_INC(cnt)        PERFSTATS_ADD(cnt, 1)

#include <time.h>

// useful to get time elapsed since comm layer init
static struct timespec perfstats_timeBase;

static _PSV_C_TYPE timer_overhead;

static inline _PSV_C_TYPE perfstats_timer_get(void)
{
  struct timespec _ts;
  (void) clock_gettime(CLOCK_MONOTONIC, &_ts);
  return (_PSV_C_TYPE) (_ts.tv_sec - perfstats_timeBase.tv_sec) * 1000000000L
         + (_PSV_C_TYPE) _ts.tv_nsec;
}

#define PERFSTATS_TGET(ts)      ts = perfstats_timer_get()
#define PERFSTATS_TSTAMP(ts)    _PSV_C_TYPE ts = perfstats_timer_get()
#define PERFSTATS_TDIFF(te, ts) ((te) - (ts) - timer_overhead)
#define PERFSTATS_TELAPSED(ts)  PERFSTATS_TDIFF(perfstats_timer_get(), ts)

static void perfstats_init(void)
{
  (void) clock_gettime(CLOCK_MONOTONIC, &perfstats_timeBase);

  //
  // Measure the timer overhead.  We only use timer_nsecs so that we
  // get a realistic measurement including the atomic summation into
  // the perfstats.  Afterward we zero it so it doesn't get printed,
  // and timer_overhead is used to adjust for the measurement cost.
  //
  {
    // ~420 ns per trial on XC for syscall, ~15 ns for vDSO. If timers are
    // used heavily, it benefits to use dynamic linking so we can use vDSO
    const int nTrials = 10000;

    for (int i = 0; i < nTrials; i++) {
      PERFSTATS_TSTAMP(ts);
      PERFSTATS_ADD(timer_nsecs, PERFSTATS_TELAPSED(ts));
    }

    timer_overhead = (PERFSTATS_LD(timer_nsecs) + nTrials) / nTrials;
    PERFSTATS_STZ(timer_nsecs);
  }
}

#define PERFSTATS_INIT() perfstats_init();

static inline void perfstats_add_post(gni_post_descriptor_t* post_desc) {
  switch (post_desc->type) {
    case GNI_POST_FMA_PUT:
    case GNI_POST_RDMA_PUT:
      PERFSTATS_ADD(sent_bytes, post_desc->length);
      break;
    case GNI_POST_FMA_GET:
    case GNI_POST_RDMA_GET:
      PERFSTATS_ADD(rcvd_bytes, post_desc->length);
      break;
    default:
      // no-op
      break;
  }
}

#define PERFSTATS_ADD_POST(post_desc) perfstats_add_post(post_desc);

#else
#define PERFSTATS_ADD(cnt, val)
#define PERFSTATS_INC(cnt)
#define PERFSTATS_DO_EPHEMERAL(MACRO)
#define PERFSTATS_DO_ALL(MACRO)
#define PERFSTATS_TGET(ts)
#define PERFSTATS_TSTAMP(ts)
#define PERFSTATS_INIT()
#define PERFSTATS_ADD_POST(post_desc)
#endif


//
// Alignment.
//
#define ALIGN_DN(i, size)  ((i) & ~((size) - 1))
#define ALIGN_UP(i, size)  ALIGN_DN((i) + (size) - 1, size)


//
// Error checking.
//

#define GNI_FAIL(rc, what)                                              \
        do {                                                            \
          char _gfBuf[200];                                             \
          snprintf(_gfBuf, sizeof(_gfBuf), "%s failed, error code %d",  \
                   what, (int) rc);                                     \
          CHPL_INTERNAL_ERROR(_gfBuf);                                  \
        } while (0)

#define GNI_CHECK(fnCall)                                               \
        do {                                                            \
          gni_return_t rc;                                              \
          if ((rc = (fnCall)) != GNI_RC_SUCCESS)                        \
            GNI_FAIL(rc, #fnCall);                                      \
        } while (0)

#define GNI_CQ_EV_FAIL(rc, ev, what)                                    \
        do {                                                            \
          char _cqeBuf[100];                                            \
          char _gcefBuf[200];                                           \
          (void) GNI_CqErrorStr(ev, _cqeBuf, sizeof(_cqeBuf));          \
          snprintf(_gcefBuf, sizeof(_gcefBuf),                          \
                   "%s failed, error code %d, str \"%s\"",              \
                   what, (int) rc, _cqeBuf);                            \
          CHPL_INTERNAL_ERROR(_gcefBuf);                                \
        } while (0)


//
// Declarations having to do with the NIC.
//
static uint32_t* nic_addr_map = NULL;

static gni_nic_device_t nic_type;


//
// The uGNI remote instance IDs we use to tell receiving polling tasks
// where inbound requests have landed are built from locale (li), comm
// domain (cdi), and request buffer (rbi) indices.  These have to fit
// in 32 bits.
//
#define _IID_LI_BITS        18
#define _IID_LI_MASK        ((1 << _IID_LI_BITS) - 1)
#define _IID_LI_POS         0
#define _IID_ENCODE_LI(li)  (((li) & _IID_LI_MASK) << _IID_LI_POS)
#define _IID_DECODE_LI(iid) (((iid) >> _IID_LI_POS) & _IID_LI_MASK)

#define _IID_CDI_BITS        7
#define _IID_CDI_MASK        ((1 << _IID_CDI_BITS) - 1)
#define _IID_CDI_POS         (_IID_LI_POS + _IID_LI_BITS)
#define _IID_ENCODE_CDI(cdi) (((cdi) & _IID_CDI_MASK) << _IID_CDI_POS)
#define _IID_DECODE_CDI(iid) (((iid) >> _IID_CDI_POS) & _IID_CDI_MASK)

#define _IID_RBI_BITS        1
#define _IID_RBI_MASK        ((1 << _IID_RBI_BITS) - 1)
#define _IID_RBI_POS         (_IID_CDI_POS + _IID_CDI_BITS)
#define _IID_ENCODE_RBI(rbi) (((rbi) & _IID_RBI_MASK) << _IID_RBI_POS)
#define _IID_DECODE_RBI(iid) (((iid) >> _IID_RBI_POS) & _IID_RBI_MASK)

#if _IID_LI_BITS + _IID_CDI_BITS + _IID_RBI_BITS > 32
#error INST_ID fields are too big!
#endif

#define GNI_ENCODE_REM_INST_ID(li, cdi, rbi)                            \
        (  _IID_ENCODE_LI(li)                                           \
         | _IID_ENCODE_CDI(cdi)                                         \
         | _IID_ENCODE_RBI(rbi))

#define GNI_DECODE_REM_INST_ID(li, cdi, rbi, iid)                       \
        ((li = _IID_DECODE_LI(iid)),                                    \
         (cdi = _IID_DECODE_CDI(iid)),                                  \
         (rbi = _IID_DECODE_RBI(iid)))

//
// Declarations having to do with memory and memory registration.
//
// We register all of the read/write memory regions in /proc/self/maps
// that have pathnames.  We can handle up to max_mem_regions of these.
// There are four regions we definitely want to register.  These are the
// static data, the part of the heap right after the static data, the
// main heap (probably on hugepages), and the stack.  We don't actually
// have to register all regions.  We really only need a minimum set of
// internal data to be registered.  The logic can handle transfers to
// and from other areas by bouncing them through buffers in registered
// memory.
//
static int registered_heap_info_set;
static pthread_once_t registered_heap_once_flag = PTHREAD_ONCE_INIT;
static size_t registered_heap_size;
static void*  registered_heap_start;

static int hugepage_info_set;
static pthread_once_t hugepage_once_flag = PTHREAD_ONCE_INIT;
static size_t hugepage_size;

//
// Memory region support.
//
// mem_regions contains the address/length pairs and uGNI memory domain
// handles for all of the registered memory regions on this node.  It is
// strictly local to the node.
// mem_regions_all is a table containing a copy of mem_regions from
// every node.  It is read-only to the local node but must be writable
// by remote nodes.  As other nodes add and remove registered memory
// regions they are responsible for updating their own mem_regions_all
// elements on all other nodes.  Thus every node is kept up-to-date on
// every other node's registrations.
// mem_regions_all_entries is a local convenience array containing the
// address of each node's element in mem_regions_all (since these
// elements are not of fixed size).
// mem_regions_all_my_entry_map contains the address of this node's
// mem_regions_all entry on all other nodes.  When a node is changing
// registrations it uses this to update its own mem_regions_all entries
// on all the other nodes.
//
typedef struct {
  uint64_t         addr;
  uint64_t         len;  // includes reg. status; see mrtl_encode() etc. below
  gni_mem_handle_t mdh;
} mem_region_t;

typedef struct {
  uint32_t     mreg_cnt;  // really hi idx + 1 (mregs[] may have holes)
  mem_region_t mregs[];
} mem_region_table_t;

static int max_mem_regions;
static size_t mem_regions_size;

static mem_region_table_t* mem_regions;
static atomic_int_least32_t mreg_free_cnt;
static uint32_t mreg_cnt_max;

static mem_region_table_t* mem_regions_all;
static mem_region_table_t** mem_regions_all_entries;

static mem_region_table_t** mem_regions_all_my_entry_map;

static chpl_bool can_register_memory = false;

//
// The high bit of the 'len' member of a mem_region_t in the table
// indicates whether the region is registered.  mrtl_encode() and
// its siblings deal with such encoded 'len' member values.
//
#define _MRTL_LENBITS  (sizeof(uint64_t) * CHAR_BIT - 1)
#define _MRTL_LENMASK  ((1UL << _MRTL_LENBITS) - 1UL)

static inline
uint64_t mrtl_encode(uint64_t len, chpl_bool reg) {
  return ((reg ? 1UL : 0UL) << _MRTL_LENBITS) | (len & _MRTL_LENMASK);
}

static inline
void mrtl_setReg(uint64_t *p_len) {
  *p_len |= 1UL << _MRTL_LENBITS;
}

static inline
uint64_t mrtl_len(uint64_t len) {
  return len & _MRTL_LENMASK;
}

static inline
chpl_bool mrtl_isReg(uint64_t len) {
  return ((len >> _MRTL_LENBITS) == 0UL) ? false : true;
}

//
// Used to serialize access to critical sections in the dynamic registration
// allocation/registration/free routines. In addition to using a pthread_mutex,
// we also have to prevent multiple tasks from running on the same pthread.
// This allows us to get away with a simple pthread_mutex (vs chapel sync var,
// or mutex+try-lock) and more importantly ensures that an entire allocation
// can be completed before yielding, since some tasking layers don't support
// yielding for some allocations. For example, qthreads doesn't support
// yielding while grabbing memory to initialize task stacks.
//
static pthread_mutex_t mem_regions_mutex = PTHREAD_MUTEX_INITIALIZER;
static __thread chpl_bool allow_task_yield = true;

//
// This is the memory region for the guaranteed NIC-registered memory
// in which the memory region map, remote fork descriptors and their
// is-free flags, and temporary bounce buffers live.  It saves time
// not to have to look it up.
//
static mem_region_t* gnr_mreg;
static mem_region_t* gnr_mreg_map;

//
// These data objects support catching SIGBUS signals due to hugepage
// fault-in failures, and reporting out-of-memory in response.
//
static struct sigaction previous_SIGBUS_sigact;
static atomic_bool SIGBUS_gate;

struct mregs_supp {
  chpl_mem_descInt_t desc;
  int ln;
  int32_t fn;
};

struct mregs_supp* mr_mregs_supplement;  // parallels mem_regions->mregs[]

static chpl_bool exit_without_cleanup = false;


//
// Declarations common to memory pools.
//
#define _SYM_REALLY_CAT(pfx, sfx)  pfx##sfx
#define _SYM_CAT(pfx, sfx)         _SYM_REALLY_CAT(pfx, sfx)
#define _MPOOL_IDX_SYM(pfx)        _SYM_CAT(pfx, _MPOOL_IDX_BASE_T)

#define _MPOOL_IDX_BASE_T int_least16_t
#define _MPOOL_IDX_T      _MPOOL_IDX_SYM(atomic_)

typedef _MPOOL_IDX_BASE_T mpool_idx_base_t;
typedef _MPOOL_IDX_T      mpool_idx_t;

static inline
void mpool_idx_init(mpool_idx_t* pvar, mpool_idx_base_t val) {
  _MPOOL_IDX_SYM(atomic_init_)(pvar, val);
}

static inline
mpool_idx_base_t mpool_idx_load(mpool_idx_t* pvar) {
  return _MPOOL_IDX_SYM(atomic_load_)(pvar);
}

static inline
void mpool_idx_store(mpool_idx_t* pvar, mpool_idx_base_t val) {
  _MPOOL_IDX_SYM(atomic_store_)(pvar, val);
}

static inline
mpool_idx_base_t mpool_idx_exchange(mpool_idx_t* pvar, mpool_idx_base_t val) {
  return _MPOOL_IDX_SYM(atomic_exchange_)(pvar, val);
}

static inline
mpool_idx_base_t mpool_idx_finc(mpool_idx_t* pvar) {
  return _MPOOL_IDX_SYM(atomic_fetch_add_)(pvar, 1);
}



//
// Declarations having to do with communication domains.
//

//
// This structure contains all the state pertaining to our use of one
// uGNI communication domain.
//
// busy:          Is this communication domain in use?
// firmly_bound:  Set to "true" if the communication domain is
//                permanently owned by a particular task/thread, as
//                for example the polling thread.
// nih:           NIC handle.
// remote_eps:    Remote endpoints.
// cqh:           Completion queue handle.
// cq_cnt_max:    Completion queue size.
// cq_cnt_curr:   Number of pending completions outstanding.
//

#define CD_ACTIVE_TRANS_MAX 128     // Max transactions in flight, per cd

typedef uint32_t cq_cnt_t;
typedef atomic_uint_least32_t cq_cnt_atomic_t;

#define CQ_CNT_INIT(cd, val) \
        atomic_init_uint_least32_t(&(cd)->cq_cnt_curr, val)
#define CQ_CNT_LOAD(cd)       \
        atomic_load_uint_least32_t(&(cd)->cq_cnt_curr)
#define CQ_CNT_INC(cd)        \
        (void) atomic_fetch_add_uint_least32_t(&(cd)->cq_cnt_curr, 1)
#define CQ_CNT_DEC(cd)        \
        (void) atomic_fetch_sub_uint_least32_t(&(cd)->cq_cnt_curr, 1)

// We create an array of comm domains and try to spread our threads to
// different indices. For tasking layers with a fixed number of threads
// this results in each thread acquiring an affinity for a comm domain,
// eliminating contention across comm domains (assuming we can create as
// many comm domains as there are threads.) We cacheline align/pad the
// comm_dom_t struct to avoid false sharing between threads accessing
// different comm domains (this is extremely important for performance)
//
// Because there are configurations where there aren't a fixed number of
// threads and machines where there would be more threads than comm
// domains we put `busy` on its own cacheline to avoid contention
// between a thread operating on an acquired CD and another thread
// that's trying to acquire it. We also try to limit cache thrashing on
// `busy` by acquiring the lock with a test-and-test-and-set op, but the
// effectiveness of that depends on which atomic layer is used (cstdlib
// is the only one with cheap atomic reads.)

typedef struct {
  atomic_bool        busy CACHE_LINE_ALIGN;
  cq_cnt_atomic_t    cq_cnt_curr CACHE_LINE_ALIGN;
  chpl_bool          firmly_bound;
  gni_nic_handle_t   nih;
  gni_ep_handle_t*   remote_eps;
  gni_cq_handle_t    cqh;
  cq_cnt_t           cq_cnt_max;
#ifdef DEBUG_STATS
  uint64_t           acqs;
  uint64_t           acqs_looks;
  uint64_t           acqs_with_rb;
  uint64_t           acqs_with_rb_looks;
  uint64_t           reacqs;
#endif
} CACHE_LINE_ALIGN comm_dom_t;


//
// Communication domain descriptors.
//
static uint32_t     comm_dom_cnt;
static uint32_t     comm_dom_cnt_max;

static comm_dom_t*  comm_doms;

static atomic_int_least32_t global_init_cdi;
static __thread int comm_dom_free_idx = -1;
static __thread comm_dom_t* cd = NULL;
static __thread int cd_idx = -1;

#define INIT_CD_BUSY(cd)      atomic_init_bool(&(cd)->busy, false)
#define CHECK_CD_BUSY(cd)     atomic_load_explicit_bool(&(cd)->busy, memory_order_acquire)
#define ACQUIRE_CD_MAYBE(cd)  (!atomic_exchange_explicit_bool(&(cd)->busy, true, memory_order_acquire))
#define RELEASE_CD(cd)        atomic_store_explicit_bool(&(cd)->busy, false, memory_order_release)


//
// Declarations having to do with individual remote references.
//

// FMA supports up to 1GB transfers (this is the total size that can
// be transferred for chained transactions)
#define MAX_FMA_TRANS_SZ ((size_t) 1 << 30)
// BTE RDMA supports up to ~4GB transfers (2^31 - 1), but the size of
// gets must be 4 byte aligned. This just follows gasnet-aries and
// uses the nearest 4MB aligned value
#define MAX_RDMA_TRANS_SZ ((size_t) 0xFFC00000)

// BTE RDMA is profitable around 1-4K. It's neck-and-neck under 4K, so
// for now follow gasnet-aries and use the higher default of 4K
#define DEFAULT_RDMA_THRESHOLD 4096
static size_t rdma_threshold = DEFAULT_RDMA_THRESHOLD;

// Largest size to use unordered transactions for
#define MAX_UNORDERED_TRANS_SZ 1024

#define ALIGN_32_DN(x)    ALIGN_DN((x), sizeof(int32_t))
#define ALIGN_32_UP(x)    ALIGN_UP((x), sizeof(int32_t))
#define IS_ALIGNED_32(x)  ((x) == ALIGN_32_DN(x))

#define ALIGN_64_DN(x)    ALIGN_DN((x), sizeof(int64_t))
#define ALIGN_64_UP(x)    ALIGN_UP((x), sizeof(int64_t))
#define IS_ALIGNED_64(x)  ((x) == ALIGN_64_DN(x))

#define VP_TO_UI64(x)     ((uint64_t) (intptr_t) (x))
#define UI64_TO_VP(x)     ((void*) (intptr_t) (x))

//
// Maximum number of PUTs/AMOs in a chained transaction list.  This
// number was determined empirically (on XC/Aries).  Doing more than
// this at once didn't seem to improve performance.
//
#define MAX_CHAINED_PUT_LEN 64
#define MAX_CHAINED_GET_LEN 64
#define MAX_CHAINED_AMO_LEN 64


//
// Declarations having to do with non-blocking PUTs and GETs.
//
// For most non-blocking transactions, we need descriptors that allow
// us to determine when they have completed.  (For certain internal
// non-blocking PUTs we won't need this, but for all non-blocking user
// transactions we do.)  When a transaction completes, what we get
// from GNI_GetCompleted() is a pointer to the gni_post_descriptor_t
// for that transaction.  So in order to match postings with their
// later completions, we need the post descriptor to exist from the
// time the transaction is posted until when the initiating task sees
// it complete.

typedef struct {
  gni_post_descriptor_t post_desc;  // POST descriptor for an NB transaction
  atomic_bool done;                 // transaction has completed?
  int cdi;                          // index of comm domain used for post
  mpool_idx_base_t next;            // free list index
} nb_desc_t;


//
// Declarations having to do with remote fork.  None of this has to
// be set up until the polling task is running and can use it.
//

static gni_cq_handle_t rf_cqh;          // completion queue handle

static gni_mem_handle_t  rf_mdh;        // remote fork req space GNI mem handle
static gni_mem_handle_t* rf_mdh_map;    // all locales' remote fork space mdhs

//
// Blocking remote forks need a "remote fork done" (rf_done) flag, for
// the remote side to set when it completes.  Such flags have to be in
// memory registered with the NIC so that the remote side can PUT to
// them directly.  When the initiating side's stack is in registered
// memory we can use an rf_done flag local to the fork function.  But
// if not, we allocate an rf_done flag out of these pools, which we
// have arranged to be always in registered memory.
//
// To ensure that these are in registered memory we allocate them up
// front, with a fixed size, through the registered-memory manager.
// There are RF_DONE_NUM_POOLS pools, with RF_DONE_NUM_PER_POOL in
// each one.  The requirements/constraints on these values are as
// follows.
//   - RF_DONE_NUM_POOLS limits the allocate/free concurrency, so it
//     should be at least as large as the number of hardware threads
//     in use.
//   - We emulate an unsigned mod by RF_DONE_NUM_POOLS by means of
//     & (RF_DONE_NUM_POOLS - 1), so that has to be a power of 2.
//   - The code does divides and mods with RF_DONE_NUM_PER_POOL, so
//     for performance that should be a power of 2.
//   - It's possible (though highly unlikely) that every task in the
//     program could need one of these at the same time, and we can't
//     get more dynamically, so we potentially need a lot of them: the
//     product of RF_DONE_NUM_POOLS and RF_DONE_NUM_PER_POOL should be
//     "large".  Fortunately they're small, so over-provisioning isn't
//     particularly costly.
//
#define RF_DONE_NUM_POOLS 128
#define RF_DONE_NUM_PER_POOL 128

typedef mpool_idx_base_t rf_done_t;
typedef mpool_idx_base_t rf_done_pool_t;

static rf_done_pool_t (*rf_done_pool)[RF_DONE_NUM_PER_POOL];
static mpool_idx_t rf_done_pool_head[RF_DONE_NUM_POOLS];
static atomic_bool rf_done_pool_lock[RF_DONE_NUM_POOLS];

static mpool_idx_t rf_done_pool_i;

static inline
mpool_idx_base_t rf_done_next_pool_i(void) 
{
  return mpool_idx_finc(&rf_done_pool_i) & (RF_DONE_NUM_POOLS - 1);
}

//
// Each locale has an array of spaces for fork requests targeted to
// that locale.  The array is triply indexed, first by the index of
// the locale from which the request originated, then by the index of
// the originating communication domain on that locale, then to select
// among multiple request buffers for that locale and comm domain.
// The extent of the second index of this array is the maximum of the
// number of communication domains on any locale.  This will waste
// some space if the number of communication domains is not the same
// on all locales, but the entries are not that big and the simplicity
// makes both sending requests and acknowledging them easier, so the
// waste seems worthwhile.
//
// The fork_reqs array has FORK_REQ_BUFS_PER_CD space designated for
// fork requests from a particular communication domain on a
// particular locale.  A communication domain C on a locale L sends
// its fork requests to the following address on locale Target_L:
//
//   fork_reqs_map[Target_L]
//   + ((L * comm_dom_cnt_max) + C) * FORK_REQ_BUFS_PER_CD + rbi
//
// where rbi selects one of the FORK_REQ_BUFS_PER_CD fork request
// buffers belonging to that locale and comm domain.  The
// fork_reqs_map array is indexed by locale; each element [i] holds
// locale i's fork_reqs base address.
//
// There is an obvious tension on MAX_SMALL_CALL_PAYLOAD here.  The
// more data we can store in the fixed-sized elements of fork_reqs,
// the fewer fork requests there will be for which the remote end has
// to do a remote GET of an argument larger than that.  But there can
// be many fork_reqs elements.  For an application running on 1,024
// locales with 64 communication domains per locale and 2 fork request
// buffers per comm domain, the current MAX_SMALL_CALL_PAYLOAD means
// that each locale has about 128 MiB of space dedicated to fork
// requests.  That's not massive, but one can see that things could
// get out of hand quickly if MAX_SMALL_CALL_PAYLOAD were very
// large.  A large job (say 8K nodes with 128 comm domains) would
// require 2 GiB per node, which is pretty large, but not horrible.
// The largest XC possible is 32K nodes and would require 8 GiB per
// node.  That's pretty excessive, so if we start running at really
// high scales, we may need to revisit this decision.  Most people
// are running much smaller scales so we believe the trade-off is
// worthwhile.  As of this writing (11/30/18) the typical size for
// remote forks for our multi-locale performance benchmark kernels
// is between 512 and 1024 bytes.  Bundles for setup and non-kernel
// spawns are usually between 128 and 512 bytes.
//
typedef enum {
  fork_op_nil,
  fork_op_small_call,
  fork_op_large_call,
  fork_op_put,
  fork_op_get,
  fork_op_free,
  fork_op_amo,
  fork_op_shutdown,
  fork_op_num_ops
} fork_op_t;

#define FORK_OP_BITS 3

typedef struct {
  unsigned char op: FORK_OP_BITS;    // operation
  c_nodeid_t    caller;              // requesting locale
  rf_done_t*    rf_done;             // where to indicate completion
#ifdef CHPL_COMM_DEBUG
  uint_least64_t seq;
#endif
} fork_base_info_t;

typedef struct {
  fork_base_info_t b;

  c_sublocid_t  subloc;  // target sublocale
  unsigned char fast:          1;
  unsigned char blocking:      1;
  unsigned short payload_size;
  chpl_fn_int_t fid;

  // TODO: is there a way to "compress" this?
  chpl_task_ChapelData_t state;
} fork_small_call_info_t;

#define MAX_SMALL_CALL_PAYLOAD 1024
#define FORK_T_MAX_SIZE \
  (sizeof(fork_small_call_info_t) + MAX_SMALL_CALL_PAYLOAD)


typedef struct {
  fork_small_call_info_t b;
  c_sublocid_t           subloc;  // target sublocale
  unsigned int           arg_size;
  void*                  arg;
} fork_large_call_info_t;


typedef struct {
  fork_base_info_t b;
  void*            tgt;                 // target loc
  void*            src;                 // source loc
  size_t           size;                // size
} fork_xfer_info_t;

typedef struct {
  fork_base_info_t b;
  void*            p;                   // pointer to memory to be freed
} fork_free_info_t;

typedef enum {
  put_32,
  put_64,
  get_32,
  get_64,
  xchg_32,
  xchg_64,
  cmpxchg_32,
  cmpxchg_64,
  and_i32,
  and_i64,
  or_i32,
  or_i64,
  xor_i32,
  xor_i64,
  add_i32,
  add_i64,
  add_r32,
  add_r64,
  num_fork_amo_cmds
} fork_amo_cmd_t;

typedef union {
  int     i;    // used by amo_res_*() mgmt of temp AMO result buffers
  int64_t i64;  // unref'd; present only to 8-byte align enclosing union type
} fork_amo_data_t;

typedef struct {
  fork_base_info_t b;
  fork_amo_cmd_t   cmd;                 // the AMO command
  void*            obj;                 // address of target datum
  fork_amo_data_t  opnd1;               // first or only operand
  fork_amo_data_t  opnd2;               // second operand
  void*            res;                 // if non-NULL, where to send result
} fork_amo_info_t;

typedef struct {
  fork_base_info_t b;
} fork_shutdown_info_t;

typedef struct {
  unsigned char buf[FORK_T_MAX_SIZE];
} fork_space_t;

typedef union fork_t {
  fork_base_info_t b;
  fork_small_call_info_t sc;
  fork_large_call_info_t lc;
  fork_xfer_info_t x;
  fork_free_info_t f;
  fork_amo_info_t  a;
  fork_shutdown_info_t s;
  fork_space_t bytes; // get fork_t to be >= FORK_T_MAX_SIZE bytes
} fork_t;

typedef struct {
  fork_t fork;
  nb_desc_t nb_desc;
  chpl_bool free;
} nb_fork_t;

typedef struct {
  chpl_task_bundle_t task;
  fork_xfer_info_t x;
} fork_xfer_task_t;

typedef struct {
  chpl_task_bundle_t task;
  fork_large_call_info_t large;
} fork_large_call_task_t;

typedef struct {
  // unlike xfer/large call, need comm_on_bundle here for two reasons:
  //  1) the generated code puts a comm_on_bundle_t as the first field
  //     in any argument bundle for an on-statement
  //  2) we need somewhere to store information to indicate completion.
  chpl_comm_on_bundle_t bundle;
  unsigned char buf[MAX_SMALL_CALL_PAYLOAD];
} fork_small_call_task_t;

#ifdef CHPL_COMM_DEBUG
static int _fork_req_bufs_per_cd = 2;
#define FORK_REQ_BUFS_PER_CD     _fork_req_bufs_per_cd
#else
#define FORK_REQ_BUFS_PER_CD     2

#if FORK_REQ_BUFS_PER_CD > (1 << _IID_RBI_BITS)
#error too many request buffers per CD for internal encoding
#endif
#endif

#define FORK_REQ_BUFS_PER_LOCALE (chpl_numNodes                         \
                                  * comm_dom_cnt_max                    \
                                  * FORK_REQ_BUFS_PER_CD)

static fork_t*  fork_reqs     = NULL;
static fork_t** fork_reqs_map = NULL;

//
// These access the fork request buffers.
//
// SEND_SIDE_FORK_REQ_BUF_ADDR gives the address on receiving locale
// li of one of the request buffers that belongs to the current locale
// and communication domain.
//
// RECV_SIDE_FORK_REQ_BUF_ADDR gives the address of that same request
// buffer, but on the receiving side.  Here the receiving locale index
// is known already (it's the current one), and the li and cdi arguments
// correspond to the sender's locale and communication domain indices,
// respectively.
//
// _FRB_IDX is a private building block function, which does the index
// calculation for the fork request buffers.
//

#define _FRB_IDX(li, cdi, rbi) \
        (((((li) * comm_dom_cnt_max) + (cdi)) * FORK_REQ_BUFS_PER_CD) + (rbi))
#define SEND_SIDE_FORK_REQ_BUF_ADDR(r_li, cdi, rbi) \
        (fork_reqs_map[r_li] + _FRB_IDX(chpl_nodeID, cdi, rbi))
#define RECV_SIDE_FORK_REQ_BUF_ADDR(s_li, cdi, rbi) \
        (&fork_reqs[_FRB_IDX(s_li, cdi, rbi)])

//
// Each locale also maintains an array of flags telling whether each
// of its remote fork buffers is free or busy.  Unlike the request
// buffers themselves, however, these flags are local.  The request
// buffer on receiving locale rli, used by sending locale sli and
// communication domain cdi and request buffer index rbi, is
//
//   fork_reqs[_FRB_IDX(sli, cdi, rbi)]
//
// The free/busy flag corresponding to that buffer is on locale sli
// itself, at
//
//   fork_reqs_free[_FRB_IDX(rli, cdi, rbi)]
//
// The free/busy flags are maintained on the sending side because the
// receiving side only needs to mark them free, while the sending side
// needs both to mark them busy and to wait for them to be marked free
// by the receiving side.  This waiting may result in many references,
// which we don't want to do over the network.
//
static chpl_bool32*  fork_reqs_free     = NULL;
static chpl_bool32** fork_reqs_free_map = NULL;

//
// These access the fork request buffer free flags.
//
// SEND_SIDE_FORK_REQ_FREE_ADDR is used on the sending side to get the
// address of the free/busy flag corresponding to the request buffer
// associated with the given remote locale, communication domain, and
// request buffer index.
//
// RECV_SIDE_FORK_REQ_FREE_ADDR is used on the receiving side to get
// the address on the sending side, of that same free/busy flag.
//
#define SEND_SIDE_FORK_REQ_FREE_ADDR(r_li, cdi, rbi) \
        (&fork_reqs_free[_FRB_IDX(r_li, cdi, rbi)])
#define RECV_SIDE_FORK_REQ_FREE_ADDR(s_li, cdi, rbi) \
        (fork_reqs_free_map[s_li] + _FRB_IDX(chpl_nodeID, cdi, rbi))


//
// Tables of NIC AMO opcodes indexed by our own network atomic codes
// (fork_amo_cmd_t) for Gemini and Aries, non-fetching and fetching.
//
static gni_fma_cmd_type_t nic_amos_gem_fetch[]  // Gemini, fetching
           = { -1,                       // put_32
               -1,                       // put_64
               -1,                       // get_32
               -1,                       // get_64
               -1,                       // xchg_32
               -1,                       // xchg_64 (special-cased in code)
               -1,                       // cmpxchg_32
               GNI_FMA_ATOMIC_CSWAP,     // cmpxchg_64
               -1,                       // and_i32
               GNI_FMA_ATOMIC_FAND,      // and_i64
               -1,                       // or_i32
               GNI_FMA_ATOMIC_FOR,       // or_i64
               -1,                       // xor_i32
               GNI_FMA_ATOMIC_FXOR,      // xor_i64
               -1,                       // add_i32
               GNI_FMA_ATOMIC_FADD,      // add_i64
               -1,                       // add_r32
               -1                        // add_r64
             };

static gni_fma_cmd_type_t nic_amos_gem[]        // Gemini, non-fetching
           = { -1,                       // put_32
               -1,                       // put_64
               -1,                       // get_32
               -1,                       // get_64
               -1,                       // xchg_32
               -1,                       // xchg_64
               -1,                       // cmpxchg_32
               -1,                       // cmpxchg_64
               -1,                       // and_i32
               GNI_FMA_ATOMIC_AND,       // and_i64
               -1,                       // or_i32
               GNI_FMA_ATOMIC_OR,        // or_i64
               -1,                       // xor_i32
               GNI_FMA_ATOMIC_XOR,       // xor_i64
               -1,                       // add_i32
               GNI_FMA_ATOMIC_ADD,       // add_i64
               -1,                       // add_r32
               -1                        // add_r64
             };

static gni_fma_cmd_type_t nic_amos_ari_fetch[]  // Aries, fetching
           = { -1,                       // put_32
               -1,                       // put_64
               -1,                       // get_32
               -1,                       // get_64
               GNI_FMA_ATOMIC2_FSWAP_S,  // xchg_32
               GNI_FMA_ATOMIC2_FSWAP,    // xchg_64
               GNI_FMA_ATOMIC2_FCSWAP_S, // cmpxchg_32
               GNI_FMA_ATOMIC2_FCSWAP,   // cmpxchg_64
               GNI_FMA_ATOMIC2_FAND_S,   // and_i32
               GNI_FMA_ATOMIC2_FAND,     // and_i64
               GNI_FMA_ATOMIC2_FOR_S,    // or_i32
               GNI_FMA_ATOMIC2_FOR,      // or_i64
               GNI_FMA_ATOMIC2_FXOR_S,   // xor_i32
               GNI_FMA_ATOMIC2_FXOR,     // xor_i64
               GNI_FMA_ATOMIC2_FIADD_S,  // add_i32
               GNI_FMA_ATOMIC2_FIADD,    // add_i64
               GNI_FMA_ATOMIC2_FFPADD_S, // add_r32
               -1                        // add_r64
             };

static gni_fma_cmd_type_t nic_amos_ari[]        // Aries, non-fetching
           = { -1,                       // put_32
               -1,                       // put_64
               -1,                       // get_32
               -1,                       // get_64
               -1,                       // xchg_32
               -1,                       // xchg_64
               -1,                       // cmpxchg_32
               -1,                       // cmpxchg_64
               GNI_FMA_ATOMIC2_AND_S,    // and_i32
               GNI_FMA_ATOMIC2_AND,      // and_i64
               GNI_FMA_ATOMIC2_OR_S,     // or_i32
               GNI_FMA_ATOMIC2_OR,       // or_i64
               GNI_FMA_ATOMIC2_XOR_S,    // xor_i32
               GNI_FMA_ATOMIC2_XOR,      // xor_i64
               GNI_FMA_ATOMIC2_IADD_S,   // add_i32
               GNI_FMA_ATOMIC2_IADD,     // add_i64
               GNI_FMA_ATOMIC2_FPADD_S,  // add_r32
               -1                        // add_r64
             };


//
// For remote GETs, there are a couple of requirements on the (local)
// target buffer.  It has to be in registered memory, and it has to be
// 4-byte aligned.  The GET size also has to be a multiple of 4-bytes.
// If any of these requirements aren't met by the user-specific local
// buffer or size, we acquire a temporary buffer to do the GET into
// and then copy the desired data into the user's buffer.  If our
// stack is in NIC-registered memory we try to use a statically-sized
// local buffer, but if it isn't or the static size is too small then
// we allocate one dynamically from one of the pools defined here.  We
// arrange for these pools always to be in registered memory.  For
// now, though, note that if the GET size is big enough, the logic
// currently is not able to provide a buffer and the program will
// halt.  This is a temporary limitation and we expect to resolve it
// in the near future.
//
// There are pools here for buffers of size 8, 64, 512, and 4096.  To
// ensure that these are in registered memory we allocate them up
// front, with a fixed size, through the registered-memory manager.
// Per size, there are gbp_N_num_pools pools, with gbp_N_num_per_pool
// items in each one.  The requirements/constraints on these values
// are as follows.
//   - gbp_N_num_pools limits the allocate/free concurrency, so it
//     should be at least as large as the number of hardware threads
//     in use.
//   - We emulate an unsigned mod by gbp_N_num_pools by means of
//     & (gbp_N_num_pools - 1), so that has to be a power of 2.
//   - The code does divides and mods with gbp_N_num_per_pool, so
//     for performance that should be a power of 2.
//   - It's possible (though highly unlikely) that every task in the
//     program could need one of these at the same time.  We can't
//     get more dynamically, so we potentially need a lot of them.
//     Thus the product of gbp_N_num_pools and gbp_N_num_per_pool
//     should be "large".  However, we expect less demand for larger
//     buffers, so we provide many of the little ones and fewer of
//     the big ones.
//

#define DECL_GET_BUF_POOL(size, nPools, nPerPool)                       \
        static const int gbp_##size##_num_pools = nPools;               \
        static const int gbp_##size##_num_per_pool = nPerPool;          \
        static int64_t* gbp_##size;                                     \
        static mpool_idx_t gbp_##size##_head[nPools];                   \
        static atomic_bool gbp_##size##_lock[nPools];                   \
        static mpool_idx_t gbp_##size##_pool_i;                         \
        static inline                                                   \
        mpool_idx_base_t gbp_##size##_next_pool_i(void)                 \
        {                                                               \
          return mpool_idx_finc(&gbp_##size##_pool_i) & (nPools - 1);   \
        }

DECL_GET_BUF_POOL(   8, 128, 128)
DECL_GET_BUF_POOL(  64, 128, 128)
DECL_GET_BUF_POOL( 512,  64,  32)
DECL_GET_BUF_POOL(4096,  32,   8)

#undef DECL_GET_BUF_POOL

static const int gbp_max_size = 4096;


//
// For fetching remote AMOs, the remote side does a PUT to store the
// result.  The target (on the initiating side) of this PUT must be in
// memory registered with the NIC so that the remote side can do the
// PUT directly.  The caller of a remote AMO function supplies a
// result buffer.  If this is not in NIC-registered memory we try to
// use one on the stack, local to the AMO function.  If this in turn
// is not in NIC-registered memory we allocate one from the pools
// defined here.  We arrange for these pools to be in registered
// memory always, so one way or another the remote AMO can always
// store the result.
//
// To ensure that these are in registered memory we allocate them up
// front, with a fixed size, through the registered-memory manager.
// There are AMO_RES_NUM_POOLS pools, with AMO_RES_NUM_PER_POOL in
// each one.  The requirements/constraints on these values are as
// follows.
//   - AMO_RES_NUM_POOLS limits the allocate/free concurrency, so it
//     should be at least as large as the number of hardware threads
//     in use.
//   - We emulate an unsigned mod by AMO_RES_NUM_POOLS by means of
//     & (AMO_RES_NUM_POOLS - 1), so that has to be a power of 2.
//   - The code does divides and mods with AMO_RES_NUM_PER_POOL, so
//     for performance that should be a power of 2.
//   - It's possible (though highly unlikely) that every task in the
//     program could need one of these at the same time, and we can't
//     get more dynamically, so we potentially need a lot of them: the
//     product of AMO_RES_NUM_POOLS and AMO_RES_NUM_PER_POOL should be
//     "large".  Fortunately they're small, so over-provisioning isn't
//     particularly costly.
//
#define AMO_RES_NUM_POOLS 128
#define AMO_RES_NUM_PER_POOL 128

static fork_amo_data_t (*amo_res_pool)[AMO_RES_NUM_PER_POOL];
static mpool_idx_t amo_res_pool_head[AMO_RES_NUM_POOLS];
static atomic_bool amo_res_pool_lock[AMO_RES_NUM_POOLS];

static mpool_idx_t amo_res_pool_i;

static inline
mpool_idx_base_t amo_res_next_pool_i(void) 
{
  return mpool_idx_finc(&amo_res_pool_i) & (AMO_RES_NUM_POOLS - 1);
}


//
// Declarations having to do with barriers.
//
// We do a simple tree-based split-phase barrier, with locale 0 as the
// root of the tree.  Each of the locales has a barrier_info_t struct,
// and knows the address of that struct in its child locales (locales
// num_children*my_idx+1 - num_children*my_idx+num_children) and its
// parent (locale (my_idx-1)/num_children).  Notify and release flags on
// all locales start out 0.  The notify step consists of each locale
// waiting for its children, if it has any, to set the child_notify
// flags in its own barrier info struct to 1, and then if it is not
// locale 0, setting the child_notify flag corresponding to itself in
// its parent's barrier info struct to 1.  Thus notification propagates
// up from the leaves of the tree to the root.  In the wait phase each
// locale except locale 0 waits for the parent_release flag in its own
// barrier info struct to become 1.  Once a locale sees that, it clears
// all of the flags in its own struct and then sets the parent_release
// flags in both of its existing children to 1.  Thus releases propagate
// down from locale 0 to the leaves.  Once waiting is complete at the
// leaves, all of the flags throughout the job are back to 0 and the
// process can repeat.
//
// Note that we can (and do) do other things while waiting for notify
// and release flags to be set.
//
// Since we do a vector/chained put, make the number of children based
// on the max chained put length. Any smaller and we'd be unnecessarily
// adding locales and thus extra 1-way network delays (each additional
// layer in a tree-based spawn adds at least the cost of a network trip
// (plus the time for the child's task to wake up). Any larger and the
// parent locale would have to wait for the round trip ACK back from the
// chained put. This seems like a good balance (assuming a child can
// wake up within roughly a 1-way network trip.)
//
#define BAR_TREE_NUM_CHILDREN MAX_CHAINED_PUT_LEN

typedef struct {
  volatile uint32_t child_notify[BAR_TREE_NUM_CHILDREN];
  volatile uint32_t parent_release;
} barrier_info_t;

static uint32_t bar_min_child;
static uint32_t bar_num_children;
static uint32_t bar_parent;

static barrier_info_t  bar_info;
static barrier_info_t* child_bar_info[BAR_TREE_NUM_CHILDREN];
static barrier_info_t* parent_bar_info;


//
// These flags tell tell the state of, or give direction to, the polling
// task.
//
static volatile chpl_bool polling_task_running     = false;
static volatile chpl_bool polling_task_please_exit = false;
static volatile chpl_bool polling_task_done        = false;

static chpl_bool polling_task_blocking_cq;


//
// Specialized argument type and values for the may_remote_proxy
// argument to do_remote_put() and ..._get().
//
typedef enum {
  may_proxy_false = false,
  may_proxy_true = true
} drpg_may_proxy_t;


//
// Local functions.
//
static uint32_t  gni_get_nic_address(int);
static void      compute_comm_dom_cnt(void);
static void      gni_setup_per_comm_dom(int);
static void      gni_init(gni_nic_handle_t*, int);
static uint8_t   GNIT_Ptag(void);
static uint32_t  GNIT_Cookie(void);
static void      mem_regions_all_pre_init(void);
static void      register_memory(void);
static chpl_bool get_next_rw_memory_range(uint64_t*, uint64_t*, char*, size_t);
static gni_return_t register_mem_region(uint64_t, uint64_t, gni_mem_handle_t*,
                                        chpl_bool);
static void      deregister_mem_region(mem_region_t*);
static mem_region_t* mreg_for_addr(void*, mem_region_table_t*);
static mem_region_t* mreg_for_local_addr(void*);
static mem_region_t* mreg_for_remote_addr(void*, c_nodeid_t);
static void      polling_task(void*);
static void      set_up_for_polling(void);
static void      ensure_registered_heap_info_set(void);
static void      make_registered_heap(void);
static size_t    get_hugepage_size(void);
static void      set_hugepage_info(void);
static void      install_SIGBUS_handler(void);
static void      SIGBUS_handler(int, siginfo_t *, void *);
static void      regMemLock(void);
static void      regMemUnlock(void);
static void      regMemBroadcast(int, int, chpl_bool);
static void      exit_all(int);
static void      exit_any(int);
static void      rf_handler(gni_cq_entry_t*);
static void      fork_call_wrapper_blocking(chpl_comm_on_bundle_t*);
static void      fork_call_wrapper_large(fork_large_call_task_t*);
static void      fork_get_wrapper(fork_xfer_task_t*);
static size_t    do_amo_on_cpu(fork_amo_cmd_t, void*, void*, void*, void*);
static void      fork_amo_wrapper(fork_amo_info_t*);
static void      release_req_buf(uint32_t, int, int);
static void      indicate_done(fork_base_info_t* b);
static void      indicate_done2(int, rf_done_t *);
static void      send_polling_response(void*, c_nodeid_t, void*, size_t,
                                       mem_region_t*);
static void      rf_done_pre_init(void);
static void      rf_done_init(void);
static rf_done_t* rf_done_alloc(void);
static void      rf_done_free(rf_done_t*);
static void      get_buf_pre_init(void);
static void      get_buf_init(void);
static void*     get_buf_alloc(size_t);
static void      get_buf_free(int64_t*);
static void      amo_res_pre_init(void);
static void      amo_res_init(void);
static fork_amo_data_t* amo_res_alloc(void);
static void      amo_res_free(fork_amo_data_t*);
static void      consume_all_outstanding_cq_events(int);
static void      do_remote_put(void*, c_nodeid_t, void*, size_t,
                               mem_region_t*, drpg_may_proxy_t);
static void      do_remote_put_buff(void*, c_nodeid_t, void*, size_t,
                                    drpg_may_proxy_t);
static void      do_remote_put_V(int, void**, c_nodeid_t*, void**, size_t*,
                                 mem_region_t**, drpg_may_proxy_t);
static void      do_remote_get(void*, c_nodeid_t, void*, size_t,
                               drpg_may_proxy_t);
static void      do_remote_get_buff(void*, c_nodeid_t, void*, size_t,
                                    drpg_may_proxy_t);
static void      do_remote_get_V(int, void**, c_nodeid_t*, mem_region_t**,
                                 void**, size_t*, mem_region_t**,
                                 drpg_may_proxy_t);
static void      do_nic_get(void*, c_nodeid_t, mem_region_t*,
                            void*, size_t, mem_region_t*);
static int       amo_cmd_2_nic_op(fork_amo_cmd_t, int);
static void      do_nic_amo(void*, void*, c_nodeid_t, void*, size_t,
                            gni_fma_cmd_type_t, void*, mem_region_t*);
static void      do_nic_amo_nf(void*, c_nodeid_t, void*, size_t,
                               gni_fma_cmd_type_t, mem_region_t*);
static void      do_nic_amo_nf_buff(void*, c_nodeid_t, void*, size_t,
                                    gni_fma_cmd_type_t, mem_region_t*);
static void      do_nic_amo_nf_V(int, uint64_t*, c_nodeid_t*, void**, size_t*,
                                 gni_fma_cmd_type_t*, mem_region_t**);
static void      fork_call_common(int, c_sublocid_t,
                                  chpl_fn_int_t,
                                  chpl_comm_on_bundle_t*, size_t,
                                  chpl_bool, chpl_bool);
static void      fork_put(void*, c_nodeid_t, void*, size_t);
static void      fork_get(void*, c_nodeid_t, void*, size_t);
static void      fork_free(c_nodeid_t, void*);
static void      fork_amo(fork_t*, c_nodeid_t);
static void      fork_shutdown(c_nodeid_t);
static void      do_fork_post(c_nodeid_t, chpl_bool,
                              uint64_t, fork_base_info_t* const, int*, int*);
static void      acquire_comm_dom(void);
static void      acquire_comm_dom_and_req_buf(c_nodeid_t, int*);
static void      release_comm_dom(void);
static chpl_bool reacquire_comm_dom(int);
static int       post_fma(c_nodeid_t, gni_post_descriptor_t*);
static void      post_fma_and_wait(c_nodeid_t, gni_post_descriptor_t*,
                                   chpl_bool);
#if HAVE_GNI_FMA_CHAIN_TRANSACTIONS
static int       post_fma_ct(c_nodeid_t*, gni_post_descriptor_t*);
static void      post_fma_ct_and_wait(c_nodeid_t*, gni_post_descriptor_t*);
#endif
static int       post_rdma(c_nodeid_t, gni_post_descriptor_t*);
static void      post_rdma_and_wait(c_nodeid_t, gni_post_descriptor_t*,
                                    chpl_bool);
static chpl_bool can_task_yield(void);
static void      local_yield(void);


//
// More debug support.
//
#ifdef CHPL_COMM_DEBUG
static void dbg_init(void);
static void dbg_init(void)
{
  atomic_init_uint_least32_t(&next_thread_idx, 0);
  proc_thread_id = pthread_self();

  debug_flag = (uint64_t) chpl_env_rt_get_int("COMM_UGNI_DEBUG", 0);
  if (debug_flag != 0) {
    debug_nodeID = (int) chpl_env_rt_get_int("COMM_UGNI_DEBUG_NODE", -1);
    if (debug_nodeID >= 0)
      debug_flag |= DBGF_1_NODE;
  }

  debug_file = stderr;
  if (_DBG_DO(DBGF_IN_FILE)) {
    char fname[100];
    FILE* f;

    (void) snprintf(fname, sizeof(fname),
                    "debug-ugni.%d.out", (int) chpl_nodeID);
    if ((f = fopen(fname, "w")) != NULL) {
      debug_file = f;
      setbuf(debug_file, NULL);
    }
  }

  {
    char buf[100];

    (void) gethostname(buf, sizeof(buf));
    DBG_P_L(DBGF_NODENAME,
            "chpl_nodeID %d is on hostname \"%.*s\"",
            (int) chpl_nodeID, (int) sizeof(buf), buf);
  }

  const int evNode = (int) chpl_env_rt_get_int("COMM_UGNI_DEBUG_CORE_NODE", -1);
  if (evNode >= 0) {
    if ((int) chpl_nodeID != evNode) {
      struct rlimit rl;
      rl.rlim_cur = 0;
      rl.rlim_max = RLIM_SAVED_MAX;
      if (setrlimit(RLIMIT_CORE, &rl) != 0)
        CHPL_INTERNAL_ERROR("setrlimit(RLIMIT_CORE)");
      printf("%d: core limit set to 0\n", (int) chpl_nodeID);
      fflush(stdout);
    }
  }

  debug_stats_flag = chpl_env_rt_get_int("COMM_UGNI_DEBUG_STATS", 0);

#ifdef CHPL_COMM_DEBUG
  FORK_REQ_BUFS_PER_CD =
      chpl_env_rt_get_int("COMM_UGNI_FORK_REQ_BUFS_PER_CD", 1);
#endif
}


static const char* fork_op_name(fork_op_t);
static const char* fork_op_name(fork_op_t op)
{
  static const char* names[] = { "nil",
                                 "small_call",
                                 "large_call",
                                 "put",
                                 "get",
                                 "free",
                                 "amo" };
  return ((int)op >= 0 && op < fork_op_num_ops) ? names[op] : "?op?";
}


static atomic_uint_least64_t dbg_fork_seq;
#define DBG_SET_SEQ(s) (s = atomic_fetch_add_uint_least64_t(&dbg_fork_seq, 1))


static const char* fork_amo_name(fork_amo_cmd_t);
static const char* fork_amo_name(fork_amo_cmd_t cmd)
{
  static const char* names[] = { "put_32",
                                 "put_64",
                                 "get_32",
                                 "get_64",
                                 "xchg_32",
                                 "xchg_64",
                                 "cmpxchg_32",
                                 "cmpxchg_64",
                                 "and_i32",
                                 "and_i64",
                                 "or_i32",
                                 "or_i64",
                                 "xor_i32",
                                 "xor_i64",
                                 "add_i32",
                                 "add_i64",
                                 "add_r32",
                                 "add_r64" };
  return ((int)cmd >= 0 && cmd < num_fork_amo_cmds) ? names[cmd] : "?cmd?";
}


static char* sprintf_rf_req(int, void*);
static char* sprintf_rf_req(int loc, void* f_in)
{
  static __thread char buf[100];
  int bufcnt;
  fork_base_info_t* f = (fork_base_info_t*) f_in;
  fork_op_t op = f->op;

  bufcnt = snprintf(buf, sizeof(buf),
                    "<%" PRIu64 ">: %s loc %i ",
                    f->seq, fork_op_name(op), loc);

  switch (op) {
  case fork_op_small_call:
    {
      fork_small_call_info_t* pc = (fork_small_call_info_t*) f;

      snprintf(&buf[bufcnt], sizeof(buf) - bufcnt,
               "ftable[%d](%d bytes)%s%s",
               pc->fid, (int) pc->payload_size,
               pc->fast ? ", fast" : "",
               pc->blocking ? "" : ", nb");
    }
    break;

  case fork_op_large_call:
    {
      fork_large_call_info_t* lc = (fork_large_call_info_t*) f;

      snprintf(&buf[bufcnt], sizeof(buf) - bufcnt,
               "ftable[%d](%d bytes) (arg %p %d)%s%s, large",
               lc->b.fid, (int) lc->b.payload_size,
               lc->arg, (int) lc->arg_size,
               lc->b.fast ? ", fast" : "",
               lc->b.blocking ? "" : ", nb");

      assert(lc->arg != NULL);
      assert(lc->arg_size > 0);
    }
    break;

  case fork_op_put:
    {
      fork_xfer_info_t* px = (fork_xfer_info_t*) f;
      snprintf(&buf[bufcnt], sizeof(buf) - bufcnt,
               "%p -> %d:%p, %zd bytes",
               px->src, loc, px->tgt, px->size);
    }
    break;

  case fork_op_get:
    {
      fork_xfer_info_t* px = (fork_xfer_info_t*) f;
      snprintf(&buf[bufcnt], sizeof(buf) - bufcnt,
               "%p <- %d:%p, %zd bytes",
               px->tgt, loc, px->src, px->size);
    }
    break;

  case fork_op_free:
    {
      fork_free_info_t* fr = (fork_free_info_t*) f;
      snprintf(&buf[bufcnt], sizeof(buf) - bufcnt, "%p", fr->p);
    }
    break;

  case fork_op_amo:
    {
      fork_amo_info_t* pa = (fork_amo_info_t*) f;
      snprintf(&buf[bufcnt], sizeof(buf) - bufcnt, "%d:%p <- %s(%p)",
               loc, pa->res, fork_amo_name(pa->cmd), pa->obj);
    }
    break;

  default:
    snprintf(&buf[bufcnt], sizeof(buf) - bufcnt, "(op %d)", (int) op);
    break;
  }

  return buf;
}


static void dbg_catf(FILE*, const char*, const char*);
static void dbg_catf(FILE* out_f, const char* in_fname, const char* match)
{
  FILE* in_f;
  char buf[1000];

  if ((in_f = fopen(in_fname, "r")) == NULL)
    CHPL_INTERNAL_ERROR("fopen() failed");

  (void) fprintf(out_f, "==============================\nfile: %s\n\n",
                 in_fname);

  while (fgets(buf, sizeof(buf), in_f) != NULL) {
    if (match == NULL || strstr(buf, match) != NULL)
      (void) fputs(buf, out_f);
  }

  (void) fclose(in_f);
}

#define DBG_CATF(flg, out_f, in_fname, match)                           \
  do {                                                                  \
    if (_DBG_DO(flg)) dbg_catf(out_f, in_fname, match);                 \
  } while (0)

#else

#define DBG_SET_SEQ(s)

#define DBG_CATF(flg, out_f, in_fname, match)

#endif



//
// Common code for task local buffering
//

static inline
chpl_comm_taskPrvData_t* get_comm_taskPrvdata(void) {
  chpl_task_prvData_t* task_prvData = chpl_task_getPrvData();
  if (task_prvData != NULL) return &task_prvData->comm_data;
  return NULL;
}

enum BuffType {
  amo_nf_buff = 1 << 0,
  get_buff    = 1 << 1,
  put_buff    = 1 << 2
};

// Per task information about non-fetching AMO buffers
typedef struct {
  int                vi;
  uint64_t           opnd1_v[MAX_CHAINED_AMO_LEN];
  c_nodeid_t         locale_v[MAX_CHAINED_AMO_LEN];
  void*              object_v[MAX_CHAINED_AMO_LEN];
  size_t             size_v[MAX_CHAINED_AMO_LEN];
  gni_fma_cmd_type_t cmd_v[MAX_CHAINED_AMO_LEN];
  mem_region_t*      remote_mr_v[MAX_CHAINED_AMO_LEN];
} amo_nf_buff_task_info_t;

// Per task information about GET buffers
typedef struct {
  int           vi;
  void*         tgt_addr_v[MAX_CHAINED_GET_LEN];
  c_nodeid_t    locale_v[MAX_CHAINED_GET_LEN];
  mem_region_t* remote_mr_v[MAX_CHAINED_GET_LEN];
  void*         src_addr_v[MAX_CHAINED_GET_LEN];
  size_t        size_v[MAX_CHAINED_GET_LEN];
  mem_region_t* local_mr_v[MAX_CHAINED_GET_LEN];
} get_buff_task_info_t;

// Per task information about PUT buffers
typedef struct {
  int           vi;
  void*         tgt_addr_v[MAX_CHAINED_PUT_LEN];
  c_nodeid_t    locale_v[MAX_CHAINED_PUT_LEN];
  void*         src_addr_v[MAX_CHAINED_PUT_LEN];
  char          src_v[MAX_CHAINED_PUT_LEN][MAX_UNORDERED_TRANS_SZ];
  size_t        size_v[MAX_CHAINED_PUT_LEN];
  mem_region_t* remote_mr_v[MAX_CHAINED_PUT_LEN];
} put_buff_task_info_t;

// Acquire a task local buffer, initializing if needed
static inline
void* task_local_buff_acquire(enum BuffType t) {
  chpl_comm_taskPrvData_t* prvData = get_comm_taskPrvdata();
  if (prvData == NULL) return NULL;

#define DEFINE_INIT(TYPE, TLS_NAME)                                           \
  if (t == TLS_NAME) {                                                        \
    TYPE* info = prvData->TLS_NAME;                                           \
    if (info == NULL) {                                                       \
      prvData->TLS_NAME = chpl_mem_alloc(sizeof(TYPE),                        \
                                         CHPL_RT_MD_COMM_PER_LOC_INFO, 0, 0); \
      info = prvData->TLS_NAME;                                               \
      info->vi = 0;                                                           \
    }                                                                         \
    return info;                                                              \
  }

  DEFINE_INIT(amo_nf_buff_task_info_t, amo_nf_buff);
  DEFINE_INIT(get_buff_task_info_t, get_buff);
  DEFINE_INIT(put_buff_task_info_t, put_buff);

#undef DEFINE_INIT
  return NULL;
}

static void amo_nf_buff_task_info_flush(amo_nf_buff_task_info_t* info);
static void get_buff_task_info_flush(get_buff_task_info_t* info);
static void put_buff_task_info_flush(put_buff_task_info_t* info);

// Flush one or more task local buffers
static inline
void task_local_buff_flush(enum BuffType t) {
  chpl_comm_taskPrvData_t* prvData = get_comm_taskPrvdata();
  if (prvData == NULL) return;

#define DEFINE_FLUSH(TYPE, TLS_NAME, FLUSH_NAME)                              \
  if (t & TLS_NAME) {                                                         \
    TYPE* info = prvData->TLS_NAME;                                           \
    if (info != NULL && info->vi > 0) {                                       \
      FLUSH_NAME(info);                                                       \
    }                                                                         \
  }

  DEFINE_FLUSH(amo_nf_buff_task_info_t, amo_nf_buff, amo_nf_buff_task_info_flush);
  DEFINE_FLUSH(get_buff_task_info_t, get_buff, get_buff_task_info_flush);
  DEFINE_FLUSH(put_buff_task_info_t, put_buff, put_buff_task_info_flush);

#undef DEFINE_FLUSH
}

// Flush and destroy one or more task local buffers
static inline
void task_local_buff_end(enum BuffType t) {
  chpl_comm_taskPrvData_t* prvData = get_comm_taskPrvdata();
  if (prvData == NULL) return;

#define DEFINE_END(TYPE, TLS_NAME, FLUSH_NAME)                                \
  if (t & TLS_NAME) {                                                         \
    TYPE* info = prvData->TLS_NAME;                                           \
    if (info != NULL && info->vi > 0) {                                       \
      FLUSH_NAME(info);                                                       \
      chpl_mem_free(info, 0, 0);                                              \
      prvData->TLS_NAME = NULL;                                               \
    }                                                                         \
  }

  DEFINE_END(amo_nf_buff_task_info_t, amo_nf_buff, amo_nf_buff_task_info_flush);
  DEFINE_END(get_buff_task_info_t, get_buff, get_buff_task_info_flush);
  DEFINE_END(put_buff_task_info_t, put_buff, put_buff_task_info_flush);

#undef END
}



//
// Chapel interface starts here
//

int32_t chpl_comm_getMaxThreads(void)
{
  return 0;
}


void chpl_comm_init(int *argc_p, char ***argv_p)
{
  // Sanity check: a maximal small call fits into a fork_t
  assert(sizeof(fork_small_call_info_t)+MAX_SMALL_CALL_PAYLOAD
         <= sizeof(fork_t));

  if (fork_op_num_ops > (1 << FORK_OP_BITS))
    CHPL_INTERNAL_ERROR("too many fork OPs for internal encoding");

  {
    PMI_BOOL initialized;
    PMI_BOOL spawned;

    if (PMI_Initialized(&initialized) != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Initialized() failed");
    if (initialized != PMI_TRUE && PMI_Init(&spawned) != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Init() failed");
  }

  {
    int rank;

    if (PMI_Get_rank_in_app(&rank) != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Get_rank_in_app() failed");
    chpl_nodeID = (int32_t) rank;
  }

  DBG_INIT();  // needs chpl_nodeID
  PERFSTATS_INIT();

  {
    int app_size;

    if (PMI_Get_size(&app_size) != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Get_size() failed");
    chpl_numNodes = (int32_t) app_size;
    if (chpl_numNodes > (1 << _IID_LI_BITS))
      CHPL_INTERNAL_ERROR("too many locales for internal encoding");
  }

  {
    GNI_CHECK(GNI_GetDeviceType(&nic_type));
    if (nic_type != GNI_DEVICE_GEMINI
        && nic_type != GNI_DEVICE_ARIES)
      CHPL_INTERNAL_ERROR("unexpected GNI device type");
  }

  //
  // Supporting data for node-level barriers depends on the number of
  // nodes and our specific node ID.
  //
  bar_min_child = BAR_TREE_NUM_CHILDREN * chpl_nodeID + 1;
  if (bar_min_child >= chpl_numNodes)
    bar_num_children = 0;
  else {
    bar_num_children = BAR_TREE_NUM_CHILDREN;
    if (bar_min_child + bar_num_children >= chpl_numNodes)
      bar_num_children = chpl_numNodes - bar_min_child;
  }
  bar_parent = (chpl_nodeID - 1) / BAR_TREE_NUM_CHILDREN;

#define _PSV_INIT(psv) atomic_init_uint_least64_t(&_PSV_VAR(psv), 0);
  PERFSTATS_DO_ALL(_PSV_INIT);
#undef _PSTAT_INIT

  //
  // We can easily reach 4k memory regions on Aries.  We can reach a
  // bit more than 3500 memory regions on Gemini but getting there is
  // tricky because we start bumping up against a number of limits
  // all at once (node memory sizes, limits on number of registered
  // pages, available page sizes).  Reflecting that, on Gemini dial
  // back the default max number of registered memory regions to 2k.
  //
  max_mem_regions =
    chpl_env_rt_get_int("COMM_UGNI_MAX_MEM_REGIONS",
                        (nic_type == GNI_DEVICE_GEMINI) ? 2048 : 4096);

  //
  // We have to create the local memory region table before the first
  // call to regMemAlloc() is made.  But that could come from the memory
  // layer, which is initialized after this, so we can't get the memory
  // from there.  We don't want to get it from our own internal memory
  // manager because that isn't set up yet and it would give us remotely
  // accessible memory anyway, which we don't need for this.  So, just
  // allocate the space from the system.
  //
  mem_regions_size = sizeof(mem_regions[0])
                     + max_mem_regions * sizeof(mem_regions->mregs[0]);
  mem_regions = (mem_region_table_t*) sys_calloc(1, mem_regions_size);
  mem_regions->mreg_cnt = 0;
  atomic_init_int_least32_t(&mreg_free_cnt, max_mem_regions);

  mr_mregs_supplement =
    (struct mregs_supp*) sys_calloc(max_mem_regions,
                                    sizeof(mr_mregs_supplement[0]));
}


void chpl_comm_post_mem_init(void)
{
  chpl_comm_init_prv_bcast_tab();
}


//
// No support for gdb for now
//
int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status)
{
  return 0;
}

void chpl_comm_task_end(void) {
  task_local_buff_end(get_buff | put_buff | amo_nf_buff);
}

void chpl_comm_post_task_init(void)
{
  if (chpl_numNodes == 1)
    return;

  //
  // Do various hugepage-related checks.  These are delayed until this
  // point in order to allow command line argument processing to happen
  // first, so that the warnings here can be turned off via --quiet.
  //
  if (getenv("HUGETLB_DEFAULT_PAGE_SIZE") == NULL) {
    if (chpl_nodeID == 0) {
      chpl_warning("without hugepages, communication performance will suffer",
                   0, 0);
    }
  } else if (chpl_comm_getenvMaxHeapSize() == 0) {
    if (chpl_nodeID == 0) {
      if (getenv("HUGETLB_NO_RESERVE") == NULL) {
        chpl_warning("dynamic heap on hugepages "
                     "needs HUGETLB_NO_RESERVE set to something",
                     0, 0);
      }

      if (strcmp(CHPL_MEM, "jemalloc") == 0
          && getenv(chpl_comm_ugni_jemalloc_conf_ev_name()) == NULL) {
        char buf[200];
        (void) snprintf(buf, sizeof(buf),
                        "dynamic heap on hugepages needs %s set properly",
                        chpl_comm_ugni_jemalloc_conf_ev_name());
        chpl_warning(buf, 0, 0);
      }
    }
  }

  //
  // Now that the memory layer and tasking have been set up, we can
  // allocate the various data we need and fill it in.  Some of this
  // need not be shared and so we could have allocated it earlier, but
  // the logical flow is clearer if we do all this in one place.
  //

  //
  // Figure out how many comm domains we need.
  //
  compute_comm_dom_cnt();

  //
  // Get FMA/BTE threshold
  //
  rdma_threshold = chpl_env_rt_get_size("UGNI_RDMA_THRESHOLD",
                                        DEFAULT_RDMA_THRESHOLD);

  //
  // Get our NIC address and share it around the job.
  //
  // To avoid having to link in MPI, we use PMI_Allgather() here.  But
  // that routine yields unordered results, so we have to figure out
  // which nic_addr_map identifier goes with which locale ourselves.  We
  // do this by gathering (locale, nic_addr) pairs and then scattering
  // the nic_addr values into the map, which is indexed by locale.
  //
  // While we're at it, also compute the maximum number of communication
  // domains on any node and share the barrier info struct addresses
  // around the job.
  //
  {
    uint32_t nic_addr;

    nic_addr = gni_get_nic_address(0);
    nic_addr_map =
        (uint32_t*) chpl_mem_allocMany(chpl_numNodes, sizeof(nic_addr_map[0]),
                                       CHPL_RT_MD_COMM_PER_LOC_INFO,
                                       0, 0);

    {
      typedef struct {
        c_nodeid_t nodeID;
        uint32_t nic_addr;
        uint32_t comm_dom_cnt;
        barrier_info_t* bar_info;
      } gdata_t;

      gdata_t  my_gdata = { chpl_nodeID, nic_addr, comm_dom_cnt, &bar_info };
      gdata_t* gdata;

      gdata = (gdata_t*) chpl_mem_allocMany(chpl_numNodes, sizeof(gdata[0]),
                                            CHPL_RT_MD_COMM_PER_LOC_INFO,
                                            0, 0);
      if (PMI_Allgather(&my_gdata, gdata, sizeof(gdata[0])) != PMI_SUCCESS)
        CHPL_INTERNAL_ERROR("PMI_Allgather(nic_addr_map etc.) failed");

      comm_dom_cnt_max = gdata[0].comm_dom_cnt;  // redundant, but safe

      for (int i = 0; i < chpl_numNodes; i++) {
        nic_addr_map[gdata[i].nodeID] = gdata[i].nic_addr;
        if (gdata[i].comm_dom_cnt > comm_dom_cnt_max)
          comm_dom_cnt_max = gdata[i].comm_dom_cnt;

        if (gdata[i].nodeID >= bar_min_child
            && gdata[i].nodeID < bar_min_child + bar_num_children)
          child_bar_info[gdata[i].nodeID - bar_min_child] = gdata[i].bar_info;
        else if (chpl_nodeID != 0 && gdata[i].nodeID == bar_parent)
          parent_bar_info = gdata[i].bar_info;
      }

      chpl_mem_free(gdata, 0, 0);
    }
  }

  //
  // We now have all the variable values (specifically: number of
  // nodes and max number of comm domains on any node) that go into
  // computing how much NIC-registered memory we need.  Tell the
  // registered-memory module how much we need and then allocate it.
  //
  mem_regions_all_pre_init();
  get_buf_pre_init();
  rf_done_pre_init();
  amo_res_pre_init();

  chpl_comm_mem_reg_init();

  get_buf_init();
  rf_done_init();
  amo_res_init();

  //
  // Create all the communication domains, including their GNI NIC
  // handles, endpoints, and completion queues.
  //
  comm_doms =
    (comm_dom_t*) chpl_mem_memalign(CACHE_LINE_SIZE,
                                    comm_dom_cnt * sizeof(comm_dom_t),
                                    CHPL_RT_MD_COMM_PER_LOC_INFO, 0, 0);

  for (int i = 0; i < comm_dom_cnt; i++)
    gni_setup_per_comm_dom(i);

  atomic_init_int_least32_t(&global_init_cdi, 0);


  //
  // Register memory.
  //
  register_memory();

  //
  // Start the polling task.
  //
  if (chpl_task_createCommTask(polling_task, NULL) != 0)
    CHPL_INTERNAL_ERROR("unable to start comm task for uGNI comm layer");

  //
  // Wait for the polling task to be running before we return.  This is
  // to ensure that we don't initiate any remote forks until the target
  // locales are ready to handle them.  There is an implicit assumption
  // here that any subsidiary pthreads cannot initiate remote forks
  // until they find work (tasks) to do.  That can't happen until after
  // we return from here, since we are the main thread.  This also makes
  // sure that the polling task acquires its permanently-assigned comm
  // domain before anyone else starts grabbing any.
  //
  while (polling_task_running == false)
    sched_yield();
}


static
uint32_t gni_get_nic_address(int device_id)
{
  unsigned int address;
  unsigned int cpu_id;
  int          i;
  int          alps_dev_id = -1;
  int          alps_address = -1;
  char*        token;
  char*        p_ptr;

  p_ptr = getenv("PMI_GNI_DEV_ID");
  if (!p_ptr) {
    GNI_CHECK(GNI_CdmGetNicAddress(device_id, &address, &cpu_id));
  }
  else {
    while ((token = strtok(p_ptr,":")) != NULL) {
      alps_dev_id = atoi(token);
      if (alps_dev_id == device_id)
        break;
      p_ptr = NULL;
    }
    assert(alps_dev_id != -1);
    p_ptr = getenv("PMI_GNI_LOC_ADDR");
    assert(p_ptr != NULL);
    i = 0;
    while ((token = strtok(p_ptr, ":")) != NULL) {
      if (i == alps_dev_id) {
        alps_address = atoi(token);
        break;
      }
      p_ptr = NULL;
      ++i;
    }
    assert(alps_address != -1);
    address = alps_address;
  }

  return (uint32_t) address;
}


//
// This may call the tasking layer, so don't call it before that is
// initialized.
//
static void compute_comm_dom_cnt(void)
{
  int commConcurrency = -1;

  //
  // See if the user specified a comm layer concurrency value in the
  // environment.
  //
  {
    const char* s;
    int         ev;

    if ((s = getenv("CHPL_RT_COMM_CONCURRENCY")) != NULL
        && sscanf(s, "%d", &ev) == 1) {
      if (ev >= 0)
        commConcurrency = ev;
      else
        chpl_warning("CHPL_RT_COMM_CONCURRENCY < 0, ignored", 0, 0);
    }
  }

  //
  // If the user specified a communication concurrency value then make
  // that many comm domains.  Otherwise, if they specified the number
  // of hardware threads for tasking, make that many.  Otherwise, make
  // as many as the tasking layer's expected maximum useful level of
  // parallelism.  In the unlikely event that none of this was done or
  // is known, make 16.  In any case, always add one for the polling
  // task.
  //
  comm_dom_cnt = 0;
  if (commConcurrency == 0)
    comm_dom_cnt = 1;
  else if (commConcurrency > 0)
    comm_dom_cnt = commConcurrency;
  else {
    const char* s;
    int         ev;

    if ((s = getenv("CHPL_RT_NUM_HARDWARE_THREADS")) != NULL
        && sscanf(s, "%d", &ev) == 1) {
      if (ev > 0)
        comm_dom_cnt = ev;
      else
        chpl_warning("CHPL_RT_NUM_HARDWARE_THREADS <= 0, ignored", 0, 0);
    }

    if (comm_dom_cnt == 0) {
      uint32_t maxPar;

      if ((maxPar = chpl_task_getMaxPar()) > 0)
        comm_dom_cnt = maxPar;
    }

    if (comm_dom_cnt == 0)
      comm_dom_cnt = 16;
  }

  comm_dom_cnt++;  // count the polling task's dedicated comm domain

  //
  // Limit us to 30 communication domains on Gemini (architectural
  // limit = 32) and 120 on Aries (architectural limit = 128).
  //
  {
    int max_comm_dom_cnt = (nic_type == GNI_DEVICE_GEMINI) ? 30 : 120;
    if (comm_dom_cnt > max_comm_dom_cnt)
      comm_dom_cnt = max_comm_dom_cnt;
  }

  if (comm_dom_cnt >= (1 << _IID_CDI_BITS))
    CHPL_INTERNAL_ERROR("too many comm domains for internal encoding");
}


static
void gni_setup_per_comm_dom(int cdi)
{
  comm_dom_t*  cd = &comm_doms[cdi];
  uint32_t     i;

  INIT_CD_BUSY(cd);
  cd->firmly_bound = false;

  //
  // Create communication domain.
  //
  gni_init(&cd->nih, cdi);

  //
  // Create completion queue.
  //
  // TODO: The cq_cnt_max value specified here allows for up to
  //       CD_ACTIVE_TRANS_MAX transactions at a time to be active on
  //       the comm domain.  At some point we should collect some
  //       statistics on demands and delays, and select a better
  //       number.  (Also, the value appropriate for the polling
  //       comm domain may be different than that for initiators.)
  //
  cd->cq_cnt_max  = CD_ACTIVE_TRANS_MAX;
  CQ_CNT_INIT(cd, 0);
  GNI_CHECK(GNI_CqCreate(cd->nih, cd->cq_cnt_max, 0, GNI_CQ_NOBLOCK, NULL,
                         NULL, &cd->cqh));

  //
  // Create GNI EndPoints (EPs) for the nodes, and bind them to their
  // nodes.  This can be replaced by an EP-Pool for better scalability.
  //
  cd->remote_eps =
    (gni_ep_handle_t*) chpl_mem_allocMany(chpl_numNodes,
                                          sizeof(cd->remote_eps[0]),
                                          CHPL_RT_MD_COMM_PER_LOC_INFO,
                                          0, 0);
  for (i = 0; i < chpl_numNodes; i++) {
    GNI_CHECK(GNI_EpCreate(cd->nih, cd->cqh, &cd->remote_eps[i]));
    GNI_CHECK(GNI_EpBind(cd->remote_eps[i], nic_addr_map[i], cdi));
  }

#ifdef DEBUG_STATS
  cd->acqs               = 0;
  cd->acqs_looks         = 0;
  cd->acqs_with_rb       = 0;
  cd->acqs_with_rb_looks = 0;
  cd->reacqs             = 0;
#endif
}


static
void gni_init(gni_nic_handle_t* nih, int cdi)
{
  int              device_id = 0;
  uint32_t         ptag;
  uint32_t         cookie;
  uint32_t         modes = (GNI_CDM_MODE_FORK_NOCOPY
                            | GNI_CDM_MODE_ERR_NO_KILL
                            | GNI_CDM_MODE_FMA_DEDICATED
                            | GNI_CDM_MODE_CQ_NIC_LOCAL_PLACEMENT);
  uint32_t         local_address = 0;
  gni_cdm_handle_t cdm_handle;

  ptag = GNIT_Ptag();
  cookie = GNIT_Cookie();

/* -------------------------------------------------------------------------- *\
  GNI_CDM_MODES:
  - Check _FORK options for the data server
\* -------------------------------------------------------------------------- */
  GNI_CHECK(GNI_CdmCreate(cdi, ptag, cookie, modes, &cdm_handle));
  GNI_CHECK(GNI_CdmAttach(cdm_handle, device_id, &local_address, nih));
}


static
uint8_t GNIT_Ptag(void)
{
#define DEFAULT_PTAG   10

  char *p_ptr,*token;
  uint8_t ptag;
  int iptag;

  if (getenv("MPIRUN_ID")) {
    iptag = atoi(getenv("MPIRUN_ID"));
    ptag = (uint8_t) (iptag & 0xff);
    if (ptag < GNI_PTAG_USER_START)
      ptag += GNI_PTAG_USER_START;
    if (ptag > GNI_PTAG_USER_END) {
      ptag = DEFAULT_PTAG;
      fprintf(stderr,"Switching to legal ptag %d\n", ptag);
    }
  }
  else {
    p_ptr = getenv("PMI_GNI_PTAG");
    if (!p_ptr)
      return DEFAULT_PTAG;
    token = strtok(p_ptr, ":");
    ptag = (uint8_t) atoi(token);
  }

  return ptag;
}

static
uint32_t GNIT_Cookie(void)
{
#define DEFAULT_COOKIE 10

  uint32_t cookie;
  char *p_ptr,*token;

  if (getenv("MPIRUN_ID"))
    cookie = DEFAULT_COOKIE;
  else {
    p_ptr = getenv("PMI_GNI_COOKIE");
    if (!p_ptr)
      return DEFAULT_COOKIE;
    token = strtok(p_ptr, ":");
    cookie = (uint32_t) atoi(token);
  }

  return cookie;
}


static
void mem_regions_all_pre_init(void)
{
  chpl_comm_mem_reg_add_request(chpl_numNodes * mem_regions_size);
}


static
void register_memory(void)
{
  uint64_t  addr;
  uint64_t  len;
  char      pathname[100];
  void*     gnr_addr;
  int       have_hugepage_module
              = (getenv("HUGETLB_DEFAULT_PAGE_SIZE") != NULL);

  //
  // Register read/write memory regions found in /proc/self/maps.  If
  // the HUGETLB_DEFAULT_PAGE_SIZE environment variable is present,
  // indicating that we are using a craype-hugepage* module, then
  // register only non-anonymous regions (those associated with a path).
  // However, don't register device memory other than from /dev/zero.
  // This gets us the hugepage regions, the stack, and some other useful
  // things.  Only the max_mem_regions largest regions are registered,
  // except that the guaranteed NIC-registered region is registered no
  // matter how small it is.  In order to enhance the performance of
  // lookups we sort the regions in our memory map by size, from large
  // to small.  The hope is that the larger regions are referenced more
  // frequently than the smaller ones.
  //
  // If HUGETLB_DEFAULT_PAGE_SIZE is absent, indicating that we don't
  // have a hugepage module loaded, only record the guaranteed
  // NIC-registered segment.
  //
  chpl_comm_mem_reg_tell(&gnr_addr, NULL);

  DBG_CATF(DBGF_MEMMAPS, debug_file, "/proc/self/maps", NULL);
  DBG_CATF(DBGF_MEMMAPS, debug_file, "/proc/self/numa_maps", NULL);

  //
  // Register any already-recorded memory regions with uGNI.
  //
  for (int i = 0; i < mem_regions->mreg_cnt; i++) {
    (void) register_mem_region(mem_regions->mregs[i].addr,
                               mrtl_len(mem_regions->mregs[i].len),
                               &mem_regions->mregs[i].mdh,
                               false /*allow_failure*/);
    mrtl_setReg(&mem_regions->mregs[i].len);
  }

  while (get_next_rw_memory_range(&addr, &len, pathname, sizeof(pathname))) {
    int i;

    //
    // Skip this region if we've already seen it.  This happens when we
    // encounter regions in /proc/self/maps that have already been added
    // by chpl_comm_impl_regMemAlloc() before we were called.
    //
    i = 0;
    while (i < mem_regions->mreg_cnt
           && (addr != mem_regions->mregs[i].addr
               || len != mrtl_len(mem_regions->mregs[i].len))) {
      i++;
    }
    if (i < mem_regions->mreg_cnt)
      continue;

    //
    // We always register the guaranteed-registered memory region, and
    // if we aren't using hugepages that's all we try to register.  But
    // if we are using hugepages then we also try to register anything
    // with a non-empty path, with the exception of device paths that
    // aren't /dev/zero.
    //
    chpl_bool register_this_one = false;
    if (addr == (uint64_t) (intptr_t) gnr_addr) {
      register_this_one = true;
    } else if (have_hugepage_module && strlen(pathname) > 0) {
      if (strncmp(pathname, "/dev/", 5) != 0
          || strcmp(pathname, "/dev/zero") == 0
          || strncmp(pathname, "/dev/zero ", 10) == 0) {
        register_this_one = true;
      }
    }

    if (!register_this_one)
      continue;

    //
    // Try to register this segment.  If we can do so then keep it.
    // Otherwise, if its presence is mandatory then complain and halt,
    // and if its presence is optional then skip it.  The mandatory
    // regions are anything on hugepages.  This ends up including at
    // least the static data segment and all our dynamic allocations.
    //
    gni_mem_handle_t mdh;
    gni_return_t gni_rc;
    if ((gni_rc = register_mem_region(addr, len, &mdh, true /*allow_failure*/))
        != GNI_RC_SUCCESS) {
      if (strstr(pathname, "huge") != NULL) {
        GNI_FAIL(gni_rc, "register_mem_region()");
      } else {
        DBG_P_L(DBGF_MEMREG,
                "GNI_MemRegister(%#" PRIx64 ", %#" PRIx64 ", \"%s\") failed "
                "-- skipping",
                addr, len, pathname);
        continue;
      }
    }

    //
    // Put the memory region in the table, keeping it sorted by
    // decreasing size.
    //
    if (mem_regions->mreg_cnt >= max_mem_regions)
      CHPL_INTERNAL_ERROR("too many preexisting memory regions");

    for (i = mem_regions->mreg_cnt;
         i > 0 && len > mrtl_len(mem_regions->mregs[i - 1].len);
         i--) {
      mem_regions->mregs[i] = mem_regions->mregs[i - 1];
    }

    mem_regions->mregs[i].addr = addr;
    mem_regions->mregs[i].len = mrtl_encode(len, true);
    mem_regions->mregs[i].mdh = mdh;
    mem_regions->mreg_cnt++;
    (void) atomic_fetch_sub_int_least32_t(&mreg_free_cnt, 1);
  }

  if (mem_regions->mreg_cnt == 0) {
    CHPL_INTERNAL_ERROR("no registerable memory regions?");
  }

  if (mem_regions->mreg_cnt > mreg_cnt_max)
    mreg_cnt_max = mem_regions->mreg_cnt;

  //
  // Find the memory region associated with guaranteed NIC-registered
  // memory.  Recording this saves time looking it up later.
  //
  {
    void* p;

    chpl_comm_mem_reg_tell(&p, NULL);

    for (int i = 0; i < mem_regions->mreg_cnt; i++) {
      if ((void*) (intptr_t) mem_regions->mregs[i].addr == p) {
        gnr_mreg = &mem_regions->mregs[i];
        break;
      }
    }

    if (gnr_mreg == NULL)
      CHPL_INTERNAL_ERROR("cannot find gnr_mreg");
  }

  //
  // Figure out how many memory region descriptors we will need to
  // exchange in the next step.  The number of memory regions active
  // at this point can vary from node to node, but the allgather needs
  // to exchange the same amount of data from each.  We could exchange
  // all max_mem_regions (default 4k) of them instead, but we probably
  // have only a couple dozen active and 2 much smaller allgathers are
  // way faster than one much larger one.
  //
  uint32_t max_mreg_cnt = mem_regions->mreg_cnt;

  {
    uint32_t all_mc[chpl_numNodes];
    if (PMI_Allgather(&max_mreg_cnt, all_mc, sizeof(*all_mc)) != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Allgather(mreg_cnt) failed");
    for (int i = 0; i < chpl_numNodes; i++) {
      if (all_mc[i] > max_mreg_cnt)
        max_mreg_cnt = all_mc[i];
    }
  }

  //
  // Share the per-locale memory descriptors around the job.
  //

  // Must be directly communicable without proxy.
  mem_regions_all = (mem_region_table_t*)
                      chpl_comm_mem_reg_allocMany(chpl_numNodes,
                                                  mem_regions_size,
                                                  CHPL_RT_MD_COMM_PER_LOC_INFO,
                                                  0, 0);
  mem_regions_all_entries =
    (mem_region_table_t**)
      chpl_mem_allocMany(chpl_numNodes, sizeof(mem_regions_all_entries[0]),
                         CHPL_RT_MD_COMM_PER_LOC_INFO, 0, 0);
  for (int i = 0; i < chpl_numNodes; i++) {
    mem_regions_all_entries[i] =
      (mem_region_table_t*) ((char*) mem_regions_all + i * mem_regions_size);
  }

  mem_regions_all_my_entry_map =
    (mem_region_table_t**)
      chpl_mem_allocMany(chpl_numNodes,
                         sizeof(mem_regions_all_my_entry_map[0]),
                         CHPL_RT_MD_COMM_PER_LOC_INFO,
                         0, 0);
  gnr_mreg_map = (mem_region_t*)
                   chpl_mem_allocMany(chpl_numNodes, sizeof(gnr_mreg_map[0]),
                                      CHPL_RT_MD_COMM_PER_LOC_INFO, 0, 0);

  {
    typedef struct {
      c_nodeid_t nodeID;
      mem_region_table_t* mem_regions_all;
      mem_region_t gnr_mreg;
      uint32_t mreg_cnt;
      mem_region_t mregs[];
    } gdata_t;

    gdata_t* my_gdata;
    size_t gdata_mregs_size = max_mreg_cnt * sizeof(my_gdata->mregs[0]);
    size_t gdata_size = sizeof(my_gdata[0]) + gdata_mregs_size;
    my_gdata =
      (gdata_t*) chpl_mem_alloc(gdata_size,
                                CHPL_RT_MD_COMM_PER_LOC_INFO,
                                0, 0);
    my_gdata->nodeID = chpl_nodeID;
    my_gdata->mem_regions_all = mem_regions_all;
    my_gdata->gnr_mreg = *gnr_mreg;
    my_gdata->mreg_cnt = mem_regions->mreg_cnt;
    memcpy(my_gdata->mregs, mem_regions->mregs, gdata_mregs_size);

    gdata_t* gdata;
    gdata = (gdata_t*) chpl_mem_allocMany(chpl_numNodes, gdata_size,
                                          CHPL_RT_MD_COMM_PER_LOC_INFO,
                                          0, 0);
    if (PMI_Allgather(my_gdata, gdata, gdata_size) != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Allgather(sdata/heap/etc. memory maps) failed");

    for (int i = 0; i < chpl_numNodes; i++) {
      gdata_t* gdp = (gdata_t*) ((char*) gdata + i * gdata_size);
      int node = gdp->nodeID;

      gnr_mreg_map[node] = gdp->gnr_mreg;
      mem_regions_all_entries[node]->mreg_cnt = gdp->mreg_cnt;
      memcpy(&mem_regions_all_entries[node]->mregs, &gdp->mregs,
             gdata_mregs_size);
      mem_regions_all_my_entry_map[node] =
          (mem_region_table_t*)
          ((char*) gdp->mem_regions_all + chpl_nodeID * mem_regions_size);
    }

    chpl_mem_free(gdata, 0, 0);
    chpl_mem_free(my_gdata, 0, 0);
  }

  can_register_memory = true;
  chpl_atomic_thread_fence(memory_order_release);
}


static
chpl_bool get_next_rw_memory_range(uint64_t* addr, uint64_t* len,
                                   char* pathname, size_t pathname_size)
{
  static FILE*  f = NULL;
  uint64_t      lo_addr;
  uint64_t      hi_addr;
  char          perms[5];
  int           p_idx;
  int           ch;
  int           scn_cnt;

  if (f == NULL) {
    if ((f = fopen("/proc/self/maps", "r")) == NULL)
      CHPL_INTERNAL_ERROR("cannot fopen(\"/proc/self/maps\")");
  }

  while (true) {
    scn_cnt = fscanf(f, "%lx-%lx%4s%*x%*x:%*x%*x", &lo_addr, &hi_addr, perms);
    if (scn_cnt == EOF)
      break;
    else if (scn_cnt != 3)
      CHPL_INTERNAL_ERROR("unrecognized /proc/self/maps line format");

    //
    // Skip regions that are not read/write.
    //
    if (perms[0] != 'r' || perms[1] != 'w') {
      while ((ch = fgetc(f)) != EOF && ch != '\n')
        ;
      continue;
    }

    if (pathname == NULL) {
      //
      // Pathname not wanted -- skip the rest of this line.
      //
      while ((ch = fgetc(f)) != EOF && ch != '\n')
        ;
    }
    else {
      //
      // Pathname wanted -- skip leading white space, then pick up as
      // many characters as will fit in supplied buffer.
      //
      while ((ch = fgetc(f)) == ' ' || ch == '\t')
        ;

      for (p_idx = 0; ch != EOF && ch != '\n'; ch = fgetc(f)) {
        if (p_idx < pathname_size)
          pathname[p_idx++] = ch;
      }
      pathname[p_idx++] = '\0';
    }

    *addr = lo_addr;
    *len  = hi_addr - lo_addr;
    return true;
  }

  (void) fclose(f);
  f = NULL;
  return false;
}


static
inline
gni_return_t register_mem_region(uint64_t addr, uint64_t len,
                                 gni_mem_handle_t* mdh, chpl_bool allow_failure)
{
  uint32_t flags = GNI_MEM_READWRITE | GNI_MEM_RELAXED_PI_ORDERING;
  gni_return_t gni_rc;

  DBG_P_L(DBGF_MEMREG,
          "GNI_MemRegister(%#" PRIx64 ", %#" PRIx64 ")",
          addr, len);
  if ((gni_rc = GNI_MemRegister(comm_doms[0].nih, addr, len,
                                NULL, flags, -1, mdh))
      != GNI_RC_SUCCESS) {
    if (allow_failure)
      DBG_P_L(DBGF_MEMREG, "GNI_MemRegister(): %d", (int) gni_rc);
    else
      GNI_FAIL(gni_rc, "GNI_MemRegister()");
  }

  return gni_rc;
}


static
inline
void deregister_mem_region(mem_region_t* mr)
{
  DBG_P_L(DBGF_MEMREG,
          "GNI_MemDeregister(%#" PRIx64 ", %#" PRIx64 ")",
          mr->addr, mrtl_len(mr->len));
  GNI_CHECK(GNI_MemDeregister(comm_doms[0].nih, &mr->mdh));
}


static
inline
mem_region_t* mreg_for_addr(void* addr, mem_region_table_t* tab)
{
  uint64_t addr_ui = (uint64_t) addr;
  mem_region_t* mr;

  mr = tab->mregs;
  for (int i = 0; i < tab->mreg_cnt; i++, mr++) {
    if (addr_ui >= mr->addr
        && addr_ui < mr->addr + mrtl_len(mr->len)
        && mrtl_isReg(mr->len))
      return mr;
  }

  return NULL;
}


static
inline
mem_region_t* mreg_for_local_addr(void* addr)
{
  static __thread mem_region_t* mr;
  PERFSTATS_INC(local_mreg_cnt);
  PERFSTATS_TSTAMP(pstStart);
  if (mr == NULL
      || (uint64_t) addr < mr->addr
      || (uint64_t) addr >= mr->addr + mrtl_len(mr->len)
      || !mrtl_isReg(mr->len)) {
    mr = mreg_for_addr(addr, mem_regions);
    PERFSTATS_ADD(local_mreg_cmps,
                  ((mr == NULL)
                   ? mem_regions->mreg_cnt
                   : (mr - &mem_regions->mregs[0] + 1)));
  }
  PERFSTATS_ADD(local_mreg_nsecs, PERFSTATS_TELAPSED(pstStart));
  return mr;
}


static
inline
mem_region_t* mreg_for_remote_addr(void* addr, c_nodeid_t locale)
{
  static __thread mem_region_t** mrs;
  mem_region_t* mr;
  if (mrs == NULL) {
    mrs = (mem_region_t**) chpl_mem_allocManyZero(chpl_numNodes,
                                                  sizeof(mrs[0]),
                                                  CHPL_RT_MD_COMM_PER_LOC_INFO,
                                                  0, 0);
  }
  PERFSTATS_INC(remote_mreg_cnt);
  PERFSTATS_TSTAMP(pstStart);
  if ((mr = mrs[locale]) == NULL
      || (uint64_t) addr < mr->addr
      || (uint64_t) addr >= mr->addr + mrtl_len(mr->len)
      || !mrtl_isReg(mr->len)) {
    mr = mrs[locale] = mreg_for_addr(addr, mem_regions_all_entries[locale]);
    PERFSTATS_ADD(remote_mreg_cmps,
                  ((mr == NULL)
                   ? mem_regions_all_entries[locale]->mreg_cnt
                   : (mr - &mem_regions_all_entries[locale]->mregs[0] + 1)));
  }
  PERFSTATS_ADD(remote_mreg_nsecs, PERFSTATS_TELAPSED(pstStart));
  return mr;
}


static
void polling_task(void* ignore)
{
  set_up_for_polling();

  polling_task_running = true;

  while (!polling_task_please_exit) {
    gni_cq_entry_t ev;
    gni_return_t   gni_rc;

    //
    // Process CQ events due to PUT data arriving in the remote-fork
    // request memory.
    if (polling_task_blocking_cq)
      gni_rc = GNI_CqWaitEvent(rf_cqh, 100, &ev);
    else
      gni_rc = GNI_CqGetEvent(rf_cqh, &ev);

    if (gni_rc == GNI_RC_SUCCESS)
      rf_handler(&ev);
    else if (gni_rc == GNI_RC_NOT_DONE)
      sched_yield();
    else if (gni_rc == GNI_RC_TIMEOUT)
      ; // no-op
    else
      GNI_CQ_EV_FAIL(gni_rc, ev, "GNI_Cq*Event(rf_cqh)");

    //
    // Process CQ events due to our request responses completing.
    //
    consume_all_outstanding_cq_events(cd_idx);
  }

  polling_task_done = true;
}


static
void set_up_for_polling(void)
{
  cq_cnt_t cq_cnt;
  uint32_t i;

  //
  // Grab a communication domain permanently.
  //
  acquire_comm_dom();
  cd->firmly_bound = true;

  //
  // Make the fork request and acknowledgement space, and communicate
  // the location of that space across the locales.
  //

  // Must be directly communicable without proxy.
  fork_reqs =
    (fork_t*) chpl_comm_mem_reg_allocMany(FORK_REQ_BUFS_PER_LOCALE,
                                          sizeof(fork_reqs[0]),
                                          CHPL_RT_MD_COMM_FRK_RCV_INFO,
                                          0, 0);

  fork_reqs_map =
    (fork_t**) chpl_mem_allocMany(chpl_numNodes, sizeof(fork_reqs_map[0]),
                                  CHPL_RT_MD_COMM_PER_LOC_INFO,
                                  0, 0);

  // Must be directly communicable without proxy.
  fork_reqs_free =
    (chpl_bool32*) chpl_comm_mem_reg_allocMany(FORK_REQ_BUFS_PER_LOCALE,
                                               sizeof(fork_reqs_free[0]),
                                               CHPL_RT_MD_COMM_FRK_DONE_FLAG,
                                               0, 0);
  for (uint32_t i = 0; i < FORK_REQ_BUFS_PER_LOCALE; i++)
    fork_reqs_free[i] = true;

  fork_reqs_free_map =
    (chpl_bool32**) chpl_mem_allocMany(chpl_numNodes,
                                       sizeof(fork_reqs_free_map[0]),
                                       CHPL_RT_MD_COMM_PER_LOC_INFO,
                                       0, 0);

  {
    typedef struct {
      c_nodeid_t   nodeID;
      fork_t*      fork_reqs;
      chpl_bool32* fork_reqs_free;
    } gdata_t;

    gdata_t  my_gdata = { chpl_nodeID,
                          fork_reqs, (chpl_bool32*) fork_reqs_free };
    gdata_t* gdata;

    gdata = (gdata_t*) chpl_mem_allocMany(chpl_numNodes, sizeof(gdata[0]),
                                          CHPL_RT_MD_COMM_PER_LOC_INFO,
                                          0, 0);
    if (PMI_Allgather(&my_gdata, gdata, sizeof(gdata[0])) != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Allgather(fork_reqs_map) failed");

    for (i = 0; i < chpl_numNodes; i++) {
      fork_reqs_map[gdata[i].nodeID]      = gdata[i].fork_reqs;
      fork_reqs_free_map[gdata[i].nodeID] = gdata[i].fork_reqs_free;
    }

    chpl_mem_free(gdata, 0, 0);
  }

  //
  // Create remote fork completion queue.
  //
    {
    uint32_t cq_mode = GNI_CQ_NOBLOCK;

    polling_task_blocking_cq = chpl_env_rt_get_bool("COMM_UGNI_BLOCKING_CQ",
                                                    true);

    if (polling_task_blocking_cq)
      cq_mode = GNI_CQ_BLOCKING;

    cq_cnt = FORK_REQ_BUFS_PER_LOCALE;
    GNI_CHECK(GNI_CqCreate(cd->nih, cq_cnt, 0, cq_mode, NULL, NULL, &rf_cqh));
  }

  //
  // Register the fork request memory.
  //
  {
    uint64_t addr  = (uint64_t) (intptr_t) fork_reqs;
    uint64_t len   = (uint64_t)
                     (FORK_REQ_BUFS_PER_LOCALE * sizeof(fork_reqs[0]));
    uint32_t flags = GNI_MEM_READWRITE | GNI_MEM_RELAXED_PI_ORDERING;

    DBG_P_L(DBGF_MEMREG,
            "RemFork space: GNI_MemRegister(%#" PRIx64 ", %#" PRIx64")",
            addr, len);
    GNI_CHECK(GNI_MemRegister(cd->nih, addr, len, rf_cqh, flags, -1, &rf_mdh));
  }

  //
  // Share the per-locale fork request memory descriptors around the
  // job.
  //
  rf_mdh_map =
    (gni_mem_handle_t*) chpl_mem_allocMany(chpl_numNodes,
                                           sizeof(rf_mdh_map[0]),
                                           CHPL_RT_MD_COMM_PER_LOC_INFO,
                                           0, 0);

  {
    typedef struct {
      c_nodeid_t       nodeID;
      gni_mem_handle_t rf_mdh;
    } gdata_t;

    gdata_t  my_gdata = { chpl_nodeID, rf_mdh };
    gdata_t* gdata;

    gdata = (gdata_t*) chpl_mem_allocMany(chpl_numNodes, sizeof(gdata[0]),
                                          CHPL_RT_MD_COMM_PER_LOC_INFO,
                                          0, 0);
    if (PMI_Allgather(&my_gdata, gdata, sizeof(gdata[0])) != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Allgather(rf_mdh_map) failed");

    for (i = 0; i < chpl_numNodes; i++)
      rf_mdh_map[gdata[i].nodeID] = gdata[i].rf_mdh;

    chpl_mem_free(gdata, 0, 0);
  }
}


void chpl_comm_rollcall(void)
{
  // Initialize diags
  chpl_comm_diags_init();

  chpl_msg(2, "executing on node %d of %d node(s): %s\n", chpl_nodeID,
           chpl_numNodes, chpl_nodeName());

  if (chpl_numNodes == 1)
    return;
}


static
void ensure_registered_heap_info_set(void)
{
  if (!registered_heap_info_set
      && pthread_once(&registered_heap_once_flag, make_registered_heap) != 0) {
    CHPL_INTERNAL_ERROR("pthread_once(&registered_heap_once_flag) failed");
  }
}


static
void make_registered_heap(void)
{
  size_t page_size = get_hugepage_size();
  size_t size_from_env;

  if (page_size == 0
      || getenv("HUGETLB_MORECORE") == NULL
      || (size_from_env = chpl_comm_getenvMaxHeapSize()) == 0) {
    registered_heap_size  = 0;
    registered_heap_start = NULL;
    registered_heap_info_set = 1;
    chpl_atomic_thread_fence(memory_order_release);
    return;
  }

  //
  // The heap is supposed to be of fixed size and on hugepages.  Set
  // it up.  (If it's to be dynamically extensible, that's handled
  // separately through chpl_comm_impl_regMem*().)
  //
  size_t nic_mem_map_limit;
  size_t nic_max_mem;
  size_t max_heap_size;
  size_t size;
  size_t decrement;
  void*  start;

  //
  // Considering the data size we'll register, compute the maximum
  // heap size that will allow all registrations to fit in the NIC
  // TLB.  Except on Gemini only, aim for only 95% of what will fit
  // because there we'll get an error if we go over.
  //
  if (nic_type == GNI_DEVICE_GEMINI) {
    const size_t nic_max_pages = (size_t) 1 << 14; // not publicly defined
    nic_max_mem = nic_max_pages * page_size;
    nic_mem_map_limit = ALIGN_DN((size_t) (0.95 * nic_max_mem), page_size);
  } else {
    const size_t nic_TLB_cache_pages = 512; // not publicly defined
    nic_max_mem = nic_TLB_cache_pages * page_size;
    nic_mem_map_limit = nic_max_mem;
  }

  {
    uint64_t  addr;
    uint64_t  len;
    size_t    data_size;

    data_size = 0;
    while (get_next_rw_memory_range(&addr, &len, NULL, 0))
      data_size += ALIGN_UP(len, page_size);

    if (data_size >= nic_mem_map_limit)
      max_heap_size = 0;
    else
      max_heap_size = nic_mem_map_limit - data_size;
  }

  //
  // As a hedge against silliness, first reduce any request so that it's
  // no larger than the physical memory.  As a beneficial side effect
  // when the user request is ridiculously large, this also causes the
  // reduce-by-5% loop below to run faster and produce a final size
  // closer to the maximum available.
  //
  size = size_from_env;
  const size_t size_phys = ALIGN_DN(chpl_sys_physicalMemoryBytes(), page_size);
  if (size > size_phys)
    size = size_phys;
  
  //
  // On Gemini-based systems, if necessary reduce the heap size until
  // we can fit all the registered pages in the NIC TLB.  Otherwise,
  // we'll get GNI_RC_ERROR_RESOURCE when we try to register memory.
  // Warn about doing this.
  //
  if (nic_type == GNI_DEVICE_GEMINI && size > max_heap_size) {
    if (chpl_nodeID == 0) {
      char buf1[20], buf2[20], buf3[20], msg[200];
      chpl_snprintf_KMG_z(buf1, sizeof(buf1), nic_max_mem);
      chpl_snprintf_KMG_z(buf2, sizeof(buf2), page_size);
      chpl_snprintf_KMG_f(buf3, sizeof(buf3), max_heap_size);
      (void) snprintf(msg, sizeof(msg),
                      "Gemini TLB can cover %s with %s pages; heap "
                      "reduced to %s to fit",
                      buf1, buf2, buf3);
      chpl_warning(msg, 0, 0);
    }

    if (nic_type == GNI_DEVICE_GEMINI)
      size = max_heap_size;
  }

  //
  // Work our way down from the starting size in (roughly) 5% steps
  // until we can actually get that much from the system.
  //
  size = ALIGN_DN(size, page_size);
  if ((decrement = ALIGN_DN((size_t) (0.05 * size), page_size)) < page_size) {
    decrement = page_size;
  }

  size += decrement;
  do {
    size -= decrement;

    start = get_huge_pages(size, GHP_DEFAULT);

    DBG_P_LP(DBGF_HUGEPAGES, "HUGEPAGES get_huge_pages(%#zx) returned %p",
            size, start);
  } while (start == NULL && size > decrement);

  if (start == NULL)
    chpl_error("cannot initialize heap: cannot get hugepage space", 0, 0);

  DBG_P_LP(DBGF_HUGEPAGES, "HUGEPAGES allocated heap start=%p size=%#zx\n",
           start, size);

  //
  // On Aries-based systems, warn if the size is larger than what will
  // fit in the TLB cache.  But since that may reduce performance but
  // won't affect function, don't reduce the size to fit.
  //
  if (nic_type == GNI_DEVICE_ARIES && size > max_heap_size) {
    if (chpl_nodeID == 0) {
      char buf1[20], buf2[20], buf3[20], msg[200];
      chpl_snprintf_KMG_z(buf1, sizeof(buf1), nic_max_mem);
      chpl_snprintf_KMG_z(buf2, sizeof(buf2), page_size);
      chpl_snprintf_KMG_f(buf3, sizeof(buf3), size);
      (void) snprintf(msg, sizeof(msg),
                      "Aries TLB cache can cover %s with %s pages; "
                      "with %s heap,\n"
                      "         cache refills may reduce performance",
                      buf1, buf2, buf3);
      chpl_warning(msg, 0, 0);
    }
  }

  registered_heap_size  = size;
  registered_heap_start = start;
  registered_heap_info_set = 1;
  chpl_atomic_thread_fence(memory_order_release);
}


static
size_t get_hugepage_size(void)
{
  if (!hugepage_info_set
      && pthread_once(&hugepage_once_flag, set_hugepage_info) != 0) {
    CHPL_INTERNAL_ERROR("pthread_once(&hugepage_once_flag) failed");
  }

  return hugepage_size;
}


static
void set_hugepage_info(void)
{
  if (chpl_numNodes > 1
      && getenv("HUGETLB_DEFAULT_PAGE_SIZE") != NULL) {
    hugepage_size = gethugepagesize();
  }

  //
  // With HUGETLB_NO_RESERVE=yes, we want to report out-of-memory in
  // response to SIGBUS signals resulting from (as best we can tell)
  // being unable to acquire hugepages at fault-in time.
  //
  {
    char const *ev;

    if ((ev = getenv("HUGETLB_NO_RESERVE")) != NULL
        && strcasecmp(ev, "yes") == 0) {
      atomic_init_bool(&SIGBUS_gate, false);
      install_SIGBUS_handler();
    }
  }

  hugepage_info_set = 1;
  chpl_atomic_thread_fence(memory_order_release);

  DBG_P_L(DBGF_HUGEPAGES,
          "setting hugepage info: use hugepages %s, sz %#zx",
          (hugepage_size > 0) ? "YES" : "NO", hugepage_size);
}


//
// This may be called from the SIGBUS handler itself (see below), so
// its error handling has to be primitive (no *printf() calls, e.g.).
//
static
void install_SIGBUS_handler(void)
{
  struct sigaction act;

  act.sa_flags = SA_RESTART | SA_SIGINFO;
  if (sigemptyset(&act.sa_mask) != 0) {
    static const char* msg = "internal error: sigemptyset() failed\n";
    int msgLen = strlen(msg);
    write(2, msg, msgLen);
    _exit(1);
  }
  act.sa_sigaction = SIGBUS_handler;
  if (sigaction(SIGBUS, &act, &previous_SIGBUS_sigact) != 0) {
    static const char* msg = "internal error: sigaction(SIGBUS) failed\n";
    int msgLen = strlen(msg);
    write(2, msg, msgLen);
    _exit(1);
  }
}


static
void SIGBUS_handler(int signo, siginfo_t *info, void *context)
{
  //
  // See if this was within an as-yet-unregistered region where we may
  // be faulting in pages as a side effect of initialization.  If so,
  // report allocation failure and exit.  Note that we're not holding
  // the dynamic registration mutex and thus mem_regions->mreg_cnt may
  // vary while we're executing this loop.  However, if the SIGBUS is
  // the result of a fault-in failure on some region in the table, that
  // count won't drop below the index of that region, because we can't
  // remove the region until after it's registered and we can't finish
  // registering it without faulting in all its pages.
  // 
  // Also note that although we replicate the format of the "official"
  // message chpl_memhook_check_post() produces, we can't just call it
  // directly or use the same functions as it does, because they're not
  // signal-safe.
  //
  if (info->si_code == BUS_ADRERR) {
    uint64_t addr = (uint64_t) (intptr_t) info->si_addr;

    int mr_i;
    mem_region_t* mr;

    for (mr_i = mem_regions->mreg_cnt - 1, mr = &mem_regions->mregs[mr_i];
         mr_i >= 0 && (addr < mr->addr
                       || addr >= mr->addr + mrtl_len(mr->len)
                       || mrtl_isReg(mr->len));
         mr_i--, mr--)
      ;

    if (mr_i >= 0) {
      //
      // This does indeed appear to be due to faulting in pages as a
      // side effect of initialization.
      //

      //
      // Only say this once per node.
      //
      while (atomic_exchange_explicit_bool(&SIGBUS_gate, true,
                                           memory_order_acquire)) {
        sleep(1); // listed as safe in a handler (sched_yield() isn't)
      }

      //
      // Produce the best message we can, under the circumstances.
      //
      static char buf[1000];
      int bufi;

      static const char* msg1 = "error: Out of memory allocating \"";
      int msg1Len = strlen(msg1);
      const char* mds = chpl_mem_descString(mr_mregs_supplement[mr_i].desc);
      int mdsLen = strlen(mds);
      static const char* msg2 = "\"";
      int msg2Len = strlen(msg2);

      bufi = sizeof(buf);
      buf[--bufi] = '\n';

      if (msg1Len + mdsLen + msg2Len > bufi) {
        static const char* msg = "error: Out of memory allocating from heap";
        int msgLen = strlen(msg);
        memcpy(&buf[bufi -= msgLen], msg, msgLen);
      } else {
        //
        // We build the message backwards, because that makes building
        // the string for the line number much simpler.
        //
        memcpy(&buf[bufi -= msg2Len], msg2, msg2Len);
        memcpy(&buf[bufi -= mdsLen], mds, mdsLen);
        memcpy(&buf[bufi -= msg1Len], msg1, msg1Len);

        //
        // Only try to provide a line number if there is one and it
        // will fit in the buffer.
        //
        chpl_bool didLine = false;
        if (bufi > 22 // type safety: enough for uint64 (ln is only int32)
            && mr_mregs_supplement[mr_i].ln > 0) {
          int ln = mr_mregs_supplement[mr_i].ln;
          buf[--bufi] = ' ';
          buf[--bufi] = ':';
          for ( ; ln > 0; ln /= 10) {
            buf[--bufi] = "0123456789"[ln % 10];
          }
          didLine = true;
        }

        //
        // Only try to provide a source file if we can give at least some
        // of it, and without using snprintf() in chpl_lookupFilename()).
        //
        if (bufi > 10
            && mr_mregs_supplement[mr_i].fn >= 0
            && mr_mregs_supplement[mr_i].fn < chpl_filenameTableSize) {
          if (!didLine)
            buf[--bufi] = ' ';
          buf[--bufi] = ':';

          //
          // Stop copying fname either when we're done or need to leave
          // room for "..." in place of chars that won't fit.
          //
          const char* fn = chpl_lookupFilename(mr_mregs_supplement[mr_i].fn);
          int fnLen = strlen(fn);
          int fni;
          for (fni = fnLen; fni > 0 && (fni <= 3 || bufi > 3); ) {
            buf[--bufi] = fn[--fni];
          }

          if (fni != 0) {
            buf[--bufi] = '.';
            buf[--bufi] = '.';
            buf[--bufi] = '.';
          }
        }
      }

      write(2, &buf[bufi], sizeof(buf) - bufi);

      //
      // It's not safe to do anything else but quit now.
      //
      _exit(1);
    }
  }

  //
  // This was not due to allocation failure on a hugepage we were trying
  // to fault in.  Restore the previous handler and re-raise the SIGBUS.
  // It should get dealt with as it would have without our handler.  If
  // the old handler returns and execution continues, install our new
  // handler again.
  //
  if (sigaction(SIGBUS, &previous_SIGBUS_sigact, NULL) != 0) {
    static const char* msg
                       = "internal error: "
                         "sigaction(SIGBUS) to reinstall old handler failed\n";
    int msgLen = strlen(msg);
    write(2, msg, msgLen);
    _exit(1);
  }

  {
    sigset_t sigbus_set;

    if (sigemptyset(&sigbus_set) != 0
        || sigaddset(&sigbus_set, SIGBUS) != 0
        || pthread_sigmask(SIG_UNBLOCK, &sigbus_set, NULL) != 0
        || raise(SIGBUS) != 0
        || pthread_sigmask(SIG_BLOCK, &sigbus_set, NULL) != 0) {
      static const char* msg = "internal error: cannot re-raise SIGBUS\n";
      int msgLen = strlen(msg);
      write(2, msg, msgLen);
      _exit(1);
    }
  }

  install_SIGBUS_handler();
}


size_t chpl_comm_impl_regMemHeapPageSize(void)
{
  size_t sz;
  if ((sz = get_hugepage_size()) > 0)
    return sz;
  return chpl_getSysPageSize();
}


void chpl_comm_impl_regMemHeapInfo(void** start_p, size_t* size_p)
{
  ensure_registered_heap_info_set();
  *start_p = registered_heap_start;
  *size_p  = registered_heap_size;
}


inline
size_t chpl_comm_impl_regMemAllocThreshold(void)
{
  size_t sz;
  if ((sz = get_hugepage_size()) > 0)
    return sz;
  return SIZE_MAX;
}


#ifdef PERFSTATS_TIME_ZERO_INIT
static __thread uint64_t defaultInit_ts;
#endif

static pthread_once_t warned_out_of_mem_regions = PTHREAD_ONCE_INIT;
static void issue_out_of_mem_regions_warning(void)
{
  char buf[200];
  snprintf(buf, sizeof(buf),
           "no more registered memory region table entries (max is %d).\n"
           "         Change using CHPL_RT_COMM_UGNI_MAX_MEM_REGIONS.",
           max_mem_regions);
  chpl_warning(buf, 0, 0);
}

void* chpl_comm_impl_regMemAlloc(size_t size,
                                 chpl_mem_descInt_t desc, int ln, int32_t fn)
{
  int mr_i;
  mem_region_t* mr;
  void* p;

  if (get_hugepage_size() == 0)
    return NULL;

  PERFSTATS_INC(regMemAlloc_cnt);

  //
  // Do we have room for another registered memory region?
  //
  if (atomic_fetch_sub_int_least32_t(&mreg_free_cnt, 1) < 1) {
    atomic_fetch_add_int_least32_t(&mreg_free_cnt, 1);

    (void)pthread_once(&warned_out_of_mem_regions, issue_out_of_mem_regions_warning);

    DBG_P_LP(DBGF_MEMREG, "chpl_regMemAlloc(%#zx): out of table entries", size);
    return NULL;
  }

  //
  // Try to get the memory.
  //
  PERFSTATS_TSTAMP(alloc_ts);
  p = get_huge_pages(ALIGN_UP(size, get_hugepage_size()), GHP_DEFAULT);
  PERFSTATS_ADD(regMem_alloc_nsecs, PERFSTATS_TELAPSED(alloc_ts));
  if (p == NULL) {
    atomic_fetch_add_int_least32_t(&mreg_free_cnt, 1);

    DBG_P_LP(DBGF_MEMREG, "chpl_regMemAlloc(%#zx): no hugepages", size);
    return NULL;
  }

  regMemLock();

  //
  // Find an entry to use and fill it in.  There must be one available,
  // because we didn't underflow the free-entry-count check above.
  //
  for (mr_i = 0;
       mr_i < max_mem_regions && mem_regions->mregs[mr_i].addr != 0;
       mr_i++)
    ;

  assert(mr_i < max_mem_regions);

  mr = &mem_regions->mregs[mr_i];
  mr->addr = (uint64_t) (uintptr_t) p;
  mr->len = mrtl_encode(size, false);

  mr_mregs_supplement[mr_i].desc = desc;
  mr_mregs_supplement[mr_i].ln = ln;
  mr_mregs_supplement[mr_i].fn = fn;

  //
  // Adjust the region count, if necessary.
  //
  if (mr_i >= mem_regions->mreg_cnt) {
    mem_regions->mreg_cnt = mr_i + 1;
    if (mem_regions->mreg_cnt > mreg_cnt_max)
      mreg_cnt_max = mem_regions->mreg_cnt;
  }

  regMemUnlock();

  DBG_P_LP(DBGF_MEMREG,
           "chpl_regMemAlloc(%#" PRIx64 "): "
           "mregs[%d] = %#" PRIx64 ", cnt %d",
           size, mr_i, mr->addr, (int) mem_regions->mreg_cnt);

#ifdef PERFSTATS_TIME_ZERO_INIT
  const size_t pgSize = get_hugepage_size();
  PERFSTATS_TSTAMP(kpage_ts);
  for (int i = 0; i < size; i += pgSize) {
    ((char*) p)[i] = 0;
  }
  PERFSTATS_TGET(defaultInit_ts);
  PERFSTATS_ADD(regMem_kpage_nsecs, PERFSTATS_TDIFF(defaultInit_ts, kpage_ts));
#endif

  return p;
}


void chpl_comm_impl_regMemPostAlloc(void* p, size_t size)
{
  mem_region_t* mr;
  int mr_i;

#ifdef PERFSTATS_TIME_ZERO_INIT
  PERFSTATS_ADD(regMem_defaultInit_nsecs, PERFSTATS_TELAPSED(defaultInit_ts));
#endif

  PERFSTATS_INC(regMemPostAlloc_cnt);

  if (get_hugepage_size() == 0)
    CHPL_INTERNAL_ERROR("chpl_comm_impl_regMemPostAlloc(): "
                        "this isn't my memory");

  DBG_P_LP(DBGF_MEMREG,
           "chpl_comm_impl_regMemPostAlloc(%p, %#" PRIx64 ")",
           p, size);

  //
  // Find the memory region table entry for this memory.
  //
  for (mr_i = mem_regions->mreg_cnt - 1, mr = &mem_regions->mregs[mr_i];
       mr_i >= 0 && (mr->addr != (uint64_t) p || mrtl_len(mr->len) != size);
       mr_i--, mr--)
    ;

  if (mr_i < 0)
    CHPL_INTERNAL_ERROR("chpl_comm_impl_regMemPostAlloc(): "
                        "can't find the memory");

  assert(!mrtl_isReg(mr->len));

  //
  // If we're in early setup and can't register memory yet then we're
  // done.  This entry will get registered and broadcast along with
  // the static entries, when we get to that.  Otherwise, register it.
  //
  if (!can_register_memory)
    return;

  PERFSTATS_TSTAMP(reg_ts);
  (void) register_mem_region(mr->addr, mrtl_len(mr->len), &mr->mdh,
                             false /*allow_failure*/);
  mrtl_setReg(&mr->len);
  PERFSTATS_ADD(regMem_reg_nsecs, PERFSTATS_TELAPSED(reg_ts));

  regMemLock();

  //
  // Update the copies of our memory regions on all nodes.  If this
  // entry is within the already-known range of our table entries then
  // send only this one.  Otherwise, send both all the entries beyond
  // the already-known range and the new count.  In general this will
  // include some new unregistered entries for which we have an address
  // but no length or MDH.  But the presence of those won't create
  // confusion because the remote nodes won't by trying to address
  // within them yet anyway, and later when we do register them we may
  // be able to send just the entries and not the count again.
  //
  if (mr_i < mem_regions_all_entries[chpl_nodeID]->mreg_cnt) {
    DBG_P_L(DBGF_MEMREG_BCAST,
            "chpl_comm_impl_regMemPostAlloc(): entry %d, bcast",
            mr_i);
    PERFSTATS_INC(regMem_bCast_cnt);
    regMemBroadcast(mr_i, 1, false /*send_mreg_cnt*/);
  } else {
    const uint32_t mreg_cnt_public =
                     mem_regions_all_entries[chpl_nodeID]->mreg_cnt;

    DBG_P_L(DBGF_MEMREG_BCAST,
            "chpl_comm_impl_regMemPostAlloc(): "
            "entry %d, bcast %d-%d and cnt %d",
            mr_i,
            (int) mreg_cnt_public, (int) mem_regions->mreg_cnt - 1,
            (int) mem_regions->mreg_cnt);
    PERFSTATS_INC(regMem_bCast_cnt);
    regMemBroadcast(mreg_cnt_public, mem_regions->mreg_cnt - mreg_cnt_public,
                    true /*send_mreg_cnt*/);
  }

  regMemUnlock();
}


chpl_bool chpl_comm_impl_regMemFree(void* p, size_t size)
{
  mem_region_t* mr;
  int mr_i;

  if (get_hugepage_size() == 0)
    return false;

  //
  // Is this memory in our table?
  //
  for (mr_i = mem_regions->mreg_cnt - 1, mr = &mem_regions->mregs[mr_i];
       mr_i >= 0 && (mr->addr != (uint64_t) p || mrtl_len(mr->len) != size);
       mr_i--, mr--)
    ;

  if (mr_i < 0)
    return false;

  assert(mrtl_isReg(mr->len));

  PERFSTATS_INC(regMemFree_cnt);

  DBG_P_LP(DBGF_MEMREG,
           "chpl_comm_impl_regMemFree(%p, %#" PRIx64 "): [%d]",
           p, size, mr_i);

  //
  // Deregister the memory and empty the entry in our table.  Note that
  // even with single-threading we can't compress the table or do
  // anything else that would move entries around, because other threads
  // may be doing lookups in it and they aren't locked out.  Also, we
  // have to finish broadcasting the update before we unlock, because as
  // soon as we unlock, the entry could be reused.
  //
  PERFSTATS_TSTAMP(dereg_ts);
  deregister_mem_region(mr);
  PERFSTATS_ADD(regMem_dereg_nsecs, PERFSTATS_TELAPSED(dereg_ts));

  regMemLock();

  mr->addr = 0;
  mr->len = 0;
  mr->mdh = (gni_mem_handle_t) { 0 };

  //
  // Adjust the memory region count downward, if this was the last
  // active region.
  //
  if (mr_i == mem_regions->mreg_cnt - 1) {
    int j;
    for (j = mr_i - 1; j >= 0 && mem_regions->mregs[j].addr == 0; j--)
      ;
    assert(j >= 0);
    mem_regions->mreg_cnt = j + 1;
  }

  //
  // Update the copies of our memory regions on all nodes.  If removing
  // this entry shortened our table then we just send the new count.
  // Note that this may leave remnant (and misleading) non-0 values in
  // table entries past the new end of the active area in remote nodes'
  // copies of our region table.  This is okay as long as these entries
  // always get overwritten if the table grows past them again, as is
  // the case now.  If removing the entry didn't change the length of
  // the table then we can just send the now-empty entry.
  //
  if (mem_regions->mreg_cnt < mem_regions_all_entries[chpl_nodeID]->mreg_cnt) {
    DBG_P_L(DBGF_MEMREG_BCAST,
            "chpl_comm_impl_regMemFree(): entry %d, bcast cnt %d",
            mr_i, (int) mem_regions->mreg_cnt);
    PERFSTATS_INC(regMem_bCast_cnt);
    regMemBroadcast(0, 0, true /*send_mreg_cnt*/);
  } else {
    DBG_P_L(DBGF_MEMREG_BCAST,
            "chpl_comm_impl_regMemFree(): entry %d, bcast",
            mr_i);
    PERFSTATS_INC(regMem_bCast_cnt);
    regMemBroadcast(mr_i, 1, false /*send_mreg_cnt*/);
  }

  regMemUnlock();

  atomic_fetch_add_int_least32_t(&mreg_free_cnt, 1);

  PERFSTATS_TSTAMP(free_ts);
  free_huge_pages(p);
  PERFSTATS_ADD(regMem_free_nsecs, PERFSTATS_TELAPSED(free_ts));

  return true;
}


#ifdef PERFSTATS_COMM_UGNI
static _PSV_C_TYPE critsec_ts;
#endif

static inline
void regMemLock(void) {
  PERFSTATS_TSTAMP(pstStart);
  if (pthread_mutex_lock(&mem_regions_mutex) != 0)
    CHPL_INTERNAL_ERROR("cannot acquire mem region lock");
  PERFSTATS_TGET(critsec_ts);
  PERFSTATS_ADD(regMem_lock_nsecs, PERFSTATS_TDIFF(critsec_ts, pstStart));
  PERFSTATS_INC(regMem_locks);
  allow_task_yield = false;
}


static inline
void regMemUnlock(void) {
  PERFSTATS_ADD(regMem_critsec_nsecs, PERFSTATS_TELAPSED(critsec_ts));
  if (pthread_mutex_unlock(&mem_regions_mutex) != 0)
    CHPL_INTERNAL_ERROR("cannot release mem region lock");
  allow_task_yield = true;
}


static inline
void regMemBroadcast(int mr_i, int mr_cnt, chpl_bool send_mreg_cnt)
{
  void* src_v[MAX_CHAINED_PUT_LEN];
  int32_t node_v[MAX_CHAINED_PUT_LEN];
  void* tgt_v[MAX_CHAINED_PUT_LEN];
  size_t size_v[MAX_CHAINED_PUT_LEN];
  mem_region_t* remote_mr_v[MAX_CHAINED_PUT_LEN];
  const int vi_limit = (mr_cnt > 0 && send_mreg_cnt)
                       ? (MAX_CHAINED_PUT_LEN - 1)  // using 2 V elems per node
                       : MAX_CHAINED_PUT_LEN;       // using 1 V elems per node
  int vi;

  vi = 0;
  for (int ni = 0; ni < (int) chpl_numNodes; ni++) {
    if (ni == chpl_nodeID) {
      //
      // Update our own map in place.
      //
      if (mr_cnt > 0) {
        memcpy((char*) &mem_regions_all_my_entry_map[ni]->mregs[mr_i],
               (char*) &mem_regions->mregs[mr_i],
               mr_cnt * sizeof(mem_region_t));
      }

      if (send_mreg_cnt) {
        mem_regions_all_my_entry_map[ni]->mreg_cnt = mem_regions->mreg_cnt;
      }
    } else {
      //
      // Update every other node's map remotely.
      //
      if (vi >= vi_limit) {
        do_remote_put_V(vi, src_v, node_v, tgt_v, size_v, remote_mr_v,
                        may_proxy_false);
        vi = 0;
      }

      if (mr_cnt > 0) {
        src_v[vi] = (char*) &mem_regions->mregs[mr_i];
        node_v[vi] = ni;
        tgt_v[vi] = (char*) &mem_regions_all_my_entry_map[ni]->mregs[mr_i];
        size_v[vi] = mr_cnt * sizeof(mem_region_t);
        remote_mr_v[vi] = &gnr_mreg_map[ni];
        vi++;
      }

      if (send_mreg_cnt) {
        src_v[vi] = (char*) &mem_regions->mreg_cnt;
        node_v[vi] = ni;
        tgt_v[vi] = (char*) &mem_regions_all_my_entry_map[ni]->mreg_cnt;
        size_v[vi] = sizeof(mem_regions->mreg_cnt);
        remote_mr_v[vi] = &gnr_mreg_map[ni];
        vi++;
      }
    }
  }

  if (vi > 0) {
    do_remote_put_V(vi, src_v, node_v, tgt_v, size_v, remote_mr_v,
                    may_proxy_false);
  }
}


wide_ptr_t* chpl_comm_broadcast_global_vars_helper() {
  //
  // Gather the global variables' wide pointers on node 0 into a
  // buffer, and broadcast the address of that buffer to the other
  // nodes.
  //
  wide_ptr_t* buf;
  if (chpl_nodeID == 0) {
    buf = (wide_ptr_t*) chpl_mem_allocMany(chpl_numGlobalsOnHeap, sizeof(*buf),
                                           CHPL_RT_MD_COMM_PER_LOC_INFO, 0, 0);
    for (int i = 0; i < chpl_numGlobalsOnHeap; i++) {
      buf[i] = *chpl_globals_registry[i];
    }
  }
  PMI_Bcast(&buf, sizeof(buf));
  return buf;
}


void chpl_comm_broadcast_private(int id, size_t size)
{
  int i;

  DBG_P_LP(DBGF_IFACE, "IFACE chpl_comm_broadcast_private(%d, %zd)", id, size);

  //
  // TODO: Currently this does a PUT and wait for remote completion to
  //       each remote locale.  A performance improvement would be to
  //       do all, or a batch of, the PUTs and only then wait for the
  //       associated CQ events.
  //
  for (i = 0; i < chpl_numNodes; i++) {
    if (i != chpl_nodeID) {
      do_remote_put(chpl_rt_priv_bcast_tab[id], i,
                    chpl_rt_priv_bcast_tab[id], size,
                    NULL, may_proxy_true);
    }
  }
}


void chpl_comm_barrier(const char *msg)
{
  DBG_P_L(DBGF_IFACE, "IFACE chpl_comm_barrier(\"%s\")", msg);

#ifdef CHPL_COMM_DEBUG
  chpl_msg(2, "%d: enter barrier for '%s'\n", chpl_nodeID, msg);
#endif

  if (chpl_numNodes == 1)
    return;

  //
  // If we can't communicate yet, just do a PMI barrier.
  //
  if (!polling_task_running) {
    if (PMI_Barrier() != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Barrier() failed");
    return;
  }

  //
  // Wait for our child locales to notify us that they have reached the
  // barrier.
  //
  DBG_P_LP(DBGF_BARRIER, "BAR wait for %d children", (int) bar_num_children);
  for (uint32_t i = 0; i < bar_num_children; i++) {
    while (bar_info.child_notify[i] == 0) {
      PERFSTATS_INC(lyield_in_bar_1_cnt);
      local_yield();
    }
  }

  if (chpl_nodeID != 0) {
    //
    // Notify our parent locale that we have reached the barrier.
    //
    static uint32_t bar_flag = 1;
    uint32_t child_in_parent = (chpl_nodeID - 1) % BAR_TREE_NUM_CHILDREN;

    DBG_P_LP(DBGF_BARRIER, "BAR notify parent %d", (int) bar_parent);
    do_remote_put(&bar_flag, bar_parent,
                  (void*) &parent_bar_info->child_notify[child_in_parent],
                  sizeof(bar_flag), NULL, may_proxy_true);

    //
    // Wait for our parent locale to release us from the barrier.
    //
    DBG_P_LP(DBGF_BARRIER, "BAR wait for parental release");
    while (bar_info.parent_release == 0) {
      PERFSTATS_INC(lyield_in_bar_2_cnt);
      local_yield();
    }
  }

  //
  // Clear all our barrier flags.
  //
  for (uint32_t i = 0; i < bar_num_children; i++)
    bar_info.child_notify[i] = 0;
  bar_info.parent_release = 0;

  //
  // Release our children.
  //
  if (bar_num_children > 0) {
    void* src_v[MAX_CHAINED_PUT_LEN];
    int32_t node_v[MAX_CHAINED_PUT_LEN];
    void* tgt_v[MAX_CHAINED_PUT_LEN];
    size_t size_v[MAX_CHAINED_PUT_LEN];

    // TODO handle this case instead of forcing children < MAX_CHAINED_PUT_LEN
    assert(bar_num_children < MAX_CHAINED_PUT_LEN);

    DBG_P_LP(DBGF_BARRIER, "BAR release %d children", (int) bar_num_children);
    for (uint32_t i = 0; i < bar_num_children; i++) {
      static uint32_t release_flag = 1;
      src_v[i] = &release_flag;
      node_v[i] = bar_min_child + i;
      tgt_v[i] = (void*) &child_bar_info[i]->parent_release;
      size_v[i] = sizeof(release_flag);
    }
    do_remote_put_V(bar_num_children, src_v, node_v, tgt_v, size_v, NULL,
                    may_proxy_true);
  }
}


void chpl_comm_pre_task_exit(int all)
{
  if (exit_without_cleanup)
    return;

  if (chpl_numNodes == 1)
    return;

  if (all) {
    if (chpl_nodeID == 0) {
      for (int i = 1; i < chpl_numNodes; i++) {
        fork_shutdown(i);
      }
    } else {
      chpl_wait_for_shutdown();
    }

    chpl_comm_barrier("chpl_comm_pre_task_exit");

    polling_task_please_exit = true;

    if (chpl_nodeID == 0) {
      while (!polling_task_done)
        sched_yield();
    }
  }

  //
  // It's handy to be able to print the registered memory regions
  // high water mark whether set up for debug info or not.
  //
  {
#ifdef CHPL_COMM_DEBUG
    const int test = _DBG_DO(DBGF_MEMREG)
                     || (chpl_nodeID == 0
                         && chpl_env_rt_get_bool("COMM_UGNI_MEMREG_HWM",
                                                 false));
    FILE* f = debug_file;
#else
    const int test = (chpl_nodeID == 0
                      && chpl_env_rt_get_bool("COMM_UGNI_MEMREG_HWM", false));
    FILE* f = stdout;
#endif

    if (test) {
      fprintf(f, "ugni comm: reg mem regions high water mark %d\n",
              (int) mreg_cnt_max);
    }
  }

  for (uint32_t i = 0; i < comm_dom_cnt; i++) {
    DBGSTAT_P_L(DBGSF_ANY, "cd[%d] acqs:               %12" PRIu64, i,
                comm_doms[i].acqs);
    DBGSTAT_P_L(DBGSF_ANY, "cd[%d] acqs_looks:         %12" PRIu64, i,
                comm_doms[i].acqs_looks);
    DBGSTAT_P_L(DBGSF_ANY, "cd[%d] acqs_with_rb:       %12" PRIu64, i,
                comm_doms[i].acqs_with_rb);
    DBGSTAT_P_L(DBGSF_ANY, "cd[%d] acqs_with_rb_looks: %12" PRIu64, i,
                comm_doms[i].acqs_with_rb_looks);
    DBGSTAT_P_L(DBGSF_ANY, "cd[%d] reacqs:             %12" PRIu64, i,
                comm_doms[i].reacqs);
  }

  //
  // Here we would like to shut everything down and free up all the
  // memory we've allocated.  But we can't do that yet, because we may
  // do a little bit more communication in support of internal things
  // like memory reporting.  (See the shutdown logic in chplexit.c.)
  // So, although at this point we can't do remote forks because we've
  // stopped the polling thread, we still have to wait until later to
  // get rid of the rest of our uGNI infrastructure and free all our
  // memory.
  //
}


void chpl_comm_exit(int all, int status)
{
  if (exit_without_cleanup)
    return;

#ifdef CHPL_COMM_DEBUG
  debug_exiting = true;
#endif

  chpl_comm_mem_reg_exit();

  if (all)
    exit_all(status);
  else
    exit_any(status);
}


static
void exit_all(int status)
{
  {
    PMI_BOOL initialized;

    if (PMI_Initialized(&initialized) != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Initialized() failed");
    if (initialized == PMI_TRUE && PMI_Finalize() != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Finalize() failed");
  }

  if (chpl_numNodes == 1)
    return;
}


static
void exit_any(int status)
{
  //
  // Don't clean up anything. Just shut everything down right now.
  //
}


static
void rf_handler(gni_cq_entry_t* ev)
{
  //
  // This is invoked for each event on the rf_cqh completion queue.
  // It has to be fast, because its speed sets the limit on how fast
  // we can handle inbound fork requests.  For that reason, we do
  // not allow it to communicate remotely.  Any communication must
  // be done in an invoked task, not in the handler itself.
  //
  uint32_t inst_id;
  uint32_t req_li;
  int      req_cdi;
  int      req_rbi;
  fork_t*  f;

  inst_id = GNI_CQ_GET_REM_INST_ID(*ev);
  GNI_DECODE_REM_INST_ID(req_li, req_cdi, req_rbi, inst_id);
  f = RECV_SIDE_FORK_REQ_BUF_ADDR(req_li, req_cdi, req_rbi);

  switch (f->b.op) {
  case fork_op_small_call:
    DBG_P_LP(DBGF_RF, "forkFrom(%d) %s",
             (int) req_li, sprintf_rf_req(-1, f)); 
    {
      fork_small_call_info_t* f_c = (fork_small_call_info_t*) f;
      
      // Reconstruct the chpl_comm_on_bundle_t and copy the arguments to it
      fork_small_call_task_t space;
      chpl_comm_on_bundle_t* on_bundle = &space.bundle;

      size_t payload_size = f_c->payload_size;
      size_t bundle_size;
      chpl_fn_p fn;

      // Copy task state (e.g. serial state) to space.
      on_bundle->task_bundle.state = f_c->state;

      // Copy the payload to space.
      // Note ptr+1 here is the same as (unsigned char*)ptr + sizeof(*ptr)
      // and it refers to the memory just after that structure.
      // Also note that we could copy to space.buf directly, but
      // it's possible for the C compiler to add padding before the buf field.
      memcpy(on_bundle + 1, f_c + 1, payload_size);
      bundle_size = sizeof(chpl_comm_on_bundle_t) + payload_size;

      if (f_c->fast) {
        chpl_ftable_call(f_c->fid, on_bundle);
        indicate_done(&f_c->b);
        // doesn't call release_req_buf, because that
        // is handled on the sender side for fast forks
      }
      else {
        if (f_c->blocking)
          fn = (chpl_fn_p) fork_call_wrapper_blocking;
        else
          fn = (chpl_fn_p) chpl_ftable[f_c->fid];

        // Save fid, caller, ack so the wrapper function can use these.
        // these are saved in the comm portion of the on_bundle.
        on_bundle->comm.fid = f_c->fid;
        on_bundle->comm.caller = f_c->b.caller;
        on_bundle->comm.rf_done = f_c->b.rf_done;

        chpl_task_startMovedTask(f_c->fid,
                                 fn,
                                 chpl_comm_on_bundle_task_bundle(on_bundle),
                                 bundle_size,
                                 f_c->subloc,
                                 chpl_nullTaskID);

        release_req_buf(req_li, req_cdi, req_rbi);
      }
    }
    break;

  case fork_op_large_call:
    DBG_P_LP(DBGF_RF, "forkFrom(%d) %s",
             (int) req_li, sprintf_rf_req(-1, f)); 
    {
      fork_large_call_info_t* f_lc = (fork_large_call_info_t*) f;
      fork_small_call_info_t* f_c = &f_lc->b;
      
      // Create a task bundle to complete a large call
      fork_large_call_task_t bundle;

      // Copy task state (e.g. serial state) to space.
      bundle.task.state = f_c->state;

      // copy the fork_large_call_info_t to it
      bundle.large = *f_lc;

      chpl_task_startMovedTask(f_c->fid,
                               (chpl_fn_p) fork_call_wrapper_large,
                               &bundle.task, sizeof(fork_large_call_task_t),
                               f_c->subloc,
                               chpl_nullTaskID);

      release_req_buf(req_li, req_cdi, req_rbi);
    }
    break;

  case fork_op_put:
    DBG_P_LP(DBGF_GETPUT|DBGF_RF, "forkFrom(%d) %s",
             (int) req_li, sprintf_rf_req((int) req_li, f));

    {
      fork_xfer_info_t f_x = f->x;

      release_req_buf(req_li, req_cdi, req_rbi);
      do_remote_put(f_x.src, req_li, f_x.tgt, f_x.size, NULL, may_proxy_false);
      indicate_done(&f_x.b);
    }
    break;

  case fork_op_get:
    DBG_P_LP(DBGF_GETPUT|DBGF_RF, "forkFrom(%d) %s",
             (int) req_li, sprintf_rf_req((int) req_li, f));

    {
      fork_xfer_task_t bundle = {.x = f->x};

      release_req_buf(req_li, req_cdi, req_rbi);
      chpl_task_startMovedTask(FID_NONE, (chpl_fn_p) fork_get_wrapper,
                               &bundle.task, sizeof(fork_xfer_task_t),
                               c_sublocid_any, chpl_nullTaskID);
    }
    break;

  case fork_op_free:
    DBG_P_LP(DBGF_RF, "forkFrom(%d) %s",
             (int) req_li, sprintf_rf_req(-1, f));

    {
      void* p = f->f.p;
      release_req_buf(req_li, req_cdi, req_rbi);
      chpl_mem_free(p, 0, 0);
    }
    break;

  case fork_op_amo:
    DBG_P_LP(DBGF_AMO|DBGF_RF, "forkFrom(%d) %s",
             (int) req_li, sprintf_rf_req((int) req_li, f));

    {
      fork_amo_info_t f_a = f->a;
      release_req_buf(req_li, req_cdi, req_rbi);
      fork_amo_wrapper(&f_a);
    }
    break;

  case fork_op_shutdown:
    DBG_P_LP(DBGF_RF, "shutdownFrom(%d) %s",
             (int) req_li, sprintf_rf_req(-1, f));

    {
      release_req_buf(req_li, req_cdi, req_rbi);
      chpl_signal_shutdown();
    }
    break;

  default:
    CHPL_INTERNAL_ERROR("unexpected fork request operation");
    break;
  }
}

static
void fork_call_wrapper_blocking(chpl_comm_on_bundle_t* f)
{
  // Call the on body 
  chpl_ftable_call(f->comm.fid, f);

  //
  // In the blocking case, let the caller know we're done.  It will free
  // the copy of the argument it made on our behalf.  In the nonblocking
  // case, we have to do a remote fork back to the initiating locale to
  // free that argument ourselves.
  //

  indicate_done2(f->comm.caller, (rf_done_t*) f->comm.rf_done);
}
  
static
void fork_call_wrapper_large(fork_large_call_task_t* f)
{
  fork_large_call_info_t* lc = &f->large;
  int caller;
  chpl_comm_on_bundle_t* bundle;
  chpl_bool blocking;
  size_t arg_size;
  void* remote_arg = NULL;
  rf_done_t* rf_done;
 
  blocking = lc->b.blocking;
  caller   = lc->b.b.caller;
  rf_done  = lc->b.b.rf_done;

  remote_arg = lc->arg;
  arg_size   = lc->arg_size;

  assert(remote_arg);
  assert(arg_size > 0);

  // Allocate a buffer to hold the complete on bundle
  bundle = chpl_mem_alloc(arg_size, CHPL_RT_MD_COMM_FRK_RCV_INFO, 0, 0);

  // Get the complete on-bundle from the initiating side.
  do_remote_get(bundle, caller, remote_arg, arg_size, may_proxy_true);

  // In the large nonblocking case, we have to do a remote fork back to the
  // initiating locale to free that argument ourselves. This can be
  // done before the user code starts running in order to reduce
  // memory pressure.
  if (!blocking) {
    fork_free(caller, remote_arg);
  }
  
  // Call the on body 
  chpl_ftable_call(lc->b.fid, bundle);

  // Free the bundle we just allocated. 
  chpl_mem_free(bundle, 0, 0);

  //
  // In the blocking case, let the caller know we're done.  It will free
  // the copy of the argument it made on our behalf.  In the nonblocking
  // case, we have to do a remote fork back to the initiating locale to
  // free that argument ourselves.
  //
  if (blocking) {
    // Blocking forks need to notify the caller that the
    // request has completed.
    indicate_done2(caller, rf_done);
  }
}

static
void fork_get_wrapper(fork_xfer_task_t* f)
{
  do_remote_get(f->x.tgt, f->x.b.caller, f->x.src, f->x.size, may_proxy_false);
  indicate_done(&f->x.b);
}


static
inline
size_t do_amo_on_cpu(fork_amo_cmd_t cmd,
                     void* res, void* obj, void* opnd1, void* opnd2)
{
  size_t res_size = 0;

#define CPU_INT_ARITH_AMO(_o, _t)                                       \
        do {                                                            \
          if (res == NULL) {                                            \
            (void) atomic_fetch_##_o##_##_t((atomic_##_t*) obj,         \
                                            *(_t*) opnd1);              \
          }                                                             \
          else {                                                        \
            _t my_res;                                                  \
            my_res = atomic_fetch_##_o##_##_t((atomic_##_t*) obj,       \
                                              *(_t*) opnd1);            \
            memcpy(res, &my_res, sizeof(my_res));                       \
            res_size = sizeof(my_res);                                  \
          }                                                             \
        } while (0)

  //
  // Here we implement AMOs which the NIC cannot do, either because
  // the target object is not in registered memory or because the NIC
  // lacks native support.  For more information, see the comment
  // before chpl_comm_atomic_store_int32().
  //
  switch (cmd) {
  case put_32:
    atomic_store_int_least32_t((atomic_int_least32_t*) obj,
                               *(int_least32_t*) opnd1);
    break;

  case put_64:
    atomic_store_int_least64_t((atomic_int_least64_t*) obj,
                               *(int_least64_t*) opnd1);
    break;

  case get_32:
    {
      int_least32_t my_res;
      my_res = atomic_load_int_least32_t((atomic_int_least32_t*) obj);
      memcpy(res, &my_res, sizeof(my_res));
      res_size = sizeof(my_res);
    }
    break;

  case get_64:
    {
      int_least64_t my_res;
      my_res = atomic_load_int_least64_t((atomic_int_least64_t*) obj);
      memcpy(res, &my_res, sizeof(my_res));
      res_size = sizeof(my_res);
    }
    break;

  case xchg_32:
    {
      int_least32_t my_res;
      my_res = atomic_exchange_int_least32_t((atomic_int_least32_t*) obj,
                                             *(int_least32_t*) opnd1);
      memcpy(res, &my_res, sizeof(my_res));
      res_size = sizeof(my_res);
    }
    break;

  case xchg_64:
    {
      int_least64_t my_res;
      my_res = atomic_exchange_int_least64_t((atomic_int_least64_t*) obj,
                                             *(int_least64_t*) opnd1);
      memcpy(res, &my_res, sizeof(my_res));
      res_size = sizeof(my_res);
    }
    break;

  case cmpxchg_32:
    {
      chpl_bool32 my_res;
      my_res = atomic_compare_exchange_strong_int_least32_t
                 ((atomic_int_least32_t*) obj,
                  *(int_least32_t*) opnd1,
                  *(int_least32_t*) opnd2);
      memcpy(res, &my_res, sizeof(my_res));
      res_size = sizeof(my_res);
    }
    break;

  case cmpxchg_64:
    //
    // If the object is not in memory registered with the NIC, use the
    // processor.  Otherwise, since the other 64-bit AMOs are done on
    // the NIC, for coherence do this one on the NIC as well.
    //
    {
      chpl_bool32 my_res;
      mem_region_t* mr;
      if ((mr = mreg_for_local_addr(obj)) == NULL) {
        my_res = atomic_compare_exchange_strong_int_least64_t
                   ((atomic_int_least64_t*) obj,
                    *(int_least64_t*) opnd1,
                    *(int_least64_t*) opnd2);
      }
      else {
        int_least64_t nic_res;
        do_nic_amo(opnd1, opnd2, chpl_nodeID, obj, sizeof(nic_res),
                   amo_cmd_2_nic_op(cmpxchg_64, 1), &nic_res, mr);
        my_res = (nic_res == *(int_least64_t*) opnd1) ? true : false;
      }
      memcpy(res, &my_res, sizeof(my_res));
      res_size = sizeof(my_res);
    }
    break;

  case and_i32:
    CPU_INT_ARITH_AMO(and, int_least32_t);
    break;

  case and_i64:
    CPU_INT_ARITH_AMO(and, int_least64_t);
    break;

  case or_i32:
    CPU_INT_ARITH_AMO(or, int_least32_t);
    break;

  case or_i64:
    CPU_INT_ARITH_AMO(or, int_least64_t);
    break;

  case xor_i32:
    CPU_INT_ARITH_AMO(xor, int_least32_t);
    break;

  case xor_i64:
    CPU_INT_ARITH_AMO(xor, int_least64_t);
    break;

  case add_i32:
    CPU_INT_ARITH_AMO(add, int_least32_t);
    break;

  case add_i64:
    CPU_INT_ARITH_AMO(add, int_least64_t);
    break;

  case add_r32:
    CPU_INT_ARITH_AMO(add, _real32);
    break;

  case add_r64:
    //
    // Emulate 64-bit real add using compare-exchange.  If the object
    // is not in memory registered with the NIC, use the processor.
    // Otherwise, since the other 64-bit AMOs are done on the NIC, for
    // coherence do this one on the NIC as well.
    //
    {
      mem_region_t* mr;

      if ((mr = mreg_for_local_addr(obj)) == NULL) {
        CPU_INT_ARITH_AMO(add, _real64);
      } else {
        int_least64_t expected;
        int_least64_t desired;
        int_least64_t nic_res;

        do {
          do_remote_get(&expected, chpl_nodeID, obj, sizeof(expected),
                        may_proxy_false);
          *(double*) &desired = *(double*) &expected + *(double*) opnd1;
          do_nic_amo(&expected, &desired, chpl_nodeID, obj,
                     sizeof(nic_res), amo_cmd_2_nic_op(cmpxchg_64, 1),
                     &nic_res, mr);
        } while (nic_res != expected);

        if (res != NULL) {
          memcpy(res, &expected, sizeof(expected));
          res_size = sizeof(expected);
        }
      }

    }
    break;

  default:
    CHPL_INTERNAL_ERROR("unsupported fork_amo command");
  }

  return res_size;

#undef CPU_INT_ARITH_AMO
}


static
void fork_amo_wrapper(fork_amo_info_t* f_a)
{
  fork_amo_data_t res;
  size_t res_size;

  res_size = do_amo_on_cpu(f_a->cmd, &res, f_a->obj, &f_a->opnd1, &f_a->opnd2);
  if (f_a->res != NULL) {
    do_remote_put(&res, f_a->b.caller, f_a->res, res_size, NULL,
                  may_proxy_false);
  }

  indicate_done(&f_a->b);
}


static
void release_req_buf(uint32_t li, int cdi, int rbi)
{
  static chpl_bool32 free_flag = true;
  send_polling_response(&free_flag, li,
                        RECV_SIDE_FORK_REQ_FREE_ADDR(li, cdi, rbi),
                        sizeof(free_flag), &gnr_mreg_map[li]);
}


static
inline
void indicate_done(fork_base_info_t* b)
{
  indicate_done2(b->caller, b->rf_done);
}


static
inline
void indicate_done2(int caller, rf_done_t *ack)
{
  static rf_done_t done = 1;
  send_polling_response(&done, caller, ack, sizeof(done), NULL);
}


static
void send_polling_response(void* src_addr, c_nodeid_t locale, void* tgt_addr,
                           size_t size, mem_region_t* mr)
{
  gni_post_descriptor_t* post_desc;

  //
  // If we're not the polling task, send the response the regular way.
  // Thus we only do asynchronous completion processing on polling task
  // response transactions initiated on its firmly bound comm domain.
  // This gets nearly all the benefit of not waiting for completions,
  // while avoiding concurrency control entirely.
  //
  if (cd == NULL || !cd->firmly_bound) {
    do_remote_put(src_addr, locale, tgt_addr, size, mr, may_proxy_false);
    return;
  }

  DBG_P_LP(DBGF_GETPUT, "SendPollResp %p -> %d:%p (%#zx)",
           src_addr, (int) locale, tgt_addr, size);

  {
    //
    // This pool of GNI Post descriptors is for the polling task to use
    // for request responses (buffer releases and "done" indicators).
    // To improve throughput, we don't wait for the remote completions
    // when we send these.  Instead, we collect the CQ completion events
    // later, as they come in.  We can have as many request responses in
    // flight simultaneously as we have Post descriptors in the pool.
    // Small-scale runs on Aries-based systems show the polling task can
    // do about 3 minimum-cost requests ("fast" remote forks, etc.) in
    // the minimum network latency of ~1 usec.  The pool size we use
    // here is enough larger that it will cover the max network latency
    // of ~1.5 usec.
    //
    // The post_id members of these are used as alloc/free indicators,
    // with 0 meaning "free".
    //
#define PPDESCS_CNT 8  // must be a power of 2; see PPDI_NEXT()

#define PPDI_NEXT(_i) (((_i) + 1) & (PPDESCS_CNT - 1))

    static gni_post_descriptor_t polling_post_descs[PPDESCS_CNT] = {{ 0 }};
    static int last_ppdi = 0;
    int ppdi;

    ppdi = last_ppdi;
    while (polling_post_descs[ppdi].post_id != 0) {
      if ((ppdi = PPDI_NEXT(ppdi)) == last_ppdi) {
        local_yield();
        consume_all_outstanding_cq_events(cd_idx);
      }
    }

    last_ppdi = PPDI_NEXT(ppdi);

    post_desc = &polling_post_descs[ppdi];
    post_desc->post_id = 1;  // mark as allocated

#undef PPDESCS_CNT
#undef PPDI_NEXT
  }

  //
  // Fill in the POST descriptor.
  //
  if (mr == NULL
      && (mr = mreg_for_remote_addr(tgt_addr, locale)) == NULL) {
    CHPL_INTERNAL_ERROR("send_polling_response(): "
                        "remote address is not NIC-registered");
  }

  post_desc->type            = GNI_POST_FMA_PUT;
  post_desc->cq_mode         = GNI_CQMODE_GLOBAL_EVENT;
  post_desc->dlvr_mode       = GNI_DLVMODE_PERFORMANCE;
  post_desc->rdma_mode       = 0;
  post_desc->src_cq_hndl     = 0;
  post_desc->local_addr      = (uint64_t) (intptr_t) src_addr;
  post_desc->remote_addr     = (uint64_t) (intptr_t) tgt_addr;
  post_desc->remote_mem_hndl = mr->mdh;
  post_desc->length          = size;

  //
  // Initiate the transaction.  Don't wait for it to complete.  We'll
  // consume any completion events later, in various places.
  //
  PERFSTATS_INC(put_cnt);
  PERFSTATS_ADD(put_byte_cnt, size);
  (void) post_fma(locale, post_desc);
}


static
void rf_done_pre_init(void)
{
  //
  // We overload rf_done indicators as pool "next" links, so the
  // latter have to fit in the former.
  //
  assert(sizeof(rf_done_pool_t) <= sizeof(rf_done_t));

  //
  // The fork request buffers and their is-free flags aren't part of
  // the rf_done flags as such, but both have to do with remote forks
  // and the former have to be in NIC-registered memory just as the
  // latter do, so this seems the best place to express that to the
  // registered-memory allocator.
  //
  chpl_comm_mem_reg_add_request(FORK_REQ_BUFS_PER_LOCALE
                                * sizeof(fork_reqs[0]));
  chpl_comm_mem_reg_add_request(FORK_REQ_BUFS_PER_LOCALE
                                * sizeof(fork_reqs_free[0]));

  chpl_comm_mem_reg_add_request(RF_DONE_NUM_POOLS
                                * sizeof(rf_done_pool[0]));
}


static
void rf_done_init(void)
{
  int i, j;

  if (RF_DONE_NUM_POOLS * RF_DONE_NUM_PER_POOL
      < comm_dom_cnt_max * FORK_REQ_BUFS_PER_CD)
    chpl_warning("(RF_DONE_NUM_POOLS * RF_DONE_NUM_PER_POOL "
                 "< comm_dom_cnt_max * FORK_REQ_BUFS_PER_CD) "
                 "may lead to hangs",
                 0, 0);

  // Must be directly communicable without proxy.
  rf_done_pool = (rf_done_pool_t (*)[RF_DONE_NUM_PER_POOL])
                 chpl_comm_mem_reg_allocMany(RF_DONE_NUM_POOLS,
                                             sizeof(rf_done_pool[0]),
                                             CHPL_RT_MD_COMM_PER_LOC_INFO,
                                             0, 0);

  for (i = 0; i < RF_DONE_NUM_POOLS; i++) {
    for (j = 0; j < RF_DONE_NUM_PER_POOL - 1; j++) {
      rf_done_pool[i][j] = j + 1;
    }
    rf_done_pool[i][j] = -1;

    mpool_idx_init(&rf_done_pool_head[i], 0);

    atomic_init_bool(&rf_done_pool_lock[i], false);
  }

  mpool_idx_init(&rf_done_pool_i, 0);
}


static
inline
rf_done_t* rf_done_alloc(void)
{
  int pool_tries;
  int i, j;
  rf_done_pool_t* rf_done_p;

  //
  // Cycle through the pools and find one that contains a free rf_done
  // flag we can use.  We give up when we've looked at all the pools
  // twice without finding anything.
  //
  // We need a termination test because we can actually run out of
  // these, if for example RF_DONE_NUM_POOLS*RF_DONE_NUM_IN_POOL tasks
  // do remote on-stmts all at once, and each of those tries to do
  // another on-stmt.  If we ever see that happen in a program that
  // isn't specifically designed to achieve it, we'll have to revisit
  // how we've approached this.
  //
  for (pool_tries = 0, i = rf_done_next_pool_i(), rf_done_p = NULL;
       pool_tries < 2 * RF_DONE_NUM_POOLS && rf_done_p == NULL;
       pool_tries++, i = (i + 1) % RF_DONE_NUM_POOLS) {
    if (mpool_idx_load(&rf_done_pool_head[i]) >= 0 &&
        !atomic_exchange_bool(&rf_done_pool_lock[i], true)) {
      if ((j = mpool_idx_load(&rf_done_pool_head[i])) >= 0) {
        rf_done_p = &rf_done_pool[i][j];
        mpool_idx_store(&rf_done_pool_head[i], *rf_done_p);
      }

      atomic_store_bool(&rf_done_pool_lock[i], false);
    }
  }

  if (rf_done_p == NULL)
    CHPL_INTERNAL_ERROR("rf_done_pool empty");

  return (rf_done_t*) rf_done_p;
}


static
inline
void rf_done_free(rf_done_t* rf_done_p)
{
  rf_done_pool_t* rfdpp = (rf_done_pool_t*) rf_done_p;
  int i = (rfdpp - (rf_done_pool_t*) rf_done_pool) / RF_DONE_NUM_PER_POOL;
  int j = (rfdpp - (rf_done_pool_t*) rf_done_pool) % RF_DONE_NUM_PER_POOL;

  //
  // Add this flag back to its pool.
  //
  while (atomic_exchange_bool(&rf_done_pool_lock[i], true))
    ;
  rf_done_pool[i][j] = mpool_idx_exchange(&rf_done_pool_head[i], j);
  atomic_store_bool(&rf_done_pool_lock[i], false);
}


static
void get_buf_pre_init(void)
{
#define ADD_REQ(size)                                                   \
  chpl_comm_mem_reg_add_request(gbp_##size##_num_pools                  \
                                * gbp_##size##_num_per_pool             \
                                * size)

  ADD_REQ(8);
  ADD_REQ(64);
  ADD_REQ(512);
  ADD_REQ(4096);

#undef ADD_REQ
}


static
inline
void get_buf_N_init(int size, int nPools, int nPerPool, int64_t **gbp,
                    mpool_idx_t gbp_head[], atomic_bool gbp_lock[],
                    mpool_idx_t *p_gbp_pool_i)
{
  const int pElemSize = size / sizeof(**gbp);

  // Must be directly communicable without proxy.
  *gbp = (int64_t*)
         chpl_comm_mem_reg_allocMany(nPools * nPerPool * pElemSize,
                                     sizeof(**gbp),
                                     CHPL_RT_MD_COMM_XMIT_RCV_BUF,
                                     0, 0);

  for (int i = 0; i < nPools; i++) {
    for (int j = 0; j < nPerPool - 1; j++) {
      (*gbp)[(i * nPerPool + j) * pElemSize] = j + 1;
    }
    (*gbp)[(i * nPerPool + nPerPool - 1) * pElemSize] = -1;

    mpool_idx_init(&gbp_head[i], 0);

    atomic_init_bool(&gbp_lock[i], false);
  }

  mpool_idx_init(p_gbp_pool_i, 0);
}


static
void get_buf_init(void)
{
  get_buf_N_init(8, gbp_8_num_pools, gbp_8_num_per_pool,
                 &gbp_8, gbp_8_head, gbp_8_lock, &gbp_8_pool_i);
  get_buf_N_init(64, gbp_64_num_pools, gbp_64_num_per_pool,
                 &gbp_64, gbp_64_head, gbp_64_lock, &gbp_64_pool_i);
  get_buf_N_init(512, gbp_512_num_pools, gbp_512_num_per_pool,
                 &gbp_512, gbp_512_head, gbp_512_lock, &gbp_512_pool_i);
  get_buf_N_init(4096, gbp_4096_num_pools, gbp_4096_num_per_pool,
                 &gbp_4096, gbp_4096_head, gbp_4096_lock, &gbp_4096_pool_i);
}


static
inline
void* get_buf_N_alloc(int size, int nPools, int nPerPool, int64_t gbp[],
                      mpool_idx_t gbp_head[], atomic_bool gbp_lock[],
                      mpool_idx_base_t gbp_next_pool_i)
{
  const int pElemSize = size / sizeof(gbp[0]);
  int i, j;
  int64_t* p;

  //
  // Cycle through the pools and find one that contains a free GET
  // buffer we can use.  Just keep trying until we find one.  We
  // cannot livelock ourselves here, because unlike for example the
  // rf_done flags, GETs cannot cause other GETs in turn.
  //
  for (i = gbp_next_pool_i, p = NULL; p == NULL; i = (i + 1) % nPools) {
    if (mpool_idx_load(&gbp_head[i]) >= 0 &&
        !atomic_exchange_bool(&gbp_lock[i], true)) {
      if ((j = mpool_idx_load(&gbp_head[i])) >= 0) {
        p = &gbp[(i * nPerPool + j) * pElemSize];
        mpool_idx_store(&gbp_head[i], p[0]);
      }

      atomic_store_bool(&gbp_lock[i], false);
    }
  }

  return p;
}


static
inline
void* get_buf_alloc(size_t size)
{
  if (size <= 8)
    return(get_buf_N_alloc(8, gbp_8_num_pools, gbp_8_num_per_pool,
                           gbp_8, gbp_8_head, gbp_8_lock,
                           gbp_8_next_pool_i()));

  if (size <= 64)
    return(get_buf_N_alloc(64, gbp_64_num_pools, gbp_64_num_per_pool,
                           gbp_64, gbp_64_head, gbp_64_lock,
                           gbp_64_next_pool_i()));

  if (size <= 512)
    return(get_buf_N_alloc(512, gbp_512_num_pools, gbp_512_num_per_pool,
                           gbp_512, gbp_512_head, gbp_512_lock,
                           gbp_512_next_pool_i()));

  if (size <= 4096)
    return(get_buf_N_alloc(4096, gbp_4096_num_pools, gbp_4096_num_per_pool,
                           gbp_4096, gbp_4096_head, gbp_4096_lock,
                           gbp_4096_next_pool_i()));

  CHPL_INTERNAL_ERROR("GET buffer allocation request too big");
  return NULL;    // thwart "failed to return a value"
}


static
inline
chpl_bool get_buf_N_free(int64_t* p,
                         int size, int nPools, int nPerPool, int64_t gbp[],
                         mpool_idx_t gbp_head[], atomic_bool gbp_lock[])
{
  const int pElemSize = size / sizeof(gbp[0]);

  if (p >= &gbp[0]
      && p <= &gbp[(nPools * nPerPool - 1) * pElemSize]) {
    const int i = ((p - gbp) / pElemSize) / nPerPool;
    const int j = ((p - gbp) / pElemSize) % nPerPool;

    while (atomic_exchange_bool(&gbp_lock[i], true))
      sched_yield();
    p[0] = mpool_idx_exchange(&gbp_head[i], j);
    atomic_store_bool(&gbp_lock[i], false);

    return true;
  }

  return false;
}


static
inline
void get_buf_free(int64_t* p)
{
  if (get_buf_N_free(p,
                     8, gbp_8_num_pools, gbp_8_num_per_pool,
                     gbp_8, gbp_8_head, gbp_8_lock))
    return;

  if (get_buf_N_free(p,
                     64, gbp_64_num_pools, gbp_64_num_per_pool,
                     gbp_64, gbp_64_head, gbp_64_lock))
    return;

  if (get_buf_N_free(p,
                     512, gbp_512_num_pools, gbp_512_num_per_pool,
                     gbp_512, gbp_512_head, gbp_512_lock))
    return;

  if (get_buf_N_free(p,
                     4096, gbp_4096_num_pools, gbp_4096_num_per_pool,
                     gbp_4096, gbp_4096_head, gbp_4096_lock))
    return;

  CHPL_INTERNAL_ERROR("GET buffer free pointer was never allocated");
}


static
void amo_res_pre_init(void)
{
  chpl_comm_mem_reg_add_request(AMO_RES_NUM_POOLS
                                * sizeof(amo_res_pool[0]));
}


static
void amo_res_init(void)
{
  int i, j;

  // Must be directly communicable without proxy.
  amo_res_pool = (fork_amo_data_t (*)[AMO_RES_NUM_PER_POOL])
                 chpl_comm_mem_reg_allocMany(AMO_RES_NUM_POOLS,
                                             sizeof(amo_res_pool[0]),
                                             CHPL_RT_MD_COMM_PER_LOC_INFO,
                                             0, 0);

  for (i = 0; i < AMO_RES_NUM_POOLS; i++) {
    for (j = 0; j < AMO_RES_NUM_PER_POOL - 1; j++) {
      amo_res_pool[i][j].i = j + 1;
    }
    amo_res_pool[i][j].i = -1;

    mpool_idx_init(&amo_res_pool_head[i], 0);

    atomic_init_bool(&amo_res_pool_lock[i], false);
  }

    mpool_idx_init(&amo_res_pool_i, 0);
}


static
inline
fork_amo_data_t* amo_res_alloc(void)
{
  int i, j;
  fork_amo_data_t* amo_res_p;

  //
  // Cycle through the pools and find one that contains a free AMO
  // result buffer we can use.  Just keep trying until we find one.
  // We cannot livelock ourselves here, because unlike for example the
  // rf_done flags, remote forks for the purpose of AMOs cannot cause
  // other remote forks in turn.
  //
  for (i = amo_res_next_pool_i(), amo_res_p = NULL;
       amo_res_p == NULL;
       i = (i + 1) % AMO_RES_NUM_POOLS) {
    if (mpool_idx_load(&amo_res_pool_head[i]) >= 0 &&
        !atomic_exchange_bool(&amo_res_pool_lock[i], true)) {
      if ((j = mpool_idx_load(&amo_res_pool_head[i])) >= 0) {
        amo_res_p = &amo_res_pool[i][j];
        mpool_idx_store(&amo_res_pool_head[i], amo_res_p->i);
      }

      atomic_store_bool(&amo_res_pool_lock[i], false);
    }
  }

  return amo_res_p;
}


static
inline
void amo_res_free(fork_amo_data_t* amo_res_p)
{
  int i = (amo_res_p - (fork_amo_data_t*) amo_res_pool) / AMO_RES_NUM_PER_POOL;
  int j = (amo_res_p - (fork_amo_data_t*) amo_res_pool) % AMO_RES_NUM_PER_POOL;

  //
  // Add this flag back to its pool.
  //
  while (atomic_exchange_bool(&amo_res_pool_lock[i], true))
    ;
  amo_res_p->i = mpool_idx_exchange(&amo_res_pool_head[i], j);
  atomic_store_bool(&amo_res_pool_lock[i], false);
}


static
inline
void consume_all_outstanding_cq_events(int cdi)
{
  if (reacquire_comm_dom(cdi)) {
    gni_cq_entry_t         ev;
    gni_post_descriptor_t* post_desc;
    gni_return_t           gni_rc;

    while ((gni_rc = GNI_CqGetEvent(cd->cqh, &ev)) == GNI_RC_SUCCESS) {
      GNI_CHECK(GNI_GetCompleted(cd->cqh, ev, &post_desc));

      if (post_desc->post_id == 1) {
        //
        // This Post descriptor is from the polling task's dedicated
        // pool for responses.  Just mark it free.
        //
        post_desc->post_id = 0;  // mark as free
      } else {
        //
        // This event is for completion of a regular transaction and the
        // post_id is the pointer to the "done" flag the initiating task
        // is waiting on.
        //
        atomic_store_bool((atomic_bool*) (intptr_t) post_desc->post_id, true);
      }
      CQ_CNT_DEC(cd);
    }

    assert(gni_rc == GNI_RC_NOT_DONE);

    release_comm_dom();
  }
}


void chpl_comm_put(void* addr, c_nodeid_t locale, void* raddr,
                   size_t size, int32_t commID, int ln, int32_t fn)
{
  DBG_P_LP(DBGF_IFACE|DBGF_GETPUT, "IFACE chpl_comm_put(%p, %d, %p, %zd)",
           addr, (int) locale, raddr, size);

  assert(addr != NULL);
  assert(raddr != NULL);
  if (size == 0)
    return;

  if (locale == chpl_nodeID) {
    memmove(raddr, addr, size);
    return;
  }

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_put)) {
      chpl_comm_cb_info_t cb_data =
        {chpl_comm_cb_event_kind_put, chpl_nodeID, locale,
         .iu.comm={addr, raddr, size, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_rdma("put", locale, size, ln, fn, commID);
  chpl_comm_diags_incr(put);

  do_remote_put(addr, locale, raddr, size, NULL, may_proxy_true);
}


static
void do_remote_put(void* src_addr, c_nodeid_t locale, void* tgt_addr,
                   size_t size, mem_region_t* remote_mr,
                   drpg_may_proxy_t may_proxy)
{
  gni_post_descriptor_t post_desc;
  mem_region_t* local_mr;
  chpl_bool do_rdma = false;
  size_t max_trans_sz = MAX_FMA_TRANS_SZ;


  DBG_P_LP(DBGF_GETPUT, "DoRemPut %p -> %d:%p (%#zx), proxy %c",
           src_addr, (int) locale, tgt_addr, size,
           may_proxy ? 'y' : 'n');

  //
  // The source address for a FMA PUT doesn't have to be registered with
  // the local NIC, but the target address does have to be registered
  // with the remote NIC.  If it isn't, then instead of our doing a
  // PUT we arrange for the remote side do a GET from us, dealing with
  // the unregistered target address itself.  If we do this then we
  // also need the source address here to be registered, because GETs
  // must have registered source addresses.
  //
  if (remote_mr == NULL)
    remote_mr = mreg_for_remote_addr(tgt_addr, locale);

  if (remote_mr == NULL) {

    if (!may_proxy) {
      CHPL_INTERNAL_ERROR("do_remote_put(): "
                          "remote address is not NIC-registered");
    }

    local_mr = mreg_for_local_addr(src_addr);
    if (local_mr != NULL) {
      //
      // The local source address is NIC-registered.  The target side
      // can do a direct GET from us.
      //
      fork_get(src_addr, locale, tgt_addr, size);
    }
    else {
      //
      // The local source address is not NIC-registered.  Transfer
      // through a trampoline buffer.
      //
      void* src_addr_xmit;

      if (size <= gbp_max_size) {
        //
        // The transfer will fit in a single trampoline buffer.
        // 
        src_addr_xmit = get_buf_alloc(size);
        memcpy(src_addr_xmit, src_addr, size);
        fork_get(src_addr_xmit, locale, tgt_addr, size);
        get_buf_free(src_addr_xmit);
      }
      else {
        //
        // The transfer is larger than the largest trampoline buffer.
        // Do it in pieces.
        // 
        src_addr_xmit = get_buf_alloc(gbp_max_size);

        do {
          memcpy(src_addr_xmit, src_addr, gbp_max_size);
          fork_get(src_addr_xmit, locale, tgt_addr, gbp_max_size);
          src_addr = (char*) src_addr + gbp_max_size;
          tgt_addr = (char*) tgt_addr + gbp_max_size;
          size -= gbp_max_size;
        } while (size > gbp_max_size);

        memcpy(src_addr_xmit, src_addr, size);
        fork_get(src_addr_xmit, locale, tgt_addr, size);
        get_buf_free(src_addr_xmit);
      }
    }

    return;
  }

  //
  // The remote address is registered, so we can do a direct PUT.
  //

  //
  // Fill in the POST descriptor.
  //
  post_desc.type            = GNI_POST_FMA_PUT;
  post_desc.cq_mode         = GNI_CQMODE_GLOBAL_EVENT;
  post_desc.dlvr_mode       = GNI_DLVMODE_PERFORMANCE;
  post_desc.rdma_mode       = 0;
  post_desc.src_cq_hndl     = 0;

  //
  // If the PUT size merits RDMA and the source addr is registered,
  // then do an RDMA put instead of FMA
  //
 if (size >= rdma_threshold &&
     (local_mr = mreg_for_local_addr(src_addr)) != NULL) {
    do_rdma = true;
    max_trans_sz = MAX_RDMA_TRANS_SZ;

    post_desc.type = GNI_POST_RDMA_PUT;
    post_desc.local_mem_hndl = local_mr->mdh;
  }


  //
  // If the transfer is larger than the maximum transaction length,
  // then we have to break it into smaller pieces.
  //
  while (size > 0) {
    size_t tsz;

    tsz = (size <= max_trans_sz) ? size : max_trans_sz;

    post_desc.local_addr      = (uint64_t) (intptr_t) src_addr;
    post_desc.remote_addr     = (uint64_t) (intptr_t) tgt_addr;
    post_desc.remote_mem_hndl = remote_mr->mdh;
    post_desc.length          = tsz;

    src_addr = (char*) src_addr + tsz;
    tgt_addr = (char*) tgt_addr + tsz;
    size -= tsz;

    //
    // Initiate the transaction and wait for it to complete.
    //
    PERFSTATS_INC(put_cnt);
    PERFSTATS_ADD(put_byte_cnt, tsz);

    if (do_rdma) {
      post_rdma_and_wait(locale, &post_desc, true);
    } else {
      post_fma_and_wait(locale, &post_desc, true);
    }
  }
}


static
void do_remote_put_V(int v_len, void** src_addr_v, c_nodeid_t* locale_v,
                     void** tgt_addr_v, size_t* size_v,
                     mem_region_t** remote_mr_v, drpg_may_proxy_t may_proxy)
{
  DBG_P_LP(DBGF_GETPUT, "DoRemPutV(%d) %p -> %d:%p (%#zx), proxy %c",
           v_len, src_addr_v[0], (int) locale_v[0], tgt_addr_v[0], size_v[0],
           may_proxy ? 'y' : 'n');

#if HAVE_GNI_FMA_CHAIN_TRANSACTIONS

  //
  // This GNI is new enough to support chained transactions.
  //

  //
  // If there are more than we can handle at once, block them up.
  //
  while (v_len > MAX_CHAINED_PUT_LEN) {
    do_remote_put_V(MAX_CHAINED_PUT_LEN, src_addr_v, locale_v, tgt_addr_v,
                    size_v, remote_mr_v, may_proxy);
    v_len -= MAX_CHAINED_PUT_LEN;
    src_addr_v += MAX_CHAINED_PUT_LEN;
    locale_v += MAX_CHAINED_PUT_LEN;
    tgt_addr_v += MAX_CHAINED_PUT_LEN;
    size_v += MAX_CHAINED_PUT_LEN;
    remote_mr_v += MAX_CHAINED_PUT_LEN;
  }

  if (v_len <= 0)
    return;

  //
  // Do all these PUTs in one chained transaction.  Except: if we have to
  // proxy any of these PUTs because they refer to unregistered memory on
  // the remote side then defer to the scalar PUT routine for that.
  //
  int vi, ci = -1;
  mem_region_t* remote_mr;
  gni_post_descriptor_t pd;
  gni_ct_put_post_descriptor_t pdc[MAX_CHAINED_PUT_LEN - 1];

  for (vi = 0, ci = -1; vi < v_len; vi++) {
    remote_mr = ((remote_mr_v == NULL)
                 ? mreg_for_remote_addr(tgt_addr_v[vi], locale_v[vi])
                 : remote_mr_v[vi]);
    if (remote_mr == NULL) {
      if (may_proxy) {
        do_remote_put(src_addr_v[vi], locale_v[vi], tgt_addr_v[vi], size_v[vi],
                      NULL, may_proxy);
      } else {
        CHPL_INTERNAL_ERROR("do_remote_put_V(): "
                            "remote address is not NIC-registered");
      }
      continue;
    }

    if (ci == -1) {
      pd.next_descr      = NULL;
      pd.type            = GNI_POST_FMA_PUT;
      pd.cq_mode         = GNI_CQMODE_GLOBAL_EVENT;
      pd.dlvr_mode       = GNI_DLVMODE_PERFORMANCE;
      pd.rdma_mode       = 0;
      pd.src_cq_hndl     = 0;
      pd.local_addr      = (uint64_t) (intptr_t) src_addr_v[vi];
      pd.remote_addr     = (uint64_t) (intptr_t) tgt_addr_v[vi];
      pd.remote_mem_hndl = remote_mr->mdh;
      pd.length          = size_v[vi];

      PERFSTATS_INC(put_cnt);
      PERFSTATS_ADD(put_byte_cnt, size_v[vi]);
    } else {
      if (ci == 0)
        pd.next_descr = &pdc[0];
      else
        pdc[ci - 1].next_descr = &pdc[ci];

      pdc[ci].next_descr      = NULL;
      pdc[ci].local_addr      = (uint64_t) (intptr_t) src_addr_v[vi];
      pdc[ci].remote_addr     = (uint64_t) (intptr_t) tgt_addr_v[vi];
      pdc[ci].remote_mem_hndl = remote_mr->mdh;
      pdc[ci].length          = size_v[vi];

      PERFSTATS_INC(put_cnt);
      PERFSTATS_ADD(put_byte_cnt, size_v[vi]);
    }

    ci++;
  }

  if (ci != -1)
    post_fma_ct_and_wait(locale_v, &pd);

#else // HAVE_GNI_FMA_CHAIN_TRANSACTIONS

  //
  // This GNI is too old to support chained transactions.  Just do
  // normal ones.
  //
  for (int vi = 0; vi < v_len; vi++) {
    do_remote_put(src_addr_v[vi], locale_v[vi], tgt_addr_v[vi], size_v[vi],
                  (remote_mr_v == NULL) ? NULL : remote_mr_v[vi], may_proxy);
  }

#endif // HAVE_GNI_FMA_CHAIN_TRANSACTIONS
}


static
void do_remote_get_V(int v_len, void** tgt_addr_v, c_nodeid_t* locale_v,
                     mem_region_t** remote_mr_v, void** src_addr_v,
                     size_t* size_v, mem_region_t** local_mr_v,
                     drpg_may_proxy_t may_proxy)
{

  DBG_P_LP(DBGF_GETPUT, "DoRemGetV(%d) %p <- %d:%p (%#zx), proxy %c",
           v_len, tgt_addr_v[0], (int) locale_v[0], src_addr_v[0], size_v[0],
           may_proxy ? 'y' : 'n');

#if HAVE_GNI_FMA_CHAIN_TRANSACTIONS

  //
  // This GNI is new enough to support chained transactions.
  //

  //
  // If there are more than we can handle at once, block them up.
  //
  while (v_len > MAX_CHAINED_GET_LEN) {
    do_remote_get_V(MAX_CHAINED_GET_LEN, tgt_addr_v, locale_v, remote_mr_v,
                    src_addr_v, size_v, local_mr_v, may_proxy);
    v_len -= MAX_CHAINED_GET_LEN;
    tgt_addr_v += MAX_CHAINED_GET_LEN;
    locale_v += MAX_CHAINED_GET_LEN;
    remote_mr_v += MAX_CHAINED_GET_LEN;
    src_addr_v += MAX_CHAINED_GET_LEN;
    size_v += MAX_CHAINED_GET_LEN;
    local_mr_v  += MAX_CHAINED_GET_LEN;
  }

  if (v_len <= 0)
    return;

  //
  // Do all these GETs in one chained transaction.  Except: if we have to
  // proxy any of these GETs because they refer to unregistered memory on
  // the remote side then defer to the scalar PUT routine for that.
  //
  int vi, ci = -1;
  mem_region_t* local_mr;
  mem_region_t* remote_mr;
  gni_post_descriptor_t pd;
  gni_ct_get_post_descriptor_t pdc[MAX_CHAINED_GET_LEN - 1];

  for (vi = 0, ci = -1; vi < v_len; vi++) {
    local_mr = ((local_mr_v == NULL)
                ? mreg_for_local_addr(tgt_addr_v[vi])
                : local_mr_v[vi]);
    remote_mr = ((remote_mr_v == NULL)
                 ? mreg_for_remote_addr(src_addr_v[vi], locale_v[vi])
                 : remote_mr_v[vi]);
    if (local_mr == NULL || remote_mr == NULL) {
      if (may_proxy) {
        do_remote_get(tgt_addr_v[vi], locale_v[vi], src_addr_v[vi], size_v[vi],
                      may_proxy);
      } else {
        CHPL_INTERNAL_ERROR("do_remote_get_V(): address is not NIC-registered");
      }
      continue;
    }

    if (ci == -1) {
      pd.next_descr      = NULL;
      pd.type            = GNI_POST_FMA_GET;
      pd.cq_mode         = GNI_CQMODE_GLOBAL_EVENT;
      pd.dlvr_mode       = GNI_DLVMODE_PERFORMANCE;
      pd.rdma_mode       = 0;
      pd.src_cq_hndl     = 0;
      pd.local_addr      = (uint64_t) (intptr_t) tgt_addr_v[vi];
      pd.remote_addr     = (uint64_t) (intptr_t) src_addr_v[vi];
      pd.local_mem_hndl  = local_mr->mdh;
      pd.remote_mem_hndl = remote_mr->mdh;
      pd.length          = size_v[vi];

      PERFSTATS_INC(get_cnt);
      PERFSTATS_ADD(get_byte_cnt, size_v[vi]);
    } else {
      if (ci == 0)
        pd.next_descr = &pdc[0];
      else
        pdc[ci - 1].next_descr = &pdc[ci];

      pdc[ci].next_descr      = NULL;
      pdc[ci].local_addr      = (uint64_t) (intptr_t) tgt_addr_v[vi];
      pdc[ci].remote_addr     = (uint64_t) (intptr_t) src_addr_v[vi];
      pdc[ci].local_mem_hndl  = local_mr->mdh;
      pdc[ci].remote_mem_hndl = remote_mr->mdh;
      pdc[ci].length          = size_v[vi];

      PERFSTATS_INC(get_cnt);
      PERFSTATS_ADD(get_byte_cnt, size_v[vi]);
    }

    ci++;
  }

  if (ci != -1)
    post_fma_ct_and_wait(locale_v, &pd);

#else // HAVE_GNI_FMA_CHAIN_TRANSACTIONS

  //
  // This GNI is too old to support chained transactions.  Just do
  // normal ones.
  //
  for (int vi = 0; vi < v_len; vi++) {
    do_remote_get(tgt_addr_v[vi], locale_v[vi], src_addr_v[vi], size_v[vi],
                  may_proxy);
  }

#endif // HAVE_GNI_FMA_CHAIN_TRANSACTIONS
}


static
void do_nic_amo_nf_V(int v_len, uint64_t* opnd1_v, c_nodeid_t* locale_v,
                     void** object_v, size_t* size_v,
                     gni_fma_cmd_type_t* cmd_v, mem_region_t** remote_mr_v)
{

#if HAVE_GNI_FMA_CHAIN_TRANSACTIONS

  //
  // This GNI is new enough to support chained transactions.
  //

  //
  // If there are more than we can handle at once, block them up.
  //
  while (v_len > MAX_CHAINED_AMO_LEN) {
    do_nic_amo_nf_V(MAX_CHAINED_PUT_LEN, opnd1_v, locale_v, object_v,
                    size_v, cmd_v, remote_mr_v);
    v_len -= MAX_CHAINED_AMO_LEN;
    opnd1_v += MAX_CHAINED_AMO_LEN;
    locale_v += MAX_CHAINED_AMO_LEN;
    object_v += MAX_CHAINED_AMO_LEN;
    size_v += MAX_CHAINED_AMO_LEN;
    cmd_v += MAX_CHAINED_AMO_LEN;
    remote_mr_v += MAX_CHAINED_AMO_LEN;
  }

  gni_post_descriptor_t post_desc;
  gni_ct_amo_post_descriptor_t pdc[MAX_CHAINED_AMO_LEN - 1];
  int vi, ci;

  if (v_len <= 0)
    return;

  //
  // Build up the base post descriptor
  //
  post_desc.next_descr      = NULL;
  post_desc.type            = GNI_POST_AMO;
  post_desc.cq_mode         = GNI_CQMODE_GLOBAL_EVENT;
  post_desc.dlvr_mode       = GNI_DLVMODE_PERFORMANCE;
  post_desc.rdma_mode       = 0;
  post_desc.src_cq_hndl     = 0;
  post_desc.remote_addr     = (uint64_t) (intptr_t) object_v[0];
  post_desc.remote_mem_hndl = remote_mr_v[0]->mdh;
  post_desc.length          = size_v[0];
  post_desc.amo_cmd         = cmd_v[0];
  post_desc.first_operand   = opnd1_v[0];

  //
  // Build up the chain of descriptors
  //
  for (vi=1, ci=0; vi<v_len; vi++, ci++) {
    if (ci == 0)
      post_desc.next_descr  = &pdc[0];
    else
      pdc[ci-1].next_descr  = &pdc[ci];
    pdc[ci].next_descr      = NULL;
    pdc[ci].remote_addr     = (uint64_t) (intptr_t) object_v[vi];
    pdc[ci].remote_mem_hndl = remote_mr_v[vi]->mdh;
    pdc[ci].length          = size_v[vi];
    pdc[ci].amo_cmd         = cmd_v[vi];
    pdc[ci].first_operand   = opnd1_v[vi];
  }

  //
  // Initiate the transaction and wait for it to complete.
  //
  post_fma_ct_and_wait(locale_v, &post_desc);

#else // HAVE_GNI_FMA_CHAIN_TRANSACTIONS

  //
  // This GNI is too old to support chained transactions.  Just do
  // normal ones.
  //
  for (int vi = 0; vi < v_len; vi++) {
    do_nic_amo_nf(&opnd1_v[vi], locale_v[vi], object_v[vi], size_v[vi],
                  cmd_v[vi], remote_mr_v[vi]);
  }

#endif // HAVE_GNI_FMA_CHAIN_TRANSACTIONS
}


void chpl_comm_getput_unordered(c_nodeid_t dst_locale, void* dst_addr,
                                c_nodeid_t src_locale, void* src_addr,
                                size_t size, int32_t commID,
                                int ln, int32_t fn)
{
  assert(dst_addr != NULL);
  assert(src_addr != NULL);

  if (size == 0)
    return;

  if (dst_locale == chpl_nodeID && src_locale == chpl_nodeID) {
    memmove(dst_addr, src_addr, size);
    return;
  }

  if (dst_locale == chpl_nodeID) {
    chpl_comm_get_unordered(dst_addr, src_locale, src_addr, size, commID, ln, fn);
  } else if (src_locale == chpl_nodeID) {
    chpl_comm_put_unordered(src_addr, dst_locale, dst_addr, size, commID, ln, fn);
  } else {
    // TODO use unordered ops in this case? Would have to ensure we always
    // flush GET buffer before the PUT buffer
    if (size <= MAX_UNORDERED_TRANS_SZ) {
      char buf[MAX_UNORDERED_TRANS_SZ];
      chpl_comm_get(buf, src_locale, src_addr, size, commID, ln, fn);
      chpl_comm_put(buf, dst_locale, dst_addr, size, commID, ln, fn);
    } else {
      // Note, we do not expect this case to trigger, but if it does we may
      // want to do on-stmt to src locale and then transfer
      char* buf = chpl_mem_alloc(size, CHPL_RT_MD_COMM_PER_LOC_INFO, 0, 0);
      chpl_comm_get(buf, src_locale, src_addr, size, commID, ln, fn);
      chpl_comm_put(buf, dst_locale, dst_addr, size, commID, ln, fn);
      chpl_mem_free(buf, 0, 0);
    }
  }
}

void chpl_comm_get_unordered(void* addr, c_nodeid_t locale, void* raddr,
                             size_t size, int32_t commID, int ln, int32_t fn)
{
  DBG_P_LP(DBGF_IFACE|DBGF_GETPUT, "IFACE chpl_comm_get_unordered(%p, %d, %p, %zd)",
           addr, (int) locale, raddr, size);

  assert(addr != NULL);
  assert(raddr != NULL);
  if (size == 0)
    return;

  if (locale == chpl_nodeID) {
    memmove(addr, raddr, size);
    return;
  }

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_get)) {
      chpl_comm_cb_info_t cb_data =
        {chpl_comm_cb_event_kind_get, chpl_nodeID, locale,
         .iu.comm={addr, raddr, size, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_rdma("unordered get", locale, size, ln, fn, commID);
  chpl_comm_diags_incr(get);

  do_remote_get_buff(addr, locale, raddr, size, may_proxy_true);
}

void chpl_comm_put_unordered(void* addr, c_nodeid_t locale, void* raddr,
                             size_t size, int32_t commID, int ln, int32_t fn)

{
  DBG_P_LP(DBGF_IFACE|DBGF_GETPUT, "IFACE chpl_comm_put_unordered(%p, %d, %p, %zd)",
           addr, (int) locale, raddr, size);

  assert(addr != NULL);
  assert(raddr != NULL);
  if (size == 0)
    return;

  if (locale == chpl_nodeID) {
    memmove(raddr, addr, size);
    return;
  }

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_put)) {
      chpl_comm_cb_info_t cb_data =
        {chpl_comm_cb_event_kind_put, chpl_nodeID, locale,
         .iu.comm={addr, raddr, size, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_rdma("unordered put", locale, size, ln, fn, commID);
  chpl_comm_diags_incr(put);

  do_remote_put_buff(addr, locale, raddr, size, may_proxy_true);
}

void chpl_comm_getput_unordered_task_fence(void) {
  task_local_buff_flush(get_buff | put_buff);
}


void chpl_comm_get(void* addr, c_nodeid_t locale, void* raddr,
                   size_t size, int32_t commID, int ln, int32_t fn)
{
  DBG_P_LP(DBGF_IFACE|DBGF_GETPUT, "IFACE chpl_comm_get(%p, %d, %p, %zd)",
           addr, (int) locale, raddr, size);

  assert(addr != NULL);
  assert(raddr != NULL);
  if (size == 0)
    return;

  if (locale == chpl_nodeID) {
    memmove(addr, raddr, size);
    return;
  }

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_get)) {
      chpl_comm_cb_info_t cb_data = 
        {chpl_comm_cb_event_kind_get, chpl_nodeID, locale,
         .iu.comm={addr, raddr, size, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_rdma("get", locale, size, ln, fn, commID);
  chpl_comm_diags_incr(get);

  do_remote_get(addr, locale, raddr, size, may_proxy_true);
}

/*
 *** START OF BUFFERED PUT OPERATIONS ***
 *
 * Support for buffered PUT operations. We internally buffer PUT operations and
 * then initiate them with chained transactions for increased transaction rate.
 */

// Flush buffered PUTs for the specified task info and reset the counter.
static inline
void put_buff_task_info_flush(put_buff_task_info_t* info) {
  if (info->vi > 0) {
    do_remote_put_V(info->vi, info->src_addr_v, info->locale_v,
                    info->tgt_addr_v, info->size_v,
                    info->remote_mr_v, may_proxy_true);
    info->vi = 0;
  }
}

static inline
void do_remote_put_buff(void* src_addr, c_nodeid_t locale, void* tgt_addr,
                        size_t size, drpg_may_proxy_t may_proxy)
{
  mem_region_t*         remote_mr;
  put_buff_task_info_t* info;

  DBG_P_LP(DBGF_GETPUT, "DoRemBuffPut %p -> %d:%p (%#zx), proxy %c",
           src_addr, (int) locale, tgt_addr, size, may_proxy ? 'y' : 'n');

  remote_mr = mreg_for_remote_addr(tgt_addr, locale);
  info = task_local_buff_acquire(put_buff);

  if (remote_mr == NULL || info == NULL || size > MAX_UNORDERED_TRANS_SZ) {
    do_remote_put(src_addr, locale, tgt_addr, size, remote_mr, may_proxy);
    return;
  }

  int vi = info->vi;
  memcpy(&info->src_v[vi], src_addr, size);
  info->src_addr_v[vi] = &info->src_v[vi];
  info->locale_v[vi] = locale;
  info->tgt_addr_v[vi] = tgt_addr;
  info->size_v[vi] = size;
  info->remote_mr_v[vi] = remote_mr;
  info->vi++;

  // flush if buffers are full
  if (info->vi == MAX_CHAINED_PUT_LEN) {
    put_buff_task_info_flush(info);
  }
}
/*** END OF BUFFERED PUT OPERATIONS ***/


/*
 *** START OF BUFFERED GET OPERATIONS ***
 *
 * Support for buffered GET operations. We internally buffer GET operations and
 * then initiate them with chained transactions for increased transaction rate.
 */

// Flush buffered GETs for the specified task info and reset the counter.
static inline
void get_buff_task_info_flush(get_buff_task_info_t* info) {
  if (info->vi > 0) {
    do_remote_get_V(info->vi, info->tgt_addr_v, info->locale_v,
                    info->remote_mr_v, info->src_addr_v, info->size_v,
                    info->local_mr_v, may_proxy_true);
    info->vi = 0;
  }
}

static inline
void do_remote_get_buff(void* tgt_addr, c_nodeid_t locale, void* src_addr,
                        size_t size, drpg_may_proxy_t may_proxy)
{
  mem_region_t*         local_mr;
  mem_region_t*         remote_mr;
  get_buff_task_info_t* info;

  DBG_P_LP(DBGF_GETPUT, "DoRemBuffGet %p <- %d:%p (%#zx), proxy %c",
           tgt_addr, (int) locale, src_addr, size, may_proxy ? 'y' : 'n');

  //
  // In order to do direct NIC gets the address must be registered and the
  // addresses and size must be 4-byte aligned. do_remote_get handles cases if
  // any of those aren't true, so just have it take care of things instead of
  // trying to reimplement that logic in do_remote_get_V. If any of these
  // aren't true, our performance is already going to suffer.
  //
  remote_mr = mreg_for_remote_addr(src_addr, locale);
  local_mr = mreg_for_local_addr(tgt_addr);
  info = task_local_buff_acquire(get_buff);
  if (local_mr == NULL || remote_mr == NULL || info == NULL ||
      !IS_ALIGNED_32((size_t) (intptr_t) src_addr) ||
      !IS_ALIGNED_32((size_t) (intptr_t) tgt_addr) ||
      !IS_ALIGNED_32(size) || size > MAX_UNORDERED_TRANS_SZ) {
    do_remote_get(tgt_addr, locale, src_addr, size, may_proxy);
    return;
  }
 
  int vi = info->vi;
  info->tgt_addr_v[vi] = tgt_addr;
  info->locale_v[vi] = locale;
  info->remote_mr_v[vi] = remote_mr;
  info->src_addr_v[vi] = src_addr;
  info->size_v[vi] = size;
  info->local_mr_v[vi] = local_mr;
  info->vi++;

  // flush if buffers are full
  if (info->vi == MAX_CHAINED_GET_LEN) {
    get_buff_task_info_flush(info);
  }
}
/*** END OF BUFFERED GET OPERATIONS ***/


static
void do_remote_get(void* tgt_addr, c_nodeid_t locale, void* src_addr,
                   size_t size, drpg_may_proxy_t may_proxy)
{
  mem_region_t*         local_mr;
  mem_region_t*         remote_mr;
  void*                 tgt_addr_xmit;
  uint64_t              xmit_size;
  void*                 src_addr_xmit;
  uint64_t              src_addr_xmit_off;

  DBG_P_LP(DBGF_GETPUT, "DoRemGet %p <- %d:%p (%#zx), proxy %c",
           tgt_addr, (int) locale, src_addr, size, may_proxy ? 'y' : 'n');

  //
  // The source address for a GET has to be registered with the remote
  // NIC.  If it isn't, then instead of our doing a GET we arrange for
  // the remote side to do a PUT to us, handling the unregistered
  // source address itself.  We also need the target address here to
  // be registered, but we don't have to worry about alignment or the
  // minimum-size issues associated with direct GETs.
  //
  remote_mr = mreg_for_remote_addr(src_addr, locale);
  if (remote_mr == NULL) {
    if (!may_proxy) {
      CHPL_INTERNAL_ERROR("do_remote_get(): "
                          "remote address is not NIC-registered");
    }

    local_mr = mreg_for_local_addr(tgt_addr);
    if (local_mr != NULL) {
      //
      // The local target address is NIC-registered.  The source side
      // can do a direct PUT to us.
      //
      fork_put(tgt_addr, locale, src_addr, size);
    }
    else {
      //
      // The local target address is not NIC-registered.  Transfer
      // through a trampoline buffer.
      //
      if (size <= gbp_max_size) {
        //
        // The transfer will fit in a single trampoline buffer.
        //
        tgt_addr_xmit = get_buf_alloc(size);
        fork_put(tgt_addr_xmit, locale, src_addr, size);
        memcpy(tgt_addr, tgt_addr_xmit, size);
        get_buf_free(tgt_addr_xmit);
      }
      else {
        //
        // The transfer is larger than the largest trampoline buffer.
        // Do it in pieces.
        // 
        tgt_addr_xmit = get_buf_alloc(gbp_max_size);

        do {
          fork_put(tgt_addr_xmit, locale, src_addr, gbp_max_size);
          memcpy(tgt_addr, tgt_addr_xmit, gbp_max_size);
          tgt_addr = (char*) tgt_addr + gbp_max_size;
          src_addr = (char*) src_addr + gbp_max_size;
          size -= gbp_max_size;
        } while (size > gbp_max_size);

        fork_put(tgt_addr_xmit, locale, src_addr, size);
        memcpy(tgt_addr, tgt_addr_xmit, size);
        get_buf_free(tgt_addr_xmit);
      }
    }

    return;
  }

  //
  // The remote address is registered, so we can do a direct GET.
  //

  //
  // If the local address isn't in a memory region known to the NIC, the
  // address is unaligned, or the size is unaligned or too short, do the
  // GET into a trampoline buffer instead.  We will copy the result
  // where it needs to go once it arrives.  The variables involved are
  // as follows:
  //
  //   tgt_addr:          caller's local target address
  //   tgt_addr_xmit:     NIC GET local target address; may differ from
  //                      tgt_addr due to registration requirements, or
  //                      alignment or length restrictions
  //   src_addr:          caller's remote source address
  //   src_addr_xmit:     NIC GET remote source address, aligned
  //                      downward to NIC requirements
  //   src_addr_xmit_off: offset of caller's remote source address from
  //                      that used for NIC GET
  //   xmit_size:         NIC GET size, including remote offset and GET
  //                      alignment requirements
  //
  // In the end we GET xmit_size bytes from src_addr_xmit, and then if
  // we need to we copy the data from a trampoline buffer to the user's
  // target address.
  //
  tgt_addr_xmit     = tgt_addr;
  src_addr_xmit     = UI64_TO_VP(ALIGN_32_DN(VP_TO_UI64(src_addr)));
  src_addr_xmit_off = VP_TO_UI64(src_addr) - VP_TO_UI64(src_addr_xmit);
  xmit_size         = ALIGN_32_UP(size + src_addr_xmit_off);

  local_mr = mreg_for_local_addr(tgt_addr_xmit);
  if (local_mr != NULL
      && IS_ALIGNED_32((size_t) (intptr_t) tgt_addr)
      && src_addr_xmit == src_addr
      && xmit_size == size) {
    //
    // The remote and local addresses are both registered and aligned,
    // and the length is aligned, so we can do a direct transfer.
    //
    do_nic_get(tgt_addr, locale, remote_mr, src_addr, size, local_mr);
    return;
  }

  local_mr = gnr_mreg;

  if (xmit_size <= gbp_max_size) {
    //
    // The transfer will fit in a single trampoline buffer.
    //
    tgt_addr_xmit = get_buf_alloc(size);

    do_nic_get(tgt_addr_xmit, locale, remote_mr,
               src_addr_xmit, xmit_size, gnr_mreg);
    memcpy(tgt_addr, (char*) tgt_addr_xmit + src_addr_xmit_off, size);

    get_buf_free(tgt_addr_xmit);
  }
  else {
    //
    // The transfer is larger than the largest trampoline buffer.
    // Do it in pieces.
    //
    tgt_addr_xmit = get_buf_alloc(gbp_max_size);

    // In the first chunk we handle src start address misalignment.
    do_nic_get(tgt_addr_xmit, locale, remote_mr,
               src_addr_xmit, gbp_max_size, gnr_mreg);
    memcpy(tgt_addr, (char*) tgt_addr_xmit + src_addr_xmit_off,
           gbp_max_size - src_addr_xmit_off);
    tgt_addr = (char*) tgt_addr + gbp_max_size - src_addr_xmit_off;
    src_addr_xmit = (char*) src_addr_xmit + gbp_max_size;
    xmit_size -= gbp_max_size;

    // The middle chunks are all full ones.
    while (xmit_size > gbp_max_size) {
      do_nic_get(tgt_addr_xmit, locale, remote_mr,
                 src_addr_xmit, gbp_max_size, gnr_mreg);
      memcpy(tgt_addr, tgt_addr_xmit, gbp_max_size);
      tgt_addr = (char*) tgt_addr + gbp_max_size;
      src_addr_xmit = (char*) src_addr_xmit + gbp_max_size;
      xmit_size -= gbp_max_size;
    }

    // In the last chunk chunk we handle length misalignment.
    do_nic_get(tgt_addr_xmit, locale, remote_mr,
               src_addr_xmit, xmit_size, gnr_mreg);
    memcpy(tgt_addr, tgt_addr_xmit, (size + src_addr_xmit_off) % gbp_max_size);

    get_buf_free(tgt_addr_xmit);
  }
}


static inline
void do_nic_get(void* tgt_addr, c_nodeid_t locale, mem_region_t* remote_mr,
                void* src_addr, size_t size, mem_region_t* local_mr)
{
  gni_post_descriptor_t post_desc;
  chpl_bool do_rdma = false;
  size_t max_trans_sz = MAX_FMA_TRANS_SZ;

  //
  // Assumes remote and local addresses are both registered and aligned,
  // and length is aligned, so we can do a direct transfer.
  //

  //
  // Fill in the POST descriptor.
  //
  post_desc.type            = GNI_POST_FMA_GET;
  post_desc.cq_mode         = GNI_CQMODE_GLOBAL_EVENT;
  post_desc.dlvr_mode       = GNI_DLVMODE_PERFORMANCE;
  post_desc.rdma_mode       = 0;
  post_desc.src_cq_hndl     = 0;

  //
  // If the GET size merits RDMA do an RDMA get instead of FMA
  //
 if (size >= rdma_threshold) {
    do_rdma = true;
    max_trans_sz = MAX_RDMA_TRANS_SZ;
    post_desc.type = GNI_POST_RDMA_GET;
  }

  //
  // If the transfer is larger than the maximum transaction length,
  // then we have to break it into smaller pieces.
  //
  while (size > 0) {
    size_t tsz;

    tsz = (size <= max_trans_sz) ? size : max_trans_sz;

    post_desc.local_addr      = (uint64_t) (intptr_t) tgt_addr;
    post_desc.local_mem_hndl  = local_mr->mdh;
    post_desc.remote_addr     = (uint64_t) (intptr_t) src_addr;
    post_desc.remote_mem_hndl = remote_mr->mdh;
    post_desc.length          = tsz;

    src_addr = (char*) src_addr + tsz;
    tgt_addr = (char*) tgt_addr + tsz;
    size -= tsz;

    //
    // Initiate the transaction and wait for it to complete.
    //
    PERFSTATS_INC(get_cnt);
    PERFSTATS_ADD(get_byte_cnt, tsz);

    if (do_rdma) {
      post_rdma_and_wait(locale, &post_desc, true);
    } else {
      post_fma_and_wait(locale, &post_desc, true);
    }

  }
}


//
// Strided bulk put/get, adapted from comm-none.c
//


//
// We use non-blocking transactions for the multiple GETs/PUTs of a
// strided transfer.  We can only track so many of these in flight at
// once, obviously.  In practice based on quick experimentation, the
// XC network is only about 10 transactions "wide" at the rate we can
// initiate them in serial code here.  For simplicity, set ourselves
// a static limit a bit larger than that and support up to 20 GETs or
// PUTs in flight at once, subject also to any per-CD limit.
//
// Prefer initiating transactions to retiring them, in order to get
// them started earlier.  Once we have as many transactions in flight
// as we can track and are thus forced to retire some in order to
// initiate more, only retire ones that have already completed.  Don't
// wait for any more to finish.  Then at the end, wait for everything
// to complete.
//
static const size_t strd_maxHandles
                    = (20 < CD_ACTIVE_TRANS_MAX) ? 20 : CD_ACTIVE_TRANS_MAX;

void chpl_comm_put_strd(void* dstaddr_arg, size_t* dststrides,
                        int32_t dstlocale,
                        void* srcaddr_arg, size_t* srcstrides,
                        size_t* count, int32_t stridelevels, size_t elemSize,
                        int32_t commID, int ln, int32_t fn)
{
  PERFSTATS_INC(put_strd_cnt);
  put_strd_common(dstaddr_arg, dststrides,
                  dstlocale,
                  srcaddr_arg, srcstrides,
                  count, stridelevels, elemSize,
                  strd_maxHandles, local_yield,
                  commID, ln, fn);
}


void chpl_comm_get_strd(void* dstaddr_arg, size_t* dststrides,
                        int32_t srclocale,
                        void* srcaddr_arg, size_t* srcstrides,
                        size_t* count, int32_t stridelevels, size_t elemSize,
                        int32_t commID, int ln, int32_t fn)
{
  PERFSTATS_INC(get_strd_cnt);
  get_strd_common(dstaddr_arg, dststrides,
                  srclocale,
                  srcaddr_arg, srcstrides,
                  count, stridelevels, elemSize,
                  strd_maxHandles, local_yield,
                  commID, ln, fn);
}


//
// Non-blocking get interface
//
chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, c_nodeid_t locale,
                                       void* raddr, size_t size,
                                       int32_t commID, int ln, int32_t fn)
{

  DBG_P_LP(DBGF_IFACE|DBGF_GETPUT, "IFACE chpl_comm_get_nb(%p, %d, %p, %zd)",
           addr, (int) locale, raddr, size);

  chpl_comm_get(addr, locale, raddr, size, commID, ln, fn);
  return NULL;
}


chpl_comm_nb_handle_t chpl_comm_put_nb(void* addr, c_nodeid_t locale,
                                       void* raddr, size_t size,
                                       int32_t commID, int ln, int32_t fn)
{
  DBG_P_LP(DBGF_IFACE|DBGF_GETPUT, "IFACE chpl_comm_put_nb(%p, %d, %p, %zd)",
           addr, (int) locale, raddr, size);

  chpl_comm_put(addr, locale, raddr, size, commID, ln, fn);
  return NULL;
}


int chpl_comm_test_nb_complete(chpl_comm_nb_handle_t h)
{
  chpl_comm_diags_incr(test_nb);

  PERFSTATS_INC(test_nb_cnt);

  return h == NULL;
}


void chpl_comm_wait_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles)
{
  chpl_comm_diags_incr(wait_nb);

  PERFSTATS_INC(wait_nb_cnt);
}


int chpl_comm_try_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles)
{
  chpl_comm_diags_incr(try_nb);

  PERFSTATS_INC(try_nb_cnt);

  return 0;
}


int chpl_comm_addr_gettable(c_nodeid_t node, void* start, size_t len)
{
  //
  // We assume here that no object can span a boundary between mapped
  // and unmapped memory, so we only have to check the start address.
  //
  return mreg_for_remote_addr(start, node) != NULL;
}


//
// Atomic operations done by Gemini or Aries are not coherent with
// respect to operations done by the processor.  This includes not
// only atomics done by the processor but also non-atomic equivalents
// to the atomic operations and indeed even loads and stores.  Thus
// ALL the operations on any given atomic variable have to be done
// using either the NIC or the processor. This is why, in addition to
// the bitwise and arithmetic operations, we have GET and PUT here.
//
// For performance, we use the NIC to do atomic operations directly
// over the network whenever we can.  When either the operation or the
// operand type is not supported by the NIC, we do a fast fork to the
// target datum's node and do the operation there.  Whether we use the
// CPU or the NIC to perform the operation is driven by the need for
// coherence with other network AMOs that might operate on the same
// datum.
//
// On Gemini, we only have 64-bit integer AMOs.  Those are all done
// by the NIC.  We don't have a 64-bit exchange, but we emulate that
// with FAX (Fetch and And-Xor).  We also don't have FP AMOs, so FP
// add is emulated using compare-exchange.  32-bit AMOs are all done
// by the CPU, via remote fork.
//
// The Aries 64-bit FP add AMO is broken in hardware and disabled in
// ugni.  It is emulated using compare-exchange on the NIC.
//
// Note that since the Gemini implementation is split, with 32-bit
// AMOs done by the CPU and 64-bit ones done by the NIC, one cannot
// safely do 32-bit integer operations on the halves of 64-bit
// objects, or 64-bit integer operations on pairs of 32-bit objects.
// None of these seem like likely desires, however.
//
// As historical information, at first the 32-bit remote atomics were
// done using 64-bit operations on the qword memory locations that
// contained the dword objects.  The bitwise integer operations were
// done directly, with identity values in the "other" (non-target)
// dwords of the qword containers.  The rest were done via remote fork
// on the target locale, with the actual operation computed on the
// processor, and a 64-bit NIC CSWAP being used to replace the old
// value of the target dword with the new value while leaving the
// other dword unchanged.  This was cute but broken. The problem had
// to do with the other dword.  The NIC/processor incoherence issue
// for AMOs required that if that other dword were the residence of a
// regular variable then it had to be picked up and restored with
// processor loads and stores, and if it were the residence of an
// atomic variable then it had to be picked up and restored with NIC
// GETs and PUTs.  But there was no way to tell which kind of variable
// it was, and thus no safe way to manipulate it.  One way to work
// around this problem would have been to pad all 32-bit atomic
// objects to 64 bits, but the technique we're now using performs
// nearly as well while not wasting memory.
//
// TODO: When the operation or type is not supported natively by the
//       NIC but the target datum is local, we should do the operation
//       directly instead of initiating a remote fork to do it.  There
//       is no functional issue here, just a performance improvement.
//

//
// Is this a 32-bit AMO on a Gemini NIC?
//
// Note that the first clause can be evaluated at compile time, so the
// whole expression turns into a check against the NIC type for 32-bit
// AMOs and disappears entirely for 64-bit ones.
//
#define IS_32_BIT_AMO_ON_GEMINI(_t) \
        (sizeof(_t) == sizeof(int32_t) && nic_type == GNI_DEVICE_GEMINI)

//
// Do an AMO by means of an AM to a remote locale.
//   _f: interface function name suffix (type)
//   _c: network AMO command
//   _t: AMO type
//
#define DEFINE_DO_FORK_AMO(_f, _c, _t)                                  \
  static inline void do_fork_amo_##_c##_##_f(void* obj,                 \
                                             void* res,                 \
                                             void* opnd1,               \
                                             void* opnd2,               \
                                             int32_t loc) {             \
    fork_t rf_req = { .a={.cmd=_c, .obj=obj} };                         \
    if (opnd1 != NULL)                                                  \
      memcpy(&rf_req.a.opnd1, opnd1, sizeof(_t));                       \
    if (opnd2 != NULL)                                                  \
      memcpy(&rf_req.a.opnd2, opnd2, sizeof(_t));                       \
    if (res != NULL && mreg_for_local_addr(res) == NULL) {              \
      rf_req.a.res = amo_res_alloc();                                   \
      fork_amo(&rf_req, loc);                                           \
      memcpy(res, rf_req.a.res, sizeof(_t));                            \
      amo_res_free(rf_req.a.res);                                       \
    }                                                                   \
    else {                                                              \
      rf_req.a.res = res;                                               \
      fork_amo(&rf_req, loc);                                           \
    }                                                                   \
  }

//
// Atomic Write functions:
//   _f: interface function name suffix (type)
//   _c: network AMO command
//   _t: AMO type
//
#define DEFINE_CHPL_COMM_ATOMIC_WRITE(_f, _c, _t)                       \
        DEFINE_DO_FORK_AMO(_f, _c, _t)                                  \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_write_##_f(void* val,                     \
                                       int32_t loc,                     \
                                       void* obj,                       \
                                       memory_order order,              \
                                       int ln, int32_t fn)              \
        {                                                               \
          mem_region_t* remote_mr;                                      \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_write_"#_f"(%p, %d, %p)",    \
                   val, (int) loc, obj);                                \
                                                                        \
          if (chpl_numNodes == 1) {                                     \
            atomic_store_##_t((atomic_##_t*) obj, *(_t*) val);          \
            return;                                                     \
          }                                                             \
                                                                        \
          chpl_comm_diags_verbose_amo("amo write", loc, ln, fn);        \
          chpl_comm_diags_incr(amo);                                    \
          if (IS_32_BIT_AMO_ON_GEMINI(_t)                               \
              || (remote_mr = mreg_for_remote_addr(obj, loc)) == NULL) {\
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, NULL, obj, val, NULL);           \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, NULL, val, NULL, loc);       \
          }                                                             \
          else if (nic_type == GNI_DEVICE_GEMINI) {                     \
            /*                                                          \
            ** Gemini PUT and AMO are not coherent (Bug 760752) when    \
            ** the memory segment has GNI_MEM_RELAXED_PI_ORDERING, as   \
            ** ours does.  Therefore, emulate the PUT using a FAX AMO.  \
            ** We don't actually need the result and indeed ignore it,  \
            ** but the corresponding non-fetching AX is broken in       \
            ** hardware and disabled in ugni.                           \
            */                                                          \
            int64_t res;                                                \
            int64_t mask = 0;                                           \
            do_nic_amo(&mask, val, loc, obj, sizeof(_t),                \
                       GNI_FMA_ATOMIC_FAX, &res, remote_mr);            \
          }                                                             \
          else {                                                        \
            do_remote_put(val, loc, obj, sizeof(_t), remote_mr,         \
                          may_proxy_false);                             \
          }                                                             \
        }

DEFINE_CHPL_COMM_ATOMIC_WRITE(int32, put_32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_WRITE(int64, put_64, int_least64_t)
DEFINE_CHPL_COMM_ATOMIC_WRITE(uint32, put_32, uint_least32_t)
DEFINE_CHPL_COMM_ATOMIC_WRITE(uint64, put_64, uint_least64_t)
DEFINE_CHPL_COMM_ATOMIC_WRITE(real32, put_32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_WRITE(real64, put_64, int_least64_t)

#undef DEFINE_CHPL_COMM_ATOMIC_WRITE


//
// Atomic Read functions:
//   _f: interface function name suffix (type)
//   _c: network AMO command
//   _t: AMO type
//
#define DEFINE_CHPL_COMM_ATOMIC_READ(_f, _c, _t)                        \
        DEFINE_DO_FORK_AMO(_f, _c, _t)                                  \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_read_##_f(void* res,                      \
                                       int32_t loc,                     \
                                       void* obj,                       \
                                       memory_order order,              \
                                       int ln, int32_t fn)              \
        {                                                               \
          mem_region_t* remote_mr;                                      \
          mem_region_t* local_mr;                                       \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_read_"#_f"(%p, %d, %p)",     \
                   res, (int) loc, obj);                                \
                                                                        \
          if (chpl_numNodes == 1) {                                     \
            *(_t*) res = atomic_load_##_t((atomic_##_t*) obj);          \
            return;                                                     \
          }                                                             \
                                                                        \
          chpl_comm_diags_verbose_amo("amo read", loc, ln, fn);         \
          chpl_comm_diags_incr(amo);                                    \
          if (IS_32_BIT_AMO_ON_GEMINI(_t)                               \
              || (remote_mr = mreg_for_remote_addr(obj, loc)) == NULL) {\
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, res, obj, NULL, NULL);           \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, res, NULL, NULL, loc);       \
          }                                                             \
          else {                                                        \
            size_t sz = sizeof(_t);                                     \
            if ((local_mr = mreg_for_local_addr(res)) == NULL)          \
              do_remote_get(res, loc, obj, sz, may_proxy_false);        \
            else                                                        \
              do_nic_get(res, loc, remote_mr, obj, sz, local_mr);       \
          }                                                             \
        }

DEFINE_CHPL_COMM_ATOMIC_READ(int32, get_32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_READ(int64, get_64, int_least64_t)
DEFINE_CHPL_COMM_ATOMIC_READ(uint32, get_32, uint_least32_t)
DEFINE_CHPL_COMM_ATOMIC_READ(uint64, get_64, uint_least64_t)
DEFINE_CHPL_COMM_ATOMIC_READ(real32, get_32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_READ(real64, get_64, int_least64_t)

#undef DEFINE_CHPL_COMM_ATOMIC_READ


//
// Atomic Exchange functions:
//   _f: interface function name suffix (type)
//   _c: network AMO command
//   _t: AMO type
//
#define DEFINE_CHPL_COMM_ATOMIC_XCHG(_f, _c, _t)                        \
        DEFINE_DO_FORK_AMO(_f, _c, _t)                                  \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_xchg_##_f(void* xchgval,                  \
                                        int32_t loc,                    \
                                        void* obj,                      \
                                        void* res,                      \
                                        memory_order order,             \
                                        int ln, int32_t fn)             \
        {                                                               \
          mem_region_t* remote_mr;                                      \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_xchg_"#_f"(%p, %d, %p, %p)", \
                   xchgval, (int) loc, obj, res);                       \
                                                                        \
          if (chpl_numNodes == 1) {                                     \
            *(_t*) res =                                                \
              atomic_exchange_##_t((atomic_##_t*) obj, *(_t*) xchgval); \
            return;                                                     \
          }                                                             \
                                                                        \
          chpl_comm_diags_verbose_amo("amo xchg", loc, ln, fn);         \
          chpl_comm_diags_incr(amo);                                    \
          if (IS_32_BIT_AMO_ON_GEMINI(_t)                               \
              || (remote_mr = mreg_for_remote_addr(obj, loc)) == NULL) {\
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, res, obj, xchgval, NULL);        \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, res, xchgval, NULL, loc);    \
          }                                                             \
          else if (nic_type == GNI_DEVICE_GEMINI) {                     \
            /*                                                          \
            ** Gemini doesn't have XCHG as such, but FAX (Fetch and     \
            ** And-Xor) can do the same thing with the right mask.      \
            */                                                          \
            int64_t mask = 0;                                           \
            do_nic_amo(&mask, xchgval, loc, obj, sizeof(_t),            \
                       GNI_FMA_ATOMIC_FAX, res, remote_mr);             \
          }                                                             \
          else {                                                        \
            do_nic_amo(xchgval, NULL, loc, obj, sizeof(_t),             \
                       amo_cmd_2_nic_op(_c, 1), res, remote_mr);        \
          }                                                             \
        }

DEFINE_CHPL_COMM_ATOMIC_XCHG(int32, xchg_32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_XCHG(int64, xchg_64, int_least64_t)
DEFINE_CHPL_COMM_ATOMIC_XCHG(uint32, xchg_32, uint_least32_t)
DEFINE_CHPL_COMM_ATOMIC_XCHG(uint64, xchg_64, uint_least64_t)
DEFINE_CHPL_COMM_ATOMIC_XCHG(real32, xchg_32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_XCHG(real64, xchg_64, int_least64_t)

#undef DEFINE_CHPL_COMM_ATOMIC_XCHG


//
// Atomic Compare-Exchange functions:
//   _f: interface function name suffix (type)
//   _c: network AMO command
//   _t: AMO type
//
#define DEFINE_CHPL_COMM_ATOMIC_CMPXCHG(_f, _c, _t)                     \
        DEFINE_DO_FORK_AMO(_f, _c, _t)                                  \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_cmpxchg_##_f(void* cmpval,                \
                                           void* xchgval,               \
                                           int32_t loc,                 \
                                           void* obj,                   \
                                           chpl_bool32* res,            \
                                           memory_order order,          \
                                           int ln, int32_t fn)          \
        {                                                               \
          mem_region_t* remote_mr;                                      \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_cmpxchg_"#_f                 \
                   "(%p, %p, %d, %p, %p)",                              \
                   cmpval, xchgval, (int) loc, obj, res);               \
                                                                        \
          if (chpl_numNodes == 1) {                                     \
            *res =                                                      \
              atomic_compare_exchange_strong_##_t((atomic_##_t*) obj,   \
                                                  *(_t*) cmpval,        \
                                                  *(_t*) xchgval);      \
            return;                                                     \
          }                                                             \
                                                                        \
          chpl_comm_diags_verbose_amo("amo cmpxchg", loc, ln, fn);      \
          chpl_comm_diags_incr(amo);                                    \
          if (IS_32_BIT_AMO_ON_GEMINI(_t)                               \
              || (remote_mr = mreg_for_remote_addr(obj, loc)) == NULL) {\
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, res, obj, cmpval, xchgval);      \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, res, cmpval, xchgval, loc);  \
          }                                                             \
          else {                                                        \
            _t my_res = 0;                                              \
            do_nic_amo(cmpval, xchgval, loc, obj, sizeof(_t),           \
                       amo_cmd_2_nic_op(_c, 1), &my_res, remote_mr);    \
            *res = (my_res == *(_t*) cmpval) ? true : false;            \
          }                                                             \
        }

DEFINE_CHPL_COMM_ATOMIC_CMPXCHG(int32, cmpxchg_32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_CMPXCHG(int64, cmpxchg_64, int_least64_t)
DEFINE_CHPL_COMM_ATOMIC_CMPXCHG(uint32, cmpxchg_32, uint_least32_t)
DEFINE_CHPL_COMM_ATOMIC_CMPXCHG(uint64, cmpxchg_64, uint_least64_t)
DEFINE_CHPL_COMM_ATOMIC_CMPXCHG(real32, cmpxchg_32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_CMPXCHG(real64, cmpxchg_64, int_least64_t)

#undef DEFINE_CHPL_COMM_ATOMIC_CMPXCHG


//
// Atomic ops for ints:
//   _f: interface function name suffix (type)
//   _o: op (and, or, xor, add)
//   _c: network AMO command
//   _t: AMO type
//
#define DEFINE_CHPL_COMM_ATOMIC_INT_OP(_f, _o, _c, _t)                  \
        DEFINE_DO_FORK_AMO(_f, _c, _t)                                  \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_##_o##_##_f(void* opnd,                   \
                                          int32_t loc,                  \
                                          void* obj,                    \
                                          memory_order order,           \
                                          int ln, int32_t fn)           \
        {                                                               \
          mem_region_t* remote_mr;                                      \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_"#_o"_"#_f                   \
                   "(%p, %d, %p)",                                      \
                   opnd, (int) loc, obj);                               \
                                                                        \
          if (chpl_numNodes == 1) {                                     \
            (void) atomic_fetch_##_o##_##_t((atomic_##_t*) obj,         \
                                            *(_t*) opnd);               \
            return;                                                     \
          }                                                             \
                                                                        \
          chpl_comm_diags_verbose_amo("amo " #_o, loc, ln, fn);         \
          chpl_comm_diags_incr(amo);                                    \
          if (IS_32_BIT_AMO_ON_GEMINI(_t)                               \
              || (remote_mr = mreg_for_remote_addr(obj, loc)) == NULL) {\
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, NULL, obj, opnd, NULL);          \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, NULL, opnd, NULL, loc);      \
          }                                                             \
          else {                                                        \
            do_nic_amo_nf(opnd, loc, obj, sizeof(_t),                   \
                          amo_cmd_2_nic_op(_c, 0), remote_mr);          \
          }                                                             \
        }                                                               \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_##_o##_unordered_##_f(void* opnd,         \
                                               int32_t loc,             \
                                               void* obj,               \
                                               int ln, int32_t fn)      \
        {                                                               \
          mem_region_t* remote_mr;                                      \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_"#_o"_unordered_"#_f         \
                   "(%p, %d, %p)",                                      \
                   opnd, (int) loc, obj);                               \
                                                                        \
          if (chpl_numNodes == 1) {                                     \
            (void) atomic_fetch_##_o##_##_t((atomic_##_t*) obj,         \
                                            *(_t*) opnd);               \
            return;                                                     \
          }                                                             \
                                                                        \
          chpl_comm_diags_verbose_amo("amo unord_" #_o, loc, ln, fn);   \
          chpl_comm_diags_incr(amo);                                    \
          if (IS_32_BIT_AMO_ON_GEMINI(_t)                               \
              || (remote_mr = mreg_for_remote_addr(obj, loc)) == NULL) {\
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, NULL, obj, opnd, NULL);          \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, NULL, opnd, NULL, loc);      \
          }                                                             \
          else {                                                        \
            do_nic_amo_nf_buff(opnd, loc, obj, sizeof(_t),              \
                               amo_cmd_2_nic_op(_c, 0), remote_mr);     \
          }                                                             \
        }                                                               \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_fetch_##_o##_##_f(void* opnd,             \
                                                int32_t loc,            \
                                                void* obj,              \
                                                void* res,              \
                                                memory_order order,     \
                                                int ln, int32_t fn)     \
        {                                                               \
          mem_region_t* remote_mr;                                      \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_fetch_"#_o"_"#_f             \
                   "(%p, %d, %p, %p)",                                  \
                   opnd, (int) loc, obj, res);                          \
                                                                        \
          if (chpl_numNodes == 1) {                                     \
            *(_t*) res =                                                \
              atomic_fetch_##_o##_##_t((atomic_##_t*) obj,              \
                                       *(_t*) opnd);                    \
            return;                                                     \
          }                                                             \
                                                                        \
          chpl_comm_diags_verbose_amo("amo fetch_" #_o, loc, ln, fn);   \
          chpl_comm_diags_incr(amo);                                    \
          if (IS_32_BIT_AMO_ON_GEMINI(_t)                               \
              || (remote_mr = mreg_for_remote_addr(obj, loc)) == NULL) {\
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, res, obj, opnd, NULL);           \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, res, opnd, NULL, loc);       \
          }                                                             \
          else {                                                        \
            do_nic_amo(opnd, NULL, loc, obj, sizeof(_t),                \
                       amo_cmd_2_nic_op(_c, 1), res, remote_mr);        \
          }                                                             \
        }

DEFINE_CHPL_COMM_ATOMIC_INT_OP(int32, and, and_i32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_INT_OP(int64, and, and_i64, int_least64_t)
DEFINE_CHPL_COMM_ATOMIC_INT_OP(uint32, and, and_i32, uint_least32_t)
DEFINE_CHPL_COMM_ATOMIC_INT_OP(uint64, and, and_i64, uint_least64_t)

DEFINE_CHPL_COMM_ATOMIC_INT_OP(int32, or, or_i32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_INT_OP(int64, or, or_i64, int_least64_t)
DEFINE_CHPL_COMM_ATOMIC_INT_OP(uint32, or, or_i32, uint_least32_t)
DEFINE_CHPL_COMM_ATOMIC_INT_OP(uint64, or, or_i64, uint_least64_t)

DEFINE_CHPL_COMM_ATOMIC_INT_OP(int32, xor, xor_i32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_INT_OP(int64, xor, xor_i64, int_least64_t)
DEFINE_CHPL_COMM_ATOMIC_INT_OP(uint32, xor, xor_i32, uint_least32_t)
DEFINE_CHPL_COMM_ATOMIC_INT_OP(uint64, xor, xor_i64, uint_least64_t)

DEFINE_CHPL_COMM_ATOMIC_INT_OP(int32, add, add_i32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_INT_OP(int64, add, add_i64, int_least64_t)
DEFINE_CHPL_COMM_ATOMIC_INT_OP(uint32, add, add_i32, uint_least32_t)
DEFINE_CHPL_COMM_ATOMIC_INT_OP(uint64, add, add_i64, uint_least64_t)

#undef DEFINE_CHPL_COMM_ATOMIC_INT_OP


//
// Atomic ops for reals:
//   _f: interface function name suffix (type)
//   _c: network AMO command
//   _t: AMO type
//
// Note: FP AMOs aren't supported on Gemini, and 64-bit FP add is
//       broken in Aries hardware and disabled in uGNI, so the only
//       thing we actually use the NIC for here is 32-bit FP add on
//       Aries.
//
#define DEFINE_CHPL_COMM_ATOMIC_REAL_OP(_f, _c, _t)                     \
        DEFINE_DO_FORK_AMO(_f, _c, _t)                                  \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_add_##_f(void* opnd,                      \
                                       int32_t loc,                     \
                                       void* obj,                       \
                                       memory_order order,              \
                                       int ln, int32_t fn)              \
        {                                                               \
          mem_region_t* remote_mr;                                      \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_add_"#_f                     \
                   "(%p, %d, %p)",                                      \
                   opnd, (int) loc, obj);                               \
                                                                        \
          if (chpl_numNodes == 1) {                                     \
            (void) atomic_fetch_add_##_t((atomic_##_t*) obj,            \
                                         *(_t*) opnd);                  \
            return;                                                     \
          }                                                             \
                                                                        \
          chpl_comm_diags_verbose_amo("amo add", loc, ln, fn);          \
          chpl_comm_diags_incr(amo);                                    \
          if (sizeof(_t) == sizeof(int_least32_t)                       \
              && nic_type == GNI_DEVICE_ARIES                           \
              && (remote_mr = mreg_for_remote_addr(obj, loc)) != NULL) {\
            do_nic_amo_nf(opnd, loc, obj, sizeof(_t),                   \
                          amo_cmd_2_nic_op(_c, 0), remote_mr);          \
          }                                                             \
          else {                                                        \
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, NULL, obj, opnd, NULL);          \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, NULL, opnd, NULL, loc);      \
          }                                                             \
        }                                                               \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_add_unordered_##_f(void* opnd,            \
                                            int32_t loc,                \
                                            void* obj,                  \
                                            int ln, int32_t fn)         \
        {                                                               \
          mem_region_t* remote_mr;                                      \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_add_unordered_"#_f           \
                   "(%p, %d, %p)",                                      \
                   opnd, (int) loc, obj);                               \
                                                                        \
          if (chpl_numNodes == 1) {                                     \
            (void) atomic_fetch_add_##_t((atomic_##_t*) obj,            \
                                         *(_t*) opnd);                  \
            return;                                                     \
          }                                                             \
                                                                        \
          chpl_comm_diags_verbose_amo("amo unord_add", loc, ln, fn);    \
          chpl_comm_diags_incr(amo);                                    \
          if (sizeof(_t) == sizeof(int_least32_t)                       \
              && nic_type == GNI_DEVICE_ARIES                           \
              && (remote_mr = mreg_for_remote_addr(obj, loc)) != NULL) {\
            do_nic_amo_nf_buff(opnd, loc, obj, sizeof(_t),              \
                               amo_cmd_2_nic_op(_c, 0), remote_mr);     \
          }                                                             \
          else {                                                        \
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, NULL, obj, opnd, NULL);          \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, NULL, opnd, NULL, loc);      \
          }                                                             \
        }                                                               \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_fetch_add_##_f(void* opnd,                \
                                             int32_t loc,               \
                                             void* obj,                 \
                                             void* res,                 \
                                             memory_order order,        \
                                             int ln, int32_t fn)        \
        {                                                               \
          mem_region_t* remote_mr;                                      \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_fetch_add_"#_f               \
                   "(%p, %d, %p, %p)",                                  \
                   opnd, (int) loc, obj, res);                          \
                                                                        \
          if (chpl_numNodes == 1) {                                     \
            *(_t*) res =                                                \
                atomic_fetch_add_##_t((atomic_##_t*) obj,               \
                                         *(_t*) opnd);                  \
            return;                                                     \
          }                                                             \
                                                                        \
          chpl_comm_diags_verbose_amo("amo fetch_add", loc, ln, fn);    \
          chpl_comm_diags_incr(amo);                                    \
          if (sizeof(_t) == sizeof(int_least32_t)                       \
              && nic_type == GNI_DEVICE_ARIES                           \
              && (remote_mr = mreg_for_remote_addr(obj, loc)) != NULL) {\
            do_nic_amo(opnd, NULL, loc, obj, sizeof(_t),                \
                       amo_cmd_2_nic_op(_c, 1), res, remote_mr);        \
          }                                                             \
          else {                                                        \
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, res, obj, opnd, NULL);           \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, res, opnd, NULL, loc);       \
          }                                                             \
        }

DEFINE_CHPL_COMM_ATOMIC_REAL_OP(real32, add_r32, _real32)
DEFINE_CHPL_COMM_ATOMIC_REAL_OP(real64, add_r64, _real64)

#undef DEFINE_CHPL_COMM_ATOMIC_REAL_OP


//
// Atomic subtract:
//   _f: interface function name suffix (type)
//   _t: AMO type
//   _negate: negation function
//
#define DEFINE_CHPL_COMM_ATOMIC_SUB(_f, _t, _negate)                    \
        /*==============================*/                              \
        void chpl_comm_atomic_sub_##_f(void* opnd,                      \
                                       int32_t loc,                     \
                                       void* obj,                       \
                                       memory_order order,              \
                                       int ln, int32_t fn)              \
        {                                                               \
          _t nopnd = _negate(*(_t*) opnd);                              \
                                                                        \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_sub_"#_f                     \
                   "(%p, %d, %p)",                                      \
                   opnd, (int) loc, obj);                               \
                                                                        \
          chpl_comm_atomic_add_##_f(&nopnd, loc, obj, order, ln, fn);   \
        }                                                               \
                                                                        \
         /*==============================*/                             \
        void chpl_comm_atomic_sub_unordered_##_f(void* opnd,            \
                                            int32_t loc,                \
                                            void* obj,                  \
                                            int ln, int32_t fn)         \
        {                                                               \
          _t nopnd = _negate(*(_t*) opnd);                              \
                                                                        \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_sub_unordered_"#_f           \
                   "(%p, %d, %p)",                                      \
                   opnd, (int) loc, obj);                               \
                                                                        \
          chpl_comm_atomic_add_unordered_##_f(&nopnd, loc, obj, ln, fn);\
        }                                                               \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_fetch_sub_##_f(void* opnd,                \
                                             int32_t loc,               \
                                             void* obj,                 \
                                             void* res,                 \
                                             memory_order order,        \
                                             int ln, int32_t fn)        \
        {                                                               \
          _t nopnd = _negate(*(_t*) opnd);                              \
                                                                        \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_fetch_sub_"#_f               \
                   "(%p, %d, %p, %p)",                                  \
                   opnd, (int) loc, obj, res);                          \
                                                                        \
          chpl_comm_atomic_fetch_add_##_f(&nopnd, loc, obj, res, order, \
                                          ln, fn);                      \
        }

#define NEGATE_I32(x) ((x) == INT_LEAST32_MIN ? (x) : -(x))
#define NEGATE_I64(x) ((x) == INT_LEAST64_MIN ? (x) : -(x))
#define NEGATE_U_OR_R(x) (-(x))

DEFINE_CHPL_COMM_ATOMIC_SUB(int32, int_least32_t, NEGATE_I32)
DEFINE_CHPL_COMM_ATOMIC_SUB(int64, int_least64_t, NEGATE_I64)
DEFINE_CHPL_COMM_ATOMIC_SUB(uint32, int_least32_t, NEGATE_U_OR_R)
DEFINE_CHPL_COMM_ATOMIC_SUB(uint64, int_least64_t, NEGATE_U_OR_R)
DEFINE_CHPL_COMM_ATOMIC_SUB(real32, _real32, NEGATE_U_OR_R)
DEFINE_CHPL_COMM_ATOMIC_SUB(real64, _real64, NEGATE_U_OR_R)

#undef DEFINE_CHPL_COMM_ATOMIC_SUB

void chpl_comm_atomic_unordered_task_fence(void) {
  task_local_buff_flush(amo_nf_buff);
}

static
inline
int amo_cmd_2_nic_op(fork_amo_cmd_t cmd, int fetching)
{
  gni_fma_cmd_type_t* table;
  gni_fma_cmd_type_t nic_cmd;

  //
  // Select a NIC AMO command based on our internal one, the kind of
  // NIC we're working with, and whether or not a result is desired.
  //
  table = (  (nic_type == GNI_DEVICE_GEMINI)
           ? (fetching ? nic_amos_gem_fetch : nic_amos_gem)
           : (fetching ? nic_amos_ari_fetch : nic_amos_ari));

  nic_cmd = ((int)cmd >= 0 && cmd < num_fork_amo_cmds) ? table[cmd] : -1;
  if ((int) nic_cmd < 0)
    CHPL_INTERNAL_ERROR("amo_cmd_2_nic_op(): unexpected AMO cmd");

  return nic_cmd;
}

// Sanity checks on amo operations. Ensure valid size and alignment and that
// the remote memory region is registered
static
inline
void check_nic_amo(size_t size, void* object, mem_region_t* remote_mr) {
  if (size == 4) {
    if (!IS_ALIGNED_32(VP_TO_UI64(object)))
      CHPL_INTERNAL_ERROR("remote AMO object must be 4-byte aligned");
  } else if (size == 8) {
    if (!IS_ALIGNED_64(VP_TO_UI64(object)))
      CHPL_INTERNAL_ERROR("remote AMO object must be 8-byte aligned");
  } else {
    CHPL_INTERNAL_ERROR("unexpected AMO size");
  }

  if (remote_mr == NULL)
    CHPL_INTERNAL_ERROR("do_nic_amo(): "
                        "remote address is not NIC-registered");
}



/*
 *** START OF NON-FETCHING BUFFERED ATOMIC OPERATIONS ***
 *
 * Support for non-fetching buffered atomic operations. We internally buffer
 * atomic operations and then initiate them with chained transactions for
 * increased transaction rate.
 */

// Flush buffered AMOs for the specified task info and reset the counter.
static inline
void amo_nf_buff_task_info_flush(amo_nf_buff_task_info_t* info) {
  if (info->vi > 0) {
    do_nic_amo_nf_V(info->vi, info->opnd1_v, info->locale_v, info->object_v,
                    info->size_v, info->cmd_v, info->remote_mr_v);
    info->vi = 0;
  }
}

// Append to task local buffers of operations and flush if full
static inline
void do_nic_amo_nf_buff(void* opnd1, c_nodeid_t locale,
                        void* object, size_t size,
                        gni_fma_cmd_type_t cmd,
                        mem_region_t* remote_mr)
{
  amo_nf_buff_task_info_t* info = task_local_buff_acquire(amo_nf_buff);
  if (info == NULL) {
    do_nic_amo_nf(opnd1, locale, object, size, cmd, remote_mr);
    return;
  }

  int vi = info->vi;
  // append arguments to buffers
  info->opnd1_v[vi]     = size == 4 ? *(uint32_t*) opnd1:
                                      *(uint64_t*) opnd1;
  info->locale_v[vi]    = locale;
  info->object_v[vi]    = object;
  info->size_v[vi]      = size;
  info->cmd_v[vi]       = cmd;
  info->remote_mr_v[vi] = remote_mr;
  info->vi++;

  // flush if buffers are full
  if (info->vi == MAX_CHAINED_AMO_LEN) {
    amo_nf_buff_task_info_flush(info);
  }

}
/*** END OF NON-FETCHING BUFFERED ATOMIC OPERATIONS ***/


static
inline
void do_nic_amo_nf(void* opnd1, c_nodeid_t locale,
                   void* object, size_t size,
                   gni_fma_cmd_type_t cmd,
                   mem_region_t* remote_mr)
{
  gni_post_descriptor_t post_desc;

  check_nic_amo(size, object, remote_mr);
  PERFSTATS_INC(amo_cnt);

  //
  // Fill in the POST descriptor.
  //
  post_desc.type            = GNI_POST_AMO;
  post_desc.cq_mode         = GNI_CQMODE_GLOBAL_EVENT;
  post_desc.dlvr_mode       = GNI_DLVMODE_PERFORMANCE;
  post_desc.rdma_mode       = 0;
  post_desc.src_cq_hndl     = 0;
  post_desc.remote_addr     = (uint64_t) (intptr_t) object;
  post_desc.remote_mem_hndl = remote_mr->mdh;
  post_desc.length          = size;
  post_desc.amo_cmd         = cmd;
  post_desc.first_operand   = size == 4 ? *(uint32_t*) opnd1:
                                          *(uint64_t*) opnd1;

  //
  // Initiate the transaction and wait for it to complete.
  //
  post_fma_and_wait(locale, &post_desc, true);
}


static
void do_nic_amo(void* opnd1, void* opnd2, c_nodeid_t locale,
                void* object, size_t size,
                gni_fma_cmd_type_t cmd, void* result,
                mem_region_t* remote_mr)
{
  mem_region_t*         local_mr = NULL;
  void*                 reg_result = result;
  fork_amo_data_t       stack_result;
  gni_post_descriptor_t post_desc;

  check_nic_amo(size, object, remote_mr);
  PERFSTATS_INC(amo_cnt);

  //
  // Make sure that, if we need a result, it is in memory known to the
  // NIC.
  //
  if (result != NULL) {
    local_mr = mreg_for_local_addr(reg_result);
    if (local_mr == NULL) {
      reg_result = &stack_result;
      local_mr = mreg_for_local_addr(reg_result);
      if (local_mr == NULL) {
        reg_result = amo_res_alloc();
        local_mr = gnr_mreg;
        if (local_mr == NULL)
          CHPL_INTERNAL_ERROR("do_nic_amo(): "
                              "result address is not NIC-registered");
      }
    }
  }

  //
  // Fill in the POST descriptor.
  //
  post_desc.type               = GNI_POST_AMO;
  post_desc.cq_mode            = GNI_CQMODE_GLOBAL_EVENT;
  post_desc.dlvr_mode          = GNI_DLVMODE_PERFORMANCE;
  post_desc.rdma_mode          = 0;
  post_desc.src_cq_hndl        = 0;
  post_desc.local_addr         = (uint64_t) (intptr_t) reg_result;
  if (reg_result != NULL)
    post_desc.local_mem_hndl   = local_mr->mdh;
  post_desc.remote_addr        = (uint64_t) (intptr_t) object;
  post_desc.remote_mem_hndl    = remote_mr->mdh;
  post_desc.length             = size;
  post_desc.amo_cmd            = cmd;
  post_desc.first_operand      = size == 4 ? *(uint32_t*) opnd1:
                                             *(uint64_t*) opnd1;
  if (opnd2 != NULL)
    post_desc.second_operand   = size == 4 ? *(uint32_t*) opnd2:
                                             *(uint64_t*) opnd2;

  //
  // Initiate the transaction and wait for it to complete.
  //
  post_fma_and_wait(locale, &post_desc, true);

  //
  // If the result wasn't registered, copy the trampoline memory to it
  //
  if (reg_result != result) {
    memcpy(result, reg_result, size);
    if (reg_result != &stack_result)
      amo_res_free(reg_result);
  }
}


void chpl_comm_execute_on(c_nodeid_t locale, c_sublocid_t subloc,
                          chpl_fn_int_t fid,
                          chpl_comm_on_bundle_t* arg, size_t arg_size,
                          int ln, int32_t fn)
{
  DBG_P_LP(DBGF_IFACE|DBGF_RF,
           "IFACE chpl_comm_execute_on(%d:%d, ftable[%d](%p, %zd))",
           (int) locale, (int) subloc, (int) fid, arg, arg_size);

  assert(locale != chpl_nodeID); // locale model code should prevent this ...

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn)) {
      chpl_comm_cb_info_t cb_data = 
        {chpl_comm_cb_event_kind_executeOn, chpl_nodeID, locale,
         .iu.executeOn={subloc, fid, arg, arg_size, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_executeOn("", locale, ln, fn);
  chpl_comm_diags_incr(execute_on);

  PERFSTATS_INC(fork_call_cnt);
  fork_call_common(locale, subloc, fid, arg, arg_size, false, true);
}


void chpl_comm_execute_on_nb(c_nodeid_t locale, c_sublocid_t subloc,
                             chpl_fn_int_t fid,
                             chpl_comm_on_bundle_t* arg, size_t arg_size,
                             int ln, int32_t fn)
{
  DBG_P_LP(DBGF_IFACE|DBGF_RF,
           "IFACE chpl_comm_execute_on_nb(%d:%d, ftable[%d](%p, %zd))",
           (int) locale, (int) subloc, (int) fid, arg, arg_size);

  assert(locale != chpl_nodeID); // locale model code should prevent this ...

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_nb)) {
      chpl_comm_cb_info_t cb_data = 
        {chpl_comm_cb_event_kind_executeOn_nb, chpl_nodeID, locale,
         .iu.executeOn={subloc, fid, arg, arg_size, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_executeOn("non-blocking", locale, ln, fn);
  chpl_comm_diags_incr(execute_on_nb);

  PERFSTATS_INC(fork_call_nb_cnt);
  fork_call_common(locale, subloc, fid, arg, arg_size, false, false);
}


void chpl_comm_execute_on_fast(c_nodeid_t locale, c_sublocid_t subloc,
                               chpl_fn_int_t fid,
                               chpl_comm_on_bundle_t* arg, size_t arg_size,
                               int ln, int32_t fn)
{
  DBG_P_LP(DBGF_IFACE|DBGF_RF,
           "IFACE chpl_comm_execute_on_fast(%d:%d, ftable[%d](%p, %zd))",
           (int) locale, (int) subloc, (int) fid, arg, arg_size);

  assert(locale != chpl_nodeID); // locale model code should prevent this ...

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_fast)) {
      chpl_comm_cb_info_t cb_data = 
        {chpl_comm_cb_event_kind_executeOn_fast, chpl_nodeID, locale,
         .iu.executeOn={subloc, fid, arg, arg_size, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_executeOn("fast", locale, ln, fn);
  chpl_comm_diags_incr(execute_on_fast);

  //
  // Note: the rf_handler() logic assumes that fast implies blocking.
  //       We enforce that here.
  //
  PERFSTATS_INC(fork_call_fast_cnt);
  fork_call_common(locale, subloc, fid, arg, arg_size, true, true);
}


static
void fork_call_common(c_nodeid_t locale, c_sublocid_t subloc,
                      chpl_fn_int_t fid,
                      chpl_comm_on_bundle_t* arg, size_t arg_size,
                      chpl_bool fast, chpl_bool blocking)
{
  size_t payload_size = arg_size - sizeof(chpl_comm_on_bundle_t);
  size_t small_msg_size = payload_size + sizeof(fork_small_call_info_t);
  int small = small_msg_size <= sizeof(fork_t);
  int large = !small;
  // heapCopyLargeArg is set below and is only relevant when large=true
  int heapCopyLargeArg = false;
  size_t f_size;
  int cdi;
  int rbi;
  chpl_bool do_fast_fork = fast && !large;
  chpl_comm_on_bundle_t *large_arg = NULL;
  fork_base_info_t *req = NULL;
  fork_t f;


  fork_base_info_t hdr = { .op           = ( large ?
                                             fork_op_large_call :
                                             fork_op_small_call ),
                           .caller       = chpl_nodeID,
                           .rf_done      = NULL };

  fork_small_call_info_t sc = { .b            = hdr,
                                .subloc       = subloc,
                                .fast         = do_fast_fork,
                                .blocking     = blocking,
                                .payload_size = payload_size,
                                .fid          = fid,
                                .state        = arg->task_bundle.state };

  fork_small_call_info_t *f_sc = &f.sc;

  if (locale < 0 || locale >= chpl_numNodes)
    CHPL_INTERNAL_ERROR("fork_call_common(): remote locale out of range");

  if (large) {
    //
    // The argument is too large to send directly in the request, so the
    // target locale will have to GET it from us.  We must guarantee the
    // argument continues to exist until the target has done so.  If
    // this is a blocking executeOn then we won't return until the whole
    // thing is done, so the remote locale can get the argument directly
    // from our incoming parameter and all is well.  But if this is a
    // non-blocking executeOn we will want to return as soon as we
    // initiate it.  In that case, we must make a copy of the argument
    // and give the remote locale a pointer to retrieve that copy
    // instead of the original.  The remote locale will free the copy
    // with a remote fork_op_free back to our locale.
    //
    // If the arg passed to us isn't in a registered region we should
    // also copy it, but only if the copy will likely be in registered
    // memory.  By copying it we save the target side the overhead of
    // doing an executeOn back to us to PUT the large arg to themself,
    // since if it's not in registered memory here they won't be able
    // to GET it directly.
    //
    if (!blocking ||
        (get_hugepage_size() > 0 && mreg_for_local_addr(arg) == NULL)) {
      heapCopyLargeArg = true;
    }

    if (heapCopyLargeArg) {
      large_arg = chpl_mem_allocMany(arg_size, sizeof(char),
                                     CHPL_RT_MD_COMM_FRK_SND_ARG, 0, 0);

      memcpy(large_arg, arg, arg_size);
    } else {
      large_arg = arg;
    }
    payload_size = sizeof(fork_large_call_info_t) - 
                   sizeof(fork_small_call_info_t);
    sc.payload_size = payload_size;
  }


  if (!large) {
    // Copy the header into the fork_t
    f.sc = sc;
    // Now copy the payload after it
    // Note ptr+1 here is the same as (unsigned char*)ptr + sizeof(*ptr)
    // and it refers to the memory just after that structure.
    memcpy(f_sc + 1,
           arg + 1,
           payload_size);
    f_size = small_msg_size;
  } else {
    // Copy the header into the fork_t
    f.lc.b = sc;
    // Set the pointer for the large call
    f.lc.arg      = large_arg;
    f.lc.arg_size = arg_size;
    f_size = sizeof(fork_large_call_info_t);
  }

  req = &f_sc->b;

  DBG_SET_SEQ(req->seq);
  DBG_P_LP(DBGF_RF, "forkTo(%d) %s",
           (int) locale,
           sprintf_rf_req(locale, req));
  if (!large) {
    PERFSTATS_INC(fork_call_small_cnt);
  } else {
    PERFSTATS_INC(fork_call_large_cnt);
  }

  //
  // Send the request to the target.
  //
  do_fork_post(locale, blocking, f_size, req, &cdi, &rbi);
  
  //
  // For fast forks, we free the remote fork request buffer on this
  // side.  In all other cases the target side frees it.
  //
  if (do_fast_fork)
    *SEND_SIDE_FORK_REQ_FREE_ADDR(locale, cdi, rbi) = true;

  //
  // For blocking large forks, we free any allocated argument now.
  // For nonblocking large forks, it will be freed by an AM
  // that the spawned task sends us.
  if (blocking && heapCopyLargeArg)
    chpl_mem_free(large_arg, 0, 0);
}


static
void fork_put(void* addr, c_nodeid_t locale, void* raddr, size_t size)
{
  fork_base_info_t hdr = { .op       = fork_op_put,
                           .caller   = chpl_nodeID,
                           .rf_done  = NULL // set in do_fork_post
                         };


  fork_xfer_info_t req = { .b = hdr,
                           .tgt = addr,
                           .src = raddr,
                           .size = size };

  if (locale < 0 || locale >= chpl_numNodes)
    CHPL_INTERNAL_ERROR("fork_put(): remote locale out of range");

  DBG_SET_SEQ(req.b.seq);
  DBG_P_LP(DBGF_GETPUT|DBGF_RF, "forkTo(%d) %s",
           (int) locale, sprintf_rf_req(locale, &req));

  //
  // Send the request to the target.  The remote locale won't make
  // the request buffer available until it knows the data it sent
  // has arrived here.
  //
  PERFSTATS_INC(fork_put_cnt);
  do_fork_post(locale, true /*blocking*/, sizeof(req), &req.b, NULL, NULL);
}


static
void fork_get(void* addr, c_nodeid_t locale, void* raddr, size_t size)
{
  fork_base_info_t hdr = { .op       = fork_op_get,
                           .caller   = chpl_nodeID,
                           .rf_done  = NULL // set in do_fork_post
                         };

  fork_xfer_info_t req = { .b = hdr,
                           .tgt = raddr,
                           .src = addr,
                           .size = size };

  if (locale < 0 || locale >= chpl_numNodes)
    CHPL_INTERNAL_ERROR("fork_get(): remote locale out of range");

  DBG_SET_SEQ(req.b.seq);
  DBG_P_LP(DBGF_GETPUT|DBGF_RF, "forkTo(%d) %s",
           (int) locale, sprintf_rf_req(locale, &req));

  //
  // Send the request to the target.  The remote locale won't make
  // the request buffer available until it knows the data it got
  // has arrived.
  //
  PERFSTATS_INC(fork_get_cnt);
  do_fork_post(locale, true /*blocking*/, sizeof(req), &req.b, NULL, NULL);
}


static
void fork_free(c_nodeid_t locale, void* p)
{
  fork_base_info_t hdr = { .op       = fork_op_free };

  fork_free_info_t req = { .b = hdr,
                           .p = p };

  if (locale < 0 || locale >= chpl_numNodes)
    CHPL_INTERNAL_ERROR("fork_free(): remote locale out of range");

  DBG_SET_SEQ(req.b.seq);
  DBG_P_LP(DBGF_RF, "forkTo(%d) %s",
           (int) locale, sprintf_rf_req(locale, &req));

  //
  // Send the request to the target.
  //
  PERFSTATS_INC(fork_free_cnt);
  do_fork_post(locale, false /*blocking*/, sizeof(req), &req.b, NULL, NULL);
}


static
void fork_amo(fork_t* p_rf_req, c_nodeid_t locale)
{
  if (locale < 0 || locale >= chpl_numNodes)
    CHPL_INTERNAL_ERROR("fork_amo(): remote locale out of range");

  p_rf_req->b.op = fork_op_amo;
  p_rf_req->a.b.caller = chpl_nodeID;

  DBG_SET_SEQ(p_rf_req->b.seq);
  DBG_P_LP(DBGF_AMO|DBGF_RF, "forkTo(%d) %s",
           (int) locale, sprintf_rf_req(locale, p_rf_req));


  //
  // Make sure that, if we need a result, it is in memory known to the
  // NIC.
  //
  if (p_rf_req->a.res != NULL
      && mreg_for_local_addr(p_rf_req->a.res) == NULL) {
    CHPL_INTERNAL_ERROR("fork_amo(): result address is not NIC-registered");
  }

  //
  // Send the request to the target.
  //
  PERFSTATS_INC(fork_amo_cnt);
  do_fork_post(locale, true /*blocking*/,
               sizeof(p_rf_req->a), &p_rf_req->a.b, NULL, NULL);
}

static
void fork_shutdown(c_nodeid_t locale)
{
  fork_base_info_t hdr = { .op = fork_op_shutdown };

  fork_shutdown_info_t req = { .b = hdr };

  if (locale < 0 || locale >= chpl_numNodes)
    CHPL_INTERNAL_ERROR("fork_shutdown(): remote locale out of range");

  DBG_SET_SEQ(req.b.seq);
  DBG_P_LP(DBGF_RF, "shutdown(%d) %s",
           (int) locale, sprintf_rf_req(locale, &req));

  //
  // Send the request to the target.
  //
  do_fork_post(locale, false /*blocking*/, sizeof(req), &req.b, NULL, NULL);
}


static
void do_fork_post(c_nodeid_t locale,
                  chpl_bool blocking,
                  uint64_t f_size, fork_base_info_t* const p_rf_req,
                  int* cdi_p, int* rbi_p)
{
  rf_done_t              stack_rf_done;
  gni_post_descriptor_t  stack_post_desc;
  gni_post_descriptor_t* post_desc_p;
  int                    rbi;

  static __thread nb_fork_t nb_fork[CD_ACTIVE_TRANS_MAX]; // nb fork descriptors
  static __thread int nb_fork_num = -1;       // number of outstanding nb forks
  static __thread int nb_fork_first_free = 0; // index of the first free desc

  if (blocking) {
    //
    // Our completion flag has to be in registered memory so the
    // remote locale can PUT directly back here to it.
    //
    if (mreg_for_local_addr(&stack_rf_done) != NULL) {
      p_rf_req->rf_done = &stack_rf_done;
    } else {
      p_rf_req->rf_done = rf_done_alloc();
    }
    *p_rf_req->rf_done = 0;
    chpl_atomic_thread_fence(memory_order_release);

    post_desc_p = &stack_post_desc;
  } else {
    p_rf_req->rf_done = NULL;

    // Initialize free flag on first call
    if (nb_fork_num == -1) {
      int i;
      for (i=0; i<CD_ACTIVE_TRANS_MAX; i++) {
        nb_fork[i].free = true;
      }
      nb_fork_num = 0;
    }
  }

  //
  // Acquire a communication domain and a remote request buffer
  //
  acquire_comm_dom_and_req_buf(locale, &rbi);
  if (cdi_p != NULL)
    *cdi_p = cd_idx;
  if (rbi_p != NULL)
    *rbi_p = rbi;

  //
  // Acquire a non-blocking descriptor. Note this must be done after we acquire
  // the comm_dom and req_buf since we do not allow yields between when we
  // acquire the non-blocking descriptor and when we do the post_fma below.
  //
  if (!blocking) {
    // find the index of the first free nb descriptor
    while (!nb_fork[nb_fork_first_free].free) {
      nb_fork_first_free = (nb_fork_first_free + 1) % CD_ACTIVE_TRANS_MAX;
    }

    //
    // Copy the arg bundle into the nb_desc and later send that space. In case
    // retransmission is needed we need the src buffer to exist until we
    // consume the completion event
    //
    nb_fork[nb_fork_first_free].free = false;
    memcpy(&nb_fork[nb_fork_first_free].fork, p_rf_req, f_size);
    post_desc_p = &nb_fork[nb_fork_first_free].nb_desc.post_desc;
    atomic_store_bool(&nb_fork[nb_fork_first_free].nb_desc.done, false);
    post_desc_p->post_id = (uint64_t) (intptr_t) &nb_fork[nb_fork_first_free].nb_desc.done;
  }

  //
  // Fill in the POST descriptor.
  //
  post_desc_p->type            = GNI_POST_FMA_PUT;
  post_desc_p->cq_mode         = GNI_CQMODE_GLOBAL_EVENT
                                 | GNI_CQMODE_REMOTE_EVENT;
  post_desc_p->dlvr_mode       = GNI_DLVMODE_PERFORMANCE;
  post_desc_p->rdma_mode       = 0;
  post_desc_p->src_cq_hndl     = 0;
  if (blocking)
    post_desc_p->local_addr    = (uint64_t) (intptr_t) p_rf_req;
  else
    post_desc_p->local_addr    = (uint64_t) (intptr_t) &nb_fork[nb_fork_first_free].fork;
  post_desc_p->remote_addr     = (uint64_t) (intptr_t)
                                 SEND_SIDE_FORK_REQ_BUF_ADDR(locale, cd_idx, rbi);
  post_desc_p->remote_mem_hndl = rf_mdh_map[locale];
  post_desc_p->length          = f_size;

  GNI_CHECK(GNI_EpSetEventData(cd->remote_eps[locale], 0,
                               GNI_ENCODE_REM_INST_ID(chpl_nodeID, cd_idx,
                                                      rbi)));
  DBG_P_LPS(DBGF_RF, "post %s",
            locale, cd_idx, rbi, p_rf_req->seq,
            fork_op_name(p_rf_req->op));

  if (blocking) {
    //
    // Initiate the transaction and wait for it to complete
    //
    post_fma_and_wait(locale, post_desc_p, blocking);

    PERFSTATS_INC(wait_rfork_cnt);
    while (! *(volatile rf_done_t*) p_rf_req->rf_done) {
      PERFSTATS_INC(lyield_in_wait_rfork_cnt);
      local_yield();
    }

    if (p_rf_req->rf_done != &stack_rf_done)
      rf_done_free(p_rf_req->rf_done);
  } else {
    //
    // Initiate the transaction and if we're out of space retire at least one
    //

    nb_fork[nb_fork_first_free].nb_desc.cdi = post_fma(locale, post_desc_p);
    nb_fork_first_free++;
    nb_fork_num++;

    // If we're at our max, retire at least one transaction. Note that we can't
    // compress transactions so maintain both the number of outstanding
    // transactions and the index of the first free one.
    if (nb_fork_num >= CD_ACTIVE_TRANS_MAX-1) {
      int i;
      chpl_bool retired_any = false;
      do {
        for (i=0; i<CD_ACTIVE_TRANS_MAX; i++) {
          nb_desc_t* nb_desc;
          chpl_bool done;

          if (nb_fork[i].free) {
            if (i < nb_fork_first_free) nb_fork_first_free = i;
            continue;
          }

          nb_desc = &nb_fork[i].nb_desc;

          done = atomic_load_explicit_bool(&nb_desc->done, memory_order_acquire);
          if (!done) {
            consume_all_outstanding_cq_events(nb_desc->cdi);
            done = atomic_load_explicit_bool(&nb_desc->done, memory_order_acquire);
          }
          if (done) {
            retired_any = true;
            nb_fork[i].free = true;
            nb_fork_num--;
            if (i < nb_fork_first_free) nb_fork_first_free = i;
          }
        }
      } while(!retired_any);
    }
  }
}


static inline
void acquire_comm_dom(void)
{
  int want_cdi;
  int want_cdi_start;
  comm_dom_t* want_cd;

#ifdef DEBUG_STATS
  uint64_t acq_looks = 0;
#endif

  //
  // If we have a firmly bound cd, this is a no-op.
  //
  if (cd != NULL && cd->firmly_bound) {
#ifdef DEBUG_STATS
    cd->acqs++;
    cd->acqs_looks++;
#endif
    return;
  }

  if (comm_dom_free_idx == -1) {
    comm_dom_free_idx = atomic_fetch_add_int_least32_t(&global_init_cdi, 1) % comm_dom_cnt;
  }

  assert(cd == NULL);

  //
  // Find an available CD with at least one free CQ entry.  Each time we
  // go through all of the CDs without acquiring one, yield before trying
  // again.
  //
  PERFSTATS_INC(acq_cd_cnt);

  want_cdi = want_cdi_start = comm_dom_free_idx;
  want_cd = &comm_doms[want_cdi];

  do {
#ifdef DEBUG_STATS
    acq_looks++;
#endif
    if (!CHECK_CD_BUSY(want_cd) && ACQUIRE_CD_MAYBE(want_cd)) {
      if (CQ_CNT_LOAD(want_cd) < want_cd->cq_cnt_max) {
        break;
      } else {
        PERFSTATS_INC(acq_cd_cq_cnt);
        RELEASE_CD(want_cd);
      }
    } else {
      PERFSTATS_INC(acq_cd_na_cnt);
    }

    want_cdi++;
    want_cd++;
    if (want_cdi >= comm_dom_cnt) {
      want_cdi = 0;
      want_cd = comm_doms;
    }

    if (want_cdi == want_cdi_start) {
      PERFSTATS_INC(lyield_in_acq_cd_cnt);
      local_yield();
    }
  } while (1);

  cd = want_cd;
  cd_idx = want_cdi;

#ifdef DEBUG_STATS
  cd->acqs++;
  cd->acqs_looks += acq_looks;
#endif
}


static
void acquire_comm_dom_and_req_buf(c_nodeid_t remote_locale, int* p_rbi)
{
  int want_cdi;
  int want_cdi_start;
  comm_dom_t* want_cd;
  int rbi;

#ifdef DEBUG_STATS
  uint64_t acq_looks = 0;
#endif

  if (comm_dom_free_idx == -1) {
    comm_dom_free_idx = atomic_fetch_add_int_least32_t(&global_init_cdi, 1) % comm_dom_cnt;
  }

  assert(cd == NULL);

  //
  // Find an available CD with at least one free CQ entry and fork
  // request buffer.  Each time we go through all of the CDs without
  // acquiring one, yield before trying again.
  //
  PERFSTATS_INC(acq_cd_rb_cnt);

  want_cdi = want_cdi_start = comm_dom_free_idx;
  want_cd = &comm_doms[want_cdi];

  do {
#ifdef DEBUG_STATS
    acq_looks++;
#endif

    if (!CHECK_CD_BUSY(want_cd) && ACQUIRE_CD_MAYBE(want_cd)) {
      if (CQ_CNT_LOAD(want_cd) < want_cd->cq_cnt_max) {
        for (rbi = 0; rbi < FORK_REQ_BUFS_PER_CD; rbi++) {
          if (*SEND_SIDE_FORK_REQ_FREE_ADDR(remote_locale, want_cdi, rbi)) {
            goto found_CD;
          } else {
            PERFSTATS_INC(acq_cd_rb_frf_cnt);
          }
        }
        RELEASE_CD(want_cd);
      } else {
        RELEASE_CD(want_cd);
        PERFSTATS_INC(acq_cd_rb_cq_cnt);
      }
    } else {
      PERFSTATS_INC(acq_cd_rb_na_cnt);
    }

    want_cdi++;
    want_cd++;
    if (want_cdi >= comm_dom_cnt) {
      want_cdi = 0;
      want_cd = comm_doms;
    }

    if (want_cdi == want_cdi_start) {
      PERFSTATS_INC(lyield_in_acq_cd_rb_cnt);
      local_yield();
    }
  } while (1);

 found_CD:

  *SEND_SIDE_FORK_REQ_FREE_ADDR(remote_locale, want_cdi, rbi) = false;

  cd = want_cd;
  cd_idx = want_cdi;

  *p_rbi = rbi;

#ifdef DEBUG_STATS
  cd->acqs_with_rb++;
  cd->acqs_with_rb_looks += acq_looks;
#endif
}


static inline
void release_comm_dom(void)
{
  assert(cd != NULL);

  //
  // This is a no-op if we have a firmly bound cd.
  //
  if (cd->firmly_bound)
    return;

  comm_dom_free_idx = cd_idx;

  RELEASE_CD(cd);

  cd = NULL;
  cd_idx = -1;
}


static
inline
chpl_bool reacquire_comm_dom(int want_cdi)
{
  //
  // If we have a firmly bound cd, this is, or at least should be, a
  // no-op.
  //
  if (cd != NULL && cd->firmly_bound) {
    if (want_cdi != cd_idx)
      CHPL_INTERNAL_ERROR("reacquire_comm_dom(): must want current cd");

#ifdef DEBUG_STATS
    cd->reacqs++;
#endif

    return true;
  }

  assert(cd == NULL);

  if (!ACQUIRE_CD_MAYBE(&comm_doms[want_cdi]))
    return false;

  PERFSTATS_INC(reacq_cd_cnt);

  cd = &comm_doms[want_cdi];
  cd_idx = want_cdi;

#ifdef DEBUG_STATS
  cd->reacqs++;
#endif

  return true;
}


static
inline
int post_fma(c_nodeid_t locale, gni_post_descriptor_t* post_desc)
{
  int cdi;

  PERFSTATS_ADD_POST(post_desc);

  if (cd == NULL)
    acquire_comm_dom();
  cdi = cd_idx;

  CQ_CNT_INC(cd);
  GNI_CHECK(GNI_PostFma(cd->remote_eps[locale], post_desc));
  release_comm_dom();
  return cdi;
}


static
void post_fma_and_wait(c_nodeid_t locale, gni_post_descriptor_t* post_desc,
                       chpl_bool do_yield)
{
  int cdi;
  atomic_bool post_done;
  uint64_t iters = 0;

  // Avoid yielding for tasks with limited comm. Avoids artificially increasing
  // the lifetime of short-lived tasks.
  if (do_yield) {
    chpl_comm_taskPrvData_t* prvData = get_comm_taskPrvdata();
    if (prvData != NULL && prvData->num_fma < 100) {
      prvData->num_fma++;
      do_yield = false;
    }
  }

  atomic_init_bool(&post_done, false);
  post_desc->post_id = (uint64_t) (intptr_t) &post_done;

  cdi = post_fma(locale, post_desc);

  //
  // Wait for the transaction to complete.  Yield initially; the
  // minimum round-trip time on the network isn't small and maybe
  // we can find something else to do in the meantime.  FMA is only
  // used for small transactions which will be relatively fast, so
  // after the initial yield, only yield every 64 attempts.
  //
  do {
    if (do_yield && (iters & 0x3F) == 0) {
      local_yield();
    }
    consume_all_outstanding_cq_events(cdi);
    iters++;
  } while (!atomic_load_explicit_bool(&post_done, memory_order_acquire));
}

#if HAVE_GNI_FMA_CHAIN_TRANSACTIONS

static
inline
int post_fma_ct(c_nodeid_t* locale_v, gni_post_descriptor_t* post_desc)
{
  int cdi;

  if (cd == NULL)
    acquire_comm_dom();
  cdi = cd_idx;
  PERFSTATS_ADD_POST(post_desc);

  if (post_desc->type == GNI_POST_FMA_PUT) {
    gni_ct_put_post_descriptor_t* pdc;
    int i;

    for (pdc = post_desc->next_descr, i = 1;
         pdc != NULL;
         pdc = pdc->next_descr, i++) {
      pdc->ep_hndl = cd->remote_eps[locale_v[i]];
      PERFSTATS_ADD(sent_bytes, pdc->length);
    }
  } else if (post_desc->type == GNI_POST_FMA_GET) {
    gni_ct_get_post_descriptor_t* pdc;
    int i;

    for (pdc = post_desc->next_descr, i = 1;
         pdc != NULL;
         pdc = pdc->next_descr, i++) {
      pdc->ep_hndl = cd->remote_eps[locale_v[i]];
    }
  } else if (post_desc->type == GNI_POST_AMO) {
    gni_ct_amo_post_descriptor_t* pdc;
    int i;

    for (pdc = post_desc->next_descr, i = 1;
         pdc != NULL;
         pdc = pdc->next_descr, i++) {
      pdc->ep_hndl = cd->remote_eps[locale_v[i]];
    }
  }

  CQ_CNT_INC(cd);
  GNI_CHECK(GNI_CtPostFma(cd->remote_eps[locale_v[0]], post_desc));
  release_comm_dom();
  return cdi;
}


static
void post_fma_ct_and_wait(c_nodeid_t* locale_v,
                          gni_post_descriptor_t* post_desc)
{
  int cdi;
  atomic_bool post_done;

  atomic_init_bool(&post_done, false);
  post_desc->post_id = (uint64_t) (intptr_t) &post_done;

  cdi = post_fma_ct(locale_v, post_desc);

  //
  // Wait for the transaction to complete.  Yield initially; the
  // minimum round-trip time on the network isn't small and maybe
  // we can find something else to do in the meantime.
  //
  do {
    local_yield();
    consume_all_outstanding_cq_events(cdi);
  } while (!atomic_load_explicit_bool(&post_done, memory_order_acquire));
}

#endif


static
inline
int post_rdma(c_nodeid_t locale, gni_post_descriptor_t* post_desc)
{
  int cdi;

  PERFSTATS_ADD_POST(post_desc);

  if (cd == NULL)
    acquire_comm_dom();
  cdi = cd_idx;

  post_desc->src_cq_hndl = cd->cqh;

  CQ_CNT_INC(cd);
  GNI_CHECK(GNI_PostRdma(cd->remote_eps[locale], post_desc));
  release_comm_dom();
  return cdi;
}


static
void post_rdma_and_wait(c_nodeid_t locale, gni_post_descriptor_t* post_desc,
                        chpl_bool do_yield)
{
  int cdi;
  atomic_bool post_done;

  atomic_init_bool(&post_done, false);
  post_desc->post_id = (uint64_t) (intptr_t) &post_done;

  cdi = post_rdma(locale, post_desc);

  //
  // Wait for the transaction to complete.  Yield initially; the
  // minimum round-trip time on the network isn't small and maybe
  // we can find something else to do in the meantime.
  //
  do {
    if (do_yield) {
      local_yield();
    }
    consume_all_outstanding_cq_events(cdi);
  } while (!atomic_load_explicit_bool(&post_done, memory_order_acquire));
}


static inline
chpl_bool can_task_yield(void) {
  return allow_task_yield;
}


static
void local_yield(void)
{
  //
  // Our task cannot make progress.  Yield, to allow some other task to
  // free up whatever resource we need.
  //
#ifdef CHPL_COMM_YIELD_TASK_WHILE_POLLING
  PERFSTATS_INC(lyield_cnt);
  if (cd == NULL) {
    //
    // Without a comm domain, just yield.
    //
    PERFSTATS_INC(tskyield_in_lyield_no_cd_cnt);
    if (can_task_yield()) 
      chpl_task_yield();
    else
      sched_yield();
  }
  else {
    //
    // If we have a comm domain then we have to hold on to our pthread
    // (because our comm domain pointer is in TLS), so just yield the
    // CPU.  This should only happen when we are the polling thread.
    //
    assert (cd->firmly_bound);
    sched_yield();
  }
#else
  sched_yield();
#endif
}


void chpl_comm_statsStartHere(void)
{
#define _PSZM(psv) PERFSTATS_STZ(psv);
  PERFSTATS_DO_EPHEMERAL(_PSZM);
#undef _PSZM
}


#ifdef PERFSTATS_COMM_UGNI
static void _psv_print(int, chpl_comm_pstats_t*);
#endif


void chpl_comm_statsReport(chpl_bool sum_over_locales)
{
#ifdef PERFSTATS_COMM_UGNI
  if (sum_over_locales) {
    chpl_comm_pstats_t ps;
    chpl_comm_pstats_t sum;

    sum = chpl_comm_pstats;
    for (int li = 0; li < chpl_numNodes; li++) {
      if (li != chpl_nodeID) {
        chpl_comm_get(&ps, li, &chpl_comm_pstats, sizeof(ps), CHPL_COMM_UNKNOWN_ID, 0, -1);
#define _PSV_SUM(psv) _PSV_ADD_FUNC(&sum.psv, _PSV_LD_FUNC(&ps.psv));
        PERFSTATS_DO_ALL(_PSV_SUM);
#undef _PSV_SUM
      }
    }
    _psv_print(-1, &sum);
  }
  else
    _psv_print(chpl_nodeID, &chpl_comm_pstats);
#else
  chpl_warning("ugni was not built with support for perfstats reporting", 0, 0);
#endif
}


#ifdef PERFSTATS_COMM_UGNI
static void _psv_print(int li, chpl_comm_pstats_t* ps)
{
  char   buf[8 * 1024];
  size_t buf_cnt;

#define _PSV_PRINT(psv)                                                 \
        do {                                                            \
          _PSV_C_TYPE _psvv = _PSV_LD_FUNC(&ps->psv);                   \
          if (_psvv != 0) {                                             \
            size_t wc = snprintf(&buf[buf_cnt], sizeof(buf) - buf_cnt,  \
                                 "%s = %" _PSV_FMT "; ", # psv, _psvv); \
            if (wc > sizeof(buf) - buf_cnt)                             \
              buf_cnt = sizeof(buf);                                    \
            else                                                        \
              buf_cnt += wc;                                            \
          }                                                             \
        } while (0);

  buf_cnt = 0;
  PERFSTATS_DO_ALL(_PSV_PRINT);
  if (li < 0)
    printf("comm perfstats over all locales: (%.*s)\n",
           (int) buf_cnt - 2, buf);
  else
    printf("comm perfstats on locale %d (%.*s)\n", li,
           (int) buf_cnt - 2, buf);
  fflush(stdout);

#undef _PSV_PRINT
}
#endif
