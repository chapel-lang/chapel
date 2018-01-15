/*
 * Copyright 2004-2018 Cray Inc.
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
#include <pmi.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>

#include <gni_pub.h>    // <stddef.h> and <stdint.h> must come first
#include <hugetlbfs.h>  // <sys/types.h> must come first

#include "chplcgfns.h"
#include "chpl-gen-includes.h"
#include "chplrt.h"
#include "chpl-cache.h"
#include "chpl-comm.h"
#include "chpl-comm-callbacks.h"
#include "chpl-comm-callbacks-internal.h"
#include "chpl-mem.h"
#include "chplsys.h"
#include "chpl-tasks.h"
#include "chpl-atomics.h"
#include "chplcast.h"
#include "chpl-linefile-support.h"
#include "comm-ugni-mem.h"
#include "config.h"
#include "error.h"

// Don't get warning macros for chpl_comm_get etc
#include "chpl-comm-no-warning-macros.h"


// uncomment this and change debug_flag below to enable UGNI debugging
//#define DEBUG 1

#ifdef DEBUG
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

static c_nodeid_t debug_nodeID = 0;

static FILE* debug_file;

static chpl_bool debug_exiting = false;

#define _DBG_DO(flg)  (((flg) & debug_flag) != 0                        \
                       && ((DBGF_1_NODE & debug_flag) == 0              \
                           || chpl_nodeID == debug_nodeID))
#define _DBG_THIS_NODE()  ((DBGF_1_NODE & debug_flag) == 0              \
                           || chpl_nodeID == debug_nodeID)

#define DBG_INIT()  dbg_init()
#define DBG_INIT_OUTPUT_FILE()  dbg_init_output_file()

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
               " (%d/%" PRId64 "/%d): " f,                              \
               chpl_nodeID, task_id(cd != NULL && cd->firmly_bound),    \
               (int) thread_idx, ## __VA_ARGS__)
#define DBG_P_LPS(flg, f, li, cdi, rbi, seq, ...)                       \
        _DBG_P(_DBG_DO(flg),                                            \
               " (%d/%" PRId64 "/%d) %d/%d/%d <%" PRIu64 ">: " f,       \
               chpl_nodeID, task_id(cd != NULL && cd->firmly_bound),    \
               (int) thread_idx,                                        \
               (int) li, cdi, rbi, seq, ## __VA_ARGS__)

#define CHPL_INTERNAL_ERROR(msg)                                        \
        do {                                                            \
          DBG_P_LP(1, "%s:%d: internal error: %s",                      \
                   __FILE__, (int) __LINE__, msg);                      \
          abort();                                                      \
        } while (0)

#define GNI_FAIL(rc, msg)                                               \
        do {                                                            \
          DBG_P_LP(1, "GNI: error code %d", (int) rc);                  \
          CHPL_INTERNAL_ERROR(msg);                                     \
        } while (0)

#define GNI_POST_FAIL(rc, msg)                                          \
        do {                                                            \
          if (rc == GNI_RC_ERROR_RESOURCE)                              \
            DBG_P_LP(1, "GNI: error code %d, cq_cnt_curr %d",           \
                     (int) rc, (int) CQ_CNT_LOAD(cd));                  \
          else                                                          \
            DBG_P_LP(1, "GNI: error code %d", (int) rc);                \
          CHPL_INTERNAL_ERROR(msg);                                     \
        } while (0)

#define GNI_CQ_EV_FAIL(rc, ev, msg)                                     \
        do {                                                            \
          char buf[1024];                                               \
          (void) GNI_CqErrorStr(ev, buf, sizeof(buf));                  \
          DBG_P_LP(1, "GNI: error code %d, str %s",                     \
                   (int) rc, buf);                                      \
          CHPL_INTERNAL_ERROR(msg);                                     \
        } while (0)

static int64_t task_id(int);
static int64_t task_id(int firmly_bound)
{
  if (firmly_bound)
    return -(int64_t) 1;
  if (debug_exiting)
    return -(int64_t) 2;
  return (int64_t) chpl_task_getId();
}
#else
#undef DEBUG_STATS

#define DBG_INIT()
#define DBG_INIT_OUTPUT_FILE()

#define DBG_P(flg, f, ...)
#define DBG_P_L(flg, f, ...)
#define DBG_P_LP(flg, f, ...)
#define DBG_P_LPS(flg, f, b, ...)

#define CHPL_INTERNAL_ERROR(msg)                                        \
        do {                                                            \
          static __thread char buf[1000];                               \
          (void) snprintf(buf, sizeof(buf),                             \
                          "%d:%s:%d: internal error: %s",               \
                          (int) chpl_nodeID, __FILE__, (int) __LINE__,  \
                          msg);                                         \
          chpl_internal_error(buf);                                     \
        } while (0)
#define GNI_FAIL(rc, msg)            CHPL_INTERNAL_ERROR(msg)
#define GNI_POST_FAIL(rc, msg)       CHPL_INTERNAL_ERROR(msg)
#define GNI_CQ_EV_FAIL(rc, ev, msg)  CHPL_INTERNAL_ERROR(msg)
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
        MACRO(put_strd_byte_cnt)                                        \
        MACRO(get_cnt)                                                  \
        MACRO(get_byte_cnt)                                             \
        MACRO(get_strd_cnt)                                             \
        MACRO(get_strd_byte_cnt)                                        \
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
        MACRO(regMem_cnt)                                               \
        MACRO(deregMem_cnt)                                             \
        MACRO(sent_bytes)                                               \
        MACRO(rcvd_bytes)                                               \
        MACRO(acq_cd_cnt)                                               \
        MACRO(acq_cd_na_cnt)                                            \
        MACRO(acq_cd_cq_cnt)                                            \
        MACRO(acq_cd_rb_cnt)                                            \
        MACRO(acq_cd_rb_na_cnt)                                         \
        MACRO(acq_cd_rb_cq_cnt)                                         \
        MACRO(acq_cd_rb_frf_1_cnt)                                      \
        MACRO(acq_cd_rb_frf_2_cnt)                                      \
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
        MACRO(lyield_in_wait_glb_cq_ev_cnt)

#define PERFSTATS_VARS_ALL(MACRO)                                       \
        PERFSTATS_VARS_EPHEMERAL(MACRO)

#define _PSV_C_TYPE      uint64_t
#define _PSV_ATOMIC_TYPE atomic_uint_least64_t
#define _PSV_ADD_FUNC    atomic_fetch_add_uint_least64_t
#define _PSV_SUB_FUNC    atomic_fetch_sub_uint_least64_t

#define _PSV_DECL(psv)  _PSV_ATOMIC_TYPE psv;
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

#define PERFSTATS_ADD(cnt, val)  (void) _PSV_ADD_FUNC(&_PSV_VAR(cnt), val)
#define PERFSTATS_INC(cnt)       PERFSTATS_ADD(cnt, 1)
#else
#define PERFSTATS_ADD(cnt, val)
#define PERFSTATS_INC(cnt)
#define PERFSTATS_DO_EPHEMERAL(MACRO)
#define PERFSTATS_DO_ALL(MACRO)
#endif


//
// Alignment.
//
#define ALIGN_DN(i, size)  ((i) & ~((size) - 1))
#define ALIGN_UP(i, size)  ALIGN_DN((i) + (size) - 1, size)


//
// Declarations having to do with the NIC.
//
static uint32_t* nic_addr_map = NULL;

static gni_nic_device_t nic_type;


//
// The uGNI instance IDs we use are built from locale, communication
// domain, and request buffer indices, plus remote operation codes.
//
#define _IID_LI_BITS  18
#define _IID_LI_MASK  ((1 << _IID_LI_BITS) - 1)
#define _IID_LI_POS   0

#define _IID_CDI_BITS 5
#define _IID_CDI_MASK ((1 << _IID_CDI_BITS) - 1)
#define _IID_CDI_POS  (_IID_LI_POS + _IID_LI_BITS)

#define _IID_RBI_BITS 1
#define _IID_RBI_MASK ((1 << _IID_RBI_BITS) - 1)
#define _IID_RBI_POS  (_IID_CDI_POS + _IID_CDI_BITS)

#if (_IID_LI_BITS + _IID_CDI_BITS + _IID_RBI_BITS) > 24
#error INST_ID fields are too big!
#endif

#define GNI_INST_ID(li, cdi, rbi)                                       \
        (  (((li ) & _IID_LI_MASK ) << _IID_LI_POS )                    \
         | (((cdi) & _IID_CDI_MASK) << _IID_CDI_POS)                    \
         | (((rbi) & _IID_RBI_MASK) << _IID_RBI_POS))
#define INST_ID_LI(id)      (((id) >> _IID_LI_POS ) & _IID_LI_MASK )
#define INST_ID_CDI(id)     (((id) >> _IID_CDI_POS) & _IID_CDI_MASK)
#define INST_ID_RBI(id)     (((id) >> _IID_RBI_POS) & _IID_RBI_MASK)


//
// Declarations having to do with memory and memory registration.
//
// We register all of the read/write memory regions in /proc/self/maps
// that have pathnames.  We can handle up to MAX_MEM_REGIONS of these.
// There are four regions we definitely want to register.  These are the
// static data, the part of the heap right after the static data, the
// main heap (probably on hugepages), and the stack.  We don't actually
// have to register all regions.  We really only need the static data
// and main heap to be registered.  The logic can handle transfers to
// and from other areas by bouncing them through a buffer in the main
// heap.
//
static int    registered_heap_info_set;
static size_t registered_heap_size;
static void*  registered_heap_start;

static int    using_hugepages;
static size_t hugepage_size;

//
// Memory regions.  mem_regions contains the address/length pairs and
// uGNI memory domain handles for all of the registered memory regions.
// mem_regions_map contains a copy of every node's mem_regions table.
// And finally, mem_regions_map_addr_map contains the mem_regions_map
// pointers on each node.
//
#define MAX_MEM_REGIONS 1000

typedef struct {
  uint64_t         addr;
  uint64_t         len;
  gni_mem_handle_t mdh;
} mem_region_t;

typedef struct {
  uint32_t     mreg_cnt;  // really hi idx + 1 (mregs[] may have holes)
  mem_region_t mregs[MAX_MEM_REGIONS];
} mem_region_table_t;

static mem_region_table_t mem_regions;


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

static inline
void mem_regions_lock(void) {
  if (pthread_mutex_lock(&mem_regions_mutex) != 0)
    CHPL_INTERNAL_ERROR("cannot acquire mem region lock");
  allow_task_yield = false;
}

static inline
void mem_regions_unlock(void) {
  if (pthread_mutex_unlock(&mem_regions_mutex) != 0)
    CHPL_INTERNAL_ERROR("cannot release mem region lock");
  allow_task_yield = true;
}

static inline
chpl_bool can_task_yield(void) {
  return allow_task_yield;
}



#ifdef DEBUG
static uint32_t mreg_cnt_max;
#endif

static mem_region_table_t* mem_regions_map;
static mem_region_table_t** mem_regions_map_addr_map;

//
// This is the memory region for the guaranteed NIC-registered memory
// in which remote fork descriptors, their is-free flags, and the
// temporary bounce buffers live.  It saves a little bit of time not
// to have to look it up.
//
static mem_region_t* gnr_mreg;

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

#define CQ_CNT_STORE(cd, val) \
        atomic_store_uint_least32_t(&(cd)->cq_cnt_curr, val)
#define CQ_CNT_LOAD(cd)       \
        atomic_load_uint_least32_t(&(cd)->cq_cnt_curr)
#define CQ_CNT_INC(cd)        \
        (void) atomic_fetch_add_uint_least32_t(&(cd)->cq_cnt_curr, 1)
#define CQ_CNT_DEC(cd)        \
        (void) atomic_fetch_sub_uint_least32_t(&(cd)->cq_cnt_curr, 1)

typedef struct {
  atomic_bool        busy;
  chpl_bool          firmly_bound;
  gni_nic_handle_t   nih;
  gni_ep_handle_t*   remote_eps;
  gni_cq_handle_t    cqh;
  cq_cnt_t           cq_cnt_max;
  cq_cnt_atomic_t    cq_cnt_curr;
#ifdef DEBUG_STATS
  uint64_t           acqs;
  uint64_t           acqs_looks;
  uint64_t           acqs_with_rb;
  uint64_t           acqs_with_rb_looks;
  uint64_t           reacqs;
#endif
  uint64_t           cache_spacer[8];    // prevent comm_doms[] inter-element
                                         //   cache line sharing; ra-atomics
                                         //     perf suffers without this
} comm_dom_t;


//
// Communication domain descriptors.
//
static uint32_t     comm_dom_cnt;
static uint32_t     comm_dom_cnt_max;

static comm_dom_t*  comm_doms;

static __thread int comm_dom_free_idx;
static __thread comm_dom_t* cd = NULL;
static __thread int cd_idx = -1;

#define INIT_CD_BUSY(cd)      atomic_init_bool(&(cd)->busy, false)
#define CHECK_CD_BUSY(cd)     atomic_load_bool(&(cd)->busy)
#define ACQUIRE_CD_MAYBE(cd)  (atomic_exchange_bool(&(cd)->busy, true) == false)
#define RELEASE_CD(cd)        atomic_store_bool(&(cd)->busy, false)


//
// Declarations having to do with individual remote references.
//

#define MAX_UGNI_TRANS_SZ ((size_t) 1 << 30)

#define ALIGN_32_DN(x)    ALIGN_DN((x), sizeof(int32_t))
#define ALIGN_32_UP(x)    ALIGN_UP((x), sizeof(int32_t))
#define IS_ALIGNED_32(x)  ((x) == ALIGN_32_DN(x))

#define ALIGN_64_DN(x)    ALIGN_DN((x), sizeof(int64_t))
#define ALIGN_64_UP(x)    ALIGN_UP((x), sizeof(int64_t))
#define IS_ALIGNED_64(x)  ((x) == ALIGN_64_DN(x))

#define VP_TO_UI64(x)     ((uint64_t) (intptr_t) (x))
#define UI64_TO_VP(x)     ((void*) (intptr_t) (x))

//
// Maximum number of PUTs in a chained transaction list.  This number
// was determined empirically (on XC/Aries).  Doing more than this at
// once didn't seem to improve performance.
//
#define MAX_CHAINED_PUT_LEN 64


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
// it complete.  Here we define pools of post descriptors to use for
// these non-blocking PUTs and GETs.  Each pool entry contains the
// post descriptor itself, a "done" flag, and free list link.  We put
// the address of the "done" flag in the post_id member of the post
// descriptor so that we can set it when we receive the completion.
//
// We allocate these up front, with a fixed size.  There are
// NB_DESC_NUM_POOLS pools, with NB_DESC_NUM_PER_POOL in each one.
// The requirements/constraints on these values are as follows.
//   - We don't need to acquire one of these unless we're holding a
//     comm domain, so we don't need more pools than we have comm
//     domains.  An Aries NIC supports 128 comm domains (FMA windows),
//     so that is our maximum.
//   - We cannot have more than CD_ACTIVE_TRANS_MAX NB transactions
//     outstanding at the same time, per comm domain, so we don't need
//     more that per pool.
//   - We emulate an unsigned mod by NB_DESC_NUM_POOLS by means of
//     & (NB_DESC_NUM_POOLS - 1), so that has to be a power of 2.
//   - The code does divides and mods with NB_DESC_NUM_PER_POOL, so
//     for performance that should be a power of 2.
//
#define NB_DESC_NUM_POOLS 128
#define NB_DESC_NUM_PER_POOL CD_ACTIVE_TRANS_MAX

typedef struct {
  gni_post_descriptor_t post_desc;  // POST descriptor for an NB transaction
  atomic_bool done;                 // transaction has completed?
  int cdi;                          // index of comm domain used for post
  mpool_idx_base_t next;            // free list index
} nb_desc_t;

static nb_desc_t nb_desc_pool[NB_DESC_NUM_POOLS][NB_DESC_NUM_PER_POOL];
static mpool_idx_t nb_desc_pool_head[NB_DESC_NUM_POOLS];
static atomic_bool nb_desc_pool_lock[NB_DESC_NUM_POOLS];

static mpool_idx_t nb_desc_pool_i;

static inline
mpool_idx_base_t nb_desc_next_pool_i(void) 
{
  return mpool_idx_finc(&nb_desc_pool_i) & (NB_DESC_NUM_POOLS - 1);
}

typedef uint16_t nb_desc_idx_t;


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
// There is an obvious tension on FORK_T_MAX_ARG_SIZE here.  The more
// data we can store in the fixed-sized elements of fork_reqs, the
// fewer fork requests there will be for which the remote end has to
// do a remote GET of an argument larger than that.  But there can be
// many fork_reqs elements.  For a hypothetical application running on
// 1k locales with 16 communication domains per locale and 2 fork
// request buffers per comm domain, the current FORK_T_MAX_ARG_SIZE
// means that each locale has about 2 MiB of space dedicated to fork
// requests.  That's not much, but one can see that things could get
// out of hand quickly if FORK_T_MAX_ARG_SIZE were very large.  As of
// this writing (11/21/11) we don't have a good handle on the
// distribution of remote fork argument sizes.  Some better
// information about that would be nice to have.
//
typedef enum {
  fork_op_nil,
  fork_op_small_call,
  fork_op_large_call,
  fork_op_put,
  fork_op_get,
  fork_op_free,
  fork_op_amo,
  fork_op_num_ops
} fork_op_t;

#define FORK_OP_BITS 3

typedef struct {
  unsigned char op: FORK_OP_BITS;    // operation
  c_nodeid_t    caller;              // requesting locale
  rf_done_t*    rf_done;             // where to indicate completion
#ifdef DEBUG
  uint_least64_t seq;
#endif
} fork_base_info_t;

typedef struct {
  fork_base_info_t b;

  c_sublocid_t  subloc;  // target sublocale
  unsigned char fast:          1;
  unsigned char blocking:      1;
  unsigned char payload_size;
  chpl_fn_int_t fid;

  // TODO: is there a way to "compress" this?
  chpl_task_ChapelData_t state;
} fork_small_call_info_t;

// Note: fork_small_call_info_t.payload_size must be able to store
// a number at least this large. That means it should be at most
// 255 as long as payload_size is an unsigned char.
#define MAX_SMALL_CALL_PAYLOAD 64
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
  unsigned char buf[FORK_T_MAX_SIZE];
} fork_space_t;

typedef union fork_t {
  fork_base_info_t b;
  fork_small_call_info_t sc;
  fork_large_call_info_t lc;
  fork_xfer_info_t x;
  fork_free_info_t f;
  fork_amo_info_t  a;
  fork_space_t bytes; // get fork_t to be >= FORK_T_MAX_SIZE bytes
} fork_t;

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

#ifdef DEBUG
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
// and knows the address of that struct in its two child locales (the
// locales with indices 2*my_index+1 and 2*my_index+2) and its parent
// (the one with index (my_index-1)/2).  The notify and release flags on
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
// QUESTION FROM REVIEW: This should be bigger.  How much bigger?
//
#define BAR_TREE_NUM_CHILDREN 2

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


//
// Comm diagnostic information.  The local_commDiagnostics_t type must
// be kept in sync with the public chpl_commDiagnostics type defined
// in chpl-comm.h.  We need our own diagnostic counter type because we
// want to use atomic operations, and doing so requires specific types
// for the individual counters.  Strictly speaking it is not necessary
// that we put the individual counters in a struct type, but it seems
// best to maintain similarity with how things are done elsewhere.
//
typedef struct {
  atomic_uint_least64_t get;
  atomic_uint_least64_t get_nb;
  atomic_uint_least64_t put;
  atomic_uint_least64_t put_nb;
  atomic_uint_least64_t test_nb;
  atomic_uint_least64_t wait_nb;
  atomic_uint_least64_t try_nb;
  atomic_uint_least64_t execute_on;
  atomic_uint_least64_t execute_on_fast;
  atomic_uint_least64_t execute_on_nb;
} local_commDiagnostics_t;

static local_commDiagnostics_t comm_diagnostics;

static chpl_bool comm_diags_disabled_temporarily = false;


//
// Specialized argument type and values for the may_remote_proxy
// argument to do_remote_put() and ..._get().
//
typedef enum {
  may_proxy_false,
  may_proxy_true
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
static void      register_memory(void);
static chpl_bool get_next_rw_memory_range(uint64_t*, uint64_t*, char*, size_t);
static void      register_mem_region(mem_region_t*);
static void      deregister_mem_region(mem_region_t*);
static mem_region_t* mreg_for_addr(void*, mem_region_table_t*);
static mem_region_t* mreg_for_local_addr(void*);
static mem_region_t* mreg_for_remote_addr(void*, c_nodeid_t);
static void      polling_task(void*);
static void      set_up_for_polling(void);
static void      exit_all(int);
static void      exit_any(int);
static void      rf_handler(gni_cq_entry_t*, void*);
static void      fork_call_wrapper_blocking(chpl_comm_on_bundle_t*);
static void      fork_call_wrapper_large(fork_large_call_task_t*);
static void      fork_get_wrapper(fork_xfer_task_t*);
static size_t    do_amo_on_cpu(fork_amo_cmd_t, void*, void*, void*, void*);
static void      fork_amo_wrapper(fork_amo_info_t*);
static void      release_req_buf(uint32_t, int, int);
static void      indicate_done(fork_base_info_t* b);
static void      indicate_done2(int, rf_done_t *);
static nb_desc_idx_t nb_desc_idx_encode(int, int);
static void      nb_desc_idx_decode(int*, int*, nb_desc_idx_t);
static nb_desc_t* nb_desc_idx_2_ptr(nb_desc_idx_t);
static chpl_comm_nb_handle_t nb_desc_idx_2_handle(nb_desc_idx_t);
static nb_desc_idx_t nb_desc_handle_2_idx(chpl_comm_nb_handle_t);
static void      nb_desc_init(void);
static nb_desc_idx_t nb_desc_alloc(void);
static void      nb_desc_free(nb_desc_idx_t);
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
                               drpg_may_proxy_t);
static void      do_remote_put_V(int, void**, c_nodeid_t*, void**, size_t*,
                                 drpg_may_proxy_t);
static void      do_remote_get(void*, c_nodeid_t, void*, size_t,
                               drpg_may_proxy_t);
static void      do_nic_get(void*, c_nodeid_t, mem_region_t*,
                            void*, size_t, mem_region_t*);
static int       amo_cmd_2_nic_op(fork_amo_cmd_t, int);
static void      do_nic_amo(void*, void*, c_nodeid_t, void*, size_t,
                            gni_fma_cmd_type_t, void*);
static void      amo_add_real32_cpu_cmpxchg(void*, void*, void*);
static void      amo_add_real64_cpu_cmpxchg(void*, void*, void*);
static void      fork_call_common(int, c_sublocid_t,
                                  chpl_fn_int_t,
                                  chpl_comm_on_bundle_t*, size_t,
                                  chpl_bool, chpl_bool);
static void      fork_put(void*, c_nodeid_t, void*, size_t);
static void      fork_get(void*, c_nodeid_t, void*, size_t);
static void      fork_free(c_nodeid_t, void*);
static void      fork_amo(fork_t*, c_nodeid_t);
static void      do_fork_post(c_nodeid_t, rf_done_t**,
                              uint64_t, fork_base_info_t*, int*, int*);
static void      acquire_comm_dom(void);
static void      acquire_comm_dom_and_req_buf(c_nodeid_t, int*);
static void      release_comm_dom(void);
static chpl_bool reacquire_comm_dom(int);
static int       post_fma(c_nodeid_t, gni_post_descriptor_t*);
static void      post_fma_and_wait(c_nodeid_t, gni_post_descriptor_t*, chpl_bool);
static int       post_fma_ct(c_nodeid_t*, gni_post_descriptor_t*);
static void      post_fma_ct_and_wait(c_nodeid_t*, gni_post_descriptor_t*);
static void      local_yield(void);


//
// More debug support.
//
#ifdef DEBUG
static void dbg_init(void);
static void dbg_init(void)
{
  const char* ev;
  uint64_t flg;

  atomic_init_uint_least32_t(&next_thread_idx, 0);
  debug_file = stderr;

  if ((ev = chpl_get_rt_env("COMM_UGNI_DEBUG", NULL)) != NULL
      && sscanf(ev, "%" SCNi64, &flg) == 1) {
    debug_flag = flg;

    if ((ev = chpl_get_rt_env("COMM_UGNI_DEBUG_NODE", NULL)) != NULL) {
      int nodeID;
      if (sscanf(ev, "%i", &nodeID) == 1) {
        debug_nodeID = nodeID;
        debug_flag |= DBGF_1_NODE;
      }
    }
  }

  if ((ev =  chpl_get_rt_env("COMM_UGNI_DEBUG_STATS", NULL)) != NULL
      && sscanf(ev, "%" SCNi64, &flg) == 1) {
    debug_stats_flag = flg;
  }

  if ((ev = chpl_get_rt_env("COMM_UGNI_FORK_REQ_BUFS_PER_CD", NULL)) != NULL
      && sscanf(ev, "%d", &FORK_REQ_BUFS_PER_CD) != 1)
    CHPL_INTERNAL_ERROR("bad FORK_REQ_BUFS_PER_CD env var value");
}


static void dbg_init_output_file(void);
static void dbg_init_output_file(void)
{
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

  //
  // Sanity check: the compiler preserves the order of the members of
  // mem_region_table_t.  (Otherwise the code for broadcasting memory
  // registration updates won't work.)
  //
  assert(offsetof(mem_region_table_t, mreg_cnt)
         < offsetof(mem_region_table_t, mregs));

  if (fork_op_num_ops > (1 << FORK_OP_BITS))
    CHPL_INTERNAL_ERROR("too many fork OPs for internal encoding");

  DBG_INIT();

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

  DBG_INIT_OUTPUT_FILE();  // needs chpl_nodeID

  {
    int app_size;

    if (PMI_Get_size(&app_size) != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Get_size() failed");
    chpl_numNodes = (int32_t) app_size;
    if (chpl_numNodes > (1 << _IID_LI_BITS))
      CHPL_INTERNAL_ERROR("too many locales for internal encoding");
  }

  {
    gni_return_t gni_rc;

    if ((gni_rc = GNI_GetDeviceType(&nic_type)) != GNI_RC_SUCCESS)
      GNI_FAIL(gni_rc, "GNI_GetDeviceType() failed");
    if (nic_type != GNI_DEVICE_GEMINI
        && nic_type != GNI_DEVICE_ARIES)
      CHPL_INTERNAL_ERROR("unexpected GNI device type");
  }

#define _PSV_INIT(psv) atomic_init_uint_least64_t(&_PSV_VAR(psv), 0);
  PERFSTATS_DO_ALL(_PSV_INIT);
#undef _PSTAT_INIT
}


void chpl_comm_post_mem_init(void)
{
  //
  // Empty.
  //
}


//
// No support for gdb for now
//
int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status)
{
  return 0;
}


int chpl_comm_numPollingTasks(void)
{
  return 1;
}


void chpl_comm_post_task_init(void)
{
  if (chpl_numNodes == 1)
    return;

  //
  // Now that the memory layer has been set up, we can allocate the various
  // data we need and fill it in.  Some of this need not be shared and so we
  // could have allocated it earlier, but the logical flow is clearer if we
  // do all this in one place.
  //

  //
  // Get our NIC address and share it around the job.
  //
  // To avoid having to link in MPI, we use PMI_Allgather() here.  But
  // that routine yields unordered results, so we have to figure out
  // which nic_addr_map identifier goes with which locale ourselves.  We
  // do this by gathering (locale, nic_addr) pairs and then scattering
  // the nic_addr values into the map, which is indexed by locale.
  //
  {
    uint32_t nic_addr;

    nic_addr = gni_get_nic_address(0);
    // chpl_comm_mem_reg no: not communicated
    nic_addr_map =
        (uint32_t*) chpl_mem_allocMany(chpl_numNodes, sizeof(nic_addr_map[0]),
                                       CHPL_RT_MD_COMM_PER_LOC_INFO,
                                       0, 0);

    {
      typedef struct {
        c_nodeid_t locale;
        uint32_t gather_val;
      } gdata_t;

      gdata_t  my_gdata = { chpl_nodeID, nic_addr };
      gdata_t* gdata;

      // chpl_comm_mem_reg no: not communicated
      gdata = (gdata_t*) chpl_mem_allocMany(chpl_numNodes, sizeof(gdata[0]),
                                            CHPL_RT_MD_COMM_PER_LOC_INFO,
                                            0, 0);
      if (PMI_Allgather(&my_gdata, gdata, sizeof(gdata[0])) != PMI_SUCCESS)
        CHPL_INTERNAL_ERROR("PMI_Allgather(nic_addr_map) failed");

      for (int i = 0; i < chpl_numNodes; i++)
        nic_addr_map[gdata[i].locale] = gdata[i].gather_val;

      chpl_mem_free(gdata, 0, 0);
    }
  }

  compute_comm_dom_cnt();

  //
  // We now have all the variable values (specifically: number of
  // nodes and max number of comm domains on any node) that go into
  // computing how much NIC-registered memory we need.  Tell the
  // registered-memory module how much we need and then allocate it.
  //
  get_buf_pre_init();
  rf_done_pre_init();
  amo_res_pre_init();

  chpl_comm_mem_reg_init();

  get_buf_init();
  nb_desc_init();
  rf_done_init();
  amo_res_init();

  //
  // Create all the communication domains, including their GNI NIC
  // handles, endpoints, and completion queues.
  //
  comm_doms =
    (comm_dom_t*) chpl_mem_allocMany(comm_dom_cnt, sizeof(comm_doms[0]),
                                     CHPL_RT_MD_COMM_PER_LOC_INFO, 0, 0);

  for (int i = 0; i < comm_dom_cnt; i++)
    gni_setup_per_comm_dom(i);

  comm_dom_free_idx = 0;

  //
  // Register memory.
  //
  register_memory();

  //
  // Share the per-locale memory descriptors around the job.  These are
  // the ones we will use for the remote sides of GETs and PUTs.  While
  // we're at it, also share the barrier info struct addresses around
  // the job.
  //
  mem_regions_map =
    (mem_region_table_t*) chpl_mem_allocMany(chpl_numNodes,
                                             sizeof(mem_regions_map[0]),
                                             CHPL_RT_MD_COMM_PER_LOC_INFO,
                                             0, 0);
  mem_regions_map_addr_map =
    (mem_region_table_t**)
      chpl_mem_allocMany(chpl_numNodes,
                         sizeof(mem_regions_map_addr_map[0]),
                         CHPL_RT_MD_COMM_PER_LOC_INFO,
                         0, 0);
  bar_min_child = BAR_TREE_NUM_CHILDREN * chpl_nodeID + 1;
  if (bar_min_child >= chpl_numNodes)
    bar_num_children = 0;
  else {
    bar_num_children = BAR_TREE_NUM_CHILDREN;
    if (bar_min_child + bar_num_children >= chpl_numNodes)
      bar_num_children = chpl_numNodes - bar_min_child;
  }
  bar_parent = (chpl_nodeID - 1) / BAR_TREE_NUM_CHILDREN;

  {
    typedef struct {
      c_nodeid_t locale;
      mem_region_table_t mem_regions;
      mem_region_table_t* mem_regions_map;
      barrier_info_t* bar_info;
    } gdata_t;

    gdata_t my_gdata = { chpl_nodeID, mem_regions, mem_regions_map, &bar_info };
    gdata_t* gdata;

    gdata = (gdata_t*) chpl_mem_allocMany(chpl_numNodes, sizeof(gdata[0]),
                                          CHPL_RT_MD_COMM_PER_LOC_INFO,
                                          0, 0);
    if (PMI_Allgather(&my_gdata, gdata, sizeof(gdata[0])) != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Allgather(sdata/heap/etc. memory maps) failed");

    for (int i = 0; i < chpl_numNodes; i++) {
      mem_regions_map[gdata[i].locale] = gdata[i].mem_regions;
      mem_regions_map_addr_map[gdata[i].locale] = gdata[i].mem_regions_map;

      if (gdata[i].locale >= bar_min_child
          && gdata[i].locale < bar_min_child + bar_num_children)
        child_bar_info[gdata[i].locale - bar_min_child] =
          gdata[i].bar_info;
      else if (chpl_nodeID != 0 && gdata[i].locale == bar_parent)
        parent_bar_info = gdata[i].bar_info;
    }

    chpl_mem_free(gdata, 0, 0);
  }

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

  //
  // Initialize the caching layer, if it is active.
  //
  chpl_cache_init();
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
  gni_return_t gni_rc;

  p_ptr = getenv("PMI_GNI_DEV_ID");
  if (!p_ptr) {
      if ((gni_rc = GNI_CdmGetNicAddress(device_id, &address, &cpu_id))
          != GNI_RC_SUCCESS)
      GNI_FAIL(gni_rc, "GNI_CdmGetNicAddress() failed");
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
  // at least enough that every processor PU we could be using can
  // have one at the same time.  In the unlikely event that none of
  // this was done or is known, make 16.  In any case, always add one
  // for the polling task.
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
      uint32_t num_PUs;

      if ((num_PUs = chpl_getNumLogicalCpus(true)) > 0)
        comm_dom_cnt = num_PUs;
    }

    if (comm_dom_cnt == 0)
      comm_dom_cnt = 16;
  }

  comm_dom_cnt++;  // count the polling task's dedicated comm domain

  //
  // For now, limit us to 30 communication domains.  (The Gemini NIC
  // only supports up to 32 anyway.  Aries supports 128, but it isn't
  // clear that we can make use of more than about 30.)
  //
  // TODO: Eventually we should collect statistics and figure out how
  //       many comm domains we actually need, that is, the most that we
  //       can really keep busy.
  //
  if (comm_dom_cnt > 30)
    comm_dom_cnt = 30;

  if (comm_dom_cnt >= (1 << _IID_CDI_BITS))
    CHPL_INTERNAL_ERROR("too many comm domains for internal encoding");
}


static
void gni_setup_per_comm_dom(int cdi)
{
  comm_dom_t*  cd = &comm_doms[cdi];
  gni_return_t gni_rc;
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
  CQ_CNT_STORE(cd, 0);
  if ((gni_rc = GNI_CqCreate(cd->nih, cd->cq_cnt_max, 0, GNI_CQ_NOBLOCK, NULL,
                             NULL, &cd->cqh))
      != GNI_RC_SUCCESS)
    GNI_FAIL(gni_rc, "GNI_CqCreate(cqh) failed");

  //
  // Create GNI EndPoints (EPs) for the nodes, and bind them to their
  // nodes.  This can be replaced by an EP-Pool for better scalability.
  //
  // chpl_comm_mem_reg no: not communicated
  cd->remote_eps =
    (gni_ep_handle_t*) chpl_mem_allocMany(chpl_numNodes,
                                          sizeof(cd->remote_eps[0]),
                                          CHPL_RT_MD_COMM_PER_LOC_INFO,
                                          0, 0);
  for (i = 0; i < chpl_numNodes; i++) {
    if ((gni_rc = GNI_EpCreate(cd->nih, cd->cqh, &cd->remote_eps[i]))
        != GNI_RC_SUCCESS)
      GNI_FAIL(gni_rc, "GNI_EpCreate(cd->remote_eps[i]) failed");

      if ((gni_rc = GNI_EpBind(cd->remote_eps[i], nic_addr_map[i],
                               GNI_INST_ID(i, 0, 0)))
        != GNI_RC_SUCCESS)
      GNI_FAIL(gni_rc, "GNI_EpBind(cd->remote_eps[i]) failed");
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
  gni_return_t     gni_rc;
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
  if ((gni_rc = GNI_CdmCreate(GNI_INST_ID(chpl_nodeID, cdi, 0),
                              ptag, cookie, modes, &cdm_handle))
      != GNI_RC_SUCCESS)
    GNI_FAIL(gni_rc, "GNI_CdmCreate() failed");

  if ((gni_rc = GNI_CdmAttach(cdm_handle, device_id, &local_address, nih))
      != GNI_RC_SUCCESS)
    GNI_FAIL(gni_rc, "GNI_CdmAttach() failed");
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
void register_memory(void)
{
  uint64_t  addr;
  uint64_t  len;
  char      pathname[100];
  void*     mem_reg_addr;
  size_t    mem_reg_size;
  int       have_hugepage_module
              = (getenv("HUGETLB_DEFAULT_PAGE_SIZE") != NULL);

  //
  // Register read/write memory regions found in /proc/self/maps.  If
  // the HUGETLB_DEFAULT_PAGE_SIZE environment variable is present,
  // indicating that we are using a craype-hugepage* module, then
  // register only non-anonymous regions (those associated with a path).
  // However, don't register device memory other than from /dev/zero.
  // This gets us the hugepage regions, the stack, and some other useful
  // things.  Only the MAX_MEM_REGIONS largest regions are registered,
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
  chpl_comm_mem_reg_tell(&mem_reg_addr, &mem_reg_size);

  mem_regions.mreg_cnt = 0;

  DBG_CATF(DBGF_MEMMAPS, debug_file, "/proc/self/maps", NULL);
  DBG_CATF(DBGF_MEMMAPS, debug_file, "/proc/self/numa_maps", NULL);

  while (get_next_rw_memory_range(&addr, &len, pathname, sizeof(pathname))) {
    int i;

    //
    // This is slightly easier to understand in the positive sense.
    // We skip everything except:
    //   - the guaranteed-registered memory region, if any, or
    //   - if we have hugepages, anything that has a path (isn't
    //     anonymous) but isn't a device other than /dev/zero.
    //
    if (! (addr == (uint64_t) (intptr_t) mem_reg_addr
           || (have_hugepage_module
               && strlen(pathname) > 0
               && (strncmp(pathname, "/dev/", 5) != 0
                   || (strncmp(pathname, "/dev/zero", 9) == 0
                       && (pathname[9] == ' '
                           || pathname[9] == '\0'))))))
      continue;

    //
    // Put the memory regions in the table, sorted in order of
    // decreasing size.
    //
    for (i = mem_regions.mreg_cnt;
         i > 0 && len > mem_regions.mregs[i - 1].len;
         i--) {
      if (i < MAX_MEM_REGIONS) {
        mem_regions.mregs[i] = mem_regions.mregs[i - 1];
      }
    }

    if (i == MAX_MEM_REGIONS && addr == (uint64_t) (intptr_t) mem_reg_addr)
      i--;

    if (i < MAX_MEM_REGIONS) {
      mem_regions.mregs[i].addr = addr;
      mem_regions.mregs[i].len = len;
      if (mem_regions.mreg_cnt < MAX_MEM_REGIONS) {
        mem_regions.mreg_cnt++;
#ifdef DEBUG
        if (mem_regions.mreg_cnt > mreg_cnt_max)
          mreg_cnt_max = mem_regions.mreg_cnt;
#endif
      }
    }
  }

  if (mem_regions.mreg_cnt == 0) {
    CHPL_INTERNAL_ERROR("no registerable memory regions?");
  }

  //
  // Now, register the recorded memory regions with uGNI.
  //
  for (int i = 0; i < mem_regions.mreg_cnt; i++) {
    register_mem_region(&mem_regions.mregs[i]);
  }

  //
  // Find the memory region associated with guaranteed NIC-registered
  // memory.  Recording this saves time looking it up later.
  //
  {
    void* p;
    size_t s;

    chpl_comm_mem_reg_tell(&p, &s);

    for (int i = 0; i < mem_regions.mreg_cnt; i++) {
      if ((void*) (intptr_t) mem_regions.mregs[i].addr == p) {
        gnr_mreg = &mem_regions.mregs[i];
        break;
      }
    }

    if (gnr_mreg == NULL)
      CHPL_INTERNAL_ERROR("cannot find gnr_mreg");
  }
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
void register_mem_region(mem_region_t* mr)
{
  uint32_t flags = GNI_MEM_READWRITE | GNI_MEM_RELAXED_PI_ORDERING;
  gni_return_t gni_rc;

  DBG_P_L(DBGF_MEMREG,
          "GNI_MemRegister[%d](%" PRIx64 ", %" PRIx64 ")",
          (int) (mr - &mem_regions.mregs[0]), mr->addr, mr->len);
  if ((gni_rc = GNI_MemRegister(comm_doms[0].nih, mr->addr, mr->len,
                                NULL, flags, -1, &mr->mdh))
      != GNI_RC_SUCCESS) {
    GNI_FAIL(gni_rc, "GNI_MemRegister() failed");
  }
}


static
void deregister_mem_region(mem_region_t* mr)
{
  gni_return_t gni_rc;

  DBG_P_L(DBGF_MEMREG,
          "GNI_MemDeregister[%d]",
          (int) (mr - &mem_regions.mregs[0]));
  if ((gni_rc = GNI_MemDeregister(comm_doms[0].nih, &mr->mdh))
      != GNI_RC_SUCCESS) {
    GNI_FAIL(gni_rc, "GNI_MemDeregister() failed");
  }
}


static
inline
mem_region_t* mreg_for_addr(void* addr, mem_region_table_t* tab)
{
  uint64_t addr_ui = (uint64_t) addr;
  mem_region_t* mr;

  mr = tab->mregs;
  for (int i = 0; i < tab->mreg_cnt; i++, mr++) {
    if (addr_ui >= mr->addr && addr_ui < mr->addr + mr->len)
      return mr;
  }

  return NULL;
}


static
inline
mem_region_t* mreg_for_local_addr(void* addr)
{
  return mreg_for_addr(addr, &mem_regions);
}


static
inline
mem_region_t* mreg_for_remote_addr(void* addr, c_nodeid_t locale)
{
  return mreg_for_addr(addr, &mem_regions_map[locale]);
}


static
void polling_task(void* ignore)
{
  gni_cq_entry_t ev;
  gni_return_t   gni_rc;

  set_up_for_polling();

  polling_task_running = true;

  while (!polling_task_please_exit) {
    //
    // Each time GNI_CqGetEvent() succeeds, we call the event (request)
    // handler.  See the comments there for more info.
    //
    gni_rc = GNI_CqGetEvent(rf_cqh, &ev);
    if (gni_rc == GNI_RC_NOT_DONE) {
      //
      // On the offhand chance we're oversubscribed, we yield the
      // processor if there isn't anything for us to do.  And if we're
      // not oversubscribed, the trip through sched_yield(2) if no
      // other process or pthread can run is quite quick.
      //
      sched_yield();
    }
    else if (gni_rc == GNI_RC_SUCCESS)
      rf_handler(&ev, NULL);
    else
      GNI_CQ_EV_FAIL(gni_rc, ev, "GNI_CqGetEvent(rf_cqh) failed in polling");
  }

  polling_task_done = true;
}


static
void set_up_for_polling(void)
{
  cq_cnt_t     cq_cnt;
  gni_return_t gni_rc;
  uint32_t     i;

  //
  // Grab a communication domain permanently.
  //
  acquire_comm_dom();
  cd->firmly_bound = true;

  //
  // Figure out the maximum number of communication domains on any
  // locale.
  //
  {
    typedef struct {
      c_nodeid_t locale;
      uint32_t gather_val;
    } gdata_t;

    gdata_t  my_gdata = { chpl_nodeID, comm_dom_cnt };
    gdata_t* gdata;

    // chpl_comm_mem_reg no: not communicated
    gdata =
      (gdata_t*) chpl_mem_allocMany(chpl_numNodes, sizeof(gdata[0]),
                                    CHPL_RT_MD_COMM_PER_LOC_INFO,
                                    0, 0);
    if (PMI_Allgather(&my_gdata, gdata, sizeof(gdata[0])) != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Allgather(comm_dom_cnt) failed");

    comm_dom_cnt_max = gdata[0].gather_val;
    for (i = 1; i < chpl_numNodes; i++) {
      if (gdata[i].gather_val > comm_dom_cnt_max)
        comm_dom_cnt_max = gdata[i].gather_val;
    }

    chpl_mem_free(gdata, 0, 0);
  }

  //
  // Make the fork request and acknowledgement space, and communicate
  // the location of that space across the locales.
  //
  fork_reqs =
    (fork_t*) chpl_comm_mem_reg_allocMany(FORK_REQ_BUFS_PER_LOCALE,
                                          sizeof(fork_reqs[0]),
                                          CHPL_RT_MD_COMM_FRK_RCV_INFO,
                                          0, 0);

  // chpl_comm_mem_reg no: not communicated
  fork_reqs_map =
    (fork_t**) chpl_mem_allocMany(chpl_numNodes, sizeof(fork_reqs_map[0]),
                                  CHPL_RT_MD_COMM_PER_LOC_INFO,
                                  0, 0);

  fork_reqs_free =
    (chpl_bool32*) chpl_comm_mem_reg_allocMany(FORK_REQ_BUFS_PER_LOCALE,
                                               sizeof(fork_reqs_free[0]),
                                               CHPL_RT_MD_COMM_FRK_DONE_FLAG,
                                               0, 0);
  for (uint32_t i = 0; i < FORK_REQ_BUFS_PER_LOCALE; i++)
    fork_reqs_free[i] = true;

  // chpl_comm_mem_reg no: not communicated
  fork_reqs_free_map =
    (chpl_bool32**) chpl_mem_allocMany(chpl_numNodes,
                                       sizeof(fork_reqs_free_map[0]),
                                       CHPL_RT_MD_COMM_PER_LOC_INFO,
                                       0, 0);

  {
    typedef struct {
      c_nodeid_t   locale;
      fork_t*      gather_fork_reqs;
      chpl_bool32* gather_fork_reqs_free;
    } gdata_t;

    gdata_t  my_gdata = { chpl_nodeID,
                          fork_reqs, (chpl_bool32*) fork_reqs_free };
    gdata_t* gdata;

    // chpl_comm_mem_reg no: not communicated
    gdata = (gdata_t*) chpl_mem_allocMany(chpl_numNodes, sizeof(gdata[0]),
                                          CHPL_RT_MD_COMM_PER_LOC_INFO,
                                          0, 0);
    if (PMI_Allgather(&my_gdata, gdata, sizeof(gdata[0])) != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Allgather(fork_reqs_map) failed");

    for (i = 0; i < chpl_numNodes; i++) {
      fork_reqs_map[gdata[i].locale]      = gdata[i].gather_fork_reqs;
      fork_reqs_free_map[gdata[i].locale] = gdata[i].gather_fork_reqs_free;
    }

    chpl_mem_free(gdata, 0, 0);
  }

  //
  // Create remote fork completion queue.
  //
  cq_cnt = FORK_REQ_BUFS_PER_LOCALE;
  if ((gni_rc = GNI_CqCreate(cd->nih, cq_cnt, 0, GNI_CQ_NOBLOCK, NULL, NULL,
                             &rf_cqh))
      != GNI_RC_SUCCESS)
    GNI_FAIL(gni_rc, "GNI_CqCreate(rf_cqh) failed");

  //
  // Register the fork request memory.
  //
  {
    uint64_t addr  = (uint64_t) (intptr_t) fork_reqs;
    uint64_t len   = (uint64_t)
                     (FORK_REQ_BUFS_PER_LOCALE * sizeof(fork_reqs[0]));
    uint32_t flags = GNI_MEM_READWRITE | GNI_MEM_RELAXED_PI_ORDERING;

    DBG_P_L(DBGF_MEMREG,
            "RemFork space: GNI_MemRegister(%" PRIx64 ", %" PRIx64")",
            addr, len);
    if ((gni_rc = GNI_MemRegister(cd->nih, addr, len, rf_cqh,
                                  flags, -1, &rf_mdh))
        != GNI_RC_SUCCESS)
      GNI_FAIL(gni_rc, "GNI_MemRegister(fork requests) failed");
  }

  //
  // Share the per-locale fork request memory descriptors around the
  // job.
  //
  // chpl_comm_mem_reg no: not communicated
  rf_mdh_map =
    (gni_mem_handle_t*) chpl_mem_allocMany(chpl_numNodes,
                                           sizeof(rf_mdh_map[0]),
                                           CHPL_RT_MD_COMM_PER_LOC_INFO,
                                           0, 0);

  {
    typedef struct {
      c_nodeid_t       locale;
      gni_mem_handle_t gather_val;
    } gdata_t;

    gdata_t  my_gdata = { chpl_nodeID, rf_mdh };
    gdata_t* gdata;

    // chpl_comm_mem_reg no: not communicated
    gdata = (gdata_t*) chpl_mem_allocMany(chpl_numNodes, sizeof(gdata[0]),
                                          CHPL_RT_MD_COMM_PER_LOC_INFO,
                                          0, 0);
    if (PMI_Allgather(&my_gdata, gdata, sizeof(gdata[0])) != PMI_SUCCESS)
      CHPL_INTERNAL_ERROR("PMI_Allgather(rf_mdh_map) failed");

    for (i = 0; i < chpl_numNodes; i++)
      rf_mdh_map[gdata[i].locale] = gdata[i].gather_val;

    chpl_mem_free(gdata, 0, 0);
  }
}


void chpl_comm_rollcall(void)
{
  atomic_init_uint_least64_t(&comm_diagnostics.get, 0);
  atomic_init_uint_least64_t(&comm_diagnostics.get_nb, 0);
  atomic_init_uint_least64_t(&comm_diagnostics.put, 0);
  atomic_init_uint_least64_t(&comm_diagnostics.put_nb, 0);
  atomic_init_uint_least64_t(&comm_diagnostics.test_nb, 0);
  atomic_init_uint_least64_t(&comm_diagnostics.wait_nb, 0);
  atomic_init_uint_least64_t(&comm_diagnostics.try_nb, 0);
  atomic_init_uint_least64_t(&comm_diagnostics.execute_on, 0);
  atomic_init_uint_least64_t(&comm_diagnostics.execute_on_fast, 0);
  atomic_init_uint_least64_t(&comm_diagnostics.execute_on_nb, 0);

  chpl_msg(2, "executing on node %d of %d node(s): %s\n", chpl_nodeID,
           chpl_numNodes, chpl_nodeName());

  if (chpl_numNodes == 1)
    return;
}


static void make_shared_heap(void)
{
  assert(!registered_heap_info_set);

  if (chpl_numNodes == 1) {
    using_hugepages = false;
    registered_heap_start = NULL;
    registered_heap_size  = 0;
    registered_heap_info_set = 1;
    return;
  }

  using_hugepages = (getenv("HUGETLB_MORECORE") == NULL) ? false : true;
  if (using_hugepages) {
    //
    // If the heap is supposed to be on hugepages, acquire that space
    // now.
    //
    const size_t page_size = hugepage_size = gethugepagesize();
    const size_t nic_max_pages = (size_t) 1 << 14; // not publicly defined
    const size_t nic_max_mem = nic_max_pages * page_size;
    size_t nic_allowed_mem;
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
    if (nic_type == GNI_DEVICE_GEMINI)
      nic_allowed_mem = ALIGN_DN((size_t) (0.95 * nic_max_mem), page_size);
    else
      nic_allowed_mem = nic_max_mem;

    {
      uint64_t  addr;
      uint64_t  len;
      size_t    data_size;

      data_size = 0;
      while (get_next_rw_memory_range(&addr, &len, NULL, 0))
        data_size += ALIGN_UP(len, page_size);

      if (data_size >= nic_allowed_mem)
        max_heap_size = 0;
      else
        max_heap_size = nic_allowed_mem - data_size;
    }

    if ((size = chpl_comm_getenvMaxHeapSize()) > 0) {
      //
      // The user specified a size.  Go with that, but issue a warning
      // from node 0 if we can't fit all the registrations in the NIC
      // TLB.  On Gemini only, reduce the heap size until we can fit in
      // the NIC TLB, because otherwise we'll get GNI_RC_ERROR_RESOURCE
      // when we try to register memory.
      //
      if (size > max_heap_size) {
        if (chpl_nodeID == 0) {
          char nmmBuf[20];
          char psBuf[20];
          char hsBuf[20];
          char msg[200];

#define P_GMK_BASE(b, f, v, t)                                          \
          ((v >= ((t) (1UL << 30)))                                     \
           ? snprintf(b, sizeof(b), "%" f "G", v / ((t) (1UL << 30)))   \
           : (v >= ((t) (1UL << 20)))                                   \
           ? snprintf(b, sizeof(b), "%" f "M", v / ((t) (1UL << 20)))   \
           : snprintf(b, sizeof(b), "%" f "K", v / ((t) (1UL << 10))))
#define P_ZI_GMK(b, v) P_GMK_BASE(b, "zd", v, size_t)
#define P_D_GMK(b, v)  P_GMK_BASE(b, ".1f", v, double)

          P_ZI_GMK(nmmBuf, nic_max_mem);
          P_ZI_GMK(psBuf, page_size);

          if (nic_type == GNI_DEVICE_GEMINI) {
            P_D_GMK(hsBuf, max_heap_size);
            (void) snprintf(msg, sizeof(msg),
                            "Gemini TLB can cover %s with %s pages; heap "
                            "reduced to %s to fit",
                            nmmBuf, psBuf, hsBuf);
          } else {
            P_ZI_GMK(hsBuf, size);
            (void) snprintf(msg, sizeof(msg),
                            "Aries TLB cache can cover %s with %s pages; "
                            "with %s heap,\n"
                            "         cache refills may reduce performance",
                            nmmBuf, psBuf, hsBuf);
          }

          chpl_warning(msg, 0, 0);

#undef P_D_GMK
#undef P_ZI_GMK
#undef P_GMK_BASE
        }

        if (nic_type == GNI_DEVICE_GEMINI)
          size = max_heap_size;
      }
    }
    else {
      //
      // The user didn't specify a size.  Start with 16GB or the most
      // we can fit, whichever is less.
      //
      size = (size_t) 16 << 30;
      if (size > max_heap_size)
        size = max_heap_size;
    }

    //
    // Work our way down from the starting size in (roughly) 5% steps
    // until we can actually get that much from the system.
    //
    size = ALIGN_DN(size, page_size);
    if ((decrement = ALIGN_DN((size_t) (0.05 * size), page_size)) < page_size)
      decrement = page_size;

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

    registered_heap_size  = size;
    registered_heap_start = start;
  }
  else {
    registered_heap_size  = 0;
    registered_heap_start = NULL;
  }

  registered_heap_info_set = 1;
}


void chpl_comm_get_registered_heap(void** start_p, size_t* size_p)
{
  if (!registered_heap_info_set)
    make_shared_heap();

  assert(registered_heap_info_set);
  *start_p = registered_heap_start;
  *size_p  = registered_heap_size;
}


inline
size_t chpl_comm_impl_regMemAllocThreshold(void)
{
  if (using_hugepages)
    return 2 * hugepage_size;
  return SIZE_MAX;
}


void* chpl_comm_impl_regMemAlloc(size_t size)
{
  int mr_i;
  void* p;

  if (!using_hugepages || size < chpl_comm_impl_regMemAllocThreshold())
    return NULL;

  PERFSTATS_INC(regMem_cnt);

  mem_regions_lock();

  //
  // Do we have room for another registered memory table entry?
  //
  for (mr_i = 0;
       mr_i < MAX_MEM_REGIONS && mem_regions.mregs[mr_i].addr != 0;
       mr_i++)
    ;

  p = NULL;
  if (mr_i < MAX_MEM_REGIONS) {
    mem_region_t* mr = &mem_regions.mregs[mr_i];

    p = get_huge_pages(ALIGN_UP(size, hugepage_size), GHP_DEFAULT);

    //
    // If we got the memory, reserve the memory region slot we found.
    //
    if (p == NULL) {
      DBG_P_LP(DBGF_MEMREG,
               "chpl_regMemAlloc(%#zx): no hugepages",
               size);
    } else {
      mr->addr = (uint64_t) (intptr_t) p;
      mr->len = 1;

      //
      // Adjust the region count, if necessary.
      //
      if (mr_i >= mem_regions.mreg_cnt) {
        mem_regions.mreg_cnt = mr_i + 1;
#ifdef DEBUG
        if (mem_regions.mreg_cnt > mreg_cnt_max)
          mreg_cnt_max = mem_regions.mreg_cnt;
#endif
      }

      DBG_P_LP(DBGF_MEMREG,
               "chpl_regMemAlloc(%" PRIx64 "): "
               "mregs[%d] = %" PRIx64 ", cnt %d",
               size, mr_i, mr->addr, (int) mem_regions.mreg_cnt);
    }
  } else {
    static int spoke = 0;

    if (!spoke) {
      chpl_warning("out of registered memory region table entries!", 0, 0);
      spoke = 1;
    }

    DBG_P_LP(DBGF_MEMREG,
             "chpl_regMemAlloc(%#zx): no free table entries",
             size);
  }

  mem_regions_unlock();

  return p;
}


void chpl_comm_impl_regMemPostAlloc(void* p, size_t size)
{
  mem_region_t* mr;
  int mr_i;

  if (!using_hugepages || size < chpl_comm_impl_regMemAllocThreshold())
    CHPL_INTERNAL_ERROR("chpl_comm_regMemPostAlloc(): this isn't my memory");

  DBG_P_LP(DBGF_MEMREG,
           "chpl_comm_regMemPostAlloc(%p, %" PRIx64 ")",
           p, size);

  //
  // Find the memory region table entry for this memory.
  //
  mr = mreg_for_addr(p, &mem_regions);
  if (mr == NULL)
    CHPL_INTERNAL_ERROR("chpl_comm_regMemPostAlloc(): can't find the memory");

  mr_i = (int) (mr - &mem_regions.mregs[0]);

  mem_regions_lock();

  //
  // Finish filling the table entry and register the memory.
  //
  mr->len = (uint64_t) size;

  register_mem_region(mr);

  //
  // Update the copies of our memory regions on all nodes.  If the new
  // entry doesn't extend our table then we just send the entry.  If it
  // does, then we need to send both the new count and the new entry.
  // And so we can do it with just one PUT, we also send everything in
  // between.
  //
  {
    size_t off;
    size_t size;
    void* src_v[MAX_CHAINED_PUT_LEN];
    int32_t node_v[MAX_CHAINED_PUT_LEN];
    void* tgt_v[MAX_CHAINED_PUT_LEN];
    size_t size_v[MAX_CHAINED_PUT_LEN];
    int ci;

    if (mem_regions.mreg_cnt == mem_regions_map[chpl_nodeID].mreg_cnt) {
      off = (char*) mr - (char*) &mem_regions;
      size = sizeof(*mr);
    } else {
      assert(mem_regions.mreg_cnt
             > mem_regions_map[chpl_nodeID].mreg_cnt);
      off = 0;
      size = (char*) &mem_regions.mregs[mr_i + 1] - (char*) &mem_regions;
    }

    ci = 0;
    for (int ni = 0; ni < (int) chpl_numNodes; ni++) {
      if (ci == MAX_CHAINED_PUT_LEN) {
        do_remote_put_V(ci, src_v, node_v, tgt_v, size_v, true);
        ci = 0;
      }

      if (ni == chpl_nodeID) {
        memcpy((char*) &mem_regions_map_addr_map[ni][chpl_nodeID] + off,
               (char*) &mem_regions + off,
               size);
      } else {
        src_v[ci] = (char*) &mem_regions + off;
        node_v[ci] = ni;
        tgt_v[ci] = (char*) &mem_regions_map_addr_map[ni][chpl_nodeID] + off;
        size_v[ci] = size;
        ci++;
      }
    }

    if (ci > 0) {
      do_remote_put_V(ci, src_v, node_v, tgt_v, size_v, true);
    }
  }

  mem_regions_unlock();
}


chpl_bool chpl_comm_impl_regMemFree(void* p, size_t size)
{
  mem_region_t* mr;
  int mr_i;

  if (!using_hugepages || size < chpl_comm_impl_regMemAllocThreshold())
    return false;

  //
  // Is this memory in our table?
  //
  mr = mreg_for_addr(p, &mem_regions);
  if (mr == NULL
      || mr->addr != (uint64_t) p
      || mr->len != size)
    return false;

  mr_i = (int) (mr - &mem_regions.mregs[0]);

  PERFSTATS_INC(deregMem_cnt);

  DBG_P_LP(DBGF_MEMREG,
           "chpl_comm_regMemFree(%p, %" PRIx64 "): [%d]",
           p, size, mr_i);

  //
  // Deregister the memory and empty the entry in our table. Note that even
  // with single-threading we can't compress the table, because other threads
  // may be doing lookups in it and they aren't locked out.
  //
  mem_regions_lock();

  deregister_mem_region(mr);

  mr->addr = 0;
  mr->len = 0;

  //
  // Adjust the memory region count downward, if necessary.
  //
  if (mr_i == mem_regions.mreg_cnt - 1) {
    int j;
    for (j = mr_i - 1; j >= 0 && mem_regions.mregs[j].addr == 0; j--)
      ;
    assert(j >= 0);
    mem_regions.mreg_cnt = j + 1;
  }

  //
  // Update the copies of our memory regions on all nodes.  If the new
  // entry doesn't shorten our table then we just send the entry.  If
  // it does, we just send the new count.
  //
  {
    size_t off;
    size_t size;
    void* src_v[MAX_CHAINED_PUT_LEN];
    int32_t node_v[MAX_CHAINED_PUT_LEN];
    void* tgt_v[MAX_CHAINED_PUT_LEN];
    size_t size_v[MAX_CHAINED_PUT_LEN];
    int ci;

    if (mem_regions.mreg_cnt == mem_regions_map[chpl_nodeID].mreg_cnt) {
      off = (char*) mr - (char*) &mem_regions;
      size = sizeof(*mr);
    } else {
      assert(mem_regions.mreg_cnt
             < mem_regions_map[chpl_nodeID].mreg_cnt);
      off = 0;
      size = sizeof(mem_regions.mreg_cnt);
    }

    ci = 0;
    for (int ni = 0; ni < (int) chpl_numNodes; ni++) {
      if (ci == MAX_CHAINED_PUT_LEN) {
        do_remote_put_V(ci, src_v, node_v, tgt_v, size_v, true);
        ci = 0;
      }

      if (ni == chpl_nodeID) {
        memcpy((char*) &mem_regions_map_addr_map[ni][chpl_nodeID] + off,
               (char*) &mem_regions + off,
               size);
      } else {
        src_v[ci] = (char*) &mem_regions + off;
        node_v[ci] = ni;
        tgt_v[ci] = (char*) &mem_regions_map_addr_map[ni][chpl_nodeID] + off;
        size_v[ci] = size;
        ci++;
      }
    }

    if (ci > 0) {
      do_remote_put_V(ci, src_v, node_v, tgt_v, size_v, true);
    }
  }

  mem_regions_unlock();

  free_huge_pages(p);

  return true;
}


void chpl_comm_broadcast_global_vars(int numGlobals)
{
  int i;

  DBG_P_LP(DBGF_IFACE, "IFACE chpl_comm_broadcast_global_vars(%d)",
           numGlobals);

  if (chpl_nodeID != 0) {
    for (i = 0; i < numGlobals; i++)
      do_remote_get(chpl_globals_registry[i], 0, chpl_globals_registry[i],
                    sizeof(wide_ptr_t), may_proxy_true);
  }
}


void chpl_comm_broadcast_private(int id, size_t size, int32_t tid)
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
      do_remote_put(chpl_private_broadcast_table[id], i,
                    chpl_private_broadcast_table[id], size,
                    may_proxy_true);
    }
  }
}


void chpl_comm_barrier(const char *msg)
{
  DBG_P_L(DBGF_IFACE, "IFACE chpl_comm_barrier(\"%s\")", msg);

  if (chpl_numNodes == 1)
    return;

  if (chpl_verbose_comm && !comm_diags_disabled_temporarily)
    printf("%d: barrier for '%s'\n", chpl_nodeID, msg);

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
                  sizeof(bar_flag), may_proxy_true);

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
  DBG_P_LP(DBGF_BARRIER, "BAR release %d children", (int) bar_num_children);
  for (uint32_t i = 0; i < bar_num_children; i++) {
    static uint32_t release_flag = 1;
    do_remote_put(&release_flag, bar_min_child + i,
                  (void*) &child_bar_info[i]->parent_release,
                  sizeof(release_flag), may_proxy_true);
  }
}


void chpl_comm_pre_task_exit(int all)
{

  if (chpl_numNodes == 1)
    return;

  if (all) {
    chpl_comm_barrier("chpl_comm_pre_task_exit");

    polling_task_please_exit = true;

    if (chpl_nodeID == 0) {
      while (!polling_task_done)
        sched_yield();
    }
  }

  DBG_P_L(DBGF_MEMREG,
          "registered memory regions high water mark: %d",
          (int) mreg_cnt_max);

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
#ifdef DEBUG
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
void rf_handler(gni_cq_entry_t* ev, void* context)
{
  //
  // This is invoked for each event on the rf_cqh completion queue.
  // It has to be fast, because its speed sets the limit on how fast
  // we can handle inbound fork requests.  For that reason, we do
  // not allow it to communicate remotely.  Any communication must
  // be done in an invoked task, not in the handler itself.
  //

  //
  // TODO: fr_copy and g_copy could be TLS pointers to space we realloc
  //       only when necessary to grow it for a larger request than we've
  //       ever seen before.
  //
  uint32_t inst_id = GNI_CQ_GET_INST_ID(*ev);
  uint32_t req_li  = INST_ID_LI(inst_id);
  int      req_cdi = INST_ID_CDI(inst_id);
  int      req_rbi = INST_ID_RBI(inst_id);
  fork_t*  f       = RECV_SIDE_FORK_REQ_BUF_ADDR(req_li, req_cdi, req_rbi);

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
        on_bundle->comm.done = f_c->b.rf_done;

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
      do_remote_put(f_x.src, req_li, f_x.tgt, f_x.size, may_proxy_false);
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

  indicate_done2(f->comm.caller, (rf_done_t*) f->comm.done);
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
  // before chpl_comm_atomic_put_int32().
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
      if (mreg_for_local_addr(obj) == NULL) {
        my_res = atomic_compare_exchange_strong_int_least64_t
                   ((atomic_int_least64_t*) obj,
                    *(int_least64_t*) opnd1,
                    *(int_least64_t*) opnd2);
      }
      else {
        int_least64_t nic_res;
        do_nic_amo(opnd1, opnd2, chpl_nodeID, obj,
                   sizeof(nic_res), amo_cmd_2_nic_op(cmpxchg_64, 1), &nic_res);
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
    //
    // Emulate 32-bit real add using compare-exchange.
    //
    {
      int_least32_t expected;
      int_least32_t desired;
      chpl_bool32 done;

      do {
        expected = atomic_load_int_least32_t((atomic_int_least32_t*) obj);
        *(float*) &desired  = *(float*) &expected + *(float*) opnd1;
        done = atomic_compare_exchange_strong_int_least32_t
                 ((atomic_int_least32_t*) obj, expected, desired);
      } while (!done);

      if (res != NULL) {
        memcpy(res, &expected, sizeof(expected));
        res_size = sizeof(expected);
      }
    }
    break;

  case add_r64:
    //
    // Emulate 64-bit real add using compare-exchange.  If the object
    // is not in memory registered with the NIC, use the processor.
    // Otherwise, since the other 64-bit AMOs are done on the NIC, for
    // coherence do this one on the NIC as well.
    //
    {
      int_least64_t expected;
      int_least64_t desired;

      if (mreg_for_local_addr(obj) == NULL) {
        chpl_bool32 done;

        do {
          expected =
            atomic_load_int_least64_t((atomic_int_least64_t*) obj);
          *(double*) &desired = *(double*) &expected + *(double*) opnd1;
          done = atomic_compare_exchange_strong_int_least64_t
                   ((atomic_int_least64_t*) obj, expected, desired);
        } while (!done);
      }
      else {
        int_least64_t nic_res;

        do {
          do_remote_get(&expected, chpl_nodeID, obj, sizeof(expected),
                        may_proxy_false);
          *(double*) &desired = *(double*) &expected + *(double*) opnd1;
          do_nic_amo(&expected, &desired, chpl_nodeID, obj,
                     sizeof(nic_res), amo_cmd_2_nic_op(cmpxchg_64, 1),
                     &nic_res);
        } while (nic_res != expected);
      }

      if (res != NULL) {
        memcpy(res, &expected, sizeof(expected));
        res_size = sizeof(expected);
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
    do_remote_put(&res, f_a->b.caller, f_a->res, res_size, may_proxy_false);
  }

  indicate_done(&f_a->b);
}


static
void release_req_buf(uint32_t li, int cdi, int rbi)
{
  static chpl_bool32 free_flag = true;
  do_remote_put(&free_flag, li, RECV_SIDE_FORK_REQ_FREE_ADDR(li, cdi, rbi),
                sizeof(free_flag), may_proxy_false);
}


static
inline
void indicate_done2(int caller, rf_done_t *ack)
{
  static rf_done_t done = 1;
  do_remote_put(&done, caller, ack, sizeof(done), may_proxy_false);
}


static
inline
void indicate_done(fork_base_info_t* b)
{
  indicate_done2(b->caller, b->rf_done);
}


//
// Non-blocking PUT/GET descriptor management
//

static
inline
nb_desc_idx_t nb_desc_idx_encode(int i, int j)
{
  return (nb_desc_idx_t) (i << 8 | j);
}


static
inline
void nb_desc_idx_decode(int* ip, int* jp, nb_desc_idx_t nbdi)
{
  *ip = nbdi >> 8;
  *jp = nbdi & 0xff;
}


static
inline
nb_desc_t* nb_desc_idx_2_ptr(nb_desc_idx_t nbdi)
{
  int i, j;
  nb_desc_idx_decode(&i, &j, nbdi);
  return &nb_desc_pool[i][j];
}


static
inline
chpl_comm_nb_handle_t nb_desc_idx_2_handle(nb_desc_idx_t nbdi)
{
  //
  // We offset j by 1 in the external handle so that when i==0, j==0
  // it doesn't end up producing a NULL handle pointer.
  //
  return (chpl_comm_nb_handle_t) ((intptr_t) nbdi + 1);
}


static
inline
nb_desc_idx_t nb_desc_handle_2_idx(chpl_comm_nb_handle_t h)
{
  //
  // We offset j by 1 in the external handle so that when i==0, j==0
  // it doesn't end up producing a NULL handle pointer.
  //
  return (nb_desc_idx_t) ((intptr_t) h - 1);
}


static
void nb_desc_init(void)
{
  int i, j;

  if (NB_DESC_NUM_POOLS < comm_dom_cnt)
    chpl_warning("(NB_DESC_NUM_POOLS < comm_dom_cnt) may lead to hangs", 0, 0);

  for (i = 0; i < NB_DESC_NUM_POOLS; i++) {
    for (j = 0; j < NB_DESC_NUM_PER_POOL - 1; j++) {
      atomic_init_bool(&nb_desc_pool[i][j].done, false);
      nb_desc_pool[i][j].next = j + 1;
    }
    nb_desc_pool[i][j].next = -1;

    mpool_idx_init(&nb_desc_pool_head[i], 0);

    atomic_init_bool(&nb_desc_pool_lock[i], false);
  }

  mpool_idx_init(&nb_desc_pool_i, 0);
}


static
inline
nb_desc_idx_t nb_desc_alloc(void)
{
  int i, j;
  nb_desc_idx_t nbdi = 0;

  //
  // Cycle through the pools and find one that contains a free nb_desc
  // flag we can use.  Just keep trying until we find one.
  //
  for (i = nb_desc_next_pool_i(), j = -1;
       j < 0;
       i = (i + 1) % NB_DESC_NUM_POOLS) {
    if (mpool_idx_load(&nb_desc_pool_head[i]) >= 0 &&
        !atomic_exchange_bool(&nb_desc_pool_lock[i], true)) {
      if ((j = mpool_idx_load(&nb_desc_pool_head[i])) >= 0) {
        nbdi = nb_desc_idx_encode(i, j);
        mpool_idx_store(&nb_desc_pool_head[i], nb_desc_pool[i][j].next);
      }

      atomic_store_bool(&nb_desc_pool_lock[i], false);
    }
  }

  return nbdi;
}


static
inline
void nb_desc_free(nb_desc_idx_t nbdi)
{
  int i, j;

  //
  // Add this flag back to its pool.
  //
  nb_desc_idx_decode(&i, &j, nbdi);
  while (atomic_exchange_bool(&nb_desc_pool_lock[i], true))
    ;
  nb_desc_pool[i][j].next = mpool_idx_exchange(&nb_desc_pool_head[i], j);
  atomic_store_bool(&nb_desc_pool_lock[i], false);
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

  if (RF_DONE_NUM_PER_POOL < comm_dom_cnt_max * FORK_REQ_BUFS_PER_CD)
    chpl_warning("(RF_DONE_NUM_PER_POOL "
                 "< comm_dom_cnt_max * FORK_REQ_BUFS_PER_CD) "
                 "may lead to hangs",
                 0, 0);

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
      if ((gni_rc = GNI_GetCompleted(cd->cqh, ev, &post_desc))
          != GNI_RC_SUCCESS)
        GNI_FAIL(gni_rc, "GNI_GetCompleted() failed");
      atomic_store_bool((atomic_bool*) (intptr_t) post_desc->post_id, true);
    }

    assert(gni_rc == GNI_RC_NOT_DONE);

    release_comm_dom();
  }
}


void chpl_comm_put(void* addr, c_nodeid_t locale, void* raddr,
                   size_t size, int32_t typeIndex,
                   int32_t commID, int ln, int32_t fn)
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
         .iu.comm={addr, raddr, size, typeIndex, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  if (chpl_verbose_comm && !comm_diags_disabled_temporarily)
    printf("%d: %s:%d: remote put to %d\n", chpl_nodeID,
           chpl_lookupFilename(fn), ln, locale);
  if (chpl_comm_diagnostics && !comm_diags_disabled_temporarily)
    (void) atomic_fetch_add_uint_least64_t(&comm_diagnostics.put, 1);

  do_remote_put(addr, locale, raddr, size, may_proxy_true);
}


static
void do_remote_put(void* src_addr, c_nodeid_t locale, void* tgt_addr,
                   size_t size, drpg_may_proxy_t may_proxy)
{
  mem_region_t*         remote_mr;
  gni_post_descriptor_t post_desc;

  DBG_P_LP(DBGF_GETPUT, "DoRemPut %p -> %d:%p (%#zx), proxy %c",
           src_addr, (int) locale, tgt_addr, size,
           may_proxy ? 'y' : 'n');

  //
  // The source address for a PUT doesn't have to be registered with
  // the local NIC, but the target address does have to be registered
  // with the remote NIC.  If it isn't, then instead of our doing a
  // PUT we arrange for the remote side do a GET from us, dealing with
  // the unregistered target address itself.  If we do this then we
  // also need the source address here to be registered, because GETs
  // must have registered source addresses.
  //
  remote_mr = mreg_for_remote_addr(tgt_addr, locale);
  if (remote_mr == NULL) {
    mem_region_t* local_mr;

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
  // If the transfer is larger than the maximum transaction length,
  // then we have to break it into smaller pieces.
  //
  while (size > 0) {
    size_t tsz;

    tsz = (size <= MAX_UGNI_TRANS_SZ) ? size : MAX_UGNI_TRANS_SZ;

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

    post_fma_and_wait(locale, &post_desc, true);
  }
}


static
void do_remote_put_V(int v_len,
                     void** src_addr_v, c_nodeid_t* locale_v, void** tgt_addr_v,
                     size_t* size_v, drpg_may_proxy_t may_proxy)
{
  mem_region_t* remote_mr;
  gni_post_descriptor_t pd;
  gni_ct_put_post_descriptor_t pdc[MAX_CHAINED_PUT_LEN - 1];

  DBG_P_LP(DBGF_GETPUT, "DoRemPut(%d) %p -> %d:%p (%#zx), proxy %c",
           v_len, src_addr_v[0], (int) locale_v[0], tgt_addr_v[0], size_v[0],
           may_proxy ? 'y' : 'n');

  //
  // If there are more than we can handle at once, block them up.
  //
  while (v_len > MAX_CHAINED_PUT_LEN) {
    do_remote_put_V(MAX_CHAINED_PUT_LEN, src_addr_v, locale_v, tgt_addr_v,
                    size_v, may_proxy);
    v_len -= MAX_CHAINED_PUT_LEN;
    src_addr_v += MAX_CHAINED_PUT_LEN;
    locale_v += MAX_CHAINED_PUT_LEN;
    tgt_addr_v += MAX_CHAINED_PUT_LEN;
    size_v += MAX_CHAINED_PUT_LEN;
  }

  if (v_len <= 0)
    return;

  //
  // Do all these PUTs in one chained transaction.  Except: defer to
  // the scalar PUT routine for any that refer to unregistered memory
  // on the remote side.
  //
  for (int vi = 0, ci = -1; vi < v_len; vi++) {
    remote_mr = mreg_for_remote_addr(tgt_addr_v[vi], locale_v[vi]);
    if (remote_mr == NULL) {
      do_remote_put(src_addr_v[vi], locale_v[vi], tgt_addr_v[vi], size_v[vi],
                    may_proxy);
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

  post_fma_ct_and_wait(locale_v, &pd);
}


void chpl_comm_get(void* addr, c_nodeid_t locale, void* raddr,
                   size_t size, int32_t typeIndex,
                   int32_t commID, int ln, int32_t fn)
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
         .iu.comm={addr, raddr, size, typeIndex, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  if (chpl_verbose_comm && !comm_diags_disabled_temporarily)
    printf("%d: %s:%d: remote get from %d\n", chpl_nodeID,
           chpl_lookupFilename(fn), ln, locale);
  if (chpl_comm_diagnostics && !comm_diags_disabled_temporarily)
    (void) atomic_fetch_add_uint_least64_t(&comm_diagnostics.get, 1);

  do_remote_get(addr, locale, raddr, size, may_proxy_true);
}


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
      memcpy(tgt_addr, tgt_addr_xmit, xmit_size);
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
  // If the transfer is larger than the maximum transaction length,
  // then we have to break it into smaller pieces.
  //
  while (size > 0) {
    size_t tsz;

    tsz = (size <= MAX_UGNI_TRANS_SZ) ? size : MAX_UGNI_TRANS_SZ;

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

    post_fma_and_wait(locale, &post_desc, true);
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

static inline
void strd_nb_helper(chpl_comm_nb_handle_t (*xferFn)(void*, int32_t, void*,
                                                    size_t,
                                                    int32_t, int32_t,
                                                    int, int32_t),
                    void* localAddr, int32_t remoteLocale, void* remoteAddr,
                    size_t cnt,
                    chpl_comm_nb_handle_t* handles, size_t* pCurrHandles,
                    int32_t typeIndex, int32_t commID, int ln, int32_t fn)
{
  size_t currHandles = *pCurrHandles;

  if (currHandles >= strd_maxHandles) {
    // reached max in flight -- retire some to make room
    while (!chpl_comm_try_nb_some(handles, currHandles)) {
      local_yield();
    }

    // compress retired transactions out of the list
    {
      size_t iOut, iIn;

      for (iOut = iIn = 0; iIn < currHandles; ) {
        if (handles[iIn] == NULL)
          iIn++;
        else
          handles[iOut++] = handles[iIn++];
      }

      currHandles = iOut;
    }
  }

  handles[currHandles] = (*xferFn)(localAddr, remoteLocale, remoteAddr, cnt,
                                   typeIndex, commID, ln, fn);
  if (handles[currHandles] != NULL)
    currHandles++;

  *pCurrHandles = currHandles;
}


void  chpl_comm_put_strd(void* dstaddr_arg, size_t* dststrides,
                         int32_t dstlocale,
                         void* srcaddr_arg, size_t* srcstrides,
                         size_t* count, int32_t stridelevels, size_t elemSize,
                         int32_t typeIndex, int32_t commID, int ln, int32_t fn)
{
  const size_t strlvls=(size_t)stridelevels;
  size_t i,j,k,t,total,off,x,carry;

  int8_t* dstaddr,*dstaddr1;
  int8_t* srcaddr,*srcaddr1;

  int *srcdisp, *dstdisp;

  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];

  chpl_comm_nb_handle_t handles[strd_maxHandles];
  size_t currHandles = 0;

  PERFSTATS_INC(put_strd_cnt);

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_put_strd)) {
      chpl_comm_cb_info_t cb_data =
        {chpl_comm_cb_event_kind_put_strd, chpl_nodeID, dstlocale,
         .iu.comm_strd={srcaddr_arg, srcstrides, dstaddr_arg, dststrides, count,
                        stridelevels, elemSize, typeIndex, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  //Only count[0] and strides are measured in number of bytes.
  cnt[0]= count[0] * elemSize;
  if (strlvls>0) {
    srcstr[0] = srcstrides[0] * elemSize;
    dststr[0] = dststrides[0] * elemSize;
    for (i=1;i<strlvls;i++)
      {
        srcstr[i] = srcstrides[i] * elemSize;
        dststr[i] = dststrides[i] * elemSize;
        cnt[i]=count[i];
      }
    cnt[strlvls]=count[strlvls];
  }

  switch (strlvls) {
  case 0:
    PERFSTATS_ADD(put_strd_byte_cnt, cnt[0]);
    chpl_comm_put(srcaddr_arg, dstlocale, dstaddr_arg, cnt[0],
                  typeIndex, commID, ln, fn);
    break;

  case 1:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;
    for(i=0; i<cnt[1]; i++) {
      PERFSTATS_ADD(put_strd_byte_cnt, cnt[0]);
      strd_nb_helper(chpl_comm_put_nb,
                     srcaddr, dstlocale, dstaddr, cnt[0],
                     handles, &currHandles,
                     typeIndex, commID, ln, fn);
      srcaddr+=srcstr[0];
      dstaddr+=dststr[0];
    }
    break;

  case 2:
    for(i=0; i<cnt[2]; i++) {
      srcaddr = (int8_t*)srcaddr_arg + srcstr[1]*i;
      dstaddr = (int8_t*)dstaddr_arg + dststr[1]*i;
      for(j=0; j<cnt[1]; j++) {
        PERFSTATS_ADD(put_strd_byte_cnt, cnt[0]);
        strd_nb_helper(chpl_comm_put_nb,
                       srcaddr, dstlocale, dstaddr, cnt[0],
                       handles, &currHandles,
                       typeIndex, commID, ln, fn);
        srcaddr+=srcstr[0];
        dstaddr+=dststr[0];
      }
    }
    break;

  case 3:
    for(i=0; i<cnt[3]; i++) {
      srcaddr1 = (int8_t*)srcaddr_arg + srcstr[2]*i;
      dstaddr1 = (int8_t*)dstaddr_arg + dststr[2]*i;
      for(j=0; j<cnt[2]; j++) {
        srcaddr = srcaddr1 + srcstr[1]*j;
        dstaddr = dstaddr1 + dststr[1]*j;
        for(k=0; k<cnt[1]; k++) {
          PERFSTATS_ADD(put_strd_byte_cnt, cnt[0]);
          strd_nb_helper(chpl_comm_put_nb,
                         srcaddr, dstlocale, dstaddr, cnt[0],
                         handles, &currHandles,
                         typeIndex, commID, ln, fn);
          srcaddr+=srcstr[0];
          dstaddr+=dststr[0];
        }
      }
    }
    break;

  default:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;

    //Number of chpl_comm_put operations to do
    total=1;
    for (i=0; i<strlvls; i++)
      total=total*cnt[i+1];

    //displacement from the dstaddr and srcaddr start points
    // chpl_comm_mem_reg no: not communicated
    srcdisp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
    // chpl_comm_mem_reg no: not communicated
    dstdisp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);

    for (j=0; j<total; j++) {
      carry=1;
      for (t=1;t<=strlvls;t++) {
        if (cnt[t]*carry>=j+1) {  //IF 1
          x=j/carry;
          off =j-(carry*x);

          if (carry!=1) {  //IF 2
            srcdisp[j]=srcstr[t-1]*x+srcdisp[off];
            dstdisp[j]=dststr[t-1]*x+dstdisp[off];
          } else {  //ELSE 2
            srcdisp[j]=srcstr[t-1]*x;
            dstdisp[j]=dststr[t-1]*x;
          }
          PERFSTATS_ADD(put_strd_byte_cnt, cnt[0]);
          strd_nb_helper(chpl_comm_put_nb,
                         srcaddr+srcdisp[j], dstlocale, dstaddr+dstdisp[j],
                         cnt[0],
                         handles, &currHandles,
                         typeIndex, commID, ln, fn);
          break;

        } else { //ELSE 1
          carry=carry*cnt[t];
        }
      }
    } // for j
    chpl_mem_free(srcdisp,0,0);
    chpl_mem_free(dstdisp,0,0);
    break;
  }

  if (currHandles > 0) {
    (void) chpl_comm_wait_nb_some(handles, currHandles);
  }
}


void  chpl_comm_get_strd(void* dstaddr_arg, size_t* dststrides,
                         int32_t srclocale,
                         void* srcaddr_arg, size_t* srcstrides,
                         size_t* count, int32_t stridelevels, size_t elemSize,
                         int32_t typeIndex, int32_t commID, int ln, int32_t fn)
{
  const size_t strlvls=(size_t)stridelevels;
  size_t i,j,k,t,total,off,x,carry;

  int8_t* dstaddr,*dstaddr1;
  int8_t* srcaddr,*srcaddr1;

  int *srcdisp, *dstdisp;
  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];

  chpl_comm_nb_handle_t handles[strd_maxHandles];
  size_t currHandles = 0;

  PERFSTATS_INC(get_strd_cnt);

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_get_strd)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_get_strd, chpl_nodeID, srclocale,
       .iu.comm_strd={srcaddr_arg, srcstrides, dstaddr_arg, dststrides, count,
                      stridelevels, elemSize, typeIndex, commID, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }

  //Only count[0] and strides are measured in number of bytes.
  cnt[0]=count[0] * elemSize;
  if(strlvls>0){
    srcstr[0] = srcstrides[0] * elemSize;
    dststr[0] = dststrides[0] * elemSize;
    for (i=1;i<strlvls;i++)
      {
        srcstr[i] = srcstrides[i] * elemSize;
        dststr[i] = dststrides[i] * elemSize;
        cnt[i]=count[i];
      }
    cnt[strlvls]=count[strlvls];
  }

  switch(strlvls) {
  case 0:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;
    PERFSTATS_ADD(get_strd_byte_cnt, cnt[0]);
    chpl_comm_get(dstaddr, srclocale, srcaddr, cnt[0],
                  typeIndex, commID, ln, fn);
    break;

  case 1:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;
    for(i=0; i<cnt[1]; i++) {
      PERFSTATS_ADD(get_strd_byte_cnt, cnt[0]);
      strd_nb_helper(chpl_comm_get_nb,
                     dstaddr, srclocale, srcaddr, cnt[0],
                     handles, &currHandles,
                     typeIndex, commID, ln, fn);
      srcaddr+=srcstr[0];
      dstaddr+=dststr[0];
    }
    break;

  case 2:
    for(i=0; i<cnt[2]; i++) {
      srcaddr = (int8_t*)srcaddr_arg + srcstr[1]*i;
      dstaddr = (int8_t*)dstaddr_arg + dststr[1]*i;
      for(j=0; j<cnt[1]; j++) {
        PERFSTATS_ADD(get_strd_byte_cnt, cnt[0]);
        strd_nb_helper(chpl_comm_get_nb,
                       dstaddr, srclocale, srcaddr, cnt[0],
                       handles, &currHandles,
                       typeIndex, commID, ln, fn);
        srcaddr+=srcstr[0];
        dstaddr+=dststr[0];
      }
    }
    break;

  case 3:
    for(i=0; i<cnt[3]; i++) {
      srcaddr1 = (int8_t*)srcaddr_arg + srcstr[2]*i;
      dstaddr1 = (int8_t*)dstaddr_arg + dststr[2]*i;
      for(j=0; j<cnt[2]; j++) {
        srcaddr = srcaddr1 + srcstr[1]*j;
        dstaddr = dstaddr1 + dststr[1]*j;
        for(k=0; k<cnt[1]; k++) {
          PERFSTATS_ADD(get_strd_byte_cnt, cnt[0]);
          strd_nb_helper(chpl_comm_get_nb,
                         dstaddr, srclocale, srcaddr, cnt[0],
                         handles, &currHandles,
                         typeIndex, commID, ln, fn);
          srcaddr+=srcstr[0];
          dstaddr+=dststr[0];
        }
      }
    }
    break;

  default:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;

    //Number of chpl_comm_get operations to do
    total=1;
    for (i=0; i<strlvls; i++)
      total=total*cnt[i+1];

    //displacement from the dstaddr and srcaddr start points
    // chpl_comm_mem_reg no: not communicated
    srcdisp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
    // chpl_comm_mem_reg no: not communicated
    dstdisp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);

    for (j=0; j<total; j++) {
      carry=1;
      for (t=1;t<=strlvls;t++) {
        if (cnt[t]*carry>=j+1) {  //IF 1
          x=j/carry;
          off =j-(carry*x);

          if (carry!=1) {  //IF 2
            srcdisp[j]=srcstr[t-1]*x+srcdisp[off];
            dstdisp[j]=dststr[t-1]*x+dstdisp[off];
          } else {  //ELSE 2
            srcdisp[j]=srcstr[t-1]*x;
            dstdisp[j]=dststr[t-1]*x;
          }
          PERFSTATS_ADD(get_strd_byte_cnt, cnt[0]);
          strd_nb_helper(chpl_comm_get_nb,
                         dstaddr+dstdisp[j], srclocale, srcaddr+srcdisp[j],
                         cnt[0],
                         handles, &currHandles,
                         typeIndex, commID, ln, fn);
          break;

        } else {  //ELSE 1
          carry=carry*cnt[t];
        }
      }
    }
    chpl_mem_free(srcdisp,0,0);
    chpl_mem_free(dstdisp,0,0);
    break;
  }

  if (currHandles > 0) {
    (void) chpl_comm_wait_nb_some(handles, currHandles);
  }
}


//
// Non-blocking get interface
//
chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, c_nodeid_t locale,
                                       void* raddr, size_t size,
                                       int32_t typeIndex, int32_t commID,
                                       int ln, int32_t fn)
{
  mem_region_t*          local_mr;
  mem_region_t*          remote_mr;
  nb_desc_idx_t          nbdi;
  nb_desc_t*             nbdp;
  gni_post_descriptor_t* post_desc;

  DBG_P_LP(DBGF_IFACE|DBGF_GETPUT, "IFACE chpl_comm_get_nb(%p, %d, %p, %zd)",
           addr, (int) locale, raddr, size);

  assert(addr != NULL);
  assert(raddr != NULL);
  if (size == 0)
    return NULL;

  if (locale < 0 || locale >= chpl_numNodes)
    CHPL_INTERNAL_ERROR("chpl_comm_get_nb(): remote locale out of range");

  if (locale == chpl_nodeID) {
    memmove(addr, raddr, size);
    return NULL;
  }

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_get_nb)) {
    chpl_comm_cb_info_t cb_data = 
      {chpl_comm_cb_event_kind_get_nb, chpl_nodeID, locale,
       .iu.comm={addr, raddr, size, typeIndex, commID, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }

  if (chpl_verbose_comm && !comm_diags_disabled_temporarily)
    printf("%d: %s:%d: remote non-blocking get from %d\n",
           chpl_nodeID, chpl_lookupFilename(fn), ln, locale);
  if (chpl_comm_diagnostics && !comm_diags_disabled_temporarily)
    (void) atomic_fetch_add_uint_least64_t(&comm_diagnostics.get_nb, 1);

  //
  // For now, if the local address isn't in a memory region known to the
  // NIC, or if they give us an unaligned address or length, or if the
  // remote address isn't in NIC-registered memory, do a blocking GET.
  // Eventually it might be worthwhile to do a nonblocking solution if
  // we can.
  //
  if ((local_mr = mreg_for_local_addr(addr)) == NULL
      || !IS_ALIGNED_32((size_t) (intptr_t) addr)
      || !IS_ALIGNED_32((size_t) (intptr_t) raddr)
      || !IS_ALIGNED_32(size)
      || (remote_mr = mreg_for_remote_addr(raddr, locale)) == NULL) {
    PERFSTATS_INC(get_nb_b_cnt);
    do_remote_get(addr, locale, raddr, size, may_proxy_true);
    return NULL;
  }

  //
  // Get a nonblocking completion descriptor, and fill in the POST
  // descriptor within it.
  //
  // When we initiate a non-blocking GET the post_id of the reference
  // is the address of the "done" flag in the NB descriptor.  When the
  // reference completes we use that post_id to set the flag.  The
  // POST descriptor has to exist all the way from the time the POST
  // is done until the matching GNI_CqGetCompleted() call, so that we
  // can pull the post_id out of it in order to record the completion.
  // Therefore it, too, is stored in the NB descriptor.  The returned
  // handle is a pointer to the descriptor itself, so that we can poll
  // the right CQ when testing for the completion event.
  //
  nbdi = nb_desc_alloc();
  nbdp = nb_desc_idx_2_ptr(nbdi);
  post_desc = &nbdp->post_desc;
  atomic_store_bool(&nbdp->done, false);

  post_desc->post_id         = (uint64_t) (intptr_t) &nbdp->done;
  post_desc->type            = GNI_POST_FMA_GET;
  post_desc->cq_mode         = GNI_CQMODE_GLOBAL_EVENT;
  post_desc->dlvr_mode       = GNI_DLVMODE_PERFORMANCE;
  post_desc->rdma_mode       = 0;
  post_desc->src_cq_hndl     = 0;

  post_desc->local_addr      = (uint64_t) (intptr_t) addr;
  post_desc->local_mem_hndl  = local_mr->mdh;
  post_desc->remote_addr     = (uint64_t) (intptr_t) raddr;
  post_desc->remote_mem_hndl = remote_mr->mdh;
  post_desc->length          = size;

  //
  // Initiate the transaction.  Don't wait for it to complete.
  //
  PERFSTATS_INC(get_nb_cnt);
  PERFSTATS_ADD(get_byte_cnt, size);

  nbdp->cdi = post_fma(locale, post_desc);

  return nb_desc_idx_2_handle(nbdi);
}


chpl_comm_nb_handle_t chpl_comm_put_nb(void* addr, c_nodeid_t locale,
                                       void* raddr, size_t size,
                                       int32_t typeIndex, int32_t commID,
                                       int ln, int32_t fn)
{
  DBG_P_LP(DBGF_IFACE|DBGF_GETPUT, "IFACE chpl_comm_put_nb(%p, %d, %p, %zd)",
           addr, (int) locale, raddr, size);

  //
  // For now, just do a blocking PUT.  At some point it will be worth
  // it do a real nonblocking implementation, but right now we don't
  // have time.
  //
  chpl_comm_put(addr, locale, raddr, size, typeIndex, commID, ln, fn);
  return NULL;

#if 0
  // When non blocking PUT is implemented, this is the code to
  // enable for communication callbacks, don't enable while using
  // blocking puts as it will log two communications
  
  // Communication callbacks
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_put_nb)) {
    chpl_comm_cb_info_t cb_data = 
      {chpl_comm_cb_event_kind_put_nb, chpl_nodeID, locale,
       .iu.comm={addr, raddr, size, typeIndex, commID, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }
#endif

}


int chpl_comm_test_nb_complete(chpl_comm_nb_handle_t h)
{
  if (chpl_verbose_comm && !comm_diags_disabled_temporarily) {
    int i, j;
    nb_desc_idx_decode(&i, &j, nb_desc_handle_2_idx(h));
    printf("%d: test nb complete (%d, %d)\n", chpl_nodeID, i, j);
  }
  if (chpl_comm_diagnostics && !comm_diags_disabled_temporarily)
    (void) atomic_fetch_add_uint_least64_t(&comm_diagnostics.test_nb, 1);

  PERFSTATS_INC(test_nb_cnt);

  return h == NULL;
}


void chpl_comm_wait_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles)
{
  if (chpl_verbose_comm && !comm_diags_disabled_temporarily) {
    if (nhandles == 1) {
      int i, j;
      nb_desc_idx_decode(&i, &j, nb_desc_handle_2_idx(h[0]));
      printf("%d: wait nb (%d, %d)\n", chpl_nodeID, i, j);
    }
    else
      printf("%d: wait nb (%zd handles)\n", chpl_nodeID, nhandles);
  }
  if (chpl_comm_diagnostics && !comm_diags_disabled_temporarily)
    (void) atomic_fetch_add_uint_least64_t(&comm_diagnostics.wait_nb, 1);

  PERFSTATS_INC(wait_nb_cnt);

  for (int i = 0; i < (int) nhandles; i++) {
    nb_desc_idx_t nbdi;
    nb_desc_t* nbdp;

    if (h[i] == NULL)
      continue;

    nbdi = nb_desc_handle_2_idx(h[i]);
    nbdp = nb_desc_idx_2_ptr(nbdi);

    //
    // Wait for the transaction to complete.  Don't yield initially
    // as we do in some other cases, because we've presumably done
    // all the other work we could already.
    //
    consume_all_outstanding_cq_events(nbdp->cdi);
    while (!atomic_load_explicit_bool(&nbdp->done, memory_order_acquire)) {
      local_yield();
      consume_all_outstanding_cq_events(nbdp->cdi);
    }

    //
    // cq_cnt_curr tells whether we have capacity to initiate another
    // transaction on this CD.  We could decrement it earlier, when we
    // handle the CQ event, but then we'd have a race until we freed
    // the NB descriptor here.  So, we delay the decrement until now.
    //
    CQ_CNT_DEC(&comm_doms[nbdp->cdi]);
    nb_desc_free(nbdi);
  }
}


int chpl_comm_try_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles)
{
  int rv = 0;

  if (chpl_verbose_comm && !comm_diags_disabled_temporarily) {
    if (nhandles == 1) {
      int i, j;
      nb_desc_idx_decode(&i, &j, nb_desc_handle_2_idx(h[0]));
      printf("%d: try nb (%d, %d)\n", chpl_nodeID, i, j);
    }
    else
      printf("%d: try nb (%zd handles)\n", chpl_nodeID, nhandles);
  }
  if (chpl_comm_diagnostics && !comm_diags_disabled_temporarily)
    (void) atomic_fetch_add_uint_least64_t(&comm_diagnostics.try_nb, 1);

  PERFSTATS_INC(try_nb_cnt);

  for (int i = 0; i < (int) nhandles; i++) {
    nb_desc_idx_t nbdi;
    nb_desc_t* nbdp;

    if (h[i] == NULL)
      continue;

    nbdi = nb_desc_handle_2_idx(h[i]);
    nbdp = nb_desc_idx_2_ptr(nbdi);

    //
    // If the transaction is not already done, try to complete it.
    //
    consume_all_outstanding_cq_events(nbdp->cdi);
    if (atomic_load_explicit_bool(&nbdp->done, memory_order_acquire)) {
      h[i] = NULL;
      rv = 1;

      //
      // cq_cnt_curr tells whether we have capacity to initiate another
      // transaction on this CD.  We could decrement it earlier, when we
      // handle the CQ event, but then we'd have a race until we freed
      // the NB descriptor here.  So, we delay the decrement until now.
      //
      CQ_CNT_DEC(&comm_doms[nbdp->cdi]);
      nb_desc_free(nbdi);
    }
  }

  return rv;
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
// Atomic Put functions:
//   _f: interface function name suffix (type)
//   _c: network AMO command
//   _t: AMO type
//
#define DEFINE_CHPL_COMM_ATOMIC_PUT(_f, _c, _t)                         \
        DEFINE_DO_FORK_AMO(_f, _c, _t)                                  \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_put_##_f(void* val,                       \
                                       int32_t loc,                     \
                                       void* obj,                       \
                                       int ln, int32_t fn)              \
        {                                                               \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_put_"#_f"(%p, %d, %p)",      \
                   val, (int) loc, obj);                                \
                                                                        \
          if (chpl_numNodes == 1) {                                     \
            atomic_store_##_t((atomic_##_t*) obj, *(_t*) val);          \
            return;                                                     \
          }                                                             \
                                                                        \
          if (IS_32_BIT_AMO_ON_GEMINI(_t)                               \
              || mreg_for_remote_addr(obj, loc) == NULL) {              \
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
                       GNI_FMA_ATOMIC_FAX, &res);                       \
          }                                                             \
          else {                                                        \
            do_remote_put(val, loc, obj, sizeof(_t), may_proxy_false);  \
          }                                                             \
        }

DEFINE_CHPL_COMM_ATOMIC_PUT(int32, put_32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_PUT(int64, put_64, int_least64_t)
DEFINE_CHPL_COMM_ATOMIC_PUT(uint32, put_32, uint_least32_t)
DEFINE_CHPL_COMM_ATOMIC_PUT(uint64, put_64, uint_least64_t)
DEFINE_CHPL_COMM_ATOMIC_PUT(real32, put_32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_PUT(real64, put_64, int_least64_t)

#undef DEFINE_CHPL_COMM_ATOMIC_PUT


//
// Atomic Get functions:
//   _f: interface function name suffix (type)
//   _c: network AMO command
//   _t: AMO type
//
#define DEFINE_CHPL_COMM_ATOMIC_GET(_f, _c, _t)                         \
        DEFINE_DO_FORK_AMO(_f, _c, _t)                                  \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_get_##_f(void* res,                       \
                                       int32_t loc,                     \
                                       void* obj,                       \
                                       int ln, int32_t fn)              \
        {                                                               \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_get_"#_f"(%p, %d, %p)",      \
                   res, (int) loc, obj);                                \
                                                                        \
          if (chpl_numNodes == 1) {                                     \
            *(_t*) res = atomic_load_##_t((atomic_##_t*) obj);          \
            return;                                                     \
          }                                                             \
                                                                        \
          if (IS_32_BIT_AMO_ON_GEMINI(_t)                               \
              || mreg_for_remote_addr(obj, loc) == NULL) {              \
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, res, obj, NULL, NULL);           \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, res, NULL, NULL, loc);       \
          }                                                             \
          else {                                                        \
            do_remote_get(res, loc, obj, sizeof(_t), may_proxy_false);  \
          }                                                             \
        }

DEFINE_CHPL_COMM_ATOMIC_GET(int32, get_32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_GET(int64, get_64, int_least64_t)
DEFINE_CHPL_COMM_ATOMIC_GET(uint32, get_32, uint_least32_t)
DEFINE_CHPL_COMM_ATOMIC_GET(uint64, get_64, uint_least64_t)
DEFINE_CHPL_COMM_ATOMIC_GET(real32, get_32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_GET(real64, get_64, int_least64_t)

#undef DEFINE_CHPL_COMM_ATOMIC_GET


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
                                        int ln, int32_t fn)             \
        {                                                               \
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
          if (IS_32_BIT_AMO_ON_GEMINI(_t)                               \
              || mreg_for_remote_addr(obj, loc) == NULL) {              \
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
                       GNI_FMA_ATOMIC_FAX, res);                        \
          }                                                             \
          else {                                                        \
            do_nic_amo(xchgval, NULL, loc, obj, sizeof(_t),             \
                       amo_cmd_2_nic_op(_c, 1), res);                   \
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
                                           int ln, int32_t fn)          \
        {                                                               \
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
          if (IS_32_BIT_AMO_ON_GEMINI(_t)                               \
              || mreg_for_remote_addr(obj, loc) == NULL) {              \
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, res, obj, cmpval, xchgval);      \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, res, cmpval, xchgval, loc);  \
          }                                                             \
          else {                                                        \
            _t my_res = 0;                                              \
            do_nic_amo(cmpval, xchgval, loc, obj, sizeof(_t),           \
                       amo_cmd_2_nic_op(_c, 1), &my_res);               \
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
                                          int ln, int32_t fn)           \
        {                                                               \
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
          if (IS_32_BIT_AMO_ON_GEMINI(_t)                               \
              || mreg_for_remote_addr(obj, loc) == NULL) {              \
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, NULL, obj, opnd, NULL);          \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, NULL, opnd, NULL, loc);      \
          }                                                             \
          else {                                                        \
            do_nic_amo(opnd, NULL, loc, obj, sizeof(_t),                \
                       amo_cmd_2_nic_op(_c, 0), NULL);                  \
          }                                                             \
        }                                                               \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_fetch_##_o##_##_f(void* opnd,             \
                                                int32_t loc,            \
                                                void* obj,              \
                                                void* res,              \
                                                int ln, int32_t fn)     \
        {                                                               \
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
          if (IS_32_BIT_AMO_ON_GEMINI(_t)                               \
              || mreg_for_remote_addr(obj, loc) == NULL) {              \
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, res, obj, opnd, NULL);           \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, res, opnd, NULL, loc);       \
          }                                                             \
          else {                                                        \
            do_nic_amo(opnd, NULL, loc, obj, sizeof(_t),                \
                       amo_cmd_2_nic_op(_c, 1), res);                   \
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
                                       int ln, int32_t fn)              \
        {                                                               \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_add_"#_f                     \
                   "(%p, %d, %p)",                                      \
                   opnd, (int) loc, obj);                               \
                                                                        \
          if (chpl_numNodes == 1) {                                     \
            amo_add_##_f##_cpu_cmpxchg(NULL, obj, opnd);                \
            return;                                                     \
          }                                                             \
                                                                        \
          if (sizeof(_t) == sizeof(int_least32_t)                       \
              && nic_type == GNI_DEVICE_ARIES                           \
              && mreg_for_remote_addr(obj, loc) != NULL) {              \
            do_nic_amo(opnd, NULL, loc, obj, sizeof(_t),                \
                       amo_cmd_2_nic_op(_c, 0), NULL);                  \
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
                                             int ln, int32_t fn)        \
        {                                                               \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_fetch_add_"#_f               \
                   "(%p, %d, %p, %p)",                                  \
                   opnd, (int) loc, obj, res);                          \
                                                                        \
          if (chpl_numNodes == 1) {                                     \
            amo_add_##_f##_cpu_cmpxchg(res, obj, opnd);                 \
            return;                                                     \
          }                                                             \
                                                                        \
          if (sizeof(_t) == sizeof(int_least32_t)                       \
              && nic_type == GNI_DEVICE_ARIES                           \
              && mreg_for_remote_addr(obj, loc) != NULL) {              \
            do_nic_amo(opnd, NULL, loc, obj, sizeof(_t),                \
                       amo_cmd_2_nic_op(_c, 1), res);                   \
          }                                                             \
          else {                                                        \
            if (loc == chpl_nodeID)                                     \
              (void) do_amo_on_cpu(_c, res, obj, opnd, NULL);           \
            else                                                        \
              do_fork_amo_##_c##_##_f(obj, res, opnd, NULL, loc);       \
          }                                                             \
        }

DEFINE_CHPL_COMM_ATOMIC_REAL_OP(real32, add_r32, float)
DEFINE_CHPL_COMM_ATOMIC_REAL_OP(real64, add_r64, double)

#undef DEFINE_CHPL_COMM_ATOMIC_REAL_OP


//
// Atomic subtract:
//   _f: interface function name suffix (type)
//   _t: AMO type
//
#define DEFINE_CHPL_COMM_ATOMIC_SUB(_f, _t)                             \
        /*==============================*/                              \
        void chpl_comm_atomic_sub_##_f(void* opnd,                      \
                                       int32_t loc,                     \
                                       void* obj,                       \
                                       int ln, int32_t fn)              \
        {                                                               \
          _t nopnd = - *(_t*) opnd;                                     \
                                                                        \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_sub_"#_f                     \
                   "(%p, %d, %p)",                                      \
                   opnd, (int) loc, obj);                               \
                                                                        \
          chpl_comm_atomic_add_##_f(&nopnd, loc, obj, ln, fn);          \
        }                                                               \
                                                                        \
        /*==============================*/                              \
        void chpl_comm_atomic_fetch_sub_##_f(void* opnd,                \
                                             int32_t loc,               \
                                             void* obj,                 \
                                             void* res,                 \
                                             int ln, int32_t fn)        \
        {                                                               \
          _t nopnd = - *(_t*) opnd;                                     \
                                                                        \
          DBG_P_LP(DBGF_IFACE|DBGF_AMO,                                 \
                   "IFACE chpl_comm_atomic_fetch_sub_"#_f               \
                   "(%p, %d, %p, %p)",                                  \
                   opnd, (int) loc, obj, res);                          \
                                                                        \
          chpl_comm_atomic_fetch_add_##_f(&nopnd, loc, obj, res,        \
                                          ln, fn);                      \
        }

DEFINE_CHPL_COMM_ATOMIC_SUB(int32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_SUB(int64, int_least64_t)
DEFINE_CHPL_COMM_ATOMIC_SUB(uint32, int_least32_t)
DEFINE_CHPL_COMM_ATOMIC_SUB(uint64, int_least64_t)
DEFINE_CHPL_COMM_ATOMIC_SUB(real32, float)
DEFINE_CHPL_COMM_ATOMIC_SUB(real64, double)

#undef DEFINE_CHPL_COMM_ATOMIC_SUB


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


static
void do_nic_amo(void* opnd1, void* opnd2, c_nodeid_t locale,
                void* object, size_t size,
                gni_fma_cmd_type_t cmd, void* result)
{
  mem_region_t*         local_mr;
  mem_region_t*         remote_mr;
  void*                 p_result = result;
  fork_amo_data_t       tmp_result;
  gni_post_descriptor_t post_desc;

  if (size == 4) {
    if (!IS_ALIGNED_32(VP_TO_UI64(object)))
      CHPL_INTERNAL_ERROR("remote AMO object must be 4-byte aligned");
  }
  else if (size == 8) {
    if (!IS_ALIGNED_64(VP_TO_UI64(object)))
      CHPL_INTERNAL_ERROR("remote AMO object must be 8-byte aligned");
  }
  else
    CHPL_INTERNAL_ERROR("unexpected AMO size");

  //
  // Make sure that, if we need a result, it is in memory known to the
  // NIC.
  //
  if (result == NULL)
    local_mr = NULL;
  else {
    local_mr = mreg_for_local_addr(p_result);
    if (local_mr == NULL) {
      p_result = &tmp_result;
      local_mr = mreg_for_local_addr(p_result);
      if (local_mr == NULL) {
        p_result = amo_res_alloc();
        local_mr = gnr_mreg;
        if (local_mr == NULL)
          CHPL_INTERNAL_ERROR("do_nic_amo(): "
                              "result address is not NIC-registered");
      }
    }
  }

  remote_mr = mreg_for_remote_addr(object, locale);
  if (remote_mr == NULL)
    CHPL_INTERNAL_ERROR("do_nic_amo(): "
                        "remote address is not NIC-registered");

  //
  // Fill in the POST descriptor.
  //
  post_desc.type            = GNI_POST_AMO;
  post_desc.cq_mode         = GNI_CQMODE_GLOBAL_EVENT;
  post_desc.dlvr_mode       = GNI_DLVMODE_PERFORMANCE;
  post_desc.rdma_mode       = 0;
  post_desc.src_cq_hndl     = 0;

  post_desc.local_addr      = (uint64_t) (intptr_t) p_result;
  if (p_result != NULL)
    post_desc.local_mem_hndl = local_mr->mdh;
  post_desc.remote_addr     = (uint64_t) (intptr_t) object;
  post_desc.remote_mem_hndl = remote_mr->mdh;
  post_desc.length          = size;

  post_desc.amo_cmd         = cmd;

  if (size == 4) {
    post_desc.first_operand = *(uint32_t*) opnd1;
    if (opnd2 != NULL)
      post_desc.second_operand = *(uint32_t*) opnd2;
  }
  else {
    post_desc.first_operand = *(uint64_t*) opnd1;
    if (opnd2 != NULL)
      post_desc.second_operand = *(uint64_t*) opnd2;
  }

  //
  // Initiate the transaction and wait for it to complete.
  //
  PERFSTATS_INC(amo_cnt);

  post_fma_and_wait(locale, &post_desc, true);

  if (p_result != result) {
    memcpy(result, p_result, size);
    if (p_result != &tmp_result)
      amo_res_free(p_result);
  }
}


static
inline
void amo_add_real32_cpu_cmpxchg(void* result, void* object, void* operand)
{
  float val;
  int_least32_t expected;
  int_least32_t desired;
  chpl_bool cxr;

  assert(sizeof(val) == sizeof(expected));

  do {
    val = *(volatile float*) object;
    memcpy(&expected, &val, sizeof(val));
    val += *(float*) operand;
    memcpy(&desired, &val, sizeof(val));
    cxr = atomic_compare_exchange_strong_int_least32_t
            ((atomic_int_least32_t*) object, expected, desired);
  } while (!cxr);

  if (result != NULL)
    memcpy(result, &expected, sizeof(val));
}


static
inline
void amo_add_real64_cpu_cmpxchg(void* result, void* object, void* operand)
{
  double val;
  int_least64_t expected;
  int_least64_t desired;
  chpl_bool cxr;

  assert(sizeof(val) == sizeof(expected));

  do {
    val = *(volatile double*) object;
    memcpy(&expected, &val, sizeof(val));
    val += *(double*) operand;
    memcpy(&desired, &val, sizeof(val));
    cxr = atomic_compare_exchange_strong_int_least64_t
            ((atomic_int_least64_t*) object, expected, desired);
  } while (!cxr);

  if (result != NULL)
    memcpy(result, &expected, sizeof(val));
}


void chpl_comm_execute_on(c_nodeid_t locale, c_sublocid_t subloc,
                          chpl_fn_int_t fid,
                          chpl_comm_on_bundle_t* arg, size_t arg_size)
{
  DBG_P_LP(DBGF_IFACE|DBGF_RF,
           "IFACE chpl_comm_execute_on(%d:%d, ftable[%d](%p, %zd))",
           (int) locale, (int) subloc, (int) fid, arg, arg_size);

  if (locale == chpl_nodeID) {
    assert(0); // locale model code should prevent this...

    chpl_ftable_call(fid, arg);
    return;
  }

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn)) {
      chpl_comm_cb_info_t cb_data = 
        {chpl_comm_cb_event_kind_executeOn, chpl_nodeID, locale,
         .iu.executeOn={subloc, fid, arg, arg_size}};
      chpl_comm_do_callbacks (&cb_data);
  }

  if (chpl_verbose_comm && !comm_diags_disabled_temporarily)
    printf("%d: remote task created on %d\n", chpl_nodeID, locale);
  if (chpl_comm_diagnostics && !comm_diags_disabled_temporarily)
    (void) atomic_fetch_add_uint_least64_t(&comm_diagnostics.execute_on, 1);

  PERFSTATS_INC(fork_call_cnt);
  fork_call_common(locale, subloc, fid, arg, arg_size, false, true);
}


