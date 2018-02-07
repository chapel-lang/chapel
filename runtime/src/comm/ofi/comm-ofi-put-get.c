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

#include "rdma/fabric.h"
#include "rdma/fi_domain.h"
#include "rdma/fi_errno.h"

#include "chpl-comm.h"
#include "chpl-comm-callbacks.h"
#include "chpl-comm-callbacks-internal.h"
#include "chpl-gen-includes.h"

// Don't get warning macros for chpl_comm_get etc
#include "chpl-comm-no-warning-macros.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "comm-ofi-internal.h"

static struct ofi_stuff* ofi = NULL;
void chpl_comm_ofi_put_get_init(struct ofi_stuff* _ofi) {
  if (ofi == NULL) {
    ofi = _ofi;
  } else {
    chpl_warning("ofi put/get already initialized.  Ignoring", 0, 0);
  }
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
  return ofi->rx_ep[get_sep_index(ofi->num_rx_ctx)];
}

static inline struct fid_ep* get_tx_ep() {
  return ofi->tx_ep[get_sep_index(ofi->num_tx_ctx)];
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
  if (node == chpl_nodeID) {
    memmove(raddr, addr, size);
    return NULL;
  }

  CHPL_COMM_DIAGS_INC(put_nb);

  return ofi_put(chpl_comm_cb_event_kind_put_nb,
                 addr, node, raddr, size, typeIndex, commID, ln, fn);
}

chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, c_nodeid_t node,
                                       void* raddr, size_t size,
                                       int32_t typeIndex, int32_t commID,
                                       int ln, int32_t fn) {
  if (node == chpl_nodeID) {
    memmove(addr, raddr, size);
    return NULL;
  }

  CHPL_COMM_DIAGS_INC(get_nb);

  return ofi_get(chpl_comm_cb_event_kind_get_nb,
                 addr, node, raddr, size, typeIndex, commID, ln, fn);
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

void  chpl_comm_put(void* addr, c_nodeid_t node, void* raddr,
                    size_t size, int32_t typeIndex, int32_t commID,
                    int ln, int32_t fn) {
  chpl_comm_nb_handle_t handle;

  if (node == chpl_nodeID) {
    memmove(raddr, addr, size);
    return;
  }

  CHPL_COMM_DIAGS_INC(put);

  handle = ofi_put(chpl_comm_cb_event_kind_put,
                   addr, node, raddr, size, typeIndex, commID, ln, fn);
  if (handle) {
    // fi_cq_read
  }
}

void  chpl_comm_get(void* addr, int32_t node, void* raddr,
                    size_t size, int32_t typeIndex, int32_t commID,
                    int ln, int32_t fn) {
  chpl_comm_nb_handle_t handle;

  if (node == chpl_nodeID) {
    memmove(addr, raddr, size);
    return;
  }

  CHPL_COMM_DIAGS_INC(get);

  handle = ofi_get(chpl_comm_cb_event_kind_get,
                   addr, node, raddr, size, typeIndex, commID, ln, fn);
  if (handle) {
    // fi_cq_read
  }
}

void  chpl_comm_put_strd(void* dstaddr_arg, size_t* dststrides, c_nodeid_t dstnode,
                         void* srcaddr_arg, size_t* srcstrides, size_t* count,
                         int32_t stridelevels, size_t elemSize, int32_t typeIndex, 
                         int32_t commID, int ln, int32_t fn) {
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

  CHPL_COMM_DIAGS_INC(put);
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_put_strd)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_put_strd, chpl_nodeID, dstnode,
       .iu.comm_strd={srcaddr_arg, srcstrides, dstaddr_arg, dststrides, count,
                      stridelevels, elemSize, typeIndex, commID, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }
}

void  chpl_comm_get_strd(void* dstaddr_arg, size_t* dststrides, c_nodeid_t srcnode,
                         void* srcaddr_arg, size_t* srcstrides, size_t* count,
                         int32_t stridelevels, size_t elemSize, int32_t typeIndex, 
                         int32_t commID, int ln, int32_t fn) {
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

  CHPL_COMM_DIAGS_INC(get);
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_get_strd)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_get_strd, chpl_nodeID, srcnode,
       .iu.comm_strd={srcaddr_arg, srcstrides, dstaddr_arg, dststrides, count,
                      stridelevels, elemSize, typeIndex, commID, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }
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
