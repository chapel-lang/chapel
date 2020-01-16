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

#include <assert.h>

#include "chplrt.h"

#include "chpl-comm.h"
#include "error.h"
#include "chpl-comm-callbacks.h"
#include "chpl-comm-callbacks-internal.h"

//
// Communication callback support
//
#define MAX_CBS_PER_EVENT 10


//
// Storage for call back information
//
static struct cb_info {
  chpl_comm_cb_fn_t fns[MAX_CBS_PER_EVENT];
} cb_info[chpl_comm_cb_num_event_kinds];

int chpl_comm_callback_counts[chpl_comm_cb_num_event_kinds] = {0};


//
// Communication callback support.
//
int chpl_comm_install_callback(chpl_comm_cb_event_kind_t event_kind,
                               chpl_comm_cb_fn_t cb_fn) {
  int i;

  if (event_kind >= chpl_comm_cb_num_event_kinds) {
    errno = ERANGE;
    return -1;
  }

  i = chpl_comm_callback_counts[event_kind];

  if (i >= MAX_CBS_PER_EVENT) {
    errno = ENOMEM;
    return -1;
  }

  chpl_comm_callback_counts[event_kind]++;
  cb_info[event_kind].fns[i]= cb_fn;

  return 0;
}


int chpl_comm_uninstall_callback(chpl_comm_cb_event_kind_t event_kind,
                                 chpl_comm_cb_fn_t cb_fn) {
  int i;
  int found_i;

  if (event_kind >= chpl_comm_cb_num_event_kinds) {
    errno = ERANGE;
    return -1;
  }

  for (i = 0, found_i = -1; i < chpl_comm_callback_counts[event_kind]; i++) {
    if (cb_info[event_kind].fns[i] == cb_fn) {
      found_i = i;
      break;
    }
  }

  if (found_i < 0) {
    errno = ENOENT;
    return -1;
  }

  for (i = found_i + 1; i < chpl_comm_callback_counts[event_kind]; i++) {
    cb_info[event_kind].fns[i - 1] = cb_info[event_kind].fns[i];
  }

  chpl_comm_callback_counts[event_kind]--;

  return 0;
}

void chpl_comm_do_callbacks(const chpl_comm_cb_info_t *info)
{
  int i;
  struct cb_info *cb;

  // Don't do anything if the event kind is bad
  if (info->event_kind >= chpl_comm_cb_num_event_kinds)
    return;

  // Call the callbacks
  cb = &cb_info[info->event_kind];
  for (i = 0; i < chpl_comm_callback_counts[info->event_kind]; i++) {
    (cb->fns[i])(info);
  }
}
