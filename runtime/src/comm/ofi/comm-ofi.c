/*
 * Copyright 2004-2017 Cray Inc.
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

#include "chplrt.h"
#include "chpl-env-gen.h"

#include "chpl-comm.h"
#include "chpl-comm-callbacks.h"
#include "chpl-comm-callbacks-internal.h"
#include "chpl-mem.h"
// #include "chpl-cache.h"
#include "chpl-tasks.h"
#include "chpl-gen-includes.h"
#include "chplsys.h"
#include "chplexit.h"
#include "error.h"

// Don't get warning macros for chpl_comm_get etc
#include "chpl-comm-no-warning-macros.h"

#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <sys/uio.h> /* for struct iovec */

#include "comm-ofi-internal.h"

// same as ofi.num_am_ctx, for now
#define num_progress_threads 1
struct progress_thread_info {
  int id;
};
static struct progress_thread_info pti[num_progress_threads];
static volatile chpl_bool progress_threads_please_exit = false;
static atomic_uint_least32_t progress_thread_count;
static void progress_thread(void *);
static inline chpl_bool progress_threads_running(void);
static inline chpl_bool progress_threads_running() {
  if (atomic_load_uint_least32_t(&progress_thread_count) == num_progress_threads) {
    return true;
  } else {
    return false;
  }
}

static struct ofi_stuff ofi;

int chpl_comm_addr_gettable(c_nodeid_t node, void* start, size_t len)
{
  // No way to know if the page is mapped on the remote (without a round trip)
  return 0;
}

int32_t chpl_comm_getMaxThreads(void) {
  // no limit
  return 0;
}

static void oob_init(void);

void chpl_comm_init(int *argc_p, char ***argv_p) {
  atomic_init_uint_least32_t(&progress_thread_count, 0);

  oob_init();
  chpl_resetCommDiagnosticsHere();
}

#ifdef CHPL_TARGET_PLATFORM_CRAY_XC
#include <pmi.h>
#endif

/*
 * Global, out-of-band initialization
 */
static void oob_init() {
#ifdef CHPL_TARGET_PLATFORM_CRAY_XC

#define USE_PMI2
#ifdef USE_PMI2
  int spawned, size, rank, appnum;

  if (PMI2_Initialized() != PMI_TRUE) {
    if (PMI2_Init(&spawned, &size, &rank, &appnum) != PMI_SUCCESS) {
      chpl_internal_error("PMI2_Init failed");
    }
    assert(spawned == 0);
    chpl_nodeID = (int32_t) rank;
    chpl_numNodes = (int32_t) size;
  }

#else
  PMI_BOOL initialized, spawned;
  int rank, app_size;

  if (PMI_Initialized(&initialized) != PMI_SUCCESS) {
    chpl_internal_error("PMI_Initialized() failed");
  }
  if (initialized != PMI_TRUE && PMI_Init(&spawned) != PMI_SUCCESS) {
    chpl_internal_error("PMI_Init() failed");
  }

  if (PMI_Get_rank_in_app(&rank) != PMI_SUCCESS) {
    chpl_internal_error("PMI_Get_rank_in_app() failed");
  }
  chpl_nodeID = (int32_t) rank;

  if (PMI_Get_size(&app_size) != PMI_SUCCESS) {
      chpl_internal_error("PMI_Get_size() failed");
  }
  chpl_numNodes = (int32_t) app_size;

#endif

#else /* CHPL_TARGET_PLATFORM_CRAY_XC */
#error "Global initialization not supported"
#endif /* CHPL_TARGET_PLATFORM_CRAY_XC */
}

void chpl_comm_post_mem_init(void) { }

//
// No support for gdb for now
//
int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
  return 0;
}

static int get_comm_concurrency(void);
static void libfabric_init(void);

void chpl_comm_post_task_init(void) {
  int i;

  if (chpl_numNodes == 1) {
    // return;
  }

  libfabric_init();
  ofi_put_get_init(&ofi);
  ofi_am_init(&ofi);

  // Start progress thread(s)
  for (i = 0; i < num_progress_threads; i++) {
    pti[i]. id = i;
    if (chpl_task_createCommTask(progress_thread, (void *) &pti[i]) != 0) {
      chpl_internal_error("unable to start progress thread");
    }
  }

  // Initialize the caching layer, if it is active.
  // chpl_cache_init();

}

