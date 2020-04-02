/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
// Shared code for tasking callbacks.
//
#include "chplrt.h"

#include "chpl-comm.h"
#include "error.h"
#include "chpl-tasks-callbacks.h"
#include "chpl-tasks-callbacks-internal.h"


//
// Tasking callback support.
//
#define MAX_CBS_PER_EVENT 10

static struct cb_info {
  chpl_task_cb_fn_t fns[MAX_CBS_PER_EVENT];
  chpl_task_cb_info_kind_t info_kinds[MAX_CBS_PER_EVENT];
} cb_info[chpl_task_cb_num_event_kinds];


//
// Tasking callback support.
//
int chpl_task_install_callback(chpl_task_cb_event_kind_t event_kind,
                               chpl_task_cb_info_kind_t info_kind,
                               chpl_task_cb_fn_t cb_fn) {
  int i;

  if (event_kind >= chpl_task_cb_num_event_kinds) {
    errno = ERANGE;
    return -1;
  }

  i = chpl_task_callback_counts[event_kind];

  if (i >= MAX_CBS_PER_EVENT) {
    errno = ENOMEM;
    return -1;
  }

  chpl_task_callback_counts[event_kind]++;

  cb_info[event_kind].fns[i]= cb_fn;
  cb_info[event_kind].info_kinds[i] = info_kind;

  return 0;
}


int chpl_task_uninstall_callback(chpl_task_cb_event_kind_t event_kind,
                                 chpl_task_cb_fn_t cb_fn) {
  int i;
  int found_i;

  if (event_kind >= chpl_task_cb_num_event_kinds) {
    errno = ERANGE;
    return -1;
  }

  for (i = 0, found_i = -1; i < chpl_task_callback_counts[event_kind]; i++) {
    if (cb_info[event_kind].fns[i] == cb_fn) {
      found_i = i;
      break;
    }
  }

  if (found_i < 0) {
    errno = ENOENT;
    return -1;
  }

  for (i = found_i + 1; i < chpl_task_callback_counts[event_kind]; i++) {
    cb_info[event_kind].fns[i - 1] = cb_info[event_kind].fns[i];
    cb_info[event_kind].info_kinds[i - 1] = cb_info[event_kind].info_kinds[i];
  }

  chpl_task_callback_counts[event_kind]--;

  return 0;
}


void chpl_task_do_callbacks_internal(chpl_task_cb_event_kind_t event_kind,
                                     chpl_fn_int_t fid,
                                     int32_t filename,
                                     int lineno,
                                     uint64_t id,
                                     int is_executeOn) {
  struct cb_info* cbp;
  chpl_task_cb_info_t info;
  int i;

  cbp = &cb_info[event_kind];

  info.nodeID = chpl_nodeID;
  info.event_kind = event_kind;

  for (i = 0; i < chpl_task_callback_counts[event_kind]; i++) {
    info.info_kind = cbp->info_kinds[i];

    switch (cbp->info_kinds[i]) {
    case chpl_task_cb_info_kind_full:
      info.iu.full.fid = fid;
      info.iu.full.filename = filename;
      info.iu.full.lineno = lineno;
      info.iu.full.id = id;
      info.iu.full.is_executeOn = is_executeOn;
      break;

    case chpl_task_cb_info_kind_id_only:
      info.iu.id_only.id = id;
      break;

    default:
      chpl_internal_error("bad callback info kind");
      break;
    }

    (*cbp->fns[i])((const chpl_task_cb_info_t*) &info);
  }
}
