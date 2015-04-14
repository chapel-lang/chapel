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

/* Dupe of the version in chpl-comm.h except with atomics */
typedef struct {
  atomic_uint_least64_t get;
  atomic_uint_least64_t get_nb;
  atomic_uint_least64_t put;
  atomic_uint_least64_t put_nb;
  atomic_uint_least64_t test_nb;
  atomic_uint_least64_t wait_nb;
  atomic_uint_least64_t try_nb;
  atomic_uint_least64_t execute_on;
  atomic_uint_least64_t execute_on_fast;
  atomic_uint_least64_t execute_on_nb;
} commDiagnostics_atomic_t;

static commDiagnostics_atomic_t comm_diagnostics;
static int chpl_comm_diags_enabled = 1; // for masking diags

#define CHPL_COMM_DIAGS_INC(comm_type)					\
  if (chpl_comm_diagnostics && chpl_comm_diags_enabled) {		\
    atomic_fetch_add_uint_least64_t(&comm_diagnostics. ## commtype, 1);	\
  }

#define CHPL_COMM_DO_DIAGS(val)						\
  if (chpl_comm_diagnostics && chpl_comm_diags_enabled) {		\
    atomic_fetch_add_uint_least64_t(val, 1);				\
  }

#define OFICHKERR(fncall) do {                   \
    int retval;                                  \
    if ((retval = fncall) != FI_SUCCESS) {       \
      chpl_internal_error(fi_strerror(retval));  \
    }                                            \
  } while (0)

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

static inline chpl_comm_nb_handle_t ofi_put(chpl_comm_cb_event_kind_t etype,
					    atomic_uint_least64_t *cval,
					    void *addr, c_nodeid_t node,
					    void* raddr, size_t size,
					    int32_t typeIndex,
					    int ln, int32_t fn);

static inline chpl_comm_nb_handle_t ofi_get(chpl_comm_cb_event_kind_t etype,
					    atomic_uint_least64_t *cval,
					    void *addr, c_nodeid_t node,
					    void* raddr, size_t size,
					    int32_t typeIndex,
					    int ln, int32_t fn);

chpl_comm_nb_handle_t chpl_comm_put_nb(void *addr, c_nodeid_t node,
				       void* raddr, size_t size,
				       int32_t typeIndex,
                                       int ln, int32_t fn) {
  return ofi_put(chpl_comm_cb_event_kind_put_nb,
		 &comm_diagnostics.put_nb,
		 addr, node, raddr, size, typeIndex, ln, fn);
}

chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, c_nodeid_t node, void* raddr,
                                       size_t size, int32_t typeIndex,
                                       int ln, int32_t fn) {
  return ofi_get(chpl_comm_cb_event_kind_get_nb,
		 &comm_diagnostics.get_nb,
		 addr, node, raddr, size, typeIndex, ln, fn);
}

int chpl_comm_test_nb_complete(chpl_comm_nb_handle_t h)
{
  // fi_cq_readfrom?
  return ((void*) h) == NULL;
}

void chpl_comm_wait_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles)
{
  size_t i;
  // fi_cq_readfrom?
  for( i = 0; i < nhandles; i++ ) {
    assert(h[i] == NULL);
  }
}

int chpl_comm_try_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles)
{
  size_t i;
  // fi_cq_readfrom?
  for( i = 0; i < nhandles; i++ ) {
    assert(h[i] == NULL);
  }
  return 0;
}

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

void  chpl_comm_put(void* addr, c_nodeid_t node, void* raddr,
                    size_t size, int32_t typeIndex,
                    int ln, int32_t fn) {
  chpl_comm_nb_handle_t handle;

  handle = ofi_put(chpl_comm_cb_event_kind_put,
		   &comm_diagnostics.put,
		   addr, node, raddr, size, typeIndex, ln, fn);
  if (handle) {
    // fi_cq_read
  }
}