static int get_comm_concurrency() {
  const char* s;
  int val;
  uint32_t lcpus;

  if ((s = getenv("CHPL_RT_COMM_CONCURRENCY")) != NULL
      && sscanf(s, "%d", &val) == 1) {
    if (val > 0) {
      return val;
    } else if (val == 0) {
      return 1;
    } else {
      chpl_warning("CHPL_RT_COMM_CONCURRENCY < 0, ignored", 0, 0);
    }
  }

  if ((s = getenv("CHPL_RT_NUM_HARDWARE_THREADS")) != NULL
      && sscanf(s, "%d", &val) == 1) {
    if (val > 0) {
      return val;
    } else {
      chpl_warning("CHPL_RT_NUM_HARDWARE_THREADS <= 0, ignored", 0, 0);
    }
  }

  if ((lcpus = chpl_getNumLogicalCpus(true)) > 0) {
    return lcpus;
  }

  chpl_warning("Could not determine comm concurrency, using 1", 0, 0);
  return 1;
}

static void libfabric_init() {
  int i, j;
  struct fi_info *info = NULL;
  struct fi_info *hints = fi_allocinfo();
  struct fi_av_attr av_attr = {0};
  struct fi_cq_attr cq_attr = {0};
  int max_tx_ctx, max_rx_ctx;
  int comm_concurrency;
  int rx_ctx_cnt;
  int rx_ctx_bits = 0;
  struct gather_info* my_addr_info;
  void* addr_infos;
  void* addrs;
  char* ta;
  char* tai;
  size_t my_addr_len=0;
  size_t addr_info_len;

  hints->mode = ~0;

  hints->caps = FI_RMA
             | FI_ATOMIC
             | FI_SOURCE /* do we want this? */
             | FI_READ
             | FI_WRITE
             | FI_REMOTE_READ
             | FI_REMOTE_WRITE
             | FI_MULTI_RECV
             | FI_FENCE;

  hints->addr_format = FI_FORMAT_UNSPEC;

  // memory layer hasn't been initialized, need to use the system allocator
#include "chpl-mem-no-warning-macros.h"
#ifdef CHPL_COMM_SUBSTRATE_SOCKETS
  hints->fabric_attr->prov_name = strdup("sockets");
#else
#ifdef CHPL_COMM_SUBSTRATE_GNI
#error "Substrate GNI not supported"
#else
#error "Substrate type not supported"
#endif
#endif
#include "chpl-mem-warning-macros.h"

  /* connectionless reliable */
  hints->ep_attr->type = FI_EP_RDM;

  hints->domain_attr->threading = FI_THREAD_UNSPEC;
  hints->domain_attr->control_progress = FI_PROGRESS_MANUAL;
  hints->domain_attr->data_progress = FI_PROGRESS_MANUAL;
  hints->domain_attr->av_type = FI_AV_TABLE;
  hints->domain_attr->mr_mode = FI_MR_SCALABLE;
  hints->domain_attr->resource_mgmt = FI_RM_ENABLED;
  // hints->domain_attr->cq_data_size

  hints->tx_attr->op_flags = FI_COMPLETION;
  hints->rx_attr->op_flags = FI_COMPLETION;

  OFICHKERR(fi_getinfo(FI_VERSION(1,0), NULL, NULL, 0, hints, &info));

  if (info == NULL) {
    chpl_internal_error("No fabrics detected.");
  } else {
#ifdef PRINT_FI_GETINFO
    struct fi_info *cur;
    for (cur = info; cur; cur = cur->next) {
      printf("---\n");
      printf("%s", fi_tostr(cur, FI_TYPE_INFO));
    }
    printf("\n");
#endif
  }

  ofi.num_am_ctx = 1; // Would we ever want more?

  max_tx_ctx = info->domain_attr->max_ep_tx_ctx;
  max_rx_ctx = info->domain_attr->max_ep_rx_ctx;
  comm_concurrency = get_comm_concurrency();

  ofi.num_tx_ctx = comm_concurrency+ofi.num_am_ctx > max_tx_ctx ?
    max_tx_ctx-ofi.num_am_ctx : comm_concurrency;
  ofi.num_rx_ctx = comm_concurrency+ofi.num_am_ctx > max_rx_ctx ?
    max_rx_ctx-ofi.num_am_ctx : comm_concurrency;

  info->ep_attr->tx_ctx_cnt = ofi.num_tx_ctx + ofi.num_am_ctx;
  info->ep_attr->rx_ctx_cnt = ofi.num_rx_ctx + ofi.num_am_ctx;

  OFICHKERR(fi_fabric(info->fabric_attr, &ofi.fabric, NULL));
  OFICHKERR(fi_domain(ofi.fabric, info, &ofi.domain, NULL));

  rx_ctx_cnt = ofi.num_rx_ctx + ofi.num_am_ctx;
  while (rx_ctx_cnt >> ++rx_ctx_bits);
  av_attr.rx_ctx_bits = rx_ctx_bits;
  av_attr.type = FI_AV_TABLE;
  av_attr.count = chpl_numNodes;
  OFICHKERR(fi_av_open(ofi.domain, &av_attr, &ofi.av, NULL));

  OFICHKERR(fi_scalable_ep(ofi.domain, info, &ofi.ep, NULL));
  OFICHKERR(fi_scalable_ep_bind(ofi.ep, &ofi.av->fid, 0));

  /* set up tx and rx contexts */
  cq_attr.format = FI_CQ_FORMAT_CONTEXT;
  cq_attr.size = 1024; /* ??? */
  cq_attr.wait_obj = FI_WAIT_UNSPEC;
  ofi.tx_ep = (struct fid_ep **) chpl_mem_allocMany(ofi.num_tx_ctx,
						   sizeof(ofi.tx_ep[0]),
						   CHPL_RT_MD_COMM_PER_LOC_INFO,
						   0, 0);
  ofi.tx_cq = (struct fid_cq **) chpl_mem_allocMany(ofi.num_tx_ctx,
						   sizeof(ofi.tx_cq[0]),
						   CHPL_RT_MD_COMM_PER_LOC_INFO,
						   0, 0);
  for (i = 0; i < ofi.num_tx_ctx; i++) {
    OFICHKERR(fi_tx_context(ofi.ep, i, NULL, &ofi.tx_ep[i], NULL));
    OFICHKERR(fi_cq_open(ofi.domain, &cq_attr, &ofi.tx_cq[i], NULL));
    OFICHKERR(fi_ep_bind(ofi.tx_ep[i], &ofi.tx_cq[i]->fid, FI_TRANSMIT));
    OFICHKERR(fi_enable(ofi.tx_ep[i]));
  }

  ofi.rx_ep = (struct fid_ep **) chpl_mem_allocMany(ofi.num_rx_ctx,
						   sizeof(ofi.rx_ep[0]),
						   CHPL_RT_MD_COMM_PER_LOC_INFO,
						   0, 0);
  ofi.rx_cq = (struct fid_cq **) chpl_mem_allocMany(ofi.num_rx_ctx,
						    sizeof(ofi.rx_cq[0]),
						   CHPL_RT_MD_COMM_PER_LOC_INFO,
						   0, 0);
  for (i = 0; i < ofi.num_rx_ctx; i++) {
    OFICHKERR(fi_rx_context(ofi.ep, i, NULL, &ofi.rx_ep[i], NULL));
    OFICHKERR(fi_cq_open(ofi.domain, &cq_attr, &ofi.rx_cq[i], NULL));
    OFICHKERR(fi_ep_bind(ofi.rx_ep[i], &ofi.rx_cq[i]->fid, FI_RECV));
    OFICHKERR(fi_enable(ofi.rx_ep[i]));
  }

  ofi.am_tx_ep = (struct fid_ep **) chpl_mem_allocMany(ofi.num_am_ctx,
						       sizeof(ofi.am_tx_ep[0]),
						       CHPL_RT_MD_COMM_PER_LOC_INFO,
						       0, 0);
  ofi.am_tx_cq = (struct fid_cq **) chpl_mem_allocMany(ofi.num_am_ctx,
						      sizeof(ofi.am_tx_cq[0]),
						      CHPL_RT_MD_COMM_PER_LOC_INFO,
						      0, 0);

  /* set up AM contexts */
  for (i = 0; i < ofi.num_am_ctx; i++) {
    OFICHKERR(fi_tx_context(ofi.ep, i+ofi.num_tx_ctx, NULL, &ofi.am_tx_ep[i], NULL));
    OFICHKERR(fi_cq_open(ofi.domain, &cq_attr, &ofi.am_tx_cq[i], NULL));
    OFICHKERR(fi_ep_bind(ofi.am_tx_ep[i], &ofi.am_tx_cq[i]->fid, FI_TRANSMIT));
    OFICHKERR(fi_enable(ofi.am_tx_ep[i]));
  }

  ofi.am_rx_ep = (struct fid_ep **) chpl_mem_allocMany(ofi.num_am_ctx,
						       sizeof(ofi.am_rx_ep[0]),
						       CHPL_RT_MD_COMM_PER_LOC_INFO,
						       0, 0);
  ofi.am_rx_cq = (struct fid_cq **) chpl_mem_allocMany(ofi.num_am_ctx,
						      sizeof(ofi.am_rx_cq[0]),
						      CHPL_RT_MD_COMM_PER_LOC_INFO,
						      0, 0);
  for (i = 0; i < ofi.num_am_ctx; i++) {
    OFICHKERR(fi_rx_context(ofi.ep, i+ofi.num_rx_ctx, NULL, &ofi.am_rx_ep[i], NULL));
    OFICHKERR(fi_cq_open(ofi.domain, &cq_attr, &ofi.am_rx_cq[i], NULL));
    OFICHKERR(fi_ep_bind(ofi.am_rx_ep[i], &ofi.am_rx_cq[i]->fid, FI_RECV));
    OFICHKERR(fi_enable(ofi.am_rx_ep[i]));
  }

  OFICHKERR(fi_enable(ofi.ep));

  // Set up address vector
  // Assumes my_addr_len is the same on all nodes
  OFICHKRET(fi_getname(&ofi.ep->fid, NULL, &my_addr_len), -FI_ETOOSMALL);
  addr_info_len = sizeof(struct gather_info) + my_addr_len;
  my_addr_info =  chpl_mem_allocMany(1, addr_info_len,
				     CHPL_RT_MD_COMM_PER_LOC_INFO,
				     0, 0);
  my_addr_info->node = chpl_nodeID;
  OFICHKERR(fi_getname(&ofi.ep->fid, &my_addr_info->info, &my_addr_len));

  addr_infos =  chpl_mem_allocMany(chpl_numNodes, addr_info_len,
				   CHPL_RT_MD_COMM_PER_LOC_INFO,
				   0, 0);

#ifdef CHPL_TARGET_PLATFORM_CRAY_XC
  // Use PMI_AllGather
  if (PMI_Allgather(my_addr_info, addr_infos, addr_info_len) != PMI_SUCCESS) {
    chpl_internal_error("PMI_Allgather() failed");
  }

  addrs =  chpl_mem_allocMany(chpl_numNodes, my_addr_len,
			      CHPL_RT_MD_COMM_PER_LOC_INFO,
			      0, 0);

  for (tai = addr_infos, ta = addrs, i = 0; i < chpl_numNodes; i++) {
    struct gather_info* ai = (struct gather_info*) tai;
    assert(i >= 0);
    assert(i < chpl_numNodes);
    memcpy(ta, ai->info, my_addr_len);
    tai += addr_info_len;
    ta += my_addr_len;
  }

#else /* CHPL_TARGET_PLATFORM_CRAY_XC */
#error "Global address exchange not supported"
#endif

  ofi.fi_addrs = chpl_mem_allocMany(chpl_numNodes, sizeof(ofi.fi_addrs[0]),
				    CHPL_RT_MD_COMM_PER_LOC_INFO,
				    0, 0);
  OFICHKRET(fi_av_insert(ofi.av, addrs, chpl_numNodes,
			 ofi.fi_addrs, 0, NULL), chpl_numNodes);

  ofi.rx_addrs = chpl_mem_allocMany(chpl_numNodes, sizeof(ofi.rx_addrs[0]),
				    CHPL_RT_MD_COMM_PER_LOC_INFO,
				    0, 0);
  for (i = 0; i < chpl_numNodes; i++) {
    ofi.rx_addrs[i] = chpl_mem_allocMany(rx_ctx_cnt, sizeof(ofi.rx_addrs[i][0]),
					 CHPL_RT_MD_COMM_PER_LOC_INFO,
					 0, 0);
    for (j = 0; j < rx_ctx_cnt; j++) {
      ofi.rx_addrs[i][j] = fi_rx_addr(ofi.fi_addrs[i], j, rx_ctx_bits);
    }
  }

  chpl_mem_free(my_addr_info, 0, 0);
  chpl_mem_free(addr_infos, 0, 0);
  chpl_mem_free(addrs, 0, 0);

  OFICHKERR(fi_mr_reg(ofi.domain, 0, SIZE_MAX,
		      FI_READ | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE |
		      FI_SEND | FI_RECV, 0,
		      (uint64_t) chpl_nodeID, 0, &ofi.mr, NULL));


  fi_freeinfo(info);  /* No error returned */
  fi_freeinfo(hints); /* No error returned */

  chpl_msg(2, "%d: completed libfabric initialization\n", chpl_nodeID);
}

