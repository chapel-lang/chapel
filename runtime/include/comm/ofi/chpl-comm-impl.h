/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#ifndef _chpl_comm_impl_h_
#define _chpl_comm_impl_h_

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

//
// Network atomic operations.
//
#include "chpl-comm-native-atomics.h"

#ifdef __cplusplus
extern "C" {
#endif

//
// Remote memory consistency release/acquire hooks.
//
#define CHPL_COMM_IMPL_UNORDERED_TASK_FENCE() \
        chpl_comm_impl_unordered_task_fence()
void chpl_comm_impl_unordered_task_fence(void);

#define CHPL_COMM_IMPL_TASK_CREATE() \
        chpl_comm_impl_task_create()
void chpl_comm_impl_task_create(void);

#define CHPL_COMM_IMPL_TASK_END() \
        chpl_comm_impl_task_end()
void chpl_comm_impl_task_end(void);

#ifdef __cplusplus
}
#endif

#endif // _chpl_comm_impl_h_
