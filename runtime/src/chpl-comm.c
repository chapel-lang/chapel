/*
 * Copyright 2004-2020 Cray Inc.
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
// Shared code for different comm implementations in
//  comm/<commlayer>/comm-<commlayer>.c
//
#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-comm-compiler-macros.h"
#include "chpl-comm-diags.h"
#include "chpl-comm-internal.h"
#include "chpl-env.h"
#include "chpl-mem.h"

// Don't get warning macros for chpl_comm_get etc.
#include "chpl-comm-no-warning-macros.h"

#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

int32_t chpl_nodeID = -1;
int32_t chpl_numNodes = -1;


//
// Global variable broadcast support.
//
void chpl_comm_register_global_var(int i, wide_ptr_t *ptr_to_wide_ptr) {
  chpl_globals_registry[i] = ptr_to_wide_ptr;
}


void chpl_comm_broadcast_global_vars(int numGlobals) {
  //
  // On node 0: gather up the global variables' wide pointers into a
  //            buffer; return that buffer if it needs deallocating
  //            after the communication, otherwise NULL.
  // On other nodes: retrieve the node 0 local address of that buffer.
  //
  wide_ptr_t* buf_on_0;
  buf_on_0 = chpl_comm_broadcast_global_vars_helper();

  //
  // On node 0: barrier to ensure the other nodes have the global vars;
  //            free the buffer if needed.
  // On other nodes: GET the buffer of wide pointers; scatter it into
  //                 our copies of the global vars; barrier to indicate
  //                 we're done.  Node 0 could actually free the buffer
  //                 as soon as we GET it, but we also have to prevent
  //                 node 0 from running any Chapel code until all the
  //                 other nodes have recorded the wide pointers.  So,
  //                 we delay our barrier to achieve both goals.
  //
  if (chpl_nodeID == 0) {
    chpl_comm_barrier("broadcast global vars");
    if (buf_on_0 != NULL) {
      chpl_mem_free(buf_on_0, 0, 0);
    }
  } else {
    wide_ptr_t* buf;
    size_t size = chpl_numGlobalsOnHeap * sizeof(*buf);
    buf = (wide_ptr_t*)
          chpl_mem_alloc(size, CHPL_RT_MD_COMM_PER_LOC_INFO, 0, 0);
    chpl_comm_get(buf, 0, buf_on_0, size, CHPL_COMM_UNKNOWN_ID, 0, -1);
    for (int i = 0; i < chpl_numGlobalsOnHeap; i++) {
      *chpl_globals_registry[i] = buf[i];
    }
    chpl_comm_barrier("broadcast global vars");
    chpl_mem_free(buf, 0, 0);
  }
}


void** chpl_rt_priv_bcast_tab;
int chpl_rt_priv_bcast_tab_len;

#define _RT_PRV_BCAST_M(sym) sizeof(sym),
size_t chpl_rt_priv_bcast_lens[chpl_rt_prv_tab_num_idxs] =
         { CHPL_RT_PRV_BCAST_TAB_ENTRIES(_RT_PRV_BCAST_M) };
#undef _RT_PRV_BCAST_M

void chpl_comm_init_prv_bcast_tab(void) {
  //
  // Make a copy of chpl_private_broadcast_table[], but with some more of
  // our own entries following the compiler-emitted ones.
  //
  chpl_rt_priv_bcast_tab_len = chpl_private_broadcast_table_len
                               + chpl_rt_prv_tab_num_idxs;
  chpl_rt_priv_bcast_tab =
    chpl_mem_allocMany(chpl_rt_priv_bcast_tab_len,
                       sizeof(chpl_rt_priv_bcast_tab[0]),
                       CHPL_RT_MD_COMM_UTIL, 0, 0);

  // Duplicate the compiler-emitted entries.
  memcpy(chpl_rt_priv_bcast_tab,
         chpl_private_broadcast_table,
         chpl_private_broadcast_table_len
         * sizeof(chpl_private_broadcast_table[0]));

  // Fill in our entries that follow those.
#define _RT_PRV_BCAST_M(sym)                                            \
  chpl_rt_priv_bcast_tab[chpl_private_broadcast_table_len               \
                         + chpl_rt_prv_tab_ ## sym ## _idx] = &sym;
  CHPL_RT_PRV_BCAST_TAB_ENTRIES(_RT_PRV_BCAST_M)
#undef _RT_PRV_BCAST_M
}


static pthread_once_t maxHeapSize_once = PTHREAD_ONCE_INIT;
static size_t maxHeapSize;

static
void set_maxHeapSize(void)
{
  const char* ev = "MAX_HEAP_SIZE";

  int pct;
  if ((pct = chpl_env_rt_get_int_pct(ev, -1, false /*doWarn*/)) > 0) {
    const size_t sysMem = chpl_sys_physicalMemoryBytes();
    maxHeapSize = (pct * sysMem + 50) / 100; // rounded percentage
    return;
  }

  maxHeapSize = chpl_env_rt_get_size(ev, 0);
}

size_t chpl_comm_getenvMaxHeapSize(void)
{
  if (pthread_once(&maxHeapSize_once, set_maxHeapSize) != 0) {
    chpl_internal_error("pthread_once(&maxHeapSize_once) failed");
  }

  return maxHeapSize;
}


void* chpl_get_global_serialize_table(int64_t idx) {
  return chpl_global_serialize_table[idx];
}

static chpl_bool can_shutdown = false;
static pthread_mutex_t shutdown_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t shutdown_cond = PTHREAD_COND_INITIALIZER;

void chpl_signal_shutdown(void) {
  pthread_mutex_lock(&shutdown_mutex);
  can_shutdown = true;
  pthread_cond_signal(&shutdown_cond);
  pthread_mutex_unlock(&shutdown_mutex);
}

void chpl_wait_for_shutdown(void) {
  pthread_mutex_lock(&shutdown_mutex);
  while (!can_shutdown) {
    pthread_cond_wait(&shutdown_cond, &shutdown_mutex);
  }
  pthread_mutex_unlock(&shutdown_mutex);
}
