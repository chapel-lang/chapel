/*
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

#include <rdma/fabric.h>
#include <rdma/fi_atomic.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_rma.h>


////////////////////////////////////////
//
// This is the libfabric API version we need.
//

#define COMM_OFI_FI_VERSION FI_VERSION(1, 8)


////////////////////////////////////////
//
// Global types and data
//

static struct fi_info* ofi_info;        // fabric interface info
static struct fid_fabric* ofi_fabric;   // fabric domain
static struct fid_domain* ofi_domain;   // fabric access domain
static int useScalableTxEp;             // use a scalable tx endpoint?
static struct fid_ep* ofi_txEpScal;     // scalable transmit endpoint
static struct fid_wait* ofi_amhWaitSet; // wait set for AM handler
static bool useWaitset = true;          // should we use the waitset?
//
// We direct RMA traffic and AM traffic to different endpoints so we can
// spread the progress load across all the threads when we're doing
// manual progress.
//
static struct fid_ep* ofi_rxEp;         // AM req receive endpoint
static struct fid_cq* ofi_rxCQ;         // AM req receive endpoint CQ
static struct fid_ep* ofi_rxEpRma;      // RMA/AMO target endpoint
static struct fid_cq* ofi_rxCQRma;      // RMA/AMO target endpoint CQ
static struct fid_cntr* ofi_rxCntrRma;  // RMA/AMO target endpoint counter

static struct fid_av* ofi_av;           // address vector
static fi_addr_t* ofi_rxAddrs;          // table of remote endpoint addresses

#define rxMsgAddr(tcip, n) (ofi_rxAddrs[2 * (n)])
#define rxRmaAddr(tcip, n) (ofi_rxAddrs[2 * (n) + 1])

//
// Transmit support.
//
static int numTxCtxs;
static int numRxCtxs;

struct perTxCtxInfo_t {
  atomic_bool allocated;
  chpl_bool bound;
  struct fid_ep* txCtx;
  struct fid_cq* txCQ;
  struct fid_cntr* txCntr;
  uint64_t numTxnsBegun;    // number of transactions ever initiated
};

static int tciTabLen;
static struct perTxCtxInfo_t* tciTab;
static chpl_bool tciTabFixedAssignments;

//
// Memory registration support.
//
#define MAX_MEM_REGIONS 10
static int numMemRegions = 0;

static struct fid_mr* ofiMrTab[MAX_MEM_REGIONS];

struct memEntry {
  void* addr;
  size_t base;
  size_t size;
  void* desc;
  uint64_t key;
};

typedef struct memEntry (memTab_t)[MAX_MEM_REGIONS];

static memTab_t memTab;
static memTab_t* memTabMap;

//
// Messaging (AM) support.
//
#define AM_MAX_MSG_SIZE (sizeof(chpl_comm_on_bundle_t) + 1024)

static int numAmHandlers = 1;

static void* amLZs;
static struct iovec ofi_iov_reqs;
static struct fi_msg ofi_msg_reqs;


////////////////////////////////////////
//
// Forward decls
//

static void emit_delayedFixedHeapMsgs(void);

static inline struct perTxCtxInfo_t* tciAlloc(void);
static inline struct perTxCtxInfo_t* tciAllocForAmHandler(void);
static inline chpl_bool tciTryRealloc(struct perTxCtxInfo_t*);
static inline void tciFree(struct perTxCtxInfo_t*);
static inline chpl_comm_nb_handle_t ofi_put(const void*, c_nodeid_t,
                                            void*, size_t);
static inline void ofi_put_ll(const void*, c_nodeid_t,
                              void*, size_t, void*);
static inline chpl_comm_nb_handle_t ofi_get(void*, c_nodeid_t,
                                            void*, size_t);
static inline void waitForCQThisTxn(struct perTxCtxInfo_t*, atomic_bool*);
static inline void waitForCQAllTxns(struct perTxCtxInfo_t*,
                                    chpl_comm_taskPrvData_t*);
static inline void checkTxCQ(struct perTxCtxInfo_t*);
static inline ssize_t readCQ(struct fid_cq*, void*, size_t);
static void reportCQError(struct fid_cq*);

static void waitForCntrAllTxns(struct perTxCtxInfo_t*);
static inline uint64_t getTxCntr(struct perTxCtxInfo_t*);
static void* allocBounceBuf(size_t);
static void freeBounceBuf(void*);
static inline void local_yield(void);

static void time_init(void);


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


//
// The ofi_rxm provider may return -FI_EAGAIN for read/write/send while
// doing on-demand connection when emulating FI_RDM endpoints.  The man
// page says: "Applications should be aware of this and retry until the
// the operation succeeds."  Handle this in a generalized way, because
// it seems like something we might encounter with other providers as
// well.
//
#define OFI_RIDE_OUT_EAGAIN(expr, progFnCall)                           \
  do {                                                                  \
    ssize_t _ret;                                                       \
    do {                                                                \
      OFI_CHK_2(expr, _ret, -FI_EAGAIN);                                \
      if (_ret == -FI_EAGAIN) {                                         \
        sched_yield();                                                  \
        progFnCall;                                                     \
      }                                                                 \
    } while (_ret == -FI_EAGAIN);                                       \
  } while (0)

#define PTHREAD_CHK(expr) CHK_EQ_TYPED(expr, 0, int, "d")


////////////////////////////////////////
//
// Network type
//

static pthread_once_t networkOnce = PTHREAD_ONCE_INIT;

typedef enum {
  networkUnknown,
  networkAries,
  networkVerbs,
} network_t;

static network_t network = networkUnknown;


static
void init_network(void) {
  if (strcmp(CHPL_TARGET_PLATFORM, "cray-xc") == 0) {
    network = networkAries;
  }
}


static
network_t getNetworkType(void) {
  PTHREAD_CHK(pthread_once(&networkOnce, init_network));
  return network;
}


////////////////////////////////////////
//
// Provider name
//
static const char* ofi_provNameEnv = "FI_PROVIDER";

static pthread_once_t provNameOnce = PTHREAD_ONCE_INIT;
static const char* provName;

static void setProvName(void);
static const char* getProviderName(void);


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
// Provider-specific behavior control.
//
static chpl_bool provCtl_sizeAvsByNumEps;  // size AVs by numEPs (RxD)
static chpl_bool provCtl_readAmoNeedsOpnd; // READ AMO needs operand (RxD)


//
// Is this name in the provider string?
//
static
chpl_bool isInProviderName(const char* s) {
  if (ofi_info == NULL
      || ofi_info->fabric_attr == NULL
      || ofi_info->fabric_attr->prov_name == NULL) {
    return false;
  }

  char pn[strlen(ofi_info->fabric_attr->prov_name) + 1];
  strcpy(pn, ofi_info->fabric_attr->prov_name);

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


////////////////////////////////////////
//
// transaction tracking
//

//
// If we need to wait for an individual transaction's network completion
// we give the address of a 'txnDone' flag as the context pointer when we
// initiate the transaction, and then just wait for the flag to become
// true.  For transactions we don't need to wait for individually, such
// as unordered AMOs, we instead give the address of the task-private
// outstanding transaction counter, and then we can wait for that to
// become zero as the network responses arrive.  We encode a flag with
// the context pointer to differentiate between these two cases, and
// checkTxCQ() figures out what to update, and how, based on this flag.
//
// For AM handler transactions things are even simpler.  The transmit
// context it uses has only a transaction counter, not a CQ.  But in
// the AM handler we only need to wait for all outstanding transactions
// to complete, not specific ones, so we can just maintain a matching
// counter in the AM handler's transmit context descriptor.  That also
// saves us having to look up the AM handler's task private data block
// address.
//

static __thread chpl_bool isAmHandler = false;

typedef enum {
  txnTrkNone,  // no tracking, ptr is ignored
  txnTrkDone,  // *ptr is atomic bool 'done' flag
  txnTrkCntr,  // *ptr is plain int (caller responsible for de-conflict)
} txnTrkType_t;

typedef struct {
  txnTrkType_t typ;
  void* ptr;
} txnTrkCtx_t;

static inline
void* txnTrkEncode(txnTrkType_t typ, void* p) {
  return (void*) (  ((uint64_t) typ << 63)
                  | ((uint64_t) p & 0x7fffffffffffffffUL));
}

static inline
txnTrkCtx_t txnTrkDecode(void* ctx) {
  const uint64_t u = (uint64_t) ctx;
  return (txnTrkCtx_t) { .typ = (u >> 63) & 1,
                         .ptr = (void*) (u & 0x7fffffffffffffffUL) };
}


static inline
chpl_comm_taskPrvData_t* get_comm_taskPrvdata(void) {
  chpl_task_prvData_t* task_prvData = chpl_task_getPrvData();
  if (task_prvData != NULL) return &task_prvData->comm_data;

  static chpl_comm_taskPrvData_t amHandlerCommData;
  assert(isAmHandler);
  return &amHandlerCommData;
}


////////////////////////////////////////
//
// Interface: initialization
//

pthread_t pthread_that_inited;

static void init_ofi(void);
static void init_ofiFabricDomain(void);
static void init_ofiDoProviderChecks(void);
static void init_ofiEp(void);
static void init_ofiEpNumCtxs(void);
static void init_ofiEpTxCtx(int, chpl_bool,
                            struct fi_cq_attr*, struct fi_cntr_attr*);
static void init_ofiExchangeAvInfo(void);
static void init_ofiForMem(void);
static void init_ofiForRma(void);
static void init_ofiForAms(void);

static void init_bar(void);

static void init_broadcast_private(void);


void chpl_comm_init(int *argc_p, char ***argv_p) {
  chpl_comm_ofi_abort_on_error =
    (chpl_env_rt_get("COMM_OFI_ABORT_ON_ERROR", NULL) != NULL);
  time_init();
  chpl_comm_ofi_oob_init();
  DBG_INIT();

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


void chpl_comm_post_mem_init(void) {
  chpl_comm_init_prv_bcast_tab();
  init_broadcast_private();
}


//
// No support for gdb for now
//
int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
  return 0;
}


void chpl_comm_post_task_init(void) {
  if (chpl_numNodes == 1)
    return;
  init_ofi();
  init_bar();
}


static
void init_ofi(void) {
  init_ofiFabricDomain();
  init_ofiDoProviderChecks();
  init_ofiEp();
  init_ofiExchangeAvInfo();
  init_ofiForMem();
  init_ofiForRma();
  init_ofiForAms();

  DBG_PRINTF(DBG_CFG,
             "AM config: recv buf size %zd MiB, %s, responses use %s",
             ofi_iov_reqs.iov_len / (1L << 20),
             (ofi_amhWaitSet == NULL) ? "polling" : "wait set",
             (tciTab[tciTabLen - 1].txCQ != NULL) ? "CQ" : "counter");
  if (useScalableTxEp) {
    DBG_PRINTF(DBG_CFG,
               "per node config: 1 scalable tx ep + %d tx ctx%s (%d fixed), "
               "%d rx ctx%s",
               numTxCtxs, (numTxCtxs == 1) ? "" : "s",
               tciTabFixedAssignments ? chpl_task_getFixedNumThreads() : 0,
               numRxCtxs, (numRxCtxs == 1) ? "" : "s");
  } else {
    DBG_PRINTF(DBG_CFG,
               "per node config: %d regular tx ep+ctx%s (%d fixed), "
               "%d rx ctx%s",
               numTxCtxs, (numTxCtxs == 1) ? "" : "s",
               tciTabFixedAssignments ? chpl_task_getFixedNumThreads() : 0,
               numRxCtxs, (numRxCtxs == 1) ? "" : "s");
  }
}


static
void init_ofiFabricDomain(void) {
  //
  // Build hints describing what we want from the provider.
  //
  struct fi_info* hints;
  CHK_TRUE((hints = fi_allocinfo()) != NULL);

  hints->caps = FI_MSG | FI_SEND | FI_RECV | FI_MULTI_RECV
                | FI_RMA | FI_READ | FI_WRITE
                | FI_REMOTE_READ | FI_REMOTE_WRITE;
  if (getNetworkType() == networkAries) {
      hints->caps |= FI_ATOMICS; // oddly, we don't get this without asking
  }

  hints->addr_format = FI_FORMAT_UNSPEC;

  hints->tx_attr->op_flags = FI_DELIVERY_COMPLETE | FI_COMPLETION;
  hints->rx_attr->op_flags = FI_MULTI_RECV        | FI_COMPLETION;

  hints->ep_attr->type = FI_EP_RDM;

  hints->domain_attr->threading = FI_THREAD_UNSPEC;

  enum fi_progress prg = FI_PROGRESS_UNSPEC;
  if (DBG_TEST_MASK(DBG_CFG)) {
    const char* ev = chpl_env_rt_get("COMM_OFI_PROGRESS", "");
    if (strcmp(ev, "") != 0) {
      if (strcasecmp(ev, "auto") == 0)
        prg = FI_PROGRESS_AUTO;
      else if (strcasecmp(ev, "manual") == 0)
        prg = FI_PROGRESS_MANUAL;
      else
        CHK_TRUE((strcasecmp(ev, "unspec") == 0));
    }
  }
  hints->domain_attr->control_progress = FI_PROGRESS_UNSPEC; // don't need
  hints->domain_attr->data_progress = prg;

  hints->domain_attr->av_type = FI_AV_TABLE;

  //
  // We can support all of the MR modes shown here, but we should only
  // throw ALLOCATED if indeed the pages are known to exist, and that's
  // only true with a fixed heap.
  //
  // PROV_KEY is marked TODO only because if the provider doesn't assert
  // that mode we may be able to avoid broadcasting keys around the job.
  //
  int mr_mode;
  if ((mr_mode = chpl_env_rt_get_int("COMM_MR_MODE", -1)) == -1) {
    mr_mode = FI_MR_LOCAL
              | FI_MR_VIRT_ADDR
              | FI_MR_PROV_KEY /*TODO*/
              | FI_MR_ENDPOINT;
    if (chpl_numNodes > 1 && chpl_comm_getenvMaxHeapSize() > 0) {
      mr_mode |= FI_MR_ALLOCATED;
    }
  }
  hints->domain_attr->mr_mode = mr_mode;

  hints->domain_attr->resource_mgmt = FI_RM_ENABLED;

  //
  // Try to find a provider that can do what we want.  If more than one
  // is found, presume that ones earlier in the list perform better (as
  // documented in 'man fi_getinfo').  We just do error reporting on
  // node 0; the other nodes should all have the same result and there's
  // no point in repeating everything numNodes times.
  //
  if (DBG_TEST_MASK(DBG_CFGFAB) && chpl_nodeID == 0) {
    DBG_PRINTF(DBG_CFGFAB, "==================== hints:");
    DBG_PRINTF(DBG_CFGFAB, "%s", fi_tostr(hints, FI_TYPE_INFO));
  }

  int ret;
  OFI_CHK_2(fi_getinfo(COMM_OFI_FI_VERSION, NULL, NULL, 0, hints, &ofi_info),
            ret, -FI_ENODATA);

  if (chpl_nodeID == 0) {
    if (ret == -FI_ENODATA) {
      const char* provider = getProviderName();

      if (DBG_TEST_MASK(DBG_CFGFABSALL)) {
        DBG_PRINTF(DBG_CFGFABSALL,
                   "==================== fi_getinfo() fabrics:");
        DBG_PRINTF(DBG_CFGFABSALL,
                   "None matched hints; available with prov_name \"%s\" are:",
                   (provider == NULL) ? "<any>" : provider);
        struct fi_info* info = NULL;
        OFI_CHK(fi_getinfo(COMM_OFI_FI_VERSION, NULL, NULL, 0, NULL, &info));
        for ( ; info != NULL; info = info->next) {
          const char* pn = info->fabric_attr->prov_name;
          if (provider == NULL
              || strncmp(pn, provider, strlen(provider)) == 0) {
            DBG_PRINTF(DBG_CFGFABSALL, "%s", fi_tostr(info, FI_TYPE_INFO));
            DBG_PRINTF(DBG_CFGFABSALL, "----------");
          }
        }
      }

      INTERNAL_ERROR_V("No provider matched for prov_name \"%s\"",
                       (provider == NULL) ? "<any>" : provider);
    }

    if (DBG_TEST_MASK(DBG_CFGFABSALL)) {
      DBG_PRINTF(DBG_CFGFABSALL, "====================\n"
                 "fi_getinfo() matched fabric(s):");
      struct fi_info* info;
      for (info = ofi_info; info != NULL; info = info->next) {
        DBG_PRINTF(DBG_CFGFABSALL, "%s", fi_tostr(ofi_info, FI_TYPE_INFO));
        DBG_PRINTF(DBG_CFGFABSALL, "----------");
      }
    } else {
      DBG_PRINTF(DBG_CFGFAB, "====================\n"
                 "fi_getinfo() matched fabric:");
      DBG_PRINTF(DBG_CFGFAB, "%s", fi_tostr(ofi_info, FI_TYPE_INFO));
      DBG_PRINTF(DBG_CFGFAB, "----------");
    }
  } else {
    //
    // Node 0 will take care of producing the error message.
    //
    if (ret == -FI_ENODATA) {
      chpl_comm_ofi_oob_fini();
      chpl_exit_any(0);
    }
  }

  fi_freeinfo(hints);

  if (verbosity >= 2) {
    if (chpl_nodeID == 0) {
      printf("COMM=ofi: using \"%s\" provider\n",
             ofi_info->fabric_attr->prov_name);
    }
  }

  //
  // Create the fabric domain and associated fabric access domain.
  //
  OFI_CHK(fi_fabric(ofi_info->fabric_attr, &ofi_fabric, NULL));
  OFI_CHK(fi_domain(ofi_fabric, ofi_info, &ofi_domain, NULL));
}


