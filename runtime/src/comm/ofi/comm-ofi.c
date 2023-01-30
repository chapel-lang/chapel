/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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
// OFI-based implementation of Chapel communication interface.
//

#include "chplrt.h"
#include "chpl-env-gen.h"

#include "chpl-atomics.h"
#include "chpl-comm.h"
#include "chpl-comm-callbacks.h"
#include "chpl-comm-callbacks-internal.h"
#include "chpl-comm-diags.h"
#include "chpl-comm-internal.h"
#include "chpl-comm-strd-xfer.h"
#include "chpl-env.h"
#include "chplexit.h"
#include "chpl-format.h"
#include "chpl-gen-includes.h"
#include "chpl-linefile-support.h"
#include "chpl-mem.h"
#include "chpl-mem-sys.h"
#include "chplsys.h"
#include "chpl-tasks.h"
#include "chpl-topo.h"
#include "chpltypes.h"
#include "error.h"

#include "comm-ofi-internal.h"

// Don't get warning macros for chpl_comm_get etc
#include "chpl-comm-no-warning-macros.h"

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h> /* for struct iovec */
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h> // for inet_pton

#ifdef CHPL_COMM_DEBUG
#include <ctype.h>
#endif

#include <rdma/fabric.h>
#include <rdma/fi_atomic.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_rma.h>

#ifdef __has_include
#if __has_include(<rdma/fi_cxi_ext.h>)
#include <rdma/fi_cxi_ext.h>
#endif
#endif

////////////////////////////////////////
//
// Data global to all comm-ofi*.c files
//

//
// These are declared extern in comm-ofi-internal.h.
//
#ifdef CHPL_COMM_DEBUG
uint64_t chpl_comm_ofi_dbg_level;
FILE* chpl_comm_ofi_dbg_file;
#endif

int chpl_comm_ofi_abort_on_error;


////////////////////////////////////////
//
// Libfabric API version
//

//
// This is used to check that the libfabric version the runtime is
// linked with in a user program is the same one it was compiled
// against.
//
#define COMM_OFI_FI_VERSION FI_VERSION(FI_MAJOR_VERSION, FI_MINOR_VERSION)


////////////////////////////////////////
//
// Types and data global just within this file.
//

static struct fi_info* ofi_info;        // fabric interface info
static struct fid_fabric* ofi_fabric;   // fabric domain
static struct fid_domain* ofi_domain;   // fabric access domain
static struct fid_ep* ofi_txEpScal;     // scalable transmit endpoint
static struct fid_poll* ofi_amhPollSet; // poll set for AM handler
static int pollSetSize = 0;             // number of fids in the poll set
static struct fid_wait* ofi_amhWaitSet; // wait set for AM handler

/*
A Chapel process uses multiple endpoints to transmit and receive. In
general, there is one transmit endpoint per thread, and one receive
endpoint per active message handler (although this depends on the
number of endpoints the hardware can support vs. the number of
threads). Some providers also support scalable endpoints, in which a
single endpoint uses multiple transmit and/or receive contexts.
Ignoring scalable endpoints for a moment, the transmit endpoints are
stored in the table tciTab below. The receive endpoint is stored in a
separate global variable ofi_rxEp. Each endpoint must have an
associated address vector (AV) that contains the addresses for
communication over that endpoint. For Chapel the addresses are the same
for all endpoints, thus it is possible to share the same AV across all
endpoints so as to reduce overhead. When a single AV is shared this way
it is stored in ofi_av and all of the address vectors in the tciTab
refer to this one as does ofi_rxAv.

When AVs are not shared between endpoints ofi_av is NULL and new address
vectors are created for the entries in tciTab and ofi_rxAv.

When a scalable transmit endpoint is used it is stored in ofi_txEpScal,
and the tciTab entries are used to hold the state for the transmit
contexts. All tciTab entries refer to ofi_av, even on platforms that do
not support AV sharing, because there is only one(scalable) transmit
endpoint. ofi_rxAv refers to ofi_av if AVs can be shared, otherwise it
refers to its own AV.
*/

static struct fid_av*   ofi_av = NULL;   // shared address vector
static fi_addr_t*       ofi_addrs;       // remote endpoint addresses

static struct fid_ep*   ofi_rxEp;       // receive endpoint
static struct fid_cq*   ofi_rxCQ;       // receive endpoint CQ
static struct fid_cntr* ofi_rxCntr;     // receive endpoint counter
static uint64_t         ofi_rxCount;    // # messages already received.
static void*            ofi_rxBuffer;   // receive buffer for new messages
static void*            ofi_rxEnd;      // first byte after buffer
static struct fid_av*   ofi_rxAv;       // address vector
static fi_addr_t*       ofi_rxAddrs;    // table of remote endpoint addresses

#define rxAddr(tcip, n) (tcip->addrs[n])

//
// Transmit support.
//
static chpl_bool envPreferScalableTxEp; // env: prefer scalable tx endpoint?
static int envCommConcurrency;          // env: communication concurrency
static ssize_t envMaxHeapSize;          // env: max heap size
static chpl_bool envOversubscribed;     // env: over-subscribed?
static chpl_bool envUseTxCntr;          // env: tasks use transmit counters
static chpl_bool envUseAmTxCntr;        // env: AMH uses transmit counters
static chpl_bool envUseAmRxCntr;        // env: AMH uses receive counters
static chpl_bool envInjectRMA;          // env: inject RMA messages
static chpl_bool envInjectAMO;          // env: inject AMO messages
static chpl_bool envInjectAM;           // env: inject AM messages
static chpl_bool envUseDedicatedAmhCores;  // env: use dedicated AM cores

static int numTxCtxs;
static int numRxCtxs;

struct perTxCtxInfo_t {
  atomic_bool allocated;        // true: in use; false: available
  chpl_bool bound;              // true: bound to an owner (usually a thread)
  struct fid_av* av;            // address vector
  fi_addr_t* addrs;             // addresses in address vector
  struct fid_ep* txCtx;         // transmit context (endpoint, if not scalable)
  struct fid_cq* txCQ;          // completion CQ
  struct fid_cntr* txCntr;      // completion counter
  struct fid* txCmplFid;        // CQ or counter fid
                                // fn: check for tx completions
  void (*checkTxCmplsFn)(struct perTxCtxInfo_t*);
                                // fn: ensure progress
  void (*ensureProgressFn)(struct perTxCtxInfo_t*);
  uint64_t numTxnsOut;          // number of transactions in flight now
                                // (and for which we expect CQ events)
  uint64_t numTxnsSent;         // number of transactions ever initiated
  void* putVisBitmap;           // nodes needing forced RMA store visibility
  void* amoVisBitmap;           // nodes needing forced AMO store visibility
};

static int tciTabLen;
static struct perTxCtxInfo_t* tciTab;
static chpl_bool tciTabBindTxCtxs;

static size_t txCQLen;

//
// Memory registration support.
//
static chpl_bool scalableMemReg;

struct memEntry {
  void* addr;
  uint64_t base;
  size_t size;
  void* desc;       // returned by fi_mr_desc
  uint64_t key;     // returned by fi_mr_key
};

#define MAX_MEM_REGIONS 10
typedef struct memEntry (memTab_t)[MAX_MEM_REGIONS];

static memTab_t memTab;
static int memTabSize = sizeof(memTab) / sizeof(struct memEntry);
static int memTabCount = 0;

static memTab_t* memTabMap;

static struct fid_mr* ofiMrTab[MAX_MEM_REGIONS];

static chpl_bool  envUseCxiHybridMR;
//
// Messaging (AM) support.
//

#define AM_MAX_EXEC_ON_PAYLOAD_SIZE 1024

struct amRequest_execOn_t {
  chpl_comm_on_bundle_t hdr;
  char space[AM_MAX_EXEC_ON_PAYLOAD_SIZE];
};

struct amRequest_execOnLrg_t {
  chpl_comm_on_bundle_t hdr;
  void* pPayload;                 // addr of arg payload on initiator node
};

#define NUM_AM_HANDLERS 1
static int numAmHandlers = NUM_AM_HANDLERS;
static int reservedCPUs[NUM_AM_HANDLERS];

//
// AM request landing zones.
//
static void* amLZs[2];
static struct iovec ofi_iov_reqs[2];
static struct fi_msg ofi_msg_reqs[2];
static int ofi_msg_i;

//
// These are the major modes in which we can operate in order to
// achieve Chapel MCM conformance.
//
// To conform to the Chapel MCM we need the following within each task:
// - Atomics have to be seen to occur strictly in program order.
// - A PUT followed by a GET from the same address must return the PUT
//   data.
// - When a write (AMO or RMA) is following by a tasking construct or an
//   on-stmt, the written data must be visible to the statement body.
//
// With libfabric we can meet these requirements using one of a few
// different combinations of message ordering settings or, as a last
// resort because it performs poorly, the delivery-complete completion
// level.  Most of the ordering issues have to do with visibility for
// the effects of writes, since libfabric's default completion level
// for reads (AMO or RMA) guarantees that the retrieved data is stored
// before the completion is delivered.  When using message orderings we
// use fenced operations and/or non-programmatic reads to ensure write
// visibility when needed.  Such non-programmatic reads may be done
// immediately after the writes they to which they apply or may be
// delayed until just before the next operation that could depend on
// the write visibility.  The latter is preferred for performance but
// can only be done if the task has a bound transmit context, because
// libfabric message ordering applies only to operations on a given
// endpoint (or context) pair.  Additional commentary throughout the
// code has more details where needed.
//
enum mcmMode_t {
  mcmm_undef,
  mcmm_msgOrdFence,                     // ATOMIC_{RAW,WAR,WAW}, SAS, fences
  mcmm_msgOrd,                          // RAW, WAW, SAW, SAS
  mcmm_dlvrCmplt,                       // delivery-complete
};

static enum mcmMode_t mcmMode;          // overall operational mode

static const char* mcmModeNames[] = { "undefined",
                                      "message-order-fence",
                                      "message-order",
                                      "delivery-complete", };

//
// Provider-specific support.
//

static bool cxiHybridMRMode = false;

////////////////////////////////////////
//
// Forward decls
//

static struct perTxCtxInfo_t* tciAlloc(void);
static struct perTxCtxInfo_t* tciAllocForAmHandler(void);
static chpl_bool tciAllocTabEntry(struct perTxCtxInfo_t*);
static void tciFree(struct perTxCtxInfo_t*);
static void waitForCQSpace(struct perTxCtxInfo_t*, size_t);
static chpl_comm_nb_handle_t ofi_put(const void*, c_nodeid_t, void*, size_t);
static void ofi_put_lowLevel(const void*, void*, c_nodeid_t,
                             uint64_t, uint64_t, size_t, void*,
                             uint64_t, struct perTxCtxInfo_t*);
static void do_remote_put_buff(void*, c_nodeid_t, void*, size_t);
static chpl_comm_nb_handle_t ofi_get(void*, c_nodeid_t, void*, size_t);
static void ofi_get_lowLevel(void*, void*, c_nodeid_t,
                             uint64_t, uint64_t, size_t, void*,
                             uint64_t, struct perTxCtxInfo_t*);
static void do_remote_get_buff(void*, c_nodeid_t, void*, size_t);
static void do_remote_amo_nf_buff(void*, c_nodeid_t, void*, size_t,
                                  enum fi_op, enum fi_datatype);
static void amEnsureProgress(struct perTxCtxInfo_t*);
static void amCheckRxTxCmpls(chpl_bool*, chpl_bool*, struct perTxCtxInfo_t*);
static void checkTxCmplsCQ(struct perTxCtxInfo_t*);
static void checkTxCmplsCntr(struct perTxCtxInfo_t*);
static size_t readCQ(struct fid_cq*, void*, size_t);
static void reportCQError(struct fid_cq*);
static void waitForTxnComplete(struct perTxCtxInfo_t*, void* ctx);
static void forceMemFxVisOneNode(c_nodeid_t, chpl_bool, chpl_bool,
                                 struct perTxCtxInfo_t*);
static void forceMemFxVisAllNodes(chpl_bool, chpl_bool, c_nodeid_t,
                                  struct perTxCtxInfo_t*);
static void forceMemFxVisAllNodes_noTcip(chpl_bool, chpl_bool);
static void* allocBounceBuf(size_t);
static void freeBounceBuf(void*);
static void local_yield(void);

static void time_init(void);

#ifdef CHPL_COMM_DEBUG
static void dbg_catfile(const char*, const char*);
#endif


////////////////////////////////////////
//
// Alignment
//

#define ALIGN_DN(i, size)  ((i) & ~((size) - 1))
#define ALIGN_UP(i, size)  ALIGN_DN((i) + (size) - 1, size)


////////////////////////////////////////
//
// Error checking
//

static void ofiErrReport(const char*, int, const char*);

#define OFI_ERR(exprStr, retVal, errStr)                                \
    do {                                                                \
      ofiErrReport(exprStr, retVal, errStr); /* may not return */       \
      INTERNAL_ERROR_V("OFI error: %s: %s", exprStr, errStr);           \
    } while (0)

#define OFI_CHK(expr) OFI_CHK_1(expr, FI_SUCCESS)

