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

static int numTxCtxs;

struct perTxCtxInfo_t {
  int inited;
  int idx;
  char name[5];
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

static int ptiTabLen;
static struct perTxCtxInfo_t* ptiTab;
static pthread_mutex_t pti_mutex = PTHREAD_MUTEX_INITIALIZER;

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

static inline struct perTxCtxInfo_t* getTxCtxInfo(chpl_bool);
static inline void releaseTxCtxInfo(struct perTxCtxInfo_t*);
static /*inline*/ chpl_comm_nb_handle_t ofi_put(void*, c_nodeid_t,
                                                void*, size_t);
static /*inline*/ chpl_comm_nb_handle_t ofi_get(void*, c_nodeid_t,
                                                void*, size_t);
static void waitForTxCQ(struct perTxCtxInfo_t*, int);
static void* allocBounceBuf(size_t);
static void freeBounceBuf(void*);

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

static void init_ofi(void);
static void init_ofiFabricDomain(void);
static int compute_comm_concurrency(void);
static void init_ofiEp(void);
static void init_ofiExchangeAvInfo(void);
static void init_ofiForMem(void);
static void init_ofiForRma(void);
static void init_ofiForAms(void);


void chpl_comm_init(int *argc_p, char ***argv_p) {
  chpl_comm_ofi_abort_on_error =
    (chpl_env_rt_get("COMM_OFI_ABORT_ON_ERROR", NULL) != NULL);
  time_init();
  chpl_comm_ofi_oob_init();
  DBG_INIT();
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
      if (DBG_TEST_MASK(DBG_FABFAIL)) {
        DBG_PRINTF(DBG_FABFAIL, "==================== hints:");
        DBG_PRINTF(DBG_FABFAIL, "%s", fi_tostr(hints, FI_TYPE_INFO));
        DBG_PRINTF(DBG_FABFAIL, "==================== fi_getinfo() fabrics:");
        DBG_PRINTF(DBG_FABFAIL,
                   "None matched hints; available with prov_name \"%s\" are:",
                   (provider == NULL) ? "<any>" : provider);
        struct fi_info* info = NULL;
        OFI_CHK(fi_getinfo(FI_VERSION(1,5), NULL, NULL, 0, NULL, &info));
        for ( ; info != NULL; info = info->next) {
          const char* pn = info->fabric_attr->prov_name;
          if (provider == NULL
              || strncmp(pn, provider, strlen(provider)) == 0) {
            DBG_PRINTF(DBG_FABFAIL, "%s", fi_tostr(info, FI_TYPE_INFO));
            DBG_PRINTF(DBG_FABFAIL, "----------");
          }
        }
      }

      INTERNAL_ERROR_V("No provider matched for prov_name \"%s\"",
                       (provider == NULL) ? "<any>" : provider);
    }

