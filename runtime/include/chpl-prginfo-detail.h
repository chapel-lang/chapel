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

#ifndef CHPL_RT_PRGINFO_DETAIL_H
#define CHPL_RT_PRGINFO_DETAIL_H

#include "chpl-prginfo.h"

//
// This file contains "private" symbols that are only meant for use by the
// module code when wiring up program info and registering dynamically
// loaded programs.
//

#ifdef __cplusplus
extern "C" {
#endif

// Expand out an enum that is used internally and by a macro.
typedef enum chpl_rt_prginfo_data_entries {
  #define ENUM_PREFIX__(name__) CHPL_RT_PRGINFO_DATA_ENTRY_IDX(name__)
  #define E_CONSTANT(name__, type__) ENUM_PREFIX__ ## name__ ,
  #define E_CALLBACK(name__, ret_type__, ...) ENUM_PREFIX__ ## name__ ,
  #include "chpl-prginfo-data-macro-adapter.h"
  CHPL_RT_PRGINFO_DATA_ENTRY_COUNT
  #undef ENUM_PREFIX__
} chpl_rt_prginfo_data_entries;

/** Define setters that module code can call to set up program data. */
#define CONCAT(a__, b__) a__##b__
#define PREFIX chpl_rt_prginfo_data_entry_set_
#define SETTER(prefix__, name__) \
  void CONCAT(prefix__, name__)(chpl_rt_prginfo* prg, const void* data);
#define E_CONSTANT(name__, type__) SETTER(PREFIX, name__)
#define E_CALLBACK(name__, ret_type__, ...) SETTER(PREFIX, name__)
#include "chpl-prginfo-data-macro-adapter.h"
#undef PREFIX
#undef SETTER
#undef CONCAT

/** Call to register a new dynamically loaded Chapel program here. If the
    passed program ID is 'null' then the implementation will assign a new
    unique ID. Otherwise it will use the ID that is passed in.

    Only the pointer will be stored - it is assumed that the pointer points
    to memory that exists for the entire duration of the loaded program. The
    passed in pointer is also assumed to point to writeable memory.
*/
chpl_rt_prg_id
chpl_rt_prginfo_register_here_nosync(chpl_rt_prg_id id, chpl_rt_prginfo* prg);

// TODO: Just pass the root program into 'chpl_rt_init'.
/** Call to register the root program here. Returns '1' if registered. */
int chpl_rt_prginfo_register_root_here(chpl_rt_prginfo* prg);

/** Dump the values of data entries to the console. */
void chpl_rt_prginfo_data_entry_dump(int no_addresses);

#ifdef __cplusplus
}
#endif

#endif
