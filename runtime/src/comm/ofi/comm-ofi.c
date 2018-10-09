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
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_errno.h>

#include "comm-ofi-internal.h"


////////////////////////////////////////
//
// Global types and data
//

typedef enum {
  am_opFree = 0,                        // descriptor is free in table
  am_opNil,                             // no-op
  am_opWhatever,                        // whatever
} am_op_t;

struct am_req_info {
  am_op_t op;
  int id;
  int nodeID;
  int* finVal;
};

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

static int numAmLZs;                    // #AM landing zones per node

struct perTxCtxInfo_t {
  int inited;
  int idx;
  char name[5];
  int isAmHandler;
  struct fid_ep* txCtx;
  struct fid_cq* txCQ;
  struct fid_cntr* txCntr;
  int mrSetIdx;
  int tbIdx;
  int numAmReqsTxed;
  int numAmReqsRxed;
  int numReadsTxed;
  int numWritesTxed;
  int numTxsOut;
};

static struct perTxCtxInfo_t* ptiTab;
#if 0
static __thread struct perTxCtxInfo_t* pti;
#endif


static struct iovec ofi_iov_reqs;
static struct fi_msg ofi_msg_reqs;
static struct am_req_info* comm_amReqLZs;

static struct am_req_info* comm_amReqs;

static int comm_amFinFlagsSize;
static int* comm_amFinFlags;

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


