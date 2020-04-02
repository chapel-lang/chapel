/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#ifndef _chpl_comm_internal_h_
#define _chpl_comm_internal_h_

#include <stdint.h>
#include "chpltypes.h"
#include "chpl-mem-desc.h"

//
// Support for broadcasting globals.  Comm layer implementations must
// supply this.  It is called collectively.  On node 0 it must arrange
// that all the global variable wide pointers are in a buffer that can
// be the source of a GET from the other nodes and return either that
// buffer's address (if it wants it freed later) or NULL.  On all other
// nodes it should return the node-0 local address of that buffer.
//
wide_ptr_t* chpl_comm_broadcast_global_vars_helper(void);

//
// These are runtime-private copies of chpl_private_broadcast_table[]
// and chpl_private_broadcast_table_len, extended with a few more
// addresses of runtime-specific variables we also want to be able
// broadcast around.
//
extern void** chpl_rt_priv_bcast_tab;
extern int chpl_rt_priv_bcast_tab_len;
extern size_t chpl_rt_priv_bcast_lens[];

#define CHPL_RT_PRV_BCAST_TAB_ENTRIES(MACRO) \
  MACRO(chpl_verbose_comm)                   \
  MACRO(chpl_comm_diagnostics)               \
  MACRO(chpl_comm_diags_print_unstable)      \
  MACRO(chpl_verbose_mem)

#define _RT_PRV_BCAST_M(sym)  chpl_rt_prv_tab_ ## sym ## _idx,
typedef enum {
  CHPL_RT_PRV_BCAST_TAB_ENTRIES(_RT_PRV_BCAST_M)
  chpl_rt_prv_tab_num_idxs,
} chpl_rt_prv_bcast_tab_idx_t;
#undef _RT_PRV_BCAST_M

//
// This is in chpl-comm.c.
//
void chpl_comm_init_prv_bcast_tab(void);

//
// Broadcast one of our runtime-specific variables.
//
#define chpl_comm_bcast_rt_private(var) \
  chpl_comm_really_bcast_rt_private(chpl_rt_prv_tab_ ## var ## _idx)

static inline
void chpl_comm_really_bcast_rt_private(int id) {
  chpl_comm_broadcast_private(chpl_private_broadcast_table_len + id,
                              chpl_rt_priv_bcast_lens[id]);
}

#endif
