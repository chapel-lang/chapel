/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
  int numTxnsOut;    // number of transactions outstanding
  void* get_buff;
  void* put_buff;
} chpl_comm_taskPrvData_t;

//
// Comm layer private area within executeOn argument bundles.
//
// Members are packed, potentially differently, in each AM request type
// to reduce space requirements.  The 'op' member must come first in all
// cases.
//
// TODO: optimize this
//
typedef uint8_t chpl_comm_amDone_t;

struct chpl_comm_bundleData_base_t {
  uint8_t op;                   // operation
  c_nodeid_t node;              // initiator's node
#ifdef CHPL_COMM_DEBUG
  uint64_t seq;
  uint32_t crc32;
#endif
};

struct chpl_comm_bundleData_execOn_t {
  struct chpl_comm_bundleData_base_t b;
  chpl_bool fast;               // do directly in AM handler; no task
  chpl_fn_int_t fid;            // function table index to call
  uint16_t argSize;             // #bytes in whole arg bundle
  c_sublocid_t subloc;          // target sublocale
  chpl_comm_amDone_t* pAmDone;  // initiator's 'amDone' flag; NULL means nonblk
};

struct chpl_comm_bundleData_execOnLrg_t {
  struct chpl_comm_bundleData_base_t b;
  chpl_fn_int_t fid;            // function table index to call
  uint32_t argSize;             // #bytes in whole arg bundle
  void* arg;                    // address of arg, on initiator
  c_sublocid_t subloc;          // target sublocale
  chpl_comm_amDone_t gotArg;    // initiator's 'got large arg' flag
  chpl_comm_amDone_t* pAmDone;  // initiator's 'amDone' flag; NULL means nonblk
};

struct chpl_comm_bundleData_RMA_t {
  struct chpl_comm_bundleData_base_t b;
  void* addr;                   // address on AM target node
  void* raddr;                  // address on AM initiator's node
  size_t size;                  // number of bytes
  chpl_comm_amDone_t* pAmDone;  // initiator's 'amDone' flag; NULL means nonblk
};

typedef union {
  int32_t i32;
  uint32_t u32;
  chpl_bool32 b32;
  int64_t i64;
  uint64_t u64;
  _real32 r32;
  _real64 r64;
} chpl_amo_datum_t;

struct chpl_comm_bundleData_AMO_t {
  struct chpl_comm_bundleData_base_t b;
  enum fi_op ofiOp;             // ofi AMO op
  enum fi_datatype ofiType;     // ofi object type
  int8_t size;                  // object size (bytes)
  void* obj;                    // object address on target node
  chpl_amo_datum_t operand1;    // first operand, if needed
  chpl_amo_datum_t operand2;    // second operand, if needed
  void* result;                 // result address on initiator's node
  chpl_comm_amDone_t* pAmDone;  // initiator's 'amDone' flag; NULL means nonblk
};

typedef union {
  struct chpl_comm_bundleData_base_t b;
  struct chpl_comm_bundleData_execOn_t xo;
  struct chpl_comm_bundleData_execOnLrg_t xol;
  struct chpl_comm_bundleData_RMA_t rma;
  struct chpl_comm_bundleData_AMO_t amo;
} chpl_comm_bundleData_t;

// The type of the communication handle.
typedef void* chpl_comm_nb_handle_t;

#endif
