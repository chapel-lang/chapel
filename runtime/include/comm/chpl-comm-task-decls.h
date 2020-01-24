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

// This is the last-resort version of this file.  It will be used only
// if the comm layer implementation does not supply one.

#ifndef _chpl_comm_task_decls_h
#define _chpl_comm_task_decls_h

// Define the type of a n.b. communications handle.
typedef void* chpl_comm_nb_handle_t;

typedef struct {
  int dummy;    // structs must be nonempty
} chpl_comm_taskPrvData_t;

//
// Comm layer private area within executeOn argument bundles
// (bundle.comm)
typedef struct {
  int64_t dummy;    // structs must be nonempty
} chpl_comm_bundleData_t;

#undef HAS_CHPL_CACHE_FNS

#endif
