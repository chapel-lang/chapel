/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifdef __cplusplus
extern "C" {
#endif

//
// Debugging
//
#ifdef CHPL_COMM_DEBUG

#define OFI_ALL_DEBUGS(m)                                               \
  m(CFG,                    "config: fabric resources used")            \
  m(CFG_AV,                 "config: address vectors")                  \
  m(PROV,                   "provider: selection")                      \
  m(PROV_HINTS,             "provider: hints")                          \
  m(PROV_ALL,               "provider: all matching")                   \
  m(IFACE,                  "interface calls")                          \
  m(IFACE_SETUP,            "interface calls: setup")                   \
  m(IFACE_AMO,              "interface calls: AMOs")                    \
  m(IFACE_AMO_READ,         "interface calls: AMO reads")               \
  m(IFACE_AMO_WRITE,        "interface calls: AMO writes")              \
  m(IFACE_MCM,              "interface calls: MCM conformance")         \
  m(AM,                     "AMs")                                      \
  m(AM_SEND,                "AMs: sends")                               \
  m(AM_RECV,                "AMs: receives")                            \
  m(AM_BUF,                 "AMs: receive buffers")                     \
  m(RMA,                    "RMA")                                      \
  m(RMA_READ,               "RMA: reads (loads, GETs)")                 \
  m(RMA_WRITE,              "RMA: writes (stores, PUTs)")               \
  m(RMA_UNORD,              "RMA: unordered operations")                \
  m(AMO,                    "AMOs")                                     \
  m(AMO_READ,               "AMOs: reads")                              \
  m(AMO_UNORD,              "AMOs: unordered operations")               \
  m(ACK,                    "tx acknowledgements")                      \
  m(ORDER,                  "ops done only for ordering")               \
  m(MR,                     "mem reg: regions")                         \
  m(MR_DESC,                "mem reg: local region descs")              \
  m(MR_KEY,                 "mem reg: remote region keys")              \
  m(HUGEPAGES,              "hugepages")                                \
  m(TCIPS,                  "tx context alloc/free")                    \
  m(OOB,                    "out-of-band calls")                        \
  m(BARRIER,                "barriers")                                 \
  m(TSTAMP,                 "timestamp output")

//
// Define the enumeration constants for the debug code bit offsets.
//
#define OFIDBG_MACRO(_enum, _desc)  DBG_##_enum##_BOFF,
typedef enum {
  OFI_ALL_DEBUGS(OFIDBG_MACRO)
} ofiDbgBOff_t;
#undef OFIDBG_MACRO

//
// Define the debug codes themselves.
//
#define OFIDBG_MACRO(_enum, _desc)  \
  static const uint64_t DBG_##_enum = (uint64_t) 1 << DBG_##_enum##_BOFF;
OFI_ALL_DEBUGS(OFIDBG_MACRO)
#undef OFIDBG_MACRO

extern uint64_t chpl_comm_ofi_dbg_level;
extern FILE* chpl_comm_ofi_dbg_file;
void chpl_comm_ofi_dbg_init(void);
char* chpl_comm_ofi_dbg_prefix(void);
char* chpl_comm_ofi_dbg_val(const void*, enum fi_datatype);

#define DBG_INIT() chpl_comm_ofi_dbg_init()

#define DBG_DO_PRINTF(fmt, ...)                                         \
  do {                                                                  \
    fprintf(chpl_comm_ofi_dbg_file, "%s" fmt "\n",                      \
            chpl_comm_ofi_dbg_prefix(), ## __VA_ARGS__);                \
  } while (0)

#define DBG_TEST_MASK(mask) ((chpl_comm_ofi_dbg_level & (mask)) != 0)

#define DBG_PRINTF(mask, fmt, ...)                                      \
  do {                                                                  \
    if (DBG_TEST_MASK(mask)) {                                          \
      DBG_DO_PRINTF(fmt, ## __VA_ARGS__);                               \
    }                                                                   \
  } while (0)

#define DBG_PRINTF_NODE0(mask, fmt, ...)                                \
    do {                                                                \
      if (chpl_nodeID == 0) {                                           \
        DBG_PRINTF(mask, fmt, ## __VA_ARGS__);                          \
      }                                                                 \
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
#define DBG_PRINTF_NODE0(mask, fmt, ...) do { } while (0)

#endif // CHPL_COMM_DEBUG


//
// Simplify internal error checking
//
extern int chpl_comm_ofi_abort_on_error;

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

#define INTERNAL_ERROR_V_NODE0(fmt, ...)                                \
    do {                                                                \
      if (chpl_nodeID == 0) {                                           \
        INTERNAL_ERROR_V(fmt, ## __VA_ARGS__);                          \
      } else {                                                          \
        chpl_comm_ofi_oob_fini();                                       \
        chpl_exit_any(0);                                               \
      }                                                                 \
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

#define CHPL_FREE(p)                                                    \
  do {                                                                  \
    chpl_mem_free((p), 0, 0);                                           \
    p = NULL;                                                           \
  } while (0)


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

#ifdef __cplusplus
}
#endif

#endif
