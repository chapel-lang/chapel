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

#include "chplrt.h"
#include "chpl-env-gen.h"

#include "chpl-comm.h"
#include "chpl-comm-callbacks.h"
#include "chpl-comm-callbacks-internal.h"
#include "chpl-comm-diags.h"
#include "chpl-comm-strd-xfer.h"
#include "chpl-linefile-support.h"
#include "chpl-mem.h"
#include "chpl-mem-sys.h"
// #include "chpl-cache.h"
#include "chpl-tasks.h"
#include "chpl-topo.h"
#include "chpl-gen-includes.h"
#include "chplsys.h"
#include "chplexit.h"
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

struct ofi_stuff {
  struct fid_fabric* fabric;
  struct fid_domain* domain;
  struct fid_av* av;
  fi_addr_t* fi_addrs;
  fi_addr_t** rx_addrs;
  struct fid_mr* mr;

  struct fid_ep* ep; /* scalable endpoint */
  int rx_ctx_bits;

  /* For async puts/gets */
  int num_tx_ctx;
  struct fid_ep** tx_ep;
  struct fid_cq** tx_cq;
  /* FI_CQ_FORMAT_CONTEXT?, FI_WAIT_UNSPEC, signaling_vector? */
  int num_rx_ctx;
  struct fid_ep** rx_ep;
  struct fid_cq** rx_cq;

  /* For active messages */
  int num_am_ctx;
  struct fid_ep** am_tx_ep;
  struct fid_cq** am_tx_cq;
  struct fid_ep** am_rx_ep;
  struct fid_cq** am_rx_cq;

};

static struct ofi_stuff ofi;


////////////////////////////////////////
//
// Error checking
//

