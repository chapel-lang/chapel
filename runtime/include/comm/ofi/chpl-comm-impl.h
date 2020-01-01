/*
 * Copyright 2004-2020 Cray Inc.
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
#define CHPL_COMM_IMPL_REG_MEM_HEAP_INFO(start_p, size_p) \
    chpl_comm_impl_regMemHeapInfo(start_p, size_p)
void chpl_comm_impl_regMemHeapInfo(void** start_p, size_t* size_p);

#define CHPL_COMM_IMPL_REG_MEM_HEAP_PAGE_SIZE() \
        chpl_comm_impl_regMemHeapPageSize()
size_t chpl_comm_impl_regMemHeapPageSize(void);

//
// Network atomic operations.
//
#include "chpl-comm-native-atomics.h"

#endif // _chpl_comm_impl_h_
