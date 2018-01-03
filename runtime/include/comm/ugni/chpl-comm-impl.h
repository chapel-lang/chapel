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

#ifndef _chpl_comm_impl_h_
#define _chpl_comm_impl_h_

//
// This is the comm layer sub-interface for dynamic allocation and
// registration of memory.
//
#define CHPL_COMM_IMPL_REG_MEM_ALLOC_THRESHOLD() \
        chpl_comm_impl_regMemAllocThreshold()
size_t chpl_comm_impl_regMemAllocThreshold(void);

#define CHPL_COMM_IMPL_REG_MEM_ALLOC(size) \
        chpl_comm_impl_regMemAlloc(size)
void* chpl_comm_impl_regMemAlloc(size_t size);

#define CHPL_COMM_IMPL_REG_MEM_POST_ALLOC(p, size)   \
  chpl_comm_impl_regMemPostAlloc(p, size)
void chpl_comm_impl_regMemPostAlloc(void* p, size_t size);

#define CHPL_COMM_IMPL_REG_MEM_FREE(p, size) \
        chpl_comm_impl_regMemFree(p, size)
chpl_bool chpl_comm_impl_regMemFree(void* p, size_t size);

//
// Network atomic operations.
//
// We support 32- and 64-bit signed integers and reals, although we
// don't necessarily support all of these types for all operations.
// In the future we might like to add other types, such as unsigned
// integers.
//

//
// Do a remote atomic store.  The value to be stored is *desired on
// the local locale.  The target location to be stored into is *object
// on the given locale.  This differs from a regular chpl_comm_put()
// in that it is coherent with other chpl_comm_atomic_*() operations.
// However, it may incur the overhead of a network operation even if
// locale refers to the calling locale.
//
#define DECL_CHPL_COMM_ATOMIC_PUT(type)                                 \
        void chpl_comm_atomic_put_ ## type                              \
            (void* desired, int32_t locale, void* object,               \
             int ln, int32_t fn);

DECL_CHPL_COMM_ATOMIC_PUT(int32)
DECL_CHPL_COMM_ATOMIC_PUT(int64)
DECL_CHPL_COMM_ATOMIC_PUT(uint32)
DECL_CHPL_COMM_ATOMIC_PUT(uint64)
DECL_CHPL_COMM_ATOMIC_PUT(real32)
DECL_CHPL_COMM_ATOMIC_PUT(real64)

//
// Do a remote atomic load.  The source location is *object on the
// given locale.  The value is returned in *result on the local
// locale.  This differs from a regular chpl_comm_get() in that it is
// coherent with other chpl_comm_atomic_*() operations.  However, it
// may incur the overhead of a network operation even if locale refers
// to the calling locale.
//
#define DECL_CHPL_COMM_ATOMIC_GET(type)                                 \
        void chpl_comm_atomic_get_ ## type                              \
            (void* result, int32_t locale, void* object,                \
             int ln, int32_t fn);

DECL_CHPL_COMM_ATOMIC_GET(int32)
DECL_CHPL_COMM_ATOMIC_GET(int64)
DECL_CHPL_COMM_ATOMIC_GET(uint32)
DECL_CHPL_COMM_ATOMIC_GET(uint64)
DECL_CHPL_COMM_ATOMIC_GET(real32)
DECL_CHPL_COMM_ATOMIC_GET(real64)

//
// Do a remote atomic exchange.  The value to be stored is *desired on
// the local locale.  The target location to be stored into is *object
// on the given locale.  The value previously stored there is returned
// in *result on the local locale.
//
#define DECL_CHPL_COMM_ATOMIC_XCHG(type)                                \
        void chpl_comm_atomic_xchg_ ## type                             \
            (void* desired, int32_t locale, void* object,               \
             void* result,                                              \
             int ln, int32_t fn);

DECL_CHPL_COMM_ATOMIC_XCHG(int32)
DECL_CHPL_COMM_ATOMIC_XCHG(int64)
DECL_CHPL_COMM_ATOMIC_XCHG(uint32)
DECL_CHPL_COMM_ATOMIC_XCHG(uint64)
DECL_CHPL_COMM_ATOMIC_XCHG(real32)
DECL_CHPL_COMM_ATOMIC_XCHG(real64)