void  chpl_comm_get(void* addr, int32_t node, void* raddr,
                    size_t size, int32_t typeIndex,
                    int ln, int32_t fn) {
  chpl_comm_nb_handle_t handle;

  handle = ofi_get(chpl_comm_cb_event_kind_get,
		   &comm_diagnostics.get,
		   addr, node, raddr, size, typeIndex, ln, fn);
  if (handle) {
    // fi_cq_read
  }
}

void  chpl_comm_put_strd(void* dstaddr_arg, size_t* dststrides, c_nodeid_t dstnode,
                         void* srcaddr_arg, size_t* srcstrides, size_t* count,
                         int32_t stridelevels, size_t elemSize, int32_t typeIndex, 
                         int ln, int32_t fn) {
  // Copied from comm none.  Would be nice to share code.

  const size_t strlvls = (size_t)stridelevels;
  size_t i,j,k,l,m,t,total,off,x,carry;

  int8_t* dstaddr,*dstaddr1,*dstaddr2,*dstaddr3;
  int8_t* srcaddr,*srcaddr1,*srcaddr2,*srcaddr3;

  int *srcdisp, *dstdisp;

  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];

  //Only count[0] and strides are measured in number of bytes.
  cnt[0] = count[0] * elemSize;
  if (strlvls>0) {
    srcstr[0] = srcstrides[0] * elemSize;
    dststr[0] = dststrides[0] * elemSize;
    for (i=1;i<strlvls;i++) {
      srcstr[i] = srcstrides[i] * elemSize;
      dststr[i] = dststrides[i] * elemSize;
      cnt[i] = count[i];
    }
    cnt[strlvls] = count[strlvls];
  }

  if (dstnode == chpl_nodeID) {
  switch(strlvls) {
  case 0:
    memmove(dstaddr_arg,srcaddr_arg,cnt[0]);
    break;
  case 1:
    dstaddr = (int8_t*)dstaddr_arg;
    srcaddr = (int8_t*)srcaddr_arg;
    for (i=0; i<cnt[1]; i++) {
      memmove((void*)dstaddr,(void*)srcaddr,cnt[0]);
      srcaddr += srcstr[0];
      dstaddr += dststr[0];
    }
    break;
  case 2:
    for (i=0; i<cnt[2]; i++) {
      srcaddr = (int8_t*)srcaddr_arg + srcstr[1]*i;
      dstaddr = (int8_t*)dstaddr_arg + dststr[1]*i;
      for (j=0; j<cnt[1]; j++) {
        memmove(dstaddr,srcaddr,cnt[0]);
        srcaddr += srcstr[0];
        dstaddr += dststr[0];
      }
    }
    break;
  case 3:
    for (i=0; i<cnt[3]; i++) {
      srcaddr1 = (int8_t*)srcaddr_arg + srcstr[2]*i;
      dstaddr1 = (int8_t*)dstaddr_arg + dststr[2]*i;
      for (j=0; j<cnt[2]; j++) {
        srcaddr = srcaddr1 + srcstr[1]*j;
        dstaddr = dstaddr1 + dststr[1]*j;
        for (k=0; k<cnt[1]; k++) {
          memmove(dstaddr,srcaddr,cnt[0]);
          srcaddr += srcstr[0];
          dstaddr += dststr[0];
        }
      }
    }
    break;
    case 4:
    for (i=0; i<cnt[4]; i++) {
      srcaddr2 = (int8_t*)srcaddr_arg + srcstr[3]*i;
      dstaddr2 = (int8_t*)dstaddr_arg + dststr[3]*i;
      for (j=0; j<cnt[3]; j++) {
        srcaddr1 = srcaddr2 + srcstr[2]*j;
        dstaddr1 = dstaddr2 + dststr[2]*j;
        for (k=0; k<cnt[2]; k++) {
          srcaddr = srcaddr1 + srcstr[1]*k;
          dstaddr = dstaddr1 + dststr[1]*k;
          for (l=0; l<cnt[1]; l++) {
            memmove(dstaddr,srcaddr,cnt[0]);
            srcaddr += srcstr[0];
            dstaddr += dststr[0];
          }
        }
      }
    }
    break;
  case 5:
    for (i=0; i<cnt[5]; i++) {
      srcaddr3 = (int8_t*)srcaddr_arg + srcstr[4]*i;
      dstaddr3 = (int8_t*)dstaddr_arg + dststr[4]*i;
      for (j=0; j<cnt[4]; j++) {
        srcaddr2 = srcaddr3 + srcstr[3]*j;
        dstaddr2 = dstaddr3 + dststr[3]*j;
        for (k=0; k<cnt[3]; k++) {
          srcaddr1 = srcaddr2 + srcstr[2]*k;
          dstaddr1 = dstaddr2 + dststr[2]*k;
          for (l=0; l<cnt[2]; l++) {
            srcaddr = srcaddr1 + srcstr[1]*l;
            dstaddr = dstaddr1 + dststr[1]*l;
            for (m=0; m<cnt[1]; m++) {
              memmove(dstaddr,srcaddr,cnt[0]);
              srcaddr += srcstr[0];
              dstaddr += dststr[0];
            }
          }
        }
      }
    }
    break;
  default:
    dstaddr = (int8_t*)dstaddr_arg;
    srcaddr = (int8_t*)srcaddr_arg;

    //Number of memmove operations to do
    total = 1;
    for (i=0; i<strlvls; i++)
      total = total*cnt[i+1];

    //displacement from the dstaddr and srcaddr start points
    // We want these allocations to be locale-aware, since computing these stripes is 
    // a kind of kernel code.
    srcdisp = chpl_mem_allocManyZero(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
    dstdisp = chpl_mem_allocManyZero(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);

    for (j=0; j<total; j++) {
      carry = 1;
      for (t=1;t<=strlvls;t++) {
        if (cnt[t]*carry>=j+1) {  //IF 1
          x = j/carry;
          off = j-(carry*x);
          if (carry!=1) {  //IF 2
            srcdisp[j] = srcstr[t-1]*x+srcdisp[off];
            dstdisp[j] = dststr[t-1]*x+dstdisp[off];
          } else {  //ELSE 2
            srcdisp[j] = srcstr[t-1]*x;
            dstdisp[j] = dststr[t-1]*x;
          }
          memmove(dstaddr+dstdisp[j],srcaddr+srcdisp[j],cnt[0]);
          break;
        } else {  //ELSE 1
          carry = carry*cnt[t];
        }
      }
    }  // for j
    chpl_mem_free(srcdisp,0,0);
    chpl_mem_free(dstdisp,0,0);
    break;
  }
  return;
  }

  chpl_internal_error("Remote strided puts not yet implemented");

  CHPL_COMM_DO_DIAGS(&comm_diagnostics.put);
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_put_strd)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_put_strd, chpl_nodeID, dstnode,
       .iu.comm_strd={srcaddr_arg, srcstrides, dstaddr_arg, dststrides, count,
		      stridelevels, elemSize, typeIndex, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }
}