static
void init_ofiDoProviderChecks(void) {
  //
  // Set/compute various provider-specific things.
  //
  if (isInProviderName("gni")) {
    //
    // gni
    //
    // If there were questionable settings associated with the fixed
    // heap on a Cray XC system, say something about that now.
    //
    emit_delayedFixedHeapMsgs();

    //
    // For now avoid using a waitset with the gni provider, because
    // although it seems to work properly we get -FI_EBUSY when we
    // try to close it.
    //
    useWaitset = false;
  } else if (isInProviderName("ofi_rxd")) {
    //
    // ofi_rxd (utility provider with tcp, verbs, possibly others)
    //
    // - Based on tracebacks after internal error aborts, RxD seems to
    //   want to record an address per accessing endpoint for at least
    //   some AVs (perhaps just those for which it handles progress?).
    //   It uses the AV attribute 'count' member to size the data
    //   structure in which it stores those.  So, that member will need
    //   to account for all transmitting endpoints.
    // - Based on analyzing a segfault, RxD has to have a non-NULL
    //   buf arg for fi_fetch_atomic(FI_ATOMIC_READ) even though the
    //   fi_atomic man page says buf is ignored for that operation
    //   and may be NULL.
    //
    provCtl_sizeAvsByNumEps = true;
    provCtl_readAmoNeedsOpnd = true;
  }
}


static
void init_ofiEp(void) {
  //
  // The AM handler is responsible not only for AM handling and progress
  // on any RMA it initiates but also progress on inbound RMA, if that
  // is needed.  It uses a wait set to organize this.
  //
  {
    struct fi_wait_attr waitSetAttr = (struct fi_wait_attr)
                                      { .wait_obj = FI_WAIT_UNSPEC, };
    int ret;
    if (useWaitset) {
      OFI_CHK_2(fi_wait_open(ofi_fabric, &waitSetAttr, &ofi_amhWaitSet),
                ret, -FI_ENOSYS);
    } else {
      ret = -FI_ENOSYS;
    }
    if (ret != FI_SUCCESS) {
      ofi_amhWaitSet = NULL;
    }
  }

  //
  // Compute numbers of transmit and receive contexts, and then create
  // the transmit context table.
  //
  useScalableTxEp = (ofi_info->domain_attr->max_ep_tx_ctx > 1
                     && chpl_env_rt_get_bool("COMM_OFI_USE_SCALABLE_EP",
                                             true));
  init_ofiEpNumCtxs();

  tciTabLen = numTxCtxs;
  CHPL_CALLOC(tciTab, tciTabLen);

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

  OFI_CHK(fi_av_open(ofi_domain, &avAttr, &ofi_av, NULL));

  if (useScalableTxEp) {
    //
    // Use a scalable transmit endpoint and multiple tx contexts.  Make
    // just one address vector, in the first tciTab[] entry.  The others
    // will be synonyms for that one, to make the references easier.
    //
    OFI_CHK(fi_scalable_ep(ofi_domain, ofi_info, &ofi_txEpScal, NULL));
    OFI_CHK(fi_scalable_ep_bind(ofi_txEpScal, &ofi_av->fid, 0));
  } else {
    //
    // Use regular transmit endpoints; see below.
    //
  }

  //
  // Worker TX contexts need completion queues, so they can tell what
  // kinds of things are completing.
  //
  const int numWorkerTxCtxs = tciTabLen - numAmHandlers;
  struct fi_cq_attr cqAttr;
  struct fi_cntr_attr cntrAttr;

  {
    cqAttr = (struct fi_cq_attr)
             { .format = FI_CQ_FORMAT_MSG,
               .size = 100, // TODO
               .wait_obj = FI_WAIT_NONE, };
    for (int i = 0; i < numWorkerTxCtxs; i++) {
      init_ofiEpTxCtx(i, false /*isAMHandler*/, &cqAttr, NULL);
    }
  }

  //
  // TX contexts for the AM handler(s) can just use counters, if the
  // provider supports them.  Otherwise, they have to use CQs also.
  //
  const enum fi_wait_obj waitObj = (ofi_amhWaitSet == NULL)
                                   ? FI_WAIT_NONE
                                   : FI_WAIT_SET;
  if (ofi_info->domain_attr->cntr_cnt > 0) {
    cntrAttr = (struct fi_cntr_attr)
               { .events = FI_CNTR_EVENTS_COMP,
                 .wait_obj = waitObj,
                 .wait_set = ofi_amhWaitSet, };
    for (int i = numWorkerTxCtxs; i < tciTabLen; i++) {
      init_ofiEpTxCtx(i, true /*isAMHandler*/, NULL, &cntrAttr);
    }
  } else {
    cqAttr = (struct fi_cq_attr)
             { .format = FI_CQ_FORMAT_MSG,
               .size = 100, // TODO
               .wait_obj = waitObj,
               .wait_cond = FI_CQ_COND_NONE,
               .wait_set = ofi_amhWaitSet, };
    for (int i = numWorkerTxCtxs; i < tciTabLen; i++) {
      init_ofiEpTxCtx(i, true /*isAMHandler*/, &cqAttr, NULL);
    }
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
               .wait_obj = waitObj,
               .wait_set = ofi_amhWaitSet, };

  OFI_CHK(fi_endpoint(ofi_domain, ofi_info, &ofi_rxEp, NULL));
  OFI_CHK(fi_ep_bind(ofi_rxEp, &ofi_av->fid, 0));
  OFI_CHK(fi_cq_open(ofi_domain, &cqAttr, &ofi_rxCQ, NULL));
  OFI_CHK(fi_ep_bind(ofi_rxEp, &ofi_rxCQ->fid,
                     FI_TRANSMIT | FI_RECV));
  OFI_CHK(fi_enable(ofi_rxEp));

  OFI_CHK(fi_endpoint(ofi_domain, ofi_info, &ofi_rxEpRma, NULL));
  OFI_CHK(fi_ep_bind(ofi_rxEpRma, &ofi_av->fid, 0));
  if (ofi_info->domain_attr->cntr_cnt == 0) {
    OFI_CHK(fi_cq_open(ofi_domain, &cqAttr, &ofi_rxCQRma, NULL));
    OFI_CHK(fi_ep_bind(ofi_rxEpRma, &ofi_rxCQRma->fid,
                       FI_TRANSMIT | FI_RECV));
  } else {
    OFI_CHK(fi_cntr_open(ofi_domain, &cntrAttr, &ofi_rxCntrRma, NULL));
    OFI_CHK(fi_ep_bind(ofi_rxEpRma, &ofi_rxCntrRma->fid,
                       FI_TRANSMIT | FI_RECV));
  }
  OFI_CHK(fi_enable(ofi_rxEpRma));
}


static
void init_ofiEpNumCtxs(void) {
  CHK_TRUE(numAmHandlers == 1); // force reviewing this if #AM handlers changes

  //
  // Note for future maintainers: if interoperability between Chapel
  // and other languages someday results in non-tasking layer threads
  // calling Chapel code which then tries to communicate across nodes,
  // then some of this may have to be adjusted, especially e.g. the
  // tciTabFixedAssignments part.
  //

  //
  // Start with the maximum number of transmit contexts.  We'll reduce
  // the number incrementally as we discover we don't need that many.
  // Initially, just make sure there are enough for each AM handler to
  // have its own, plus at least one more.
  //
  const struct fi_domain_attr* dom_attr = ofi_info->domain_attr;
  int maxWorkerTxCtxs;
  if (useScalableTxEp)
    maxWorkerTxCtxs = dom_attr->max_ep_tx_ctx - numAmHandlers;
  else
    maxWorkerTxCtxs = dom_attr->ep_cnt - numAmHandlers;

  CHK_TRUE(maxWorkerTxCtxs > 0);

  //
  // If the user manually limited the communication concurrency, take
  // that into account.
  //
  const int commConcurrency = chpl_env_rt_get_int("COMM_CONCURRENCY", 0);
  if (commConcurrency > 0) {
    if (maxWorkerTxCtxs > commConcurrency) {
      maxWorkerTxCtxs = commConcurrency;
    }
  } else if (commConcurrency < 0) {
    chpl_warning("CHPL_RT_COMM_CONCURRENCY < 0, ignored", 0, 0);
  }

  const int fixedNumThreads = chpl_task_getFixedNumThreads();
  if (fixedNumThreads > 0) {
    //
    // The tasking layer uses a fixed number of threads.  If we can
    // have at least that many worker tx contexts, plus 1 for threads
    // that aren't fixed workers (like the process itself for example),
    // then each tasking layer fixed thread can have a private context
    // for the duration of the run.
    //
    CHK_TRUE(fixedNumThreads == chpl_task_getMaxPar()); // sanity
    if (maxWorkerTxCtxs > fixedNumThreads + 1)
      maxWorkerTxCtxs = fixedNumThreads + 1;
    tciTabFixedAssignments = (maxWorkerTxCtxs == fixedNumThreads + 1);
  } else {
    //
    // The tasking layer doesn't have a fixed number of threads, but
    // it still must have a maximum useful level of parallelism.  We
    // shouldn't need more worker tx contexts than whatever that is.
    //
    const int taskMaxPar = chpl_task_getMaxPar();
    if (maxWorkerTxCtxs > taskMaxPar)
      maxWorkerTxCtxs = taskMaxPar;

    tciTabFixedAssignments = false;
  }

  //
  // Now we know how many transmit contexts we'll have.
  //
  numTxCtxs = maxWorkerTxCtxs + numAmHandlers;
  if (useScalableTxEp) {
    ofi_info->ep_attr->tx_ctx_cnt = numTxCtxs;
  }

  //
  // Receive contexts are much easier -- we just need one
  // for each AM handler.
  //
  CHK_TRUE(dom_attr->max_ep_rx_ctx >= numAmHandlers);
  numRxCtxs = numAmHandlers;
}