    if (DBG_TEST_MASK(DBG_FAB)) {
      if (DBG_TEST_MASK(DBG_FABSALL)) {
        DBG_PRINTF(DBG_FABSALL, "====================\n"
                   "fi_getinfo() matched fabric(s):");
        struct fi_info* info;
        for (info = ofi_info; info != NULL; info = info->next) {
          DBG_PRINTF(DBG_FABSALL, "%s", fi_tostr(ofi_info, FI_TYPE_INFO));
          DBG_PRINTF(DBG_FABSALL, "----------");
        }
    } else 
      DBG_PRINTF(DBG_FAB, "====================\n"
                 "fi_getinfo() matched fabric:");
      DBG_PRINTF(DBG_FAB, "%s", fi_tostr(ofi_info, FI_TYPE_INFO));
      DBG_PRINTF(DBG_FAB, "----------");
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
  // Compute number of transmit contexts and create our scalable
  // transmit endpoint.  Each worker thread needs its own transmit
  // context, plus each AM handler needs one to send 'finished'
  // indicators on.  We only need one receive context for now, for
  // requests sent to our single AM handler.
  //
  CHK_TRUE(numAmHandlers == 1); // force rework here if #AM handlers changes

  const int commConcurrency = compute_comm_concurrency();
  numTxCtxs = commConcurrency + numAmHandlers;

  {
    const struct fi_domain_attr* dom_attr = ofi_info->domain_attr;
    if (numTxCtxs > dom_attr->max_ep_tx_ctx)
      numTxCtxs = dom_attr->max_ep_tx_ctx - numAmHandlers;
    CHK_TRUE(numTxCtxs > 0);
    ofi_info->ep_attr->tx_ctx_cnt = numTxCtxs + numAmHandlers;

    CHK_TRUE(dom_attr->max_ep_rx_ctx >= numAmHandlers);
    ofi_info->ep_attr->rx_ctx_cnt = numAmHandlers;
  }

  //
  // Create address vectors for each thread.
  //
  struct fi_av_attr ofi_avAttr = { 0 };
  ofi_avAttr.type = FI_AV_TABLE;
  ofi_avAttr.count = chpl_numNodes;
  ofi_avAttr.name = NULL;
  ofi_avAttr.rx_ctx_bits = 0;

  OFI_CHK(fi_av_open(ofi_domain, &ofi_avAttr, &ofi_av, NULL));
}


static
int compute_comm_concurrency(void) {
  int val;

  // problematic: CHPL_RT_COMM_CONCURRENCY==0 ignored, but no warning
  if ((val = chpl_env_rt_get_int("COMM_CONCURRENCY", 0)) > 0) {
    return val;
  } else if (val < 0) {
    chpl_warning("CHPL_RT_COMM_CONCURRENCY < 0, ignored", 0, 0);
  }

  if ((val = chpl_task_getFixedNumThreads()) > 0)
    return val;

  // problematic: similar
  if ((val = chpl_env_rt_get_int("NUM_HARDWARE_THREADS", 0)) > 0) {
    return val;
  } else if (val < 0) {
    chpl_warning("CHPL_RT_NUM_HARDWARE_THREADS < 0, ignored", 0, 0);
  }

  if ((val = chpl_topo_getNumCPUsLogical(true)) > 0) {
    return val;
  }

  chpl_warning("Could not determine comm concurrency, using 1", 0, 0);
  return 1;
}


static
void init_ofiEp(void) {
  ptiTabLen = numTxCtxs;
  CHPL_CALLOC(ptiTab, ptiTabLen);

  //
  // Transmit.
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
  // Worker TX contexts use CQs; AM handler just needs a counter.
  //
  for (int i = 0; i < ptiTabLen; i++) {
    OFI_CHK(fi_tx_context(ofi_txEp, i, NULL, &ptiTab[i].txCtx, NULL));
    if (i < ptiTabLen - numAmHandlers) {
      // Regular worker thread tx context.
      OFI_CHK(fi_cq_open(ofi_domain, &txCqAttr, &ptiTab[i].txCQ, NULL));
      OFI_CHK(fi_ep_bind(ptiTab[i].txCtx, &ptiTab[i].txCQ->fid, FI_TRANSMIT));
      ptiTab[i].txCtxHasCQ = true;
    } else {
      // AM handler tx context.
      OFI_CHK(fi_cntr_open(ofi_domain, &txCntrAttr, &ptiTab[i].txCntr, NULL));
      OFI_CHK(fi_ep_bind(ptiTab[i].txCtx, &ptiTab[i].txCntr->fid, FI_WRITE));
      ptiTab[i].txCtxHasCQ = false;
    }
    OFI_CHK(fi_enable(ptiTab[i].txCtx));
  }

  //
  // Receive.
  //
  // For the CQ length, allow for an appreciable proportion of the job
  // to send requests to us at once.
  //
  struct fi_cq_attr rxCqAttr = { 0 };
  rxCqAttr.format = FI_CQ_FORMAT_DATA;
  rxCqAttr.size = chpl_numNodes * numTxCtxs;
  rxCqAttr.wait_obj = FI_WAIT_NONE;

  OFI_CHK(fi_endpoint(ofi_domain, ofi_info, &ofi_rxEp, NULL));
  OFI_CHK(fi_ep_bind(ofi_rxEp, &ofi_av->fid, 0));
  OFI_CHK(fi_cq_open(ofi_domain, &rxCqAttr, &ofi_rxCQ, NULL));
  OFI_CHK(fi_ep_bind(ofi_rxEp, &ofi_rxCQ->fid, FI_RECV));
  OFI_CHK(fi_enable(ofi_rxEp));
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
  CHPL_FREE(memTab);
  CHPL_FREE(ofiMrTab);

  CHPL_FREE(amLZs);

  CHPL_FREE(ofi_rxAddrs);

  OFI_CHK(fi_close(&ofi_rxEp->fid));
  OFI_CHK(fi_close(&ofi_rxCQ->fid));

  for (int i = 0; i < ptiTabLen; i++) {
    OFI_CHK(fi_close(&ptiTab[i].txCtx->fid));
    if (i < ptiTabLen - numAmHandlers)
      OFI_CHK(fi_close(&ptiTab[i].txCQ->fid));
    else
      OFI_CHK(fi_close(&ptiTab[i].txCntr->fid));
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
  // We only need a fixed heap if we're using the gni provider.
  //
  if (strcmp(getProviderName(), "gni") != 0)
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
} amOp_t;

static void amRequestExecOn(c_nodeid_t, c_sublocid_t, chpl_fn_int_t,
                            chpl_comm_on_bundle_t*, size_t,
                            chpl_bool, chpl_bool);
static void amRequestRMA(c_nodeid_t, amOp_t, void*, void*, size_t);
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
                     .nodeID = chpl_nodeID,
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
                     .nodeID = chpl_nodeID,
                     .raddr = addr,
                     .size = size,
                     .pDone = NULL };
  amRequestCommon(node, &arg,
                  (offsetof(chpl_comm_on_bundle_t, comm)
                   + sizeof(arg.comm.rma)),
                  &arg.comm.rma.pDone);
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
  CHK_TRUE((tcip = getTxCtxInfo(true /*isWorker*/)) != NULL);

