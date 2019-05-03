/*
 * Copyright 2004-2019 Cray Inc.
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
#include "chpl-comm-diags.h"
#include "chpl-comm-internal.h"
#include "chpl-env.h"
#include "chpl-mem.h"

#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

int32_t chpl_nodeID = -1;
int32_t chpl_numNodes = -1;


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