void chpl_comm_rollcall(void) {
  // Do this again to clear out any comms that happened during initialization
  chpl_resetCommDiagnosticsHere();
  chpl_msg(2, "executing on node %d of %d node(s): %s\n", chpl_nodeID, 
           chpl_numNodes, chpl_nodeName());
}

void chpl_comm_desired_shared_heap(void** start_p, size_t* size_p) {
  // Use of FI_MR_SCALABLE could cover entire address space

  *start_p = NULL;
  *size_p  = 0;
}

void chpl_comm_broadcast_global_vars(int numGlobals) {
#ifdef CHPL_TARGET_PLATFORM_CRAY_XC
  // Use PMI_AllGather
#else /* CHPL_TARGET_PLATFORM_CRAY_XC */
  int i;
  if (chpl_nodeID != 0) {
    for (i = 0; i < numGlobals; i++) {
      chpl_comm_get(chpl_globals_registry[i], 0, chpl_globals_registry[i],
                    sizeof(wide_ptr_t), -1 /*typeIndex: unused*/, 0, 0);
    }
  }
#endif /* CHPL_TARGET_PLATFORM_CRAY_XC */
}

void chpl_comm_broadcast_private(int id, size_t size, int32_t tid) {
#ifdef CHPL_TARGET_PLATFORM_CRAY_XC
  // Use PMI_AllGather
#else /* CHPL_TARGET_PLATFORM_CRAY_XC */
  int i;
  for (i = 0; i < chpl_numNodes; i++) {
    if (i != chpl_nodeID) {
      do_remote_put(chpl_private_broadcast_table[id], i,
                    chpl_private_broadcast_table[id], size,
                    may_proxy_true);
    }
  }
#endif /* CHPL_TARGET_PLATFORM_CRAY_XC */
}