  void* mrDesc = NULL;
#if 1 // TODO: needed for gni provider?
  CHK_TRUE(mrGetLocalDesc(&mrDesc, arg, argSize) == 0);
#endif

  OFI_CHK(fi_send(tcip->txCtx, arg, argSize, mrDesc, ofi_rxAddrs[node], NULL));
  DBG_PRINTF(DBG_AM | DBG_AMSEND,
             "tx AM req to %d, op %d, size %zd, pDone %p",
             node, (int) arg->comm.op.op, argSize, pDone);
  tcip->numAmReqsTxed++;
  tcip->numTxsOut++;

  //
  // Wait for network completion.
  //
  waitForTxCQ(tcip, 1);

  releaseTxCtxInfo(tcip);
 
  if (pDone != NULL) {
    //
    // Wait for executeOn completion indicator.
    //
    DBG_PRINTF(DBG_AM | DBG_AMSEND,
               "waiting for done indication in %p", pDone);
    while (!*(volatile chpl_comm_amDone_t*) pDone)
      sched_yield();
    if (pDone != &myDone)
      freeBounceBuf(pDone);
    DBG_PRINTF(DBG_AM | DBG_AMSEND, "saw done indication");
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
  CHK_TRUE((tcip = getTxCtxInfo(false /*isWorker*/)) != NULL);

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
             "node %d: AM req execOn ftable[%d]",
             (int) xo->nodeID, (int) xo->fid);
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
  chpl_ftable_call(xo->fid, p);
  if (xo->pDone != NULL) {
    chpl_comm_amDone_t done = 1;
    (void) ofi_put(&done, xo->nodeID, xo->pDone, sizeof(*xo->pDone));
  }
}


