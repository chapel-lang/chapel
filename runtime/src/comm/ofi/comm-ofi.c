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
// OFI-based implementation of Chapel communication interface.
//

#include "chplrt.h"
#include "chpl-env-gen.h"

// #include "chpl-cache.h"
#include "chpl-comm.h"
#include "chpl-comm-callbacks.h"
#include "chpl-comm-callbacks-internal.h"
#include "chpl-comm-diags.h"
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

#include <rdma/fabric.h>
#include <rdma/fi_atomic.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_rma.h>


////////////////////////////////////////
//
// Global types and data
//

static struct fi_info* ofi_info;        // fabric interface info
static struct fid_fabric* ofi_fabric;   // fabric domain
static struct fid_domain* ofi_domain;   // fabric access domain
static struct fid_ep* ofi_txEp;         // scalable transmit endpoint
static struct fid_ep* ofi_rxEp;         // AM req receive endpoint
static struct fid_cq* ofi_rxCQ;         // receive endpoint CQ
static struct fid_av* ofi_av;           // address vector, table style
static fi_addr_t* ofi_rxAddrs;          // remote receive addrs

static int txCQSize;                    // txCQ size

struct perTxCtxInfo_t {
  atomic_bool allocated;
  chpl_bool bound;
  chpl_bool txCtxHasCQ;
  struct fid_ep* txCtx;
  struct fid_cq* txCQ;
  struct fid_cntr* txCntr;
  int numAmReqsTxed;
  int numAmReqsRxed;
  int numReadsTxed;
  int numWritesTxed;
  int numTxsOut;
};

static int tciTabLen;
static struct perTxCtxInfo_t* tciTab;
static chpl_bool tciTabFixedAssignments;

#define MAX_MEM_REGIONS 10
static int numMemRegions = 0;

static struct fid_mr* ofiMrTab[MAX_MEM_REGIONS];

struct memEntry {
  void* addr;
  size_t size;
  void* desc;
  uint64_t key;
};

typedef struct memEntry (memTab_t)[MAX_MEM_REGIONS];

static memTab_t memTab;
static memTab_t* memTabMap;

static int numAmHandlers = 1;

static void* amLZs;
static struct iovec ofi_iov_reqs;
static struct fi_msg ofi_msg_reqs;


////////////////////////////////////////
//
// Forward decls
//

static inline struct perTxCtxInfo_t* tciAlloc(chpl_bool);
static inline void tciFree(struct perTxCtxInfo_t*);
static /*inline*/ chpl_comm_nb_handle_t ofi_put(const void*, c_nodeid_t,
                                                void*, size_t);
static /*inline*/ chpl_comm_nb_handle_t ofi_get(void*, c_nodeid_t,
                                                void*, size_t);
static void waitForTxCQ(struct perTxCtxInfo_t*, int);
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

#define OFI_CHK_VAL(expr, wantVal)                                      \
    do {                                                                \
      int _rc = (expr);                                                 \
      if (_rc != wantVal) {                                             \
        INTERNAL_ERROR_V("%s: %s", #expr, fi_strerror(- _rc));          \
      }                                                                 \
    } while (0)

#define OFI_CHK(expr) OFI_CHK_VAL(expr, FI_SUCCESS)

#define PTHREAD_CHK(expr) CHK_EQ_TYPED(expr, 0, int, "d")


////////////////////////////////////////
//
// Provider name
//

static pthread_once_t provNameOnce = PTHREAD_ONCE_INIT;
static const char* provName;

static void setProviderName(void);
static const char* getProviderName(void);


static
void setProviderName(void) {
  //
  // For now, allow specifying the provider via env var.
  //
  CHK_TRUE((provName = chpl_env_rt_get("COMM_OFI_PROVIDER", "sockets"))
           != NULL);
}


static
const char* getProviderName(void) {
  PTHREAD_CHK(pthread_once(&provNameOnce, setProviderName));
  return provName;
}


////////////////////////////////////////
//
// Interface: initialization
//

pthread_t pthread_that_inited;

static void init_ofi(void);
static void init_ofiFabricDomain(void);
static void init_ofiEp(void);
static void init_ofiEpNumCtxs(void);
static void init_ofiExchangeAvInfo(void);
static void init_ofiForMem(void);
static void init_ofiForRma(void);
static void init_ofiForAms(void);

static void init_bar(void);


void chpl_comm_init(int *argc_p, char ***argv_p) {
  chpl_comm_ofi_abort_on_error =
    (chpl_env_rt_get("COMM_OFI_ABORT_ON_ERROR", NULL) != NULL);
  time_init();
  chpl_comm_ofi_oob_init();
  DBG_INIT();

  pthread_that_inited = pthread_self();
}


void chpl_comm_post_mem_init(void) { }


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

  // chpl_cache_init();
}


static
void init_ofi(void) {
  init_ofiFabricDomain();
  init_ofiEp();
  init_ofiExchangeAvInfo();
  init_ofiForMem();
  init_ofiForRma();
  init_ofiForAms();
}