void chpl_comm_execute_on_nb(c_nodeid_t locale, c_sublocid_t subloc,
                             chpl_fn_int_t fid,
                             chpl_comm_on_bundle_t* arg, size_t arg_size)
{
  DBG_P_LP(DBGF_IFACE|DBGF_RF,
           "IFACE chpl_comm_execute_on_nb(%d:%d, ftable[%d](%p, %zd))",
           (int) locale, (int) subloc, (int) fid, arg, arg_size);

  if (locale == chpl_nodeID) {

    assert(0); // locale model code should prevent this...

    // No copy of the argument is necessary for a local fork
    // since the tasking layer will do whatever it takes
    // to copy the argument to chpl_task_startMovedTask.

    chpl_task_startMovedTask(fid, chpl_ftable[fid],
                             chpl_comm_on_bundle_task_bundle(arg), arg_size,
                             subloc,
                             chpl_nullTaskID);
    return;
  }

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_nb)) {
      chpl_comm_cb_info_t cb_data = 
        {chpl_comm_cb_event_kind_executeOn_nb, chpl_nodeID, locale,
         .iu.executeOn={subloc, fid, arg, arg_size}};
      chpl_comm_do_callbacks (&cb_data);
  }

  if (chpl_verbose_comm && !comm_diags_disabled_temporarily)
    printf("%d: remote non-blocking task created on %d\n", chpl_nodeID,
           locale);
  if (chpl_comm_diagnostics && !comm_diags_disabled_temporarily)
    (void) atomic_fetch_add_uint_least64_t(&comm_diagnostics.execute_on_nb, 1);

  PERFSTATS_INC(fork_call_nb_cnt);
  fork_call_common(locale, subloc, fid, arg, arg_size, false, false);
}


