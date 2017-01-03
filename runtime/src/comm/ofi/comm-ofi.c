/*
 * Copyright 2004-2016 Cray Inc.
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

#include "rdma/fabric.h"
#include "rdma/fi_domain.h"
#include "rdma/fi_errno.h"

#include "chpl-comm.h"
#include "chpl-comm-callbacks.h"
#include "chpl-comm-callbacks-internal.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include "chpl-gen-includes.h"
#include "chplsys.h"
#include "chplexit.h"
#include "error.h"

// Don't get warning macros for chpl_comm_get etc
#include "chpl-comm-no-warning-macros.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "comm-ofi-internal.h"

/* libfabric stuff */
typedef struct libfabric_stuff_t {
  int32_t numLocales;
  struct fid_fabric* fabric;
  struct fid_domain* domain;
  /* For async puts/gets */
  struct fid_cq* cq; /* FI_CQ_FORMAT_CONTEXT?, FI_WAIT_UNSPEC, signaling_vector? */
  struct fid_ep* endpoint;
} libfabric_stuff_t;
static libfabric_stuff_t lf;

int chpl_comm_addr_gettable(c_nodeid_t node, void* start, size_t len)
{

  // Use of FI_MR_SCALABLE could cover entire address space, but
  // there's no way to know whether the page is allocated already

  return 1;
}

int32_t chpl_comm_getMaxThreads(void) {
  // no limit
  return 0;
}

static void libfabric_init(void);
static void oob_init(void);

void chpl_comm_init(int *argc_p, char ***argv_p) {
  oob_init();
  libfabric_init();
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

static void libfabric_init() {
  struct fi_info *info = NULL;
  struct fi_info *hints = fi_allocinfo();

  /***
      - What do we need for AM?
   ***/

  hints->mode = ~0;

  hints->caps = FI_RMA
             | FI_ATOMIC
             | FI_SOURCE /* do we want this? */
             | FI_READ
             | FI_WRITE
             | FI_REMOTE_READ
             | FI_REMOTE_WRITE
             | FI_FENCE;

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
  hints->domain_attr->control_progress = FI_PROGRESS_AUTO;
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

  OFICHKERR(fi_fabric(info->fabric_attr, &lf.fabric, NULL));
  OFICHKERR(fi_domain(lf.fabric, info, &lf.domain, NULL));

  lf.numLocales = chpl_numNodes;

  fi_freeinfo(hints); /* No error returned */
  fi_freeinfo(info);  /* No error returned */

}

void chpl_comm_post_mem_init(void) { }

//
// No support for gdb for now
//
int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
  return 0;
}

void chpl_comm_post_task_init(void) {
  // Start polling task
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

#ifdef CHPL_TARGET_PLATFORM_CRAY_XC
  // Use PMI_Barrier if helper thread not spawned yet
#else /* CHPL_TARGET_PLATFORM_CRAY_XC */
#endif /* CHPL_TARGET_PLATFORM_CRAY_XC */
}

void chpl_comm_pre_task_exit(int all) {
  // Tear down the polling thread
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
#error "Global initialization not supported"
#endif /* CHPL_TARGET_PLATFORM_CRAY_XC */

  OFICHKERR(fi_close(&lf.domain->fid));
  OFICHKERR(fi_close(&lf.fabric->fid));

}

static void exit_any(int status) {
  // Should we tear down the polling thread?
}

int chpl_comm_numPollingTasks(void) { return 0; }

void chpl_comm_make_progress(void) { }

