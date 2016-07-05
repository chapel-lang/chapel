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

// TODO -- make this entirely tasking layer private
// and make impl define chpl_task_bundle_t instead of chpl_task_bundleData_t.

// uses task-layer specific chpl_task_bundleData_t
typedef struct chpl_task_bundle_s {
  // Values are public here so that a communication layer
  // can make use of them and reduce duplication.

  unsigned int serial_state :  1;   // used in gasnet !small !large
  unsigned int countRunning :  1;   
  unsigned int is_executeOn :  1;
  unsigned int lineno       : 29;
  int          filename;

  c_sublocid_t requestedSubloc;  // used in gasnet !small !large

  chpl_fn_p    requested_fn; // can this turn into an fid?
                             // comm layer task wrapper fns, such as
                             // to handle getting the argument bundle for
                             // the "large" case, make always using fid
                             // difficult

  chpl_task_bundleData_t task_prv; // typically stores 8 byte task ID
} chpl_task_bundle_t;

typedef chpl_task_bundle_t* chpl_task_bundle_p;

#endif
