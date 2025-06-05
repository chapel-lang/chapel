/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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
#include "chpltypes.h"
#include "chpl-atomics.h"

#ifndef _chpl_comm_native_atomics_h_
#define _chpl_comm_native_atomics_h_

#ifdef __cplusplus
extern "C" {
#endif

//
// These are the declarations for network-native atomic ops, for the
// comm layers that support those.
//

//
// We support 32- and 64-bit signed ints, uints and reals, although we
// don't necessarily support all of these types for all operations.
//

//
// Do a remote atomic write.  The value to be written is *desired on the
// local node.  The target location to be written into is *object on the
// given node.  This differs from chpl_comm_put() in that it is coherent
// with other chpl_comm_atomic_*() operations (but may incur network
// overhead) even if the target is the calling node.
//
#define DECL_CHPL_COMM_ATOMIC_WRITE(type)                               \
  void chpl_comm_atomic_write_ ## type                                  \
         (void* desired, c_nodeid_t node, void* object,                 \
          chpl_memory_order order, int ln, int32_t fn);

DECL_CHPL_COMM_ATOMIC_WRITE(int8)
DECL_CHPL_COMM_ATOMIC_WRITE(int16)
DECL_CHPL_COMM_ATOMIC_WRITE(int32)
DECL_CHPL_COMM_ATOMIC_WRITE(int64)
DECL_CHPL_COMM_ATOMIC_WRITE(uint8)
DECL_CHPL_COMM_ATOMIC_WRITE(uint16)
DECL_CHPL_COMM_ATOMIC_WRITE(uint32)
DECL_CHPL_COMM_ATOMIC_WRITE(uint64)
DECL_CHPL_COMM_ATOMIC_WRITE(real32)
DECL_CHPL_COMM_ATOMIC_WRITE(real64)

//
// Do a remote atomic read.  The source location is *object on the
// given node.  The value is returned in *result on the local node.
// This differs from chpl_comm_get() in that it is coherent with
// other chpl_comm_atomic_*() operations (but may incur network
// overhead) even if the source is the calling node.
//
#define DECL_CHPL_COMM_ATOMIC_READ(type)                                \
  void chpl_comm_atomic_read_ ## type                                   \
         (void* result, c_nodeid_t node, void* object,                  \
          chpl_memory_order order, int ln, int32_t fn);

DECL_CHPL_COMM_ATOMIC_READ(int8)
DECL_CHPL_COMM_ATOMIC_READ(int16)
DECL_CHPL_COMM_ATOMIC_READ(int32)
DECL_CHPL_COMM_ATOMIC_READ(int64)
DECL_CHPL_COMM_ATOMIC_READ(uint8)
DECL_CHPL_COMM_ATOMIC_READ(uint16)
DECL_CHPL_COMM_ATOMIC_READ(uint32)
DECL_CHPL_COMM_ATOMIC_READ(uint64)
DECL_CHPL_COMM_ATOMIC_READ(real32)
DECL_CHPL_COMM_ATOMIC_READ(real64)

//
// Do a remote atomic exchange.  The value to be stored is *desired on
// the local node.  The target location to be stored into is *object
// on the given node.  The value previously stored there is returned
// in *result on the local node.
//
#define DECL_CHPL_COMM_ATOMIC_XCHG(type)                                \
  void chpl_comm_atomic_xchg_ ## type                                   \
         (void* desired, c_nodeid_t node, void* object, void* result,   \
          chpl_memory_order order, int ln, int32_t fn);

DECL_CHPL_COMM_ATOMIC_XCHG(int8)
DECL_CHPL_COMM_ATOMIC_XCHG(int16)
DECL_CHPL_COMM_ATOMIC_XCHG(int32)
DECL_CHPL_COMM_ATOMIC_XCHG(int64)
DECL_CHPL_COMM_ATOMIC_XCHG(uint8)
DECL_CHPL_COMM_ATOMIC_XCHG(uint16)
DECL_CHPL_COMM_ATOMIC_XCHG(uint32)
DECL_CHPL_COMM_ATOMIC_XCHG(uint64)
DECL_CHPL_COMM_ATOMIC_XCHG(real32)
DECL_CHPL_COMM_ATOMIC_XCHG(real64)

//
// Do a remote atomic compare and exchange.  The value to be matched is
// *expected on the local node.  If the match succeeds, the value to be
// stored is *desired on the local node, otherwise the old value is
// stored in *expected.  The target location to be stored into is
// *object on the given node.  Whether the exchange occurred or not is
// returned in *result on the local node.
//
#define DECL_CHPL_COMM_ATOMIC_CMPXCHG(type)                             \
  void chpl_comm_atomic_cmpxchg_ ## type                                \
         (void* expected, void* desired, c_nodeid_t node, void* object, \
          chpl_bool32* result, chpl_memory_order succ, chpl_memory_order fail,    \
          int ln, int32_t fn);

DECL_CHPL_COMM_ATOMIC_CMPXCHG(int8)
DECL_CHPL_COMM_ATOMIC_CMPXCHG(int16)
DECL_CHPL_COMM_ATOMIC_CMPXCHG(int32)
DECL_CHPL_COMM_ATOMIC_CMPXCHG(int64)
DECL_CHPL_COMM_ATOMIC_CMPXCHG(uint8)
DECL_CHPL_COMM_ATOMIC_CMPXCHG(uint16)
DECL_CHPL_COMM_ATOMIC_CMPXCHG(uint32)
DECL_CHPL_COMM_ATOMIC_CMPXCHG(uint64)
DECL_CHPL_COMM_ATOMIC_CMPXCHG(real32)
DECL_CHPL_COMM_ATOMIC_CMPXCHG(real64)

//
// Do a remote atomic binary operation, non-fetching or fetching.  In
// either case, the operand is *operand on the local node and the target
// location is *object on the given node.  For the fetching style, the
// value the target had prior to the operation is returned in *result on
// the local node.
//
// We support AND, OR, and XOR for various int types, and ADD and SUB
// for both int and real types.
//
#define DECL_CHPL_COMM_ATOMIC_NONFETCH_BINARY(op, type)                 \
  void chpl_comm_atomic_ ## op ## _ ## type                             \
         (void* operand, c_nodeid_t node, void* object,                 \
          chpl_memory_order order, int ln, int32_t fn);
#define DECL_CHPL_COMM_ATOMIC_NONFETCH_UNORDERED_BINARY(op, type)       \
  void chpl_comm_atomic_ ## op ## _unordered_ ## type                   \
         (void* operand, c_nodeid_t node, void* object,                 \
          int ln, int32_t fn);
#define DECL_CHPL_COMM_ATOMIC_FETCH_BINARY(op, type)                    \
  void chpl_comm_atomic_fetch_ ## op ## _ ## type                       \
         (void* operand, c_nodeid_t node, void* object, void* result,   \
          chpl_memory_order order, int ln, int32_t fn);
#define DECL_CHPL_COMM_ATOMIC_BINARY(op, type)                          \
  DECL_CHPL_COMM_ATOMIC_NONFETCH_BINARY(op, type)                       \
  DECL_CHPL_COMM_ATOMIC_NONFETCH_UNORDERED_BINARY(op, type)             \
  DECL_CHPL_COMM_ATOMIC_FETCH_BINARY(op, type)

DECL_CHPL_COMM_ATOMIC_BINARY(and, int8)
DECL_CHPL_COMM_ATOMIC_BINARY(and, int16)
DECL_CHPL_COMM_ATOMIC_BINARY(and, int32)
DECL_CHPL_COMM_ATOMIC_BINARY(and, int64)
DECL_CHPL_COMM_ATOMIC_BINARY(and, uint8)
DECL_CHPL_COMM_ATOMIC_BINARY(and, uint16)
DECL_CHPL_COMM_ATOMIC_BINARY(and, uint32)
DECL_CHPL_COMM_ATOMIC_BINARY(and, uint64)

DECL_CHPL_COMM_ATOMIC_BINARY(or, int8)
DECL_CHPL_COMM_ATOMIC_BINARY(or, int16)
DECL_CHPL_COMM_ATOMIC_BINARY(or, int32)
DECL_CHPL_COMM_ATOMIC_BINARY(or, int64)
DECL_CHPL_COMM_ATOMIC_BINARY(or, uint8)
DECL_CHPL_COMM_ATOMIC_BINARY(or, uint16)
DECL_CHPL_COMM_ATOMIC_BINARY(or, uint32)
DECL_CHPL_COMM_ATOMIC_BINARY(or, uint64)

DECL_CHPL_COMM_ATOMIC_BINARY(xor, int8)
DECL_CHPL_COMM_ATOMIC_BINARY(xor, int16)
DECL_CHPL_COMM_ATOMIC_BINARY(xor, int32)
DECL_CHPL_COMM_ATOMIC_BINARY(xor, int64)
DECL_CHPL_COMM_ATOMIC_BINARY(xor, uint8)
DECL_CHPL_COMM_ATOMIC_BINARY(xor, uint16)
DECL_CHPL_COMM_ATOMIC_BINARY(xor, uint32)
DECL_CHPL_COMM_ATOMIC_BINARY(xor, uint64)


DECL_CHPL_COMM_ATOMIC_BINARY(add, int8)
DECL_CHPL_COMM_ATOMIC_BINARY(add, int16)
DECL_CHPL_COMM_ATOMIC_BINARY(add, int32)
DECL_CHPL_COMM_ATOMIC_BINARY(add, int64)
DECL_CHPL_COMM_ATOMIC_BINARY(add, uint8)
DECL_CHPL_COMM_ATOMIC_BINARY(add, uint16)
DECL_CHPL_COMM_ATOMIC_BINARY(add, uint32)
DECL_CHPL_COMM_ATOMIC_BINARY(add, uint64)
DECL_CHPL_COMM_ATOMIC_BINARY(add, real32)
DECL_CHPL_COMM_ATOMIC_BINARY(add, real64)

DECL_CHPL_COMM_ATOMIC_BINARY(sub, int8)
DECL_CHPL_COMM_ATOMIC_BINARY(sub, int16)
DECL_CHPL_COMM_ATOMIC_BINARY(sub, int32)
DECL_CHPL_COMM_ATOMIC_BINARY(sub, int64)
DECL_CHPL_COMM_ATOMIC_BINARY(sub, uint8)
DECL_CHPL_COMM_ATOMIC_BINARY(sub, uint16)
DECL_CHPL_COMM_ATOMIC_BINARY(sub, uint32)
DECL_CHPL_COMM_ATOMIC_BINARY(sub, uint64)
DECL_CHPL_COMM_ATOMIC_BINARY(sub, real32)
DECL_CHPL_COMM_ATOMIC_BINARY(sub, real64)

DECL_CHPL_COMM_ATOMIC_BINARY(min, int8)
DECL_CHPL_COMM_ATOMIC_BINARY(min, int16)
DECL_CHPL_COMM_ATOMIC_BINARY(min, int32)
DECL_CHPL_COMM_ATOMIC_BINARY(min, int64)
DECL_CHPL_COMM_ATOMIC_BINARY(min, uint8)
DECL_CHPL_COMM_ATOMIC_BINARY(min, uint16)
DECL_CHPL_COMM_ATOMIC_BINARY(min, uint32)
DECL_CHPL_COMM_ATOMIC_BINARY(min, uint64)
DECL_CHPL_COMM_ATOMIC_BINARY(min, real32)
DECL_CHPL_COMM_ATOMIC_BINARY(min, real64)

DECL_CHPL_COMM_ATOMIC_BINARY(max, int8)
DECL_CHPL_COMM_ATOMIC_BINARY(max, int16)
DECL_CHPL_COMM_ATOMIC_BINARY(max, int32)
DECL_CHPL_COMM_ATOMIC_BINARY(max, int64)
DECL_CHPL_COMM_ATOMIC_BINARY(max, uint8)
DECL_CHPL_COMM_ATOMIC_BINARY(max, uint16)
DECL_CHPL_COMM_ATOMIC_BINARY(max, uint32)
DECL_CHPL_COMM_ATOMIC_BINARY(max, uint64)
DECL_CHPL_COMM_ATOMIC_BINARY(max, real32)
DECL_CHPL_COMM_ATOMIC_BINARY(max, real64)

void chpl_comm_atomic_unordered_task_fence(void);

#ifdef __cplusplus
}
#endif

#endif // _chpl_comm_native_atomics_h_