#define OFI_CHK(ofiCall, wantVal)                                       \
    do {                                                                \
      int _rc = (ofiCall);                                              \
      if (_rc != wantVal) {                                             \
        chpl_internal_error_v("%s: %s", #ofiCall, fi_strerror(- _rc));  \
      }                                                                 \
    } while (0)

#define OFI_CHK_SUCCESS(ofiCall) OFI_CHK(ofiCall, FI_SUCCESS)


////////////////////////////////////////
//
// Interface: initialization
//

static void libfabric_init(void);
static int get_comm_concurrency(void);
static void libfabric_init_addrvec(int, int);

static void init_am(void);
static void shutdown_am(void);

static void init_rdma(void);


void chpl_comm_init(int *argc_p, char ***argv_p) {
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
  if (chpl_numNodes == 1) {
    // return;
  }

  libfabric_init();
  init_am();
  init_rdma();

  // Initialize the caching layer, if it is active.
  // chpl_cache_init();

}


static void libfabric_init() {
  int i;
  struct fi_info *info = NULL;
  struct fi_info *hints = fi_allocinfo();
  struct fi_av_attr av_attr = {0};
  struct fi_cq_attr cq_attr = {0};
  int max_tx_ctx, max_rx_ctx;
  int comm_concurrency;
  int rx_ctx_cnt;
  int rx_ctx_bits = 0;

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

#if defined(CHPL_COMM_SUBSTRATE_SOCKETS)
  //
  // fi_freeinfo(hints) will free() hints->fabric_attr->prov_name; this
  // is documented, though poorly.  So, get that space from malloc().
  //
  {
    const char s[] = "sockets";
    char* sDup = sys_malloc(sizeof(s));
    strcpy(sDup, s);
    hints->fabric_attr->prov_name = sDup;
  }
#elif defined(CHPL_COMM_SUBSTRATE_GNI)
#error "Substrate GNI not supported"
#else
#error "Substrate type not supported"
#endif

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

  OFI_CHK_SUCCESS(fi_getinfo(FI_VERSION(1,0), NULL, NULL, 0, hints, &info));

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

  OFI_CHK_SUCCESS(fi_fabric(info->fabric_attr, &ofi.fabric, NULL));
  OFI_CHK_SUCCESS(fi_domain(ofi.fabric, info, &ofi.domain, NULL));

  rx_ctx_cnt = ofi.num_rx_ctx + ofi.num_am_ctx;
  while (rx_ctx_cnt >> ++rx_ctx_bits);
  av_attr.rx_ctx_bits = rx_ctx_bits;
  av_attr.type = FI_AV_TABLE;
  av_attr.count = chpl_numNodes;
  OFI_CHK_SUCCESS(fi_av_open(ofi.domain, &av_attr, &ofi.av, NULL));

  OFI_CHK_SUCCESS(fi_scalable_ep(ofi.domain, info, &ofi.ep, NULL));
  OFI_CHK_SUCCESS(fi_scalable_ep_bind(ofi.ep, &ofi.av->fid, 0));

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
    OFI_CHK_SUCCESS(fi_tx_context(ofi.ep, i, NULL, &ofi.tx_ep[i], NULL));
    OFI_CHK_SUCCESS(fi_cq_open(ofi.domain, &cq_attr, &ofi.tx_cq[i], NULL));
    OFI_CHK_SUCCESS(fi_ep_bind(ofi.tx_ep[i], &ofi.tx_cq[i]->fid, FI_TRANSMIT));
    OFI_CHK_SUCCESS(fi_enable(ofi.tx_ep[i]));
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
    OFI_CHK_SUCCESS(fi_rx_context(ofi.ep, i, NULL, &ofi.rx_ep[i], NULL));
    OFI_CHK_SUCCESS(fi_cq_open(ofi.domain, &cq_attr, &ofi.rx_cq[i], NULL));
    OFI_CHK_SUCCESS(fi_ep_bind(ofi.rx_ep[i], &ofi.rx_cq[i]->fid, FI_RECV));
    OFI_CHK_SUCCESS(fi_enable(ofi.rx_ep[i]));
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
    OFI_CHK_SUCCESS(fi_tx_context(ofi.ep, i+ofi.num_tx_ctx, NULL, &ofi.am_tx_ep[i], NULL));
    OFI_CHK_SUCCESS(fi_cq_open(ofi.domain, &cq_attr, &ofi.am_tx_cq[i], NULL));
    OFI_CHK_SUCCESS(fi_ep_bind(ofi.am_tx_ep[i], &ofi.am_tx_cq[i]->fid, FI_TRANSMIT));
    OFI_CHK_SUCCESS(fi_enable(ofi.am_tx_ep[i]));
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
    OFI_CHK_SUCCESS(fi_rx_context(ofi.ep, i+ofi.num_rx_ctx, NULL, &ofi.am_rx_ep[i], NULL));
    OFI_CHK_SUCCESS(fi_cq_open(ofi.domain, &cq_attr, &ofi.am_rx_cq[i], NULL));
    OFI_CHK_SUCCESS(fi_ep_bind(ofi.am_rx_ep[i], &ofi.am_rx_cq[i]->fid, FI_RECV));
    OFI_CHK_SUCCESS(fi_enable(ofi.am_rx_ep[i]));
  }

  OFI_CHK_SUCCESS(fi_enable(ofi.ep));

  libfabric_init_addrvec(rx_ctx_cnt, rx_ctx_bits);

  OFI_CHK_SUCCESS(fi_mr_reg(ofi.domain, 0, SIZE_MAX,
                      FI_READ | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE |
                      FI_SEND | FI_RECV, 0,
                      (uint64_t) chpl_nodeID, 0, &ofi.mr, NULL));

  fi_freeinfo(info);  /* No error returned */
  fi_freeinfo(hints); /* No error returned */

  chpl_msg(2, "%d: completed libfabric initialization\n", chpl_nodeID);
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

  if ((lcpus = chpl_topo_getNumCPUsLogical(true)) > 0) {
    return lcpus;
  }

  chpl_warning("Could not determine comm concurrency, using 1", 0, 0);
  return 1;
}