static
void init_ofiEpTxCtx(int i, chpl_bool isAMHandler,
                     struct fi_cq_attr* cqAttr,
                     struct fi_cntr_attr* cntrAttr) {
  struct perTxCtxInfo_t* tcip = &tciTab[i];
  atomic_init_bool(&tcip->allocated, false);
  tcip->bound = false;
  if (useScalableTxEp) {
    OFI_CHK(fi_tx_context(ofi_txEpScal, i, NULL, &tcip->txCtx, NULL));
  } else {
    OFI_CHK(fi_endpoint(ofi_domain, ofi_info, &tcip->txCtx, NULL));
    OFI_CHK(fi_ep_bind(tcip->txCtx, &ofi_av->fid, 0));
  }
  if (cqAttr != NULL) {
    OFI_CHK(fi_cq_open(ofi_domain, cqAttr, &tcip->txCQ, NULL));
    OFI_CHK(fi_ep_bind(tcip->txCtx, &tcip->txCQ->fid,
                       FI_TRANSMIT | FI_RECV));
  } else {
    OFI_CHK(fi_cntr_open(ofi_domain, cntrAttr, &tcip->txCntr, NULL));
    OFI_CHK(fi_ep_bind(tcip->txCtx, &tcip->txCntr->fid,
                       (isAMHandler ? FI_WRITE
                                    : FI_SEND | FI_READ | FI_WRITE)));
  }
  OFI_CHK(fi_enable(tcip->txCtx));
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
  if (DBG_TEST_MASK(DBG_CFGAV)) {
    //
    // Sanity-check our same-address-length assumption.
    //
    size_t len = 0;
    size_t lenRma = 0;

    OFI_CHK_1(fi_getname(&ofi_rxEp->fid, NULL, &len), -FI_ETOOSMALL);
    OFI_CHK_1(fi_getname(&ofi_rxEpRma->fid, NULL, &lenRma), -FI_ETOOSMALL);
    CHK_TRUE(len == lenRma);

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
  CHPL_CALLOC_SZ(my_addr, 2 * my_addr_len, 1);
  OFI_CHK(fi_getname(&ofi_rxEp->fid, my_addr, &my_addr_len));
  OFI_CHK(fi_getname(&ofi_rxEpRma->fid, my_addr + my_addr_len, &my_addr_len));
  CHPL_CALLOC_SZ(addrs, chpl_numNodes, 2 * my_addr_len);
  if (DBG_TEST_MASK(DBG_CFGAV)) {
    char nameBuf[128];
    size_t nameLen;
    nameLen = sizeof(nameBuf);
    char nameBuf2[128];
    size_t nameLen2;
    nameLen2 = sizeof(nameBuf2);
    (void) fi_av_straddr(ofi_av, my_addr, nameBuf, &nameLen);
    (void) fi_av_straddr(ofi_av, my_addr + my_addr_len, nameBuf2, &nameLen2);
    DBG_PRINTF(DBG_CFGAV, "my_addrs: %.*s%s, %.*s%s",
               (int) nameLen, nameBuf,
               (nameLen <= sizeof(nameBuf)) ? "" : "[...]",
               (int) nameLen2, nameBuf2,
               (nameLen2 <= sizeof(nameBuf2)) ? "" : "[...]");
  }
  chpl_comm_ofi_oob_allgather(my_addr, addrs, 2 * my_addr_len);

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
  CHPL_CALLOC(ofi_rxAddrs, 2 * chpl_numNodes);
  CHK_TRUE(fi_av_insert(ofi_av, addrs, 2 * chpl_numNodes, ofi_rxAddrs, 0, NULL)
           == 2 * chpl_numNodes);

  CHPL_FREE(my_addr);
  CHPL_FREE(addrs);
}


static
void init_ofiForMem(void) {
  //
  // With scalable memory registration we just register the whole
  // address space here; with non-scalable we register each region
  // individually.
  //
  void* fixedHeapStart;
  size_t fixedHeapSize;
  chpl_comm_impl_regMemHeapInfo(&fixedHeapStart, &fixedHeapSize);

  //
  // At present the user can specify a fixed heap in which case we will
  // register that and only that, or they'd better be using a provider
  // which supports scalable registration of the entire address space.
  //
  if (fixedHeapStart != NULL && fixedHeapSize != 0) {
    numMemRegions = 1;
    memTab[0].addr = fixedHeapStart;
    memTab[0].base = ((ofi_info->domain_attr->mr_mode & FI_MR_VIRT_ADDR) == 0)
                     ? (size_t) fixedHeapStart
                     : (size_t) 0;
    memTab[0].size = fixedHeapSize;
  } else {
    CHK_TRUE((ofi_info->domain_attr->mr_mode & FI_MR_BASIC) == 0);
    numMemRegions = 1;
    memTab[0].addr = (void*) 0;
    memTab[0].base = 0;
    memTab[0].size = SIZE_MAX;
  }

  uint64_t bufAcc = FI_RECV | FI_REMOTE_READ | FI_REMOTE_WRITE;
  if ((ofi_info->domain_attr->mr_mode & FI_MR_LOCAL) != 0) {
    bufAcc |= FI_SEND | FI_READ | FI_WRITE;
  }

  for (int i = 0; i < numMemRegions; i++) {
    DBG_PRINTF(DBG_MR, "[%d] fi_mr_reg(%p, %#zx, %#" PRIx64 ")",
               i, memTab[i].addr, memTab[i].size, bufAcc);
    OFI_CHK(fi_mr_reg(ofi_domain,
                      memTab[i].addr, memTab[i].size,
                      bufAcc, 0, 0, 0, &ofiMrTab[i], NULL));
    memTab[i].desc = fi_mr_desc(ofiMrTab[i]);
    memTab[i].key  = fi_mr_key(ofiMrTab[i]);
    DBG_PRINTF(DBG_MR, "[%d]     key %#" PRIx64, i, memTab[i].key);
    if ((ofi_info->domain_attr->mr_mode & FI_MR_ENDPOINT) != 0) {
      OFI_CHK(fi_mr_bind(ofiMrTab[i], &ofi_rxEpRma->fid, 0));
      OFI_CHK(fi_mr_enable(ofiMrTab[i]));
    }
  }

  //
  // Share the memory regions around the job.
  //
  CHPL_CALLOC(memTabMap, chpl_numNodes);
  chpl_comm_ofi_oob_allgather(&memTab, memTabMap, sizeof(memTabMap[0]));
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
  // it needs renewing.
  //
  const size_t amLZSize = (size_t) 40 << 20;

  //
  // Set the minimum multi-receive buffer space.  Some providers don't
  // have fi_setopt() for some ep types, so allow this to fail in that
  // case.  Note, however, that if it does fail and we get overruns,
  // we'll die.
  //
  {
    const size_t sz = AM_MAX_MSG_SIZE;
    int ret;
    OFI_CHK_2(fi_setopt(&ofi_rxEp->fid, FI_OPT_ENDPOINT,
                        FI_OPT_MIN_MULTI_RECV, &sz, sizeof(sz)),
              ret, -FI_ENOSYS);
  }

  //
  // Pre-post multi-receive buffer for inbound AM requests.
  //
  CHPL_CALLOC_SZ(amLZs, 1, amLZSize);

  ofi_iov_reqs.iov_base = amLZs;
  ofi_iov_reqs.iov_len = amLZSize;
  ofi_msg_reqs.msg_iov = &ofi_iov_reqs;
  ofi_msg_reqs.desc = NULL;
  ofi_msg_reqs.iov_count = 1;
  ofi_msg_reqs.addr = FI_ADDR_UNSPEC;
  ofi_msg_reqs.context = NULL;
  ofi_msg_reqs.data = 0x0;
  OFI_CHK(fi_recvmsg(ofi_rxEp, &ofi_msg_reqs, FI_MULTI_RECV));
  DBG_PRINTF(DBG_AM | DBG_AMRECV,
             "pre-post fi_recvmsg(AMLZs, len %zd)",
             ofi_msg_reqs.msg_iov->iov_len);

  init_amHandling();
}


#if 0
static
void init_ofiPerThread(void) {
}
#endif


void chpl_comm_rollcall(void) {
  // Initialize diags
  chpl_comm_diags_init();

  chpl_msg(2, "executing on node %d of %d node(s): %s\n", chpl_nodeID,
           chpl_numNodes, chpl_nodeName());
}


//
// Chapel global and private variable support
//

wide_ptr_t* chpl_comm_broadcast_global_vars_helper(void) {
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
  if (isInProviderName("verbs")) {
    fflush(NULL);
    _exit(status);
  }
}


static
void fini_ofi(void) {
  if (chpl_numNodes <= 1)
    return;

  for (int i = 0; i < numMemRegions; i++) {
    OFI_CHK(fi_close(&ofiMrTab[i]->fid));
  }

  CHPL_FREE(memTabMap);

  CHPL_FREE(memTabMap);

  CHPL_FREE(amLZs);

  CHPL_FREE(ofi_rxAddrs);

  OFI_CHK(fi_close(&ofi_rxEp->fid));
  OFI_CHK(fi_close(&ofi_rxCQ->fid));
  OFI_CHK(fi_close(&ofi_rxEpRma->fid));
  if (ofi_rxCQRma != NULL) {
    OFI_CHK(fi_close(&ofi_rxCQRma->fid));
  } else {
    OFI_CHK(fi_close(&ofi_rxCntrRma->fid));
  }

  for (int i = 0; i < tciTabLen; i++) {
    OFI_CHK(fi_close(&tciTab[i].txCtx->fid));
    if (tciTab[i].txCQ != NULL) {
      OFI_CHK(fi_close(&tciTab[i].txCQ->fid));
    } else {
      OFI_CHK(fi_close(&tciTab[i].txCntr->fid));
    }
  }

  if (useScalableTxEp) {
    OFI_CHK(fi_close(&ofi_txEpScal->fid));
  }

  OFI_CHK(fi_close(&ofi_av->fid));

  if (ofi_amhWaitSet != NULL) {
    OFI_CHK(fi_close(&ofi_amhWaitSet->fid));
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

static size_t nic_mem_map_limit;

static void init_fixedHeap(void);

static size_t get_hugepageSize(void);
static void init_hugepageSize(void);


void chpl_comm_impl_regMemHeapInfo(void** start_p, size_t* size_p) {
  PTHREAD_CHK(pthread_once(&fixedHeapOnce, init_fixedHeap));
  *start_p = fixedHeapStart;
  *size_p  = fixedHeapSize;
}


static
void init_fixedHeap(void) {
  //
  // We only need a fixed heap if we're multinode, and either we're
  // on a Cray XC system or the user has explicitly specified a heap
  // size.
  //
  size_t size = chpl_comm_getenvMaxHeapSize();
  if ( ! (chpl_numNodes > 1
          && (getNetworkType() == networkAries || size != 0))) {
    return;
  }

  //
  // On XC systems you really ought to use hugepages.  If called for,
  // a message will be emitted later.
  //
  size_t page_size;
  chpl_bool have_hugepages;
  void* start;

  if ((page_size = get_hugepageSize()) == 0) {
    page_size = chpl_getSysPageSize();
    have_hugepages = false;
  } else {
    have_hugepages = true;
  }

  if (size == 0) {
    size = (size_t) 16 << 30;
  }

  size = ALIGN_UP(size, page_size);

  //
  // The heap is supposed to be of fixed size and on hugepages.  Set
  // it up.
  //

  //
  // Considering the data size we'll register, compute the maximum
  // heap size that will allow all registrations to fit in the NIC
  // TLB.
  //
  const size_t nic_TLB_cache_pages = 512; // not publicly defined
  nic_mem_map_limit = nic_TLB_cache_pages * page_size;

  //
  // As a hedge against silliness, first reduce any request so that it's
  // no larger than the physical memory.  As a beneficial side effect
  // when the user request is ridiculously large, this also causes the
  // reduce-by-5% loop below to run faster and produce a final size
  // closer to the maximum available.
  //
  const size_t size_phys = ALIGN_DN(chpl_sys_physicalMemoryBytes(), page_size);
  if (size > size_phys)
    size = size_phys;

  //
  // Work our way down from the starting size in (roughly) 5% steps
  // until we can actually get that much from the system.
  //
  size_t decrement;

  if ((decrement = ALIGN_DN((size_t) (0.05 * size), page_size)) < page_size) {
    decrement = page_size;
  }

  size += decrement;
  do {
    size -= decrement;
    DBG_PRINTF(DBG_HUGEPAGES, "try allocating fixed heap, size %#zx", size);
    if (have_hugepages) {
      start = chpl_comm_ofi_hp_get_huge_pages(size);
    } else {
      CHK_SYS_MEMALIGN(start, page_size, size);
    }
  } while (start == NULL && size > decrement);

  if (start == NULL)
    chpl_error("cannot initialize heap: cannot get memory", 0, 0);

  DBG_PRINTF(DBG_MR, "fixed heap on %spages, start=%p size=%#zx\n",
             have_hugepages ? "huge" : "regular ", start, size);

  fixedHeapSize  = size;
  fixedHeapStart = start;
}


static
void emit_delayedFixedHeapMsgs(void) {
  //
  // We only need a fixed heap if we're multinode on a Cray XC system
  // and using the gni provider.
  //
  if (chpl_numNodes <= 1
      || getNetworkType() != networkAries) {
    return;
  }

  //
  // On XC systems you really ought to use hugepages.
  //
  void* start;
  size_t size;
  chpl_comm_impl_regMemHeapInfo(&start, &size);
  if (hugepageSize == 0) {
    chpl_warning_explicit("not using hugepages may reduce performance",
                          __LINE__, __FILE__);
  }

  //
  // Warn if the size is larger than what will fit in the TLB cache.
  // While that may reduce performance it won't affect function, though,
  // so don't do anything dramatic like reducing the size to fit.
  //
  if (size > nic_mem_map_limit) {
    if (chpl_nodeID == 0) {
      size_t page_size = chpl_comm_impl_regMemHeapPageSize();
      char buf1[20], buf2[20], buf3[20], msg[200];
      chpl_snprintf_KMG_z(buf1, sizeof(buf1), nic_mem_map_limit);
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
}


size_t chpl_comm_impl_regMemHeapPageSize(void) {
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

  for (int i = 0; i < numMemRegions; i++) {
    char* tabAddr = (char*) (*tab)[i].addr;
    char* tabAddrEnd = tabAddr + (*tab)[i].size;
    if (myAddr >= tabAddr && myAddr + size <= tabAddrEnd)
      return &(*tab)[i];
  }

  return NULL;
}


static inline
int mrGetDesc(void** pDesc, void* addr, size_t size) {
  struct memEntry* mr;
  if ((mr = getMemEntry(&memTab, addr, size)) == NULL) {
    DBG_PRINTF(DBG_MRDESC, "mrGetDesc(%p, %zd): no entry", addr, size);
    return -1;
  }
  DBG_PRINTF(DBG_MRDESC, "mrGetDesc(%p, %zd): desc %p", addr, size, mr->desc);
  if (pDesc != NULL)
    *pDesc = mr->desc;
  return 0;
}


static inline
int mrGetKey(uint64_t* pKey, uint64_t* pOff,
             int iNode, void* addr, size_t size) {
  struct memEntry* mr;
  if ((mr = getMemEntry(&memTabMap[iNode], addr, size)) == NULL) {
    DBG_PRINTF(DBG_MRKEY, "mrGetKey(%d:%p, %zd): no entry",
               iNode, addr, size);
    return -1;
  }

  const uint64_t key = mr->key;
  const uint64_t off = (uint64_t) addr - mr->base;
  DBG_PRINTF(DBG_MRKEY, "mrGetKey(%d:%p, %zd): key %" PRIx64 ", off %" PRIx64,
             iNode, addr, size, key, off);
  if (pKey != NULL) {
    *pKey = key;
    *pOff = off;
  }
  return 0;
}


static inline
int mrGetLocalKey(void* addr, size_t size) {
  return mrGetKey(NULL, NULL, chpl_nodeID, addr, size);
}


////////////////////////////////////////
//
// Interface: Active Messages
//


typedef enum {
  am_opNil = 0,                         // no-op
  am_opExecOn,                          // call a function table function
  am_opExecOnLrg,                       // call fn tab fn, arg large/separate
  am_opGet,                             // do an RMA GET
  am_opPut,                             // do an RMA PUT
  am_opAMO,                             // do an AMO
  am_opShutdown,                        // signal main process for shutdown
} amOp_t;

#ifdef CHPL_COMM_DEBUG
static const char* am_opName(amOp_t);
static const char* amo_opName(enum fi_op);
static const char* amo_typeName(enum fi_datatype);
#endif

static void amRequestExecOn(c_nodeid_t, c_sublocid_t, chpl_fn_int_t,
                            chpl_comm_on_bundle_t*, size_t,
                            chpl_bool, chpl_bool);
static void amRequestRMA(c_nodeid_t, amOp_t, void*, void*, size_t);
static void amRequestAMO(c_nodeid_t, void*, const void*, const void*, void*,
                         int, enum fi_datatype, size_t);
static void amRequestCommon(c_nodeid_t, chpl_comm_on_bundle_t*, size_t,
                            chpl_comm_amDone_t**, chpl_bool, chpl_bool);


static inline
void ensure_progress(void) {
  if (isAmHandler
      && ofi_info->domain_attr->data_progress == FI_PROGRESS_MANUAL) {
    if (ofi_rxCQRma != NULL) {
      struct fi_cq_data_entry cqe;
      (void) readCQ(ofi_rxCQRma, &cqe, 1);
    } else {
      (void) fi_cntr_read(ofi_rxCntrRma);
    }
  }
}


void chpl_comm_task_end(void) { }


void chpl_comm_execute_on(c_nodeid_t node, c_sublocid_t subloc,
                          chpl_fn_int_t fid,
                          chpl_comm_on_bundle_t *arg, size_t argSize,
                          int ln, int32_t fn) {
  DBG_PRINTF(DBG_INTERFACE,
             "chpl_comm_execute_on(%d, %d, %d, %p, %zd)",
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


#ifdef BLAH
static void fork_nb_wrapper(chpl_comm_on_bundle_t *f) {
  chpl_ftable_call(f->task_bundle.requested_fid, f);
}
#endif


void chpl_comm_execute_on_nb(c_nodeid_t node, c_sublocid_t subloc,
                             chpl_fn_int_t fid,
                             chpl_comm_on_bundle_t *arg, size_t argSize,
                             int ln, int32_t fn) {
  DBG_PRINTF(DBG_INTERFACE,
             "chpl_comm_execute_on_nb(%d, %d, %d, %p, %zd)",
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
  DBG_PRINTF(DBG_INTERFACE,
             "chpl_comm_execute_on_fast(%d, %d, %d, %p, %zd)",
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
  CHK_TRUE(!(fast && !blocking)); // handler doesn't expect fast nonblocking
  if (argSize <= AM_MAX_MSG_SIZE) {
    arg->comm.xo = (struct chpl_comm_bundleData_execOn_t)
                     { .b = (struct chpl_comm_bundleData_base_t)
                            { .op = am_opExecOn, .node = chpl_nodeID },
                       .fast = fast,
                       .fid = fid,
                       .argSize = argSize,
                       .subloc = subloc,
                       .pAmDone = NULL };
    amRequestCommon(node, arg, argSize,
                    blocking ? &arg->comm.xo.pAmDone : NULL,
                    false, blocking);
  } else {
    arg->comm.xol = (struct chpl_comm_bundleData_execOnLrg_t)
                      { .b = (struct chpl_comm_bundleData_base_t)
                             { .op = am_opExecOnLrg, .node = chpl_nodeID },
                        .fid = fid,
                        .argSize = argSize,
                        .arg = arg,
                        .subloc = subloc,
                        .gotArg = 0,
                        .pAmDone = NULL };
    chpl_atomic_thread_fence(memory_order_release);
    amRequestCommon(node, arg, sizeof(*arg),
                    blocking ? &arg->comm.xol.pAmDone : NULL,
                    false, blocking);
    if (!blocking) {
      //
      // Even if non-blocking, we cannot return until after the target
      // node has retrieved the argument from us.
      //
      while (!*(volatile chpl_comm_amDone_t*) &arg->comm.xol.gotArg) {
        local_yield();
      }
    }
  }
}


static inline
void amRequestRMA(c_nodeid_t node, amOp_t op,
                  void* addr, void* raddr, size_t size) {
  chpl_comm_on_bundle_t arg;
  arg.comm.rma = (struct chpl_comm_bundleData_RMA_t)
                   { .b = (struct chpl_comm_bundleData_base_t)
                          { .op = op, .node = chpl_nodeID },
                     .addr = raddr,
                     .raddr = addr,
                     .size = size,
                     .pAmDone = NULL };
  amRequestCommon(node, &arg,
                  (offsetof(chpl_comm_on_bundle_t, comm)
                   + sizeof(arg.comm.rma)),
                  &arg.comm.rma.pAmDone, false, true);
}


static inline
void amRequestAMO(c_nodeid_t node, void* object,
                  const void* operand1, const void* operand2, void* result,
                  int ofiOp, enum fi_datatype ofiType, size_t size) {
  DBG_PRINTF(DBG_AMO,
             "AMO via AM: obj %d:%p, opnd1 <%s>, opnd2 <%s>, res %p, "
             "op %s, typ %s, sz %zd",
             (int) node, object,
             DBG_VAL(operand1, ofiType), DBG_VAL(operand2, ofiType), result,
             amo_opName(ofiOp), amo_typeName(ofiType), size);

  void* myResult = result;
  size_t resSize = (ofiOp == FI_CSWAP) ? sizeof(chpl_bool32) : size;
  if (myResult != NULL) {
    if (mrGetLocalKey(myResult, resSize) != 0) {
      myResult = allocBounceBuf(resSize);
      DBG_PRINTF(DBG_AMO, "AMO result BB: %p", myResult);
      CHK_TRUE(mrGetLocalKey(myResult, resSize) == 0);
    }
  }

  chpl_comm_on_bundle_t arg;
  arg.comm.amo = (struct chpl_comm_bundleData_AMO_t)
                   { .b = (struct chpl_comm_bundleData_base_t)
                          { .op = am_opAMO, .node = chpl_nodeID },
                     .ofiOp = ofiOp,
                     .ofiType = ofiType,
                     .size = size,
                     .obj = object,
                     .operand1 = { 0 },
                     .operand2 = { 0 },
                     .result = myResult,
                     .pAmDone = NULL };
  if (operand1 != NULL) {
    memcpy(&arg.comm.amo.operand1, operand1, size);
  }
  if (operand2 != NULL) {
    memcpy(&arg.comm.amo.operand2, operand2, size);
  }
  amRequestCommon(node, &arg,
                  (offsetof(chpl_comm_on_bundle_t, comm)
                   + sizeof(arg.comm.amo)),
                  &arg.comm.amo.pAmDone, true, true);
  if (myResult != result) {
    memcpy(result, myResult, resSize);
    freeBounceBuf(myResult);
  }
}

static inline
void amRequestShutdown(c_nodeid_t node) {
  chpl_comm_on_bundle_t arg;
  arg.comm.b = (struct chpl_comm_bundleData_base_t)
                 { .op = am_opShutdown, .node = chpl_nodeID };
  amRequestCommon(node, &arg,
                  (offsetof(chpl_comm_on_bundle_t, comm) + sizeof(arg.comm.b)),
                  NULL, false, true);
}


static inline
void amRequestCommon(c_nodeid_t node,
                     chpl_comm_on_bundle_t* arg, size_t argSize,
                     chpl_comm_amDone_t** ppAmDone, chpl_bool isOrderedAMO,
                     chpl_bool yieldDuringTxnWait) {
  //
  // If blocking, make sure target can RMA PUT the indicator to us.
  //
  chpl_comm_amDone_t amDone;
  chpl_comm_amDone_t* pAmDone = NULL;
  if (ppAmDone != NULL) {
    pAmDone = &amDone;
    if (mrGetLocalKey(pAmDone, sizeof(*pAmDone)) != 0) {
      pAmDone = allocBounceBuf(sizeof(*pAmDone));
      CHK_TRUE(mrGetLocalKey(pAmDone, sizeof(*pAmDone)) == 0);
    }
    *pAmDone = 0;
    chpl_atomic_thread_fence(memory_order_release);

    *ppAmDone = pAmDone;
  }

#ifdef CHPL_COMM_DEBUG
  if (DBG_TEST_MASK(DBG_AM | DBG_AMSEND | DBG_AMRECV)) {
    static atomic_uint_least64_t seq;

    static chpl_bool seqInited = false;
    if (!seqInited) {
      static pthread_mutex_t seqLock = PTHREAD_MUTEX_INITIALIZER;
      PTHREAD_CHK(pthread_mutex_lock(&seqLock));
      atomic_init_uint_least64_t(&seq, 1);
      seqInited = true;
      PTHREAD_CHK(pthread_mutex_unlock(&seqLock));
    }

    arg->comm.b.seq = atomic_fetch_add_uint_least64_t(&seq, 1);

#ifdef DEBUG_CRC_MSGS
    arg->comm.b.crc = 0;
    arg->comm.b.crc = xcrc32((void*) arg, argSize, 0xffffffff);
#endif
  }
#endif

  struct perTxCtxInfo_t* tcip;
  CHK_TRUE((tcip = tciAlloc()) != NULL);

  chpl_comm_on_bundle_t* myArg = arg;
  void* mrDesc = NULL;
  if (mrGetDesc(&mrDesc, myArg, argSize) != 0) {
    myArg = allocBounceBuf(argSize);
    DBG_PRINTF(DBG_AM, "AM arg BB: %p", myArg);
    CHK_TRUE(mrGetDesc(NULL, myArg, argSize) == 0);
    memcpy(myArg, arg, argSize);
  }

  atomic_bool txnDone;
  void* ctx;

  if (myArg->comm.b.op == am_opAMO) {
    //
    // For an ordered AMO, retire all our outstanding (unordered) ones
    // before we initiate this one, and then wait for it specifically
    // afterward.  For an unordered one, just initiate it straightaway
    // and we'll wait for it later, if ever.
    //
    chpl_comm_taskPrvData_t* prvData = get_comm_taskPrvdata();
    if (isOrderedAMO) {
      waitForCQAllTxns(tcip, prvData);
      atomic_init_bool(&txnDone, false);
      ctx = txnTrkEncode(txnTrkDone, &txnDone);
    } else {
      ctx = txnTrkEncode(txnTrkCntr, &prvData->numTxnsOut);
      prvData->numTxnsOut++;  // count txn now, saving control flow later
    }
  } else {
    //
    // Wait for non-AMOs individually after initiating them, but there's
    // no need to synchronize beforehand with any (unordered) AMOs that
    // might be outstanding.
    //
    atomic_init_bool(&txnDone, false);
    ctx = txnTrkEncode(txnTrkDone, &txnDone);
  }

  DBG_PRINTF(DBG_AM | DBG_AMSEND,
             "tx AM req to %d: seqId %d:%" PRIu64 ", %s, size %zd, "
             "pAmDone %p, ctx %p",
             node, chpl_nodeID, myArg->comm.b.seq,
             am_opName(myArg->comm.b.op), argSize, pAmDone, ctx);
  OFI_RIDE_OUT_EAGAIN(fi_send(tcip->txCtx, myArg, argSize,
                              mrDesc, rxMsgAddr(tcip, node), ctx),
                      checkTxCQ(tcip));

  //
  // If this is a non-AMO or ordered AMO then wait for the network
  // completion.  Yield initially while doing so; the minimum round
  // trip time on the network isn't small and maybe we can find
  // something else to do in the meantime.  After that, only yield
  // every 64 attempts.
  //
  tciFree(tcip);

  if (myArg->comm.b.op != am_opAMO || isOrderedAMO) {
    int iters = 0;
    do {
      if (yieldDuringTxnWait && (iters++ & 0x3f) == 0) {
        local_yield();
      }
      if (tciTryRealloc(tcip)) {
        checkTxCQ(tcip);
        tciFree(tcip);
      }
    } while (!atomic_load_explicit_bool(&txnDone, memory_order_acquire));

    atomic_destroy_bool(&txnDone);
  }

  if (myArg != arg) {
    freeBounceBuf(myArg);
  }

  if (pAmDone != NULL) {
    //
    // Wait for executeOn completion indicator.
    //
    DBG_PRINTF(DBG_AM | DBG_AMSEND,
               "waiting for amDone indication in %p", pAmDone);
    while (!*(volatile chpl_comm_amDone_t*) pAmDone) {
      local_yield();
    }
    DBG_PRINTF(DBG_AM | DBG_AMSEND, "saw amDone indication in %p", pAmDone);
    if (pAmDone != &amDone)
      freeBounceBuf(pAmDone);
  }
}


////////////////////////////////////////
//
// Internal active message support
//

static int numAmHandlersActive;
static atomic_bool amHandlersExit;
static pthread_cond_t amStartStopCond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t amStartStopMutex = PTHREAD_MUTEX_INITIALIZER;


static void amHandler(void*);
static void processRxAmReq(struct perTxCtxInfo_t*);
static void amHandleExecOn(chpl_comm_on_bundle_t*);
static inline void amWrapExecOnBody(void*);
static void amHandleExecOnLrg(chpl_comm_on_bundle_t*);
static void amWrapExecOnLrgBody(void*);
static void amWrapGet(void*);
static void amWrapPut(void*);
static void amHandleAMO(struct perTxCtxInfo_t*, chpl_comm_on_bundle_t*);
static inline void amSendDone(struct chpl_comm_bundleData_base_t*,
                              chpl_comm_amDone_t*);

static inline void doCpuAMO(void*, const void*, const void*, void*,
                            enum fi_op, enum fi_datatype, size_t size);


static
void init_amHandling(void) {
  //
  // Start AM handler thread(s).  Don't proceed from here until at
  // least one is running.
  //
  atomic_init_bool(&amHandlersExit, false);

  PTHREAD_CHK(pthread_mutex_lock(&amStartStopMutex));
  for (int i = 0; i < numAmHandlers; i++) {
    CHK_TRUE(chpl_task_createCommTask(amHandler, NULL) == 0);
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
  // here until the last one has finished (TODO).
  //
  PTHREAD_CHK(pthread_mutex_lock(&amStartStopMutex));
  atomic_store_bool(&amHandlersExit, true);
  PTHREAD_CHK(pthread_cond_wait(&amStartStopCond, &amStartStopMutex));
  PTHREAD_CHK(pthread_mutex_unlock(&amStartStopMutex));

  atomic_destroy_bool(&amHandlersExit);
}


//
// The AM handler runs this.
//
static
void amHandler(void* argNil) {
  struct perTxCtxInfo_t* tcip;
  CHK_TRUE((tcip = tciAllocForAmHandler()) != NULL);

  isAmHandler = true;

  DBG_PRINTF(DBG_THREADS, "AM handler running");

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
    int ret;
    if (ofi_amhWaitSet != NULL) {
      OFI_CHK_2(fi_wait(ofi_amhWaitSet, 100 /*ms*/), ret, -FI_ETIMEDOUT);
    } else {
      sched_yield();
      ret = FI_SUCCESS;
    }
    if (ret == FI_SUCCESS) {
      processRxAmReq(tcip);
      if (tcip->txCQ != NULL) {
        checkTxCQ(tcip);
      } else {
        getTxCntr(tcip);
      }
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

  DBG_PRINTF(DBG_THREADS, "AM handler done");
}


static
void processRxAmReq(struct perTxCtxInfo_t* tcip) {
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
      chpl_comm_on_bundle_t* req = (chpl_comm_on_bundle_t*) cqes[i].buf;
      DBG_PRINTF(DBG_AM | DBG_AMRECV,
                 "CQ rx AM req @ buffer offset %zd: "
                 "seqId %d:%" PRIu64 ", %s, size %zd",
                 (char*) req - (char*) ofi_msg_reqs.msg_iov->iov_base,
                 req->comm.b.node, req->comm.b.seq,
                 am_opName(req->comm.b.op), cqes[i].len);

#if defined(CHPL_COMM_DEBUG) && defined(DEBUG_CRC_MSGS)
      if (DBG_TEST_MASK(DBG_AM)) {
        unsigned int sent_crc = req->comm.b.crc;
        req->comm.b.crc = 0;
        unsigned int rcvd_crc = xcrc32((void*) req, req->comm.xo.argSize,
                                       0xffffffff);
        CHK_TRUE(rcvd_crc == sent_crc);
      }
#endif

      switch (req->comm.b.op) {
      case am_opExecOn:
        if (req->comm.xo.fast) {
          amWrapExecOnBody(req);
        } else {
          amHandleExecOn(req);
        }
        break;

      case am_opExecOnLrg:
        amHandleExecOnLrg(req);
        break;

      case am_opGet:
        //
        // We use a task here mainly to ensure that the GET this AM
        // performs completes before we send the 'done' indicator.  If
        // the AM handler did the GET directly, its contextless RMA
        // completion counter would make it hard to tell when that GET
        // had completed.
        //
        chpl_task_startMovedTask(FID_NONE, (chpl_fn_p) amWrapGet,
                                 chpl_comm_on_bundle_task_bundle(req),
                                 sizeof(*req), c_sublocid_any,
                                 chpl_nullTaskID);
        break;

      case am_opPut:
        //
        // We use a task here mainly to ensure that the PUT this AM
        // performs completes before we send the 'done' indicator.  If
        // the AM handler did the PUT directly, its contextless RMA
        // completion counter would make it hard to tell when that PUT
        // had completed.
        //
        chpl_task_startMovedTask(FID_NONE, (chpl_fn_p) amWrapPut,
                                 chpl_comm_on_bundle_task_bundle(req),
                                 sizeof(*req), c_sublocid_any,
                                 chpl_nullTaskID);
        break;

      case am_opAMO:
        amHandleAMO(tcip, req);
        break;

      case am_opShutdown:
        chpl_signal_shutdown();
        break;

      default:
        INTERNAL_ERROR_V("unexpected AM op %d", req->comm.b.op);
        break;
      }
    }

    if ((cqes[i].flags & FI_MULTI_RECV) != 0) {
      //
      // Multi-receive buffer filled; post another one.  This should
      // not be seen except on the last received event!
      //
      CHK_TRUE(i == numEvents - 1);
      OFI_CHK(fi_recvmsg(ofi_rxEp, &ofi_msg_reqs, FI_MULTI_RECV));
      DBG_PRINTF(DBG_AM | DBG_AMRECV,
                 "re-post fi_recvmsg(AMLZs, len %zd)",
                 ofi_msg_reqs.msg_iov->iov_len);
    }

    CHK_TRUE((cqes[i].flags & ~(FI_MSG | FI_RECV | FI_MULTI_RECV)) == 0);
  }
}


static
void amHandleExecOn(chpl_comm_on_bundle_t* req) {
  struct chpl_comm_bundleData_execOn_t* xo = &req->comm.xo;
  DBG_PRINTF(DBG_AM | DBG_AMRECV,
             "amHandleExecOn(seqId %d:%" PRIu64 "): fid %d, pAmDone %p",
             (int) xo->b.node, xo->b.seq, xo->fid, xo->pAmDone);

  //
  // We only need a wrapper if we have to send a 'done' indicator back
  // or we need to produce the AM debug output.
  //
  chpl_fn_p fn = ((xo->pAmDone == NULL && !DBG_TEST_MASK(DBG_AM | DBG_AMRECV))
                  ? chpl_ftable[xo->fid]
                  : (chpl_fn_p) amWrapExecOnBody);
  chpl_task_startMovedTask(xo->fid, fn, chpl_comm_on_bundle_task_bundle(req),
                           xo->argSize, xo->subloc, chpl_nullTaskID);
}


static inline
void amWrapExecOnBody(void* p) {
  chpl_comm_on_bundle_t* req = (chpl_comm_on_bundle_t*) p;
  struct chpl_comm_bundleData_execOn_t* xo = &req->comm.xo;
  DBG_PRINTF(DBG_AM | DBG_AMRECV,
             "amWrapExecOnBody(seqId %d:%" PRIu64 "): "
             "%schpl_ftable_call(%d, %p)",
             (int) xo->b.node, xo->b.seq,
             (xo->fast ? "fast " : ""), (int) xo->fid, p);

  chpl_ftable_call(xo->fid, p);
  if (xo->pAmDone != NULL) {
    amSendDone(&xo->b, xo->pAmDone);
  } else {
    DBG_PRINTF(DBG_AM | DBG_AMRECV,
               "amWrapExecOnBody(seqId %d:%" PRIu64 " NB): amDone",
               (int) xo->b.node, xo->b.seq);
  }
}


static
void amHandleExecOnLrg(chpl_comm_on_bundle_t* req) {
  struct chpl_comm_bundleData_execOnLrg_t* xol = &req->comm.xol;
  DBG_PRINTF(DBG_AM | DBG_AMRECV,
             "amHandleExecOnLrg(seqId %d:%" PRIu64 "): fid %d, pAmDone %p",
             (int) xol->b.node, xol->b.seq, xol->fid, xol->pAmDone);

  chpl_task_startMovedTask(xol->fid, (chpl_fn_p) amWrapExecOnLrgBody,
                           chpl_comm_on_bundle_task_bundle(req),
                           xol->argSize, xol->subloc, chpl_nullTaskID);
}


static
void amWrapExecOnLrgBody(void* p) {
  chpl_comm_on_bundle_t* req = (chpl_comm_on_bundle_t*) p;
  struct chpl_comm_bundleData_execOnLrg_t* xol = &req->comm.xol;
  c_nodeid_t node = xol->b.node;
  DBG_PRINTF(DBG_AM | DBG_AMRECV,
             "amWrapExecOnLrgBody(seqId %d:%" PRIu64 "): "
             "chpl_ftable_call(%d, %p)",
             (int) node, xol->b.seq, (int) xol->fid, p);

  //
  // Create space for the full bundle and fill in the header part from
  // what we've already received.  Retrieve the remainder, that is, the
  // args proper, from the initiating node.
  //
  chpl_comm_on_bundle_t* reqCopy;
  CHPL_CALLOC_SZ(reqCopy, 1, xol->argSize);
  chpl_memcpy(reqCopy, req, xol->argSize);
  req = reqCopy;
  xol = &req->comm.xol;

  chpl_comm_on_bundle_t* reqOnOrig = (chpl_comm_on_bundle_t*) xol->arg;
  size_t remnantSize = xol->argSize - sizeof(*req);
  CHK_TRUE(mrGetKey(NULL, NULL, node, &reqOnOrig[1], remnantSize) == 0);
  (void) ofi_get(&req[1], node, &reqOnOrig[1], remnantSize);

  //
  // Iff this is a nonblocking executeOn, tell the initiator we've got
  // the rest of the bundle.  They have to be held until we've got it,
  // so that it doesn't disappear before then.  We don't have to do this
  // for blocking executeOn because the initiator won't proceed until
  // the entire executeOn is complete anyway.  We can save a little bit
  // of time here by not waiting for a network response.  Either we or
  // someone else will consume that completion later.  In the meantime
  // we can go ahead with the executeOn body.
  //
  if (xol->pAmDone == NULL) {
    static __thread chpl_comm_amDone_t* myGotArg = NULL;
    if (myGotArg == NULL) {
      myGotArg = allocBounceBuf(1);
      *myGotArg = 1;
    }

    chpl_comm_amDone_t* origGotArg = &reqOnOrig->comm.xol.gotArg;
    DBG_PRINTF(DBG_AM, "AM seqId %d:%" PRIu64 ": set gotArg (NB) %p",
               (int) node, xol->b.seq, origGotArg);
    ofi_put_ll(myGotArg, node, origGotArg, sizeof(*origGotArg),
               txnTrkEncode(txnTrkNone, NULL));
  }

  //
  // Now we can finally call the body function.
  //
  chpl_ftable_call(xol->fid, req);
  if (xol->pAmDone != NULL) {
    amSendDone(&xol->b, xol->pAmDone);
  } else {
    DBG_PRINTF(DBG_AM | DBG_AMRECV,
               "amWrapExecOnLrgBody(seqId %d:%" PRIu64 " NB): amDone",
               (int) node, xol->b.seq);
  }

  CHPL_FREE(req);
}


static
void amWrapGet(void* p) {
  chpl_comm_on_bundle_t* req = (chpl_comm_on_bundle_t*) p;
  struct chpl_comm_bundleData_RMA_t* rma = &req->comm.rma;
  DBG_PRINTF(DBG_AM | DBG_AMRECV,
             "amWrapGet(seqId %d:%" PRIu64 "): %p <- %d:%p (%zd bytes)",
             (int) rma->b.node, rma->b.seq,
             rma->addr, (int) rma->b.node, rma->raddr, rma->size);

  CHK_TRUE(mrGetKey(NULL, NULL, rma->b.node, rma->raddr, rma->size) == 0);
  (void) ofi_get(rma->addr, rma->b.node, rma->raddr, rma->size);

  amSendDone(&rma->b, rma->pAmDone);
}


static
void amWrapPut(void* p) {
  chpl_comm_on_bundle_t* req = (chpl_comm_on_bundle_t*) p;
  struct chpl_comm_bundleData_RMA_t* rma = &req->comm.rma;
  DBG_PRINTF(DBG_AM | DBG_AMRECV,
             "amWrapPut(seqId %d:%" PRIu64 ") %d:%p <-- %p (%zd bytes)",
             (int) rma->b.node, rma->b.seq,
             (int) rma->b.node, rma->raddr, rma->addr, rma->size);

  CHK_TRUE(mrGetKey(NULL, NULL, rma->b.node, rma->raddr, rma->size) == 0);
  (void) ofi_put(rma->addr, rma->b.node, rma->raddr, rma->size);

  amSendDone(&rma->b, rma->pAmDone);
}


static
void amHandleAMO(struct perTxCtxInfo_t* tcip, chpl_comm_on_bundle_t* req) {
  struct chpl_comm_bundleData_AMO_t* amo = &req->comm.amo;
  if (amo->ofiOp == FI_CSWAP) {
    DBG_PRINTF(DBG_AM | DBG_AMRECV | DBG_AMO,
               "amHandleAMO(seqId %d:%" PRIu64 "): "
               "obj %p, opnd1 %s, opnd2 %s, "
               "res %p, ofiOp %s, ofiType %s, sz %d",
               (int) amo->b.node, amo->b.seq,
               amo->obj,
               DBG_VAL(&amo->operand1, amo->ofiType),
               DBG_VAL(&amo->operand2, amo->ofiType),
               amo->result, amo_opName(amo->ofiOp),
               amo_typeName(amo->ofiType), amo->size);
  } else if (amo->result != NULL) {
    if (amo->ofiOp == FI_ATOMIC_READ) {
      DBG_PRINTF(DBG_AM | DBG_AMRECV | DBG_AMO,
                 "amHandleAMO(seqId %d:%" PRIu64 "): "
                 "obj %p, res %p, ofiOp %s, ofiType %s, sz %d",
                 (int) amo->b.node, amo->b.seq,
                 amo->obj, amo->result, amo_opName(amo->ofiOp),
                 amo_typeName(amo->ofiType), amo->size);
    } else {
      DBG_PRINTF(DBG_AM | DBG_AMRECV | DBG_AMO,
                 "amHandleAMO(seqId %d:%" PRIu64 "): "
                 "obj %p, opnd %s, res %p, ofiOp %s, ofiType %s, sz %d",
                 (int) amo->b.node, amo->b.seq,
                 amo->obj, DBG_VAL(&amo->operand1, amo->ofiType),
                 amo->result, amo_opName(amo->ofiOp),
                 amo_typeName(amo->ofiType), amo->size);
    }
  } else {
    DBG_PRINTF(DBG_AM | DBG_AMRECV | DBG_AMO,
               "amHandleAMO(seqId %d:%" PRIu64 "): "
               "obj %p, opnd %s, ofiOp %s, ofiType %s, sz %d",
               (int) amo->b.node, amo->b.seq,
               amo->obj, DBG_VAL(&amo->operand1, amo->ofiType),
               amo_opName(amo->ofiOp), amo_typeName(amo->ofiType),
               amo->size);
  }
  chpl_amo_datum_t result;
  size_t resSize = (amo->ofiOp == FI_CSWAP) ? sizeof(chpl_bool32) : amo->size;
  doCpuAMO(amo->obj, &amo->operand1, &amo->operand2, &result,
           amo->ofiOp, amo->ofiType, amo->size);

  if (amo->result != NULL) {
    if (amo->b.node == chpl_nodeID) {
      //
      // Short-circuit result delivery for same-node AMOs.
      //
      memcpy(amo->result, &result, resSize);
      chpl_atomic_thread_fence(memory_order_release);
    } else {
      CHK_TRUE(mrGetKey(NULL, NULL, amo->b.node, amo->result, resSize) == 0);
      (void) ofi_put(&result, amo->b.node, amo->result, resSize);

      //
      // We must guarantee the result has arrived at the destination
      // before we send the 'done' indicator.  Currently ofi_put() does
      // not return until it's seen the delivery completion event, so
      // the guarantee holds.  But someday we might like to get better
      // comm/compute overlap by starting the next AM while this result
      // PUT is still in flight, and sending this 'done' later once we
      // know the latter got there.
      //
    }
  }

  if (amo->b.node == chpl_nodeID) {
    *amo->pAmDone = 1;
    chpl_atomic_thread_fence(memory_order_release);
  } else {
    amSendDone(&amo->b, amo->pAmDone);
  }
}


static inline
void amSendDone(struct chpl_comm_bundleData_base_t* b,
                chpl_comm_amDone_t* pAmDone) {
  static __thread chpl_comm_amDone_t* amDone = NULL;
  if (amDone == NULL) {
    amDone = allocBounceBuf(1);
    *amDone = 1;
  }

  //
  // Send the 'done' indicator without waiting for a completion.
  // Either we or someone else will consume that completion later.
  //
  DBG_PRINTF(DBG_AM, "AM seqId %d:%" PRIu64 ": set pAmDone (NB) %p",
             (int) b->node, b->seq, pAmDone);
  ofi_put_ll(amDone, b->node, pAmDone, sizeof(*pAmDone),
             txnTrkEncode(txnTrkNone, NULL));
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
  get_strd_common(dstaddr_arg, dststrides,
                  srcnode,
                  srcaddr_arg, srcstrides,
                  count, stridelevels, elemSize,
                  1, NULL,
                  commID, ln, fn);
}

#define MAX_UNORDERED_TRANS_SZ 1024
void chpl_comm_getput_unordered(c_nodeid_t dstnode, void* dstaddr,
                                c_nodeid_t srcnode, void* srcaddr,
                                size_t size, int32_t commID,
                                int ln, int32_t fn) {
  assert(dstaddr != NULL);
  assert(srcaddr != NULL);

  if (size == 0)
    return;

  if (dstnode == chpl_nodeID && srcnode == chpl_nodeID) {
    memmove(dstaddr, srcaddr, size);
    return;
  }

  if (dstnode == chpl_nodeID) {
    chpl_comm_get(dstaddr, srcnode, srcaddr, size, commID, ln, fn);
  } else if (srcnode == chpl_nodeID) {
    chpl_comm_put(srcaddr, dstnode, dstaddr, size, commID, ln, fn);
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
  chpl_comm_get(addr, node, raddr, size, commID, ln, fn);
}

void chpl_comm_put_unordered(void* addr, c_nodeid_t node, void* raddr,
                             size_t size, int32_t commID, int ln, int32_t fn) {
  chpl_comm_put(addr, node, raddr, size, commID, ln, fn);
}

void chpl_comm_getput_unordered_task_fence(void) { }


////////////////////////////////////////
//
// Internal communication support
//

static inline struct perTxCtxInfo_t* tciAllocCommon(chpl_bool);
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

    if (!atomic_exchange_bool(&_ttcip->allocated, true)) {
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
      || (tciTabFixedAssignments && chpl_task_isFixedThread())) {
    _ttcip->bound = true;
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

  if (bindToAmHandler) {
    //
    // AM handlers use tciTab[numWorkerTxCtxs .. tciTabLen - 1].  For
    // now we only support a single AM handler, so this is simple.  If
    // we ever have more, the CHK_FALSE will force us to revisit this.
    //
    tcip = &tciTab[numWorkerTxCtxs];
    CHK_FALSE(atomic_exchange_bool(&tcip->allocated, true));
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
      if (!atomic_exchange_bool(&tciTab[iw].allocated, true)) {
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


static inline
chpl_bool tciTryRealloc(struct perTxCtxInfo_t* tcip) {
  if (tcip == _ttcip && tcip->bound) {
    DBG_PRINTF(DBG_TCIPS, "tryRealloced bound tciTab[%td]", tcip - tciTab);
    return true;
  }

  if (!atomic_exchange_bool(&tcip->allocated, true)) {
    DBG_PRINTF(DBG_TCIPS, "tryRealloced tciTab[%td]", tcip - tciTab);
    CHK_TRUE(!tcip->bound);
    return true;
  }

  return false;
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
chpl_comm_nb_handle_t ofi_put(const void* addr, c_nodeid_t node,
                              void* raddr, size_t size) {
  //
  // Don't ask the provider to transfer more than it wants to.
  //
  if (size > ofi_info->ep_attr->max_msg_size) {
    DBG_PRINTF(DBG_RMA | DBG_RMAWRITE,
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

  DBG_PRINTF(DBG_RMA | DBG_RMAWRITE,
             "PUT %d:%p <= %p, size %zd",
             (int) node, raddr, addr, size);

  void* myAddr = (void*) addr;

  uint64_t mrKey;
  uint64_t mrRaddr;
  if (mrGetKey(&mrKey, &mrRaddr, node, raddr, size) == 0) {
    //
    // The remote address is RMA-accessible; PUT directly to it.
    //
    void* mrDesc = NULL;
    if (mrGetDesc(&mrDesc, myAddr, size) != 0) {
      myAddr = allocBounceBuf(size);
      DBG_PRINTF(DBG_RMA | DBG_RMAWRITE, "PUT src BB: %p", myAddr);
      CHK_TRUE(mrGetDesc(&mrDesc, myAddr, size) == 0);
      memcpy(myAddr, addr, size);
    }

    atomic_bool txnDone;
    atomic_init_bool(&txnDone, false);

    void* ctx = txnTrkEncode(txnTrkDone, &txnDone);

    struct perTxCtxInfo_t* tcip;
    CHK_TRUE((tcip = tciAlloc()) != NULL);

    DBG_PRINTF(DBG_RMA | DBG_RMAWRITE,
               "tx write: %d:%p <= %p, size %zd, key 0x%" PRIx64 ", ctx %p",
               (int) node, raddr, myAddr, size, mrKey, ctx);
    OFI_RIDE_OUT_EAGAIN(fi_write(tcip->txCtx, myAddr, size,
                                 mrDesc, rxRmaAddr(tcip, node),
                                 mrRaddr, mrKey, ctx),
                        checkTxCQ(tcip));

    if (tcip->txCQ != NULL) {
      waitForCQThisTxn(tcip, &txnDone);
    } else {
      //
      // TODO: This is the AM handler and we could optimize this to
      //       let us return more quickly and handle the completion
      //       later.  But for now just wait for it.
      //
      tcip->numTxnsBegun++;
      waitForCntrAllTxns(tcip);
    }

    tciFree(tcip);
  } else {
    //
    // The remote address is not RMA-accessible.  Make sure that the
    // local one is, then do the opposite RMA from the remote side.
    //
    if (mrGetLocalKey(myAddr, size) != 0) {
      myAddr = allocBounceBuf(size);
      DBG_PRINTF(DBG_RMA | DBG_RMAWRITE, "PUT via AM GET tgt BB: %p", myAddr);
      CHK_TRUE(mrGetLocalKey(myAddr, size) == 0);
      memcpy(myAddr, addr, size);
    }

    DBG_PRINTF(DBG_RMA | DBG_RMAWRITE,
               "PUT %d:%p <= %p, size %zd, via AM GET",
               (int) node, raddr, myAddr, size);
    amRequestRMA(node, am_opGet, myAddr, raddr, size);
  }

  if (myAddr != addr) {
    freeBounceBuf(myAddr);
  }

  return NULL;
}


static inline
void ofi_put_ll(const void* addr, c_nodeid_t node,
                void* raddr, size_t size, void* ctx) {
  DBG_PRINTF(DBG_RMA | DBG_RMAWRITE,
             "PUT %d:%p <= %p, size %zd",
             (int) node, raddr, addr, size);

  uint64_t mrKey = 0;
  uint64_t mrRaddr = 0;
  CHK_TRUE(mrGetKey(&mrKey, &mrRaddr, node, raddr, size) == 0);

  void* myAddr = (void*) addr;
  void* mrDesc = NULL;
  CHK_TRUE(mrGetDesc(&mrDesc, myAddr, size) == 0);

  struct perTxCtxInfo_t* tcip;
  CHK_TRUE((tcip = tciAlloc()) != NULL);

  DBG_PRINTF(DBG_RMA | DBG_RMAWRITE,
             "tx write ll: %d:%p <= %p, size %zd, key 0x%" PRIx64 ", ctx %p",
             (int) node, raddr, myAddr, size, mrKey, ctx);
  OFI_RIDE_OUT_EAGAIN(fi_write(tcip->txCtx, myAddr, size,
                               mrDesc, rxRmaAddr(tcip, node),
                               mrRaddr, mrKey, ctx),
                      checkTxCQ(tcip));
  if (tcip->txCntr != NULL) {
    tcip->numTxnsBegun++;
  }
  tciFree(tcip);
}


static inline
chpl_comm_nb_handle_t ofi_get(void* addr, c_nodeid_t node,
                              void* raddr, size_t size) {
  //
  // Don't ask the provider to transfer more than it wants to.
  //
  if (size > ofi_info->ep_attr->max_msg_size) {
    DBG_PRINTF(DBG_RMA | DBG_RMAWRITE,
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

  DBG_PRINTF(DBG_RMA | DBG_RMAREAD,
             "GET %p <= %d:%p, size %zd",
             addr, (int) node, raddr, size);

  void* myAddr = addr;

  uint64_t mrKey;
  uint64_t mrRaddr;
  if (mrGetKey(&mrKey, &mrRaddr, node, raddr, size) == 0) {
    //
    // The remote address is RMA-accessible; GET directly from it.
    //
    void* mrDesc = NULL;
    if (mrGetDesc(&mrDesc, myAddr, size) != 0) {
      myAddr = allocBounceBuf(size);
      DBG_PRINTF(DBG_RMA | DBG_RMAREAD, "GET tgt BB: %p", myAddr);
      CHK_TRUE(mrGetDesc(&mrDesc, myAddr, size) == 0);
    }

    atomic_bool txnDone;
    atomic_init_bool(&txnDone, false);

    void* ctx = txnTrkEncode(txnTrkDone, &txnDone);

    struct perTxCtxInfo_t* tcip;
    CHK_TRUE((tcip = tciAlloc()) != NULL);
    DBG_PRINTF(DBG_RMA | DBG_RMAREAD,
               "tx read: %p <= %d:%p(0x%" PRIx64 "), size %zd, key 0x%" PRIx64
               ", ctx %p",
               myAddr, (int) node, raddr, mrRaddr, size, mrKey, ctx);
    OFI_RIDE_OUT_EAGAIN(fi_read(tcip->txCtx, myAddr, size,
                                mrDesc, rxRmaAddr(tcip, node),
                                mrRaddr, mrKey, ctx),
                        checkTxCQ(tcip));
    waitForCQThisTxn(tcip, &txnDone);
    tciFree(tcip);
  } else {
    //
    // The remote address is not RMA-accessible.  Make sure that the
    // local one is, then do the opposite RMA from the remote side.
    //
    if (mrGetLocalKey(myAddr, size) != 0) {
      myAddr = allocBounceBuf(size);
      DBG_PRINTF(DBG_RMA | DBG_RMAWRITE, "GET via AM PUT src BB: %p", myAddr);
      CHK_TRUE(mrGetLocalKey(myAddr, size) == 0);
    }

    DBG_PRINTF(DBG_RMA | DBG_RMAREAD,
               "GET %p <= %d:%p, size %zd, via AM PUT",
               myAddr, (int) node, raddr, size);
    amRequestRMA(node, am_opPut, myAddr, raddr, size);
  }

  if (myAddr != addr) {
    memcpy(addr, myAddr, size);
    freeBounceBuf(myAddr);
  }

  return NULL;
}


static
chpl_comm_nb_handle_t ofi_amo(struct perTxCtxInfo_t* tcip,
                              c_nodeid_t node, uint64_t object, uint64_t mrKey,
                              const void* operand1, const void* operand2,
                              void* result,
                              enum fi_op ofiOp, enum fi_datatype ofiType,
                              size_t size) {
  //
  // A wrinkle: for a cmpxchg/CSWAP our caller wants chpl_bool32 true
  // if the compare&swap succeeded, false otherwise.  But the libfabric
  // atomic compare returns the previous value of the operand, not T/F.
  // We have to synthesize the result to be returned to our caller,
  // based on comparing that previous value to the comparand.
  //
  chpl_amo_datum_t ofiCmpRes;
  void* myRes = (ofiOp == FI_CSWAP) ? &ofiCmpRes : result;
  void* mrDescRes = NULL;
  if (myRes != NULL && mrGetDesc(&mrDescRes, myRes, size) != 0) {
    myRes = allocBounceBuf(size);
    DBG_PRINTF(DBG_AMO, "AMO result BB: %p", myRes);
    CHK_TRUE(mrGetDesc(&mrDescRes, myRes, size) == 0);
  }

  void* myOpnd1 = (void*) operand1;
  void* mrDescOpnd1 = NULL;
  if (myOpnd1 != NULL && mrGetDesc(&mrDescOpnd1, myOpnd1, size) != 0) {
    myOpnd1 = allocBounceBuf(size);
    DBG_PRINTF(DBG_AMO, "AMO operand1 BB: %p", myOpnd1);
    CHK_TRUE(mrGetDesc(&mrDescOpnd1, myOpnd1, size) == 0);
    memcpy(myOpnd1, operand1, size);
  }

  void* myOpnd2 = (void*) operand2;
  void* mrDescOpnd2 = NULL;
  if (myOpnd2 != NULL && mrGetDesc(&mrDescOpnd2, myOpnd2, size) != 0) {
    myOpnd2 = allocBounceBuf(size);
    DBG_PRINTF(DBG_AMO, "AMO operand2 BB: %p", myOpnd2);
    CHK_TRUE(mrGetDesc(&mrDescOpnd2, myOpnd2, size) == 0);
    memcpy(myOpnd2, operand2, size);
  }

  chpl_bool ordered = true; // always true now, but soon we'll have unordered

  //
  // For an ordered AMO, retire all our outstanding (unordered) ones
  // before we initiate this one, and then wait for it specifically
  // afterward.  For an unordered one, just initiate it straightaway
  // and we'll wait for it later, if ever.
  //
  atomic_bool txnDone;
  atomic_init_bool(&txnDone, false);

  void* ctx;

  if (tcip->txCQ != NULL) {
    chpl_comm_taskPrvData_t* prvData = get_comm_taskPrvdata();
    if (ordered) {
      waitForCQAllTxns(tcip, prvData);
      ctx = txnTrkEncode(txnTrkDone, &txnDone);
    } else {
      ctx = txnTrkEncode(txnTrkCntr, &prvData->numTxnsOut);
      prvData->numTxnsOut++;  // counting txn now allows smaller prvData scope
    }
  } else {
    if (ordered) {
      waitForCntrAllTxns(tcip);
    }
    ctx = NULL;
  }

  DBG_PRINTF(DBG_AMO,
             "tx AMO: obj %d:%" PRIx64 ", opnd1 <%s>, opnd2 <%s>, "
             "op %s, typ %s, sz %zd, ctx %p",
             (int) node, object,
             DBG_VAL(myOpnd1, ofiType), DBG_VAL(myOpnd2, ofiType),
             amo_opName(ofiOp), amo_typeName(ofiType), size, ctx);

  if (ofiOp == FI_CSWAP) {
    OFI_CHK(fi_compare_atomic(tcip->txCtx,
                              myOpnd2, 1, mrDescOpnd2, myOpnd1, mrDescOpnd1,
                              myRes, mrDescRes,
                              rxRmaAddr(tcip, node), object, mrKey,
                              ofiType, ofiOp, ctx));
  } else if (result != NULL) {
    void* bufArg = myOpnd1;
    if (provCtl_readAmoNeedsOpnd) {
      // Workaround for RxD bug.
      if (ofiOp == FI_ATOMIC_READ && bufArg == NULL) {
        static int64_t dummy;
        bufArg = &dummy;
      }
    }
    OFI_CHK(fi_fetch_atomic(tcip->txCtx,
                            bufArg, 1, mrDescOpnd1, myRes, mrDescRes,
                            rxRmaAddr(tcip, node), object, mrKey,
                            ofiType, ofiOp, ctx));
  } else {
    OFI_CHK(fi_atomic(tcip->txCtx,
                      myOpnd1, 1, mrDescOpnd1,
                      rxRmaAddr(tcip, node), object, mrKey,
                      ofiType, ofiOp, ctx));
  }

  //
  // Wait for network completion.
  //
  if (tcip->txCQ != NULL) {
    if (ordered) {
      waitForCQThisTxn(tcip, &txnDone);
    }
  } else {
    tcip->numTxnsBegun++;
    if (ordered) {
      waitForCntrAllTxns(tcip);
    }
  }

  if (ofiOp == FI_CSWAP) {
    *(chpl_bool32*) result = (memcmp(myRes, operand1, size) == 0);
    if (myRes != &ofiCmpRes) {
      freeBounceBuf(myRes);
    }
  } else if (result != NULL) {
    if (myRes != result) {
      memcpy(result, myRes, size);
      freeBounceBuf(myRes);
    }
  }

  if (result != NULL) {
    DBG_PRINTF(DBG_AMO,
               "  AMO result: %p is %s",
               result,
               DBG_VAL(result, (ofiOp == FI_CSWAP) ? FI_INT32 : ofiType));
  }

  if (myOpnd1 != operand1) {
    freeBounceBuf(myOpnd1);
  }

  if (myOpnd2 != operand2) {
    freeBounceBuf(myOpnd2);
  }

  return NULL;
}


static inline
void waitForCQThisTxn(struct perTxCtxInfo_t* tcip, atomic_bool* pTxnDone) {
  while (!atomic_load_explicit_bool(pTxnDone, memory_order_acquire)) {
    checkTxCQ(tcip);
  }
}


static inline
void waitForCQAllTxns(struct perTxCtxInfo_t* tcip,
                      chpl_comm_taskPrvData_t* prvData) {
  while (prvData->numTxnsOut > 0) {
    checkTxCQ(tcip);
  }
}


static inline
void checkTxCQ(struct perTxCtxInfo_t* tcip) {
  struct fi_cq_msg_entry cqes[5];
  const int cqesSize = sizeof(cqes) / sizeof(cqes[0]);
  const ssize_t numEvents = readCQ(tcip->txCQ, cqes, cqesSize);

  if (numEvents == 0) {
    sched_yield();
    ensure_progress();
  } else {
    for (int i = 0; i < numEvents; i++) {
      struct fi_cq_msg_entry* cqe = &cqes[i];
      DBG_PRINTF(DBG_ACK, "CQ ack tx, flags %#" PRIx64, cqe->flags);
      const txnTrkCtx_t trk = txnTrkDecode(cqe->op_context);
      switch (trk.typ) {
      case txnTrkNone:
        break;
      case txnTrkDone:
        atomic_store_explicit_bool((atomic_bool*) trk.ptr, true,
                                   memory_order_release);
        break;
      case txnTrkCntr:
        (*((int*) trk.ptr))--;
        break;
      default:
        INTERNAL_ERROR_V("unexpected trk.typ %d", trk.typ);
        break;
      }
    }
  }
}


static inline
ssize_t readCQ(struct fid_cq* cq, void* buf, size_t count) {
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
  struct fi_cq_err_entry err = { 0 };
  fi_cq_readerr(cq, &err, 0);
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
                     "flags %#" PRIx64 ", len %zd, olen %zd",
                     err.flags, err.len, err.olen);
  } else {
    char bufProv[100];
    (void) fi_cq_strerror(cq, err.prov_errno, err.err_data,
                          bufProv, sizeof(bufProv));
    INTERNAL_ERROR_V("fi_cq_read(): err %d, strerror %s",
                     err.err, bufProv);
  }
}


static
void waitForCntrAllTxns(struct perTxCtxInfo_t* tcip) {
  while (getTxCntr(tcip) < tcip->numTxnsBegun) {
    sched_yield();
    ensure_progress();
  }
}


static inline
uint64_t getTxCntr(struct perTxCtxInfo_t* tcip) {
  const uint64_t count = fi_cntr_read(tcip->txCntr);
  if (count > tcip->numTxnsBegun) {
    INTERNAL_ERROR_V("fi_cntr_read() %" PRIu64 ", but numTxnsBegun %" PRIu64,
                     count, tcip->numTxnsBegun);
  }
  return count;
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
}


////////////////////////////////////////
//
// Interface: network atomics
//

static inline void doAMO(c_nodeid_t, void*, const void*, const void*, void*,
                         int, enum fi_datatype, size_t);


//
// WRITE
//
#define DEFN_CHPL_COMM_ATOMIC_WRITE(fnType, ofiType, Type)              \
  void chpl_comm_atomic_write_##fnType                                  \
         (void* desired, c_nodeid_t node, void* object,                 \
          memory_order order, int ln, int32_t fn) {                     \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_write_%s(%p, %d, %p, %d, %s)",         \
               #fnType, desired, (int) node, object,                    \
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
DEFN_CHPL_COMM_ATOMIC_WRITE(real32, FI_FLOAT, float)
DEFN_CHPL_COMM_ATOMIC_WRITE(real64, FI_DOUBLE, double)


//
// READ
//
#define DEFN_CHPL_COMM_ATOMIC_READ(fnType, ofiType, Type)               \
  void chpl_comm_atomic_read_##fnType                                   \
         (void* result, c_nodeid_t node, void* object,                  \
          memory_order order, int ln, int32_t fn) {                     \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_read_%s(%p, %d, %p, %d, %s)",          \
               #fnType, result, (int) node, object,                     \
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
DEFN_CHPL_COMM_ATOMIC_READ(real32, FI_FLOAT, float)
DEFN_CHPL_COMM_ATOMIC_READ(real64, FI_DOUBLE, double)


#define DEFN_CHPL_COMM_ATOMIC_XCHG(fnType, ofiType, Type)               \
  void chpl_comm_atomic_xchg_##fnType                                   \
         (void* desired, c_nodeid_t node, void* object, void* result,   \
          memory_order order, int ln, int32_t fn) {                     \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_xchg_%s(%p, %d, %p, %p, %d, %s)",      \
               #fnType, desired, (int) node, object, result,            \
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
DEFN_CHPL_COMM_ATOMIC_XCHG(real32, FI_FLOAT, float)
DEFN_CHPL_COMM_ATOMIC_XCHG(real64, FI_DOUBLE, double)


#define DEFN_CHPL_COMM_ATOMIC_CMPXCHG(fnType, ofiType, Type)            \
  void chpl_comm_atomic_cmpxchg_##fnType                                \
         (void* expected, void* desired, c_nodeid_t node, void* object, \
          chpl_bool32* result, memory_order order,                      \
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_cmpxchg_%s(%p, %p, %d, %p, %p, "       \
               "%d, %s)",                                               \
               #fnType, expected, desired, (int) node, object, result,  \
               ln, chpl_lookupFilename(fn));                            \
    chpl_comm_diags_verbose_amo("amo cmpxchg", node, ln, fn);           \
    chpl_comm_diags_incr(amo);                                          \
    doAMO(node, object, expected, desired, result,                      \
          FI_CSWAP, ofiType, sizeof(Type));                             \
  }

DEFN_CHPL_COMM_ATOMIC_CMPXCHG(int32, FI_INT32, int32_t)
DEFN_CHPL_COMM_ATOMIC_CMPXCHG(int64, FI_INT64, int64_t)
DEFN_CHPL_COMM_ATOMIC_CMPXCHG(uint32, FI_UINT32, uint32_t)
DEFN_CHPL_COMM_ATOMIC_CMPXCHG(uint64, FI_UINT64, uint64_t)
DEFN_CHPL_COMM_ATOMIC_CMPXCHG(real32, FI_FLOAT, float)
DEFN_CHPL_COMM_ATOMIC_CMPXCHG(real64, FI_DOUBLE, double)


#define DEFN_IFACE_AMO_SIMPLE_OP(fnOp, ofiOp, fnType, ofiType, Type)    \
  void chpl_comm_atomic_##fnOp##_##fnType                               \
         (void* operand, c_nodeid_t node, void* object,                 \
          memory_order order, int ln, int32_t fn) {                     \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_%s_%s(<%s>, %d, %p, %d, %s)",          \
               #fnOp, #fnType, DBG_VAL(operand, ofiType), (int) node,   \
               object, ln, chpl_lookupFilename(fn));                    \
    chpl_comm_diags_verbose_amo("amo " #fnOp, node, ln, fn);            \
    chpl_comm_diags_incr(amo);                                          \
    doAMO(node, object, operand, NULL, NULL,                            \
          ofiOp, ofiType, sizeof(Type));                                \
  }                                                                     \
                                                                        \
  void chpl_comm_atomic_##fnOp##_unordered_##fnType                     \
         (void* operand, c_nodeid_t node, void* object,                 \
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_%s_unordered_%s(<%s>, %d, %p, %d, %s)",\
               #fnOp, #fnType, DBG_VAL(operand, ofiType), (int) node,   \
               object, ln, chpl_lookupFilename(fn));                    \
    chpl_comm_atomic_##fnOp##_##fnType(operand, node, object,           \
                                       memory_order_seq_cst, ln, fn);   \
  }                                                                     \
                                                                        \
  void chpl_comm_atomic_fetch_##fnOp##_##fnType                         \
         (void* operand, c_nodeid_t node, void* object, void* result,   \
          memory_order order, int ln, int32_t fn) {                     \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_fetch_%s_%s(<%s>, %d, %p, %p, "        \
               "%d, %s)",                                               \
               #fnOp, #fnType, DBG_VAL(operand, ofiType), (int) node,   \
               object, result, ln, chpl_lookupFilename(fn));            \
    chpl_comm_diags_verbose_amo("amo fetch_" #fnOp, node, ln, fn);      \
    chpl_comm_diags_incr(amo);                                          \
    doAMO(node, object, operand, NULL, result,                          \
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
DEFN_IFACE_AMO_SIMPLE_OP(add, FI_SUM, real32, FI_FLOAT, float)
DEFN_IFACE_AMO_SIMPLE_OP(add, FI_SUM, real64, FI_DOUBLE, double)


#define DEFN_IFACE_AMO_SUB(fnType, ofiType, Type, negate)               \
  void chpl_comm_atomic_sub_##fnType                                    \
         (void* operand, c_nodeid_t node, void* object,                 \
          memory_order order, int ln, int32_t fn) {                     \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_sub_%s(<%s>, %d, %p, %d, %s)",         \
               #fnType, DBG_VAL(operand, ofiType), (int) node, object,  \
               ln, chpl_lookupFilename(fn));                            \
    Type myOpnd = negate(*(Type*) operand);                             \
    chpl_comm_diags_verbose_amo("amo sub", node, ln, fn);               \
    chpl_comm_diags_incr(amo);                                          \
    doAMO(node, object, &myOpnd, NULL, NULL,                            \
          FI_SUM, ofiType, sizeof(Type));                               \
  }                                                                     \
                                                                        \
  void chpl_comm_atomic_sub_unordered_##fnType                          \
         (void* operand, c_nodeid_t node, void* object,                 \
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_sub_unordered_%s(<%s>, %d, %p, "       \
               "%d, %s)",                                               \
               #fnType, DBG_VAL(operand, ofiType), (int) node, object,  \
               ln, chpl_lookupFilename(fn));                            \
    chpl_comm_atomic_sub_##fnType(operand, node, object,                \
                                  memory_order_seq_cst, ln, fn);        \
  }                                                                     \
                                                                        \
  void chpl_comm_atomic_fetch_sub_##fnType                              \
         (void* operand, c_nodeid_t node, void* object, void* result,   \
          memory_order order, int ln, int32_t fn) {                     \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_fetch_sub_%s(<%s>, %d, %p, %p, "       \
               "%d, %s)",                                               \
               #fnType, DBG_VAL(operand, ofiType), (int) node, object,  \
               result, ln, chpl_lookupFilename(fn));                    \
    Type myOpnd = negate(*(Type*) operand);                             \
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
DEFN_IFACE_AMO_SUB(real32, FI_FLOAT, float, NEGATE_U_OR_R)
DEFN_IFACE_AMO_SUB(real64, FI_DOUBLE, double, NEGATE_U_OR_R)

void chpl_comm_atomic_unordered_task_fence(void) {
  return;
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
    validByType[FI_INT32]  = computeAtomicValid(FI_INT64);
    validByType[FI_UINT64] = computeAtomicValid(FI_UINT64);
    validByType[FI_FLOAT]  = computeAtomicValid(FI_FLOAT);
    validByType[FI_DOUBLE] = computeAtomicValid(FI_DOUBLE);
    inited = true;
  }

  return validByType[ofiType];
}


static inline
void doAMO(c_nodeid_t node, void* object,
           const void* operand1, const void* operand2, void* result,
           int ofiOp, enum fi_datatype ofiType, size_t size) {
  if (chpl_numNodes <= 1) {
    doCpuAMO(object, operand1, operand2, result, ofiOp, ofiType, size);
    return;
  }

  uint64_t mrKey;
  uint64_t mrRaddr;
  if (isAtomicValid(ofiType)
      && mrGetKey(&mrKey, &mrRaddr, node, object, size) == 0) {
    //
    // The type is supported for network atomics and the object address
    // is remotely accessible.  Do the AMO natively.
    //
    struct perTxCtxInfo_t* tcip;
    CHK_TRUE((tcip = tciAlloc()) != NULL);
    ofi_amo(tcip, node, mrRaddr, mrKey, operand1, operand2, result,
            ofiOp, ofiType, size);
    tciFree(tcip);
  } else {
    //
    // We can't do the AMO on the network, so do it on the CPU.  If the
    // object is on this node do it directly; otherwise, use an AM.
    //
    if (node == chpl_nodeID) {
      doCpuAMO(object, operand1, operand2, result, ofiOp, ofiType, size);
    } else {
      amRequestAMO(node, object, operand1, operand2, result,
                   ofiOp, ofiType, size);
    }
  }
}


static inline
void doCpuAMO(void* obj,
              const void* operand1, const void* operand2, void* result,
              enum fi_op ofiOp, enum fi_datatype ofiType, size_t size) {
  CHK_TRUE(size == 4 || size == 8);

  chpl_amo_datum_t* myOpnd1 = (chpl_amo_datum_t*) operand1;
  chpl_amo_datum_t* myOpnd2 = (chpl_amo_datum_t*) operand2;

#define CPU_INT_ARITH_AMO(_o, _t, _m)                                   \
  do {                                                                  \
    if (result == NULL) {                                               \
      (void) atomic_fetch_##_o##_##_t((atomic_##_t*) obj,               \
                                      myOpnd1->_m);                     \
    } else {                                                            \
      *(_t*) result = atomic_fetch_##_o##_##_t((atomic_##_t*) obj,      \
                                               myOpnd1->_m);            \
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
        atomic_store_uint_least32_t(obj, myOpnd1->u32);
      } else {
        atomic_store_uint_least64_t(obj, myOpnd1->u64);
      }
    } else {
      //
      // exchange
      //
      if (size == 4) {
        *(uint32_t*) result = atomic_exchange_uint_least32_t(obj,
                                                             myOpnd1->u32);
      } else {
        *(uint64_t*) result = atomic_exchange_uint_least64_t(obj,
                                                             myOpnd1->u64);
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
      *(chpl_bool32*) result =
        atomic_compare_exchange_strong_uint_least32_t(obj,
                                                      myOpnd1->u32,
                                                      myOpnd2->u32);
    } else {
      *(chpl_bool32*) result =
        atomic_compare_exchange_strong_uint_least64_t(obj,
                                                      myOpnd1->u64,
                                                      myOpnd2->u64);
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
      chpl_amo_datum_t xpctd;
      chpl_amo_datum_t dsrd;
      chpl_bool32 done;

      do {
        xpctd.u32 = atomic_load_int_least32_t(obj);
        dsrd.r32 = xpctd.r32 + myOpnd1->r32;
        done = atomic_compare_exchange_strong_uint_least32_t(obj,
                                                             xpctd.u32,
                                                             dsrd.u32);
      } while (!done);

      if (result != NULL) {
        *(float*) result = xpctd.r32;
      }
    } else if (ofiType == FI_DOUBLE) {
      chpl_amo_datum_t xpctd;
      chpl_amo_datum_t dsrd;
      chpl_bool32 done;

      do {
        xpctd.u64 = atomic_load_int_least64_t(obj);
        dsrd.r64 = xpctd.r64 + myOpnd1->r64;
        done = atomic_compare_exchange_strong_uint_least64_t(obj,
                                                             xpctd.u64,
                                                             dsrd.u64);
      } while (!done);

      if (result != NULL) {
        *(double*) result = xpctd.r64;
      }
    } else {
      INTERNAL_ERROR_V("doCpuAMO(): unsupported ofiOp %d, ofiType %d",
                       ofiOp, ofiType);
    }
    break;

#if 0
    //
    // This was lifted from comm=ugni.  I'm not going to ever use it
    // directly.  But I'm leaving it here to remind myself to cover the
    // case where the object is in registered memory, when I add support
    // for AMOs done directly in the fabric.
    //

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
                    *(int_least64_t*) operand1,
                    *(int_least64_t*) operand2);
      }
      else {
        int_least64_t nic_res;
        do_nic_amo(operand1, operand2, chpl_nodeID, obj, sizeof(nic_res),
                   amo_cmd_2_nic_op(cmpxchg_64, 1), &nic_res, mr);
        my_res = (nic_res == *(int_least64_t*) operand1) ? true : false;
      }
      memcpy(result, &my_res, sizeof(my_res));
    }
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
        *(float*) &desired  = *(float*) &expected + *(float*) operand1;
        done = atomic_compare_exchange_strong_int_least32_t
                 ((atomic_int_least32_t*) obj, expected, desired);
      } while (!done);

      if (result != NULL) {
        memcpy(result, &expected, sizeof(expected));
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
      mem_region_t* mr;

      if ((mr = mreg_for_local_addr(obj)) == NULL) {
        chpl_bool32 done;

        do {
          expected =
            atomic_load_int_least64_t((atomic_int_least64_t*) obj);
          *(double*) &desired = *(double*) &expected + *(double*) operand1;
          done = atomic_compare_exchange_strong_int_least64_t
                   ((atomic_int_least64_t*) obj, expected, desired);
        } while (!done);
      }
      else {
        int_least64_t nic_res;

        do {
          do_remote_get(&expected, chpl_nodeID, obj, sizeof(expected),
                        may_proxy_false);
          *(double*) &desired = *(double*) &expected + *(double*) operand1;
          do_nic_amo(&expected, &desired, chpl_nodeID, obj,
                     sizeof(nic_res), amo_cmd_2_nic_op(cmpxchg_64, 1),
                     &nic_res, mr);
        } while (nic_res != expected);
      }

      if (result != NULL) {
        memcpy(result, &expected, sizeof(expected));
      }
    }
    break;
#endif

  default:
    INTERNAL_ERROR_V("doCpuAMO(): unsupported ofiOp %d, ofiType %d",
                     ofiOp, ofiType);
  }

  if (DBG_TEST_MASK(DBG_AMO)) {
    if (result == NULL) {
      DBG_PRINTF(DBG_AMO,
                 "doCpuAMO(%p, %s, %s, %s): now %s",
                 obj, amo_opName(ofiOp), amo_typeName(ofiType),
                 DBG_VAL(myOpnd1, ofiType),
                 DBG_VAL((chpl_amo_datum_t*) obj, ofiType));
    } else if (ofiOp == FI_ATOMIC_READ) {
      DBG_PRINTF(DBG_AMO,
                 "doCpuAMO(%p, %s, %s): res %p is %s",
                 obj, amo_opName(ofiOp), amo_typeName(ofiType), result,
                 DBG_VAL(result, ofiType));
    } else {
      DBG_PRINTF(DBG_AMO,
                 "doCpuAMO(%p, %s, %s, %s, %s): now %s, res %p is %s",
                 obj, amo_opName(ofiOp), amo_typeName(ofiType),
                 DBG_VAL(myOpnd1, ofiType),
                 DBG_VAL(myOpnd2, ofiType),
                 DBG_VAL((chpl_amo_datum_t*) obj, ofiType), result,
                 DBG_VAL(result, (ofiOp == FI_CSWAP) ? FI_INT32 : ofiType));
    }
  }

#undef CPU_INT_ARITH_AMO
}


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


void chpl_comm_barrier(const char *msg) {
#ifdef CHPL_COMM_DEBUG
  chpl_msg(2, "%d: enter barrier for '%s'\n", chpl_nodeID, msg);
#endif

  if (chpl_numNodes == 1) {
    return;
  }

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
  chpl_comm_ofi_dbg_level = chpl_env_rt_get_int("COMM_OFI_DEBUG", 0);

  const char* ev;
  if ((ev = chpl_env_rt_get("COMM_OFI_DEBUG_FNAME", NULL)) == NULL) {
    chpl_comm_ofi_dbg_file = stdout;
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
  case am_opNil: return "opNil";
  case am_opExecOn: return "opExecOn";
  case am_opExecOnLrg: return "opExecOnLrg";
  case am_opGet: return "opGet";
  case am_opPut: return "opPut";
  case am_opAMO: return "opAMO";
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
  case FI_INT32: return "int32";
  case FI_UINT32: return "uint32";
  case FI_INT64: return "int64";
  case FI_UINT64: return "uint64";
  case FI_FLOAT: return "float";
  case FI_DOUBLE: return "double";
  default: return "amoType???";
  }
}

#endif
