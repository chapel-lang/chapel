/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_RT_COMM_INTERNAL_H
#define CHPL_RT_COMM_INTERNAL_H

#include <stdint.h>
#include "chpltypes.h"
#include "chpl-mem-desc.h"
#include "chpl-prginfo.h"

#ifdef __cplusplus
extern "C" {
#endif

//
// Support for broadcasting globals. Comm layer implementations must
// supply this. It is called collectively. On node 0 it must arrange
// that all the global variable wide pointers are in a buffer that can
// be the source of a GET from the other nodes and return either that
// buffer's address (if it wants it freed later) or NULL. On all other
// nodes it should return the node-0 local address of that buffer.
//
wide_ptr_t* chpl_rt_comm_broadcast_global_vars_impl(chpl_rt_prginfo* prg);

// This "unified" table is allocated at runtime and merges the address table
// of the root program and the runtime together. This approach is not
// compatible with dynamic loading, but some comm layers (such as 'ofi') rely
// upon it and have not been adjusted yet.
extern void** chpl_rt_unified_private_broadcast_table;
extern int chpl_rt_unified_private_broadcast_table_len;
extern bool chpl_rt_use_unified_private_broadcast_table;

// Called by the comm layer to initialize the unified broadcast table.
void chpl_rt_comm_init_unified_private_broadcast_table(void);

// Each program has a table of broadcastable symbols, and so does the runtime.
extern void* chpl_rt_private_broadcast_table_for_rt[];
extern size_t chpl_rt_private_broadcast_table_for_rt_len;
extern size_t chpl_rt_private_broadcast_table_for_rt_byte_lens[];

// These are entries for broadcastable symbols that the runtime needs. They do
// NOT include any per-program symbols. Those can be found in the broadcast
// table for a given Chapel program, see 'chpl_private_broadcast_table' in the
// header 'chpl-prginfo-data-macro.h'.
#define CHPL_RT_RUNTIME_PRIVATE_BROADCAST_TABLE_ENTRIES(MACRO__)  \
  MACRO__(chpl_verbose_comm)                                      \
  MACRO__(chpl_comm_diagnostics)                                  \
  MACRO__(chpl_comm_diags_print_unstable)                         \
  MACRO__(chpl_verbose_comm_stacktrace)                           \
  MACRO__(chpl_verbose_gpu)                                       \
  MACRO__(chpl_gpu_diagnostics)                                   \
  MACRO__(chpl_gpu_diags_print_unstable)                          \
  MACRO__(chpl_verbose_gpu_stacktrace)                            \
  MACRO__(chpl_verbose_mem)

// Helper macros for managing the table of runtime symbols.
#define CHPL_RT_BCAST_TABLE_FOR_RT_PREFIX \
  chpl_rt_runtime_private_broadcast_table_for_rt_
#define CHPL_RT_BCAST_TABLE_FOR_RT_ENTRY(sym__) \
  CHPL_RT_BCAST_TABLE_FOR_RT_PREFIX ## sym__ ## _idx
#define CHPL_RT_BCAST_TABLE_FOR_RT_ENUM_ENTRY(sym__) \
  CHPL_RT_BCAST_TABLE_FOR_RT_PREFIX ## sym__ ## _idx ,

typedef enum chpl_rt_private_broadcast_table_entries_for_rt {
  CHPL_RT_RUNTIME_PRIVATE_BROADCAST_TABLE_ENTRIES(
    CHPL_RT_BCAST_TABLE_FOR_RT_ENUM_ENTRY
  )
  chpl_rt_runtime_private_broadcast_table_for_rt_num_entries
} chpl_rt_private_broadcast_table_entries_for_rt;
#undef CHPL_RT_BCAST_TABLE_FOR_RT_ENTRY_ENUM

static inline void chpl_rt_comm_broadcast_rt_symbol_hook(int32_t idx) {
  assert(0 <= idx && idx < chpl_rt_private_broadcast_table_for_rt_len);

  size_t size = chpl_rt_private_broadcast_table_for_rt_byte_lens[idx];
  int32_t idx_to_use = idx;

  // Adjust to use a "unified index" if needed.
  if (chpl_rt_use_unified_private_broadcast_table) {
    chpl_rt_prginfo* prg = CHPL_RT_PRGINFO_ROOT;
    CHPL_RT_PRGINFO_DECLARE(prg, chpl_private_broadcast_table_len);
    idx_to_use = chpl_private_broadcast_table_len + idx;
  }

  // Passing a 'NULL' program indicates we are asking for a runtime symbol.
  chpl_rt_comm_private_broadcast(NULL, idx_to_use, size);
}

// Runtime code calls this to broadcast a runtime-specific symbol.
#define chpl_rt_comm_broadcast_rt_symbol(sym__) do {                \
  int32_t idx = (int32_t) CHPL_RT_BCAST_TABLE_FOR_RT_ENTRY(sym__);  \
  chpl_rt_comm_broadcast_rt_symbol_hook(idx);                       \
} while (0)

// Per comm layer implementation.
void chpl_rt_comm_private_broadcast_impl(chpl_rt_prginfo* prg, int32_t id,
                                         size_t size);

// Given a program info pointer OR a program ID, fetch a broadcast table.
// If 'prg' is not 'NULL', it will be used, otherwise 'prg_id' will be used
// to translate to a 'chpl_rt_prginfo*' on the current locale. If both
// 'prg' and 'prg_id' are empty then the runtime table will be used instead.
void* chpl_rt_comm_fetch_broadcast_table(chpl_rt_prginfo* prg,
                                         chpl_rt_prg_id prg_id,
                                         size_t* out_table_len);

// Per comm layer implementation.
void chpl_rt_comm_execute_on_impl(chpl_rt_prginfo* prg,
                                  c_nodeid_t node,
                                  c_sublocid_t subloc,
                                  chpl_fn_int_t fid,
                                  chpl_comm_on_bundle_t *arg,
                                  size_t arg_size,
                                  int ln,
                                  int32_t fn);

// Per comm layer implementation.
void chpl_rt_comm_execute_on_fast_impl(chpl_rt_prginfo* prg,
                                       c_nodeid_t node,
                                       c_sublocid_t subloc,
                                       chpl_fn_int_t fid,
                                       chpl_comm_on_bundle_t *arg,
                                       size_t arg_size,
                                       int ln,
                                       int32_t fn);

// Per comm layer implementation.
void chpl_rt_comm_execute_on_nb_impl(chpl_rt_prginfo* prg,
                                     c_nodeid_t node,
                                     c_sublocid_t subloc,
                                     chpl_fn_int_t fid,
                                     chpl_comm_on_bundle_t *arg,
                                     size_t arg_size,
                                     int ln,
                                     int32_t fn);

#ifdef __cplusplus
}
#endif

#endif
