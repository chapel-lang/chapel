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

#ifndef _chpl_tasks_callbacks_internal_h_
#define _chpl_tasks_callbacks_internal_h_

#include <assert.h>
#include <stdint.h>

#include "chpl-tasks-callbacks.h"


//
// Don't refer to this directly.  It's only here in public to support
// reducing overhead by inlining the test for >0.
//
int chpl_task_callback_counts[chpl_task_cb_num_event_kinds];


void chpl_task_do_callbacks_internal(chpl_task_cb_event_kind_t,
                                     chpl_fn_int_t fid,
                                     int32_t filename,
                                     int lineno,
                                     uint64_t id,
                                     int is_executeOn);


static inline
int chpl_task_have_callbacks(chpl_task_cb_event_kind_t event_kind) {
  assert(event_kind < chpl_task_cb_num_event_kinds);
  return (chpl_task_callback_counts[event_kind] > 0);
}


static inline
void chpl_task_do_callbacks(chpl_task_cb_event_kind_t event_kind,
                            chpl_fn_int_t fid,
                            int32_t filename,
                            int lineno,
                            uint64_t id,
                            int is_executeOn) {
  if (chpl_task_have_callbacks(event_kind))
    chpl_task_do_callbacks_internal(event_kind,
                                    fid, filename, lineno, id, is_executeOn);
}

#endif
