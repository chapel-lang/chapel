/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#include "chplrt.h"

#include "chpl-comm.h"
#include "chpl-comm-internal.h"
#include "chpl-comm-strd-xfer.h"
#include "chplexit.h"
#include "error.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"

#include "chplcgfns.h"
#include "chpl-gen-includes.h"
#include "chpl-linefile-support.h"

// Don't get warning macros for chpl_comm_get etc
#include "chpl-comm-no-warning-macros.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>

// Helper functions

static int mysystem(const char* command, const char* description,
                    int ignorestatus) {
  int status = system(command);

  if (status == -1) {
    chpl_error("system() fork failed", 0, CHPL_FILE_IDX_COMMAND_LINE);
  } else if (status != 0 && !ignorestatus) {
    chpl_error(description, 0, CHPL_FILE_IDX_COMMAND_LINE);
  }

  return status;
}

// Chapel interface
chpl_comm_nb_handle_t chpl_comm_put_nb(void *addr, c_nodeid_t node, void* raddr,
                                       size_t size, int32_t commID,
                                       int ln, int32_t fn)
{
  assert(node == 0);
  memmove(raddr, addr, size);
  return NULL;
}

chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, c_nodeid_t node, void* raddr,
                                       size_t size, int32_t commID,
                                       int ln, int32_t fn)
{
  assert(node == 0);
  memmove(addr, raddr, size);
  return NULL;
}

int chpl_comm_test_nb_complete(chpl_comm_nb_handle_t h)
{
  return ((void*) h) == NULL;
}

void chpl_comm_wait_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles)
{
  size_t i;
  for( i = 0; i < nhandles; i++ ) {
    assert(h[i] == NULL);
  }
}

int chpl_comm_try_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles)
{
  size_t i;
  for( i = 0; i < nhandles; i++ ) {
    assert(h[i] == NULL);
  }
  return 0;
}

void chpl_comm_free_nb_handle(chpl_comm_nb_handle_t h) {
  assert(h == NULL);
}

int chpl_comm_addr_gettable(c_nodeid_t node, void* start, size_t len)
{
  return 0;
}

int32_t chpl_comm_getMaxThreads(void) {
  return 0;
}

void chpl_comm_init(int *argc_p, char ***argv_p) {
  chpl_set_num_locales_on_node(1);
  chpl_numNodes = 1;
  chpl_nodeID = 0;
}

void chpl_comm_pre_mem_init(void) { }

void chpl_comm_post_mem_init(void) { }

static const char* chpl_get_debugger_cmd_file(void);
static const char* chpl_get_debugger_cmd_file(void) {
  //
  // WARNING: while it would be nicer for users to just be able to specify
  // arbitrary arguments, this is susceptible to injection. By forcing the
  // user to specify a debugger command file (which we then pass via
  // --command/--source) we can avoid this problem.
  //
  const char* debuggerCmdFile = chpl_env_rt_get("DEBUGGER_CMD_FILE", NULL);
  if (debuggerCmdFile != NULL) {
    if (access(debuggerCmdFile, R_OK) != 0) {
      chpl_warning("CHPL_RT_DEBUGGER_CMD_FILE file not found, it will be ignored", 0,
                   CHPL_FILE_IDX_COMMAND_LINE);
      debuggerCmdFile = NULL;
    }
  }
  return debuggerCmdFile;
}

int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {

  const char* debuggerCmdFile = chpl_get_debugger_cmd_file();
  char* command;
  if (debuggerCmdFile != NULL) {
    command = chpl_glom_strings(4,
      "gdb -q -ex 'break debuggerBreakHere' --command ",
      debuggerCmdFile,
      " --args ",
      argv[0]);
  } else {
    command = chpl_glom_strings(2,
      "gdb -q -ex 'break debuggerBreakHere' --args ",
      argv[0]);
  }

  for (int i=1; i<argc; i++) {
    if (i != gdbArgnum) {
      command = chpl_glom_strings(3, command, " ", argv[i]);
    }
  }
  *status = mysystem(command, "running gdb", 0);

  return 1;
}

int chpl_comm_run_in_lldb(int argc, char* argv[], int lldbArgnum, int* status) {

  const char* debuggerCmdFile = chpl_get_debugger_cmd_file();
  char* command;
  if (debuggerCmdFile != NULL) {
    command = chpl_glom_strings(4,
      "lldb -o 'b debuggerBreakHere' --source ",
      debuggerCmdFile,
      " -- ",
      argv[0]);
  } else {
    command = chpl_glom_strings(2,
      "lldb -o 'b debuggerBreakHere' -- ",
      argv[0]);
  }

  for (int i=1; i<argc; i++) {
    if (i != lldbArgnum) {
      command = chpl_glom_strings(3, command, " ", argv[i]);
    }
  }
  *status = mysystem(command, "running lldb", 0);

  return 1;
}

