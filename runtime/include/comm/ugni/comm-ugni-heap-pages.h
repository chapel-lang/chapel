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

#ifndef _comm_ugni_heap_pages_h_
#define _comm_ugni_heap_pages_h_

//
// System and heap page sizes, for the uGNI communication interface.
//

#include <stdint.h>

size_t chpl_comm_ugni_getSysPageSize(void);
size_t chpl_comm_ugni_getHeapPageSize(void);

#endif // _comm_ugni_heap_pages_h_
