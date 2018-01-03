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

#include <stdio.h>
#include <string.h>
#include "chpllaunch.h"
#include "error.h"


// Simple launcher that just sets GASNET_PSHM_NODES and launchers the _real

int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  char numlocalesval[12]; // big enough for int32_t
  char baseCommand[4096];

  snprintf(numlocalesval, sizeof(numlocalesval), "%d", (int)numLocales);
  if (setenv("GASNET_PSHM_NODES", numlocalesval, 1) != 0) {
    chpl_error("Cannot setenv(\"GASNET_PSHM_NODES\")", 0, 0);
  }

  chpl_compute_real_binary_name(argv[0]);
  snprintf(baseCommand, sizeof(baseCommand), "%s", chpl_get_real_binary_name());

  return chpl_launch_using_exec(baseCommand,
                                chpl_bundle_exec_args(argc, argv, 0, NULL),
                                NULL);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, int32_t filename) {
  return 0;
}


void chpl_launch_print_help(void) {
}
