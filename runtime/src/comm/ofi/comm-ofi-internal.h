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
// Internal declarations for the OFI-based Chapel comm layer.
//

#ifndef _comm_ofi_internal_h_
#define _comm_ofi_internal_h_

#include "chpl-mem.h"
#include "chpl-mem-sys.h"
#include "error.h"

#include <stdint.h>
#include <stdio.h>

#include <rdma/fabric.h>
#include <rdma/fi_domain.h>


//
// Debugging
//
#ifdef CHPL_COMM_DEBUG

uint64_t chpl_comm_ofi_dbg_level;
FILE* chpl_comm_ofi_dbg_file;

#define DBG_STATS                   0x1UL
#define DBG_STATSNODES              0x2UL
#define DBG_STATSTHREADS            0x4UL
#define DBG_STATSPROGRESS           0x8UL
#define DBG_CFG                    0x10UL
#define DBG_CFGFAB                 0x20UL
#define DBG_CFGFABSALL             0x40UL
#define DBG_CFGAV                  0x80UL
#define DBG_THREADS               0x100UL
#define DBG_THREADDETAILS         0x200UL
#define DBG_TCIPS                 0x800UL
#define DBG_INTERFACE            0x1000UL
#define DBG_AM                  0x10000UL
#define DBG_AMSEND              0x20000UL
#define DBG_AMRECV              0x40000UL
#define DBG_RMA                0x100000UL
#define DBG_RMAWRITE           0x200000UL
#define DBG_RMAREAD            0x400000UL
#define DBG_RMAUNORD           0x800000UL
#define DBG_AMO               0x1000000UL
#define DBG_ACK               0x2000000UL
#define DBG_MR               0x10000000UL
#define DBG_MRDESC           0x20000000UL
#define DBG_MRKEY            0x40000000UL
#define DBG_HUGEPAGES        0x80000000UL
#define DBG_BARRIER         0x100000000UL
#define DBG_OOB            0x1000000000UL
#define DBG_TSTAMP        0x10000000000UL

void chpl_comm_ofi_dbg_init(void);
char* chpl_comm_ofi_dbg_prefix(void);
char* chpl_comm_ofi_dbg_val(const void*, enum fi_datatype);

#define DBG_INIT() chpl_comm_ofi_dbg_init()

#define DBG_DO_PRINTF(fmt, ...)                                         \
  do {                                                                  \
    fprintf(chpl_comm_ofi_dbg_file, fmt "\n", ## __VA_ARGS__);          \
  } while (0)

#define DBG_TEST_MASK(mask) ((chpl_comm_ofi_dbg_level & (mask)) != 0)

#define DBG_PRINTF(mask, fmt, ...)                                      \
  do {                                                                  \
    if (DBG_TEST_MASK(mask)) {                                          \
      DBG_DO_PRINTF("%s" fmt, chpl_comm_ofi_dbg_prefix(),               \
                    ## __VA_ARGS__);                                    \
    }                                                                   \
  } while (0)

#define DBG_VAL(pV, typ) chpl_comm_ofi_dbg_val(pV, typ)

//#define DEBUG_CRC_MSGS
#ifdef DEBUG_CRC_MSGS
#include <libiberty.h>
#endif

#else // CHPL_COMM_DEBUG

#define DBG_INIT()
#define DBG_DO_PRINTF(fmt, ...) do { } while (0)
#define DBG_TEST_MASK(mask) 0
#define DBG_PRINTF(mask, fmt, ...) do { } while (0)

#endif // CHPL_COMM_DEBUG


//
// Simplify internal error checking
//
int chpl_comm_ofi_abort_on_error;

#define INTERNAL_ERROR_V(fmt, ...)                                      \
  do {                                                                  \
    if (chpl_comm_ofi_abort_on_error) {                                 \
      fprintf(stderr, "%d: %s:%d: " fmt, chpl_nodeID,                   \
              __FILE__, (int) __LINE__, ## __VA_ARGS__);                \
      abort();                                                          \
    } else {                                                            \
      chpl_internal_error_v("%d: %s:%d: " fmt, chpl_nodeID,             \
                            __FILE__, (int) __LINE__, ## __VA_ARGS__);  \
    }                                                                   \
  } while (0)

#define CHK_TRUE(expr)                                                  \
    do {                                                                \
      if (!(expr)) {                                                    \
        INTERNAL_ERROR_V("!(%s)", #expr);                               \
      }                                                                 \
    } while (0)

#define CHK_FALSE(expr)                                                 \
    do {                                                                \
      if (expr) {                                                       \
        INTERNAL_ERROR_V("%s", #expr);                                  \
      }                                                                 \
    } while (0)

#define CHK_EQ_TYPED(expr, wantVal, type, fmtSpec)                      \
    do {                                                                \
      type _exprVal = (expr);                                           \
      type _wantVal = (wantVal);                                        \
      if (_exprVal != _wantVal) {                                       \
        INTERNAL_ERROR_V("%s == %" fmtSpec ", expected %" fmtSpec,      \
                         #expr, _exprVal, _wantVal);                    \
      }                                                                 \
    } while (0)


//
// Memory allocation
//

// wish we had typeof() in all target compilers ...

#define CHK_SYS_MALLOC_SZ(p, n, s)                                      \
    do {                                                                \
      if ((p = sys_malloc((n) * (s))) == NULL) {                        \
        INTERNAL_ERROR_V("sys_malloc(%#zx): out of memory",             \
                         (size_t) (n) * (size_t) (s));                  \
      }                                                                 \
    } while (0)

#define CHK_SYS_MALLOC(p, n) CHK_SYS_MALLOC_SZ(p, n, sizeof(*(p)))

#define CHK_SYS_CALLOC_SZ(p, n, s)                                      \
    do {                                                                \
      if ((p = sys_calloc((n), (s))) == NULL) {                         \
        INTERNAL_ERROR_V("sys_calloc(%#zx, %#zx): out of memory",       \
                         (size_t) (n), (size_t) (s));                   \
      }                                                                 \
    } while (0)

#define CHK_SYS_CALLOC(p, n) CHK_SYS_CALLOC_SZ(p, n, sizeof(*(p)))

#define CHK_SYS_MEMALIGN(p, a, s)                                       \
    do {                                                                \
      if ((p = sys_memalign((a), (s))) == NULL) {                       \
        INTERNAL_ERROR_V("sys_memalign(%#zx, %#zx): out of memory",     \
                         (size_t) (a), (size_t) (s));                   \
      }                                                                 \
    } while (0)

#define CHPL_CALLOC_SZ(p, n, s)                                         \
  do {                                                                  \
    p = chpl_mem_calloc((n), (s), CHPL_RT_MD_COMM_UTIL, 0, 0);          \
  } while (0)

#define CHPL_CALLOC(p, n) CHPL_CALLOC_SZ(p, n, sizeof(*(p)))

#define CHPL_FREE(p) chpl_mem_free((p), 0, 0)


//
// Out-of-band support
//

struct gather_info {
  int node;
  char info[0];
};

void chpl_comm_ofi_oob_init(void);
void chpl_comm_ofi_oob_fini(void);
void chpl_comm_ofi_oob_barrier(void);
void chpl_comm_ofi_oob_allgather(const void*, void*, size_t);
void chpl_comm_ofi_oob_bcast(void*, size_t);


//
// Hugepage interface
//

void* chpl_comm_ofi_hp_get_huge_pages(size_t);
size_t chpl_comm_ofi_hp_gethugepagesize(void);


//
// Other/utility
//
double chpl_comm_ofi_time_get(void);

#endif