////////////////////////////////////////
//
// Forward decls
//

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
        chpl_internal_error_v("%s: %s", #expr, fi_strerror(- _rc));     \
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
static void init_ofiForAms(void);
static void init_ofiForRma(void);
static void init_ofiForMem(void);

static void init_am_handler(void);


void chpl_comm_init(int *argc_p, char ***argv_p) {
  time_init();
  DBG_INIT();
  chpl_comm_ofi_oob_init();
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

  init_am_handler();

  // Initialize the caching layer, if it is active.
  // chpl_cache_init();

}


static
void init_ofi(void) {
  init_ofiFabricDomain();
  init_ofiEp();
  init_ofiExchangeAvInfo();
  init_ofiForAms();
  init_ofiForRma();
  init_ofiForMem();
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
  hints->domain_attr->control_progress = FI_PROGRESS_MANUAL;
  hints->domain_attr->data_progress = FI_PROGRESS_MANUAL;
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

      chpl_internal_error_v("No provider matched for prov_name \"%s\"",
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
  // Compute numbers of outbound and inbound contexts and then create
  // our scalable endpoint.  Each worker thread should get its own
  // transmit context, plus the AM handler needs one to send 'finished'
  // indicators on.  We only need 1 receive context, for requests sent
  // to our our AM handler.
  //
  numTxCtxs = compute_comm_concurrency();

  {
    const struct fi_domain_attr* dom_attr = ofi_info->domain_attr;
    if (numTxCtxs + 1 > dom_attr->max_ep_tx_ctx)
      numTxCtxs = dom_attr->max_ep_tx_ctx - 1;
    CHK_TRUE(numTxCtxs > 0);
    ofi_info->ep_attr->tx_ctx_cnt = numTxCtxs + 1;

    CHK_TRUE(dom_attr->max_ep_rx_ctx >= 1);
    ofi_info->ep_attr->rx_ctx_cnt = 1;
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
  CHPL_CALLOC(ptiTab, numTxCtxs + 1);

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
  for (int i = 0; i < numTxCtxs + 1; i++) {
    OFI_CHK(fi_tx_context(ofi_txEp, i, NULL, &ptiTab[i].txCtx, NULL));
    if (i < numTxCtxs) {
      OFI_CHK(fi_cq_open(ofi_domain, &txCqAttr, &ptiTab[i].txCQ, NULL));
      OFI_CHK(fi_ep_bind(ptiTab[i].txCtx, &ptiTab[i].txCQ->fid, FI_TRANSMIT));
    } else {
      OFI_CHK(fi_cntr_open(ofi_domain, &txCntrAttr, &ptiTab[i].txCntr, NULL));
      OFI_CHK(fi_ep_bind(ptiTab[i].txCtx, &ptiTab[i].txCntr->fid, FI_WRITE));
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
void init_ofiForAms(void) {
  //
  // If the user didn't specify it, compute the number of AM landing
  // zones per thread.  We should have enough that we needn't re-post
  // the multi-receive buffer more often than, say, once per second.
  // We know from the Chapel performance/comm/low-level/many-to-one test
  // that a single Chapel task (a core) cannot initiate more than about
  // 150k "fast" AM requests per second.  So a reasonable low limit is
  // that times the number of nodes in the job.  We also know from that
  // same test that the Chapel runtime comm=ugni AM handler can only
  // handle just over 1.5m "fast" AM requests per second.  Ours cannot
  // achieve that rate yet, but it's a reasonable upper limit.
  //
  {
    const int maxAmsPerSecPerInitiator = 150000;
    numAmLZs = chpl_numNodes * maxAmsPerSecPerInitiator;

    const int maxAmsPerSecPerHandler = 1500000;
    if (numAmLZs > maxAmsPerSecPerHandler)
      numAmLZs = maxAmsPerSecPerHandler;
  }

  //
  // Create space for inbound AM request landing zones.
  //
  CHPL_CALLOC(comm_amReqLZs, numAmLZs);

  //
  // Pre-post multi-receive buffer for inbound AM requests.
  //
  ofi_iov_reqs.iov_base = comm_amReqLZs;
  ofi_iov_reqs.iov_len = numAmLZs * sizeof(*comm_amReqLZs);
  ofi_msg_reqs.msg_iov = &ofi_iov_reqs;
  ofi_msg_reqs.desc = NULL;
  ofi_msg_reqs.iov_count = 1;
  ofi_msg_reqs.addr = FI_ADDR_UNSPEC;
  ofi_msg_reqs.context = NULL;
  ofi_msg_reqs.data = 0x0;
  OFI_CHK(fi_recvmsg(ofi_rxEp, &ofi_msg_reqs, FI_MULTI_RECV));
  DBG_PRINTF(DBG_AM | DBG_AMRECV, "pre-post fi_recvmsg(AMReqs)");

  //
  // Create initiator-side AM request and 'finished' space.
  //
  const int numAmBufs = numTxCtxs;
  CHPL_CALLOC(comm_amReqs, numAmBufs);

  comm_amFinFlagsSize = numAmBufs * sizeof(comm_amFinFlags[0]);
  CHPL_CALLOC(comm_amFinFlags, numAmBufs);

  for (int i = 0; i < numTxCtxs + 1; i++) {
    ptiTab[i].tbIdx = i;
  }
}


static
void init_ofiForRma(void) {
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

static void fini_am_handler(void);
static void fini_ofi(void);


void chpl_comm_pre_task_exit(int all) {
  if (all) {
    chpl_comm_barrier("chpl_comm_pre_task_exit");
    fini_am_handler();
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

  CHPL_FREE(comm_amFinFlags);

  CHPL_FREE(comm_amReqs);
  CHPL_FREE(comm_amReqLZs);

  CHPL_FREE(ofi_rxAddrs);

  OFI_CHK(fi_close(&ofi_rxEp->fid));
  OFI_CHK(fi_close(&ofi_rxCQ->fid));

  for (int i = 0; i < numTxCtxs + 1; i++) {
    OFI_CHK(fi_close(&ptiTab[i].txCtx->fid));
    if (i < numTxCtxs)
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


////////////////////////////////////////
//
// Interface: Active Message support
//

#define num_am_handlers 1

static int am_handler_count;
static atomic_bool am_handlers_please_exit;
static pthread_cond_t amh_startStop_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t amh_startStop_mutex = PTHREAD_MUTEX_INITIALIZER;

struct ofi_am_info {
  c_nodeid_t node;
  c_sublocid_t subloc;
  chpl_bool serial_state; // To prevent creation of new tasks
  chpl_bool fast;
  chpl_bool blocking;
  chpl_fn_int_t fid;
  void* ack;
};


static void am_handler(void *);


static
void init_am_handler(void) {
  atomic_init_bool(&am_handlers_please_exit, false);

  if (num_am_handlers > 0) {
    // Start AM handler thread(s).  Don't proceed from here until at
    // least one is running.
    PTHREAD_CHK(pthread_mutex_lock(&amh_startStop_mutex));

    for (int i = 0; i < num_am_handlers; i++) {
      if (chpl_task_createCommTask(am_handler, NULL) != 0) {
        chpl_internal_error("unable to start AM handler thread");
      }
    }

    // Some AM handler thread we created will release us.
    PTHREAD_CHK(pthread_cond_wait(&amh_startStop_cond,
                                  &amh_startStop_mutex));
    PTHREAD_CHK(pthread_mutex_unlock(&amh_startStop_mutex));
  }
}


static
void fini_am_handler(void) {
  //
  // Tear down the AM handler thread(s).  On node 0, don't proceed from
  // here until the last one has finished (TODO).
  //
  PTHREAD_CHK(pthread_mutex_lock(&amh_startStop_mutex));
  atomic_store_bool(&am_handlers_please_exit, true);
  PTHREAD_CHK(pthread_cond_wait(&amh_startStop_cond,
                                &amh_startStop_mutex));
  PTHREAD_CHK(pthread_mutex_unlock(&amh_startStop_mutex));

  atomic_destroy_bool(&am_handlers_please_exit);
}


#if 0
static void handle_am(struct fi_cq_data_entry*);
#endif

static void execute_on_common(c_nodeid_t, c_sublocid_t,
                              chpl_fn_int_t,
                              chpl_comm_on_bundle_t*, size_t,
                              chpl_bool, chpl_bool);


int chpl_comm_numPollingTasks(void) { return 1; }


void chpl_comm_make_progress(void) { }


/*
 * The AM handler runs this.
 */
static void am_handler(void *argNil) {
#if 0
  struct progress_thread_info* pti = args;
  const int id = pti->id;
  const int num_rbufs = 2;
  struct iovec iov[num_rbufs];
  struct fi_msg msg[num_rbufs];
  struct ofi_am_info* dst_buf[num_rbufs];
  const int rbuf_len = 10;
  const size_t rbuf_size = rbuf_len*sizeof(dst_buf[0][0]);
  const int num_cqes = rbuf_len;
  struct fi_cq_data_entry cqes[num_cqes];
  int num_read;

  int i;

  for (i = 0; i < num_rbufs; i++) {
    dst_buf[i] = chpl_mem_allocMany(rbuf_len, sizeof(dst_buf[i][0]),
                                    CHPL_RT_MD_COMM_UTIL, 0, 0);
    iov[i].iov_base = dst_buf[i];
    iov[i].iov_len = rbuf_size;
    msg[i].msg_iov = &iov[i];
    msg[i].desc = (void **) fi_mr_desc(ofi.mr);
    msg[i].iov_count = 1;
    msg[i].addr = FI_ADDR_UNSPEC;
    msg[i].context = (void *) (uint64_t) i;
    msg[i].data = 0x0;
    OFI_CHK(fi_recvmsg(ofi.am_rx_ep[id], &msg[i], FI_MULTI_RECV));
  }
#endif

  // Count this AM handler thread as running.  The creator thread
  // wants to be released as soon as at least one AM handler thread
  // is running, so if we're the first, do that.
  PTHREAD_CHK(pthread_mutex_lock(&amh_startStop_mutex));
  if (++am_handler_count == 1)
    PTHREAD_CHK(pthread_cond_signal(&amh_startStop_cond));
  PTHREAD_CHK(pthread_mutex_unlock(&amh_startStop_mutex));

  // Wait for events
  while (!atomic_load_bool(&am_handlers_please_exit)) {
#if 0
    num_read = fi_cq_read(ofi.am_rx_cq[id], cqes, num_cqes);
    if (num_read > 0) {
      for (i = 0; i < num_read; i++) {
        handle_am(&cqes[i]);
        // send ack
      }
    } else {
      if (num_read != -FI_EAGAIN) {
        chpl_internal_error(fi_strerror(-num_read));
      }
    }
#else
    sched_yield();
#endif
  }

  // Un-count this AM handler thread.  Whoever told us to exit wants to
  // be released once all the AM handler threads are done, so if we're
  // the last, do that.
  PTHREAD_CHK(pthread_mutex_lock(&amh_startStop_mutex));
  if (--am_handler_count == 0)
    PTHREAD_CHK(pthread_cond_signal(&amh_startStop_cond));
  PTHREAD_CHK(pthread_mutex_unlock(&amh_startStop_mutex));
}


void chpl_comm_execute_on(c_nodeid_t node, c_sublocid_t subloc,
                          chpl_fn_int_t fid,
                          chpl_comm_on_bundle_t *arg, size_t arg_size) {
  assert(node != chpl_nodeID); // handled by the locale model

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_executeOn, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, arg_size}};
    chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_printf("remote task created on %d", (int) node);
  chpl_comm_diags_incr(execute_on);

  execute_on_common(node, subloc, fid, arg, arg_size, false, true);
}


#ifdef BLAH
static void fork_nb_wrapper(chpl_comm_on_bundle_t *f) {
  chpl_ftable_call(f->task_bundle.requested_fid, f);
}
#endif


void chpl_comm_execute_on_nb(c_nodeid_t node, c_sublocid_t subloc,
                             chpl_fn_int_t fid,
                             chpl_comm_on_bundle_t *arg, size_t arg_size) {
  assert(node != chpl_nodeID); // handled by the locale model

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_nb)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_executeOn_nb, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, arg_size}};
    chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_printf("remote non-blocking task created on %d",
                                 (int) node);
  chpl_comm_diags_incr(execute_on_nb);

  execute_on_common(node, subloc, fid, arg, arg_size, false, false);
}


void chpl_comm_execute_on_fast(c_nodeid_t node, c_sublocid_t subloc,
                               chpl_fn_int_t fid,
                               chpl_comm_on_bundle_t *arg, size_t arg_size) {
  assert(node != chpl_nodeID); // handled by the locale model

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_fast)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_executeOn_fast, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, arg_size}};
    chpl_comm_do_callbacks (&cb_data);
  }

  chpl_comm_diags_verbose_printf("remote (no-fork) task created on %d",
                                 (int) node);
  chpl_comm_diags_incr(execute_on_fast);

  execute_on_common(node, subloc, fid, arg, arg_size, true, true);
}


static void execute_on_common(c_nodeid_t node, c_sublocid_t subloc,
                              chpl_fn_int_t fid,
                              chpl_comm_on_bundle_t* arg, size_t arg_size,
                              chpl_bool fast, chpl_bool blocking) {
  chpl_internal_error("Remote ons not yet implemented");

  // bundle args
  // send to remote AM port
}


#if 0
static void handle_am(struct fi_cq_data_entry* cqe) {

}
#endif


////////////////////////////////////////
//
// Interface: RDMA support
//


#if 0
static __thread int sep_index = -1;
static inline int get_sep_index(int num_ctxs) {
  if (sep_index == -1) {
    sep_index = chpl_task_getId() % num_ctxs;
  }
  return sep_index;
}
#endif


static inline chpl_comm_nb_handle_t ofi_put(chpl_comm_cb_event_kind_t etype,
                                            void *addr, c_nodeid_t node,
                                            void* raddr, size_t size,
                                            int32_t typeIndex, int32_t commID,
                                            int ln, int32_t fn);

static inline chpl_comm_nb_handle_t ofi_get(chpl_comm_cb_event_kind_t etype,
                                            void *addr, c_nodeid_t node,
                                            void* raddr, size_t size,
                                            int32_t typeIndex, int32_t commID,
                                            int ln, int32_t fn);


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
    assert(h[i] == NULL);
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
    assert(h[i] == NULL);
  }
  return 0;
}


