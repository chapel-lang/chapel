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

#ifndef _COMM_TASK_DECLS_H_
#define _COMM_TASK_DECLS_H_

#include <stddef.h>
#include <stdint.h>

#include <rdma/fabric.h>
#include <rdma/fi_domain.h>

#include "chpltypes.h"

// The type of task private data.
#include "chpl-cache-task-decls.h"
#define HAS_CHPL_CACHE_FNS

typedef struct {
  chpl_cache_taskPrvData_t cache_data;
  void* amo_nf_buff;
  void* get_buff;
  void* put_buff;
} chpl_comm_taskPrvData_t;

//
// Comm layer private area within executeOn argument bundles.
//
typedef struct {
  chpl_bool fast;               // do directly in AM handler; no task
  chpl_fn_int_t fid;            // function table index to call
  c_nodeid_t node;              // initiator's node
  c_sublocid_t subloc;          // target sublocale
  size_t argSize;               // #bytes in whole arg bundle
  void* pAmDone;                // initiator's 'amDone' flag; NULL means nonblk
#ifdef CHPL_COMM_DEBUG
  uint64_t seq;
  uint32_t crc;
#endif
} chpl_comm_bundleData_t;

// The type of the communication handle.
typedef void* chpl_comm_nb_handle_t;

#endif