//
// Do a remote atomic compare and exchange.  The value to be matched
// is *expected on the local locale.  If the match succeeds, the value
// to be stored is *desired on the local locale.  The target location
// to be stored into is *object on the given locale.  Whether the
// exchange occurred or not is returned in *result on the local
// locale.
//
#define DECL_CHPL_COMM_ATOMIC_CMPXCHG(type)                             \
        void chpl_comm_atomic_cmpxchg_ ## type                          \
            (void* expected, void* desired,                             \
             int32_t locale, void* object, chpl_bool32* result,         \
             int ln, int32_t fn);

DECL_CHPL_COMM_ATOMIC_CMPXCHG(int32)
DECL_CHPL_COMM_ATOMIC_CMPXCHG(int64)
DECL_CHPL_COMM_ATOMIC_CMPXCHG(uint32)
DECL_CHPL_COMM_ATOMIC_CMPXCHG(uint64)
DECL_CHPL_COMM_ATOMIC_CMPXCHG(real32)
DECL_CHPL_COMM_ATOMIC_CMPXCHG(real64)

//
// Do a remote atomic binary operation, non-fetching or fetching.  In
// either case, the operand is *operand on the local locale and the
// target location is *object on the given locale.  For the fetching
// style, the value the target had prior to the operation is returned
// in *result on the local locale.
//
// We support AND, OR, and XOR for integers, and ADD and SUB for
// integers and reals.  In the future we might like to add other
// operations, such as MIN and MAX.
//
//
#define DECL_CHPL_COMM_ATOMIC_NONFETCH_BINARY(op, type)                 \
        void chpl_comm_atomic_ ## op ## _ ## type                       \
                (void* operand, int32_t locale, void* object,           \
                 int ln, int32_t fn);
#define DECL_CHPL_COMM_ATOMIC_FETCH_BINARY(op, type)                    \
        void chpl_comm_atomic_fetch_ ## op ## _ ## type                 \
                (void* operand, int32_t locale, void* object,           \
                 void* result,                                          \
                 int ln, int32_t fn);
#define DECL_CHPL_COMM_ATOMIC_BINARY(op, type)                          \
        DECL_CHPL_COMM_ATOMIC_NONFETCH_BINARY(op, type)                 \
        DECL_CHPL_COMM_ATOMIC_FETCH_BINARY(op, type)

DECL_CHPL_COMM_ATOMIC_BINARY(and, int32)
DECL_CHPL_COMM_ATOMIC_BINARY(and, int64)
DECL_CHPL_COMM_ATOMIC_BINARY(and, uint32)
DECL_CHPL_COMM_ATOMIC_BINARY(and, uint64)

DECL_CHPL_COMM_ATOMIC_BINARY(or, int32)
DECL_CHPL_COMM_ATOMIC_BINARY(or, int64)
DECL_CHPL_COMM_ATOMIC_BINARY(or, uint32)
DECL_CHPL_COMM_ATOMIC_BINARY(or, uint64)

DECL_CHPL_COMM_ATOMIC_BINARY(xor, int32)
DECL_CHPL_COMM_ATOMIC_BINARY(xor, int64)
DECL_CHPL_COMM_ATOMIC_BINARY(xor, uint32)
DECL_CHPL_COMM_ATOMIC_BINARY(xor, uint64)

DECL_CHPL_COMM_ATOMIC_BINARY(add, int32)
DECL_CHPL_COMM_ATOMIC_BINARY(add, int64)
DECL_CHPL_COMM_ATOMIC_BINARY(add, uint32)
DECL_CHPL_COMM_ATOMIC_BINARY(add, uint64)
DECL_CHPL_COMM_ATOMIC_BINARY(add, real32)
DECL_CHPL_COMM_ATOMIC_BINARY(add, real64)

DECL_CHPL_COMM_ATOMIC_BINARY(sub, int32)
DECL_CHPL_COMM_ATOMIC_BINARY(sub, int64)
DECL_CHPL_COMM_ATOMIC_BINARY(sub, uint32)
DECL_CHPL_COMM_ATOMIC_BINARY(sub, uint64)
DECL_CHPL_COMM_ATOMIC_BINARY(sub, real32)
DECL_CHPL_COMM_ATOMIC_BINARY(sub, real64)

//
// Internal statistics gathering and reporting.
//
void chpl_comm_statsStartHere(void);
void chpl_comm_statsReport(chpl_bool);

#endif // _chpl_comm_impl_h_