void chpl_comm_execute_on_fast(c_nodeid_t locale, c_sublocid_t subloc,
                               chpl_fn_int_t fid,
                               chpl_comm_on_bundle_t* arg, size_t arg_size)
{
  DBG_P_LP(DBGF_IFACE|DBGF_RF,
           "IFACE chpl_comm_execute_on_fast(%d:%d, ftable[%d](%p, %zd))",
           (int) locale, (int) subloc, (int) fid, arg, arg_size);

  if (locale == chpl_nodeID) {
    assert(0); // locale model code should prevent this...
    chpl_ftable_call(fid, arg);
    return;
  }

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_fast)) {
      chpl_comm_cb_info_t cb_data = 
        {chpl_comm_cb_event_kind_executeOn_fast, chpl_nodeID, locale,
         .iu.executeOn={subloc, fid, arg, arg_size}};
      chpl_comm_do_callbacks (&cb_data);
  }

  if (chpl_verbose_comm && !comm_diags_disabled_temporarily)
    printf("%d: remote (no-fork) task created on %d\n",
           chpl_nodeID, locale);
  if (chpl_comm_diagnostics && !comm_diags_disabled_temporarily)
    (void) atomic_fetch_add_uint_least64_t(&comm_diagnostics.execute_on_fast, 1);

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
  rf_done_t **blockhere = NULL;
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
    // target locale will have to get it from us.  We cannot return
    // before it does so, because once we return we cannot guarantee the
    // argument's continued existence.  If this is a blocking fork then
    // we won't return until the whole thing is done, so the remote
    // locale can get the argument directly from our incoming parameter
    // and all is well.  But if this is a non-blocking fork we will
    // return as soon as we initiate it.  In that case, we must make a
    // copy of the argument and give the remote locale a pointer to
    // retrieve that copy instead of the original.  The remote locale
    // will free the copy with a remote fork_op_free back to our locale,
    // as soon as it has retrieved the argument.

    // If it's nonblocking, we need to copy the argument bundle to the
    // heap since the caller can reuse that memory.
    //
    // If the arg passed to us isn't in the mapped region, we
    // should also copy it now, but only if the heap is
    // in registered memory (because with minimal registered, the heap
    // isn't registered either).
    // By copying it now, we save the target side the overhead of having
    // to do an executeOn back to us to do a PUT to retrieve the large
    // arg, since it not being in registered memory means they won't be able
    // to GET it directly.
    if (!blocking ||
        (registered_heap_start != NULL && mreg_for_local_addr(arg) == NULL)) {
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
  
  if (blocking) {
    // do_fork_post will set ack and wait on it
    blockhere = &f_sc->b.rf_done;
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
  do_fork_post(locale, blockhere, f_size, req, &cdi, &rbi);
  
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
  do_fork_post(locale, & req.b.rf_done, sizeof(req), &req.b, NULL, NULL);
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
  do_fork_post(locale, & req.b.rf_done, sizeof(req), &req.b, NULL, NULL);
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
  do_fork_post(locale, NULL, sizeof(req), &req.b, NULL, NULL);
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
  do_fork_post(locale, &p_rf_req->a.b.rf_done,
               sizeof(p_rf_req->a), &p_rf_req->b, NULL, NULL);
}


static
void do_fork_post(c_nodeid_t locale,
                  rf_done_t** rf_done_slot,
                  uint64_t f_size, fork_base_info_t* p_rf_req,
                  int* cdi_p, int* rbi_p)
{
  rf_done_t             rf_done;
  rf_done_t*            rf_done_addr;
  gni_post_descriptor_t post_desc;
  int                   rbi;
  gni_return_t          gni_rc;
  chpl_bool blocking =  (rf_done_slot != NULL);

  if (blocking) {
    //
    // If our completion flag in our activation record isn't in mapped
    // memory, the remote locale will not be able to PUT directly back
    // to it.  Instead they will have to do a remote fork back to us
    // to do a fork_op_get, whereupon we'll end up right back here and
    // do the same thing again.  Ultimately we'll either recurse this
    // way until we run out of something (memory, say) or hang because
    // our tasking layer cannot run any more tasks.  So instead of
    // doing that, allocate a completion flag from the heap, make sure
    // it's mapped, and give the target locale that instead of the one
    // on our stack.
    //
    // This can happen if task stacks aren't in the registered heap.
    //
    mem_region_t* rf_done_mr;

    rf_done_addr = &rf_done;
    rf_done_mr = mreg_for_local_addr(rf_done_addr);
    if (rf_done_mr == NULL) {
      rf_done_addr = rf_done_alloc();
      rf_done_mr = gnr_mreg;
      if (rf_done_mr == NULL)
        CHPL_INTERNAL_ERROR("do_fork_post(): "
                            "rf_done_addr is not NIC-registered");
    }

    *rf_done_addr = 0;
    atomic_thread_fence(memory_order_release);
    *rf_done_slot = rf_done_addr;
  }
  else
    rf_done_addr = NULL;

  //
  // Fill in the POST descriptor.
  //
  acquire_comm_dom_and_req_buf(locale, &rbi);

  post_desc.type            = GNI_POST_FMA_PUT;
  post_desc.cq_mode         = GNI_CQMODE_GLOBAL_EVENT
                              | GNI_CQMODE_REMOTE_EVENT;
  post_desc.dlvr_mode       = GNI_DLVMODE_PERFORMANCE;
  post_desc.rdma_mode       = 0;
  post_desc.src_cq_hndl     = 0;
  post_desc.local_addr      = (uint64_t) (intptr_t) p_rf_req;
  post_desc.remote_addr     = (uint64_t) (intptr_t)
                              SEND_SIDE_FORK_REQ_BUF_ADDR(locale, cd_idx, rbi);
  post_desc.remote_mem_hndl = rf_mdh_map[locale];
  post_desc.length          = f_size;

  //
  // Initiate the transaction and wait for it to complete.
  //
  if (FORK_REQ_BUFS_PER_CD > 1) {
    gni_rc = GNI_EpSetEventData(cd->remote_eps[locale],
                                GNI_INST_ID(locale, 0, 0),
                                GNI_INST_ID(chpl_nodeID, cd_idx, rbi));
    if (gni_rc != GNI_RC_SUCCESS)
      GNI_FAIL(gni_rc, "GNI_EpSetEvent() failed");
  }

  DBG_P_LPS(DBGF_RF, "post %s",
            locale, cd_idx, rbi, p_rf_req->seq,
            fork_op_name(p_rf_req->op));

  if (cdi_p != NULL)
    *cdi_p = cd_idx;
  if (rbi_p != NULL)
    *rbi_p = rbi;

  // note: Do __NOT__ yield while waiting for the ack on a NB fork. We want to
  // ensure any subsequent NB tasks are spawned before we yield the processor.
  // For a case like `coforall loc in Locales do on loc do body()` this ensures
  // we've forked all remote tasks before we give up this task to potentially
  // work on the body for this locale.
  post_fma_and_wait(locale, &post_desc, blocking);

  if (blocking) {
    PERFSTATS_INC(wait_rfork_cnt);
    while (! *(volatile rf_done_t*) rf_done_addr) {
      PERFSTATS_INC(lyield_in_wait_rfork_cnt);
      local_yield();
    }

    if (rf_done_addr != &rf_done)
      rf_done_free(rf_done_addr);
  }
}


static
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
    if (!CHECK_CD_BUSY(want_cd)) {
      if (CQ_CNT_LOAD(want_cd) < want_cd->cq_cnt_max) {
        if (ACQUIRE_CD_MAYBE(want_cd)) {
          if (CQ_CNT_LOAD(want_cd) < want_cd->cq_cnt_max)
            break;
          else {
            PERFSTATS_INC(acq_cd_cq_cnt);
            RELEASE_CD(want_cd);
          }
        }
        else
          PERFSTATS_INC(acq_cd_na_cnt);
      }
      else
        PERFSTATS_INC(acq_cd_cq_cnt);
    }
    else
      PERFSTATS_INC(acq_cd_na_cnt);

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

  if (want_cdi < comm_dom_cnt - 1)
    comm_dom_free_idx = want_cdi + 1;
  else
    comm_dom_free_idx = 0;

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

    if (!CHECK_CD_BUSY(want_cd)) {
      if (CQ_CNT_LOAD(want_cd) < want_cd->cq_cnt_max) {
        for (rbi = 0; rbi < FORK_REQ_BUFS_PER_CD; rbi++) {
          if (*SEND_SIDE_FORK_REQ_FREE_ADDR(remote_locale, want_cdi, rbi)) {
            if (ACQUIRE_CD_MAYBE(want_cd)) {
              //
              // We found a CD that was available, had room left in its
              // completion queue, and had a free fork request buffer.  We
              // have successfully acquired it.  If it still has room in
              // its CQ and that fork request buffer or a subsequent one
              // is still free then it's ours.  Otherwise, release it and
              // keep looking.
              //
              if (CQ_CNT_LOAD(want_cd) >= want_cd->cq_cnt_max) {
                PERFSTATS_INC(acq_cd_rb_cq_cnt);
                RELEASE_CD(want_cd);
              }
              else {
                do {
                  if (*SEND_SIDE_FORK_REQ_FREE_ADDR(remote_locale, want_cdi,
                                                    rbi))
                    goto found_CD;
                } while (++rbi < FORK_REQ_BUFS_PER_CD);
                PERFSTATS_INC(acq_cd_rb_frf_1_cnt);
                RELEASE_CD(want_cd);
              }
            }
            else
              PERFSTATS_INC(acq_cd_rb_na_cnt);
          }
          else
            PERFSTATS_INC(acq_cd_rb_frf_2_cnt);
        }
      }
      else
        PERFSTATS_INC(acq_cd_rb_cq_cnt);
    }
    else
      PERFSTATS_INC(acq_cd_rb_na_cnt);

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
  if (want_cdi < comm_dom_cnt - 1)
    comm_dom_free_idx = want_cdi + 1;
  else
    comm_dom_free_idx = 0;

  *SEND_SIDE_FORK_REQ_FREE_ADDR(remote_locale, want_cdi, rbi) = false;

  cd = want_cd;
  cd_idx = want_cdi;

  *p_rbi = rbi;

#ifdef DEBUG_STATS
  cd->acqs_with_rb++;
  cd->acqs_with_rb_looks += acq_looks;
#endif
}


