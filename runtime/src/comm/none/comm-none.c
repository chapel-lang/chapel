/*
 * Copyright 2004-2015 Cray Inc.
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
    chpl_error("system() fork failed", 0, "(command-line)");
  } else if (status != 0 && !ignorestatus) {
    chpl_error(description, 0, "(command-line)");
  }

  return status;
}

// Chapel interface
chpl_comm_nb_handle_t chpl_comm_put_nb(void *addr, c_nodeid_t node, void* raddr,
                                       int32_t elemSize, int32_t typeIndex,
                                       int32_t len,
                                       int ln, c_string fn)
{
  assert(node == 0);
  chpl_memcpy(raddr, addr, len*elemSize);
  return NULL;
}

chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, c_nodeid_t node, void* raddr,
                                       int32_t elemSize, int32_t typeIndex,
                                       int32_t len,
                                       int ln, c_string fn)
{
  assert(node == 0);
  chpl_memcpy(addr, raddr, len*elemSize);
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

int chpl_comm_is_in_segment(c_nodeid_t node, void* start, size_t len)
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

void chpl_comm_desired_shared_heap(void** start_p, size_t* size_p) {
  *start_p = NULL;
  *size_p  = 0;
}

void chpl_comm_broadcast_global_vars(int numGlobals) { }

void chpl_comm_broadcast_private(int id, int32_t sizee, int32_t tid) { }

void chpl_comm_barrier(const char *msg) { }

void chpl_comm_pre_task_exit(int all) { }

void chpl_comm_exit(int all, int status) { }

void  chpl_comm_put(void* addr, int32_t locale, void* raddr,
                    int32_t size, int32_t typeIndex, int32_t len,
                    int ln, c_string fn) {
  assert(locale==0);

  memmove(raddr, addr, size*len);
}

void  chpl_comm_get(void* addr, int32_t locale, void* raddr,
                    int32_t size, int32_t typeIndex, int32_t len,
                    int ln, c_string fn) {
  assert(locale==0);

  memmove(addr, raddr, size*len);
}

void  chpl_comm_put_strd(void* dstaddr_arg, void* dststrides, int32_t dstlocale,
                         void* srcaddr_arg, void* srcstrides, void* count,
                         int32_t stridelevels, int32_t elemSize, int32_t typeIndex,
                         int ln, c_string fn)
{
  const size_t strlvls = (size_t)stridelevels;
  int i,j,k,l,m,t,total,off,x,carry;

  int8_t* dstaddr,*dstaddr1,*dstaddr2,*dstaddr3;
  int8_t* srcaddr,*srcaddr1,*srcaddr2,*srcaddr3;

  int *srcdisp, *dstdisp;

  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];

  assert(dstlocale==0);

  //Only count[0] and strides are meassured in number of bytes.
  cnt[0] = ((int32_t*)count)[0] * elemSize;
  if (strlvls>0) {
    srcstr[0] = ((int32_t*)srcstrides)[0] * elemSize;
    dststr[0] = ((int32_t*)dststrides)[0] * elemSize;
    for (i=1;i<strlvls;i++) {
      srcstr[i] = ((int32_t*)srcstrides)[i] * elemSize;
      dststr[i] = ((int32_t*)dststrides)[i] * elemSize;
      cnt[i] = ((int32_t*)count)[i];
    }
    cnt[strlvls] = ((int32_t*)count)[strlvls];
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

void  chpl_comm_get_strd(void* dstaddr_arg, void* dststrides, int32_t srclocale,
                         void* srcaddr_arg, void* srcstrides, void* count,
                         int32_t stridelevels, int32_t elemSize, int32_t typeIndex,
                         int ln, c_string fn)
{
  const size_t strlvls = (size_t)stridelevels;
  int i,j,k,l,m,t,total,off,x,carry;

  int8_t* dstaddr,*dstaddr1,*dstaddr2,*dstaddr3;
  int8_t* srcaddr,*srcaddr1,*srcaddr2,*srcaddr3;

  int *srcdisp, *dstdisp;
  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];

  assert(srclocale==0);

  //Only count[0] and strides are meassured in number of bytes.
  cnt[0] = ((int32_t*)count)[0] * elemSize;
  if (strlvls>0) {
    srcstr[0] = ((int32_t*)srcstrides)[0] * elemSize;
    dststr[0] = ((int32_t*)dststrides)[0] * elemSize;
    for (i=1;i<strlvls;i++) {
      srcstr[i] = ((int32_t*)srcstrides)[i] * elemSize;
      dststr[i] = ((int32_t*)dststrides)[i] * elemSize;
      cnt[i] = ((int32_t*)count)[i];
      }
    cnt[strlvls] = ((int32_t*)count)[strlvls];
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
  int           arg_size;
  char          arg[0];       // variable-sized data here
} fork_t;

void chpl_comm_fork(c_nodeid_t node, c_sublocid_t subloc,
                    chpl_fn_int_t fid, void *arg, int32_t arg_size) {
  assert(node==0);

  chpl_ftable_call(fid, arg);
}

static void fork_nb_wrapper(fork_t* f) {
  if (f->arg_size)
    chpl_ftable_call(f->fid, &f->arg);
  else
    chpl_ftable_call(f->fid, NULL);
  chpl_mem_free(f, 0, 0);
}

void chpl_comm_fork_nb(c_nodeid_t node, c_sublocid_t subloc,
                       chpl_fn_int_t fid, void *arg, int32_t arg_size) {
  fork_t *info;
  int     info_size;

  assert(node==0);

  info_size = sizeof(fork_t) + arg_size;
  info = (fork_t*)chpl_mem_allocMany(info_size, sizeof(char),
                                     CHPL_RT_MD_COMM_FORK_SEND_NB_INFO, 0, 0);
  info->fid = fid;
  info->arg_size = arg_size;
  if (arg_size)
    chpl_memcpy(&(info->arg), arg, arg_size);
  chpl_task_startMovedTask((chpl_fn_p)fork_nb_wrapper, (void*)info,
                           subloc, chpl_nullTaskID, false);
}

// Same as chpl_comm_fork()
void chpl_comm_fork_fast(c_nodeid_t node, c_sublocid_t subloc,
                         chpl_fn_int_t fid, void *arg, int32_t arg_size) {
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
void chpl_getCommDiagnosticsHere(chpl_commDiagnostics *cd) { }

uint64_t chpl_numCommGets(void) { return 0; }
uint64_t chpl_numCommNBGets(void) { return 0; }
uint64_t chpl_numCommPuts(void) { return 0; }
uint64_t chpl_numCommNBPuts(void) { return 0; }
uint64_t chpl_numCommTestNB(void) { return 0; }
uint64_t chpl_numCommWaitNB(void) { return 0; }
uint64_t chpl_numCommTryNB(void) { return 0; }
uint64_t chpl_numCommForks(void) { return 0; }
uint64_t chpl_numCommFastForks(void) { return 0; }
uint64_t chpl_numCommNBForks(void) { return 0; }
