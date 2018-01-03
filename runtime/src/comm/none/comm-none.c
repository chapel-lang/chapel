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

#include "chpl-comm.h"
#include "chplexit.h"
#include "error.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"

#include "chplcgfns.h"
#include "chpl-gen-includes.h"
#include "chpl-linefile-support.h"

// Don't get warning macros for chpl_comm_get etc
#include "chpl-comm-no-warning-macros.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

// Helper functions

static int mysystem(const char* command, const char* description,
                    int ignorestatus) {
  int status = system(command);

  if (status == -1) {
    chpl_error("system() fork failed", 0, CHPL_FILE_IDX_COMMAND_LINE);
  } else if (status != 0 && !ignorestatus) {
    chpl_error(description, 0, CHPL_FILE_IDX_COMMAND_LINE);
  }

  return status;
}

// Chapel interface
chpl_comm_nb_handle_t chpl_comm_put_nb(void *addr, c_nodeid_t node, void* raddr,
                                       size_t size, int32_t typeIndex,
                                       int32_t commID, int ln, int32_t fn)
{
  assert(node == 0);
  chpl_memcpy(raddr, addr, size);
  return NULL;
}

chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, c_nodeid_t node, void* raddr,
                                       size_t size, int32_t typeIndex,
                                       int32_t commID, int ln, int32_t fn)
{
  assert(node == 0);
  chpl_memcpy(addr, raddr, size);
  return NULL;
}

int chpl_comm_test_nb_complete(chpl_comm_nb_handle_t h)
{
  return ((void*) h) == NULL;
}

void chpl_comm_wait_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles)
{
  size_t i;
  for( i = 0; i < nhandles; i++ ) {
    assert(h[i] == NULL);
  }
}

int chpl_comm_try_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles)
{
  size_t i;
  for( i = 0; i < nhandles; i++ ) {
    assert(h[i] == NULL);
  }
  return 0;
}

int chpl_comm_addr_gettable(c_nodeid_t node, void* start, size_t len)
{
  return 0;
}

int32_t chpl_comm_getMaxThreads(void) {
  return 0;
}

void chpl_comm_init(int *argc_p, char ***argv_p) {
  chpl_numNodes = 1;
  chpl_nodeID = 0;
}

void chpl_comm_post_mem_init(void) { }

int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
  int i;
  char* command = chpl_glom_strings(2, "gdb -q -ex 'break gdbShouldBreakHere' --args ",
                                    argv[0]);
  for (i=1; i<argc; i++) {
    if (i != gdbArgnum) {
      command = chpl_glom_strings(3, command, " ", argv[i]);
    }
  }
  *status = mysystem(command, "running gdb", 0);

  return 1;
}

void chpl_comm_post_task_init(void) { }

void chpl_comm_rollcall(void) {
  chpl_msg(2, "executing on a single node\n");
}

void chpl_comm_get_registered_heap(void** start_p, size_t* size_p) {
  *start_p = NULL;
  *size_p  = 0;
}

void chpl_comm_broadcast_global_vars(int numGlobals) { }

void chpl_comm_broadcast_private(int id, size_t size, int32_t tid) { }

void chpl_comm_barrier(const char *msg) { }

void chpl_comm_pre_task_exit(int all) { }

void chpl_comm_exit(int all, int status) { }

void  chpl_comm_put(void* addr, c_nodeid_t node, void* raddr,
                    size_t size, int32_t typeIndex,
                    int32_t commID, int ln, int32_t fn) {
  assert(node==0);

  memmove(raddr, addr, size);
}

void  chpl_comm_get(void* addr, c_nodeid_t node, void* raddr,
                    size_t size, int32_t typeIndex,
                    int32_t commID, int ln, int32_t fn) {
  assert(node==0);

  memmove(addr, raddr, size);
}

void  chpl_comm_put_strd(void* dstaddr_arg, size_t* dststrides, c_nodeid_t dstnode,
                         void* srcaddr_arg, size_t* srcstrides, size_t* count,
                         int32_t stridelevels, size_t elemSize, int32_t typeIndex,
                         int32_t commID, int ln, int32_t fn)
{
  const size_t strlvls = (size_t)stridelevels;
  size_t i,j,k,l,m,t,total,off,x,carry;

  int8_t* dstaddr,*dstaddr1,*dstaddr2,*dstaddr3;
  int8_t* srcaddr,*srcaddr1,*srcaddr2,*srcaddr3;

  int *srcdisp, *dstdisp;

  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];

  assert(dstnode==0);

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
}

void  chpl_comm_get_strd(void* dstaddr_arg, size_t* dststrides, c_nodeid_t srcnode,
                         void* srcaddr_arg, size_t* srcstrides, size_t* count,
                         int32_t stridelevels, size_t elemSize, int32_t typeIndex,
                         int32_t commID, int ln, int32_t fn)
{
  const size_t strlvls = (size_t)stridelevels;
  size_t i,j,k,l,m,t,total,off,x,carry;

  int8_t* dstaddr,*dstaddr1,*dstaddr2,*dstaddr3;
  int8_t* srcaddr,*srcaddr1,*srcaddr2,*srcaddr3;

  int *srcdisp, *dstdisp;
  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];

  assert(srcnode==0);

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
}

typedef struct {
  chpl_fn_int_t fid;
  size_t        arg_size;
  char          arg[0];       // variable-sized data here
} fork_t;

void chpl_comm_execute_on(c_nodeid_t node, c_sublocid_t subloc,
                    chpl_fn_int_t fid,
                    chpl_comm_on_bundle_t *arg, size_t arg_size) {
  assert(node==0);

  chpl_ftable_call(fid, arg);
}

void chpl_comm_execute_on_nb(c_nodeid_t node, c_sublocid_t subloc,
                       chpl_fn_int_t fid,
                       chpl_comm_on_bundle_t *arg, size_t arg_size) {
  assert(node==0);

  chpl_task_startMovedTask(fid, chpl_ftable[fid],
                           chpl_comm_on_bundle_task_bundle(arg), arg_size,
                           subloc, chpl_nullTaskID);
}

// Same as chpl_comm_execute_on()
void chpl_comm_execute_on_fast(c_nodeid_t node, c_sublocid_t subloc,
                         chpl_fn_int_t fid,
                         chpl_comm_on_bundle_t *arg, size_t arg_size) {
  assert(node==0);

  chpl_ftable_call(fid, arg);
}

int chpl_comm_numPollingTasks(void) { return 0; }

void chpl_comm_make_progress(void)
{
}

void chpl_startVerboseComm() { }
void chpl_stopVerboseComm() { }
void chpl_startVerboseCommHere() { }
void chpl_stopVerboseCommHere() { }

void chpl_startCommDiagnostics() { }
void chpl_stopCommDiagnostics() { }
void chpl_startCommDiagnosticsHere() { }
void chpl_stopCommDiagnosticsHere() { }

void chpl_resetCommDiagnosticsHere() { }
void chpl_getCommDiagnosticsHere(chpl_commDiagnostics *cd) {
  memset(cd, 0, sizeof(chpl_commDiagnostics));
}