void  chpl_comm_get_strd(void* dstaddr_arg, size_t* dststrides, c_nodeid_t srcnode,
                         void* srcaddr_arg, size_t* srcstrides, size_t* count,
                         int32_t stridelevels, size_t elemSize, int32_t typeIndex, 
                         int ln, int32_t fn) {
  // Copied from comm none.  Would be nice to share code.

  const size_t strlvls = (size_t)stridelevels;
  size_t i,j,k,l,m,t,total,off,x,carry;

  int8_t* dstaddr,*dstaddr1,*dstaddr2,*dstaddr3;
  int8_t* srcaddr,*srcaddr1,*srcaddr2,*srcaddr3;

  int *srcdisp, *dstdisp;
  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];

  //Only count[0] and strides are measured in number of bytes.
  cnt[0] = count[0] * elemSize;
  if (strlvls>0) {
    srcstr[0] = srcstrides[0] * elemSize;
    dststr[0] = dststrides[0] * elemSize;
    for (i=1;i<strlvls;i++) {
      srcstr[i] = srcstrides[i] * elemSize;
      dststr[i] = dststrides[i] * elemSize;
      cnt[i] = count[i];
      }
    cnt[strlvls] = count[strlvls];
  }

  if (srcnode == chpl_nodeID) {
  switch(strlvls) {
  case 0:
    dstaddr = (int8_t*)dstaddr_arg;
    srcaddr = (int8_t*)srcaddr_arg;
    memmove(dstaddr,srcaddr,cnt[0]);
    break;
  case 1:
    dstaddr = (int8_t*)dstaddr_arg;
    srcaddr = (int8_t*)srcaddr_arg;
    for (i=0; i<cnt[1]; i++) {
      memmove(dstaddr,srcaddr,cnt[0]);
      srcaddr += srcstr[0];
      dstaddr += dststr[0];
    }
    break;
  case 2:
    for (i=0; i<cnt[2]; i++) {
      srcaddr = (int8_t*)srcaddr_arg + srcstr[1]*i;
      dstaddr = (int8_t*)dstaddr_arg + dststr[1]*i;
      for (j=0; j<cnt[1]; j++) {
        memmove(dstaddr,srcaddr,cnt[0]);
        srcaddr += srcstr[0];
        dstaddr += dststr[0];
      }
    }
    break;
  case 3:
    for (i=0; i<cnt[3]; i++) {
      srcaddr1 = (int8_t*)srcaddr_arg + srcstr[2]*i;
      dstaddr1 = (int8_t*)dstaddr_arg + dststr[2]*i;
      for (j=0; j<cnt[2]; j++) {
        srcaddr = srcaddr1 + srcstr[1]*j;
        dstaddr = dstaddr1 + dststr[1]*j;
        for (k=0; k<cnt[1]; k++) {
          memmove(dstaddr,srcaddr,cnt[0]);
          srcaddr += srcstr[0];
          dstaddr += dststr[0];
        }
      }
    }
    break;
  case 4:
    for (i=0; i<cnt[4]; i++) {
      srcaddr2 = (int8_t*)srcaddr_arg + srcstr[3]*i;
      dstaddr2 = (int8_t*)dstaddr_arg + dststr[3]*i;
      for (j=0; j<cnt[3]; j++) {
        srcaddr1 = srcaddr2 + srcstr[2]*j;
        dstaddr1 = dstaddr2 + dststr[2]*j;
        for (k=0; k<cnt[2]; k++) {
          srcaddr = srcaddr1 + srcstr[1]*k;
          dstaddr = dstaddr1 + dststr[1]*k;
          for (l=0; l<cnt[1]; l++) {
            memmove(dstaddr,srcaddr,cnt[0]);
            srcaddr += srcstr[0];
            dstaddr += dststr[0];
          }
        }
      }
    }
    break;
  case 5:
    for (i=0; i<cnt[5]; i++) {
      srcaddr3 = (int8_t*)srcaddr_arg + srcstr[4]*i;
      dstaddr3 = (int8_t*)dstaddr_arg + dststr[4]*i;
      for (j=0; j<cnt[4]; j++) {
        srcaddr2 = srcaddr3 + srcstr[3]*j;
        dstaddr2 = dstaddr3 + dststr[3]*j;
        for (k=0; k<cnt[3]; k++) {
          srcaddr1 = srcaddr2 + srcstr[2]*k;
          dstaddr1 = dstaddr2 + dststr[2]*k;
          for (l=0; l<cnt[2]; l++) {
            srcaddr = srcaddr1 + srcstr[1]*l;
            dstaddr = dstaddr1 + dststr[1]*l;
            for (m=0; m<cnt[1]; m++) {
              memmove(dstaddr,srcaddr,cnt[0]);
              srcaddr += srcstr[0];
              dstaddr += dststr[0];
            }
          }
        }
      }
    }
    break;
  default:
    dstaddr = (int8_t*)dstaddr_arg;
    srcaddr = (int8_t*)srcaddr_arg;

    //Number of memmove operations to do
    total = 1;
    for (i=0; i<strlvls; i++)
      total = total*cnt[i+1];

    //displacement from the dstaddr and srcaddr start points
    srcdisp = chpl_mem_allocManyZero(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
    dstdisp = chpl_mem_allocManyZero(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);

    for (j=0; j<total; j++) {
      carry = 1;
      for (t=1;t<=strlvls;t++) {
        if (cnt[t]*carry>=j+1) {  //IF 1
          x = j/carry;
          off = j-(carry*x);
          if (carry!=1) {  //IF 2
            srcdisp[j] = srcstr[t-1]*x+srcdisp[off];
            dstdisp[j] = dststr[t-1]*x+dstdisp[off];
          } else {  //ELSE 2
            srcdisp[j] = srcstr[t-1]*x;
            dstdisp[j] = dststr[t-1]*x;
          }
          memmove(dstaddr+dstdisp[j],srcaddr+srcdisp[j],cnt[0]);
          break;
        } else {  //ELSE 1
          carry = carry*cnt[t];
        }
      }
    }  // for j
    chpl_mem_free(srcdisp,0,0);
    chpl_mem_free(dstdisp,0,0);
    break;
  }
  return;
  }

  chpl_internal_error("Remote strided gets not yet implemented");

  CHPL_COMM_DO_DIAGS(&comm_diagnostics.get);
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_get_strd)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_get_strd, chpl_nodeID, srcnode,
       .iu.comm_strd={srcaddr_arg, srcstrides, dstaddr_arg, dststrides, count,
                      stridelevels, elemSize, typeIndex, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }
}


