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

#include "chplrt.h"
#include "chpl-env-gen.h"

#include "rdma/fabric.h"
#include "rdma/fi_domain.h"
#include "rdma/fi_errno.h"

#include "chpl-comm.h"
#include "chpl-comm-callbacks.h"
#include "chpl-comm-callbacks-internal.h"
#include "chpl-gen-includes.h"

// Don't get warning macros for chpl_comm_get etc
#include "chpl-comm-no-warning-macros.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "comm-ofi-internal.h"

static struct ofi_stuff* ofi = NULL;
void chpl_comm_ofi_am_init(struct ofi_stuff* _ofi) {
  if (ofi == NULL) {
    ofi = _ofi;
  } else {
    chpl_warning("ofi AM already initialized.  Ignoring", 0, 0);
  }
}

static void execute_on_common(c_nodeid_t node, c_sublocid_t subloc,
                              chpl_fn_int_t fid,
                              chpl_comm_on_bundle_t* arg, size_t arg_size,
                              chpl_bool fast, chpl_bool blocking);

void chpl_comm_execute_on(c_nodeid_t node, c_sublocid_t subloc,
                          chpl_fn_int_t fid,
                          chpl_comm_on_bundle_t *arg, size_t arg_size) {
  assert(node != chpl_nodeID); // handled by the locale model

  CHPL_COMM_DIAGS_INC(execute_on);

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn)) {
    chpl_comm_cb_info_t cb_data = 
      {chpl_comm_cb_event_kind_executeOn, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, arg_size}};
    chpl_comm_do_callbacks (&cb_data);
  }

  execute_on_common(node, subloc, fid, arg, arg_size, false, true);
}

#ifdef BLAH
static void fork_nb_wrapper(chpl_comm_on_bundle_t *f) {
  chpl_ftable_call(f->task_bundle.requested_fid, f);
}
#endif

void chpl_comm_execute_on_nb(c_nodeid_t node, c_sublocid_t subloc,
                             chpl_fn_int_t fid,
                             chpl_comm_on_bundle_t *arg, size_t arg_size) {
  assert(node != chpl_nodeID); // handled by the locale model

  CHPL_COMM_DIAGS_INC(execute_on_nb);

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_nb)) {
    chpl_comm_cb_info_t cb_data = 
      {chpl_comm_cb_event_kind_executeOn_nb, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, arg_size}};
    chpl_comm_do_callbacks (&cb_data);
  }

  execute_on_common(node, subloc, fid, arg, arg_size, false, false);
}

void chpl_comm_execute_on_fast(c_nodeid_t node, c_sublocid_t subloc,
                               chpl_fn_int_t fid,
                               chpl_comm_on_bundle_t *arg, size_t arg_size) {
  assert(node != chpl_nodeID); // handled by the locale model

  CHPL_COMM_DIAGS_INC(execute_on_fast);

  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_fast)) {
    chpl_comm_cb_info_t cb_data = 
      {chpl_comm_cb_event_kind_executeOn_fast, chpl_nodeID, node,
       .iu.executeOn={subloc, fid, arg, arg_size}};
    chpl_comm_do_callbacks (&cb_data);
  }

  execute_on_common(node, subloc, fid, arg, arg_size, true, true);
}

static void execute_on_common(c_nodeid_t node, c_sublocid_t subloc,
                              chpl_fn_int_t fid,
                              chpl_comm_on_bundle_t* arg, size_t arg_size,
                              chpl_bool fast, chpl_bool blocking)
{
  chpl_internal_error("Remote ons not yet implemented");

  // bundle args
  // send to remote AM port
}

//
//
//

void chpl_comm_ofi_am_handler(struct fi_cq_data_entry* cqe) {

}