void chpl_comm_post_task_init(void) { }

void chpl_comm_rollcall(void) {
  chpl_msg(2, "executing on a single node\n");
}

wide_ptr_t* chpl_comm_broadcast_global_vars_helper(void) { return NULL; }

void chpl_comm_broadcast_private(int id, size_t size) { }

void chpl_comm_impl_barrier(const char *msg) { }

void chpl_comm_pre_task_exit(int all) { }

void chpl_comm_exit(int all, int status) { }

void  chpl_comm_put(void* addr, c_nodeid_t node, void* raddr,
                    size_t size, int32_t commID, int ln, int32_t fn) {
  assert(node==0);

  memmove(raddr, addr, size);
}

void  chpl_comm_get(void* addr, c_nodeid_t node, void* raddr,
                    size_t size, int32_t commID, int ln, int32_t fn) {
  assert(node==0);

  memmove(addr, raddr, size);
}

void  chpl_comm_put_strd(void* dstaddr_arg, size_t* dststrides, c_nodeid_t dstnode,
                         void* srcaddr_arg, size_t* srcstrides, size_t* count,
                         int32_t stridelevels, size_t elemSize, int32_t commID,
                         int ln, int32_t fn)
{
  assert(dstnode==0);
  put_strd_common(dstaddr_arg, dststrides, dstnode,
                  srcaddr_arg, srcstrides,
                  count, stridelevels, elemSize,
                  1, NULL, // "nb" xfers block, so no need for yield
                  commID, ln, fn);
}

void  chpl_comm_get_strd(void* dstaddr_arg, size_t* dststrides, c_nodeid_t srcnode,
                         void* srcaddr_arg, size_t* srcstrides, size_t* count,
                         int32_t stridelevels, size_t elemSize, int32_t commID,
                         int ln, int32_t fn)
{
  assert(srcnode==0);
  get_strd_common(dstaddr_arg, dststrides, srcnode,
                  srcaddr_arg, srcstrides,
                  count, stridelevels, elemSize,
                  1, NULL, // "nb" xfers block, so no need for yield
                  commID, ln, fn);
}

void chpl_comm_getput_unordered(c_nodeid_t dstnode, void* dstaddr,
                                c_nodeid_t srcnode, void* srcaddr,
                                size_t size, int32_t commID,
                                int ln, int32_t fn)
{
  assert(srcnode==0);
  assert(dstnode==0);
  memmove(dstaddr, srcaddr, size);
}

void chpl_comm_get_unordered(void* addr, c_nodeid_t node, void* raddr,
                             size_t size, int32_t commID, int ln, int32_t fn)
{
  assert(node == 0);
  memmove(addr, raddr, size);
}

void chpl_comm_put_unordered(void* addr, c_nodeid_t node, void* raddr,
                             size_t size, int32_t commID, int ln, int32_t fn)
{
  assert(node == 0);
  memmove(raddr, addr, size);
}

void chpl_comm_getput_unordered_task_fence(void) { }

typedef struct {
  chpl_fn_int_t fid;
  size_t        arg_size;
  char          arg[0];       // variable-sized data here
} fork_t;

void chpl_comm_execute_on(c_nodeid_t node, c_sublocid_t subloc,
                          chpl_fn_int_t fid,
                          chpl_comm_on_bundle_t *arg, size_t arg_size,
                          int ln, int32_t fn) {
  assert(node==0);

  chpl_ftable_call(fid, arg);
}

void chpl_comm_execute_on_nb(c_nodeid_t node, c_sublocid_t subloc,
                             chpl_fn_int_t fid,
                             chpl_comm_on_bundle_t *arg, size_t arg_size,
                             int ln, int32_t fn) {
  assert(node==0);

  chpl_task_startMovedTask(fid, chpl_ftable[fid],
                           chpl_comm_on_bundle_task_bundle(arg), arg_size,
                           subloc, chpl_nullTaskID);
}

// Same as chpl_comm_execute_on()
void chpl_comm_execute_on_fast(c_nodeid_t node, c_sublocid_t subloc,
                               chpl_fn_int_t fid,
                               chpl_comm_on_bundle_t *arg, size_t arg_size,
                               int ln, int32_t fn) {
  assert(node==0);

  chpl_ftable_call(fid, arg);
}

void chpl_comm_ensure_progress(void) { }