static
void init_ofiFabricDomain(void) {
  //
  // Build hints describing what we want from the provider.
  //
  const char* provider = getProviderName();

  struct fi_info* hints;
  CHK_TRUE((hints = fi_allocinfo()) != NULL);

  hints->caps = FI_MSG | FI_SEND | FI_RECV | FI_MULTI_RECV
                | FI_RMA | FI_READ | FI_WRITE
                | FI_ATOMICS
                | FI_REMOTE_READ | FI_REMOTE_WRITE;

  hints->mode = 0; // TODO: may need ~0 here and handle modes for good gni perf

  hints->addr_format = FI_FORMAT_UNSPEC;

  hints->ep_attr->type = FI_EP_RDM;

  hints->domain_attr->threading = FI_THREAD_UNSPEC;
  hints->domain_attr->control_progress = FI_PROGRESS_MANUAL/*FI_PROGRESS_AUTO*/;
  hints->domain_attr->data_progress = FI_PROGRESS_MANUAL/*FI_PROGRESS_AUTO*/;
  hints->domain_attr->av_type = FI_AV_TABLE;
  hints->domain_attr->mr_mode = ((strcmp(provider, "gni") == 0)
                                 ? FI_MR_BASIC
                                 : FI_MR_SCALABLE);
  hints->domain_attr->resource_mgmt = FI_RM_ENABLED;

  // fi_freeinfo(hints) will free() hints->fabric_attr->prov_name; this
  // is documented, though poorly.  So, get that space from malloc().
  CHK_SYS_CALLOC(hints->fabric_attr->prov_name, strlen(provider) + 1);
  strcpy(hints->fabric_attr->prov_name, provider);

  //
  // Try to find a provider that can do what we want.  If more than one
  // is found, presume that ones earlier in the list perform better (as
  // documented in 'man fi_getinfo').  We just do error reporting on
  // node 0; the other nodes should all have the same result and there's
  // no point in repeating everything numNodes times.
  //
  int ret;
  ret = fi_getinfo(FI_VERSION(1,5), NULL, NULL, 0, hints, &ofi_info);
  if (chpl_nodeID == 0) {
    if (ret == -FI_ENODATA) {
      if (DBG_TEST_MASK(DBG_CFGFAB | DBG_CFGFABSALL)) {
        DBG_PRINTF(DBG_CFGFAB, "==================== hints:");
        DBG_PRINTF(DBG_CFGFAB, "%s", fi_tostr(hints, FI_TYPE_INFO));
        DBG_PRINTF(DBG_CFGFABSALL,
                   "==================== fi_getinfo() fabrics:");
        DBG_PRINTF(DBG_CFGFABSALL,
                   "None matched hints; available with prov_name \"%s\" are:",
                   (provider == NULL) ? "<any>" : provider);
        struct fi_info* info = NULL;
        OFI_CHK(fi_getinfo(FI_VERSION(1,5), NULL, NULL, 0, NULL, &info));
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

    if (DBG_TEST_MASK(DBG_CFGFAB | DBG_CFGFABSALL)) {
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
    }
  }

  OFI_CHK(ret);

  fi_freeinfo(hints);

  //
  // Create the fabric domain and associated fabric access domain.
  //
  OFI_CHK(fi_fabric(ofi_info->fabric_attr, &ofi_fabric, NULL));
  OFI_CHK(fi_domain(ofi_fabric, ofi_info, &ofi_domain, NULL));

  //
  // Create the address vector covering the nodes.
  //
  struct fi_av_attr ofi_avAttr = { 0 };
  ofi_avAttr.type = FI_AV_TABLE;
  ofi_avAttr.count = chpl_numNodes;
  ofi_avAttr.name = NULL;
  ofi_avAttr.rx_ctx_bits = 0;

  OFI_CHK(fi_av_open(ofi_domain, &ofi_avAttr, &ofi_av, NULL));
}


static
void init_ofiEp(void) {
  //
  // Compute numbers of transmit and receive contexts, and then create
  // the transmit context table.
  //
  init_ofiEpNumCtxs();

  tciTabLen = ofi_info->ep_attr->tx_ctx_cnt;
  CHPL_CALLOC(tciTab, tciTabLen);

  //
  // Create transmit contexts.
  //
  // For the CQ lengths, allow for whichever maxOutstanding (AMs or
  // RMAs) value is larger, plus quite a few for AM responses because
  // the network round-trip latency ought to be quite a bit more than
  // our AM handling time, so we want to be able to have many responses
  // in flight at once.
  //
  OFI_CHK(fi_scalable_ep(ofi_domain, ofi_info, &ofi_txEp, NULL));
  OFI_CHK(fi_scalable_ep_bind(ofi_txEp, &ofi_av->fid, 0));

  txCQSize = 100;  // TODO

  struct fi_cq_attr txCqAttr = { 0 };
  txCqAttr.format = FI_CQ_FORMAT_CONTEXT;
  txCqAttr.size = txCQSize;
  txCqAttr.wait_obj = FI_WAIT_NONE;

  struct fi_cntr_attr txCntrAttr = { 0 };
  txCntrAttr.events = FI_CNTR_EVENTS_COMP;
  txCntrAttr.wait_obj = FI_WAIT_NONE;

  //
  // Worker TX contexts need completion queues.  Those for AM handlers
  // can just use counters.
  //
  const int numWorkerTxCtxs = tciTabLen - numAmHandlers;
  for (int i = 0; i < numWorkerTxCtxs; i++) {
    atomic_init_bool(&tciTab[i].allocated, false);
    tciTab[i].bound = false;
    OFI_CHK(fi_tx_context(ofi_txEp, i, NULL, &tciTab[i].txCtx, NULL));
    OFI_CHK(fi_cq_open(ofi_domain, &txCqAttr, &tciTab[i].txCQ, NULL));
    OFI_CHK(fi_ep_bind(tciTab[i].txCtx, &tciTab[i].txCQ->fid, FI_TRANSMIT));
    tciTab[i].txCtxHasCQ = true;
    OFI_CHK(fi_enable(tciTab[i].txCtx));
  }

  for (int i = numWorkerTxCtxs; i < tciTabLen; i++) {
    atomic_init_bool(&tciTab[i].allocated, false);
    tciTab[i].bound = false;
    OFI_CHK(fi_tx_context(ofi_txEp, i, NULL, &tciTab[i].txCtx, NULL));
    OFI_CHK(fi_cntr_open(ofi_domain, &txCntrAttr, &tciTab[i].txCntr, NULL));
    OFI_CHK(fi_ep_bind(tciTab[i].txCtx, &tciTab[i].txCntr->fid, FI_WRITE));
    tciTab[i].txCtxHasCQ = false;
    OFI_CHK(fi_enable(tciTab[i].txCtx));
  }

  //
  // Create receive contexts.
  //
  // For the CQ length, allow for an appreciable proportion of the job
  // to send requests to us at once.
  //
  struct fi_cq_attr rxCqAttr = { 0 };
  rxCqAttr.format = FI_CQ_FORMAT_DATA;
  rxCqAttr.size = chpl_numNodes * numWorkerTxCtxs;
  rxCqAttr.wait_obj = FI_WAIT_NONE;

  OFI_CHK(fi_endpoint(ofi_domain, ofi_info, &ofi_rxEp, NULL));
  OFI_CHK(fi_ep_bind(ofi_rxEp, &ofi_av->fid, 0));
  OFI_CHK(fi_cq_open(ofi_domain, &rxCqAttr, &ofi_rxCQ, NULL));
  OFI_CHK(fi_ep_bind(ofi_rxEp, &ofi_rxCQ->fid, FI_RECV));
  OFI_CHK(fi_enable(ofi_rxEp));
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
  int numWorkerTxCtxs = dom_attr->max_ep_tx_ctx - numAmHandlers;
  CHK_TRUE(numWorkerTxCtxs > 0);

  //
  // If the user manually limited the communication concurrency, take
  // that into account.
  //
  const int commConcurrency = chpl_env_rt_get_int("COMM_CONCURRENCY", 0);
  if (commConcurrency > 0) {
    if (numWorkerTxCtxs > commConcurrency) {
      numWorkerTxCtxs = commConcurrency;
    }
  } else if (commConcurrency < 0) {
    chpl_warning("CHPL_RT_COMM_CONCURRENCY < 0, ignored", 0, 0);
  }

  const int fixedNumThreads = chpl_task_getFixedNumThreads();
  if (fixedNumThreads > 0) {
    //
    // The tasking layer uses a fixed number of threads.  If we can
    // have at least that many worker tx contexts then each tasking
    // layer thread can have a private one for the duration of the
    // run.
    //
    CHK_TRUE(fixedNumThreads == chpl_task_getMaxPar()); // sanity
    if (numWorkerTxCtxs > fixedNumThreads)
      numWorkerTxCtxs = fixedNumThreads;
    tciTabFixedAssignments = (numWorkerTxCtxs == fixedNumThreads);
  } else {
    //
    // The tasking layer doesn't have a fixed number of threads, but
    // it still must have a maximum useful level of parallelism.  We
    // shouldn't need more worker tx contexts than whatever that is.
    //
    const int taskMaxPar = chpl_task_getMaxPar();
    if (numWorkerTxCtxs > taskMaxPar)
      numWorkerTxCtxs = taskMaxPar;

    tciTabFixedAssignments = false;
  }

  //
  // Now we know how many transmit contexts we'll have.
  //
  ofi_info->ep_attr->tx_ctx_cnt = numWorkerTxCtxs + numAmHandlers;

  //
  // Receive contexts are much easier -- we just need one
  // for each AM handler.
  //
  CHK_TRUE(dom_attr->max_ep_rx_ctx >= numAmHandlers);
  ofi_info->ep_attr->rx_ctx_cnt = numAmHandlers;

  DBG_PRINTF(DBG_CFG, "per node, %zd tx ctxs%s, %zd rx ctxs",
             ofi_info->ep_attr->tx_ctx_cnt,
             tciTabFixedAssignments ? " (fixed to workers)" : "",
             ofi_info->ep_attr->rx_ctx_cnt);
}


static
void init_ofiExchangeAvInfo(void) {
  //
  // Exchange addresses with the rest of the nodes.
  //
  void* my_addr;
  void* addrs;
  size_t my_addr_len = 0;

  //
  // Get everybody else's address.
  // Note: this assumes my_addr_len is the same on all nodes.
  //
  CHK_TRUE(fi_getname(&ofi_rxEp->fid, NULL, &my_addr_len) == -FI_ETOOSMALL);
  CHPL_CALLOC_SZ(my_addr, my_addr_len, 1);
  OFI_CHK(fi_getname(&ofi_rxEp->fid, my_addr, &my_addr_len));
  CHPL_CALLOC_SZ(addrs, chpl_numNodes, my_addr_len);
  if (DBG_TEST_MASK(DBG_CFGAV)) {
    char nameBuf[128];
    size_t nameLen;
    nameLen = sizeof(nameBuf);
    (void) fi_av_straddr(ofi_av, my_addr, nameBuf, &nameLen);
    DBG_PRINTF(DBG_CFGAV, "my_addr: %.*s%s",
               (int) nameLen, nameBuf,
               (nameLen <= sizeof(nameBuf)) ? "" : "[...]");
  }
  chpl_comm_ofi_oob_allgather(my_addr, addrs, my_addr_len);

  //
  // Insert the addresses into the address vectors and build up a vector
  // of remote receive endpoints.
  //
  CHPL_CALLOC(ofi_rxAddrs, chpl_numNodes);
  CHK_TRUE(fi_av_insert(ofi_av, addrs, chpl_numNodes, ofi_rxAddrs, 0, NULL)
           == chpl_numNodes);

  CHPL_FREE(my_addr);
  CHPL_FREE(addrs);

  if (chpl_nodeID == 0 && DBG_TEST_MASK(DBG_CFGAV)) {
    DBG_PRINTF(DBG_CFGAV, "====================");
    DBG_PRINTF(DBG_CFGAV, "Address vector");
    char addrBuf[my_addr_len + 1];
    size_t addrLen;
    char nameBuf[128];
    size_t nameLen;
    for (int i = 0; i < chpl_numNodes; i++) {
      addrLen = sizeof(addrBuf);
      OFI_CHK(fi_av_lookup(ofi_av, i, addrBuf, &addrLen));
      CHK_TRUE(addrLen <= sizeof(addrBuf));
      nameLen = sizeof(nameBuf);
      (void) fi_av_straddr(ofi_av, addrBuf, nameBuf, &nameLen);
      DBG_PRINTF(DBG_CFGAV, "addrVec[%d]: %.*s%s",
                 i, (int) nameLen, nameBuf,
                 (nameLen <= sizeof(nameBuf)) ? "" : "[...]");
    }
    DBG_PRINTF(DBG_CFGAV, "====================");
  }
}


static
void init_ofiForMem(void) {
  //
  // With scalable memory registration we just register the whole
  // address space here; with non-scalable we register each region
  // individually.
  //
  uint64_t bufAcc = FI_READ | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE;

  if ((ofi_info->domain_attr->mr_mode & FI_MR_BASIC) == 0) {
    // scalable MR -- just one memory region, the whole address space
    numMemRegions = 1;
    memTab[0].addr = (void*) 0;
    memTab[0].size = SIZE_MAX;
  } else {
    void* fixedHeapStart;
    size_t fixedHeapSize;
    chpl_comm_impl_regMemHeapInfo(&fixedHeapStart, &fixedHeapSize);
    if (fixedHeapStart != NULL) {
      numMemRegions = 1;
      memTab[0].addr = fixedHeapStart;
      memTab[0].size = fixedHeapSize;
    }
  }

  for (int i = 0; i < numMemRegions; i++) {
    OFI_CHK(fi_mr_reg(ofi_domain,
                      memTab[i].addr, memTab[i].size,
                      bufAcc, 0, 0, 0, &ofiMrTab[i], NULL));
    memTab[i].desc = fi_mr_desc(ofiMrTab[i]);
    memTab[i].key  = fi_mr_key(ofiMrTab[i]);
    DBG_PRINTF(DBG_MR,
               "[%d] fi_mr_reg(%p, %#zx): key %#" PRIx64,
               i, memTab[i].addr, memTab[i].size,
               memTab[i].key);
  }

  //
  // Share the memory regions around the job.
  //
  CHPL_CALLOC(memTabMap, chpl_numNodes);
  chpl_comm_ofi_oob_allgather(&memTab, memTabMap, sizeof(memTabMap[0]));
}


static
void init_ofiForRma(void) {
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
  // case.
  //
  {
    const size_t sz = 2048; // TODO
    const int ret = fi_setopt(&ofi_rxEp->fid, FI_OPT_ENDPOINT,
                              FI_OPT_MIN_MULTI_RECV, &sz, sizeof(sz));
    CHK_TRUE(ret == FI_SUCCESS || ret == -FI_ENOSYS);
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
  DBG_PRINTF(DBG_AM | DBG_AMRECV, "pre-post fi_recvmsg(AMReqs)");

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


void chpl_comm_broadcast_global_vars(int numGlobals) {
  // TODO: this won't work in the presence of address space randomization
  int i;
  if (chpl_nodeID != 0) {
    for (i = 0; i < numGlobals; i++) {
      chpl_comm_get(chpl_globals_registry[i], 0, chpl_globals_registry[i],
                    sizeof(wide_ptr_t), -1 /*typeIndex: unused*/,
                    CHPL_COMM_UNKNOWN_ID, 0, 0);
    }
  }
}


void chpl_comm_broadcast_private(int id, size_t size, int32_t tid) {
  // TODO: this won't work in the presence of address space randomization
  int i;
  for (i = 0; i < chpl_numNodes; i++) {
    if (i != chpl_nodeID) {
      chpl_comm_put(chpl_private_broadcast_table[id], i,
                    chpl_private_broadcast_table[id], size,
                    -1 /*typeIndex: unused*/, CHPL_COMM_UNKNOWN_ID, 0, 0);
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


void chpl_comm_pre_task_exit(int all) {
  if (all) {
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
  // TODO
}


static
void fini_ofi(void) {
  if (chpl_numNodes <= 1)
    return;

  OFI_CHK(fi_close(&ofiMrTab[0]->fid));
  if ((ofi_info->domain_attr->mr_mode & FI_MR_BASIC) != 0) {
    for (int i = 1; i < numMemRegions; i++) {
      OFI_CHK(fi_close(&ofiMrTab[i]->fid));
    }
  }

  CHPL_FREE(memTabMap);

  CHPL_FREE(memTabMap);

  CHPL_FREE(amLZs);

  CHPL_FREE(ofi_rxAddrs);

  OFI_CHK(fi_close(&ofi_rxEp->fid));
  OFI_CHK(fi_close(&ofi_rxCQ->fid));

  for (int i = 0; i < tciTabLen; i++) {
    OFI_CHK(fi_close(&tciTab[i].txCtx->fid));
    OFI_CHK(fi_close(tciTab[i].txCtxHasCQ
                     ? &tciTab[i].txCQ->fid
                     : &tciTab[i].txCntr->fid));
  }

  OFI_CHK(fi_close(&ofi_txEp->fid));
  OFI_CHK(fi_close(&ofi_av->fid));
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
  PTHREAD_CHK(pthread_once(&fixedHeapOnce, init_fixedHeap));
  *start_p = fixedHeapStart;
  *size_p  = fixedHeapSize;
}


static
void init_fixedHeap(void) {
  //
  // We only need a fixed heap if we're multinode and using the
  // gni provider.
  //
  if (chpl_numNodes <= 1 || strcmp(getProviderName(), "gni") != 0)
    return;

  //
  // On XE systems you have to use hugepages, and on XC systems you
  // really ought to.
  //
  // TODO: differentiate and do the right thing for XE.
  //
  size_t page_size;
  chpl_bool have_hugepages;
  size_t size;
  void* start;

  if ((page_size = get_hugepageSize()) == 0) {
    chpl_warning_explicit("not using hugepages may reduce performance",
                          __LINE__, __FILE__);
    page_size = chpl_getSysPageSize();
    have_hugepages = false;
  } else {
    have_hugepages = true;
  }

  if ((size = chpl_comm_getenvMaxHeapSize()) == 0) {
    size = (size_t) 16 << 30;  // TODO: different for XE?
  }

  size = ALIGN_UP(size, page_size);


  //
  // The heap is supposed to be of fixed size and on hugepages.  Set
  // it up.
  //
  size_t nic_mem_map_limit;
  size_t nic_max_mem;
  size_t max_heap_size;

  //
  // Considering the data size we'll register, compute the maximum
  // heap size that will allow all registrations to fit in the NIC
  // TLB.  Except on Gemini only, aim for only 95% of what will fit
  // because there we'll get an error if we go over.
  //
#if 0 // TODO: assume Aries; allows deeper testing
  if (nic_type == GNI_DEVICE_GEMINI) {
    const size_t nic_max_pages = (size_t) 1 << 14; // not publicly defined
    nic_max_mem = nic_max_pages * page_size;
    nic_mem_map_limit = ALIGN_DN((size_t) (0.95 * nic_max_mem), page_size);
  } else {
#endif
    const size_t nic_TLB_cache_pages = 512; // not publicly defined
    nic_max_mem = nic_TLB_cache_pages * page_size;
    nic_mem_map_limit = nic_max_mem;
#if 0 // TODO: assume Aries; allows deeper testing
  }
#endif

#if 0 // TODO: assume Aries; allows deeper testing
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
#else
  max_heap_size = nic_mem_map_limit;
#endif

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

#if 0 // TODO: assume Aries; allows deeper testing
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
#endif

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

  //
  // On Aries-based systems, warn if the size is larger than what will
  // fit in the TLB cache.  But since that may reduce performance but
  // won't affect function, don't reduce the size to fit.
  //
  if (/*nic_type == GNI_DEVICE_ARIES &&*/ size > max_heap_size) { // TODO: assume Aries; allows deeper testing
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

  fixedHeapSize  = size;
  fixedHeapStart = start;
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
int mrGetDesc(void** pDesc, int iNode, void* addr, size_t size) {
  if ((ofi_info->domain_attr->mr_mode & (FI_MR_BASIC | FI_MR_LOCAL)) == 0) {
    DBG_PRINTF(DBG_MRDESC, "mrGet%sDesc(%p, %zd): scalable",
               (iNode == -1) ? "Local" : "", addr, size);
    if (pDesc != NULL)
      *pDesc = NULL;
    return 0;
  }

  memTab_t* tab = (iNode == -1) ? &memTab : &memTabMap[iNode];
  struct memEntry* mr;
  if ((mr = getMemEntry(tab, addr, size)) == NULL) {
    DBG_PRINTF(DBG_MRDESC, "mrGet%sDesc(%p, %zd): no entry",
               (iNode == -1) ? "Local" : "", addr, size);
    return -1;
  }
  DBG_PRINTF(DBG_MRDESC, "mrGet%sDesc(%p, %zd): desc %p",
             (iNode == -1) ? "Local" : "", addr, size, mr->desc);
  if (pDesc != NULL)
    *pDesc = mr->desc;
  return 0;
}


static inline
int mrGetLocalDesc(void** pDesc, void* addr, size_t size) {
  return mrGetDesc(pDesc, -1, addr, size);
}


static inline
int mrGetKey(uint64_t* pKey, int iNode, void* addr, size_t size) {
  struct memEntry* mr;
  if ((mr = getMemEntry(&memTabMap[iNode], addr, size)) == NULL) {
    DBG_PRINTF(DBG_MRKEY, "mrGetKey(%d:%p, %zd): no entry",
               iNode, addr, size);
    return -1;
  }
  DBG_PRINTF(DBG_MRKEY, "mrGetKey(%d:%p, %zd): key %" PRIx64,
             iNode, addr, size, mr->key);
  if (pKey != NULL)
    *pKey = mr->key;
  return 0;
}


static inline
int mrGetLocalKey(uint64_t* pKey, void* addr, size_t size) {
  if (chpl_numNodes <= 1) {
    //
    // It okay that we don't have a local key in this case, but
    // you'd better not want its value.
    //
    CHK_TRUE(pKey == NULL);
    return 0;
  }
  return mrGetKey(pKey, chpl_nodeID, addr, size);
}


////////////////////////////////////////
//
// Interface: Active Messages
//


typedef enum {
  am_opNil = 0,                         // no-op
  am_opCall,                            // call a function table function
  am_opGet,                             // do an RMA GET
  am_opPut,                             // do an RMA PUT
  am_opAMO,                             // do an AMO
} amOp_t;

static void amRequestExecOn(c_nodeid_t, c_sublocid_t, chpl_fn_int_t,
                            chpl_comm_on_bundle_t*, size_t,
                            chpl_bool, chpl_bool);
static void amRequestRMA(c_nodeid_t, amOp_t, void*, void*, size_t);
static void amRequestAMO(c_nodeid_t, void*, const void*, const void*, void*,
                         int, enum fi_datatype, size_t);
static void amRequestCommon(c_nodeid_t, chpl_comm_on_bundle_t*, size_t,
                            chpl_comm_amDone_t**);


int chpl_comm_numPollingTasks(void) {
  return 1;
}


void chpl_comm_make_progress(void) {
}


void chpl_comm_execute_on(c_nodeid_t node, c_sublocid_t subloc,
                          chpl_fn_int_t fid,
                          chpl_comm_on_bundle_t *arg, size_t argSize) {
  CHK_TRUE(node != chpl_nodeID); // handled by the locale model

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_executeOn, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, argSize}};
    chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_printf("remote task created on %d", (int) node);
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
                             chpl_comm_on_bundle_t *arg, size_t argSize) {
  CHK_TRUE(node != chpl_nodeID); // handled by the locale model

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_nb)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_executeOn_nb, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, argSize}};
    chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_printf("remote non-blocking task created on %d",
                                 (int) node);
  chpl_comm_diags_incr(execute_on_nb);

  amRequestExecOn(node, subloc, fid, arg, argSize, false, false);
}


void chpl_comm_execute_on_fast(c_nodeid_t node, c_sublocid_t subloc,
                               chpl_fn_int_t fid,
                               chpl_comm_on_bundle_t *arg, size_t argSize) {
  CHK_TRUE(node != chpl_nodeID); // handled by the locale model

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_fast)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_executeOn_fast, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, argSize}};
    chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_printf("remote (no-fork) task created on %d",
                                 (int) node);
  chpl_comm_diags_incr(execute_on_fast);

  amRequestExecOn(node, subloc, fid, arg, argSize, true, true);
}


static inline
void amRequestExecOn(c_nodeid_t node, c_sublocid_t subloc,
                     chpl_fn_int_t fid,
                     chpl_comm_on_bundle_t* arg, size_t argSize,
                     chpl_bool fast, chpl_bool blocking) {
  arg->comm.xo = (struct chpl_comm_bundleData_execOn_t)
                   { .op = am_opCall,
                     .fast = false,
                     .fid = fid,
                     .argSize = argSize,
                     .node = chpl_nodeID,
                     .subloc = subloc,
                     .pDone = NULL };
  amRequestCommon(node, arg, argSize, blocking ? &arg->comm.xo.pDone : NULL);
}


static inline
void amRequestRMA(c_nodeid_t node, amOp_t op,
                  void* addr, void* raddr, size_t size) {
  chpl_comm_on_bundle_t arg;
  arg.comm.rma = (struct chpl_comm_bundleData_RMA_t)
                   { .op = op,
                     .addr = raddr,
                     .node = chpl_nodeID,
                     .raddr = addr,
                     .size = size,
                     .pDone = NULL };
  amRequestCommon(node, &arg,
                  (offsetof(chpl_comm_on_bundle_t, comm)
                   + sizeof(arg.comm.rma)),
                  &arg.comm.rma.pDone);
}


static inline
void amRequestAMO(c_nodeid_t node, void* object,
                  const void* operand1, const void* operand2, void* result,
                  int ofiOp, enum fi_datatype ofiType, size_t size) {
  DBG_PRINTF(DBG_AMO,
             "AMO via AM: obj %d:%p, opnd1 <%s>, opnd2 <%s>, res %p, "
             "op %d, typ %d, sz %zd",
             (int) node, object,
             DBG_VAL(operand1, ofiType), DBG_VAL(operand2, ofiType), result,
             ofiOp, ofiType, size);
  void* myResult = result;
  size_t resSize = (ofiOp == FI_CSWAP) ? sizeof(chpl_bool32) : size;
  if (myResult != NULL) {
    if (mrGetLocalDesc(NULL, myResult, resSize) != 0) {
      myResult = allocBounceBuf(resSize);
      DBG_PRINTF(DBG_AMO, "AMO result BB: %p", myResult);
      CHK_TRUE(mrGetLocalDesc(NULL, myResult, resSize) == 0);
    }
  }

  chpl_comm_on_bundle_t arg;
  arg.comm.amo = (struct chpl_comm_bundleData_AMO_t)
                   { .op = am_opAMO,
                     .ofiOp = ofiOp,
                     .ofiType = ofiType,
                     .size = size,
                     .node = chpl_nodeID,
                     .obj = object,
                     .operand1 = { 0 },
                     .operand2 = { 0 },
                     .result = myResult,
                     .pDone = NULL };
  if (operand1 != NULL) {
    memcpy(&arg.comm.amo.operand1, operand1, size);
  }
  if (operand2 != NULL) {
    memcpy(&arg.comm.amo.operand2, operand2, size);
  }
  amRequestCommon(node, &arg,
                  (offsetof(chpl_comm_on_bundle_t, comm)
                   + sizeof(arg.comm.amo)),
                  &arg.comm.amo.pDone);
  if (myResult != result) {
    memcpy(result, myResult, resSize);
    freeBounceBuf(myResult);
  }
}


static inline
void amRequestCommon(c_nodeid_t node,
                     chpl_comm_on_bundle_t* arg, size_t argSize,
                     chpl_comm_amDone_t** ppDone) {
  //
  // If blocking, make sure target can RMA PUT the indicator to us.
  //
  chpl_comm_amDone_t myDone;
  chpl_comm_amDone_t* pDone = NULL;
  if (ppDone != NULL) {
    pDone = &myDone;
    if (mrGetLocalKey(NULL, pDone, sizeof(*pDone)) != 0) {
      pDone = allocBounceBuf(sizeof(*pDone));
      CHK_TRUE(mrGetLocalKey(NULL, pDone, sizeof(*pDone)) == 0);
    }
    *pDone = 0;
    atomic_thread_fence(memory_order_release);

    *ppDone = pDone;
  }

  struct perTxCtxInfo_t* tcip;
  CHK_TRUE((tcip = tciAlloc(false /*bindToAmHandler*/)) != NULL);

  chpl_comm_on_bundle_t* myArg = arg;
  void* mrDesc = NULL;
  if (mrGetLocalDesc(&mrDesc, myArg, argSize) != 0) {
    myArg = allocBounceBuf(argSize);
    DBG_PRINTF(DBG_AMO, "AMO arg BB: %p", myArg);
    CHK_TRUE(mrGetLocalDesc(NULL, myArg, argSize) == 0);
    memcpy(myArg, arg, argSize);
  }

  OFI_CHK(fi_send(tcip->txCtx, myArg, argSize, mrDesc, ofi_rxAddrs[node],
                  NULL));
  DBG_PRINTF(DBG_AM | DBG_AMSEND,
             "tx AM req to %d, op %d, size %zd, pDone %p",
             node, (int) myArg->comm.op.op, argSize, pDone);
  tcip->numAmReqsTxed++;
  tcip->numTxsOut++;

  if (myArg != arg) {
    freeBounceBuf(myArg);
  }

  //
  // Wait for network completion.
  //
  waitForTxCQ(tcip, 1);

  tciFree(tcip);

  if (pDone != NULL) {
    //
    // Wait for executeOn completion indicator.
    //
    DBG_PRINTF(DBG_AM | DBG_AMSEND,
               "waiting for done indication in %p", pDone);
    while (!*(volatile chpl_comm_amDone_t*) pDone)
      local_yield();
    DBG_PRINTF(DBG_AM | DBG_AMSEND, "saw done indication in %p", pDone);
    if (pDone != &myDone)
      freeBounceBuf(pDone);
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
static void amExecOnWrapper(void*);
static void amGetWrapper(void*);
static void amPutWrapper(void*);
static void amHandleAMO(chpl_comm_on_bundle_t*);
static void amSendDone(c_nodeid_t, chpl_comm_amDone_t*);

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
  CHK_TRUE((tcip = tciAlloc(true /*bindToAmHandler*/)) != NULL);

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
  // Process AM requests.
  //
  while (!atomic_load_bool(&amHandlersExit)) {
    processRxAmReq(tcip);

    const int count = fi_cntr_read(tcip->txCntr);
    if (count > 0) {
      DBG_PRINTF(DBG_ACK, "tx ack counter %d", count);
      tcip->numTxsOut -= count;
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
  const int maxAmsToDo = 10;
  struct fi_cq_data_entry cqes[maxAmsToDo];

  int ret;
  CHK_TRUE((ret = fi_cq_read(ofi_rxCQ, cqes, maxAmsToDo)) > 0
           || ret == -FI_EAGAIN);

  if (ret > 0) {
    const int numEvents = ret;
    for (int i = 0; i < numEvents; i++) {
      if ((cqes[i].flags & FI_RECV) != 0) {
        //
        // This event is for an inbound AM request.  Handle it.
        //
        chpl_comm_on_bundle_t* req = (chpl_comm_on_bundle_t*) cqes[i].buf;
        DBG_PRINTF(DBG_AM | DBG_AMRECV,
                   "CQ rx AM req %p, op %d, len %zd",
                   req, req->comm.op.op, cqes[i].len);
        tcip->numAmReqsRxed++;
        switch (req->comm.op.op) {
        case am_opCall:
          amHandleExecOn(req);
          break;

        case am_opGet:
          //
          // We use a task here mainly to ensure that the GET this AM
          // performs completes before we send the 'done' indicator.  If
          // the AM handler did the GET directly, its contextless RMA
          // completion counter would make it hard to tell when that GET
          // had completed.
          //
          DBG_PRINTF(DBG_AM | DBG_AMRECV,
                     "AM req startMovedTask(amGetWrapper())");
          chpl_task_startMovedTask(FID_NONE, (chpl_fn_p) amGetWrapper,
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
          DBG_PRINTF(DBG_AM | DBG_AMRECV,
                     "AM req startMovedTask(amPutWrapper())");
          chpl_task_startMovedTask(FID_NONE, (chpl_fn_p) amPutWrapper,
                                   chpl_comm_on_bundle_task_bundle(req),
                                   sizeof(*req), c_sublocid_any,
                                   chpl_nullTaskID);
          break;

        case am_opAMO:
          amHandleAMO(req);
          break;

        default:
          INTERNAL_ERROR_V("unexpected AM op %d", req->comm.op.op);
          break;
        }
      }

      if ((cqes[i].flags & FI_MULTI_RECV) != 0) {
        //
        // Multi-receive buffer filled; post another one.
        //
        OFI_CHK(fi_recvmsg(ofi_rxEp, &ofi_msg_reqs, FI_MULTI_RECV));
        DBG_PRINTF(DBG_AM | DBG_AMRECV, "re-post fi_recvmsg(AMReqs)");
      }

      CHK_TRUE((cqes[i].flags & ~(FI_MSG | FI_RECV | FI_MULTI_RECV)) == 0);
    }
  }
}


static
void amHandleExecOn(chpl_comm_on_bundle_t* req) {
  struct chpl_comm_bundleData_execOn_t* xo = &req->comm.xo;
  DBG_PRINTF(DBG_AM | DBG_AMRECV,
             "amHandleExecOn() for node %d: ftable[%d]",
             (int) xo->node, (int) xo->fid);
  chpl_comm_on_bundle_t* reqCopy;
  CHPL_CALLOC_SZ(reqCopy, 1, xo->argSize);
  chpl_memcpy(reqCopy, req, xo->argSize);
  chpl_task_startMovedTask(xo->fid, (chpl_fn_p) amExecOnWrapper,
                           chpl_comm_on_bundle_task_bundle(reqCopy),
                           xo->argSize, xo->subloc, chpl_nullTaskID);
}


static
void amExecOnWrapper(void* p) {
  chpl_comm_on_bundle_t* req = (chpl_comm_on_bundle_t*) p;
  struct chpl_comm_bundleData_execOn_t* xo = &req->comm.xo;

  DBG_PRINTF(DBG_AM | DBG_AMRECV,
             "amExecOnWrapper(): chpl_ftable_call(%d, %p)", (int) xo->fid, p);
  chpl_ftable_call(xo->fid, p);
  if (xo->pDone != NULL) {
    amSendDone(xo->node, xo->pDone);
  }
}


static
void amGetWrapper(void* p) {
  chpl_comm_on_bundle_t* req = (chpl_comm_on_bundle_t*) p;
  struct chpl_comm_bundleData_RMA_t* rma = &req->comm.rma;

  DBG_PRINTF(DBG_AM | DBG_AMRECV,
             "amGetWrapper(): %p <-- %d:%p (%zd bytes)",
             rma->addr, (int) rma->node, rma->raddr, rma->size);
  CHK_TRUE(mrGetKey(NULL, rma->node, rma->raddr, rma->size) == 0); // sanity
  (void) ofi_get(rma->addr, rma->node, rma->raddr, rma->size);

  amSendDone(rma->node, rma->pDone);
}


static
void amPutWrapper(void* p) {
  chpl_comm_on_bundle_t* req = (chpl_comm_on_bundle_t*) p;
  struct chpl_comm_bundleData_RMA_t* rma = &req->comm.rma;

  DBG_PRINTF(DBG_AM | DBG_AMRECV,
             "amPutWrapper() %d:%p <-- %p (%zd bytes)",
             (int) rma->node, rma->raddr, rma->addr, rma->size);
  CHK_TRUE(mrGetKey(NULL, rma->node, rma->raddr, rma->size) == 0); // sanity
  (void) ofi_put(rma->addr, rma->node, rma->raddr, rma->size);

  amSendDone(rma->node, rma->pDone);
}


static
void amHandleAMO(chpl_comm_on_bundle_t* req) {
  struct chpl_comm_bundleData_AMO_t* amo = &req->comm.amo;
  if (amo->ofiOp == FI_CSWAP) {
    DBG_PRINTF(DBG_AM | DBG_AMRECV | DBG_AMO,
               "amHandleAMO() for node %d: obj %p, opnd1 %s, opnd2 %s, "
               "res %p, ofiOp %d, ofiType %d, sz %d",
               amo->node, amo->obj,
               DBG_VAL(&amo->operand1, amo->ofiType),
               DBG_VAL(&amo->operand2, amo->ofiType),
               amo->result, amo->ofiOp, amo->ofiType, amo->size);
  } else if (amo->result != NULL) {
    if (amo->ofiOp == FI_ATOMIC_READ) {
      DBG_PRINTF(DBG_AM | DBG_AMRECV | DBG_AMO,
                 "amHandleAMO() for node %d: obj %p, "
                 "res %p, ofiOp %d, ofiType %d, sz %d",
                 amo->node, amo->obj,
                 amo->result, amo->ofiOp, amo->ofiType, amo->size);
    } else {
      DBG_PRINTF(DBG_AM | DBG_AMRECV | DBG_AMO,
                 "amHandleAMO() for node %d: obj %p, opnd %s, "
                 "res %p, ofiOp %d, ofiType %d, sz %d",
                 amo->node, amo->obj,
                 DBG_VAL(&amo->operand1, amo->ofiType),
                 amo->result, amo->ofiOp, amo->ofiType, amo->size);
    }
  } else {
    DBG_PRINTF(DBG_AM | DBG_AMRECV | DBG_AMO,
               "amHandleAMO() for node %d: obj %p, opnd %s, "
               "ofiOp %d, ofiType %d, sz %d",
               amo->node, amo->obj,
               DBG_VAL(&amo->operand1, amo->ofiType),
               amo->ofiOp, amo->ofiType, amo->size);
  }
  chpl_amo_datum_t result;
  size_t resSize = (amo->ofiOp == FI_CSWAP) ? sizeof(chpl_bool32) : amo->size;
  doCpuAMO(amo->obj, &amo->operand1, &amo->operand2, &result,
           amo->ofiOp, amo->ofiType, amo->size);

  //
  // AMOs can be same-node; short-circuit responses in that case.
  //
  if (amo->result != NULL) {
    if (amo->node == chpl_nodeID) {
      memcpy(amo->result, &result, resSize);
      atomic_thread_fence(memory_order_release);
    } else {
      CHK_TRUE(mrGetKey(NULL, amo->node, amo->result, resSize) == 0);
      (void) ofi_put(&result, amo->node, amo->result, resSize);

      //
      // TODO:
      // HACK!!!  We need to make sure the result arrives before
      //          telling the originator the AMO is done.  For now
      //          we wait here for the tx counter to clear, but
      //          there is surely a better way, perhaps a queue of
      //          outbound writes or some such.
      //
      {
        struct perTxCtxInfo_t* tcip;
        CHK_TRUE((tcip = tciAlloc(false /*bindToAmHandler*/)) != NULL);
        do {
          const int count = fi_cntr_read(tcip->txCntr);
          if (count > 0) {
            DBG_PRINTF(DBG_ACK, "tx ack counter %d after AMO result", count);
            tcip->numTxsOut -= count;
          }
        } while (tcip->numTxsOut > 0);
        tciFree(tcip);
      }
    }
  }

  if (amo->node == chpl_nodeID) {
    *amo->pDone = 1;
  } else {
    amSendDone(amo->node, amo->pDone);
  }
}


static
void amSendDone(c_nodeid_t node, chpl_comm_amDone_t* pDone) {
  static __thread chpl_comm_amDone_t* myDone = NULL;
  if (myDone == NULL) {
    myDone = allocBounceBuf(1);
    CHK_TRUE(mrGetLocalDesc(NULL, myDone, 1) == 0);
    *myDone = 1;
  }

  CHK_TRUE(mrGetKey(NULL, node, pDone, sizeof(*pDone)) == 0);
  DBG_PRINTF(DBG_AM, "AM set pDone %d:%p", node, pDone);
  (void) ofi_put(myDone, node, pDone, sizeof(*pDone));
}


////////////////////////////////////////
//
// Interface: RMA
//

chpl_comm_nb_handle_t chpl_comm_put_nb(void *addr, c_nodeid_t node,
                                       void* raddr, size_t size,
                                       int32_t typeIndex, int32_t commID,
                                       int ln, int32_t fn) {
  chpl_comm_put(addr, node, raddr, size, typeIndex, commID, ln, fn);
  return NULL;
}


chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, c_nodeid_t node,
                                       void* raddr, size_t size,
                                       int32_t typeIndex, int32_t commID,
                                       int ln, int32_t fn) {
  chpl_comm_get(addr, node, raddr, size, typeIndex, commID, ln, fn);
  return NULL;
}


int chpl_comm_test_nb_complete(chpl_comm_nb_handle_t h) {
  if (chpl_verbose_comm && chpl_comm_diags_is_enabled()) {
    chpl_comm_diags_verbose_printf("test nb complete (%p)", h);
  }
  chpl_comm_diags_incr(test_nb);

  // fi_cq_readfrom?
  return ((void*) h) == NULL;
}


void chpl_comm_wait_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles) {
  if (chpl_verbose_comm && chpl_comm_diags_is_enabled()) {
    if (nhandles == 1)
      chpl_comm_diags_verbose_printf("wait nb complete (%p)", h);
    else
      chpl_comm_diags_verbose_printf("wait nb (%zd handles)", nhandles);
  }
  chpl_comm_diags_incr(wait_nb);

  size_t i;
  // fi_cq_readfrom?
  for( i = 0; i < nhandles; i++ ) {
    CHK_TRUE(h[i] == NULL);
  }
}


int chpl_comm_try_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles) {

  if (chpl_verbose_comm && chpl_comm_diags_is_enabled()) {
    if (nhandles == 1)
      chpl_comm_diags_verbose_printf("try nb (%p)", h);
    else
      chpl_comm_diags_verbose_printf("try nb (%zd handles)", nhandles);
  }
  chpl_comm_diags_incr(try_nb);

  size_t i;
  // fi_cq_readfrom?
  for( i = 0; i < nhandles; i++ ) {
    CHK_TRUE(h[i] == NULL);
  }
  return 0;
}


void chpl_comm_put(void* addr, c_nodeid_t node, void* raddr,
                   size_t size, int32_t typeIndex, int32_t commID,
                   int ln, int32_t fn) {
  //
  // addr and raddr are sanity checks; node==chpl_nodeID is supposed
  // to be handled by our caller.
  //
  CHK_TRUE(addr != NULL);
  CHK_TRUE(raddr != NULL);
  CHK_TRUE(node != chpl_nodeID);

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_put)) {
      chpl_comm_cb_info_t cb_data =
        {chpl_comm_cb_event_kind_put, chpl_nodeID, node,
         .iu.comm={addr, raddr, size, typeIndex, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_printf("%s:%d: remote put to %d",
                                 chpl_lookupFilename(fn), ln, (int) node);
  chpl_comm_diags_incr(put);

  (void) ofi_put(addr, node, raddr, size);
}


void chpl_comm_get(void* addr, int32_t node, void* raddr,
                   size_t size, int32_t typeIndex, int32_t commID,
                   int ln, int32_t fn) {
  //
  // addr and raddr are sanity checks; node==chpl_nodeID is supposed
  // to be handled by our caller.
  //
  CHK_TRUE(addr != NULL);
  CHK_TRUE(raddr != NULL);
  CHK_TRUE(node != chpl_nodeID);

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_get)) {
      chpl_comm_cb_info_t cb_data =
        {chpl_comm_cb_event_kind_get, chpl_nodeID, node,
         .iu.comm={addr, raddr, size, typeIndex, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_printf("%s:%d: remote get from %d",
                                 chpl_lookupFilename(fn), ln, (int) node);
  chpl_comm_diags_incr(get);

  (void) ofi_get(addr, node, raddr, size);
}


void chpl_comm_put_strd(void* dstaddr_arg, size_t* dststrides,
                        c_nodeid_t dstnode,
                        void* srcaddr_arg, size_t* srcstrides,
                        size_t* count, int32_t stridelevels, size_t elemSize,
                        int32_t typeIndex, int32_t commID,
                        int ln, int32_t fn) {
  put_strd_common(dstaddr_arg, dststrides,
                  dstnode,
                  srcaddr_arg, srcstrides,
                  count, stridelevels, elemSize,
                  1, NULL,
                  typeIndex, commID, ln, fn);
}


void chpl_comm_get_strd(void* dstaddr_arg, size_t* dststrides,
                        c_nodeid_t srcnode,
                        void* srcaddr_arg, size_t* srcstrides, size_t* count,
                        int32_t stridelevels, size_t elemSize,
                        int32_t typeIndex, int32_t commID,
                        int ln, int32_t fn) {
  get_strd_common(dstaddr_arg, dststrides,
                  srcnode,
                  srcaddr_arg, srcstrides,
                  count, stridelevels, elemSize,
                  1, NULL,
                  typeIndex, commID, ln, fn);
}


////////////////////////////////////////
//
// Internal communication support
//

static struct perTxCtxInfo_t* findFreeTciTabEntry(chpl_bool);

static inline
struct perTxCtxInfo_t* tciAlloc(chpl_bool bindToAmHandler) {
  static __thread struct perTxCtxInfo_t* tcip;
  if (tcip != NULL) {
    //
    // If the last tx context we used is bound to our thread or can be
    // re-allocated, use that.
    //
    if (tcip->bound) {
      DBG_PRINTF(DBG_THREADS, "I re-have bound tciTab[%td]",
                 tcip - tciTab);
      return tcip;
    }

    if (!atomic_exchange_bool(&tcip->allocated, true)) {
      DBG_PRINTF(DBG_THREADS, "I re-have tciTab[%td]", tcip - tciTab);
      return tcip;
    }
  }

  //
  // Find a tx context that isn't busy and use that one.
  //
  tcip = findFreeTciTabEntry(bindToAmHandler);
  if (bindToAmHandler || tciTabFixedAssignments)
    tcip->bound = true;
  DBG_PRINTF(DBG_THREADS, "I have%s tciTab[%td]",
             bindToAmHandler ? " bound" : "", tcip - tciTab);
  return tcip;
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
    // we ever have more, the CHK_TRUE force us to revisit this code.
    //
    tcip = &tciTab[numWorkerTxCtxs];
    CHK_FALSE(atomic_exchange_bool(&tcip->allocated, true));
    tcip->bound = true;
    return tcip;
  }

  //
  // Workers use tciTab[0 .. numWorkerTxCtxs - 1].  Search forever for
  // an entry we can use.  Give up (and kill the program) only if we
  // discover they're all bound, because we won't be able to get one
  // in that case.
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
void tciFree(struct perTxCtxInfo_t* tcip) {
  //
  // Bound contexts stay bound.  We only release non-bound ones.
  //
  if (!tcip->bound)
    atomic_store_bool(&tcip->allocated, false);
}


static /*inline*/
chpl_comm_nb_handle_t ofi_put(const void* addr, c_nodeid_t node,
                              void* raddr, size_t size) {
  DBG_PRINTF(DBG_RMA | DBG_RMAWRITE,
             "PUT %d:%p <= %p, size %zd",
             (int) node, raddr, addr, size);

  void* mrDesc = NULL;
  void* myAddr = (void*) addr;
  if (mrGetLocalDesc(&mrDesc, myAddr, size) != 0) {
    myAddr = allocBounceBuf(size);
    DBG_PRINTF(DBG_RMA | DBG_RMAWRITE, "PUT via BB: %p", myAddr);
    CHK_TRUE(mrGetLocalDesc(&mrDesc, myAddr, size) == 0);
    memcpy(myAddr, addr, size);
  }

  uint64_t mrKey;
  if (mrGetKey(&mrKey, node, raddr, size) == 0) {
    //
    // The remote address is RMA-accessible; PUT directly to it.
    //
    struct perTxCtxInfo_t* tcip;
    CHK_TRUE((tcip = tciAlloc(false /*bindToAmHandler*/)) != NULL);

    DBG_PRINTF(DBG_RMA | DBG_RMAWRITE,
               "tx write: %d:%p <= %p, size %zd, key 0x%" PRIx64,
               (int) node, raddr, myAddr, size, mrKey);
    OFI_CHK(fi_write(tcip->txCtx, myAddr, size,
                     mrDesc, ofi_rxAddrs[node], (uint64_t) raddr, mrKey, 0));
    tcip->numTxsOut++;

    if (tcip->txCtxHasCQ) {
      waitForTxCQ(tcip, 1);
    } else {
      const int count = fi_cntr_read(tcip->txCntr);
      DBG_PRINTF(DBG_ACK, "tx ack counter %d after PUT", count);
      tcip->numTxsOut -= count;
    }

    tciFree(tcip);
  } else {
    //
    // The remote address is not RMA-accessible.  We have arranged
    // that the local one is, so go there and do the opposite RMA.
    //
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


static /*inline*/
chpl_comm_nb_handle_t ofi_get(void *addr, c_nodeid_t node,
                              void* raddr, size_t size) {
  DBG_PRINTF(DBG_RMA | DBG_RMAREAD,
             "GET %p <= %d:%p, size %zd",
             addr, (int) node, raddr, size);

  void* mrDesc = NULL;
  void* myAddr = addr;
  if (mrGetLocalDesc(&mrDesc, myAddr, size) != 0) {
    myAddr = allocBounceBuf(size);
    DBG_PRINTF(DBG_RMA | DBG_RMAREAD, "GET via BB: %p", myAddr);
    CHK_TRUE(mrGetLocalDesc(&mrDesc, myAddr, size) == 0);
  }

  uint64_t mrKey;
  if (mrGetKey(&mrKey, node, raddr, size) == 0) {
    //
    // The remote address is RMA-accessible; GET directly from it.
    //
    struct perTxCtxInfo_t* tcip;
    CHK_TRUE((tcip = tciAlloc(false /*bindToAmHandler*/)) != NULL);
    DBG_PRINTF(DBG_RMA | DBG_RMAREAD,
               "tx read: %p <= %d:%p, size %zd, key 0x%" PRIx64,
               myAddr, (int) node, raddr, size, mrKey);
    OFI_CHK(fi_read(tcip->txCtx, myAddr, size,
                    mrDesc, ofi_rxAddrs[node], (uint64_t) raddr, mrKey, 0));
    tcip->numTxsOut++;

    CHK_TRUE(tcip->txCtxHasCQ);
    waitForTxCQ(tcip, 1);

    tciFree(tcip);
  } else {
    //
    // The remote address is not RMA-accessible.  We have arranged
    // that the local one is, so go there and do the opposite RMA.
    //
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
                              c_nodeid_t node, void* object, uint64_t mrKey,
                              const void* operand1, const void* operand2,
                              void* result,
                              enum fi_op ofiOp, enum fi_datatype ofiType,
                              size_t size) {
  DBG_PRINTF(DBG_AMO,
             "tx AMO: obj %d:%p, opnd1 <%s>, opnd2 <%s>, res %p, "
             "op %d, typ %d, sz %zd",
             (int) node, object,
             DBG_VAL(operand1, ofiType), DBG_VAL(operand2, ofiType), result,
             ofiOp, ofiType, size);

  void* myRes = result;
  size_t resSize = (ofiOp == FI_CSWAP) ? sizeof(chpl_bool32) : size;
  void* mrDescRes = NULL;
  if (myRes != NULL && mrGetLocalDesc(&mrDescRes, myRes, resSize) != 0) {
    myRes = allocBounceBuf(resSize);
    DBG_PRINTF(DBG_AMO, "AMO result BB: %p", myRes);
    CHK_TRUE(mrGetLocalDesc(&mrDescRes, myRes, resSize) == 0);
  }

  void* myOpnd1 = (void*) operand1;
  void* mrDescOpnd1 = NULL;
  if (myOpnd1 != NULL && mrGetLocalDesc(&mrDescOpnd1, myOpnd1, size) != 0) {
    myOpnd1 = allocBounceBuf(size);
    DBG_PRINTF(DBG_AMO, "AMO operand1 BB: %p", myOpnd1);
    CHK_TRUE(mrGetLocalDesc(&mrDescOpnd1, myOpnd1, size) == 0);
    memcpy(myOpnd1, operand1, size);
  }

  void* myOpnd2 = (void*) operand2;
  void* mrDescOpnd2 = NULL;
  if (myOpnd2 != NULL && mrGetLocalDesc(&mrDescOpnd2, myOpnd2, size) != 0) {
    myOpnd2 = allocBounceBuf(size);
    DBG_PRINTF(DBG_AMO, "AMO operand2 BB: %p", myOpnd2);
    CHK_TRUE(mrGetLocalDesc(&mrDescOpnd2, myOpnd2, size) == 0);
    memcpy(myOpnd2, operand2, size);
  }

  if (ofiOp == FI_CSWAP) {
    OFI_CHK(fi_compare_atomic(tcip->txCtx,
                              myOpnd2, 1, mrDescOpnd2, myOpnd1, mrDescOpnd1,
                              myRes, mrDescRes,
                              ofi_rxAddrs[node], (uint64_t) object, mrKey,
                              ofiType, ofiOp, NULL));
  } else if (result != NULL) {
    CHK_TRUE(operand2 == NULL);
    OFI_CHK(fi_fetch_atomic(tcip->txCtx,
                            myOpnd1, 1, mrDescOpnd1, myRes, mrDescRes,
                            ofi_rxAddrs[node], (uint64_t) object, mrKey,
                            ofiType, ofiOp, NULL));
  } else {
    CHK_TRUE(operand2 == NULL);
    OFI_CHK(fi_atomic(tcip->txCtx,
                      myOpnd1, 1, mrDescOpnd1,
                      ofi_rxAddrs[node], (uint64_t) object, mrKey,
                      ofiType, ofiOp, NULL));
  }

  tcip->numTxsOut++;
  CHK_TRUE(tcip->txCtxHasCQ);  // (so far) only expect AMOs from workers
  waitForTxCQ(tcip, 1);

  tciFree(tcip);

  if (myRes != result) {
    memcpy(result, myRes, resSize);
    freeBounceBuf(myRes);
  }

  if (myOpnd1 != operand1) {
    freeBounceBuf(myOpnd1);
  }

  if (myOpnd2 != operand2) {
    freeBounceBuf(myOpnd2);
  }

  return NULL;
}


static
void waitForTxCQ(struct perTxCtxInfo_t* tcip, int numOut) {
  if (numOut > 0) {
    struct fi_cq_entry cqes[numOut];
    int numRetired = 0;

    do {
      int ret;
      CHK_TRUE((ret = fi_cq_read(tcip->txCQ, cqes, numOut)) > 0
               || ret == -FI_EAGAIN
               || ret == -FI_EAVAIL);

      if (ret == -FI_EAVAIL) {
        struct fi_cq_err_entry err = { 0 };
        char bufProv[100];
        fi_cq_readerr(tcip->txCQ, &err, 0);
        (void) fi_cq_strerror(tcip->txCQ, err.prov_errno, err.err_data,
                              bufProv, sizeof(bufProv));
        INTERNAL_ERROR_V("fi_cq_read(): err %d, strerror %s",
                         err.err, bufProv);
      }

      if (ret > 0) {
        const int numEvents = ret;
        numRetired += numEvents;
        for (int i = 0; i < numEvents; i++) {
          DBG_PRINTF(DBG_ACK, "CQ ack tx");
        }
      }
    } while (numRetired < numOut);

    tcip->numTxsOut -= numRetired;
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
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_write_%s(%p, %d, %p, %d, %s)",         \
               #fnType, desired, (int) node, object,                    \
               ln, chpl_lookupFilename(fn));                            \
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
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_read_%s(%p, %d, %p, %d, %s)",          \
               #fnType, result, (int) node, object,                     \
               ln, chpl_lookupFilename(fn));                            \
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
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_xchg_%s(%p, %d, %p, %p, %d, %s)",      \
               #fnType, desired, (int) node, object, result,            \
               ln, chpl_lookupFilename(fn));                            \
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
          chpl_bool32* result,                                          \
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_cmpxchg_%s(%p, %p, %d, %p, %p, "       \
               "%d, %s)",                                               \
               #fnType, expected, desired, (int) node, object, result,  \
               ln, chpl_lookupFilename(fn));                            \
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
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_%s_%s(<%s>, %d, %p, %d, %s)",          \
               #fnOp, #fnType, DBG_VAL(operand, ofiType), (int) node,   \
               object, ln, chpl_lookupFilename(fn));                    \
    doAMO(node, object, operand, NULL, NULL,                            \
          ofiOp, ofiType, sizeof(Type));                                \
  }                                                                     \
                                                                        \
  void chpl_comm_atomic_##fnOp##_buff_##fnType                          \
         (void* operand, c_nodeid_t node, void* object,                 \
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_%s_buff_%s(<%s>, %d, %p, %d, %s)",     \
               #fnOp, #fnType, DBG_VAL(operand, ofiType), (int) node,   \
               object, ln, chpl_lookupFilename(fn));                    \
    chpl_comm_atomic_##fnOp##_##fnType(operand, node, object, ln, fn);  \
  }                                                                     \
                                                                        \
  void chpl_comm_atomic_fetch_##fnOp##_##fnType                         \
         (void* operand, c_nodeid_t node, void* object, void* result,   \
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_fetch_%s_%s(<%s>, %d, %p, %p, "        \
               "%d, %s)",                                               \
               #fnOp, #fnType, DBG_VAL(operand, ofiType), (int) node,   \
               object, result, ln, chpl_lookupFilename(fn));            \
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
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_sub_%s(<%s>, %d, %p, %d, %s)",         \
               #fnType, DBG_VAL(operand, ofiType), (int) node, object,  \
               ln, chpl_lookupFilename(fn));                            \
    Type myOpnd = negate(*(Type*) operand);                             \
    doAMO(node, object, &myOpnd, NULL, NULL,                            \
          FI_SUM, ofiType, sizeof(Type));                               \
  }                                                                     \
                                                                        \
  void chpl_comm_atomic_sub_buff_##fnType                               \
         (void* operand, c_nodeid_t node, void* object,                 \
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_sub_buff_%s(<%s>, %d, %p, "            \
               "%d, %s)",                                               \
               #fnType, DBG_VAL(operand, ofiType), (int) node, object,  \
               ln, chpl_lookupFilename(fn));                            \
    chpl_comm_atomic_sub_##fnType(operand, node, object, ln, fn);       \
  }                                                                     \
                                                                        \
  void chpl_comm_atomic_fetch_sub_##fnType                              \
         (void* operand, c_nodeid_t node, void* object, void* result,   \
          int ln, int32_t fn) {                                         \
    DBG_PRINTF(DBG_INTERFACE,                                           \
               "chpl_comm_atomic_fetch_sub_%s(<%s>, %d, %p, %p, "       \
               "%d, %s)",                                               \
               #fnType, DBG_VAL(operand, ofiType), (int) node, object,  \
               result, ln, chpl_lookupFilename(fn));                    \
    Type myOpnd = negate(*(Type*) operand);                             \
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


void chpl_comm_atomic_buff_flush(void) {
  return;
}


//
// internal AMO utilities
//

static inline
void doAMO(c_nodeid_t node, void* object,
           const void* operand1, const void* operand2, void* result,
           int ofiOp, enum fi_datatype ofiType, size_t size) {
  if (chpl_numNodes <= 1) {
    doCpuAMO(object, operand1, operand2, result, ofiOp, ofiType, size);
    return;
  }

  uint64_t mrKey;
  if (mrGetKey(&mrKey, node, object, size) == 0) {
    struct perTxCtxInfo_t* tcip = NULL;
    CHK_TRUE((tcip = tciAlloc(false /*bindToAmHandler*/)) != NULL);

    size_t count;
    if (fi_atomicvalid(tcip->txCtx, ofiType, ofiOp, &count) == 0
        && count > 0) {
      //
      // The object address is remotely-accessible and the atomic op
      // and type are supported in the network.  Do the AMO natively.
      //
      ofi_amo(tcip, node, object, mrKey, operand1, operand2, result,
              ofiOp, ofiType, size);
      tciFree(tcip);
      return;
    }

    tciFree(tcip);
  }

  amRequestAMO(node, object, operand1, operand2, result,
               ofiOp, ofiType, size);
}


static inline
void doCpuAMO(void* obj,
              const void* operand1, const void* operand2, void* result,
              enum fi_op ofiOp, enum fi_datatype ofiType, size_t size) {
  CHK_TRUE(size == 4 || size == 8);

  chpl_amo_datum_t myOpnd1 = { 0 };
  if (operand1 != NULL)
    memcpy(&myOpnd1, operand1, size);

  chpl_amo_datum_t myOpnd2 = { 0 };
  if (operand2 != NULL)
    memcpy(&myOpnd2, operand2, size);

  chpl_amo_datum_t* myResult = (chpl_amo_datum_t*) result;

#define CPU_INT_ARITH_AMO(_o, _t, _m)                                   \
  do {                                                                  \
    _t my_res = atomic_fetch_##_o##_##_t((atomic_##_t*) obj,            \
                                         myOpnd1._m);                   \
    if (result != NULL) {                                               \
      myResult->_m = my_res;                                            \
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
        atomic_store_uint_least32_t(obj, myOpnd1.u32);
      } else {
        atomic_store_uint_least64_t(obj, myOpnd1.u64);
      }
    } else {
      //
      // exchange
      //
      if (size == 4) {
        myResult->u32 = atomic_exchange_uint_least32_t(obj, myOpnd1.u32);
      } else {
        myResult->u64 = atomic_exchange_uint_least64_t(obj, myOpnd1.u64);
      }
    }
    break;

  case FI_ATOMIC_READ:
    if (size == 4) {
      myResult->u32 = atomic_load_uint_least32_t(obj);
    } else {
      myResult->u64 = atomic_load_uint_least64_t(obj);
    }
    break;

  case FI_CSWAP:
    if (size == 4) {
      myResult->b32 =
        atomic_compare_exchange_strong_uint_least32_t(obj,
                                                      myOpnd1.u32,
                                                      myOpnd2.u32);
    } else {
      myResult->b32 =
        atomic_compare_exchange_strong_uint_least64_t(obj,
                                                      myOpnd1.u64,
                                                      myOpnd2.u64);
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
        dsrd.r32 = xpctd.r32 + myOpnd1.r32;
        done = atomic_compare_exchange_strong_uint_least32_t(obj,
                                                             xpctd.u32,
                                                             dsrd.u32);
      } while (!done);

      if (result != NULL) {
        myResult->r32 = xpctd.r32;
      }
    } else if (ofiType == FI_DOUBLE) {
      chpl_amo_datum_t xpctd;
      chpl_amo_datum_t dsrd;
      chpl_bool32 done;

      do {
        xpctd.u64 = atomic_load_int_least64_t(obj);
        dsrd.r64 = xpctd.r64 + myOpnd1.r64;
        done = atomic_compare_exchange_strong_uint_least64_t(obj,
                                                             xpctd.u64,
                                                             dsrd.u64);
      } while (!done);

      if (result != NULL) {
        myResult->r64 = xpctd.r64;
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
    chpl_amo_datum_t myObj = { 0 };
    if (ofiType == FI_INT32)
      memcpy(&myObj.i32, obj, sizeof(myObj.i32));
    else if (ofiType == FI_UINT32)
      memcpy(&myObj.u32, obj, sizeof(myObj.u32));
    else if (ofiType == FI_INT64)
      memcpy(&myObj.i64, obj, sizeof(myObj.i64));
    else if (ofiType == FI_UINT64)
      memcpy(&myObj.u64, obj, sizeof(myObj.u64));
    else if (ofiType == FI_FLOAT)
      memcpy(&myObj.r32, obj, sizeof(myObj.i32));
    else
      memcpy(&myObj.r64, obj, sizeof(myObj.r64));

    if (result == NULL)
      DBG_PRINTF(DBG_AMO,
                 "doCpuAMO(%p, %d, %d, %s): now %s",
                 obj, ofiOp, ofiType,
                 DBG_VAL(&myOpnd1, ofiType),
                 DBG_VAL(&myObj, ofiType));
    else {
      chpl_amo_datum_t myRes2 = { 0 };
      if (ofiType == FI_INT32 || ofiType == FI_UINT32 || ofiType == FI_FLOAT
          || ofiOp == FI_CSWAP)
        myRes2.u32 = myResult->u32;
      else
        myRes2.u64 = myResult->u64;
      DBG_PRINTF(DBG_AMO,
                 "doCpuAMO(%p, %d, %d, %s, %s): now %s, %p = was %s",
                 obj, ofiOp, ofiType,
                 DBG_VAL(&myOpnd1, ofiType),
                 DBG_VAL(&myOpnd2, ofiType),
                 DBG_VAL(&myObj, ofiType), result,
                 DBG_VAL(&myRes2, ofiType));
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
  chpl_msg(2, "%d: enter barrier for '%s'\n", chpl_nodeID, msg);

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
// Comm diagnostics
//

void chpl_startVerboseComm() {
  chpl_verbose_comm = 1;
  chpl_comm_diags_disable();
  chpl_comm_broadcast_private(0 /* &chpl_verbose_comm */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_diags_enable();
}


void chpl_stopVerboseComm() {
  chpl_verbose_comm = 0;
  chpl_comm_diags_disable();
  chpl_comm_broadcast_private(0 /* &chpl_verbose_comm */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_diags_enable();
}


void chpl_startVerboseCommHere() {
  chpl_verbose_comm = 1;
}


void chpl_stopVerboseCommHere() {
  chpl_verbose_comm = 0;
}


void chpl_startCommDiagnostics() {
  chpl_comm_diagnostics = 1;
  chpl_comm_diags_disable();
  chpl_comm_broadcast_private(1 /* &chpl_comm_diagnostics */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_diags_enable();
}


void chpl_stopCommDiagnostics() {
  chpl_comm_diagnostics = 0;
  chpl_comm_diags_disable();
  chpl_comm_broadcast_private(1 /* &chpl_comm_diagnostics */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_diags_enable();
}


void chpl_startCommDiagnosticsHere() {
  chpl_comm_diagnostics = 1;
}


void chpl_stopCommDiagnosticsHere() {
  chpl_comm_diagnostics = 0;
}


void chpl_resetCommDiagnosticsHere() {
  chpl_comm_diags_reset();
}


void chpl_getCommDiagnosticsHere(chpl_commDiagnostics *cd) {
  chpl_comm_diags_copy(cd);
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


#ifdef DEBUG

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

#endif