void chpl_comm_barrier(const char *msg) {
  chpl_msg(2, "%d: enter barrier for '%s'\n", chpl_nodeID, msg);

  if (chpl_numNodes == 1) {
    return;
  }

  if (!progress_threads_running()) {
    // Comm layer setup is not complete yet
#ifdef CHPL_TARGET_PLATFORM_CRAY_XC
    if (PMI_Barrier() != PMI_SUCCESS) {
      chpl_internal_error("PMI_Barrier failed");
    }
#else /* CHPL_TARGET_PLATFORM_CRAY_XC */
#error "Out-of-band barrier not yet implemented"
#endif /* CHPL_TARGET_PLATFORM_CRAY_XC */
  } else {
    //  Use PMI_Barrier() for now
#ifdef CHPL_TARGET_PLATFORM_CRAY_XC
    if (PMI_Barrier() != PMI_SUCCESS) {
      chpl_internal_error("PMI_Barrier failed");
    }
#else /* CHPL_TARGET_PLATFORM_CRAY_XC */
#error "Out-of-band barrier not yet implemented"
#endif /* CHPL_TARGET_PLATFORM_CRAY_XC */
  }

}

void chpl_comm_pre_task_exit(int all) {
  // Tear down the progress thread
}

static void exit_all(int status);
static void exit_any(int status);
void chpl_comm_exit(int all, int status) {
  if (all) {
    exit_all(status);
  } else {
    exit_any(status);
  }
}