static void libfabric_init_addrvec(int rx_ctx_cnt, int rx_ctx_bits) {
  struct gather_info* my_addr_info;
  void* addr_infos;
  char* addrs;
  char* tai;
  size_t my_addr_len;
  size_t addr_info_len;
  int i, j;

  // Assumes my_addr_len is the same on all nodes
  my_addr_len = 0;
  OFI_CHK(fi_getname(&ofi.ep->fid, NULL, &my_addr_len), -FI_ETOOSMALL);
  addr_info_len = sizeof(struct gather_info) + my_addr_len;
  my_addr_info = chpl_mem_alloc(addr_info_len,
                                CHPL_RT_MD_COMM_UTIL,
                                0, 0);
  my_addr_info->node = chpl_nodeID;
  OFI_CHK_SUCCESS(fi_getname(&ofi.ep->fid, &my_addr_info->info, &my_addr_len));

  addr_infos = chpl_mem_allocMany(chpl_numNodes, addr_info_len,
                                  CHPL_RT_MD_COMM_PER_LOC_INFO,
                                  0, 0);

  chpl_comm_ofi_oob_allgather(my_addr_info, addr_infos, addr_info_len);

  addrs = chpl_mem_allocMany(chpl_numNodes, my_addr_len,
                             CHPL_RT_MD_COMM_PER_LOC_INFO,
                             0, 0);

  for (tai = addr_infos, i = 0; i < chpl_numNodes; i++) {
    struct gather_info* ai = (struct gather_info*) tai;
    assert(i >= 0);
    assert(i < chpl_numNodes);
    memcpy(addrs + ai->node * my_addr_len, ai->info, my_addr_len);
    tai += addr_info_len;
  }

  ofi.fi_addrs = chpl_mem_allocMany(chpl_numNodes, sizeof(ofi.fi_addrs[0]),
                                    CHPL_RT_MD_COMM_PER_LOC_INFO,
                                    0, 0);
  OFI_CHK(fi_av_insert(ofi.av, addrs, chpl_numNodes, ofi.fi_addrs, 0, NULL),
          chpl_numNodes);

  ofi.rx_addrs = chpl_mem_allocMany(chpl_numNodes, sizeof(ofi.rx_addrs[0]),
                                    CHPL_RT_MD_COMM_PER_LOC_INFO,
                                    0, 0);
  for (i = 0; i < chpl_numNodes; i++) {
    ofi.rx_addrs[i] = chpl_mem_allocMany(rx_ctx_cnt,
                                         sizeof(ofi.rx_addrs[i][0]),
                                         CHPL_RT_MD_COMM_PER_LOC_INFO,
                                         0, 0);
    for (j = 0; j < rx_ctx_cnt; j++) {
      ofi.rx_addrs[i][j] = fi_rx_addr(ofi.fi_addrs[i], j, rx_ctx_bits);
    }
  }

  chpl_mem_free(my_addr_info, 0, 0);
  chpl_mem_free(addr_infos, 0, 0);
  chpl_mem_free(addrs, 0, 0);
}


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

void chpl_comm_pre_task_exit(int all) {
  if (all) {
    chpl_comm_barrier("chpl_comm_pre_task_exit");
    shutdown_am();
  }
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
    OFI_CHK_SUCCESS(fi_close(&ofi.tx_ep[i]->fid));
    OFI_CHK_SUCCESS(fi_close(&ofi.tx_cq[i]->fid));
  }

  for (i = 0; i < ofi.num_rx_ctx; i++) {
    OFI_CHK_SUCCESS(fi_close(&ofi.rx_ep[i]->fid));
    OFI_CHK_SUCCESS(fi_close(&ofi.rx_cq[i]->fid));
  }

  for (i = 0; i < ofi.num_am_ctx; i++) {
    OFI_CHK_SUCCESS(fi_close(&ofi.am_tx_ep[i]->fid));
    OFI_CHK_SUCCESS(fi_close(&ofi.am_tx_cq[i]->fid));
    OFI_CHK_SUCCESS(fi_close(&ofi.am_rx_ep[i]->fid));
    OFI_CHK_SUCCESS(fi_close(&ofi.am_rx_cq[i]->fid));
  }

  OFI_CHK_SUCCESS(fi_close(&ofi.ep->fid));
  OFI_CHK_SUCCESS(fi_close(&ofi.av->fid));
  OFI_CHK_SUCCESS(fi_close(&ofi.mr->fid));
  OFI_CHK_SUCCESS(fi_close(&ofi.domain->fid));
  OFI_CHK_SUCCESS(fi_close(&ofi.fabric->fid));

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

  chpl_comm_ofi_oob_fini();
}

static void exit_any(int status) {
  // Should we tear down the progress thread?
}


////////////////////////////////////////
//
// Interface: Active Message support
//

#define num_progress_threads 1

struct progress_thread_info {
  int id;
};

static struct progress_thread_info pti[num_progress_threads];

static void progress_thread(void *);

static int progress_thread_count;
static atomic_bool progress_threads_please_exit;
static pthread_cond_t progress_startStop_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t progress_startStop_mutex = PTHREAD_MUTEX_INITIALIZER;

struct ofi_am_info {
  c_nodeid_t node;
  c_sublocid_t subloc;
  chpl_bool serial_state; // To prevent creation of new tasks
  chpl_bool fast;
  chpl_bool blocking;
  chpl_fn_int_t fid;
  void* ack;
};


static void handle_am(struct fi_cq_data_entry*);

static void execute_on_common(c_nodeid_t, c_sublocid_t,
                              chpl_fn_int_t,
                              chpl_comm_on_bundle_t*, size_t,
                              chpl_bool, chpl_bool);


