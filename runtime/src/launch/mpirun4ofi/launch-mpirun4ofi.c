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

#include <stdio.h>
#include <string.h>
#include "arg.h"
#include "chplcgfns.h"
#include "chpl-env.h"
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "error.h"

static char** chpl_launch_create_argv(const char *launch_cmd,
                                      int argc, char* argv[],
                                      int32_t numLocales) {
  if (strcmp(CHPL_COMM, "ofi") != 0) {
    chpl_error("mpirun4ofi only supports CHPL_COMM=ofi", 0, 0);
  }

  static char _nlbuf[16];
  snprintf(_nlbuf, sizeof(_nlbuf), "%d", getArgNumLocales());

  char** largv = NULL;
  int largv_size = 0;
  int largc = 0;
#define ADD_LARGV(s)                                                    \
  do {                                                                  \
    if (largc >= largv_size) {                                          \
      largv_size += 10;                                                 \
      largv = chpl_mem_realloc(largv, largv_size * sizeof(*largv),      \
                               CHPL_RT_MD_COMMAND_BUFFER, -1, 0);       \
    }                                                                   \
    largv[largc++] = (char*) (s);                                       \
  } while (0)

  ADD_LARGV(launch_cmd);
  ADD_LARGV("-np");
  ADD_LARGV(_nlbuf);
  ADD_LARGV("-map-by");
  ADD_LARGV("ppr:1:node");
  ADD_LARGV("-map-by");
  ADD_LARGV("node:oversubscribe");
  ADD_LARGV("-bind-to");
  ADD_LARGV("none");
  if (chpl_env_rt_get_bool("OVERSUBSCRIBED", false)) {
    ADD_LARGV("-mca");
    ADD_LARGV("mpi_yield_when_idle");
    ADD_LARGV("1");
  }

  return chpl_bundle_exec_args(argc, argv, largc, largv);
}


int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  const char *cmd = "mpirun";
  return chpl_launch_using_exec(cmd,
                                chpl_launch_create_argv(cmd, argc, argv,
                                                        numLocales),
                                argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, int32_t filename) {
  return 0;
}


void chpl_launch_print_help(void) {
}