static void exit_all(int status) {
  int i;

  for (i = 0; i < ofi.num_tx_ctx; i++) {
    OFICHKERR(fi_close(&ofi.tx_ep[i]->fid));
    OFICHKERR(fi_close(&ofi.tx_cq[i]->fid));
  }

  for (i = 0; i < ofi.num_rx_ctx; i++) {
    OFICHKERR(fi_close(&ofi.rx_ep[i]->fid));
    OFICHKERR(fi_close(&ofi.rx_cq[i]->fid));
  }

  for (i = 0; i < ofi.num_am_ctx; i++) {
    OFICHKERR(fi_close(&ofi.am_tx_ep[i]->fid));
    OFICHKERR(fi_close(&ofi.am_tx_cq[i]->fid));
    OFICHKERR(fi_close(&ofi.am_rx_ep[i]->fid));
    OFICHKERR(fi_close(&ofi.am_rx_cq[i]->fid));
  }

  OFICHKERR(fi_close(&ofi.ep->fid));
  OFICHKERR(fi_close(&ofi.av->fid));
  OFICHKERR(fi_close(&ofi.mr->fid));
  OFICHKERR(fi_close(&ofi.domain->fid));
  OFICHKERR(fi_close(&ofi.fabric->fid));

  chpl_mem_free(ofi.fi_addrs, 0, 0);
  for (i = 0; i < chpl_numNodes; i++) {
    chpl_mem_free(ofi.rx_addrs[i], 0, 0);
  }
  chpl_mem_free(ofi.rx_addrs, 0, 0);
  chpl_mem_free(ofi.tx_ep, 0, 0);
  chpl_mem_free(ofi.tx_cq, 0, 0);
  chpl_mem_free(ofi.rx_ep, 0, 0);
  chpl_mem_free(ofi.rx_cq, 0, 0);
  chpl_mem_free(ofi.am_tx_ep, 0, 0);
  chpl_mem_free(ofi.am_tx_cq, 0, 0);
  chpl_mem_free(ofi.am_rx_ep, 0, 0);
  chpl_mem_free(ofi.am_rx_cq, 0, 0);

#ifdef CHPL_TARGET_PLATFORM_CRAY_XC

#ifdef USE_PMI2
  if (PMI2_Initialized() != PMI_TRUE) {
    if (PMI2_Finalize() != PMI_SUCCESS) {
      chpl_internal_error("PMI2_Finalize failed");
    }
  }
#else
  PMI_BOOL initialized;

  if (PMI_Initialized(&initialized) != PMI_SUCCESS) {
    chpl_internal_error("PMI_Initialized failed");
  }
  if ((initialized == PMI_TRUE) && (PMI_Finalize() != PMI_SUCCESS)) {
    chpl_internal_error("PMI_Finalize failed");
  }

#endif

#else /* CHPL_TARGET_PLATFORM_CRAY_XC */
#error "Global teardown not supported"
#endif /* CHPL_TARGET_PLATFORM_CRAY_XC */

}

