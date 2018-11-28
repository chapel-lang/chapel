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

//
// Tasking-related macros for the Chapel uGNI communication layer.
//

#ifndef _COMM_TASK_DECLS_H_
#define _COMM_TASK_DECLS_H_

#include <stddef.h>
#include <stdint.h>

#include <rdma/fabric.h>
#include <rdma/fi_domain.h>

#include "chpltypes.h"

typedef struct {
  int dummy;    // structs must be nonempty
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

struct chpl_comm_bundleData_op_t {
  uint8_t op;                   // operation; must come first
};

struct chpl_comm_bundleData_execOn_t {
  uint8_t op;                   // operation; must come first
  chpl_bool fast;               // do directly in AM handler; no task
  chpl_fn_int_t fid;            // function table index to call
  uint16_t argSize;             // #bytes in whole arg bundle
  c_nodeid_t node;              // initiator's node
  c_sublocid_t subloc;          // target sublocale
  chpl_comm_amDone_t* pDone;    // initiator's 'done' flag; nonblocking if NULL
};

struct chpl_comm_bundleData_RMA_t {
  uint8_t op;                   // operation; must come first
  void* addr;                   // address on AM target node
  c_nodeid_t node;              // initiator's node
  void* raddr;                  // initiator's address
  size_t size;                  // number of bytes
  chpl_comm_amDone_t* pDone;    // initiator's 'done' flag; nonblocking if NULL
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
  uint8_t op;                   // operation; must come first
  enum fi_op ofiOp;             // ofi AMO op
  enum fi_datatype ofiType;     // ofi object type
  int8_t size;                  // object size (bytes)
  c_nodeid_t node;              // initiator's node
  void* obj;                    // object address on target node
  chpl_amo_datum_t operand1;    // first operand, if needed
  chpl_amo_datum_t operand2;    // second operand, if needed
  void* result;                 // result address on initiator's node
  chpl_comm_amDone_t* pDone;    // initiator's 'done' flag; nonblocking if NULL
};

typedef union {
  struct chpl_comm_bundleData_op_t op;
  struct chpl_comm_bundleData_execOn_t xo;
  struct chpl_comm_bundleData_RMA_t rma;
  struct chpl_comm_bundleData_AMO_t amo;
} chpl_comm_bundleData_t;

//
// Nonblocking GET support.  Handle is a unique handle for the GET.
// This value is initially returned by chpl_com_get_nb(), and can then
// be passed to chpl_comm_test_get_nb() while polling for the GET to
// complete.  Once chpl_comm_test_get_nb() returns true, however, the
// handle is expired and must not be passed to it again.
//
// Code external to the comm layer must not assume anything about or
// change any of the contents of a nonblocking GET handle.  The only
// supported interface is via the functions described below.
//
// chpl_comm_get_nb()
//   Get 'size' bytes of remote data at 'raddr' on locale 'locale' to
//   local data at 'addr', nonblocking.
//
// chpl_comm_test_get_nb()
//   Return nonzero if the GET associated with the given handle has
//   completed.
//
typedef void* chpl_comm_nb_handle_t;
#ifdef BLAH

chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, int32_t locale, void* raddr,
                                       size_t size, int32_t typeIndex,
                                       int32_t commID, int ln, int32_t fn);
chpl_bool chpl_comm_test_get_nb(chpl_comm_nb_handle_t handle,
                                int ln, int32_t fn);
#endif
#endif
