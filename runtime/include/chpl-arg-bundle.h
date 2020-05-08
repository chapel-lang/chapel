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

#ifndef _chpl_arg_bundle_h_
#define _chpl_arg_bundle_h_

#ifndef LAUNCHER

#include <stdint.h>
#include "chplsys.h"
#include "chpltypes.h"
#include "chpl-comm.h"
#include "chpl-tasks.h"

typedef union {
  chpl_arg_bundle_kind_t kind;
  chpl_comm_on_bundle_t comm;
  chpl_task_bundle_t task;
} chpl_any_arg_bundle_t;

static inline
chpl_bool chpl_argBundleIsTask(void* arg) {
  return ((chpl_any_arg_bundle_t*) arg)->kind == CHPL_ARG_BUNDLE_KIND_TASK;
}

static inline
chpl_task_bundle_t* chpl_argBundleTaskArgBundle(void* arg) {
  chpl_any_arg_bundle_t* aab = (chpl_any_arg_bundle_t*) arg;
  return chpl_argBundleIsTask(arg)
         ? &aab->task
         : &aab->comm.task_bundle;
}

static inline
size_t chpl_argBundleSizeofHdr(void* arg) {
  return chpl_argBundleIsTask(arg)
         ? sizeof(chpl_task_bundle_t)
         : sizeof(chpl_comm_on_bundle_t);
}

#endif // LAUNCHER

#endif
