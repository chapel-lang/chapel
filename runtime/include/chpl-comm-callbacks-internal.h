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

#ifndef _chpl_comm_callbacks_internal_h_
#define _chpl_comm_callbacks_internal_h_


#include <stdint.h>
#include <assert.h>

#include "chpl-comm-callbacks.h"

//
// This is here for use in the runtime comm code to quickly
// determine if the code needs to call any callbacks:
// Typical code:
//
//    if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_put)) {
//       chpl_comm_cb_info_t cb_data = {.... actual data ....};
//       chpl_comm_do_callbacks (&cb_data);
//    }
//

int chpl_comm_callback_counts[chpl_comm_cb_num_event_kinds];

static inline
int chpl_comm_have_callbacks(chpl_comm_cb_event_kind_t event_kind) {
  assert(event_kind < chpl_comm_cb_num_event_kinds);
  return (chpl_comm_callback_counts[event_kind] > 0);
}

void chpl_comm_do_callbacks (const chpl_comm_cb_info_t *cb_data);

#endif