#define OFI_CHK_1(expr, want1)                                          \
    do {                                                                \
      int retVal = (expr);                                              \
      if (retVal != want1) {                                            \
        OFI_ERR(#expr, retVal, fi_strerror(-retVal));                   \
      }                                                                 \
    } while (0)

#define OFI_CHK_2(expr, retVal, want2)                                  \
    do {                                                                \
      retVal = (expr);                                                  \
      if (retVal != FI_SUCCESS && retVal != want2) {                    \
        OFI_ERR(#expr, retVal, fi_strerror(-retVal));                   \
      }                                                                 \
    } while (0)

#define OFI_CHK_3(expr, retVal, want2, want3)                           \
    do {                                                                \
      retVal = (expr);                                                  \
      if (retVal != FI_SUCCESS && retVal != want2 && retVal != want3) { \
        OFI_ERR(#expr, retVal, fi_strerror(-retVal));                   \
      }                                                                 \
    } while (0)

#define OFI_CHK_COUNT(expr, retVal)                                     \
  do {                                                                  \
    retVal = (expr);                                                    \
    if (retVal < 0) {                                                   \
      OFI_ERR(#expr, retVal, fi_strerror(-retVal));                     \
    }                                                                   \
  } while (0)


#define PTHREAD_CHK(expr) CHK_EQ_TYPED(expr, 0, int, "d")


////////////////////////////////////////
//
// Early declarations for AM handling and progress
//

//
// Ideally these would be declared with related stuff later in the
// file, but they're needed earlier than that and with each other,
// so they're here instead.
//

//
// Is this the (an) AM handler thread?
//
static __thread chpl_bool isAmHandler = false;


//
// Flag used to tell AM handler(s) to exit.
//
static atomic_bool amHandlersExit;


//
// Should the AM handler do liveness checks?
//
static chpl_bool amDoLivenessChecks = false;


//
// The ofi_rxm provider may return -FI_EAGAIN for read/write/send while
// doing on-demand connection when emulating FI_RDM endpoints.  The man
// page says: "Applications should be aware of this and retry until the
// the operation succeeds."  Handle this in a generalized way, because
// it seems like something we might encounter with other providers as
// well.
//
#define OFI_RIDE_OUT_EAGAIN(tcip, expr)                                 \
  do {                                                                  \
    ssize_t _ret;                                                       \
    if (isAmHandler) {                                                  \
      do {                                                              \
        OFI_CHK_2(expr, _ret, -FI_EAGAIN);                              \
        if (_ret == -FI_EAGAIN) {                                       \
          (*tcip->ensureProgressFn)(tcip);                              \
        }                                                               \
      } while (_ret == -FI_EAGAIN                                       \
               && !atomic_load_bool(&amHandlersExit));                  \
    } else {                                                            \
      do {                                                              \
        OFI_CHK_2(expr, _ret, -FI_EAGAIN);                              \
        if (_ret == -FI_EAGAIN) {                                       \
          (*tcip->ensureProgressFn)(tcip);                              \
        }                                                               \
      } while (_ret == -FI_EAGAIN);                                     \
    }                                                                   \
  } while (0)


////////////////////////////////////////
//
// Providers
//

//
// provider name in environment
//

static const char* ofi_provNameEnv = "FI_PROVIDER";
static pthread_once_t provNameOnce = PTHREAD_ONCE_INIT;
static const char* provName;

static
void setProvName(void) {
  provName = getenv(ofi_provNameEnv);
}

static
const char* getProviderName(void) {
  PTHREAD_CHK(pthread_once(&provNameOnce, setProvName));
  return provName;
}


//
// provider name parsing
//

static inline
chpl_bool isInProvName(const char* s, const char* prov_name) {
  if (prov_name == NULL) {
    return false;
  }

  char pn[strlen(prov_name) + 1];
  strcpy(pn, prov_name);

  char* tok;
  char* strSave;
  for (char* pnPtr = pn;
       (tok = strtok_r(pnPtr, ";", &strSave)) != NULL;
       pnPtr = NULL) {
    if (strcmp(s, tok) == 0) {
      return true;
    }
  }

  return false;
}


//
// provider type
//
typedef enum {
  provType_efa,
  provType_gni,
  provType_verbs,
  provType_rxd,
  provType_rxm,
  provType_tcp,
  provType_cxi,
  provTypeCount
} provider_t;


//
// provider sets
//

typedef chpl_bool providerSet_t[provTypeCount];

static inline
void providerSetSet(providerSet_t* s, provider_t p) {
  CHK_TRUE(p >= 0 && p < provTypeCount);
  (*s)[p] = 1;
}

static inline
int providerSetTest(providerSet_t* s, provider_t p) {
  CHK_TRUE(p >= 0 && p < provTypeCount);
  return (*s)[p] != 0;
}


//
// providers in use
//

static pthread_once_t providerInUseOnce = PTHREAD_ONCE_INIT;
static providerSet_t providerInUseSet;

static
void init_providerInUse(void) {
  if (chpl_numNodes <= 1) {
    return;
  }

  //
  // We can be using only one primary provider.
  //
  const char* pn = ofi_info->fabric_attr->prov_name;
  if (isInProvName("efa", pn)) {
    providerSetSet(&providerInUseSet, provType_efa);
  } else if (isInProvName("gni", pn)) {
    providerSetSet(&providerInUseSet, provType_gni);
  } else if (isInProvName("tcp", pn)) {
    providerSetSet(&providerInUseSet, provType_tcp);
  } else if (isInProvName("verbs", pn)) {
    providerSetSet(&providerInUseSet, provType_verbs);
  } else if (isInProvName("cxi", pn)) {
    providerSetSet(&providerInUseSet, provType_cxi);
  }

  //
  // We can be using any number of utility providers.
  //
  if (isInProvName("ofi_rxd", pn)) {
    providerSetSet(&providerInUseSet, provType_rxd);
  }
  if (isInProvName("ofi_rxm", pn)) {
    providerSetSet(&providerInUseSet, provType_rxm);
  }
}

static
chpl_bool providerInUse(provider_t p) {
  if (ofi_info != NULL) {
    // Early exit hedge: don't init "in use" info until we have one.
    PTHREAD_CHK(pthread_once(&providerInUseOnce, init_providerInUse));
  }

  return providerSetTest(&providerInUseSet, p);
}


//
// provider-specific behavior control
//

static chpl_bool provCtl_sizeAvsByNumEps;  // size AVs by numEPs (RxD)
static chpl_bool provCtl_readAmoNeedsOpnd; // READ AMO needs operand (RxD)


////////////////////////////////////////
//
// transaction tracking
//

//
// If we need to wait for an individual transaction's network completion
// we give the address of a 'txnDone' flag as the context pointer when we
// initiate the transaction, and then just wait for the flag to become
// true.  We encode this information in the context pointer we pass to
// libfabric, and then it hands it back to us in the CQ entry, and then
// checkTxCQ() uses that to figure out what to update.
//

typedef enum {
  txnTrkId,    // no tracking as such, context "ptr" is just an id value
  txnTrkDone,  // *ptr is atomic bool 'done' flag
  txnTrkTypeCount
} txnTrkType_t;

#define TXNTRK_TYPE_BITS 1
#define TXNTRK_ADDR_BITS (64 - TXNTRK_TYPE_BITS)
#define TXNTRK_TYPE_MASK ((1UL << TXNTRK_TYPE_BITS) - 1UL)
#define TXNTRK_ADDR_MASK (~(TXNTRK_TYPE_MASK << TXNTRK_ADDR_BITS))

typedef struct {
  txnTrkType_t typ;
  void* ptr;
} txnTrkCtx_t;

static inline
void* txnTrkEncode(txnTrkType_t typ, void* p) {
  assert((((uint64_t) txnTrkTypeCount - 1UL) & ~TXNTRK_TYPE_MASK) == 0UL);
  assert((((uint64_t) p) & ~TXNTRK_ADDR_MASK) == 0UL);
  return (void*) (  ((uint64_t) typ << TXNTRK_ADDR_BITS)
                  | ((uint64_t) p & TXNTRK_ADDR_MASK));
}

static inline
void* txnTrkEncodeId(intptr_t id) {
  return txnTrkEncode(txnTrkId, (void*) id);
}

static inline
void* txnTrkEncodeDone(void* pDone) {
  return txnTrkEncode(txnTrkDone, pDone);
}

static inline
txnTrkCtx_t txnTrkDecode(void* ctx) {
  const uint64_t u = (uint64_t) ctx;
  return (txnTrkCtx_t) { .typ = (u >> TXNTRK_ADDR_BITS) & TXNTRK_TYPE_MASK,
                         .ptr = (void*) (u & TXNTRK_ADDR_MASK) };
}


////////////////////////////////////////
//
// transaction ordering
//

//
// This is a little dummy memory buffer used when we need to do a
// remote GET or PUT to enforce ordering.  Its contents must _not_
// by considered meaningful, because it can be written to at any
// time by any remote node.
//
static uint32_t* orderDummy;
static void* orderDummyMRDesc;

struct orderDummyMap_t {
  uint64_t mrRaddr;
  uint64_t mrKey;
};

static struct orderDummyMap_t* orderDummyMap;


////////////////////////////////////////
//
// bitmaps
//

#define bitmapElemWidth 64
#define __bitmapBaseType_t(w) uint##w##_t
#define _bitmapBaseType_t(w) __bitmapBaseType_t(w)
#define bitmapBaseType_t _bitmapBaseType_t(bitmapElemWidth)

struct bitmap_t {
  size_t len;
  bitmapBaseType_t map[0];
};

static inline
size_t bitmapElemIdx(size_t i) {
  return i / bitmapElemWidth;
}

static inline
size_t bitmapOff(size_t i) {
  return i % bitmapElemWidth;
}

static inline
size_t bitmapNumElems(size_t len) {
  return (((ssize_t) len) - 1) / bitmapElemWidth + 1;
}

static inline
size_t bitmapSizeofMap(size_t len) {
  return bitmapNumElems(len) * sizeof(bitmapBaseType_t);
}

static inline
size_t bitmapSizeof(size_t len) {
  return offsetof(struct bitmap_t, map) + bitmapSizeofMap(len);
}

static inline
bitmapBaseType_t bitmapElemBit(size_t i) {
  return ((bitmapBaseType_t) 1) << bitmapOff(i);
}

static inline
void bitmapClear(struct bitmap_t* b, size_t i) {
  b->map[bitmapElemIdx(i)] &= ~bitmapElemBit(i);
}

static inline
void bitmapSet(struct bitmap_t* b, size_t i) {
  b->map[bitmapElemIdx(i)] |= bitmapElemBit(i);
}

static inline
int bitmapTest(struct bitmap_t* b, size_t i) {
  return (b->map[bitmapElemIdx(i)] & bitmapElemBit(i)) != 0;
}

#define BITMAP_FOREACH_SET(b, i)                                        \
  do {                                                                  \
    size_t _eWid = bitmapElemWidth;                                     \
    size_t _eCnt = bitmapNumElems((b)->len);                            \
    size_t _bCnt = (b)->len;                                            \
    for (size_t _ei = 0; _ei < _eCnt; _ei++, _bCnt -= _eWid) {          \
      if ((b)->map[_ei] != 0) {                                         \
        size_t _bi_end = (_eWid < _bCnt) ? _eWid : _bCnt;               \
        for (size_t _bi = 0; _bi < _bi_end; _bi++) {                    \
          if (((b)->map[_ei] & bitmapElemBit(_bi)) != 0) {              \
            size_t i = _ei * bitmapElemWidth + _bi;

#define BITMAP_FOREACH_SET_OR(b1, b2, i)                                \
  do {                                                                  \
    size_t _eWid = bitmapElemWidth;                                     \
    size_t _eCnt = bitmapNumElems((b1)->len);                           \
    size_t _bCnt = (b1)->len;                                           \
    for (size_t _ei = 0; _ei < _eCnt; _ei++, _bCnt -= _eWid) {          \
      bitmapBaseType_t _m = (b1)->map[_ei] | (b2)->map[_ei];            \
      if (_m != 0) {                                                    \
        size_t _bi_end = (_eWid < _bCnt) ? _eWid : _bCnt;               \
        for (size_t _bi = 0; _bi < _bi_end; _bi++) {                    \
          if ((_m & bitmapElemBit(_bi)) != 0) {                         \
            size_t i = _ei * bitmapElemWidth + _bi;

#define BITMAP_FOREACH_SET_END  } } } } } while (0);

static inline
struct bitmap_t* bitmapAlloc(size_t len) {
  struct bitmap_t* b;
  CHPL_CALLOC_SZ(b, 1, bitmapSizeof(len));
  b->len = len;
  return b;
}


////////////////////////////////////////
//
// task private data
//

static inline
chpl_comm_taskPrvData_t* get_comm_taskPrvdata(void) {
  chpl_task_infoRuntime_t* infoRuntime;
  if ((infoRuntime = chpl_task_getInfoRuntime()) != NULL) {
    return &infoRuntime->comm_data;
  }

  if (isAmHandler) {
    static __thread chpl_comm_taskPrvData_t amHandlerCommData;
    return &amHandlerCommData;
  }

  return NULL;
}


////////////////////////////////////////
//
// task local buffering
//

// Largest size to use unordered transactions for
#define MAX_UNORDERED_TRANS_SZ 1024

//
// Maximum number of PUTs/AMOs in a chained transaction list.  This
// is a provisional value, not yet tuned.
//
#define MAX_TXNS_IN_FLIGHT 64

#define MAX_CHAINED_AMO_NF_LEN MAX_TXNS_IN_FLIGHT
#define MAX_CHAINED_PUT_LEN MAX_TXNS_IN_FLIGHT
#define MAX_CHAINED_GET_LEN MAX_TXNS_IN_FLIGHT

enum BuffType {
  amo_nf_buff = 1 << 0,
  get_buff    = 1 << 1,
  put_buff    = 1 << 2
};

// Per task information about non-fetching AMO buffers
typedef struct {
  chpl_bool          new;
  int                vi;
  uint64_t           opnd_v[MAX_CHAINED_AMO_NF_LEN];
  c_nodeid_t         locale_v[MAX_CHAINED_AMO_NF_LEN];
  void*              object_v[MAX_CHAINED_AMO_NF_LEN];
  size_t             size_v[MAX_CHAINED_AMO_NF_LEN];
  enum fi_op         cmd_v[MAX_CHAINED_AMO_NF_LEN];
  enum fi_datatype   type_v[MAX_CHAINED_AMO_NF_LEN];
  uint64_t           remote_mr_v[MAX_CHAINED_AMO_NF_LEN];
  void*              local_mr;
} amo_nf_buff_task_info_t;

// Per task information about GET buffers
typedef struct {
  chpl_bool     new;
  int           vi;
  void*         tgt_addr_v[MAX_CHAINED_GET_LEN];
  c_nodeid_t    locale_v[MAX_CHAINED_GET_LEN];
  uint64_t      remote_mr_v[MAX_CHAINED_GET_LEN];
  void*         src_addr_v[MAX_CHAINED_GET_LEN];
  size_t        size_v[MAX_CHAINED_GET_LEN];
  void*         local_mr_v[MAX_CHAINED_GET_LEN];
} get_buff_task_info_t;

// Per task information about PUT buffers
typedef struct {
  chpl_bool     new;
  int           vi;
  void*         tgt_addr_v[MAX_CHAINED_PUT_LEN];
  c_nodeid_t    locale_v[MAX_CHAINED_PUT_LEN];
  void*         src_addr_v[MAX_CHAINED_PUT_LEN];
  char          src_v[MAX_CHAINED_PUT_LEN][MAX_UNORDERED_TRANS_SZ];
  size_t        size_v[MAX_CHAINED_PUT_LEN];
  uint64_t      remote_mr_v[MAX_CHAINED_PUT_LEN];
  void*         local_mr_v[MAX_CHAINED_PUT_LEN];
} put_buff_task_info_t;

// Acquire a task local buffer, initializing if needed
static inline
void* task_local_buff_acquire(enum BuffType t) {
  chpl_comm_taskPrvData_t* prvData = get_comm_taskPrvdata();
  if (prvData == NULL) return NULL;

#define DEFINE_INIT(TYPE, TLS_NAME)                                     \
  if (t == TLS_NAME) {                                                  \
    TYPE* info = prvData->TLS_NAME;                                     \
    if (info == NULL) {                                                 \
      CHPL_CALLOC_SZ(prvData->TLS_NAME, 1, sizeof(TYPE));               \
      info = prvData->TLS_NAME;                                         \
      info->new = true;                                                 \
      info->vi = 0;                                                     \
    }                                                                   \
    return info;                                                        \
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

#define DEFINE_FLUSH(TYPE, TLS_NAME, FLUSH_NAME)                        \
  if (t & TLS_NAME) {                                                   \
    TYPE* info = prvData->TLS_NAME;                                     \
    if (info != NULL && info->vi > 0) {                                 \
      FLUSH_NAME(info);                                                 \
    }                                                                   \
  }

  DEFINE_FLUSH(amo_nf_buff_task_info_t, amo_nf_buff,
               amo_nf_buff_task_info_flush);
  DEFINE_FLUSH(get_buff_task_info_t, get_buff, get_buff_task_info_flush);
  DEFINE_FLUSH(put_buff_task_info_t, put_buff, put_buff_task_info_flush);

#undef DEFINE_FLUSH
}

// Flush and destroy one or more task local buffers
static inline
void task_local_buff_end(enum BuffType t) {
  chpl_comm_taskPrvData_t* prvData = get_comm_taskPrvdata();
  if (prvData == NULL) return;

#define DEFINE_END(TYPE, TLS_NAME, FLUSH_NAME)                          \
  if (t & TLS_NAME) {                                                   \
    TYPE* info = prvData->TLS_NAME;                                     \
    if (info != NULL && info->vi > 0) {                                 \
      FLUSH_NAME(info);                                                 \
      chpl_mem_free(info, 0, 0);                                        \
      prvData->TLS_NAME = NULL;                                         \
    }                                                                   \
  }

  DEFINE_END(amo_nf_buff_task_info_t, amo_nf_buff,
             amo_nf_buff_task_info_flush);
  DEFINE_END(get_buff_task_info_t, get_buff, get_buff_task_info_flush);
  DEFINE_END(put_buff_task_info_t, put_buff, put_buff_task_info_flush);

#undef END
}


////////////////////////////////////////
//
// Interface: initialization
//

pthread_t pthread_that_inited;

static void init_ofi(void);
static void init_ofiFabricDomain(void);
static void init_ofiReserveCores(void);
static void init_ofiDoProviderChecks(void);
static void init_ofiEp(void);
static void init_ofiEpTxCtx(int, chpl_bool, struct fi_av_attr*,
                            struct fi_cq_attr*, struct fi_cntr_attr*);
static void init_ofiExchangeAvInfo(void);
static void init_ofiForMem(void);
static void init_ofiForRma(void);
static void init_ofiForAms(void);
static void init_ofiConnections(void);

static void init_bar(void);

static void init_broadcast_private(void);

////////////////////////////////////////
//
// Misc.
//

const char *chpl_comm_oob = "unknown";

//
// forward decls
//
static chpl_bool mrGetKey(uint64_t*, uint64_t*, int, void*, size_t);
static chpl_bool mrGetLocalKey(void*, size_t);
static chpl_bool mrGetDesc(void**, void*, size_t);

void chpl_comm_pre_topo_init(void) {
  chpl_comm_ofi_abort_on_error =
    (chpl_env_rt_get("COMM_OFI_ABORT_ON_ERROR", NULL) != NULL);
  time_init();
  chpl_comm_ofi_oob_init();
  DBG_INIT();
  int32_t rank;
  int32_t count = chpl_comm_ofi_oob_locales_on_node(&rank);
  chpl_set_num_locales_on_node(count);
  if (rank != -1) {
    chpl_set_local_rank(rank);
  }
}

void chpl_comm_init(int *argc_p, char ***argv_p) {
  //
  // Gather run-invariant environment info as early as possible.
  //
  envPreferScalableTxEp = chpl_env_rt_get_bool("COMM_OFI_PREFER_SCALABLE_EP",
                                               true);

  envCommConcurrency = chpl_env_rt_get_int("COMM_CONCURRENCY", 0);
  if (envCommConcurrency < 0) {
    chpl_warning("CHPL_RT_COMM_CONCURRENCY < 0, ignored", 0, 0);
    envCommConcurrency = 0;
  }

  envMaxHeapSize = chpl_comm_getenvMaxHeapSize();

  envOversubscribed = chpl_env_rt_get_bool("OVERSUBSCRIBED", false);

  envUseTxCntr = chpl_env_rt_get_bool("COMM_OFI_TX_COUNTER", false);
  envUseAmTxCntr = chpl_env_rt_get_bool("COMM_OFI_AM_TX_COUNTER", false);
  envUseAmRxCntr = chpl_env_rt_get_bool("COMM_OFI_AM_RX_COUNTER", false);

  envUseCxiHybridMR = chpl_env_rt_get_bool("COMM_OFI_CXI_HYBRID_MR", true);

  envInjectRMA = chpl_env_rt_get_bool("COMM_OFI_INJECT_RMA", true);
  envInjectAMO = chpl_env_rt_get_bool("COMM_OFI_INJECT_AMO", true);
  envInjectAM = chpl_env_rt_get_bool("COMM_OFI_INJECT_AM", true);
  envUseDedicatedAmhCores = chpl_env_rt_get_bool(
                                  "COMM_OFI_DEDICATED_AMH_CORES", false);
  //
  // The user can specify the provider by setting either the Chapel
  // CHPL_RT_COMM_OFI_PROVIDER environment variable or the libfabric
  // FI_PROVIDER one, with the former overriding the latter if both
  // are set.
  //
  {
    const char* s = chpl_env_rt_get("COMM_OFI_PROVIDER", NULL);
    if (s != NULL) {
      chpl_env_set(ofi_provNameEnv, s, 1 /*overwrite*/);
    }
  }

  pthread_that_inited = pthread_self();
}

void chpl_comm_pre_mem_init(void) {
  //
  // Reserve cores for the AM handlers. This is done here because it has to
  // happen after chpl_topo_init has been called, but before other functions
  // access information about the cores, such as pinning the heap.
  //
  init_ofiReserveCores();
}


void chpl_comm_post_mem_init(void) {
  DBG_PRINTF(DBG_IFACE_SETUP, "%s()", __func__);

  chpl_comm_init_prv_bcast_tab();
  init_broadcast_private();

  /*
    Previously this was called by init_ofi which is called by
    chpl_comm_post_task_init. It's been moved here because fi_domain,
    which this function calls, is not thread-safe and may cause crashes
    if threads are using malloc when it is called. Moving it here
    causes fi_domain to be called before the worker threads are
    created, avoiding the issue.
  */
  if (chpl_numNodes > 1) {
    init_ofiFabricDomain();
  }
}


//
// No support for gdb for now
//
int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
  return 0;
}

//
// No support for lldb for now
//
int chpl_comm_run_in_lldb(int argc, char* argv[], int lldbArgnum, int* status) {
  return 0;
}


void chpl_comm_post_task_init(void) {
  DBG_PRINTF(DBG_IFACE_SETUP, "%s()", __func__);

  if (chpl_numNodes == 1)
    return;
  init_ofi();
  init_bar();
}


static
void init_ofi(void) {
  if (verbosity >= 2) {
    if (chpl_nodeID == 0) {
      void* start;
      size_t size;
      chpl_comm_regMemHeapInfo(&start, &size);
      char buf[10];
      printf("COMM=ofi: %s MCM mode, \"%s\" provider, \"%s\" device, %s fixed heap\n",
             mcmModeNames[mcmMode], ofi_info->fabric_attr->prov_name,
             ofi_info->domain_attr->name,
             ((size == 0)
              ? "no"
              : chpl_snprintf_KMG_z(buf, sizeof(buf), size)));
    }
  }

  init_ofiDoProviderChecks();
  init_ofiEp();
  init_ofiExchangeAvInfo();
  init_ofiForMem();
  init_ofiForRma();
  init_ofiForAms();

  CHPL_CALLOC(orderDummy, 1);
  CHK_TRUE(mrGetDesc(&orderDummyMRDesc, orderDummy, sizeof(*orderDummy)));
  CHPL_CALLOC(orderDummyMap, chpl_numNodes);
  struct orderDummyMap_t odm;
  CHK_TRUE(mrGetKey(&odm.mrKey, &odm.mrRaddr, chpl_nodeID, orderDummy,
                    sizeof(*orderDummy)));
  chpl_comm_ofi_oob_allgather(&odm, orderDummyMap, sizeof(orderDummyMap[0]));

  init_ofiConnections();

  DBG_PRINTF(DBG_CFG,
             "AM config: recv buf size %zd MiB, %s, responses use %s",
             ofi_iov_reqs[ofi_msg_i].iov_len / (1L << 20),
             (ofi_amhPollSet == NULL) ? "explicit polling" : "poll+wait sets",
             (tciTab[tciTabLen - 1].txCntr == NULL) ? "CQ" : "counter");
  if (ofi_txEpScal != NULL) {
    DBG_PRINTF(DBG_CFG,
               "per node config: 1 scalable tx ep + %d tx ctx%s (%d bound), "
               "%d rx ctx%s",
               numTxCtxs, (numTxCtxs == 1) ? "" : "s",
               tciTabBindTxCtxs ? chpl_task_getFixedNumThreads() : 0,
               numRxCtxs, (numRxCtxs == 1) ? "" : "s");
  } else {
    DBG_PRINTF(DBG_CFG,
               "per node config: %d regular tx ep+ctx%s (%d bound), "
               "%d rx ctx%s",
               numTxCtxs, (numTxCtxs == 1) ? "" : "s",
               tciTabBindTxCtxs ? chpl_task_getFixedNumThreads() : 0,
               numRxCtxs, (numRxCtxs == 1) ? "" : "s");
  }
}


#ifdef CHPL_COMM_DEBUG
struct cfgHint {
  const char* str;
  unsigned long int val;
};


static
chpl_bool getCfgHint(const char* evName, struct cfgHint hintVals[],
                     chpl_bool justOne, uint64_t* pVal) {
  const char* ev = chpl_env_rt_get(evName, "");
  if (strcmp(ev, "") == 0) {
    return false;
  }

  *pVal = 0;

  char evCopy[strlen(ev) + 1];
  strcpy(evCopy, ev);
  char* p = strtok(evCopy, "|");
  while (p != NULL) {
    int i;
    for (i = 0; hintVals[i].str != NULL; i++) {
      if (strcmp(p, hintVals[i].str) == 0) {
        *pVal |= hintVals[i].val;
        break;
      }
    }
    if (hintVals[i].str == NULL) {
      INTERNAL_ERROR_V("unknown config hint val in CHPL_RT_%s: \"%s\"",
                       evName, p);
    }
    p = strtok(NULL, "|");
    if (justOne && p != NULL) {
      INTERNAL_ERROR_V("too many config hint vals in CHPL_RT_%s=\"%s\"",
                       evName, ev);
    }
  }

  return true;
}


static
void debugOverrideHints(struct fi_info* hints) {
  #define CFG_HINT(s)    { #s, (uint64_t) (s) }
  #define CFG_HINT_NULL  { NULL, 0ULL }

  uint64_t val;

  {
    struct cfgHint hintVals[] = { CFG_HINT(FI_ATOMIC),
                                  CFG_HINT(FI_DIRECTED_RECV),
                                  CFG_HINT(FI_FENCE),
                                  CFG_HINT(FI_HMEM),
                                  CFG_HINT(FI_LOCAL_COMM),
                                  CFG_HINT(FI_MSG),
                                  CFG_HINT(FI_MULTICAST),
                                  CFG_HINT(FI_MULTI_RECV),
                                  CFG_HINT(FI_NAMED_RX_CTX),
                                  CFG_HINT(FI_READ),
                                  CFG_HINT(FI_RECV),
                                  CFG_HINT(FI_REMOTE_COMM),
                                  CFG_HINT(FI_REMOTE_READ),
                                  CFG_HINT(FI_REMOTE_WRITE),
                                  CFG_HINT(FI_RMA),
                                  CFG_HINT(FI_RMA_EVENT),
                                  CFG_HINT(FI_RMA_PMEM),
                                  CFG_HINT(FI_SEND),
                                  CFG_HINT(FI_SHARED_AV),
                                  CFG_HINT(FI_SOURCE),
                                  CFG_HINT(FI_SOURCE_ERR),
                                  CFG_HINT(FI_TAGGED),
                                  CFG_HINT(FI_TRIGGER),
                                  CFG_HINT(FI_VARIABLE_MSG),
                                  CFG_HINT(FI_WRITE),
                                  CFG_HINT_NULL, };
    if (getCfgHint("COMM_OFI_HINTS_CAPS",
                   hintVals, false /*justOne*/, &val)) {
      hints->caps = val;
    }
  }

  {
    struct cfgHint hintVals[] = { CFG_HINT(FI_COMMIT_COMPLETE),
                                  CFG_HINT(FI_COMPLETION),
                                  CFG_HINT(FI_DELIVERY_COMPLETE),
                                  CFG_HINT(FI_INJECT),
                                  CFG_HINT(FI_INJECT_COMPLETE),
                                  CFG_HINT(FI_TRANSMIT_COMPLETE),
                                  CFG_HINT_NULL, };
    if (getCfgHint("COMM_OFI_HINTS_TX_OP_FLAGS",
                   hintVals, false /*justOne*/, &val)) {
      hints->tx_attr->op_flags = val;
    }
  }

  {
    struct cfgHint hintVals[] = { CFG_HINT(FI_ORDER_ATOMIC_RAR),
                                  CFG_HINT(FI_ORDER_ATOMIC_RAW),
                                  CFG_HINT(FI_ORDER_ATOMIC_WAR),
                                  CFG_HINT(FI_ORDER_ATOMIC_WAW),
                                  CFG_HINT(FI_ORDER_NONE),
                                  CFG_HINT(FI_ORDER_RAR),
                                  CFG_HINT(FI_ORDER_RAS),
                                  CFG_HINT(FI_ORDER_RAW),
                                  CFG_HINT(FI_ORDER_RMA_RAR),
                                  CFG_HINT(FI_ORDER_RMA_RAW),
                                  CFG_HINT(FI_ORDER_RMA_WAR),
                                  CFG_HINT(FI_ORDER_RMA_WAW),
                                  CFG_HINT(FI_ORDER_SAR),
                                  CFG_HINT(FI_ORDER_SAS),
                                  CFG_HINT(FI_ORDER_SAW),
                                  CFG_HINT(FI_ORDER_WAR),
                                  CFG_HINT(FI_ORDER_WAS),
                                  CFG_HINT(FI_ORDER_WAW),
                                  CFG_HINT_NULL, };
    if (getCfgHint("COMM_OFI_HINTS_MSG_ORDER",
                   hintVals, false /*justOne*/, &val)) {
      hints->tx_attr->msg_order = hints->rx_attr->msg_order = val;
    }
  }

  {
    struct cfgHint hintVals[] = { CFG_HINT(FI_COMMIT_COMPLETE),
                                  CFG_HINT(FI_COMPLETION),
                                  CFG_HINT(FI_DELIVERY_COMPLETE),
                                  CFG_HINT(FI_MULTI_RECV),
                                  CFG_HINT_NULL, };
    if (getCfgHint("COMM_OFI_HINTS_RX_OP_FLAGS",
                   hintVals, false /*justOne*/, &val)) {
      hints->rx_attr->op_flags = val;
    }
  }

  {
    struct cfgHint hintVals[] = { CFG_HINT(FI_PROGRESS_UNSPEC),
                                  CFG_HINT(FI_PROGRESS_AUTO),
                                  CFG_HINT(FI_PROGRESS_MANUAL),
                                  CFG_HINT_NULL, };
    if (getCfgHint("COMM_OFI_HINTS_CONTROL_PROGRESS",
                   hintVals, true /*justOne*/, &val)) {
      hints->domain_attr->control_progress = (enum fi_progress) val;
    }
    if (getCfgHint("COMM_OFI_HINTS_DATA_PROGRESS",
                   hintVals, true /*justOne*/, &val)) {
      hints->domain_attr->data_progress = (enum fi_progress) val;
    }
  }

  {
    struct cfgHint hintVals[] = { CFG_HINT(FI_THREAD_UNSPEC),
                                  CFG_HINT(FI_THREAD_SAFE),
                                  CFG_HINT(FI_THREAD_FID),
                                  CFG_HINT(FI_THREAD_DOMAIN),
                                  CFG_HINT(FI_THREAD_COMPLETION),
                                  CFG_HINT(FI_THREAD_ENDPOINT),
                                  CFG_HINT_NULL, };
    if (getCfgHint("COMM_OFI_HINTS_THREADING",
                   hintVals, true /*justOne*/, &val)) {
      hints->domain_attr->threading = (enum fi_threading) val;
    }
  }

  {
    struct cfgHint hintVals[] = { CFG_HINT(FI_MR_UNSPEC),
                                  CFG_HINT(FI_MR_BASIC),
                                  CFG_HINT(FI_MR_SCALABLE),
                                  CFG_HINT(FI_MR_LOCAL),
                                  CFG_HINT(FI_MR_RAW),
                                  CFG_HINT(FI_MR_VIRT_ADDR),
                                  CFG_HINT(FI_MR_ALLOCATED),
                                  CFG_HINT(FI_MR_PROV_KEY),
                                  CFG_HINT(FI_MR_MMU_NOTIFY),
                                  CFG_HINT(FI_MR_RMA_EVENT),
                                  CFG_HINT(FI_MR_ENDPOINT),
                                  CFG_HINT(FI_MR_HMEM),
                                  CFG_HINT_NULL, };
    if (getCfgHint("COMM_OFI_HINTS_MR_MODE",
                   hintVals, false /*justOne*/, &val)) {
      hints->domain_attr->mr_mode = (int) val;
    }
  }

  #undef CFG_HINT
  #undef CFG_HINT_NULL
}
#endif


static inline
chpl_bool isInProvider(const char* s, struct fi_info* info) {
  return isInProvName(s, info->fabric_attr->prov_name);
}


static inline
chpl_bool isGoodCoreProvider(struct fi_info* info) {
  return (!isInProvName("sockets", info->fabric_attr->prov_name)
          && !isInProvName("tcp", info->fabric_attr->prov_name));
}

// This function allows us to filter out unusable providers returned by
// libfabric. libfabric currently has bugs associated with some providers
// and address types.
static inline
chpl_bool isUseableProvider(struct fi_info* info) {
  chpl_bool result = true;

  // Ignore any provider for the T2 interface on a mac. As of v1.13.0
  // libfabric does not filter this interface internally. The T2 interface
  // is identified by the link local address fe80::aede:48ff:fe00:1122.

  static struct sockaddr_in6 t2;
  static chpl_bool initialized = false;
  static chpl_bool darwin = false;

  if (! initialized) {
    darwin = !strcmp(CHPL_TARGET_PLATFORM, "darwin");
    if (darwin) {
      int rc = inet_pton(AF_INET6, "fe80::aede:48ff:fe00:1122", &t2.sin6_addr);
      if (rc != 1) {
        INTERNAL_ERROR_V("inet_pton failed: %s", strerror(errno));
      }
    }
    initialized = true;
  }
  if (darwin && (info->addr_format == FI_SOCKADDR_IN6) &&
      !memcmp(&t2.sin6_addr,
              &((struct sockaddr_in6 *)info->src_addr)->sin6_addr,
              sizeof(t2.sin6_addr))) {
    DBG_PRINTF_NODE0(DBG_PROV, "skipping T2 interface");
    result = false;
  }

  // The verbs provider currently doesn't handle FI_SOCKADDR_IB addresses
  // properly. Trying to use one will result in internal errors in fi_domain.

  if ((info->addr_format == FI_SOCKADDR_IB) && isInProvider("verbs", info)) {
     DBG_PRINTF_NODE0(DBG_PROV, "skipping FI_SOCKADDR_IB address");
     result = false;
  }

  // The sockets provider has a bug w/ IPv6 addresses
  if ((info->addr_format == FI_SOCKADDR_IN6) && isInProvider("sockets", info)) {
     DBG_PRINTF_NODE0(DBG_PROV, "skipping sockets/FI_SOCKADDR_IN6 provider");
     result = false;
  }

  return result;
}

static inline
struct fi_info* findProvInList(struct fi_info* info,
                               chpl_bool accept_ungood_provs,
                               chpl_bool accept_RxD_provs,
                               chpl_bool accept_RxM_provs,
                               chpl_bool accept_sockets_provs) {

  for (; info != NULL; info = info->next) {
    // break out of the loop when we find one that meets all of our criteria
    if (!accept_ungood_provs && !isGoodCoreProvider(info)) {
      continue;
    }
    if (!accept_RxD_provs && isInProvider("ofi_rxd", info)) {
      continue;
    }
    if (!accept_RxM_provs && isInProvider("ofi_rxm", info)) {
      continue;
    }
    if (!accept_sockets_provs && isInProvider("sockets", info)) {
      continue;
    }
    if (!isUseableProvider(info)) {
      continue;
    }
    // got one
    break;
  }
  if (info && (isInProvider("sockets", info))) {
    chpl_warning("sockets provider is deprecated", 0, 0);
  }

  // some providers incorrectly report that they supports FI_ORDER_ATOMIC_RAW
  // and FI_ORDER_ATOMIC_WAR
  if (info && (isInProvider("cxi", info))) {
    info->tx_attr->msg_order &= ~(FI_ORDER_ATOMIC_RAW | FI_ORDER_ATOMIC_WAR);
    info->rx_attr->msg_order &= ~(FI_ORDER_ATOMIC_RAW | FI_ORDER_ATOMIC_WAR);
  }
  return (info == NULL) ? NULL : fi_dupinfo(info);
}


static
chpl_bool findProvGivenHints(struct fi_info** p_infoOut,
                             struct fi_info* infoIn,
                             chpl_bool accept_RxD_provs,
                             chpl_bool accept_RxM_provs,
                             chpl_bool accept_sockets_provs,
                             enum mcmMode_t mcmm) {
  struct fi_info* infoList;
  int ret;
  OFI_CHK_2(fi_getinfo(COMM_OFI_FI_VERSION, NULL, NULL, 0, infoIn, &infoList),
            ret, -FI_ENODATA);

  struct fi_info* info;
  info = findProvInList(infoList,
                        (getProviderName() != NULL) /*accept_ungood_provs*/,
                        accept_RxD_provs, accept_RxM_provs,
                        accept_sockets_provs);

  DBG_PRINTF_NODE0(DBG_PROV,
                   "** %s desirable provider with %s",
                   (info == NULL) ? "no" : "found", mcmModeNames[mcmm]);

  if (info == NULL) {
    *p_infoOut = infoList;
    return false;
  }

  fi_freeinfo(infoList);
  *p_infoOut = info;
  return true;
}


static
chpl_bool findProvGivenList(struct fi_info** p_infoOut,
                            struct fi_info* infoIn,
                            chpl_bool accept_RxD_provs,
                            chpl_bool accept_RxM_provs,
                            chpl_bool accept_sockets_provs,
                            enum mcmMode_t mcmm) {
  struct fi_info* info;
  info = findProvInList(infoIn,
                        true /*accept_ungood_provs*/,
                        accept_RxD_provs, accept_RxM_provs,
                        accept_sockets_provs);

  DBG_PRINTF_NODE0(DBG_PROV,
                   "** %s less-desirable provider with %s",
                   (info == NULL) ? "no" : "found", mcmModeNames[mcmm]);

  if (info == NULL) {
    return false;
  }

  *p_infoOut = info;
  return true;
}


static
chpl_bool canBindTxCtxs(struct fi_info* info) {
  //
  // This function decides whether we will be able to run with bound
  // transmit contexts if we choose to use the passed-in provider.
  //

  //
  // We can only support contexts bound to threads if the tasking layer
  // uses a fixed number of threads.
  //
  int fixedNumThreads = chpl_task_getFixedNumThreads();
  if (fixedNumThreads <= 0) {
    return false;
  }

  //
  // Gather invariant info.  The simplistic first-time check here is
  // sufficient because we only get called from single-threaded code
  // while examining provider candidates.
  //
  static chpl_bool haveInvariants = false;
  if (!haveInvariants) {
    haveInvariants = true;
    envPreferScalableTxEp = chpl_env_rt_get_bool("COMM_OFI_PREFER_SCALABLE_EP",
                                                 true);
    envCommConcurrency = chpl_env_rt_get_int("COMM_CONCURRENCY", 0);
    if (envCommConcurrency < 0) {
      chpl_warning("CHPL_RT_COMM_CONCURRENCY < 0, ignored", 0, 0);
      envCommConcurrency = 0;
    }
  }

  //
  // Note for future maintainers: if interoperability between Chapel
  // and other languages someday results in non-tasking layer threads
  // calling Chapel code which then tries to communicate across nodes,
  // then some of this may need adjusting, notably the fixed-threads
  // logic.
  //

  //
  // Start with the maximum number of transmit contexts/endpoints the
  // provider could support.  Reduce that to allow for one tx context to
  // be shared among non-worker pthreads including the process itself,
  // and for one private tx context for each AM handler.  If the user
  // limited communication concurrency via the environment to less than
  // what's left, reduce further.  If this leaves us with at least as
  // many tx contexts as the tasking layer's fixed thread count then
  // we'll use bound tx contexts with this provider.
  //
  const struct fi_domain_attr* dom_attr = info->domain_attr;
  int numWorkerTxCtxs = ((envPreferScalableTxEp
                          && dom_attr->max_ep_tx_ctx > 1)
                         ? dom_attr->max_ep_tx_ctx
                         : dom_attr->ep_cnt)
                        - 1
                        - numAmHandlers;
  if (envCommConcurrency > 0 && envCommConcurrency < numWorkerTxCtxs) {
    numWorkerTxCtxs = envCommConcurrency;
  }

  return fixedNumThreads <= numWorkerTxCtxs;
}


//
// The find*Prov() functions operate in one of two ways, as selected by
// the inputIsHints flag.  We call them in some order the first time to
// try to find a fabric with a "good" provider, and then if all those
// attempts fail we call them again in the same order to find a fabric
// with a not-so-good-but-acceptable provider.  To save a little time,
// we use the candidate list resulting from the first call as the input
// for the second one.
//
// If inputIsHints is true, then infoIn contains basic hints that are to
// be adjusted appropriately for the major mode the function tries to
// match, and then passed to fi_getinfo() to get a list of candidate
// fabrics.  If the candidate list contains a fabric instance with a
// good provider, output variables *p_infoOut and *p_modeOut are set to
// that fabric and the resulting major operational mode, respectively,
// and the function returns true.  Otherwise, *p_infoOut is set to the
// entire candidate list, *p_modeOut is unchanged, and the function
// returns false.
//
// If inputIsHints is false, then infoIn is the candidate list from the
// first call.  If that list contains a fabric instance with any usable
// provider, output variables *p_infoOut and *p_modeOut are set to that
// fabric and the resulting major operational mode, respectively, and
// the function returns true.  Otherwise *p_infoOut and *p_modeOut are
// unchanged and the function returns false.
//
// Each find*Prov() function has companion is*Prov() and setCheck*Prov()
// functions.  The is*Prov() functions return the corresponding MCM mode
// constant iff the given fabric instance has the capabilities that the
// find*Prov() function looks for.  These functions are used in setting
// the MCM conformance mode when the provider is forced rather than
// chosen.  The setCheck*Prov() functions either add the appropriate
// capabilities to existing hints ("set") or determine whether a given
// fabric instance has all the capabilities needed ("check").  These
// functions are building blocks for both the find*Prov() and is*Prov()
// functions, and serve to encapsulate in just one place the specific
// capabilities needed for each MCM conformance mode.
//
typedef chpl_bool (fnFindProv_t)(struct fi_info**, enum mcmMode_t*,
                                 struct fi_info*, chpl_bool);
static fnFindProv_t findMsgOrderFenceProv;
static fnFindProv_t findMsgOrderProv;
static fnFindProv_t findDlvrCmpltProv;

typedef enum mcmMode_t (fnIsProv_t)(struct fi_info*);
static fnIsProv_t isMsgOrderFenceProv;
static fnIsProv_t isMsgOrderProv;
static fnIsProv_t isDlvrCmpltProv;

static
struct fi_info* setCheckMsgOrderFenceProv(struct fi_info* info,
                                          chpl_bool set) {
  uint64_t need_caps = FI_ATOMIC | FI_FENCE;
  //
  // Note: we don't ask for FI_ORDER_ATOMIC_RAW because the some providers
  // doesn't support it.  FI_ORDER_ATOMIC_WAR ordering is enforced by the
  // MCM.
  //
  uint64_t need_msg_orders =   FI_ORDER_ATOMIC_WAW
                             | FI_ORDER_SAS;
  if (set) {
    // Only use this mode if the tasking layer has a fixed number of threads.
    if (chpl_task_hasFixedNumThreads() == false) {
      return NULL;
    }
    info = fi_dupinfo(info);
    info->caps |= need_caps;
    info->tx_attr->msg_order = need_msg_orders;
    info->rx_attr->msg_order = need_msg_orders;
    return info;
  } else {
    //
    // In addition to needing to be able to support the specific message
    // ordering settings we require, for message-order-fence mode there
    // must be a fixed number of worker threads so that we can bind
    // tx contexts to threads and delay memory visibility.
    //
    return ((info->caps & need_caps) == need_caps
            && (info->tx_attr->msg_order & need_msg_orders) == need_msg_orders
            && (info->rx_attr->msg_order & need_msg_orders) == need_msg_orders
            && chpl_task_hasFixedNumThreads())
           ? info
           : NULL;
  }
}


static
chpl_bool findMsgOrderFenceProv(struct fi_info** p_infoOut,
                                enum mcmMode_t* p_modeOut,
                                struct fi_info* infoIn,
                                chpl_bool inputIsHints) {
  //
  // Try to find a provider that can conform to the MCM using atomic
  // message orderings plus fences.  We try to avoid using either the
  // RxD or RxM utility providers here, because we really only want
  // to use atomics where the network can do them natively.
  //
  const char* prov_name = getProviderName();
  const chpl_bool accept_RxD_provs = isInProvName("ofi_rxd", prov_name);
  const chpl_bool accept_RxM_provs = isInProvName("ofi_rxm", prov_name);
  const chpl_bool accept_sockets_provs = isInProvName("sockets", prov_name);
  enum mcmMode_t mcmm = mcmm_msgOrdFence;
  chpl_bool ret = false;

  if (inputIsHints) {
    struct fi_info* infoAdj = setCheckMsgOrderFenceProv(infoIn, true /*set*/);
    if (infoAdj) {
      ret = findProvGivenHints(p_infoOut, infoAdj,
                              accept_RxD_provs, accept_RxM_provs,
                              accept_sockets_provs, mcmm);
      fi_freeinfo(infoAdj);
    } else {
      DBG_PRINTF_NODE0(DBG_PROV, "** ignoring providers with %s",
                   mcmModeNames[mcmm]);
    }
  } else {
    ret = findProvGivenList(p_infoOut, infoIn,
                            accept_RxD_provs, accept_RxM_provs,
                            accept_sockets_provs, mcmm);
  }

  if (ret) {
    *p_modeOut = mcmm;
  }

  return ret;
}


static
enum mcmMode_t isMsgOrderFenceProv(struct fi_info* info) {
  return (setCheckMsgOrderFenceProv(info, false /*check*/) == info)
         ? mcmm_msgOrdFence
         : mcmm_undef;
}


static
struct fi_info* setCheckMsgOrderProv(struct fi_info* info,
                                     chpl_bool set) {
  uint64_t need_msg_orders = FI_ORDER_RAW
                             | FI_ORDER_WAW
                             | FI_ORDER_SAW
                             | FI_ORDER_SAS;
  if (set) {
    info = fi_dupinfo(info);
    info->tx_attr->msg_order = need_msg_orders;
    info->rx_attr->msg_order = need_msg_orders;
    return info;
  } else {
    return ((info->tx_attr->msg_order & need_msg_orders) == need_msg_orders
            && (info->rx_attr->msg_order & need_msg_orders) == need_msg_orders)
           ? info
           : NULL;
  }
}


static
chpl_bool findMsgOrderProv(struct fi_info** p_infoOut,
                           enum mcmMode_t* p_modeOut,
                           struct fi_info* infoIn,
                           chpl_bool inputIsHints) {
  //
  // Try to find a provider that supports message orderings sufficient
  // for our MCM, all by themselves.
  //
  const char* prov_name = getProviderName();
  const chpl_bool accept_RxD_provs = isInProvName("ofi_rxd", prov_name);
  const chpl_bool accept_RxM_provs = true;
  const chpl_bool accept_sockets_provs = isInProvName("sockets", prov_name);
  enum mcmMode_t mcmm = mcmm_msgOrd;
  chpl_bool ret = false;

  if (inputIsHints) {
    struct fi_info* infoAdj = setCheckMsgOrderProv(infoIn, true /*set*/);
    if (infoAdj) {
      ret = findProvGivenHints(p_infoOut, infoAdj,
                              accept_RxD_provs, accept_RxM_provs,
                              accept_sockets_provs, mcmm);
      fi_freeinfo(infoAdj);
    } else {
      DBG_PRINTF_NODE0(DBG_PROV, "** ignoring providers with %s",
                   mcmModeNames[mcmm]);
    }
  } else {
    ret = findProvGivenList(p_infoOut, infoIn,
                            accept_RxD_provs, accept_RxM_provs,
                            accept_sockets_provs, mcmm);
  }

  if (ret) {
    *p_modeOut = mcmm;
  }

  return ret;
}


static
enum mcmMode_t isMsgOrderProv(struct fi_info* info) {
  return (setCheckMsgOrderProv(info, false /*check*/) == info)
         ? mcmm_msgOrd
         : mcmm_undef;
}


static
struct fi_info* setCheckDlvrCmpltProv(struct fi_info* info,
                                      chpl_bool set) {
  uint64_t need_op_flags = FI_DELIVERY_COMPLETE;
  if (set) {
    info = fi_dupinfo(info);
    info->tx_attr->op_flags |= need_op_flags;
    return info;
  } else {
    return ((info->tx_attr->op_flags & need_op_flags) == need_op_flags)
           ? info
           : NULL;
  }
}


static
chpl_bool findDlvrCmpltProv(struct fi_info** p_infoOut,
                            enum mcmMode_t* p_modeOut,
                            struct fi_info* infoIn,
                            chpl_bool inputIsHints) {
  //
  // Try to find a provider that supports delivery-complete.
  //
  const char* prov_name = getProviderName();
  const chpl_bool accept_RxD_provs = isInProvName("ofi_rxd", prov_name);
  const chpl_bool accept_RxM_provs = isInProvName("ofi_rxm", prov_name);
  const chpl_bool accept_sockets_provs = isInProvName("sockets", prov_name);
  enum mcmMode_t mcmm = mcmm_dlvrCmplt;
  chpl_bool ret = false;

  if (inputIsHints) {
    struct fi_info* infoAdj = setCheckDlvrCmpltProv(infoIn, true /*set*/);
    if (infoAdj) {
      ret = findProvGivenHints(p_infoOut, infoAdj,
                              accept_RxD_provs, accept_RxM_provs,
                              accept_sockets_provs, mcmm);
      fi_freeinfo(infoAdj);
    } else {
      DBG_PRINTF_NODE0(DBG_PROV, "** ignoring providers with %s",
                   mcmModeNames[mcmm]);
    }
  } else {
    ret = findProvGivenList(p_infoOut, infoIn,
                            accept_RxD_provs, accept_RxM_provs,
                            accept_sockets_provs, mcmm);
  }

  if (ret) {
    *p_modeOut = mcmm;
  }

  return ret;
}


static
enum mcmMode_t isDlvrCmpltProv(struct fi_info* info) {
  return (setCheckDlvrCmpltProv(info, false /*check*/) == info)
         ? mcmm_dlvrCmplt
         : mcmm_undef;
}


static
struct fi_info* getBaseProviderHints(chpl_bool* pTxAttrsForced);

static
void heedSlingshotSettings(struct fi_info* info);

static
void init_ofiFabricDomain(void) {
  //
  // Get hints describing our base requirements, the ones that are
  // independent of which MCM conformance mode we'll eventually use.
  //
  chpl_bool txAttrsForced;
  struct fi_info* hints = getBaseProviderHints(&txAttrsForced);

  DBG_PRINTF_NODE0(DBG_PROV_HINTS,
                   "====================\n"
                   "initial hints");
  DBG_PRINTF_NODE0(DBG_PROV_HINTS,
                   "%s", fi_tostr(hints, FI_TYPE_INFO));
  DBG_PRINTF_NODE0(DBG_PROV_HINTS,
                   "====================");

  //
  // Look for a provider that can do one of our message ordering sets
  // or one that can do delivery-complete.  We can't just get all the
  // providers that match our fundamental needs and then look through
  // the list to find one that can do what we want, though, because
  // capabilities that aren't in our hints might not be expressed by
  // any returned provider.  (Providers will not typically volunteer
  // capabilities that aren't asked for, especially capabilities that
  // have performance costs.)  So here, first see if we get a "good"
  // core provider using the various hint sets and if that doesn't
  // succeed, settle for a not-so-good provider.  "Good" here means
  // "neither tcp nor sockets".
  //
  // There are some wrinkles:
  // - Setting either the transaction orderings or the completion type
  //   in manually overridden hints causes those hints to be used as-is,
  //   turning off both the good-provider check and any attempt to find
  //   something sufficient for the MCM.
  // - Setting the FI_PROVIDER environment variable to manually specify
  //   a provider turns off the good-provider checks.
  // - We can't accept the RxM utility provider with any core provider
  //   for delivery-complete, because although RxM will match that it
  //   cannot actually do it, and programs will fail.  This is a known
  //   bug that can't be fixed without breaking other things:
  //     https://github.com/ofiwg/libfabric/issues/5601
  //   Explicitly including ofi_rxm in FI_PROVIDER overrides this.
  // - The sockets provider is deprecated. It is only used if it is
  //   specified via the FI_PROVIDER environment variable.
  //

  mcmMode = mcmm_undef;

  //
  // Take manually overridden hints as forcing provider selection if
  // they adjust either the transaction orderings or completion type.
  // Otherwise, just flow those overrides into the selection process
  // below.
  //
  if (txAttrsForced) {
    int ret;
    OFI_CHK_2(fi_getinfo(COMM_OFI_FI_VERSION, NULL, NULL, 0, hints, &ofi_info),
              ret, -FI_ENODATA);
    if (ret != FI_SUCCESS) {
      const char* prov_name = getProviderName();
      INTERNAL_ERROR_V_NODE0("No (forced) provider for prov_name \"%s\"",
                             (prov_name == NULL) ? "<any>" : prov_name);
    }
  }

  struct {
    fnFindProv_t* fnFind;
    fnIsProv_t* fnIs;
    struct fi_info* infoList;
  } capTry[] = { { findMsgOrderFenceProv, isMsgOrderFenceProv, NULL },
                 { findMsgOrderProv, isMsgOrderProv, NULL },
                 { findDlvrCmpltProv, isDlvrCmpltProv, NULL }, };
  size_t capTryLen = sizeof(capTry) / sizeof(capTry[0]);

  if (ofi_info == NULL) {
    // Search for a good provider.
    for (int i = 0; ofi_info == NULL && i < capTryLen; i++) {
      struct fi_info* info = NULL;
      enum mcmMode_t mcmm;
      if ((*capTry[i].fnFind)(&info, &mcmm, hints, true /*inputIsHints*/)) {
        ofi_info = info;
        mcmMode = mcmm;
      } else {
        capTry[i].infoList = info;
      }
    }

    // If necessary, search for a less-good provider.
    for (int i = 0; ofi_info == NULL && i < capTryLen; i++) {
      (void) (*capTry[i].fnFind)(&ofi_info, &mcmMode, capTry[i].infoList,
                                 false /*inputIsHints*/);
    }

    // ofi_info has the result, if any.  Free intermediate list(s).
    for (int i = 0; i < capTryLen && capTry[i].infoList != NULL; i++) {
      fi_freeinfo(capTry[i].infoList);
    }
  } else {
    //
    // The capability set, and provider, were forced upon us.  Figure out
    // what capability set we'll be using.
    //
    for (int i = 0; mcmMode == mcmm_undef && i < capTryLen; i++) {
      mcmMode = (*capTry[i].fnIs)(ofi_info);
    }

    if (mcmMode == mcmm_undef) {
      chpl_warning("Forced provider has unknown capability set", 0, 0);
    }
  }

  if (ofi_info == NULL) {
    //
    // We didn't find any provider at all.
    // NOTE: execution ends here.
    //
    const char* prov_name = getProviderName();
    INTERNAL_ERROR_V_NODE0("No libfabric provider for prov_name \"%s\"",
                           (prov_name == NULL) ? "<any>" : prov_name);
  }

  //
  // If we get here, we have a provider in ofi_info.
  //
  fi_freeinfo(hints);

  if (DBG_TEST_MASK(DBG_PROV_ALL)) {
    if (chpl_nodeID == 0) {
      DBG_PRINTF(DBG_PROV_ALL,
                 "====================\n"
                 "matched fabric(s):");
      struct fi_info* info;
      for (info = ofi_info; info != NULL; info = info->next) {
        DBG_PRINTF(DBG_PROV_ALL, "%s", fi_tostr(ofi_info, FI_TYPE_INFO));
      }
    }
  } else {
    DBG_PRINTF_NODE0(DBG_PROV,
                     "====================\n"
                     "matched fabric:");
    DBG_PRINTF_NODE0(DBG_PROV, "%s", fi_tostr(ofi_info, FI_TYPE_INFO));
  }

  DBG_PRINTF_NODE0(DBG_PROV | DBG_PROV_ALL,
                   "====================");

  //
  // Create the fabric domain and associated fabric access domain.
  //
  OFI_CHK(fi_fabric(ofi_info->fabric_attr, &ofi_fabric, NULL));

  if (strcmp(CHPL_TARGET_PLATFORM, "hpe-cray-ex") == 0
      && chpl_env_rt_get_bool("COMM_OFI_SLINGSHOT_CHECK_ENV", true)) {
    heedSlingshotSettings(ofi_info);
  }

  OFI_CHK(fi_domain(ofi_fabric, ofi_info, &ofi_domain, NULL));
}


//
// Reserve cores for the AM handler(s).
//
static
void init_ofiReserveCores() {
  for (int i = 0; i < numAmHandlers; i++) {
    reservedCPUs[i] = envUseDedicatedAmhCores ?
      chpl_topo_reserveCPUPhysical() : -1;
  }
}


static
struct fi_info* getBaseProviderHints(chpl_bool* pTxAttrsForced) {
  //
  // Build hints describing our base requirements, the ones that are
  // independent of which MCM conformance mode we'll eventually use:
  // - capabilities:
  //   - messaging (send/receive), including multi-receive
  //   - RMA
  //   - transactions directed at both self and remote nodes
  //   - on Cray XC, atomics (gni provider doesn't volunteer this)
  // - tx endpoints:
  //   - completion events
  //   - send-after-send ordering
  // - rx endpoint ordering same as tx
  // - RDM endpoints
  // - "domain" threading model, since we manage thread contention ourselves
  // - resource management, to improve the odds we hear about exhaustion
  // - table-style address vectors
  // - the memory registration modes we can support
  //
  const char* prov_name = getProviderName();
  struct fi_info* hints;
  CHK_TRUE((hints = fi_allocinfo()) != NULL);

  hints->caps = (FI_MSG | FI_MULTI_RECV
                 | FI_RMA | FI_LOCAL_COMM | FI_REMOTE_COMM);
  if ((strcmp(CHPL_TARGET_PLATFORM, "cray-xc") == 0
       && (prov_name == NULL || isInProvName("gni", prov_name)))
      || chpl_env_rt_get_bool("COMM_OFI_HINTS_CAPS_ATOMIC", false)) {
    hints->caps |= FI_ATOMIC;
  }

  hints->tx_attr->op_flags = FI_COMPLETION;
  hints->tx_attr->msg_order = FI_ORDER_SAS;

  hints->rx_attr->msg_order = hints->tx_attr->msg_order;

  hints->ep_attr->type = FI_EP_RDM;

  hints->domain_attr->threading = FI_THREAD_DOMAIN;
  hints->domain_attr->resource_mgmt = FI_RM_ENABLED;
  hints->domain_attr->av_type = FI_AV_TABLE;
  hints->domain_attr->mr_mode = (  FI_MR_LOCAL
                                 | FI_MR_VIRT_ADDR
                                 | FI_MR_PROV_KEY // TODO: avoid pkey bcast?
                                 | FI_MR_ENDPOINT);

  // Set FI_MR_ALLOCATED if there is more than one node and the maximum
  // heap size was specified and the CHPL_RT_OVERSUBSCRIBED environment
  // variable was not set, otherwise we risk running out of memory when
  // the heap is allocated. If CHPL_RT_OVERSUBSCRIBED is set then we may
  // be sharing the node with other processes, in which case we can't size
  // the fixed heap correctly.

  if (chpl_numNodes > 1 && envMaxHeapSize != 0 && !envOversubscribed) {
    hints->domain_attr->mr_mode |= FI_MR_ALLOCATED;
  }

  *pTxAttrsForced = false;

#ifdef CHPL_COMM_DEBUG
  struct fi_info* hintsOrig = fi_dupinfo(hints);
  debugOverrideHints(hints);
  *pTxAttrsForced =
    (hints->tx_attr->op_flags == hintsOrig->tx_attr->op_flags
     && hints->tx_attr->msg_order == hintsOrig->tx_attr->msg_order)
    ? false
    : true;
  fi_freeinfo(hintsOrig);
#endif

  return hints;
}


static
void heedSlingshotSettings(struct fi_info* info) {
  //
  // Deal with Slingshot network related settings.
  //
  const char* evDevs = getenv("SLINGSHOT_DEVICES");
  const char* evSvcIds = getenv("SLINGSHOT_SVC_IDS");
  const char* evTcs = getenv("SLINGSHOT_TCS");
  const char* evVnis = getenv("SLINGSHOT_VNIS");

  DBG_PRINTF(DBG_SLINGSHOT,
             "SLINGSHOT_DEVICES %s%s%s, "
             "SVC_IDS %s%s%s, "
             "TCS %s%s%s, "
             "VNIS %s%s%s",
             (evDevs == NULL) ? "" : "\"",
             (evDevs == NULL) ? "not set" : evDevs,
             (evDevs == NULL) ? "" : "\"",
             (evSvcIds == NULL) ? "" : "\"",
             (evSvcIds == NULL) ? "not set" : evSvcIds,
             (evSvcIds == NULL) ? "" : "\"",
             (evTcs == NULL) ? "" : "\"",
             (evTcs == NULL) ? "not set" : evTcs,
             (evTcs == NULL) ? "" : "\"",
             (evVnis == NULL) ? "" : "\"",
             (evVnis == NULL) ? "not set" : evVnis,
             (evVnis == NULL) ? "" : "\"");

  CHK_TRUE((evDevs == NULL) == (evSvcIds == NULL)
           && (evSvcIds == NULL) == (evTcs == NULL)
           && (evTcs == NULL) == (evVnis == NULL)); // sanity
  if (evDevs == NULL) {
    return;
  }

  //
  // Libfabric allocates and frees struct fi_info structures and their
  // substructures with the system allocator.  Therefore, that instead
  // of the Chapel allocator for this auth_key since the pointer to it
  // will be assigned into that struct.
  //
  struct ss_auth_key {
    uint32_t svc_id;
    uint16_t vni;
  };
  struct ss_auth_key* auth_key;
  CHK_SYS_MALLOC(auth_key, 1);

  //
  // Service ID.  If there are more than one, then we have access to
  // more than one interface.  But we only support one anyway and our
  // domain logic will have found the first of them, so just take the
  // first service ID.
  //
  {
    char ev[strlen(evSvcIds) + 1];  // non-constant, for strtok()
    strcpy(ev, evSvcIds);

    char* tok;
    char* lasts;
    CHK_TRUE((tok = strtok_r(ev, ",", &lasts)) != NULL);
    CHK_TRUE(sscanf(tok, "%" SCNu32, &auth_key->svc_id) == 1);
    DBG_PRINTF(DBG_SLINGSHOT, "Slingshot svc_id %" PRIu32, auth_key->svc_id);
  }

  //
  // VNI.  Similarly to the service ID case, if there are more than one,
  // just take the first.
  //
  {
    char ev[strlen(evVnis) + 1];  // non-constant, for strtok()
    strcpy(ev, evVnis);

    char* tok;
    char* lasts;
    CHK_TRUE((tok = strtok_r(ev, ",", &lasts)) != NULL);

    char ev2[strlen(tok) + 1];
    strcpy(ev2, tok);

    char* tok2;
    char* lasts2;
    CHK_TRUE((tok2 = strtok_r(ev2, ":", &lasts2)) != NULL);
    CHK_TRUE(sscanf(tok2, "%" SCNu16, &auth_key->vni) == 1);
    DBG_PRINTF(DBG_SLINGSHOT, "Slingshot VNI %" PRIu16, auth_key->vni);
  }

  info->domain_attr->auth_key = (void*) auth_key;
  info->domain_attr->auth_key_size = sizeof(*auth_key);
}


static
size_t get_hugepageSize(void);

static
void init_ofiDoProviderChecks(void) {
  //
  // Set/compute various provider-specific things.
  //
  if (providerInUse(provType_gni)) {
    //
    // gni (Cray XC)
    //
    // - Warn if they don't use hugepages.
    // - Warn if the fixed heap size is larger than what will fit in the
    //   TLB cache.  While that may reduce performance it won't affect
    //   function, though, so don't do anything dramatic like reducing
    //   the size to fit.
    //
    size_t page_size = get_hugepageSize();
    if (page_size == 0) {
      if (chpl_nodeID == 0) {
        chpl_warning_explicit("not using hugepages may reduce performance",
                              __LINE__, __FILE__);
      }
      page_size = chpl_getSysPageSize();
    }

    const size_t nic_TLB_cache_pages = 512; // not publicly defined
    const size_t nic_mem_map_limit = nic_TLB_cache_pages * page_size;
    void* start;
    size_t size;
    chpl_comm_impl_regMemHeapInfo(&start, &size);
    if (size > nic_mem_map_limit) {
      if (chpl_nodeID == 0) {
        size_t page_size = chpl_comm_impl_regMemHeapPageSize();
        char buf1[20], buf2[20], buf3[20], msg[200];
        (void) snprintf(msg, sizeof(msg),
                        "Aries TLB cache can cover %s with %s pages; "
                        "with %s heap,\n"
                        "         cache refills may reduce performance",
                        chpl_snprintf_KMG_z(buf1, sizeof(buf1),
                                            nic_mem_map_limit),
                        chpl_snprintf_KMG_z(buf2, sizeof(buf2), page_size),
                        chpl_snprintf_KMG_f(buf3, sizeof(buf3), size));
        chpl_warning(msg, 0, 0);
      }
    }
  }

  if (providerInUse(provType_rxd)) {
    //
    // ofi_rxd (utility provider with tcp, verbs, possibly others)
    //
    // - Based on tracebacks after internal error aborts, RxD seems to
    //   want to record an address per accessing endpoint for at least
    //   some AVs (perhaps just those for which it handles progress?).
    //   It uses the AV attribute 'count' member to size the data
    //   structure in which it stores those.  So, that member will need
    //   to account for all transmitting endpoints.
    //
    provCtl_sizeAvsByNumEps = true;
  }

  //
  // RxD and perhaps other providers must have a non-NULL buf arg for
  // fi_fetch_atomic(FI_ATOMIC_READ) or they segfault, even though the
  // fi_atomic man page says buf is ignored for that operation and may
  // be NULL.
  //
  provCtl_readAmoNeedsOpnd = true;
}


static
void init_ofiEp(void) {
  //
  // The AM handler is responsible not only for AM handling and progress
  // on any RMA it initiates but also progress on inbound RMA, if that
  // is needed.  It uses poll and wait sets to manage this, if it can.
  // Note: we'll either have both a poll and a wait set, or neither.
  //
  // We don't use poll and wait sets with the efa provider because that
  // doesn't support wait objects.  I tried just setting the cq_attr
  // wait object to FI_WAIT_UNSPEC for all providers, since we don't
  // reference the wait object explicitly anyway, but then saw hangs
  // with (at least) the tcp;ofi_rxm provider.
  //
  // We don't use poll and wait sets with the gni provider because (1)
  // it returns -ENOSYS for fi_poll_open() and (2) although a wait set
  // seems to work properly during execution, we haven't found a way to
  // avoid getting -FI_EBUSY when we try to close it.
  //
  // We don't use poll and wait sets on macOS because the underlying
  // libfabric support needs epoll, which is from Linux rather than
  // POSIX and thus not present on macOS.  Unfortunately, libfabric has
  // not (yet?) worked around the lack of epoll because macOS is rather
  // a secondary platform.  One can find comments in the libfabric issue
  // https://github.com/ofiwg/libfabric/issues/5453 that provide some
  // background, though that issue is for an unrelated problem.
  //
  if (!providerInUse(provType_efa)
      && !providerInUse(provType_gni)
      && strcmp(CHPL_TARGET_PLATFORM, "darwin") != 0) {
    int ret;
    struct fi_poll_attr pollSetAttr = (struct fi_poll_attr)
                                      { .flags = 0, };
    OFI_CHK_2(fi_poll_open(ofi_domain, &pollSetAttr, &ofi_amhPollSet),
              ret, -FI_ENOSYS);
    if (ret == FI_SUCCESS) {
      struct fi_wait_attr waitSetAttr = (struct fi_wait_attr)
                                        { .wait_obj = FI_WAIT_UNSPEC, };
      OFI_CHK_2(fi_wait_open(ofi_fabric, &waitSetAttr, &ofi_amhWaitSet),
                ret, -FI_ENOSYS);
      if (ret != FI_SUCCESS) {
        ofi_amhPollSet = NULL;
        ofi_amhWaitSet = NULL;
      }
    } else {
      ofi_amhPollSet = NULL;
    }
  }

  //
  // Compute numbers of transmit and receive contexts, and then create
  // the transmit context table.
  //
  tciTabBindTxCtxs = canBindTxCtxs(ofi_info);
  if (tciTabBindTxCtxs) {
    numTxCtxs = chpl_task_getFixedNumThreads() + numAmHandlers + 1;
  } else {
    numTxCtxs = chpl_task_getMaxPar() + numAmHandlers + 1;
  }
  DBG_PRINTF(DBG_CFG,"tciTabBindTxCtxs %s numTxCtxs %d numAmHandlers %d",
             tciTabBindTxCtxs ? "true" : "false", numTxCtxs, numAmHandlers);
  const chpl_bool useScalEp = envPreferScalableTxEp
                              && ofi_info->domain_attr->max_ep_tx_ctx > 1;
  if (useScalEp) {
    ofi_info->ep_attr->tx_ctx_cnt = numTxCtxs;
  }

  CHK_TRUE(ofi_info->domain_attr->max_ep_rx_ctx >= numAmHandlers);
  numRxCtxs = numAmHandlers;

  tciTabLen = numTxCtxs;
  CHK_TRUE(tciTabLen > 0);
  CHPL_CALLOC(tciTab, tciTabLen);

  // Use "hybrid" MR mode for the cxi provider if it's available

  if (providerInUse(provType_cxi)) {
    if (envUseCxiHybridMR) {
#ifdef FI_CXI_DOM_OPS_3
      struct fi_cxi_dom_ops *dom_ops;
      int rc;
      rc = fi_open_ops(&ofi_domain->fid, FI_CXI_DOM_OPS_3, 0,
                          (void **)&dom_ops, NULL);
      if (rc == FI_SUCCESS) {
        rc = dom_ops->enable_hybrid_mr_desc(&ofi_domain->fid, true);
        if (rc == FI_SUCCESS) {
          cxiHybridMRMode = true;
        } else {
          DBG_PRINTF(DBG_PROV, "enable_hybrid_mr_desc failed: %s",
                     fi_strerror(rc));
        }
      } else {
        DBG_PRINTF(DBG_PROV, "fi_open_ops failed: %s", fi_strerror(rc));
      }
#endif
    }
    if (cxiHybridMRMode) {
      DBG_PRINTF(DBG_PROV, "cxi hybrid MR mode enabled");
    } else {
      DBG_PRINTF(DBG_PROV, "cxi hybrid MR mode disabled");
    }
  }

  //
  // Create transmit contexts.
  //

  //
  // For the CQ lengths, allow for whichever maxOutstanding (AMs or
  // RMAs) value is larger, plus quite a few for AM responses because
  // the network round-trip latency ought to be quite a bit more than
  // our AM handling time, so we want to be able to have many responses
  // in flight at once.
  //
  struct fi_av_attr avAttr = (struct fi_av_attr)
                             { .type = FI_AV_TABLE,
                               .count = chpl_numNodes * 2 /* AM, RMA+AMO */,
                               .name = NULL,
                               .rx_ctx_bits = 0, };
  if (provCtl_sizeAvsByNumEps) {
    // Workaround for RxD peculiarity.
    avAttr.count *= numTxCtxs;
  }

  // If possible, share address vectors to reduce overhead. The EFA
  // provider does not allow AVs to be shared among endpoints. As a
  // result, if we are not using the EFA provider then the same AV
  // (ofi_av) is shared by all endpoints. If we are using the EFA provider
  // then the receive endpoint has its own AV in ofi_rxAV. For the
  // transmit endpoints it depends on whether or not we are also using a
  // scalable endpoint. If so, all of the contexts for the endpoint share
  // ofi_av. Otherwise each  has its own AV (created in
  // init_ofiEpTxCtx).

  chpl_bool useSharedAv = !providerInUse(provType_efa);
  if (useSharedAv) {
    DBG_PRINTF(DBG_TCIPS, "using shared AV");
    //
    // all endpoints share ofi_av
    //
    OFI_CHK(fi_av_open(ofi_domain, &avAttr, &ofi_av, NULL));
    ofi_rxAv = ofi_av;
  } else {
    DBG_PRINTF(DBG_TCIPS, "using individual AVs");
    if (useScalEp) {
      //
      // all scalable EP contexts share ofi_av
      //
      DBG_PRINTF(DBG_TCIPS, "using scalable EP");
      OFI_CHK(fi_av_open(ofi_domain, &avAttr, &ofi_av, NULL));
    } else {
      //
      // each endpoint gets its own AV in init_ofiEpTxCtx below
      //
    }
    //
    // receive endpoint has its own AV
    //
    OFI_CHK(fi_av_open(ofi_domain, &avAttr, &ofi_rxAv, NULL));
  }

  if (useScalEp) {
    OFI_CHK(fi_scalable_ep(ofi_domain, ofi_info, &ofi_txEpScal, NULL));
    OFI_CHK(fi_scalable_ep_bind(ofi_txEpScal, &ofi_av->fid, 0));
  } else {
    //
    // Use regular transmit endpoints; see below.
    //
  }

  const int numWorkerTxCtxs = tciTabLen - numAmHandlers;
  struct fi_cq_attr cqAttr;
  struct fi_cntr_attr cntrAttr;

  {
    cqAttr = (struct fi_cq_attr)
             { .format = FI_CQ_FORMAT_MSG,
               .size = 100 + MAX_TXNS_IN_FLIGHT,
               .wait_obj = FI_WAIT_NONE, };
    txCQLen = cqAttr.size;
    cntrAttr = (struct fi_cntr_attr)
               { .events = FI_CNTR_EVENTS_COMP,
                 .wait_obj = FI_WAIT_UNSPEC, };
    DBG_PRINTF(DBG_TCIPS, "creating tx endpoints/contexts");
    for (int i = 0; i < numWorkerTxCtxs; i++) {
      init_ofiEpTxCtx(i, false /*isAMHandler*/, &avAttr, &cqAttr,
                      envUseTxCntr ? &cntrAttr : NULL);
    }
  }

  //
  // TX contexts for the AM handler(s) can just use counters, if the
  // provider supports them.  Otherwise, they have to use CQs also.
  //
  const enum fi_wait_obj waitObj = (ofi_amhWaitSet == NULL)
                                   ? FI_WAIT_NONE
                                   : FI_WAIT_SET;

  cqAttr = (struct fi_cq_attr)
           { .format = FI_CQ_FORMAT_MSG,
             .size = 100,
             .wait_obj = waitObj,
             .wait_cond = FI_CQ_COND_NONE,
             .wait_set = ofi_amhWaitSet, };
  cntrAttr = (struct fi_cntr_attr)
             { .events = FI_CNTR_EVENTS_COMP,
               .wait_obj = FI_WAIT_UNSPEC,
               .wait_set = ofi_amhWaitSet, };
  DBG_PRINTF(DBG_TCIPS, "creating AM handler tx endpoints/contexts");
  for (int i = numWorkerTxCtxs; i < tciTabLen; i++) {
    init_ofiEpTxCtx(i, true /*isAMHandler*/, &avAttr, &cqAttr,
                    envUseAmTxCntr ? &cntrAttr : NULL);
  }

  //
  // Create receive contexts.
  //
  // For the CQ length, allow for an appreciable proportion of the job
  // to send requests to us at once.
  //
  cqAttr = (struct fi_cq_attr)
           { .size = chpl_numNodes * numWorkerTxCtxs,
             .format = FI_CQ_FORMAT_DATA,
             .wait_obj = waitObj,
             .wait_cond = FI_CQ_COND_NONE,
             .wait_set = ofi_amhWaitSet, };
  cntrAttr = (struct fi_cntr_attr)
             { .events = FI_CNTR_EVENTS_COMP,
               .wait_obj = FI_WAIT_UNSPEC,
               .wait_set = ofi_amhWaitSet, };

  OFI_CHK(fi_endpoint(ofi_domain, ofi_info, &ofi_rxEp, NULL));
  OFI_CHK(fi_ep_bind(ofi_rxEp, &ofi_rxAv->fid, 0));
  OFI_CHK(fi_cq_open(ofi_domain, &cqAttr, &ofi_rxCQ, &ofi_rxCQ));
  int cqFlags = FI_TRANSMIT | FI_RECV;
  if (envUseAmRxCntr) {
    DBG_PRINTF(DBG_TCIPS, "AM handler using rx completion counter");
    OFI_CHK(fi_cntr_open(ofi_domain, &cntrAttr, &ofi_rxCntr, &ofi_rxCntr));
    OFI_CHK(fi_ep_bind(ofi_rxEp, &ofi_rxCntr->fid, FI_RECV));
    cqFlags |= FI_SELECTIVE_COMPLETION;
  } else {
    DBG_PRINTF(DBG_TCIPS, "AM handler using rx completion queue");
  }

  OFI_CHK(fi_ep_bind(ofi_rxEp, &ofi_rxCQ->fid, cqFlags));

  OFI_CHK(fi_enable(ofi_rxEp));

  //
  // If we're using poll and wait sets, put all the progress-related
  // CQs and/or counters in the poll set.
  //
  if (ofi_amhPollSet != NULL) {
    OFI_CHK(fi_poll_add(ofi_amhPollSet, &ofi_rxCQ->fid, 0));
    if (ofi_rxCntr != NULL) {
      OFI_CHK(fi_poll_add(ofi_amhPollSet, &ofi_rxCntr->fid, 0));
    }
    OFI_CHK(fi_poll_add(ofi_amhPollSet, tciTab[tciTabLen - 1].txCmplFid, 0));
    pollSetSize = 3;
  }
}


static
void init_ofiEpTxCtx(int i, chpl_bool isAMHandler,
                     struct fi_av_attr* avAttr,
                     struct fi_cq_attr* cqAttr,
                     struct fi_cntr_attr* cntrAttr) {
  struct perTxCtxInfo_t* tcip = &tciTab[i];
  atomic_init_bool(&tcip->allocated, false);
  tcip->bound = false;

  if (ofi_txEpScal == NULL) {
    //
    // not using a scalable endpoint
    //
    if (ofi_av == NULL) {
      //
      // create an AV for this endpoint
      //
      OFI_CHK(fi_av_open(ofi_domain, avAttr, &tcip->av, NULL));
    } else {
      //
      // use the shared AV
      //
      tcip->av = ofi_av;
    }
    OFI_CHK(fi_endpoint(ofi_domain, ofi_info, &tcip->txCtx, NULL));
    OFI_CHK(fi_ep_bind(tcip->txCtx, &tcip->av->fid, 0));
  } else {
    //
    // using a scalable endpoint. Share ofi_av among its contexts.
    //
    tcip->av = ofi_av;
    OFI_CHK(fi_tx_context(ofi_txEpScal, i, NULL, &tcip->txCtx, NULL));
  }

  uint64_t cqFlags = FI_TRANSMIT | FI_RECV;
  if (cntrAttr != NULL) {
    DBG_PRINTF(DBG_TCIPS, "using transmit completion counters");
    OFI_CHK(fi_cntr_open(ofi_domain, cntrAttr, &tcip->txCntr,
                         &tcip->checkTxCmplsFn));
    tcip->txCmplFid = &tcip->txCntr->fid;
    OFI_CHK(fi_ep_bind(tcip->txCtx, tcip->txCmplFid,
                       FI_READ | FI_SEND | FI_WRITE));
    tcip->checkTxCmplsFn = checkTxCmplsCntr;
    cqFlags |= FI_SELECTIVE_COMPLETION;
  } else {
    DBG_PRINTF(DBG_TCIPS, "using transmit completion queues");
  }

  if (cqAttr != NULL) {
    OFI_CHK(fi_cq_open(ofi_domain, cqAttr, &tcip->txCQ,
                       &tcip->checkTxCmplsFn));
    tcip->txCmplFid = &tcip->txCQ->fid;
    OFI_CHK(fi_ep_bind(tcip->txCtx, tcip->txCmplFid, cqFlags));
    if (cntrAttr == NULL) {
      tcip->checkTxCmplsFn = checkTxCmplsCQ;
    }
  }

  OFI_CHK(fi_enable(tcip->txCtx));

  tcip->ensureProgressFn = isAMHandler
                           ? amEnsureProgress
                           : tcip->checkTxCmplsFn;

}

static
void insertAddrs(struct fid_av* av, char *addrs, size_t numAddrs,
                 fi_addr_t **fi_addrs_p) {
  fi_addr_t *fi_addrs;
  CHPL_CALLOC(fi_addrs, numAddrs);
  CHK_TRUE(fi_av_insert(av, addrs, numAddrs, fi_addrs, 0, NULL) ==
           numAddrs);
  *fi_addrs_p = fi_addrs;
}


static
void init_ofiExchangeAvInfo(void) {
  //
  // Exchange addresses with the rest of the nodes.
  //

  //
  // Get everybody else's address.
  // Note: this assumes all addresses, job-wide, are the same length.
  //
  if (DBG_TEST_MASK(DBG_CFG_AV)) {
    //
    // Sanity-check our same-address-length assumption.
    //
    size_t len = 0;
    OFI_CHK_1(fi_getname(&ofi_rxEp->fid, NULL, &len), -FI_ETOOSMALL);

    size_t* lens;
    CHPL_CALLOC(lens, chpl_numNodes);
    chpl_comm_ofi_oob_allgather(&len, lens, sizeof(len));
    if (chpl_nodeID == 0) {
      for (int i = 0; i < chpl_numNodes; i++) {
        CHK_TRUE(lens[i] == len);
      }
    }
  }

  char* my_addr;
  char* addrs;
  size_t my_addr_len = 0;

  OFI_CHK_1(fi_getname(&ofi_rxEp->fid, NULL, &my_addr_len), -FI_ETOOSMALL);

  CHPL_CALLOC_SZ(my_addr, my_addr_len, 1);
  OFI_CHK(fi_getname(&ofi_rxEp->fid, my_addr, &my_addr_len));
  CHPL_CALLOC_SZ(addrs, chpl_numNodes, my_addr_len);
  if (DBG_TEST_MASK(DBG_CFG_AV)) {
    char nameBuf[128];
    size_t nameLen;
    nameLen = sizeof(nameBuf);
    (void) fi_av_straddr(ofi_rxAv, my_addr, nameBuf, &nameLen);
    DBG_PRINTF(DBG_CFG_AV, "my_addrs: %.*s%s",
               (int) nameLen, nameBuf,
               (nameLen <= sizeof(nameBuf)) ? "" : "[...]");
  }
  chpl_comm_ofi_oob_allgather(my_addr, addrs, my_addr_len);

  //
  // Insert the addresses into the address vector and build up a vector
  // of remote receive endpoints.
  //
  // All the transmit context table entries have address vectors and we
  // always use the one associated with our tx context.  But if we have
  // a scalable endpoint then all of those AVs are really the same one.
  // Only when the provider cannot support scalable EPs and we have
  // multiple actual endpoints are the AVs individualized to those.
  //
  size_t numAddrs = chpl_numNodes;
  if (ofi_av != NULL) {
    insertAddrs(ofi_av, addrs, numAddrs, &ofi_addrs);
  }
  if (ofi_rxAv != ofi_av) {
    insertAddrs(ofi_rxAv, addrs, numAddrs, &ofi_rxAddrs);
  }
  for (int i = 0; i < tciTabLen; i++) {
    if (ofi_av != NULL) {
      tciTab[i].av = ofi_av;
      tciTab[i].addrs = ofi_addrs;
    } else {
      insertAddrs(tciTab[i].av, addrs, numAddrs, &tciTab[i].addrs);
    }
    assert(tciTab[i].av != NULL);
    assert(tciTab[i].addrs != NULL);
  }
  CHPL_FREE(my_addr);
  CHPL_FREE(addrs);
}


static void findMoreMemoryRegions(void);

static
void init_ofiForMem(void) {
  void* fixedHeapStart;
  size_t fixedHeapSize;
  chpl_comm_impl_regMemHeapInfo(&fixedHeapStart, &fixedHeapSize);

  //
  // We default to scalable registration if none of the settings that
  // force basic registration are present, but the user can override
  // that by specifying use of a fixed heap.  Note that this is to
  // some extent just a backstop, because if the user does specify a
  // fixed heap we will have earlier included FI_MR_ALLOCATED in our
  // hints, which might well have caused the selection of a provider
  // which requires basic registration.
  //
  const uint64_t basicMemRegBits = (FI_MR_BASIC
                                    | FI_MR_LOCAL
                                    | FI_MR_VIRT_ADDR
                                    | FI_MR_ALLOCATED
                                    | FI_MR_PROV_KEY);
  scalableMemReg = ((ofi_info->domain_attr->mr_mode & basicMemRegBits) == 0
                    && fixedHeapSize == 0);

  //
  // With scalable memory registration we just register the whole
  // address space here; with non-scalable we register each region
  // individually.  Currently with non-scalable we actually only
  // register a fixed heap.  We may do something more complicated
  // in the future, though.
  //
  if (scalableMemReg) {
    memTab[0].addr = (void*) 0;
    memTab[0].base = 0;
    memTab[0].size = SIZE_MAX;
    memTabCount = 1;
  } else {
    if (fixedHeapSize == 0) {
      INTERNAL_ERROR_V("must specify fixed heap with %s provider",
                       ofi_info->fabric_attr->prov_name);
    }

    memTab[0].addr = fixedHeapStart;
    memTab[0].base = 0;
    memTab[0].size = fixedHeapSize;
    memTabCount = 1;

    findMoreMemoryRegions();

    if ((ofi_info->domain_attr->mr_mode & FI_MR_VIRT_ADDR) == 0) {
      for (int i = 0; i < memTabCount; i++) {
        memTab[i].base = (uint64_t) memTab[i].addr;
      }
    }
  }

  const chpl_bool prov_key =
    ((ofi_info->domain_attr->mr_mode & FI_MR_PROV_KEY) != 0);

  uint64_t bufAcc = FI_RECV | FI_REMOTE_READ | FI_REMOTE_WRITE;

  if (((ofi_info->domain_attr->mr_mode & FI_MR_LOCAL) != 0) ||
       cxiHybridMRMode) {
    bufAcc |= FI_SEND | FI_READ | FI_WRITE;
  }

  for (int i = 0; i < memTabCount; i++) {
    DBG_PRINTF(DBG_MR, "[%d] fi_mr_reg(%p, %#zx, %#" PRIx64 ")",
               i, memTab[i].addr, memTab[i].size, bufAcc);
    OFI_CHK(fi_mr_reg(ofi_domain,
                      memTab[i].addr, memTab[i].size,
                      bufAcc, 0, (prov_key ? 0 : i), 0, &ofiMrTab[i], NULL));
    if ((ofi_info->domain_attr->mr_mode & FI_MR_ENDPOINT) != 0) {
      OFI_CHK(fi_mr_bind(ofiMrTab[i], &ofi_rxEp->fid, 0));
      OFI_CHK(fi_mr_enable(ofiMrTab[i]));
    }
    memTab[i].desc = fi_mr_desc(ofiMrTab[i]);
    memTab[i].key  = fi_mr_key(ofiMrTab[i]);
    DBG_PRINTF(DBG_MR, "[%d]     key %#" PRIx64 " %s", i, memTab[i].key,
               prov_key ? "(prov)" : "");
    CHK_TRUE(prov_key || memTab[i].key == i);
  }

  //
  // Unless we're doing scalable registration of the entire address
  // space, share the memory regions around the job.
  //
  if (!scalableMemReg) {
    CHPL_CALLOC(memTabMap, chpl_numNodes);
    chpl_comm_ofi_oob_allgather(&memTab, memTabMap, sizeof(memTabMap[0]));
  }
}


static
chpl_bool nextMemMapEntry(void** pAddr, size_t* pSize,
                          char* retPath, size_t retPathSize);

static
void findMoreMemoryRegions(void) {
  DBG_CATFILE(DBG_MEMMAP, "/proc/self/maps", NULL);

  //
  // Look through /proc/self/maps for memory regions we'd like to
  // register.  So far we just try to register the program's data
  // segment, heap, and stack.  To identify the first of those we
  // need our program's name, so get that first.
  //
  char progName[1000];
  ssize_t pnLen = readlink("/proc/self/exe", progName, sizeof(progName));
  if (pnLen > 0) {
    if (pnLen >= sizeof(progName)) {
      pnLen--;
    }
    progName[pnLen] = '\0';
  }

  void* addr;
  size_t size;
  char path[1000];
  while (memTabCount < memTabSize - 1
         && nextMemMapEntry(&addr, &size, path, sizeof(path))) {
    //
    // Record this memory region if we want it.  Don't record a region
    // more than once.
    //
    chpl_bool seen = false;
    for (int i = 0; i < memTabCount; i++) {
      if (addr == memTab[i].addr && size == memTab[i].size) {
        seen = true;
        break;
      }
    }

    if (!seen) {
      //
      // Matching the initialized data, heap, and stack is easy (first
      // three tests). The uninitialized .bss part of the static data
      // is assumed to have an empty path and directly follow something
      // we've already registered, namely the initialized data.
      //
      if ((pnLen > 0 && strcmp(path, progName) == 0)
          || strcmp(path, "[heap]") == 0
          || strcmp(path, "[stack]") == 0
          || (path[0] == '\0'
              && memTabCount > 0
              && (char*) addr == ((char*) memTab[memTabCount - 1].addr
                                  + memTab[memTabCount - 1].size))) {
        DBG_PRINTF(DBG_MR, "record mem map region: %p %#zx \"%s\"",
                   addr, size, path);
        memTab[memTabCount].addr = addr;
        memTab[memTabCount].size = size;
        memTabCount++;
      }
    }
  }
}


static
chpl_bool nextMemMapEntry(void** pAddr, size_t* pSize,
                          char* retPath, size_t retPathSize) {
  static FILE* f = NULL;

  if (f == NULL) {
    if ((f = fopen("/proc/self/maps", "r")) == NULL) {
      INTERNAL_ERROR_V("cannot fopen(\"/proc/self/maps\")");
    }
  }

  while (true) {
    uint64_t lo_addr;
    uint64_t hi_addr;
    char perms[5];
    int ch;

    int scn_cnt;
    scn_cnt = fscanf(f, "%" SCNx64 "-%" SCNx64 "%4s%*x%*x:%*x%*x", &lo_addr, &hi_addr, perms);
    if (scn_cnt == EOF) {
      break;
    } else if (scn_cnt != 3) {
      INTERNAL_ERROR_V("unrecognized /proc/self/maps line format");
    }

    //
    // Skip regions that are not read/write.
    //
    if (perms[0] != 'r' || perms[1] != 'w') {
      while ((ch = fgetc(f)) != EOF && ch != '\n')
        ;
      continue;
    }

    if (retPath == NULL) {
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

      int p_idx;
      for (p_idx = 0; ch != EOF && ch != '\n'; ch = fgetc(f)) {
        if (p_idx < retPathSize - 1) {
          retPath[p_idx++] = ch;
        }
      }
      retPath[p_idx] = '\0';
    }

    *pAddr = (void*) (intptr_t) lo_addr;
    *pSize  = hi_addr - lo_addr;
    return true;
  }

  (void) fclose(f);
  f = NULL;
  return false;
}


static int isAtomicValid(enum fi_datatype);

static
void init_ofiForRma(void) {
  //
  // We need to make an initial call to isAtomicValid() to let it
  // initialize its internals.  The datatype here doesn't matter.
  //
  (void) isAtomicValid(FI_INT32);
}


static void init_amHandling(void);

static
void init_ofiForAms(void) {
  //
  // Compute the amount of space we should allow for AM landing zones.
  // We should have enough that we needn't re-post the multi-receive
  // buffer more often than, say, every tenth of a second.  We know from
  // the Chapel performance/comm/low-level/many-to-one test that the
  // comm=ugni AM handler can handle just over 150k "fast" AM requests
  // in 0.1 sec.  Assuming an average AM request size of 256 bytes, a 40
  // MiB buffer is enough to give us the desired 0.1 sec lifetime before
  // it needs renewing.  We actually then split this in half and create
  // 2 half-sized buffers (see below), so reflect that here also.
  //
  const size_t amLZSize = ((size_t) 40 << 20) / 2;

  //
  // Set the minimum multi-receive buffer space.  Make it big enough to
  // hold a max-sized request from every potential sender, but no more
  // than 10% of the buffer size.  Some providers don't have fi_setopt()
  // for some ep types, so allow this to fail in that case.  But note
  // that if it does fail and we get overruns we'll die or, worse yet,
  // silently compute wrong results.
  //
  {
    size_t sz = chpl_numNodes * tciTabLen * sizeof(struct amRequest_execOn_t);
    if (sz > amLZSize / 10) {
        sz = amLZSize / 10;
    }
    int ret;
    OFI_CHK_2(fi_setopt(&ofi_rxEp->fid, FI_OPT_ENDPOINT,
                        FI_OPT_MIN_MULTI_RECV, &sz, sizeof(sz)),
              ret, -FI_ENOSYS);
    DBG_PRINTF(DBG_AM_BUF, "FI_OPT_MIN_MULTI_RECV %zd", sz);
  }

  //
  // Pre-post multi-receive buffer for inbound AM requests.  In reality
  // set up two of these and swap back and forth between them, to hedge
  // against receiving "buffer filled and released" events out of order
  // with respect to the messages stored within them.
  //
  CHPL_CALLOC_SZ(amLZs[0], 1, amLZSize);
  CHPL_CALLOC_SZ(amLZs[1], 1, amLZSize);

  ofi_iov_reqs[0] = (struct iovec) { .iov_base = amLZs[0],
                                     .iov_len = amLZSize, };
  ofi_iov_reqs[1] = (struct iovec) { .iov_base = amLZs[1],
                                     .iov_len = amLZSize, };
  ofi_msg_reqs[0] = (struct fi_msg) { .msg_iov = &ofi_iov_reqs[0],
                                      .desc = NULL,
                                      .iov_count = 1,
                                      .addr = FI_ADDR_UNSPEC,
                                      .context = txnTrkEncodeId(__LINE__),
                                      .data = 0x0, };
  ofi_msg_reqs[1] = (struct fi_msg) { .msg_iov = &ofi_iov_reqs[1],
                                      .desc = NULL,
                                      .iov_count = 1,
                                      .addr = FI_ADDR_UNSPEC,
                                      .context = txnTrkEncodeId(__LINE__),
                                      .data = 0x0, };
  ofi_rxCount = 0;
  ofi_msg_i = 0;
  ofi_rxBuffer = ofi_msg_reqs[0].msg_iov->iov_base;
  ofi_rxEnd = (void *) ((char *) ofi_rxBuffer +
              ofi_msg_reqs[0].msg_iov->iov_len);
  for (int i = 0; i < 2; i++) {
    memset(ofi_msg_reqs[i].msg_iov->iov_base, '\0',
           ofi_msg_reqs[i].msg_iov->iov_len);
    OFI_CHK(fi_recvmsg(ofi_rxEp, &ofi_msg_reqs[i], FI_MULTI_RECV));
    DBG_PRINTF(DBG_AM_BUF,
             "pre-post fi_recvmsg(AMLZs %p, len %#zx)",
              ofi_msg_reqs[i].msg_iov->iov_base,
              ofi_msg_reqs[i].msg_iov->iov_len);
  }

  init_amHandling();
}


static void amRequestNop(c_nodeid_t, chpl_bool, struct perTxCtxInfo_t*);

static
void init_ofiConnections(void) {
  //
  // With providers that do dynamic endpoint connection we have seen
  // fairly dramatic connection overheads under certain circumstances.
  // As an aid to performance analysis, here we allow for forcing the
  // endpoint connections to be established early, during startup,
  // rather than later during timed sections of user code.
  //
  if (!providerInUse(provType_tcp)
      && !providerInUse(provType_verbs)) {
    return;
  }

  if (!chpl_env_rt_get_bool("COMM_OFI_CONNECT_EAGERLY", false)) {
    return;
  }

  //
  // We do this by firing a no-op AM to every remote node from every
  // still-inactive tx context.  (In effect, this means from all tx
  // contexts that aren't already bound to AM handlers.)  At present we
  // use blocking AMs for this, but if needed we could delay blocking
  // until after the last one had been initiated.  One way or another,
  // we need to not return until after all the connections have been
  // established.
  //
  for (c_nodeid_t node = (chpl_nodeID + 1) % chpl_numNodes;
       node != chpl_nodeID;
       node = (node + 1) % chpl_numNodes) {
    for (int i = 0; i < tciTabLen; i++) {
      struct perTxCtxInfo_t* tcip = &tciTab[i];
      if (!tcip->bound) {
        CHK_TRUE(tciAllocTabEntry(tcip));
        while (tcip->txCntr == NULL && tcip->numTxnsOut >= txCQLen) {
          (*tcip->checkTxCmplsFn)(tcip);
        }
        amRequestNop(node, true /*blocking*/, tcip);
        tciFree(tcip);
      }
    }
  }
}


void chpl_comm_rollcall(void) {
  DBG_PRINTF(DBG_IFACE_SETUP, "%s()", __func__);

  // Initialize diags
  chpl_comm_diags_init();

  chpl_msg(2, "executing on node %d of %d node(s): %s\n", chpl_nodeID,
           chpl_numNodes, chpl_nodeName());

  //
  // Only node 0 in multi-node programs does liveness checks, and only
  // after we're sure all the other nodes' AM handlers are running. By default
  // we only do liveness checks with the rxm utility provider to turn hangs
  // into hard failures.
  //
  bool envLivenessChecks = chpl_env_rt_get_bool("COMM_OFI_LIVENESS_CHECKS",
                                                providerInUse(provType_rxm));
  if (envLivenessChecks && chpl_numNodes > 1 && chpl_nodeID == 0) {
    amDoLivenessChecks = true;
  }
}


//
// Chapel global and private variable support
//

wide_ptr_t* chpl_comm_broadcast_global_vars_helper(void) {
  DBG_PRINTF(DBG_IFACE_SETUP, "%s()", __func__);

  //
  // Gather the global variables' wide pointers on node 0 into a
  // buffer, and broadcast the address of that buffer to the other
  // nodes.
  //
  wide_ptr_t* buf;
  if (chpl_nodeID == 0) {
    CHPL_CALLOC(buf, chpl_numGlobalsOnHeap);
    for (int i = 0; i < chpl_numGlobalsOnHeap; i++) {
      buf[i] = *chpl_globals_registry[i];
    }
  }
  chpl_comm_ofi_oob_bcast(&buf, sizeof(buf));
  return buf;
}


static void*** chplPrivBcastTabMap;

static
void init_broadcast_private(void) {
  //
  //
  // Share the nodes' private broadcast tables around.  These are
  // needed by chpl_comm_broadcast_private(), below.
  //
  void** pbtMap;
  size_t pbtSize = chpl_rt_priv_bcast_tab_len
                   * sizeof(chpl_rt_priv_bcast_tab[0]);
  CHPL_CALLOC(pbtMap, chpl_numNodes * pbtSize);
  chpl_comm_ofi_oob_allgather(chpl_rt_priv_bcast_tab, pbtMap, pbtSize);
  CHPL_CALLOC(chplPrivBcastTabMap, chpl_numNodes);
  for (int i = 0; i < chpl_numNodes; i++) {
    chplPrivBcastTabMap[i] = &pbtMap[i * chpl_rt_priv_bcast_tab_len];
  }
}


void chpl_comm_broadcast_private(int id, size_t size) {
  DBG_PRINTF(DBG_IFACE_SETUP, "%s(%d, %zd)", __func__, id, size);

  for (int i = 0; i < chpl_numNodes; i++) {
    if (i != chpl_nodeID) {
      (void) ofi_put(chpl_rt_priv_bcast_tab[id], i,
                     chplPrivBcastTabMap[i][id], size);
    }
  }
}


////////////////////////////////////////
//
// Interface: shutdown
//

static void exit_all(int);
static void exit_any(int);

static void fini_amHandling(void);
static void fini_ofi(void);

static void amRequestShutdown(c_nodeid_t);

void chpl_comm_pre_task_exit(int all) {
  DBG_PRINTF(DBG_IFACE_SETUP, "%s(%d)", __func__, all);

  if (all) {
    if (chpl_nodeID == 0) {
      for (int node = 1; node < chpl_numNodes; node++) {
        amRequestShutdown(node);
      }
    } else {
      chpl_wait_for_shutdown();
    }

    chpl_comm_barrier("chpl_comm_pre_task_exit");
    fini_amHandling();
  }
}


void chpl_comm_exit(int all, int status) {
  DBG_PRINTF(DBG_IFACE_SETUP, "%s(%d, %d)", __func__, all, status);

  if (all) {
    exit_all(status);
  } else {
    exit_any(status);
  }
}


static void exit_all(int status) {
  fini_ofi();
  chpl_comm_ofi_oob_fini();
}


static void exit_any(int status) {
  //
  // (Over)abundance of caution mode: if exiting unilaterally with the
  // 'verbs' provider in use, call _exit() now instead of allowing the
  // usual runtime control flow to call exit() and invoke the atexit(3)
  // functions.  Otherwise we run the risk of segfaulting due to some
  // broken destructor code in librdmacm.  That was fixed years ago by
  //     https://github.com/linux-rdma/rdma-core/commit/9ef8ed2
  // but the fix doesn't seem to have made it into general circulation
  // yet.
  //
  // Flush all the stdio FILE streams first, in the hope of not losing
  // any output.
  //
  if (providerInUse(provType_verbs)) {
    fflush(NULL);
    _exit(status);
  }
}


static
void fini_ofi(void) {
  if (chpl_numNodes <= 1)
    return;

  for (int i = 0; i < memTabCount; i++) {
    OFI_CHK(fi_close(&ofiMrTab[i]->fid));
  }

  if (memTabMap != NULL) {
    CHPL_FREE(memTabMap);
  }

  CHPL_FREE(amLZs[1]);
  CHPL_FREE(amLZs[0]);

  CHPL_FREE(ofi_rxAddrs);

  if (ofi_amhPollSet != NULL) {
    OFI_CHK(fi_poll_del(ofi_amhPollSet, tciTab[tciTabLen - 1].txCmplFid, 0));
    OFI_CHK(fi_poll_del(ofi_amhPollSet, &ofi_rxCQ->fid, 0));
    if (ofi_rxCntr != NULL) {
      OFI_CHK(fi_poll_del(ofi_amhPollSet, &ofi_rxCntr->fid, 0));
    }
  }

  OFI_CHK(fi_close(&ofi_rxEp->fid));
  OFI_CHK(fi_close(&ofi_rxCQ->fid));
  if (ofi_rxCntr != NULL) {
    OFI_CHK(fi_close(&ofi_rxCntr->fid));
  }

  for (int i = 0; i < tciTabLen; i++) {
    OFI_CHK(fi_close(&tciTab[i].txCtx->fid));
    if (tciTab[i].txCntr != NULL) {
      OFI_CHK(fi_close(&tciTab[i].txCntr->fid));
    }
    if (tciTab[i].txCQ != NULL) {
      OFI_CHK(fi_close(&tciTab[i].txCQ->fid));
    }
  }

  if (ofi_txEpScal != NULL) {
    OFI_CHK(fi_close(&ofi_txEpScal->fid));
  }

  for (int i = 0; i < tciTabLen; i++) {
    if (tciTab[i].av != ofi_av) {
      OFI_CHK(fi_close(&tciTab[i].av->fid));
      CHPL_FREE(tciTab[i].addrs);
    }
  }

  if (ofi_rxAv != ofi_av) {
    OFI_CHK(fi_close(&ofi_rxAv->fid));
    CHPL_FREE(ofi_rxAddrs);
  }

  if (ofi_av != NULL) {
    OFI_CHK(fi_close(&ofi_av->fid));
    CHPL_FREE(ofi_addrs);
  }

  if (ofi_amhPollSet != NULL) {
    OFI_CHK(fi_close(&ofi_amhWaitSet->fid));
    OFI_CHK(fi_close(&ofi_amhPollSet->fid));
  }

  OFI_CHK(fi_close(&ofi_domain->fid));
  OFI_CHK(fi_close(&ofi_fabric->fid));

  fi_freeinfo(ofi_info);
}


////////////////////////////////////////
//
// Interface: Registered memory
//

static pthread_once_t fixedHeapOnce = PTHREAD_ONCE_INIT;
static size_t fixedHeapSize;
static void*  fixedHeapStart;

static pthread_once_t hugepageOnce = PTHREAD_ONCE_INIT;
static size_t hugepageSize;

static void init_fixedHeap(void);

static size_t get_hugepageSize(void);
static void init_hugepageSize(void);


void chpl_comm_impl_regMemHeapInfo(void** start_p, size_t* size_p) {
  DBG_PRINTF(DBG_IFACE_SETUP, "%s()", __func__);

  PTHREAD_CHK(pthread_once(&fixedHeapOnce, init_fixedHeap));
  *start_p = fixedHeapStart;
  *size_p  = fixedHeapSize;
}


static
void init_fixedHeap(void) {
  //
  // Determine whether or not we'll use a fixed heap, and if so what its
  // address and size are.  Note that this has to be able to run early,
  // before we've actually selected a provider and set up libfabric,
  // because it can indirectly be called from memory layer init which
  // may need to configure itself differently with or without a fixed
  // heap.
  //

  //
  // Get hints describing our base requirements, the ones that are
  // independent of which MCM conformance mode we'll eventually use.
  //
  chpl_bool txAttrsForced;
  struct fi_info* hints = getBaseProviderHints(&txAttrsForced);

  //
  // If hint construction didn't at least tentatively say we'll use a
  // fixed heap, we definitely won't.  The checks it does are based on
  // run-invariant info such as environment settings, whether this is
  // a multi-node program, etc.
  //
  if ((hints->domain_attr->mr_mode & FI_MR_ALLOCATED) == 0) {
    DBG_PRINTF_NODE0(DBG_HEAP, "fixedHeap: base hints say no");
    return;
  }

  //
  // Now do further checks.  We default to using a fixed heap on Cray XC
  // and (for now) HPE Cray EX systems unless the user explicitly says
  // not to.  That was checked in base hint construction.  On other
  // platforms, we'll use a fixed heap if the best-performing "good"
  // provider (not sockets, not tcp, not verbs;ofi_rxd) out of those
  // that can meet our base requirements has FI_MR_ALLOCATED set to
  // indicate it wants one.
  //
  if (strcmp(CHPL_TARGET_PLATFORM, "cray-xc") != 0
      && strcmp(CHPL_TARGET_PLATFORM, "hpe-cray-ex") != 0) {
    struct fi_info* infoList;
    int ret;
    OFI_CHK_2(fi_getinfo(COMM_OFI_FI_VERSION, NULL, NULL, 0, hints, &infoList),
              ret, -FI_ENODATA);
    if (infoList == NULL) {
      //
      // We found no providers at all, thus none requiring a fixed heap.
      //
      DBG_PRINTF_NODE0(DBG_HEAP,
                       "fixedHeap: no, because no providers (?)");
      return;
    }

    struct fi_info* info;
    for (info = infoList; info != NULL; info = info->next) {
      if (isGoodCoreProvider(info)
          && (!isInProvider("verbs", info)
              || !isInProvider("ofi_rxd", info))
          && isUseableProvider(info)) {
        break;
      }
    }

    chpl_bool useHeap;
    if (info == NULL) {
      DBG_PRINTF_NODE0(DBG_HEAP,
                       "fixedHeap: no, no provider needs it");
      useHeap = false;
    } else if ((info->domain_attr->mr_mode & FI_MR_ALLOCATED) == 0) {
      DBG_PRINTF_NODE0(DBG_HEAP,
                       "fixedHeap: no, best provider '%s' doesn't need it",
                       info->fabric_attr->prov_name);
      useHeap = false;
    } else {
      DBG_PRINTF_NODE0(DBG_HEAP,
                       "fixedHeap: yes, best provider '%s' needs it",
                       info->fabric_attr->prov_name);
      useHeap = true;
    }

    fi_freeinfo(infoList);

    if (!useHeap) {
      return;
    }
  }

  //
  // If we get this far we'll use a fixed heap.
  //
  uint64_t total_memory = chpl_sys_physicalMemoryBytes();

  //
  // Don't use more than 85% of the total memory for heaps.
  //
  uint64_t max_heap_memory = (size_t) (0.85 * total_memory);

  int num_locales_on_node = chpl_get_num_locales_on_node();
  size_t max_heap_per_locale = (size_t) (max_heap_memory / num_locales_on_node);


  //
  // If the maximum heap size is not specified or it's greater than the maximum heap per
  // locale, set it to the maximum heap per locale.
  //
  ssize_t size = envMaxHeapSize;
  CHK_TRUE(size != 0);
  if ((size < 0) || (size > max_heap_per_locale)) {
    size = max_heap_per_locale;
  }

  //
  // Check for hugepages.  On certain systems you really ought to use
  // them.  But if you're on such a system and don't, we'll emit the
  // message later.
  //
  size_t page_size;
  chpl_bool have_hugepages;

  if ((page_size = get_hugepageSize()) == 0) {
    page_size = chpl_getSysPageSize();
    have_hugepages = false;
  } else {
    have_hugepages = true;
  }

  //
  // We'll make a fixed heap, on whole (huge)pages.
  //
  size = ALIGN_UP(size, page_size);

  //
  // Work our way down from the starting size in (roughly) 5% steps
  // until we can actually allocate a heap that size.
  //
  size_t decrement;
  if ((decrement = ALIGN_DN((size_t) (0.05 * size), page_size)) < page_size) {
    decrement = page_size;
  }

  void* start;
  size += decrement;
  do {
    size -= decrement;
#ifdef CHPL_COMM_DEBUG
    if (DBG_TEST_MASK(DBG_HEAP)) {
      char buf[10];
      DBG_PRINTF(DBG_HEAP, "try allocating fixed heap, size %s (%#zx)",
                 chpl_snprintf_KMG_z(buf, sizeof(buf), size), size);
    }
#endif
    if (have_hugepages) {
      start = chpl_comm_ofi_hp_get_huge_pages(size);
    } else {
      CHK_SYS_MEMALIGN(start, page_size, size);
    }
  } while (start == NULL && size > decrement);

  if (start == NULL)
    chpl_error("cannot create fixed heap: cannot get memory", 0, 0);

  chpl_comm_regMemHeapTouch(start, size);

#ifdef CHPL_COMM_DEBUG
  if (DBG_TEST_MASK(DBG_HEAP)) {
    char buf[10];
    DBG_PRINTF(DBG_HEAP, "fixed heap on %spages, start=%p size=%s (%#zx)\n",
               have_hugepages ? "huge" : "regular ", start,
               chpl_snprintf_KMG_z(buf, sizeof(buf), size), size);
  }
#endif
  fixedHeapSize  = size;
  fixedHeapStart = start;
}


size_t chpl_comm_impl_regMemHeapPageSize(void) {
  DBG_PRINTF(DBG_IFACE_SETUP, "%s()", __func__);

  size_t sz;
  if ((sz = get_hugepageSize()) > 0)
    return sz;
  return chpl_getSysPageSize();
}


static
size_t get_hugepageSize(void) {
  PTHREAD_CHK(pthread_once(&hugepageOnce, init_hugepageSize));
  return hugepageSize;
}


static
void init_hugepageSize(void) {
  if (chpl_numNodes > 1
      && getenv("HUGETLB_DEFAULT_PAGE_SIZE") != NULL) {
    hugepageSize = chpl_comm_ofi_hp_gethugepagesize();
  }

  DBG_PRINTF(DBG_HUGEPAGES,
             "setting hugepage info: use hugepages %s, sz %#zx",
             (hugepageSize > 0) ? "YES" : "NO", hugepageSize);
}


static inline
struct memEntry* getMemEntry(memTab_t* tab, void* addr, size_t size) {
  char* myAddr = (char*) addr;

  for (int i = 0; i < memTabCount; i++) {
    char* tabAddr = (char*) (*tab)[i].addr;
    char* tabAddrEnd = tabAddr + (*tab)[i].size;
    if (myAddr >= tabAddr && myAddr + size <= tabAddrEnd)
      return &(*tab)[i];
  }

  return NULL;
}


static inline
chpl_bool mrGetDesc(void** pDesc, void* addr, size_t size) {
  chpl_bool ret;
  void* desc;

  if (scalableMemReg) {
    ret = true;
    desc = NULL;
  } else {
    struct memEntry* mr;
    ret = (mr = getMemEntry(&memTab, addr, size)) != NULL;
    if (ret) {
      desc = mr->desc;
      DBG_PRINTF(DBG_MR_DESC, "mrGetDesc(%p, %zd): desc %p", addr, size, desc);
    } else {
      desc = NULL;
      DBG_PRINTF(DBG_MR_DESC, "mrGetDesc(%p, %zd): no entry", addr, size);
    }
  }

  if (pDesc != NULL) {
    *pDesc = desc;
  }
  return ret;
}


static inline
void* mrLocalize(void** pDesc, const void* addr, size_t size,
                 chpl_bool isSource, const char* what) {
  void* mrAddr = (void*) addr;
  if (mrAddr == NULL) {
    *pDesc = NULL;
  } else if (!mrGetDesc(pDesc, mrAddr, size)) {
    mrAddr = allocBounceBuf(size);
    DBG_PRINTF(DBG_MR_BB, "%s BB: %p", what, mrAddr);
    CHK_TRUE(mrGetDesc(pDesc, mrAddr, size));
    if (isSource) {
      memcpy(mrAddr, addr, size);
    }
  }
  return mrAddr;
}


static inline
void* mrLocalizeSource(void** pDesc, const void* addr, size_t size,
                       const char* what) {
  return mrLocalize(pDesc, addr, size, true /*isSource*/, what);
}


static inline
void* mrLocalizeTarget(void** pDesc, const void* addr, size_t size,
                       const char* what) {
  return mrLocalize(pDesc, addr, size, false /*isSource*/, what);
}


static inline
void mrUnLocalizeSource(void* mrAddr, const void* addr) {
  if (mrAddr != NULL && mrAddr != addr) {
    freeBounceBuf(mrAddr);
  }
}


static inline
void mrUnLocalizeTarget(void* mrAddr, void* addr, size_t size) {
  if (mrAddr != NULL && mrAddr != addr) {
    memcpy(addr, mrAddr, size);
    freeBounceBuf(mrAddr);
  }
}


static inline
chpl_bool mrGetKey(uint64_t* pKey, uint64_t* pOff,
                   int iNode, void* addr, size_t size) {
  uint64_t key;
  uint64_t off;

  if (scalableMemReg) {
    key = 0;
    off = (uint64_t) addr;
  } else {
    struct memEntry* mr;
    if ((mr = getMemEntry(&memTabMap[iNode], addr, size)) == NULL) {
      DBG_PRINTF(DBG_MR_KEY, "mrGetKey(%d:%p, %zd): no entry",
                 iNode, addr, size);
      return false;
    }

    key = mr->key;
    off = (uint64_t) addr - mr->base;
    DBG_PRINTF(DBG_MR_KEY,
               "mrGetKey(%d:%p, %zd): key %" PRIx64 ", off %" PRIx64,
               iNode, addr, size, key, off);
  }

  if (pKey != NULL) {
    *pKey = key;
    *pOff = off;
  }
  return true;
}


static inline
chpl_bool mrGetLocalKey(void* addr, size_t size) {
  return mrGetKey(NULL, NULL, chpl_nodeID, addr, size);
}


static inline
void* mrLocalizeRemote(const void* addr, size_t size, chpl_bool isSource,
                       const char* what) {
  void* mrAddr = (void*) addr;
  if (!mrGetLocalKey(mrAddr, size)) {
    mrAddr = allocBounceBuf(size);
    DBG_PRINTF(DBG_MR_BB, "%s BB: %p", what, mrAddr);
    CHK_TRUE(mrGetLocalKey(mrAddr, size));
    if (isSource) {
      memcpy(mrAddr, addr, size);
    }
  }
  return mrAddr;
}


static inline
void* mrLocalizeSourceRemote(const void* addr, size_t size,
                             const char* what) {
  return mrLocalizeRemote(addr, size, true /*isSource*/, what);
}


static inline
void* mrLocalizeTargetRemote(const void* addr, size_t size,
                             const char* what) {
  return mrLocalizeRemote(addr, size, false /*isSource*/, what);
}


////////////////////////////////////////
//
// Interface: memory consistency
//

static void retireDelayedAmDone(chpl_bool);


// Initialize the transmit context, which is an opaque pointer passed
// to the transmit routines and returned in the completion event.
// In the case of a completion queue it's a boolean that is set
// to 'true' when the event occurs. Otherwise it's the line number where
// it was created, which is used for debugging as there are no completion
// events when using a counter. With a counter we can't signal that
// individual submissions are complete, we have to wait for the counter
// to indicate that they all completed.

static inline
void *txCtxInit(struct perTxCtxInfo_t* tcip, int line, atomic_bool *done) {
  void *ctx;
  if (tcip->txCntr == NULL) {
    atomic_init_bool(done, false);
    ctx = txnTrkEncodeDone(done);
  } else {
    ctx = txnTrkEncodeId(line);
  }
  return ctx;
}

static inline
void txCtxCleanup(void *ctx) {
  const txnTrkCtx_t trk = txnTrkDecode(ctx);
  if (trk.typ == txnTrkDone) {
    atomic_destroy_bool((atomic_bool*) trk.ptr);
  }
}

static inline
void mcmReleaseOneNode(c_nodeid_t node, struct perTxCtxInfo_t* tcip,
                       const char* dbgOrderStr) {
  DBG_PRINTF(DBG_ORDER,
             "dummy GET from %d for %s ordering",
             (int) node, dbgOrderStr);
  uint64_t flags = (mcmMode == mcmm_msgOrdFence) ? FI_FENCE : 0;
  atomic_bool txnDone;
  void *ctx = txCtxInit(tcip, __LINE__, &txnDone);
  ofi_get_lowLevel(orderDummy, orderDummyMRDesc, node,
                   orderDummyMap[node].mrRaddr, orderDummyMap[node].mrKey,
                   sizeof(*orderDummy), ctx, flags, tcip);
  waitForTxnComplete(tcip, ctx);
  txCtxCleanup(ctx);
}


static
void mcmReleaseAllNodes(struct bitmap_t* b1, struct bitmap_t* b2,
                        c_nodeid_t skipNode,
                        struct perTxCtxInfo_t* tcip,
                        const char* dbgOrderStr) {
  //
  // Do a transaction do force all outstanding operations' remote
  // memory effects to be visible, on every node in one bitmap or
  // either of two bitmaps.  The effects of the transactions we do
  // here don't matter, only their completions.
  //
  // TODO: Allow multiple of these transactions outstanding at once,
  //       instead of waiting for each one before firing the next.
  //
  if (b1 == NULL) {
    if (b2 == NULL) {
      // Nothing to do.
    } else {
      BITMAP_FOREACH_SET(b2, node) {
        if (skipNode < 0 || node != skipNode) {
          (*tcip->checkTxCmplsFn)(tcip);
          mcmReleaseOneNode(node, tcip, dbgOrderStr);
          bitmapClear(b2, node);
        }
      } BITMAP_FOREACH_SET_END
    }
  } else {
    if (b2 == NULL) {
      BITMAP_FOREACH_SET(b1, node) {
        if (skipNode < 0 || node != skipNode) {
          (*tcip->checkTxCmplsFn)(tcip);
          mcmReleaseOneNode(node, tcip, dbgOrderStr);
          bitmapClear(b1, node);
        }
      } BITMAP_FOREACH_SET_END
    } else {
      BITMAP_FOREACH_SET_OR(b1, b2, node) {
        if (skipNode < 0 || node != skipNode) {
          (*tcip->checkTxCmplsFn)(tcip);
          mcmReleaseOneNode(node, tcip, dbgOrderStr);
          bitmapClear(b1, node);
          bitmapClear(b2, node);
        }
      } BITMAP_FOREACH_SET_END
    }
  }
}


void chpl_comm_impl_unordered_task_fence(void) {
  DBG_PRINTF(DBG_IFACE_MCM, "%s()", __func__);

  task_local_buff_end(get_buff | put_buff | amo_nf_buff);
}


inline
void chpl_comm_impl_task_create(void) {
  DBG_PRINTF(DBG_IFACE_MCM, "%s()", __func__);

  retireDelayedAmDone(false /*taskIsEnding*/);
  forceMemFxVisAllNodes_noTcip(true /*checkPuts*/, true /*checkAmos*/);
}


void chpl_comm_impl_task_end(void) {
  DBG_PRINTF(DBG_IFACE_MCM, "%s()", __func__);

  task_local_buff_end(get_buff | put_buff | amo_nf_buff);
  retireDelayedAmDone(true /*taskIsEnding*/);
  forceMemFxVisAllNodes_noTcip(true /*checkPuts*/, true /*checkAmos*/);
}


////////////////////////////////////////
//
// Interface: Active Messages
//

typedef enum {
  am_opExecOn = CHPL_ARG_BUNDLE_KIND_COMM, // impl-nonspecific on-stmt
  am_opExecOnLrg,                          // on-stmt, large arg
  am_opGet,                                // do an RMA GET
  am_opPut,                                // do an RMA PUT
  am_opAMO,                                // do an AMO
  am_opFAMOResult,                         // return result of fetching AMO
  am_opFree,                               // free some memory
  am_opNop,                                // do nothing; for MCM & liveness
  am_opShutdown,                           // signal main process for shutdown
} amOp_t;

static inline
chpl_bool op_uses_on_bundle(amOp_t op) {
  return op == am_opExecOn || op == am_opExecOnLrg;
}

//
// Members are packed, potentially differently, in each AM request type
// to reduce space requirements.  The 'op' member must come first in all
// cases, so the AM handler can tell what kind of request it's looking
// at.
//

typedef uint8_t amDone_t;

struct amRequest_base_t {
  chpl_arg_bundle_kind_t op;  // operation
  c_nodeid_t node;            // initiator's node
  amDone_t* pAmDone;          // initiator's 'done' flag; may be NULL
#ifdef CHPL_COMM_DEBUG
  uint64_t seq;
#endif
};

struct amRequest_RMA_t {
  struct amRequest_base_t b;
  void* addr;                   // address on AM target node
  void* raddr;                  // address on AM initiator's node
  size_t size;                  // number of bytes
};

typedef union {
  int32_t i32;
  uint32_t u32;
  chpl_bool32 b32;
  int64_t i64;
  uint64_t u64;
  _real32 r32;
  _real64 r64;
} chpl_amo_datum_t;

struct amRequest_AMO_t {
  struct amRequest_base_t b;
  enum fi_op ofiOp;             // ofi AMO op
  enum fi_datatype ofiType;     // ofi object type
  int8_t size;                  // object size (bytes)
  void* obj;                    // object address on target node
  chpl_amo_datum_t opnd;        // operand, if needed
  chpl_amo_datum_t cmpr;        // comparand, if needed
  void* result;                 // result address on initiator's node
};

struct amRequest_FAMO_result_t {
  struct amRequest_base_t b;
  int8_t size;                  // result size (bytes)
  chpl_amo_datum_t result;      // the result
  enum fi_datatype ofiType;     // result ofi type
  void* res;                    // result's address on the node that initiated
                                // the fetching AMO
};

struct amRequest_free_t {
  struct amRequest_base_t b;
  void* p;                      // address to free, on AM target node
};

typedef union {
  struct amRequest_base_t b;
  struct amRequest_execOn_t xo;      // present only to set the max req size
  struct amRequest_execOnLrg_t xol;
  struct amRequest_RMA_t rma;
  struct amRequest_AMO_t amo;
  struct amRequest_FAMO_result_t famo_result;
  struct amRequest_free_t free;
} amRequest_t;

struct taskArg_RMA_t {
  chpl_task_bundle_t hdr;
  struct amRequest_RMA_t rma;
};


#ifdef CHPL_COMM_DEBUG
static const char* am_opName(amOp_t);
static const char* amo_opName(enum fi_op);
static const char* amo_typeName(enum fi_datatype);
static const char* am_seqIdStr(amRequest_t*);
static const char* am_reqStr(c_nodeid_t, amRequest_t*, size_t);
static const char* am_reqStartStr(amRequest_t*);
static const char* am_reqDoneStr(amRequest_t*);
static void am_debugPrep(amRequest_t*);
#endif

static void amRequestExecOn(c_nodeid_t, c_sublocid_t, chpl_fn_int_t,
                            chpl_comm_on_bundle_t*, size_t,
                            chpl_bool, chpl_bool);
static void amRequestRmaPut(c_nodeid_t, void*, void*, size_t);
static void amRequestRmaGet(c_nodeid_t, void*, void*, size_t);
static void amRequestAMO(c_nodeid_t, void*, const void*, const void*, void*,
                         int, enum fi_datatype, size_t);
static void amRequestFAMOResult(c_nodeid_t, chpl_amo_datum_t*,
                                enum fi_datatype, void*, size_t, amDone_t*);
static void amRequestFree(c_nodeid_t, void*);
static void amRequestNop(c_nodeid_t, chpl_bool, struct perTxCtxInfo_t*);
static void amRequestCommon(c_nodeid_t, amRequest_t*, size_t,
                            chpl_bool, struct perTxCtxInfo_t*);
static void amWaitForDone(amDone_t*);


void chpl_comm_execute_on(c_nodeid_t node, c_sublocid_t subloc,
                          chpl_fn_int_t fid,
                          chpl_comm_on_bundle_t *arg, size_t argSize,
                          int ln, int32_t fn) {
  DBG_PRINTF(DBG_IFACE,
             "%s(%d, %d, %d, %p, %zd)", __func__,
             (int) node, (int) subloc, (int) fid, arg, argSize);

  CHK_TRUE(node != chpl_nodeID); // handled by the locale model

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_executeOn, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, argSize, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_executeOn("", node, ln, fn);
  chpl_comm_diags_incr(execute_on);

  amRequestExecOn(node, subloc, fid, arg, argSize, false, true);
}


void chpl_comm_execute_on_nb(c_nodeid_t node, c_sublocid_t subloc,
                             chpl_fn_int_t fid,
                             chpl_comm_on_bundle_t *arg, size_t argSize,
                             int ln, int32_t fn) {
  DBG_PRINTF(DBG_IFACE,
             "%s(%d, %d, %d, %p, %zd)", __func__,
             (int) node, (int) subloc, (int) fid, arg, argSize);

  CHK_TRUE(node != chpl_nodeID); // handled by the locale model

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_nb)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_executeOn_nb, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, argSize, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_executeOn("non-blocking", node, ln, fn);
  chpl_comm_diags_incr(execute_on_nb);

  amRequestExecOn(node, subloc, fid, arg, argSize, false, false);
}


void chpl_comm_execute_on_fast(c_nodeid_t node, c_sublocid_t subloc,
                               chpl_fn_int_t fid,
                               chpl_comm_on_bundle_t *arg, size_t argSize,
                               int ln, int32_t fn) {
  DBG_PRINTF(DBG_IFACE,
             "%s(%d, %d, %d, %p, %zd)", __func__,
             (int) node, (int) subloc, (int) fid, arg, argSize);

  CHK_TRUE(node != chpl_nodeID); // handled by the locale model

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_fast)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_executeOn_fast, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, argSize, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_executeOn("fast", node, ln, fn);
  chpl_comm_diags_incr(execute_on_fast);

  amRequestExecOn(node, subloc, fid, arg, argSize, true, true);
}


static inline
void amRequestExecOn(c_nodeid_t node, c_sublocid_t subloc,
                     chpl_fn_int_t fid,
                     chpl_comm_on_bundle_t* arg, size_t argSize,
                     chpl_bool fast, chpl_bool blocking) {
  assert(!isAmHandler);
  CHK_TRUE(!(fast && !blocking)); // handler doesn't expect fast nonblocking

  retireDelayedAmDone(false /*taskIsEnding*/);

  arg->comm = (chpl_comm_bundleData_t) { .fast = fast,
                                         .fid = fid,
                                         .node = chpl_nodeID,
                                         .subloc = subloc,
                                         .argSize = argSize, };

  if (argSize <= sizeof(amRequest_t)) {
    //
    // The arg bundle will fit in max-sized AM request; just send it.
    //
    arg->kind = am_opExecOn;
    amRequestCommon(node, (amRequest_t*) arg, argSize, blocking, NULL);
  } else {
    //
    // The arg bundle is too large for an AM request.  Send a copy of
    // the header to the target and have it retrieve the payload part
    // itself.
    //
    // For the nonblocking case we have to make a copy of the caller's
    // payload because as soon as we return, the caller may destroy
    // the original.  We also make a copy if the original is not in
    // registered memory and needs to be, in order to save the target
    // the overhead of doing an AM back to us to PUT the bundle to
    // itself.
    //
    arg->kind = am_opExecOnLrg;
    amRequest_t req = { .xol = { .hdr = *arg,
                                 .pPayload = &arg->payload, }, };

    chpl_bool heapCopyArg = !blocking || !mrGetLocalKey(arg, argSize);
    if (heapCopyArg) {
      size_t payloadSize = argSize - offsetof(chpl_comm_on_bundle_t, payload);
      req.xol.pPayload = allocBounceBuf(payloadSize);
      memcpy(req.xol.pPayload, &arg->payload, payloadSize);
    }

    amRequestCommon(node, &req, sizeof(req.xol), blocking, NULL);

    //
    // If blocking and we heap-copied the arg, free that now.  The
    // nonblocking case has to be handled from the target side, since
    // only there do we know when we don't need the copy any more.
    //
    if (heapCopyArg && blocking) {
      freeBounceBuf(req.xol.pPayload);
    }
  }
}


static inline
void amRequestRmaPut(c_nodeid_t node, void* addr, void* raddr, size_t size) {
  assert(!isAmHandler);

  retireDelayedAmDone(false /*taskIsEnding*/);

  //
  // Make sure the local address is remotely accessible.
  //
  void* myAddr = mrLocalizeSourceRemote(addr, size, "RMA via AM");

  DBG_PRINTF(DBG_RMA | DBG_RMA_WRITE,
             "PUT %d:%p <= %p, size %zd, via AM GET",
             (int) node, raddr, myAddr, size);
  amRequest_t req = { .rma = { .b = { .op = am_opGet, // remote node does GET
                                      .node = chpl_nodeID, },
                               .addr = raddr,
                               .raddr = myAddr,
                               .size = size, }, };
  amRequestCommon(node, &req, sizeof(req.rma), true, NULL);

  mrUnLocalizeSource(myAddr, addr);
}


static inline
void amRequestRmaGet(c_nodeid_t node, void* addr, void* raddr, size_t size) {
  assert(!isAmHandler);

  retireDelayedAmDone(false /*taskIsEnding*/);

  //
  // Make sure the local address is remotely accessible.
  //
  void* myAddr = mrLocalizeTargetRemote(addr, size, "RMA via AM");

  DBG_PRINTF(DBG_RMA | DBG_RMA_READ,
             "GET %p <= %d:%p, size %zd, via AM PUT",
             myAddr, (int) node, raddr, size);
  amRequest_t req = { .rma = { .b = { .op = am_opPut, // remote node does PUT
                                      .node = chpl_nodeID, },
                               .addr = raddr,
                               .raddr = myAddr,
                               .size = size, }, };
  amRequestCommon(node, &req, sizeof(req.rma), true, NULL);

  mrUnLocalizeTarget(myAddr, addr, size);
}


static inline
void amRequestAMO(c_nodeid_t node, void* object,
                  const void* opnd, const void* cmpr, void* result,
                  int ofiOp, enum fi_datatype ofiType, size_t size) {
  assert(!isAmHandler);
  DBG_PRINTF((ofiOp == FI_ATOMIC_READ) ? DBG_AMO_READ : DBG_AMO,
             "AMO via AM: obj %d:%p, opnd <%s>, cmpr <%s>, res %p, "
             "op %s, typ %s, sz %zd",
             (int) node, object,
             DBG_VAL(opnd, ofiType), DBG_VAL(cmpr, ofiType), result,
             amo_opName(ofiOp), amo_typeName(ofiType), size);

  struct perTxCtxInfo_t* tcip;
  CHK_TRUE((tcip = tciAlloc()) != NULL);

  void* myResult = result;

  //
  // If this is a non-fetching atomic and the task is ending (therefore
  // this is the _downEndCount()) we do it as a regular nonblocking AM.
  // If it's non-fetching and the task is not ending we may be able to
  // do it as a blocking AM but delay waiting for the 'done' indicator
  // until sometime later, when the next thing with MCM implications
  // comes along.  For fetching atomic ops or if we lack task-private
  // data for some reason, we have to do a normal blocking AM.
  //
  //
  chpl_bool blocking = true;
  amDone_t* pAmDone = NULL;
  if (myResult == NULL) {
    chpl_comm_taskPrvData_t* prvData = get_comm_taskPrvdata();
    if (prvData != NULL) {
      blocking = false;

      if (prvData->taskIsEnding) {
        pAmDone = NULL;
      } else {
        //
        // In the delayed-blocking case we have to prep the indicator
        // ourselves -- the common AM code doesn't do it.
        //
        prvData->amDonePending = true;
        prvData->amDone = 0;
        chpl_atomic_thread_fence(memory_order_release);
        pAmDone = &prvData->amDone;
      }
    }
  } else {
    myResult = mrLocalizeTargetRemote(myResult, size, "AMO result");
  }

  amRequest_t req = { .amo = { .b = { .op = am_opAMO,
                                      .node = chpl_nodeID,
                                      .pAmDone = pAmDone, },
                               .ofiOp = ofiOp,
                               .ofiType = ofiType,
                               .size = size,
                               .obj = object,
                               .result = myResult, }, };

  if (opnd != NULL) {
    memcpy(&req.amo.opnd, opnd, size);
  }
  if (cmpr != NULL) {
    memcpy(&req.amo.cmpr, cmpr, size);
  }

  //
  // The common code will block on our behalf only for a fetching AMO.
  // A non-fetching one will either be non- or delayed-blocking, and
  // neither of those require anything from the common code.
  //
  amRequestCommon(node, &req, sizeof(req.amo), blocking, tcip);
  mrUnLocalizeTarget(myResult, result, size);
  tciFree(tcip);
}


/*
amRequestFAMOResult

This function is invoked by a fetching AMO handler to return the result
back to the locale that initiated the fetching AMO. The scenario is
that locale A has sent a fetching AMO to locale B via an AM. The AM
handler on B invokes this function to send an AM back to A that causes
A to store the result of the fetching AMO and set the pAmDone flag.
*/

static inline
void amRequestFAMOResult(c_nodeid_t node,
                  chpl_amo_datum_t *result,
                  enum fi_datatype ofiType, // type of the result
                  void *targetAddr, // address of result on remote node
                  size_t size,       // size of the result
                  amDone_t *pAmDone) // address of pAmDone on remote node
{

  DBG_PRINTF(DBG_AM | DBG_AM_SEND, "amRequestFAMOResult");
  struct perTxCtxInfo_t* tcip;
  CHK_TRUE((tcip = tciAlloc()) != NULL);

  amRequest_t req = { .famo_result = { .b = { .op = am_opFAMOResult,
                                      .node = chpl_nodeID,
                                      .pAmDone = pAmDone, },
                               .size = size,
                               .ofiType = ofiType,
                               .res = targetAddr,
                               .result = *result}, };

  amRequestCommon(node, &req, sizeof(req.famo_result), false /* blocking */,
                  tcip);
  tciFree(tcip);
}


static inline
void amRequestFree(c_nodeid_t node, void* p) {
  amRequest_t req = { .free = { .b = { .op = am_opFree,
                                       .node = chpl_nodeID, },
                                .p = p, }, };
  amRequestCommon(node, &req, sizeof(req.free), false, NULL);
}


static inline
void amRequestNop(c_nodeid_t node, chpl_bool blocking,
                  struct perTxCtxInfo_t* tcip) {
  amRequest_t req = { .b = { .op = am_opNop,
                             .node = chpl_nodeID, }, };
  amRequestCommon(node, &req, sizeof(req.b), blocking, tcip);
}


static inline
void amRequestShutdown(c_nodeid_t node) {
  assert(!isAmHandler);
  amRequest_t req = { .b = { .op = am_opShutdown,
                             .node = chpl_nodeID, }, };
  amRequestCommon(node, &req, sizeof(req.b), false, NULL);
}


typedef void (amReqFn_t)(c_nodeid_t node,
                         amRequest_t* req, size_t reqSize, void* mrDesc,
                         chpl_bool blocking, struct perTxCtxInfo_t* tcip);

static amReqFn_t amReqFn_selector;

static inline
void amRequestCommon(c_nodeid_t node,
                     amRequest_t* req, size_t reqSize,
                     chpl_bool blocking,
                     struct perTxCtxInfo_t* tcip) {
  //
  // If blocking, make sure target can RMA PUT the indicator to us.
  // For the delayed-blocking AMO case our caller will have set up the
  // 'done' pointer in the AM request descriptor already; the target
  // side still sends back a 'done' indicator in that case, but we're
  // not responsible for either the set-up or the waiting here.
  //
  amDone_t amDone = 0;
  amDone_t* pAmDone = NULL;
  if (blocking) {
    pAmDone = mrLocalizeTargetRemote(&amDone, sizeof(amDone),
                                     "AM done indicator");
    if (op_uses_on_bundle(req->b.op)) {
      req->xo.hdr.comm.pAmDone = pAmDone;
    } else {
      req->b.pAmDone = pAmDone;
    }
    *pAmDone = 0;
    chpl_atomic_thread_fence(memory_order_release);
  }

#ifdef CHPL_COMM_DEBUG
  am_debugPrep(req);
#endif

  struct perTxCtxInfo_t* myTcip = tcip;
  if (myTcip == NULL) {
    CHK_TRUE((myTcip = tciAlloc()) != NULL);
  }

  void* mrDesc;
  amRequest_t* myReq = mrLocalizeSource(&mrDesc, req, reqSize, "AM req");

  amReqFn_selector(node, myReq, reqSize, mrDesc, blocking, myTcip);

  if (tcip == NULL) {
    tciFree(myTcip);
  }

  mrUnLocalizeSource(myReq, req);

  if (blocking) {
    amWaitForDone(pAmDone);
    mrUnLocalizeSource(pAmDone, &amDone); // don't need or want target copyout
  }
}


static amReqFn_t amReqFn_msgOrdFence;
static amReqFn_t amReqFn_msgOrd;
static amReqFn_t amReqFn_dlvrCmplt;

static inline
void amReqFn_selector(c_nodeid_t node,
                      amRequest_t* req, size_t reqSize, void* mrDesc,
                      chpl_bool blocking, struct perTxCtxInfo_t* tcip) {
  switch (mcmMode) {
  case mcmm_msgOrdFence:
    amReqFn_msgOrdFence(node, req, reqSize, mrDesc, blocking, tcip);
    break;
  case mcmm_msgOrd:
    amReqFn_msgOrd(node, req, reqSize, mrDesc, blocking, tcip);
    break;
  case mcmm_dlvrCmplt:
    amReqFn_dlvrCmplt(node, req, reqSize, mrDesc, blocking, tcip);
    break;
  default:
    INTERNAL_ERROR_V("unexpected mcmMode %d", mcmMode);
    break;
  }
}


static ssize_t wrap_fi_send(c_nodeid_t node,
                            amRequest_t* req, size_t reqSize, void* mrDesc,
                            void* ctx,
                            struct perTxCtxInfo_t* tcip);
static ssize_t wrap_fi_inject(c_nodeid_t node,
                              amRequest_t* req, size_t reqSize,
                              struct perTxCtxInfo_t* tcip);
static ssize_t wrap_fi_sendmsg(c_nodeid_t node,
                               amRequest_t* req, size_t reqSize, void* mrDesc,
                               void* ctx, uint64_t flags,
                               struct perTxCtxInfo_t* tcip);


//
// Implements amRequestCommon() when MCM mode is message ordering with fences.
//
static
void amReqFn_msgOrdFence(c_nodeid_t node,
                         amRequest_t* req, size_t reqSize, void* mrDesc,
                         chpl_bool blocking, struct perTxCtxInfo_t* tcip) {
  //
  // For on-stmts and AMOs that might modify their target variable, MCM
  // conformance requires us first to ensure that previous AMOs and PUTs
  // to any node are visible.  Similarly, for GETs we have to ensure
  // that previous AMOs and PUTs to the target node are visible, and for
  // PUTs we have to ensure that previous AMOs to the target node are
  // visible.  Do that here for all nodes except this op's target.  For
  // that node, we'll use a fenced send instead.
  //
  chpl_bool havePutsOut = false;
  chpl_bool haveAmosOut = false;

  switch (req->b.op) {
  case am_opExecOn:
  case am_opExecOnLrg:
    forceMemFxVisAllNodes(true /*checkPuts*/, true /*checkAmos*/,
                          node /*skipNode*/, tcip);
    havePutsOut = (tcip->putVisBitmap != NULL
                   && bitmapTest(tcip->putVisBitmap, node));
    haveAmosOut = (tcip->amoVisBitmap != NULL
                   && bitmapTest(tcip->amoVisBitmap, node));
    break;
  case am_opAMO:
    {
      chpl_bool amoHasMemFx = (req->amo.ofiOp != FI_ATOMIC_READ);
      forceMemFxVisAllNodes(amoHasMemFx /*checkPuts*/, true /*checkAmos*/,
                            node /*skipNode*/, tcip);
      havePutsOut = (amoHasMemFx
                     && tcip->putVisBitmap != NULL
                     && bitmapTest(tcip->putVisBitmap, node));
      haveAmosOut = (tcip->amoVisBitmap != NULL
                     && bitmapTest(tcip->amoVisBitmap, node));
    }
    break;
  case am_opGet:
    havePutsOut = (tcip->putVisBitmap != NULL
                   && bitmapTest(tcip->putVisBitmap, node));
    haveAmosOut = (tcip->amoVisBitmap != NULL
                   && bitmapTest(tcip->amoVisBitmap, node));
    break;
  case am_opPut:
    haveAmosOut = (tcip->amoVisBitmap != NULL
                   && bitmapTest(tcip->amoVisBitmap, node));
    break;
  }

  if (havePutsOut || haveAmosOut) {
    //
    // Special case: Do a fenced send if we need it for ordering with
    // respect to some prior operation(s).  If we can inject, do so and
    // just collect the completion later.  Otherwise, wait for it here.
    //
    if (!blocking
        && reqSize <= ofi_info->tx_attr->inject_size
        && envInjectAM) {
      void* ctx = txnTrkEncodeId(__LINE__);
      uint64_t flags = FI_FENCE | FI_INJECT;
      (void) wrap_fi_sendmsg(node, req, reqSize, mrDesc, ctx, flags, tcip);
    } else {
      atomic_bool txnDone;
      void *ctx = txCtxInit(tcip, __LINE__, &txnDone);
      uint64_t flags = FI_FENCE;
      (void) wrap_fi_sendmsg(node, req, reqSize, mrDesc, ctx, flags, tcip);
      waitForTxnComplete(tcip, ctx);
      txCtxCleanup(ctx);
    }

    if (havePutsOut) {
      bitmapClear(tcip->putVisBitmap, node);
    }
    if (haveAmosOut) {
      bitmapClear(tcip->amoVisBitmap, node);
    }
  } else {
    //
    // General case.
    //
    atomic_bool txnDone;
    void *ctx = txCtxInit(tcip, __LINE__, &txnDone);
    (void) wrap_fi_send(node, req, reqSize, mrDesc, ctx, tcip);
    waitForTxnComplete(tcip, ctx);
    txCtxCleanup(ctx);
  }
}


//
// Implements amRequestCommon() when MCM mode is message ordering.
//
static
void amReqFn_msgOrd(c_nodeid_t node,
                    amRequest_t* req, size_t reqSize, void* mrDesc,
                    chpl_bool blocking, struct perTxCtxInfo_t* tcip) {
  //
  // For on-stmts and AMOs that might modify their target variable, MCM
  // conformance requires us first to ensure that all previous AMOs and
  // PUTs are visible.  Similarly, for GET ops, we have to ensure that
  // AMOs and PUTs to the same node are visible.  And for PUT ops, we
  // have to ensure that AMOs to the same node are visible.  No other AM
  // operations depend on AMO or PUT visibility.
  //
  switch (req->b.op) {
  case am_opExecOn:
  case am_opExecOnLrg:
    forceMemFxVisAllNodes(true /*checkPuts*/, true /*checkAmos*/,
                          -1 /*skipNode*/, tcip);
    break;
  case am_opAMO:
    if (req->amo.ofiOp != FI_ATOMIC_READ) {
      forceMemFxVisAllNodes(true /*checkPuts*/, true /*checkAmos*/,
                            -1 /*skipNode*/, tcip);
    }
    break;
  case am_opGet:
    forceMemFxVisOneNode(node, true /*checkPuts*/, true /*checkAmos*/, tcip);
    break;
  case am_opPut:
    forceMemFxVisOneNode(node, false /*checkPuts*/, true /*checkAmos*/, tcip);
    break;
  }

  if (!blocking
      && reqSize <= ofi_info->tx_attr->inject_size
      && envInjectAM) {
    //
    // Special case: injection is the quickest.  We use that if this is
    // a non-blocking AM and the size doesn't exceed the injection size
    // limit.  (We could even inject a small-enough AM request if this
    // were a blocking AM, but there's no point because we're going to
    // wait for it to get done on the target anyway.)
    //
    (void) wrap_fi_inject(node, req, reqSize, tcip);
  } else {
    //
    // General case.
    //
    atomic_bool txnDone;
    void *ctx = txCtxInit(tcip, __LINE__, &txnDone);
    (void) wrap_fi_send(node, req, reqSize, mrDesc, ctx, tcip);
    waitForTxnComplete(tcip, ctx);
    txCtxCleanup(ctx);
  }
}


//
// Implements amRequestCommon() when MCM mode is delivery complete.
//
static
void amReqFn_dlvrCmplt(c_nodeid_t node,
                       amRequest_t* req, size_t reqSize, void* mrDesc,
                       chpl_bool blocking, struct perTxCtxInfo_t* tcip) {
  if (!blocking
      && (reqSize <= ofi_info->tx_attr->inject_size)
      && (ofi_info->tx_attr->msg_order & FI_ORDER_SAS)
      && envInjectAM) {

    /*
    Special case: injection is the quickest.  We use that if this is a
    non-blocking AM, the size doesn't exceed the injection size limit,
    and the provider supports send-after-send (SAS) message ordering so
    that AMs are not reordered by the network.  (We could even inject a
    small-enough AM request if this were a blocking AM, but there's no
    point because we're going to wait for it to get done on the target
    anyway.)
    */

    (void) wrap_fi_inject(node, req, reqSize, tcip);
  } else {
    //
    // General case.
    //
    atomic_bool txnDone;
    void *ctx = txCtxInit(tcip, __LINE__, &txnDone);
    (void) wrap_fi_send(node, req, reqSize, mrDesc, ctx, tcip);
    waitForTxnComplete(tcip, ctx);
    txCtxCleanup(ctx);
  }
}


static inline
ssize_t wrap_fi_send(c_nodeid_t node,
                     amRequest_t* req, size_t reqSize, void* mrDesc,
                     void* ctx,
                     struct perTxCtxInfo_t* tcip) {
  if (DBG_TEST_MASK(DBG_AM | DBG_AM_SEND)
      || (req->b.op == am_opAMO && DBG_TEST_MASK(DBG_AMO))) {
    DBG_DO_PRINTF("tx AM send to %d: %s, ctx %p",
                  (int) node, am_reqStr(node, req, reqSize), ctx);
  }
  OFI_RIDE_OUT_EAGAIN(tcip,
                      fi_send(tcip->txCtx, req, reqSize, mrDesc,
                              rxAddr(tcip, node), ctx));
  tcip->numTxnsOut++;
  tcip->numTxnsSent++;
  return FI_SUCCESS;
}


static inline
ssize_t wrap_fi_inject(c_nodeid_t node,
                       amRequest_t* req, size_t reqSize,
                       struct perTxCtxInfo_t* tcip) {
  if (DBG_TEST_MASK(DBG_AM | DBG_AM_SEND)
      || (req->b.op == am_opAMO && DBG_TEST_MASK(DBG_AMO))) {
    DBG_DO_PRINTF("tx AM send inject to %d: %s",
                  (int) node, am_reqStr(node, req, reqSize));
  }
  // TODO: How quickly/often does local resource throttling happen?
  OFI_RIDE_OUT_EAGAIN(tcip,
                      fi_inject(tcip->txCtx, req, reqSize,
                                rxAddr(tcip, node)));
  tcip->numTxnsSent++;
  return FI_SUCCESS;
}


static inline
ssize_t wrap_fi_sendmsg(c_nodeid_t node,
                        amRequest_t* req, size_t reqSize, void* mrDesc,
                        void* ctx, uint64_t flags,
                        struct perTxCtxInfo_t* tcip) {
  const struct iovec msg_iov = { .iov_base = req,
                                 .iov_len = reqSize };
  const struct fi_msg msg = { .msg_iov = &msg_iov,
                              .desc = &mrDesc,
                              .iov_count = 1,
                              .addr = rxAddr(tcip, node),
                              .context = ctx };
  if (DBG_TEST_MASK(DBG_AM | DBG_AM_SEND)
      || (req->b.op == am_opAMO && DBG_TEST_MASK(DBG_AMO))) {
    DBG_DO_PRINTF("tx AM send msg to %d: %s, ctx %p, flags %#" PRIx64,
                  (int) node, am_reqStr(node, req, reqSize), ctx, flags);
  }
  OFI_RIDE_OUT_EAGAIN(tcip,
                      fi_sendmsg(tcip->txCtx, &msg, flags));
  tcip->numTxnsOut++;
  tcip->numTxnsSent++;
  return FI_SUCCESS;
}


static inline
void amWaitForDone(amDone_t* pAmDone) {
  //
  // Wait for completion indicator.
  //
  DBG_PRINTF(DBG_AM | DBG_AM_SEND,
             "waiting for amDone indication in %p", pAmDone);
  while (!*(volatile amDone_t*) pAmDone) {
    local_yield();
  }
  DBG_PRINTF(DBG_AM | DBG_AM_SEND, "saw amDone indication in %p", pAmDone);
}


static inline
void retireDelayedAmDone(chpl_bool taskIsEnding) {
  //
  // Wait for the completion of any delayed-blocking AM.
  //
  chpl_comm_taskPrvData_t* prvData = get_comm_taskPrvdata();
  if (prvData != NULL) {
    if (prvData->amDonePending) {
      amWaitForDone((amDone_t*) &prvData->amDone);
      prvData->amDonePending = false;
    }
    if (taskIsEnding) {
      prvData->taskIsEnding = true;
    }
  }
}


////////////////////////////////////////
//
// Handler-side active message support
//

static int numAmHandlersActive;
static pthread_cond_t amStartStopCond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t amStartStopMutex = PTHREAD_MUTEX_INITIALIZER;

static void amHandler(void*);
static void processRxAmReq(void);
static void processRxAmReqCQ(void);
static void processRxAmReqCntr(void);
static void amHandleExecOn(chpl_comm_on_bundle_t*);
static void amWrapExecOnBody(void*);
static void amHandleExecOnLrg(chpl_comm_on_bundle_t*);
static void amWrapExecOnLrgBody(struct amRequest_execOnLrg_t*);
static void amWrapGet(struct taskArg_RMA_t*);
static void amWrapPut(struct taskArg_RMA_t*);
static void amHandleAMO(struct amRequest_AMO_t*);
static void amHandleFAMOResult(struct amRequest_FAMO_result_t*);
static void amPutDone(c_nodeid_t, amDone_t*);
static void amCheckLiveness(void);

static void doCpuAMO(void*, const void*, const void*, void*,
                     enum fi_op, enum fi_datatype, size_t size);


static
void init_amHandling(void) {
  //
  // Sanity checks.
  //
  {
    chpl_comm_taskPrvData_t pd;
    CHK_TRUE(sizeof(pd.amDone) >= sizeof(amDone_t));
  }

  //
  // Start AM handler thread(s).  Don't proceed from here until at
  // least one is running.
  //
  atomic_init_bool(&amHandlersExit, false);
  PTHREAD_CHK(pthread_mutex_lock(&amStartStopMutex));
  for (int i = 0; i < numAmHandlers; i++) {
    CHK_TRUE(chpl_task_createCommTask(amHandler, NULL, reservedCPUs[i]) == 0);
  }
  PTHREAD_CHK(pthread_cond_wait(&amStartStopCond, &amStartStopMutex));
  PTHREAD_CHK(pthread_mutex_unlock(&amStartStopMutex));
}


static
void fini_amHandling(void) {
  if (chpl_numNodes <= 1)
    return;

  //
  // Tear down the AM handler thread(s).  On node 0, don't proceed from
  // here until the last one has finished.
  //
  PTHREAD_CHK(pthread_mutex_lock(&amStartStopMutex));
  atomic_store_bool(&amHandlersExit, true);
  // AM handler may be waiting on the receive counter. Break it out.
  if (ofi_rxCntr != NULL) {
    OFI_CHK(fi_cntr_add(ofi_rxCntr, 1));
  }

  PTHREAD_CHK(pthread_cond_wait(&amStartStopCond, &amStartStopMutex));
  PTHREAD_CHK(pthread_mutex_unlock(&amStartStopMutex));

  atomic_destroy_bool(&amHandlersExit);
}


//
// The AM handler runs this.
//
static __thread struct perTxCtxInfo_t* amTcip;

static
void amHandler(void* argNil) {
  struct perTxCtxInfo_t* tcip;
  CHK_TRUE((tcip = tciAllocForAmHandler()) != NULL);
  amTcip = tcip;

  isAmHandler = true;

  DBG_PRINTF(DBG_AM, "AM handler running");

  //
  // Count this AM handler thread as running.  The creator thread
  // wants to be released as soon as at least one AM handler thread
  // is running, so if we're the first, do that.
  //
  PTHREAD_CHK(pthread_mutex_lock(&amStartStopMutex));
  if (++numAmHandlersActive == 1)
    PTHREAD_CHK(pthread_cond_signal(&amStartStopCond));
  PTHREAD_CHK(pthread_mutex_unlock(&amStartStopMutex));

  //
  // Process AM requests and watch transmit responses arrive.
  //
  while (!atomic_load_bool(&amHandlersExit)) {
    chpl_bool hadRxEvent, hadTxEvent;
    amCheckRxTxCmpls(&hadRxEvent, &hadTxEvent, tcip);
    if (hadRxEvent) {
      processRxAmReq();
    } else if (!hadTxEvent) {
      //
      // No activity; avoid CPU monopolization.
      //
      int ret;
      OFI_CHK_3(fi_wait(ofi_amhWaitSet, 100 /*ms*/), ret,
                -FI_EINTR, -FI_ETIMEDOUT);
    }

    if (amDoLivenessChecks) {
      amCheckLiveness();
    }
  }

  //
  // Un-count this AM handler thread.  Whoever told us to exit wants to
  // be released once all the AM handler threads are done, so if we're
  // the last, do that.
  //
  PTHREAD_CHK(pthread_mutex_lock(&amStartStopMutex));
  if (--numAmHandlersActive == 0)
    PTHREAD_CHK(pthread_cond_signal(&amStartStopCond));
  PTHREAD_CHK(pthread_mutex_unlock(&amStartStopMutex));

  DBG_PRINTF(DBG_AM, "AM handler done");
}

static
size_t handleAmReq(amRequest_t *req) {
  size_t size = 0;
  switch (req->b.op) {
    case am_opExecOn:
      DBG_PRINTF(DBG_AM | DBG_AM_RECV, "rx AM sizeof %lu argSize %lu", sizeof(req->xo.hdr), req->xo.hdr.comm.argSize);
      if (req->xo.hdr.comm.fast) {
        amWrapExecOnBody(&req->xo.hdr);
      } else {
        amHandleExecOn(&req->xo.hdr);
      }
      size = req->xo.hdr.comm.argSize;
      break;

    case am_opExecOnLrg:
      DBG_PRINTF(DBG_AM | DBG_AM_RECV, "rx AM sizeof %lu argSize %lu", sizeof(req->xol.hdr), req->xol.hdr.comm.argSize);
      amHandleExecOnLrg(&req->xol.hdr);
      size = sizeof(req->xol);
      break;

    case am_opGet:
      {
        struct taskArg_RMA_t arg = { .hdr.kind = CHPL_ARG_BUNDLE_KIND_TASK,
                                     .rma = req->rma, };
        chpl_task_startMovedTask(FID_NONE, (chpl_fn_p) amWrapGet,
                                 &arg, sizeof(arg), c_sublocid_any,
                                 chpl_nullTaskID);
      }
      size = sizeof(req->rma);
      break;

    case am_opPut:
      {
        struct taskArg_RMA_t arg = { .hdr.kind = CHPL_ARG_BUNDLE_KIND_TASK,
                                     .rma = req->rma, };
        chpl_task_startMovedTask(FID_NONE, (chpl_fn_p) amWrapPut,
                                 &arg, sizeof(arg), c_sublocid_any,
                                 chpl_nullTaskID);
      }
      size = sizeof(req->rma);
      break;

    case am_opAMO:
      amHandleAMO(&req->amo);
      size = sizeof(req->amo);
      break;

    case am_opFAMOResult:
      amHandleFAMOResult(&req->famo_result);
      size = sizeof(req->famo_result);
      break;

    case am_opFree:
      CHPL_FREE(req->free.p);
      size = sizeof(req->free);
      break;

    case am_opNop:
      DBG_PRINTF(DBG_AM | DBG_AM_RECV, "%s", am_reqDoneStr(req));
      if (req->b.pAmDone != NULL) {
        amPutDone(req->b.node, req->b.pAmDone);
      }
      size = sizeof(req->b);
      break;

    case am_opShutdown:
      chpl_signal_shutdown();
      size = sizeof(req->b);
      break;

    default:
      INTERNAL_ERROR_V("unexpected AM op %d", (int) req->b.op);
      break;
  }
  return size;
}

static
void processRxAmReqCntr(void) {
  //
  // Process requests received on the AM request endpoint.
  //

  OFI_CHK(fi_cntr_wait(ofi_rxCntr, ofi_rxCount+1, -1));
  uint64_t todo = fi_cntr_read(ofi_rxCntr) - ofi_rxCount;
  if (atomic_load_bool(&amHandlersExit)) {
    return;
  }
  if (todo == 0) {
    uint64_t errors = fi_cntr_readerr(ofi_rxCntr);
    if (errors > 0) {
      INTERNAL_ERROR_V("error count %" PRIu64, errors);
    }
  }
  size_t size;

  for (int i = 0; i < todo; i++) {
    // skip any padding and look for the next message

    char *ptr = ofi_rxBuffer;
    // limit how far we'll look for the message in the buffer
    char *horizon = ptr + 2 * sizeof(amRequest_t);
    if (horizon > (char *) ofi_rxEnd) {
      horizon = (char *) ofi_rxEnd;
    }
    for (; ptr < horizon && *ptr == '\0'; ptr++); // do nothing
    if (ptr >= horizon) {

      // look in the other buffer
      int other = 1 - ofi_msg_i;
      ptr = ofi_msg_reqs[other].msg_iov->iov_base;
      void *rxEnd = (void *) ((char *) ofi_msg_reqs[other].msg_iov->iov_base +
                            ofi_msg_reqs[other].msg_iov->iov_len);
      horizon = ptr + 2 * sizeof(amRequest_t);
      if (horizon > (char *) rxEnd) {
        horizon = (char *) rxEnd;
      }

      // skip any padding and look for the next message
      for (; ptr < horizon && *ptr == '\0'; ptr++); // do nothing
      CHK_TRUE(ptr < horizon);
      // found it. zero and repost current buffer, switch to other
      memset(ofi_msg_reqs[ofi_msg_i].msg_iov->iov_base, '\0',
             ofi_msg_reqs[ofi_msg_i].msg_iov->iov_len);
      OFI_CHK(fi_recvmsg(ofi_rxEp, &ofi_msg_reqs[ofi_msg_i], FI_MULTI_RECV));
      ofi_msg_i = other;
      ofi_rxEnd = rxEnd;
    }

    ofi_rxBuffer = ptr;

    //
    // This event is for an inbound AM request.  Handle it.
    //
    amRequest_t* req = (amRequest_t*) ofi_rxBuffer;
    DBG_PRINTF(DBG_AM_BUF,
               "CQ rx AM req @ buffer offset %zd seqId %s",
               (char*) req - (char*) ofi_iov_reqs[ofi_msg_i].iov_base,
               am_seqIdStr(req));
    DBG_PRINTF(DBG_AM | DBG_AM_RECV,
               "rx AM req: %s",
               am_reqStr(chpl_nodeID, req, 0));
    size = handleAmReq(req);
    ofi_rxBuffer = (void *) ((char *) ofi_rxBuffer +  size);
  }
  ofi_rxCount += todo;
}

static
void processRxAmReqCQ(void) {
  //
  // Process requests received on the AM request endpoint.
  //
  struct fi_cq_data_entry cqes[5];
  const size_t maxEvents = sizeof(cqes) / sizeof(cqes[0]);
  ssize_t ret;
  CHK_TRUE((ret = fi_cq_read(ofi_rxCQ, cqes, maxEvents)) > 0
           || ret == -FI_EAGAIN
           || ret == -FI_EAVAIL);
  if (ret == -FI_EAVAIL) {
    reportCQError(ofi_rxCQ);
  }

  const size_t numEvents = (ret == -FI_EAGAIN) ? 0 : ret;

  for (int i = 0; i < numEvents; i++) {
    if ((cqes[i].flags & FI_RECV) != 0) {
      //
      // This event is for an inbound AM request.  Handle it.
      //
      amRequest_t* req = (amRequest_t*) cqes[i].buf;
      DBG_PRINTF(DBG_AM_BUF,
                 "CQ rx AM req @ buffer offset %zd, sz %zd, seqId %s",
                 (char*) req - (char*) ofi_iov_reqs[ofi_msg_i].iov_base,
                 cqes[i].len, am_seqIdStr(req));
      DBG_PRINTF(DBG_AM | DBG_AM_RECV,
                 "rx AM req: %s",
                 am_reqStr(chpl_nodeID, req, cqes[i].len));
      (void) handleAmReq(req);
    }
    if ((cqes[i].flags & FI_MULTI_RECV) != 0) {
      //
      // Multi-receive buffer filled; post the other one.
      //
      ofi_msg_i = 1 - ofi_msg_i;
      OFI_CHK(fi_recvmsg(ofi_rxEp, &ofi_msg_reqs[ofi_msg_i], FI_MULTI_RECV));
      DBG_PRINTF(DBG_AM_BUF,
                 "re-post fi_recvmsg(AMLZs %p, len %#zx)",
                 ofi_msg_reqs[ofi_msg_i].msg_iov->iov_base,
                 ofi_msg_reqs[ofi_msg_i].msg_iov->iov_len);
    }

    CHK_TRUE((cqes[i].flags & ~(FI_MSG | FI_RECV | FI_MULTI_RECV)) == 0);
  }
}

static
void processRxAmReq(void) {
  if (ofi_rxCntr == NULL) {
    processRxAmReqCQ();
  } else {
    processRxAmReqCntr();
  }
}

static
void amHandleExecOn(chpl_comm_on_bundle_t* req) {
  chpl_comm_bundleData_t* comm = &req->comm;
  chpl_task_startMovedTask(comm->fid, (chpl_fn_p) amWrapExecOnBody, req,
                           comm->argSize, comm->subloc, chpl_nullTaskID);
}


static inline
void amWrapExecOnBody(void* p) {
  DBG_PRINTF(DBG_AM | DBG_AM_RECV, "%s", am_reqStartStr(p));

  chpl_comm_bundleData_t* comm = &((chpl_comm_on_bundle_t*) p)->comm;

  chpl_ftable_call(comm->fid, p);
  forceMemFxVisAllNodes_noTcip(true /*checkPuts*/, true /*checkAmos*/);
  DBG_PRINTF(DBG_AM | DBG_AM_RECV, "%s", am_reqDoneStr(p));
  if (comm->pAmDone != NULL) {
    amPutDone(comm->node, comm->pAmDone);
  }
}


static inline
void amHandleExecOnLrg(chpl_comm_on_bundle_t* req) {
  struct amRequest_execOnLrg_t* xol = (struct amRequest_execOnLrg_t*) req;
  xol->hdr.kind = am_opExecOn;  // was am_opExecOnLrg, to direct us here
  chpl_task_startMovedTask(FID_NONE, (chpl_fn_p) amWrapExecOnLrgBody,
                           xol, sizeof(*xol),
                           xol->hdr.comm.subloc, chpl_nullTaskID);
}


static
void amWrapExecOnLrgBody(struct amRequest_execOnLrg_t* xol) {
  DBG_PRINTF(DBG_AM | DBG_AM_RECV, "%s", am_reqStartStr((amRequest_t*) xol));

  //
  // TODO: We could stack-allocate "bundle" here, if it was small enough
  //       (TBD) not to create the potential for stack overflow.  Some
  //       systems have fast enough networks that saving the dynamic
  //       alloc should be performance-visible.
  //

  //
  // The bundle header is in our argument, but we have to retrieve the
  // payload from the initiating side.
  //
  chpl_comm_bundleData_t* comm = &xol->hdr.comm;
  c_nodeid_t node = comm->node;

  chpl_comm_on_bundle_t* bundle;
  CHPL_CALLOC_SZ(bundle, 1, comm->argSize);
  *bundle = xol->hdr;

  size_t payloadSize = comm->argSize
                       - offsetof(chpl_comm_on_bundle_t, payload);
  CHK_TRUE(mrGetKey(NULL, NULL, node, xol->pPayload, payloadSize));
  (void) ofi_get(&bundle->payload, node, xol->pPayload, payloadSize);

  //
  // Iff this is a nonblocking executeOn, now that we have the payload
  // we can free the copy of it on the initiating side.  In the blocking
  // case the initiator will free it if that is necessary, since they
  // have to wait for the whole executeOn to complete anyway.  We save
  // some time here by not waiting for a network response.  Either we or
  // someone else will consume that completion later.  In the meantime
  // we can go ahead with the executeOn body.
  //
  if (comm->pAmDone == NULL) {
    amRequestFree(node, xol->pPayload);
  }

  //
  // Now we can finally call the body function.
  //
  chpl_ftable_call(bundle->comm.fid, bundle);
  forceMemFxVisAllNodes_noTcip(true /*checkPuts*/, true /*checkAmos*/);
  DBG_PRINTF(DBG_AM | DBG_AM_RECV, "%s", am_reqDoneStr((amRequest_t*) xol));
  if (comm->pAmDone != NULL) {
    amPutDone(node, comm->pAmDone);
  }

  CHPL_FREE(bundle);
}


static
void amWrapGet(struct taskArg_RMA_t* tsk_rma) {
  struct amRequest_RMA_t* rma = &tsk_rma->rma;
  DBG_PRINTF(DBG_AM | DBG_AM_RECV, "%s", am_reqStartStr((amRequest_t*) rma));

  CHK_TRUE(mrGetKey(NULL, NULL, rma->b.node, rma->raddr, rma->size));
  (void) ofi_get(rma->addr, rma->b.node, rma->raddr, rma->size);

  DBG_PRINTF(DBG_AM | DBG_AM_RECV, "%s", am_reqDoneStr((amRequest_t*) rma));
  amPutDone(rma->b.node, rma->b.pAmDone);
}


static
void amWrapPut(struct taskArg_RMA_t* tsk_rma) {
  struct amRequest_RMA_t* rma = &tsk_rma->rma;
  DBG_PRINTF(DBG_AM | DBG_AM_RECV, "%s", am_reqStartStr((amRequest_t*) rma));

  CHK_TRUE(mrGetKey(NULL, NULL, rma->b.node, rma->raddr, rma->size));
  (void) ofi_put(rma->addr, rma->b.node, rma->raddr, rma->size);

  //
  // Note: the RMA bytes must be visible in target memory before the
  // 'done' indicator is.
  //

  DBG_PRINTF(DBG_AM | DBG_AM_RECV, "%s", am_reqDoneStr((amRequest_t*) rma));
  amPutDone(rma->b.node, rma->b.pAmDone);
}


static
void amHandleAMO(struct amRequest_AMO_t* amo) {
  DBG_PRINTF(DBG_AM | DBG_AM_RECV, "%s", am_reqStartStr((amRequest_t*) amo));
  assert(amo->b.node != chpl_nodeID);    // should be handled on initiator

  chpl_amo_datum_t result;
  size_t resSize = amo->size;
  doCpuAMO(amo->obj, &amo->opnd, &amo->cmpr, &result,
           amo->ofiOp, amo->ofiType, amo->size);

  if (amo->result != NULL) {
      // Use a non-blocking AMO to return the result and set pAmDone.
      DBG_PRINTF(DBG_AM | DBG_AM_RECV, "sending FAMO result via AM");
      amRequestFAMOResult(amo->b.node, &result, amo->ofiType, amo->result, resSize, amo->b.pAmDone);
  }

  // PUT the "done" flag if necessary
  if ((amo->b.pAmDone != NULL) && (amo->result == NULL)) {
      DBG_PRINTF(DBG_AM | DBG_AM_RECV,
                 "writing FAMO done flag via non-blocking PUT");
    amPutDone(amo->b.node, amo->b.pAmDone);
  }
  DBG_PRINTF(DBG_AM | DBG_AM_RECV, "%s", am_reqDoneStr((amRequest_t*) amo));
}


static inline
void amPutDone(c_nodeid_t node, amDone_t* pAmDone) {
  static __thread amDone_t* amDone = NULL;
  static __thread void* mrDesc;
  if (amDone == NULL) {
    amDone = allocBounceBuf(1);
    *amDone = 1;
    CHK_TRUE(mrGetDesc(&mrDesc, amDone, sizeof(amDone)));
  }

  struct perTxCtxInfo_t* tcip = amTcip;  // NULL if not AM handler task/thread
  if (tcip == NULL) {
    CHK_TRUE((tcip = tciAlloc()) != NULL);
  }

  //
  // Send the 'done' indicator.  If necessary, first make sure the
  // memory effects of all communication ops done by this AM are
  // visible.  We and the AM's initiator are the same Chapel task,
  // for purposes of MCM conformance, and the memory effects of the
  // AM must be visible to that initiator by the time it sees the
  // 'done' indicator.  In the delivery-complete mode everything is
  // already visible.  In the message-order mode we don't have to
  // force PUT visibility because this 'done' is also a PUT and we
  // use write-after-write message ordering, but we do have to force
  // AMO visibility.  In message-order-fence mode we have to force
  // both PUT and AMO visibility.
  //
  if (mcmMode != mcmm_dlvrCmplt) {
    forceMemFxVisAllNodes(mcmMode == mcmm_msgOrdFence /*checkPuts*/,
                          true /*checkAmos*/,
                          -1 /*skipNode*/, tcip);
  }

  uint64_t mrKey = 0;
  uint64_t mrRaddr = 0;
  uint64_t flags = 0;

  if (envInjectRMA) {
    flags = FI_INJECT;
  }
  CHK_TRUE(mrGetKey(&mrKey, &mrRaddr, node, pAmDone, sizeof(*pAmDone)));
  ofi_put_lowLevel(amDone, mrDesc, node, mrRaddr, mrKey, sizeof(*pAmDone),
                   txnTrkEncodeId(__LINE__), flags, tcip);

  if (amTcip == NULL) {
    tciFree(tcip);
  }
}


// amHandleFAMOResult

// This function is invoked to handle the result of a fetching AMO
// implemented via an AM. The scenario is that locale A has sent a
// fetching AMO to locale B via an AM. The AM handler on B sends back an
// AM to locale A that invokes this function to store the result of the
// fetching AMO and set the pAmDone flag.

static
void amHandleFAMOResult(struct amRequest_FAMO_result_t* famo) {
  DBG_PRINTF(DBG_AM | DBG_AM_RECV, "FAMO result");
  assert(famo->b.node != chpl_nodeID);    // should be handled on initiator

  memcpy(famo->res, &famo->result, famo->size);
  assert(famo->b.pAmDone != NULL);
  // make sure the object is written before pAmDone is set
  chpl_atomic_thread_fence(memory_order_release);
  *(famo->b.pAmDone) = 1;
}


static inline
void amCheckLiveness(void) {
  //
  // Only node 0 does liveness checks.  It cycles through the others,
  // checking to make sure we can AM to them.  To minimize overhead, we
  // try not to do a liveness check any more frequently than about every
  // 10 seconds and we also try not to make time calls much more often
  // than that, because they're expensive.  A "liveness check" is really
  // just a check that we can send a no-op AM without an unrecoverable
  // error resulting.  That's sufficient to get us an -EMFILE return if
  // we run up against the open file limit, for example.
  //
  const double timeInterval = 10.0;
  static __thread double lastTime = 0.0;

  static __thread int countInterval = 10000;
  static __thread int count;

  if (lastTime == 0.0) {
    //
    // The first time we've been called, initialize.
    //
    lastTime = chpl_comm_ofi_time_get();
    count = countInterval;
  } else if (--count == 0) {
    //
    // After the first time, do the "liveness" checks and adjust the
    // counter interval as needed.
    //
    double time = chpl_comm_ofi_time_get();

    double timeRatio = (time - lastTime) / timeInterval;
    const double minTimeRatio = 3.0 / 4.0;
    const double maxTimeRatio = 4.0 / 3.0;
    if (timeRatio < minTimeRatio) {
      timeRatio = minTimeRatio;
    } else if (timeRatio > maxTimeRatio) {
      timeRatio = maxTimeRatio;
    }
    countInterval /= timeRatio;

    static __thread c_nodeid_t node = 1;
    if (--node == 0) {
      node = chpl_numNodes - 1;
    }
    amRequestNop(node, false /*blocking*/, amTcip);
    count = countInterval;
    lastTime = time;
  }
}


////////////////////////////////////////
//
// Interface: RMA
//

chpl_comm_nb_handle_t chpl_comm_put_nb(void* addr, c_nodeid_t node,
                                       void* raddr, size_t size,
                                       int32_t commID, int ln, int32_t fn) {
  chpl_comm_put(addr, node, raddr, size, commID, ln, fn);
  return NULL;
}


chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, c_nodeid_t node,
                                       void* raddr, size_t size,
                                       int32_t commID, int ln, int32_t fn) {
  chpl_comm_get(addr, node, raddr, size, commID, ln, fn);
  return NULL;
}


int chpl_comm_test_nb_complete(chpl_comm_nb_handle_t h) {
  chpl_comm_diags_incr(test_nb);

  // fi_cq_readfrom?
  return ((void*) h) == NULL;
}


void chpl_comm_wait_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles) {
  chpl_comm_diags_incr(wait_nb);

  size_t i;
  // fi_cq_readfrom?
  for( i = 0; i < nhandles; i++ ) {
    CHK_TRUE(h[i] == NULL);
  }
}


int chpl_comm_try_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles) {
  chpl_comm_diags_incr(try_nb);

  size_t i;
  // fi_cq_readfrom?
  for( i = 0; i < nhandles; i++ ) {
    CHK_TRUE(h[i] == NULL);
  }
  return 0;
}


void chpl_comm_put(void* addr, c_nodeid_t node, void* raddr,
                   size_t size, int32_t commID, int ln, int32_t fn) {
  DBG_PRINTF(DBG_IFACE,
             "%s(%p, %d, %p, %zd, %d)", __func__,
             addr, (int) node, raddr, size, (int) commID);

  retireDelayedAmDone(false /*taskIsEnding*/);

  //
  // Sanity checks, self-communication.
  //
  CHK_TRUE(addr != NULL);
  CHK_TRUE(raddr != NULL);

  if (size == 0) {
    return;
  }

  if (node == chpl_nodeID) {
    memmove(raddr, addr, size);
    return;
  }

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_put)) {
      chpl_comm_cb_info_t cb_data =
        {chpl_comm_cb_event_kind_put, chpl_nodeID, node,
         .iu.comm={addr, raddr, size, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_rdma("put", node, size, ln, fn, commID);
  chpl_comm_diags_incr(put);

  (void) ofi_put(addr, node, raddr, size);
}


void chpl_comm_get(void* addr, int32_t node, void* raddr,
                   size_t size, int32_t commID, int ln, int32_t fn) {
  DBG_PRINTF(DBG_IFACE,
             "%s(%p, %d, %p, %zd, %d)", __func__,
             addr, (int) node, raddr, size, (int) commID);

  retireDelayedAmDone(false /*taskIsEnding*/);

  //
  // Sanity checks, self-communication.
  //
  CHK_TRUE(addr != NULL);
  CHK_TRUE(raddr != NULL);

  if (size == 0) {
    return;
  }

  if (node == chpl_nodeID) {
    memmove(addr, raddr, size);
    return;
  }

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_get)) {
      chpl_comm_cb_info_t cb_data =
        {chpl_comm_cb_event_kind_get, chpl_nodeID, node,
         .iu.comm={addr, raddr, size, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_rdma("get", node, size, ln, fn, commID);
  chpl_comm_diags_incr(get);

  (void) ofi_get(addr, node, raddr, size);
}


void chpl_comm_put_strd(void* dstaddr_arg, size_t* dststrides,
                        c_nodeid_t dstnode,
                        void* srcaddr_arg, size_t* srcstrides,
                        size_t* count, int32_t stridelevels, size_t elemSize,
                        int32_t commID, int ln, int32_t fn) {
  DBG_PRINTF(DBG_IFACE,
             "%s(%p, %p, %d, %p, %p, %p, %d, %zd, %d)", __func__,
             dstaddr_arg, dststrides, (int) dstnode, srcaddr_arg, srcstrides,
             count, (int) stridelevels, elemSize, (int) commID);

  put_strd_common(dstaddr_arg, dststrides,
                  dstnode,
                  srcaddr_arg, srcstrides,
                  count, stridelevels, elemSize,
                  1, NULL,
                  commID, ln, fn);
}


void chpl_comm_get_strd(void* dstaddr_arg, size_t* dststrides,
                        c_nodeid_t srcnode,
                        void* srcaddr_arg, size_t* srcstrides, size_t* count,
                        int32_t stridelevels, size_t elemSize,
                        int32_t commID, int ln, int32_t fn) {
  DBG_PRINTF(DBG_IFACE,
             "%s(%p, %p, %d, %p, %p, %p, %d, %zd, %d)", __func__,
             dstaddr_arg, dststrides, (int) srcnode, srcaddr_arg, srcstrides,
             count, (int) stridelevels, elemSize, (int) commID);

  get_strd_common(dstaddr_arg, dststrides,
                  srcnode,
                  srcaddr_arg, srcstrides,
                  count, stridelevels, elemSize,
                  1, NULL,
                  commID, ln, fn);
}


void chpl_comm_getput_unordered(c_nodeid_t dstnode, void* dstaddr,
                                c_nodeid_t srcnode, void* srcaddr,
                                size_t size, int32_t commID,
                                int ln, int32_t fn) {
  DBG_PRINTF(DBG_IFACE,
             "%s(%d, %p, %d, %p, %zd, %d)", __func__,
             (int) dstnode, dstaddr, (int) srcnode, srcaddr, size,
             (int) commID);

  assert(dstaddr != NULL);
  assert(srcaddr != NULL);

  if (size == 0)
    return;

  if (dstnode == chpl_nodeID && srcnode == chpl_nodeID) {
    retireDelayedAmDone(false /*taskIsEnding*/);
    memmove(dstaddr, srcaddr, size);
    return;
  }

  if (dstnode == chpl_nodeID) {
    chpl_comm_get_unordered(dstaddr, srcnode, srcaddr, size, commID, ln, fn);
  } else if (srcnode == chpl_nodeID) {
    chpl_comm_put_unordered(srcaddr, dstnode, dstaddr, size, commID, ln, fn);
  } else {
    if (size <= MAX_UNORDERED_TRANS_SZ) {
      char buf[MAX_UNORDERED_TRANS_SZ];
      chpl_comm_get(buf, srcnode, srcaddr, size, commID, ln, fn);
      chpl_comm_put(buf, dstnode, dstaddr, size, commID, ln, fn);
    } else {
      // Note, we do not expect this case to trigger, but if it does we may
      // want to do on-stmt to src node and then transfer
      char* buf = chpl_mem_alloc(size, CHPL_RT_MD_COMM_PER_LOC_INFO, 0, 0);
      chpl_comm_get(buf, srcnode, srcaddr, size, commID, ln, fn);
      chpl_comm_put(buf, dstnode, dstaddr, size, commID, ln, fn);
      chpl_mem_free(buf, 0, 0);
    }
  }
}


void chpl_comm_get_unordered(void* addr, c_nodeid_t node, void* raddr,
                             size_t size, int32_t commID, int ln, int32_t fn) {
  DBG_PRINTF(DBG_IFACE,
             "%s(%p, %d, %p, %zd, %d)", __func__,
             addr, (int) node, raddr, size, (int) commID);

  retireDelayedAmDone(false /*taskIsEnding*/);

  //
  // Sanity checks, self-communication.
  //
  CHK_TRUE(addr != NULL);
  CHK_TRUE(raddr != NULL);

  if (size == 0) {
    return;
  }

  if (node == chpl_nodeID) {
    memmove(addr, raddr, size);
    return;
  }

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_get)) {
      chpl_comm_cb_info_t cb_data =
        {chpl_comm_cb_event_kind_get, chpl_nodeID, node,
         .iu.comm={addr, raddr, size, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_rdma("unordered get", node, size, ln, fn, commID);
  chpl_comm_diags_incr(get);

  do_remote_get_buff(addr, node, raddr, size);
}


void chpl_comm_put_unordered(void* addr, c_nodeid_t node, void* raddr,
                             size_t size, int32_t commID, int ln, int32_t fn) {
  DBG_PRINTF(DBG_IFACE,
             "%s(%p, %d, %p, %zd, %d)", __func__,
             addr, (int) node, raddr, size, (int) commID);

  retireDelayedAmDone(false /*taskIsEnding*/);

  //
  // Sanity checks, self-communication.
  //
  CHK_TRUE(addr != NULL);
  CHK_TRUE(raddr != NULL);

  if (size == 0) {
    return;
  }

  if (node == chpl_nodeID) {
    memmove(raddr, addr, size);
    return;
  }

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_put)) {
      chpl_comm_cb_info_t cb_data =
        {chpl_comm_cb_event_kind_put, chpl_nodeID, node,
         .iu.comm={addr, raddr, size, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_rdma("unordered put", node, size, ln, fn, commID);
  chpl_comm_diags_incr(put);

  do_remote_put_buff(addr, node, raddr, size);
}


void chpl_comm_getput_unordered_task_fence(void) {
  DBG_PRINTF(DBG_IFACE_MCM, "%s()", __func__);

  task_local_buff_flush(get_buff | put_buff);
}


////////////////////////////////////////
//
// Internal communication support
//

static struct perTxCtxInfo_t* tciAllocCommon(chpl_bool);
static struct perTxCtxInfo_t* findFreeTciTabEntry(chpl_bool);

static __thread struct perTxCtxInfo_t* _ttcip;


static inline
struct perTxCtxInfo_t* tciAlloc(void) {
  return tciAllocCommon(false /*bindToAmHandler*/);
}


static inline
struct perTxCtxInfo_t* tciAllocForAmHandler(void) {
  return tciAllocCommon(true /*bindToAmHandler*/);
}


static inline
struct perTxCtxInfo_t* tciAllocCommon(chpl_bool bindToAmHandler) {
  if (_ttcip != NULL) {
    //
    // If the last tx context we used is bound to our thread or can be
    // re-allocated, use that.
    //
    if (_ttcip->bound) {
      DBG_PRINTF(DBG_TCIPS, "realloc bound tciTab[%td]", _ttcip - tciTab);
      return _ttcip;
    }

    if (tciAllocTabEntry(_ttcip)) {
      DBG_PRINTF(DBG_TCIPS, "realloc tciTab[%td]", _ttcip - tciTab);
      return _ttcip;
    }
  }

  //
  // Find a tx context that isn't busy and use that one.  If this is
  // for either the AM handler or a tasking layer fixed worker thread,
  // bind it permanently.
  //
  _ttcip = findFreeTciTabEntry(bindToAmHandler);
  if (bindToAmHandler
      || (tciTabBindTxCtxs && chpl_task_isFixedThread())) {
    _ttcip->bound = true;
    _ttcip->putVisBitmap = bitmapAlloc(chpl_numNodes);
    if ((ofi_info->caps & FI_ATOMIC) != 0) {
      _ttcip->amoVisBitmap = bitmapAlloc(chpl_numNodes);
    }
  }
  DBG_PRINTF(DBG_TCIPS, "alloc%s tciTab[%td]",
             _ttcip->bound ? " bound" : "", _ttcip - tciTab);
  return _ttcip;
}


static
struct perTxCtxInfo_t* findFreeTciTabEntry(chpl_bool bindToAmHandler) {
  //
  // Find a tx context that isn't busy.  Note that tx contexts for
  // AM handlers and other threads come out of different blocks of
  // the table.
  //
  const int numWorkerTxCtxs = tciTabLen - numAmHandlers;
  struct perTxCtxInfo_t* tcip;

  CHK_TRUE(numWorkerTxCtxs > 0);

  if (bindToAmHandler) {
    //
    // AM handlers use tciTab[numWorkerTxCtxs .. tciTabLen - 1].  For
    // now we only support a single AM handler, so this is simple.  If
    // we ever have more, the CHK_FALSE will force us to revisit this.
    //
    tcip = &tciTab[numWorkerTxCtxs];
    CHK_TRUE(tciAllocTabEntry(tcip));
    return tcip;
  }

  //
  // Workers use tciTab[0 .. numWorkerTxCtxs - 1].  Search forever for
  // an entry we can use.  Give up (and kill the program) only if we
  // discover they're all bound, because if that's true we can predict
  // we'll never find a free one.
  //
  static __thread int last_iw = 0;
  tcip = NULL;

  do {
    int iw = last_iw;
    chpl_bool allBound = true;

    do {
      if (++iw >= numWorkerTxCtxs)
        iw = 0;
      allBound = allBound && tciTab[iw].bound;
      if (tciAllocTabEntry(&tciTab[iw])) {
        tcip = &tciTab[iw];
      }
    } while (tcip == NULL && iw != last_iw);

    if (tcip == NULL) {
      CHK_FALSE(allBound);
      local_yield();
    }
  } while (tcip == NULL);

  return tcip;
}


//
// This returns true if you successfully allocated the given tciTab
// entry and false otherwise.
//
static inline
chpl_bool tciAllocTabEntry(struct perTxCtxInfo_t* tcip) {
  return(!atomic_exchange_bool(&tcip->allocated, true));
}


static inline
void tciFree(struct perTxCtxInfo_t* tcip) {
  //
  // Bound contexts stay bound.  We only release non-bound ones.
  //
  if (!tcip->bound) {
    DBG_PRINTF(DBG_TCIPS, "free tciTab[%td]", tcip - tciTab);
    atomic_store_bool(&tcip->allocated, false);
  }
}


static inline
void waitForCQSpace(struct perTxCtxInfo_t* tcip, size_t len) {
  //
  // Make sure we have at least the given number of free CQ entries.
  //
  if (tcip->txCQ != NULL && txCQLen - tcip->numTxnsOut < len) {
    (*tcip->checkTxCmplsFn)(tcip);
    while (txCQLen - tcip->numTxnsOut < len) {
      sched_yield();
      (*tcip->checkTxCmplsFn)(tcip);
    }
  }
}


typedef chpl_comm_nb_handle_t (rmaPutFn_t)(void* myAddr, void* mrDesc,
                                           c_nodeid_t node,
                                           uint64_t mrRaddr, uint64_t mrKey,
                                           size_t size,
                                           struct perTxCtxInfo_t* tcip);

static rmaPutFn_t rmaPutFn_selector;

static inline
chpl_comm_nb_handle_t ofi_put(const void* addr, c_nodeid_t node,
                              void* raddr, size_t size) {
  //
  // Don't ask the provider to transfer more than it wants to.
  //
  if (size > ofi_info->ep_attr->max_msg_size) {
    DBG_PRINTF(DBG_RMA | DBG_RMA_WRITE,
               "splitting large PUT %d:%p <= %p, size %zd",
               (int) node, raddr, addr, size);

    size_t chunkSize = ofi_info->ep_attr->max_msg_size;
    for (size_t i = 0; i < size; i += chunkSize) {
      if (chunkSize > size - i) {
        chunkSize = size - i;
      }
      (void) ofi_put(&((const char*) addr)[i], node, &((char*) raddr)[i],
                     chunkSize);
    }

    return NULL;
  }

  DBG_PRINTF(DBG_RMA | DBG_RMA_WRITE,
             "PUT %d:%p <= %p, size %zd",
             (int) node, raddr, addr, size);

  //
  // If the remote address is directly accessible do an RMA from this
  // side; otherwise do the opposite RMA from the other side.
  //
  chpl_comm_nb_handle_t ret;
  uint64_t mrKey;
  uint64_t mrRaddr;
  if (mrGetKey(&mrKey, &mrRaddr, node, raddr, size)) {
    struct perTxCtxInfo_t* tcip;
    CHK_TRUE((tcip = tciAlloc()) != NULL);
    if (tcip->txCntr == NULL) {
      waitForCQSpace(tcip, 1);
    }

    void* mrDesc;
    void* myAddr = mrLocalizeSource(&mrDesc, addr, size, "PUT src");

    ret = rmaPutFn_selector(myAddr, mrDesc, node, mrRaddr, mrKey, size,
                            tcip);

    mrUnLocalizeSource(myAddr, addr);
    tciFree(tcip);
  } else {
    amRequestRmaPut(node, (void*) addr, raddr, size);
    ret = NULL;
  }

  return ret;
}


static rmaPutFn_t rmaPutFn_msgOrdFence;
static rmaPutFn_t rmaPutFn_msgOrd;
static rmaPutFn_t rmaPutFn_dlvrCmplt;

static inline
chpl_comm_nb_handle_t rmaPutFn_selector(void* myAddr, void* mrDesc,
                                        c_nodeid_t node,
                                        uint64_t mrRaddr, uint64_t mrKey,
                                        size_t size,
                                        struct perTxCtxInfo_t* tcip) {
  chpl_comm_nb_handle_t ret = NULL;

  switch (mcmMode) {
  case mcmm_msgOrdFence:
    ret = rmaPutFn_msgOrdFence(myAddr, mrDesc, node, mrRaddr, mrKey, size,
                               tcip);
    break;
  case mcmm_msgOrd:
    ret = rmaPutFn_msgOrd(myAddr, mrDesc, node, mrRaddr, mrKey, size,
                          tcip);
    break;
  case mcmm_dlvrCmplt:
    ret = rmaPutFn_dlvrCmplt(myAddr, mrDesc, node, mrRaddr, mrKey, size,
                             tcip);
    break;
  default:
    INTERNAL_ERROR_V("unexpected mcmMode %d", mcmMode);
    break;
  }

  return ret;
}


static ssize_t wrap_fi_write(const void* addr, void* mrDesc,
                             c_nodeid_t node,
                             uint64_t mrRaddr, uint64_t mrKey,
                             size_t size, void* ctx,
                             struct perTxCtxInfo_t* tcip);
static ssize_t wrap_fi_inject_write(const void* addr,
                                    c_nodeid_t node,
                                    uint64_t mrRaddr, uint64_t mrKey,
                                    size_t size,
                                    struct perTxCtxInfo_t* tcip);
static ssize_t wrap_fi_writemsg(const void* addr, void* mrDesc,
                                c_nodeid_t node,
                                uint64_t mrRaddr, uint64_t mrKey,
                                size_t size, void* ctx, uint64_t flags,
                                struct perTxCtxInfo_t* tcip);



//
// Implements ofi_put() when MCM mode is message ordering with fences.
//
static
chpl_comm_nb_handle_t rmaPutFn_msgOrdFence(void* myAddr, void* mrDesc,
                                           c_nodeid_t node,
                                           uint64_t mrRaddr, uint64_t mrKey,
                                           size_t size,
                                           struct perTxCtxInfo_t* tcip) {
  if (tcip->bound
      && size <= ofi_info->tx_attr->inject_size
      && (tcip->amoVisBitmap == NULL
          || !bitmapTest(tcip->amoVisBitmap, node))
      && envInjectRMA) {
    //
    // Special case: write injection has the least latency.  We can use
    // that if this PUT doesn't need a fence, its size doesn't exceed
    // the injection size limit, and we have a bound tx context so we
    // can delay forcing the memory visibility until later.
    //
    (void) wrap_fi_inject_write(myAddr, node, mrRaddr, mrKey, size, tcip);
  } else {
    atomic_bool txnDone;
    void *ctx = txCtxInit(tcip, __LINE__, &txnDone);

    if (tcip->bound
        && bitmapTest(tcip->amoVisBitmap, node)) {
      //
      // Special case: If our last operation was an AMO (which can only
      // be true with a bound tx context) then we need to do a fenced
      // PUT to force the AMO to complete before this PUT.  We may still
      // be able to inject the PUT, though.
      //
      uint64_t flags = FI_FENCE;
      if (size <= ofi_info->tx_attr->inject_size
          && envInjectRMA) {
        flags |= FI_INJECT;
      }
      (void) wrap_fi_writemsg(myAddr, mrDesc, node, mrRaddr, mrKey, size,
                              ctx, flags, tcip);
    } else {
      //
      // General case.
      //
      (void) wrap_fi_write(myAddr, mrDesc, node, mrRaddr, mrKey, size,
                           ctx, tcip);
    }

    waitForTxnComplete(tcip, ctx);
    txCtxCleanup(ctx);
  }

  //
  // When using message ordering we have to do something after the PUT
  // to force it into visibility, and on the same tx context as the PUT
  // itself because libfabric message ordering is specific to endpoint
  // pairs.  With a bound tx context we can do it later, when needed.
  // Otherwise we have to do it here, before we release the tx context.
  //
  if (tcip->bound) {
    bitmapSet(tcip->putVisBitmap, node);
  } else {
    mcmReleaseOneNode(node, tcip, "PUT");
  }

  return NULL;
}


//
// Implements ofi_put() when MCM mode is message ordering.
//
static
chpl_comm_nb_handle_t rmaPutFn_msgOrd(void* myAddr, void* mrDesc,
                                      c_nodeid_t node,
                                      uint64_t mrRaddr, uint64_t mrKey,
                                      size_t size,
                                      struct perTxCtxInfo_t* tcip) {
  //
  // When using message ordering we have to do something after the PUT
  // to force it into visibility, and on the same tx context as the PUT
  // itself because libfabric message ordering is specific to endpoint
  // pairs.  With a bound tx context we can do it later, when needed.
  // Otherwise we have to do it here, before we release the tx context.
  //

  if (tcip->bound
      && size <= ofi_info->tx_attr->inject_size
      && envInjectRMA) {
    //
    // Special case: write injection has the least latency.  We can use
    // that if this PUT's size doesn't exceed the injection size limit
    // and we have a bound tx context so we can delay forcing the
    // memory visibility until later.
    //
    (void) wrap_fi_inject_write(myAddr, node, mrRaddr, mrKey, size, tcip);
  } else {
    //
    // General case.
    //
    atomic_bool txnDone;
    void *ctx = txCtxInit(tcip, __LINE__, &txnDone);
    (void) wrap_fi_write(myAddr, mrDesc, node, mrRaddr, mrKey, size,
                         ctx, tcip);
    waitForTxnComplete(tcip, ctx);
    txCtxCleanup(ctx);
  }

  if (tcip->bound) {
    bitmapSet(tcip->putVisBitmap, node);
  } else {
    mcmReleaseOneNode(node, tcip, "PUT");
  }

  return NULL;
}


//
// Implements ofi_put() when MCM mode is delivery complete.
//
static
chpl_comm_nb_handle_t rmaPutFn_dlvrCmplt(void* myAddr, void* mrDesc,
                                         c_nodeid_t node,
                                         uint64_t mrRaddr, uint64_t mrKey,
                                         size_t size,
                                         struct perTxCtxInfo_t* tcip) {
  atomic_bool txnDone;
  void *ctx = txCtxInit(tcip, __LINE__, &txnDone);
  (void) wrap_fi_write(myAddr, mrDesc, node, mrRaddr, mrKey,
                       size, ctx, tcip);
  waitForTxnComplete(tcip, ctx);
  txCtxCleanup(ctx);
  return NULL;
}


static inline
ssize_t wrap_fi_write(const void* addr, void* mrDesc,
                      c_nodeid_t node,
                      uint64_t mrRaddr, uint64_t mrKey,
                      size_t size, void* ctx,
                      struct perTxCtxInfo_t* tcip) {
  DBG_PRINTF(DBG_RMA | DBG_RMA_WRITE,
             "tx write: %d:%#" PRIx64 " <= %p, size %zd, ctx %p",
             (int) node, mrRaddr, addr, size, ctx);
  OFI_RIDE_OUT_EAGAIN(tcip,
                      fi_write(tcip->txCtx, addr, size,
                               mrDesc, rxAddr(tcip, node),
                               mrRaddr, mrKey, ctx));
  tcip->numTxnsOut++;
  tcip->numTxnsSent++;
  return FI_SUCCESS;
}


static inline
ssize_t wrap_fi_inject_write(const void* addr,
                             c_nodeid_t node,
                             uint64_t mrRaddr, uint64_t mrKey,
                             size_t size,
                             struct perTxCtxInfo_t* tcip) {
  DBG_PRINTF(DBG_RMA | DBG_RMA_WRITE,
             "tx write inject: %d:%#" PRIx64 " <= %p, size %zd",
             (int) node, mrRaddr, addr, size);
  // TODO: How quickly/often does local resource throttling happen?
  OFI_RIDE_OUT_EAGAIN(tcip,
                      fi_inject_write(tcip->txCtx, addr, size,
                                      rxAddr(tcip, node),
                                      mrRaddr, mrKey));
  tcip->numTxnsSent++;
  return FI_SUCCESS;
}


static inline
ssize_t wrap_fi_writemsg(const void* addr, void* mrDesc,
                         c_nodeid_t node,
                         uint64_t mrRaddr, uint64_t mrKey,
                         size_t size, void* ctx, uint64_t flags,
                         struct perTxCtxInfo_t* tcip) {
  struct iovec msg_iov = (struct iovec)
                         { .iov_base = (void*) addr,
                           .iov_len = size };
  struct fi_rma_iov rma_iov = (struct fi_rma_iov)
                              { .addr = (uint64_t) mrRaddr,
                                .len = size,
                                .key = mrKey };
  struct fi_msg_rma msg = (struct fi_msg_rma)
                          { .msg_iov = &msg_iov,
                            .desc = &mrDesc,
                            .iov_count = 1,
                            .addr = rxAddr(tcip, node),
                            .rma_iov = &rma_iov,
                            .rma_iov_count = 1,
                            .context = ctx };

  if ((flags & FI_INJECT) && (size > ofi_info->tx_attr->inject_size)) {
    flags &= ~FI_INJECT;
  }
  DBG_PRINTF(DBG_RMA | DBG_RMA_WRITE,
             "tx write msg: %d:%#" PRIx64 " <= %p, size %zd, ctx %p, "
             "flags %#" PRIx64,
             (int) node, mrRaddr, addr, size, ctx, flags);
  OFI_RIDE_OUT_EAGAIN(tcip, fi_writemsg(tcip->txCtx, &msg, flags));
  tcip->numTxnsOut++;
  tcip->numTxnsSent++;
  return FI_SUCCESS;
}


static inline
void ofi_put_lowLevel(const void* addr, void* mrDesc, c_nodeid_t node,
                      uint64_t mrRaddr, uint64_t mrKey, size_t size,
                      void* ctx, uint64_t flags,
                      struct perTxCtxInfo_t* tcip) {
  if (flags == FI_INJECT
      && size <= ofi_info->tx_attr->inject_size
      && envInjectRMA) {
    (void) wrap_fi_inject_write(addr, node, mrRaddr, mrKey, size, tcip);
  } else if (flags == 0) {
    (void) wrap_fi_write(addr, mrDesc, node, mrRaddr, mrKey, size, ctx, tcip);
  } else {
    (void) wrap_fi_writemsg(addr, mrDesc, node, mrRaddr, mrKey, size, ctx,
                            flags, tcip);
  }
}


static
void ofi_put_V(int v_len, void** addr_v, void** local_mr_v,
               c_nodeid_t* locale_v, void** raddr_v, uint64_t* remote_mr_v,
               size_t* size_v) {
  DBG_PRINTF(DBG_RMA | DBG_RMA_WRITE | DBG_RMA_UNORD,
             "put_V(%d): %d:%p <= %p, size %zd, key 0x%" PRIx64,
             v_len, (int) locale_v[0], raddr_v[0], addr_v[0], size_v[0],
             remote_mr_v[0]);

  assert(!isAmHandler);

  struct perTxCtxInfo_t* tcip;
  CHK_TRUE((tcip = tciAlloc()) != NULL);

  //
  // Make sure we have enough free CQ entries to initiate the entire
  // batch of transactions.
  //
  waitForCQSpace(tcip, v_len);

  //
  // Initiate the batch.  Record which nodes we PUT to, so that we can
  // force them to be visible in target memory at the end.
  //
  if (tcip->putVisBitmap == NULL) {
    tcip->putVisBitmap = bitmapAlloc(chpl_numNodes);
  }

  for (int vi = 0; vi < v_len; vi++) {
    (void) wrap_fi_writemsg(addr_v[vi], local_mr_v[vi],
                            locale_v[vi],
                            (uint64_t) raddr_v[vi], remote_mr_v[vi],
                            size_v[vi], txnTrkEncodeId(__LINE__),
                            ((vi < v_len - 1) ? FI_MORE : 0), tcip);
    bitmapSet(tcip->putVisBitmap, locale_v[vi]);
  }

  //
  // Enforce Chapel MCM: force all of the above PUTs to appear in
  // target memory.
  //
  mcmReleaseAllNodes(tcip->putVisBitmap, NULL, -1 /*skipNode*/, tcip,
                     "unordered PUT");

  tciFree(tcip);
}


/*
 *** START OF BUFFERED PUT OPERATIONS ***
 *
 * Support for buffered PUT operations. We internally buffer PUT operations and
 * then initiate them all at once for increased transaction rate.
 */

// Flush buffered PUTs for the specified task info and reset the counter.
static inline
void put_buff_task_info_flush(put_buff_task_info_t* info) {
  if (info->vi > 0) {
    DBG_PRINTF(DBG_RMA_UNORD,
               "put_buff_task_info_flush(): info has %d entries",
               info->vi);
    ofi_put_V(info->vi, info->src_addr_v, info->local_mr_v,
              info->locale_v, info->tgt_addr_v, info->remote_mr_v,
              info->size_v);
    info->vi = 0;
  }
}


static inline
void do_remote_put_buff(void* addr, c_nodeid_t node, void* raddr,
                        size_t size) {
  uint64_t mrKey;
  uint64_t mrRaddr;
  put_buff_task_info_t* info;
  if (size > MAX_UNORDERED_TRANS_SZ
      || !mrGetKey(&mrKey, &mrRaddr, node, raddr, size)
      || (info = task_local_buff_acquire(put_buff)) == NULL) {
    (void) ofi_put(addr, node, raddr, size);
    return;
  }

  void* mrDesc;
  CHK_TRUE(mrGetDesc(&mrDesc, info->src_v, size));

  int vi = info->vi;
  memcpy(&info->src_v[vi], addr, size);
  info->src_addr_v[vi] = &info->src_v[vi];
  info->locale_v[vi] = node;
  info->tgt_addr_v[vi] = (void*) mrRaddr;
  info->size_v[vi] = size;
  info->remote_mr_v[vi] = mrKey;
  info->local_mr_v[vi] = mrDesc;
  info->vi++;

  DBG_PRINTF(DBG_RMA_UNORD,
             "do_remote_put_buff(): info[%d] = "
             "{%p, %d, %p, %zd, %" PRIx64 ", %p}",
             vi, info->src_addr_v[vi], (int) node, raddr, size, mrKey, mrDesc);

  // flush if buffers are full
  if (info->vi == MAX_CHAINED_PUT_LEN) {
    put_buff_task_info_flush(info);
  }
}
/*** END OF BUFFERED PUT OPERATIONS ***/


typedef chpl_comm_nb_handle_t (rmaGetFn_t)(void* myAddr, void* mrDesc,
                                           c_nodeid_t node,
                                           uint64_t mrRaddr, uint64_t mrKey,
                                           size_t size, void* ctx,
                                           struct perTxCtxInfo_t* tcip);

static rmaGetFn_t rmaGetFn_selector;

static inline
chpl_comm_nb_handle_t ofi_get(void* addr, c_nodeid_t node,
                              void* raddr, size_t size) {
  //
  // Don't ask the provider to transfer more than it wants to.
  //
  if (size > ofi_info->ep_attr->max_msg_size) {
    DBG_PRINTF(DBG_RMA | DBG_RMA_READ,
               "splitting large GET %p <= %d:%p, size %zd",
               addr, (int) node, raddr, size);

    size_t chunkSize = ofi_info->ep_attr->max_msg_size;
    for (size_t i = 0; i < size; i += chunkSize) {
      if (chunkSize > size - i) {
        chunkSize = size - i;
      }
      (void) ofi_get(&((char*) addr)[i], node, &((char*) raddr)[i],
                     chunkSize);
    }

    return NULL;
  }

  DBG_PRINTF(DBG_RMA | DBG_RMA_READ,
             "GET %p <= %d:%p, size %zd",
             addr, (int) node, raddr, size);

  //
  // If the remote address is directly accessible do an RMA from this
  // side; otherwise do the opposite RMA from the other side.
  //
  chpl_comm_nb_handle_t ret;
  uint64_t mrKey;
  uint64_t mrRaddr;
  if (mrGetKey(&mrKey, &mrRaddr, node, raddr, size)) {
    struct perTxCtxInfo_t* tcip;
    CHK_TRUE((tcip = tciAlloc()) != NULL);
    waitForCQSpace(tcip, 1);

    void* mrDesc;
    void* myAddr = mrLocalizeTarget(&mrDesc, addr, size, "GET tgt");

    atomic_bool txnDone;
    void *ctx = txCtxInit(tcip, __LINE__, &txnDone);
    ret = rmaGetFn_selector(myAddr, mrDesc, node, mrRaddr, mrKey, size,
                            ctx, tcip);

    waitForTxnComplete(tcip, ctx);
    txCtxCleanup(ctx);
    mrUnLocalizeTarget(myAddr, addr, size);
    tciFree(tcip);
  } else {
    amRequestRmaGet(node, addr, raddr, size);
    ret = NULL;
  }

  return ret;
}


static rmaGetFn_t rmaGetFn_msgOrdFence;
static rmaGetFn_t rmaGetFn_msgOrd;
static rmaGetFn_t rmaGetFn_dlvrCmplt;


static inline
chpl_comm_nb_handle_t rmaGetFn_selector(void* myAddr, void* mrDesc,
                                        c_nodeid_t node,
                                        uint64_t mrRaddr, uint64_t mrKey,
                                        size_t size, void* ctx,
                                        struct perTxCtxInfo_t* tcip) {
  chpl_comm_nb_handle_t ret = NULL;

  switch (mcmMode) {
  case mcmm_msgOrdFence:
    ret = rmaGetFn_msgOrdFence(myAddr, mrDesc, node, mrRaddr, mrKey, size,
                               ctx, tcip);
    break;
  case mcmm_msgOrd:
    ret = rmaGetFn_msgOrd(myAddr, mrDesc, node, mrRaddr, mrKey, size,
                          ctx, tcip);
    break;
  case mcmm_dlvrCmplt:
    ret = rmaGetFn_dlvrCmplt(myAddr, mrDesc, node, mrRaddr, mrKey, size,
                             ctx, tcip);
    break;
  default:
    INTERNAL_ERROR_V("unexpected mcmMode %d", mcmMode);
    break;
  }

  return ret;
}


static ssize_t wrap_fi_read(void* addr, void* mrDesc,
                            c_nodeid_t node,
                            uint64_t mrRaddr, uint64_t mrKey,
                            size_t size, void* ctx,
                            struct perTxCtxInfo_t* tcip);
static ssize_t wrap_fi_readmsg(void* addr, void* mrDesc,
                               c_nodeid_t node,
                               uint64_t mrRaddr, uint64_t mrKey,
                               size_t size, void* ctx, uint64_t flags,
                               struct perTxCtxInfo_t* tcip);


//
// Implements ofi_get() when MCM mode is message ordering with fences.
//
static
chpl_comm_nb_handle_t rmaGetFn_msgOrdFence(void* myAddr, void* mrDesc,
                                           c_nodeid_t node,
                                           uint64_t mrRaddr, uint64_t mrKey,
                                           size_t size, void* ctx,
                                           struct perTxCtxInfo_t* tcip) {
  chpl_bool havePutsOut = tcip->putVisBitmap != NULL
                          && bitmapTest(tcip->putVisBitmap, node);
  chpl_bool haveAmosOut = tcip->amoVisBitmap != NULL
                          && bitmapTest(tcip->amoVisBitmap, node);

  if (havePutsOut || haveAmosOut) {
    //
    // Special case: If our last operation was a write whose memory
    // effects might not be visible yet (which can only be true with
    // a bound tx context) then this GET needs to be fenced to force
    // that visibility.
    //
    (void) wrap_fi_readmsg(myAddr, mrDesc, node, mrRaddr, mrKey, size, ctx,
                           FI_FENCE, tcip);
    if (havePutsOut) {
      bitmapClear(tcip->putVisBitmap, node);
    }
    if (haveAmosOut) {
      bitmapClear(tcip->amoVisBitmap, node);
    }
  } else {
    //
    // General case.
    //
    (void) wrap_fi_read(myAddr, mrDesc, node, mrRaddr, mrKey, size, ctx, tcip);
  }

  return NULL;
}


//
// Implements ofi_get() when MCM mode is message ordering.
//
static
chpl_comm_nb_handle_t rmaGetFn_msgOrd(void* myAddr, void* mrDesc,
                                      c_nodeid_t node,
                                      uint64_t mrRaddr, uint64_t mrKey,
                                      size_t size, void* ctx,
                                      struct perTxCtxInfo_t* tcip) {
  //
  // This GET will force any outstanding PUT to the same node to be
  // visible.
  //
  if (tcip->bound) {
    if (tcip->putVisBitmap != NULL) {
      bitmapClear(tcip->putVisBitmap, node);
    }
    if (tcip->amoVisBitmap != NULL) {
      bitmapClear(tcip->amoVisBitmap, node);
    }
  }
  (void) wrap_fi_read(myAddr, mrDesc, node, mrRaddr, mrKey, size, ctx, tcip);
  return NULL;
}


//
// Implements ofi_get() when MCM mode is delivery complete.
//
static
chpl_comm_nb_handle_t rmaGetFn_dlvrCmplt(void* myAddr, void* mrDesc,
                                         c_nodeid_t node,
                                         uint64_t mrRaddr, uint64_t mrKey,
                                         size_t size, void* ctx,
                                         struct perTxCtxInfo_t* tcip) {
  (void) wrap_fi_read(myAddr, mrDesc, node, mrRaddr, mrKey, size, ctx, tcip);
  return NULL;
}


static inline
ssize_t wrap_fi_read(void* addr, void* mrDesc,
                     c_nodeid_t node,
                     uint64_t mrRaddr, uint64_t mrKey,
                     size_t size, void* ctx,
                     struct perTxCtxInfo_t* tcip) {
  DBG_PRINTF(DBG_RMA | DBG_RMA_READ,
             "tx read: %p <= %d:%#" PRIx64 ", size %zd, ctx %p",
             addr, (int) node, mrRaddr, size, ctx);
  OFI_RIDE_OUT_EAGAIN(tcip,
                      fi_read(tcip->txCtx, addr, size,
                              mrDesc, rxAddr(tcip, node),
                              mrRaddr, mrKey, ctx));
  tcip->numTxnsOut++;
  tcip->numTxnsSent++;
  return FI_SUCCESS;
}


static inline
ssize_t wrap_fi_readmsg(void* addr, void* mrDesc,
                        c_nodeid_t node,
                        uint64_t mrRaddr, uint64_t mrKey,
                        size_t size, void* ctx, uint64_t flags,
                        struct perTxCtxInfo_t* tcip) {
  struct iovec msg_iov = (struct iovec)
                         { .iov_base = addr,
                           .iov_len = size };
  struct fi_rma_iov rma_iov = (struct fi_rma_iov)
                              { .addr = (uint64_t) mrRaddr,
                                .len = size,
                                .key = mrKey };
  struct fi_msg_rma msg = (struct fi_msg_rma)
                          { .msg_iov = &msg_iov,
                            .desc = &mrDesc,
                            .iov_count = 1,
                            .addr = rxAddr(tcip, node),
                            .rma_iov = &rma_iov,
                            .rma_iov_count = 1,
                            .context = ctx };
  DBG_PRINTF(DBG_RMA | DBG_RMA_READ,
             "tx read msg: %p <= %d:%#" PRIx64 ", size %zd, ctx %p, "
             "flags %#" PRIx64,
             addr, (int) node, mrRaddr, size, ctx, flags);
  OFI_RIDE_OUT_EAGAIN(tcip, fi_readmsg(tcip->txCtx, &msg, flags));
  tcip->numTxnsOut++;
  tcip->numTxnsSent++;
  return FI_SUCCESS;
}


static inline
void ofi_get_lowLevel(void* addr, void* mrDesc, c_nodeid_t node,
                      uint64_t mrRaddr, uint64_t mrKey, size_t size,
                      void* ctx, uint64_t flags,
                      struct perTxCtxInfo_t* tcip) {
  if (flags == 0) {
    (void) wrap_fi_read(addr, mrDesc, node, mrRaddr, mrKey, size, ctx, tcip);
  } else {
    (void) wrap_fi_readmsg(addr, mrDesc, node, mrRaddr, mrKey, size, ctx,
                           flags, tcip);
  }
}


static
void ofi_get_V(int v_len, void** addr_v, void** local_mr_v,
               c_nodeid_t* locale_v, void** raddr_v, uint64_t* remote_mr_v,
               size_t* size_v) {
  DBG_PRINTF(DBG_RMA | DBG_RMA_READ | DBG_RMA_UNORD,
             "get_V(%d): %p <= %d:%p, size %zd, key 0x%" PRIx64,
             v_len, addr_v[0], (int) locale_v[0], raddr_v[0], size_v[0],
             remote_mr_v[0]);

  assert(!isAmHandler);

  struct perTxCtxInfo_t* tcip;
  CHK_TRUE((tcip = tciAlloc()) != NULL);

  //
  // Make sure we have enough free CQ entries to initiate the entire
  // batch of transactions.
  //
  waitForCQSpace(tcip, v_len);

  for (int vi = 0; vi < v_len; vi++) {
    struct iovec msg_iov = (struct iovec)
                           { .iov_base = addr_v[vi],
                             .iov_len = size_v[vi] };
    struct fi_rma_iov rma_iov = (struct fi_rma_iov)
                                { .addr = (uint64_t) raddr_v[vi],
                                  .len = size_v[vi],
                                  .key = remote_mr_v[vi] };
    struct fi_msg_rma msg = (struct fi_msg_rma)
                            { .msg_iov = &msg_iov,
                              .desc = &local_mr_v[vi],
                              .iov_count = 1,
                              .addr = rxAddr(tcip, locale_v[vi]),
                              .rma_iov = &rma_iov,
                              .rma_iov_count = 1,
                              .context = txnTrkEncodeId(__LINE__),
                              .data = 0 };
    DBG_PRINTF(DBG_RMA | DBG_RMA_READ,
               "tx readmsg: %p <= %d:%p, size %zd, key 0x%" PRIx64,
               msg.msg_iov->iov_base, (int) locale_v[vi],
               (void*) msg.rma_iov->addr, msg.msg_iov->iov_len,
               msg.rma_iov->key);
    tcip->numTxnsOut++;
    tcip->numTxnsSent++;
    if (tcip->numTxnsOut < txCQLen && vi < v_len - 1) {
      // Add another transaction to the group and go on without waiting.
      OFI_RIDE_OUT_EAGAIN(tcip,
                          fi_readmsg(tcip->txCtx, &msg, FI_MORE));
    } else {
      // Initiate last transaction in group and wait for whole group.
      OFI_RIDE_OUT_EAGAIN(tcip,
                          fi_readmsg(tcip->txCtx, &msg, 0));
      while (tcip->numTxnsOut > 0) {
        (*tcip->ensureProgressFn)(tcip);
      }
    }
  }

  tciFree(tcip);
}


/*
 *** START OF BUFFERED GET OPERATIONS ***
 *
 * Support for buffered GET operations. We internally buffer GET operations and
 * then initiate them all at once for increased transaction rate.
 */

// Flush buffered GETs for the specified task info and reset the counter.
static inline
void get_buff_task_info_flush(get_buff_task_info_t* info) {
  if (info->vi > 0) {
    DBG_PRINTF(DBG_RMA_UNORD,
               "get_buff_task_info_flush(): info has %d entries",
               info->vi);
    ofi_get_V(info->vi, info->tgt_addr_v, info->local_mr_v,
              info->locale_v, info->src_addr_v, info->remote_mr_v,
              info->size_v);
    info->vi = 0;
  }
}


static inline
void do_remote_get_buff(void* addr, c_nodeid_t node, void* raddr,
                        size_t size) {
  uint64_t mrKey;
  uint64_t mrRaddr;
  get_buff_task_info_t* info;
  if (size > MAX_UNORDERED_TRANS_SZ
      || !mrGetKey(&mrKey, &mrRaddr, node, raddr, size)
      || (info = task_local_buff_acquire(get_buff)) == NULL) {
    (void) ofi_get(addr, node, raddr, size);
    return;
  }

  void* mrDesc;
  CHK_TRUE(mrGetDesc(&mrDesc, addr, size));

  int vi = info->vi;
  info->tgt_addr_v[vi] = addr;
  info->locale_v[vi] = node;
  info->remote_mr_v[vi] = mrKey;
  info->src_addr_v[vi] = (void*) mrRaddr;
  info->size_v[vi] = size;
  info->local_mr_v[vi] = mrDesc;
  info->vi++;

  DBG_PRINTF(DBG_RMA_UNORD,
             "do_remote_get_buff(): info[%d] = "
             "{%p, %d, %" PRIx64 ", %p, %zd, %p}",
             vi, addr, (int) node, mrKey, raddr, size, mrDesc);

  // flush if buffers are full
  if (info->vi == MAX_CHAINED_GET_LEN) {
    get_buff_task_info_flush(info);
  }
}
/*** END OF BUFFERED GET OPERATIONS ***/


struct amoBundle_t {
  struct fi_msg_atomic m;
  struct fi_ioc iovOpnd;
  void* mrDescOpnd;
  struct fi_rma_ioc iovObj;
  struct fi_ioc iovCmpr;
  void* mrDescCmpr;
  struct fi_ioc iovRes;
  void* mrDescRes;
  c_nodeid_t node;
  size_t size;
};

typedef chpl_comm_nb_handle_t (amoFn_t)(struct amoBundle_t *ab,
                                        struct perTxCtxInfo_t* tcip);

static amoFn_t amoFn_selector;

static
chpl_comm_nb_handle_t ofi_amo(c_nodeid_t node, uint64_t object, uint64_t mrKey,
                              const void* opnd, const void* cmpr, void* result,
                              enum fi_op ofiOp, enum fi_datatype ofiType,
                              size_t size) {
  if (ofiOp == FI_ATOMIC_READ && opnd == NULL && provCtl_readAmoNeedsOpnd) {
    //
    // Workaround for bug wherein operand is unused but nevertheless
    // must not be NULL.
    //
    static int64_t dummy;
    opnd = &dummy;
  }

  //
  // Any or all of these may be NULL but it's clearer and, with inlining,
  // just as fast to simply make the calls.
  //
  void* mrDescOpnd;
  void* myOpnd = mrLocalizeSource(&mrDescOpnd, opnd, size, "AMO operand");

  void* mrDescCmpr;
  void* myCmpr = mrLocalizeSource(&mrDescCmpr, cmpr, size, "AMO comparand");

  void* mrDescRes;
  void* myRes = mrLocalizeTarget(&mrDescRes, result, size, "AMO result");

  struct perTxCtxInfo_t* tcip;
  CHK_TRUE((tcip = tciAlloc()) != NULL);
  assert(tcip->txCQ != NULL);  // AMOs require a CQ, at least for now

  struct amoBundle_t ab = { .m = { .msg_iov = &ab.iovOpnd,
                                   .desc = &ab.mrDescOpnd,
                                   .iov_count = 1,
                                   .addr = rxAddr(tcip, node),
                                   .rma_iov = &ab.iovObj,
                                   .rma_iov_count = 1,
                                   .datatype = ofiType,
                                   .op = ofiOp, },
                            .iovOpnd = { .addr = myOpnd,
                                         .count = 1, },
                            .mrDescOpnd = mrDescOpnd,
                            .iovObj = { .addr = object,
                                      .count = 1,
                                      .key = mrKey, },
                            .iovCmpr = { .addr = myCmpr,
                                         .count = 1, },
                            .mrDescCmpr = mrDescCmpr,
                            .iovRes = { .addr = myRes,
                                         .count = 1, },
                            .mrDescRes = mrDescRes,
                            .node = node,
                            .size = size, };

  chpl_comm_nb_handle_t ret;
  ret = amoFn_selector(&ab, tcip);

  tciFree(tcip);

  mrUnLocalizeTarget(myRes, result, size);
  mrUnLocalizeSource(myCmpr, cmpr);
  mrUnLocalizeSource(myOpnd, opnd);

  return ret;
}


static amoFn_t amoFn_msgOrdFence;
static amoFn_t amoFn_msgOrd;
static amoFn_t amoFn_dlvrCmplt;

static inline
chpl_comm_nb_handle_t amoFn_selector(struct amoBundle_t *ab,
                                     struct perTxCtxInfo_t* tcip) {
  chpl_comm_nb_handle_t ret = NULL;

  switch (mcmMode) {
  case mcmm_msgOrdFence:
    ret = amoFn_msgOrdFence(ab, tcip);
    break;
  case mcmm_msgOrd:
    ret = amoFn_msgOrd(ab, tcip);
    break;
  case mcmm_dlvrCmplt:
    ret = amoFn_dlvrCmplt(ab, tcip);
    break;
  default:
    INTERNAL_ERROR_V("unexpected mcmMode %d", mcmMode);
    break;
  }

  return ret;
}


static ssize_t wrap_fi_inject_atomic(struct amoBundle_t* ab,
                                     struct perTxCtxInfo_t* tcip);
static ssize_t wrap_fi_atomicmsg(struct amoBundle_t* ab, uint64_t flags,
                                 struct perTxCtxInfo_t* tcip);


//
// Implements ofi_amo() when MCM mode is message ordering with fences.
//
static
chpl_comm_nb_handle_t amoFn_msgOrdFence(struct amoBundle_t *ab,
                                        struct perTxCtxInfo_t* tcip) {
  if (ab->iovRes.addr == NULL
      && tcip->bound
      && ab->size <= ofi_info->tx_attr->inject_size
      && (tcip->putVisBitmap == NULL
          || !bitmapTest(tcip->putVisBitmap, ab->node))
      && envInjectAMO) {
    //
    // Special case: injection is the quickest.  We can use that if
    // this is a non-fetching operation, we have a bound tx context so
    // we can delay forcing the memory visibility until later, the size
    // doesn't exceed the injection size limit, and we don't need a
    // fence because the last thing we did to the same target node was
    // not a PUT.
    //
    (void) wrap_fi_inject_atomic(ab, tcip);
  } else {
    //
    // If we need a result wait for it; otherwise, we can collect
    // the completion later and message ordering will ensure MCM
    // conformance.
    //
    atomic_bool txnDone;
    if (ab->iovRes.addr != NULL) {
      ab->m.context = txCtxInit(tcip, __LINE__, &txnDone);;
    }

    if (tcip->bound
        && tcip->putVisBitmap != NULL
        && bitmapTest(tcip->putVisBitmap, ab->node)) {
      //
      // Special case: If our last operation to the same remote node was
      // a PUT (which we only see with a bound tx context) then we need
      // to do a fenced AMO to force the PUT to complete before this
      // AMO.  We may still be able to inject the AMO, however.
      //
      uint64_t flags = FI_FENCE;
      if (ab->size <= ofi_info->tx_attr->inject_size
          && envInjectAMO) {
        flags |= FI_INJECT;
      }
      (void) wrap_fi_atomicmsg(ab, flags, tcip);
      bitmapClear(tcip->putVisBitmap, ab->node);
    } else {
      //
      // General case.
      //
      uint64_t flags = 0;

      //
      // If it's a fetching AMO (read) and the provider doesn't support
      // FI_ORDER_ATOMIC_RAW, then we must do a fenced AMO to force any
      // outstanding non-fetching AMOs (writes) to complete before this AMO.
      //
      if ((ab->iovRes.addr != NULL) &&
        (ofi_info->tx_attr->msg_order & FI_ORDER_ATOMIC_RAW) == 0) {
        flags = FI_FENCE;
      }
      (void) wrap_fi_atomicmsg(ab, flags, tcip);
    }

    if (ab->iovRes.addr != NULL) {
      waitForTxnComplete(tcip, ab->m.context);
      txCtxCleanup(ab->m.context);
    }
  }

  //
  // When using message ordering we have to do something after the
  // operation to force it into visibility, and on the same tx context
  // as the operation itself because libfabric message ordering is
  // specific to endpoint pairs.  With a bound tx context we can do it
  // later, when needed.  Otherwise we have to do it now.
  //
  if (tcip->bound) {
    bitmapSet(tcip->amoVisBitmap, ab->node);
  } else {
    mcmReleaseOneNode(ab->node, tcip, "AMO");
  }

  return NULL;
}


//
// Implements ofi_amo() when MCM mode is message ordering.
//
static
chpl_comm_nb_handle_t amoFn_msgOrd(struct amoBundle_t *ab,
                                   struct perTxCtxInfo_t* tcip) {
  if (ab->m.op != FI_ATOMIC_READ) {
    forceMemFxVisAllNodes(true /*checkPuts*/, true /*checkAmos*/,
                          -1 /*skipNode*/, tcip);
  }

  if (tcip->bound
      && ab->iovRes.addr == NULL
      && ab->size <= ofi_info->tx_attr->inject_size
      && envInjectAMO) {
    //
    // Special case: injection is the quickest.  We can use that if this
    // is a non-fetching operation, we have a bound tx context so we can
    // delay forcing the memory visibility until later, and the size
    // doesn't exceed the injection size limit.
    //
    (void) wrap_fi_inject_atomic(ab, tcip);
  } else {
    //
    // General case.
    //
    // If we need a result wait for it; otherwise, message ordering will
    // ensure MCM conformance and we can collect the completion later.
    //
    if (ab->iovRes.addr == NULL) {
      (void) wrap_fi_atomicmsg(ab, 0, tcip);
    } else {
      atomic_bool txnDone;
      ab->m.context = txCtxInit(tcip, __LINE__, &txnDone);
      (void) wrap_fi_atomicmsg(ab, 0, tcip);
      waitForTxnComplete(tcip, ab->m.context);
      txCtxCleanup(ab->m.context);
    }
  }

  //
  // When using message ordering we have to do something after the
  // operation to force it into visibility, and on the same tx context
  // as the operation itself because libfabric message ordering is
  // specific to endpoint pairs.  With a bound tx context we can do it
  // later, when needed.  Otherwise we have to do it now.
  //
  if (tcip->bound) {
    bitmapSet(tcip->amoVisBitmap, ab->node);
  } else {
    mcmReleaseOneNode(ab->node, tcip, "AMO");
  }

  return NULL;
}


//
// Implements ofi_amo() when MCM mode is delivery complete.
//
static
chpl_comm_nb_handle_t amoFn_dlvrCmplt(struct amoBundle_t *ab,
                                      struct perTxCtxInfo_t* tcip) {
  atomic_bool txnDone;
  ab->m.context = txCtxInit(tcip, __LINE__, &txnDone);
  (void) wrap_fi_atomicmsg(ab, 0, tcip);
  waitForTxnComplete(tcip, ab->m.context);
  txCtxCleanup(ab->m.context);
  return NULL;
}


static inline
ssize_t wrap_fi_inject_atomic(struct amoBundle_t* ab,
                              struct perTxCtxInfo_t* tcip) {
  DBG_PRINTF(DBG_AMO,
             "tx AMO (inject): obj %d:%#" PRIx64 ", opnd <%s>, "
             "op %s, typ %s, sz %zd",
             (int) ab->node, ab->iovObj.addr,
             DBG_VAL(ab->iovOpnd.addr, ab->m.datatype),
             amo_opName(ab->m.op), amo_typeName(ab->m.datatype), ab->size);
  // TODO: How quickly/often does local resource throttling happen?
  OFI_RIDE_OUT_EAGAIN(tcip,
                      fi_inject_atomic(tcip->txCtx,
                                       ab->iovOpnd.addr, 1,
                                       ab->m.addr,
                                       ab->iovObj.addr, ab->iovObj.key,
                                       ab->m.datatype, ab->m.op));
  tcip->numTxnsSent++;
  return FI_SUCCESS;
}


static inline
ssize_t wrap_fi_atomicmsg(struct amoBundle_t* ab, uint64_t flags,
                          struct perTxCtxInfo_t* tcip) {
#ifdef CHPL_COMM_DEBUG
  char flagBuf[256];
#endif
  if (ab->iovRes.addr == NULL) {
    // Non-fetching.
    DBG_PRINTF(DBG_AMO,
               "tx AMO (msg): obj %d:%#" PRIx64 ", opnd <%s>, "
               "op %s, typ %s, sz %zd, ctx %p, flags <%s> (%#" PRIx64 ")",
               (int) ab->node, ab->iovObj.addr,
               DBG_VAL(ab->iovOpnd.addr, ab->m.datatype),
               amo_opName(ab->m.op), amo_typeName(ab->m.datatype), ab->size,
               ab->m.context, fi_tostr_r(flagBuf, sizeof(flagBuf), &flags,
                                         FI_TYPE_OP_FLAGS), flags);
    OFI_RIDE_OUT_EAGAIN(tcip, fi_atomicmsg(tcip->txCtx, &ab->m, flags));
  } else {
    // Fetching.
    if (ab->m.op == FI_CSWAP) {
      // CSWAP, operand + comparand.
      DBG_PRINTF(DBG_AMO,
                 "tx AMO (msg): obj %d:%#" PRIx64 ", opnd <%s>, cmpr <%s>, "
                 "op %s, typ %s, res %p, sz %zd, ctx %p, flags <%s> (%#"
                 PRIx64 ")",
                 (int) ab->node, ab->iovObj.addr,
                 DBG_VAL(ab->iovOpnd.addr, ab->m.datatype),
                 DBG_VAL(ab->iovCmpr.addr, ab->m.datatype),
                 amo_opName(ab->m.op), amo_typeName(ab->m.datatype),
                 ab->iovRes.addr, ab->size, ab->m.context,
                 fi_tostr_r(flagBuf, sizeof(flagBuf), &flags,
                                         FI_TYPE_OP_FLAGS),flags);
      OFI_RIDE_OUT_EAGAIN(tcip, fi_compare_atomicmsg(tcip->txCtx, &ab->m,
                                   &ab->iovCmpr, &ab->mrDescCmpr, 1,
                                   &ab->iovRes, &ab->mrDescRes, 1, flags));
    } else {
      // Fetching, but no comparand.
      if (ab->m.op == FI_ATOMIC_READ) {
        DBG_PRINTF(DBG_AMO_READ,
                   "tx AMO (msg): obj %d:%#" PRIx64 ", "
                   "op %s, typ %s, res %p, sz %zd, ctx %p, flags <%s> (%#"
                    PRIx64 ")",
                   (int) ab->node, ab->iovObj.addr,
                   amo_opName(ab->m.op), amo_typeName(ab->m.datatype),
                   ab->iovRes.addr, ab->size, ab->m.context,
                   fi_tostr_r(flagBuf, sizeof(flagBuf), &flags,
                              FI_TYPE_OP_FLAGS),flags);
      } else {
        DBG_PRINTF(DBG_AMO,
                   "tx AMO (msg): obj %d:%#" PRIx64 ", opnd <%s>, "
                   "op %s, typ %s, res %p, sz %zd, ctx %p, flags <%s> (%#"
                   PRIx64 ")",
                   (int) ab->node, ab->iovObj.addr,
                   DBG_VAL(ab->iovOpnd.addr, ab->m.datatype),
                   amo_opName(ab->m.op), amo_typeName(ab->m.datatype),
                   ab->iovRes.addr, ab->size, ab->m.context,
                   fi_tostr_r(flagBuf, sizeof(flagBuf), &flags,
                              FI_TYPE_OP_FLAGS), flags);
      }
      OFI_RIDE_OUT_EAGAIN(tcip, fi_fetch_atomicmsg(tcip->txCtx, &ab->m,
                                 &ab->iovRes, &ab->mrDescRes, 1, flags));
    }
  }

  tcip->numTxnsOut++;
  tcip->numTxnsSent++;
  return FI_SUCCESS;
}


static
void ofi_amo_nf_V(int v_len, uint64_t* opnd_v, void* local_mr,
                  c_nodeid_t* locale_v, void** object_v, uint64_t* remote_mr_v,
                  size_t* size_v, enum fi_op* cmd_v,
                  enum fi_datatype* type_v) {
  DBG_PRINTF(DBG_AMO | DBG_AMO_UNORD,
             "amo_nf_V(%d): obj %d:%p, opnd <%s>, op %s, typ %s, sz %zd, "
             "key 0x%" PRIx64,
             v_len, (int) locale_v[0], object_v[0],
             DBG_VAL(&opnd_v[0], type_v[0]),
             amo_opName(cmd_v[0]), amo_typeName(type_v[0]), size_v[0],
             remote_mr_v[0]);

  assert(!isAmHandler);

  struct perTxCtxInfo_t* tcip;
  CHK_TRUE((tcip = tciAlloc()) != NULL);

  //
  // Make sure we have enough free CQ entries to initiate the entire
  // batch of transactions.
  //
  waitForCQSpace(tcip, v_len);

  //
  // Initiate the batch.  Record which nodes we do AMOs to, so that we
  // can force visibility in target memory at the end.
  //
  if (tcip->amoVisBitmap == NULL) {
    tcip->amoVisBitmap = bitmapAlloc(chpl_numNodes);
  }

  for (int vi = 0; vi < v_len; vi++) {
    struct amoBundle_t ab = { .m = { .msg_iov = &ab.iovOpnd,
                                     .desc = &local_mr,
                                     .iov_count = 1,
                                     .addr = rxAddr(tcip, locale_v[vi]),
                                     .rma_iov = &ab.iovObj,
                                     .rma_iov_count = 1,
                                     .datatype = type_v[vi],
                                     .op = cmd_v[vi],
                                     .context = txnTrkEncodeId(__LINE__), },
                              .iovOpnd = { .addr = &opnd_v[vi],
                                           .count = 1, },
                              .iovObj = { .addr = (uint64_t) object_v[vi],
                                        .count = 1,
                                        .key = remote_mr_v[vi], },
                              .node = locale_v[vi],
                              .size = size_v[vi], };
    (void) wrap_fi_atomicmsg(&ab, (vi < v_len - 1) ? FI_MORE : 0, tcip);
    bitmapSet(tcip->amoVisBitmap, locale_v[vi]);
  }

  //
  // Enforce Chapel MCM: force visibility of the memory effects of all
  // these AMOs.
  //
  mcmReleaseAllNodes(tcip->amoVisBitmap, NULL, -1 /*skipNode*/, tcip,
                     "unordered AMO");

  tciFree(tcip);
}


static
void amEnsureProgress(struct perTxCtxInfo_t* tcip) {
  (*tcip->checkTxCmplsFn)(tcip);

  //
  // We only have responsibility for inbound operations if we're doing
  // manual progress.
  //
  if (ofi_info->domain_attr->data_progress != FI_PROGRESS_MANUAL) {
    return;
  }

  (void) amCheckRxTxCmpls(NULL, NULL, tcip);
}


static
void amCheckRxTxCmpls(chpl_bool* pHadRxEvent, chpl_bool* pHadTxEvent,
                      struct perTxCtxInfo_t* tcip) {
  if (ofi_amhPollSet != NULL) {
    void* contexts[pollSetSize];
    int ret;
    OFI_CHK_COUNT(fi_poll(ofi_amhPollSet, contexts, pollSetSize), ret);

    //
    // Process the CQs/counters that had events.  We really only have
    // to consume completions for our transmit endpoint.  If we have
    // inbound AM messages we'll let the caller know and those can be
    // dealt with in the main poll loop.  For inbound RMA, ensuring
    // progress is all that's needed, and the poll call itself will
    // have done that.
    //
    for (int i = 0; i < ret; i++) {
      if (contexts[i] == &ofi_rxCQ) {
        if (pHadRxEvent != NULL) {
          *pHadRxEvent = true;
        }
      } else if (contexts[i] == &tcip->checkTxCmplsFn) {
        (*tcip->checkTxCmplsFn)(tcip);
        if (pHadTxEvent != NULL) {
          *pHadTxEvent = true;
        }
      } else {
        INTERNAL_ERROR_V("unexpected context %p from fi_poll()",
                         contexts[i]);
      }
    }
  } else {

    // The provider can't do poll sets.  Consume transmit completions,
    // and progress the receive endpoint as required by some providers
    // (e.g. EFA, which may exchange handshake messages in the background
    // during a transmit and therefore requires progressing the receive
    // checkpoint so that handshakes are received). Inbound operations
    // will be handled by the main loop. Also, avoid CPU monopolization
    // even if we had events, because we can't actually tell.

    sched_yield();
    int rc = fi_cq_read(ofi_rxCQ, NULL, 0);
    if (rc == 0) {
      if (pHadRxEvent != NULL) {
        *pHadRxEvent = true;
      }
    } else if (rc != -FI_EAGAIN) {
      INTERNAL_ERROR_V("fi_cq_read failed: %s", fi_strerror(rc));
    }
    (*tcip->checkTxCmplsFn)(tcip);
    if (pHadTxEvent != NULL) {
      *pHadTxEvent = true;
    }
  }
}


static
void checkTxCmplsCQ(struct perTxCtxInfo_t* tcip) {
  struct fi_cq_msg_entry cqes[txCQLen];
  const size_t cqesSize = sizeof(cqes) / sizeof(cqes[0]);
  const size_t numEvents = readCQ(tcip->txCQ, cqes, cqesSize);

  tcip->numTxnsOut -= numEvents;
  for (int i = 0; i < numEvents; i++) {
    struct fi_cq_msg_entry* cqe = &cqes[i];
    const txnTrkCtx_t trk = txnTrkDecode(cqe->op_context);
    DBG_PRINTF(DBG_ACK, "CQ ack tx, flags %#" PRIx64 ", ctx %d:%p",
               cqe->flags, trk.typ, trk.ptr);
    if (trk.typ == txnTrkDone) {
      atomic_store_explicit_bool((atomic_bool*) trk.ptr, true,
                                 memory_order_release);
    } else if (trk.typ != txnTrkId) {
      INTERNAL_ERROR_V("unexpected trk.typ %d", trk.typ);
    }
  }
}


static
void checkTxCmplsCntr(struct perTxCtxInfo_t* tcip) {
  if (tcip->numTxnsOut > 0) {
    OFI_CHK(fi_cntr_wait(tcip->txCntr, tcip->numTxnsSent, -1));
    uint64_t count = fi_cntr_read(tcip->txCntr);
    if (count > tcip->numTxnsSent) {
      INTERNAL_ERROR_V("fi_cntr_read() %" PRIu64 ", but numTxnsSent %" PRIu64,
                       count, tcip->numTxnsSent);
    }
    tcip->numTxnsOut = tcip->numTxnsSent - count;
    if (tcip->numTxnsOut > 0) {
      count = fi_cntr_readerr(tcip->txCntr);
      if (count > 0) {
        INTERNAL_ERROR_V("error count %" PRIu64, count);
      }
    }
  }
}


static inline
size_t readCQ(struct fid_cq* cq, void* buf, size_t count) {
  ssize_t ret;
  CHK_TRUE((ret = fi_cq_read(cq, buf, count)) > 0
           || ret == -FI_EAGAIN
           || ret == -FI_EAVAIL);
  if (ret == -FI_EAVAIL) {
    reportCQError(cq);
  }
  return (ret == -FI_EAGAIN) ? 0 : ret;
}


static
void reportCQError(struct fid_cq* cq) {
  char err_data[ofi_info->domain_attr->max_err_data];
  struct fi_cq_err_entry err = (struct fi_cq_err_entry)
                               { .err_data = err_data,
                                 .err_data_size = sizeof(err_data) };
  fi_cq_readerr(cq, &err, 0);
  const txnTrkCtx_t trk = txnTrkDecode(err.op_context);
  if (err.err == FI_ETRUNC) {
    //
    // This only happens when reading from the CQ associated with the
    // inbound AM request multi-receive buffer.
    //
    // We ran out of inbound buffer space and a message was truncated.
    // If the fi_setopt(FI_OPT_MIN_MULTI_RECV) worked and nobody sent
    // anything larger than that, this shouldn't happen.  In any case,
    // we can't recover, but let's provide some information to help
    // aid failure analysis.
    //
    INTERNAL_ERROR_V("fi_cq_readerr(): AM recv buf FI_ETRUNC: "
                     "flags %#" PRIx64 ", len %zd, olen %zd, ctx %d:%p",
                     err.flags, err.len, err.olen, trk.typ, trk.ptr);
  } else {
    char buf[100];
    const char* errStr = fi_cq_strerror(cq, err.prov_errno, err.err_data,
                                        buf, sizeof(buf));
    INTERNAL_ERROR_V("fi_cq_read(): err %d, prov_errno %d, errStr %s, "
                     "ctx %d:%p",
                     err.err, err.prov_errno, errStr, trk.typ, trk.ptr);
  }
}


static inline
void waitForTxnComplete(struct perTxCtxInfo_t* tcip, void* ctx) {
  (*tcip->ensureProgressFn)(tcip);
  const txnTrkCtx_t trk = txnTrkDecode(ctx);
  if (trk.typ == txnTrkDone) {
    // wait for the individual transmission to complete
    while (!atomic_load_explicit_bool((atomic_bool*) trk.ptr,
                                      memory_order_acquire)) {
      sched_yield();
      (*tcip->ensureProgressFn)(tcip);
    }
  } else {
    // wait for all outstanding transmissions to complete
    while (tcip->numTxnsOut > 0) {
      sched_yield();
      (*tcip->ensureProgressFn)(tcip);
    }
  }
}


static inline
void forceMemFxVisOneNode(c_nodeid_t node,
                          chpl_bool checkPuts, chpl_bool checkAmos,
                          struct perTxCtxInfo_t* tcip) {
  //
  // Enforce MCM: make sure the memory effects of the operations we've
  // done on a specific node are actually visible.  Note the check for
  // a bound tx context -- we can't have outstanding network operations
  // without that.
  //
  if (tcip->bound) {
    chpl_bool havePutsOut = (checkPuts
                             && tcip->putVisBitmap != NULL
                             && bitmapTest(tcip->putVisBitmap, node));
    chpl_bool haveAmosOut = (checkAmos
                             && tcip->amoVisBitmap != NULL
                             && bitmapTest(tcip->amoVisBitmap, node));
    if (havePutsOut || haveAmosOut) {
      mcmReleaseOneNode(node, tcip, "PUT");
      if (havePutsOut) {
        bitmapClear(tcip->putVisBitmap, node);
      }
      if (haveAmosOut) {
        bitmapClear(tcip->amoVisBitmap, node);
      }
    }
  }
}


static inline
void forceMemFxVisAllNodes(chpl_bool checkPuts, chpl_bool checkAmos,
                           c_nodeid_t skipNode,
                           struct perTxCtxInfo_t* tcip) {
  //
  // Enforce MCM: make sure the memory effects of all the operations
  // we've done so far, to any node, are actually visible.  This is only
  // needed if we have a bound tx context.  Otherwise, we would have
  // forced visibility at the time of the operation.
  //
  if (tcip->bound) {
    mcmReleaseAllNodes(checkPuts ? tcip->putVisBitmap : NULL,
                       checkAmos ? tcip->amoVisBitmap : NULL,
                       skipNode, tcip, "PUT and/or AMO");
  }
}


static inline
void forceMemFxVisAllNodes_noTcip(chpl_bool checkPuts, chpl_bool checkAmos) {
  //
  // Enforce MCM: make sure the memory effects of all the operations
  // we've done so far, to any node, are actually visible.  This is only
  // needed if we're using message ordering (with or without fences) for
  // MCM conformance.  Otherwise, we would have forced visibility at the
  // time of the operation.
  //
  if (chpl_numNodes > 1 && mcmMode != mcmm_dlvrCmplt) {
    struct perTxCtxInfo_t* tcip;
    CHK_TRUE((tcip = tciAlloc()) != NULL);
    forceMemFxVisAllNodes(checkPuts, checkAmos, -1 /*skipNode*/, tcip);
    tciFree(tcip);
  }
}


static
void* allocBounceBuf(size_t size) {
  void* p;
  CHPL_CALLOC_SZ(p, 1, size);
  return p;
}


static
void freeBounceBuf(void* p) {
  CHPL_FREE(p);
}


static inline
void local_yield(void) {
#ifdef CHPL_COMM_DEBUG
  pthread_t pthreadWas = { 0 };
  if (chpl_task_isFixedThread()) {
    pthreadWas = pthread_self();
  }
#endif

  //
  // Our task cannot make progress.  Yield, to allow some other task to
  // free up whatever resource we need.
  //
  // DANGER: Don't call this function on a worker thread while holding
  //         a tciTab[] entry, that is, between tcip=tciAlloc() and
  //         tciFree().  If you do and your task switches threads due
  //         to the chpl_task_yield(), we can end up with two threads
  //         using the same tciTab[] entry simultaneously.
  //
  chpl_task_yield();

#ifdef CHPL_COMM_DEBUG
  //
  // There are things in the comm layer that will break if tasks can
  // switch threads when they think their thread is a fixed worker.
  //
  if (chpl_task_isFixedThread()) {
    CHK_TRUE(pthread_self() == pthreadWas);
  }
#endif
}


////////////////////////////////////////
//
// Interface: network atomics
//

static void doAMO(c_nodeid_t, void*, const void*, const void*, void*,
                  int, enum fi_datatype, size_t);


//
// WRITE
//
#define DEFN_CHPL_COMM_ATOMIC_WRITE(fnType, ofiType, Type)              \
  void chpl_comm_atomic_write_##fnType                                  \
         (void* desired, c_nodeid_t node, void* object,                 \
          memory_order order, int ln, int32_t fn) {                     \
    DBG_PRINTF(DBG_IFACE_AMO_WRITE,                                     \
               "%s(%p, %d, %p, %d, %s)", __func__,                      \
               desired, (int) node, object,                             \
               ln, chpl_lookupFilename(fn));                            \
    chpl_comm_diags_verbose_amo("amo write", node, ln, fn);             \
    chpl_comm_diags_incr(amo);                                          \
    doAMO(node, object, desired, NULL, NULL,                            \
          FI_ATOMIC_WRITE, ofiType, sizeof(Type));                      \
  }

DEFN_CHPL_COMM_ATOMIC_WRITE(int32, FI_INT32, int32_t)
DEFN_CHPL_COMM_ATOMIC_WRITE(int64, FI_INT64, int64_t)
DEFN_CHPL_COMM_ATOMIC_WRITE(uint32, FI_UINT32, uint32_t)
DEFN_CHPL_COMM_ATOMIC_WRITE(uint64, FI_UINT64, uint64_t)
DEFN_CHPL_COMM_ATOMIC_WRITE(real32, FI_FLOAT, _real32)
DEFN_CHPL_COMM_ATOMIC_WRITE(real64, FI_DOUBLE, _real64)


//
// READ
//
#define DEFN_CHPL_COMM_ATOMIC_READ(fnType, ofiType, Type)               \
  void chpl_comm_atomic_read_##fnType                                   \
         (void* result, c_nodeid_t node, void* object,                  \
          memory_order order, int ln, int32_t fn) {                     \
    DBG_PRINTF(DBG_IFACE_AMO_READ,                                      \
               "%s(%p, %d, %p, %d, %s)", __func__,                      \
               result, (int) node, object,                              \
               ln, chpl_lookupFilename(fn));                            \
    chpl_comm_diags_verbose_amo("amo read", node, ln, fn);              \
    chpl_comm_diags_incr(amo);                                          \
    doAMO(node, object, NULL, NULL, result,                             \
          FI_ATOMIC_READ, ofiType, sizeof(Type));                       \
  }

DEFN_CHPL_COMM_ATOMIC_READ(int32, FI_INT32, int32_t)
DEFN_CHPL_COMM_ATOMIC_READ(int64, FI_INT64, int64_t)
DEFN_CHPL_COMM_ATOMIC_READ(uint32, FI_UINT32, uint32_t)
DEFN_CHPL_COMM_ATOMIC_READ(uint64, FI_UINT64, uint64_t)
DEFN_CHPL_COMM_ATOMIC_READ(real32, FI_FLOAT, _real32)
DEFN_CHPL_COMM_ATOMIC_READ(real64, FI_DOUBLE, _real64)


#define DEFN_CHPL_COMM_ATOMIC_XCHG(fnType, ofiType, Type)               \
  void chpl_comm_atomic_xchg_##fnType                                   \
         (void* desired, c_nodeid_t node, void* object, void* result,   \
          memory_order order, int ln, int32_t fn) {                     \
    DBG_PRINTF(DBG_IFACE_AMO,                                           \
               "%s(%p, %d, %p, %p, %d, %s)", __func__,                  \
               desired, (int) node, object, result,                     \
               ln, chpl_lookupFilename(fn));                            \
    chpl_comm_diags_verbose_amo("amo xchg", node, ln, fn);              \
    chpl_comm_diags_incr(amo);                                          \
    doAMO(node, object, desired, NULL, result,                          \
          FI_ATOMIC_WRITE, ofiType, sizeof(Type));                      \
  }

DEFN_CHPL_COMM_ATOMIC_XCHG(int32, FI_INT32, int32_t)
DEFN_CHPL_COMM_ATOMIC_XCHG(int64, FI_INT64, int64_t)
DEFN_CHPL_COMM_ATOMIC_XCHG(uint32, FI_UINT32, uint32_t)
DEFN_CHPL_COMM_ATOMIC_XCHG(uint64, FI_UINT64, uint64_t)
DEFN_CHPL_COMM_ATOMIC_XCHG(real32, FI_FLOAT, _real32)
DEFN_CHPL_COMM_ATOMIC_XCHG(real64, FI_DOUBLE, _real64)


#define DEFN_CHPL_COMM_ATOMIC_CMPXCHG(fnType, ofiType, Type)            \
  void chpl_comm_atomic_cmpxchg_##fnType                                \
         (void* expected, void* desired, c_nodeid_t node, void* object, \
          chpl_bool32* result, memory_order succ, memory_order fail,    \
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_IFACE_AMO,                                           \
               "%s(%p, %p, %d, %p, %p, %d, %s)", __func__,              \
               expected, desired, (int) node, object, result,           \
               ln, chpl_lookupFilename(fn));                            \
    chpl_comm_diags_verbose_amo("amo cmpxchg", node, ln, fn);           \
    chpl_comm_diags_incr(amo);                                          \
    Type old_value;                                                     \
    Type old_expected;                                                  \
    memcpy(&old_expected, expected, sizeof(Type));                      \
    doAMO(node, object, desired, &old_expected, &old_value,             \
          FI_CSWAP, ofiType, sizeof(Type));                             \
    *result = (chpl_bool32)(old_value == old_expected);                 \
    if (!*result) memcpy(expected, &old_value, sizeof(Type));           \
  }

DEFN_CHPL_COMM_ATOMIC_CMPXCHG(int32, FI_INT32, int32_t)
DEFN_CHPL_COMM_ATOMIC_CMPXCHG(int64, FI_INT64, int64_t)
DEFN_CHPL_COMM_ATOMIC_CMPXCHG(uint32, FI_UINT32, uint32_t)
DEFN_CHPL_COMM_ATOMIC_CMPXCHG(uint64, FI_UINT64, uint64_t)
DEFN_CHPL_COMM_ATOMIC_CMPXCHG(real32, FI_FLOAT, _real32)
DEFN_CHPL_COMM_ATOMIC_CMPXCHG(real64, FI_DOUBLE, _real64)


#define DEFN_IFACE_AMO_SIMPLE_OP(fnOp, ofiOp, fnType, ofiType, Type)    \
  void chpl_comm_atomic_##fnOp##_##fnType                               \
         (void* opnd, c_nodeid_t node, void* object,                    \
          memory_order order, int ln, int32_t fn) {                     \
    DBG_PRINTF(DBG_IFACE_AMO,                                           \
               "%s(<%s>, %d, %p, %d, %s)", __func__,                    \
               DBG_VAL(opnd, ofiType), (int) node,                      \
               object, ln, chpl_lookupFilename(fn));                    \
    chpl_comm_diags_verbose_amo("amo " #fnOp, node, ln, fn);            \
    chpl_comm_diags_incr(amo);                                          \
    doAMO(node, object, opnd, NULL, NULL,                               \
          ofiOp, ofiType, sizeof(Type));                                \
  }                                                                     \
                                                                        \
  void chpl_comm_atomic_##fnOp##_unordered_##fnType                     \
         (void* opnd, c_nodeid_t node, void* object,                    \
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_IFACE_AMO,                                           \
               "%s(<%s>, %d, %p, %d, %s)", __func__,                    \
               DBG_VAL(opnd, ofiType), (int) node,                      \
               object, ln, chpl_lookupFilename(fn));                    \
    chpl_comm_diags_verbose_amo("amo unord_" #fnOp, node, ln, fn);      \
    chpl_comm_diags_incr(amo);                                          \
    do_remote_amo_nf_buff(opnd, node, object, sizeof(Type),             \
                          ofiOp, ofiType);                              \
  }                                                                     \
                                                                        \
  void chpl_comm_atomic_fetch_##fnOp##_##fnType                         \
         (void* opnd, c_nodeid_t node, void* object, void* result,      \
          memory_order order, int ln, int32_t fn) {                     \
    DBG_PRINTF(DBG_IFACE_AMO,                                           \
               "%s(<%s>, %d, %p, %p, %d, %s)", __func__,                \
               DBG_VAL(opnd, ofiType), (int) node,                      \
               object, result, ln, chpl_lookupFilename(fn));            \
    chpl_comm_diags_verbose_amo("amo fetch_" #fnOp, node, ln, fn);      \
    chpl_comm_diags_incr(amo);                                          \
    doAMO(node, object, opnd, NULL, result,                             \
          ofiOp, ofiType, sizeof(Type));                                \
  }

DEFN_IFACE_AMO_SIMPLE_OP(and, FI_BAND, int32, FI_INT32, int32_t)
DEFN_IFACE_AMO_SIMPLE_OP(and, FI_BAND, int64, FI_INT64, int64_t)
DEFN_IFACE_AMO_SIMPLE_OP(and, FI_BAND, uint32, FI_UINT32, uint32_t)
DEFN_IFACE_AMO_SIMPLE_OP(and, FI_BAND, uint64, FI_UINT64, uint64_t)

DEFN_IFACE_AMO_SIMPLE_OP(or, FI_BOR, int32, FI_INT32, int32_t)
DEFN_IFACE_AMO_SIMPLE_OP(or, FI_BOR, int64, FI_INT64, int64_t)
DEFN_IFACE_AMO_SIMPLE_OP(or, FI_BOR, uint32, FI_UINT32, uint32_t)
DEFN_IFACE_AMO_SIMPLE_OP(or, FI_BOR, uint64, FI_UINT64, uint64_t)

DEFN_IFACE_AMO_SIMPLE_OP(xor, FI_BXOR, int32, FI_INT32, int32_t)
DEFN_IFACE_AMO_SIMPLE_OP(xor, FI_BXOR, int64, FI_INT64, int64_t)
DEFN_IFACE_AMO_SIMPLE_OP(xor, FI_BXOR, uint32, FI_UINT32, uint32_t)
DEFN_IFACE_AMO_SIMPLE_OP(xor, FI_BXOR, uint64, FI_UINT64, uint64_t)

DEFN_IFACE_AMO_SIMPLE_OP(add, FI_SUM, int32, FI_INT32, int32_t)
DEFN_IFACE_AMO_SIMPLE_OP(add, FI_SUM, int64, FI_INT64, int64_t)
DEFN_IFACE_AMO_SIMPLE_OP(add, FI_SUM, uint32, FI_UINT32, uint32_t)
DEFN_IFACE_AMO_SIMPLE_OP(add, FI_SUM, uint64, FI_UINT64, uint64_t)
DEFN_IFACE_AMO_SIMPLE_OP(add, FI_SUM, real32, FI_FLOAT, _real32)
DEFN_IFACE_AMO_SIMPLE_OP(add, FI_SUM, real64, FI_DOUBLE, _real64)


#define DEFN_IFACE_AMO_SUB(fnType, ofiType, Type, negate)               \
  void chpl_comm_atomic_sub_##fnType                                    \
         (void* opnd, c_nodeid_t node, void* object,                    \
          memory_order order, int ln, int32_t fn) {                     \
    DBG_PRINTF(DBG_IFACE_AMO,                                           \
               "%s(<%s>, %d, %p, %d, %s)", __func__,                    \
               DBG_VAL(opnd, ofiType), (int) node, object,              \
               ln, chpl_lookupFilename(fn));                            \
    Type myOpnd = negate(*(Type*) opnd);                                \
    chpl_comm_diags_verbose_amo("amo sub", node, ln, fn);               \
    chpl_comm_diags_incr(amo);                                          \
    doAMO(node, object, &myOpnd, NULL, NULL,                            \
          FI_SUM, ofiType, sizeof(Type));                               \
  }                                                                     \
                                                                        \
  void chpl_comm_atomic_sub_unordered_##fnType                          \
         (void* opnd, c_nodeid_t node, void* object,                    \
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_IFACE_AMO,                                           \
               "%s(<%s>, %d, %p, %d, %s)", __func__,                    \
               DBG_VAL(opnd, ofiType), (int) node, object,              \
               ln, chpl_lookupFilename(fn));                            \
    Type myOpnd = negate(*(Type*) opnd);                                \
    chpl_comm_diags_verbose_amo("amo unord_sub", node, ln, fn);         \
    chpl_comm_diags_incr(amo);                                          \
    do_remote_amo_nf_buff(&myOpnd, node, object, sizeof(Type),          \
                          FI_SUM, ofiType);                             \
  }                                                                     \
                                                                        \
  void chpl_comm_atomic_fetch_sub_##fnType                              \
         (void* opnd, c_nodeid_t node, void* object, void* result,      \
          memory_order order, int ln, int32_t fn) {                     \
    DBG_PRINTF(DBG_IFACE_AMO,                                           \
               "%s(<%s>, %d, %p, %p, %d, %s)", __func__,                \
               DBG_VAL(opnd, ofiType), (int) node, object,              \
               result, ln, chpl_lookupFilename(fn));                    \
    Type myOpnd = negate(*(Type*) opnd);                                \
    chpl_comm_diags_verbose_amo("amo fetch_sub", node, ln, fn);         \
    chpl_comm_diags_incr(amo);                                          \
    doAMO(node, object, &myOpnd, NULL, result,                          \
          FI_SUM, ofiType, sizeof(Type));                               \
  }

#define NEGATE_I32(x) ((x) == INT32_MIN ? (x) : -(x))
#define NEGATE_I64(x) ((x) == INT64_MIN ? (x) : -(x))
#define NEGATE_U_OR_R(x) (-(x))

DEFN_IFACE_AMO_SUB(int32, FI_INT32, int32_t, NEGATE_I32)
DEFN_IFACE_AMO_SUB(int64, FI_INT64, int64_t, NEGATE_I64)
DEFN_IFACE_AMO_SUB(uint32, FI_UINT32, uint32_t, NEGATE_U_OR_R)
DEFN_IFACE_AMO_SUB(uint64, FI_UINT64, uint64_t, NEGATE_U_OR_R)
DEFN_IFACE_AMO_SUB(real32, FI_FLOAT, _real32, NEGATE_U_OR_R)
DEFN_IFACE_AMO_SUB(real64, FI_DOUBLE, _real64, NEGATE_U_OR_R)

void chpl_comm_atomic_unordered_task_fence(void) {
  DBG_PRINTF(DBG_IFACE_MCM, "%s()", __func__);

  task_local_buff_flush(amo_nf_buff);
}


//
// internal AMO utilities
//

static
int computeAtomicValid(enum fi_datatype ofiType) {
  //
  // At least one provider (ofi_rxm) segfaults if the endpoint given to
  // fi*atomicvalid() entirely lacks atomic caps.  The man page isn't
  // clear on whether this should work, so just avoid that situation.
  //
  if ((ofi_info->tx_attr->caps & FI_ATOMIC) == 0) {
    return 0;
  }

  struct fid_ep* ep = tciTab[0].txCtx; // assume same answer for all endpoints
  size_t count;                        // ignored

#define my_valid(typ, op) \
  (fi_atomicvalid(ep, typ, op, &count) == 0 && count > 0)
#define my_fetch_valid(typ, op) \
  (fi_fetch_atomicvalid(ep, typ, op, &count) == 0 && count > 0)
#define my_compare_valid(typ, op) \
  (fi_compare_atomicvalid(ep, typ, op, &count) == 0 && count > 0)

  // For integral types, all operations matter.
  if (ofiType == FI_INT32
      || ofiType == FI_UINT32
      || ofiType == FI_INT64
      || ofiType == FI_UINT64) {
    return (   my_valid(ofiType, FI_SUM)
            && my_valid(ofiType, FI_BOR)
            && my_valid(ofiType, FI_BAND)
            && my_valid(ofiType, FI_BXOR)
            && my_valid(ofiType, FI_ATOMIC_WRITE)
            && my_fetch_valid(ofiType, FI_SUM)
            && my_fetch_valid(ofiType, FI_BOR)
            && my_fetch_valid(ofiType, FI_BAND)
            && my_fetch_valid(ofiType, FI_BXOR)
            && my_fetch_valid(ofiType, FI_ATOMIC_READ)
            && my_fetch_valid(ofiType, FI_ATOMIC_WRITE)
            && my_compare_valid(ofiType, FI_CSWAP));
  }

  //
  // For real types, only sum, read, write, and cswap matter.
  //
  return (   my_valid(ofiType, FI_SUM)
          && my_valid(ofiType, FI_ATOMIC_WRITE)
          && my_fetch_valid(ofiType, FI_SUM)
          && my_fetch_valid(ofiType, FI_ATOMIC_READ)
          && my_fetch_valid(ofiType, FI_ATOMIC_WRITE)
          && my_compare_valid(ofiType, FI_CSWAP));

#undef my_valid
#undef my_fetch_valid
#undef my_compare_valid
}

static
int isAtomicValid(enum fi_datatype ofiType) {
  static chpl_bool inited = false;
  static int validByType[FI_DATATYPE_LAST];

  if (!inited) {
    validByType[FI_INT32]  = computeAtomicValid(FI_INT32);
    validByType[FI_UINT32] = computeAtomicValid(FI_UINT32);
    validByType[FI_INT64]  = computeAtomicValid(FI_INT64);
    validByType[FI_UINT64] = computeAtomicValid(FI_UINT64);
    validByType[FI_FLOAT]  = computeAtomicValid(FI_FLOAT);
    validByType[FI_DOUBLE] = computeAtomicValid(FI_DOUBLE);
    inited = true;
  }

  return validByType[ofiType];
}


static inline
void doAMO(c_nodeid_t node, void* object,
           const void* opnd, const void* cmpr, void* result,
           int ofiOp, enum fi_datatype ofiType, size_t size) {
  if (chpl_numNodes <= 1) {
    doCpuAMO(object, opnd, cmpr, result, ofiOp, ofiType, size);
    return;
  }

  retireDelayedAmDone(false /*taskIsEnding*/);

  uint64_t mrKey;
  uint64_t mrRaddr;
  if (!isAtomicValid(ofiType)
      || !mrGetKey(&mrKey, &mrRaddr, node, object, size)) {
    //
    // We can't do the AMO on the network, so do it on the CPU.  If the
    // object is on this node do it directly; otherwise, use an AM.
    //
    if (node == chpl_nodeID) {
      if (ofiOp != FI_ATOMIC_READ) {
        forceMemFxVisAllNodes_noTcip(true /*checkPuts*/, true /*checkAmos*/);
      }
      doCpuAMO(object, opnd, cmpr, result, ofiOp, ofiType, size);
    } else {
      amRequestAMO(node, object, opnd, cmpr, result,
                   ofiOp, ofiType, size);
    }
  } else {
    //
    // The type is supported for network atomics and the object address
    // is remotely accessible.  Do the AMO natively.
    //
    ofi_amo(node, mrRaddr, mrKey, opnd, cmpr, result,
            ofiOp, ofiType, size);
  }
}


static inline
void doCpuAMO(void* obj,
              const void* opnd, const void* cmpr, void* result,
              enum fi_op ofiOp, enum fi_datatype ofiType, size_t size) {
  CHK_TRUE(size == 4 || size == 8);

  chpl_amo_datum_t* myOpnd = (chpl_amo_datum_t*) opnd;
  chpl_amo_datum_t* myCmpr = (chpl_amo_datum_t*) cmpr;

#define CPU_INT_ARITH_AMO(_o, _t, _m)                                   \
  do {                                                                  \
    if (result == NULL) {                                               \
      (void) atomic_fetch_##_o##_##_t((atomic_##_t*) obj,               \
                                      myOpnd->_m);                      \
    } else {                                                            \
      *(_t*) result = atomic_fetch_##_o##_##_t((atomic_##_t*) obj,      \
                                               myOpnd->_m);             \
    }                                                                   \
  } while (0)

  //
  // Here we implement AMOs which the NIC cannot or should not do.
  //
  switch (ofiOp) {
  case FI_ATOMIC_WRITE:
    if (result == NULL) {
      //
      // write
      //
      if (size == 4) {
        atomic_store_uint_least32_t(obj, myOpnd->u32);
      } else {
        atomic_store_uint_least64_t(obj, myOpnd->u64);
      }
    } else {
      //
      // exchange
      //
      if (size == 4) {
        *(uint32_t*) result = atomic_exchange_uint_least32_t(obj, myOpnd->u32);
      } else {
        *(uint64_t*) result = atomic_exchange_uint_least64_t(obj, myOpnd->u64);
      }
    }
    break;

  case FI_ATOMIC_READ:
    if (size == 4) {
      *(uint32_t*) result = atomic_load_uint_least32_t(obj);
    } else {
      *(uint64_t*) result = atomic_load_uint_least64_t(obj);
    }
    break;

  case FI_CSWAP:
    if (size == 4) {
      uint32_t myCmprVal = myCmpr->u32;
      (void) atomic_compare_exchange_strong_uint_least32_t(obj,
                                                           &myCmprVal,
                                                           myOpnd->u32);
      *(uint32_t*) result = myCmprVal;
    } else {
      uint64_t myCmprVal = myCmpr->u64;
      (void) atomic_compare_exchange_strong_uint_least64_t(obj,
                                                           &myCmprVal,
                                                           myOpnd->u64);
      *(uint64_t*) result = myCmprVal;
    }
    break;

  case FI_BAND:
    if (ofiType == FI_INT32) {
      CPU_INT_ARITH_AMO(and, int_least32_t, i32);
    } else if (ofiType == FI_UINT32) {
      CPU_INT_ARITH_AMO(and, uint_least32_t, u32);
    } else if (ofiType == FI_INT64) {
      CPU_INT_ARITH_AMO(and, int_least64_t, i64);
    } else if (ofiType == FI_UINT64) {
      CPU_INT_ARITH_AMO(and, uint_least64_t, u64);
    } else {
      INTERNAL_ERROR_V("doCpuAMO(): unsupported ofiOp %d, ofiType %d",
                       ofiOp, ofiType);
    }
    break;

  case FI_BOR:
    if (ofiType == FI_INT32) {
      CPU_INT_ARITH_AMO(or, int_least32_t, i32);
    } else if (ofiType == FI_UINT32) {
      CPU_INT_ARITH_AMO(or, uint_least32_t, u32);
    } else if (ofiType == FI_INT64) {
      CPU_INT_ARITH_AMO(or, int_least64_t, i64);
    } else if (ofiType == FI_UINT64) {
      CPU_INT_ARITH_AMO(or, uint_least64_t, u64);
    } else {
      INTERNAL_ERROR_V("doCpuAMO(): unsupported ofiOp %d, ofiType %d",
                       ofiOp, ofiType);
    }
    break;

  case FI_BXOR:
    if (ofiType == FI_INT32) {
      CPU_INT_ARITH_AMO(xor, int_least32_t, i32);
    } else if (ofiType == FI_UINT32) {
      CPU_INT_ARITH_AMO(xor, uint_least32_t, u32);
    } else if (ofiType == FI_INT64) {
      CPU_INT_ARITH_AMO(xor, int_least64_t, i64);
    } else if (ofiType == FI_UINT64) {
      CPU_INT_ARITH_AMO(xor, uint_least64_t, u64);
    } else {
      INTERNAL_ERROR_V("doCpuAMO(): unsupported ofiOp %d, ofiType %d",
                       ofiOp, ofiType);
    }
    break;

  case FI_SUM:
    if (ofiType == FI_INT32) {
      CPU_INT_ARITH_AMO(add, int_least32_t, i32);
    } else if (ofiType == FI_UINT32) {
      CPU_INT_ARITH_AMO(add, uint_least32_t, u32);
    } else if (ofiType == FI_INT64) {
      CPU_INT_ARITH_AMO(add, int_least64_t, i64);
    } else if (ofiType == FI_UINT64) {
      CPU_INT_ARITH_AMO(add, uint_least64_t, u64);
    } else if (ofiType == FI_FLOAT) {
      CPU_INT_ARITH_AMO(add, _real32, r32);
    } else if (ofiType == FI_DOUBLE) {
      CPU_INT_ARITH_AMO(add, _real64, r64);
    } else {
      INTERNAL_ERROR_V("doCpuAMO(): unsupported ofiOp %d, ofiType %d",
                       ofiOp, ofiType);
    }
    break;


  default:
    INTERNAL_ERROR_V("doCpuAMO(): unsupported ofiOp %d, ofiType %d",
                     ofiOp, ofiType);
  }

  if (DBG_TEST_MASK(DBG_AMO | DBG_AMO_READ)) {
    if (result == NULL) {
      DBG_PRINTF(DBG_AMO,
                 "doCpuAMO(%p, %s, %s, %s): now %s",
                 obj, amo_opName(ofiOp), amo_typeName(ofiType),
                 DBG_VAL(myOpnd, ofiType),
                 DBG_VAL((chpl_amo_datum_t*) obj, ofiType));
    } else if (ofiOp == FI_ATOMIC_READ) {
      DBG_PRINTF(DBG_AMO_READ,
                 "doCpuAMO(%p, %s, %s): res %p is %s",
                 obj, amo_opName(ofiOp), amo_typeName(ofiType), result,
                 DBG_VAL(result, ofiType));
    } else {
      DBG_PRINTF(DBG_AMO,
                 "doCpuAMO(%p, %s, %s, %s, %s): now %s, res %p is %s",
                 obj, amo_opName(ofiOp), amo_typeName(ofiType),
                 DBG_VAL(myOpnd, ofiType),
                 DBG_VAL(myCmpr, ofiType),
                 DBG_VAL((chpl_amo_datum_t*) obj, ofiType), result,
                 DBG_VAL(result, (ofiOp == FI_CSWAP) ? FI_INT32 : ofiType));
    }
  }

#undef CPU_INT_ARITH_AMO
}


/*
 *** START OF NON-FETCHING BUFFERED ATOMIC OPERATIONS ***
 *
 * Support for non-fetching buffered atomic operations. We internally buffer
 * atomic operations and then initiate them all at once for increased
 * transaction rate.
 */

// Flush buffered AMOs for the specified task info and reset the counter.
static inline
void amo_nf_buff_task_info_flush(amo_nf_buff_task_info_t* info) {
  if (info->vi > 0) {
    DBG_PRINTF(DBG_AMO_UNORD,
               "amo_nf_buff_task_info_flush(): info has %d entries",
               info->vi);
    ofi_amo_nf_V(info->vi, info->opnd_v, info->local_mr,
                 info->locale_v, info->object_v, info->remote_mr_v,
                 info->size_v, info->cmd_v, info->type_v);
    info->vi = 0;
  }
}


static inline
void do_remote_amo_nf_buff(void* opnd, c_nodeid_t node,
                           void* object, size_t size,
                           enum fi_op ofiOp, enum fi_datatype ofiType) {
  //
  // "Unordered" is possible only for actual network atomic ops.
  //
  if (chpl_numNodes <= 1) {
    doCpuAMO(object, opnd, NULL, NULL, ofiOp, ofiType, size);
    return;
  }

  retireDelayedAmDone(false /*taskIsEnding*/);

  uint64_t mrKey;
  uint64_t mrRaddr;
  if (!isAtomicValid(ofiType)
      || !mrGetKey(&mrKey, &mrRaddr, node, object, size)) {
    if (node == chpl_nodeID) {
      doCpuAMO(object, opnd, NULL, NULL, ofiOp, ofiType, size);
    } else {
      amRequestAMO(node, object, opnd, NULL, NULL,
                   ofiOp, ofiType, size);
    }
    return;
  }

  amo_nf_buff_task_info_t* info = task_local_buff_acquire(amo_nf_buff);
  if (info == NULL) {
    ofi_amo(node, mrRaddr, mrKey, opnd, NULL, NULL, ofiOp, ofiType, size);
    return;
  }

  if (info->new) {
    //
    // The AMO operands are stored in a vector in the info, and we only
    // need one local memory descriptor for that vector.
    //
    CHK_TRUE(mrGetDesc(&info->local_mr, info->opnd_v, size));
    info->new = false;
  }

  int vi = info->vi;
  info->opnd_v[vi]      = size == 4 ? *(uint32_t*) opnd:
                                      *(uint64_t*) opnd;
  info->locale_v[vi]    = node;
  info->object_v[vi]    = (void*) mrRaddr;
  info->size_v[vi]      = size;
  info->cmd_v[vi]       = ofiOp;
  info->type_v[vi]      = ofiType;
  info->remote_mr_v[vi] = mrKey;
  info->vi++;

  DBG_PRINTF(DBG_AMO_UNORD,
             "do_remote_amo_nf_buff(): info[%d] = "
             "{%p, %d, %p, %zd, %d, %d, %" PRIx64 ", %p}",
             vi, &info->opnd_v[vi], (int) node, object, size,
             (int) ofiOp, (int) ofiType, mrKey, info->local_mr);

  // flush if buffers are full
  if (info->vi == MAX_CHAINED_AMO_NF_LEN) {
    amo_nf_buff_task_info_flush(info);
  }
}
/*** END OF NON-FETCHING BUFFERED ATOMIC OPERATIONS ***/


////////////////////////////////////////
//
// Interface: utility
//

int chpl_comm_addr_gettable(c_nodeid_t node, void* start, size_t size) {
  // No way to know if the page is mapped on the remote (without a round trip)
  return 0;
}


int32_t chpl_comm_getMaxThreads(void) {
  // no limit
  return 0;
}


////////////////////////////////////////
//
// Interface: barriers
//

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
// and release flags to be set.  In fact we have to task-yield while
// doing so, in case the PUTs need to be done via AM for some reason
// (unregistered memory, e.g.).
//
// TODO: vectorize the child PUTs.
//
#define BAR_TREE_NUM_CHILDREN 64

typedef struct {
  volatile int child_notify[BAR_TREE_NUM_CHILDREN];
  volatile int parent_release;
}  bar_info_t;

static c_nodeid_t bar_childFirst;
static c_nodeid_t bar_numChildren;
static c_nodeid_t bar_parent;

static bar_info_t bar_info;
static bar_info_t** bar_infoMap;


static
void init_bar(void) {
  bar_childFirst = BAR_TREE_NUM_CHILDREN * chpl_nodeID + 1;
  if (bar_childFirst >= chpl_numNodes)
    bar_numChildren = 0;
  else {
    bar_numChildren = BAR_TREE_NUM_CHILDREN;
    if (bar_childFirst + bar_numChildren >= chpl_numNodes)
      bar_numChildren = chpl_numNodes - bar_childFirst;
  }
  bar_parent = (chpl_nodeID - 1) / BAR_TREE_NUM_CHILDREN;

  CHPL_CALLOC(bar_infoMap, chpl_numNodes);
  const bar_info_t* p = &bar_info;
  chpl_comm_ofi_oob_allgather(&p, bar_infoMap, sizeof(p));
}


void chpl_comm_impl_barrier(const char *msg) {
  DBG_PRINTF(DBG_IFACE_SETUP, "%s('%s')", __func__, msg);

  DBG_PRINTF(DBG_BARRIER, "barrier '%s'", (msg == NULL) ? "" : msg);

  if (pthread_equal(pthread_self(), pthread_that_inited)
      || numAmHandlersActive == 0) {
    //
    // Either this is the main (chpl_comm_init()ing) thread or
    // comm layer setup is not complete yet.  Use OOB barrier.
    //
    chpl_comm_ofi_oob_barrier();
    DBG_PRINTF(DBG_BARRIER, "barrier '%s' done via out-of-band",
               (msg == NULL) ? "" : msg);
    return;
  }

  //
  // Ensure our outstanding nonfetching AMOs and PUTs are visible.
  // (Visibility of operations done by other tasks on this node is
  // the caller's responsibility.)
  //
  retireDelayedAmDone(false /*taskIsEnding*/);
  forceMemFxVisAllNodes_noTcip(true /*checkPuts*/, true /*checkAmos*/);

  //
  // Wait for our child locales to notify us that they have reached the
  // barrier.
  //
  DBG_PRINTF(DBG_BARRIER, "BAR wait for %d children", (int) bar_numChildren);
  for (uint32_t i = 0; i < bar_numChildren; i++) {
    while (bar_info.child_notify[i] == 0) {
      local_yield();
    }
  }

  const int one = 1;

  if (chpl_nodeID != 0) {
    //
    // Notify our parent locale that we have reached the barrier.
    //
    c_nodeid_t parChild = (chpl_nodeID - 1) % BAR_TREE_NUM_CHILDREN;

    DBG_PRINTF(DBG_BARRIER, "BAR notify parent %d", (int) bar_parent);
    ofi_put(&one, bar_parent,
            (void*) &bar_infoMap[bar_parent]->child_notify[parChild],
            sizeof(one));

    //
    // Wait for our parent locale to release us from the barrier.
    //
    DBG_PRINTF(DBG_BARRIER, "BAR wait for parental release");
    while (bar_info.parent_release == 0) {
      local_yield();
    }
  }

  //
  // Clear all our barrier flags.
  //
  for (int i = 0; i < bar_numChildren; i++)
    bar_info.child_notify[i] = 0;
  bar_info.parent_release = 0;

  //
  // Release our children.
  //
  if (bar_numChildren > 0) {
    for (int i = 0; i < bar_numChildren; i++) {
      c_nodeid_t child = bar_childFirst + i;
      DBG_PRINTF(DBG_BARRIER, "BAR release child %d", (int) child);
      ofi_put(&one, child,
              (void*) &bar_infoMap[child]->parent_release,
              sizeof(one));
    }
  }

  DBG_PRINTF(DBG_BARRIER, "barrier '%s' done via PUTs",
             (msg == NULL) ? "" : msg);
}


////////////////////////////////////////
//
// Time
//

static double timeBase;


static
void time_init(void) {
  timeBase = chpl_comm_ofi_time_get();
}


double chpl_comm_ofi_time_get(void) {
  struct timespec _ts;
  (void) clock_gettime(CLOCK_MONOTONIC, &_ts);
  return ((double) _ts.tv_sec + (double) _ts.tv_nsec * 1e-9) - timeBase;
}


////////////////////////////////////////
//
// Error reporting
//

//
// Here we just handle a few special cases where we think we can be
// more informative than usual.  If we return, the usual internal
// error message will be printed.
//
static
void ofiErrReport(const char* exprStr, int retVal, const char* errStr) {
  if (retVal == -FI_EMFILE) {
    //
    // We've run into the limit on the number of files we can have open
    // at once.
    //
    // Some providers open a lot of files.  The tcp provider, as one
    // example, can open as many as roughly 9 files per node, plus 2
    // socket files for each connected endpoint.  Because of this, one
    // can exceed a quite reasonable open-file limit in a job running on
    // a fairly modest number of many-core locales.  Thus for example,
    // extremeBlock will get -FI_EMFILE with an open file limit of 1024
    // when run on 32 24-core locales.  Here, try to inform the user
    // about this without getting overly technical.
    //
    INTERNAL_ERROR_V(
      "OFI error: %s: %s:\n"
      "  The program has reached the limit on the number of files it can\n"
      "  have open at once.  This may be because the product of the number\n"
      "  of locales (%d) and the communication concurrency (roughly %d) is\n"
      "  a significant fraction of the open-file limit (%ld).  If so,\n"
      "  either setting CHPL_RT_COMM_CONCURRENCY to decrease communication\n"
      "  concurrency or running on fewer locales may allow the program to\n"
      "  execute successfully.  Or, you may be able to use `ulimit` to\n"
      "  increase the open file limit and achieve the same result.",
      exprStr, errStr, (int) chpl_numNodes, numTxCtxs, sysconf(_SC_OPEN_MAX));
  }
}


#ifdef CHPL_COMM_DEBUG

////////////////////////////////////////
//
// Debugging support
//

void chpl_comm_ofi_dbg_init(void) {
  const char* ev;
  if ((ev = chpl_env_rt_get("COMM_OFI_DEBUG", NULL)) == NULL) {
    return;
  }

  //
  // Compute the debug level from the keywords in the env var.
  //
  {
    #define OFIDBG_MACRO(_enum, _desc) { #_enum, _desc },
    static struct {
      const char* kw;
      const char* desc;
    } dbgCodes[] = { OFI_ALL_DEBUGS(OFIDBG_MACRO) };
    static const int dbgCodesLen = sizeof(dbgCodes) / sizeof(dbgCodes[0]);

    char evc[strlen(ev) + 1];  // non-constant, for strtok()
    strcpy(evc, ev);

    //
    // Loop over comma-separated tokens in the env var.
    //
    chpl_comm_ofi_dbg_level = 0;
    char* tok;
    char* strSave;
    for (char* evcPtr = evc;
         (tok = strtok_r(evcPtr, ",", &strSave)) != NULL;
         evcPtr = NULL) {
      //
      // Users can use lowercase and dashes; table contains uppercase
      // and underbars, because it defines C symbols.  Canonicalize
      // user's token.
      //
      char ctok[strlen(tok)];
      for (int i = 0; i < sizeof(ctok); i++) {
        if (tok[i] == '-') {
          ctok[i] = '_';
        } else {
          ctok[i] = toupper(tok[i]);
        }
      }

      //
      // Find user's keyword in table.
      //
      int i, iPrefix;
      for (i = 0, iPrefix = -1; i < dbgCodesLen; i++) {
        if (strncmp(ctok, dbgCodes[i].kw, sizeof(ctok)) == 0) {
          if (dbgCodes[i].kw[sizeof(ctok)] == '\0') {
            break;
          } else {
            // Accept a match to the prefix of a keyword iff unambiguous.
            iPrefix = (iPrefix < 0) ? i : dbgCodesLen;
          }
        }
      }

      //
      // Add found debug bit to our set of same, or say "what?".
      //
      if (i >= 0 && i < dbgCodesLen) {
        chpl_comm_ofi_dbg_level |= (uint64_t) 1 << i;
      } else if (iPrefix >= 0 && iPrefix < dbgCodesLen) {
        chpl_comm_ofi_dbg_level |= (uint64_t) 1 << iPrefix;
      } else {
        //
        // All nodes exit on error, but only node 0 says why.
        //
        if (chpl_nodeID == 0) {
          if (ctok[0] != '?' && strncmp(ctok, "HELP", sizeof(ctok)) != 0) {
            printf("Warning: unknown or ambiguous comm=ofi debug keyword "
                   "\"%s\"\n",
                   tok);
          }

          //
          // Print pretty table of debug keywords and descriptions.
          //
          printf("Debug keywords (case ignored, -_ equiv) and descriptions\n");
          printf("--------------------------------------------------------\n");

          int kwLenMax = 0;
          for (int ic = 0; ic < dbgCodesLen; ic++) {
            if (strlen(dbgCodes[ic].kw) > kwLenMax) {
              kwLenMax = strlen(dbgCodes[ic].kw);
            }
          }

          for (int ic = 0; ic < dbgCodesLen; ic++) {
            char kw[strlen(dbgCodes[ic].kw)];
            for (int ik = 0; ik < sizeof(kw); ik++) {
              if (dbgCodes[ic].kw[ik] == '_') {
                kw[ik] = '-';
              } else {
                kw[ik] = tolower(dbgCodes[ic].kw[ik]);
              }
            }
            printf("%.*s:%*s %s\n",
                   (int) sizeof(kw), kw, (int) (kwLenMax - sizeof(kw)), "",
                   dbgCodes[ic].desc);
          }
        }

        chpl_comm_ofi_oob_fini();
        chpl_exit_any(0);
      }
    }
  }

  if ((ev = chpl_env_rt_get("COMM_OFI_DEBUG_FNAME", NULL)) == NULL) {
    chpl_comm_ofi_dbg_file = stderr;
  } else if (!strcmp(ev, "STDOUT")) {
    chpl_comm_ofi_dbg_file = stdout;
  } else if (!strcmp(ev, "STDERR")) {
    chpl_comm_ofi_dbg_file = stderr;
  } else {
    char fname[strlen(ev) + 6 + 1];
    int fnameLen;
    fnameLen = snprintf(fname, sizeof(fname), "%s.%d", ev, (int) chpl_nodeID);
    CHK_TRUE(fnameLen < sizeof(fname));
    CHK_TRUE((chpl_comm_ofi_dbg_file = fopen(fname, "w")) != NULL);
    setbuf(chpl_comm_ofi_dbg_file, NULL);
  }
}


char* chpl_comm_ofi_dbg_prefix(void) {
  static __thread char buf[30];
  int len;

  if (buf[0] == '\0' || DBG_TEST_MASK(DBG_TSTAMP)) {
    buf[len = 0] = '\0';
    if (chpl_nodeID >= 0)
      len += snprintf(&buf[len], sizeof(buf) - len, "%d", chpl_nodeID);
    if (chpl_task_getId() == chpl_nullTaskID)
      len += snprintf(&buf[len], sizeof(buf) - len, ":_");
    else
      len += snprintf(&buf[len], sizeof(buf) - len, ":%ld",
                      (long int) chpl_task_getId());
    if (DBG_TEST_MASK(DBG_TSTAMP))
      len += snprintf(&buf[len], sizeof(buf) - len, "%s%.9f",
                      ((len == 0) ? "" : ": "), chpl_comm_ofi_time_get());
    if (len > 0)
      len += snprintf(&buf[len], sizeof(buf) - len, ": ");
  }
  return buf;
}


char* chpl_comm_ofi_dbg_val(const void* pV, enum fi_datatype ofiType) {
  static __thread char buf[5][32];
  static __thread int iBuf = 0;
  char* s = buf[iBuf];

  if (pV == NULL) {
    snprintf(s, sizeof(buf[0]), "NIL");
  } else {
    switch (ofiType) {
    case FI_INT32:
      snprintf(s, sizeof(buf[0]), "%" PRId32, *(const int_least32_t*) pV);
      break;
    case FI_UINT32:
      snprintf(s, sizeof(buf[0]), "%#" PRIx32, *(const uint_least32_t*) pV);
      break;
    case FI_INT64:
      snprintf(s, sizeof(buf[0]), "%" PRId64, *(const int_least64_t*) pV);
      break;
    case FI_FLOAT:
      snprintf(s, sizeof(buf[0]), "%.6g", (double) *(const float*) pV);
      break;
    case FI_DOUBLE:
      snprintf(s, sizeof(buf[0]), "%.16g", *(const double*) pV);
      break;
    default:
      snprintf(s, sizeof(buf[0]), "%#" PRIx64, *(const uint_least64_t*) pV);
      break;
    }
  }

  if (++iBuf >= sizeof(buf) / sizeof(buf[0]))
    iBuf = 0;

  return s;
}


static
const char* am_opName(amOp_t op) {
  switch (op) {
  case am_opExecOn: return "opExecOn";
  case am_opExecOnLrg: return "opExecOnLrg";
  case am_opGet: return "opGet";
  case am_opPut: return "opPut";
  case am_opAMO: return "opAMO";
  case am_opFAMOResult: return "opFAMOResult";
  case am_opFree: return "opFree";
  case am_opNop: return "opNop";
  case am_opShutdown: return "opShutdown";
  default: return "op???";
  }
}


static
const char* amo_opName(enum fi_op ofiOp) {
  switch (ofiOp) {
  case FI_ATOMIC_WRITE: return "write";
  case FI_ATOMIC_READ: return "read";
  case FI_CSWAP: return "cswap";
  case FI_BAND: return "band";
  case FI_BOR: return "bor";
  case FI_BXOR: return "bxor";
  case FI_SUM: return "sum";
  default: return "amoOp???";
  }
}


static
const char* amo_typeName(enum fi_datatype ofiType) {
  switch (ofiType) {
  case FI_INT8: return "char";
  case FI_UINT8: return "uchar";
  case FI_INT16: return "int16";
  case FI_UINT16: return "uint16";
  case FI_INT32: return "int32";
  case FI_UINT32: return "uint32";
  case FI_INT64: return "int64";
  case FI_UINT64: return "uint64";
  case FI_FLOAT: return "real32";
  case FI_DOUBLE: return "real64";
  case FI_FLOAT_COMPLEX: return "float-complex";
  case FI_DOUBLE_COMPLEX: return "double-complex";
  case FI_LONG_DOUBLE: return "long-double";
  case FI_LONG_DOUBLE_COMPLEX: return "long-double-complex";
  default: return "amoType???";
  }
}


static
const char* am_seqIdStr(amRequest_t* req) {
  static __thread char buf[30];
  if (op_uses_on_bundle(req->b.op)) {
    (void) snprintf(buf, sizeof(buf), "%d:%" PRIu64,
                   req->xo.hdr.comm.node, req->xo.hdr.comm.seq);
  } else {
    (void) snprintf(buf, sizeof(buf), "%d:%" PRIu64,
                   req->b.node, req->b.seq);
  }
  return buf;
}


static
const char* am_reqStr(c_nodeid_t tgtNode, amRequest_t* req, size_t reqSize) {
  static __thread char buf[1000];
  int len;

  len = snprintf(buf, sizeof(buf), "seqId %s, %s, sz %zd",
                 am_seqIdStr(req), am_opName(req->b.op), reqSize);

  switch (req->b.op) {
  case am_opExecOn:
    len += snprintf(buf + len, sizeof(buf) - len, ", fid %d(arg %p, sz %zd)%s",
                    req->xo.hdr.comm.fid, &req->xo.hdr.payload,
                    reqSize - offsetof(chpl_comm_on_bundle_t, payload),
                    req->xo.hdr.comm.fast ? ", fast" : "");
    break;

  case am_opExecOnLrg:
    len += snprintf(buf + len, sizeof(buf) - len, ", fid %d(arg %p, sz %zd)",
                    req->xol.hdr.comm.fid, req->xol.pPayload,
                    req->xol.hdr.comm.argSize);
    break;

  case am_opGet:
    len += snprintf(buf + len, sizeof(buf) - len, ", %d:%p <- %d:%p, sz %zd",
                    (int) tgtNode, req->rma.addr,
                    req->rma.b.node, req->rma.raddr,
                    req->rma.size);
    break;
  case am_opPut:
    len += snprintf(buf + len, sizeof(buf) - len, ", %d:%p -> %d:%p, sz %zd",
                    (int) tgtNode, req->rma.addr,
                    req->rma.b.node, req->rma.raddr, req->rma.size);
    break;

  case am_opAMO:
    if (req->amo.ofiOp == FI_CSWAP) {
      len += snprintf(buf + len, sizeof(buf) - len,
                      ", obj %p, opnd %s, cmpr %s, res %p"
                      ", ofiOp %s, ofiType %s, sz %d",
                      req->amo.obj,
                      DBG_VAL(&req->amo.opnd, req->amo.ofiType),
                      DBG_VAL(&req->amo.cmpr, req->amo.ofiType),
                      req->amo.result, amo_opName(req->amo.ofiOp),
                      amo_typeName(req->amo.ofiType), req->amo.size);
    } else if (req->amo.result != NULL) {
      if (req->amo.ofiOp == FI_ATOMIC_READ) {
        len += snprintf(buf + len, sizeof(buf) - len,
                        ", obj %p, res %p"
                        ", ofiOp %s, ofiType %s, sz %d",
                        req->amo.obj,
                        req->amo.result, amo_opName(req->amo.ofiOp),
                        amo_typeName(req->amo.ofiType), req->amo.size);
      } else {
        len += snprintf(buf + len, sizeof(buf) - len,
                        ", obj %p, opnd %s, res %p"
                        ", ofiOp %s, ofiType %s, sz %d",
                        req->amo.obj,
                        DBG_VAL(&req->amo.opnd, req->amo.ofiType),
                        req->amo.result, amo_opName(req->amo.ofiOp),
                        amo_typeName(req->amo.ofiType), req->amo.size);
      }
    } else {
      len += snprintf(buf + len, sizeof(buf) - len,
                      ", obj %p, opnd %s"
                      ", ofiOp %s, ofiType %s, sz %d",
                      req->amo.obj,
                      DBG_VAL(&req->amo.opnd, req->amo.ofiType),
                      amo_opName(req->amo.ofiOp),
                      amo_typeName(req->amo.ofiType), req->amo.size);
    }
    break;

  case am_opFAMOResult:
    len += snprintf(buf + len, sizeof(buf) - len,
                    ", res %p, result %s"
                    ", ofiType %s, sz %d",
                    req->famo_result.res,
                    DBG_VAL(&req->famo_result.result, req->famo_result.ofiType),
                    amo_typeName(req->famo_result.ofiType),
                    req->famo_result.size);
    break;

  case am_opFree:
    len += snprintf(buf + len, sizeof(buf) - len, ", %p",
                    req->free.p);
    break;

  default:
    break;
  }

  amDone_t* pAmDone = op_uses_on_bundle(req->b.op)
                      ? req->xo.hdr.comm.pAmDone
                      : req->b.pAmDone;
  if (pAmDone == NULL) {
    (void) snprintf(buf + len, sizeof(buf) - len, ", NB");
  } else {
    (void) snprintf(buf + len, sizeof(buf) - len, ", pAmDone %p", pAmDone);
  }

  return buf;
}


static
const char* am_reqStartStr(amRequest_t* req) {
  static __thread char buf[100];
  (void) snprintf(buf, sizeof(buf),
                  "run AM seqId %s body in %s",
                  am_seqIdStr(req), (amTcip == NULL) ? "task" : "AM handler");
  return buf;
}


static
const char* am_reqDoneStr(amRequest_t* req) {
  static __thread char buf[100];
  amDone_t* pAmDone = op_uses_on_bundle(req->b.op)
                      ? req->xo.hdr.comm.pAmDone
                      : req->b.pAmDone;
  if (pAmDone == NULL) {
    (void) snprintf(buf, sizeof(buf),
                    "fini AM seqId %s, NB",
                    am_seqIdStr(req));
  } else {
    (void) snprintf(buf, sizeof(buf),
                    "fini AM seqId %s, set pAmDone %p",
                    am_seqIdStr(req), pAmDone);
  }
  return buf;
}


static inline
void am_debugPrep(amRequest_t* req) {
  if (DBG_TEST_MASK(DBG_AM | DBG_AM_SEND | DBG_AM_RECV)
      || (req->b.op == am_opAMO && DBG_TEST_MASK(DBG_AMO))) {
    static chpl_bool seqInited = false;
    static atomic_uint_least64_t seqNext;
    if (!seqInited) {
      static pthread_mutex_t seqLock = PTHREAD_MUTEX_INITIALIZER;
      PTHREAD_CHK(pthread_mutex_lock(&seqLock));
      if (!seqInited) {
        seqInited = true;
        atomic_init_uint_least64_t(&seqNext, 1);
      }
      PTHREAD_CHK(pthread_mutex_unlock(&seqLock));
    }

    uint64_t seq = atomic_fetch_add_uint_least64_t(&seqNext, 1);
    if (op_uses_on_bundle(req->b.op)) {
      req->xo.hdr.comm.seq = seq;
    } else {
      req->b.seq = seq;
    }
  }
}


static
void dbg_catfile(const char* fname, const char* match) {
  FILE* f;
  char buf[1000];

  if ((f = fopen(fname, "r")) == NULL) {
    INTERNAL_ERROR_V("%s(): fopen(\"%s\") failed", __func__, fname);
  }

  (void) fprintf(chpl_comm_ofi_dbg_file,
                 "==============================\nfile: %s\n\n",
                 fname);

  while (fgets(buf, sizeof(buf), f) != NULL) {
    if (match == NULL || strstr(buf, match) != NULL) {
      (void) fputs(buf, chpl_comm_ofi_dbg_file);
    }
  }

  (void) fclose(f);
}

#endif
