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
// Internal declarations for the OFI-based Chapel comm layer.
//

#ifndef _comm_ofi_internal_h_
#define _comm_ofi_internal_h_

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_errno.h>


//
// Debugging
//

#define DEBUG 1

#ifdef DEBUG

uint64_t chpl_comm_ofi_dbg_level;
#define DBG_STATS                 0x1UL
#define DBG_STATSNODES            0x2UL
#define DBG_STATSTHREADS          0x4UL
#define DBG_STATSPROGRESS         0x8UL
#define DBG_CONFIG               0x10UL
#define DBG_CONFIGMORE           0x20UL
#define DBG_THREADS             0x100UL
#define DBG_THREADDETAILS       0x200UL
#define DBG_AM                 0x1000UL
#define DBG_AMSEND             0x2000UL
#define DBG_AMRECV             0x4000UL
#define DBG_RMA               0x10000UL
#define DBG_RMAWRITE          0x20000UL
#define DBG_RMAREAD           0x40000UL
#define DBG_ACK              0x100000UL
#define DBG_COMMPROGRESS     0x200000UL
#define DBG_MR              0x1000000UL
#define DBG_FAB            0x10000000UL
#define DBG_FABSALL        0x20000000UL
#define DBG_FABFAIL        0x40000000UL
#define DBG_TSTAMP        0x100000000UL

void chpl_comm_ofi_dbg_init(void);
char* chpl_comm_ofi_dbg_prefix(void);

#define DBG_INIT() chpl_comm_ofi_dbg_init()

#define DBG_DO_PRINTF(fmt, ...) printf(fmt "\n", ## __VA_ARGS__)

#define DBG_TEST_MASK(mask) ((chpl_comm_ofi_dbg_level & (mask)) != 0)

#define DBG_PRINTF(mask, fmt, ...)                                      \
  do {                                                                  \
    if (DBG_TEST_MASK(mask)) {                                          \
      DBG_DO_PRINTF("%s" fmt, chpl_comm_ofi_dbg_prefix(),               \
                    ## __VA_ARGS__);                                    \
    }                                                                   \
  } while (0)

#else // DEBUG

#define DBG_INIT()
#define DBG_DO_PRINTF(fmt, ...)
#define DBG_TEST_MASK(mask) 0
#define DBG_PRINTF(mask, fmt, ...)

#endif // DEBUG


//
// Simplify internal error checking
//
#define CHK_TRUE(expr)                                                  \
    do {                                                                \
      if (!(expr)) {                                                    \
        chpl_internal_error_v("!(%s)", #expr);                          \
      }                                                                 \
    } while (0)

#define CHK_EQ_TYPED(expr, wantVal, type, fmtSpec)                      \
    do {                                                                \
      type _exprVal = (expr);                                           \
      type _wantVal = (wantVal);                                        \
      if (_exprVal != _wantVal) {                                       \
        chpl_internal_error_v("%s == %" fmtSpec ", expected %" fmtSpec, \
                              #expr, _exprVal, _wantVal);               \
      }                                                                 \
    } while (0)


//
// Memory allocation
//

// wish we had typeof() in all target compilers ...

#define CHK_SYS_CALLOC_SZ(p, n, s)                                      \
    do {                                                                \
      if ((p = sys_calloc((n), (s))) == NULL) {                         \
        chpl_internal_error_v("sys_calloc(%#zx, %#zx): out of memory",  \
                              (size_t) (n), (size_t) (s));              \
      }                                                                 \
    } while (0)

#define CHK_SYS_CALLOC(p, n) CHK_SYS_CALLOC_SZ(p, n, sizeof(*(p)))

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
void chpl_comm_ofi_oob_allgather(void*, void*, int);


//
// Other/utility
//
double chpl_comm_ofi_time_get(void);

#endif
