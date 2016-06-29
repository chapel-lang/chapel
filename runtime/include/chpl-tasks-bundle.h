/*
 * Copyright 2004-2016 Cray Inc.
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

#ifndef _chpl_tasks_bundle_h_
#define _chpl_tasks_bundle_h_

// uses task-layer specific chpl_task_bundleData_t
typedef struct chpl_task_bundle_s {
// don't think I need this  unsigned char header_length;
// instead, wrapper just uses a type with chpl_task_bundle included.
  chpl_bool    serial_state;
  chpl_bool    countRunning;
  chpl_bool    is_executeOn;
  int          lineno;
  int          filename;
  c_sublocid_t requestedSubloc;
  chpl_fn_p    requested_fn;
  chpl_task_bundleData_t task_prv;
} chpl_task_bundle_t;

typedef chpl_task_bundle_t* chpl_task_bundle_p;

#endif