static void exit_any(int status) {
  // Should we tear down the progress thread?
}

int chpl_comm_numPollingTasks(void) { return 1; }

void chpl_comm_make_progress(void) { }

// In comm-ofi-am.c
void am_handler(struct fi_cq_data_entry* cqe);

/*
 * Set up the progress thread
 */
static void progress_thread(void *args) {
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
				    CHPL_RT_MD_COMM_PER_LOC_INFO, 0, 0);
    iov[i].iov_base = dst_buf[i];
    iov[i].iov_len = rbuf_size;
    msg[i].msg_iov = &iov[i];
    msg[i].desc = (void **) fi_mr_desc(ofi.mr);
    msg[i].iov_count = 1;
    msg[i].addr = FI_ADDR_UNSPEC;
    msg[i].context = (void *) (uint64_t) i;
    msg[i].data = 0x0;
    OFICHKERR(fi_recvmsg(ofi.am_rx_ep[id], &msg[i], FI_MULTI_RECV));
  }

  atomic_fetch_add_uint_least32_t(&progress_thread_count, 1);

  // Wait for events
  while (!progress_threads_please_exit) {
    num_read = fi_cq_read(ofi.rx_cq[id], cqes, num_cqes);
    if (num_read > 0) {
      for (i = 0; i < num_read; i++) {
	ofi_am_handler(&cqes[i]);
	// send ack
      }
    } else {
      if (num_read != -FI_EAGAIN) {
	chpl_internal_error(fi_strerror(-num_read));
      }      
    }
  }

  atomic_fetch_sub_uint_least32_t(&progress_thread_count, 1);

}
