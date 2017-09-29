/*
 * Copyright 2004-2017 Cray Inc.
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

#ifndef _chpl_comm_count_calls_h_
#define _chpl_comm_count_calls_h_

#include "chpl-atomics.h"

// Optional counting of calls to gen_comm_get/put
// Uncomment the following line to enable it
//#define CHPL_COMM_COUNT_CALLS 1
extern atomic_uint_least64_t chpl_comm_n_get_calls;
extern atomic_uint_least64_t chpl_comm_n_put_calls;
extern atomic_uint_least64_t chpl_comm_n_prefetch_calls;

#endif