static inline chpl_comm_nb_handle_t ofi_put(chpl_comm_cb_event_kind_t etype,
					    atomic_uint_least64_t *cval,
					    void *addr, c_nodeid_t node,
					    void* raddr, size_t size,
					    int32_t typeIndex,
					    int ln, int32_t fn) {
  if (node == chpl_nodeID) {
    memmove(raddr, addr, size);
    return NULL;
  }

  CHPL_COMM_DO_DIAGS(cval);

  if (chpl_comm_have_callbacks(etype)) {
    chpl_comm_cb_info_t cb_data = {etype, chpl_nodeID, node,
				   .iu.comm={addr, raddr, size,
					     typeIndex, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }

  // fi_write

  chpl_internal_error("Remote puts not yet implemented");

  return NULL;
}

static inline chpl_comm_nb_handle_t ofi_get(chpl_comm_cb_event_kind_t etype,
					    atomic_uint_least64_t *cval,
					    void *addr, c_nodeid_t node,
					    void* raddr, size_t size,
					    int32_t typeIndex,
					    int ln, int32_t fn) {
  if (node == chpl_nodeID) {
    memmove(addr, raddr, size);
    return NULL;
  }

  CHPL_COMM_DO_DIAGS(cval);

  if (chpl_comm_have_callbacks(etype)) {
    chpl_comm_cb_info_t cb_data = {etype, chpl_nodeID, node,
				   .iu.comm={addr, raddr, size,
					     typeIndex, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }

  // fi_read

  chpl_internal_error("Remote gets not yet implemented");

  return NULL;
}

typedef struct {
  chpl_bool     serial_state; // To prevent creation of new tasks
  chpl_fn_int_t fid;
  int           arg_size;
  char          arg[0];       // variable-sized data here
} fork_t;

void chpl_comm_execute_on(c_nodeid_t node, c_sublocid_t subloc,
                          chpl_fn_int_t fid,
                          chpl_comm_on_bundle_t *arg, size_t arg_size) {
  assert(node != chpl_nodeID); // handled by the locale model

  chpl_internal_error("Remote ons not yet implemented");

  CHPL_COMM_DO_DIAGS(&comm_diagnostics.execute_on);

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn)) {
    chpl_comm_cb_info_t cb_data = 
      {chpl_comm_cb_event_kind_executeOn, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, arg_size}};
    chpl_comm_do_callbacks (&cb_data);
  }

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

  chpl_internal_error("Remote ons not yet implemented");

  CHPL_COMM_DO_DIAGS(&comm_diagnostics.execute_on_nb);

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_nb)) {
    chpl_comm_cb_info_t cb_data = 
      {chpl_comm_cb_event_kind_executeOn_nb, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, arg_size}};
    chpl_comm_do_callbacks (&cb_data);
  }
}

