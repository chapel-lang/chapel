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
#include "chpl-exec.h"
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

static chpl_bool chpl_get_command_output(const char* command,
                                         char* output, size_t output_size);
static chpl_bool chpl_get_command_output(const char* command,
                                         char* output, size_t output_size) {
  FILE* pipe = popen(command, "r");
  if (!pipe) {
    return false;
  }
  size_t bytes_read = fread(output, sizeof(char), output_size - 1, pipe);
  output[bytes_read] = '\0';
  int status = pclose(pipe);
  return (status == 0);
}

static chpl_bool chpl_lldb_supports_python(void);
static chpl_bool chpl_lldb_supports_python(void) {
  char output[256];
  const char* command = "lldb --python-path";
  if (!chpl_get_command_output(command, output, sizeof(output))) {
    return false;
  }

  if (output[0] == '\0') {
    return false;
  }
  // remove trailing newline if present
  size_t len = strlen(output);
  if (len > 0 && output[len - 1] == '\n') {
    output[len - 1] = '\0';
  }
  return access(output, R_OK) == 0;
}

int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {

  char* command = (char*)"gdb -q";

  const char* gdb_commands = chpl_glom_strings(2, CHPL_HOME, "/runtime/etc/debug/gdb.commands");
  if (access(gdb_commands, R_OK) == 0) {
    command = chpl_glom_strings(4, command, " -x \"", gdb_commands, "\"");
  } else {
    chpl_warning(
      "Could not find 'gdb.commands' file, falling back to basic settings",
      0, CHPL_FILE_IDX_COMMAND_LINE);
    command = chpl_glom_strings(2, command, " -ex 'break debuggerBreakHere'");
  }

  const char* debuggerCmdFile = chpl_get_debugger_cmd_file();
  if (debuggerCmdFile != NULL) {
    command = chpl_glom_strings(4, command, " --command \"", debuggerCmdFile, "\"");
  }

  command = chpl_glom_strings(3, command, " --args ", argv[0]);

  for (int i=1; i<argc; i++) {
    if (i != gdbArgnum) {
      command = chpl_glom_strings(3, command, " ", argv[i]);
    }
  }
  *status = chpl_invoke_using_system(command, "running gdb", 0);

  return 1;
}

int chpl_comm_run_in_lldb(int argc, char* argv[], int lldbArgnum, int* status) {

  char* command = (char*)"lldb";

  const char* lldb_commands = chpl_glom_strings(2, CHPL_HOME, "/runtime/etc/debug/lldb.commands");
  if (access(lldb_commands, R_OK) == 0) {
    command = chpl_glom_strings(4, command, " --source \"", lldb_commands, "\"");
  } else {
    chpl_warning(
      "Could not find 'lldb.commands' file, falling back to basic settings",
      0, CHPL_FILE_IDX_COMMAND_LINE);
    command = chpl_glom_strings(2, command, " -o 'b debuggerBreakHere'");
  }


  if (chpl_lldb_supports_python()) {
    const char* pretty_printer = chpl_glom_strings(2, CHPL_HOME, "/runtime/etc/debug/chpl_lldb_pretty_print.py");
    if (access(pretty_printer, R_OK) == 0) {
      command = chpl_glom_strings(4, command,
        " -o 'command script import \"", pretty_printer, "\"'");
    } else {
      chpl_warning("Could not find lldb pretty-printer, it will be ignored",
                    0, CHPL_FILE_IDX_COMMAND_LINE);
    }
  } else {
    chpl_warning(
      "LLDB does not support scripting with Python"
      ", pretty-printer will not be used",
      0, CHPL_FILE_IDX_COMMAND_LINE
    );
  }

  const char* debuggerCmdFile = chpl_get_debugger_cmd_file();
  if (debuggerCmdFile != NULL) {
    command = chpl_glom_strings(4, command, " --source \"", debuggerCmdFile, "\"");
  }

  command = chpl_glom_strings(3, command, " -- ", argv[0]);


  for (int i=1; i<argc; i++) {
    if (i != lldbArgnum) {
      command = chpl_glom_strings(3, command, " ", argv[i]);
    }
  }
  *status = chpl_invoke_using_system(command, "running lldb", 0);

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
