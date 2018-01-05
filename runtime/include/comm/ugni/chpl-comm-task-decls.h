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

#include "chpltypes.h"

// The type of task private data.
#include "chpl-cache-task-decls.h"
#define HAS_CHPL_CACHE_FNS

typedef struct {
  chpl_cache_taskPrvData_t cache_data;
} chpl_comm_taskPrvData_t;

//
// Comm layer private area within executeOn argument bundles
// (bundle.comm)
typedef struct {
  chpl_fn_int_t fid;
  int caller;
  void* done; // where to indicate completion on caller
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

chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, int32_t locale, void* raddr,
                                       size_t size, int32_t typeIndex,
                                       int32_t commID, int ln, int32_t fn);
chpl_bool chpl_comm_test_get_nb(chpl_comm_nb_handle_t handle,
                                int ln, int32_t fn);

#endif