void init_am(void) {
  atomic_init_bool(&progress_threads_please_exit, false);

  if (num_progress_threads > 0) {
    // Start progress thread(s).  Don't proceed from here until at
    // least one is running.
    CHK_EQ_0(pthread_mutex_lock(&progress_startStop_mutex));

    for (int i = 0; i < num_progress_threads; i++) {
      pti[i].id = i;
      if (chpl_task_createCommTask(progress_thread, (void *) &pti[i]) != 0) {
        chpl_internal_error("unable to start progress thread");
      }
    }

    // Some progress thread we created will release us.
    CHK_EQ_0(pthread_cond_wait(&progress_startStop_cond,
                               &progress_startStop_mutex));
    CHK_EQ_0(pthread_mutex_unlock(&progress_startStop_mutex));
  }
}


void shutdown_am(void) {
  //
  // Tear down the progress thread(s).  On node 0, don't proceed from
  // here until the last one has finished (TODO).
  //
  CHK_EQ_0(pthread_mutex_lock(&progress_startStop_mutex));
  atomic_store_bool(&progress_threads_please_exit, true);
  CHK_EQ_0(pthread_cond_wait(&progress_startStop_cond,
                             &progress_startStop_mutex));
  CHK_EQ_0(pthread_mutex_unlock(&progress_startStop_mutex));

  atomic_destroy_bool(&progress_threads_please_exit);
}


int chpl_comm_numPollingTasks(void) { return 1; }


void chpl_comm_make_progress(void) { }


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
    OFI_CHK_SUCCESS(fi_recvmsg(ofi.am_rx_ep[id], &msg[i], FI_MULTI_RECV));
  }

  // Count this progress thread as running.  The creator thread wants to
  // be released as soon as at least one progress thread is running, so
  // if we're the first, do that.
  CHK_EQ_0(pthread_mutex_lock(&progress_startStop_mutex));
  if (++progress_thread_count == 1)
    CHK_EQ_0(pthread_cond_signal(&progress_startStop_cond));
  CHK_EQ_0(pthread_mutex_unlock(&progress_startStop_mutex));

  // Wait for events
  while (!atomic_load_bool(&progress_threads_please_exit)) {
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
  }

  // Un-count this progress thread.  Whoever told us to exit wants to
  // be released once all the progress threads are done, so if we're
  // the last, do that.
  CHK_EQ_0(pthread_mutex_lock(&progress_startStop_mutex));
  if (--progress_thread_count == 0)
    CHK_EQ_0(pthread_cond_signal(&progress_startStop_cond));
  CHK_EQ_0(pthread_mutex_unlock(&progress_startStop_mutex));
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
                              chpl_bool fast, chpl_bool blocking)
{
  chpl_internal_error("Remote ons not yet implemented");

  // bundle args
  // send to remote AM port
}


static void handle_am(struct fi_cq_data_entry* cqe) {

}


////////////////////////////////////////
//
// Interface: RDMA support
//

static void init_rdma(void) {
}


static __thread int sep_index = -1;
static inline int get_sep_index(int num_ctxs) {
  if (sep_index == -1) {
    sep_index = chpl_task_getId() % num_ctxs;
  }
  return sep_index;
}


// Consider making the contexts and cqs thread local variables
static inline struct fid_ep* get_rx_ep(void);
static inline struct fid_ep* get_tx_ep(void);

static inline struct fid_ep* get_rx_ep() {
  return ofi.rx_ep[get_sep_index(ofi.num_rx_ctx)];
}


static inline struct fid_ep* get_tx_ep() {
  return ofi.tx_ep[get_sep_index(ofi.num_tx_ctx)];
}


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


int chpl_comm_test_nb_complete(chpl_comm_nb_handle_t h)
{
  if (chpl_verbose_comm && chpl_comm_diags_is_enabled()) {
    chpl_comm_diags_verbose_printf("test nb complete (%p)", h);
  }
  chpl_comm_diags_incr(test_nb);

  // fi_cq_readfrom?
  return ((void*) h) == NULL;
}


void chpl_comm_wait_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles)
{
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


int chpl_comm_try_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles)
{

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

int chpl_comm_addr_gettable(c_nodeid_t node, void* start, size_t len)
{
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

  if (progress_thread_count == 0) {
    // Comm layer setup is not complete yet; use OOB barrier
    chpl_comm_ofi_oob_barrier();
  } else {
    // Use OOB barrier for now, but we can do better in the future
    chpl_comm_ofi_oob_barrier();
  }

}
