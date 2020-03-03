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

#ifndef _chpl_comm_impl_h_
#define _chpl_comm_impl_h_

#include <stdint.h>

#include "chpl-mem-desc.h"

//
// This is the comm layer sub-interface for dynamic allocation and
// registration of memory.
//
#define CHPL_COMM_IMPL_REG_MEM_HEAP_INFO(start_p, size_p) \
    chpl_comm_impl_regMemHeapInfo(start_p, size_p)
void chpl_comm_impl_regMemHeapInfo(void** start_p, size_t* size_p);

#define CHPL_COMM_IMPL_REG_MEM_HEAP_PAGE_SIZE() \
        chpl_comm_impl_regMemHeapPageSize()
size_t chpl_comm_impl_regMemHeapPageSize(void);

#define CHPL_COMM_IMPL_REG_MEM_ALLOC_THRESHOLD() \
        chpl_comm_impl_regMemAllocThreshold()
size_t chpl_comm_impl_regMemAllocThreshold(void);

#define CHPL_COMM_IMPL_REG_MEM_ALLOC(size, desc, ln, fn) \
    chpl_comm_impl_regMemAlloc(size, desc, ln, fn)
void* chpl_comm_impl_regMemAlloc(size_t size,
                                 chpl_mem_descInt_t desc, int ln, int32_t fn);

#define CHPL_COMM_IMPL_REG_MEM_POST_ALLOC(p, size) \
  chpl_comm_impl_regMemPostAlloc(p, size)
void chpl_comm_impl_regMemPostAlloc(void* p, size_t size);

#define CHPL_COMM_IMPL_REG_MEM_FREE(p, size) \
        chpl_comm_impl_regMemFree(p, size)
chpl_bool chpl_comm_impl_regMemFree(void* p, size_t size);

//
// Network atomic operations.
//
#include "chpl-comm-native-atomics.h"

//
// Internal statistics gathering and reporting.
//
void chpl_comm_statsStartHere(void);
void chpl_comm_statsReport(chpl_bool);

#endif // _chpl_comm_impl_h_