void chpl_comm_execute_on_fast(c_nodeid_t node, c_sublocid_t subloc,
                               chpl_fn_int_t fid,
                               chpl_comm_on_bundle_t *arg, size_t arg_size) {
  assert(node != chpl_nodeID); // handled by the locale model

  chpl_internal_error("Remote ons not yet implemented");

  CHPL_COMM_DO_DIAGS(&comm_diagnostics.execute_on_fast);

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_fast)) {
    chpl_comm_cb_info_t cb_data = 
      {chpl_comm_cb_event_kind_executeOn_fast, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, arg_size}};
    chpl_comm_do_callbacks (&cb_data);
  }
}

int chpl_comm_numPollingTasks(void) { return 0; }

void chpl_comm_make_progress(void) { }

/*
 * comm diagnostics support
 *
 * Would be good to make this uniform over all comm layers
 */

void chpl_startVerboseComm() {
  chpl_verbose_comm = 1;
  chpl_comm_diags_enabled = 0;
  chpl_comm_broadcast_private(0 /* &chpl_verbose_comm */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_diags_enabled = 1;
}

void chpl_stopVerboseComm() {
  chpl_verbose_comm = 0;
  chpl_comm_diags_enabled = 0;
  chpl_comm_broadcast_private(0 /* &chpl_verbose_comm */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_diags_enabled = 1;
}

void chpl_startVerboseCommHere() {
  chpl_verbose_comm = 1;
}

void chpl_stopVerboseCommHere() {
  chpl_verbose_comm = 1;
}

void chpl_startCommDiagnostics() {
  chpl_comm_diagnostics = 1;
  chpl_comm_diags_enabled = 0;
  chpl_comm_broadcast_private(1 /* &chpl_comm_diagnostics */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_diags_enabled = 1;
}

void chpl_stopCommDiagnostics() {
  chpl_comm_diagnostics = 0;
  chpl_comm_diags_enabled = 0;
  chpl_comm_broadcast_private(1 /* &chpl_comm_diagnostics */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_diags_enabled = 1;
}

void chpl_startCommDiagnosticsHere() {
  chpl_comm_diagnostics = 1;
}

void chpl_stopCommDiagnosticsHere() {
  chpl_comm_diagnostics = 0;
}

void chpl_resetCommDiagnosticsHere() {
  atomic_store_uint_least64_t(&comm_diagnostics.get, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.get_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.put, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.put_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.test_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.wait_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.try_nb, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.execute_on, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.execute_on_fast, 0);
  atomic_store_uint_least64_t(&comm_diagnostics.execute_on_nb, 0);
}

void chpl_getCommDiagnosticsHere(chpl_commDiagnostics *cd) {
  cd->get = atomic_load_uint_least64_t(&comm_diagnostics.get);
  cd->get_nb = atomic_load_uint_least64_t(&comm_diagnostics.get_nb);
  cd->put = atomic_load_uint_least64_t(&comm_diagnostics.put);
  cd->put_nb = atomic_load_uint_least64_t(&comm_diagnostics.put_nb);
  cd->test_nb = atomic_load_uint_least64_t(&comm_diagnostics.test_nb);
  cd->wait_nb = atomic_load_uint_least64_t(&comm_diagnostics.wait_nb);
  cd->try_nb = atomic_load_uint_least64_t(&comm_diagnostics.try_nb);
  cd->execute_on = atomic_load_uint_least64_t(&comm_diagnostics.execute_on);
  cd->execute_on_fast = atomic_load_uint_least64_t(&comm_diagnostics.execute_on_fast);
  cd->execute_on_nb = atomic_load_uint_least64_t(&comm_diagnostics.execute_on_nb);
}