static
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
  gni_return_t gni_rc;

  if (post_desc->type == GNI_POST_FMA_PUT)
    PERFSTATS_ADD(sent_bytes, post_desc->length);
  else
    PERFSTATS_ADD(rcvd_bytes, post_desc->length);

  if (cd == NULL)
    acquire_comm_dom();
  cdi = cd_idx;

  CQ_CNT_INC(cd);

  if ((gni_rc = GNI_PostFma(cd->remote_eps[locale], post_desc))
      != GNI_RC_SUCCESS)
    GNI_POST_FAIL(gni_rc, "PostFMA() failed");

  release_comm_dom();

  return cdi;
}


static
void post_fma_and_wait(c_nodeid_t locale, gni_post_descriptor_t* post_desc, chpl_bool do_yield)
{
  int cdi;
  atomic_bool post_done;

  atomic_init_bool(&post_done, false);
  post_desc->post_id = (uint64_t) (intptr_t) &post_done;

  cdi = post_fma(locale, post_desc);

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

  CQ_CNT_DEC(&comm_doms[cdi]);
}


static
inline
int post_fma_ct(c_nodeid_t* locale_v, gni_post_descriptor_t* post_desc)
{
  int cdi;
  gni_return_t gni_rc;

  if (cd == NULL)
    acquire_comm_dom();
  cdi = cd_idx;

  if (post_desc->type == GNI_POST_FMA_PUT)
    PERFSTATS_ADD(sent_bytes, post_desc->length);
  else
    PERFSTATS_ADD(rcvd_bytes, post_desc->length);

  {
    gni_ct_put_post_descriptor_t* pdc;
    int i;

    for (pdc = post_desc->next_descr, i = 1;
         pdc != NULL;
         pdc = pdc->next_descr, i++) {
      pdc->ep_hndl = cd->remote_eps[locale_v[i]];
      if (post_desc->type == GNI_POST_FMA_PUT)
        PERFSTATS_ADD(sent_bytes, pdc->length);
      else
        PERFSTATS_ADD(rcvd_bytes, pdc->length);
    }
  }

  CQ_CNT_INC(cd);

  if ((gni_rc = GNI_CtPostFma(cd->remote_eps[locale_v[0]], post_desc))
      != GNI_RC_SUCCESS)
    GNI_POST_FAIL(gni_rc, "CTPostFMA() failed");

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

  CQ_CNT_DEC(&comm_doms[cdi]);
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


void chpl_startVerboseComm()
{
  chpl_verbose_comm = 1;
  comm_diags_disabled_temporarily = true;
  chpl_comm_broadcast_private(0 /* &chpl_verbose_comm */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  comm_diags_disabled_temporarily = false;
}


void chpl_stopVerboseComm()
{
  chpl_verbose_comm = 0;
  comm_diags_disabled_temporarily = true;
  chpl_comm_broadcast_private(0 /* &chpl_verbose_comm */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  comm_diags_disabled_temporarily = false;
}


void chpl_startVerboseCommHere()
{
  chpl_verbose_comm = 1;
}


void chpl_stopVerboseCommHere()
{
  chpl_verbose_comm = 0;
}


void chpl_startCommDiagnostics()
{
  chpl_comm_diagnostics = 1;
  comm_diags_disabled_temporarily = true;
  chpl_comm_broadcast_private(1 /* &chpl_comm_diagnostics */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  comm_diags_disabled_temporarily = false;
}


void chpl_stopCommDiagnostics()
{
  chpl_comm_diagnostics = 0;
  comm_diags_disabled_temporarily = true;
  chpl_comm_broadcast_private(1 /* &chpl_comm_diagnostics */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  comm_diags_disabled_temporarily = false;
}


void chpl_startCommDiagnosticsHere()
{
  chpl_comm_diagnostics = 1;
}


void chpl_stopCommDiagnosticsHere()
{
  chpl_comm_diagnostics = 0;
}


void chpl_resetCommDiagnosticsHere()
{
  atomic_store_uint_least64_t(&comm_diagnostics.get, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.get_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.put, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.put_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.test_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.wait_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.try_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.execute_on, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.execute_on_fast, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.execute_on_nb, 0);

#define _PSV_STORE(psv) atomic_store_uint_least64_t(&_PSV_VAR(psv), 0);
  PERFSTATS_DO_ALL(_PSV_STORE);
#undef _PSV_STORE
}


void chpl_getCommDiagnosticsHere(chpl_commDiagnostics *cd)
{
  cd->get             = atomic_load_uint_least64_t(&comm_diagnostics.get);
  cd->get_nb          = atomic_load_uint_least64_t(&comm_diagnostics.get_nb);
  cd->put             = atomic_load_uint_least64_t(&comm_diagnostics.put);
  cd->put_nb          = atomic_load_uint_least64_t(&comm_diagnostics.put_nb);
  cd->test_nb         = atomic_load_uint_least64_t(&comm_diagnostics.test_nb);
  cd->wait_nb         = atomic_load_uint_least64_t(&comm_diagnostics.wait_nb);
  cd->try_nb          = atomic_load_uint_least64_t(&comm_diagnostics.try_nb);
  cd->execute_on      = atomic_load_uint_least64_t(&comm_diagnostics.execute_on);
  cd->execute_on_fast = atomic_load_uint_least64_t(&comm_diagnostics.execute_on_fast);
  cd->execute_on_nb   = atomic_load_uint_least64_t(&comm_diagnostics.execute_on_nb);
}

void chpl_comm_ugni_help_register_global_var(int i, wide_ptr_t wide)
{
}


void chpl_comm_statsStartHere(void)
{
#define _PSV_STORE(psv) atomic_store_uint_least64_t(&_PSV_VAR(psv), 0);
  PERFSTATS_DO_EPHEMERAL(_PSV_STORE);
#undef _PSV_STORE
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
        chpl_comm_get(&ps, li, &chpl_comm_pstats, sizeof(ps), -1, CHPL_COMM_UNKNOWN_ID, 0, -1);
#define _PSV_SUM(psv) sum.psv += ps.psv;
        PERFSTATS_DO_ALL(_PSV_SUM);
#undef _PSV_SUM
      }
    }
    _psv_print(-1, &sum);
  }
  else
    _psv_print(chpl_nodeID, &chpl_comm_pstats);
#endif
}


#ifdef PERFSTATS_COMM_UGNI
static void _psv_print(int li, chpl_comm_pstats_t* ps)
{
  char   buf[8 * 1024];
  size_t buf_cnt;

#define _PSV_PRINT(psv)                                                 \
        do {                                                            \
          size_t wc = snprintf(&buf[buf_cnt], sizeof(buf) - buf_cnt,    \
                               "%s = %" PRIu64 "; ", # psv, ps->psv);   \
          if (wc > sizeof(buf) - buf_cnt)                               \
            buf_cnt = sizeof(buf);                                      \
          else                                                          \
            buf_cnt += wc;                                              \
        } while (0);

  buf_cnt = 0;
  PERFSTATS_DO_ALL(_PSV_PRINT);
  if (li < 0)
    printf("comm perfstats over all locales: (%.*s)\n",
           (int) buf_cnt - 2, buf);
  else
    printf("comm perfstats on locale %d (%.*s)\n", li,
           (int) buf_cnt - 2, buf);

#undef _PSV_PRINT
}
#endif
