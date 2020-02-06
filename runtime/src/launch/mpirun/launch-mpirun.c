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

#include <stdio.h>
#include <string.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "chplcgfns.h"
#include "error.h"

#define CHPL_SPMD "--spmd"

static char* mpi_num_ranks = NULL;

static char _nlbuf[16];
static char** chpl_launch_create_argv(const char *launch_cmd,
                                      int argc, char* argv[],
                                      int32_t numLocales) {
  const int largc = 3;
  char *largv[largc];

  int numranks;

  // TODO When MPI Comm layer is supported, support -nl>1 && CHPL_COMM!=none
  if (numLocales != 1) {
    chpl_error("For mpirun, specify number of SPMD images via --spmd <#> instead of -nl", 0, 0);
  }
  if (strcmp(CHPL_COMM, "none") != 0) {
    chpl_error("mpirun only supports CHPL_COMM=none", 0, 0);
  }
  // Get the number of ranks
  if (mpi_num_ranks == NULL) {
    numranks = 1;
  } else {
    numranks = atoi(mpi_num_ranks);
  }


  largv[0] = (char *) launch_cmd;
  largv[1] = (char *) "-np";
  sprintf(_nlbuf, "%d", numranks);
  largv[2] = _nlbuf;

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
  // handle --spmd <nrank> or --spmd=<nrank>
  if (!strcmp(argv[argNum], CHPL_SPMD)) {
    mpi_num_ranks = argv[argNum+1];
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_SPMD"=", strlen(CHPL_SPMD))) {
    mpi_num_ranks = &(argv[argNum][strlen(CHPL_SPMD)+1]);
    return 1;
  }

  return 0;
}


void chpl_launch_print_help(void) {
  fprintf(stdout, "LAUNCHER FLAGS:\n");
  fprintf(stdout, "===============\n");
  fprintf(stdout, "  %s : specify number of SPMD images \n", CHPL_SPMD);
}