void chpl_comm_put(void* addr, c_nodeid_t node, void* raddr,
                   size_t size, int32_t typeIndex, int32_t commID,
                   int ln, int32_t fn) {
  chpl_comm_nb_handle_t handle;

  if (node == chpl_nodeID) {
    memmove(raddr, addr, size);
    return;
  }

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

  handle = ofi_put(chpl_comm_cb_event_kind_put,
                   addr, node, raddr, size, typeIndex, commID, ln, fn);
  if (handle) {
    // fi_cq_read
  }
}


void chpl_comm_get(void* addr, int32_t node, void* raddr,
                   size_t size, int32_t typeIndex, int32_t commID,
                   int ln, int32_t fn) {
  chpl_comm_nb_handle_t handle;

  if (node == chpl_nodeID) {
    memmove(addr, raddr, size);
    return;
  }

  chpl_comm_diags_verbose_printf("%s:%d: remote get from %d",
                                 chpl_lookupFilename(fn), ln, (int) node);
  chpl_comm_diags_incr(get);

  handle = ofi_get(chpl_comm_cb_event_kind_get,
                   addr, node, raddr, size, typeIndex, commID, ln, fn);
  if (handle) {
    // fi_cq_read
  }
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


static inline chpl_comm_nb_handle_t ofi_put(chpl_comm_cb_event_kind_t etype,
                                            void *addr, c_nodeid_t node,
                                            void* raddr, size_t size,
                                            int32_t typeIndex, int32_t commID,
                                            int ln, int32_t fn) {

  if (chpl_comm_have_callbacks(etype)) {
    chpl_comm_cb_info_t cb_data = {etype, chpl_nodeID, node,
                                   .iu.comm={addr, raddr, size,
                                             typeIndex, commID, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }

  // fi_write

  chpl_internal_error("Remote puts not yet implemented");

  return NULL;
}


static inline chpl_comm_nb_handle_t ofi_get(chpl_comm_cb_event_kind_t etype,
                                            void *addr, c_nodeid_t node,
                                            void* raddr, size_t size,
                                            int32_t typeIndex, int32_t commID,
                                            int ln, int32_t fn) {
  if (chpl_comm_have_callbacks(etype)) {
    chpl_comm_cb_info_t cb_data = {etype, chpl_nodeID, node,
                                   .iu.comm={addr, raddr, size,
                                             typeIndex, commID, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }

  // fi_read

  chpl_internal_error("Remote gets not yet implemented");

  return NULL;
}


////////////////////////////////////////
//
// Interface: utility
//

int chpl_comm_addr_gettable(c_nodeid_t node, void* start, size_t len) {
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

  if (am_handler_count == 0) {
    // Comm layer setup is not complete yet; use OOB barrier
    chpl_comm_ofi_oob_barrier();
  } else {
    // Use OOB barrier for now, but we can do better in the future
    chpl_comm_ofi_oob_barrier();
  }

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