static inline
void amHandleGet(chpl_comm_on_bundle_t* req) {
}


static
void amGetWrapper(void* p) {
  chpl_comm_on_bundle_t* req = (chpl_comm_on_bundle_t*) p;
  struct chpl_comm_bundleData_RMA_t* rma = &req->comm.rma;

  DBG_PRINTF(DBG_AM | DBG_AMRECV,
             "AM req GET %p <-- %d:%p (%zd bytes)",
             rma->addr, (int) rma->nodeID, rma->raddr, rma->size);
  CHK_TRUE(mrGetKey(NULL, rma->nodeID, rma->raddr, rma->size) == 0); // sanity
  (void) ofi_get(rma->addr, rma->nodeID, rma->raddr, rma->size);

  CHK_TRUE(mrGetKey(NULL, rma->nodeID, rma->pDone, sizeof(*rma->pDone)) == 0);
  chpl_comm_amDone_t done = 1;
  (void) ofi_put(&done, rma->nodeID, rma->pDone, sizeof(*rma->pDone));
}


static
void amPutWrapper(void* p) {
  chpl_comm_on_bundle_t* req = (chpl_comm_on_bundle_t*) p;
  struct chpl_comm_bundleData_RMA_t* rma = &req->comm.rma;

  DBG_PRINTF(DBG_AM | DBG_AMRECV,
             "AM req PUT %d:%p <-- %p (%zd bytes)",
             (int) rma->nodeID, rma->raddr, rma->addr, rma->size);
  CHK_TRUE(mrGetKey(NULL, rma->nodeID, rma->raddr, rma->size) == 0); // sanity
  (void) ofi_put(rma->addr, rma->nodeID, rma->raddr, rma->size);

  CHK_TRUE(mrGetKey(NULL, rma->nodeID, rma->pDone, sizeof(*rma->pDone)) == 0);
  chpl_comm_amDone_t done = 1;
  (void) ofi_put(&done, rma->nodeID, rma->pDone, sizeof(*rma->pDone));
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

static inline
struct perTxCtxInfo_t* getTxCtxInfo(chpl_bool isWorker) {
  static __thread struct perTxCtxInfo_t* tcip;

  if (tcip == NULL) {
    PTHREAD_CHK(pthread_mutex_lock(&pti_mutex));

    if (isWorker) {
      static int tciiw;
      if (tciiw >= ptiTabLen - numAmHandlers) {
        INTERNAL_ERROR_V("out of ptiTab[] entries for workers");
      }
      tcip = &ptiTab[tciiw++];
    } else {
      static int tciia = -1; // needs execution-time init
      if (tciia < 0)
        tciia = numTxCtxs - 1;
      if (tciia < ptiTabLen - numAmHandlers) {
        INTERNAL_ERROR_V("out of ptiTab[] entries for AM handlers");
      }
      tcip = &ptiTab[tciia--];
    }

    PTHREAD_CHK(pthread_mutex_unlock(&pti_mutex));

    DBG_PRINTF(DBG_THREADS, "I have ptiTab[%td]", tcip - ptiTab);
  }

  return tcip;
}


static inline
void releaseTxCtxInfo(struct perTxCtxInfo_t* tcip) {
  //
  // There is nothing to do here unless and until we support threads
  // overloaded on ptiTab[] entries.
  //
}


static /*inline*/
chpl_comm_nb_handle_t ofi_put(void* addr, c_nodeid_t node,
                              void* raddr, size_t size) {
  DBG_PRINTF(DBG_RMA | DBG_RMAWRITE,
             "PUT %d:%p <= %p, size %zd",
             (int) node, raddr, addr, size);

  void* mrDesc;
  void* myAddr = addr;
  if (mrGetLocalDesc(&mrDesc, myAddr, size) != 0) {
    myAddr = allocBounceBuf(size);
    DBG_PRINTF(DBG_RMA | DBG_RMAWRITE, "PUT BB: %p", myAddr);
    CHK_TRUE(mrGetLocalDesc(&mrDesc, myAddr, size) == 0);
    chpl_no_overlap_memcpy(myAddr, addr, size);
  }

  uint64_t mrKey;
  if (mrGetKey(&mrKey, node, raddr, size) == 0) {
    //
    // The remote address is RMA-accessible; PUT directly to it.
    //
    struct perTxCtxInfo_t* tcip;
    CHK_TRUE((tcip = getTxCtxInfo(true /*isWorker*/)) != NULL);

    DBG_PRINTF(DBG_RMA | DBG_RMAWRITE,
               "tx write: %d:%p <= %p, size %zd, key 0x%" PRIx64,
               (int) node, raddr, myAddr, size, mrKey);
    OFI_CHK(fi_write(tcip->txCtx, myAddr, size,
                     mrDesc, ofi_rxAddrs[node], (uint64_t) raddr, mrKey, 0));

    if (tcip->txCtxHasCQ) {
      waitForTxCQ(tcip, 1);
    } else {
      const int count = fi_cntr_read(tcip->txCntr);
      DBG_PRINTF(DBG_ACK, "tx ack counter %d", count);
      tcip->numTxsOut -= count;
    }

    releaseTxCtxInfo(tcip);
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

  void* mrDesc;
  void* myAddr = addr;
  if (mrGetLocalDesc(&mrDesc, myAddr, size) != 0) {
    myAddr = allocBounceBuf(size);
    DBG_PRINTF(DBG_RMA | DBG_RMAREAD, "GET BB: %p", myAddr);
    CHK_TRUE(mrGetLocalDesc(&mrDesc, myAddr, size) == 0);
  }

  uint64_t mrKey;
  if (mrGetKey(&mrKey, node, raddr, size) == 0) {
    //
    // The remote address is RMA-accessible; GET directly from it.
    //
    struct perTxCtxInfo_t* tcip;
    CHK_TRUE((tcip = getTxCtxInfo(true /*isWorker*/)) != NULL);
    DBG_PRINTF(DBG_RMA | DBG_RMAREAD,
               "tx read: %p <= %d:%p, size %zd, key 0x%" PRIx64,
               myAddr, (int) node, raddr, size, mrKey);
    OFI_CHK(fi_read(tcip->txCtx, myAddr, size,
                    mrDesc, ofi_rxAddrs[node], (uint64_t) raddr, mrKey, 0));

    CHK_TRUE(tcip->txCtxHasCQ);
    waitForTxCQ(tcip, 1);

    releaseTxCtxInfo(tcip);
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
    chpl_no_overlap_memcpy(addr, myAddr, size);
    freeBounceBuf(myAddr);
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
               || ret == -FI_EAGAIN);

      if (ret > 0) {
        const int numEvents = ret;
        numRetired += numEvents;
        for (int i = 0; i < numEvents; i++) {
          DBG_PRINTF(DBG_ACK, "CQ ack tx");
        }
      }
    } while (numRetired < numOut);
  }
}


static
void* allocBounceBuf(size_t size) {
  void* p;
  CHPL_CALLOC_SZ(p, 1, size);
  return p;
}


static void freeBounceBuf(void* p) {
  CHPL_FREE(p);
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


void chpl_comm_barrier(const char *msg) {
  chpl_msg(2, "%d: enter barrier for '%s'\n", chpl_nodeID, msg);

  if (chpl_numNodes == 1) {
    return;
  }

  DBG_PRINTF(DBG_BARRIER, "barrier '%s'", (msg == NULL) ? "" : msg);

  if (numAmHandlersActive == 0) {
    // Comm layer setup is not complete yet; use OOB barrier
    chpl_comm_ofi_oob_barrier();
  } else {
    // Use OOB barrier for now, but we can do better in the future
    chpl_comm_ofi_oob_barrier();
  }

  DBG_PRINTF(DBG_BARRIER, "barrier '%s' done", (msg == NULL) ? "" : msg);
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

#endif
